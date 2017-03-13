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

CHANNELS= ['et', 'mt', 'em','tt']
ANALYSIS= ['sm','mssm','Hhh']
METHODS= [8 ,9, 10, 11, 12 , 13, 14, 15, 16]

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = { "channel":"mt" , "outputfolder":"output", "folder":"/vols/cms/dw515/Offline/output/MSSM/Jan11/" , "paramfile":"scripts/Params_2016_spring16.json", "cat":"inclusive", "year":"2016", "era":"mssmsummer16", "sel":"(1)", "set_alias":[], "analysis":"mssm", "var":"m_vis(7,0,140)", "method":8 , "do_ss":False, "sm_masses":"125", "ggh_masses":"1000", "bbh_masses":"1000", "qcd_os_ss_ratio":-1, "add_sm_background":"", "syst_tau_scale":"", "syst_eff_t":"", "syst_tquark":"", "syst_zwt":"", "syst_w_fake_rate":"", "syst_scale_j":"", "syst_eff_b":"",  "syst_fake_b":"" ,"norm_bins":False, "blind":False, "x_blind_min":100, "x_blind_max":4000, "ratio":False, "y_title":"dN/dM_{T}^{tot} (1/GeV)", "x_title":"m_{T}^{tot} (GeV)", "custom_y_range":False, "y_axis_min":0.001, "y_axis_max":100,"custom_x_range":False, "x_axis_min":0.001, "x_axis_max":100, "log_x":False, "log_y":False, "extra_pad":0.0, "signal_scale":1, "draw_signal_mass":"", "draw_signal_tanb":10, "signal_scheme":"run2_mssm", "lumi":"12.9 fb^{-1} (13 TeV)", "no_plot":False, "ratio_range":"0.7,1.3", "datacard":"", "do_custom_uncerts":False, "uncert_title":"Systematic uncertainty", "custom_uncerts_wt_up":"wt_up","custom_uncerts_wt_down":"wt_down", "add_flat_uncert":0, "add_stat_to_syst":False }

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
parser.add_argument("--paramfile", dest="paramfile", type=str,
    help="Name of parameter file")
parser.add_argument("--cat", dest="cat", type=str,
    help="Category")
parser.add_argument("--datacard", dest="datacard", type=str,
    help="Datacard name")
parser.add_argument("--year", dest="year", type=str,
    help="Year")
parser.add_argument("--era", dest="era", type=str,
    help="Era")
parser.add_argument("--sel", dest="sel", type=str,
    help="Selection")
parser.add_argument("--set_alias", action="append", dest="set_alias", type=str,
    help="Overwrite alias selection using this options. Specify with the form --set_alias=nameofaliastoreset:newselection")
parser.add_argument("--analysis", dest="analysis", type=str, 
    help="Analysis.  Supported options: %(CHANNELS)s" % vars())
parser.add_argument("--var", dest="var", type=str,
    help="Variable to plot")
parser.add_argument("--method", dest="method", type=int,
    help="Method.  Supported options: %(METHODS)s" % vars())
parser.add_argument("--do_ss", dest="do_ss", action='store_true',
    help="Do same-sign.")
parser.add_argument("--sm_masses", dest="sm_masses", type=str,
    help="Comma seperated list of SM signal masses.")
parser.add_argument("--ggh_masses", dest="ggh_masses", type=str,
    help="Comma seperated list of SUSY ggH signal masses.")
parser.add_argument("--bbh_masses", dest="bbh_masses", type=str,
    help="Comma seperated list of SUSY bbH signal masses.")
parser.add_argument("--qcd_os_ss_ratio", dest="qcd_os_ss_ratio", type=float,
    help="QCD OS/SS ratio")
parser.add_argument("--add_sm_background", dest="add_sm_background", type=str,
    help="Add SM Higgs background for MSSM")
parser.add_argument("--syst_tau_scale", dest="syst_tau_scale", type=str,
    help="If this string is set then the systematic shift due to tau energy scale is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_eff_t", dest="syst_eff_t", type=str, default='',
    help="If this string is set then the systematic shift due to tau ID is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_tquark", dest="syst_tquark", type=str,
    help="If this string is set then the top-quark weight systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_zwt", dest="syst_zwt", type=str,
    help="If this string is set then the z-reweighting systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_w_fake_rate", dest="syst_w_fake_rate", type=str, default='',
    help="If this string is set then the W+jets fake-rate systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_scale_j", dest="syst_scale_j", type=str,
    help="If this string is set then the jet scale systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_eff_b", dest="syst_eff_b", type=str,
    help="If this string is set then the b-tag efficiency systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_fake_b", dest="syst_fake_b", type=str,
    help="If this string is set then the b-tag fake-rate systematic is performed with the set string appended to the resulting histogram name")
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
parser.add_argument("--extra_pad", dest="extra_pad", type=float,
    help="Fraction of extra whitespace at top of plot.")
parser.add_argument("--signal_scale", dest="signal_scale", type=float,
    help="Signal scale.")
parser.add_argument("--draw_signal_mass", dest="draw_signal_mass", type=str,
    help="Signal mass.")
parser.add_argument("--draw_signal_tanb", dest="draw_signal_tanb", type=float,
    help="Signal tanb.")
parser.add_argument("--signal_scheme", dest="signal_scheme", type=str,
    help="Signal scale.")
parser.add_argument("--lumi", dest="lumi", type=str,
    help="Lumi.")
parser.add_argument("--no_plot", dest="no_plot", action='store_true',
    help="If option is set then no pdf or png plots will be created only the output root file will be produced.")
parser.add_argument("--ratio_range", dest="ratio_range", type=str,
    help="y-axis range for ratio plot in format MIN,MAX")
parser.add_argument("--do_custom_uncerts", dest="do_custom_uncerts", action='store_true',
    help="Do custom uncertainty band. Up and down weights for this uncertainty band should be set using \"custom_uncerts_wt_up\" and \"custom_uncerts_wt_down\" options")
parser.add_argument("--custom_uncerts_wt_up", dest="custom_uncerts_wt_up", type=str,
    help="Up weight for custom uncertainty band")
parser.add_argument("--custom_uncerts_wt_down", dest="custom_uncerts_wt_down", type=str,
    help="Down weight for custom uncertainty band")
parser.add_argument("--uncert_title", dest="uncert_title", type=str,
    help="Custom uncertainty band legend label")
parser.add_argument("--add_stat_to_syst", dest="add_stat_to_syst", action='store_true',
    help="Add custom uncertainty band to statistical uncertainty.")
parser.add_argument("--add_flat_uncert", dest="add_flat_uncert", type=float,
    help="If set to non-zero will add a flat uncertainty band in quadrature to the uncertainty.")

options = parser.parse_args(remaining_argv)   

print ''
print '################### Options ###################'
print 'channel           = ' + options.channel
print 'outputfolder      = ' + options.outputfolder
print 'folder            = ' + options.folder
print 'paramfile         = ' + options.paramfile
print 'cat               = ' + options.cat
print 'year              = ' + options.year
print 'era               = ' + options.era
print 'sel               = ' + options.sel
print 'analysis          = ' + options.analysis
print 'var               = ' + options.var
print 'method            ='  ,  options.method
print 'do_ss             ='  ,  options.do_ss
print 'sm_masses         = ' +  options.sm_masses
print 'ggh_masses        = ' +  options.ggh_masses
print 'bbh_masses        = ' +  options.bbh_masses
print 'qcd_os_ss_ratio   ='  ,  options.qcd_os_ss_ratio
print 'add_sm_background ='  ,  options.add_sm_background
print 'syst_tau_scale    ='  ,  options.syst_tau_scale
print 'syst_eff_t        ='  ,  options.syst_eff_t
print 'syst_tquark       ='  ,  options.syst_tquark
print 'syst_zwt          ='  ,  options.syst_zwt
print 'syst_w_fake_rate  ='  ,  options.syst_w_fake_rate
print 'syst_scale_j      ='  ,  options.syst_scale_j
print 'syst_eff_b        ='  ,  options.syst_eff_b
print 'syst_fake_b       ='  ,  options.syst_fake_b
print 'blind             ='  ,  options.blind
print 'x_blind_min       ='  ,  options.x_blind_min
print 'x_blind_max       ='  ,  options.x_blind_max
print 'ratio             ='  ,  options.ratio
print 'ratio_range       ='  ,  options.ratio_range
print 'y_title           ='  ,  options.y_title
print 'x_title           ='  ,  options.x_title
print 'custom_y_range    ='  ,  options.custom_y_range
print 'y_axis_min        ='  ,  options.y_axis_min
print 'y_axis_max        ='  ,  options.y_axis_max
print 'custom_x_range    ='  ,  options.custom_x_range
print 'x_axis_min        ='  ,  options.x_axis_min
print 'x_axis_max        ='  ,  options.x_axis_max
print 'log_x             ='  ,  options.log_x
print 'log_y             ='  ,  options.log_x
print 'extra_pad         ='  ,  options.extra_pad
print 'signal_scale      ='  ,  options.signal_scale
print 'draw_signal_mass  ='  ,  options.draw_signal_mass
print 'draw_signal_tanb  ='  ,  options.draw_signal_tanb
print 'signal_scheme     ='  ,  options.signal_scheme
print 'lumi              ='  ,  options.lumi
print 'no_plot           ='  ,  options.no_plot
print '###############################################'
print ''

