import ROOT
import sys
import re
import os
import glob
import json
import itertools
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser
import argparse
import ConfigParser
import UserCode.ICHiggsTauTau.plotting as plotting
from collections import OrderedDict
import copy

CHANNELS= ['tttt','ettt','mttt','tttt','eett','mmtt',"ttt","mmmm"]

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = { 
    "channel":"mmtt" , "outputfolder":"output",
    "folder":"/vols/cms/gu18/Offline/output/4tau/2411_ff_v2" , "signal_folder":"",
    "paramfile":"scripts/params_UL_2018.json", "cat":"inclusive", "year":"2018",
    "campaign":"UL", "sel":"(1)", "set_alias":[], "analysis":"mssm", "var":"m_vis(7,0,140)",
    "method":1, "norm_bins":False, "blind":False, "x_blind_min":100, "x_blind_max":4000, "ratio":False,
    "y_title":"", "x_title":"", "custom_y_range":False, "y_axis_min":0.001,
    "y_axis_max":100,"custom_x_range":False, "x_axis_min":0.001, "x_axis_max":100, "log_x":False,
    "log_y":False, "extra_pad":0.0, "signal_scale":1, "lumi":"12.9 fb^{-1} (13 TeV)", "no_plot":False,
    "ratio_range":"0.7,1.3", "datacard":"", "add_wt":"","do_unrolling":True, "extra_name":"",
    "symmetrise":False, "mergeXbins":False, "threePads":False,"auto_blind":False,"plot_signals":""
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
parser.add_argument("--data_folder", dest="data_folder", type=str,
    help="If specified will use as input folder for data samples, else will use same directroy specified by \"folder\" option.")
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
parser.add_argument("--vsjets", default="loose", type=str,
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
parser.add_argument("--replace_name", default=None, type=str,
      help="Extra name for plot")
parser.add_argument("--no_signal", dest="no_signal", action='store_true',
    help="Do not run the signal samples.")
parser.add_argument("--ff_from", default="all", type=str,
      help="Which tau to use the ff from")
parser.add_argument("--aiso", default="", type=str,
      help="Which tau to ant isolate")
parser.add_argument("--aiso_and_iso", default="", type=str,
      help="Which tau to use full phase space")
parser.add_argument("--under_legend", default="", type=str,
      help="Text to go underneath the legend")
parser.add_argument("--no_charge_sel", dest="no_charge_sel", action='store_true',
    help="Run with no charge sel.")
parser.add_argument("--no_sig_sel", dest="no_sig_sel", action='store_true',
    help="Use all phase space except signal region.")
parser.add_argument("--do_ff_systs", dest="do_ff_systs", action='store_true',
    help="Do fake factor systamatic shifts.")
parser.add_argument("--add_stat_to_syst", dest="add_stat_to_syst", action='store_true',
    help="Add custom uncertainty band to statistical uncertainty.")
parser.add_argument("--syst_tau_id", dest="syst_tau_id", action='store_true',
    help="Run tau id systematic uncertainty")
parser.add_argument("--syst_tau_id_new", dest="syst_tau_id_new", action='store_true',
    help="Run tau id systematic uncertainty")
parser.add_argument("--syst_etau_fakerate", dest="syst_etau_fakerate", action='store_true',
    help="Run etau fakerate systematic uncertainty")
parser.add_argument("--syst_mtau_fakerate", dest="syst_mtau_fakerate", action='store_true',
    help="Run mtau fakerate systematic uncertainty")
parser.add_argument("--syst_doubletau_trg", dest="syst_doubletau_trg", action='store_true',
    help="Run doubletau trigger systematic uncertainty")
parser.add_argument("--syst_prefire", dest="syst_prefire", action='store_true',
    help="Run prefire systematic uncertainty")
parser.add_argument("--syst_tau_scale", dest="syst_tau_scale", action='store_true',
    help="Run tau energy scale systematic uncertainty")
parser.add_argument("--syst_tau_scale_group", dest="syst_tau_scale_group", action='store_true',
    help="Run tau grouped energy scale systematic uncertainties")
parser.add_argument("--syst_jet_scale_group", dest="syst_jet_scale_group", action='store_true',
    help="Run jet grouped energy scale systematic uncertainties")
parser.add_argument("--syst_electron_scale", dest="syst_electron_scale", action='store_true',
    help="Run electron energy scale systematic uncertainty")
parser.add_argument("--syst_efake_scale_0pi", dest="syst_efake_scale_0pi", action='store_true',
    help="Run efake 0pi energy scale systematic uncertainty")
parser.add_argument("--syst_efake_scale_1pi", dest="syst_efake_scale_1pi", action='store_true',
    help="Run efake 1pi energy scale systematic uncertainty")
parser.add_argument("--syst_muon_scale", dest="syst_muon_scale", action='store_true',
    help="Run muon energy scale systematic uncertainty")
parser.add_argument("--syst_mufake_scale_0pi", dest="syst_mufake_scale_0pi", action='store_true',
    help="Run mufake 0pi energy scale systematic uncertainty")
parser.add_argument("--syst_mufake_scale_1pi", dest="syst_mufake_scale_1pi", action='store_true',
    help="Run mufake 1pi energy scale systematic uncertainty")
parser.add_argument("--syst_jet_res", dest="syst_jet_res", action='store_true',
    help="Run jet energy resolution systematic uncertainty")
parser.add_argument("--syst_met_unclustered", dest="syst_met_unclustered", action='store_true',
    help="Run MET unclustered systematic uncertainty")
parser.add_argument("--syst_met_scale", dest="syst_met_scale", action='store_true',
    help="Run MET energy scale systematic uncertainty")
parser.add_argument("--syst_met_res", dest="syst_met_res", action='store_true',
    help="Run MET energy resolution systematic uncertainty")
parser.add_argument("--syst_electron_id", dest="syst_electron_id", action='store_true',
    help="Run systematic variations for electron id")
parser.add_argument("--syst_electron_trg", dest="syst_electron_trg", action='store_true',
    help="Run systematic variations for electron trigger")
parser.add_argument("--syst_muon_id", dest="syst_muon_id", action='store_true',
    help="Run systematic variations for muon id")
parser.add_argument("--syst_muon_trg", dest="syst_muon_trg", action='store_true',
    help="Run systematic variations for muon trigger")
parser.add_argument("--syst_signal_theory", dest="syst_signal_theory", action='store_true',
    help="Run systematic variations for the theory shifts on the signal")
parser.add_argument("--syst_k_factor", dest="syst_k_factor", action='store_true',
    help="Run systematic variations for the ZZto4L k factors")
parser.add_argument("--rebin_with_data", dest="rebin_with_data", action='store_true',
    help="Use data in the rebinning algorithm")
parser.add_argument("--symmetrise_uncertainty", dest="symmetrise_uncertainty", action='store_true',
    help="Symmetrise uncertainties in dictionary defined in code.")
parser.add_argument("--shrink_final_bin", dest="shrink_final_bin", action='store_true',
    help="Shrink the size of the final bin.")
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
if options.no_sig_sel and options.ff_from == "all":
  print "ERROR: Need to specify ff_from when using no_sig_sel"
  exit()

cats = {}

VsJets_wp_fail = options.vsjets_fail
VsJets_wp = options.vsjets
VsMu_wp = options.vsmu
VsEle_wp = options.vsele
lepton_iso = "0.15"
if options.channel == "mmmm": lepton_iso = "0.35"

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

if options.no_sig_sel:
  options.charges_non_zero = False
  options.no_charge_sel = True


if options.charges_non_zero: 
	charge_sel = "(q_1+q_2+q_3+q_4)!=0"
elif options.no_charge_sel:
  charge_sel = "(1)"
else:
	charge_sel = "(q_1+q_2+q_3+q_4)==0"

if options.channel == 'ttt':
  if options.charges_non_zero:
    charge_sel = "fabs(q_1+q_2+q_3)!=1"
  elif options.no_charge_sel:
    charge_sel = "(1)"
  else:
    charge_sel = "fabs(q_1+q_2+q_3)==1"

tpt = "40"
if options.year == "2016_preVFP" or options.year == "2016_postVFP":
  mpt = "23"
  ept = "26"
elif options.year == "2017":
  mpt = "25"
  ept = "28"
elif options.year == "2018":
  mpt = "25"
  ept = "33"


if options.channel == "tttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{tpt} && pt_2>{tpt} && trg_doubletau_12) || (pt_1>{tpt} && pt_3>{tpt} && trg_doubletau_13) || (pt_1>{tpt} && pt_4>{tpt} && trg_doubletau_14) || (pt_2>{tpt} && pt_3>{tpt} && trg_doubletau_23) || (pt_2>{tpt} && pt_4>{tpt} && trg_doubletau_24) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))".format(tpt=tpt)
  cats['data_veto'] = "(1)"
if options.channel == "ttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"))
  cats['trigger'] = "((pt_1>{tpt} && pt_2>{tpt} && trg_doubletau_12) || (pt_1>{tpt} && pt_3>{tpt} && trg_doubletau_13) || (pt_2>{tpt} && pt_3>{tpt} && trg_doubletau_23))".format(tpt=tpt)
  cats['data_veto'] = "(1)"
elif options.channel == "ettt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{ept} && trg_singleelectron_1) || (pt_2>{tpt} && pt_3>{tpt} && trg_doubletau_23) || (pt_2>{tpt} && pt_4>{tpt} && trg_doubletau_24) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))".format(tpt=tpt,ept=ept)
  cats['data_veto'] = "!(isTau && (trg_singleelectron_1))"
elif options.channel == "mttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{mpt} && trg_singlemuon_1) || (pt_2>{tpt} && pt_3>{tpt} && trg_doubletau_23) || (pt_2>{tpt} && pt_4>{tpt} && trg_doubletau_24) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))".format(tpt=tpt,mpt=mpt)
  cats['data_veto'] = "!(isTau && (pt_1>{mpt} && trg_singlemuon_1))".format(mpt=mpt)
elif options.channel == "emtt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{ept} && trg_singleelectron_1) || (pt_2>{mpt} && trg_singlemuon_2) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))".format(ept=ept,mpt=mpt,tpt=tpt)
  cats['data_veto'] = "(!(isTau && (pt_2>{mpt} && trg_singlemuon_2))) && (!(isSingleElectron && ((pt_1>{mpt} && trg_singlemuon_1) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))))".format(mpt=mpt,tpt=tpt)
elif options.channel == "mmtt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{mpt} && trg_singlemuon_1) || (pt_2>{mpt} && trg_singlemuon_2) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))".format(mpt=mpt,tpt=tpt)
  cats['data_veto'] = "!(isTau && ((pt_1>{mpt} && trg_singlemuon_1) || (pt_2>{mpt} && trg_singlemuon_2)))".format(mpt=mpt)
elif options.channel == "eett":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=e_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{ept} && trg_singleelectron_1) || (pt_2>{ept} && trg_singleelectron_2) || (pt_3>{tpt} && pt_4>{tpt} && trg_doubletau_34))".format(ept=ept,tpt=tpt)
  cats['data_veto'] = "!(isTau && ((pt_1>{ept} && trg_singleelectron_1) || (pt_2>{ept} && trg_singleelectron_2)))".format(ept=ept)
elif options.channel == "mmmm":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=m_sel.replace("X","3"),sel_4=m_sel.replace("X","4"))
  cats['trigger'] = "((pt_1>{mpt} && trg_singlemuon_1) || (pt_2>{mpt} && trg_singlemuon_2) || (pt_3>{mpt} && trg_singlemuon_3) || (pt_4>{mpt} && trg_singlemuon_4))".format(mpt=mpt)
  cats['data_veto'] = "(1)"


