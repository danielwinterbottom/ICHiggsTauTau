#!/usr/bin/env python

# python scripts/makeDatacardsSMCPDecays16.py --cfg=scripts/plot_cpdecays_2016.cfg -c 'et,mt,tt' scripts/Params_2016_cpdecays16.json -s 'mlcpdecays16' --embedding --total_jes --batch

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

#common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_scale_met="CMS_htt_boson_scale_met_13TeV" --syst_res_met="CMS_htt_boson_reso_met_13TeV" --syst_eff_b=CMS_eff_b_13TeV --syst_fake_b=CMS_fake_b_13TeV --syst_quarkmass="CMS_FiniteQuarkMass_13TeV" --syst_ps="CMS_PS_ggH_13TeV" --syst_ue="CMS_UE_ggH_13TeV" '

# common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_scale_met="CMS_htt_boson_scale_met_13TeV" --syst_res_met="CMS_htt_boson_reso_met_13TeV" --syst_quarkmass="CMS_FiniteQuarkMass_13TeV" --syst_ps="CMS_PS_ggH_13TeV" --syst_ue="CMS_UE_ggH_13TeV" --syst_prefire="CMS_PreFire_13TeV" '

# common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_qcd_scale="CMS_scale_gg_13TeV" --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_scale_met="CMS_htt_boson_scale_met_13TeV" --syst_res_met="CMS_htt_boson_reso_met_13TeV" --syst_quarkmass="CMS_FiniteQuarkMass_13TeV" --syst_ps="CMS_PS_ggH_13TeV" --syst_ue="CMS_UE_ggH_13TeV"  '
# common_shape_systematics = ' --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" --syst_prefire="CMS_PreFire_13TeV" '
common_shape_systematics = ' --syst_prefire="CMS_PreFire_13TeV" '

if options.regional_jes:
    common_shape_systematics += ' --syst_scale_j_full="CMS_scale_j_eta0to5_13TeV" --syst_scale_j_cent="CMS_scale_j_eta0to3_13TeV" --syst_scale_j_hf="CMS_scale_j_eta3to5_13TeV" --syst_scale_j_rbal="CMS_scale_j_RelativeBal_13TeV" '
    # common_shape_systematics += ' --syst_scale_j_full_corr="CMS_scale_j_eta0to5_corr_13TeV" --syst_scale_j_cent_corr="CMS_scale_j_eta0to3_corr_13TeV" --syst_scale_j_hf_corr="CMS_scale_j_eta3to5_corr_13TeV" '
    # common_shape_systematics += ' --syst_scale_j_full_uncorr="CMS_scale_j_eta0to5_uncorr_13TeV" --syst_scale_j_cent_uncorr="CMS_scale_j_eta0to3_uncorr_13TeV" --syst_scale_j_hf_uncorr="CMS_scale_j_eta3to5_uncorr_13TeV" '

if options.total_jes:
    common_shape_systematics += ' --syst_scale_j="CMS_scale_j_13TeV" '
    # common_shape_systematics += ' --syst_scale_j_corr="CMS_scale_j_corr_13TeV" '
    # common_shape_systematics += ' --syst_scale_j_uncorr="CMS_scale_j_uncorr_13TeV" '


em_shape_systematics=' --syst_tau_scale="CMS_scale_e_13TeV" --syst_em_qcd_rate_0jet="CMS_em_QCD_0JetRate_13TeV" --syst_em_qcd_rate_1jet="CMS_em_QCD_1JetRate_13TeV" --syst_em_qcd_shape_0jet="CMS_em_QCD_0JetShape_13TeV" --syst_em_qcd_shape_1jet="CMS_em_QCD_1JetShape_13TeV" --syst_em_qcd_extrap="CMS_em_QCD_IsoExtrap_13TeV" --syst_qcd_bkg="CMS_em_QCD_BackgroundSubtraction_13TeV" '
et_shape_systematics=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
mt_shape_systematics=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
# tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" '
et_shape_systematics=''
mt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" '
zmm_shape_systematics=' --folder=/vols/cms/dw515/Offline/output/SM/Apr09/ '


if options.embedding:
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

