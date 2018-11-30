#!/usr/bin/env python

#./scripts/makeDatacardsSMCP17.py --cfg=scripts/plot_sm_2017.cfg -c 'em,et,mt,tt' scripts/Params_2017_smsummer17_v5.json -s 'cpsummer16_ff' --regional_jes --embedding

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
      "em" : ' ',
      "zmm" : ' ',
  }
jes_systematics=''
if options.split_jes:
  jes_systematics = ' --syst_scale_j_by_source="CMS_scale_j_SOURCE_13TeV" '


common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_scale_met="CMS_htt_boson_scale_met_13TeV" --syst_res_met="CMS_htt_boson_reso_met_13TeV" --syst_quarkmass="CMS_FiniteQuarkMass_13TeV" --syst_ps="CMS_PS_ggH_13TeV" --syst_ue="CMS_UE_ggH_13TeV" '

if options.regional_jes or options.total_jes:
  common_shape_systematics += ' --syst_scale_j_full="CMS_scale_j_eta0to5_13TeV" --syst_scale_j_cent="CMS_scale_j_eta0to3_13TeV" --syst_scale_j_hf="CMS_scale_j_eta3to5_13TeV" --syst_scale_j_rbal="CMS_scale_j_RelativeBal_13TeV"  --syst_scale_j_rsamp="CMS_scale_j_RelativeSample_13TeV" '
  common_shape_systematics += ' --syst_scale_j_full_corr="CMS_scale_j_eta0to5_corr_13TeV" --syst_scale_j_cent_corr="CMS_scale_j_eta0to3_corr_13TeV" --syst_scale_j_hf_corr="CMS_scale_j_eta3to5_corr_13TeV" '
  common_shape_systematics += ' --syst_scale_j_full_uncorr="CMS_scale_j_eta0to5_uncorr_13TeV" --syst_scale_j_cent_uncorr="CMS_scale_j_eta0to3_uncorr_13TeV" --syst_scale_j_hf_uncorr="CMS_scale_j_eta3to5_uncorr_13TeV" '
if options.total_jes:
  common_shape_systematics += ' --syst_scale_j="CMS_scale_j_13TeV" '
  common_shape_systematics += ' --syst_scale_j_corr="CMS_scale_j_corr_13TeV" '
  common_shape_systematics += ' --syst_scale_j_uncorr="CMS_scale_j_uncorr_13TeV" '


em_shape_systematics=' --syst_tau_scale="CMS_scale_e_13TeV" --syst_em_qcd_rate_0jet="CMS_em_QCD_0JetRate_13TeV" --syst_em_qcd_rate_1jet="CMS_em_QCD_1JetRate_13TeV" --syst_em_qcd_shape_0jet="CMS_em_QCD_0JetShape_13TeV" --syst_em_qcd_shape_1jet="CMS_em_QCD_1JetShape_13TeV" --syst_em_qcd_extrap="CMS_em_QCD_IsoExtrap_13TeV" --syst_qcd_bkg="CMS_em_QCD_BackgroundSubtraction_13TeV" '
et_shape_systematics=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" --syst_e_scale="CMS_scale_e_13TeV" '
mt_shape_systematics=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
zmm_shape_systematics=' --folder=/vols/cms/dw515/Offline/output/SM/Apr09/ '


if options.embedding:
  et_shape_systematics+=' --syst_e_scale="CMS_scale_e_13TeV" '
  common_shape_systematics+=' --syst_embedding_tt="CMS_ttbar_embeded_13TeV" '

extra_channel = {
      "et" : ' '+common_shape_systematics+ ' '+et_shape_systematics,
      "mt" : ' '+common_shape_systematics+ ' '+mt_shape_systematics,
      "tt" : ' '+common_shape_systematics+ ' '+tt_shape_systematics,
      "em" : ' '+common_shape_systematics+ ' '+em_shape_systematics,
      "zmm" : ' '+common_shape_systematics+ ' '+zmm_shape_systematics
  }

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' ',
      "zmm" : ' '
  }

