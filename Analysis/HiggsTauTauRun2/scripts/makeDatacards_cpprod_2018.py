#!/usr/bin/env python

#./scripts/makeDatacards_cpprod_2018.py -c 'em,et,mt,tt' --embedding --output_folder cpprodcard2018 

import sys
from optparse import OptionParser
import os
import string
import shlex
from subprocess import Popen, PIPE

CHANNELS= ['et', 'mt', 'em','tt', 'zmm']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

def run_command(command):
    print command
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',default='scripts/params_2018.json',
                  help="Specify the parameter file containing the luminosity and cross section information - can be used to override config file.")
parser.add_option("--cfg", dest="config", type='string', default='scripts/plot_cpprod_2018.cfg',
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
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='cpprod_extrapt',
                  help="datacard scheme - can be used to override config file")
parser.add_option("-e", dest="energy", type='string', default='13',
                  help="The C.O.M. energy is written into the datacard name, default is 13")
parser.add_option("--no_shape_systs", dest="no_shape_systs", action='store_true', default=False,
                  help="Do not add shape systematics")
parser.add_option("--no_shift_systs", dest="no_shift_systs", action='store_true', default=False,
                  help="Do not add shape systematics if they require trees from deperate directory (but the rest are run as usual)")
parser.add_option("--total_jes", dest="total_jes", action='store_true', default=False,
                  help="Do total JES uncertainties.")
parser.add_option("--split_jes", dest="split_jes", action='store_true', default=False,
                  help="If set then the JES uncertainties split by source are added")
parser.add_option("--regional_jes", dest="regional_jes", action='store_true', default=False,
                  help="Split JES by sources grouped regionally")
parser.add_option("--norm_systs", dest="norm_systs", action='store_true', default=False,
                  help="Add shapes for evaluating normalisation uncerts")
parser.add_option("--year", dest="year", type='string', default='',
                  help="Output names are data-taking year dependent. This value is read from the config file if present")
parser.add_option("--embedding", dest="embedding", action='store_true', default=False,
                  help="Add shapes are embedded samples.")
parser.add_option("--batch", dest="batch", action='store_true', default=False,
                  help="Submit on batch.")
parser.add_option("--hadd", dest="hadd", action='store_true', default=False,
                  help="Hadd output files (post batch).")

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
YEAR=2018
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
no_shift_systs = options.no_shift_systs

########## Set up schemes and options

#### Always apply these options:

extra_global = ' '


#### Apply these options for specific channels

# update these to latest shape systematics
extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' ',
      "zmm" : ' ',
  }
jes_systematics=''
if options.split_jes:
  jes_systematics = ' --syst_scale_j_by_source="CMS_scale_j_SOURCE_13TeV" '


common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV"  --syst_quarkmass="CMS_FiniteQuarkMass_13TeV" --syst_ps="CMS_*PS_ggH_13TeV" --syst_ue="CMS_UE_ggH_13TeV" --syst_res_j="CMS_res_j_13TeV" '

if not no_shift_systs:
  common_shape_systematics+=' --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_scale_met="CMS_htt_boson_scale_met_13TeV" --syst_res_met="CMS_htt_boson_reso_met_13TeV" '
  common_shape_systematics += ' --syst_scale_j_regrouped="CMS_scale_j_*group_13TeV"'

if options.total_jes:
  common_shape_systematics += ' --syst_scale_j="CMS_scale_j_13TeV" '

em_shape_systematics=' --syst_qcd_bkg="CMS_em_QCD_BackgroundSubtraction_13TeV" --syst_em_qcd="CMS_em_QCD_*BIN_13TeV" --syst_eff_b_weights="CMS_eff_b_13TeV" '
# need to add new QCD uncertainties

if not no_shift_systs:
  em_shape_systematics+=' --syst_e_scale="CMS_scale_e_13TeV" --syst_mu_scale="CMS_scale_mu_13TeV" '

et_shape_systematics=' --syst_tau_id_diff="CMS_eff_t_*PT_13TeV" --syst_tau_trg_diff="CMS_eff_Xtrigger_et_*DM_13TeV" --syst_eff_b_weights="CMS_eff_b_13TeV" '
if not no_shift_systs:
  et_shape_systematics+=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_tau_scale_3prong1pi0="CMS_scale_t_3prong1pizero_13TeV" --syst_e_scale="CMS_scale_e_13TeV" '

