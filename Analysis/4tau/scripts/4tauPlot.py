import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser
import argparse
import ConfigParser
import UserCode.ICHiggsTauTau.plotting as plotting
from collections import OrderedDict
import copy

CHANNELS= ['tttt','ettt','mttt','tttt','eett','mmtt',"ttt"]

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = { 
    "channel":"mt" , "outputfolder":"output",
    "folder":"/vols/cms/dw515/Offline/output/MSSM/Jan11/" , "signal_folder":"", "embed_folder":"",
    "paramfile":"scripts/Params_2016_spring16.json", "cat":"inclusive", "year":"2016",
    "campaign":"UL", "sel":"(1)", "set_alias":[], "analysis":"mssm", "var":"m_vis(7,0,140)",
    "method":1 , "do_ss":False, "sm_masses":"125", "ggh_masses":"", "bbh_masses":"",
    "bbh_nlo_masses":"", "nlo_qsh":False, "qcd_os_ss_ratio":-1, "add_sm_background":"",
    "syst_e_scale":"", "syst_mu_scale":"", "syst_tau_scale":"", "syst_tau_scale_0pi":"",
    "syst_tau_scale_1pi":"", "syst_tau_scale_3prong":"", "syst_tau_scale_3prong1pi0":"", "syst_eff_t":"", "syst_tquark":"",
    "syst_zwt":"", "syst_w_fake_rate":"", "syst_scale_j":"", "syst_res_j":"", "syst_scale_j_rbal":"",
    "syst_scale_j_rsamp":"", "syst_scale_j_full":"", "syst_scale_j_cent":"", "syst_scale_j_hf":"", 
    "syst_scale_j_full_corr":"", "syst_scale_j_cent_corr":"", "syst_scale_j_hf_corr":"",
    "syst_scale_j_full_uncorr":"", "syst_scale_j_cent_uncorr":"", "syst_scale_j_hf_uncorr":"",
    "syst_scale_j_by_source":"","jes_sources":"1:27", "syst_eff_b":"", "syst_fake_b":"", "syst_eff_b_weights":"",
    "norm_bins":False, "blind":False, "x_blind_min":100, "x_blind_max":4000, "ratio":False,
    "y_title":"", "x_title":"", "custom_y_range":False, "y_axis_min":0.001,
    "y_axis_max":100,"custom_x_range":False, "x_axis_min":0.001, "x_axis_max":100, "log_x":False,
    "log_y":False, "extra_pad":0.0, "signal_scale":1, "draw_signal_mass":"", "draw_signal_tanb":10,
    "signal_scheme":"run2_mssm", "lumi":"12.9 fb^{-1} (13 TeV)", "no_plot":False,
    "ratio_range":"0.7,1.3", "datacard":"", "do_custom_uncerts":False, "uncert_title":"Systematic uncertainty", 
    "custom_uncerts_wt_up":"","custom_uncerts_wt_down":"", "add_flat_uncert":0,
    "add_stat_to_syst":False, "add_wt":"", "custom_uncerts_up_name":"", "custom_uncerts_down_name":"",
    "do_ff_systs":False, "syst_efake_0pi_scale":"", "syst_efake_1pi_scale":"",
    "syst_mufake_0pi_scale":"", "syst_mufake_1pi_scale":"", "scheme":"","scheme":"", "syst_zpt_es":"",
    "syst_zpt_tt":"", "syst_zpt_statpt0":"", "syst_zpt_statpt40":"", "syst_zpt_statpt80":"",
    "syst_jfake_m":"", "syst_jfake_e":"", "syst_z_mjj":"", "syst_qcd_scale":"", "syst_quarkmass":"",
    "syst_ps":"", "syst_ue":"", "doNLOScales":False, "gen_signal":False, "doPDF":False,
    "doMSSMReWeighting":False, "do_unrolling":True, "syst_tau_id_dm0":"", "syst_tau_id_dm1":"",
    "syst_tau_id_dm10":"", "syst_lfake_dm0":"","syst_lfake_dm1":"","syst_qcd_shape_wsf":"",
    "syst_scale_met_unclustered":"","syst_scale_met_clustered":"",
    "extra_name":"", "no_default":False, "no_systs":False, "embedding":False,"syst_embedding_tt":"",
    "vbf_background":False, "syst_em_qcd":"", "syst_prefire":"",
    "syst_scale_met":"", "syst_res_met":"", "split_sm_scheme": False,
    "ggh_scheme": "powheg", "symmetrise":False, "mergeXbins":False, 'em_qcd_weight':"",
    "syst_scale_j_corr":"","syst_scale_j_uncorr":"", "syst_qcd_bkg":"",
    "ff_ss_closure":False, "threePads":False,"auto_blind":False,
    "syst_tau_id_diff":"", "syst_tau_trg_diff":"",
    "syst_scale_j_regrouped":"", "syst_tau_scale_grouped":""

}

if options.cfg:
    config = ConfigParser.SafeConfigParser()
    config.read([options.cfg])
    defaults.update(dict(config.items("Defaults")))

parser = argparse.ArgumentParser(
    parents=[conf_parser]
    )
parser.set_defaults(**defaults)
parser.add_argument("--channel", dest="channel", type=str,
    help="Tau decay channel to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_argument("--outputfolder", dest="outputfolder", type=str,
    help="Name of output folder")
parser.add_argument("--folder", dest="folder", type=str,
    help="Name of input folder")
parser.add_argument("--signal_folder", dest="signal_folder", type=str,
    help="If specified will use as input folder for signal samples, else will use same directroy specified by \"folder\" option.")
parser.add_argument("--embed_folder", dest="embed_folder", type=str,
    help="If specified will use as input folder for embed samples, else will use same directroy specified by \"folder\" option.")
parser.add_argument("--paramfile", dest="paramfile", type=str,
    help="Name of parameter file")
parser.add_argument("--cat", dest="cat", type=str,
    help="Category")
parser.add_argument("--datacard", dest="datacard", type=str,
    help="Datacard name")
parser.add_argument("--year", dest="year", type=str,
    help="Year")
parser.add_argument("--campaign", dest="campaign", type=str,
    help="Campaign")
parser.add_argument("--sel", dest="sel", type=str,
    help="Selection")
parser.add_argument("--set_alias", action="append", dest="set_alias", type=str,
    help="Overwrite alias selection using this options. Specify with the form --set_alias=nameofaliastoreset:newselection")
parser.add_argument("--var", dest="var", type=str,
    help="Variable to plot")
parser.add_argument("--norm_bins", dest="norm_bins", action='store_true',
    help="Normalize bins by bin width.")
parser.add_argument("--blind", dest="blind", action='store_true',
    help="Blind histogram.")
parser.add_argument("--x_blind_min", dest="x_blind_min", type=float,
    help="Minimum x for blinding.")
parser.add_argument("--x_blind_max", dest="x_blind_max", type=float,
    help="Maximum x for blinding.")
parser.add_argument("--ratio", dest="ratio", action='store_true',
    help="Draw ratio.")
parser.add_argument("--y_title", dest="y_title", type=str,
    help="Y-axis title.")
parser.add_argument("--x_title", dest="x_title", type=str,
    help="X-axis title.")
parser.add_argument("--custom_y_range", dest="custom_y_range", action='store_true',
    help="Use custom y-axis range")
parser.add_argument("--y_axis_min", dest="y_axis_min", type=float,
    help="Minimum y-axis value.")
parser.add_argument("--y_axis_max", dest="y_axis_max", type=float,
    help="Maximum y-axis value.")
