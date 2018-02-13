#!/usr/bin/env python

#./scripts/makeDatacardsSMCP16.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg -c 'mt,tt' scripts/Params_2016_smsummer16.json -s 'cpsummer16'

import sys
from optparse import OptionParser
import os
import string

CHANNELS= ['et', 'mt', 'em','tt']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',default='',
                  help="Specify the parameter file containing the luminosity and cross section information - can be used to override config file.")
parser.add_option("--cfg", dest="config", type='string',
                  help="The config file that will be passed to HiggsTauTauPlot.py. Parameter file, input folder and signal scheme taken from this cfg file unless overriden by command line options")
parser.add_option("-i","--input", dest="folder", type='string', default='',
                  help="The input folder, containing the output of HTT - can be used to override config file")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("--output_folder", dest="output_folder", type='string', default='./',
                  help="Output folder where plots/datacards will be saved to.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--blind", dest="blind", action='store_true', default=False,
                  help="blind data")
parser.add_option("--extra", dest="extra", type='string', default='',
                  help="Extra command line options, applied to every datacard")
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='',
                  help="datacard scheme - can be used to override config file")
parser.add_option("-e", dest="energy", type='string', default='13',
                  help="The C.O.M. energy is written into the datacard name, default is 13")
parser.add_option("--no_shape_systs", dest="no_shape_systs", action='store_true', default=False,
                  help="Do not add shape systematics")
parser.add_option("--total_jes", dest="total_jes", action='store_true', default=False,
                  help="Do not split JES by source")
parser.add_option("--norm_systs", dest="norm_systs", action='store_true', default=False,
                  help="Add shapes for evaluating normalisation uncerts")
parser.add_option("--year", dest="year", type='string', default='',
                  help="Output names are data-taking year dependent. This value is read from the config file if present")

(options, args) = parser.parse_args()
output_folder = options.output_folder
output = options.output
if output: 
  output = '-'+output
  print 'Appending "%(output)s" to datacard outputs' % vars()

channels = options.channels

print 'Processing channels:      %(channels)s' % vars()

### Do some validation of the input
helpMsg = "Run 'datacard_inputs.py -h' for help."

if not channels:
  print 'Error, no channels specified. ' + helpMsg
  sys.exit(1)

for channel in channels:
  validate_channel(channel)


CFG=options.config
BLIND = options.blind
BLIND = " "
if options.blind: BLIND = "--blind"
COM = options.energy

#Hacky config file parsing
with open("%(CFG)s"%vars(),"r") as cfgfile:
  lines = cfgfile.readlines()

configmap={}

for ind in range(0,len(lines)):
  if len(lines[ind].split("="))>1:
    configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])
if "signal_scheme" in configmap:
  SCHEME= configmap["signal_scheme"].rstrip('\n')
YEAR=2015
if "year" in configmap:
  YEAR=configmap["year"].rstrip('\n')
FOLDER=configmap["folder"].rstrip('\n')
PARAMS=configmap["paramfile"].rstrip('\n')

#Override config file params
if not options.scheme == "":
  SCHEME=options.scheme
if not options.folder ==  "":
  FOLDER=options.folder
if not options.params == "":
  PARAMS=options.params
if not options.year == "":
  YEAR=options.year


########## Set up schemes and options

#### Always apply these options:

extra_global = ' '


#### Apply these options for specific channels

# update these to latest shape systematics
extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' '
  }
if not options.total_jes:
  jes_systematics = ' --syst_scale_j_by_source="CMS_scale_j_SOURCE_13TeV" '
else: 
  jes_systematics = ' --syst_scale_j="CMS_scale_j_13TeV" '
common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_z_mjj="CMS_htt_zmumuShape_VBF_13TeV" --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_scale_met_clustered="CMS_scale_met_clustered_13TeV" '
em_shape_systematics=' --syst_tau_scale="CMS_scale_e_em_13TeV" '
et_shape_systematics=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" --syst_qcd_shape_wsf="WSFUncert_et_cat_13TeV" --syst_tau_id_dm0="CMS_tauDMReco_1prong_13TeV" --syst_tau_id_dm1="CMS_tauDMReco_1prong1pizero_13TeV" --syst_tau_id_dm10="CMS_tauDMReco_3prong_13TeV" --syst_lfake_dm0="CMS_eFakeTau_1prong_13TeV" --syst_lfake_dm1="CMS_eFakeTau_1prong1pizero_13TeV"  '
mt_shape_systematics=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" --syst_qcd_shape_wsf="WSFUncert_mt_cat_13TeV" --syst_tau_id_dm0="CMS_tauDMReco_1prong_13TeV" --syst_tau_id_dm1="CMS_tauDMReco_1prong1pizero_13TeV" --syst_tau_id_dm10="CMS_tauDMReco_3prong_13TeV" --syst_lfake_dm0="CMS_mFakeTau_1prong_13TeV" --syst_lfake_dm1="CMS_mFakeTau_1prong1pizero_13TeV" '
tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '

