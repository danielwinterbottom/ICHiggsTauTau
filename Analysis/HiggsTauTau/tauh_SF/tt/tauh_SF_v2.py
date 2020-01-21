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
from tauh_SF_funcs import CleanHist, MinTarget, Graph, FitResults 
ROOT.gROOT.SetBatch(True)
plotting.ModTDRStyle()
ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)
ROOT.Math.MinimizerOptions.SetDefaultPrintLevel(1)
ROOT.gStyle.SetOptFit(0)

def parse_arguments():
    parser =argparse.ArgumentParser()
    parser.add_argument("--year", 
            help="Run '2016', '2017' or '2018'")
    parser.add_argument("--pred_using", 
            help="data is predicted using 'MC' or 'embed'")
    parser.add_argument("--DM_type",
            help="decay mode type to be 'mva' or 'HPS'")
    return parser.parse_args()


def main(args):
    if args.DM_type == "mva":
        DM_nums=[0, 1,2, 10, 11]
        DM_nums=[11]
    else:
        DM_nums=[0, 1, 10, 11]
        DM_nums=[11]

    for num in DM_nums:    

        datacard_pass = ROOT.TFile.Open("{}/{}/datacard_pt_2_pass_{}DM{}_mt_{}.root".format(args.pred_using, args.year, args.DM_type, num, args.year), "READ")
        datacard_total = ROOT.TFile.Open("{}/{}/datacard_pt_2_total_{}DM{}_mt_{}.root".format(args.pred_using, args.year, args.DM_type, num, args.year), "READ")
        
        data_obs_pass = datacard_pass.Get("mt_pass_{}DM{}/data_obs".format(args.DM_type, num))
        data_obs_total = datacard_total.Get("mt_total_{}DM{}/data_obs".format(args.DM_type, num))
        
        ZTT_pass = datacard_pass.Get("mt_pass_{}DM{}/ZTT".format(args.DM_type, num))
        ZTT_total = datacard_total.Get("mt_total_{}DM{}/ZTT".format(args.DM_type, num))
        
        
        hist_bkgs_pass=[]
        for hist_name in ["ZL","ZJ","TTJ", "VVJ","W","QCD"]:
            hist_bkgs_pass.append(datacard_pass.Get("mt_pass_{}DM{}/{}".format(args.DM_type, num, hist_name)))
        
        hist_bkgs_total=[]
        for hist_name in ["ZL","ZJ","TTJ", "VVJ","W","QCD"]:
            hist_bkgs_total.append(datacard_total.Get("mt_total_{}DM{}/{}".format(args.DM_type, num, hist_name)))
       
        for hist in hist_bkgs_pass:
            data_obs_pass.Add(hist,-1.0)
        
        for hist in hist_bkgs_total:
            data_obs_total.Add(hist,-1.0)
        
        data_obs_pass, data_obs_total = CleanHist(data_obs_pass, data_obs_total)
        ZTT_pass, ZTT_total = CleanHist(ZTT_pass, ZTT_total)
        
       
        eff_data = TGraphAsymmErrors()
        eff_data.Divide(data_obs_pass, data_obs_total)
        eff_data_graph = Graph(root_graph=eff_data)        
        pred_step = 0.1
        x_low, x_high = 20, 400
        x_pred = np.arange(x_low, x_high + pred_step/2, pred_step)
        eff_data_fitted = FitResults(eff_data_graph, x_pred)
        
        fig, (ax, ax_ratio) = plt.subplots(2, 1, figsize=(7, 7), sharex=True,
                                  gridspec_kw = {'height_ratios':[2, 1]})
        data_color = 'k'
        trans = 0.3
        plt_data = ax.errorbar(eff_data_graph.x, eff_data_graph.y, xerr=(eff_data_graph.x_error_low, eff_data_graph.x_error_high),
                    yerr=(eff_data_graph.y_error_low, eff_data_graph.y_error_high), fmt=data_color+'.', markersize=5)        
        plt_data_fitted = ax.plot(x_pred, eff_data_fitted.y_pred, data_color+'--')

        plt_data_fitted = ax.plot(x_pred, eff_data_fitted.y_pred, data_color+'--')
        ax.fill(np.concatenate([x_pred, x_pred[::-1]]),
                np.concatenate([eff_data_fitted.y_pred - eff_data_fitted.sigma_pred,
                               (eff_data_fitted.y_pred + eff_data_fitted.sigma_pred)[::-1]]),
                alpha=trans, fc=data_color, ec='None')
        plt.savefig("test.pdf")

        #c1 = ROOT.TCanvas()
        #eff_data.Draw()
        #c1.SaveAs("{}/{}/eff_data_{}DM{}.pdf".format(args.pred_using, args.year, args.DM_type, num))
        
        eff_MC = TGraphAsymmErrors()
        eff_MC.Divide(ZTT_pass, ZTT_total)
        #c2 = ROOT.TCanvas()
        #eff_MC.Draw()
        #c2.SaveAs("{}/{}/eff_MC_{}DM{}.pdf".format(args.pred_using, args.year, args.DM_type, num))

if __name__=="__main__":
    args=parse_arguments()
    main(args)