parser.add_argument("--custom_x_range", dest="custom_x_range", action='store_true',
    help="Use custom x-axis range")
parser.add_argument("--x_axis_min", dest="x_axis_min", type=float,
    help="Minimum x-axis value.")
parser.add_argument("--x_axis_max", dest="x_axis_max", type=float,
    help="Maximum x-axis value.")
parser.add_argument("--log_x", dest="log_x", action='store_true',
    help="Set log scale on x-axis.")
parser.add_argument("--log_y", dest="log_y", action='store_true',
    help="Set log scale on y-axis.")
parser.add_argument("--lumi", dest="lumi", type=str,
    help="Lumi.")
parser.add_argument("--no_plot", dest="no_plot", action='store_true',
    help="If option is set then no pdf or png plots will be created only the output root file will be produced.")
parser.add_argument("--ratio_range", dest="ratio_range", type=str,
    help="y-axis range for ratio plot in format MIN,MAX")
parser.add_argument("--add_wt", dest="add_wt", type=str,
    help="Name of additional weight to be applied to all templates.")
parser.add_argument("--charges_non_zero", dest="charges_non_zero", action='store_true',
    help="If option is set then will run for events where the charges do not sum to 0.")
parser.add_argument("--plot_signals", dest="plot_signals", type=str,
			help="Comma separated list of what signals to plot")
parser.add_argument("--vsjets", default="vvvloose", type=str,
      help="vs jets deep tau working point")
parser.add_argument("--vsjets_fail", default=None, type=str,
      help="vs jets deep tau working point for fail FF")
parser.add_argument("--vsmu", default="vloose", type=str,
      help="vs muon deep tau working point")
parser.add_argument("--vsele", default="vvloose", type=str,
      help="vs electron deep tau working point")
parser.add_argument("--auto_rebinning", dest="auto_rebinning", action='store_true',
    help="Do auto rebinning")
parser.add_argument("--bin_threshold", dest="bin_threshold", type=float, default=100000.0,
    help="Threshold for bin auto rebin value")
parser.add_argument("--bin_uncert_fraction", dest="bin_uncert_fraction", type=float, default=0.5,
    help="Threshold for bin auto rebin fractional uncertainty")
parser.add_argument("--signal_scale", dest="signal_scale", type=float, default=1.0,
    help="Scale the signal by this amount")
parser.add_argument("--do_trg_sf", dest="do_trg_sf", action='store_true',
    help="Derive and apply trigger scale factors here.")
parser.add_argument("--method", dest="method", type=int,
    help="Method to model backgrouns.")
parser.add_argument("--no_data", dest="no_data", action='store_true',
    help="Do not draw data.")
parser.add_argument("--do_unrolling", dest="do_unrolling", action='store_true',
    help="If argument is set to true will unroll 2D histograms into 1D histogram.")
parser.add_argument("--plot_from_dc", default="", type=str,
      help="If not empty will draw plot straight from datacard")
parser.add_argument("--extra_name", default="", type=str,
      help="Extra name for plot")
options = parser.parse_args(remaining_argv)   

print ''
print '################### Options ###################'
print 'channel           = ' + options.channel
print 'outputfolder      = ' + options.outputfolder
print 'folder            = ' + options.folder
print 'paramfile         = ' + options.paramfile
print 'cat               = ' + options.cat
print 'datacard          = ' + options.datacard
print 'year              = ' + options.year
print 'campaing          = ' + options.campaign
print 'sel               = ' + options.sel
print 'var               = ' + options.var
print '###############################################'
print ''

if options.campaign == "ReReco" and options.channel in ["eett","mmtt"]:
	print "ERROR: Do not have data files to run these channels for ReReco"
	exit()
if (options.campaign == "ReReco" and options.year not in ["2016","2017","2018"]) or (options.campaign == "UL" and options.year not in ["2016_preVFP","2016_postVFP","2017","2018"]):
	print "ERROR: Year requested not available for campaign"
	exit()

cats = {}

VsJets_wp_fail = options.vsjets_fail
VsJets_wp = options.vsjets
VsMu_wp = options.vsmu
VsEle_wp = options.vsele
lepton_iso = "0.15"

t_sel = ""
if VsJets_wp != "None":
  t_sel += "deepTauVsJets_%(VsJets_wp)s_X>0.5" % vars()
if VsMu_wp != "None":
  if t_sel != "": t_sel += " && "
  t_sel += "deepTauVsMu_%(VsMu_wp)s_X>0.5" % vars()
if VsEle_wp != "None":
  if t_sel != "": t_sel += " && "
  t_sel += "deepTauVsEle_%(VsEle_wp)s_X>0.5" % vars()

if t_sel == "": t_sel = "(1)"

e_sel = "iso_X<%(lepton_iso)s" % vars()
m_sel = "iso_X<%(lepton_iso)s" % vars()


if options.charges_non_zero: 
	charge_sel = "(q_1+q_2+q_3+q_4)!=0"
else:
	charge_sel = "(q_1+q_2+q_3+q_4)==0"

if options.channel == 'ttt':
   charge_sel = "(1)"

if options.channel == "tttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "(trg_doubletau_12 || trg_doubletau_13 || trg_doubletau_14 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34)"
  cats['trigger'] = "(1)"
  cats['data_veto'] = "(1)"
if options.channel == "ttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"))
  cats['trigger'] = "(trg_doubletau_12 || trg_doubletau_13 || trg_doubletau_23)"
  cats['data_veto'] = "(1)"
elif options.channel == "ettt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "(trg_singleelectron_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34)"
  cats['data_veto'] = "!(isTau && (trg_singleelectron_1))"
elif options.channel == "mttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "(trg_singlemuon_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34)"
  cats['data_veto'] = "!(isTau && (trg_singlemuon_1))"
elif options.channel == "emtt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "(trg_singleelectron_1 || trg_singlemuon_2 || trg_doubletau_34)"
  cats['data_veto'] = "(!(isTau && (trg_singlemuon_2))) && (!(isSingleElectron && (trg_singlemuon_1 || trg_doubletau_34)))"
elif options.channel == "mmtt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  #cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4").replace("Jets_loose_4>","Jets_loose_4<"))
  cats['trigger'] = "(trg_singlemuon_1 || trg_singlemuon_2 || trg_doubletau_34)"
  cats['data_veto'] = "!(isTau && (trg_singlemuon_1 || trg_singlemuon_2))"
elif options.channel == "eett":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=e_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "(trg_singleelectron_1 || trg_singleelectron_2 || trg_doubletau_34)"
  cats['data_veto'] = "!(isTau && (trg_singleelectron_1 || trg_singleelectron_2))"
 # cats['trigger'] = "(1)"
  cats['data_veto'] = "(1)"

cats['baseline'] = "(({charge_sel}) && {current_baseline} && {trigger_sel})".format(charge_sel=charge_sel,current_baseline=cats['baseline'],trigger_sel=cats['trigger'])

