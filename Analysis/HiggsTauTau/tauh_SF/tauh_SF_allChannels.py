import ROOT
from ROOT import TGraphAsymmErrors
import UserCode.ICHiggsTauTau.plotting as plotting
import math
from matplotlib import pyplot as plt
from sklearn.gaussian_process.kernels import Matern, ConstantKernel
from sklearn.gaussian_process import GaussianProcessRegressor
import copy
import scipy
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
from scipy import interpolate
from tauh_SF_funcs import Histogram, CleanHist, MinTarget, Graph, FitResults 
ROOT.gROOT.SetBatch(True)
plotting.ModTDRStyle()
ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)
ROOT.Math.MinimizerOptions.SetDefaultPrintLevel(1)
ROOT.gStyle.SetOptFit(0)

def main():
    output_file = ROOT.TFile("outputs/trigger_SF_tauh.root", 'RECREATE', '')
    DM_types = ['mva', 'HPS'] 
    years = [2016, 2017, 2018]
    predSamples = ["MC", "embed"]
    for DM_type in DM_types:
        for year in years:
            for predSample in predSamples:
                if DM_type=='mva':
                    DM_nums=[0, 1, 2, 10, 11]
                if DM_type=='HPS':
                    DM_nums=[0, 1, 10, 11]
                if(year!=2016):
                    channels = ["tt", "mt","et"]
                else:
                    channels = ["tt", "mt"]
                ch_validity_thrs = { 'et': 35, 'mt': 32, 'tt': 40 }
                
                for channel in channels:
                    with PdfPages("outputs/SF_plots_{}Channel_{}_{}DM_{}.pdf".format(channel, year ,DM_type, predSample)) as pdf:
                        for num in DM_nums:    
                            print "*************"
                            print channel, predSample, year, DM_type, num, year
                            print "*************"
                            datacard_pass = ROOT.TFile.Open("{}/{}/{}/datacard_pt_2_pass_{}DM{}_mt_{}.root".format(channel, predSample, year, DM_type, num, year), "READ")
                            datacard_total = ROOT.TFile.Open("{}/{}/{}/datacard_pt_2_total_{}DM{}_mt_{}.root".format(channel, predSample, year, DM_type, num, year), "READ")
                            
                            data_obs_pass = datacard_pass.Get("mt_pass_{}DM{}/data_obs".format(DM_type, num))
                            data_obs_total = datacard_total.Get("mt_total_{}DM{}/data_obs".format(DM_type, num))
                            
                            ZTT_pass = datacard_pass.Get("mt_pass_{}DM{}/ZTT".format(DM_type, num))
                            ZTT_total = datacard_total.Get("mt_total_{}DM{}/ZTT".format(DM_type, num))
                            
                            
                            hist_bkgs_pass=[]
                            for hist_name in ["ZL","ZJ","TTJ", "VVJ","W","QCD"]:
                                hist_bkgs_pass.append(datacard_pass.Get("mt_pass_{}DM{}/{}".format(DM_type, num, hist_name)))
                            
                            hist_bkgs_total=[]
                            for hist_name in ["ZL","ZJ","TTJ", "VVJ","W","QCD"]:
                                hist_bkgs_total.append(datacard_total.Get("mt_total_{}DM{}/{}".format(DM_type, num, hist_name)))
                           
                            for hist in hist_bkgs_pass:
                                data_obs_pass.Add(hist,-1.0)
                            
                            for hist in hist_bkgs_total:
                                data_obs_total.Add(hist,-1.0)
                            
                            data_obs_pass, data_obs_total = CleanHist(data_obs_pass, data_obs_total)
                            ZTT_pass, ZTT_total = CleanHist(ZTT_pass, ZTT_total)
                            
                            eff_data_input = TGraphAsymmErrors()
                            eff_data_input.Divide(data_obs_pass, data_obs_total)
                            eff_MC_input = TGraphAsymmErrors()
                            eff_MC_input.Divide(ZTT_pass, ZTT_total)
                                
                            for i in range(0,eff_data_input.GetN())[::-1]:
                                xx=ROOT.Double()
                                yy=ROOT.Double()
                                eff_data_input.GetPoint(i,xx,yy)
                                if yy ==0: eff_data_input.SetPoint(i, xx, 0.0001) 
                            for i in range(0,eff_MC_input.GetN())[::-1]:
                                xx=ROOT.Double()
                                yy=ROOT.Double()
                                eff_MC_input.GetPoint(i,xx,yy)
                                if yy ==0: eff_MC_input.SetPoint(i, xx, 0.0001) 


                            eff_data_graph = Graph(root_graph=eff_data_input)        
                            eff_MC_graph = Graph(root_graph=eff_MC_input)        
                            
                            pred_step = 0.1
                            x_low, x_high = 20, 400
                            x_pred = np.arange(x_low, x_high + pred_step/2, pred_step)
                            eff_data_fitted = FitResults(eff_data_graph, x_pred)
                            eff_MC_fitted = FitResults(eff_MC_graph, x_pred)

                            sf = eff_data_fitted.y_pred / eff_MC_fitted.y_pred 
                            sf_sigma = np.sqrt( (eff_data_fitted.sigma_pred / eff_MC_fitted.y_pred) ** 2 \
                                         + (eff_data_fitted.y_pred / (eff_MC_fitted.y_pred ** 2) * eff_MC_fitted.sigma_pred ) ** 2 )        

                            fig, (ax, ax_ratio) = plt.subplots(2, 1, figsize=(7, 7), sharex=True, gridspec_kw = {'height_ratios':[2, 1]})
                            
                            MC_color = 'g'
                            data_color = 'k'
                            trans = 0.3
                            
                            plt_data = ax.errorbar(eff_data_graph.x, eff_data_graph.y, xerr=(eff_data_graph.x_error_low, eff_data_graph.x_error_high),
                                        yerr=(eff_data_graph.y_error_low, eff_data_graph.y_error_high), fmt=data_color+'.', markersize=5)        
                            plt_MC = ax.errorbar(eff_MC_graph.x, eff_MC_graph.y, xerr=(eff_MC_graph.x_error_low, eff_MC_graph.x_error_high),
                                        yerr=(eff_MC_graph.y_error_low, eff_MC_graph.y_error_high), fmt=MC_color+'.', markersize=5)        

                            plt_data_fitted = ax.plot(x_pred, eff_data_fitted.y_pred, data_color+'--')
                            ax.fill(np.concatenate([x_pred, x_pred[::-1]]), np.concatenate([eff_data_fitted.y_pred - eff_data_fitted.sigma_pred,
                                        (eff_data_fitted.y_pred + eff_data_fitted.sigma_pred)[::-1]]), alpha=trans, fc=data_color, ec='None')
                            
                            plt_MC_fitted = ax.plot(x_pred, eff_MC_fitted.y_pred, MC_color+'--')
                            ax.fill(np.concatenate([x_pred, x_pred[::-1]]), np.concatenate([eff_MC_fitted.y_pred - eff_MC_fitted.sigma_pred,
                                       (eff_MC_fitted.y_pred + eff_MC_fitted.sigma_pred)[::-1]]),alpha=trans, fc=MC_color, ec='None')
                            
                            ax_ratio.plot(x_pred, sf, 'b--')
                            ax_ratio.fill(np.concatenate([x_pred, x_pred[::-1]]), np.concatenate([sf - sf_sigma, (sf + sf_sigma)[::-1]]),alpha=trans, fc='b', ec='None')
                            
                            title = "Turn-ons: {} Channel - {}DM{} - {} - {} Samples".format(channel ,DM_type.upper(), num, year, predSample)
                            ax.set_title(title, fontsize=16)
                            ax.set_ylabel("Efficiency", fontsize=12)
                            ax.set_ylim([ 0., 1.1 ])
                            ax.set_xlim([ 20, min(400, plt.xlim()[1]) ])

                            ax_ratio.set_xlabel("$p_T$ (GeV)", fontsize=12)
                            ax_ratio.set_ylabel("Data/MC SF", fontsize=12)
                            ax_ratio.set_ylim([0.5, 1.49])

                            validity_plt = ax.plot( [ ch_validity_thrs[channel] ] * 2, ax.get_ylim(), 'r--' )
                            ax_ratio.plot( [ ch_validity_thrs[channel] ] * 2, ax_ratio.get_ylim(), 'r--' )

                            ax.legend([ plt_data, plt_MC, plt_data_fitted[0], plt_MC_fitted[0], validity_plt[0] ],
                                                      [ "Data", "MC", "Data fitted", "MC fitted", "Validity range"], fontsize=12, loc='lower right')

                            plt.subplots_adjust(hspace=0)
                            pdf.savefig(bbox_inches='tight')
                            plt.close()
                            
                            out_name_pattern = '{}Channel_{}_PredUsing{}Samples_{}DM_{}_{{}}'.format(channel, year, predSample, DM_type, num )
                            output_file.WriteTObject(eff_data_input, out_name_pattern.format('EffOfData'), 'Overwrite')
                            output_file.WriteTObject(eff_MC_input, out_name_pattern.format('EffOfMC'), 'Overwrite')
                            eff_data_fitted_hist = Histogram.CreateTH1(eff_data_fitted.y_pred, [x_low, x_high],
                                                                       eff_data_fitted.sigma_pred, fixed_step=True)
                            eff_MC_fitted_hist = Histogram.CreateTH1(eff_MC_fitted.y_pred, [x_low, x_high],
                                                                     eff_MC_fitted.sigma_pred, fixed_step=True)
                            SF_fitted_hist = eff_data_fitted_hist.Clone()
                            SF_fitted_hist.Divide(eff_MC_fitted_hist)
                            output_file.WriteTObject(eff_data_fitted_hist, out_name_pattern.format('EffOfData_Fitted'), 'Overwrite')
                            output_file.WriteTObject(eff_MC_fitted_hist, out_name_pattern.format('EffOfMC_Fitted'), 'Overwrite')
                            output_file.WriteTObject(SF_fitted_hist, out_name_pattern.format('SF_Fitted'), 'Overwrite')

    output_file.Close()




if __name__=="__main__":
    main()
