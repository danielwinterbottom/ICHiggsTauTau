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

CHANNELS= ['et', 'mt', 'em','tt','zmm','zee','mj']
ANALYSIS= ['sm','mssm','Hhh']
METHODS= [8 ,9, 10, 11, 12 , 13, 14, 15, 16, 17, 18, 19, 20]

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = { "channel":"mt" , "outputfolder":"output", "folder":"/vols/cms/dw515/Offline/output/MSSM/Jan11/" , "paramfile":"scripts/Params_2016_spring16.json", "cat":"inclusive", "year":"2016", "era":"mssmsummer16", "sel":"(1)", "set_alias":[], "analysis":"mssm", "var":"m_vis(7,0,140)", "method":8 , "do_ss":False, "sm_masses":"125", "ggh_masses":"", "bbh_masses":"", "bbh_nlo_masses":"", "nlo_qsh":False, "qcd_os_ss_ratio":-1, "add_sm_background":"", "syst_tau_scale":"", "syst_tau_scale_0pi":"", "syst_tau_scale_1pi":"", "syst_tau_scale_3prong":"", "syst_eff_t":"", "syst_tquark":"", "syst_zwt":"", "syst_w_fake_rate":"", "syst_scale_j":"", "syst_eff_b":"",  "syst_fake_b":"" ,"norm_bins":False, "blind":False, "x_blind_min":100, "x_blind_max":4000, "ratio":False, "y_title":"", "x_title":"", "custom_y_range":False, "y_axis_min":0.001, "y_axis_max":100,"custom_x_range":False, "x_axis_min":0.001, "x_axis_max":100, "log_x":False, "log_y":False, "extra_pad":0.0, "signal_scale":1, "draw_signal_mass":"", "draw_signal_tanb":10, "signal_scheme":"run2_mssm", "lumi":"12.9 fb^{-1} (13 TeV)", "no_plot":False, "ratio_range":"0.7,1.3", "datacard":"", "do_custom_uncerts":False, "uncert_title":"Systematic uncertainty", "custom_uncerts_wt_up":"","custom_uncerts_wt_down":"", "add_flat_uncert":0, "add_stat_to_syst":False, "add_wt":"", "custom_uncerts_up_name":"", "custom_uncerts_down_name":"", "do_ff_systs":False, "syst_efake_0pi_scale":"", "syst_efake_1pi_scale":"", "scheme":"", "syst_zpt_es":"", "syst_zpt_tt":"", "syst_zpt_statpt0":"", "syst_zpt_statpt40":"", "syst_zpt_statpt80":"", "syst_jfake_m":"", "syst_jfake_e":"","doNLOScales":False, "gen_signal":False, "doPDF":False }

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
parser.add_argument("--bbh_nlo_masses", dest="bbh_nlo_masses", type=str,
    help="Comma seperated list of SUSY NLO bbH signal masses.")
parser.add_argument("--nlo_qsh", dest="nlo_qsh", action='store_true',
    help="Do the Up/Down Qsh variations for NLO samples.")
parser.add_argument("--doNLOScales", dest="doNLOScales", action='store_true',
    help="Do the Up/Down QCD scale variations for NLO samples and compute uncertainties.")
parser.add_argument("--doPDF", dest="doPDF", action='store_true',
    help="Do PDF and alphaS variations for NLO samples and compute uncertainties.")
parser.add_argument("--bbh_masses", dest="bbh_masses", type=str,
    help="Comma seperated list of SUSY bbH signal masses.")
parser.add_argument("--qcd_os_ss_ratio", dest="qcd_os_ss_ratio", type=float,
    help="QCD OS/SS ratio")
parser.add_argument("--add_sm_background", dest="add_sm_background", type=str,
    help="Add SM Higgs background for MSSM")
parser.add_argument("--syst_tau_scale", dest="syst_tau_scale", type=str,
    help="If this string is set then the systematic shift due to tau energy scale is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_tau_scale_0pi", dest="syst_tau_scale_0pi", type=str,
    help="If this string is set then the systematic shift due to the 1 prong 0 pi tau energy scale is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_tau_scale_1pi", dest="syst_tau_scale_1pi", type=str,
    help="If this string is set then the systematic shift due to the 1 prong 1 pi tau energy scale is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_tau_scale_3prong", dest="syst_tau_scale_3prong", type=str,
    help="If this string is set then the systematic shift due to 3 prong tau energy scale is performed with the set string appended to the resulting histogram name")
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
parser.add_argument("--custom_uncerts_down_name", dest="custom_uncerts_down_name", type=str,
    help="Name of histogram to use for uncertainty down band")
parser.add_argument("--custom_uncerts_up_name", dest="custom_uncerts_up_name", type=str,
    help="Name of histogram to use for uncertainty up band")
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
parser.add_argument("--add_wt", dest="add_wt", type=str,
    help="Name of additional weight to be applied to all templates.")
parser.add_argument("--do_ff_systs", dest="do_ff_systs", action='store_true',
    help="Do fake-factor systamatic shifts.")
parser.add_argument("--syst_efake_0pi_scale", dest="syst_efake_0pi_scale", type=str,
    help="If this string is set then the e->tau dm=0 fake-rate systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_efake_1pi_scale", dest="syst_efake_1pi_scale", type=str,
    help="If this string is set then the e->tau dm=1 fake-rate systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--scheme", dest="scheme", type=str,
    help="Set plotting scheme")
parser.add_argument("--syst_zpt_es", dest="syst_zpt_es", type=str,
    help="If this string is set then the zpT muon ES systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_zpt_tt", dest="syst_zpt_tt", type=str,
    help="If this string is set then the zpT tt X-section systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_zpt_statpt0", dest="syst_zpt_statpt0", type=str,
    help="If this string is set then the zpT statistical pt0 systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_zpt_statpt40", dest="syst_zpt_statpt40", type=str,
    help="If this string is set then the zpT statistical pt40 systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_zpt_statpt80", dest="syst_zpt_statpt80", type=str,
    help="If this string is set then the zpT statistical pt80 systematic is performed with the set string appended to the resulting histogram name")
parser.add_argument("--syst_jfake_e", dest="syst_jfake_e", type=str,
    help="If set, adds the e->jet fake rate uncertainty with the set string appended to the resulting histogram name")
parser.add_argument("--syst_jfake_m", dest="syst_jfake_m", type=str,
    help="If set, adds the e->jet fake rate uncertainty with the set string appended to the resulting histogram name")
parser.add_argument("--gen_signal", dest="gen_signal", action='store_true',
    help="If set then use generator-level tree for signal")


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
print 'do_ff_systs       ='  ,  options.do_ff_systs
print '###############################################'
print ''

compare_w_shapes = False
compare_qcd_shapes = False
if options.scheme == "qcd_shape": compare_qcd_shapes = True
if options.scheme == "w_shape": compare_w_shapes = True
if options.era == "mssmsummer16": options.lumi = "35.9 fb^{-1} (13 TeV)"


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
            cats['baseline'] = '(iso_1<0.15 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
            cats['baseline_antiisotau'] = '(iso_1<0.15 && 1 && mva_olddm_tight_2<0.5 && antiele_2 && antimu_2 && !leptonveto && trg_singlemuon)'
            cats['ichep_baseline'] = '(iso_1<0.15 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto && trg_singlemuon)'
        elif options.channel == 'et':
            cats['baseline'] = '(iso_1<0.1  && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
            cats['baseline_antiisotau'] = '(iso_1<0.1 && mva_olddm_tight_2<0.5 && antiele_2 && antimu_2 && !leptonveto && trg_singleelectron)'
            cats['ichep_baseline'] = '(iso_1<0.1 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto && trg_singleelectron)'
        elif options.channel == 'mj':        
            cats['baseline'] = '(iso_1<0.15 && !leptonveto)'
if options.channel == 'tt':
    cats['baseline'] = '(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
    if options.era == 'mssmsummer16': cats['baseline'] = '(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
elif options.channel == 'em':
    cats['baseline'] = '(iso_1<0.15 && iso_2<0.2 && !leptonveto)'
    cats['loose_baseline'] = '(iso_1<0.5 && iso_2>0.2 && iso_2<0.5 && !leptonveto &&trg_muonelectron)'
elif options.channel == 'zmm':
    cats['baseline'] = '(iso_1<0.15 && iso_2<0.15)'
elif options.channel == 'zee':
    cats['baseline'] = '(iso_1<0.1 && iso_2<0.1)'
    
cats['inclusive'] = '(1)' 
cats['w_os'] = 'os'
cats['w_sdb'] = 'mt_1>70.'
cats['w_sdb_os'] = 'os'
cats['tt_qcd_norm'] = '(mva_olddm_tight_1>0.5 && mva_olddm_medium_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)&&trg_doubletau'
if options.era == 'mssmsummer16': cats['tt_qcd_norm'] = '(mva_olddm_medium_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)&&trg_doubletau'
cats['qcd_loose_shape'] = '(iso_1>0.2 && iso_1<0.5 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'

# MSSM categories
cats['btag'] = '(n_bjets>=1)'
cats['nobtag'] = '(n_bjets==0)'
# loose/tight iso-MT categories
cats['nobtag_tight'] = cats['nobtag']
cats['nobtag_loosemt'] = cats['nobtag']
cats['btag_tight'] = cats['btag']
cats['btag_loosemt'] = cats['btag']
cats['atleast1bjet'] = '(n_bjets>0)'
cats['btag_wnobtag']='(n_lowpt_jets>=1)' # this is the one that is used for the b-tag method 16!
cats['0jet'] = '(n_jets==0)'
cats['1jet'] = '(n_jets==1)'
cats['ge2jet'] = '(n_jets>=2)'
cats['btag_tight_wnobtag']='(n_lowpt_jets>=1)'
cats['w_shape']=''
cats['qcd_shape']=''
cats['w_shape_comp']=''
cats['qcd_shape_comp']=''


# Overwrite selection depending on whether tight or loose-mt categories is chosen - this can still be overwritten from command line using the --set_alias=sel:(...) option
if options.cat == 'nobtag_tight' or options.cat == 'btag_tight':
    if options.channel == 'mt' or options.channel == 'et': options.sel = '(mt_1<40)'
if options.cat == 'nobtag_loosemt' or options.cat == 'btag_loosemt':
    if options.channel == 'mt' or options.channel == 'et': options.sel = '(mt_1<70 && mt_1>40)'

if options.era == "mssmsummer16":
    if options.channel == "em": cats['baseline']+=" && trg_muonelectron"
    if options.channel == "et" or options.channel == 'zee': cats['baseline']+=" && trg_singleelectron"
    if options.channel in ['mt','zmm','mj']: cats['baseline']+=" && trg_singlemuon"
    if options.channel == "tt": cats['baseline']+=" && trg_doubletau"

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