if options.do_trg_sf:
  if options.channel in ["tttt","ttt"]:
    sf_string = "(1)" # string too long to do here
  if options.channel == "ettt":
    #sf_string = "((trigeff_singlee_data_1 + trigeff_doubletau_data_2*trigeff_doubletau_data_3 + trigeff_doubletau_data_2*trigeff_doubletau_data_4 + trigeff_doubletau_data_3*trigeff_doubletau_data_4 - (trigeff_singlee_data_1*trigeff_doubletau_data_2*trigeff_doubletau_data_3) - (trigeff_singlee_data_1*trigeff_doubletau_data_2*trigeff_doubletau_data_4) - (trigeff_singlee_data_1*trigeff_doubletau_data_3*trigeff_doubletau_data_4) - (trigeff_doubletau_data_2*trigeff_doubletau_data_3*trigeff_doubletau_data_2*trigeff_doubletau_data_4) - (trigeff_doubletau_data_2*trigeff_doubletau_data_3*trigeff_doubletau_data_3*trigeff_doubletau_data_4) - (trigeff_doubletau_data_2*trigeff_doubletau_data_4*trigeff_doubletau_data_3*trigeff_doubletau_data_4) + (trigeff_singlee_data_1*trigeff_doubletau_data_2*trigeff_doubletau_data_3*trigeff_doubletau_data_2*trigeff_doubletau_data_4) + (trigeff_singlee_data_1*trigeff_doubletau_data_2*trigeff_doubletau_data_3*trigeff_doubletau_data_3*trigeff_doubletau_data_4) + (trigeff_singlee_data_1*trigeff_doubletau_data_2*trigeff_doubletau_data_4*trigeff_doubletau_data_3*trigeff_doubletau_data_4) + (trigeff_doubletau_data_2*trigeff_doubletau_data_3*trigeff_doubletau_data_2*trigeff_doubletau_data_4*trigeff_doubletau_data_3*trigeff_doubletau_data_4) - (trigeff_singlee_data_1*trigeff_doubletau_data_2*trigeff_doubletau_data_3*trigeff_doubletau_data_2*trigeff_doubletau_data_4*trigeff_doubletau_data_3*trigeff_doubletau_data_4)) / (trigeff_singlee_mc_1 + trigeff_doubletau_mc_2*trigeff_doubletau_mc_3 + trigeff_doubletau_mc_2*trigeff_doubletau_mc_4 + trigeff_doubletau_mc_3*trigeff_doubletau_mc_4 - (trigeff_singlee_mc_1*trigeff_doubletau_mc_2*trigeff_doubletau_mc_3) - (trigeff_singlee_mc_1*trigeff_doubletau_mc_2*trigeff_doubletau_mc_4) - (trigeff_singlee_mc_1*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4) - (trigeff_doubletau_mc_2*trigeff_doubletau_mc_3*trigeff_doubletau_mc_2*trigeff_doubletau_mc_4) - (trigeff_doubletau_mc_2*trigeff_doubletau_mc_3*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4) - (trigeff_doubletau_mc_2*trigeff_doubletau_mc_4*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4) + (trigeff_singlee_mc_1*trigeff_doubletau_mc_2*trigeff_doubletau_mc_3*trigeff_doubletau_mc_2*trigeff_doubletau_mc_4) + (trigeff_singlee_mc_1*trigeff_doubletau_mc_2*trigeff_doubletau_mc_3*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4) + (trigeff_singlee_mc_1*trigeff_doubletau_mc_2*trigeff_doubletau_mc_4*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4) + (trigeff_doubletau_mc_2*trigeff_doubletau_mc_3*trigeff_doubletau_mc_2*trigeff_doubletau_mc_4*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4) - (trigeff_singlee_mc_1*trigeff_doubletau_mc_2*trigeff_doubletau_mc_3*trigeff_doubletau_mc_2*trigeff_doubletau_mc_4*trigeff_doubletau_mc_3*trigeff_doubletau_mc_4)))"
    sf_string = "(1)"
  if options.channel == "mttt":
    sf_string = "(1)"
  if options.channel == "emtt":
    sf_string = "(1)"
  if options.channel == "eett":
    sf_string = "((trigeff_doubletau_data_3*trigeff_doubletau_data_4 + trigeff_singlee_data_1 + trigeff_singlee_data_2 - (trigeff_doubletau_data_3*trigeff_doubletau_data_4*trigeff_singlee_data_1) - (trigeff_doubletau_data_3*trigeff_doubletau_data_4*trigeff_singlee_data_2) - (trigeff_singlee_data_1*trigeff_singlee_data_2) + (trigeff_doubletau_data_3*trigeff_doubletau_data_4*trigeff_singlee_data_1*trigeff_singlee_data_2)) / (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4 + trigeff_singlee_mc_1 + trigeff_singlee_mc_2 - (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4*trigeff_singlee_mc_1) - (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4*trigeff_singlee_mc_2) - (trigeff_singlee_mc_1*trigeff_singlee_mc_2) + (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4*trigeff_singlee_mc_1*trigeff_singlee_mc_2)))"
  if options.channel == "mmtt":
    sf_string = "((trigeff_doubletau_data_3*trigeff_doubletau_data_4 + trigeff_singlem_data_1 + trigeff_singlem_data_2 - (trigeff_doubletau_data_3*trigeff_doubletau_data_4*trigeff_singlem_data_1) - (trigeff_doubletau_data_3*trigeff_doubletau_data_4*trigeff_singlem_data_2) - (trigeff_singlem_data_1*trigeff_singlem_data_2) + (trigeff_doubletau_data_3*trigeff_doubletau_data_4*trigeff_singlem_data_1*trigeff_singlem_data_2)) / (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4 + trigeff_singlem_mc_1 + trigeff_singlem_mc_2 - (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4*trigeff_singlem_mc_1) - (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4*trigeff_singlem_mc_2) - (trigeff_singlem_mc_1*trigeff_singlem_mc_2) + (trigeff_doubletau_mc_3*trigeff_doubletau_mc_4*trigeff_singlem_mc_1*trigeff_singlem_mc_2)))"
  sf_string = "(1)"
  
  if options.add_wt == "":
    options.add_wt = "(" + sf_string + ")"
  else:
    options.add_wt = "((" + options.add_wt + ")*(" + sf_string + "))"

cats['inclusive'] = '(1)' 
cats['btag'] = '(n_bjets>=1)'
cats['nobtag'] = '(n_bjets==0)'
cats['z_control'] = '((q_1==-q_2) && (q_3==-q_4))'
cats['2l2t_sig'] = '((q_1==q_2) && (q_3==q_4))'
cats['z_control_nobtag'] = '((q_1==-q_2) && (q_3==-q_4) && (n_bjets==0))'
cats['2l2t_sig_nobtag'] = '((q_1==q_2) && (q_3==q_4) && (n_bjets==0))'
cats['z_control_btag'] = '((q_1==-q_2) && (q_3==-q_4) && (n_bjets>0))'
cats['2l2t_sig_btag'] = '((q_1==q_2) && (q_3==q_4) && (n_bjets>0))'


# Overwrite any category selections if the --set_alias option is used
for i in options.set_alias:
		cat_to_overwrite = i.split(':')[0]
		cat_to_overwrite=cat_to_overwrite.replace("\"","")
		overwrite_with = i.split(':')[1]
		overwrite_with=overwrite_with.replace("\"","")
		start_index=overwrite_with.find("{")
		end_index=overwrite_with.find("}")
		while start_index >0:
				replace_with=overwrite_with[start_index:end_index+1]
				replace_with=replace_with.replace("{","")
				replace_with=replace_with.replace("}","")
				replace_string = cats[replace_with]
				overwrite_with=overwrite_with[0:start_index] + replace_string  + overwrite_with[end_index+1:]
				start_index=overwrite_with.find("{")
				end_index=overwrite_with.find("}")

		print 'Overwriting alias: \"'+cat_to_overwrite+'\" with selection: \"'+overwrite_with+'\"'
		if cat_to_overwrite == 'sel':
				options.sel = overwrite_with
		else:
				cats[cat_to_overwrite] = overwrite_with
		
