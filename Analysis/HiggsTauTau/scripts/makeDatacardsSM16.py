#!/usr/bin/env python

#./scripts/makeDatacardsSM16.py --cfg=scripts/new_plot_sm_2016_NewPlotting.cfg -c 'mt,tt' scripts/Params_2016_summer16.json -s 'smsummer16'

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

common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_z_mjj="CMS_htt_zmumuShape_VBF_13TeV" '
em_shape_systematics=' --syst_tau_scale="CMS_scale_e_em_13TeV" '
et_shape_systematics=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong__13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
mt_shape_systematics=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong__13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong__13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '

extra_channel = {
      "et" : ' '+common_shape_systematics,
      "mt" : ' '+common_shape_systematics,
      "tt" : ' '+common_shape_systematics,
      "em" : ' '+common_shape_systematics
  }


#extra_channel = {
#    "et" : ' --syst_eff_t="CMS_eff_t_mssmHigh_et_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" --syst_w_fake_rate="CMS_htt_wFakeShape_13TeV" --syst_efake_0pi_scale="CMS_scale_t_efake_1prong0pi0_13TeV" --syst_efake_1pi_scale="CMS_scale_t_efake_1prong1pi0_13TeV" --syst_tau_scale_0pi=CMS_scale_t_1prong0pi0_13TeV --syst_tau_scale_1pi=CMS_scale_t_1prong1pi0_13TeV --syst_tau_scale_3prong=CMS_scale_t_3prong0pi0_13TeV --syst_zpt_es=CMS_htt_dyShape_scale_m_13TeV --syst_zpt_tt=CMS_htt_dyShape_tjXsec_13TeV --syst_zpt_statpt0=CMS_htt_dyShape_stat_m400pt0_13TeV --syst_zpt_statpt40=CMS_htt_dyShape_stat_m400pt40_13TeV --syst_zpt_statpt80=CMS_htt_dyShape_stat_m400pt80_13TeV', 
#    "mt" : ' --syst_eff_t="CMS_eff_t_mssmHigh_mt_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" --syst_w_fake_rate="CMS_htt_wFakeShape_13TeV" --syst_tau_scale_0pi=CMS_scale_t_1prong0pi0_13TeV --syst_tau_scale_1pi=CMS_scale_t_1prong1pi0_13TeV --syst_tau_scale_3prong=CMS_scale_t_3prong0pi0_13TeV  --syst_zpt_es=CMS_htt_dyShape_scale_m_13TeV --syst_zpt_tt=CMS_htt_dyShape_tjXsec_13TeV --syst_zpt_statpt0=CMS_htt_dyShape_stat_m400pt0_13TeV --syst_zpt_statpt40=CMS_htt_dyShape_stat_m400pt40_13TeV --syst_zpt_statpt80=CMS_htt_dyShape_stat_m400pt80_13TeV ',
#    "tt" : ' --syst_eff_t="CMS_eff_t_mssmHigh_tt_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tau_scale_0pi=CMS_scale_t_1prong0pi0_13TeV --syst_tau_scale_1pi=CMS_scale_t_1prong1pi0_13TeV --syst_tau_scale_3prong=CMS_scale_t_3prong0pi0_13TeV --syst_zpt_es=CMS_htt_dyShape_scale_m_13TeV --syst_zpt_tt=CMS_htt_dyShape_tjXsec_13TeV --syst_zpt_statpt0=CMS_htt_dyShape_stat_m400pt0_13TeV --syst_zpt_statpt40=CMS_htt_dyShape_stat_m400pt40_13TeV --syst_zpt_statpt80=CMS_htt_dyShape_stat_m400pt80_13TeV ',
#    "em" : ' --syst_tau_scale="CMS_scale_e_em_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" --syst_zpt_es=CMS_htt_dyShape_scale_m_13TeV --syst_zpt_tt=CMS_htt_dyShape_tjXsec_13TeV --syst_zpt_statpt0=CMS_htt_dyShape_stat_m400pt0_13TeV --syst_zpt_statpt40=CMS_htt_dyShape_stat_m400pt40_13TeV --syst_zpt_statpt80=CMS_htt_dyShape_stat_m400pt80_13TeV --syst_jfake_e="CMS_htt_em_eFake_13TeV" --syst_jfake_m="CMS_htt_em_muFake_13TeV" ',
#}

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' '
  }

# update these to latest norm systematics
#if options.norm_systs:
#  extra_channel["et"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '
#  extra_channel["mt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '
#  extra_channel["tt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '
#  extra_channel["em"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '


if SCHEME == 'smsummer16':
  
  # replace m_vis with sv fit ass when it has been run
  VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'
  VAR_0JET_TT = 'm_vis[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'
  
  VAR_BOOSTED = 'pt_tt,m_vis[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_vis[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]'
  
  VAR_VBF = 'mjj,m_vis[300,700,1100,1500],[0,95,115,135,155,400]'
  VAR_VBF_TT = 'mjj,m_vis[0,300,500,800],[0,40,60,70,80,90,100,110,120,130,150,200,250]'

  scheme_et = [
    ("12",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("12",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("12",   "vbf",     "vbf",  VAR_VBF, '--set_alias="sel:mt_1<50" ')
  ]
  scheme_mt = [
    ("12",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("12",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("12",   "vbf",     "vbf",  VAR_VBF, '--set_alias="sel:mt_1<50" ')
  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "vbf",     "vbf",  VAR_VBF_TT, '')
  ]
  scheme_em = [
    ("15",   "0jet",    "0jet",  VAR_0JET_EM, '--set_alias="sel:mt_1<50" '),
    ("15",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("15",   "vbf",     "vbf",  VAR_VBF, '--set_alias="sel:mt_1<50" ')
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

    
    os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
              ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s --ratio --log_y --norm_bins --extra_pad=0.2'
              ' --var="%(var)s" %(extra)s --no_plot' % vars())
              
  os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
  os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())