# Additional selections to seperate MC samples by gen flags

z_sels = {}
if options.channel == 'et':
    z_sels['ztt_sel'] = '(gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<5)'
    z_sels['zj_sel'] = '(gen_match_2==6)'
elif options.channel in ['mt','mj']:
    z_sels['ztt_sel'] = '(gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<5)'
    z_sels['zj_sel'] = '(gen_match_2==6)'
elif options.channel in ['mj']:
    z_sels['ztt_sel'] = '(0)'
    z_sels['zl_sel'] = '(0)'
    z_sels['zj_sel'] = '(1)'
elif options.channel == 'tt':
    z_sels['ztt_sel'] = '(gen_match_1==5&&gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<6&&gen_match_1<6&&!(gen_match_1==5&&gen_match_2==5))'
    z_sels['zj_sel'] = '(gen_match_2==6||gen_match_1==6)'
elif options.channel == 'em':
    z_sels['ztt_sel'] = '(gen_match_1>2 && gen_match_2>3)'
    z_sels['zll_sel'] = '(gen_match_1<3 || gen_match_2<4)'
elif options.channel == 'zee' or  options.channel == 'zmm':
    z_sels['ztt_sel'] = '(gen_match_1>2&&gen_match_1<6 && gen_match_2>2&&gen_match_2<6)'
    if options.channel == 'zmm': z_sels['zl_sel'] = '(gen_match_1==2&&gen_match_2==2)'
    else: z_sels['zl_sel'] = '(gen_match_1==1&&gen_match_2==1)'
    z_sels['zj_sel'] = '(!('+z_sels['zl_sel']+') && !('+z_sels['ztt_sel']+'))'

top_sels = {}
vv_sels = {}
top_sels['ttt_sel'] = z_sels['ztt_sel']
top_sels['ttj_sel'] = '!('+z_sels['ztt_sel']+')'
vv_sels['vvt_sel'] = z_sels['ztt_sel']
vv_sels['vvj_sel'] = '!('+z_sels['ztt_sel']+')'

if options.channel in ['et','mt','mj']:
  vv_sels['vvt_sel'] = '(gen_match_2<6)'
  vv_sels['vvj_sel'] = '(gen_match_2==6)'
  top_sels['ttt_sel'] = '(gen_match_2<6)' 
  top_sels['ttj_sel'] = '(gen_match_2==6)'
elif options.channel == 'tt':
  vv_sels['vvt_sel'] = '(gen_match_1<6 && gen_match_2<6)'
  vv_sels['vvj_sel'] = '(!(gen_match_1<6 && gen_match_2<6))'
  top_sels['ttt_sel'] = '(gen_match_1<6 && gen_match_2<6)' 
  top_sels['ttj_sel'] = '(!(gen_match_1<6 && gen_match_2<6))'
if options.channel in ['mj']:
  vv_sels['vvt_sel'] = '(0)'
  vv_sels['vvj_sel'] = '(1)'
  top_sels['ttt_sel'] = '(0)' 
  top_sels['ttj_sel'] = '(1)'
    
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
wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']