# Add data sample names
if (options.channel == 'tttt' or options.channel == 'ttt'):
	if options.year == "2016-preVFP":
		data_samples = ['TauB','TauC','TauD','TauE','TauF']
	elif options.year == "2016-postVFP":
		data_samples = ['TauF','TauG','TauH']
	elif options.year == "2016":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','TauG','TauH']
	elif options.year == "2017":
		data_samples = ['TauB','TauC','TauD','TauE','TauF']
	elif options.year == "2018":
		data_samples = ['TauA','TauB','TauC','TauD'] 

elif options.channel == 'ettt':
	if options.year == "2016_preVFP":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
	elif options.year == "2016_postVFP":
		data_samples = ['TauF','TauG','TauH','SingleElectronF','SingleElectronG','SingleElectronH']
	elif options.year == "2016":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','TauG','TauH','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','SingleElectronG','SingleElectronH']
	elif options.year == "2017":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
	elif options.year == "2018":
		data_samples = ['TauA','TauB','TauC','TauD','EGammaA','EGammaB','EGammaC','EGammaD']

elif options.channel == 'mttt':
	if options.year == "2016_preVFP":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
	elif options.year == "2016_postVFP":
		data_samples = ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH']
	elif options.year == "2016":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','TauG','TauH','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleMuonG','SingleMuonH']
	elif options.year == "2017":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
	elif options.year == "2018":
		data_samples = ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD']

elif options.channel == 'eett':
	if options.year == "2016_preVFP":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
	elif options.year == "2016_postVFP":
		data_samples = ['TauF','TauG','TauH','SingleElectronF','SingleElectronG','SingleElectronH']
	elif options.year == "2017":
		data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
	elif options.year == "2018":
		data_samples = ['TauA','TauB','TauC','TauD','EGammaA','EGammaB','EGammaC','EGammaD']

elif options.channel == 'mmtt':
  if options.year == "2016_preVFP":
	  data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
  elif options.year == "2016_postVFP":
	  data_samples = ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH']
  elif options.year == "2017":
	  data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD']

elif options.channel == 'emtt':
  if options.year == "2016_preVFP":
	  data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
  elif options.year == "2016_postVFP":
	  data_samples = ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH','SingleElectronF','SingleElectronG','SingleElectronH']
  elif options.year == "2016":
	  data_samples = ['TauB','TauC','TauD','TauE','TauF','TauG','TauH','SingleMuonB', 'SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleMuonG','SingleMuonH','SingleElectronB', 'SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','SingleElectronG','SingleElectronH']
  elif options.year == "2017":
	  data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleElectronB', 'SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD','EGammaA','EGammaB','EGammaC','EGammaD']

if options.year == "2018":
    if options.campaign == "ReReco":
      ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
      top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
      vv_samples = [
      				'T-tW-ext1', 'Tbar-tW-ext1','Tbar-t','WWTo2L2Nu','T-t',
      				'WWToLNuQQ','WZTo1L3Nu','WZTo3LNu','WZTo3LNu-ext1','WZTo2L2Q',
      				'ZZTo2L2Nu-ext1','ZZTo2L2Nu-ext2','ZZTo2L2Q','ZZTo4L-ext','ZZTo4L'
      				]
      vvv_samples = []
      wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
      wgam_samples = ['WGToLNuG']
      ewkz_samples = ['EWKZ2Jets']
    elif options.campaign == "UL":
      ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO']
      top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
      vv_samples = ['WZTo1L1Nu2Q','WZTo3LNu','WWTo1L1Nu2Q','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L','Tbar-t','Tbar-tW','T-t','T-tW']
      vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
      wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
      wgam_samples = ['WGToLNuG']
      ewkz_samples = ['EWKZ2Jets_ZToLL']
      signal_samples = ["phi200A100To4Tau","phi200A20To4Tau","phi300A20To4Tau","phi100A150To4Tau","phi300A150To4Tau","phi300A60To4Tau","phi100A100To4Tau","phi200A60To4Tau","phi300A100To4Tau","phi100A60To4Tau","phi200A150To4Tau"]