# the CP in decays datacards
if SCHEME == 'cpdecays16':
    VAR_INCLUSIVE_TT     = "m_sv,aco_angle_mod[0,90,110,130,150],(14,0,6.28319)"
    VAR_0JET_TT          = "m_sv,aco_angle_mod[0,110,150],(14,0,6.28319)"
    VAR_BOOSTED_TT       = "m_sv,aco_angle_mod[0,90,110,130,150],(14,0,6.28319)"
    VAR_DIJET_TT         = "m_sv,aco_angle_mod[0,90,110,140],(14,0,6.28319)"
    VAR_DIJETWIDE_TT     = "m_sv,aco_angle_mod[0,90,150],(14,0,6.28319)"

    scheme_em = []
    scheme_et = []
    scheme_mt = []
    scheme_tt = [
        # ("17", "inclusive_rho", "inclusive_rho",               VAR_INCLUSIVE_TT, ''),
        ("17", "0jet_rho",           "0jet_rho",                 VAR_0JET_TT,      ' --ff_ss_closure --add_wt=wt_prefire'),
        ("17", "boosted_rho",        "boosted_rho",              VAR_BOOSTED_TT,   ' --ff_ss_closure --add_wt=wt_prefire'),
        # ("17",   "dijet_rho", "dijet_rho",  VAR_DIJETWIDE_TT, ''),
        ("17", "dijet_lowboost_rho", "dijet_lowboost_rho",       VAR_DIJET_TT,     ' --ff_ss_closure --add_wt=wt_prefire'),
        ("17", "dijet_boosted_rho",  "dijet_boosted_rho",        VAR_DIJET_TT,     ' --ff_ss_closure --add_wt=wt_prefire'),

        ("17", "0jet_other",           "0jet_other",                 VAR_0JET_TT,      ' --ff_ss_closure --add_wt=wt_prefire'),
        ("17", "boosted_other",        "boosted_other",              VAR_BOOSTED_TT,   ' --ff_ss_closure --add_wt=wt_prefire'),
        ("17", "dijet_lowboost_other", "dijet_lowboost_other",       VAR_DIJET_TT,     ' --ff_ss_closure --add_wt=wt_prefire'),
        ("17", "dijet_boosted_other",  "dijet_boosted_other",        VAR_DIJET_TT,     ' --ff_ss_closure --add_wt=wt_prefire'),

        # ("17", "0jet_rho_idg0p5",    "0jet_rho_idg0p5",          VAR_0JET_TT,      ' --ff_ss_closure --add_wt=wt_prefire'),
        # ("17", "boosted_rho_idg0p5", "boosted_rho_idg0p5",       VAR_BOOSTED_TT,   ' --ff_ss_closure --add_wt=wt_prefire'),
        # ("17", "dijet_rho_idg0p5",   "dijet_rho_idg0p5",         VAR_DIJETWIDE_TT, ' --ff_ss_closure --add_wt=wt_prefire'),
        # # ("17",   "dijet_lowboost_rho_idg0p5",   "dijet_lowboost_rho_idg0p5",  VAR_DIJET_TT, ''),
        # # ("17",   "dijet_boosted_rho_idg0p5",     "dijet_boosted_rho_idg0p5",  VAR_DIJET_TT, ''),

        # ("17", "0jet_rho_idl0p5",    "0jet_rho_idl0p5",          VAR_0JET_TT,      ' --ff_ss_closure --add_wt=wt_prefire'),
        # ("17", "boosted_rho_idl0p5", "boosted_rho_idl0p5",       VAR_BOOSTED_TT,   ' --ff_ss_closure --add_wt=wt_prefire'),
        # ("17", "dijet_rho_idl0p5",   "dijet_rho_idl0p5",         VAR_DIJETWIDE_TT, ' --ff_ss_closure --add_wt=wt_prefire'),
        # # ("17",   "dijet_lowboost_rho_idl0p5", "dijet_lowboost_rho_idl0p5",  VAR_DIJET_TT, ''),
        # # ("17",   "dijet_boosted_rho_idl0p5",   "dijet_boosted_rho_idl0p5",  VAR_DIJET_TT, ''),
    ]

    bkg_schemes = {
        'et' : 'et_default',
        'mt' : 'mt_with_zmm',
        'em' : 'em_default',
        'tt' : 'tt_default',
    }
    ANA = 'sm'