if options.era == "mssmsummer16":
    
    # Add data sample names
    if options.channel in ['mt','zmm','mj']: 
        data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleMuonG','SingleMuonHv2','SingleMuonHv3']
    if options.channel == 'em': 
        data_samples = ['MuonEGB','MuonEGC','MuonEGD','MuonEGE','MuonEGF','MuonEGG','MuonEGHv2','MuonEGHv3']
    if options.channel == 'et' or options.channel == 'zee': 
        data_samples = ['SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','SingleElectronG','SingleElectronHv2','SingleElectronHv3']
    if options.channel == 'tt': 
        data_samples = ['TauB','TauC','TauD','TauE','TauF','TauG','TauHv2','TauHv3']
        
    # Add MC sample names   
    ztt_samples = ['DYJetsToLL-LO-ext1','DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
    vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','VVTo2L2Nu-ext1','ZZTo2L2Q','ZZTo4L-amcat','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q']
    wgam_samples = ['WGToLNuG','WGToLNuG-ext','WGstarToLNuEE','WGstarToLNuMuMu']
    top_samples = ['TT']
    ztt_shape_samples = ['DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
    wjets_samples = ['WJetsToLNu-LO', 'WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W2JetsToLNu-LO-ext','W3JetsToLNu-LO','W3JetsToLNu-LO-ext','W4JetsToLNu-LO','W4JetsToLNu-LO-ext1','W4JetsToLNu-LO-ext2']

sm_samples = { 'ggH' : 'GluGluHToTauTau_M-*', 'qqH' : 'VBFHToTauTau_M-*', 'WplusH' : 'WplusHToTauTau_M-*', 'WminusH' : 'WminusHToTauTau_M-*', 'ZH' : 'ZHToTauTau_M-*', 'TTH' : 'TTHToTauTau_M-*' }
if options.analysis == 'mssm': sm_samples = { 'ggH' : 'GluGluToHToTauTau_M-*', 'qqH' : 'VBFHToTauTau_M-*', 'WplusH' : 'WplusHToTauTau_M-*', 'WminusH' : 'WminusHToTauTau_M-*', 'ZH' : 'ZHToTauTau_M-*'}
mssm_samples = { 'ggH' : 'SUSYGluGluToHToTauTau_M-*', 'bbH' : 'SUSYGluGluToBBHToTauTau_M-*' }
mssm_nlo_samples = { 'bbH' : 'SUSYGluGluToBBHToTauTau_M-*-NLO' }
mssm_lo_samples = { 'bbH-LO' : 'SUSYGluGluToBBHToTauTau_M-*' }
mssm_nlo_qsh_samples = { 'bbH-QshUp' : 'SUSYGluGluToBBHToTauTau_M-*-NLO-QshUp', 'bbH-QshDown' : 'SUSYGluGluToBBHToTauTau_M-*-NLO-QshDown' }
if options.nlo_qsh: mssm_nlo_samples.update(mssm_nlo_qsh_samples)
Hhh_samples = { 'ggH' : 'GluGluToRadionToHHTo2B2Tau_M-*' }

# set systematics: first index sets folder name contaning systematic samples, second index sets string to be appended to output histograms, third index specifies the weight to be applied , 4th lists samples that should be skipped
systematics = OrderedDict()
systematics['default'] = ('','', 'wt', [], False)

if options.syst_tau_scale != '':
    systematics['scale_t_up'] = ('TSCALE_UP' , '_'+options.syst_tau_scale+'Up', 'wt', ['QCD','jetFakes'], False)
    systematics['scale_t_down'] = ('TSCALE_DOWN' , '_'+options.syst_tau_scale+'Down', 'wt', ['QCD','jetFakes'], False)
if options.syst_tau_scale_0pi != '':
    systematics['scale_t_0pi_up'] = ('TSCALE0PI_UP' , '_'+options.syst_tau_scale_0pi+'Up', 'wt', ['QCD','jetFakes'], False)
    systematics['scale_t_0pi_down'] = ('TSCALE0PI_DOWN' , '_'+options.syst_tau_scale_0pi+'Down', 'wt', ['QCD','jetFakes'], False)
if options.syst_tau_scale_1pi != '':
    systematics['scale_t_1pi_up'] = ('TSCALE1PI_UP' , '_'+options.syst_tau_scale_1pi+'Up', 'wt', ['QCD','jetFakes'], False)
    systematics['scale_t_1pi_down'] = ('TSCALE1PI_DOWN' , '_'+options.syst_tau_scale_1pi+'Down', 'wt', ['QCD','jetFakes'], False)
if options.syst_tau_scale_3prong != '':
    systematics['scale_t_3prong_up'] = ('TSCALE3PRONG_UP' , '_'+options.syst_tau_scale_3prong+'Up', 'wt', ['QCD','jetFakes'], False)
    systematics['scale_t_3prong_down'] = ('TSCALE3PRONG_DOWN' , '_'+options.syst_tau_scale_3prong+'Down', 'wt', ['QCD','jetFakes'], False)
if options.syst_efake_0pi_scale != '':
    systematics['scale_efake_0pi_up'] = ('EFAKE0PI_UP' , '_'+options.syst_efake_0pi_scale+'Up', 'wt', ['ZTT','VVT','VVJ','TTT','TTJ','QCD','signal','W','jetFakes'], False)
    systematics['scale_efake_0pi_down'] = ('EFAKE0PI_DOWN' , '_'+options.syst_efake_0pi_scale+'Down', 'wt', ['ZTT','VVT','VVJ','TTT','TTJ','QCD','signal','W','jetFakes'], False)
if options.syst_efake_1pi_scale != '':
    systematics['scale_efake_1pi_up'] = ('EFAKE1PI_UP' , '_'+options.syst_efake_1pi_scale+'Up', 'wt', ['ZTT','VVT','VVJ','TTT','TTJ','QCD','signal','W','jetFakes'], False)
    systematics['scale_efake_1pi_down'] = ('EFAKE1PI_DOWN' , '_'+options.syst_efake_1pi_scale+'Down', 'wt', ['ZTT','VVT','VVJ','TTT','TTJ','QCD','signal','W','jetFakes'], False)
if options.syst_eff_t != '':
    systematics['syst_eff_t_up'] = ('' , '_'+options.syst_eff_t+'Up', 'wt*wt_tau_id_up', ['ZL','ZJ','VVJ','TTJ','QCD','W'], False)
    systematics['syst_eff_t_down'] = ('' , '_'+options.syst_eff_t+'Down', 'wt*wt_tau_id_down', ['ZL','ZJ','VVJ','TTJ','QCD','W'], False)
if options.syst_tquark != '':
    systematics['syst_tquark_up'] = ('' , '_'+options.syst_tquark+'Up', 'wt*wt_tquark_up', ['ZTT','ZL','ZJ','VVT','VVJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_tquark_down'] = ('' , '_'+options.syst_tquark+'Down', 'wt*wt_tquark_down', ['ZTT','ZL','ZJ','VVJ','VVT','QCD','W', 'signal','jetFakes'], False)    
if options.syst_zwt != '':
    systematics['syst_zwt_up'] = ('' , '_'+options.syst_zwt+'Up', 'wt*wt_zpt_up', ['ZL','ZJ','VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_zwt_down'] = ('' , '_'+options.syst_zwt+'Down', 'wt*wt_zpt_down', ['ZL','ZJ','VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
if options.syst_w_fake_rate != '':
    systematics['syst_w_fake_rate_up'] = ('' , '_'+options.syst_w_fake_rate+'Up', 'wt*wt_tau_fake_up', ['ZTT','ZL','ZJ','VVT','VVJ','TTT','TTJ','QCD','signal','jetFakes'], False)
    systematics['syst_w_fake_rate_down'] = ('' , '_'+options.syst_w_fake_rate+'Down', 'wt*wt_tau_fake_down', ['ZTT','ZL','ZJ','VVT','VVJ','TTT','TTJ','QCD','signal','jetFakes'], False)
if options.syst_jfake_m != '':
    systematics['syst_jfake_m_up'] = ('' , '_'+options.syst_jfake_m+'Up', 'wt*idisoweight_up_2', ['ZTT','QCD','signal','TT'], False)
    systematics['syst_jfake_m_down'] = ('' , '_'+options.syst_jfake_m+'Down', 'wt*idisoweight_down_2', ['ZTT','QCD','signal','TT'], False)
if options.syst_jfake_e != '':
    systematics['syst_jfake_e_up'] = ('' , '_'+options.syst_jfake_e+'Up', 'wt*idisoweight_up_1', ['ZTT','QCD','signal','TT'], False)
    systematics['syst_jfake_e_down'] = ('' , '_'+options.syst_jfake_e+'Down', 'wt*idisoweight_down_1', ['ZTT','QCD','signal','TT'], False)
if options.syst_scale_j != '':
    systematics['syst_scale_j_up'] = ('JES_UP' , '_'+options.syst_scale_j+'Up', 'wt', [], False)
    systematics['syst_scale_j_down'] = ('JES_DOWN' , '_'+options.syst_scale_j+'Down', 'wt', [], False)
if options.syst_eff_b != '':
    systematics['syst_b_up'] = ('BTAG_UP' , '_'+options.syst_eff_b+'Up', 'wt', [], False)
    systematics['syst_b_down'] = ('BTAG_DOWN' , '_'+options.syst_eff_b+'Down', 'wt', [], False)
if options.syst_fake_b != '':
    systematics['syst_fake_b_up'] = ('BFAKE_UP' , '_'+options.syst_fake_b+'Up', 'wt', [], False)
    systematics['syst_fake_b_down'] = ('BFAKE_DOWN' , '_'+options.syst_fake_b+'Down', 'wt', [], False)
if options.syst_zpt_es != '':
    systematics['syst_zpt_es_up'] = ('' , '_'+options.syst_zpt_es+'Up', 'wt*wt_zpt_esup', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_zpt_es_down'] = ('' , '_'+options.syst_zpt_es+'Down', 'wt*wt_zpt_esdown', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
if options.syst_zpt_tt != '':
    systematics['syst_zpt_tt_up'] = ('' , '_'+options.syst_zpt_tt+'Up', 'wt*wt_zpt_ttup', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_zpt_tt_down'] = ('' , '_'+options.syst_zpt_tt+'Down', 'wt*wt_zpt_ttdown', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
if options.syst_zpt_statpt0 != '':
    systematics['syst_zpt_statpt0_up'] = ('' , '_'+options.syst_zpt_statpt0+'Up', 'wt*wt_zpt_stat_m400pt0_up', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_zpt_statpt0_down'] = ('' , '_'+options.syst_zpt_statpt0+'Down', 'wt*wt_zpt_stat_m400pt0_down', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
if options.syst_zpt_statpt40 != '':
    systematics['syst_zpt_statpt40_up'] = ('' , '_'+options.syst_zpt_statpt40+'Up', 'wt*wt_zpt_stat_m400pt40_up', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_zpt_statpt40_down'] = ('' , '_'+options.syst_zpt_statpt40+'Down', 'wt*wt_zpt_stat_m400pt40_down', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
if options.syst_zpt_statpt80 != '':
    systematics['syst_zpt_statpt80_up'] = ('' , '_'+options.syst_zpt_statpt80+'Up', 'wt*wt_zpt_stat_m400pt80_up', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)
    systematics['syst_zpt_statpt80_down'] = ('' , '_'+options.syst_zpt_statpt80+'Down', 'wt*wt_zpt_stat_m400pt80_down', ['VVT','VVJ','TTT','TTJ','QCD','W','signal','jetFakes'], False)

if options.method in [17,18] and options.do_ff_systs and options.channel in ['et','mt','tt']:
    processes = ['tt','w','qcd']
    dms = ['dm0', 'dm1']
    njets = ['njet0','njet1']
    for process in processes:
      template_name = 'ff_'+process+'_syst'
      if process is 'qcd' or options.channel == 'tt': template_name = 'ff_'+process+'_'+options.channel+'_syst'
      weight_name = 'wt_ff_'+options.cat+'_'+process+'_syst_'
      systematics[template_name+'_up']   = ('' , '_'+template_name+'Up',   weight_name+'up',   ['ZTT','ZJ','ZL','VVT','VVJ','TTT','TTJ','QCD','W','signal'], True)
      systematics[template_name+'_down'] = ('' , '_'+template_name+'Down', weight_name+'down', ['ZTT','ZJ','ZL','VVT','VVJ','TTT','TTJ','QCD','W','signal'], True)
      if options.channel == 'tt' and process in ['w','tt']: continue
      for dm in dms: 
        for njet in njets:
          template_name = 'ff_'+process+'_'+dm+'_'+njet
          if process != 'tt': template_name+='_'+options.channel
          template_name+='_stat'
          weight_name = 'wt_ff_'+options.cat+'_'+process+'_'+dm+'_'+njet+'_stat_'
          systematics[template_name+'_up']   = ('' , '_'+template_name+'Up',   weight_name+'up',   ['ZTT','ZJ','ZL','VVT','VVJ','TTT','TTJ','QCD','W','signal'], True)
          systematics[template_name+'_down'] = ('' , '_'+template_name+'Down', weight_name+'down', ['ZTT','ZJ','ZL','VVT','VVJ','TTT','TTJ','QCD','W','signal'], True)
    if options.channel == "tt":
      processes = ['dy', 'w', 'tt']
      for process in processes:
        template_name = 'ff_'+process+'_frac_tt_syst'
        weight_name = 'wt_ff_'+options.cat+'_'+process+'_frac_syst_'
        systematics[template_name+'_up']   = ('' , '_'+template_name+'Up',   weight_name+'up',   ['ZTT','ZJ','ZL','VVT','VVJ','TTT','TTJ','QCD','W','signal'], True)
        systematics[template_name+'_down'] = ('' , '_'+template_name+'Down', weight_name+'down', ['ZTT','ZJ','ZL','VVT','VVJ','TTT','TTJ','QCD','W','signal'], True)

if options.qcd_os_ss_ratio > 0:
    qcd_os_ss_ratio = options.qcd_os_ss_ratio
else:
    if options.channel == 'et':
        qcd_os_ss_ratio = 1.02
        if options.cat == 'inclusive': qcd_os_ss_ratio = 1.13
        elif options.cat in ['nobtag', 'nobtag_tight', 'nobtag_loosemt']: qcd_os_ss_ratio = 1.11
        elif options.cat in ['btag', 'btag_tight', 'btag_loosemt']: qcd_os_ss_ratio = 1.16
    elif options.channel in ['mt','mj']: 
        qcd_os_ss_ratio = 1.18
        if options.cat == 'inclusive': qcd_os_ss_ratio = 1.12
        elif options.cat in ['nobtag', 'nobtag_tight', 'nobtag_loosemt']: qcd_os_ss_ratio = 1.14
        elif options.cat in ['btag', 'btag_tight', 'btag_loosemt']: qcd_os_ss_ratio = 1.01
    elif options.channel == 'zmm' or options.channel == 'zee':
        qcd_os_ss_ratio = 1.06   
    else:
        qcd_os_ss_ratio = 1.0
#if options.do_ss:
#    qcd_os_ss_ratio = 1.0


# Get array of signal masses to process        
ggh_masses=None
bbh_masses=None
sm_masses=None
if options.sm_masses != "": sm_masses = options.sm_masses.split(',')
if options.ggh_masses != "": ggh_masses = options.ggh_masses.split(',')
if options.bbh_masses != "": bbh_masses = options.bbh_masses.split(',')
if options.bbh_nlo_masses != "": bbh_nlo_masses = options.bbh_nlo_masses.split(',')

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

def GetZTTNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True):
    if get_os: OSSS = 'os'
    else: OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['ztt_sel'])
    return ana.SummedFactory('ZTT'+add_name, samples, plot, full_selection)

def GetZLLNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True):
    if get_os: OSSS = 'os'
    else: OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['zll_sel'])
    return ana.SummedFactory('ZLL'+add_name, samples, plot, full_selection)

def GetZLNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True):
    if get_os: OSSS = 'os'
    else: OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['zl_sel'])
    return ana.SummedFactory('ZL'+add_name, samples, plot, full_selection)

def GetZJNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True):
    if get_os: OSSS = 'os'
    else: OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS, z_sels['zj_sel'])
    return ana.SummedFactory('ZJ'+add_name, samples, plot, full_selection)

def GenerateZLL(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True, doZL=True, doZJ=True):
    if options.channel == 'em':
        zll_node = GetZLLNode(ana, add_name, samples, plot, wt, sel, cat, z_sels, get_os)
        ana.nodes[nodename].AddNode(zll_node)
    else:
        if doZL:
            zl_node = GetZLNode(ana, add_name, samples, plot, wt, sel, cat, z_sels, get_os)
            ana.nodes[nodename].AddNode(zl_node)
        if doZJ:
            zj_node = GetZJNode(ana, add_name, samples, plot, wt, sel, cat, z_sels, get_os)
            ana.nodes[nodename].AddNode(zj_node)  

def GenerateZTT(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, get_os=True):
    ztt_node = GetZTTNode(ana, add_name, samples, plot, wt, sel, cat, z_sels, get_os)  
    ana.nodes[nodename].AddNode(ztt_node)    

def GetTTTNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}, get_os=True):
  if get_os: OSSS = 'os'
  else: OSSS = '!os'  
  full_selection = BuildCutString(wt, sel, cat, OSSS, top_sels['ttt_sel'])
  return ana.SummedFactory('TTT'+add_name, samples, plot, full_selection)
  
def GetTTJNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}, get_os=True):
  if get_os: OSSS = 'os'
  else: OSSS = '!os'  
  full_selection = BuildCutString(wt, sel, cat, OSSS, top_sels['ttj_sel'])
  print full_selection
  return ana.SummedFactory('TTJ'+add_name, samples, plot, full_selection)

def GenerateTop(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}, get_os=True, doTTT=True, doTTJ=True):
  if doTTT:
      ttt_node = GetTTTNode(ana, add_name, samples, plot, wt, sel, cat, top_sels, get_os)
      ana.nodes[nodename].AddNode(ttt_node)
  if doTTJ:
      ttj_node = GetTTJNode(ana, add_name, samples, plot, wt, sel, cat, top_sels, get_os)
      ana.nodes[nodename].AddNode(ttj_node)