if options.year == "2017":
  if options.campaign == "ReReco":
    ztt_samples = ['DYJetsToLL-LO','DYJetsToLL-LO-ext1','DY1JetsToLL-LO','DY1JetsToLL-LO-ext','DY2JetsToLL-LO','DY2JetsToLL-LO-ext','DY3JetsToLL-LO','DY3JetsToLL-LO-ext','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','DYJetsToLL_M-10-50-LO-ext1']
    top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
    vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WZTo2L2Q','WZTo3LNu', 'ZZTo2L2Q','ZZTo4L-ext','ZZTo4L','VVTo2L2Nu']
    vvv_samples = []
    wjets_samples = ['WJetsToLNu-LO','WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
    wgam_samples = ['WGToLNuG']
    ewkz_samples = ['EWKZ2Jets']
    signal_samples = []
  elif options.campaign == "UL":
    ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO']
    top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
    vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L']
    vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
    wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
    wgam_samples = ['WGToLNuG']
    ewkz_samples = ['EWKZ2Jets_ZToLL']
    signal_samples = []

if options.year == "2016_postVFP":
  ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO']
  top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
  vv_samples = ['WZTo3LNu','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L','T-tW', 'Tbar-tW','Tbar-t','T-t']
  vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
  wgam_samples = ['WGToLNuG']
  wjets_samples = ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
  ewkz_samples = ['EWKZ2Jets_ZToLL']
  signal_samples = []

if options.year == "2016_preVFP":
  ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO']
  top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
  vv_samples = ['WZTo1L1Nu2Q','WZTo3LNu','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L','T-tW', 'Tbar-tW','Tbar-t','T-t']
  vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
  wjets_samples = ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
  wgam_samples = ['WGToLNuG']
  ewkz_samples = ['EWKZ2Jets_ZToLL']
  signal_samples = []

if options.year == "2016":
  ztt_samples = ['DYJetsToLL-LO-ext1','DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
  vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','VVTo2L2Nu-ext1','ZZTo2L2Q','ZZTo4L-amcat','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q']
  vvv_samples = []
  wgam_samples = ['WGToLNuG-ext1','WGToLNuG-ext2','WGToLNuG-ext3']
  wjets_samples = ['WJetsToLNu-LO', 'WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W2JetsToLNu-LO-ext','W3JetsToLNu-LO','W3JetsToLNu-LO-ext','W4JetsToLNu-LO','W4JetsToLNu-LO-ext1','W4JetsToLNu-LO-ext2']
  ewkz_samples = ['EWKZ2Jets_ZToLL','EWKZ2Jets_ZToLL-ext1','EWKZ2Jets_ZToLL-ext2']
  signal_samples = []

vv_samples = ["ZZTo4L"]

ROOT.TH1.SetDefaultSumw2(True)

correct_gen_matches = []
t_gen_matches = []
e_gen_matches = []
m_gen_matches = []
lfake_gen_matches = []
jfake_gen_matches = []

for in0,ch in enumerate(options.channel):
  ind = in0+1
  if ch == "e":
    correct_gen_matches.append("(gen_match_%(ind)i==1 || gen_match_%(ind)i==3)" % vars()) 
  elif ch == "m":
    correct_gen_matches.append("(gen_match_%(ind)i==2 || gen_match_%(ind)i==4)" % vars())
  elif ch == "t":
    correct_gen_matches.append("(gen_match_%(ind)i==5)" % vars())
    lfake_gen_matches.append("(gen_match_%(ind)i<5)" % vars())
    jfake_gen_matches.append("(gen_match_%(ind)i==6)" % vars())

  t_gen_matches.append("(gen_match_%(ind)i==5 || gen_match_%(ind)i==3 || gen_match_%(ind)i==4)" % vars())
  e_gen_matches.append("(gen_match_%(ind)i==1)" % vars())
  m_gen_matches.append("(gen_match_%(ind)i==2)" % vars())

if options.channel == "ttt":
   summed_gen_matches = "({gm1} + {gm2} + {gm3})".format(gm1=correct_gen_matches[0],gm2=correct_gen_matches[1],gm3=correct_gen_matches[2])
   summed_t_gen_matches = "({gm1} + {gm1} + {gm3})".format(gm1=t_gen_matches[0],gm2=t_gen_matches[1],gm3=t_gen_matches[2])
   summed_e_gen_matches = "({gm1} + {gm2} + {gm3})".format(gm1=e_gen_matches[0],gm2=e_gen_matches[1],gm3=e_gen_matches[2])
   summed_m_gen_matches = "({gm1} + {gm2} + {gm3})".format(gm1=m_gen_matches[0],gm2=m_gen_matches[1],gm3=m_gen_matches[2])
else:
   summed_gen_matches = "({gm1} + {gm2} + {gm3} + {gm4})".format(gm1=correct_gen_matches[0],gm2=correct_gen_matches[1],gm3=correct_gen_matches[2],gm4=correct_gen_matches[3])
   summed_t_gen_matches = "({gm1} + {gm2} + {gm3} + {gm4})".format(gm1=t_gen_matches[0],gm2=t_gen_matches[1],gm3=t_gen_matches[2],gm4=t_gen_matches[3])
   summed_e_gen_matches = "({gm1} + {gm2} + {gm3} + {gm4})".format(gm1=e_gen_matches[0],gm2=e_gen_matches[1],gm3=e_gen_matches[2],gm4=e_gen_matches[3])
   summed_m_gen_matches = "({gm1} + {gm2} + {gm3} + {gm4})".format(gm1=m_gen_matches[0],gm2=m_gen_matches[1],gm3=m_gen_matches[2],gm4=m_gen_matches[3])

summed_lfake_gen_matches = "("
for ind,i in enumerate(lfake_gen_matches):
  summed_lfake_gen_matches += i
  if ind + 1 < len(lfake_gen_matches):
    summed_lfake_gen_matches += " + "
summed_lfake_gen_matches += ")"

summed_jfake_gen_matches = "("
for ind,i in enumerate(jfake_gen_matches):
  summed_jfake_gen_matches += i
  if ind + 1 < len(jfake_gen_matches):
    summed_jfake_gen_matches += " + "
summed_jfake_gen_matches += ")"

other_sels = {
              "R" :"({sgm} == {len_channel})".format(sgm=summed_gen_matches,len_channel=len(options.channel)),
              "JF":"({jfgm} > 0)".format(jfgm=summed_jfake_gen_matches),
              "LF":"({lfgm} > 0) && ({jfgm} == 0)".format(lfgm=summed_jfake_gen_matches,jfgm=summed_jfake_gen_matches),
              }

z_sels = {"Z"+key:val for (key,val) in other_sels.items()}
top_sels = {"TT"+key:val for (key,val) in other_sels.items()}
vv_sels = {"VV"+key:val for (key,val) in other_sels.items()}
w_sels = {"W"+key:val for (key,val) in other_sels.items()}
vvv_sels = {"VVV"+key:val for (key,val) in other_sels.items()}


# All functions defined here

def BuildCutString(wt='', sel='', cat='',bkg_sel='', add_sel=''):
    full_selection = '(1)'
    if wt != '':
        full_selection = '('+wt+')'
    if sel != '':
        full_selection += '*('+sel+')'
    if add_sel != '':
        full_selection += '*('+add_sel+')'
    if bkg_sel != '':
        full_selection += '*('+bkg_sel+')'
    if cat != '':
        full_selection += '*('+cat+')'
    return full_selection

def GetNode(ana, name, add_name='', samples=[], plot='', wt='', sel='', cat='', add_sel=''):
    full_selection = BuildCutString(wt, sel, cat, add_sel)
    return ana.SummedFactory(name+add_name, samples, plot, full_selection)

def GenerateZTT(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}):
    for key, val in z_sels.items():
      ztt_node = GetNode(ana, key, add_name, samples, plot, wt, "("+val+"&&"+sel+")", cat)  
      ana.nodes[nodename].AddNode(ztt_node)
    
def GenerateEWKZ(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}):
    ewkz_node = GetNode(ana, "EWKZ", add_name, samples, plot, wt, sel, cat)  
    ana.nodes[nodename].AddNode(ewkz_node) 

def GenerateTop(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}):
    for key, val in top_sels.items():
      tt_node = GetNode(ana, key, add_name, samples, plot, wt, "("+val+"&&"+sel+")", cat)
      ana.nodes[nodename].AddNode(tt_node)

def GenerateVV(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}):
    for key, val in vv_sels.items():
      vv_node = GetNode(ana, key, add_name, samples, plot, wt, "("+val+"&&"+sel+")", cat)
      ana.nodes[nodename].AddNode(vv_node)