mt_shape_systematics=' --syst_tau_id_diff="CMS_eff_t_*PT_13TeV" --syst_tau_trg_diff="CMS_eff_Xtrigger_mt_*DM_13TeV" --syst_eff_b_weights="CMS_eff_b_13TeV" '
if not no_shift_systs:
  mt_shape_systematics+=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_tau_scale_3prong1pi0="CMS_scale_t_3prong1pizero_13TeV" --syst_mu_scale="CMS_scale_mu_13TeV" '

tt_shape_systematics=' --syst_tau_id_diff="CMS_eff_t_*DM_13TeV" --syst_tau_trg_diff="CMS_eff_t_trg_*DM_13TeV" '
if not no_shift_systs:
  tt_shape_systematics+=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_tau_scale_3prong1pi0="CMS_scale_t_3prong1pizero_13TeV" '

if options.embedding:
  common_shape_systematics+=' --syst_embedding_tt="CMS_ttbar_embeded_13TeV" '

extra_channel = {
      "et" : ' '+common_shape_systematics+ ' '+et_shape_systematics,
      "mt" : ' '+common_shape_systematics+ ' '+mt_shape_systematics,
      "tt" : ' '+common_shape_systematics+ ' '+tt_shape_systematics,
      "em" : ' '+common_shape_systematics+ ' '+em_shape_systematics,
  }

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' ',
      "zmm" : ' '
  }

