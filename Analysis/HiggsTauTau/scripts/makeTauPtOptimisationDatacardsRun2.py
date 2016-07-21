#!/usr/bin/env python

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
                  help="The config file that will be passed to HiggsTauTauPlot5. Parameter file, input folder and signal scheme taken from this cfg file unless overriden by command line options")
parser.add_option("-i","--input", dest="folder", type='string', default='',
                  help="The input folder, containing the output of HTT - can be used to override config file")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--blind", dest="blind", action='store_true', default=False,
                  help="blind data")
parser.add_option("--extra", dest="extra", type='string', default='',
                  help="Extra command line options, applied to every datacard")
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='',
                  help="datacard scheme - can be used to override config file")
parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
                  help="Make inputs for visible mass.")
parser.add_option("--svfit", dest="svfit", action='store_true', default=False,
                  help="Make inputs for svfit mass.")
parser.add_option("--mttot", dest="mttot", action='store_true', default=False,
                  help="Make inputs for total transverse mass.")
parser.add_option("--mtlep", dest="mtlep", action='store_true', default=False,
                  help="Make inputs for mt(lep1,lep2).")
parser.add_option("--mt1", dest="mt1", action='store_true', default=False,
                  help="Make inputs for mt(lep1,met).")
parser.add_option("--mt2", dest="mt2", action='store_true', default=False,
                  help="Make inputs for mt(lep2,met).")
parser.add_option("--mtsq", dest="mtsq", action='store_true', default=False,
                  help="Make inputs for sqrt(mt1*mt1+mt2*mt2).")
parser.add_option("--mtsv", dest="mtsv", action='store_true', default=False,
                  help="Make inputs for total transverse svfit mass.")
parser.add_option("--constant_binning",dest="const", action='store_true', default=False,
                  help="Use constant binning (196,0,3920)")
parser.add_option("--add_sm",dest="add_sm",action='store_true', default=False,
                  help="Add SM H as background")
#parser.add_option("--dijet", dest="dijet", action='store_true', default=False,
#                  help="Make inputs for dijet mass.")
#parser.add_option("--mttbb", dest="mttbb", action='store_true', default=False,
#                  help="Make inputs for mttbb.")
#parser.add_option("--mH", dest="mH", action='store_true', default=False,
#                  help="Make inputs for mH using kinematic fitting.")
#parser.add_option("--masscuts", dest="masscuts", action='store_true', default=False,
#                  help="Apply mass cuts on m_bb and m_tautau to 4 body mass for H->hh analysis.")
parser.add_option("-e", dest="energy", type='string', default='13',
                  help="The C.O.M. energy is written into the datacard name, default is 13")
parser.add_option("--no_shape_systs", dest="no_shape_systs", action='store_true', default=False,
                  help="Do not add shape systematics")
parser.add_option("--norm_systs", dest="norm_systs", action='store_true', default=False,
                  help="Add shapes for evaluating normalisation uncerts")

(options, args) = parser.parse_args()

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
BLIND = "false"
if options.blind: BLIND = "true"
COM = options.energy
YEAR = '2015'

#Hacky config file parsing
with open("%(CFG)s"%vars(),"r") as cfgfile:
  lines = cfgfile.readlines()

configmap={}

for ind in range(0,len(lines)):
  if len(lines[ind].split("="))>1:
    configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])
if "signal_scheme" in configmap:
  SCHEME= configmap["signal_scheme"].rstrip('\n')
FOLDER=configmap["folder"].rstrip('\n')
PARAMS=configmap["paramfile"].rstrip('\n')

#Override config file params
if not options.scheme == "":
  SCHEME=options.scheme
if not options.folder ==  "":
  FOLDER=options.folder
if not options.params == "":
  PARAMS=options.params


########## Set up schemes and options

#### Always apply these options:
#extra_global = ' --fix_empty_hists="ggH.*,ggA*,qqH.*,VH.*,WH.*,ZH.*,ttH.*,bbH.*"'
extra_global = ' '
if options.add_sm:
  extra_global = ' --add_sm_background="125" '

#### Apply these options for specific channels

extra_channel = {
    "et" : ' --syst_tau_scale="CMS_scale_t_et_13TeV" --syst_eff_t="CMS_eff_t_mssmHigh_et_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" ', 
    "mt" : ' --syst_tau_scale="CMS_scale_t_mt_13TeV" --syst_eff_t="CMS_eff_t_mssmHigh_mt_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" ',
    "tt" : ' --syst_tau_scale="CMS_scale_t_tt_13TeV" --syst_eff_t="CMS_eff_t_mssmHigh_tt_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" ',
    "em" : ' --syst_tau_scale="CMS_scale_e_em_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" ',
}

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' '
  }