#common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_z_mjj="CMS_htt_zmumuShape_VBF_13TeV" '
#em_shape_systematics=' --syst_tau_scale="CMS_scale_e_em_13TeV" '
#et_shape_systematics=' --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" --syst_qcd_shape_wsf="WSFUncert_et_cat_13TeV" --syst_tau_id_dm0="CMS_tauDMReco_1prong_13TeV" --syst_tau_id_dm1="CMS_tauDMReco_1prong1pizero_13TeV" --syst_tau_id_dm10="CMS_tauDMReco_3prong_13TeV" --syst_lfake_dm0="CMS_eFakeTau_1prong_13TeV" --syst_lfake_dm1="CMS_eFakeTau_1prong1pizero_13TeV"  '
#mt_shape_systematics=' --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" --syst_qcd_shape_wsf="WSFUncert_mt_cat_13TeV" --syst_tau_id_dm0="CMS_tauDMReco_1prong_13TeV" --syst_tau_id_dm1="CMS_tauDMReco_1prong1pizero_13TeV" --syst_tau_id_dm10="CMS_tauDMReco_3prong_13TeV" --syst_lfake_dm0="CMS_mFakeTau_1prong_13TeV" --syst_lfake_dm1="CMS_mFakeTau_1prong1pizero_13TeV" '
#tt_shape_systematics=' '

extra_channel = {
      "et" : ' '+common_shape_systematics+ ' '+et_shape_systematics,
      "mt" : ' '+common_shape_systematics+ ' '+mt_shape_systematics,
      "tt" : ' '+common_shape_systematics+ ' '+tt_shape_systematics,
      "em" : ' '+common_shape_systematics+ ' '+em_shape_systematics
  }

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' '
  }


if SCHEME == 'cpsummer16':
  
  VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'
  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'sjdphi(20,-3.2,3.2)' 
  VAR_DIJET_WIDEBINS = 'sjdphi(10,-3.2,3.2)' 
  
  VAR_0JET_LT_WCR = 'mt_1[80,200]'
  VAR_BOOSTED_WCR = 'mt_1[80,200]'
  
  VAR_0JET_LT_QCDCR = 'm_vis(4,40,200)' 
  VAR_BOOSTED_LT_QCDCR = 'm_sv(4,40,200)'
  
  VAR_0JET_TT_QCDCR = 'm_sv[0,300]'
  VAR_BOOSTED_TT_QCDCR = 'm_sv[0,300]' 

  scheme_et = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" --set_alias="w_shape:({w_shape}&&iso_1>0.1)" '),
    ("21",   "dijet",     "dijet",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_WIDEBINS, '--set_alias="sel:mt_1<50" ')


  ]
  scheme_mt = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:(mt_1>80&&mt_1<200)" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" --set_alias="w_shape:({w_shape}&&iso_1>0.15)" '),
    ("21",   "dijet",     "dijet",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_WIDEBINS, '--set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_0JET_TT_QCDCR, ' --do_ss '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_BOOSTED_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet",     "dijet",  VAR_DIJET, ''),
    ("8",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ''),
    ("8",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ''),
    ("8",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ''),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_WIDEBINS, ''),
    ("8",   "dijet",      "dijet_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_lowM",      "dijet_lowM_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_highM",      "dijet_highM_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_lowboost",     "dijet_lowboost_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_DIJET_WIDEBINS, ' --do_ss ')
  ]
  scheme_em = [
    ("15",   "0jet",    "0jet",  VAR_0JET_EM, '--set_alias="sel:mt_1<50" '),
    ("15",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("15",   "dijet",     "dijet",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  ANA = 'sm'
  
if SCHEME == 'cpsummer16_aachen':
  
  VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'
  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'sjdphi(12,-3.2,3.2)' 
  VAR_DIJET_THINBINS = 'sjdphi(20,-3.2,3.2)' # use this with ic cuts
  
  VAR_0JET_LT_WCR = 'mt_1[80,200]'
  VAR_BOOSTED_WCR = 'mt_1[80,200]'
  
  VAR_0JET_LT_QCDCR = 'm_vis(4,40,200)' 
  VAR_BOOSTED_LT_QCDCR = 'm_sv(4,40,200)'
  
  VAR_0JET_TT_QCDCR = 'm_sv[0,300]'
  VAR_BOOSTED_TT_QCDCR = 'm_sv[0,300]'
  
  VAR_DIJET_TT_QCD = 'sjdphi(1,-3.2,3.2)'

  scheme_et = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" --set_alias="w_shape:({w_shape}&&iso_1>0.1)" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')


  ]
  scheme_mt = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:(mt_1>80&&mt_1<200)" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" --set_alias="w_shape:({w_shape}&&iso_1>0.15)" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_0JET_TT_QCDCR, ' --do_ss '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_BOOSTED_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ''),
    ("8",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ''),
    ("8",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, ''),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ''),
    ("8",   "dijet_lowM",      "dijet_lowM_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_highM",      "dijet_highM_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_lowMjj",     "dijet_lowMjj_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss ')
  ]
  #scheme_em = [
  #  ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
  #  ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
  #  ("19",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
  #  ("19",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
  #  ("19",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
  #  ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" ')
  #]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" ')
  ]

  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  ANA = 'sm'