if SCHEME == 'cpsummer16_ff':
  
  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  #VAR_0JET_LT='m_vis(20,50,250)'
  #VAR_0JET_EM='m_vis(20,50,250)'
  #VAR_0JET_TT='m_vis(20,50,250)' 

  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]' 

  VAR_DIJET = 'm_sv,sjdphi[50,80,100,115,130,150],(12,-3.2,3.2)'
  VAR_TT_TI_HI='m_sv,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'
  VAR_TT_LO_HI='m_sv,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'
  VAR_ET_LO_HI='m_sv,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'
  VAR_ET_TI_HI='m_sv,sjdphi[50,80,100,150],(12,-3.2,3.2)'

  scheme_et = [
    ("17",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:mt_1<50" --do_ff_systs ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --do_ff_systs ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_TT, ' --do_ff_systs '),
    ("17",   "boosted", "boosted",  VAR_BOOSTED_TT, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --do_ff_systs ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "inclusive",    "ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"'),
    ("19",   "dijet_loosemjj_lowboost",     "dijet_loosemjj_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_lowboost",     "dijet_tightmjj_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'cpsummer16_classic':

  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]'

  VAR_DIJET = 'm_sv,sjdphi[50,80,100,115,130,150],(12,-3.2,3.2)'
  VAR_TT_TI_HI='m_sv,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'
  VAR_TT_LO_HI='m_sv,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'
  VAR_ET_LO_HI='m_sv,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'
  VAR_ET_TI_HI='m_sv,sjdphi[50,80,100,150],(12,-3.2,3.2)'

  scheme_et = [
    ("12",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" '),
    ("12",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" '),
    ("12",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:mt_1<50" ')
  ]
  scheme_mt = [
    ("12",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" '),
    ("12",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("12",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ' '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ' '),
    ("8",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' '),
    ("8",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' '),
    ("8",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' '),
    ("8",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "inclusive",    "ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"'),
    ("19",   "dijet_loosemjj_lowboost",     "dijet_loosemjj_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_lowboost",     "dijet_tightmjj_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'cpsummer16_ff_mvis':

  VAR_0JET_LT = 'm_vis[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_vis[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_vis[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  VAR_BOOSTED = 'pt_tt,m_vis[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_vis[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]'

  VAR_DIJET = 'm_vis,sjdphi[50,80,100,115,130,150],(12,-3.2,3.2)'

  VAR_TT_LO_HI='m_vis,sjdphi[50,80,100,110,145],(12,-3.2,3.2)'
  VAR_TT_TI_HI='m_vis,sjdphi[50,80,100,115,130,160],(12,-3.2,3.2)'

  VAR_ET_LO_HI='m_vis,sjdphi[50,80,100,115,150],(12,-3.2,3.2)'

  VAR_EM_TI_LO='m_vis,sjdphi[50,100,115,130,160],(12,-3.2,3.2)'
 
  VAR_DIJET_TT_1 = VAR_DIJET
  VAR_DIJET_TT_2 = VAR_DIJET
  VAR_DIJET_MT_1 = VAR_DIJET
  VAR_DIJET_MT_2 = VAR_DIJET
  VAR_DIJET_ET_1 = VAR_DIJET
  VAR_DIJET_ET_2 = VAR_DIJET
  VAR_DIJET_EM_1 = VAR_DIJET
  VAR_DIJET_EM_2 = VAR_DIJET

  scheme_et = [
    ("17",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_ET_2, '--set_alias="sel:mt_1<50" --do_ff_systs ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_TT, ' --do_ff_systs '),
    ("17",   "boosted", "boosted",  VAR_BOOSTED_TT, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --do_ff_systs ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "inclusive",    "ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"'),
    ("19",   "dijet_loosemjj_lowboost",     "dijet_loosemjj_lowboost",  VAR_DIJET_EM_1, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET_EM_2, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_lowboost",     "dijet_tightmjj_lowboost",  VAR_EM_TI_LO, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_EM_2, ' --set_alias="sel:pzeta>-10" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'cpsummer16_ff_fracs':

  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]'

  VAR_DIJET =  'm_sv,fabs(sjdphi)[50,80,100,115,130,150],(6,0,3.2)'
  VAR_TT_TI_HI='m_sv,fabs(sjdphi)[50,80,100,115,150],(6,0,3.2)'
  VAR_TT_LO_HI='m_sv,fabs(sjdphi)[50,80,100,115,150],(6,0,3.2)'
  VAR_ET_LO_HI='m_sv,fabs(sjdphi)[50,80,100,115,150],(6,0,3.2)'
  VAR_ET_TI_HI='m_sv,fabs(sjdphi)[50,80,100,150],(6,0,3.2)'  

  scheme_et = [
    ("0",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" ')
  ]
  scheme_mt = [
    ("0",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" ')

  ]
  scheme_tt = [
    ("0",   "0jet",    "0jet_1",  VAR_0JET_TT, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "boosted", "boosted_1",  VAR_BOOSTED_TT, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost_1", VAR_DIJET, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted_1",  VAR_TT_LO_HI, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost_1", VAR_DIJET, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted_1",  VAR_TT_TI_HI, ' --set_alias="baseline:({baseline_aisotau1})" '),

    ("0",   "0jet",    "0jet_2",  VAR_0JET_TT, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "boosted", "boosted_2",  VAR_BOOSTED_TT, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost_2", VAR_DIJET, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted_2",  VAR_TT_LO_HI, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost_2", VAR_DIJET, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted_2",  VAR_TT_TI_HI, ' --set_alias="baseline:({baseline_aisotau2})" ')

  ]
  scheme_em = []
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'tt' : 'tt_default',
    'em' : 'em_default'
  }
  ANA = 'sm'


if SCHEME == 'control_plots':
  scheme_mt=[
    ("17",   "inclusive",    "pt_1",  'pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs  --set_alias="inclusive:(n_bjets==0)" '),
    ("17",   "inclusive",    "pt_2",  'pt_2[30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0)" '),
    ("17",   "inclusive",    "met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0)" '),
    ("17",   "inclusive",    "pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0)" '),
    ("17",   "inclusive",    "m_vis",  'm_vis(25,0,250)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0)" '),
    ("17",   "inclusive",    "mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_jets>1&&n_bjets==0)" '),
    ("17",   "inclusive",    "sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_jets>1&&n_bjets==0)" '),
    ("17",   "inclusive",    "n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0)" '),
    ("17",   "inclusive",    "m_sv",  'm_sv(30,0,300)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0)" '),

    ("17",   "inclusive",    "lomsv_pt_1",  'pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs  --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_pt_2",  'pt_2[30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_m_vis",  'm_vis(25,0,250)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_m_sv",  'm_sv(30,0,300)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),

    ("17",   "inclusive",    "himsv_pt_1",  'pt_1[20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs  --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_pt_2",  'pt_2[30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_m_vis",  'm_vis(25,0,250)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_m_sv",  'm_sv(30,0,300)', ' --set_alias="sel:mt_1<50" --do_ff_systs --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),

  ]
  scheme_et=scheme_mt
  scheme_tt=[
    ("17",   "inclusive",    "pt_1",  'pt_1[50,55,60,70,80,90,100,120,140,160,200]', ' --do_ff_systs  '),
    ("17",   "inclusive",    "pt_2",  'pt_2[40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --do_ff_systs '),
    ("17",   "inclusive",    "met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --do_ff_systs '),
    ("17",   "inclusive",    "pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --do_ff_systs '),
    ("17",   "inclusive",    "m_vis",  'm_vis(25,0,250)', ' --do_ff_systs '),
    ("17",   "inclusive",    "mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --do_ff_systs --set_alias="inclusive:(n_jets>1)" '),
    ("17",   "inclusive",    "sjdphi",  'sjdphi(12,-3.2,3.2)', ' --do_ff_systs --set_alias="inclusive:(n_jets>1)" '),
    ("17",   "inclusive",    "n_jets",  'n_jets(4,0,4)', ' --do_ff_systs '),
    ("17",   "inclusive",    "m_sv",  'm_sv(30,0,300)', ' --do_ff_systs '),

    ("17",   "inclusive",    "lomsv_pt_1",  'pt_1[50,55,60,70,80,90,100,120,140,160,200]', ' --do_ff_systs  --set_alias="inclusive:(m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_pt_2",  'pt_2[40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --do_ff_systs --set_alias="inclusive:(m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --do_ff_systs --set_alias="inclusive:(m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --do_ff_systs --set_alias="inclusive:(m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_m_vis",  'm_vis(25,0,250)', ' --do_ff_systs --set_alias="inclusive:(m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --do_ff_systs --set_alias="inclusive:(n_jets>1&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_sjdphi",  'sjdphi(12,-3.2,3.2)', ' --do_ff_systs --set_alias="inclusive:(n_jets>1&&m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_n_jets",  'n_jets(4,0,4)', ' --do_ff_systs --set_alias="inclusive:(m_sv<100)" '),
    ("17",   "inclusive",    "lomsv_m_sv",  'm_sv(30,0,300)', ' --do_ff_systs --set_alias="inclusive:(m_sv<100)" '),
 
    ("17",   "inclusive",    "himsv_pt_1",  'pt_1[50,55,60,70,80,90,100,120,140,160,200]', ' --do_ff_systs  --set_alias="inclusive:(m_sv>150)" '),
    ("17",   "inclusive",    "himsv_pt_2",  'pt_2[40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --do_ff_systs --set_alias="inclusive:(m_sv>150)" '),
    ("17",   "inclusive",    "himsv_met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --do_ff_systs --set_alias="inclusive:(m_sv>150)" '),
    ("17",   "inclusive",    "himsv_pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --do_ff_systs --set_alias="inclusive:(m_sv>150)" '),
    ("17",   "inclusive",    "himsv_m_vis",  'm_vis(25,0,250)', ' --do_ff_systs --set_alias="inclusive:(m_sv>150)" '),
    ("17",   "inclusive",    "himsv_mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --do_ff_systs --set_alias="inclusive:(n_jets>1&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_sjdphi",  'sjdphi(12,-3.2,3.2)', ' --do_ff_systs --set_alias="inclusive:(n_jets>1&&m_sv>150)" '),
    ("17",   "inclusive",    "himsv_n_jets",  'n_jets(4,0,4)', ' --do_ff_systs --set_alias="inclusive:(m_sv>150)" '),
    ("17",   "inclusive",    "himsv_m_sv",  'm_sv(30,0,300)', ' --do_ff_systs --set_alias="inclusive:(m_sv>150)" '),

  ]
  scheme_em=[
    ("19",   "inclusive",    "pt_1",  'pt_1[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0)" '),
    ("19",   "inclusive",    "pt_2",  'pt_2[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0)" '),
    ("19",   "inclusive",    "met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0)" '),
    ("19",   "inclusive",    "pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0)" '),
    ("19",   "inclusive",    "m_vis",  'm_vis(25,0,250)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0)" '),
    ("19",   "inclusive",    "mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0)" '),
    ("19",   "inclusive",    "sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0)" '), 
    ("19",   "inclusive",    "n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0)" '),
    ("19",   "inclusive",    "m_sv",  'm_sv(30,0,300)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0)" '),

    ("19",   "inclusive",    "lomsv_pt_1",  'pt_1[10,15,20,25,20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_pt_2",  'pt_2[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_m_vis",  'm_vis(25,0,250)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),
    ("19",   "inclusive",    "lomsv_m_sv",  'm_sv(30,0,300)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&m_sv<100)" '),

    ("19",   "inclusive",    "himsv_pt_1",  'pt_1[10,15,20,25,20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_pt_2",  'pt_2[10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,140,160,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_met",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_pt_tt",  'pt_tt[0,25,50,75,100,125,150,175,200,250,300,400]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_m_vis",  'm_vis(25,0,250)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_mjj",  'mjj[0,50,100,150,200,250,300,400,500,600,800]', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_sjdphi",  'sjdphi(12,-3.2,3.2)', ' --set_alias="sel:pzeta>-35" --set_alias="inclusive:(n_jets>1&&n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_n_jets",  'n_jets(4,0,4)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),
    ("19",   "inclusive",    "himsv_m_sv",  'm_sv(30,0,300)', ' --set_alias="sel:pzeta>-35"  --set_alias="inclusive:(n_bjets==0&&m_sv>150)" '),

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
        if options.embedding: extra+=' --embedding'
        extra_jes = options.extra + ' ' + extra_global + ' ' + jes_systematics + ' ' + opts + ' --no_default '

        if not options.hadd:
            if not options.batch:
                print 'python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s --var="%(var)s" %(extra)s --no_plot' % vars()
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
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
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes1'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=1:9' % vars())
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes2'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=10:18' % vars())
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
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