if SCHEME == 'cpdecays16_ss':
    VAR_INCLUSIVE_TT     = "m_sv,aco_angle_mod[0,90,110,130,150],(14,0,6.3)"

    VAR_0JET_TT          = "m_sv,aco_angle_mod[0,110,150],(14,0,6.3)"
    VAR_0JETIDG0p5_TT    = "m_sv,aco_angle_mod[0,115,150],(14,0,6.3)"
    VAR_0JETIDL0p5_TT    = "m_sv,aco_angle_mod[0,115,150],(14,0,6.3)"

    VAR_BOOSTED_TT       = "m_sv,aco_angle_mod[0,90,110,130,150],(14,0,6.3)"

    VAR_DIJET_TT         = "m_sv,aco_angle_mod[0,90,110,150],(14,0,6.3)"

    VAR_DIJETWIDE_TT     = "m_sv,aco_angle_mod[0,90,150],(14,0,6.3)"

    scheme_em = []
    scheme_et = []
    scheme_mt = [
        ("12",   "0jet_mixed",    "0jet_mixed",  VAR_0JET_TT, ' --set_alias="sel:mt_1<50" '),
        ("12",   "boosted_mixed", "boosted_mixed",  VAR_BOOSTED_TT, ' --set_alias="sel:mt_1<50" '),
        ("12",   "dijet_lowboost_mixed",    "dijet_lowboost_mixed",  VAR_DIJET_TT, ' --set_alias="sel:mt_1<50" '),
        ("12",   "dijet_boosted_mixed",      "dijet_boosted_mixed",   VAR_DIJET_TT, ' --set_alias="sel:mt_1<50" '),
    ]
    scheme_tt = [
        # ("17", "inclusive_rho", "inclusive_rho", VAR_INCLUSIVE_TT, ''),
        ("8",   "0jet_rho",    "0jet_rho",  VAR_0JETIDG0p5_TT, ''),
        ("8",   "boosted_rho", "boosted_rho",  VAR_BOOSTED_TT, ''),
        ("8",   "dijet_rho", "dijet_rho",  VAR_DIJETWIDE_TT, ''),
        # ("8",   "dijet_lowboost_rho",    "dijet_lowboost_rho",  VAR_DIJET_TT, ''),
        # ("8",   "dijet_boosted_rho",     "dijet_boosted_rho",   VAR_DIJET_TT, ''),

        ("8",   "0jet_rho_idg0p5",    "0jet_rho_idg0p5",  VAR_0JETIDG0p5_TT, ''),
        ("8",   "boosted_rho_idg0p5",  "boosted_rho_idg0p5",  VAR_BOOSTED_TT, ''),
        ("8",   "dijet_rho_idg0p5", "dijet_rho_idg0p5",  VAR_DIJETWIDE_TT, ''),
        # ("17",   "dijet_lowboost_rho_idg0p5",   "dijet_lowboost_rho_idg0p5",  VAR_DIJET_TT, ''),
        # ("17",   "dijet_boosted_rho_idg0p5",     "dijet_boosted_rho_idg0p5",  VAR_DIJET_TT, ''),

        ("8",   "0jet_rho_idl0p5",    "0jet_rho_idl0p5",  VAR_0JETIDL0p5_TT, ''),
        ("8",   "boosted_rho_idl0p5",  "boosted_rho_idl0p5",  VAR_BOOSTED_TT, ''),
        ("8",   "dijet_rho_idl0p5", "dijet_rho_idl0p5",  VAR_DIJETWIDE_TT, ''),
        # ("17",   "dijet_lowboost_rho_idl0p5",   "dijet_lowboost_rho_idl0p5",  VAR_DIJET_TT, ''),
        # ("17",   "dijet_boosted_rho_idl0p5",     "dijet_boosted_rho_idl0p5",  VAR_DIJET_TT, ''),
    ]

    bkg_schemes = {
        'et' : 'et_default',
        'mt' : 'mt_with_zmm',
        'em' : 'em_default',
        'tt' : 'tt_default',
    }
    ANA = 'sm'

