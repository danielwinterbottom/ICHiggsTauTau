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

CHANNELS= ['tttt','ettt','mttt','tttt','eett','mmtt']

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
    "era":"mssmsummer16", "sel":"(1)", "set_alias":[], "analysis":"mssm", "var":"m_vis(7,0,140)",
    "method":8 , "do_ss":False, "sm_masses":"125", "ggh_masses":"", "bbh_masses":"",
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
    "doMSSMReWeighting":False, "do_unrolling":1, "syst_tau_id_dm0":"", "syst_tau_id_dm1":"",
    "syst_tau_id_dm10":"", "syst_lfake_dm0":"","syst_lfake_dm1":"","syst_qcd_shape_wsf":"",
    "syst_scale_met_unclustered":"","syst_scale_met_clustered":"",
    "extra_name":"", "no_default":False, "no_systs":False, "embedding":False,"syst_embedding_tt":"",
    "vbf_background":False, "syst_em_qcd":"", "syst_prefire":"",
    "syst_scale_met":"", "syst_res_met":"", "split_sm_scheme": False,
    "ggh_scheme": "powheg", "symmetrise":False, "mergeXbins":False, 'em_qcd_weight':"",
    "syst_scale_j_corr":"","syst_scale_j_uncorr":"", "syst_qcd_bkg":"",
    "ff_ss_closure":False, "threePads":False,"auto_blind":False,
    "syst_tau_id_diff":"", "syst_tau_trg_diff":"",
    "syst_scale_j_regrouped":"", "syst_tau_scale_grouped":"",

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
parser.add_argument("--era", dest="era", type=str,
    help="Era")
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
print 'era               = ' + options.era
print 'sel               = ' + options.sel
print 'analysis          = ' + options.analysis
print 'var               = ' + options.var
print '###############################################'
print ''


cats = {}


VsJets_wp = "vvvloose"
VsMu_wp = "vloose"
VsEle_wp = "vvloose"
lepton_iso = "0.15"

t_sel = "deepTauVsJets_%(VsJets_wp)s_X>0.5 && deepTauVsMu_%(VsMu_wp)s_X>0.5 && deepTauVsEle_%(VsEle_wp)s_X>0.5" % vars()
e_sel = "iso_X<%(lepton_iso)s" % vars()
m_sel = "iso_X<%(lepton_iso)s" % vars()


if options.charges_non_zero: 
  charge_sel = "(q_1+q_2+q_3+q_4)!=0"
else:
  charge_sel = "(q_1+q_2+q_3+q_4)==0"

if options.channel == "tttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
elif options.channel == "ettt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
elif options.channel == "mttt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
elif options.channel == "emtt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
elif options.channel == "mmtt":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))
elif options.channel == "eett":
  cats['baseline'] = "({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=e_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4"))

cats['baseline'] = "(({charge_sel}) && {current_baseline})".format(charge_sel=charge_sel,current_baseline=cats['baseline'])

cats['inclusive'] = '(1)' 
cats['btag'] = '(n_bjets>=1)'
cats['nobtag'] = '(n_bjets==0)'

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
if options.channel == 'tttt':
  if options.year == "2016-preVFP":
    data_samples = ['TauB','TauC','TauD','TauE','TauF']
  elif options.year == "2016-postVFP":
    data_samples = ['TauF','TauG','TauH']
  elif options.year == "2017":
    data_samples = ['TauB','TauC','TauD','TauE','TauF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD'] 

elif options.channel == 'ettt':
  if options.year == "2016-preVFP":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
  elif options.year == "2016-postVFP":
    data_samples = ['TauF','TauG','TauH','SingleElectronF','SingleElectronG','SingleElectronH']
  elif options.year == "2017":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','EGammaA','EGammaB','EGammaC','EGammaD']

elif options.channel == 'mttt':
  if options.year == "2016-preVFP":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
  elif options.year == "2016-postVFP":
    data_samples = ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH']
  elif options.year == "2017":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD']

elif options.channel == 'eett':
  if options.year == "2016-preVFP":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','DoubleEGB','DoubleEGC','DoubleEGD','DoubleEGE','DoubleEGF']
  elif options.year == "2016-postVFP":
    data_samples = ['TauF','TauG','TauH','DoubleEGF','DoubleEGG','DoubleEGH']
  elif options.year == "2017":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','DoubleEGB','DoubleEGC','DoubleEGD','DoubleEGE','DoubleEGF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','EGammaA','EGammaB','EGammaC','EGammaD']

elif options.channel == 'mmtt':
  if options.year == "2016-preVFP":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','DoubleMuonB','DoubleMuonC','DoubleMuonD','DoubleMuonE','DoubleMuonF']
  elif options.year == "2016-postVFP":
    data_samples = ['TauF','TauG','TauH','DoubleMuonF','DoubleMuonG','DoubleMuonH']
  elif options.year == "2017":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','DoubleMuonB','DoubleMuonC','DoubleMuonD','DoubleMuonE','DoubleMuonF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','DoubleMuonA','DoubleMuonB','DoubleMuonC','DoubleMuonD']

elif options.channel == 'emtt':
  if options.year == "2016-preVFP":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','MuonEGB','MuonEGC','MuonEGD','MuonEGE','MuonEGF']
  elif options.year == "2016-postVFP":
    data_samples = ['TauF','TauG','TauH','MuonEGF','MuonEGG','MuonEGH']
  elif options.year == "2017":
    data_samples = ['TauB','TauC','TauD','TauE','TauF','MuonEGB','MuonEGC','MuonEGD','MuonEGE','MuonEGF']
  elif options.year == "2018":
    data_samples = ['TauA','TauB','TauC','TauD','MuonEGA','MuonEGB','MuonEGC','MuonEGD']