def GenerateVVV(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', vvv_sels={}):
    vvv_node = GetNode(ana, "VVV", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(vvv_node)

def GenerateW(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', w_sels={}):
    wg_node = GetNode(ana, "WGam", add_name, wgam_samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(wg_node)
    for key, val in w_sels.items():
      w_node = GetNode(ana, key, add_name, samples, plot, wt, "("+val+"&&"+sel+")", cat)
      ana.nodes[nodename].AddNode(w_node)

def GenerateFakeTaus(ana, add_name='', data_samples=[], mc_samples=[], plot='', wt='', sel='', cat='', charges_non_zero=False, data_veto=None):
  vj = "deepTauVsJets_" + VsJets_wp
  if VsJets_wp_fail == None:
    vjf = "1"
  else:
    vjf = "deepTauVsJets_" + VsJets_wp_fail
  
  ff_sel = cat.replace(vj,vjf).replace("1_1","1").replace("1_2","1").replace("1_3","1").replace("1_4","1").replace("!1>0.5","(1)").replace("&& 1>0.5","").replace("&& 1<0.5","")


  # pass selections
  pass_1 = "({vjn}_1>0.5)".format(vjn=vj)
  pass_2 = "({vjn}_2>0.5)".format(vjn=vj)
  pass_3 = "({vjn}_3>0.5)".format(vjn=vj)
  pass_4 = "({vjn}_4>0.5)".format(vjn=vj)

  # fail selections
  fail_1 = "({vjn}_1<0.5)".format(vjn=vj)
  fail_2 = "({vjn}_2<0.5)".format(vjn=vj)
  fail_3 = "({vjn}_3<0.5)".format(vjn=vj)
  fail_4 = "({vjn}_4<0.5)".format(vjn=vj)

  if options.channel in ["emtt","eett","mmtt"]:
    #ff_wt = "(wt_ff_ml_3 * %(fail_3)s * %(pass_4)s)" % vars()
    #ff_wt = "(wt_ff_ml_3 * %(fail_3)s * %(fail_4)s)" % vars()
    #ff_wt = "(wt_ff_ml_4 * %(pass_3)s * %(fail_4)s)" % vars()
    #ff_wt = "(wt_ff_ml_3 * wt_ff_ml_4 * %(fail_3)s * %(fail_4)s)" % vars()
    #ff_wt = "(0.5*((wt_ff_ml_3 * %(fail_3)s * %(pass_4)s) + (wt_ff_ml_4 * %(pass_3)s * %(fail_4)s)))" % vars()
    ff_wt = "((wt_ff_ml_3 * %(fail_3)s * %(pass_4)s) + (wt_ff_ml_4 * %(pass_3)s * %(fail_4)s)  - (wt_ff_ml_3 * wt_ff_ml_4 * %(fail_3)s * %(fail_4)s))" % vars()
    #ff_wt = "(%(ff_raw_34)s * %(ff_corr_34)s * %(fail_3)s * %(fail_4)s)" % vars()
  #elif options.channel in ["ettt","mttt"]:
  #elif options.channel in ["tttt"]:


  if data_veto == None:
    ff_data = GetNode(ana, 'jetFakes', add_name, data_samples, plot, wt, sel, ff_sel)
  else:
    ff_data = GetNode(ana, 'jetFakes', add_name, data_samples, plot, ff_wt, sel, "("+ff_sel+")&&("+data_veto+")")

  ana.nodes[nodename].AddNode(ff_data)

def GenerateSignal(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    for i in samples:
      signal_node = GetNode(ana, i, add_name, [i], plot, wt, sel, cat)
      ana.nodes[nodename].AddNode(signal_node)
 
def PrintSummary(nodename='', data_strings=['data_obs'], add_names=''):
    print ''
    print '################### Summary ###################'
    nodes = ana.nodes[nodename].SubNodes()
    bkg_total = ufloat(0.000000001,0.000000001)
    sig_total = ufloat(0.000000001,0.000000001)
    for node in nodes:
        if node.shape.rate.n == 0: per_err = 0
        else: per_err = node.shape.rate.s/node.shape.rate.n
        print node.name.ljust(10) , ("%.2f" % node.shape.rate.n).ljust(10), '+/-'.ljust(5), ("%.2f" % node.shape.rate.s).ljust(7), "(%.4f)" % per_err
        if True in [node.name.find(add_name) != -1 and add_name is not '' for add_name in add_names]: continue
        if node.name not in data_strings: bkg_total += node.shape.rate      
    if bkg_total.n == 0: per_err = 0        
    else: per_err = bkg_total.s/bkg_total.n
    print 'Total bkg'.ljust(10) , ("%.2f" % bkg_total.n).ljust(10), '+/-'.ljust(5), ("%.2f" % bkg_total.s).ljust(7), "(%.4f)" % per_err
    if sig_total.n == 0: per_err = 0
    else: per_err = sig_total.s/sig_total.n
    print 'Total sig'.ljust(10) , ("%.2f" % sig_total.n).ljust(10), '+/-'.ljust(5), ("%.2f" % sig_total.s).ljust(7), "(%.4f)" % per_err
    print '###############################################'
    print ''
    

def FixBins(ana,outfile='output.root'):
    #Fix empty histograms
    nodes = ana.nodes[nodename].SubNodes()
    for node in nodes:
        if 'data_obs' in node.name: continue
        hist = outfile.Get(nodename+'/'+node.name)
        outfile.cd(nodename)
        #Fix empty histogram
        if hist.Integral() == 0.0:
            hist.SetBinContent(hist.GetNbinsX()/2, 0.00001)
            hist.SetBinError(hist.GetNbinsX()/2, 0.00001)
            hist.Write()
        outfile.cd()

def GetTotals(ana,add_name="",outfile='outfile.root'):
    # add histograms to get totals for backgrounds split into real/fake taus and make a total backgrounds histogram
    outfile.cd(nodename)
    nodes = ana.nodes[nodename].SubNodes()
    first_hist=True
    for node in nodes:
      if add_name not in node.name: continue
      if first_hist:
          total_bkg = ana.nodes[nodename].nodes[node.name].shape.hist.Clone()
          first_hist=False
      else: total_bkg.Add(ana.nodes[nodename].nodes[node.name].shape.hist.Clone())
    if not first_hist:        
      total_bkg.SetName('total_bkg'+add_name)
      total_bkg.Write()
    outfile.cd()

def RunPlotting(ana, cat='',cat_data='', sel='', add_name='', wt='wt', do_data=True, samples_to_skip=[], outfile='output.root'):
    # produce template for observed data
    if do_data:
      weight='(1)'
      data_sel = "((" +cats["data_veto"] + ") && (" + sel + "))"
      full_selection = BuildCutString(weight, data_sel, cat_data)
      ana.nodes[nodename].AddNode(ana.SummedFactory('data_obs', data_samples, plot_unmodified, full_selection))
    #else:
    if options.add_wt : wt+='*'+options.add_wt
     
    # produce templates for backgrounds
    if options.method == 1:
      if 'ZTT' not in samples_to_skip:
          GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels)
      if 'TT' not in samples_to_skip:    
          GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels) 
      if 'VV' not in samples_to_skip:
          GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels)
      if 'VVV' not in samples_to_skip:
          GenerateVVV(ana, add_name, vvv_samples, plot, wt, sel, cat, vvv_sels) 
      if 'W' not in samples_to_skip:
          GenerateW(ana, add_name, wjets_samples, plot, wt, sel, cat, w_sels)
      if 'signal' not in samples_to_skip:
          GenerateSignal(ana, add_name, signal_samples, plot, wt, sel, cat)
    elif options.method == 2:
      if 'jetFakes' not in samples_to_skip:
        mc_samples = ztt_samples + vv_samples + vvv_samples + wgam_samples + top_samples + wjets_samples + ewkz_samples
        GenerateFakeTaus(ana, add_name, data_samples, mc_samples, plot, wt, sel, cat, options.charges_non_zero, data_veto=cats["data_veto"]) 
 
      if options.channel == "tttt":
        cat = "("+cat+")&&(gen_match_1<6 && gen_match_2<6 && gen_match_3<6 && gen_match_4<6)"
      elif options.channel == "ttt":
        cat = "("+cat+")&&(gen_match_1<6 && gen_match_2<6 && gen_match_3<6)"
      elif options.channel in ["ettt","mttt"]:
        cat = "("+cat+")&&(gen_match_2<6 && gen_match_3<6 && gen_match_4<6)"
      elif options.channel in ["eett","mmtt","emtt"]:
        cat = "("+cat+")&&(gen_match_3<6 && gen_match_4<6)"

      #if 'ZTT' not in samples_to_skip:
      #    GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels)
      #if 'TT' not in samples_to_skip:
      #    GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels)
      if 'VV' not in samples_to_skip:
          GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels)
      #if 'VVV' not in samples_to_skip:
      #    GenerateVVV(ana, add_name, vvv_samples, plot, wt, sel, cat, vvv_sels)
      #if 'W' not in samples_to_skip:
      #    GenerateW(ana, add_name, wjets_samples, plot, wt, sel, cat, w_sels)
      if 'signal' not in samples_to_skip:
          GenerateSignal(ana, add_name, signal_samples, plot, wt, sel, cat)

def Get1DBinNumFrom2D(h2d,xbin,ybin):
    Nxbins = h2d.GetNbinsX()
    return (ybin-1)*Nxbins + xbin -1

def Get1DBinNumFrom3D(h3d,xbin,ybin,zbin):
    Nxbins = h3d.GetNbinsX()
    Nybins = h3d.GetNbinsY()
    return (zbin-1)*Nxbins*Nybins + (ybin-1)*Nxbins + xbin -1

def UnrollHist2D(h2d,inc_y_of=True):
    # inc_y_of = True includes the y over-flow bins
    if inc_y_of: n = 1
    else: n = 0
    Nbins = (h2d.GetNbinsY()+n)*(h2d.GetNbinsX())
    h1d = ROOT.TH1D(h2d.GetName(), '', Nbins, 0, Nbins)
    for i in range(1,h2d.GetNbinsX()+1):
      for j in range(1,h2d.GetNbinsY()+1+n):
        glob_bin = Get1DBinNumFrom2D(h2d,i,j)
        content = h2d.GetBinContent(i,j)
        error = h2d.GetBinError(i,j)
        h1d.SetBinContent(glob_bin+1,content)
        h1d.SetBinError(glob_bin+1,error)
    return h1d

def UnrollHist3D(h3d,inc_y_of=False,inc_z_of=True):
    if inc_y_of: ny = 1
    else: ny = 0
    if inc_z_of: nz = 1
    else: nz = 0

    Nbins = (h3d.GetNbinsZ()+nz)*(h3d.GetNbinsY()+ny)*(h3d.GetNbinsX())
    h1d = ROOT.TH1D(h3d.GetName(), '', Nbins, 0, Nbins)
    for i in range(1,h3d.GetNbinsX()+1):
      for j in range(1,h3d.GetNbinsY()+1+ny):
        for k in range(1,h3d.GetNbinsZ()+1+nz):
          glob_bin = Get1DBinNumFrom3D(h3d,i,j,k)
          content = h3d.GetBinContent(i,j,k)
          error = h3d.GetBinError(i,j,k)
          h1d.SetBinContent(glob_bin+1,content)
          h1d.SetBinError(glob_bin+1,error)
    return h1d


# Create output file
is_2d=False
is_3d=False
var_name = options.var.split('[')[0]
var_name = var_name.split('(')[0]
if var_name.count(',') == 1:
    is_2d = True
    var_name = var_name.split(',')[0]+'_vs_'+var_name.split(',')[1]
if var_name.count(',') == 2:
    is_3d = True
    var_name = var_name.split(',')[0]+'_vs_'+var_name.split(',')[1]+'_vs_'+var_name.split(',')[2]    

if options.datacard != "":
  if options.extra_name != "":
    output_name = options.outputfolder+'/'+ options.datacard + '_' + options.extra_name + '.root'
  else:
    output_name = options.outputfolder+'/'+ options.datacard +'.root'
else:
  if options.extra_name != "": 
    output_name = options.outputfolder+'/datacard_'+options.extra_name+'_'+options.cat+'_'+options.channel+'_'+options.year+'.root'
  else: 
    output_name = options.outputfolder+'/datacard_'+var_name+'_'+options.cat+'_'+options.channel+'_'+options.year+'.root'
outfile = ROOT.TFile(output_name, 'RECREATE')
  
cats['cat'] = '('+cats[options.cat]+')*('+cats['baseline']+')'
sel = options.sel
plot = options.var
plot_unmodified = plot
if options.datacard != "": nodename = options.channel+'_'+options.datacard
else: nodename = options.channel+'_'+options.cat   


add_names = []
cats_unmodified = copy.deepcopy(cats)

systematics = OrderedDict()
systematics['default'] = ('','', 'wt', [], False)

if options.plot_from_dc == "":
  max_systs_per_pass = 30 # code uses too much memory if we try and process too many systematics at once so set the maximum number of systematics processed per loop here
  while len(systematics) > 0:
    ana = Analysis()
    ana.remaps = {}
    if options.channel == 'emtt':
        ana.remaps['MuonEG'] = 'data_obs'
    elif options.channel in ['mttt']:
        ana.remaps['SingleMuon'] = 'data_obs'
    elif options.channel in ['mmtt']:
        ana.remaps['SingleMuon'] = 'data_obs'
    elif options.year != '2018' and options.channel in ['eett','ettt']:
        ana.remaps['SingleElectron'] = 'data_obs'
    elif options.year == '2018' and options.channel in ['eett','ettt']:
        ana.remaps['EGamma'] = 'data_obs'
    elif options.channel == 'tttt':
        ana.remaps['Tau'] = 'data_obs'  
        
    ana.nodes.AddNode(ListNode(nodename))

    prev_dir=None    
    for index, systematic in enumerate(list(systematics.keys())[:max_systs_per_pass]):
        if prev_dir is not None and systematics[systematic][0] is not prev_dir: continue # this ensures that we process the same trees from every call to ana.Run() - i.e trees in sub-directory systematics[systematic][0]
        prev_dir = systematics[systematic][0]
        print "Processing:", systematic
        print ""

        plot = options.var
        cats=copy.deepcopy(cats_unmodified)
        
        add_folder_name = systematics[systematic][0]
        add_name = systematics[systematic][1]
        isFFSyst = systematics[systematic][4]
        samples_to_skip = systematics[systematic][3]
        add_names.append(add_name)
        syst_add_name=add_folder_name
        

        mc_input_folder_name = options.folder
        if add_folder_name != '': mc_input_folder_name += '/'+add_folder_name
        
        if options.signal_folder: signal_mc_input_folder_name = options.signal_folder
        else: signal_mc_input_folder_name = options.folder
        if add_folder_name != '': signal_mc_input_folder_name += '/'+add_folder_name
        
        if options.embed_folder: embed_input_folder_name = options.embed_folder
        else: embed_input_folder_name = options.folder
        if add_folder_name != '' and 'EmbedZTT' not in samples_to_skip: embed_input_folder_name += '/'+add_folder_name
      
        # Add all data files
        for sample_name in data_samples:
            ana.AddSamples(options.folder+'/'+sample_name+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name)
        
        # Add all MC background files
        for sample_name in ztt_samples + vv_samples + vvv_samples + wgam_samples + top_samples + wjets_samples + ewkz_samples:
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name)
        for sample_name in signal_samples:
            ana.AddSamples(signal_mc_input_folder_name+'/'+sample_name+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name)         
 
        ana.AddInfo(options.paramfile, scaleTo='data_obs')
    
        # Add data only for default
        if systematic == 'default': do_data = True
        else: do_data = False
                
        #Run default plot
        do_data=(not options.no_data)
        weight="wt"
        #if options.plot_from_dc == "": RunPlotting(ana, cats['cat'], cats_unmodified['cat'], sel, add_name, weight, do_data, samples_to_skip,outfile)
        RunPlotting(ana, cats['cat'], cats_unmodified['cat'], sel, add_name, weight, do_data, samples_to_skip,outfile)
        del systematics[systematic]

    ana.Run()
    ana.nodes.Output(outfile)

    # fix negative bns,empty histograms etc.
    FixBins(ana,outfile)
    for n in add_names: 
      GetTotals(ana,n,outfile)
    PrintSummary(nodename, ['data_obs'], add_names)