def GetVVTNode(ana, add_name ='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}, get_os=True): 
  if get_os: OSSS = 'os'
  else: OSSS = '!os'  
  full_selection = BuildCutString(wt, sel, cat, OSSS, vv_sels['vvt_sel'])
  return ana.SummedFactory('VVT'+add_name, samples, plot, full_selection)

def GetVVJNode(ana, add_name ='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}, get_os=True): 
  if get_os: OSSS = 'os'
  else: OSSS = '!os'  
  full_selection = BuildCutString(wt, sel, cat, OSSS, vv_sels['vvj_sel'])
  return ana.SummedFactory('VVJ'+add_name, samples, plot, full_selection)

def GenerateVV(ana, add_name ='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}, get_os=True, doVVT=True, doVVJ=True): 
  if doVVT:
      vvt_node = GetVVTNode(ana, add_name, samples, plot, wt, sel, cat, vv_sels, get_os)
      ana.nodes[nodename].AddNode(vvt_node)
  if doVVJ:
      vvj_node = GetVVJNode(ana, add_name, samples, plot, wt, sel, cat, vv_sels, get_os)
      ana.nodes[nodename].AddNode(vvj_node)
      
def GetWGNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', get_os=True):
  if get_os:
      OSSS = 'os'
  else:
      OSSS = '!os'   
  full_selection = BuildCutString(wt, sel, cat, OSSS)
  wg_node = ana.SummedFactory('WGam'+add_name, samples, plot, full_selection)
  return wg_node

def GetWNode(ana, name='W', samples=[], data=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
  if get_os: OSSS = 'os'
  else: OSSS = '!os'
  full_selection = BuildCutString(wt, sel, cat, OSSS, '')
  if cats['w_shape'] != '': shape_cat = cats['w_shape']
  else: shape_cat = cat
  if method == 14:
      shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
  shape_selection = BuildCutString(wt, sel, shape_cat, OSSS, '')
  
  if method in [8, 9, 15, 19, 20]:
      w_node = ana.SummedFactory(name, samples, plot, full_selection)
  elif method in [10, 11]:
      control_sel = cats['w_sdb']+' && '+ OSSS
      w_control_full_selection = BuildCutString(wt, control_sel, cat, OSSS)
      subtract_node = GetSubtractNode(ana,'',plot,wt,control_sel,cat,method,qcd_os_ss_ratio,True,False) 
      if shape_selection == full_selection:
          w_shape = None
      else:    
          w_shape = ana.SummedFactory('w_shape', samples, plot, shape_selection)
      w_node = HttWNode(name,
        ana.SummedFactory('data_obs', data, plot, w_control_full_selection),
        subtract_node,
        ana.SummedFactory('W_cr', samples, plot, w_control_full_selection),
        ana.SummedFactory('W_sr', samples, plot, full_selection),
        w_shape)
  elif method in [12, 13, 14, 16]:
      if method == 16:
          cat_nobtag = '('+cats['btag_wnobtag']+')*('+cats['baseline']+')'
          full_selection = BuildCutString(wt, sel, cat_nobtag, OSSS)
          ss_selection = BuildCutString(wt, '', cat_nobtag, '!os', '')
          os_selection = BuildCutString(wt, '', cat_nobtag, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection = BuildCutString(wt, control_sel, cat_nobtag, OSSS)
          w_control_full_selection_os = BuildCutString(wt, control_sel, cat_nobtag)
          w_control_full_selection_ss = BuildCutString(wt, control_sel, cat_nobtag, '!os')
          btag_extrap_sel_num = BuildCutString(wt, sel, cat, OSSS, '')
          btag_extrap_sel_den = BuildCutString(wt, sel, cat_nobtag, OSSS, '')
          btag_extrap_num_node = ana.SummedFactory('btag', samples, plot, btag_extrap_sel_num)
          btag_extrap_den_node = ana.SummedFactory('no_btag', samples, plot, btag_extrap_sel_den)
          subtract_node_os = GetSubtractNode(ana,'_os',plot,wt,control_sel,cat_nobtag,method,qcd_os_ss_ratio,True,False) 
          subtract_node_ss = GetSubtractNode(ana,'_ss',plot,wt,control_sel,cat_nobtag,method,qcd_os_ss_ratio,False,False)
          
      else:
          full_selection = BuildCutString(wt, sel, cat, OSSS)
          ss_selection = BuildCutString(wt, '', cat, '!os', '')
          os_selection = BuildCutString(wt, '', cat, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection = BuildCutString(wt, control_sel, cat, OSSS)
          w_control_full_selection_os = BuildCutString(wt, control_sel, cat)
          w_control_full_selection_ss = BuildCutString(wt, control_sel, cat, '!os')
          btag_extrap_num_node = None
          btag_extrap_den_node = None
          subtract_node_os = GetSubtractNode(ana,'_os',plot,wt,control_sel,cat,method,qcd_os_ss_ratio,True,False) 
          subtract_node_ss = GetSubtractNode(ana,'_ss',plot,wt,control_sel,cat,method,qcd_os_ss_ratio,False,False)
          
      if shape_selection == full_selection:
          w_shape = None
      else:    
          w_shape = ana.SummedFactory('w_shape', samples, plot, shape_selection)
      w_node = HttWOSSSNode(name,
        ana.SummedFactory('data_os', data, plot, w_control_full_selection_os),
        subtract_node_os,
        ana.SummedFactory('data_ss', data_samples, plot, w_control_full_selection_ss),
        subtract_node_ss,
        ana.SummedFactory('W_cr', samples, plot, w_control_full_selection),
        ana.SummedFactory('W_sr', samples, plot, full_selection),
        ana.SummedFactory('W_os', samples, plot, os_selection),
        ana.SummedFactory('W_ss', samples, plot, ss_selection),
        w_shape,
        qcd_factor,
        get_os,
        btag_extrap_num_node,
        btag_extrap_den_node) 
  return w_node

def GenerateW(ana, add_name='', samples=[], data=[], wg_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
  w_node_name = 'W'  
  if options.channel == 'em':
      w_total_node = SummedNode('W'+add_name)
      w_total_node.AddNode(GetWGNode(ana, add_name, wg_samples, plot, wt, sel, cat))
      ana.nodes[nodename].AddNode(GetWGNode(ana, add_name, wg_samples, plot, wt, sel, cat))
      w_node_name+='J'
  ana.nodes[nodename].AddNode(GetWNode(ana, w_node_name+add_name, samples, data, plot, wt, sel, cat, method, qcd_factor, get_os))
  if options.channel == 'em':
      w_total_node.AddNode(GetWNode(ana, w_node_name+add_name, samples, data, plot, wt, sel, cat, method, qcd_factor, get_os))
      ana.nodes[nodename].AddNode(w_total_node)

def GetSubtractNode(ana,add_name,plot,wt,sel,cat,method,qcd_os_ss_ratio,OSSS,includeW=False):
  subtract_node = SummedNode('total_bkg'+add_name)
  if includeW:
      w_node = GetWNode(ana, 'W', wjets_samples, data_samples, plot, wt, sel, cat, method, qcd_os_ss_ratio, OSSS)
      subtract_node.AddNode(w_node)
  ttt_node = GetTTTNode(ana, "", top_samples, plot, wt, sel, cat, top_sels, OSSS)
  ttj_node = GetTTJNode(ana, "", top_samples, plot, wt, sel, cat, top_sels, OSSS)
  vvt_node = GetVVTNode(ana, "", vv_samples, plot, wt, sel, cat, vv_sels, OSSS)
  vvj_node = GetVVJNode(ana, "", vv_samples, plot, wt, sel, cat, vv_sels, OSSS)
  subtract_node.AddNode(ttt_node)
  subtract_node.AddNode(ttj_node)
  subtract_node.AddNode(vvt_node)
  subtract_node.AddNode(vvj_node)
  ztt_node = GetZTTNode(ana, "", ztt_samples, plot, wt, sel, cat, z_sels, OSSS)
  subtract_node.AddNode(ztt_node)
  if options.channel not in ["em"]:
      zl_node = GetZLNode(ana, "", ztt_samples, plot, wt, sel, cat, z_sels, OSSS)
      zj_node = GetZJNode(ana, "", ztt_samples, plot, wt, sel, cat, z_sels, OSSS)
      subtract_node.AddNode(zl_node)
      subtract_node.AddNode(zj_node)
  if options.channel in ["em"]:
      zll_node = GetZLLNode(ana, "", ztt_samples, plot, wt, sel, cat, z_sels, OSSS)
      subtract_node.AddNode(zll_node)
  if options.channel == "em":
      wg_node = GetWGNode(ana, "", wgam_samples, plot, wt, sel, cat, OSSS)
      subtract_node.AddNode(wg_node)
  return subtract_node
      
def GenerateQCD(ana, add_name='', data=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
    shape_node = None
    OSSS = "!os"
    if get_os: OSSS = "os"
    if options.channel != 'tt':
        
        if method in [9, 11, 13, 14]:
            if method in [9, 11, 13]: shape_cat = '('+cats[options.cat]+')*('+cats['qcd_loose_shape']+')'
            elif method == 14: shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
            shape_selection = BuildCutString(wt, sel, shape_cat, '!os')
            subtract_node = GetSubtractNode(ana,'',plot,wt,sel,shape_cat,method,qcd_os_ss_ratio,False,True)  
            shape_node = SubtractNode('shape', ana.SummedFactory('data_ss', data, plot, shape_selection), subtract_node)
        
        #if options.channel == 'em': qcd_os_ss_factor = 1
        qcd_os_ss_factor = qcd_factor
        weight = wt
        if method in [15,19]:
            #qcd_os_ss_factor = 1
            if get_os and options.channel == "em":
                weight = wt+'*wt_em_qcd'
            if method == 19:
                shape_selection = BuildCutString(weight, sel, em_shape_cat, '!os')
                subtract_node = GetSubtractNode(ana,'',plot,weight,sel,em_shape_cat,method,1,False,True)
                shape_node = SubtractNode('shape', ana.SummedFactory('data_ss',data, plot, shape_selection), subtract_node)
        
        if cats['qcd_shape'] != "": 
            shape_cat = cats['qcd_shape']
            shape_selection = BuildCutString(weight, sel, shape_cat, '!os')
            subtract_node = GetSubtractNode(ana,'',plot,weight,sel,shape_cat,method,1,False,True)
            shape_node = SubtractNode('shape', ana.SummedFactory('data_ss',data, plot, shape_selection), subtract_node)
        
        full_selection = BuildCutString(weight, sel, cat, '!os')
        subtract_node = GetSubtractNode(ana,'',plot,weight,sel,cat,method,qcd_os_ss_ratio,False,True)
        if get_os: qcd_ratio = qcd_os_ss_factor
        else: qcd_ratio = 1.0
        ana.nodes[nodename].AddNode(HttQCDNode('QCD'+add_name,
          ana.SummedFactory('data_ss', data, plot, full_selection),
          subtract_node,
          qcd_ratio,
          shape_node))
        
    else:
        qcd_sdb_cat = cats[options.cat]+' && '+cats['tt_qcd_norm'] 
        
        subtract_node = GetSubtractNode(ana,'',plot,wt,sel,cat,method,qcd_os_ss_ratio,False,True)        
        num_selection = BuildCutString(wt, sel, cat, '!os')
        num_node = SubtractNode('ratio_num',
                     ana.SummedFactory('data', data, plot, num_selection),
                     subtract_node)
        
        subtract_node = GetSubtractNode(ana,'',plot,wt+'*wt_tau2_id_loose',sel,qcd_sdb_cat,method,qcd_os_ss_ratio,False,True)
        den_selection = BuildCutString(wt, sel, qcd_sdb_cat, '!os')
        den_node = SubtractNode('ratio_den',
                     ana.SummedFactory('data', data, plot, den_selection),
                     subtract_node)
        shape_node = None   
        full_selection = BuildCutString(wt, sel, qcd_sdb_cat, OSSS)
        subtract_node = GetSubtractNode(ana,'',plot,wt+'*wt_tau2_id_loose',sel,qcd_sdb_cat,method,qcd_os_ss_ratio,get_os,True)
        
        if options.method == 20:
            num_node = None
            den_node = None
            subtract_node = GetSubtractNode(ana,'',plot,wt+'*wt_tt_qcd_nobtag',sel,cat,method,qcd_os_ss_ratio,False,True)
            full_selection = BuildCutString(wt+'*wt_tt_qcd_nobtag', sel, qcd_sdb_cat, OSSS)

        ana.nodes[nodename].AddNode(HttQCDNode('QCD'+add_name,
          ana.SummedFactory('data', data, plot, full_selection),
          subtract_node,
          1,
          shape_node,
          num_node,
          den_node))
            
        
def GenerateFakeTaus(ana, add_name='', data=[], plot='', wt='', sel='', cat_name='',get_os=True,ff_syst_weight=None):
    print "Generating fake tau background via fake-factor method. In order for this to work you must first ensure that the fake-factor weights are included in the input tree for the channel and category you wish use. Weights should be named as: wt_ff_channel_category"
    
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    
    # Select data from anti-isolated region
    if options.channel != "tt":
        if options.channel == 'mt':
            anti_isolated_sel = '(iso_1<0.15 && mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
            if options.era == "mssmsummer16": anti_isolated_sel +=" && trg_singlemuon"
        elif options.channel == 'et': 
            anti_isolated_sel = '(iso_1<0.1  && mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
            if options.era == "mssmsummer16": anti_isolated_sel +=" && trg_singleelectron"
        ff_cat = cats[cat_name] +" && "+ anti_isolated_sel
        if ff_syst_weight is not None: fake_factor_wt_string = ff_syst_weight
        else: fake_factor_wt_string = "wt_ff_"+options.cat
        fake_factor_wt_string+='*wt_tau_id_loose'
        if wt is not "": wt+="*"+fake_factor_wt_string
        else: wt=fake_factor_wt_string
    
        full_selection = BuildCutString(wt, sel, ff_cat, OSSS, '')
        # Calculate FF for anti-isolated data (f1) then subtract contributions from real taus (f2)
        f1 = ana.SummedFactory('data', data, plot, full_selection)
        f2 = GetSubtractNode(ana,'',plot,wt,sel+'*(gen_match_2<6)',ff_cat,8,1.0,True,True)
        ana.nodes[nodename].AddNode(SubtractNode('jetFakes'+add_name, f1, f2))
        
    if options.channel == 'tt':
        anti_isolated_sel_1 = '(mva_olddm_medium_1<0.5 && mva_olddm_vloose_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
        anti_isolated_sel_2 = '(mva_olddm_medium_2<0.5 && mva_olddm_vloose_2>0.5 && mva_olddm_medium_1>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
        if options.era == "mssmsummer16": 
          anti_isolated_sel_1 +=" && trg_doubletau"
          anti_isolated_sel_2 +=" && trg_doubletau"
        
        ff_cat_1 = cats[cat_name] +" && "+ anti_isolated_sel_1
        ff_cat_2 = cats[cat_name] +" && "+ anti_isolated_sel_2
        if ff_syst_weight is not None: 
            fake_factor_wt_string_1 = ff_syst_weight+'_1'
            fake_factor_wt_string_2 = ff_syst_weight+'_2'
        else:
          fake_factor_wt_string_1 = "wt_ff_"+options.cat+"_1"
          fake_factor_wt_string_2 = "wt_ff_"+options.cat+"_2"
        if wt is not "": 
            wt_1=wt+"*"+fake_factor_wt_string_1
            wt_2=wt+"*"+fake_factor_wt_string_2
        else: 
            wt_1=fake_factor_wt_string_1
            wt_2=fake_factor_wt_string_2
    
        full_selection_1 = BuildCutString(wt_1, sel, ff_cat_1, OSSS, '')
        full_selection_2 = BuildCutString(wt_2, sel, ff_cat_2, OSSS, '')
        
        ff_total_node = SummedNode('jetFakes'+add_name)
        f1_total_node = SummedNode('data')
        f1_total_node.AddNode(ana.SummedFactory('data_1', data, plot, full_selection_1))
        f1_total_node.AddNode(ana.SummedFactory('data_2', data, plot, full_selection_2))
        f2_total_node = SummedNode('total_bkg')
        f2_total_node.AddNode(GetSubtractNode(ana,'_1',plot,wt_1+'*wt_tau1_id_loose',sel+'*(gen_match_1<6)',ff_cat_1,8,1.0,True,True))
        f2_total_node.AddNode(GetSubtractNode(ana,'_2',plot,wt_2+'*wt_tau2_id_loose',sel+'*(gen_match_2<6)',ff_cat_2,8,1.0,True,True))
        ana.nodes[nodename].AddNode(SubtractNode('jetFakes'+add_name, f1_total_node, f2_total_node))
        
def GenerateSMSignal(ana, add_name='', plot='', masses=['125'], wt='', sel='', cat='', get_os=True, sm_bkg = ''):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS)
    if masses is not None:
        for mass in masses:  
            if sm_bkg != '':
                add_str = '_SM'+sm_bkg
            else:
                add_str = mass
            for key in sm_samples:
                sample_name = sm_samples[key].replace('*',mass)
                ana.nodes[nodename].AddNode(ana.BasicFactory(key+add_str+add_name, sample_name, plot, full_selection))
            
def GenerateMSSMSignal(ana, add_name='', bbh_add_name='', plot='', ggh_masses = ['1000'], bbh_masses = ['1000'], wt='', sel='', cat='', get_os=True, do_ggH=True, do_bbH=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    if options.gen_signal: OSSS='1'
    full_selection = BuildCutString(wt, sel, cat, OSSS)
    for key in mssm_samples:
        masses = None
        if key == 'ggH':
            masses = ggh_masses
        elif key == 'bbH':
            masses = bbh_masses
        if masses is not None:    
            for mass in masses:
                if key == 'ggH' and not do_ggH:
                    continue
                if key == 'bbH' and not do_bbH:
                    continue
                sample_name = mssm_samples[key].replace('*',mass)
                add_name_2 = ''
                if bbh_add_name == '-LO' and key is 'bbH': 
                    sample_name = mssm_lo_samples['bbH-LO'].replace('*',mass)
                    add_name_2 = bbh_add_name
                ana.nodes[nodename].AddNode(ana.BasicFactory(key+add_name_2+mass+add_name, sample_name, plot, full_selection))
                
def GenerateNLOMSSMSignal(ana, add_name='', plot='', ggh_nlo_masses = ['1000'], bbh_nlo_masses = ['1000'],wt='wt', sel='', cat='', doScales=True, doPDF=False, get_os=True,do_ggH=True, do_bbH=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    if options.gen_signal: OSSS='1'
    weights = {'':'1'}
    wt_noscale = wt
    if doScales: weights = {'':'1','muR1muF2':'wt_mur1_muf2','muR1muF0.5':'wt_mur1_muf0p5','muR2muF1':'wt_mur2_muf1','muR2muF2':'wt_mur2_muf2','muR0.5muF1':'wt_mur0p5_muf1','muR0.5muF0.5':'wt_mur0p5_muf0p5'}
    if doPDF:
      for i in range(1,101): weights['PDF_'+str(i)] = 'wt_pdf_'+str(i)
      weights['AlphaS_Up'] = 'wt_alphasup'
      weights['AlphaS_Down'] = 'wt_alphasdown'
    for weight in weights:
      wt = weights[weight]+'*'+wt_noscale  
      full_selection = BuildCutString(wt, sel, cat, OSSS)
      for key in mssm_nlo_samples:
          if 'Qsh' in key and weight is not '': continue
          if 'ggH' in key:
              masses = ggh_nlo_masses
          elif 'bbH' in key:
              masses = bbh_nlo_masses
          if masses is not None:    
              for mass in masses:
                  if key == 'ggH' and not do_ggH:
                      continue
                  if key == 'bbH' and not do_bbH:
                      continue
                  sample_name = mssm_nlo_samples[key].replace('*',mass)
                  ana.nodes[nodename].AddNode(ana.BasicFactory(key+mass+add_name+weight, sample_name, plot, full_selection))
        
        
def GenerateHhhSignal(ana, add_name='', plot='', masses = ['700'], wt='', sel='', cat='', get_os=True):
    if get_os:
        OSSS = 'os'
    else:
        OSSS = '!os'
    full_selection = BuildCutString(wt, sel, cat, OSSS)
    if masses is not None:
        for mass in masses:
            for key in Hhh_samples:
                sample_name = Hhh_samples[key].replace('*',mass)
                ana.nodes[nodename].AddNode(ana.BasicFactory(key+mass+add_name, sample_name, plot, full_selection))
        
def PrintSummary(nodename='', data_strings=['data_obs'], add_name=''):
    print ''
    print '################### Summary ###################'
    nodes = ana.nodes[nodename].SubNodes()
    bkg_total = ufloat(0.000000001,0.000000001)
    sig_total = ufloat(0.000000001,0.000000001)
    for node in nodes:
        if options.method == 18 and 'jetFakes' == node.name: continue
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

def FixBins(ana,outfile='output.root'):
    #Fix empty histograms
    nodes = ana.nodes[nodename].SubNodes()
    for node in nodes:
        if 'data_obs' in node.name: continue
        hist = node.shape.hist
        outfile.cd(nodename)
        write_hist=False
        #Fix empty histogram
        if hist.Integral() == 0.0:
            hist.SetBinContent(hist.GetNbinsX()/2, 0.00001)
            hist.SetBinError(hist.GetNbinsX()/2, 0.00001)
            write_hist=True
        outfile.cd()
                                                                                                                                
def NormFFSysts(ana,outfile='output.root'):
    nominal_hist = outfile.Get(nodename+'/jetFakes')
    nominal_scale = nominal_hist.Integral(0,nominal_hist.GetNbinsX()+1)
    directory = outfile.Get(nodename)
    outfile.cd(nodename)
    hists_to_add=[]
    for key in directory.GetListOfKeys():
        hist_name = key.GetName()
        hist = directory.Get(hist_name).Clone()
        if not isinstance(hist,ROOT.TDirectory):
           if 'jetFakes' not in hist_name: continue
           if hist_name == 'jetFakes': continue
           norm = nominal_scale/hist.Integral(0,hist.GetNbinsX()+1)
           hist.Scale(norm)
           norm_hist_name = hist_name
           norm_hist_name = norm_hist_name.replace('jetFakes','jetFakes_norm')
           hist.SetName(norm_hist_name)
           hists_to_add.append(hist)
    for hist in hists_to_add: hist.Write()
    
def PDFUncerts(nodename, infile):
  def RMS(a):
    from numpy import mean, sqrt, square  
    rms = sqrt(mean(square(a-mean(a))))
    return rms
  outstring1=''
  outstring2=''
  
  for mass in bbh_nlo_masses:
    nominal_error=ROOT.Double()
    nominal = outfile.Get(nodename+'/bbH'+mass).IntegralAndError(-1, -1,nominal_error)
    sample_name='SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO'
    evt_nom = ana.info[sample_name]['evt']
    pdf_variations_nosf=[]
    pdf_variations=[]
    pdf_variations_nosf.append(nominal)
    pdf_variations.append(nominal)
    for i in range(1,101):
      wt = 'wt_pdf_'+str(i)
      name = 'PDF_'+str(i)
      val = outfile.Get(nodename+'/bbH'+mass+name).Integral(-1, -1)
      pdf_variations_nosf.append(val)
      evt_var = ana.info[sample_name]['evt_'+wt]
      sf = evt_nom/evt_var
      pdf_variations.append(val*sf)
    pdf_uncert_nosf=RMS(pdf_variations_nosf)/nominal*100
    pdf_uncert=RMS(pdf_variations)/nominal*100
    #print pdf_uncert_nosf, pdf_uncert
    
    outstring1+=mass+','
    outstring2+=str(pdf_uncert)+','
    
    nominal_error=ROOT.Double()
    nominal = outfile.Get(nodename+'/bbH'+mass).IntegralAndError(-1, -1,nominal_error)
    alphas_down_error_nosf=ROOT.Double()
    alphas_down_nosf = outfile.Get(nodename+'/bbH'+mass+'AlphaS_Down').IntegralAndError(-1, -1,alphas_down_error_nosf)
    alphas_up_error_nosf=ROOT.Double()
    alphas_up_nosf = outfile.Get(nodename+'/bbH'+mass+'AlphaS_Up').IntegralAndError(-1, -1,alphas_up_error_nosf)
    evt_var = ana.info[sample_name]['evt_wt_alphasdown']
    sf = evt_nom/evt_var
    alphas_down=alphas_down_nosf*sf
    alphas_down_error=alphas_down_error_nosf*sf
    evt_var = ana.info[sample_name]['evt_wt_alphasup']
    sf = evt_nom/evt_var
    alphas_up=alphas_up_nosf*sf
    alphas_up_error=alphas_up_error_nosf*sf
    
    alphas_uncert = (alphas_up-alphas_down)/2/nominal
    alphas_uncert_error = math.sqrt(alphas_down_error**2+alphas_up_error**2)/(alphas_up-alphas_down)*alphas_uncert
    #(alphas_up_nosf-alphas_down_nosf)/2/nominal
    #print alphas_uncert*100, '\\% $\\pm$', alphas_uncert_error*100,'\\%'
  print outstring1
  print outstring2

def DONLOUncerts(nodename,infile):
    def LargestDiff(nominal,scales_shifted):
        largest_diff=0
        value = nominal
        for i in scales_shifted:
            diff = abs(scales_shifted[i] - nominal)
            if diff > largest_diff: 
              largest_diff = diff
              value = scales_shifted[i]
        return value
    if not options.bbh_nlo_masses: return
    outstring='%'+options.channel+' '+options.datacard+'\n'
    if options.nlo_qsh: outstring+='\\begin{table}[H]\n\\centering\n\\resizebox{\\textwidth}{!}{\n\\begin{tabular}{ |c|c|c| }\n\\hline\nSignal Mass (GeV) & Qsh Uncertainty &  Qsh Uncertainty (*)'
    else: outstring+='\\begin{table}[H]\n\\centering\n\\resizebox{\\textwidth}{!}{\n\\begin{tabular}{ |c|c|c| }\n\\hline\nSignal Mass (GeV) & Scale Uncertainty &  Scale Uncertainty (*)'
    outstring += '\\\\\n\\hline\n'
    #outstring2='{'
    #outstring3='{'
    #outstring4='{'
    for mass in bbh_nlo_masses:
      nominal_error=ROOT.Double()
      nominal = outfile.Get(nodename+'/bbH'+mass).IntegralAndError(-1, -1,nominal_error) 
      samples = {'bbH*':'', 'bbH*muR0.5muF0.5':'wt_mur0p5_muf0p5', 'bbH*muR1muF0.5':'wt_mur1_muf0p5', 'bbH*muR0.5muF1':'wt_mur0p5_muf1', 'bbH*muR2muF2':'wt_mur2_muf2', 'bbH*muR2muF1':'wt_mur2_muf1', 'bbH*muR1muF2':'wt_mur1_muf2'} 
      qsh_down_error=ROOT.Double()
      qsh_up_error=ROOT.Double()
      if options.nlo_qsh:
        qsh_down = outfile.Get(nodename+'/bbH-QshDown'+mass).IntegralAndError(-1, -1,qsh_down_error) 
        qsh_up = outfile.Get(nodename+'/bbH-QshUp'+mass).IntegralAndError(-1, -1,qsh_up_error)
        qsh_uncert_1=(max(nominal,qsh_down,qsh_up) - min(nominal,qsh_down,qsh_up))/2
        up_error = nominal_error 
        down_error = nominal_error
        if max(nominal,qsh_down,qsh_up) is qsh_up: up_error = qsh_up_error
        if max(nominal,qsh_down,qsh_up) is qsh_down: up_error = qsh_down_error
        if min(nominal,qsh_down,qsh_up) is qsh_up: down_error = qsh_up_error
        if min(nominal,qsh_down,qsh_up) is qsh_down: down_error = qsh_down_error
        qsh_error_1 = math.sqrt(up_error**2 + down_error**2)
        qsh_uncert_2 = (qsh_up - qsh_down)/2
        qsh_error_2 = math.sqrt(qsh_up_error**2 + qsh_down_error**2)
      scale_max = nominal
      scale_min = nominal
      scale_nosf_max = nominal
      scale_nosf_min = nominal
      up_dic = {}
      down_dic = {}
      for samp in samples:
        acceptance_error = ROOT.Double()  
        acceptance = outfile.Get(nodename+'/'+samp.replace('*',mass)).IntegralAndError(-1, -1,acceptance_error)
        if samp is 'bbH*': sf = 1.0 
        else: 
          sample_name='SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO'
          evt_nom = ana.info[sample_name]['evt']
          evt_var = ana.info[sample_name]['evt_'+samples[samp]]
          sf = evt_nom/evt_var
        acceptance_nosf = acceptance
        acceptance*=sf
        #if samples[samp] in ['wt_mur0p5_muf0p5', 'wt_mur1_muf0p5', 'wt_mur0p5_muf1']: down_dic[samples[samp]] = acceptance
        #if samples[samp] in ['wt_mur2_muf2','wt_mur2_muf1','wt_mur1_muf2']: up_dic[samples[samp]] = acceptance
        if samples[samp] in ['wt_mur0p5_muf0p5']: 
            down_dic[samples[samp]] = [acceptance,acceptance_error]
        if samples[samp] in ['wt_mur2_muf2']: up_dic[samples[samp]] = [acceptance,acceptance_error]
        if acceptance > scale_max: 
            scale_max = acceptance
            up_error = acceptance_error
        if acceptance < scale_min:
            scale_min = acceptance
            down_error = acceptance_error
        if acceptance_nosf > scale_nosf_max: scale_nosf_max = acceptance_nosf
        if acceptance_nosf < scale_nosf_min: scale_nosf_min = acceptance_nosf
      #up_nom = LargestDiff(nominal,up_dic)
      #down_nom = LargestDiff(nominal,down_dic)
      up_nom = up_dic['wt_mur2_muf2'][0]
      down_nom = down_dic['wt_mur0p5_muf0p5'][0]
      uncert = (scale_max-scale_min)/2
      uncert_error = math.sqrt(up_error**2+down_error**2)/(scale_max-scale_min)*uncert
      uncert_nosf = (scale_nosf_max-scale_nosf_min)/2
      uncert_alt_method = (up_nom-down_nom)/2
      uncert_alt_error = math.sqrt(up_dic['wt_mur2_muf2'][1]**2 + down_dic['wt_mur0p5_muf0p5'][1]**2)/(up_nom-down_nom) *uncert_alt_method
      pythia_error=ROOT.Double()
      pythia_yield = outfile.Get(nodename+'/bbH'+mass).IntegralAndError(-1, -1,pythia_error) 
      outstring +=mass#+ ' & '+ str(round(pythia_yield,1))+' $\pm$ '+str(round(pythia_error,1))+ ' & '+ str(round(nominal,1))+' $\pm$ '+str(round(nominal_error,1))+ '('+str(round((pythia_yield-nominal)*100/pythia_yield,2))+' \%)' + ' & '+ str(round(uncert_nosf/nominal,2)) + ' & '+ str(round(uncert/nominal,2))+ ' & '+ str(round(uncert_alt_method/nominal,2))
      if options.nlo_qsh: 
        outstring+=' & '+ str(round(100*qsh_uncert_1/nominal,1))+' $\pm$ '+str(round(100*qsh_error_1/nominal,1))+' & '+ str(round(100*qsh_uncert_2/nominal,1))+' $\pm$ '+str(round(100*qsh_error_2/nominal,1))+'\\\\\n'  
      else:
        outstring+=' & '+ str(round(100*uncert/nominal,1))+'\\% $\\pm$ '+ str(round(100*uncert_error/nominal,1)) +'\\% & '+ str(round(100*uncert_alt_method/nominal,1))+'\\% $\\pm$ '+str(round(100*uncert_alt_error/nominal,1)) +'\\% \\\\\n'
      #outstring2 +=str(round(100*uncert/nominal,1))+','
      #outstring3 +=str(round(100*uncert_error/nominal,1))+','
      #outstring4+=mass+','
    outstring+='\\hline\n\\end{tabular}}\n\\end{table}'
    print outstring
    #print outstring2
    #print outstring3
    #print outstring4
    
def ScaleUncertBand(nodename='',outfile='output.root',NormScales=True):
    hist_names=['bbH*muR0.5muF0.5','bbH*muR1muF0.5','bbH*muR0.5muF1','bbH*muR2muF1','bbH*muR1muF2','bbH*muR2muF2']
    mass = '100'
    if options.draw_signal_mass: mass = options.draw_signal_mass
    hists=[]
    for hist_name in hist_names:
        sf = 1.0
        if NormScales:
            sample_name='SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO'
            evt_nom = ana.info[sample_name]['evt']
            evt_var = ana.info[sample_name]['evt_'+hist_name.replace('bbH*','wt_').replace('muF','_muf').replace('muR','mur').replace('0.5','0p5')]
            sf = evt_nom/evt_var
        hist_name=hist_name.replace('*',mass)
        hist = outfile.Get(nodename+'/'+hist_name).Clone()
        if NormScales: hist.Scale(sf)
        hists.append(hist)
    nom_hist = outfile.Get(nodename+'/bbH'+mass)
    up_hist = nom_hist.Clone()
    down_hist = nom_hist.Clone()
    up_hist.SetName('ScaleUp')
    down_hist.SetName('ScaleDown')
    for i in range (1,nom_hist.GetNbinsX()+1):
        for hist in hists:
          max_content = up_hist.GetBinContent(i)
          min_content = down_hist.GetBinContent(i)
          content = hist.GetBinContent(i)
          if content > max_content: up_hist.SetBinContent(i,content)
          if content < min_content: down_hist.SetBinContent(i,content)
    outfile.cd(nodename)
    up_hist.Write()
    down_hist.Write()
    outfile.cd()
        
    
def DYUncertBand(outfile='output.root',ScaleToData=True):
    bkg_hist = outfile.Get(nodename+'/total_bkg')
    nominal_hist = outfile.Get(nodename+'/ZLL')
    up_hist = outfile.Get(nodename+'/total_bkg').Clone()
    down_hist = outfile.Get(nodename+'/total_bkg').Clone()
    up_hist.SetName('total_bkg_up')
    down_hist.SetName('total_bkg_down')
    shifts=['_ES', '_TT', '_Stat0', '_Stat40', '_Stat80']
    for i in range(1,nominal_hist.GetNbinsX()+1):
      nom_content = nominal_hist.GetBinContent(i)
      bkg_content = bkg_hist.GetBinContent(i)
      uncert=0
      for shift in shifts:
          shift_hist_up = outfile.Get(nodename+'/ZLL'+shift+'Up')
          shift_hist_down = outfile.Get(nodename+'/ZLL'+shift+'Down')
          up = abs(shift_hist_up.GetBinContent(i) - nom_content)
          down = abs(shift_hist_down.GetBinContent(i) - nom_content)
          uncert=math.sqrt(max([up,down])**2+uncert**2)
      up_hist.SetBinContent(i, bkg_content+uncert)
      down_hist.SetBinContent(i, bkg_content-uncert)
    outfile.cd(nodename)
    up_hist.Write()
    down_hist.Write()
    outfile.cd()
    if ScaleToData:
      data_hist=outfile.Get(nodename+'/data_obs')
      data_total=data_hist.Integral(-1,-1)
      bkg_total=bkg_hist.Integral(-1,-1)
      data_hist.Scale(bkg_total/data_total)
      outfile.cd(nodename)
      data_hist.Write()
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
            if options.method == 18 and 'jetFakes' == node.name: continue
            if first_hist:
                total_bkg = ana.nodes[nodename].nodes[node.name].shape.hist.Clone()
                first_hist=False
            else: total_bkg.Add(ana.nodes[nodename].nodes[node.name].shape.hist.Clone())
    if not first_hist:        
        total_bkg.SetName('total_bkg'+add_name)
        total_bkg.Write()
    outfile.cd()
    
def RunPlotting(ana, cat='', sel='', add_name='', wt='wt', do_data=True, samples_to_skip=[], outfile='output.root',ff_syst_weight=None):
    doTTJ = 'TTJ' not in samples_to_skip
    doTTT = 'TTT' not in samples_to_skip
    doVVJ = 'VVJ' not in samples_to_skip
    doVVT = 'VVT' not in samples_to_skip
    doZL  = 'ZL'  not in samples_to_skip
    doZJ  = 'ZJ'  not in samples_to_skip
    
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
        doVVJ=False
        doTTJ=False
        GenerateFakeTaus(ana, add_name, data_samples, plot, wt, sel, options.cat,not options.do_ss,ff_syst_weight)
        
        # use existing methods to calculate background due to non-fake taus
        add_fake_factor_selection = "gen_match_2<6"
        if options.channel == "tt": add_fake_factor_selection = "gen_match_1<6 && gen_match_2<6"
        residual_cat=cat+"&&"+add_fake_factor_selection
        
        if 'ZTT' not in samples_to_skip:
            GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, residual_cat, z_sels, not options.do_ss)                                
        if 'ZLL' not in samples_to_skip:
            GenerateZLL(ana, add_name, ztt_samples, plot, wt, sel, residual_cat, z_sels, not options.do_ss,doZL,False)
        if 'TT' not in samples_to_skip:    
            GenerateTop(ana, add_name, top_samples, plot, wt, sel, residual_cat, top_sels, not options.do_ss, doTTT, doTTJ)  
        if 'VV' not in samples_to_skip:
            GenerateVV(ana, add_name, vv_samples, plot, wt, sel, residual_cat, vv_sels, not options.do_ss, doVVT, doVVJ)  
    
    else:
        method = options.method
        if options.method == 18:
            GenerateFakeTaus(ana, add_name, data_samples, plot, wt, sel, options.cat,not options.do_ss,ff_syst_weight)
            if options.channel == 'tt': method = 8
            elif options.cat == "btag_loosemt" or options.cat == "btag_tight": method = 16
            elif options.channel == 'et' or options.channel == 'mt': method = 12
        if 'ZTT' not in samples_to_skip:
            GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels, not options.do_ss)                                
        if 'ZLL' not in samples_to_skip:
            GenerateZLL(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels, not options.do_ss,doZL,doZJ)
        if 'TT' not in samples_to_skip:    
            GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels, not options.do_ss, doTTT, doTTJ)  
        if 'VV' not in samples_to_skip:
            GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels, not options.do_ss, doVVT, doVVJ)  
        if 'W' not in samples_to_skip:
            GenerateW(ana, add_name, wjets_samples, data_samples, wgam_samples, plot, wt, sel, cat, method, qcd_os_ss_ratio, not options.do_ss)
        if 'QCD' not in samples_to_skip:
            GenerateQCD(ana, add_name, data_samples, plot, wt, sel, cat, method, qcd_os_ss_ratio, not options.do_ss)
        if compare_w_shapes:
          cat_relax=cats['w_shape_comp']
          GenerateW(ana, '_shape', wjets_samples, data_samples, wgam_samples, plot, wt, sel, cat_relax, 8, qcd_os_ss_ratio, not options.do_ss)    
        if compare_qcd_shapes:
          cat_relax=cats['qcd_shape_comp']
          GenerateQCD(ana, '_shape', data_samples, plot, wt, sel, cat_relax, method, qcd_os_ss_ratio, not options.do_ss)
           
    if 'signal' not in samples_to_skip:
        if options.analysis == 'sm':
            GenerateSMSignal(ana, add_name, plot, sm_masses, wt, sel, cat, not options.do_ss)
        elif options.analysis == 'mssm' and (options.ggh_masses != "" or options.bbh_masses != ""):
            bbh_add_name = ''
            if options.bbh_nlo_masses: bbh_add_name = '-LO'
            GenerateMSSMSignal(ana, add_name, bbh_add_name, plot, ggh_masses, bbh_masses, wt, sel, cat, not options.do_ss)
            if options.add_sm_background:
                GenerateSMSignal(ana, add_name, plot, ['125'],  wt, sel, cat, not options.do_ss, options.add_sm_background)  
        elif options.analysis == 'Hhh':
            GenerateHhhSignal(ana, add_name, plot, ggh_masses, wt, sel, cat, not options.do_ss)
        if options.analysis == 'mssm' and options.bbh_nlo_masses != "":
            GenerateNLOMSSMSignal(ana, add_name, plot, [''], bbh_nlo_masses, wt, sel, cat, options.doNLOScales, options.doPDF, not options.do_ss)
            
    ana.Run()
    ana.nodes.Output(outfile)

    # fix negative bns,empty histograms etc.
    FixBins(ana,outfile)
    # add histograms to get totals for backgrounds split into real/fake taus and make a total backgrounds histogram
    GetTotals(ana,add_name,outfile)
    
    if compare_w_shapes:
      nominal_hist = outfile.Get(nodename+'/W')
      nominal_scale = nominal_hist.Integral(0,nominal_hist.GetNbinsX()+1)
      directory = outfile.Get(nodename)
      outfile.cd(nodename)
      shape_hist = outfile.Get(nodename+'/W_shape')
      shape_scale = shape_hist.Integral(0,shape_hist.GetNbinsX()+1)
      shape_hist.Scale(nominal_scale/shape_scale)
      shape_hist.Write()
    if compare_qcd_shapes:
      nominal_hist = outfile.Get(nodename+'/QCD')
      nominal_scale = nominal_hist.Integral(0,nominal_hist.GetNbinsX()+1)
      directory = outfile.Get(nodename)
      outfile.cd(nodename)
      shape_hist = outfile.Get(nodename+'/QCD_shape')
      shape_scale = shape_hist.Integral(0,shape_hist.GetNbinsX()+1)
      shape_hist.Scale(nominal_scale/shape_scale)
      shape_hist.Write()

# Create output file
is_2d=False
var_name = options.var.split('[')[0]
var_name = var_name.split('(')[0]
if ',' in var_name:
    is_2d = True
    var_name = var_name.split(',')[0]+'_vs_'+var_name.split(',')[1]

if options.datacard != "": datacard_name = options.datacard
else: datacard_name = options.cat
output_name = options.outputfolder+'/datacard_'+var_name+'_'+datacard_name+'_'+options.channel+'_'+options.year+'.root'
outfile = ROOT.TFile(output_name, 'RECREATE')

for systematic in systematics:
    
    print "Processing:", systematic
    print ""
    
    add_folder_name = systematics[systematic][0]
    add_name = systematics[systematic][1]
    isFFSyst = systematics[systematic][4]
    ff_syst_weight = None
    if not isFFSyst: weight = systematics[systematic][2]
    else:
        weight='wt'
        ff_syst_weight = systematics[systematic][2]
    if options.add_wt is not "": weight+="*"+options.add_wt
    if options.channel == "tt": weight+='*wt_tau_id_medium'
    samples_to_skip = systematics[systematic][3]
    
    ana = Analysis()
    
    ana.remaps = {}
    if options.channel == 'em':
        ana.remaps['MuonEG'] = 'data_obs'
    elif options.channel in ['mt','mj','zmm']:
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
    for sample_name in ztt_samples + vv_samples + wgam_samples + top_samples + ztt_shape_samples + wjets_samples:
        ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
     
    # Add all MC signal files
    
    if options.analysis == 'sm':
        signal_samples = sm_samples
    elif options.analysis == 'mssm':
        signal_samples = mssm_samples
        if options.bbh_nlo_masses: signal_samples['bbH'] = mssm_nlo_samples['bbH']
        if options.nlo_qsh: signal_samples.update(mssm_nlo_qsh_samples)
        if options.bbh_nlo_masses and options.bbh_masses:  signal_samples.update(mssm_lo_samples)
    elif options.analysis == 'Hhh':
        signal_samples = Hhh_samples

    for samp in signal_samples:
        if options.analysis == "sm":
            masses=sm_masses
        elif samp == 'ggH':
            masses = ggh_masses
        elif (samp == 'bbH' and not options.bbh_nlo_masses) or samp == 'bbH-LO':
            masses = bbh_masses
        elif 'bbH' in samp:
            masses = bbh_nlo_masses
        if masses is not None:    
            for mass in masses:
                sample_name = signal_samples[samp].replace('*',mass)
                tree_name = 'ntuple'
                if options.gen_signal: tree_name = 'gen_ntuple'
                ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', tree_name, None, sample_name)
    if options.add_sm_background and options.analysis == 'mssm':
        for samp in sm_samples:
            sample_name = sm_samples[samp].replace('*',options.add_sm_background)
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
            
    ana.AddInfo(options.paramfile, scaleTo='data_obs')
    
    cat = '('+cats[options.cat]+')*('+cats['baseline']+')'
    if options.channel=="em": em_shape_cat = '('+cats[options.cat]+')*('+cats['loose_baseline']+')'
    sel = options.sel
    plot = options.var
    if options.datacard != "": nodename = options.channel+'_'+options.datacard
    else: nodename = options.channel+'_'+options.cat
    
    ana.nodes.AddNode(ListNode(nodename))
    if options.do_custom_uncerts and options.custom_uncerts_wt_up != "" and options.custom_uncerts_wt_down !="":
        ana_up   = Analysis()
        ana_down = Analysis()
        ana_up = copy.deepcopy(ana)
        ana_down = copy.deepcopy(ana)

    # Add data only for default
    if systematic == 'default': do_data = True
    else: do_data = False
            
    #Run default plot 
    if options.scheme == 'signal': 
        samples_to_skip.extend(['TTT','TTJ','VVT','VVJ','W','QCD','jetFakes','ZLL','ZTT','ZL'])
        do_data = False
    RunPlotting(ana, cat, sel, add_name, weight, do_data, samples_to_skip,outfile,ff_syst_weight)
    
    
    if options.do_custom_uncerts and options.custom_uncerts_wt_up != "" and options.custom_uncerts_wt_down !="":
        RunPlotting(ana_up, cat, sel, '_custom_uncerts_up', weight+'*'+options.custom_uncerts_wt_up, do_data, ['signal'],outfile,ff_syst_weight)
        RunPlotting(ana_down, cat, sel, '_custom_uncerts_down', weight+'*'+options.custom_uncerts_wt_down, do_data, ['signal'],outfile,ff_syst_weight)
    
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
                if masses is not None:    
                    for mass in masses:
                        xs = ana.info[sm_samples[samp].replace('*',mass)]['xs']
                        sf = 1.0/xs
                        sm_hist = ana.nodes[nodename].nodes[samp_name+mass+add_name].shape.hist
                        sm_hist.Scale(sf)
                        sm_hist.Write()
        if options.analysis == "mssm":
            for samp in mssm_samples:
                if samp == 'ggH':
                    masses = ggh_masses
                elif samp == 'bbH' and not options.bbh_nlo_masses:
                    masses = bbh_masses
                elif 'bbH' in samp:
                    masses = bbh_nlo_masses
                if masses is not None:    
                    for mass in masses:
                        xs = ana.info[mssm_samples[samp].replace('*',mass)]['xs']
                        sf = 1.0/xs
                        mssm_hist = ana.nodes[nodename].nodes[samp+mass+add_name].shape.hist
                        mssm_hist.Scale(sf)
                        mssm_hist.Write()
        if options.analysis == "Hhh":
            for samp in Hhh_samples:
                masses = ggh_masses
                if masses is not None:
                    for mass in masses:
                        xs = ana.info[Hhh_samples[samp].replace('*',mass)]['xs']
                        sf = 1.0/xs
                        mssm_hist = ana.nodes[nodename].nodes[samp+mass+add_name].shape.hist
                        mssm_hist.Scale(sf)
                        mssm_hist.Write()
        outfile.cd()
if options.method in [17,18] and options.do_ff_systs: NormFFSysts(ana,outfile)
if options.doNLOScales: 
    ScaleUncertBand(nodename,outfile)
    DONLOUncerts(nodename,outfile)
if options.doPDF:
    PDFUncerts(nodename,outfile)

outfile.Close()
if is_2d: exit(0) # 2d plotting cosmetics not currently supported
plot_file = ROOT.TFile(output_name, 'READ')

if options.method in [12,16] or (options.channel != "tt" and options.method == "18"):
    w_os = plot_file.Get(nodename+"/W.subnodes/W_os")    
    w_ss = plot_file.Get(nodename+"/W.subnodes/W_ss")
    w_os_error=ROOT.Double(0.)
    w_ss_error=ROOT.Double(0.)
    w_os_total = w_os.IntegralAndError(0,w_os.GetNbinsX()+1,w_os_error)
    w_ss_total = w_ss.IntegralAndError(0,w_ss.GetNbinsX()+1,w_ss_error)
    w_os_ss = w_os_total/w_ss_total
    w_os_ss_error = math.sqrt( (w_os_error/w_os_total)**2 + (w_ss_error/w_ss_total)**2 )*w_os_ss

    #print "W OS/SS ratio = ", w_os_ss, "+/-", w_os_ss_error, "("+str(100*w_os_ss_error/w_os_ss)+" %)"

if options.custom_uncerts_wt_up != "" and options.custom_uncerts_wt_down != "": 
    custom_uncerts_up_name = "total_bkg_custom_uncerts_up"
    custom_uncerts_down_name = "total_bkg_custom_uncerts_down"
else:
    custom_uncerts_up_name = options.custom_uncerts_up_name
    custom_uncerts_down_name = options.custom_uncerts_down_name

if not options.no_plot:
    if options.datacard != "": plot_name = options.outputfolder+'/'+var_name+'_'+options.datacard+'_'+options.channel+'_'+options.year
    else: plot_name = options.outputfolder+'/'+var_name+'_'+options.cat+'_'+options.channel+'_'+options.year
    if options.log_x: plot_name += "_logx" 
    if options.log_y: plot_name += "_logy"
    titles = plotting.SetAxisTitles(options.var,options.channel)
    if options.x_title == "": x_title = titles[0]
    else: x_title = options.x_title
    
    if options.y_title == "": 
        y_title = titles[1]
    else: y_title = options.y_title
    scheme = options.channel
    if compare_w_shapes: scheme = 'w_shape'
    if compare_qcd_shapes: scheme = 'qcd_shape'
    if options.scheme != "": scheme = options.scheme
    FF = options.method==17 or options.method==18
    if scheme != 'signal':
      print x_title 
      plotting.HTTPlot(nodename, 
        plot_file, 
        options.signal_scale, 
        options.draw_signal_mass,
        FF,
        options.norm_bins,
        options.channel,
        options.blind,
        options.x_blind_min,
        options.x_blind_max,
        options.ratio,
        options.log_y,
        options.log_x,
        options.ratio_range,
        options.custom_x_range,
        options.x_axis_min,
        options.x_axis_max,
        options.custom_y_range,
        options.y_axis_max,
        options.y_axis_min,
        x_title,
        y_title,
        options.extra_pad,
        options.signal_scheme,
        options.do_custom_uncerts,
        options.add_stat_to_syst,
        options.add_flat_uncert,
        options.uncert_title,
        options.lumi,
        plot_name,
        custom_uncerts_up_name,
        custom_uncerts_down_name,
        scheme
        )
    else:    
      plotting.HTTPlotSignal(nodename, 
        plot_file, 
        options.signal_scale, 
        options.draw_signal_mass,
        options.norm_bins,
        options.channel,
        options.blind,
        options.x_blind_min,
        options.x_blind_max,
        options.ratio,
        options.log_y,
        options.log_x,
        options.ratio_range,
        options.custom_x_range,
        options.x_axis_min,
        options.x_axis_max,
        options.custom_y_range,
        options.y_axis_max,
        options.y_axis_min,
        x_title,
        y_title,
        options.extra_pad,
        options.signal_scheme,
        options.do_custom_uncerts,
        options.add_stat_to_syst,
        options.add_flat_uncert,
        options.uncert_title,
        options.lumi,
        plot_name,
        custom_uncerts_up_name,
        custom_uncerts_down_name
        )
   # hists = [plot_file.Get(nodename+"/bbH-LO700"), plot_file.Get(nodename+"/bbH700") ]
   # plotting.CompareHists(hists,
   #          ['Pythia','amc@NLO'],
   #          "bb#phi 700",
   #          options.ratio,
   #          options.log_y,
   #          options.log_x,
   #          options.ratio_range,
   #          options.custom_x_range,
   #          options.x_axis_max,
   #          options.x_axis_min,
   #          options.custom_y_range,
   #          options.y_axis_max,
   #          options.y_axis_min,
   #          x_title,
   #          y_title,
   #          options.extra_pad,
   #          False,
   #          plot_name,
   #          "#mu#tau_{h}")
           
