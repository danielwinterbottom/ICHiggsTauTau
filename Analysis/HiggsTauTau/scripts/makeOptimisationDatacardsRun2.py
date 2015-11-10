#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

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
                  help="Only make inputs for visible mass, no svfit.")
parser.add_option("--svfit", dest="svfit", action='store_true', default=False,
                  help="Only make inputs for svfit mass.")
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
  configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])

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
extra_global = ' --fix_empty_hists="ggH.*,ggA*,qqH.*,VH.*,WH.*,ZH.*,ttH.*,bbH.*"'

#### Apply these options for specific channels
extra_channel = {
  "et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W" ',
  "mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W" ',
  "tt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W" ',
  "em" : ' --fix_empty_bins="QCD"'
}

#################################################################
#### Old SM scheme
#################################################################
if SCHEME == 'run2_sm':
#  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300]"
  scheme_et = [
#    ("8",   "db03iso0p07",    "db03iso0p07",  BINS_FINE, ''),
#    ("8",   "db03iso0p08",    "db03iso0p08",  BINS_FINE, ''),
#    ("8",   "db03iso0p09",    "db03iso0p09",  BINS_FINE, ''),
#    ("8",   "db03iso0p1",    "db03iso0p1",  BINS_FINE, ''),
#    ("8",   "db03iso0p11",    "db03iso0p11",  BINS_FINE, ''),
#    ("8",   "db03iso0p12",    "db03iso0p12",  BINS_FINE, ''),
#    ("8",   "db03iso0p13",    "db03iso0p13",  BINS_FINE, ''),
#    ("8",   "ea03iso0p07",    "ea03iso0p07",  BINS_FINE, ''),
#    ("8",   "ea03iso0p08",    "ea03iso0p08",  BINS_FINE, ''),
#    ("8",   "ea03iso0p09",    "ea03iso0p09",  BINS_FINE, ''),
#    ("8",   "ea03iso0p1",    "ea03iso0p1",  BINS_FINE, ''),
#    ("8",   "ea03iso0p11",    "ea03iso0p11",  BINS_FINE, ''),
#    ("8",   "ea03iso0p12",    "ea03iso0p12",  BINS_FINE, ''),
#    ("8",   "ea03iso0p13",    "ea03iso0p13",  BINS_FINE, ''),
    ("8",   "ea03iso0p14",    "ea03iso0p14",  BINS_FINE, ''),
    ("8",   "ea03iso0p15",    "ea03iso0p15",  BINS_FINE, ''),
    ("8",   "ea03iso0p16",    "ea03iso0p16",  BINS_FINE, ''),
    ("8",   "ea03iso0p17",    "ea03iso0p17",  BINS_FINE, ''),
    ("8",   "ea03iso0p18",    "ea03iso0p18",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p07",    "db03allchiso0p07",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p08",    "db03allchiso0p08",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p09",    "db03allchiso0p09",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p1",    "db03allchiso0p1",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p11",    "db03allchiso0p11",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p12",    "db03allchiso0p12",  BINS_FINE, ''),
#    ("8",   "db03allchiso0p13",    "db03allchiso0p13",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p07",    "db04allchiso0p07",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p08",    "db04allchiso0p08",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p09",    "db04allchiso0p09",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p1",    "db04allchiso0p1",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p11",    "db04allchiso0p11",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p12",    "db04allchiso0p12",  BINS_FINE, ''),
#    ("8",   "db04allchiso0p13",    "db04allchiso0p13",  BINS_FINE, ''),
    ("8",   "db04allchiso0p14",    "db04allchiso0p14",  BINS_FINE, ''),
    ("8",   "db04allchiso0p15",    "db04allchiso0p15",  BINS_FINE, ''),
    ("8",   "db04allchiso0p16",    "db04allchiso0p16",  BINS_FINE, ''),
    ("8",   "db04allchiso0p17",    "db04allchiso0p17",  BINS_FINE, ''),
    ("8",   "db04allchiso0p18",    "db04allchiso0p18",  BINS_FINE, ''),

#    ("5",   "vbf",          "vbf",        BINS,      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
#    ("8",   "inclusive",    "inclusive",  BINS_FINE,  '')
    ("8",   "db03iso0p07",    "db03iso0p07",  BINS_FINE, ''),
    ("8",   "db03iso0p08",    "db03iso0p08",  BINS_FINE, ''),
    ("8",   "db03iso0p09",    "db03iso0p09",  BINS_FINE, ''),
    ("8",   "db03iso0p1",    "db03iso0p1",  BINS_FINE, ''),
    ("8",   "db03iso0p11",    "db03iso0p11",  BINS_FINE, ''),
    ("8",   "db03iso0p12",    "db03iso0p12",  BINS_FINE, ''),
    ("8",   "db03iso0p13",    "db03iso0p13",  BINS_FINE, ''),
    ("8",   "ea03iso0p07",    "ea03iso0p07",  BINS_FINE, ''),
    ("8",   "ea03iso0p08",    "ea03iso0p08",  BINS_FINE, ''),
    ("8",   "ea03iso0p09",    "ea03iso0p09",  BINS_FINE, ''),
    ("8",   "ea03iso0p1",    "ea03iso0p1",  BINS_FINE, ''),
    ("8",   "ea03iso0p11",    "ea03iso0p11",  BINS_FINE, ''),
    ("8",   "ea03iso0p12",    "ea03iso0p12",  BINS_FINE, ''),
    ("8",   "ea03iso0p13",    "ea03iso0p13",  BINS_FINE, ''),
    ("8",   "db03allchiso0p07",    "db03allchiso0p07",  BINS_FINE, ''),
    ("8",   "db03allchiso0p08",    "db03allchiso0p08",  BINS_FINE, ''),
    ("8",   "db03allchiso0p09",    "db03allchiso0p09",  BINS_FINE, ''),
    ("8",   "db03allchiso0p1",    "db03allchiso0p1",  BINS_FINE, ''),
    ("8",   "db03allchiso0p11",    "db03allchiso0p11",  BINS_FINE, ''),
    ("8",   "db03allchiso0p12",    "db03allchiso0p12",  BINS_FINE, ''),
    ("8",   "db03allchiso0p13",    "db03allchiso0p13",  BINS_FINE, ''),
    ("8",   "db04allchiso0p07",    "db04allchiso0p07",  BINS_FINE, ''),
    ("8",   "db04allchiso0p08",    "db04allchiso0p08",  BINS_FINE, ''),
    ("8",   "db04allchiso0p09",    "db04allchiso0p09",  BINS_FINE, ''),
    ("8",   "db04allchiso0p1",    "db04allchiso0p1",  BINS_FINE, ''),
    ("8",   "db04allchiso0p11",    "db04allchiso0p11",  BINS_FINE, ''),
    ("8",   "db04allchiso0p12",    "db04allchiso0p12",  BINS_FINE, ''),
    ("8",   "db04allchiso0p13",    "db04allchiso0p13",  BINS_FINE, ''),

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
#    ("8",   "inclusive",    "inclusive",  BINS_FINE, '')
    #("8",   "edb03iso0p12",    "edb03iso0p12",  BINS_FINE, ''),
    #("8",   "edb03iso0p13",    "edb03iso0p13",  BINS_FINE, ''),
    #("8",   "edb03iso0p14",    "edb03iso0p14",  BINS_FINE, ''),
    #("8",   "edb03iso0p15",    "edb03iso0p15",  BINS_FINE, ''),
    #("8",   "edb03iso0p16",    "edb03iso0p16",  BINS_FINE, ''),
   # ("8",   "edb03iso0p17",    "edb03iso0p17",  BINS_FINE, ''),
   # ("8",   "edb03iso0p18",    "edb03iso0p18",  BINS_FINE, ''),
    ("8",   "edb03iso0p19",    "edb03iso0p19",  BINS_FINE, ''),
    ("8",   "edb03iso0p20",    "edb03iso0p20",  BINS_FINE, ''),
    ("8",   "edb03iso0p21",    "edb03iso0p21",  BINS_FINE, ''),
#    ("8",   "mdb03iso0p12",    "mdb03iso0p12",  BINS_FINE, ''),
#    ("8",   "mdb03iso0p13",    "mdb03iso0p13",  BINS_FINE, ''),
#    ("8",   "mdb03iso0p14",    "mdb03iso0p14",  BINS_FINE, ''),
#    ("8",   "mdb03iso0p15",    "mdb03iso0p15",  BINS_FINE, ''),
#    ("8",   "mdb03iso0p16",    "mdb03iso0p16",  BINS_FINE, ''),
#    ("8",   "mdb03iso0p17",    "mdb03iso0p17",  BINS_FINE, ''),
    #("8",   "mdb03iso0p18",    "mdb03iso0p18",  BINS_FINE, ''),
    ("8",   "mdb03iso0p19",    "mdb03iso0p19",  BINS_FINE, ''),
    ("8",   "mdb03iso0p20",    "mdb03iso0p20",  BINS_FINE, ''),
    ("8",   "mdb03iso0p21",    "mdb03iso0p21",  BINS_FINE, ''),
#    ("8",   "eea03iso0p12",    "eea03iso0p12",  BINS_FINE, ''),
#    ("8",   "eea03iso0p13",    "eea03iso0p13",  BINS_FINE, ''),
#    ("8",   "eea03iso0p14",    "eea03iso0p14",  BINS_FINE, ''),
#    ("8",   "eea03iso0p15",    "eea03iso0p15",  BINS_FINE, ''),
#    ("8",   "eea03iso0p16",    "eea03iso0p16",  BINS_FINE, ''),
#    ("8",   "eea03iso0p17",    "eea03iso0p17",  BINS_FINE, ''),
    #("8",   "eea03iso0p18",    "eea03iso0p18",  BINS_FINE, ''),
    ("8",   "eea03iso0p19",    "eea03iso0p19",  BINS_FINE, ''),
    ("8",   "eea03iso0p20",    "eea03iso0p20",  BINS_FINE, ''),
    ("8",   "eea03iso0p21",    "eea03iso0p21",  BINS_FINE, ''),
#    ("8",   "mea03iso0p12",    "mea03iso0p12",  BINS_FINE, ''),
#    ("8",   "mea03iso0p13",    "mea03iso0p13",  BINS_FINE, ''),
#    ("8",   "mea03iso0p14",    "mea03iso0p14",  BINS_FINE, ''),
#    ("8",   "mea03iso0p15",    "mea03iso0p15",  BINS_FINE, ''),
#    ("8",   "mea03iso0p16",    "mea03iso0p16",  BINS_FINE, ''),
#    ("8",   "mea03iso0p17",    "mea03iso0p17",  BINS_FINE, ''),
    #("8",   "mea03iso0p18",    "mea03iso0p18",  BINS_FINE, ''),
    ("8",   "mea03iso0p19",    "mea03iso0p19",  BINS_FINE, ''),
    ("8",   "mea03iso0p20",    "mea03iso0p20",  BINS_FINE, ''),
    ("8",   "mea03iso0p21",    "mea03iso0p21",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p12",    "edb03allchiso0p12",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p13",    "edb03allchiso0p13",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p14",    "edb03allchiso0p14",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p15",    "edb03allchiso0p15",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p16",    "edb03allchiso0p16",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p17",    "edb03allchiso0p17",  BINS_FINE, ''),
#    ("8",   "edb03allchiso0p18",    "edb03allchiso0p18",  BINS_FINE, ''),
    ("8",   "edb03allchiso0p19",    "edb03allchiso0p19",  BINS_FINE, ''),
    ("8",   "edb03allchiso0p20",    "edb03allchiso0p20",  BINS_FINE, ''),
    ("8",   "edb03allchiso0p21",    "edb03allchiso0p21",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p12",    "mdb03allchiso0p12",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p13",    "mdb03allchiso0p13",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p14",    "mdb03allchiso0p14",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p15",    "mdb03allchiso0p15",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p16",    "mdb03allchiso0p16",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p17",    "mdb03allchiso0p17",  BINS_FINE, ''),
#    ("8",   "mdb03allchiso0p18",    "mdb03allchiso0p18",  BINS_FINE, ''),
    ("8",   "mdb03allchiso0p19",    "mdb03allchiso0p19",  BINS_FINE, ''),
    ("8",   "mdb03allchiso0p20",    "mdb03allchiso0p20",  BINS_FINE, ''),
    ("8",   "mdb03allchiso0p21",    "mdb03allchiso0p21",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p12",    "edb04allchiso0p12",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p13",    "edb04allchiso0p13",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p14",    "edb04allchiso0p14",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p15",    "edb04allchiso0p15",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p16",    "edb04allchiso0p16",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p17",    "edb04allchiso0p17",  BINS_FINE, ''),
#    ("8",   "edb04allchiso0p18",    "edb04allchiso0p18",  BINS_FINE, ''),
    ("8",   "edb04allchiso0p19",    "edb04allchiso0p19",  BINS_FINE, ''),
    ("8",   "edb04allchiso0p20",    "edb04allchiso0p20",  BINS_FINE, ''),
    ("8",   "edb04allchiso0p21",    "edb04allchiso0p21",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p12",    "mdb04allchiso0p12",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p13",    "mdb04allchiso0p13",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p14",    "mdb04allchiso0p14",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p15",    "mdb04allchiso0p15",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p16",    "mdb04allchiso0p16",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p17",    "mdb04allchiso0p17",  BINS_FINE, ''),
#    ("8",   "mdb04allchiso0p18",    "mdb04allchiso0p18",  BINS_FINE, ''),
    ("8",   "mdb04allchiso0p19",    "mdb04allchiso0p19",  BINS_FINE, ''),
    ("8",   "mdb04allchiso0p20",    "mdb04allchiso0p20",  BINS_FINE, ''),
    ("8",   "mdb04allchiso0p21",    "mdb04allchiso0p21",  BINS_FINE, ''),

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

cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'em' : scheme_em,
  'tt' : scheme_tt
}



plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' , "60", "2500" if ANA=='mssm' else "125"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      , "100", "2500"  if ANA=='mssm' else "125")
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

      os.system('$CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/bin/HiggsTauTauPlot5 --cfg=%(CFG)s --channel=%(ch)s'
        ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s%(bin)s" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())
    os.system('rm datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())

