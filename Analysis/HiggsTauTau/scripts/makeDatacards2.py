#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

CHANNELS= ['et', 'mt', 'em','mtmet']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',
                  help="Specify the paramter file containing the luminosity and cross section information.")
parser.add_option("--cfg", dest="config", type='string',
                  help="The config file that will be passed to HiggsTauTauPlot3.")
parser.add_option("-i","--input", dest="folder", type='string',
                  help="The input folder, containing the output of HiggsTauTau")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--blind", dest="blind", action='store_true', default=False,
                  help="blind data")
parser.add_option("--extra", dest="extra", type='string', default='',
                  help="Extra command line options, applied to every datacard")
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='old_sm',
                  help="datacard scheme")
parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
                  help="Only make inputs for visible mass, no svfit.")
parser.add_option("--svfit", dest="svfit", action='store_true', default=False,
                  help="Only make inputs for svfit mass.")
parser.add_option("-e", dest="energy", type='string', default='8',
                  help="The C.O.M. energy is written into the datacard name, default is 8")

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


PARAMS=options.params
CFG=options.config
FOLDER = options.folder
BLIND = options.blind
BLIND = "false"
if options.blind: BLIND = "true"
COM = options.energy

########## Set up schemes and options

#### Always apply these options:
extra_global = ' --fix_empty_hists="ggH.*,qqH.*,VH.*,bbH.*"'

#### Apply these options for specific channels
extra_channel = {
  "et" : ' --fix_empty_bins="QCD.*,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD.*" --syst_tau_scale="CMS_scale_t_etau_'+COM+'TeV"',
  "mt" : ' --fix_empty_bins="QCD.*,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD.*" --syst_tau_scale="CMS_scale_t_mutau_'+COM+'TeV"',
  "mtmet" : ' --extra_pad=1.2 --fix_empty_bins="QCD.*,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD" --syst_tau_scale="CMS_scale_t_mutausoft_'+COM+'TeV"',
  "em" : ' --fix_empty_bins="Fakes"'
}