if options.norm_systs:
  extra_channel["et"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  extra_channel["mt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  extra_channel["tt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  extra_channel["em"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  #extra_channel["et"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  #extra_channel["mt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  #extra_channel["tt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '
  #extra_channel["em"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_met_scale="CMS_scale_met_13TeV" --syst_met_res="CMS_scale_res_13TeV" '

#################################################################
#### Old SM scheme
#################################################################

if SCHEME == 'run2_sm':
#  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350]"
  scheme_et = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, '--qcd_os_ss_factor=1.0'),
    ("8",   "inclusive",    "inclusivemt40",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 ')
#    ("5",   "vbf",          "vbf",        BINS,      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  '--qcd_os_ss_factor=1.17'),
    ("8",   "inclusive",    "inclusivemt40",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.17')
#    ("5",   "vbf",          "vbf",        BINS,       ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV:50:1.15:0.15"'),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_tt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  '')
#    ("5",   "vbf",          "vbf",        BINS,       ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV:50:1.15:0.15"'),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_em = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, '--qcd_os_ss_factor=2')
#    ("5",   "vbf",          "vbf",        BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, (
#      (' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')+
#       ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"')),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  sig_scheme = 'run2_sm'
  ANA = 'sm'

if SCHEME == 'run2_Hhh':
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  scheme_et = [
    ("8", "inclusive",  "inclusive", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8", "2jet0tag",  "2jet0tag", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8", "2jet1tag",  "2jet1tag", BINS_FINE, '--set_alias="sel:mt_1<30"'), 
    ("8", "2jet2tag",  "2jet2tag", BINS_FINE, '--set_alias="sel:mt_1<30"')
   ]

  scheme_em = [
    ("8", "inclusive",  "inclusive", BINS_FINE, '--set_alias="sel:mt_1<30"'),
   ]

  scheme_tt = [
    ("8", "inclusive",  "inclusive", BINS_FINE, '--set_alias="sel:mt_1<30"'),
   ]

  scheme_mt = [
    ("8", "inclusive",  "inclusive", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8", "2jet0tag",  "2jet0tag", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8", "2jet1tag",  "2jet1tag", BINS_FINE, '--set_alias="sel:mt_1<30"'), 
    ("8", "2jet2tag",  "2jet2tag", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8", "2jet0taghigh",  "2jet0taghigh", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8", "2jet1taghigh",  "2jet1taghigh", BINS_FINE, '--set_alias="sel:mt_1<30"'), 
    ("8", "2jet2taghigh",  "2jet2taghigh", BINS_FINE, '--set_alias="sel:mt_1<30"'),
   ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  sig_scheme = 'run2_Hhh'
  ANA = 'Hhh'

if SCHEME == 'run2_mssm_nocrs':
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
#  BINS_FINE_MTSV="[0,9.93847,19.8421,30.1921,39.6147,49.4733,60.2784,69.9045,79.0905,89.4836,101.242,109.027,120.346,129.599,139.564,150.295,161.851,170.045,178.654,187.698,202.13,223.114,252.433,271.843,300.063,323.135,347.981,403.552,503.981,599.075,694.743,786.038,889.33,1110.65,1288.02,1493.7,1689.99,1912.07,2110.56,2272.85,2508.8,2701.7,2909.43,3133.14,3291.76,3458.4,3724.32,3912.87]"
#  BINS_MTSV="[0,19.8421,39.6147,60.2784,79.0905,101.242,120.346,139.564,161.851,178.654,202.13,252.433,300.063,347.981,403.552,503.981,694.743,889.33,1110.65,1288.02,1493.7,1689.99,1912.07,2110.56,2272.85,2508.8,2701.7,2909.43,3133.14,3291.76,3458.4,3724.32,3912.87]"

  scheme_et = [
    ("12",   "inclusive",  "inclusive",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 '),
    ("12",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 '),
    ("14",   "btag",    "btag",  BINS, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 '),
  ]
  scheme_mt = [
    ("12",   "inclusive",  "inclusive",  BINS_FINE, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 '),
    ("12",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 '),
    ("14",   "btag",    "btag",  BINS, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 '),
  ]
  scheme_tt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("8",   "nobtag",    "nobtag",  BINS_FINE, ''),
    ("8",   "btag",    "btag",  BINS, ''),

  ]
  scheme_em = [
    ("15",   "inclusive",    "inclusive",  BINS_FINE, '--set_alias="sel:pzeta>-20" '),
    ("15",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("15",   "btag",    "btag",  BINS, '--set_alias="sel:pzeta>-20"')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  sig_scheme = 'run2_mssm'
  ANA = 'mssm'


if SCHEME == 'run2_mssm':
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  if options.const:
    BINS_FINE="(98,0,3920)"
    BINS="(98,0,3920)"
#  BINS_FINE_MTSV="[0,9.93847,19.8421,30.1921,39.6147,49.4733,60.2784,69.9045,79.0905,89.4836,101.242,109.027,120.346,129.599,139.564,150.295,161.851,170.045,178.654,187.698,202.13,223.114,252.433,271.843,300.063,323.135,347.981,403.552,503.981,599.075,694.743,786.038,889.33,1110.65,1288.02,1493.7,1689.99,1912.07,2110.56,2272.85,2508.8,2701.7,2909.43,3133.14,3291.76,3458.4,3724.32,3912.87]"
#  BINS_MTSV="[0,19.8421,39.6147,60.2784,79.0905,101.242,120.346,139.564,161.851,178.654,202.13,252.433,300.063,347.981,403.552,503.981,694.743,889.33,1110.65,1288.02,1493.7,1689.99,1912.07,2110.56,2272.85,2508.8,2701.7,2909.43,3133.14,3291.76,3458.4,3724.32,3912.87]"

  scheme_et = [
    ("12",   "inclusive",  "inclusive",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 '),
    ("12",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 '),
    ("12",   "nobtag",    "nobtag_wjets_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.0 '),
    ("12",   "nobtag",    "nobtag_wjets_ss_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.0 --do_ss=true '),
    ("12",   "nobtag",    "nobtag_qcd_cr",  BINS_FINE, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 --do_ss=true '),
    ("14",   "btag",    "btag",  BINS, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 '),
    ("14",   "btag",    "btag_wjets_cr",  BINS, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.0 '),
    ("14",   "btag",    "btag_wjets_ss_cr",  BINS, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.0 --do_ss=true '),
    ("14",   "btag",    "btag_qcd_cr",  BINS, '--set_alias="sel:mt_1<40" --qcd_os_ss_factor=1.0 --do_ss=true '),
  ]
  scheme_mt = [
    ("12",   "inclusive",  "inclusive",  BINS_FINE, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 '),
    ("12",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 '),
    ("12",   "nobtag",    "nobtag_wjets_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.17 '),
    ("12",   "nobtag",    "nobtag_wjets_ss_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.17 --do_ss=true '),
    ("12",   "nobtag",    "nobtag_qcd_cr",  BINS_FINE, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 --do_ss=true '),
    ("14",   "btag",    "btag",  BINS, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 '),
    ("14",   "btag",    "btag_wjets_cr",  BINS, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.17 '),
    ("14",   "btag",    "btag_wjets_ss_cr",  BINS, '--set_alias="sel:mt_1>70" --qcd_os_ss_factor=1.17 --do_ss=true '),
    ("14",   "btag",    "btag_qcd_cr",  BINS, '--set_alias="sel:mt_1<30" --qcd_os_ss_factor=1.17 --do_ss=true '),
  ]
  scheme_tt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("8",   "nobtag",    "nobtag",  BINS_FINE, ''),
    ("8",   "btag",    "btag",  BINS, ''),
    ("8",   "nobtag",    "nobtagmvadboldvloosept40",  BINS_FINE, '--set_alias="baseline:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldvloosept40",  BINS, '--set_alias="baseline:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldloosept40",  BINS_FINE, '--set_alias="baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldloosept40",  BINS, '--set_alias="baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldmediumpt40",  BINS_FINE, '--set_alias="baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldmediumpt40",  BINS, '--set_alias="baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldtightpt40",  BINS_FINE, '--set_alias="baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldtightpt40",  BINS, '--set_alias="baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldvtightpt40",  BINS_FINE, ''),
    ("8",   "btag",    "btagmvadboldvtightpt40",  BINS, ''),
    ("8",   "nobtag",    "nobtagmvadboldvvtightpt60",  BINS_FINE, '--set_alias="baseline:(mva_olddm_vvtight_1>0.5 && mva_olddm_vvtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vvtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldvvtightpt60",  BINS, '--set_alias="baseline:(mva_olddm_vvtight_1>0.5 && mva_olddm_vvtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)"  --set_alias="tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vvtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldvloosept50",  BINS_FINE, '--set_alias="baseline:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldvloosept50",  BINS, '--set_alias="baseline:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldloosept50",  BINS_FINE, '--set_alias="baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldloosept50",  BINS, '--set_alias="baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldmediumpt50",  BINS_FINE, '--set_alias="baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldmediumpt50",  BINS, '--set_alias="baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldtightpt50",  BINS_FINE, '--set_alias="baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldtightpt50",  BINS, '--set_alias="baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldvtightpt50",  BINS_FINE, '--set_alias="sel:pt_1>50 && pt_2>50"'),
    ("8",   "btag",    "btagmvadboldvtightpt50",  BINS, '--set_alias="sel:pt_1>50 && pt_2>50"'),
    ("8",   "nobtag",    "nobtagmvadboldvvtightpt50",  BINS_FINE, '--set_alias="baseline:(mva_olddm_vvtight_1>0.5 && mva_olddm_vvtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vvtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldvvtightpt50",  BINS, '--set_alias="baseline:(mva_olddm_vvtight_1>0.5 && mva_olddm_vvtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>50 && pt_2>50" --set_alias="tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vvtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldvloosept60",  BINS_FINE, '--set_alias="baseline:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldvloosept60",  BINS, '--set_alias="baseline:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_vloose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldloosept60",  BINS_FINE, '--set_alias="baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldloosept60",  BINS, '--set_alias="baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldmediumpt60",  BINS_FINE, '--set_alias="baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldmediumpt60",  BINS, '--set_alias="baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldtightpt60",  BINS_FINE, '--set_alias="baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldtightpt60",  BINS, '--set_alias="baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_vloose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "nobtag",    "nobtagmvadboldvtightpt60",  BINS_FINE, '--set_alias="sel:pt_1>60 && pt_2>60"'),
    ("8",   "btag",    "btagmvadboldvtightpt60",  BINS, '--set_alias="sel:pt_1>60 && pt_2>60"'),
    ("8",   "nobtag",    "nobtagmvadboldvvtightpt60",  BINS_FINE, '--set_alias="baseline:(mva_olddm_vvtight_1>0.5 && mva_olddm_vvtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vvtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
    ("8",   "btag",    "btagmvadboldvvtightpt60",  BINS, '--set_alias="baseline:(mva_olddm_vvtight_1>0.5 && mva_olddm_vvtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)" --set_alias="sel:pt_1>60 && pt_2>60" --set_alias="tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vvtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"'),
  
  ]
  scheme_em = [
    ("15",   "inclusive",    "inclusive",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("15",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("15",   "btag",    "btag",  BINS, '--set_alias="sel:pzeta>-20"')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  sig_scheme = 'run2_mssm'
  ANA = 'mssm'


cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'em' : scheme_em,
  'tt' : scheme_tt
}



plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' , "60", "2500" if ANA=='mssm' else "120"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      , "100", "2500"  if ANA=='mssm' else "160"),
  ('mt_tot' , "m_{T}^{tot} [GeV]"        , '-mttot'      , "100", "3000"),
  ('mt_sv' , "m_{T#tau#tau} [GeV]"        , '-mtsv'      , "100", "3000"),
  ('mt_lep' , "m_{T}^{lep} [GeV]"        , '-mtlep'      , "100", "3000"),
  ('sqrt(mt_1*mt_1+mt_2*mt_2)' , "m_{T}^{sq} [GeV]"        , '-mtsq'      , "100", "3000"),
  ('mt_1' , "m_{T,1} [GeV]"        , '-mt1'      , "100", "3000"),
  ('mt_2' , "m_{T,2} [GeV]"        , '-mt2'      , "100", "3000")
 ]
if options.mvis:
    plots = plots[0]
if options.svfit: 
    plots = plots[1]
if options.mttot: 
    plots = plots[2]
if options.mtsv: 
    plots = plots[3]
if options.mtlep: 
    plots = plots[4]
if options.mtsq: 
    plots = plots[5]
if options.mt1: 
    plots = plots[6]
if options.mt2: 
    plots = plots[7]

var     = plots[0]
xlab    = plots[1]
dc_app  = plots[2]
blind_min = plots[3]
blind_max = plots[4]
for ch in channels:
  scheme = cat_schemes[ch]
  bkg_scheme = bkg_schemes[ch]
  for x in scheme:
    cat_num = x[0]
    cat_str = x[1]
    dc      = x[2]
    bin     = x[3]
    opts    = x[4]
    extra = options.extra + extra_global + extra_channel[ch] + opts

    os.system('$CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/bin/HiggsTauTauPlot5 --cfg=%(CFG)s --channel=%(ch)s'
        ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s%(bin)s" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbosity=0'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
  varsplit = var.split('(')
  varname=varsplit[0]
  os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(varname)s_*_%(ch)s_%(YEAR)s.root' % vars())
  os.system('rm datacard_%(varname)s_*_%(ch)s_%(YEAR)s.root' % vars())

