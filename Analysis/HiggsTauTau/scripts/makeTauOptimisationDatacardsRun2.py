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
parser.add_option("--mtsv", dest="mtsv", action='store_true', default=False,
                  help="Make inputs for total transverse svfit mass.")
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
parser.add_option("--shape_systs", dest="shape_systs", action='store_true', default=False,
                  help="Add systs shapes")

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
#extra_global = ' --fix_empty_hists="ggH.*,ggA*,qqH.*,VH.*,WH.*,ZH.*,ttH.*,bbH.*"'
extra_global = ' '

#### Apply these options for specific channels
extra_channel = {
    "et" : ' ',
    "mt" : ' ',
    "tt" : ' ',
    "em" : ' '
}
if options.shape_systs:
  extra_channel = {
      "et" : ' --syst_tau_scale="CMS_scale_t_et_13TeV" ',
      "mt" : ' --syst_tau_scale="CMS_scale_t_mt_13TeV" ',
      "tt" : ' --syst_tau_scale="CMS_scale_t_tt_13TeV" ',
      "em" : ' --syst_tau_scale="CMS_scale_e_em_13TeV" '
  }

#################################################################
#### Old SM scheme
#################################################################

if SCHEME == 'run2_sm':
#  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350]"
  scheme_et = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ''),
    ("8",   "notwoprong",    "inclusivemtnotwoprong",  BINS_FINE, '--set_alias="sel:mt_1<40"')
#    ("5",   "vbf",          "vbf",        BINS,      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("8",   "notwoprong",    "inclusivemtnotwoprong",  BINS_FINE, '--set_alias="sel:mt_1<40"')
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
    ("8",   "inclusive",    "inclusive",  BINS_FINE, '')
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

if SCHEME == 'run2_mssm':
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
#  MTTOTBINSFINE="[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,450,500,550,600,700,1000,1300,1600,1900,2200,2500,3000,3500,4000]"
#  MTTOTBINS="[0,40,80,120,160,200,240,280,320,360,400,450,500,550,600,700,1000,1500,2000,2500,3000,3500,4000]"
#  if options.mttot or options.mtsv:
#    BINS_FINE=MTTOTBINSFINE
#    BINS=MTTOTBINS

  scheme_et = [
#    ("8",   "inclusive",    "inclusive",  BINS_FINE if not options.mttot else MTTOTBINSFINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbloose",  "dbloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbmedium",  "dbmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbtight",  "dbtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwloose",  "puwloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwmedium",  "puwmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwtight",  "puwtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvloose",  "mvadbnewvloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewloose",  "mvadbnewloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewmedium",  "mvadbnewmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewtight",  "mvadbnewtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvtight",  "mvadbnewvtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvvtight",  "mvadbnewvvtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtag",    "nobtag",  BINS_FINE if not options.mttot else MTTOTBINSFINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btag",    "btag",  BINS if not options.mttot else MTTOTBINS, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtagnotwoprong",    "nobtagnotwoprong",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btagnotwoprong",    "btagnotwoprong",  BINS, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtaghigh",    "nobtaghigh",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btaghigh",    "btaghigh",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtaghighnotwoprong",    "nobtaghighnotwoprong",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btaghighnotwoprong",    "btaghighnotwoprong",  BINS, '--set_alias="sel:mt_1<30"')
  ]
  scheme_mt = [
#    ("8",   "inclusive",    "inclusive",  BINS_FINE if not options.mttot else MTTOTBINSFINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbloose",  "dbloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbmedium",  "dbmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbtight",  "dbtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwloose",  "puwloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwmedium",  "puwmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwtight",  "puwtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvloose",  "mvadbnewvloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewloose",  "mvadbnewloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewmedium",  "mvadbnewmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewtight",  "mvadbnewtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvtight",  "mvadbnewvtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvvtight",  "mvadbnewvvtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "notwoprong",  "notwoprong",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtag",    "nobtag",  BINS_FINE if not options.mttot else MTTOTBINSFINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btag",    "btag",  BINS if not options.mttot else MTTOTBINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "nobtagnotwoprong",    "nobtagnotwoprong",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    #("8",   "btagnotwoprong",    "btagnotwoprong",  BINS, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtaghigh",    "nobtaghigh",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btaghigh",    "btaghigh",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "nobtaghighnotwoprong",    "nobtaghighnotwoprong",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
#    ("8",   "btaghighnotwoprong",    "btaghighnotwoprong",  BINS, '--set_alias="sel:mt_1<30"')
  ]
  scheme_tt = [
#    ("8",   "inclusive",    "inclusive",  BINS_FINE if not options.mttot else MTTOTBINSFINE,  ''),
#    ("8",   "nobtag",    "nobtag",  BINS_FINE if not options.mttot else MTTOTBINSFINE, ''),
#    ("8",   "btag",    "btag",  BINS if not options.mttot else MTTOTBINS, ''),
    ("8",   "dbloose",  "dbloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbmedium",  "dbmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "dbtight",  "dbtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwloose",  "puwloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwmedium",  "puwmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "puwtight",  "puwtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvloose",  "mvadbnewvloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewloose",  "mvadbnewloose",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewmedium",  "mvadbnewmedium",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewtight",  "mvadbnewtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvtight",  "mvadbnewvtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "mvadbnewvvtight",  "mvadbnewvvtight",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    #("8",   "notwoprong",  "notwoprong",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    #("8",   "notwoprong",    "notwoprong",  BINS_FINE,  ''),
    #("8",   "nobtagnotwoprong",    "nobtagnotwoprong",  BINS_FINE, ''),
    #("8",   "btagnotwoprong",    "btagnotwoprong",  BINS, ''),

  ]
  scheme_em = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btag",    "btag",  BINS, '--set_alias="sel:pzeta>-20"')
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
  ('mt_sv' , "m_{T#tau#tau} [GeV]"        , '-mtsv'      , "100", "3000")
 ]
if options.mvis:
    plots = plots[0]
if options.svfit: 
    plots = plots[1]
if options.mttot: 
    plots = plots[2]
if options.mtsv: 
    plots = plots[3]

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
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
  os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())
  os.system('rm datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())