if options.era == "mssmsummer16": options.lumi = "35.9 fb^{-1} (13 TeV)"

# Define categories here
cats = {}
if options.analysis == 'sm':
    if options.channel == 'mt':
        cats['baseline'] = '(iso_1<0.15 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
    elif options.channel == 'et': 
        cats['baseline'] = '(iso_1<0.1  && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
elif options.analysis == 'mssm':
    if options.channel == 'mt':        
        cats['baseline'] = '(iso_1<0.15 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
    elif options.channel == 'et':
        cats['baseline'] = '(iso_1<0.1  && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
    if options.era == 'mssmsummer16':
        if options.channel == 'mt':        
            cats['baseline'] = '(iso_1<0.15 && mva_olddm_loose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
        elif options.channel == 'et':
            cats['baseline'] = '(iso_1<0.1  && mva_olddm_loose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
if options.channel == 'tt':
    cats['baseline'] = '(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
elif options.channel == 'em':
    cats['baseline'] = '(iso_1<0.15 && iso_2<0.2 && !leptonveto)'
elif options.channel == 'zmm':
    cats['baseline'] = '(iso_1<0.15 && iso_2<0.15)'
elif options.channel == 'zee':
    cats['baseline'] = '(iso_1<0.1 && iso_2<0.1)'
    
cats['inclusive'] = '(1)' 
cats['w_os'] = 'os'
cats['w_sdb'] = 'mt_1>70.'
cats['w_sdb_os'] = 'os'
cats['tt_qcd_norm'] = '(mva_olddm_tight_1>0.5 && mva_olddm_medium_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
cats['qcd_loose_shape'] = '(iso_1>0.2 && iso_1<0.5 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'

# MSSM categories
cats['btag'] = '(n_jets<=1 && n_bjets>=1)'
cats['nobtag'] = '(n_bjets==0)'
# loose/tight iso-MT categories
cats['nobtag_tight'] = cats['nobtag']
cats['nobtag_loosemt'] = '('+cats['nobtag']+ ' && mt_1>40)'
cats['nobtag_looseiso'] = '('+cats['nobtag']+' && mva_olddm_tight_2<0.5)'
cats['btag_tight'] = cats['btag']
cats['btag_loosemt'] = '('+cats['btag']+ ' && mt_1>40)'
cats['btag_looseiso'] = '('+cats['btag']+' && mva_olddm_tight_2<0.5)'

# Perhaps the safest thing to do is to set the tau isolation WP in the baseline selection - this means setting different baselines if one of the tight/loose-mt categories are chosen (maybe messy)
if options.cat == 'nobtag_tight' or options.cat == 'nobtag_loosemt' or options.cat == 'btag_tight' or options.cat == 'btag_loosemt':
    if options.channel == 'mt':        
        cats['baseline'] = '(iso_1<0.15 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
    elif options.channel == 'et':
        cats['baseline'] = '(iso_1<0.1  && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
# And aldo overwrite selection if one of the tight categories is chosen - this can still be overwritten from command line using the --set_alias=sel:(...) option
if options.cat == 'nobtag_tight' or options.cat == 'btag_tight':
    if options.channel == 'mt' or options.channel == 'et': options.sel = '(mt_1<40)'
# Also need to adjust btag wnobtag category (used for method 16) for different categories
cats['btag_wnobtag']='(n_jets <=1 && n_lowpt_jets>=1)'
if options.channel == 'mt' or options.channel == 'et':
    if options.cat == 'btag_loosemt': cats['btag_wnobtag']='(n_jets <=1 && n_lowpt_jets>=1 && mt_1>40)'
    if options.cat == 'btag_looseiso': cats['btag_wnobtag']='(n_jets <=1 && n_lowpt_jets>=1 && mva_olddm_tight_2<0.5)'

if options.era == "mssmsummer16":
    if options.channel == "em": cats['baseline']+=" && trg_muonelectron"
    if options.channel == "et" or options.channel == 'zee': cats['baseline']+=" && trg_singleelectron"
    if options.channel == "mt" or options.channel == 'zmm': cats['baseline']+=" && trg_singlemuon"
    if options.channel == "tt": cats['baseline']+=" && trg_doubletau"


# Overwrite any category selections if the --set_alias option is used
for i in options.set_alias:
    cat_to_overwrite = i.split(':')[0]
    cat_to_overwrite=cat_to_overwrite.replace("\"","")
    overwrite_with = i.split(':')[1]
    overwrite_with=overwrite_with.replace("\"","")
    print 'Overwriting alias: \"'+cat_to_overwrite+'\" with selection: \"'+overwrite_with+'\"'
    if cat_to_overwrite == 'sel':
        options.sel = overwrite_with
    else:
        cats[cat_to_overwrite] = overwrite_with

# Additional selections to seperate MC samples by gen flags

z_sels = {}
if options.channel == 'et':
    z_sels['ztt_sel'] = '(gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<5)'
    z_sels['zj_sel'] = '(gen_match_2==6)'
elif options.channel == 'mt':
    z_sels['ztt_sel'] = '(gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<5)'
    z_sels['zj_sel'] = '(gen_match_2==6)'
elif options.channel == 'tt':
    z_sels['ztt_sel'] = '(gen_match_1==5&&gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<6&&gen_match_1<6&&!(gen_match_1==5&&gen_match_2==5))'
    z_sels['zj_sel'] = '(gen_match_2==6||gen_match_1==6)'
elif options.channel == 'em':
    z_sels['ztt_sel'] = '(gen_match_1>2 && gen_match_2>3)'
    z_sels['zll_sel'] = '(gen_match_1<3 || gen_match_2<4)'
elif options.channel == 'zee' or  options.channel == 'zmm':
    z_sels['ztt_sel'] = '(1)'
    z_sels['zll_sel'] = '(1)'

top_sels = {}
top_sels['ttt_sel'] = z_sels['ztt_sel']
top_sels['ttj_sel'] = '!('+z_sels['ztt_sel']+')'

vv_sels = {}
vv_sels['vvt_sel'] = z_sels['ztt_sel']
vv_sels['vvj_sel'] = '!('+z_sels['ztt_sel']+')'
    
# Add data sample names
if options.channel == 'mt': 
    data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD']
if options.channel == 'em': 
    data_samples = ['MuonEGB','MuonEGC','MuonEGD']
if options.channel == 'et': 
    data_samples = ['SingleElectronB','SingleElectronC','SingleElectronD']
if options.channel == 'tt': 
    data_samples = ['TauB','TauC','TauD']
    
# Add MC sample names   
ztt_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','ZZTo2L2Q','ZZTo4L','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q']
wgam_samples = ['WGToLNuG','WGstarToLNuEE','WGstarToLNuMuMu']
top_samples = ['TT']
ztt_shape_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']

if options.channel == 'em': 
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t', 'T-t','WWTo1L1Nu2Q','VVTo2L2Nu', 'ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
else:
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','ZZTo4L','T-tW','T-t','Tbar-tW','Tbar-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
    
w_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t','T-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']

