#!/usr/bin/env python

#./scripts/makeDatacardsTauID.py --cfg=scripts/new_plot_sm_2016_NewPlotting_tauid.cfg -c 'mt' scripts/Params_2016_smsummer16.json -s "tauid2016" --embedding

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
                  help="Do total JES uncertainties.")
parser.add_option("--no_split_jes", dest="no_split_jes", action='store_true', default=False,
                  help="If set then the JES uncertainties split by source are not added")
parser.add_option("--regional_jes", dest="regional_jes", action='store_true', default=False,
                  help="Split JES by sources grouped regionally")
parser.add_option("--norm_systs", dest="norm_systs", action='store_true', default=False,
                  help="Add shapes for evaluating normalisation uncerts")
parser.add_option("--year", dest="year", type='string', default='',
                  help="Output names are data-taking year dependent. This value is read from the config file if present")
parser.add_option("--embedding", dest="embedding", action='store_true', default=False,
                  help="Add shapes are embedded samples.")

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

  
common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_scale_j_full="CMS_scale_j_13TeV" '


mt_shape_systematics=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '

et_shape_systematics=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '

tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '

if options.embedding:
  #mt_shape_systematics+=' --syst_mu_scale="CMS_scale_m_13TeV" ' 
  et_shape_systematics+=' --syst_e_scale="CMS_scale_e_13TeV" ' 
  common_shape_systematics+=' --syst_embedding_tt="CMS_ttbar_embeded_13TeV" '

extra_channel = {
      "et" : ' '+common_shape_systematics+ ' '+et_shape_systematics,
      "mt" : ' '+common_shape_systematics+ ' '+mt_shape_systematics,
      "tt" : ' '+common_shape_systematics+ ' '+tt_shape_systematics,
      "em" : ' '+common_shape_systematics
  }

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' '
  }


if SCHEME == 'tauid2016':
  

  scheme_mt = [
    ("12",   "pass",    "pass",  'm_vis[40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200]', '--set_alias="sel:mt_1<40" '),
    ("12",   "fail",    "fail",  'm_vis[40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200]', '--set_alias="sel:mt_1<40" '),
    
  ]
  scheme_et = [
    ("12",   "pass",    "pass",  'm_vis[40,50,60,70,80,90,110,130,150]', '--set_alias="sel:mt_1<40" '),
    ("12",   "fail",    "fail",  'm_vis[40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200]', '--set_alias="sel:mt_1<40" '),
  ]
  scheme_tt = [
    ("8",   "inclusive",    "inclusive",  'm_vis[40,50,60,70,80,90,110,130,150]', ' '),

  ] 

  bkg_schemes = {
    'et' : 'et_default',  
    'mt' : 'mt_with_zmm',
    'tt' : 'tt_default'
  }
  ANA = 'sm'


cat_schemes = {
  'et' : scheme_et,  
  'mt' : scheme_mt,
  'tt' : scheme_tt, 
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
    if options.embedding: extra+=' --embedding'

    
    os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
              ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s '
              ' --var="%(var)s" %(extra)s --no_plot' % vars())
    
  os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
  os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())