if SCHEME == 'mlcpdecays16':

    VAR_H_ET        = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_ET = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_ET = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_TT_ET       = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZLL_ET      = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_ET        = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_ET = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_ET = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_TT_OTHER_ET       = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZLL_OTHER_ET      = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_H_MT        = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_MT = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_MT = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_TT_MT       = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZLL_MT      = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_MT        = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_MT = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_MT = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_TT_OTHER_MT       = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZLL_OTHER_MT      = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_H_TT        = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_TT = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_TT = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_TT        = "IC_Feb13_fix1_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_TT = "IC_Feb13_fix1_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_TT = "IC_Feb13_fix1_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_IDG0p5_TT = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.7],(14,0,6.28319)"
    VAR_H_IDG0p5_TT = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.6],(14,0,6.28319)"


    scheme_em = []
    scheme_et = []
    scheme_mt = []
    scheme_tt = [
        ("17", "higgs"   ,         "higgs"  ,          VAR_H_TT,        ' --ff_ss_closure '),
        ("17", "zttEmbed",         "zttEmbed",         VAR_ZTTEMBED_TT, ' --ff_ss_closure '),
        ("17", "jetFakes",         "jetFakes",         VAR_JETFAKES_TT, ' --ff_ss_closure '),

        # ("17", "higgs"   ,         "higgs"  ,          VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed",         "zttEmbed",         VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes",         "jetFakes",         VAR_IDG0p5_TT,   ' --ff_ss_closure '),

        # ("17", "higgs_idg0p5",     "higgs_idg0p5",     VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed_idg0p5",  "zttEmbed_idg0p5",  VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes_idg0p5",  "jetFakes_idg0p5",  VAR_IDG0p5_TT,   ' --ff_ss_closure '),

        # ("17", "higgs_idgl0p5",    "higgs_idgl0p5",    VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed_idgl0p5", "zttEmbed_idgl0p5", VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes_idgl0p5", "jetFakes_idgl0p5", VAR_IDG0p5_TT,   ' --ff_ss_closure '),

        # ("17", "higgs_idl0p5",     "higgs_idl0p5",     VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed_idl0p5",  "zttEmbed_idl0p5",  VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes_idl0p5",  "jetFakes_idl0p5",  VAR_IDG0p5_TT,   ' --ff_ss_closure '),
    ]

    bkg_schemes = {
        'et' : 'et_default',
        'mt' : 'mt_with_zmm',
        'em' : 'em_default',
        'tt' : 'tt_default',
    }
    ANA = 'sm'