if SCHEME == 'cpsummer16_2d':
  
  VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'

  #VAR_0JET_LT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'
  #VAR_0JET_EM = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'


  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'm_sv,sjdphi[0,80,100,115,130,150],(12,-3.2,3.2)' 
  #VAR_DIJET = 'm_sv,sjdphi[0,100,150],(20,-3.2,3.2)'  
  #VAR_DIJET = 'm_sv,D0[0,100,150],(10,0,1)'   
  VAR_DIJET = 'm_sv,D0star[0,80,100,115,130,150],(12,-1,1)' 
 
  VAR_DIJET_TT_QCD = 'sjdphi(1,-3.2,3.2)' 
  
  VAR_0JET_LT_WCR = 'mt_1[80,200]'
  VAR_BOOSTED_WCR = 'mt_1[80,200]'
  
  VAR_0JET_LT_QCDCR = 'm_vis(4,40,200)' 
  VAR_BOOSTED_LT_QCDCR = 'm_sv(4,40,200)'
  
  VAR_0JET_TT_QCDCR = 'm_sv[0,300]'
  VAR_BOOSTED_TT_QCDCR = 'm_sv[0,300]' 

  scheme_et = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, ' --set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, ' --set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" --set_alias="w_shape:({w_shape}&&iso_1>0.1)" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:mt_1<50"  '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')
  ]
  scheme_mt = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:(mt_1>80&&mt_1<200)" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" --set_alias="w_shape:({w_shape}&&iso_1>0.15)" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:mt_1<50"  '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_0JET_TT_QCDCR, ' --do_ss '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_BOOSTED_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' '),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' '),
    ("8",   "dijet_lowboost",      "dijet_lowboost_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  ANA = 'sm'


cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'em' : scheme_em,
  'tt' : scheme_tt
}


dc_app='-2D'
for ch in channels:
  scheme = cat_schemes[ch]
  bkg_scheme = bkg_schemes[ch]
  for x in scheme:
    cat_num = x[0]
    cat_str = x[1]
    dc      = x[2]
    var     = x[3]
    opts    = x[4]
    extra = options.extra + ' ' + extra_global + ' ' + extra_channel[ch] + ' ' + opts
    extra_jes = options.extra + ' ' + extra_global + ' ' + jes_systematics + ' ' + opts + ' --no_default '

    
    os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
              ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s '
              ' --var="%(var)s" %(extra)s --no_plot' % vars())
    
    if jes_systematics and not options.no_shape_systs:
      # have to do this to avoid using too much memory...  
      os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes1' 
              ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s '
              ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=1:9' % vars())    
      os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes2'
              ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s '
              ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=10:18' % vars()) 
      os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes3'
              ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s '
              ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=19:27' % vars())
              
  os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
  os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())