if options.aiso != "":
  for X in options.aiso:
    cats['baseline'] = cats['baseline'].replace("deepTauVsJets_%(VsJets_wp)s_%(X)s>0.5" % vars(), "(deepTauVsJets_%(VsJets_wp)s_%(X)s<0.5 && deepTauVsJets_iso_%(X)s>0.1)" % vars() )

if options.aiso_and_iso != "":
  for X in options.aiso_and_iso:
    cats['baseline'] = cats['baseline'].replace("deepTauVsJets_%(VsJets_wp)s_%(X)s>0.5" % vars(), "(deepTauVsJets_iso_%(X)s>0.1)" % vars() )

if options.no_sig_sel:
  remove = ""
  for ind, s in enumerate(options.channel):
    if s == "t": remove += str(ind+1)

  for i in options.ff_from: remove = remove.replace(i,"")
  no_sig = "(!((q_1+q_2+q_3+q_4)==0"
  for X in remove:
    no_sig += " && deepTauVsJets_%(VsJets_wp)s_%(X)s>0.5" % vars()
    cats['baseline'] = cats['baseline'].replace("deepTauVsJets_%(VsJets_wp)s_%(X)s>0.5" % vars(), "(deepTauVsJets_iso_%(X)s>0.1)" % vars() )
  no_sig += "))"
  cats['baseline'] = "("+cats['baseline']+"*"+no_sig+")"

cats['baseline'] = "(({charge_sel}) && {current_baseline} && {trigger_sel})".format(charge_sel=charge_sel,current_baseline=cats['baseline'],trigger_sel=cats['trigger'])

cats['inclusive'] = '(1)' 
cats['nobtag'] = '(n_bjets==0)'
cats['mvis2_0-100'] = '(mvis_min_sum_dR_2 > 0) && (mvis_min_sum_dR_2 < 100)'
cats['mvis2_100-200'] = '(mvis_min_sum_dR_2 > 100) && (mvis_min_sum_dR_2 < 200)'
cats['mvis2_200-500'] = '(mvis_min_sum_dR_2 > 200) && (mvis_min_sum_dR_2 < 500)'
cats['z_control'] = '(q_1==-q_2)'
cats['2l2t_sig'] = '(q_1==q_2)'

cats['btag'] = '(n_bjets>=1)'
cats['z_control_nobtag'] = '((q_1==-q_2) && (n_bjets==0))'
cats['2l2t_sig_nobtag'] = '((q_1==q_2) && (n_bjets==0))'
cats['z_control_btag'] = '((q_1==-q_2) && (n_bjets>0))'
cats['2l2t_sig_btag'] = '((q_1==q_2) && (n_bjets>0))'

cats['ettt_check'] = '(((q_1==-q_2) && (deepTauVsEle_loose_2>0.5)) || ((q_1==-q_3) && (deepTauVsEle_loose_3>0.5)) || ((q_1==-q_4) && (deepTauVsEle_loose_4>0.5)))'
cats['mttt_check'] = '(((q_1==-q_2) && (deepTauVsMu_loose_2>0.5)) || ((q_1==-q_3) && (deepTauVsMu_loose_3>0.5)) || ((q_1==-q_4) && (deepTauVsMu_loose_4>0.5)))'


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
	if options.year == "2016_preVFP":
		data_samples = ['TauB','TauC','TauD','TauE','TauF']
	elif options.year == "2016_postVFP":
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

elif options.channel == 'mmmm':
  if options.year == "2016_preVFP":
    data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
  elif options.year == "2016_postVFP":
    data_samples = ['SingleMuonF','SingleMuonG','SingleMuonH']
  elif options.year == "2017":
    data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
  elif options.year == "2018":
    data_samples = ['SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD']

def ReformatSignals(signals,do_format):
  rf_signals = []
  for signal in signals:
    if do_format==True:
      split = re.split('(\d+)', signal)
      reformat = split[2]+split[3]+split[0][-3:]+split[1]
      rf_signals.append([signal,reformat])
    else:
      rf_signals.append([signal,signal])
  return rf_signals

sig_samples=[]
grid_phi = ["100","110","125","140","160","180","200","250","300"]
grid_A = ["60","70","80","90","100","125","140","160"]
for mp in grid_phi:
  for mA in grid_A:
       sig_samples.append("ZstarTophi{}A{}To4Tau".format(mp,mA))

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
      ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO','DYJetsToLL-LO-ext1']
      top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
      vv_samples = ['WZTo1L1Nu2Q','WZTo3LNu','WWTo1L1Nu2Q','WWTo2L2Nu','ZZTo2L2Nu','Tbar-t','Tbar-tW','T-t','T-tW']
      vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
      wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
      wgam_samples = ['WGToLNuG']
      ewkz_samples = ['EWKZ2Jets_ZToLL']
      qqzz_samples = ['ZZTo4L']
      higgs_samples = ['GluGluHToTauTau_M125','VBFHToTauTau_M125','WminusHToTauTau_M125','WplusHToTauTau_M125','ZHToTauTau_M125','ZHToTauTau_M125-ext1']
      hzz_samples = ['VBF_HToZZTo4L_M125','GluGlu_HToZZTo4L_M125']
      ggzz_samples = ['GluGluToContinToZZTo2e2mu','GluGluToContinToZZTo2e2tau','GluGluToContinToZZTo2mu2tau','GluGluToContinToZZTo4e','GluGluToContinToZZTo4mu','GluGluToContinToZZTo4tau']
      #signal_samples = ["phi200A100To4Tau","phi200A20To4Tau","phi300A20To4Tau","phi100A150To4Tau","phi300A150To4Tau","phi300A60To4Tau","phi100A100To4Tau","phi200A60To4Tau","phi300A100To4Tau","phi100A60To4Tau","phi200A150To4Tau"]
      signal_samples = sig_samples
      signal_samples = ReformatSignals(signal_samples,True)