if options.era == "mssmsummer16":
    
    # Add data sample names
    if options.channel == 'mt' or options.channel == 'zmm': 
        data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleMuonG','SingleMuonHv2','SingleMuonHv3']
    if options.channel == 'em': 
        data_samples = ['MuonEGB','MuonEGC','MuonEGD','MuonEGE','MuonEGF','MuonEGG','MuonEGHv2','MuonEGHv3']
    if options.channel == 'et' or options.channel == 'zee': 
        data_samples = ['SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','SingleElectronG','SingleElectronHv2','SingleElectronHv3']
    if options.channel == 'tt': 
        data_samples = ['TauB','TauC','TauD','TauE','TauF','TauG','TauHv2','TauHv3']
        
    # Add MC sample names   
    ztt_samples = ['DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
    vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','ZZTo2L2Q','ZZTo4L-amcat','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q']
    wgam_samples = ['WGToLNuG','WGstarToLNuEE','WGstarToLNuMuMu']
    top_samples = ['TT']
    ztt_shape_samples = ['DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
    
    if options.channel == 'em': 
        qcd_sub_samples = ['DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t', 'T-t','WWTo1L1Nu2Q','VVTo2L2Nu', 'ZZTo4L-amcat','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
    else:
        qcd_sub_samples = ['DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','ZZTo4L-amcat','T-tW','T-t','Tbar-tW','Tbar-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
        
    w_sub_samples = ['DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t','T-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo4L-amcat','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
    wjets_samples = ['WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO-ext2']

sm_samples = { 'ggH' : 'GluGluHToTauTau', 'qqH' : 'VBFHToTauTau', 'WplusH' : 'WplusHToTauTau', 'WminusH' : 'WminusHToTauTau', 'ZH' : 'ZHToTauTau', 'TTH' : 'TTHToTauTau' }
mssm_samples = { 'ggH' : 'SUSYGluGluToHToTauTau', 'bbH' : 'SUSYGluGluToBBHToTauTau' }
Hhh_samples = { 'ggH' : 'GluGluToRadionToHHTo2B2Tau' }

# set systematics: first index sets folder name contaning systematic samples, second index sets string to be appended to output histograms, third index specifies the weight to be applied , 4th lists samples that should be skipped
systematics = OrderedDict()
systematics['default'] = ('','', 'wt', [])
if options.syst_tau_scale != '':
    systematics['scale_t_up'] = ('TSCALE_UP' , '_'+options.syst_tau_scale+'Up', 'wt', [])
    systematics['scale_t_down'] = ('TSCALE_DOWN' , '_'+options.syst_tau_scale+'Down', 'wt', [])
if options.syst_eff_t != '':
    systematics['syst_eff_t_up'] = ('' , '_'+options.syst_eff_t+'Up', 'wt*wt_tau_id_up', ['ZLL','VVJ','TTJ','QCD','W'] )
    systematics['syst_eff_t_down'] = ('' , '_'+options.syst_eff_t+'Down', 'wt*wt_tau_id_down', ['ZLL','VVJ','TTJ','QCD','W'])
if options.syst_tquark != '':
    systematics['syst_tquark_up'] = ('' , '_'+options.syst_tquark+'Up', 'wt*wt_tquark_up', ['ZTT','ZLL','VV','QCD','W','signal'])
    systematics['syst_tquark_down'] = ('' , '_'+options.syst_tquark+'Down', 'wt*wt_tquark_down', ['ZTT','ZLL','VV','QCD','W', 'signal'])    
if options.syst_zwt != '':
    systematics['syst_zwt_up'] = ('' , '_'+options.syst_zwt+'Up', 'wt*wt_zpt_up', ['ZLL','VV','TT','QCD','W','signal'])
    systematics['syst_zwt_down'] = ('' , '_'+options.syst_zwt+'Down', 'wt*wt_zpt_down', ['ZLL','VV','TT','QCD','W','signal'])
if options.syst_w_fake_rate != '':
    systematics['syst_w_fake_rate_up'] = ('' , '_'+options.syst_w_fake_rate+'Up', 'wt*wt_tau_fake_up', ['ZTT','ZLL','VV','TT','QCD','signal'])
    systematics['syst_w_fake_rate_down'] = ('' , '_'+options.syst_w_fake_rate+'Down', 'wt*wt_tau_fake_down', ['ZTT','ZLL','VV','TT','QCD','signal'])
if options.syst_scale_j != '':
    systematics['syst_scale_j_up'] = ('JES_UP' , '_'+options.syst_scale_j+'Up', 'wt', [])
    systematics['syst_scale_j_down'] = ('JES_DOWN' , '_'+options.syst_scale_j+'Down', 'wt', [])
if options.syst_eff_b != '':
    systematics['syst_b_up'] = ('BTAG_UP' , '_'+options.syst_eff_b+'Up', 'wt', [])
    systematics['syst_b_down'] = ('BTAG_DOWN' , '_'+options.syst_eff_b+'Down', 'wt', [])
if options.syst_fake_b != '':
    systematics['syst_fake_b_up'] = ('BFAKE_UP' , '_'+options.syst_fake_b+'Up', 'wt', [])
    systematics['syst_fake_b_down'] = ('BFAKE_DOWN' , '_'+options.syst_fake_b+'Down', 'wt', [])
        
if options.qcd_os_ss_ratio > 0:
    qcd_os_ss_ratio = options.qcd_os_ss_ratio
else:
    if options.channel == 'et':
        qcd_os_ss_ratio = 1.02
    elif options.channel == 'mt':
        qcd_os_ss_ratio = 1.18
    elif options.channel == 'zmm' or options.channel == 'zee':
        qcd_os_ss_ratio = 2.0    
    else:
        qcd_os_ss_ratio = 1.0
if options.do_ss:
    qcd_os_ss_ratio = 1.0

# Get array of signal masses to process        
        
sm_masses = options.sm_masses.split(',')
ggh_masses = options.ggh_masses.split(',')
bbh_masses = options.bbh_masses.split(',')

ROOT.TH1.SetDefaultSumw2(True)

# All functions defined here

def BuildCutString(wt='', sel='', cat='', sign='os',bkg_sel=''):
    full_selection = '(1)'
    if wt != '':
        full_selection = '('+wt+')'
    if sel != '':
        full_selection += '*('+sel+')'
    if sign != '':
        full_selection += '*('+sign+')'
    if bkg_sel != '':
        full_selection += '*('+bkg_sel+')'
    if cat != '':
        full_selection += '*('+cat+')'
    return full_selection 

def GenerateZTT(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['ztt_sel'])
    ana.nodes[nodename].AddNode(ana.SummedFactory('ZTT'+add_name, samples, plot, full_selection))

def GenerateZLL(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True, doZL=True, doZJ=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    if options.channel == 'em' or options.channel == 'zmm' or options.channel == 'zee':
        full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['zll_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZLL'+add_name, samples, plot, full_selection))
    else:
        if doZL:
            full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['zl_sel'])
            ana.nodes[nodename].AddNode(ana.SummedFactory('ZL'+add_name, samples, plot, full_selection))
        if doZJ:
            full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['zj_sel'])
            ana.nodes[nodename].AddNode(ana.SummedFactory('ZJ'+add_name, samples, plot, full_selection))
        
def GenerateTop(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}, get_os=True, doTTT=True, doTTJ=True):
  if get_os:
      OSSS = 'os'
  else:
      OSSS = '!os'  
  if doTTT:
      full_selection = BuildCutString(wt, sel, cat, OSSS, top_sels['ttt_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('TTT'+add_name, samples, plot, full_selection))
  if doTTJ:
      full_selection = BuildCutString(wt, sel, cat, OSSS, top_sels['ttj_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('TTJ'+add_name, samples, plot, full_selection))

def GenerateVV(ana, add_name ='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}, get_os=True, doVVT=True, doVVJ=True): 
  if get_os:
      OSSS = 'os'
  else:
      OSSS = '!os'  
  if doVVT:
      full_selection = BuildCutString(wt, sel, cat, OSSS, vv_sels['vvt_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('VVT'+add_name, samples, plot, full_selection))
  if doVVJ:
      full_selection = BuildCutString(wt, sel, cat, OSSS, vv_sels['vvj_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('VVJ'+add_name, samples, plot, full_selection))
  
def GetWGNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', get_os=True):
  full_selection = BuildCutString(wt, sel, cat, OSSS)
  wg_node = ana.SummedFactory('WGam'+add_name, samples, plot, full_selection)
  return wg_node

def GetWNode(ana, name='W', samples=[], data=[], sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
  if get_os:
      OSSS = 'os'
  else:
      OSSS = '!os'
  full_selection = BuildCutString(wt, sel, cat, OSSS, '')
  shape_cat = cat
  if method == 14:
      shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
  shape_selection = BuildCutString(wt, sel, shape_cat, OSSS, '')
  
  if method in [8, 9, 15]:
      w_node = ana.SummedFactory(name, samples, plot, full_selection)
  elif method in [10, 11]:
      control_sel = cats['w_sdb']+' && '+ OSSS
      w_control_full_selection = BuildCutString('wt', control_sel, cat, OSSS)
      if shape_selection == full_selection:
          w_shape = None
      else:    
          w_shape = ana.SummedFactory('w_shape', samples, plot, shape_selection)
      w_node = HttWNode(name,
        ana.SummedFactory('data_obs', data, plot, w_control_full_selection),
        ana.SummedFactory('backgrounds', sub_samples, plot, w_control_full_selection),
        ana.SummedFactory('w_control', samples, plot, w_control_full_selection),
        ana.SummedFactory('w_signal', samples, plot, full_selection),
        w_shape)
  elif method in [12, 13, 14, 16]:
      if method == 16:
          cat_nobtag = '('+cats['btag_wnobtag']+')*('+cats['baseline']+')'
          
          full_selection = BuildCutString(wt, sel, cat_nobtag, OSSS)
          ss_selection = BuildCutString(wt, '', cat_nobtag, '!os', '')
          os_selection = BuildCutString(wt, '', cat_nobtag, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection = BuildCutString('wt', control_sel, cat_nobtag, OSSS)
          w_control_full_selection_os = BuildCutString('wt', control_sel, cat_nobtag)
          w_control_full_selection_ss = BuildCutString('wt', control_sel, cat_nobtag, '!os')
          btag_extrap_sel_num = BuildCutString(wt, sel, cat, OSSS, '')
          btag_extrap_sel_den = BuildCutString(wt, sel, cat_nobtag, OSSS, '')
              
          btag_extrap_num_node = ana.SummedFactory('btag', samples, plot, btag_extrap_sel_num)
          btag_extrap_den_node = ana.SummedFactory('no_btag', samples, plot, btag_extrap_sel_den)
          
      else:
          full_selection = BuildCutString(wt, sel, cat, OSSS)
          ss_selection = BuildCutString(wt, '', cat, '!os', '')
          os_selection = BuildCutString(wt, '', cat, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection = BuildCutString('wt', control_sel, cat, OSSS)
          w_control_full_selection_os = BuildCutString('wt', control_sel, cat)
          w_control_full_selection_ss = BuildCutString('wt', control_sel, cat, '!os')
          btag_extrap_num_node = None
          btag_extrap_den_node = None
      
      if shape_selection == full_selection:
          w_shape = None
      else:    
          w_shape = ana.SummedFactory('w_shape', samples, plot, shape_selection)
      w_node = HttWOSSSNode(name,
        ana.SummedFactory('data_os_obs', data, plot, w_control_full_selection_os),
        ana.SummedFactory('backgrounds_os', sub_samples, plot, w_control_full_selection_os),
        ana.SummedFactory('data_ss_obs', data_samples, plot, w_control_full_selection_ss),
        ana.SummedFactory('backgrounds_ss', sub_samples, plot, w_control_full_selection_ss),
        ana.SummedFactory('w_control', samples, plot, w_control_full_selection),
        ana.SummedFactory('w_signal', samples, plot, full_selection),
        ana.SummedFactory('w_os', samples, plot, os_selection),
        ana.SummedFactory('w_ss', samples, plot, ss_selection),
        w_shape,
        qcd_factor,
        get_os,
        btag_extrap_num_node,
        btag_extrap_den_node) 
  return w_node

def GenerateW(ana, name='W', add_name='', samples=[], data=[], sub_samples=[], wg_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
  w_node_name = name  
  if options.channel == 'em':
      w_total_node = SummedNode('W'+add_name)
      w_total_node.AddNode(GetWGNode(ana, add_name, wg_samples, plot, wt, sel, cat))
      ana.nodes[nodename].AddNode(GetWGNode(ana, add_name, wg_samples, plot, wt, sel, cat))
      w_node_name = name+'J'
  ana.nodes[nodename].AddNode(GetWNode(ana, w_node_name+add_name, samples, data, sub_samples, plot, wt, sel, cat, method, qcd_factor, get_os))
  if options.channel == 'em':
      w_total_node.AddNode(GetWNode(ana, w_node_name+add_name, samples, data, sub_samples, plot, wt, sel, cat, method, qcd_factor, get_os))
      ana.nodes[nodename].AddNode(w_total_node)
      
def GenerateQCD(ana, add_name='', data=[], qcd_sub_samples=[], w_sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
    tt_qcd_norm = cats['tt_qcd_norm']
    if options.channel == 'et' or options.channel == 'mt':
        ttqcdcat = '('+cats[options.cat]+')*(iso_1<0.1 && antiele_2 && antimu_2 && !leptonveto)*(tau_decay_mode_2!=6&&tau_decay_mode_2!=5)'
    elif options.channel == 'tt':
        ttqcdcat = '('+cats[options.cat]+')*(antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
        
    qcd_sdb_sel = '(!os && ' + sel + ')'
    w_extrp_sdb_sel = '(!os && '+ cats['w_sdb'] + ')'
    w_extrp_sig_sel = '(!os && ' + sel + ')'
    w_sdb_sel = '(!os && ' + cats['w_sdb'] + ')'
    w_sdb_sel_osss = cats['w_sdb']
    qcd_cat = cat
    qcd_sdb_cat = cat
    qcd_extrap_sel = '(!os && ' + sel + ')'
    
    shape_node = None
    
    if options.channel != 'tt':
        
        if method in [9, 11, 13, 16]:
            if method == 16:
                shape_cat = cats[options.cat]
            else:
                shape_cat = '('+cats[options.cat]+')*('+cats['qcd_loose_shape']+')'
            shape_selection = BuildCutString('wt', qcd_sdb_sel, shape_cat, '')
            shape_node = ana.SummedFactory('shape', data, plot, shape_selection)
        elif method in [10, 12, 14]:
            if method == 14:
                shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
            else: 
                shape_cat = cat
            shape_selection = BuildCutString('wt', qcd_sdb_sel, shape_cat, '')
            bkg_shape = ana.SummedFactory('bkg_shape', qcd_sub_samples, plot, shape_selection)
            bkg_shape.AddNode(GetWNode(ana, 'W_shape', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, shape_cat, method, qcd_os_ss_ratio, False))
            shape_node = SubtractNode('shape', ana.SummedFactory('data_shape', data, plot, shape_selection), bkg_shape)
        
        if options.channel == 'em':
            qcd_os_ss_factor = 1
        else:
            qcd_os_ss_factor = qcd_factor
        weight = wt
        if method == 15:
            qcd_os_ss_factor = 1
            if get_os and options.channel == "em":
                weight = wt+'*wt_em_qcd'
        
        full_selection = BuildCutString(weight, qcd_sdb_sel, qcd_cat, '')
        subtract_node = ana.SummedFactory('subtract_node', qcd_sub_samples, plot, full_selection)
        w_node = GetWNode(ana, 'Wss', wjets_samples, data_samples, w_sub_samples, plot, weight, sel, cat, method, qcd_os_ss_ratio, False)
        subtract_node.AddNode(w_node)
        
        ana.nodes[nodename].AddNode(HttQCDNode('QCD'+add_name,
          ana.SummedFactory('data_ss', data, plot, full_selection),
          subtract_node,
          qcd_os_ss_factor,
          shape_node))
        
    else:
        OSSS = False
        if get_os: OSSS = True
        if method == 8:
            if OSSS: 
                qcd_sdb_sel =  '(os && ' + sel + ')'
            qcd_sdb_cat = ttqcdcat+'*'+tt_qcd_norm 
            
        shape_cat = '('+cats[options.cat]+')*('+cats['tt_qcd_norm']+')'
        shape_selection = BuildCutString('wt', qcd_sdb_sel, shape_cat, '')
        bkg_shape = ana.SummedFactory('bkg_shape', qcd_sub_samples, plot, shape_selection)
        bkg_shape.AddNode(GetWNode(ana, 'W_shape', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, shape_cat, method, qcd_os_ss_ratio, OSSS))
        shape_node = SubtractNode('shape', ana.SummedFactory('data_shape', data, plot, shape_selection), bkg_shape)
            
        full_selection = BuildCutString('wt', qcd_sdb_sel, qcd_sdb_cat, '')
        subtract_node = ana.SummedFactory('subtract_node', qcd_sub_samples, plot, full_selection)
        if method == 8:
            w_node = GetWNode(ana, 'Wos', wjets_samples, data_samples, w_sub_samples, plot, 'wt', qcd_sdb_sel, qcd_sdb_cat, method, qcd_os_ss_ratio, get_os)
        else:
            w_node = GetWNode(ana, 'Wss', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, qcd_sdb_cat, method, qcd_os_ss_ratio, False)
        subtract_node.AddNode(w_node)
        num_selection = BuildCutString('wt', sel, qcd_cat, '!os')
        den_selection = BuildCutString('wt', sel, qcd_sdb_cat, '!os')

        num_node = SubtractNode('ratio_num',
                     ana.SummedFactory('data_num', data, plot, num_selection),
                     ana.SummedFactory('bkg_num', qcd_sub_samples, plot, num_selection))
        w_num_node = GetWNode(ana, 'W_num', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, qcd_cat, method, qcd_os_ss_ratio, False)
        num_node = SubtractNode('ratio_num',
                     num_node,
                     w_num_node)
        den_node = SubtractNode('ratio_den',
                     ana.SummedFactory('data_den', data, plot, den_selection),
                     ana.SummedFactory('bkg_den', qcd_sub_samples, plot, den_selection))
        w_den_node = GetWNode(ana, 'W_den', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, qcd_sdb_cat, method, qcd_os_ss_ratio, False)
        den_node = SubtractNode('ratio_den',
                     den_node,
                     w_den_node) 
        
        ana.nodes[nodename].AddNode(HttQCDNode('QCD'+add_name,
          ana.SummedFactory('data_ss', data, plot, full_selection),
          subtract_node,
          1,
          shape_node,
          num_node,
          den_node))
        
def GenerateFakeTaus(ana, add_name='', data=[], plot='', wt='', sel='', cat_name='',get_os=True):
    print "Generating fake tau background via fake-factor method. In order for this to work you must first ensure that the fake-faktor weights are included in the input tree for the channel and category you wish use. Weights should be named as: wt_ff_channel_category"
    
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    
    # Select data from anti-isolated region
    if options.channel != "tt":
        if options.channel == 'mt':
            anti_isolated_sel = '(iso_1<0.15 && mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
        elif options.channel == 'et': 
            anti_isolated_sel = '(iso_1<0.1  && mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
            
        ff_cat = cats[cat_name] +" && "+ anti_isolated_sel
        fake_factor_wt_string = "wt_ff_"+options.channel+"_"+options.cat
        if wt is not "": wt+="*"+fake_factor_wt_string
        else: wt=fake_factor_wt_string
    
        full_selection = BuildCutString(wt, sel, ff_cat, OSSS, '')
        # Calculate FF for anti-isolated data (f1) then subtract contributions from real taus (f2)
        f1 = ana.SummedFactory('f1', data, plot, full_selection)
        f2 = ana.SummedFactory('f2', ztt_samples+vv_samples+top_samples+wjets_samples, plot, full_selection+"*(gen_match_2<6)")
        ana.nodes[nodename].AddNode(SubtractNode('FakeTaus'+add_name, f1, f2))
        
    if options.channel == 'tt':
        anti_isolated_sel_1 = '(mva_olddm_tight_1<0.5 && mva_olddm_vloose_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
        anti_isolated_sel_2 = '(mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && mva_olddm_tight_1>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
        
        ff_cat_1 = cats[cat_name] +" && "+ anti_isolated_sel_1
        ff_cat_2 = cats[cat_name] +" && "+ anti_isolated_sel_2
        fake_factor_wt_string_1 = "wt_ff_"+options.channel+"_"+options.cat+"_1"
        fake_factor_wt_string_2 = "wt_ff_"+options.channel+"_"+options.cat+"_2"
        if wt is not "": 
            wt_1=wt+"*"+fake_factor_wt_string_1
            wt_2=wt+"*"+fake_factor_wt_string_2
        else: 
            wt_1=fake_factor_wt_string_1
            wt_2=fake_factor_wt_string_2
    
        full_selection_1 = BuildCutString(wt_1, sel, ff_cat_1, OSSS, '')
        full_selection_2 = BuildCutString(wt_2, sel, ff_cat_1, OSSS, '')
        
        ff_total_node = SummedNode('FakeTaus'+add_name)
        f1_total_node = SummedNode('f1'+add_name)
        f1_total_node.AddNode(ana.SummedFactory('f1_1'+add_name, data, plot, full_selection_1))
        f1_total_node.AddNode(ana.SummedFactory('f1_2'+add_name, data, plot, full_selection_2))
        f2_total_node = SummedNode('f2'+add_name)
        f2_total_node.AddNode(ana.SummedFactory('f2_1'+add_name, ztt_samples+vv_samples+top_samples+wjets_samples, plot, full_selection_1+"*(gen_match_1<6)"))
        f2_total_node.AddNode(ana.SummedFactory('f2_2'+add_name, ztt_samples+vv_samples+top_samples+wjets_samples, plot, full_selection_2+"*(gen_match_2<6)"))
        ana.nodes[nodename].AddNode(SubtractNode('FakeTaus'+add_name, f1_total_node, f2_total_node))
        
def GenerateSMSignal(ana, add_name='', plot='', masses=['125'], wt='', sel='', cat='', get_os=True, sm_bkg = ''):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS)
    for mass in masses:  
        if sm_bkg != '':
            add_str = '_SM'+sm_bkg
        else:
            add_str = mass
        for key in sm_samples:
            sample_name = sm_samples[key]+'_M-'+mass
            ana.nodes[nodename].AddNode(ana.BasicFactory(key+add_str+add_name, sample_name, plot, full_selection))
            
def GenerateMSSMSignal(ana, add_name='', plot='', ggh_masses = ['1000'], bbh_masses = ['1000'], wt='', sel='', cat='', get_os=True, do_ggH=True, do_bbH=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS)
    for key in mssm_samples:
        if key == 'ggH':
            masses = ggh_masses
        elif key == 'bbH':
            masses = bbh_masses
        for mass in masses:
            if key == 'ggH' and not do_ggH:
                continue
            if key == 'bbH' and not do_bbH:
                continue
            sample_name = mssm_samples[key]+'_M-'+mass
            ana.nodes[nodename].AddNode(ana.BasicFactory(key+mass+add_name, sample_name, plot, full_selection))
        
def GenerateHhhSignal(ana, add_name='', plot='', masses = ['700'], wt='', sel='', cat='', get_os=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS)
    for mass in masses:
        for key in Hhh_samples:
            sample_name = Hhh_samples[key]+'_M-'+mass
            ana.nodes[nodename].AddNode(ana.BasicFactory(key+mass+add_name, sample_name, plot, full_selection))
        
def PrintSummary(nodename='', data_strings=['data_obs'], add_name=''):
    print ''
    print '################### Summary ###################'
    nodes = ana.nodes[nodename].SubNodes()
    bkg_total = ufloat(0.000000001,0.000000001)
    sig_total = ufloat(0.000000001,0.000000001)
    for node in nodes:
        if node.shape.rate.n == 0: per_err = 0
        else: per_err = node.shape.rate.s/node.shape.rate.n
        print node.name.ljust(10) , ("%.2f" % node.shape.rate.n).ljust(10), '+/-'.ljust(5), ("%.2f" % node.shape.rate.s).ljust(7), "(%.4f)" % per_err
        if True in [node.name.find(sig) != -1 for sig in signal_samples.keys()] and node.name.find("_SM"+options.add_sm_background) ==-1:
            sig_total += node.shape.rate
        elif node.name not in data_strings:
            bkg_total += node.shape.rate
    if bkg_total.n == 0: per_err = 0        
    else: per_err = bkg_total.s/bkg_total.n
    print 'Total bkg'.ljust(10) , ("%.2f" % bkg_total.n).ljust(10), '+/-'.ljust(5), ("%.2f" % bkg_total.s).ljust(7), "(%.4f)" % per_err
    if sig_total.n == 0: per_err = 0
    else: per_err = sig_total.s/sig_total.n
    print 'Total sig'.ljust(10) , ("%.2f" % sig_total.n).ljust(10), '+/-'.ljust(5), ("%.2f" % sig_total.s).ljust(7), "(%.4f)" % per_err
    print '###############################################'
    print ''
    
def signalComp(leg,plots,colour,stacked):
  return dict([('leg_text',leg),('plot_list',plots),('colour',colour),('in_stack',stacked)])

def backgroundComp(leg,plots,colour):
  return dict([('leg_text',leg),('plot_list',plots),('colour',colour)])

def createAxisHists(n,src,xmin=0,xmax=499):
  result = []
  for i in range(0,n):
    res = src.Clone()
    res.Reset()
    res.SetTitle("")
    res.SetName("axis%(i)d"%vars())
    res.SetAxisRange(xmin,xmax)
    res.SetStats(0)
    result.append(res)
  return result

def Plot(nodename, infile=None):
    ROOT.gROOT.SetBatch(ROOT.kTRUE)
    ROOT.TH1.AddDirectory(False)
    
    # Define signal schemes here
    sig_schemes = {}
    sig_schemes['sm_default'] = ( str(int(options.signal_scale))+"#times SM H("+options.draw_signal_mass+" GeV)#rightarrow#tau#tau", ["ggH", "qqH"], True ) 
    sig_schemes['run2_mssm'] = ( str(int(options.signal_scale))+"#times gg#phi("+options.draw_signal_mass+" GeV)#rightarrow#tau#tau", ["ggH"], False )
    sig_schemes['run2_mssm_bbH'] = ( str(int(options.signal_scale))+"#times bb#phi("+options.draw_signal_mass+" GeV)#rightarrow#tau#tau", ["bbH"], False )
    #sig_schemes['run2_mssm'] = ( str(int(options.signal_scale))+"#times gg#phi("+options.draw_signal_mass+" GeV)#rightarrow#tau#tau", ["ggH"], False )
    
    plotting.ModTDRStyle(r=0.04, l=0.14)
    
    background_schemes = {'mt':[backgroundComp("t#bar{t}",["TTT","TTJ"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("Electroweak",["VVT","VVJ","W"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrow#mu#mu",["ZL","ZJ"],ROOT.TColor.GetColor(100,192,232)),backgroundComp("Z#rightarrow#tau#tau",["ZTT"],ROOT.TColor.GetColor(248,206,104))],
    'et':[backgroundComp("t#bar{t}",["TTT","TTJ"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("Electroweak",["VVT","VVJ","W"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrowee",["ZL","ZJ"],ROOT.TColor.GetColor(100,192,232)),backgroundComp("Z#rightarrow#tau#tau",["ZTT"],ROOT.TColor.GetColor(248,206,104))],
    'tt':[backgroundComp("t#bar{t}",["TTT","TTJ"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("Electroweak",["VVT","VVJ","W","ZL","ZJ"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrow#tau#tau",["ZTT"],ROOT.TColor.GetColor(248,206,104))],
    'em':[backgroundComp("t#bar{t}",["TTT", "TTJ"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("Electroweak",["VVJ","VVT","W"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrowll",["ZLL"],ROOT.TColor.GetColor(100,192,232)),backgroundComp("Z#rightarrow#tau#tau",["ZTT"],ROOT.TColor.GetColor(248,206,104))],
    'zm':[backgroundComp("Misidentified #mu", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("t#bar{t}",["TT"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("Electroweak",["VV","W","ZJ"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrow#tau#tau",["ZTT"],ROOT.TColor.GetColor(248,206,104)),backgroundComp("Z#rightarrow#mu#mu",["ZL"],ROOT.TColor.GetColor(100,192,232))],
    'zmm':[backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("t#bar{t}",["TTT","TTJ"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("Electroweak",["VVT","VVJ","W"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrow#mu#mu",["ZLL"],ROOT.TColor.GetColor(100,192,232))],
    'zee':[backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)),backgroundComp("t#bar{t}",["TTT","TTJ"],ROOT.TColor.GetColor(155,152,204)),backgroundComp("Electroweak",["VVT","VVJ","W"],ROOT.TColor.GetColor(222,90,106)),backgroundComp("Z#rightarrow ee",["ZLL"],ROOT.TColor.GetColor(100,192,232))]
    }
    if options.method == 17:
        for chan in ["et", "mt", "tt"]:    
            background_schemes[chan].remove(backgroundComp("QCD", ["QCD"], ROOT.TColor.GetColor(250,202,255)))
            background_schemes[chan].insert(1,backgroundComp("j#rightarrow#tau",["FakeTaus"],ROOT.TColor.GetColor(250,202,255)))
        
    total_datahist = infile.Get(nodename+'/data_obs').Clone()
    
    blind_datahist = total_datahist.Clone()
    total_datahist.SetMarkerStyle(20)
    blind_datahist.SetMarkerStyle(20)
    blind_datahist.SetLineColor(1)
    
    #Blinding by hand using requested range, set to 200-4000 by default:
    if options.blind:
        for i in range(0,total_datahist.GetNbinsX()):
          low_edge = total_datahist.GetBinLowEdge(i+1)
          high_edge = low_edge+total_datahist.GetBinWidth(i+1)
          if ((low_edge > float(options.x_blind_min) and low_edge < float(options.x_blind_max)) or (high_edge > float(options.x_blind_min) and high_edge<float(options.x_blind_max))):
            blind_datahist.SetBinContent(i+1,0)
            blind_datahist.SetBinError(i+1,0)
    if options.norm_bins:
        blind_datahist.Scale(1.0,"width")
        total_datahist.Scale(1.0,"width")
        
    #Create stacked plot for the backgrounds
    bkg_histos = []
    for i,t in enumerate(background_schemes[options.channel]):
        plots = t['plot_list']
        h = ROOT.TH1F()
        for j,k in enumerate(plots):
            if h.GetEntries()==0:
                h = infile.Get(nodename+'/'+k).Clone()
                
                h.SetName(k)
            else:
                h.Add(infile.Get(nodename+'/'+k).Clone())
        h.SetFillColor(t['colour'])
        h.SetLineColor(ROOT.kBlack)
        h.SetMarkerSize(0)
    
        if options.norm_bins:
            h.Scale(1.0,"width")
        bkg_histos.append(h)
        
    stack = ROOT.THStack("hs","")
    bkghist = ROOT.TH1F()
    for hists in bkg_histos:
      stack.Add(hists.Clone())
      if bkghist.GetEntries()==0:
          bkghist = hists.Clone()
      else:
          bkghist.Add(hists.Clone())
      
    c1 = ROOT.TCanvas()
    c1.cd()    
    
    if options.ratio:
        pads=plotting.TwoPadSplit(0.29,0.01,0.01)
    else:
        pads=plotting.OnePad()
    pads[0].cd()
    
    if(options.log_y): pads[0].SetLogy(1)
    if(options.log_x): pads[0].SetLogx(1)
    if options.custom_x_range:
        if options.x_axis_max > bkghist.GetXaxis().GetXmax(): options.x_axis_max = bkghist.GetXaxis().GetXmax()
    if options.ratio:
        if(options.log_x): pads[1].SetLogx(1)
        axish = createAxisHists(2,bkghist,bkghist.GetXaxis().GetXmin(),bkghist.GetXaxis().GetXmax()-0.01)
        axish[1].GetXaxis().SetTitle(options.x_title)
        axish[1].GetXaxis().SetLabelSize(0.03)
        axish[1].GetYaxis().SetNdivisions(4)
        axish[1].GetYaxis().SetTitle("Obs/Exp")
        axish[1].GetYaxis().SetTitleOffset(1.6)
        axish[1].GetYaxis().SetTitleSize(0.04)
        axish[1].GetYaxis().SetLabelSize(0.03)
    
        axish[0].GetXaxis().SetTitleSize(0)
        axish[0].GetXaxis().SetLabelSize(0)
        if options.custom_x_range:
          axish[0].GetXaxis().SetRangeUser(options.x_axis_min,options.x_axis_max-0.01)
          axish[1].GetXaxis().SetRangeUser(options.x_axis_min,options.x_axis_max-0.01)
        if options.custom_y_range:
          axish[0].GetYaxis().SetRangeUser(options.y_axis_min,options.y_axis_max)
          axish[1].GetYaxis().SetRangeUser(options.y_axis_min,options.y_axis_max)
    else:
        axish = createAxisHists(1,bkghist,bkghist.GetXaxis().GetXmin(),bkghist.GetXaxis().GetXmax()-0.01)
        if options.custom_x_range:
          axish[0].GetXaxis().SetRangeUser(options.x_axis_min,options.x_axis_max-0.01)
        if options.custom_y_range:                                                                
          axish[0].GetYaxis().SetRangeUser(options.y_axis_min,options.y_axis_max)
    axish[0].GetYaxis().SetTitle(options.y_title)
    axish[0].GetYaxis().SetTitleOffset(1.6)
    axish[0].GetYaxis().SetTitleSize(0.04)
    axish[0].GetYaxis().SetLabelSize(0.03)
    axish[0].GetXaxis().SetTitle(options.x_title)
    axish[1].GetXaxis().SetTitleSize(0.04)
    if not options.ratio: axish[0].GetXaxis().SetLabelSize(0.03)
    if not options.custom_y_range:
        if(options.log_y): 
            axish[0].SetMinimum(0.0009)
            print 1.1*bkghist.GetMaximum()
            print axish[0].GetMinimum()
            axish[0].SetMaximum(10**((1+options.extra_pad)*(math.log10(1.1*bkghist.GetMaximum() - math.log10(axish[0].GetMinimum())))))
        else: 
            axish[0].SetMinimum(0)
            axish[0].SetMaximum(1.1*(1+options.extra_pad)*bkghist.GetMaximum())
    axish[0].Draw()
    
    #Draw uncertainty band
    bkghist.SetFillColor(plotting.CreateTransparentColor(12,0.4))
    bkghist.SetLineColor(plotting.CreateTransparentColor(12,0.4))
    bkghist.SetMarkerSize(0)
    bkghist.SetMarkerColor(plotting.CreateTransparentColor(12,0.4))
    
    sighist = ROOT.TH1F()
    if options.draw_signal_mass != "":
        scheme = sig_schemes[options.signal_scheme]
        for i in scheme[1]: 
            h = infile.Get(nodename+'/'+i+options.draw_signal_mass).Clone()
            if sighist.GetEntries() == 0: sighist = h
            else: sighist.Add(h)
        sighist.SetLineColor(ROOT.kBlue)
        sighist.SetLineWidth(3)
        sighist.Scale(options.signal_scale)
        if options.norm_bins: sighist.Scale(1.0,"width")
        if scheme[2]: 
            stack.Add(sighist.Clone())
            if not options.custom_y_range: axish[0].SetMaximum(1.1*(1+options.extra_pad)*stack.GetMaximum())
        stack.Draw("histsame")
        if not scheme[2]: sighist.Draw("histsame")
        
    else:
        stack.Draw("histsame")
        
    ## Add another signal mass point
    #sighist2 = ROOT.TH1F()
    #sighist2 = ana.nodes[nodename].nodes["ggH200"].shape.hist.Clone()
    #sighist2.SetLineColor(ROOT.kRed)
    #sighist2.SetLineWidth(3)
    #sighist2.Scale(options.signal_scale)
    #if options.norm_bins: sighist2.Scale(1.0,"width")
    #sighist2.Draw("histsame")
    error_hist = bkghist.Clone()
    if options.do_custom_uncerts:
      bkg_uncert_up = infile.Get(nodename+'/total_bkg_custom_uncerts_up').Clone()
      bkg_uncert_down = infile.Get(nodename+'/total_bkg_custom_uncerts_down').Clone()
      for i in range(1,bkg_uncert_up.GetNbinsX()+1): 
          stat_error=error_hist.GetBinError(i)
          bin_up = bkg_uncert_up.GetBinContent(i)
          bin_down = bkg_uncert_down.GetBinContent(i)
          error = abs(bin_up - bin_down)/2
          if options.add_stat_to_syst: error = math.sqrt(error**2+stat_error**2)
          band_center = abs(max(bin_up,bin_down) - error)          
          error_hist.SetBinContent(i,band_center)
          error_hist.SetBinError(i,error)
          
    if options.add_flat_uncert > 0:
      for i in range(1,error_hist.GetNbinsX()+1): 
          stat_error=error_hist.GetBinError(i)
          error = options.add_flat_uncert*error_hist.GetBinContent(i)
          error = math.sqrt(error**2+stat_error**2)
          error_hist.SetBinError(i,error)
    
    error_hist.Draw("e2same")
    blind_datahist.Draw("E same")
    axish[0].Draw("axissame")
    
    #Setup legend
    legend = plotting.PositionedLegend(0.30,0.30,3,0.03)
    legend.SetTextFont(42)
    legend.SetTextSize(0.022)
    legend.SetFillColor(0)
    legend.AddEntry(blind_datahist,"Observation","PE")
    #Drawn on legend in reverse order looks better
    bkg_histos.reverse()
    background_schemes[options.channel].reverse()
    for legi,hists in enumerate(bkg_histos):
        legend.AddEntry(hists,background_schemes[options.channel][legi]['leg_text'],"f")
    if options.do_custom_uncerts and options.uncert_title != "": legend.AddEntry(error_hist,options.uncert_title,"f")
    else: legend.AddEntry(error_hist,"Background uncertainty","f")
    if options.draw_signal_mass != "":
        legend.AddEntry(sighist,sig_schemes[options.signal_scheme][0],"l")
    ## Add a second signal mass
    #legend.AddEntry(sighist2,str(int(options.signal_scale))+"#times gg#phi(200 GeV)#rightarrow#tau#tau","l")  
    legend.Draw("same")
    if options.channel == "em": channel_label = "e#mu"
    if options.channel == "et": channel_label = "e#tau"
    if options.channel == "mt": channel_label = "#mu#tau"
    if options.channel == "tt": channel_label = "#tau#tau"
    if options.channel == "zmm": channel_label = "z#mu#mu"
    if options.channel == "zee": channel_label = "zee"
    latex2 = ROOT.TLatex()
    latex2.SetNDC()
    latex2.SetTextAngle(0)
    latex2.SetTextColor(ROOT.kBlack)
    latex2.SetTextSize(0.028)
    latex2.DrawLatex(0.145,0.955,channel_label)
    
    #CMS and lumi labels
    plotting.FixTopRange(pads[0], plotting.GetPadYMax(pads[0]), options.extra_pad if options.extra_pad>0 else 0.30)
    plotting.DrawCMSLogo(pads[0], 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 1.0)
    plotting.DrawTitle(pads[0], options.lumi, 3)
    
    #Add ratio plot if required
    if options.ratio:
        ratio_bkghist = plotting.MakeRatioHist(error_hist.Clone(),error_hist.Clone(),True,False)
        blind_ratio = plotting.MakeRatioHist(blind_datahist.Clone(),bkghist.Clone(),True,False)
        pads[1].cd()
        pads[1].SetGrid(0,1)
        axish[1].Draw("axis")
        axish[1].SetMinimum(float(options.ratio_range.split(',')[0]))
        axish[1].SetMaximum(float(options.ratio_range.split(',')[1]))
        ratio_bkghist.SetMarkerSize(0)
        ratio_bkghist.Draw("e2same")
        blind_ratio.DrawCopy("e0same")
        pads[1].RedrawAxis("G")
        
    pads[0].cd()
    pads[0].GetFrame().Draw()
    pads[0].RedrawAxis()
    
    if options.datacard != "": plot_name = options.outputfolder+'/'+var_name+'_'+options.datacard+'_'+options.channel+'_'+options.year
    else: plot_name = options.outputfolder+'/'+var_name+'_'+options.cat+'_'+options.channel+'_'+options.year
    if(options.log_y): plot_name+="_logy"
    if(options.log_x): plot_name+="_logx"
    c1.SaveAs(plot_name+'.pdf')
    c1.SaveAs(plot_name+'.png')
    
def FixBins(ana,outfile='output.root'):
    #Fix negative bins, empty histograms and empty bins
    nodes = ana.nodes[nodename].SubNodes()
    for node in nodes:
        hist = node.shape.hist
        outfile.cd(nodename)
        #Fix negative bins
        write_hist=False
        for i in range(1,hist.GetNbinsX()+1):
            if hist.GetBinContent(i) < 0:
                hist.SetBinContent(i,0.0000001)
                write_hist=True
        #Fix empty histogram
        if hist.Integral() == 0.0:
            hist.SetBinContent(hist.GetNbinsX()/2, 0.00001)
            write_hist=True
        #Fix empty bins
        first_populated = 0
        last_populated = 0
        bins = hist.GetNbinsX()
        for i in range(1,bins):
            if hist.GetBinContent(i) > 0 and first_populated == 0: first_populated = i
            if hist.GetBinContent(bins-(i-1)) > 0. and last_populated == 0: last_populated = bins-(i-1)
        av_weight = (hist.Integral() / hist.GetEntries())
        for i in range (first_populated+1,last_populated):
            if hist.GetBinContent(i) == 0.0: 
             hist.SetBinError(i, av_weight)
             write_hist=True
        if write_hist: hist.Write(node.name,ROOT.TObject.kOverwrite)
        outfile.cd()
    
def GetTotals(ana,add_name="",outfile='outfile.root'):
    # add histograms to get totals for backgrounds split into real/fake taus and make a total backgrounds histogram
    outfile.cd(nodename)
    nodes = ana.nodes[nodename].SubNodes()
    nodenames=[]
    for node in nodes: nodenames.append(node.name)
    for i in ['TT', 'VV', 'Z']:
        j = 'T'
        outname = i+add_name
        first_hist=True
        if options.channel == 'em' and i is 'Z':
            if first_hist and 'ZLL'+add_name in nodenames: 
                sum_hist = ana.nodes[nodename].nodes['ZLL'+add_name].shape.hist.Clone()
                first_hist=False
            elif 'ZLL'+add_name in nodenames: sum_hist.Add(ana.nodes[nodename].nodes['ZLL'+add_name].shape.hist.Clone())
            if first_hist and'ZTT'+add_name in nodenames: 
                sum_hist = ana.nodes[nodename].nodes['ZTT'+add_name].shape.hist.Clone()
                first_hist=False
            elif 'ZTT'+add_name in nodenames: sum_hist.Add(ana.nodes[nodename].nodes['ZTT'+add_name].shape.hist.Clone())
            if not first_hist:
                sum_hist.SetName(outname)
                sum_hist.Write()
        elif (options.channel == 'zee' or options.channel == 'zmm') and i is 'Z':
            if first_hist and 'ZLL'+add_name in nodenames:
                sum_hist = ana.nodes[nodename].nodes['ZLL'+add_name].shape.hist.Clone()
                first_hist=False
            elif 'ZLL'+add_name in nodenames: sum_hist.Add(ana.nodes[nodename].nodes['ZLL'+add_name].shape.hist.Clone())
            if not first_hist:
                sum_hist.SetName(outname)
                sum_hist.Write()
        else: 
            if i is 'Z':
                outname = 'ZLL'+add_name
                j = 'L'
            if i+'J' or i+j in [node.name for node in nodes]:
                if first_hist and i+'J'+add_name in nodenames: 
                    sum_hist = ana.nodes[nodename].nodes[i+'J'+add_name].shape.hist.Clone()
                    first_hist=False
                elif i+'J'+add_name in nodenames: sum_hist.Add(ana.nodes[nodename].nodes[i+'J'+add_name].shape.hist.Clone())
                if first_hist and i+j+add_name in nodenames:
                    sum_hist = ana.nodes[nodename].nodes[i+j+add_name].shape.hist.Clone()    
                    first_hist=False
                elif i+j+add_name in nodenames: sum_hist.Add(ana.nodes[nodename].nodes[i+j+add_name].shape.hist.Clone())
            if not first_hist:    
                sum_hist.SetName(outname)
                sum_hist.Write()

    first_hist=True
    for node in nodes:
        if True not in [node.name.find(sig) != -1 for sig in signal_samples.keys()] and node.name != 'data_obs' and node.name.find("_SM"+options.add_sm_background) ==-1:
            if first_hist:
                total_bkg = ana.nodes[nodename].nodes[node.name].shape.hist.Clone()
                first_hist=False
            else: total_bkg.Add(ana.nodes[nodename].nodes[node.name].shape.hist.Clone())
    if not first_hist:        
        total_bkg.SetName('total_bkg'+add_name)
        total_bkg.Write()
    outfile.cd()
    
def RunPlotting(ana, cat='', sel='', add_name='', wt='wt', do_data=True, samples_to_skip=[], outfile='output.root'):
    doTTJ = 'TTJ' not in samples_to_skip
    doTTT = 'TTT' not in samples_to_skip
    doVVJ = 'VVJ' not in samples_to_skip
    doVVT = 'VVT' not in samples_to_skip
    
    # produce template for observed data
    if do_data:
        if options.do_ss:
          OSSS = '!os'
        else:
            OSSS = 'os'
        full_selection = BuildCutString('wt', sel, cat, OSSS)
        ana.nodes[nodename].AddNode(ana.SummedFactory('data_obs', data_samples, plot, full_selection))
    
    # produce templates for backgrounds
    if options.method == 17 and options.channel != "em":
        # method 17 uses the fake factor method!
        GenerateFakeTaus(ana, add_name, data_samples, plot, wt, sel, options.cat,not options.do_ss)
        
        # use existing methods to calculate background due to non-fake taus - for W background must use method 8 to compute this!
        add_fake_factor_selection = "gen_match_2<6"
        if options.channel == "tt": add_fake_factor_selection = "!(gen_match_1==6 || gen_match_2==6)"
        residual_cat=cat+"&&"+add_fake_factor_selection
        
        if 'ZTT' not in samples_to_skip and options.channel != 'zee' and options.channel != 'zmm':
            GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, residual_cat, z_sels, not options.do_ss)                                
        if 'ZLL' not in samples_to_skip:
            GenerateZLL(ana, add_name, ztt_samples, plot, wt, sel, residual_cat, z_sels, not options.do_ss)
        if 'TT' not in samples_to_skip:    
            GenerateTop(ana, add_name, top_samples, plot, wt, sel, residual_cat, top_sels, not options.do_ss, doTTT, doTTJ)  
        if 'VV' not in samples_to_skip:
            GenerateVV(ana, add_name, vv_samples, plot, wt, sel, residual_cat, vv_sels, not options.do_ss, doVVT, doVVJ)  
        if 'W' not in samples_to_skip:
            GenerateW(ana, 'W', add_name, wjets_samples, data_samples, w_sub_samples, wgam_samples, plot, wt, sel, residual_cat, 8, qcd_os_ss_ratio, not options.do_ss)
    
    else:
        if 'ZTT' not in samples_to_skip and options.channel != 'zee' and options.channel != 'zmm':
            GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels, not options.do_ss)                                
        if 'ZLL' not in samples_to_skip:
            GenerateZLL(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels, not options.do_ss)
        if 'TT' not in samples_to_skip:    
            GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels, not options.do_ss, doTTT, doTTJ)  
        if 'VV' not in samples_to_skip:
            GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels, not options.do_ss, doVVT, doVVJ)  
        if 'W' not in samples_to_skip:
            GenerateW(ana, 'W', add_name, wjets_samples, data_samples, w_sub_samples, wgam_samples, plot, wt, sel, cat, options.method, qcd_os_ss_ratio, not options.do_ss)
        if 'QCD' not in samples_to_skip:
            GenerateQCD(ana, add_name, data_samples, qcd_sub_samples, w_sub_samples, plot, wt, sel, cat, options.method, qcd_os_ss_ratio, not options.do_ss)
            
    if 'signal' not in samples_to_skip:
        if options.analysis == 'sm':
            GenerateSMSignal(ana, add_name, plot, sm_masses, wt, sel, cat, not options.do_ss)
        elif options.analysis == 'mssm':
            GenerateMSSMSignal(ana, add_name, plot, ggh_masses, bbh_masses, wt, sel, cat, not options.do_ss)
            if options.add_sm_background:
                GenerateSMSignal(ana, add_name, plot, ['125'],  wt, sel, cat, not options.do_ss, options.add_sm_background)  
        elif options.analysis == 'Hhh':
            GenerateHhhSignal(ana, add_name, plot, ggh_masses, wt, sel, cat, not options.do_ss)
            
    ana.Run()
    ana.nodes.Output(outfile)
    # fix negative bns,empty histograms etc.
    FixBins(ana,outfile)
    # add histograms to get totals for backgrounds split into real/fake taus and make a total backgrounds histogram
    GetTotals(ana,add_name,outfile)


# Create output file
var_name = options.var.split('[')[0]
var_name = var_name.split('(')[0]

if options.datacard != "": datacard_name = options.datacard
else: datacard_name = options.cat
output_name = options.outputfolder+'/datacard_'+var_name+'_'+datacard_name+'_'+options.channel+'_'+options.year+'.root'
outfile = ROOT.TFile(output_name, 'RECREATE')

for systematic in systematics:
    
    print "Processing:", systematic
    print ""
    
    add_folder_name = systematics[systematic][0]
    add_name = systematics[systematic][1]
    weight = systematics[systematic][2]
    samples_to_skip = systematics[systematic][3]
    
    ana = Analysis()
    
    ana.remaps = {}
    if options.channel == 'em':
        ana.remaps['MuonEG'] = 'data_obs'
    elif options.channel == 'mt' or options.channel == 'zmm':
        ana.remaps['SingleMuon'] = 'data_obs'
    elif options.channel == 'et' or options.channel == 'zee':
        ana.remaps['SingleElectron'] = 'data_obs'
    elif options.channel == 'tt':
        ana.remaps['Tau'] = 'data_obs'
    
    mc_input_folder_name = options.folder
    if add_folder_name != '':
        mc_input_folder_name += '/'+add_folder_name
        
    # Add all data files
    for sample_name in data_samples:
        ana.AddSamples(options.folder+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
    
    # Add all MC background files
    for sample_name in ztt_samples + vv_samples + wgam_samples + top_samples + ztt_shape_samples + qcd_sub_samples + w_sub_samples + wjets_samples:
        ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
     
    # Add all MC signal files
    
    if options.analysis == 'sm':
        signal_samples = sm_samples
    elif options.analysis == 'mssm':
        signal_samples = mssm_samples
    elif options.analysis == 'Hhh':
        signal_samples = Hhh_samples

    for samp in signal_samples:
        if options.analysis == "sm":
            masses=sm_masses
        elif samp == 'ggH':
            masses = ggh_masses
        else:
            masses = bbh_masses
        for mass in masses:
            sample_name = signal_samples[samp]+'_M-'+mass
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
    
    if options.add_sm_background and options.analysis == 'mssm':
        for samp in sm_samples:
            sample_name = sm_samples[samp]+'_M-'+options.add_sm_background
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
            
    ana.AddInfo(options.paramfile, scaleTo='data_obs')
    
    cat = '('+cats[options.cat]+')*('+cats['baseline']+')'
    sel = options.sel
    plot = options.var
    if options.datacard != "": nodename = options.channel+'_'+options.datacard
    else: nodename = options.channel+'_'+options.cat
    
    ana.nodes.AddNode(ListNode(nodename))
    if options.do_custom_uncerts:
      ana_up   = Analysis()
      ana_down = Analysis()
      ana_up = copy.deepcopy(ana)
      ana_down = copy.deepcopy(ana)
    
    # Add data only for default
    if systematic == 'default': do_data = True
    else: do_data = False
            
    #Run default plot        
    RunPlotting(ana, cat, sel, add_name, weight, do_data, samples_to_skip,outfile)
    
    if options.do_custom_uncerts:
        RunPlotting(ana_up, cat, sel, '_custom_uncerts_up', weight+'*'+options.custom_uncerts_wt_up, do_data, ['signal'],outfile)
        RunPlotting(ana_down, cat, sel, '_custom_uncerts_down', weight+'*'+options.custom_uncerts_wt_down, do_data, ['signal'],outfile)
    
    PrintSummary(nodename, ['data_obs'], add_name)
    
    # When adding signal samples to the data-card we want to scale all XS to 1pb - correct XS times BR is then applied at combine harvestor level 
    if 'signal' not in samples_to_skip:
        outfile.cd(nodename)
        if options.analysis == "sm" or options.add_sm_background:
            if options.analysis == "sm":
                masses = sm_masses
            else:
                masses = [options.add_sm_background]
            for samp in sm_samples:
                if options.analysis == "sm":
                    samp_name = samp
                else:
                    samp_name = samp+"_SM"
                for mass in masses:
                    xs = ana.info[sm_samples[samp]+'_M-'+mass]['xs']
                    sf = 1.0/xs
                    sm_hist = ana.nodes[nodename].nodes[samp_name+mass+add_name].shape.hist
                    sm_hist.Scale(sf)
                    sm_hist.Write()
        if options.analysis == "mssm":
            for samp in mssm_samples:
                if samp == 'ggH':
                    masses = ggh_masses
                elif samp == 'bbH':
                    masses = bbh_masses
                for mass in masses:
                    xs = ana.info[mssm_samples[samp]+'_M-'+mass]['xs']
                    sf = 1.0/xs
                    mssm_hist = ana.nodes[nodename].nodes[samp+mass+add_name].shape.hist
                    mssm_hist.Scale(sf)
                    mssm_hist.Write()
        if options.analysis == "Hhh":
            for samp in Hhh_samples:
                masses = ggh_masses
                for mass in masses:
                    xs = ana.info[Hhh_samples[samp]+'_M-'+mass]['xs']
                    sf = 1.0/xs
                    mssm_hist = ana.nodes[nodename].nodes[samp+mass+add_name].shape.hist
                    mssm_hist.Scale(sf)
                    mssm_hist.Write()
        outfile.cd()
outfile.Close()
plot_file = ROOT.TFile(output_name, 'READ')
if not options.no_plot:
    Plot(nodename,plot_file)                    
            
    
                