# do rebinning
def FindRebinning(hist,BinThreshold=100,BinUncertFraction=0.5):

  # getting binning
  binning = []
  for i in range(1,hist.GetNbinsX()+2):
    binning.append(hist.GetBinLowEdge(i))

  # left to right
  finished = False
  k = 0
  while finished == False and k < 1000:
    k += 1
    for i in range(1,hist.GetNbinsX()):
      if hist.GetBinContent(i) != 0: uncert_frac = hist.GetBinError(i)/hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      if uncert_frac > BinUncertFraction and hist.GetBinContent(i) < BinThreshold:
        #binning.remove(hist.GetBinLowEdge(i+1))
        binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i+1))))
        hist = RebinHist(hist,binning)
        break
      elif i+1 == hist.GetNbinsX():
        finished = True 

  # right to left
  finished = False
  k = 0
  while finished == False and k < 1000:
    k+= 1
    for i in reversed(range(2,hist.GetNbinsX()+1)):
      if hist.GetBinContent(i) != 0: uncert_frac = hist.GetBinError(i)/hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      if uncert_frac > BinUncertFraction and hist.GetBinContent(i) < BinThreshold:
#        binning.remove(hist.GetBinLowEdge(i))
        binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i))))
        hist = RebinHist(hist,binning)
        break
      elif i == 2:
        finished = True

  return binning  