if options.year == "2017":
  if options.campaign == "ReReco":
    ztt_samples = ['DYJetsToLL-LO','DYJetsToLL-LO-ext1','DY1JetsToLL-LO','DY1JetsToLL-LO-ext','DY2JetsToLL-LO','DY2JetsToLL-LO-ext','DY3JetsToLL-LO','DY3JetsToLL-LO-ext','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','DYJetsToLL_M-10-50-LO-ext1']
    top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
    vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WZTo2L2Q','WZTo3LNu', 'ZZTo2L2Q','ZZTo4L-ext','ZZTo4L','VVTo2L2Nu']
    vvv_samples = []
    wjets_samples = ['WJetsToLNu-LO','WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
    wgam_samples = ['WGToLNuG']
    ewkz_samples = ['EWKZ2Jets']
    hzz_samples = ['VBF_HToZZTo4L_M125','GluGlu_HToZZTo4L_M125']
    signal_samples = []
  elif options.campaign == "UL":
    ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO','DYJetsToLL-LO-ext1']
    top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
    vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu']
    vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
    wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
    wgam_samples = ['WGToLNuG']
    ewkz_samples = ['EWKZ2Jets_ZToLL']
    qqzz_samples = ['ZZTo4L']
    #hzz_samples = ['VBF_HToZZTo4L_M125','GluGlu_HToZZTo4L_M125','ZHToTauTau_M125']
    hzz_samples = ['VBF_HToZZTo4L_M125','GluGlu_HToZZTo4L_M125']
    higgs_samples = ['GluGluHToTauTau_M125','VBFHToTauTau_M125','WminusHToTauTau_M125','WplusHToTauTau_M125','ZHToTauTau_M125']
    ggzz_samples = ['GluGluToContinToZZTo2e2mu','GluGluToContinToZZTo2e2tau','GluGluToContinToZZTo2mu2tau','GluGluToContinToZZTo4e','GluGluToContinToZZTo4mu','GluGluToContinToZZTo4tau']
    signal_samples = sig_samples
    signal_samples = ReformatSignals(signal_samples,True)


if options.year == "2016_postVFP":
  ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO']
  top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
  vv_samples = ['WZTo3LNu','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','T-tW', 'Tbar-tW','Tbar-t','T-t']
  vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
  wgam_samples = ['WGToLNuG']
  wjets_samples = ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
  qqzz_samples = ['ZZTo4L']
  higgs_samples = ['GluGluHToTauTau_M125','VBFHToTauTau_M125','WminusHToTauTau_M125','WplusHToTauTau_M125','ZHToTauTau_M125']
  hzz_samples = ['VBF_HToZZTo4L_M125','GluGlu_HToZZTo4L_M125']
  ggzz_samples = ['GluGluToContinToZZTo2e2mu','GluGluToContinToZZTo2e2tau','GluGluToContinToZZTo2mu2tau','GluGluToContinToZZTo4e','GluGluToContinToZZTo4mu','GluGluToContinToZZTo4tau']
  ewkz_samples = ['EWKZ2Jets_ZToLL']
  signal_samples = sig_samples
  signal_samples = ReformatSignals(signal_samples,True)


if options.year == "2016_preVFP":
  ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO']
  top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
  vv_samples = ['WZTo1L1Nu2Q','WZTo3LNu','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','T-tW', 'Tbar-tW','Tbar-t','T-t']
  vvv_samples = ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1']
  wjets_samples = ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu']
  qqzz_samples = ['ZZTo4L']
  higgs_samples = ['GluGluHToTauTau_M125','VBFHToTauTau_M125','WminusHToTauTau_M125','WplusHToTauTau_M125','ZHToTauTau_M125']
  hzz_samples = ['VBF_HToZZTo4L_M125','GluGlu_HToZZTo4L_M125']
  ggzz_samples = ['GluGluToContinToZZTo2e2mu','GluGluToContinToZZTo2e2tau','GluGluToContinToZZTo2mu2tau','GluGluToContinToZZTo4e','GluGluToContinToZZTo4mu','GluGluToContinToZZTo4tau']
  wgam_samples = ['WGToLNuG']
  ewkz_samples = ['EWKZ2Jets_ZToLL']
  signal_samples = sig_samples
  signal_samples = ReformatSignals(signal_samples,True)


if options.year == "2016":
  ztt_samples = ['DYJetsToLL-LO-ext1','DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
  vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','VVTo2L2Nu-ext1','ZZTo2L2Q','ZZTo4L-amcat','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q']
  vvv_samples = []
  wgam_samples = ['WGToLNuG-ext1','WGToLNuG-ext2','WGToLNuG-ext3']
  wjets_samples = ['WJetsToLNu-LO', 'WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W2JetsToLNu-LO-ext','W3JetsToLNu-LO','W3JetsToLNu-LO-ext','W4JetsToLNu-LO','W4JetsToLNu-LO-ext1','W4JetsToLNu-LO-ext2']
  ewkz_samples = ['EWKZ2Jets_ZToLL','EWKZ2Jets_ZToLL-ext1','EWKZ2Jets_ZToLL-ext2']
  signal_samples = []

#vv_samples = ["ZZTo4L"]

ROOT.TH1.SetDefaultSumw2(True)

correct_gen_matches = []
lepton_gen_matches = []
tau_gen_matches = []
t_gen_matches = []
e_gen_matches = []
m_gen_matches = []
lfake_gen_matches = []
jfake_gen_matches = []

for in0,ch in enumerate(options.channel):
  ind = in0+1
  if ch == "e":
    correct_gen_matches.append("(gen_match_%(ind)i==1 || gen_match_%(ind)i==3)" % vars()) 
    lepton_gen_matches.append(correct_gen_matches[-1])
  elif ch == "m":
    correct_gen_matches.append("(gen_match_%(ind)i==2 || gen_match_%(ind)i==4)" % vars())
    lepton_gen_matches.append(correct_gen_matches[-1])
  elif ch == "t":
    correct_gen_matches.append("(gen_match_%(ind)i==5)" % vars())
    tau_gen_matches.append(correct_gen_matches[-1])
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
              "LF":"({lfgm} > 0) && ({jfgm} == 0)".format(lfgm=summed_lfake_gen_matches,jfgm=summed_jfake_gen_matches),
              #"JFL":"((({jfl})==0) && ({gt}))".format(jfl=" && ".join(lepton_gen_matches),gt=" && ".join(tau_gen_matches))
              #"inclusive": "(1)",
              }

if options.channel.count("t") == 0: 
  other_sels["R"] = "(1)"
  del other_sels["JF"]
  del other_sels["LF"]
  del other_sels["JLF"]

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

def GenerateHZZ(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', hzz_sels={}):
    hzz_node = GetNode(ana, "HZZ", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(hzz_node)
    
def GenerateGGZZ(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', ggzz_sels={}):
    ggzz_node = GetNode(ana, "ggZZ", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(ggzz_node)

def GenerateQQZZ(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', qqzz_sels={}):
    qqzz_node = GetNode(ana, "qqZZ", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(qqzz_node)

def GenerateHiggs(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', qqzz_sels={}):
    higgs_node = GetNode(ana, "Higgs", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(higgs_node)

def GenerateMCFakeTaus(ana, ff_from, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    total_sel = []
    for tau in ff_from:
      ff_not_from = [ind+1 for ind, ch in enumerate(options.channel) if (ch == "t" and str(ind+1) != tau)]
      fake_sel = ["(gen_match_"+str(i)+"==6)" for i in ff_not_from] 
      total_sel.append("((gen_match_"+str(tau)+"!=6)&&("+"||".join(fake_sel)+"))")
    mc_sel = "("+"||".join(total_sel)+")"
    mc_jetfakes_node = GetNode(ana, "MC_jetFakes", add_name, samples, plot, wt, mc_sel, cat)
    ana.nodes[nodename].AddNode(mc_jetfakes_node)


def GenerateFakeTaus(ana, add_name='', data_samples=[], mc_samples=[], plot='', wt='', sel='', cat='', charges_non_zero=False, data_veto=None, wt_ext="", type_ext="", intermediate_shift=[], spec_wt_ext=None):

  vj = "deepTauVsJets_" + VsJets_wp
  ff_sel = copy.deepcopy(cat)
  replace = "1234"
  if options.ff_from != "all": replace = options.ff_from
  for i in replace:
    ff_sel = ff_sel.replace("%(vj)s_%(i)s>0.5" % vars(),"1")

  pass_sel = "({vjn}_X>0.5)".format(vjn=vj,ind=i)
  fail_sel = "({vjn}_X<0.5 && deepTauVsJets_iso_X>0.1)".format(vjn=vj)

  tau_numbers = [i+1 for i in range(0,len(options.channel)) if options.channel[i] == "t"] # get position of hadronic taus in channel i.e emtt = [3,4]
  for ind, n_ff in enumerate(range(1,options.channel.count("t")+1)): # loop through number of hadronic taus in the channel
    for aiso_tau_numbers in list(itertools.combinations(tau_numbers, n_ff)): # loop through different combinations of taus to anti-isolate

      name = "".join([str(i) for i in aiso_tau_numbers])

      if options.ff_from == "all" or options.ff_from == name:

        sign = "1.0" if ((ind % 2) == 0 or options.ff_from != "all") else "-1.0"
        if options.channel == "tttt":
          sign = str(float(sign) * -1)
          if len(name) == 1: 
            continue
          elif len(name) == 2:
            sign = "1.0"
          elif len(name) == 3:
            sign = "-2.0"
          elif len(name) == 4:
            sign = "3.0"


        iso_tau_numbers = [i for i in tau_numbers if i not in aiso_tau_numbers] # get isolated taus
        
        # get selections     
        ff_wt_list = []
        run = True
        for i in aiso_tau_numbers:

          if spec_wt_ext == None:
            ff_wt_list.append("wt_ff_ml_{}{}".format(i,wt_ext))
          elif spec_wt_ext[0] == name:
            if spec_wt_ext[1] == "wt_ff_ml_{}".format(i):
              ff_wt_list.append("wt_ff_ml_{}{}".format(i,wt_ext))
            else:
              ff_wt_list.append("wt_ff_ml_{}".format(i))
          else:
            run = False

        if not run: continue

        total_ff_wt = " * ".join(ff_wt_list)
        total_gen_match_sel = " * ".join(["(gen_match_{}!=6)".format(i) for i in aiso_tau_numbers])
        total_fail = " * ".join(["({}_{}<0.5 && deepTauVsJets_iso_{}>0.1)".format(vj,i,i) for i in aiso_tau_numbers])
        total_pass = " * ".join(["({}_{}>0.5)".format(vj,i) for i in iso_tau_numbers if (str(i) not in options.aiso and str(i) not in options.aiso_and_iso and not options.no_sig_sel)])
   
        # remove empty selections
        ff_data_wt_list = [sign,total_ff_wt,total_fail,total_pass]
        #ff_data_wt_list = ["0.08824675003743619",sign,total_fail,total_pass]
        #ff_data_wt_list = [sign,total_fail,total_pass]
        if "" in ff_data_wt_list: ff_data_wt_list.remove("")
        ff_mc_wt_list = [sign,"wt",total_ff_wt,total_gen_match_sel,total_fail,total_pass]
        #ff_mc_wt_list = ["0.08824675003743619",sign,"wt",total_gen_match_sel,total_fail,total_pass]
        #ff_mc_wt_list = [sign,"0",total_gen_match_sel,total_fail,total_pass]
        #ff_mc_wt_list = [sign,"wt",total_gen_match_sel,total_fail,total_pass]

        if "" in ff_mc_wt_list: ff_mc_wt_list.remove("")
  
        # make full selections
        ff_data_wt = " * ".join(ff_data_wt_list)
        ff_mc_wt = " * ".join(ff_mc_wt_list)

        # add constant uncerts
        add_name_dict = {add_name:"1"}

        #print add_name_dict
        for an, scale in add_name_dict.items():
          # decorrelate between channels
          if an != "":
            if options.channel != "tttt":
              add_name_channel = "_" + options.channel + an
            else:
              add_name_channel = "_ttt" + an
          else:
            add_name_channel = ""

          if data_veto == None:
            ff_data = GetNode(ana, 'jetFakes{}'.format(name), add_name_channel, data_samples, plot, "("+scale+")*("+ff_data_wt+")", sel, ff_sel)
            ff_mc = GetNode(ana, 'jetFakes{}_subtract'.format(name), add_name_channel, mc_samples, plot, "("+scale+")*("+ff_mc_wt+")", sel, ff_sel)
          else:
            ff_data = GetNode(ana, 'jetFakes{}'.format(name), add_name_channel, data_samples, plot, "("+scale+")*("+ff_data_wt+")", sel, "("+ff_sel+")&&("+data_veto+")")
            ff_mc = GetNode(ana, 'jetFakes{}_subtract'.format(name), add_name_channel, mc_samples, plot, "("+scale+")*("+ff_mc_wt+")", sel, "("+ff_sel+")&&("+data_veto+")")

          ana.nodes[nodename].AddNode(SubtractNode('jetFakes{}'.format(name)+add_name_channel, ff_data, ff_mc))  

#def GenerateFakeTaus(ana, add_name='', data_samples=[], mc_samples=[], plot='', wt='', sel='', cat='', charges_non_zero=False, data_veto=None,wt_ext="",type_ext="", intermediate_shift=[]):
#  vj = "deepTauVsJets_" + VsJets_wp
#  if VsJets_wp_fail == None:
#    vjf = "1"
#  else:
#    vjf = "deepTauVsJets_" + VsJets_wp_fail
# 
#  ff_sel = copy.deepcopy(cat)
#  replace = "1234"
#  if options.ff_from != "all": replace = options.ff_from
#  pass_sel = {}
#  fail_sel = {}
#
#   
#  s_wt_ext = ""
#  d_wt_ext = ""
#  t_wt_ext = ""
#  q_wt_ext = ""
#  
#  if type_ext == "single":
#    s_wt_ext = wt_ext
#  elif type_ext == "double":
#    d_wt_ext = wt_ext
#  elif type_ext == "triple":
#    t_wt_ext = wt_ext
#  elif type_ext == "quadruple":
#    q_wt_ext = wt_ext
#  elif type_ext == "all":
#    s_wt_ext = wt_ext
#    d_wt_ext = wt_ext
#    t_wt_ext = wt_ext
#    q_wt_ext = wt_ext
#
#  for i in replace:
#    ff_sel = ff_sel.replace("%(vj)s_%(i)s>0.5" % vars(),"1")
#    pass_sel[i] = "({vjn}_{ind}>0.5)".format(vjn=vj,ind=i)
#    fail_sel[i] = "({vjn}_{ind}<0.5 && deepTauVsJets_iso_{ind}>0.1)".format(vjn=vj,ind=i)
#   
#  if options.ff_from == "all":
#    if options.channel in ["emtt","eett","mmtt"]:
#      ff_wt = ("((wt_ff_ml_3{s_ext} * GM3 * {fail_3} * {pass_4})" 
#              "+ (wt_ff_ml_4{s_ext} * GM4 * {pass_3} * {fail_4})"  
#              "- (wt_ff_ml_3{d_ext} * wt_ff_ml_4{d_ext} * GM3 * GM4 * {fail_3} * {fail_4}))").format(fail_3=fail_sel["3"],fail_4=fail_sel["4"],pass_3=pass_sel["3"],pass_4=pass_sel["4"],s_ext=s_wt_ext,d_ext=d_wt_ext)
#    elif options.channel in ["ettt","mttt"]:
#      ff_wt = ("((wt_ff_ml_2{s_ext} * GM2 * {fail_2} * {pass_3} * {pass_4})"
#              "+ (wt_ff_ml_3{s_ext} * GM3 * {pass_2} * {fail_3} * {pass_4})" 
#              "+ (wt_ff_ml_4{s_ext} * GM4 * {pass_2} * {pass_3} * {fail_4})" 
#              "- (wt_ff_ml_2{d_ext} * wt_ff_ml_3{d_ext} * GM2 * GM3 * {fail_2} * {fail_3} * {pass_4})"
#              "- (wt_ff_ml_2{d_ext} * wt_ff_ml_4{d_ext} * GM2 * GM4 * {fail_2} * {pass_3} * {fail_4})"
#              "- (wt_ff_ml_3{d_ext} * wt_ff_ml_4{d_ext} * GM3 * GM4 * {pass_2} * {fail_3} * {fail_4})"
#              "+ (wt_ff_ml_2{t_ext} * wt_ff_ml_3{t_ext} * wt_ff_ml_4{t_ext} * GM2 * GM3 * GM4 * {fail_2} * {fail_3} * {fail_4}))").format(fail_2=fail_sel["2"],fail_3=fail_sel["3"],fail_4=fail_sel["4"],pass_2=pass_sel["2"],pass_3=pass_sel["3"],pass_4=pass_sel["4"],s_ext=s_wt_ext,d_ext=d_wt_ext,t_ext=t_wt_ext)
#    elif options.channel in ["ttt"]:
#      ff_wt = ("((wt_ff_ml_1{s_ext} * GM1 * {fail_1} * {pass_2} * {pass_3})"
#              "+ (wt_ff_ml_2{s_ext} * GM2 * {pass_1} * {fail_2} * {pass_3})"
#              "+ (wt_ff_ml_3{s_ext} * GM3 * {pass_1} * {pass_2} * {fail_3})"
#              "- (wt_ff_ml_1{d_ext} * wt_ff_ml_2{d_ext} * GM1 * GM2 * {fail_1} * {fail_2} * {pass_3})"
#              "- (wt_ff_ml_1{d_ext} * wt_ff_ml_3{d_ext} * GM1 * GM3 * {fail_1} * {pass_2} * {fail_3})"
#              "- (wt_ff_ml_2{d_ext} * wt_ff_ml_3{d_ext} * GM2 * GM3 * {pass_1} * {fail_2} * {fail_3})"
#              "+ (wt_ff_ml_1{t_ext} * wt_ff_ml_2{t_ext} * wt_ff_ml_3{t_ext} * GM1 * GM2 * GM3 * {fail_1} * {fail_2} * {fail_3}))").format(fail_1=fail_sel["1"],fail_2=fail_sel["2"],fail_3=fail_sel["3"],pass_1=pass_sel["1"],pass_2=pass_sel["2"],pass_3=pass_sel["3"],s_ext=s_wt_ext,d_ext=d_wt_ext,t_ext=t_wt_ext)
#    elif options.channel in ["tttt"]:
#      ff_wt = ("((wt_ff_ml_1{s_ext} * GM1 * {fail_1} * {pass_2} * {pass_3} * {pass_4})" 
#              "+ (wt_ff_ml_2{s_ext} * GM2 * {pass_1} * {fail_2} * {pass_3} * {pass_4})"
#              "+ (wt_ff_ml_3{s_ext} * GM3 * {pass_1} * {pass_2} * {fail_3} * {pass_4})"
#              "+ (wt_ff_ml_4{s_ext} * GM4 * {pass_1} * {pass_2} * {pass_3} * {fail_4})"
#              "- (wt_ff_ml_1{d_ext} * wt_ff_ml_2{d_ext} * GM1 * GM2 * {fail_1} * {fail_2} * {pass_3} * {pass_4})"
#              "- (wt_ff_ml_1{d_ext} * wt_ff_ml_3{d_ext} * GM1 * GM3 * {fail_1} * {pass_2} * {fail_3} * {pass_4})"
#              "- (wt_ff_ml_1{d_ext} * wt_ff_ml_4{d_ext} * GM1 * GM4 * {fail_1} * {pass_2} * {pass_3} * {fail_4})"
#              "- (wt_ff_ml_2{d_ext} * wt_ff_ml_3{d_ext} * GM2 * GM3 * {pass_1} * {fail_2} * {fail_3} * {pass_4})"
#              "- (wt_ff_ml_2{d_ext} * wt_ff_ml_4{d_ext} * GM2 * GM4 * {pass_1} * {fail_2} * {pass_3} * {fail_4})"
#              "- (wt_ff_ml_3{d_ext} * wt_ff_ml_4{d_ext} * GM3 * GM4 * {pass_1} * {pass_2} * {fail_3} * {fail_4})"
#              "+ (wt_ff_ml_1{t_ext} * wt_ff_ml_2{t_ext} * wt_ff_ml_3{t_ext} * GM1 * GM2 * GM3 * {fail_1} * {fail_2} * {fail_3} * {pass_4})"
#              "+ (wt_ff_ml_1{t_ext} * wt_ff_ml_2{t_ext} * wt_ff_ml_4{t_ext} * GM1 * GM2 * GM4 * {fail_1} * {fail_2} * {pass_3} * {fail_4})"
#              "+ (wt_ff_ml_1{t_ext} * wt_ff_ml_3{t_ext} * wt_ff_ml_4{t_ext} * GM1 * GM3 * GM4 * {fail_1} * {pass_2} * {fail_3} * {fail_4})"
#              "+ (wt_ff_ml_2{t_ext} * wt_ff_ml_3{t_ext} * wt_ff_ml_4{t_ext} * GM2 * GM3 * GM4 * {pass_1} * {fail_2} * {fail_3} * {fail_4})"
#              "- (wt_ff_ml_1{q_ext} * wt_ff_ml_2{q_ext} * wt_ff_ml_3{q_ext} * wt_ff_ml_4{q_ext} * GM1 * GM2 * GM3 * GM4 * {fail_1} * {fail_2} * {fail_3} * {fail_4}))").format(fail_1=fail_sel["1"],fail_2=fail_sel["2"],fail_3=fail_sel["3"],fail_4=fail_sel["4"],pass_1=pass_sel["1"],pass_2=pass_sel["2"],pass_3=pass_sel["3"],pass_4=pass_sel["4"],s_ext=s_wt_ext,d_ext=d_wt_ext,t_ext=t_wt_ext,q_ext=q_wt_ext)
#  elif len(options.ff_from) == 1:
#    ff_wt = "(wt_ff_ml_{} * GM{} * {})".format(options.ff_from+wt_ext,options.ff_from,fail_sel[options.ff_from])
#  elif len(options.ff_from) == 2:
#    ff_wt = "(wt_ff_ml_{} * wt_ff_ml_{} * GM{} * GM{} * {} * {})".format(options.ff_from[0]+wt_ext,options.ff_from[1]+wt_ext,options.ff_from[0],options.ff_from[1],fail_sel[options.ff_from[0]],fail_sel[options.ff_from[1]])
#  elif len(options.ff_from) == 3:
#    ff_wt = "(wt_ff_ml_{} * wt_ff_ml_{} * wt_ff_ml_{} * GM{} * GM{} * GM{} * {} * {} * {})".format(options.ff_from[0]+wt_ext,options.ff_from[1]+wt_ext,options.ff_from[2]+wt_ext,options.ff_from[0],options.ff_from[1],options.ff_from[2],fail_sel[options.ff_from[0]],fail_sel[options.ff_from[1]],fail_sel[options.ff_from[2]])
#  elif len(options.ff_from) == 4:
#    ff_wt = "(wt_ff_ml_{} * wt_ff_ml_{} * wt_ff_ml_{} * wt_ff_ml_{} * GM{} * GM{} * GM{} * GM{} * {} * {} * {} * {})".format(options.ff_from[0]+wt_ext,options.ff_from[1]+wt_ext,options.ff_from[2]+wt_ext,options.ff_from[3]+wt_ext,options.ff_from[0],options.ff_from[1],options.ff_from[2],options.ff_from[3],fail_sel[options.ff_from[0]],fail_sel[options.ff_from[1]],fail_sel[options.ff_from[2]],fail_sel[options.ff_from[3]])
#
#  ff_data_wt = ff_wt.replace("GM1","(1)").replace("GM2","(1)").replace("GM3","(1)").replace("GM4","(1)")
#  ff_mc_wt = "wt*(" + ff_wt.replace("GM1","(gen_match_1!=6)").replace("GM2","(gen_match_2!=6)").replace("GM3","(gen_match_3!=6)").replace("GM4","(gen_match_4!=6)") + ")"
#
#  ff_data_wt_init = copy.deepcopy(ff_wt)
#  ff_mc_wt_init = copy.deepcopy(ff_wt)
#  if wt_ext != "" and intermediate_shift != []:
#    for shift in intermediate_shift:
#      for n in ["1","2","3","4"]:  
#        for t in list(set([s_wt_ext,d_wt_ext,t_wt_ext,q_wt_ext])):
#          ff_data_wt = ff_data_wt_init.replace("wt_ff_ml_{}{}".format(n,t),"(wt_ff_ml_{} + {}*(wt_ff_ml_{}{} - wt_ff_ml_{}))".format(n,shift,n,t,n))
#          ff_mc_wt = ff_mc_wt_init.replace("wt_ff_ml_{}{}".format(n,t),"(wt_ff_ml_{} + {}*(wt_ff_ml_{}{} - wt_ff_ml_{}))".format(n,shift,n,t,n))
#
#      if data_veto == None:
#        ff_data = GetNode(ana, 'jetFakes', "_int_shift_"+shift.replace(".","p")+add_name, data_samples, plot, ff_data_wt, sel, ff_sel)
#      else:
#        ff_data = GetNode(ana, 'jetFakes', "_int_shift_"+shift.replace(".","p")+add_name, data_samples, plot, ff_data_wt, sel, "("+ff_sel+")&&("+data_veto+")")
#
#      #ana.nodes[nodename].AddNode(ff_data)
#      ana.nodes[nodename].AddNode(SubtractNode("jetFakes_int_shift_"+shift.replace(".","p")+add_name, ff_data, ff_mc))
#  else:
#
#    if data_veto == None:
#      ff_data = GetNode(ana, 'jetFakes', add_name, data_samples, plot, ff_data_wt, sel, ff_sel)
#      ff_mc = GetNode(ana, 'jetFakes_subtract', add_name, mc_samples, plot, ff_mc_wt, sel, ff_sel)
#    else:
#      ff_data = GetNode(ana, 'jetFakes', add_name, data_samples, plot, ff_data_wt, sel, "("+ff_sel+")&&("+data_veto+")")
#      ff_mc = GetNode(ana, 'jetFakes_subtract', add_name, mc_samples, plot, ff_mc_wt, sel, "("+ff_sel+")&&("+data_veto+")")
#
#    ana.nodes[nodename].AddNode(SubtractNode('jetFakes'+add_name, ff_data, ff_mc))
#    #ana.nodes[nodename].AddNode(ff_data)

def GenerateSignal(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    for i in samples:
      signal_node = GetNode(ana, i[1], add_name, [i[0]], plot, wt, sel, cat)
      ana.nodes[nodename].AddNode(signal_node)
 
def PrintSummary(nodename='', data_strings=['data_obs'], add_names=''):
    print ''
    print '################### Summary ###################'
    nodes = ana.nodes[nodename].SubNodes()
    bkg_total = ufloat(0.000000001,0.000000001)
    sig_total = ufloat(0.000000001,0.000000001)
    flat_sig = [val[1] for val in signal_samples]
    for node in nodes:
        if node.shape.rate.n == 0: per_err = 0
        else: per_err = node.shape.rate.s/node.shape.rate.n
        print node.name.ljust(10) , ("%.2f" % node.shape.rate.n).ljust(10), '+/-'.ljust(5), ("%.2f" % node.shape.rate.s).ljust(7), "(%.4f)" % per_err 
        if True in [node.name.find(add_name) != -1 and add_name is not '' for add_name in add_names]: continue
        if node.name not in data_strings and not node.name.endswith("Up") and not node.name.endswith("Down") and not node.name in flat_sig: 
          bkg_total += node.shape.rate      
        if node.name not in data_strings and not node.name.endswith("Up") and not node.name.endswith("Down") and node.name in flat_sig:
          sig_total += node.shape.rate
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
    from itertools import chain
    outfile.cd(nodename)
    nodes = ana.nodes[nodename].SubNodes()
    first_hist=True
    for node in nodes:
      if add_name not in node.name: continue
      if node.name in list(chain.from_iterable(signal_samples)): continue
      if node.name == "data_obs": continue
      if node.name.endswith("Up"): continue
      if node.name.endswith("Down"): continue
      if first_hist:
          total_bkg = ana.nodes[nodename].nodes[node.name].shape.hist.Clone()
          first_hist=False
      else: total_bkg.Add(ana.nodes[nodename].nodes[node.name].shape.hist.Clone())
    if not first_hist:        
      total_bkg.SetName('total_bkg'+add_name)
      total_bkg.Write()
   
   # first_hist = True
   # for node in nodes:
   #   if add_name not in node.name: continue
   #   if node.name == "data_obs": continue
   #   if node.name.endswith("Up"): continue
   #   if node.name.endswith("Down"): continue
   #   if node.name not in list(chain.from_iterable(signal_samples)): continue
   #   if first_hist:
   #     total_sig = ana.nodes[nodename].nodes[node.name].shape.hist.Clone()
   #     first_hist=False
   #   else: total_sig.Add(ana.nodes[nodename].nodes[node.name].shape.hist.Clone())
   # if not first_hist:
   #   total_sig.SetName('total_sig'+add_name)
   #   total_sig.Write()

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
          if options.channel == "mmmm":
            GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels)
          else:
            GenerateVV(ana, add_name, vv_samples+ggzz_samples+qqzz_samples, plot, wt, sel, cat, vv_sels)
      if 'VVV' not in samples_to_skip:
          GenerateVVV(ana, add_name, vvv_samples, plot, wt, sel, cat, vvv_sels) 
      if 'W' not in samples_to_skip:
          GenerateW(ana, add_name, wjets_samples, plot, wt, sel, cat, w_sels)
      if 'qqZZ' not in samples_to_skip and options.channel == "mmmm":
          GenerateQQZZ(ana, add_name, qqzz_samples, plot, wt, sel, cat)         
      if 'HZZ' not in samples_to_skip and options.channel == "mmmm": 
          GenerateHZZ(ana, add_name, hzz_samples, plot, "("+wt+"/wt_ggZZ_k_factor)", sel, cat)
      if 'Higgs' not in samples_to_skip:
          GenerateHiggs(ana, add_name, higgs_samples, plot, wt, sel, cat)
      if 'ggZZ' not in samples_to_skip and options.channel == "mmmm": 
          GenerateGGZZ(ana, add_name, ggzz_samples, plot, wt, sel, cat)          
      if 'signal' not in samples_to_skip and not options.no_signal:
          run_signal_samples = copy.deepcopy(signal_samples)
          for k,v in signal_samples:
            if k in samples_to_skip:
              run_signal_samples.remove(k)
          GenerateSignal(ana, add_name, run_signal_samples, plot, wt, sel, cat)
    elif options.method == 2:
      mc_samples = ztt_samples + vv_samples + vvv_samples + wgam_samples + top_samples + wjets_samples + ewkz_samples + ggzz_samples + qqzz_samples
      if 'jetFakes' not in samples_to_skip:

        if options.do_ff_systs:
          #wt_exts = ["_q_sum_down","_q_sum_up","_non_closure_down","_non_closure_up","_subtract_pass_non_closure_down","_subtract_pass_non_closure_up","_subtract_fail_non_closure_down","_subtract_fail_non_closure_up","_subtraction_up","_subtraction_down"]
          wt_exts = ["_non_closure_down","_non_closure_up","_subtract_pass_non_closure_down","_subtract_pass_non_closure_up","_subtract_fail_non_closure_down","_subtract_fail_non_closure_up","_subtraction_up","_subtraction_down"]
          if options.channel == "ttt":
            spec_wt_exts = {
                            "1":{"wt_ff_ml_1":["_iso_0f_up","_iso_0f_down"]},
                            "2":{"wt_ff_ml_2":["_iso_0f_up","_iso_0f_down"]},
                            "3":{"wt_ff_ml_3":["_iso_0f_up","_iso_0f_down"]},
                            "12":{"wt_ff_ml_1":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_2":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "13":{"wt_ff_ml_1":["_iso_1p1f_up","_iso_1p1f_down"],
                                  "wt_ff_ml_3":["_iso_1f1p_up","_iso_1f1p_down"]},
                            "23":{"wt_ff_ml_2":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_3":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "123":{"wt_ff_ml_1":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_2":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_3":["_iso_2f_up","_iso_2f_down"]}
                            }
          elif options.channel == "tttt":
            spec_wt_exts = {
                            "12":{"wt_ff_ml_1":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_2":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "13":{"wt_ff_ml_1":["_iso_1p1f_up","_iso_1p1f_down"],
                                  "wt_ff_ml_3":["_iso_1f1p_up","_iso_1f1p_down"]},
                            "14":{"wt_ff_ml_1":["_iso_1p1f_up","_iso_1p1f_down"],
                                  "wt_ff_ml_4":["_iso_1f1p_up","_iso_1f1p_down"]},
                            "23":{"wt_ff_ml_2":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_3":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "24":{"wt_ff_ml_2":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_4":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "34":{"wt_ff_ml_3":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_4":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "123":{"wt_ff_ml_1":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_2":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_3":["_iso_2f_up","_iso_2f_down"]},
                            "124":{"wt_ff_ml_1":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_2":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_4":["_iso_2f_up","_iso_2f_down"]},
                            "134":{"wt_ff_ml_1":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_3":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_4":["_iso_2f_up","_iso_2f_down"]},
                            "234":{"wt_ff_ml_2":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_3":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_4":["_iso_2f_up","_iso_2f_down"]},
                            "1234":{"wt_ff_ml_1":["_iso_2f_up","_iso_2f_down"],
                                    "wt_ff_ml_2":["_iso_2f_up","_iso_2f_down"],
                                    "wt_ff_ml_3":["_iso_2f_up","_iso_2f_down"],
                                    "wt_ff_ml_4":["_iso_2f_up","_iso_2f_down"]},
                            }
          elif options.channel in ["ettt","mttt"]:
            spec_wt_exts = {
                            "2":{"wt_ff_ml_2":["_iso_0f_up","_iso_0f_down"]},
                            "3":{"wt_ff_ml_3":["_iso_0f_up","_iso_0f_down"]},
                            "4":{"wt_ff_ml_4":["_iso_0f_up","_iso_0f_down"]},
                            "23":{"wt_ff_ml_2":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_3":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "24":{"wt_ff_ml_2":["_iso_1p1f_up","_iso_1p1f_down"],
                                  "wt_ff_ml_4":["_iso_1f1p_up","_iso_1f1p_down"]},
                            "34":{"wt_ff_ml_3":["_iso_1f1p_up","_iso_1f1p_down"],
                                  "wt_ff_ml_4":["_iso_1p1f_up","_iso_1p1f_down"]},
                            "234":{"wt_ff_ml_2":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_3":["_iso_2f_up","_iso_2f_down"],
                                   "wt_ff_ml_4":["_iso_2f_up","_iso_2f_down"]}
                            }

          elif options.channel in ["eett","mmtt","emtt"]:
            spec_wt_exts = {
                            "3":{"wt_ff_ml_3":["_iso_0f_up","_iso_0f_down"]},
                            "4":{"wt_ff_ml_4":["_iso_0f_up","_iso_0f_down"]},
                            "34":{"wt_ff_ml_3":["_iso_1f_up","_iso_1f_down"],
                                  "wt_ff_ml_4":["_iso_1f_up","_iso_1f_down"]},
                            }



        else:
          spec_wt_exts = {}
          wt_exts = []


        GenerateFakeTaus(ana, add_name, data_samples, mc_samples, plot, wt, sel, cat, options.charges_non_zero, data_veto=cats["data_veto"])
        for wt_ext in wt_exts: 
          GenerateFakeTaus(ana, wt_ext.replace("_up","Up").replace("_down","Down"), data_samples, mc_samples, plot, wt, sel, cat, options.charges_non_zero, data_veto=cats["data_veto"],wt_ext=wt_ext, spec_wt_ext=None)
        for num, val in spec_wt_exts.items():
          for wt_name, ext in val.items():
            for wt_ext in ext:
              GenerateFakeTaus(ana, wt_ext.replace("_up","Up").replace("_down","Down"), data_samples, mc_samples, plot, wt, sel, cat, options.charges_non_zero, data_veto=cats["data_veto"],wt_ext=wt_ext, spec_wt_ext=[num,wt_name])

          

      # if use ff_from need to add the rest back with MC
      if "MC_jetFakes" not in samples_to_skip and options.ff_from != "all":
          GenerateMCFakeTaus(ana, options.ff_from, add_name, mc_samples, plot, wt, sel, cat)
 
      if options.channel == "tttt":
        cat = "("+cat+")&&(gen_match_1<6 && gen_match_2<6 && gen_match_3<6 && gen_match_4<6)"
      elif options.channel == "ttt":
        cat = "("+cat+")&&(gen_match_1<6 && gen_match_2<6 && gen_match_3<6)"
      elif options.channel in ["ettt","mttt"]:
        cat = "("+cat+")&&(gen_match_2<6 && gen_match_3<6 && gen_match_4<6)"
      elif options.channel in ["eett","mmtt","emtt"]:
        cat = "("+cat+")&&(gen_match_3<6 && gen_match_4<6)"

      if 'ZTT' not in samples_to_skip:
          GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels)
      if 'TT' not in samples_to_skip:
          GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels)
      if 'VV' not in samples_to_skip:
          GenerateVV(ana, add_name, vv_samples+ggzz_samples+qqzz_samples, plot, wt, sel, cat, vv_sels)
      if 'Higgs' not in samples_to_skip:
          GenerateHiggs(ana, add_name, higgs_samples+hzz_samples, plot, wt, sel, cat) 
      if 'VVV' not in samples_to_skip:
          GenerateVVV(ana, add_name, vvv_samples, plot, wt, sel, cat, vvv_sels)
      if 'W' not in samples_to_skip:
          GenerateW(ana, add_name, wjets_samples, plot, wt, sel, cat, w_sels)
      if 'signal' not in samples_to_skip and not options.no_signal:
          #GenerateSignal(ana, add_name, signal_samples, plot, wt, sel, cat)
          run_signal_samples = copy.deepcopy(signal_samples)
          for k,v in signal_samples:
            if k in samples_to_skip:
              run_signal_samples.remove([k,v])
          GenerateSignal(ana, add_name, run_signal_samples, plot, wt, sel, cat)


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

def NormSignals(outfile,add_name):
    outfile.cd(nodename)
    for samp in signal_samples:
       xs = ana.info[samp[0]]['xs']
       if xs == 1.: continue
       sf = 1.0/xs
       hist_ = outfile.Get(nodename+'/'+samp[1]+add_name)
       if type(hist_) == type(ROOT.TObject()): continue
       hist_norm = hist_.Clone()
       #hist_norm.SetName(hist_.GetName()+"_norm")
       hist_norm.SetName(samp[1]+"_norm"+add_name)
       hist_norm.Scale(sf)
       hist_norm.Write("",ROOT.TObject.kOverwrite)
       #hist_.Scale(sf)
       #hist_.Write("",ROOT.TObject.kOverwrite)
    outfile.cd()


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

if options.replace_name != None:
  output_name = options.outputfolder+"/"+options.replace_name+".root"

if options.plot_from_dc == "": outfile = ROOT.TFile(output_name, 'RECREATE')

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

if options.syst_tau_id:
  dm_bins=["0","1","10"]
  for i,dm in enumerate(dm_bins):
    up_wts = []
    down_wts = []
    for ind, ch in enumerate(options.channel):
       if ch == "t":
         if(dm == "10"):
           up_wts.append("((idisoweight_ratio_{0}_up*((tau_decay_mode_{0}==10) || (tau_decay_mode_{0}==11)))+((tau_decay_mode_{0}!=10) && (tau_decay_mode_{0}!=11)))".format(ind+1))
           down_wts.append("((idisoweight_ratio_{0}_down*((tau_decay_mode_{0}==10) || (tau_decay_mode_{0}==11)))+((tau_decay_mode_{0}!=10) && (tau_decay_mode_{0}!=11)))".format(ind+1))
         else:
           up_wts.append("((idisoweight_ratio_{0}_up*(tau_decay_mode_{0}=={1}))+(tau_decay_mode_{0}!={1}))".format(ind+1,dm))
           down_wts.append("((idisoweight_ratio_{0}_down*(tau_decay_mode_{0}=={1}))+(tau_decay_mode_{0}!={1}))".format(ind+1,dm))

    bin_name = "syst_tau_id_DM"+dm 
    systematics['{}_up'.format(bin_name)] = ('' , '_'+bin_name+'Up', 'wt*'+"*".join(up_wts), ['jetFakes'], False)
    systematics['{}_down'.format(bin_name)] = ('' , '_'+bin_name+'Down', 'wt*'+"*".join(down_wts), ['jetFakes'], False)

if options.syst_tau_id_new:
  # uncert0, uncert1, syst_dm$DM_$ERA should be uncorrelated across DMs and eras
  dm_bins=["0","1","10","11"]
  for j in ["uncert0","uncert1","syst_dm_year"]:
     for k in ["2016_preVFP","2016_postVFP","2017","2018"]:
        for i,dm in enumerate(dm_bins):
           up_wts = []
           down_wts = []
           for ind, ch in enumerate(options.channel):
             if options.year == k: 
                 if ch == "t":
                    up_wts.append("((idisoweight_ratio_{0}_{2}_up*(tau_decay_mode_{0}=={1}))+(tau_decay_mode_{0}!={1}))".format(ind+1,dm,j))
                    down_wts.append("((idisoweight_ratio_{0}_{2}_down*(tau_decay_mode_{0}=={1}))+(tau_decay_mode_{0}!={1}))".format(ind+1,dm,j))
             else:
                 if ch == "t":
                    up_wts.append("(((1)*(tau_decay_mode_{0}=={1}))+(tau_decay_mode_{0}!={1}))".format(ind+1,dm,j))
                    down_wts.append("(((1)*(tau_decay_mode_{0}=={1}))+(tau_decay_mode_{0}!={1}))".format(ind+1,dm,j))

        bin_name = "syst_tau_id_{}_DM".format(j)+dm+k
        if j == "syst_dm_year": bin_name = "syst_tau_id_{}_DM".format(j.replace("_year","")+dm+k)
        systematics['{}_up'.format(bin_name)] = ('' , '_'+bin_name+'Up', 'wt*'+"*".join(up_wts), ['jetFakes'], False)
        systematics['{}_down'.format(bin_name)] = ('' , '_'+bin_name+'Down', 'wt*'+"*".join(down_wts), ['jetFakes'], False)

  # syst_$ERA should be correlated across DMs but uncorrelated by eras
  for k in ["2016_preVFP","2016_postVFP","2017","2018"]:
     up_wts = []
     down_wts = []
     for ind, ch in enumerate(options.channel):
        if options.year == k:
           if ch == "t":
              up_wts.append("(idisoweight_ratio_{0}_syst_year_up)".format(ind+1))
              down_wts.append("(idisoweight_ratio_{0}_syst_year_down)".format(ind+1))
        else:
           if ch == "t":
              up_wts.append("(1)".format(ind+1))
              down_wts.append("(1)".format(ind+1))

     bin_name = "syst_tau_id_syst_"+k
     systematics['{}_up'.format(bin_name)] = ('' , '_'+bin_name+'Up', 'wt*'+"*".join(up_wts), ['jetFakes'], False)
     systematics['{}_down'.format(bin_name)] = ('' , '_'+bin_name+'Down', 'wt*'+"*".join(down_wts), ['jetFakes'], False)

  # syst_alleras should correlated across DMs and eras
  for k in ["syst_all_eras","high_pt"]:
     up_wts =[]
     down_wts = []
     for ind, ch in enumerate(options.channel):
        if ch == "t":
           up_wts.append("(idisoweight_ratio_{0}_{1}_up)".format(ind+1,k))
           down_wts.append("(idisoweight_ratio_{0}_{1}_down)".format(ind+1,k))
     bin_name = "syst_tau_id_{}".format(k)
     systematics['{}_up'.format(bin_name)] = ('' , '_'+bin_name+'Up', 'wt*'+"*".join(up_wts), ['jetFakes'], False)
     systematics['{}_down'.format(bin_name)] = ('' , '_'+bin_name+'Down', 'wt*'+"*".join(down_wts), ['jetFakes'], False)


if options.syst_etau_fakerate:
  eta_bins = ["0","1.5","2.3"]
  for i,eta in enumerate(eta_bins):
    up_wts = []
    down_wts = []
    for ind, ch in enumerate(options.channel):
      if ch == "t":
         if i != len(eta_bins):
           up_wts.append("((etau_fakerate_ratio_{0}_up*(fabs(eta_{0} >= {1}) && fabs(eta_{0} < {2})))+ (fabs(eta_{0} >= {2})))".format(ind+1,eta_bins[i],eta_bins[i+1]))
           down_wts.append("((etau_fakerate_ratio_{0}_down*(fabs(eta_{0} >= {1}) && fabs(eta_{0} < {2})))+ (fabs(eta_{0} >= {2})))".format(ind+1,eta_bins[i],eta_bins[i+1]))
	 else:
           up_wts.append("((etau_fakerate_ratio_{0}_up*(fabs(eta_{0} >= {1}))) + (fabs(eta_{0} < {2})))".format(ind+1,eta_bins[i],eta_bins[i-1]))
           down_wts.append("((etau_fakerate_ratio_{0}_down*(fabs(eta_{0} >= {1}))) + (fabs(eta_{0} < {2})))".format(ind+1,eta_bins[i],eta_bins[i-1]))

    bin_name = "syst_etau_fakerate_"+eta
    systematics['{}_up'.format(bin_name)] = ('' , '_'+bin_name+'Up', 'wt*'+"*".join(up_wts), ['jetFakes'], False)
    systematics['{}_down'.format(bin_name)] = ('' , '_'+bin_name+'Down', 'wt*'+"*".join(down_wts), ['jetFakes'], False)

if options.syst_mtau_fakerate:
  eta_bins=["0","0.4","0.8","1.2","1.7","2.3"]
  for i,eta in enumerate(eta_bins):
    up_wts = []
    down_wts = []
    for ind, ch in enumerate(options.channel):
      if ch == "t":
         if i != len(eta_bins):
           up_wts.append("((mtau_fakerate_ratio_{0}_up*(fabs(eta_{0} >= {1}) && fabs(eta_{0} < {2})))+ (fabs(eta_{0} >= {2})))".format(ind+1,eta_bins[i],eta_bins[i+1]))
           down_wts.append("((mtau_fakerate_ratio_{0}_down*(fabs(eta_{0} >= {1}) && fabs(eta_{0} < {2})))+ (fabs(eta_{0} >= {2})))".format(ind+1,eta_bins[i],eta_bins[i+1]))
         else:
           up_wts.append("((mtau_fakerate_ratio_{0}_up*(fabs(eta_{0} >= {1}))) + (fabs(eta_{0} < {2})))".format(ind+1,eta_bins[i],eta_bins[i-1]))
           down_wts.append("((mtau_fakerate_ratio_{0}_down*(fabs(eta_{0} >= {1}))) + (fabs(eta_{0} < {2})))".format(ind+1,eta_bins[i],eta_bins[i-1]))       

    bin_name = "syst_mtau_fakerate_"+eta
    systematics['{}_up'.format(bin_name)] = ('' , '_'+bin_name+'Up', 'wt*'+"*".join(up_wts), ['jetFakes'], False)
    systematics['{}_down'.format(bin_name)] = ('' , '_'+bin_name+'Down', 'wt*'+"*".join(down_wts), ['jetFakes'], False)


if options.syst_doubletau_trg:
  if options.channel.count("t") >= 2: 
    systematics['syst_doubletau_trg_up'] = ('', '_syst_doubletau_trgUp', 'wt*total_trg_ratio_doubletau_up', ['jetFakes'], False)
    systematics['syst_doubletau_trg_down'] = ('', '_syst_doubletau_trgDown', 'wt*total_trg_ratio_doubletau_down', ['jetFakes'], False)

if options.syst_prefire:
    systematics['syst_prefire_up'] = ('' , '_syst_prefireUp', 'wt*wt_prefire_up/wt_prefire', ['jetFakes'], False)
    systematics['syst_prefire_down'] = ('' , '_syst_prefireDown', 'wt*wt_prefire_down/wt_prefire', ['jetFakes'], False)

# ---------------------
if options.syst_tau_scale:
    systematics['syst_tau_scale_up'] = ('TSCALE_UP' , '_syst_tau_scale'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_tau_scale_down'] = ('TSCALE_DOWN' , '_syst_tau_scale'+'Down', 'wt', ['jetFakes'], False)

if options.syst_tau_scale_group:
    names = ["syst_1prong","syst_1prong1pizero","syst_3prong","syst_3prong1pizero"]
    folders = ["TSCALE0PI","TSCALE1PI","TSCALE3PRONG","TSCALE3PRONG1PI0"]
    syst_dict = dict(zip(names, folders))
    for name, folder in syst_dict.iteritems():
        systematics["scale_tau_scale_{}_up".format(name)] = ("{}_UP".format(folder), "_{}Up".format(name),"wt", ["jetFakes"], False)
        systematics["scale_tau_scale_{}_down".format(name)] = ("{}_DOWN".format(folder), "_{}Down".format(name),"wt", ["jetFakes"], False)

if options.syst_jet_scale_group:
    # need dict of syst names and folders of where the shifted trees are found
    names = ["syst_Absolute", "syst_Absolute_year", "syst_BBEC1", "syst_BBEC1_year",
            "syst_EC2", "syst_EC2_year", "syst_FlavorQCD", "syst_HF", "syst_HF_year",
            "syst_RelativeBal", "syst_RelativeSample_year"]
    folders = ["JESABS", "JESABS_YEAR", "JESBBEC1", "JESBBEC1_YEAR",
            "JESEC2", "JESEC2_YEAR", "JESFLAV", "JESHF", "JESHF_YEAR",
            "JESRBAL", "JESRELSAMP_YEAR"]

    syst_dict = dict(zip(names, folders))

    #replaceYear = ""
    #if (options.year == '2016_preVFP'): replaceYear = "2016preVFP"
    #elif (options.year == '2016_postVFP'): replaceYear = "2016postVFP"
    #elif (options.year == '2017'): replaceYear = "2017"
    #elif (options.year == '2018'): replaceYear = "2018"
    #else: assert ValueError("Regrouped JES only works for full RunII analyses")

    #for name, folder in syst_dict.iteritems():
    #    if "year" in name: name=name.replace("year", replaceYear)
    #    systematics['syst_scale_j_{}_up'.format(name)] = ("{}_UP".format(folder), "_{}Up".format(name),"wt", ['jetFakes'], False)
    #    systematics['syst_scale_j_{}_down'.format(name)] = ("{}_DOWN".format(folder), "_{}Down".format(name),"wt", ['jetFakes'], False)
    for name, folder in syst_dict.iteritems():
      if "year" in name:
        for yr in ["2016_preVFP","2016_postVFP","2017","2018"]:
          year_name = name.replace("year",yr)
          systematics['syst_scale_j_{}_up'.format(year_name)] = ("{}_UP".format(folder) if yr == options.year else "", "_{}Up".format(year_name),"wt", ['jetFakes'], False)
          systematics['syst_scale_j_{}_down'.format(year_name)] = ("{}_DOWN".format(folder) if yr == options.year else "", "_{}Down".format(year_name),"wt", ['jetFakes'], False)
      else:
        systematics['syst_scale_j_{}_up'.format(name)] = ("{}_UP".format(folder), "_{}Up".format(name),"wt", ['jetFakes'], False)
        systematics['syst_scale_j_{}_down'.format(name)] = ("{}_DOWN".format(folder), "_{}Down".format(name),"wt", ['jetFakes'], False)


if options.syst_electron_scale:
    systematics['syst_electron_scale_up'] = ('ESCALE_UP' , '_syst_electron_scale'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_electron_scale_down'] = ('ESCALE_DOWN' , '_syst_electron_scale'+'Down', 'wt', ['jetFakes'], False)
if options.syst_efake_scale_0pi:
    systematics['syst_efake_scale_0pi_up'] = ('EFAKE0PI_UP' , '_syst_efake_scale_0pi'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_efake_scale_0pi_down'] = ('EFAKE0PI_DOWN' , '_syst_efake_scale_0pi'+'Down', 'wt', ['jetFakes'], False)
if options.syst_efake_scale_1pi:
    systematics['syst_efake_scale_1pi_up'] = ('EFAKE1PI_UP' , '_syst_efake_scale_1pi'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_efake_scale_1pi_down'] = ('EFAKE1PI_DOWN' , '_syst_efake_scale_1pi'+'Down', 'wt', ['jetFakes'], False)


if options.syst_muon_scale:
    systematics['syst_muon_scale_up'] = ('MUSCALE_UP' , '_syst_muon_scale'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_muon_scale_down'] = ('MUSCALE_DOWN' , '_syst_muon_scale'+'Down', 'wt', ['jetFakes'], False)
if options.syst_mufake_scale_0pi:
    systematics['syst_mufake_scale_0pi_up'] = ('MUFAKE0PI_UP' , '_syst_mufake_scale_0pi'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_mufake_scale_0pi_down'] = ('MUFAKE0PI_DOWN' , '_syst_mufake_scale_0pi'+'Down', 'wt', ['jetFakes'], False)
if options.syst_mufake_scale_1pi:
    systematics['syst_mufake_scale_1pi_up'] = ('MUFAKE1PI_UP' , '_syst_mufake_scale_1pi'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_mufake_scale_1pi_down'] = ('MUFAKE1PI_DOWN' , '_syst_mufake_scale_1pi'+'Down', 'wt', ['jetFakes'], False)


if options.syst_jet_res:
    systematics['syst_jet_res_up'] = ('JER_UP' , '_syst_jet_res'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_jet_res_down'] = ('JER_DOWN' , '_syst_jet_res'+'Down', 'wt', ['jetFakes'], False)

if options.syst_met_unclustered:
    systematics['syst_met_unclustered_up'] = ('METUNCL_UP', '_syst_met_unclustered'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_met_unclustered_down'] = ('METUNCL_DOWN', '_syst_met_unclustered'+'Down', 'wt', ['jetFakes'], False)
if options.syst_met_scale:
    systematics['syst_met_scale_up'] = ('MET_SCALE_UP', '_syst_met_scale'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_met_scale_down'] = ('MET_SCALE_DOWN', '_syst_met_scale'+'Down', 'wt', ['jetFakes'], False)
if options.syst_met_res:
    systematics['syst_met_res_up'] = ('MET_RES_UP', '_syst_met_res'+'Up', 'wt', ['jetFakes'], False)
    systematics['syst_met_res_down'] = ('MET_RES_DOWN', '_syst_met_res'+'Down', 'wt', ['jetFakes'], False)

if options.syst_electron_id:
    if options.channel == "eett":
      systematics['syst_electron_id_up'] = ('', '_syst_electron_id'+'Up', 'wt*((1.02)*(gen_match_1==1 || gen_match_1==3) + (!(gen_match_1==1 || gen_match_1==3)))*((1.02)*(gen_match_2==1 || gen_match_2==3) + (!(gen_match_2==1 || gen_match_2==3)))', ['jetFakes'], False)
      systematics['syst_electron_id_down'] = ('', '_syst_electron_id'+'Down', 'wt*((1.0/1.02)*(gen_match_1==1 || gen_match_1==3) + (!(gen_match_1==1 || gen_match_1==3)))*((1.0/1.02)*(gen_match_2==1 || gen_match_2==3) + (!(gen_match_2==1 || gen_match_2==3)))', ['jetFakes'], False)
    elif options.channel == "emtt" or options.channel == "ettt":
      systematics['syst_electron_id_up'] = ('', '_syst_electron_id'+'Up', 'wt*((1.02)*(gen_match_1==1 || gen_match_1==3) + (!(gen_match_1==1 || gen_match_1==3)))', ['jetFakes'], False)
      systematics['syst_electron_id_down'] = ('', '_syst_electron_id'+'Down', 'wt*((1.0/1.02)*(gen_match_1==1 || gen_match_1==3) + (!(gen_match_1==1 || gen_match_1==3)))', ['jetFakes'], False)

if options.syst_muon_id:
    if options.channel == "mmtt":
      systematics['syst_muon_id_up'] = ('', '_syst_muon_id'+'Up', 'wt*((1.02)*(gen_match_1==2 || gen_match_1==4) + (!(gen_match_1==2 || gen_match_1==4)))*((1.02)*(gen_match_2==2 || gen_match_2==4) + (!(gen_match_2==2 || gen_match_2==4)))', ['jetFakes'], False)
      systematics['syst_muon_id_down'] = ('', '_syst_muon_id'+'Down', 'wt*((1.0/1.02)*(gen_match_1==2 || gen_match_1==4) + (!(gen_match_1==2 || gen_match_1==4)))*((1.0/1.02)*(gen_match_2==2 || gen_match_2==4) + (!(gen_match_2==2 || gen_match_2==4)))', ['jetFakes'], False)
    elif options.channel == "mttt":
      systematics['syst_muon_id_up'] = ('', '_syst_muon_id'+'Up', 'wt*((1.02)*(gen_match_1==2 || gen_match_1==4) + (!(gen_match_1==2 || gen_match_1==4)))', ['jetFakes'], False)
      systematics['syst_muon_id_down'] = ('', '_syst_muon_id'+'Down', 'wt*((1.0/1.02)*(gen_match_1==2 || gen_match_1==4) + (!(gen_match_1==2 || gen_match_1==4)))', ['jetFakes'], False)
    elif options.channel == "emtt":
      systematics['syst_muon_id_up'] = ('', '_syst_muon_id'+'Up', 'wt*((1.02)*(gen_match_2==2 || gen_match_2==4) + (!(gen_match_2==2 || gen_match_2==4)))', ['jetFakes'], False)
      systematics['syst_muon_id_down'] = ('', '_syst_muon_id'+'Down', 'wt*((1.0/1.02)*(gen_match_2==2 || gen_match_2==4) + (!(gen_match_2==2 || gen_match_2==4)))', ['jetFakes'], False)

if options.syst_electron_trg:
  if options.channel.count("e") >= 1:
    systematics['syst_singlee_trg_up'] = ('', '_syst_singlee_trgUp', 'wt*total_trg_ratio_singlee_up', ['jetFakes'], False)
    systematics['syst_singlee_trg_down'] = ('', '_syst_singlee_trgDown', 'wt*total_trg_ratio_singlee_down', ['jetFakes'], False)

if options.syst_muon_trg:
  if options.channel.count("m") >= 1:
    systematics['syst_singlem_trg_up'] = ('', '_syst_singlem_trgUp', 'wt*total_trg_ratio_singlem_up', ['jetFakes'], False)
    systematics['syst_singlem_trg_down'] = ('', '_syst_singlem_trgDown', 'wt*total_trg_ratio_singlem_down', ['jetFakes'], False)

if options.syst_signal_theory:
  import json
  with open("input/4tau_xs_uncerts.json") as jsonfile: xs_shift = json.load(jsonfile)

  for k,v in signal_samples:

    removed_signal_samples = [i[0] for i in signal_samples]
    removed_signal_samples.remove(k)

    systematics['syst_signal_{}_qcd_scale_up'.format(v)] = ('', '_syst_signal_qcd_scaleUp', '(wt*wt_mc_1003/{})'.format(xs_shift[k]["qcd_scale"]["Up"]), ['jetFakes','VV','VVV','ZTT','TT','MC_jetFakes','Higgs']+removed_signal_samples, False)
    systematics['syst_signal_{}_qcd_scale_down'.format(v)] = ('', '_syst_signal_qcd_scaleDown', '(wt*wt_mc_1002/{})'.format(xs_shift[k]["qcd_scale"]["Down"]), ['jetFakes','VV','VVV','ZTT','TT','MC_jetFakes','Higgs']+removed_signal_samples, False)

    systematics['syst_signal_{}_pdf_up'.format(v)] = ('', '_syst_signal_pdfUp', '(wt*wt_mc_pdf/{})'.format(xs_shift[k]["pdf"]["Up"]), ['jetFakes','VV','VVV','ZTT','TT','MC_jetFakes','Higgs']+removed_signal_samples, False)
    systematics['syst_signal_{}_pdf_down'.format(v)] = ('', '_syst_signal_pdfDown', '(wt/(wt_mc_pdf*{}))'.format(xs_shift[k]["pdf"]["Down"]), ['jetFakes','VV','VVV','ZTT','TT','MC_jetFakes','Higgs']+removed_signal_samples, False)

    systematics['syst_signal_{}_alphas_up'.format(v)] = ('', '_syst_signal_alphasUp', '(wt*wt_mc_1112/{})'.format(xs_shift[k]["alpha_s"]["Up"]), ['jetFakes','VV','VVV','ZTT','TT','MC_jetFakes','Higgs']+removed_signal_samples, False)
    systematics['syst_signal_{}_alphas_down'.format(v)] = ('', '_syst_signal_alphasDown', '(wt*wt_mc_1111/{})'.format(xs_shift[k]["alpha_s"]["Down"]), ['jetFakes','VV','VVV','ZTT','TT','MC_jetFakes','Higgs']+removed_signal_samples, False)

if options.syst_k_factor: 
    systematics['syst_qqZZ_k_factor_up'] = ('', '_syst_qqZZ_k_factorUp', 'wt*wt_qqZZ_k_factor', ['jetFakes','VVV','ZTT','TT','MC_jetFakes','Higgs','signal'], False)
    systematics['syst_qqZZ_k_factor_down'] = ('', '_syst_qqZZ_k_factorDown', '(wt/wt_qqZZ_k_factor)', ['jetFakes','VVV','ZTT','TT','MC_jetFakes','Higgs','signal'], False)

    systematics['syst_ggZZ_k_factor_up'] = ('', '_syst_ggZZ_k_factorUp', 'wt*wt_ggZZ_k_factor', ['jetFakes','VVV','ZTT','TT','MC_jetFakes','Higgs','signal'], False)
    systematics['syst_ggZZ_k_factor_down'] = ('', '_syst_ggZZ_k_factorDown', '(wt/wt_ggZZ_k_factor)', ['jetFakes','VVV','ZTT','TT','MC_jetFakes','Higgs','signal'], False)


    #for k in ["PDFScale","QCDScale","As","PDFReplica"]:
    #  systematics['syst_ggZZ_k_factor_{}_up'.format(k)] = ('', '_syst_ggZZ_k_factor_{}Up'.format(k), 'wt*wt_ggZZ_{}Up'.format(k), ['jetFakes','VVV','ZTT','TT','MC_jetFakes','signal'], False)
    #  systematics['syst_ggZZ_k_factor_{}_down'.format(k)] = ('', '_syst_ggZZ_k_factor_{}Down'.format(k), 'wt*wt_ggZZ_{}Dn'.format(k), ['jetFakes','VVV','ZTT','TT','MC_jetFakes','signal'], False)


if options.plot_from_dc == "":
  max_systs_per_pass = 500 # code uses too much memory if we try and process too many systematics at once so set the maximum number of systematics processed per loop here
  while len(systematics) > 0:
    ana = Analysis()
    ana.remaps = {}
    if options.channel == 'emtt':
        ana.remaps['MuonEG'] = 'data_obs'
    elif options.channel in ['mttt','mmtt','mmmm']:
        ana.remaps['SingleMuon'] = 'data_obs'
    elif options.year != '2018' and options.channel in ['eett','ettt']:
        ana.remaps['SingleElectron'] = 'data_obs'
    elif options.year == '2018' and options.channel in ['eett','ettt']:
        ana.remaps['EGamma'] = 'data_obs'
    elif options.channel == 'tttt' or options.channel == 'ttt':
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
        weight = systematics[systematic][2]
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
        
        if options.data_folder: data_input_folder_name = options.data_folder
        else: data_input_folder_name = options.folder
         
        # Add all data files
        for sample_name in data_samples:
            ana.AddSamples(data_input_folder_name+'/'+sample_name+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name)
        
        # Add all MC background files
        for sample_name in ztt_samples + vv_samples + vvv_samples + wgam_samples + top_samples + wjets_samples + ewkz_samples + qqzz_samples + hzz_samples + ggzz_samples + higgs_samples:
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name)
        #print(ana.trees)
        for sample_name in signal_samples:
            ana.AddSamples(signal_mc_input_folder_name+'/'+sample_name[0]+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name[0])         
 
        ana.AddInfo(options.paramfile, scaleTo='data_obs')
    
        # Add data only for default
        if systematic == 'default': do_data = True
        else: do_data = False
                
        #Run default plot
        do_data=(not options.no_data)
        #weight="wt"
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

  #hist.Print("all")
  # getting binning
  binning = []
  for i in range(1,hist.GetNbinsX()+2):
    binning.append(hist.GetBinLowEdge(i))

  # remove outer 0 bins
  still_zero = True
  for i in range(1,hist.GetNbinsX()):
    if not (hist.GetBinContent(i) == 0 and hist.GetBinError(i) == 0): still_zero = False
    if still_zero:
      binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i))))
    else:
      break
  hist = RebinHist(hist,binning)

  still_zero = True
  for i in reversed(range(2,hist.GetNbinsX()+1)):
    if not (hist.GetBinContent(i) == 0 and hist.GetBinError(i) == 0): still_zero = False
    if still_zero:
      binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i+1))))
    else:
      break
  hist = RebinHist(hist,binning)

  # left to right
  finished = False
  k = 0
  while finished == False and k < 1000:
    k += 1
    for i in range(1,hist.GetNbinsX()):
      if hist.GetBinContent(i) > 0: uncert_frac = hist.GetBinError(i)/hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      #print hist.GetBinLowEdge(i), uncert_frac, BinUncertFraction,  hist.GetBinContent(i), BinThreshold
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
      if hist.GetBinContent(i) > 0: uncert_frac = hist.GetBinError(i)/hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      #print hist.GetBinLowEdge(i), uncert_frac, BinUncertFraction,  hist.GetBinContent(i), BinThreshold
      if uncert_frac > BinUncertFraction and hist.GetBinContent(i) < BinThreshold:
#        binning.remove(hist.GetBinLowEdge(i))
        binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i))))
        hist = RebinHist(hist,binning)
        break
      elif i == 2:
        finished = True
 
#  hist.Print("all")
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

def TotalUnc(h0, hists=[]):
  #sum in quadrature several systematic uncertainties to form total uncertainty band
  hout = h0.Clone()
  hup = h0.Clone()
  hdown = h0.Clone()
  hout.SetName(h0.GetName()+'_custom_uncerts')
  hup.SetName(h0.GetName()+'_custom_uncerts_up')
  hdown.SetName(h0.GetName()+'_custom_uncerts_down')
  for i in range(1,h0.GetNbinsX()+2):
    x0 = h0.GetBinContent(i)
    uncerts_up = [0.]
    uncerts_down = [0.]
    for h in hists:
      x = h.GetBinContent(i)
      if x>x0: uncerts_up.append(x-x0)
      if x<x0: uncerts_down.append(x0-x)
    up = 0.
    down = 0.
    for u in uncerts_up: up+=u**2
    for u in uncerts_down: down+=u**2
    up = up**.5
    down = down**.5

    hup.SetBinContent(i,x0+up)
    hdown.SetBinContent(i,x0-down)
    c = (x0+up + x0-down)/2
    u = (up+down)/2
    hout.SetBinContent(i,c)
    hout.SetBinError(i,u)
  return (hout, hup, hdown)


def SymmetriseUncertainty(hist,hist_up,hist_down):
  for i in range(0,hist_up.GetNbinsX()+1):
    max_shift = max(abs(hist_up.GetBinContent(i)-hist.GetBinContent(i)),abs(hist_down.GetBinContent(i)-hist.GetBinContent(i)))
    hist_up.SetBinContent(i,hist.GetBinContent(i)+max_shift)
    hist_down.SetBinContent(i,hist.GetBinContent(i)-max_shift)
  return hist_up, hist_down

if options.symmetrise_uncertainty and options.plot_from_dc == "":
  symmetrise_dict = {"jetFakes":["_all_non_closure"]}
  directory = outfile.Get(nodename)
  outfile.cd(nodename)
  for key in directory.GetListOfKeys():
    if key.GetName() in symmetrise_dict.keys():
      for syst in symmetrise_dict[key.GetName()]:
        hist = directory.Get(key.GetName()).Clone()
        hist_up = directory.Get(key.GetName()+syst+"Up").Clone()
        hist_down = directory.Get(key.GetName()+syst+"Down").Clone()
        hist_up, hist_down = SymmetriseUncertainty(hist,hist_up,hist_down)
        outfile.cd(nodename)
        hist_up.Write()
        hist_down.Write()


if is_2d and options.do_unrolling and options.plot_from_dc == "":
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
if is_3d and options.do_unrolling and options.plot_from_dc == "":
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

#if options.add_stat_to_syst and options.plot_from_dc == "":
#
#  custom_uncerts_up_name = 'total_bkg_uncerts_custom_up'
#  custom_uncerts_down_name = 'total_bkg_uncerts_custom_down'
#  directory = outfile.Get(nodename)
#  h0 = directory.Get('total_bkg')
#  hists=[]
#  for hist in directory.GetListOfKeys():
#    if ".subnodes" in hist.GetName(): continue
#
#    proc_names = ["VVR","VVLF","VVJF","jetFakes"]
#    if hist.GetName().endswith("Up") or hist.GetName().endswith("Down"):
#      for p in proc_names:
#        if p+"_" in hist.GetName():
#          no_syst_name = p
#          temp_hist = h0.Clone()
#          temp_hist.Add(directory.Get(no_syst_name),-1)
#          temp_hist.Add(directory.Get(hist.GetName()))
#          hists.append(temp_hist)
#
#  (uncert, up, down) = TotalUnc(h0, hists)
#  outfile.cd(nodename)
#  uncert.Write()
#  up.Write()
#  down.Write()


if options.plot_from_dc == "": outfile.Close()
if options.plot_from_dc != "": output_name = options.plot_from_dc

plot_file = ROOT.TFile(output_name, 'READ')

if options.auto_rebinning:
  outfile_rebin = ROOT.TFile(output_name.replace(".root","_rebinned.root"), 'RECREATE')
  outfile_rebin.mkdir(nodename)
  outfile_rebin.cd(nodename)
  total_bkghist = plot_file.Get(nodename+'/total_bkg').Clone()
  binning = FindRebinning(total_bkghist,BinThreshold=options.bin_threshold,BinUncertFraction=options.bin_uncert_fraction)
  if options.rebin_with_data:
    data_obs = plot_file.Get(nodename+'/data_obs').Clone()
    data_obs = RebinHist(data_obs,binning)  
    binning = FindRebinning(data_obs,BinThreshold=options.bin_threshold,BinUncertFraction=options.bin_uncert_fraction)


  print "New binning:", binning
  hists_done = []
  for i in  plot_file.Get(nodename).GetListOfKeys():
      if i.GetName() not in hists_done:
        if ".subnodes" not in i.GetName():
          RebinHist(plot_file.Get(nodename+'/'+i.GetName()).Clone(),binning).Write()
          hists_done.append(i.GetName())

  if options.symmetrise_uncertainty:
    symmetrise_dict = {"jetFakes":["_all_non_closure"]}
    directory = outfile_rebin.Get(nodename)
    outfile_rebin.cd(nodename)
    for key in directory.GetListOfKeys():
      if key.GetName() in symmetrise_dict.keys():
        for syst in symmetrise_dict[key.GetName()]:
          hist = directory.Get(key.GetName()).Clone()
          hist_up = directory.Get(key.GetName()+syst+"Up").Clone()
          hist_down = directory.Get(key.GetName()+syst+"Down").Clone()
          hist_up, hist_down = SymmetriseUncertainty(hist,hist_up,hist_down)
          outfile_rebin.cd(nodename)
          hist_up.Write()
          hist_down.Write()


  outfile_rebin.Close()
  plot_file = ROOT.TFile(output_name.replace(".root","_rebinned.root"))
else:
  plot_file = ROOT.TFile(output_name)


def ReplaceCategoryName(name):
  replace_dict = {
                  "z_control_nobtag": "Opposite Sign Leptons",
                  "2l2t_sig_nobtag": "Same Sign Leptons",
                  "nobtag": "",
                  "inclusive": "",
                  }
  if name in replace_dict.keys():
    return replace_dict[name]
  else:
    return name

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
    if options.replace_name != None: plot_name = options.outputfolder+'/'+options.replace_name
    titles = plotting.SetAxisTitles(options.var,options.channel,options.norm_bins)
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
      cat=ReplaceCategoryName(options.cat),
      plot_signals=options.plot_signals.split(","),
      draw_data=(not options.no_data),
      under_legend=options.under_legend,
      add_stat_to_syst=options.add_stat_to_syst,
      do_custom_uncerts=options.add_stat_to_syst,
      shrink_final_bin=options.shrink_final_bin
      )

#norm signal yields on datacards to 1pb AFTER plotting    
if not options.no_signal and not options.plot_from_dc:
  print "norm signal yields"
  outfile =  ROOT.TFile(output_name, 'UPDATE')
  for add_name in add_names:
      NormSignals(outfile,add_name)