if SCHEME == 'cpprod':
  
  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 


  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]' 

  VAR_DIJET = 'm_sv,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_TT_TI_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_TT_LO_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_HI='m_sv,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'

  VAR_TT_LO_LO='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_MT_LO_LO='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_LO='m_sv,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_LO_LO='m_sv,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_LO_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'

  VAR_TT_TI_LO='m_sv,sjdphi[50,80,100,130,200],(12,-3.2,3.2)'
  VAR_MT_TI_LO='m_sv,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_LO='m_sv,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_TI_LO='m_sv,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'


  scheme_et = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "boosted",    "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_ET_LO_LO, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_ET_TI_LO, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:mt_1<50"   ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "boosted",    "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_MT_LO_LO, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_MT_TI_LO, ' --set_alias="sel:mt_1<50"   '),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50"   ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_TT, '   '),
    ("17",   "boosted", "2018_boosted",  VAR_BOOSTED_TT, '   '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_TT_LO_LO, '   '),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_TT_LO_HI, '   '),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_TT_TI_LO, '   '),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_TT_TI_HI, '   ')
  ]
  scheme_em = [
    ("19",   "0jet",    "2018_0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35"  '),
    ("19",   "boosted", "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35"  '),
    ("19",   "inclusive",    "2018_ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"  '),
    ("19",   "dijet_loosemjj_lowboost",     "2018_dijet_loosemjj_lowboost",  VAR_EM_LO_LO, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_EM_LO_HI, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_tightmjj_lowboost",     "2018_dijet_tightmjj_lowboost",  VAR_EM_TI_LO, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10"  ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'cpprod_extrapt':

  VAR_0JET_LT = 'svfit_mass[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'svfit_mass[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_TT = 'svfit_mass[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  VAR_BOOSTED = 'pt_tt,svfit_mass[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,svfit_mass[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]'

  VAR_DIJET =  'svfit_mass,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_TT_TI_HI='svfit_mass,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_TT_LO_HI='svfit_mass,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_HI='svfit_mass,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_HI='svfit_mass,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_TT_LO_LO='svfit_mass,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_MT_LO_LO='svfit_mass,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_LO='svfit_mass,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_LO_LO='svfit_mass,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_LO_HI='svfit_mass,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_TT_TI_LO='svfit_mass,sjdphi[50,80,100,130,200],(12,-3.2,3.2)'
  VAR_MT_TI_LO='svfit_mass,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_LO='svfit_mass,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_TI_LO='svfit_mass,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'  

  sel='(sqrt(2*met*pt_1*(1-cos(met_dphi_1)))>50)'

  scheme_et = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_LT, ' --set_alias="sel:%(sel)s"  --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "boosted",    "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_ET_LO_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_ET_TI_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars())
  ]
  scheme_mt = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_LT, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "boosted",    "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_MT_LO_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_MT_TI_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars())

  ]
  scheme_tt = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_TT, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "boosted", "2018_boosted",  VAR_BOOSTED_TT, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_TT_LO_LO, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_TT_TI_LO, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --set_alias="baseline:({baseline}&&pt_1>50)"  ')
  ]
  scheme_em = [
    ("19",   "0jet",    "2018_0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35"  '),
    ("19",   "boosted", "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35"  '),
    ("19",   "inclusive",    "2018_ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"  '),
    ("19",   "dijet_loosemjj_lowboost",     "2018_dijet_loosemjj_lowboost",  VAR_EM_LO_LO, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_EM_LO_HI, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_tightmjj_lowboost",     "2018_dijet_tightmjj_lowboost",  VAR_EM_TI_LO, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10"  ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'


if SCHEME == 'cpprod_extrapt_svfit':
  
  VAR_0JET_LT = 'svfit_mass_test[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'svfit_mass_test[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'svfit_mass_test[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 


  VAR_BOOSTED = 'pt_tt,svfit_mass_test[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,svfit_mass_test[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]' 

  VAR_DIJET = 'svfit_mass_test,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_TT_TI_HI='svfit_mass_test,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_TT_LO_HI='svfit_mass_test,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_HI='svfit_mass_test,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_HI='svfit_mass_test,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'

  VAR_TT_LO_LO='svfit_mass_test,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_MT_LO_LO='svfit_mass_test,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_LO='svfit_mass_test,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_LO_LO='svfit_mass_test,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_LO_HI='svfit_mass_test,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'

  VAR_TT_TI_LO='svfit_mass_test,sjdphi[50,80,100,130,200],(12,-3.2,3.2)'
  VAR_MT_TI_LO='svfit_mass_test,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_LO='svfit_mass_test,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'
  VAR_EM_TI_LO='svfit_mass_test,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'

  sel='(sqrt(2*met*pt_1*(1-cos(met_dphi_1)))>50)'


  scheme_et = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_LT, ' --set_alias="sel:%(sel)s"  --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "boosted",    "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_ET_LO_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_ET_TI_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars())
  ]
  scheme_mt = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_LT, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "boosted",    "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_MT_LO_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_MT_TI_LO, ' --set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars()),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:%(sel)s" --set_alias="baseline:({baseline}&&pt_2>30)"  ' % vars())

  ]
  scheme_tt = [
    ("17",   "0jet",    "2018_0jet",  VAR_0JET_TT, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "boosted", "2018_boosted",  VAR_BOOSTED_TT, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_loosemjj_lowboost",    "2018_dijet_loosemjj_lowboost", VAR_TT_LO_LO, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_tightmjj_lowboost",    "2018_dijet_tightmjj_lowboost", VAR_TT_TI_LO, ' --set_alias="baseline:({baseline}&&pt_1>50)"  '),
    ("17",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --set_alias="baseline:({baseline}&&pt_1>50)"  ')
  ]
  scheme_em = [
    ("19",   "0jet",    "2018_0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35"  '),
    ("19",   "boosted", "2018_boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35"  '),
    ("19",   "inclusive",    "2018_ttbar",  'svfit_mass_test[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"  '),
    ("19",   "dijet_loosemjj_lowboost",     "2018_dijet_loosemjj_lowboost",  VAR_EM_LO_LO, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_loosemjj_boosted",     "2018_dijet_loosemjj_boosted",  VAR_EM_LO_HI, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_tightmjj_lowboost",     "2018_dijet_tightmjj_lowboost",  VAR_EM_TI_LO, ' --set_alias="sel:pzeta>-10"  '),
    ("19",   "dijet_tightmjj_boosted",     "2018_dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10"  ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'control_plots':
  scheme_mt=[
    ("17",   "inclusive",    "pt_1",  'pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100]', ' --set_alias="sel:mt_1<50"   --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "pt_2",  'pt_2[30,35,40,45,50,55,60,70,80,90,100]', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "met",  'met[0,10,20,30,40,50,60,70,80,90,100]', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300]', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "m_vis",  'm_vis(20,0,200)', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_jets>1&&n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_jets>1&&n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("17",   "inclusive",    "m_sv",  'm_sv(20,0,200)', ' --set_alias="sel:mt_1<50"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),

  ]
  scheme_et=scheme_mt
  scheme_tt=[
    ("17",   "inclusive",    "pt_1",  'pt_1[50,55,60,70,80,90,100,120,140]', '   '),
    ("17",   "inclusive",    "pt_2",  'pt_2[40,45,50,55,60,70,80,90,100]', '  '),
    ("17",   "inclusive",    "met",  'met[0,10,20,30,40,50,60,70,80,90,100]', '  '),
    ("17",   "inclusive",    "pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300]', '  '),
    ("17",   "inclusive",    "m_vis",  'm_vis(25,0,250)', '  '),
    ("17",   "inclusive",    "mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', '  --set_alias="inclusive:(n_jets>1)" '),
    ("17",   "inclusive",    "sjdphi",  'sjdphi(12,-3.2,3.2)', '  --set_alias="inclusive:(n_jets>1)" '),
    ("17",   "inclusive",    "n_jets",  'n_jets(4,0,4)', '  '),
    ("17",   "inclusive",    "m_sv",  'm_sv(30,0,300)', '  '),

  ]
  scheme_em=[
    ("19",   "inclusive",    "pt_1",  'pt_1[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "pt_2",  'pt_2[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "met",  'met[0,10,20,30,40,50,60,70,80,90,100]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "m_vis",  'm_vis(20,0,200)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0&&n_loose_bjets<2)" '), 
    ("19",   "inclusive",    "n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),
    ("19",   "inclusive",    "m_sv",  'm_sv(20,0,200)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2)" '),

  ]

  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'sync':
  VAR_MVIS = 'm_vis(20,0,200)'
  VAR_DIJET = 'm_vis,sjdphi(20,0,200),(8,-3.2,3.2)'
  ADD_STRING_MT = ' ' #' --set_alias "sel:(mt_1<50)" '
  scheme_et = [
  ]
  scheme_mt = [
    ("17",   "inclusive",  "inclusive",        VAR_MVIS,   ' --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2&&pt_2>30)" '),
    ("17",   "inclusive",  "0jet",        VAR_MVIS,   ' --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2&&pt_2>30&&n_jets==0)" '),
    ("17",   "inclusive",  "boosted",        VAR_MVIS,   ' --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2&&pt_2>30&&(n_jets==1||(n_jets>1&&mjj<300)))" '),
    ("17",   "inclusive",  "vbf",        VAR_DIJET,   ' --set_alias="inclusive:(n_bjets==0&&n_loose_bjets<2&&pt_2>30&&n_jets>1&&mjj>300)" '),

  ]
  scheme_tt = [
    ("17",   "inclusive",  "inclusive",        VAR_MVIS,   ' --set_alias="inclusive:(pt_1>40)" '),
    ("17",   "inclusive",  "0jet",        VAR_MVIS,   ' --set_alias="inclusive:(pt_1>40&&n_jets==0)" '),
    ("17",   "inclusive",  "boosted",        VAR_MVIS,   ' --set_alias="inclusive:(pt_1>40&&(n_jets==1||(n_jets>1&&jdeta<2.5)))" '),
    ("17",   "inclusive",  "vbf",        VAR_DIJET,   ' --set_alias="inclusive:(pt_1>40&&n_jets>1&&jdeta>2.5)" '),
  ]
  scheme_em = [
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'


cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'em' : scheme_em,
  'tt' : scheme_tt
}

qsub_command = 'qsub -e ./err -o ./out -cwd -V -q hep.q -v CFG="{}",ch="{}",cat_num="{}",cat_str="{}",YEAR="{}",output_folder="{}",dc="{}",PARAMS="{}",FOLDER="{}",BLIND="{}"'

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
        if options.embedding: extra+=' --embedding '
        if ch in ['em','et','mt'] and SCHEME != 'sync': extra+=' --add_wt=\"wt_btag\" '
        if ch in ['et','mt','tt'] and cat_num in ['17','18'] and SCHEME != 'sync': extra+=' --do_ff_systs '

        extra_jes = options.extra + ' ' + extra_global + ' ' + jes_systematics + ' ' + opts + ' --no_default '

        if not options.hadd:
            if not options.batch:
                print 'python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s --var="%(var)s" %(extra)s --no_plot' % vars()
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra)s --no_plot' % vars())

            else:
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra="{}"'.format(extra)
                        + ' ./scripts/batch_datacards.sh'
                        )

            if jes_systematics and not options.no_shape_systs and not options.batch:
              # have to do this to avoid using too much memory...
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes1'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=1:9' % vars())
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes2'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=10:18' % vars())
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes3'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=19:27' % vars())

            elif jes_systematics and not options.no_shape_systs and options.batch:
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra_jes="{}"'.format(extra_jes)
                        + ' -v extra_name=jes1,jes_sources=1:9 ./scripts/batch_datacards_jes.sh'
                        )
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra_jes="{}"'.format(extra_jes)
                        + ' -v extra_name=jes2,jes_sources=10:18 ./scripts/batch_datacards_jes.sh'
                        )
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra_jes="{}"'.format(extra_jes)
                        + ' -v extra_name=jes3,jes_sources=19:27 ./scripts/batch_datacards_jes.sh'
                        )

    if not options.batch:
        os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
        os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())

    if options.hadd:
        os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
        os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root ' % vars())