#################################################################
#### Old SM scheme
#################################################################
if options.scheme == 'old_sm':
  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  scheme_et = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ""),
    ("5",   "vbf",          "vbf",        BINS,      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("5",   "vbf",          "vbf",        BINS,       ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'),
    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_mtmet = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("5",   "vbf",          "vbf",        BINS,       ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'),
    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_em = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("5",   "vbf",          "vbf",        BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, (
      (' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')+
       ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"')),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'mtmet' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'sm_default'
  ANA = 'sm'

#################################################################
#### Thesis SM
#################################################################
if options.scheme == 'thesis_sm':
  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  scheme_et = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ""),
    ("5",   "th_vbf",       "vbf",        BINS,      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
    ("0",   "th_0jet_low",  "0jet_low",   BINS_FINE, ""),
    ("1",   "th_0jet_high", "0jet_high",  BINS_FINE, ""),
    ("2",   "th_1jet_low",  "boost_low",  BINS_FINE, ""),
    ("3",   "th_1jet_high", "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("5",   "th_vbf",          "vbf",        BINS,    ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
    ("0",   "th_0jet_low",     "0jet_low",   BINS_FINE,  ''),
    ("1",   "th_0jet_high",    "0jet_high",  BINS_FINE,  ''),
    ("2",   "th_1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'),
    ("3",   "th_1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_mtmet = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("5",   "th_vbf",       "vbf",        BINS,       ''),
    ("0",   "th_0jet_low",  "0jet_low",   BINS_FINE,  ''),
    ("1",   "th_0jet_high", "0jet_high",  BINS_FINE,  ''),
    ("2",   "th_1jet_low",  "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'),
    ("3",   "th_1jet_high", "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_em = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("5",   "th_vbf",       "vbf",        BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("0",   "th_0jet_low",  "0jet_low",   BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("1",   "th_0jet_high", "0jet_high",  BINS_FINE, (
      (' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')+
      ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"')),
    ("2",   "th_1jet_low",  "boost_low",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("3",   "th_1jet_high", "boost_high", BINS_FINE, (
      (' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')))
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'mtmet' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'sm_default'
  ANA = 'sm'

#################################################################
#### New SM scheme
#################################################################
if options.scheme == 'new_sm':
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  extra_channel["em"] += '  --set_alias="sel:em_gf_mva>-0.681"'
  if COM=='8': extra_global += ' --hww_masses=110,115,120,125,130,135,140,145,150,155,160'
  extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["et"] += ' --syst_zl_shift="CMS_htt_ZLScale_etau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mt"] += ' --syst_zl_shift="CMS_htt_ZLScale_mutau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mtmet"] += ' --set_alias="sel:mt_1<45." --set_alias="w_sdb:mt_1>45." --set_alias="w_vbf_sdb:mt_1>45."'
  scheme_et = [
    ("8",   "inclusive",                "inclusive",              BINS_FINE,  ""),
    ("5",   "new_vbf",                  "vbf",                    BINS,       (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_vbf_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_loose",            "vbf_loose",              BINS,       (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_vbf_loose_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_tight",            "vbf_tight",              BINS,       (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_vbf_tight_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="vbf_loose_jets20:(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0 && pt_tt>100.)"'
      ' --set_alias="twojet:(n_jets>=2 && n_jetsingap==0 && mjj>700. && jdeta>4.0 && pt_tt>100. && n_bjets==0)"'
      ' --set_alias="vbf_loose:(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && pt_tt>100.)"'
      ' --set_alias="w_vbf_extrap_cat:(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && pt_tt>100.)"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"')),
    ("0",   "new_0jet_low",             "0jet_low",               BINS_FINE,  ""),
    ("1",   "new_0jet_medium",          "0jet_medium",            BINS_FINE,  ""),
    ("1",   "new_0jet_high",            "0jet_high",              BINS_FINE,  ""),
    ("2",   "new_1jet_medium",          "1jet_medium",            BINS_FINE,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_1jet_medium_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_shape_os:1"')),
    ("3",   "new_1jet_high_lowhiggs",   "1jet_high_lowhiggs",     BINS_FINE,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_1jet_high_lowhiggs_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_shape_os:1"')),
    ("4",   "new_1jet_high_highhiggs",  "1jet_high_mediumhiggs",  BINS_FINE,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_1jet_high_mediumhiggs_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"'
      ' --set_alias="QCD_Shape_Sample:Special_4_Data"'))
  ]
  scheme_mt = [
    ("8",   "inclusive",                "inclusive",              BINS_FINE,  ""),
    ("5",   "new_vbf",                  "vbf",                    BINS, (
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_vbf_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_loose",            "vbf_loose",              BINS, (
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_vbf_loose_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_tight",            "vbf_tight",              BINS, (     
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_vbf_tight_'+COM+'TeV:60:1.0:0.10"'
      ' --set_alias="vbf_loose_jets20:(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0 && pt_tt>100.)"'
      ' --set_alias="twojet:(n_jets>=2 && n_jetsingap==0 && mjj>700. && jdeta>4.0 && pt_tt>100. && n_bjets==0)"'
      ' --set_alias="vbf_loose:(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && pt_tt>100.)"'
      ' --set_alias="w_vbf_extrap_cat:(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && pt_tt>100.)"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"')),
    ("0",   "new_0jet_low",             "0jet_low",               BINS_FINE,  ""),
    ("1",   "new_0jet_medium",          "0jet_medium",            BINS_FINE,  ""),
    ("1",   "new_0jet_high",            "0jet_high",              BINS_FINE,  ""),
    ("2",   "new_1jet_medium",          "1jet_medium",            BINS_FINE,  (
      ' --set_alias="w_shape_os:1"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet_medium_'+COM+'TeV":60:1.1:0.10')),
    ("3",   "new_1jet_high_lowhiggs",   "1jet_high_lowhiggs",     BINS_FINE,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet_high_lowhiggs_'+COM+'TeV":60:1.1:0.10'
      ' --set_alias="w_shape_os:1"')),
    ("4",   "new_1jet_high_highhiggs",  "1jet_high_mediumhiggs",  BINS_FINE,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet_high_mediumhiggs_'+COM+'TeV":60:1.1:0.10'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"'
      ' --set_alias="QCD_Shape_Sample:Special_4_Data"'))
  ]
  scheme_mtmet = [
    ("8",   "inclusive",                "inclusive",              BINS_FINE,  ' --syst_l1met=CMS_L1etm_mutausoft_inclusive_8TeV'),
    ("5",   "new_vbf",                  "vbf",                    BINS, (
      ' --syst_l1met=CMS_L1etm_mutausoft_vbf_8TeV'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="vbf_loose_jets20:(n_jets>=2 && n_jetsingap==0 && mjj>500. && jdeta>3.5 && n_bjets==0)"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="w_shape_os:1"')),
    ("0",   "new_0jet_low",             "0jet_low",               BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_0jet_low_8TeV'
      ' --qcd_os_ss_factor=1.058')),
    ("1",   "new_0jet_medium",          "0jet_medium",            BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_0jet_medium_8TeV'
      ' --qcd_os_ss_factor=0.981')),
    ("1",   "new_0jet_high",            "0jet_high",              BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_0jet_high_8TeV'
      ' --qcd_os_ss_factor=0.766')),
    ("2",   "new_1jet_medium",          "1jet_medium",            BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_1jet_medium_8TeV'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutausoft_1jet_medium_'+COM+'TeV"'
      ' --set_alias="w_shape_os:1" '
      ' --qcd_os_ss_factor=1.098')),
    ("3",   "new_1jet_high",            "1jet_high",              BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_1jet_high_8TeV'
      ' --set_alias="w_shape_os:1"'
      ' --qcd_os_ss_factor=1.016')),
    ("3",   "new_1jet_high_lowhiggs",   "1jet_high_lowhiggs",     BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_1jet_high_lowhiggs_8TeV'
      ' --set_alias="w_shape_os:1"'
      ' --qcd_os_ss_factor=1.016')),
    ("4",   "new_1jet_high_highhiggs",  "1jet_high_mediumhiggs",  BINS_FINE,  (
      ' --syst_l1met=CMS_L1etm_mutausoft_1jet_high_mediumhiggs_8TeV'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"'
      ' --set_alias="QCD_Shape_Sample:Special_4_Data"'))
  ]
  scheme_em = [
    ("8",   "inclusive",    "inclusive",              BINS_FINE, (
        ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("5",   "new_vbf_loose","vbf_loose",              BINS,      (
        ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
        ' --set_alias="vbf_no_cjv:(n_jets>=2 && mjj>500. && jdeta>3.5 && n_bjets==0)"'
        ' --set_alias="vbf_loose:(n_jets>=2 && mjj>500. && jdeta>3.5 && n_bjets==0)"')),
    ("5",   "vbf",          ("vbf" if COM=='8' else "vbf_loose"), BINS,      (
        ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
        ' --set_alias="vbf_no_cjv:(n_jets>=2 && mjj>500. && jdeta>3.5 && n_bjets==0)"'
        ' --set_alias="vbf_loose:(n_jets>=2 && mjj>500. && jdeta>3.5 && n_bjets==0)"')),
    ("5",   "new_vbf_tight","vbf_tight",              BINS,      (
        ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
        ' --set_alias="vbf_no_cjv:(n_jets>=2 && mjj>500. && jdeta>3.5 && pt_tt>100.)"'
        ' --set_alias="vbf_loose:(n_jets>=2 && mjj>500. && jdeta>3.5 && pt_tt>100.)"')),
    ("0",   "0jet_low",     "0jet_low",             BINS_FINE, (
        ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("1",   "0jet_high",    "0jet_high",            BINS_FINE, (
        (' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')+
        ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"')),
    ("2",   "1jet_low",     "1jet_low",             BINS_FINE, (
        ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("3",   "1jet_high",    "1jet_high",            BINS_FINE, (
        ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'))
  ]
  bkg_schemes = {
    'et'    : 'et_default',
    'mt'    : 'mt_with_zmm',
    'mtmet' : 'mt_default',
    'em'    : 'em_default'
  }
  sig_scheme = 'sm_default'
  ANA = 'sm'

#################################################################
#### Control Plots
#################################################################
if options.scheme == 'control_plots':
  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  extra_channel["em"] += '  --set_alias="sel:em_gf_mva>-0.681"'
  if COM=='8': extra_channel["em"] += ' --hww_masses=110,115,120,125,130,135,140,145,150,155,160'
  extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["et"] += ' --syst_zl_shift="CMS_htt_ZLScale_etau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mt"] += ' --syst_zl_shift="CMS_htt_ZLScale_mutau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mtmet"] += ' --set_alias="sel:mt_1<45." --set_alias="w_sdb:mt_1>45." --set_alias="w_vbf_sdb:mt_1>45."'
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  scheme_et = [
    ("8",   "inclusive",                "0jet_low",              BINS_FINE,  ""),
    ("2",   "twojet",                   "vbf_loose",             BINS,      (' --set_alias="w_shape_os:1"')),
    ("2",   "twojet",                   "vbf",                   BINS,      (' --set_alias="w_shape_os:1"')),
    ("2",   "1jet",                     "1jet_medium",           BINS_FINE, (' --set_alias="w_shape_os:1"'))
  ]
  scheme_mt = [
    ("8",   "inclusive",                "0jet_low",              BINS_FINE,  ""),
    ("2",   "twojet",                   "vbf_loose",             BINS,      (' --set_alias="w_shape_os:1"')),
    ("2",   "twojet",                   "vbf",                   BINS,      (' --set_alias="w_shape_os:1"')),
    ("2",   "1jet",                     "1jet_medium",           BINS_FINE, (' --set_alias="w_shape_os:1" --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet_medium_'+COM+'TeV"'))
  ]
  scheme_mtmet = [
    ("8",   "inclusive",                "inclusive",              BINS_FINE,  ' --syst_l1met=CMS_L1etm_mutausoft_inclusive_8TeV'),
    ("2",   "twojet",                   "vbf",                    BINS, (
      ' --syst_l1met=CMS_L1etm_mutausoft_vbf_8TeV'
      ' --set_alias="w_shape_os:1"')),
    ("2",   "1jet",                     "1jet_medium",            BINS_FINE, (
      ' --syst_l1met=CMS_L1etm_mutausoft_1jet_medium_8TeV'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutausoft_1jet_medium_'+COM+'TeV"'
      ' --set_alias="w_shape_os:1" '
      ' --qcd_os_ss_factor=1.098'))
  ]
  scheme_em = [
    ("8",   "inclusive",       "0jet_low",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("2",   "twojet",          "vbf_loose", BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("2",   "twojet",          "vbf",       BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("2",   "1jet",            "1jet_low",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'mtmet' : 'mt_default',
    'em' : 'em_default'
  }
  sig_scheme = 'sm_default'
  ANA = 'sm'

#################################################################
#### Old MSSM scheme
#################################################################
if options.scheme == 'old_mssm':
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
  scheme_et = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ''),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ''),
    ("12",   "btag",        "btag",       BINS,       ' --sub_ztt_top_frac=0.015') 
  ]
  scheme_mt = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ''),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ''),
    ("12",   "btag",        "btag",       BINS,       ' --sub_ztt_top_frac=0.015') 
  ]
  scheme_mtmet = scheme_mt
  scheme_em = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("12",   "btag",        "btag",       BINS,       ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV" --sub_ztt_top_frac=0.015') 
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'mssm_default'
  ANA = 'mssm'

#################################################################
#### New MSSM scheme
#################################################################
if options.scheme == 'new_mssm':
  #BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500,1600,1800,2000"
  # BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
  extra_global += ' --add_extra_binning="(400,0,2000):_fine_binning"'
  extra_global += ' --add_sm_background="125"'
  extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["et"] += ' --syst_zl_shift="CMS_htt_ZLScale_etau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mt"] += ' --syst_zl_shift="CMS_htt_ZLScale_mutau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mtmet"] += ' --set_alias="sel:mt_1<30."'
  scheme_et = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ''),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  (
    ' --syst_qcd_shape="CMS_htt_QCDShape_etau_nobtag_'+COM+'TeV:60:1.0:0.10"')),
    ("12",   "btag",        "btag",       BINS,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_btag_'+COM+'TeV:60:1.0:0.10"'
      ' --sub_ztt_top_frac=0.015'))
  ]
  scheme_mt = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ''),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_nobtag_'+COM+'TeV:60:1.0:0.10"')),
    ("12",   "btag",        "btag",       BINS,  (
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_btag_'+COM+'TeV:60:1.0:0.10"'
      ' --sub_ztt_top_frac=0.015'))
  ]
  scheme_mtmet = scheme_mt
  scheme_em = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("12",   "btag",        "btag",       BINS,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV" --sub_ztt_top_frac=0.015') 
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'mssm_default'
  ANA = 'mssm'

cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'em' : scheme_em,
  'mtmet' : scheme_mtmet
}

plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' ,   "60"    ,"2500" if ANA=='mssm' else "120"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      ,   "100"   ,"2500" if ANA=='mssm' else "160")
 ]
if options.mvis: del plots[1]
if options.svfit: del plots[0]

for pl in plots:
  var     = pl[0]
  xlab    = pl[1]
  dc_app  = pl[2]
  blind_min = pl[3]
  blind_max = pl[4]
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

      os.system('./bin/HiggsTauTauPlot4 --cfg=%(CFG)s --channel=%(ch)s'
        ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s[%(bin)s]" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_*.root' % vars())
    os.system('rm datacard_*.root')