if options.year == "2018":
    #ztt_samples = ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
    ztt_samples = ['DYJetsToLL-LO','DYJetsToLL_M-10-50-LO']
    top_samples = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
    vv_samples = [
            'T-tW-ext1', 'Tbar-tW-ext1','Tbar-t','WWTo2L2Nu','T-t',
            'WWToLNuQQ','WZTo1L3Nu','WZTo3LNu','WZTo3LNu-ext1','WZTo2L2Q',
            'ZZTo2L2Nu-ext1','ZZTo2L2Nu-ext2','ZZTo2L2Q','ZZTo4L-ext','ZZTo4L'
            ]
    #wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
    wjets_samples = ['WJetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
    wgam_samples = ['WGToLNuG']
    ewkz_samples = ['EWKZ2Jets']

ROOT.TH1.SetDefaultSumw2(True)

z_sels = {}
top_sels = {}
vv_sels = {}

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
    ztt_node = GetNode(ana, "ZTT", add_name, samples, plot, wt, sel, cat)  
    ana.nodes[nodename].AddNode(ztt_node)
    
def GenerateEWKZ(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}):
    ewkz_node = GetNode(ana, "EWKZ", add_name, samples, plot, wt, sel, cat)  
    ana.nodes[nodename].AddNode(ewkz_node) 

def GenerateTop(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}):
    tt_node = GetNode(ana, "TT", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(tt_node)

def GenerateVV(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}):
    vv_node = GetNode(ana, "VV", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(vv_node)

def GenerateW(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    wg_node = GetNode(ana, "WGam", add_name, wgam_samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(wg_node)
    w_node = GetNode(ana, "W", add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(w_node)
 
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
      weight='wt'
      if options.add_wt : weight+='*'+options.add_wt
      full_selection = BuildCutString(weight, sel, cat_data)
      ana.nodes[nodename].AddNode(ana.SummedFactory('data_obs', data_samples, plot_unmodified, full_selection))
   
    # produce templates for backgrounds
    if 'ZTT' not in samples_to_skip:
        GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels)
    if 'TT' not in samples_to_skip:    
        GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels) 
    if 'VV' not in samples_to_skip:
        GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels) 
    if 'W' not in samples_to_skip:
        GenerateW(ana, add_name, wjets_samples, plot, wt, sel, cat)


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

if options.datacard != "": datacard_name = options.datacard
else: datacard_name = options.cat
if options.extra_name != "": 
  output_name = options.outputfolder+'/datacard_'+options.extra_name+'_'+datacard_name+'_'+options.channel+'_'+options.year+'.root'
else: 
  output_name = options.outputfolder+'/datacard_'+var_name+'_'+datacard_name+'_'+options.channel+'_'+options.year+'.root'
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

max_systs_per_pass = 30 # code uses too much memory if we try and process too many systematics at once so set the maximum number of systematics processed per loop here
while len(systematics) > 0:
  ana = Analysis()
  
  ana.remaps = {}
  if options.channel == 'emtt':
      ana.remaps['MuonEG'] = 'data_obs'
  elif options.channel in ['mttt']:
      ana.remaps['SingleMuon'] = 'data_obs'
  elif options.channel in ['mmtt']:
      ana.remaps['DoubleMuon'] = 'data_obs'
  elif options.year != '2018' and options.channel in ['ettt']:
      ana.remaps['SingleElectron'] = 'data_obs'
  elif options.year != '2018' and options.channel in ['eett']:
      ana.remaps['DoubleEG'] = 'data_obs'
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
      for sample_name in ztt_samples + vv_samples + wgam_samples + top_samples + wjets_samples + ewkz_samples:
          ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'_{}.root'.format(options.year), 'ntuple', None, sample_name)
          
      ana.AddInfo(options.paramfile, scaleTo='data_obs')
  
      # Add data only for default
      if systematic == 'default': do_data = True
      else: do_data = False
              
      #Run default plot
      do_data=False 
      weight=""
      RunPlotting(ana, cats['cat'], cats_unmodified['cat'], sel, add_name, weight, do_data, samples_to_skip,outfile)
      del systematics[systematic]

  ana.Run()
  ana.nodes.Output(outfile)

  # fix negative bns,empty histograms etc.
  FixBins(ana,outfile)
  for n in add_names: 
    GetTotals(ana,n,outfile)
  PrintSummary(nodename, ['data_obs'], add_names)

outfile.Close()

plot_file = ROOT.TFile(output_name, 'READ')


if not options.no_plot:
    if options.extra_name != '': vname = options.extra_name
    else: vname = var_name

    if options.datacard != "": plot_name = options.outputfolder+'/'+vname+'_'+options.datacard+'_'+options.channel+'_'+options.year
    else: plot_name = options.outputfolder+'/'+vname+'_'+options.cat+'_'+options.channel+'_'+options.year
    if options.log_x: plot_name += "_logx" 
    if options.log_y: plot_name += "_logy"
    titles = plotting.SetAxisTitles(options.var,options.channel)
    if options.x_title == "": 
      x_title = titles[0]
    else: x_title = options.x_title
    
    if options.y_title == "": 
        y_title = titles[1]
    else: y_title = options.y_title

    plotting.HTTPlot(nodename, 
      plot_file, 
      1, 
      "",
      False,
      options.norm_bins,
      options.channel,
      options.blind,
      options.x_blind_min,
      options.x_blind_max,
      options.ratio,
      False,
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
      False,
      "",
      False,
      False,
      False,
      "",
      options.lumi,
      plot_name,
      "",
      "",
      options.channel,
      False,
      False,
      False,
      False,
      options.cat,
      False
      )

outfile =  ROOT.TFile(output_name, 'UPDATE')
outfile.Close()

