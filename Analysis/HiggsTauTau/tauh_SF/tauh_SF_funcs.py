import ROOT
from ROOT import TGraphAsymmErrors
import UserCode.ICHiggsTauTau.plotting as plotting
import argparse
import math
from matplotlib import pyplot as plt
from sklearn.gaussian_process.kernels import Matern, ConstantKernel
from sklearn.gaussian_process import GaussianProcessRegressor
import copy
import scipy
import numpy as np
from scipy import interpolate

def CleanHist(hist_pass, hist_total):
    hist_pass.ClearUnderflowAndOverflow()
    hist_total.ClearUnderflowAndOverflow()

    for i in range(1, hist_total.GetNbinsX()+1 ):
        hist_pass.SetBinContent(i, round(hist_pass.GetBinContent(i),4))
        hist_total.SetBinContent(i, round(hist_total.GetBinContent(i),4))
        
        if  hist_total.GetBinContent(i)<0:
            print "****"
            print (hist_total.GetName(), i, hist_pass.GetBinContent(i), hist_total.GetBinContent(i))
            hist_total.SetBinContent(i,0)
            print (hist_total.GetName(), i, hist_pass.GetBinContent(i), hist_total.GetBinContent(i))
            print "****"
        if  hist_pass.GetBinContent(i)<0:
            print "****"
            print (hist_total.GetName(), i, hist_pass.GetBinContent(i), hist_total.GetBinContent(i))
            hist_pass.SetBinContent(i,0)
            print (hist_total.GetName(), i, hist_pass.GetBinContent(i), hist_total.GetBinContent(i))
            print "****"
        
        if  hist_pass.GetBinContent(i)>hist_total.GetBinContent(i):
            print "****"
            print (hist_total.GetName(), i, hist_pass.GetBinContent(i), hist_total.GetBinContent(i))
            hist_pass.SetBinContent(i, hist_total.GetBinContent(i))
            print (hist_total.GetName(), i, hist_pass.GetBinContent(i), hist_total.GetBinContent(i))
            print "****"
    return hist_pass, hist_total


def MinTarget(dy, eff):
    y = np.cumsum(dy)
    return np.sum(((eff.y - y) / (eff.y_error_high + eff.y_error_low)) ** 2)


class Histogram:
    def __init__(self, th1_hist):
        n_bins = th1_hist.GetNbinsX()
        self.values = np.zeros(n_bins)
        self.errors = np.zeros(n_bins)
        self.edges = np.zeros(n_bins + 1)
        for n in range(n_bins):
            self.values[n] = th1_hist.GetBinContent(n + 1)
            self.edges[n] = th1_hist.GetBinLowEdge(n + 1)
            self.errors[n] = th1_hist.GetBinError(n + 1)
        self.edges[n_bins] = th1_hist.GetBinLowEdge(n_bins + 1)

    @staticmethod
    def CreateTH1(values, edges, errors, fixed_step=False):
        if fixed_step:
            th1_hist = ROOT.TH1F('', '', len(values), edges[0], edges[-1])
        else:
            th1_hist = ROOT.TH1F('', '', len(edges) - 1, array('f', edges))
        for n in range(len(values)):
            th1_hist.SetBinContent(n + 1, values[n])
            th1_hist.SetBinError(n + 1, errors[n])
        return th1_hist



class Graph:
    def __init__(self, **kwargs):
        if 'root_graph' in kwargs:
            graph = kwargs['root_graph']
            n_points = graph.GetN()
        elif 'n_points' in kwargs:
            graph = None
            n_points = kwargs['n_points']
        else:
            raise RuntimeError("Invalid arguments for Graph init")

        self.x = np.zeros(n_points)
        self.x_error_low = np.zeros(n_points)
        self.x_error_high = np.zeros(n_points)
        self.y = np.zeros(n_points)
        self.y_error_low = np.zeros(n_points)
        self.y_error_high = np.zeros(n_points)

        if graph is not None:
            for n in range(n_points):
                self.x[n] = graph.GetX()[n]
                self.x_error_low[n] = graph.GetErrorXlow(n)
                self.x_error_high[n] = graph.GetErrorXhigh(n)
                self.y[n] = graph.GetY()[n]
                self.y_error_low[n] = graph.GetErrorYlow(n)
                self.y_error_high[n] = graph.GetErrorYhigh(n)

    def ToRootGraph(self, n_active_points=None):
        n_points = n_active_points if n_active_points is not None else len(self.x)
        return ROOT.TGraphAsymmErrors(n_points, array('d', self.x), array('d', self.y),
                                      array('d', self.x_error_low), array('d', self.x_error_high),
                                      array('d', self.y_error_low), array('d', self.y_error_high))