if SCHEME == 'mlcpdecays16_new':

    VAR_H_ET              = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8,0.9],(14,0,6.28319)"
    VAR_ZTTEMBED_ET       = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8,0.9],(14,0,6.28319)"
    VAR_JETFAKES_ET       = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8,0.9],(14,0,6.28319)"
    VAR_TT_ET             = "IC_Apr02_max_score,aco_angle_mod[0.0,0.7],(14,0,6.28319)"
    VAR_ZLL_ET            = "IC_Apr02_max_score,aco_angle_mod[0.0,0.7],(14,0,6.28319)"

    VAR_H_OTHER_ET        = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_ET = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_ET = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_TT_OTHER_ET       = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZLL_OTHER_ET      = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_H_MT              = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8,0.9],(14,0,6.28319)"
    VAR_ZTTEMBED_MT       = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8,0.9],(14,0,6.28319)"
    VAR_JETFAKES_MT       = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8,0.9],(14,0,6.28319)"
    VAR_TT_MT             = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.7],(14,0,6.28319)"
    VAR_ZLL_MT            = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.7],(14,0,6.28319)"

    VAR_H_OTHER_MT        = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_MT = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_MT = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_TT_OTHER_MT       = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZLL_OTHER_MT      = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_H_TT              = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_TT       = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_TT       = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_TT_A1              = "IC_Feb13_fix1_max_score,aco_angle_1[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_TT_A1       = "IC_Feb13_fix1_max_score,aco_angle_1[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_TT_A1       = "IC_Feb13_fix1_max_score,aco_angle_1[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_TT        = "IC_Feb13_fix1_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_TT = "IC_Feb13_fix1_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_TT = "IC_Feb13_fix1_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_IDG0p5_TT         = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.7],(14,0,6.28319)"
    VAR_H_IDG0p5_TT       = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.6],(14,0,6.28319)"

    VAR_IDG0p5_TT         = VAR_H_TT
    VAR_H_IDG0p5_TT       = VAR_H_TT

    scheme_em = []
    scheme_et = [
        ("17", "higgs",           "higgs",          VAR_H_ET,        ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zttEmbed",        "zttEmbed",       VAR_ZTTEMBED_ET, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zll",             "zll",            VAR_ZLL_ET,      ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "tt",              "tt",             VAR_TT_ET,       ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "jetFakes",        "jetFakes",       VAR_JETFAKES_ET, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),

        ("17", "higgs_other",           "higgs_other",          VAR_H_OTHER_ET,        ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zttEmbed_other",        "zttEmbed_other",       VAR_ZTTEMBED_OTHER_ET, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zll_other",             "zll_other",            VAR_ZLL_OTHER_ET,      ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "tt_other",              "tt_other",             VAR_TT_OTHER_ET,       ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "jetFakes_other",        "jetFakes_other",       VAR_JETFAKES_OTHER_ET, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
    ]
    scheme_mt = [
        ("17", "higgs",           "higgs",          VAR_H_MT,        ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zttEmbed",        "zttEmbed",       VAR_ZTTEMBED_MT, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zll",             "zll",            VAR_ZLL_MT,      ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "tt",              "tt",             VAR_TT_MT,       ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "jetFakes",        "jetFakes",       VAR_JETFAKES_MT, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),

        ("17", "higgs_other",           "higgs_other",          VAR_H_OTHER_MT,        ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zttEmbed_other",        "zttEmbed_other",       VAR_ZTTEMBED_OTHER_MT, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zll_other",             "zll_other",            VAR_ZLL_OTHER_MT,      ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "tt_other",              "tt_other",             VAR_TT_OTHER_MT,       ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "jetFakes_other",        "jetFakes_other",       VAR_JETFAKES_OTHER_MT, ' --set_alias="sel:mt_1<50" --ff_ss_closure --add_wt=wt_prefire '),
    ]
    scheme_tt = [
        ("17", "higgs_rho"   ,           "higgs_rho"  ,            VAR_H_TT,              ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "zttEmbed_rho",           "zttEmbed_rho",           VAR_ZTTEMBED_TT,       ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "jetFakes_rho",           "jetFakes_rho",           VAR_JETFAKES_TT,       ' --ff_ss_closure --add_wt=wt_prefire '),

        ("17", "higgs_a1rho"   ,           "higgs_a1rho"  ,            VAR_H_TT_A1,              ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "zttEmbed_a1rho",           "zttEmbed_a1rho",           VAR_ZTTEMBED_TT_A1,       ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "jetFakes_a1rho",           "jetFakes_a1rho",           VAR_JETFAKES_TT_A1,       ' --ff_ss_closure --add_wt=wt_prefire '),

        # ("17", "higgs_other",        "higgs_other"  ,      VAR_H_OTHER_TT,        ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "zttEmbed_other",     "zttEmbed_other",     VAR_ZTTEMBED_OTHER_TT, ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "jetFakes_other",     "jetFakes_other",     VAR_JETFAKES_OTHER_TT, ' --ff_ss_closure --add_wt=wt_prefire '),

        # ("17", "higgs"   ,         "higgs"  ,            VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed",         "zttEmbed",           VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes",         "jetFakes",           VAR_IDG0p5_TT,   ' --ff_ss_closure '),

        # ("17", "higgs_idg0p5",       "higgs_idg0p5",       VAR_H_IDG0p5_TT, ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "zttEmbed_idg0p5",    "zttEmbed_idg0p5",    VAR_IDG0p5_TT,   ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "jetFakes_idg0p5",    "jetFakes_idg0p5",    VAR_IDG0p5_TT,   ' --ff_ss_closure --add_wt=wt_prefire '),

        # ("17", "higgs_NOTidg0p5",    "higgs_NOTidg0p5",    VAR_H_IDG0p5_TT, ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "zttEmbed_NOTidg0p5", "zttEmbed_NOTidg0p5", VAR_IDG0p5_TT,   ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "jetFakes_NOTidg0p5", "jetFakes_NOTidg0p5", VAR_IDG0p5_TT,   ' --ff_ss_closure --add_wt=wt_prefire '),

        # ("17", "higgs_idgl0p5",    "higgs_idgl0p5",    VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed_idgl0p5", "zttEmbed_idgl0p5", VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes_idgl0p5", "jetFakes_idgl0p5", VAR_IDG0p5_TT,   ' --ff_ss_closure '),

        # ("17", "higgs_idl0p5",     "higgs_idl0p5",     VAR_H_IDG0p5_TT, ' --ff_ss_closure '),
        # ("17", "zttEmbed_idl0p5",  "zttEmbed_idl0p5",  VAR_IDG0p5_TT,   ' --ff_ss_closure '),
        # ("17", "jetFakes_idl0p5",  "jetFakes_idl0p5",  VAR_IDG0p5_TT,   ' --ff_ss_closure '),
    ]

    bkg_schemes = {
        'et' : 'et_default',
        'mt' : 'mt_with_zmm',
        'em' : 'em_default',
        'tt' : 'tt_default',
    }
    ANA = 'sm'

if SCHEME == 'mlcpdecays16_nn_vienna':

    VAR_H_TT              = "IC_Vienna_fix_ggh_score+IC_Vienna_fix_qqh_score,aco_angle_mod[0.0,0.4,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_GGH_TT            = "IC_Vienna_fix_max_score,aco_angle_mod[0.0,0.3,0.4,0.5,0.6],(14,0,6.28319)"
    VAR_QQH_TT            = "IC_Vienna_fix_max_score,aco_angle_mod[0.0,0.4,0.6],(14,0,6.28319)"
    VAR_ZTTEMBED_TT       = "IC_Vienna_fix_max_score,aco_angle_mod[0.0,0.3,0.4,0.5,0.6,0.7],(14,0,6.28319)"
    VAR_JETFAKES_TT       = "IC_Vienna_fix_max_score,aco_angle_mod[0.0,0.3,0.4,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_MISC_TT           = "IC_Vienna_fix_max_score,aco_angle_mod[0.0,0.4,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_TT_A1RHO              = "IC_Vienna_fix_ggh_score+IC_Vienna_fix_qqh_score,aco_angle_1[0.0,0.4,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_GGH_TT_A1RHO            = "IC_Vienna_fix_max_score,aco_angle_1[0.0,0.3,0.4,0.5,0.6],(14,0,6.28319)"
    VAR_QQH_TT_A1RHO            = "IC_Vienna_fix_max_score,aco_angle_1[0.0,0.4,0.6],(14,0,6.28319)"
    VAR_ZTT_A1RHOEMBED_TT_A1RHO = "IC_Vienna_fix_max_score,aco_angle_1[0.0,0.3,0.4,0.5,0.6,0.7],(14,0,6.28319)"
    VAR_JETFAKES_TT_A1RHO       = "IC_Vienna_fix_max_score,aco_angle_1[0.0,0.3,0.4,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_MISC_TT_A1RHO           = "IC_Vienna_fix_max_score,aco_angle_1[0.0,0.4,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_TT        = "IC_Vienna_fix_ggh_score+IC_Vienna_fix_qqh_score[0.0,0.3,0.6,0.7,0.8,1.0]"
    VAR_GGH_OTHER_TT      = "IC_Vienna_fix_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]"
    VAR_QQH_OTHER_TT      = "IC_Vienna_fix_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]"
    VAR_ZTTEMBED_OTHER_TT = "IC_Vienna_fix_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]"
    VAR_JETFAKES_OTHER_TT = "IC_Vienna_fix_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]"
    VAR_MISC_OTHER_TT     = "IC_Vienna_fix_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]"

    scheme_em = []
    scheme_et = []
    scheme_mt = []
    scheme_tt = [
        # ("17", "NN_sm_higgs_rho",          "higgs_rho",          VAR_H_TT,              ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "NN_sm_ggh_rho",            "ggh_rho",            VAR_GGH_TT,            ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "NN_sm_qqh_rho",            "qqh_rho",            VAR_QQH_TT,            ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "NN_sm_zttEmbed_rho",       "zttEmbed_rho",       VAR_ZTTEMBED_TT,       ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "NN_sm_jetFakes_rho",       "jetFakes_rho",       VAR_JETFAKES_TT,       ' --ff_ss_closure --add_wt=wt_prefire '),
        # ("17", "NN_sm_misc_rho",           "misc_rho",           VAR_MISC_TT,           ' --ff_ss_closure --add_wt=wt_prefire '),

        ("17", "NN_sm_higgs_a1rho",          "higgs_a1rho",          VAR_H_TT_A1RHO,              ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "NN_sm_ggh_a1rho",            "ggh_a1rho",            VAR_GGH_TT_A1RHO,            ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "NN_sm_qqh_a1rho",            "qqh_a1rho",            VAR_QQH_TT_A1RHO,            ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "NN_sm_zttEmbed_a1rho",       "zttEmbed_a1rho",       VAR_ZTT_A1RHOEMBED_TT_A1RHO, ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "NN_sm_jetFakes_a1rho",       "jetFakes_a1rho",       VAR_JETFAKES_TT_A1RHO,       ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "NN_sm_misc_a1rho",           "misc_a1rho",           VAR_MISC_TT_A1RHO,           ' --ff_ss_closure --add_wt=wt_prefire '),

#         ("17", "NN_sm_higgs_other",    "higgs_other",    VAR_H_OTHER_TT,        ' --ff_ss_closure --add_wt=wt_prefire '),
#         ("17", "NN_sm_ggh_other",      "ggh_other",      VAR_GGH_OTHER_TT,      ' --ff_ss_closure --add_wt=wt_prefire '),
#         ("17", "NN_sm_qqh_other",      "qqh_other",      VAR_QQH_OTHER_TT,      ' --ff_ss_closure --add_wt=wt_prefire '),
#         ("17", "NN_sm_zttEmbed_other", "zttEmbed_other", VAR_ZTTEMBED_OTHER_TT, ' --ff_ss_closure --add_wt=wt_prefire '),
#         ("17", "NN_sm_jetFakes_other", "jetFakes_other", VAR_JETFAKES_OTHER_TT, ' --ff_ss_closure --add_wt=wt_prefire '),
#         ("17", "NN_sm_misc_other",     "misc_other",     VAR_MISC_OTHER_TT,     ' --ff_ss_closure --add_wt=wt_prefire '),
    ]

    bkg_schemes = {
        'et' : 'et_default',
        'mt' : 'mt_with_zmm',
        'em' : 'em_default',
        'tt' : 'tt_default',
    }
    ANA = 'sm'

if SCHEME == 'mlcpdecays16_nn':

    VAR_H_TT              = "IC_keras_sm4_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_TT       = "IC_keras_sm4_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_TT       = "IC_keras_sm4_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_TT        = "IC_keras_sm4_max_score[0.0,0.4,0.5,0.6,0.7,0.8,1.0]"
    VAR_ZTTEMBED_OTHER_TT = "IC_keras_sm4_max_score[0.0,0.4,0.5,0.6,0.7,0.8,1.0]"
    VAR_JETFAKES_OTHER_TT = "IC_keras_sm4_max_score[0.0,0.4,0.5,0.6,0.7,0.8,1.0]"

    scheme_em = []
    scheme_et = []
    scheme_mt = []
    scheme_tt = [
        ("17", "NN_higgs",          "higgs",          VAR_H_TT,              ' --add_wt=wt_prefire '),
        ("17", "NN_zttEmbed",       "zttEmbed",       VAR_ZTTEMBED_TT,       ' --add_wt=wt_prefire '),
        ("17", "NN_jetFakes",       "jetFakes",       VAR_JETFAKES_TT,       ' --add_wt=wt_prefire '),

        ("17", "NN_higgs_other",    "higgs_other",    VAR_H_OTHER_TT,        ' --add_wt=wt_prefire '),
        ("17", "NN_zttEmbed_other", "zttEmbed_other", VAR_ZTTEMBED_OTHER_TT, ' --add_wt=wt_prefire '),
        ("17", "NN_jetFakes_other", "jetFakes_other", VAR_JETFAKES_OTHER_TT, ' --add_wt=wt_prefire '),
    ]

    bkg_schemes = {
        'et' : 'et_default',
        'mt' : 'mt_with_zmm',
        'em' : 'em_default',
        'tt' : 'tt_default',
    }
    ANA = 'sm'



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
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra)s --ratio --ratio_range 0,2 --log_y ' % vars())

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