def RebinHist(hist,binning):
  # getting initial binning
  initial_binning = []
  for i in range(1,hist.GetNbinsX()+2):
    initial_binning.append(hist.GetBinLowEdge(i))
  
  new_binning = array('f', map(float,binning))
  hout = ROOT.TH1D(hist.GetName(),'',len(new_binning)-1, new_binning)
  for i in range(1,hout.GetNbinsX()+1):
    for j in range(1,hist.GetNbinsX()+1):
      if hist.GetBinCenter(j) > hout.GetBinLowEdge(i) and hist.GetBinCenter(j) < hout.GetBinLowEdge(i+1):
        new_content = hout.GetBinContent(i)+hist.GetBinContent(j)
        new_error = (hout.GetBinError(i)**2+hist.GetBinError(j)**2)**0.5
        hout.SetBinContent(i,new_content)
        hout.SetBinError(i,new_error)
  #hout.Print("all")
  return hout

if is_2d and options.do_unrolling:
  x_lines = []
  y_labels = []
  first_hist = True
  # loop over all TH2Ds and for each one unroll to produce TH1D and add to datacard
  directory = outfile.Get(nodename)
  outfile.cd(nodename)
  hists_to_add = []
  for key in directory.GetListOfKeys():
    hist_name = key.GetName()
    hist = directory.Get(hist_name).Clone()

    if not isinstance(hist,ROOT.TDirectory):
      include_of = True
      if 'dijet' in options.cat: include_of = False

      if options.symmetrise: Symmetrise(hist)
      if options.mergeXbins: MergeXBins(hist)

      h1d = UnrollHist2D(hist,include_of)
      hists_to_add.append(h1d)
      if first_hist:
        first_hist=False
        Nxbins = hist.GetNbinsX()
        for i in range(1,hist.GetNbinsY()+1): x_lines.append(Nxbins*i)
        for j in range(1,hist.GetNbinsY()+1): y_labels.append([hist.GetYaxis().GetBinLowEdge(j),hist.GetYaxis().GetBinLowEdge(j+1)])
        if include_of: y_labels.append([hist.GetYaxis().GetBinLowEdge(hist.GetNbinsY()+1),-1])
  for hist in hists_to_add: hist.Write("",ROOT.TObject.kOverwrite)

# sm 3D unrolling
if is_3d and options.do_unrolling:
  x_lines = []
  y_labels = []
  z_labels = []
  first_hist = True
  # loop over all TH3Ds and for each one unroll to produce TH1D and add to datacard
  directory = outfile.Get(nodename)
  outfile.cd(nodename)
  hists_to_add = []
  for key in directory.GetListOfKeys():
    hist_name = key.GetName()
    hist = directory.Get(hist_name).Clone()
    if not isinstance(hist,ROOT.TDirectory):
      include_y_of = False
      include_z_of = True
      h1d = UnrollHist3D(hist,include_y_of,include_z_of)
      hists_to_add.append(h1d)
      if first_hist:
        first_hist=False
        Nxbins = hist.GetNbinsX()
        for i in range(1,hist.GetNbinsY()+1): x_lines.append(Nxbins*i)
        for j in range(1,hist.GetNbinsY()+1): y_labels.append([hist.GetYaxis().GetBinLowEdge(j),hist.GetYaxis().GetBinLowEdge(j+1)])
        if include_y_of: y_labels.append([hist.GetYaxis().GetBinLowEdge(hist.GetNbinsY()+1),-1])
        for j in range(1,hist.GetNbinsZ()+1): z_labels.append([hist.GetZaxis().GetBinLowEdge(j),hist.GetZaxis().GetBinLowEdge(j+1)])
        if include_z_of: z_labels.append([hist.GetZaxis().GetBinLowEdge(hist.GetNbinsZ()+1),-1])
  for hist in hists_to_add: hist.Write("",ROOT.TObject.kOverwrite)

outfile.Close()


if options.plot_from_dc != "": output_name = options.plot_from_dc
plot_file = ROOT.TFile(output_name, 'READ')

if options.auto_rebinning:
  outfile_rebin = ROOT.TFile(output_name.replace(".root","_rebinned.root"), 'RECREATE')
  outfile_rebin.mkdir(nodename)
  outfile_rebin.cd(nodename)
  total_bkghist = plot_file.Get(nodename+'/total_bkg').Clone()
  binning = FindRebinning(total_bkghist,BinThreshold=options.bin_threshold,BinUncertFraction=options.bin_uncert_fraction)
  print "New binning:", binning
  hists_done = []
  for i in  plot_file.Get(nodename).GetListOfKeys():
      if i.GetName() not in hists_done:
        if ".subnodes" not in i.GetName():
          RebinHist(plot_file.Get(nodename+'/'+i.GetName()).Clone(),binning).Write()
          hists_done.append(i.GetName())
  outfile_rebin.Close()
  plot_file = ROOT.TFile(output_name.replace(".root","_rebinned.root"), 'READ')
else:
  plot_file = ROOT.TFile(output_name, 'READ')

if not options.no_plot:
    if options.extra_name != '': vname = options.extra_name
    else: vname = var_name

    if options.datacard != "": 
      if options.extra_name != "":
        plot_name = options.outputfolder+'/'+options.datacard+"_"+options.extra_name
      else:
        plot_name = options.outputfolder+'/'+options.datacard
    else: plot_name = options.outputfolder+'/'+vname+'_'+options.cat+'_'+options.channel+'_'+options.year
    if options.charges_non_zero: plot_name += "_cnz"
    if options.log_x: plot_name += "_logx" 
    if options.log_y: plot_name += "_logy"
    titles = plotting.SetAxisTitles(options.var,options.channel)
    if options.x_title == "": 
      x_title = titles[0]
    else: x_title = options.x_title
    
    if options.y_title == "": 
        y_title = titles[1]
    else: y_title = options.y_title

    plotting.HTTPlot(
      nodename=nodename, 
      infile=plot_file, 
      signal_scale=options.signal_scale,
      norm_bins=options.norm_bins,
      channel=options.channel,
      blind=options.blind,
      x_blind_min=options.x_blind_min,
      x_blind_max=options.x_blind_max,
      ratio=options.ratio,
      log_y=options.log_y,
      log_x=options.log_x,
      ratio_range=options.ratio_range,
      custom_x_range=options.custom_x_range,
      x_axis_min=options.x_axis_min,
      x_axis_max=options.x_axis_max,
      custom_y_range=options.custom_y_range,
      y_axis_max=options.y_axis_max,
      y_axis_min=options.y_axis_min,
      x_title=x_title,
      y_title=y_title,
      lumi=options.lumi,
      plot_name=plot_name,
      cat=options.cat,
      plot_signals=options.plot_signals.split(","),
      draw_data=(not options.no_data)
      )