class FitResults:
    def __init__(self, eff, x_pred):
        kernel_high = ConstantKernel()
        kernel_low = ConstantKernel() * Matern(nu=1, length_scale_bounds=(10, 100), length_scale=20)
        N = eff.x.shape[0]
        res = scipy.optimize.minimize(MinTarget, np.zeros(N), args=(eff,), bounds = [ [0, 1] ] * N, options={"maxfun": int(1e6)})
        if not res.success:
            print(res)
            raise RuntimeError("Unable to prefit")
        eff = copy.deepcopy(eff)
        new_y = np.cumsum(res.x)
        delta = eff.y - new_y
        eff.y_error_low = np.sqrt(eff.y_error_low ** 2 + delta ** 2)
        eff.y_error_high = np.sqrt(eff.y_error_high ** 2 + delta ** 2)
        eff.y = new_y
        yerr = np.maximum(eff.y_error_low, eff.y_error_high)
        
        self.pt_start_flat = eff.x[-1]
        best_chi2_ndof = float('inf')
        for n in range(1, N):
            flat_eff, residuals, _, _, _ = np.polyfit(eff.x[N-n-1:], eff.y[N-n-1:], 0, w=1/yerr[N-n-1:], full=True)
            chi2_ndof = residuals[0] / n
            #print(n, chi2_ndof)
            if (chi2_ndof > 0 and chi2_ndof < best_chi2_ndof) or eff.x[N-n-1] + eff.x_error_high[N-n-1] >= 100:
                self.pt_start_flat = eff.x[N-n-1]
                best_chi2_ndof = chi2_ndof
        if best_chi2_ndof > 20:
            print("Unable to determine the high pt region")
            self.pt_start_flat = eff.x[-1]

        low_pt = eff.x <= self.pt_start_flat
        high_pt = eff.x >= self.pt_start_flat

        self.gp_high = GaussianProcessRegressor(kernel=kernel_high, alpha=yerr[high_pt] ** 2, n_restarts_optimizer=10)
        self.gp_high.fit(np.atleast_2d(eff.x[high_pt]).T, eff.y[high_pt])
        self.gp_low = GaussianProcessRegressor(kernel=kernel_low, alpha=np.append([0], yerr[low_pt] ** 2),
                                               n_restarts_optimizer=10)
        self.gp_low.fit(np.atleast_2d(np.append([10], eff.x[low_pt])).T, np.append([0], eff.y[low_pt]))

        self.y_pred, sigma_pred = self.Predict(x_pred)

        sigma_orig = np.zeros(N)
        for n in range(N):
            idx = np.argmin(abs(x_pred - eff.x[n]))
            sigma_orig[n] = sigma_pred[idx]

        interp_kind = 'linear'
        sp = interpolate.interp1d(eff.x, sigma_orig, kind=interp_kind, fill_value="extrapolate")
        sigma_interp = sp(x_pred)
        max_unc = 0.05 / math.sqrt(2)
        sigma_pred, = self.ApplyStep(x_pred, [ [ sigma_pred, sigma_interp ] ], eff.x[0], eff.x[-1] )
        outer_trend = np.minimum(np.ones(x_pred.shape[0]), (x_pred - eff.x[-1]) / eff.x[-1])
        outer_sigma = np.maximum(sigma_pred, sigma_pred + (max_unc - sigma_pred) * outer_trend )
        self.sigma_pred = np.where(x_pred < eff.x[-1], sigma_pred, outer_sigma )

    def Predict(self, x_pred):
        y_pred_high, sigma_high = self.gp_high.predict(np.atleast_2d(x_pred).T, return_std=True)
        y_pred_low, sigma_low = self.gp_low.predict(np.atleast_2d(x_pred).T, return_std=True)
        return self.ApplyStep(x_pred, [ [y_pred_low, y_pred_high], [sigma_low, sigma_high] ], self.pt_start_flat)

    def ApplyStep(self, x_pred, functions, x0, x1 = None):
        step = (np.tanh(0.1*(x_pred - x0)) + 1) / 2
        if x1 is not None:
            step *= (np.tanh(0.1*(x1 - x_pred)) + 1) / 2
        step = np.where(step > 0.999, 1, step)
        step = np.where(step < 0.001, 0, step)
        results = []
        for fn in functions:
            results.append(fn[0] * (1-step) + fn[1] * step)
        return tuple(results)
