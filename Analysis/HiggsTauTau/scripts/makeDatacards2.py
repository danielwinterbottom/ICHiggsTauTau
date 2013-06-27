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
  "et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD" --syst_tau_scale="CMS_scale_t_etau_'+COM+'TeV"',
  "mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD" --syst_tau_scale="CMS_scale_t_mutau_'+COM+'TeV"',
  "mtmet" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD" --syst_tau_scale="CMS_scale_t_mutaumet_'+COM+'TeV"',
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
    ("5",   "vbf",          "vbf",        BINS, ""),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("5",   "vbf",          "vbf",        BINS,       ''),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'),
    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_mtmet = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("5",   "vbf",          "vbf",        BINS,       ''),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'),
    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_em = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("5",   "vbf",          "vbf",        BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV" --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"'),
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
#### New SM scheme
#################################################################
if options.scheme == 'new_sm':
  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mtmet"] += ' --set_alias="sel:mt_1<30."'
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  scheme_et = [
    ("8",   "inclusive",                "inclusive",              BINS_FINE,  ""),
    ("5",   "new_vbf",                  "vbf",                    BINS,       (
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_loose",            "vbf_loose",              BINS,       (
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_tight",            "vbf_tight",              BINS,       (
      ' --set_alias="vbf_loose_jets20:(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0 && pt_tt>100.)"'
      ' --set_alias="vbf_loose:(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && pt_tt>100.)"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"')),
    ("0",   "new_0jet_low",             "0jet_low",               BINS_FINE,  ""),
    ("1",   "new_0jet_medium",          "0jet_medium",            BINS_FINE,  ""),
    ("1",   "new_0jet_high",            "0jet_high",              BINS_FINE,  ""),
    ("2",   "new_1jet_medium",          "1jet_medium",            BINS_FINE,  ' --set_alias="w_shape_os:1"'),
    ("3",   "new_1jet_high",            "1jet_high",              BINS_FINE,  ' --set_alias="w_shape_os:1"'),
    ("3",   "new_1jet_high_lowhiggs",   "1jet_high_lowhiggs",     BINS_FINE,  ' --set_alias="w_shape_os:1"'),
    ("3",   "new_1jet_high_highhiggs",  "1jet_high_mediumhiggs",  BINS_FINE,  (
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Shape_Sample:Special_4_Data"'))
  ]
  scheme_mt = [
    ("8",   "inclusive",                "inclusive",              BINS_FINE,  ""),
    ("5",   "new_vbf",                  "vbf",                    BINS, (
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_loose",            "vbf_loose",              BINS, (
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"')),
    ("5",   "new_vbf_tight",            "vbf_tight",              BINS, (     
      ' --set_alias="vbf_loose_jets20:(n_lowpt_jets>=2 && n_jetsingap_lowpt==0 && mjj_lowpt>200. && jdeta_lowpt>2.0 && pt_tt>100.)"'
      ' --set_alias="vbf_loose:(n_jets>=2 && n_jetsingap==0 && mjj>200. && jdeta>2.0 && pt_tt>100.)"'
      ' --set_alias="w_vbf_os:1"'
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="QCD_Eff_Sample:Special_4_Data"')),
    ("0",   "new_0jet_low",             "0jet_low",               BINS_FINE,  ""),
    ("1",   "new_0jet_medium",          "0jet_medium",            BINS_FINE,  ""),
    ("1",   "new_0jet_high",            "0jet_high",              BINS_FINE,  ""),
    ("2",   "new_1jet_medium",          "1jet_medium",            BINS_FINE,  ' --set_alias="w_shape_os:1" --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet_medium_'+COM+'TeV"'),
    ("3",   "new_1jet_high",            "1jet_high",              BINS_FINE,  ' --set_alias="w_shape_os:1"'),
    ("3",   "new_1jet_high_lowhiggs",   "1jet_high_lowhiggs",     BINS_FINE,  ' --set_alias="w_shape_os:1"'),
    ("3",   "new_1jet_high",            "1jet_high",              BINS_FINE,  ' --set_alias="w_shape_os:1"'),
    ("3",   "new_1jet_high_highhiggs",  "1jet_high_mediumhiggs",  BINS_FINE,  (
      ' --set_alias="w_shape_os:1"'
      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'
      ' --set_alias="QCD_Shape_Sample:Special_4_Data"'))
  ]
  scheme_mtmet = scheme_mt
  scheme_em = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ' --set_alias="sel:em_gf_mva>-0.5" --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("5",   "vbf",          "vbf",        BINS,      ' --set_alias="sel:em_vbf_mva>-0.15" --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("5",   "new_vbf_loose","vbf_loose",  BINS,      ' --set_alias="sel:em_vbf_mva>-0.15" --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("5",   "new_vbf_tight","vbf_tight",  BINS,      ' --set_alias="sel:em_vbf_mva>-0.15" --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ' --set_alias="sel:em_gf_mva>-0.5" --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ' --set_alias="sel:em_gf_mva>-0.5" --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV" --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"'),
    ("2",   "1jet_low",     "1jet_low",   BINS_FINE, ' --set_alias="sel:em_gf_mva>-0.5" --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("3",   "1jet_high",    "1jet_high",  BINS_FINE, ' --set_alias="sel:em_gf_mva>-0.5" --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')
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
#### Old MSSM scheme
#################################################################
if options.scheme == 'old_mssm':
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
  # extra_global += ' --add_extra_binning="(150,0,1500):_fine_binning"'
  # extra_global += ' --add_sm_background="125"'
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
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500,1600,1800,2000"
  # BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
  extra_global += ' --add_extra_binning="(400,0,2000):_fine_binning"'
  extra_global += ' --add_sm_background="125"'
  extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mtmet"] += ' --set_alias="sel:mt_1<30."'
  scheme_et = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ''),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ''),
    ("12",   "btag",        "btag",       BINS_FINE,  ' --sub_ztt_top_frac=0.015')
  ]
  scheme_mt = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ''),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ''),
    ("12",   "btag",        "btag",       BINS_FINE,  ' --sub_ztt_top_frac=0.015')
  ]
  scheme_mtmet = scheme_mt
  scheme_em = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("11",   "nobtag",      "nobtag",     BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
    ("12",   "btag",        "btag",       BINS_FINE,  ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV" --sub_ztt_top_frac=0.015') 
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







