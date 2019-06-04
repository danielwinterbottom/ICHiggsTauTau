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
# et_shape_systematics=' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" --syst_e_scale="CMS_scale_e_13TeV" '
# mt_shape_systematics=' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
# tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_w_fake_rate="CMS_htt_jetToTauFake_13TeV" '
tt_shape_systematics=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" '
et_shape_systematics=''
mt_shape_systematics=''
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

    VAR_H_ET              = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_ET       = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_ET       = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_TT_ET             = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZLL_ET            = "IC_Apr02_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_ET        = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_ET = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_ET = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_TT_OTHER_ET       = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZLL_OTHER_ET      = "IC_Apr02_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_H_MT              = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZTTEMBED_MT       = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_MT       = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_TT_MT             = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_ZLL_MT            = "IC_Mar26_fix2_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

    VAR_H_OTHER_MT        = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZTTEMBED_OTHER_MT = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_JETFAKES_OTHER_MT = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_TT_OTHER_MT       = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"
    VAR_ZLL_OTHER_MT      = "IC_Mar26_fix2_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]"

    VAR_H_TT              = "aco_angle_mod(14,0,6.28319)"
    VAR_ZTTEMBED_TT       = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"
    VAR_JETFAKES_TT       = "IC_Feb13_fix1_max_score,aco_angle_mod[0.0,0.5,0.6,0.7,0.8],(14,0,6.28319)"

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
        ("17", "higgs"   ,           "higgs"  ,            VAR_H_TT,              ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zttEmbed",           "zttEmbed",           VAR_ZTTEMBED_TT,       ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "jetFakes",           "jetFakes",           VAR_JETFAKES_TT,       ' --ff_ss_closure --add_wt=wt_prefire '),

        ("17", "higgs_other",        "higgs_other"  ,      VAR_H_OTHER_TT,        ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "zttEmbed_other",     "zttEmbed_other",     VAR_ZTTEMBED_OTHER_TT, ' --ff_ss_closure --add_wt=wt_prefire '),
        ("17", "jetFakes_other",     "jetFakes_other",     VAR_JETFAKES_OTHER_TT, ' --ff_ss_closure --add_wt=wt_prefire '),

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
        ("17", "NN_sm_higgs",          "higgs",          VAR_H_TT,              ' --add_wt=wt_prefire '),
        ("17", "NN_sm_ggh",            "ggh",            VAR_GGH_TT,            ' --add_wt=wt_prefire '),
        ("17", "NN_sm_qqh",            "qqh",            VAR_QQH_TT,            ' --add_wt=wt_prefire '),
        ("17", "NN_sm_zttEmbed",       "zttEmbed",       VAR_ZTTEMBED_TT,       ' --add_wt=wt_prefire '),
        ("17", "NN_sm_jetFakes",       "jetFakes",       VAR_JETFAKES_TT,       ' --add_wt=wt_prefire '),
        ("17", "NN_sm_misc",           "misc",           VAR_MISC_TT,           ' --add_wt=wt_prefire '),

        ("17", "NN_sm_higgs_other",    "higgs_other",    VAR_H_OTHER_TT,        ' --add_wt=wt_prefire '),
        ("17", "NN_sm_ggh_other",      "ggh_other",      VAR_GGH_OTHER_TT,      ' --add_wt=wt_prefire '),
        ("17", "NN_sm_qqh_other",      "qqh_other",      VAR_QQH_OTHER_TT,      ' --add_wt=wt_prefire '),
        ("17", "NN_sm_zttEmbed_other", "zttEmbed_other", VAR_ZTTEMBED_OTHER_TT, ' --add_wt=wt_prefire '),
        ("17", "NN_sm_jetFakes_other", "jetFakes_other", VAR_JETFAKES_OTHER_TT, ' --add_wt=wt_prefire '),
        ("17", "NN_sm_misc_other",     "misc_other",     VAR_MISC_OTHER_TT,     ' --add_wt=wt_prefire '),
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


if SCHEME == 'cpsummer16':
  
  VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'
  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'sjdphi(20,-3.2,3.2)' 
  VAR_DIJET_WIDEBINS = 'sjdphi(10,-3.2,3.2)' 
  
  VAR_0JET_LT_WCR = 'mt_1[80,200]'
  VAR_BOOSTED_WCR = 'mt_1[80,200]'
  
  VAR_0JET_LT_QCDCR = 'm_vis(4,40,200)' 
  VAR_BOOSTED_LT_QCDCR = 'm_sv(4,40,200)'
  
  VAR_0JET_TT_QCDCR = 'm_sv[0,300]'
  VAR_BOOSTED_TT_QCDCR = 'm_sv[0,300]' 

  scheme_et = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" --add_wt=wt_quarkmass'),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" --set_alias="w_shape:({w_shape}&&iso_1>0.1)" '),
    ("21",   "dijet",     "dijet",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_WIDEBINS, '--set_alias="sel:mt_1<50" ')


  ]
  scheme_mt = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:(mt_1>80&&mt_1<200)" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" --set_alias="w_shape:({w_shape}&&iso_1>0.15)" '),
    ("21",   "dijet",     "dijet",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_WIDEBINS, '--set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_0JET_TT_QCDCR, ' --do_ss '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_BOOSTED_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet",     "dijet",  VAR_DIJET, ''),
    ("8",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ''),
    ("8",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ''),
    ("8",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ''),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_WIDEBINS, ''),
    ("8",   "dijet",      "dijet_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_lowM",      "dijet_lowM_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_highM",      "dijet_highM_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_lowboost",     "dijet_lowboost_qcd_cr",  VAR_DIJET, ' --do_ss '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_DIJET_WIDEBINS, ' --do_ss ')
  ]
  scheme_em = [
    ("15",   "0jet",    "0jet",  VAR_0JET_EM, '--set_alias="sel:mt_1<50" '),
    ("15",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("15",   "dijet",     "dijet",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
  }
  ANA = 'sm'
  
if SCHEME == 'cpsummer16_aachen':
  
  VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'
  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'sjdphi(12,-3.2,3.2)' 
  VAR_DIJET_THINBINS = 'sjdphi(20,-3.2,3.2)' # use this with ic cuts
  
  VAR_0JET_LT_WCR = 'mt_1[80,200]'
  VAR_BOOSTED_WCR = 'mt_1[80,200]'
  
  VAR_0JET_LT_QCDCR = 'm_vis(4,40,200)' 
  VAR_BOOSTED_LT_QCDCR = 'm_sv(4,40,200)'
  
  VAR_0JET_TT_QCDCR = 'm_sv[0,300]'
  VAR_BOOSTED_TT_QCDCR = 'm_sv[0,300]'
  
  VAR_DIJET_TT_QCD = 'sjdphi(1,-3.2,3.2)'

  scheme_et = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" --set_alias="w_shape:({w_shape}&&iso_1>0.1)" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')


  ]
  scheme_mt = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_0JET_LT_WCR, '--set_alias="sel:(mt_1>80&&mt_1<200)" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_BOOSTED_WCR, '--set_alias="sel:mt_1>80" --set_alias="boosted:({boosted}&&n_bjets==0)" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_BOOSTED_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" --set_alias="w_shape:({w_shape}&&iso_1>0.15)" '),
    ("21",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_highM",     "dijet_highM",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("21",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_0JET_TT_QCDCR, ' --do_ss '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_BOOSTED_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ''),
    ("8",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ''),
    ("8",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, ''),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ''),
    ("8",   "dijet_lowM",      "dijet_lowM_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_highM",      "dijet_highM_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_lowMjj",     "dijet_lowMjj_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_DIJET_TT_QCD, ' --do_ss ')
  ]
  #scheme_em = [
  #  ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
  #  ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
  #  ("19",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
  #  ("19",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
  #  ("19",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
  #  ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" ')
  #]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_lowM",     "dijet_lowM",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_highM",     "dijet_highM",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_lowMjj",     "dijet_lowMjj",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-35" ')
  ]

  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  ANA = 'sm'


if SCHEME == 'cpsummer16_2d':
  
  #VAR_0JET_LT = 'tau_decay_mode_2,m_vis[0,1,10],[0,60,65,70,75,80,85,90,95,100,105,110,400]'
  #VAR_0JET_EM = 'pt_2,m_vis[15,25,35],[0,50,55,60,65,70,75,80,85,90,95,100,400]'

  VAR_0JET_LT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'm_sv,sjdphi[0,80,100,115,130,150],(12,-3.2,3.2)' 
  #VAR_DIJET = 'm_sv,DCP,D0[0,80,100,115,130,150],[-1,0,1],(6,0,1)'
  #VAR_DIJET = 'm_sv,DCP,D0[0,80,100,115,130,150],[-1,-0.4,0.4,1],[0,0.25,0.5,0.75,1]'

 
  VAR_DIJET_TT_QCD = 'sjdphi(1,-3.2,3.2)' 
  
  VAR_WCR = 'mt_1[70,200]'
  
  VAR_0JET_LT_QCDCR = 'm_vis(4,40,200)' 
  VAR_LT_QCDCR = 'm_sv(4,40,200)'
  
  VAR_TT_QCDCR = 'm_sv[0,300]'

  scheme_et = [
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, ' --set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_WCR, '--set_alias="sel:mt_1>70" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" --set_alias="w_shape:({w_shape}&&iso_1>0.1)" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:mt_1<50"  '),
    ("24",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("24",   "dijet",    "wjets_dijet_cr",  VAR_WCR, '--set_alias="sel:(mt_1>70)"  '),
    ("24",   "dijet",    "antiiso_dijet_cr",  VAR_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" ')

  ]
  scheme_mt = [
    ("21",   "inclusive",    "met_cr",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', ' --set_alias="sel:mt_1<50" --set_alias="inclusive:(m_vis>50&&m_vis<80)" '),  
    ("21",   "0jet",    "0jet",  VAR_0JET_LT, '--set_alias="sel:mt_1<50" '),
    ("21",   "0jet",    "wjets_0jet_cr",  VAR_WCR, '--set_alias="sel:(mt_1>70)" '),
    ("21",   "0jet",    "antiiso_0jet_cr",  VAR_0JET_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" '),
    ("21",   "boosted", "boosted",  VAR_BOOSTED, '--set_alias="sel:mt_1<50" '),
    ("21",   "boosted", "wjets_boosted_cr",  VAR_WCR, '--set_alias="sel:mt_1>70" '),
    ("21",   "boosted",    "antiiso_boosted_cr",  VAR_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" --set_alias="w_shape:({w_shape}&&iso_1>0.15)" '),
    ("21",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:mt_1<50"  '),
    ("24",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("24",   "dijet",    "wjets_dijet_cr",  VAR_WCR, '--set_alias="sel:(mt_1>70)"  '),
    ("24",   "dijet",    "antiiso_dijet_cr",  VAR_LT_QCDCR, '--set_alias="sel:mt_1<50" --set_alias="baseline:(iso_1>0.15 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && (trg_singlemuon*(pt_1>23) || trg_mutaucross*(pt_1<23)))" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.15)" ')


  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_TT_QCDCR, ' --do_ss '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' '),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' '),
    ("8",   "dijet_lowboost",      "dijet_lowboost_qcd_cr",  VAR_TT_QCDCR, ' --do_ss '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_TT_QCDCR, ' --do_ss ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "ttbar",    "ttbar",  'm_sv[0,300]', ' --set_alias="sel:pzeta<-50" '),
  ]
  scheme_zmm = [
    ("8",   "inclusive",    "met_cr",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', '  --set_alias="inclusive:(m_vis>70&&m_vis<120)" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'


if SCHEME == 'cpsummer16_neww':
  

  VAR_0JET_LT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,40,60,70,80,90,100,110,120,130,150,200,250]' 
  
  VAR_DIJET = 'm_sv,sjdphi[0,80,100,115,130,150],(12,-3.2,3.2)' 
  #VAR_DIJET = 'm_sv,jdphi[0,80,100,115,130,150],(12,-3.2,3.2)'
  #VAR_DIJET = 'm_sv,(jeta_1>jeta_2)[0,80,100,115,130,150],(2,0,2)'
  #VAR_DIJET = 'm_sv,DCP,D0[0,80,100,115,130,150],[-1,0,1],(6,0,1)'
  #VAR_DIJET = 'm_sv,DCP,D0[0,80,100,115,130,150],[-1,0,1],(1,0,1)'
  #VAR_DIJET = 'm_sv,DCP,D0[0,80,100,115,130,150],[-1,1],(6,0,1)'

  #VAR_DIJET = 'm_sv,DCP,D0[0,80,100,115,130,150],[-1,-0.4,0.4,1],[0,0.25,0.5,0.75,1]'

 
  VAR_DIJET_TT_QCD = 'm_sv[0,300]'
  
  VAR_WCR = 'mt_1[70,200]'
  VAR_QCDCR = 'mt_1[0,70]'
  
  VAR_TT_QCDCR = 'm_sv[0,300]'

  scheme_et = [
    ("12",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" '),
    ("12",   "0jet",    "0jet_wjets_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("12",   "0jet",    "0jet_wjets_ss_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" --do_ss '),
    ("12",   "0jet",    "0jet_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss '),
    ("12",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" '),
    ("12",   "boosted",    "boosted_wjets_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("12",   "boosted",    "boosted_wjets_ss_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" --do_ss '),
    ("12",   "boosted",    "boosted_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss '),
    ("12",   "dijet_lowboost",    "dijet_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_lowboost",    "dijet_lowboost_wjets_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("12",   "dijet_lowboost",    "dijet_lowboost_wjets_ss_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" --do_ss '),
    ("12",   "dijet_lowboost",    "dijet_lowboost_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss '),
    ("23",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("23",   "dijet_boosted",    "dijet_boosted_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss ')
  ]
  scheme_mt = [
    ("12",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" '),
    ("12",   "0jet",    "0jet_wjets_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("12",   "0jet",    "0jet_wjets_ss_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" --do_ss '),
    ("12",   "0jet",    "0jet_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss '),
    ("12",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" '),
    ("12",   "boosted",    "boosted_wjets_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("12",   "boosted",    "boosted_wjets_ss_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" --do_ss '),
    ("12",   "boosted",    "boosted_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss '),
    ("12",   "dijet_lowboost",    "dijet_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" '),
    ("12",   "dijet_lowboost",    "dijet_lowboost_wjets_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" '),
    ("12",   "dijet_lowboost",    "dijet_lowboost_wjets_ss_cr",  VAR_WCR, ' --set_alias="sel:mt_1>70" --do_ss '),
    ("12",   "dijet_lowboost",    "dijet_lowboost_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss '),
    ("23",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" '),
    ("23",   "dijet_boosted",    "dijet_boosted_qcd_cr",  VAR_QCDCR, ' --set_alias="sel:mt_1<50" --do_ss ')

  ]
  scheme_tt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_TT, ''),
    ("8",   "0jet",    "0jet_qcd_cr",  VAR_TT_QCDCR, ' --set_alias="baseline:({tt_qcd_norm})" '),
    ("8",   "boosted", "boosted",  VAR_BOOSTED_TT, ''),
    ("8",   "boosted", "boosted_qcd_cr",  VAR_TT_QCDCR, ' --set_alias="baseline:({tt_qcd_norm})" '),
    ("8",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' '),
    ("8",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' '),
    ("8",   "dijet_lowboost",      "dijet_lowboost_qcd_cr",  VAR_TT_QCDCR, ' --set_alias="baseline:({tt_qcd_norm})" '),
    ("8",   "dijet_boosted",     "dijet_boosted_qcd_cr",  VAR_TT_QCDCR, ' --set_alias="baseline:({tt_qcd_norm})" ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "inclusive",    "ttbar",  'm_sv[0,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"'),
  ]
  scheme_zmm = [
    ("8",   "inclusive",    "met_cr",  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200]', '  --set_alias="inclusive:(m_vis>70&&m_vis<120)" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'cpsummer16_ff':
  
  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 

  #VAR_0JET_LT='m_vis(20,50,250)'
  #VAR_0JET_EM='m_vis(20,50,250)'
  #VAR_0JET_TT='m_vis(20,50,250)'

  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]' 

  VAR_DIJET = 'm_sv,sjdphi[50,80,100,115,130,150,200],(12,-3.2,3.2)'
  VAR_TT_TI_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_TT_LO_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_LO_HI='m_sv,sjdphi[50,80,100,115,150,200],(12,-3.2,3.2)'
  VAR_ET_TI_HI='m_sv,sjdphi[50,80,100,150,200],(12,-3.2,3.2)'

  scheme_et = [
    ("17",   "0jet",    "2016_0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "2016_boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "2016_dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "2016_dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:mt_1<50" --do_ff_systs ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "2016_0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "2016_boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "2016_dijet_loosemjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "2016_dijet_tightmjj_lowboost", VAR_DIJET, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --do_ff_systs ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "2016_0jet",  VAR_0JET_TT, ' --do_ff_systs '),
    ("17",   "boosted", "2016_boosted",  VAR_BOOSTED_TT, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_lowboost",    "2016_dijet_loosemjj_lowboost", VAR_DIJET, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "2016_dijet_tightmjj_lowboost", VAR_DIJET, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --do_ff_systs ')
  ]
  scheme_em = [
    ("19",   "0jet",    "2016_0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "boosted", "2016_boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '),
    ("19",   "inclusive",    "2016_ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"'),
    ("19",   "dijet_loosemjj_lowboost",     "2016_dijet_loosemjj_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_lowboost",     "2016_dijet_tightmjj_lowboost",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" ')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'

if SCHEME == 'cpsummer16_ff_newbins':
  
  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 

  #VAR_0JET_LT='m_vis(20,50,250)'
  #VAR_0JET_EM='m_vis(20,50,250)'
  #VAR_0JET_TT='m_vis(20,50,250)'

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
    ("17",   "0jet",    "2016_0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "boosted",    "2016_boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_loosemjj_lowboost",    "2016_dijet_loosemjj_lowboost", VAR_ET_LO_LO, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_tightmjj_lowboost",    "2016_dijet_tightmjj_lowboost", VAR_ET_TI_LO, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_ET_TI_HI, '--set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "2016_0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "boosted",    "2016_boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_loosemjj_lowboost",    "2016_dijet_loosemjj_lowboost", VAR_MT_LO_LO, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_tightmjj_lowboost",    "2016_dijet_tightmjj_lowboost", VAR_MT_TI_LO, ' --set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_DIJET, '--set_alias="sel:mt_1<50" --do_ff_systs --add_wt=wt_prefire ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "2016_0jet",  VAR_0JET_TT, ' --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "boosted", "2016_boosted",  VAR_BOOSTED_TT, ' --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_loosemjj_lowboost",    "2016_dijet_loosemjj_lowboost", VAR_TT_LO_LO, ' --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_tightmjj_lowboost",    "2016_dijet_tightmjj_lowboost", VAR_TT_TI_LO, ' --do_ff_systs --add_wt=wt_prefire '),
    ("17",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --do_ff_systs --add_wt=wt_prefire ')
  ]
  scheme_em = [
    ("19",   "0jet",    "2016_0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" --add_wt=wt_prefire '),
    ("19",   "boosted", "2016_boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" --add_wt=wt_prefire '),
    ("19",   "inclusive",    "2016_ttbar",  'm_sv[50,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)" --add_wt=wt_prefire '),
    ("19",   "dijet_loosemjj_lowboost",     "2016_dijet_loosemjj_lowboost",  VAR_EM_LO_LO, ' --set_alias="sel:pzeta>-10" --add_wt=wt_prefire '),
    ("19",   "dijet_loosemjj_boosted",     "2016_dijet_loosemjj_boosted",  VAR_EM_LO_HI, ' --set_alias="sel:pzeta>-10" --add_wt=wt_prefire '),
    ("19",   "dijet_tightmjj_lowboost",     "2016_dijet_tightmjj_lowboost",  VAR_EM_TI_LO, ' --set_alias="sel:pzeta>-10" --add_wt=wt_prefire '),
    ("19",   "dijet_tightmjj_boosted",     "2016_dijet_tightmjj_boosted",  VAR_DIJET, ' --set_alias="sel:pzeta>-10" --add_wt=wt_prefire ')
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

 
if SCHEME == 'cpsummer16_ff_jdphi':
  
  VAR_0JET_LT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 
  
  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,80,90,100,110,120,130,140,150,160,300]' 
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[0,70,80,90,100,110,120,130,150,200,250]' 

  VAR_DIJET = 'm_sv,fabs(jdphi)[0,80,100,115,130,150],(12,0,3.2)'

  VAR_TT_LO_HI='m_sv,fabs(jdphi)[0,80,110,145],(12,0,3.2)'
  VAR_TT_TI_HI='m_sv,fabs(jdphi)[0,80,100,115,130,150],(12,0,3.2)'

  VAR_ET_LO_HI='m_sv,fabs(jdphi)[0,80,90,115,150],(12,0,3.2)'

  VAR_EM_TI_LO='m_sv,fabs(jdphi)[0,80,100,115,130,150],(12,0,3.2)'

  VAR_ET_LO_LO='m_sv,fabs(jdphi)[0,80,100,115,130,170],(12,0,3.2)'


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
    ("17",   "dijet_lowboost",    "dijet_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_ET_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),

    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_ET_LO_LO, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_ET_2, '--set_alias="sel:mt_1<50" --do_ff_systs ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_lowboost",    "dijet_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),
  
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_TT, ' --do_ff_systs '),
    ("17",   "boosted", "boosted",  VAR_BOOSTED_TT, ' --do_ff_systs '),
    ("17",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_TT_2, ' --do_ff_systs '),

    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --do_ff_systs ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET_EM_1, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_EM_2, ' --set_alias="sel:pzeta>-10" '),
    ("19",   "inclusive",    "ttbar",  'm_sv[0,300]', ' --set_alias="sel:pzeta<-50" --set_alias="inclusive:(n_jets>0)"'),

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
  
if SCHEME == 'sync2016':
  
  VAR = 'm_vis(35,0,350)' 

  scheme_et = [
    ("12",   "inclusive",    "inclusive",  VAR, ' --set_alias="sel:mt_1<50" ')
  ]
  scheme_mt = [
    ("12",   "inclusive",    "inclusive",  VAR, ' --set_alias="sel:mt_1<50" ')

  ]
  scheme_tt = [
    ("8",   "inclusive",    "inclusive",  VAR, '  ')
  ]
  scheme_em = [
    ("19",   "inclusive",    "inclusive",  VAR, ' --set_alias="sel:pzeta>-35" '), 

  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm' 

if SCHEME == 'ff_fracs_pt': 
  VAR_MT_0JET='pt_2[20,30,40,50,60,80,100,150,250]'
  VAR_MT_1JET='pt_2[20,30,40,50,60,80,100,150,250]'
  VAR_MT_2JET='pt_2[20,30,40,50,60,80,100,150,250]'
  VAR_MT_BTAG='pt_2[20,30,40,50,60,80,100,150,250]'
 
  VAR_ET_0JET='pt_2[20,30,40,50,60,100,250]'
  VAR_ET_1JET='pt_2[20,30,40,50,60,80,100,250]' 
  VAR_ET_2JET='pt_2[20,30,40,50,60,80,100,150,250]'
  VAR_ET_BTAG='pt_2[20,30,40,50,60,80,100,250]'

  VAR_TT_0JET_1='pt_1[40,50,60,80,100,250]'
  VAR_TT_1JET_1='pt_1[40,50,60,80,100,250]'
  VAR_TT_2JET_1='pt_1[40,50,60,80,100,250]'
  VAR_TT_BTAG_1='pt_2[40,50,60,80,250]'

  VAR_TT_0JET_2='pt_2[40,50,60,80,100,250]'
  VAR_TT_1JET_2='pt_2[40,50,60,80,100,250]'
  VAR_TT_2JET_2='pt_2[40,50,60,80,100,250]'
  VAR_TT_BTAG_2='pt_2[40,50,60,80,250]' 

  scheme_mt = [
    ("0",   "0jet", "0jet",  VAR_MT_0JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "1jet", "1jet",  VAR_MT_1JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "2jet", "2jet",  VAR_MT_2JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "btag", "btag",  VAR_MT_BTAG, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "0jet", "0jet_highmt",  VAR_MT_0JET, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "1jet", "1jet_highmt",  VAR_MT_1JET, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "2jet", "2jet_highmt",  VAR_MT_2JET, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "btag", "btag_highmt",  VAR_MT_BTAG, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "0jet", "0jet_ss",  VAR_MT_0JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss '),
    ("0",   "1jet", "1jet_ss",  VAR_MT_1JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss '),
    ("0",   "2jet", "2jet_ss",  VAR_MT_2JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss '),
    ("0",   "btag", "btag_ss",  VAR_MT_BTAG, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss ')
  ]
  scheme_et = [
    ("0",   "0jet", "0jet",  VAR_ET_0JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "1jet", "1jet",  VAR_ET_1JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "2jet", "2jet",  VAR_ET_2JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "btag", "btag",  VAR_ET_BTAG, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "0jet", "0jet_highmt",  VAR_ET_0JET, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "1jet", "1jet_highmt",  VAR_ET_1JET, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "2jet", "2jet_highmt",  VAR_ET_2JET, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "btag", "btag_highmt",  VAR_ET_BTAG, ' --set_alias="sel:mt_1>70" --set_alias="baseline:({baseline_aisotau})" '),
    ("0",   "0jet", "0jet_ss",  VAR_ET_0JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss '),
    ("0",   "1jet", "1jet_ss",  VAR_ET_1JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss '),
    ("0",   "2jet", "2jet_ss",  VAR_ET_2JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss '),
    ("0",   "btag", "btag_ss",  VAR_ET_BTAG, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --do_ss ')
  ]
  scheme_tt = [
    ("0",   "0jet", "0jet_1",  VAR_TT_0JET_1, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "1jet", "1jet_1",  VAR_TT_1JET_1, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "2jet", "2jet_1",  VAR_TT_2JET_1, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "btag", "btag_1",  VAR_TT_BTAG_1, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("0",   "0jet", "0jet_ss_1",  VAR_TT_0JET_1, ' --set_alias="baseline:({baseline_aisotau1})" --do_ss '),
    ("0",   "1jet", "1jet_ss_1",  VAR_TT_1JET_1, ' --set_alias="baseline:({baseline_aisotau1})" --do_ss '),
    ("0",   "2jet", "2jet_ss_1",  VAR_TT_2JET_1, ' --set_alias="baseline:({baseline_aisotau1})" --do_ss '),
    ("0",   "btag", "btag_ss_1",  VAR_TT_BTAG_1, ' --set_alias="baseline:({baseline_aisotau1})" --do_ss '),

    ("0",   "0jet", "0jet_2",  VAR_TT_0JET_2, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "1jet", "1jet_2",  VAR_TT_1JET_2, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "2jet", "2jet_2",  VAR_TT_2JET_2, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "btag", "btag_2",  VAR_TT_BTAG_2, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("0",   "0jet", "0jet_ss_2",  VAR_TT_0JET_2, ' --set_alias="baseline:({baseline_aisotau2})" --do_ss '),
    ("0",   "1jet", "1jet_ss_2",  VAR_TT_1JET_2, ' --set_alias="baseline:({baseline_aisotau2})" --do_ss '),
    ("0",   "2jet", "2jet_ss_2",  VAR_TT_2JET_2, ' --set_alias="baseline:({baseline_aisotau2})" --do_ss '),
    ("0",   "btag", "btag_ss_2",  VAR_TT_BTAG_2, ' --set_alias="baseline:({baseline_aisotau2})" --do_ss ')
  ]

  scheme_em = []
  scheme_zmm = []

  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'tt' : 'tt_default'
  }
  ANA = 'sm'

  
if SCHEME == 'ff_fracs':
  for ch in extra_channel:
    extra_channel[ch]+=' --syst_eff_b=CMS_eff_b_13TeV --syst_fake_b=CMS_fake_b_13TeV '
    
 # VAR_MT = 'm_vis[0,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240]'
 # VAR_MT_0JET = 'm_vis[0,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,200,240]'
 # VAR_MT_BOOSTED_LOW = 'm_vis[0,40,50,60,70,80,90,100,110,120,130,140,160,180,200,240]'
 # VAR_MT_BOOSTED_HIGH = 'm_vis[0,40,60,80,100,120,140,160,200,240]'
 # VAR_MT_DIJET_LOW = 'm_vis[0,40,60,80,120,160,200,240]'
 # VAR_MT_DIJET_HIGH = 'm_vis[0,50,100,150,240]'
 # VAR_MT_BTAG='m_vis[0,40,60,80,100,120,140,160,200,240]'
 # 
 # VAR_ET = 'm_vis[0,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240]'
 # VAR_ET_0JET = 'm_vis[0,50,60,70,80,90,100,110,120,130,140,160,180,200,240]'
 # VAR_ET_BOOSTED_LOW = 'm_vis[0,40,50,60,70,80,90,100,110,120,130,140,160,180,200,240]'
 # VAR_ET_BOOSTED_HIGH = 'm_vis[0,40,60,80,100,120,140,160,200,240]'
 # VAR_ET_DIJET_LOW = 'm_vis[0,60,80,120,160,240]'
 # VAR_ET_DIJET_HIGH = 'm_vis[0,75,150,240]'
 # VAR_ET_BTAG='m_vis[0,40,60,80,100,120,140,160,200,240]'

  VAR_MT = 'mt_1(28,0,140)'
  VAR_MT_0JET = 'mt_1(28,0,140)'
  VAR_MT_BOOSTED_LOW = 'mt_1(28,0,140)'
  VAR_MT_BOOSTED_HIGH = 'mt_1(14,0,140)'
  VAR_MT_DIJET_LOW = 'mt_1(14,0,140)'
  VAR_MT_DIJET_HIGH = 'mt_1(7,0,140)'
  VAR_MT_BTAG = 'mt_1(28,0,140)'

  VAR_ET = 'mt_1(28,0,140)'
  VAR_ET_0JET = 'mt_1(28,0,140)'
  VAR_ET_BOOSTED_LOW = 'mt_1(28,0,140)'
  VAR_ET_BOOSTED_HIGH = 'mt_1(14,0,140)'
  VAR_ET_DIJET_LOW = 'mt_1(14,0,140)'
  VAR_ET_DIJET_HIGH = 'mt_1(7,0,140)'
  VAR_ET_BTAG='mt_1(28,0,140)'

  
  VAR_TT = 'm_vis[0,40,60,80,100,110,120,130,140,150,160,170,180,190,200,220,240]'
  VAR_TT_0JET = 'm_vis[0,90,100,110,120,130,140,150,160,170,180,200,220,240]'
  VAR_TT_BOOSTED_LOW = 'm_vis[0,40,60,80,100,110,120,130,140,150,160,170,180,200,220,240]'
  VAR_TT_BOOSTED_HIGH = 'm_vis[0,40,60,80,100,120,160,200,240]'
  VAR_TT_DIJET_LOW = 'm_vis[0,60,100,140,180,240]'
  VAR_TT_DIJET_HIGH = 'm_vis[0,50,100,150,240]'
  VAR_TT_BTAG='m_vis[0,40,80,120,160,200,240]'
  

  VAR = 'm_vis[0,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240]'
  

  scheme_mt = [
    ("12",   "inclusive", "inclusive",  VAR_MT, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("12",   "0jet", "0jet",  VAR_MT_0JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("12",   "boosted", "boosted_low",  VAR_MT_BOOSTED_LOW, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --set_alias="boosted:({boosted}&&pt_tt<100)" '),
    ("12",   "boosted", "boosted_high",  VAR_MT_BOOSTED_HIGH, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --set_alias="boosted:({boosted}&&pt_tt>100)" '),
    ("12",   "dijet_lowboost", "dijet_lowboost",  VAR_MT_DIJET_LOW, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("23",   "dijet_boosted", "dijet_boosted",  VAR_MT_DIJET_HIGH, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("16",   "btag", "btag",  VAR_MT_BTAG, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" ')
  ]
  scheme_et = [
    ("12",   "inclusive", "inclusive",  VAR_ET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("12",   "0jet", "0jet",  VAR_ET_0JET, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("12",   "boosted", "boosted_low",  VAR_ET_BOOSTED_LOW, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --set_alias="boosted:({boosted}&&pt_tt<100)" '),
    ("12",   "boosted", "boosted_high",  VAR_ET_BOOSTED_HIGH, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" --set_alias="boosted:({boosted}&&pt_tt>100)" '),
    ("12",   "dijet_lowboost", "dijet_lowboost",  VAR_ET_DIJET_LOW, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("23",   "dijet_boosted", "dijet_boosted",  VAR_ET_DIJET_HIGH, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" '),
    ("16",   "btag", "btag",  VAR_ET_BTAG, ' --set_alias="sel:mt_1<50" --set_alias="baseline:({baseline_aisotau})" ')
  ]
  scheme_tt = [
    ("8",   "inclusive", "inclusive_1",  VAR_TT, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("8",   "0jet", "0jet_1",  VAR_TT_0JET, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("8",   "boosted", "boosted_low_1",  VAR_TT_BOOSTED_LOW, ' --set_alias="baseline:({baseline_aisotau1})" --set_alias="boosted:({boosted}&&pt_tt<100)" '),
    ("8",   "boosted", "boosted_high_1",  VAR_TT_BOOSTED_HIGH, ' --set_alias="baseline:({baseline_aisotau1})" --set_alias="boosted:({boosted}&&pt_tt>100)" '),
    #("8",   "btag", "btag_1",  VAR_TT_BTAG, ' --set_alias="baseline:({baseline_aisotau1})" ')
    ("8",   "dijet_lowboost", "dijet_lowboost_1",  VAR_TT_DIJET_LOW, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("8",   "dijet_boosted", "dijet_boosted_1",  VAR_TT_DIJET_HIGH, ' --set_alias="baseline:({baseline_aisotau1})" '),
    ("8",   "inclusive", "inclusive_2",  VAR_TT, '--set_alias="baseline:({baseline_aisotau2})" '),
    ("8",   "0jet", "0jet_2",  VAR_TT_0JET, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("8",   "boosted", "boosted_low_2",  VAR_TT_BOOSTED_LOW, ' --set_alias="baseline:({baseline_aisotau2})" --set_alias="boosted:({boosted}&&pt_tt<100)" '),
    ("8",   "boosted", "boosted_high_2",  VAR_TT_BOOSTED_HIGH, ' --set_alias="baseline:({baseline_aisotau2})" --set_alias="boosted:({boosted}&&pt_tt>100)" '),
    ("8",   "dijet_lowboost", "dijet_lowboost_2",  VAR_TT_DIJET_LOW, ' --set_alias="baseline:({baseline_aisotau2})" '),
    ("8",   "dijet_boosted", "dijet_boosted_2",  VAR_TT_DIJET_HIGH, ' --set_alias="baseline:({baseline_aisotau2})" ')
    #("8",   "btag", "btag_2",  VAR_TT_BTAG, ' --set_alias="baseline:({baseline_aisotau2})" ')
  ]
  
  scheme_em = []
  scheme_zmm = []

  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'tt' : 'tt_default'
  }
  ANA = 'sm'  

if SCHEME == 'mlcpsummer16_2d':
    # define signal regions for each training

    # LOWMJJ
    VAR_MT_LOWMJJ_FAKE =    'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_MT_LOWMJJ_GGH =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_MT_LOWMJJ_QQH =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_MT_LOWMJJ_MISC =    'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_MT_LOWMJJ_TT =      'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_LOWMJJ_ZLL =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_MT_LOWMJJ_ZTT =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'

    VAR_ET_LOWMJJ_FAKE =    'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_ET_LOWMJJ_GGH =     'IC_lowMjj_Jun4_max_score[0.0,0.4,0.5,0.6,0.7,1.0]'
    VAR_ET_LOWMJJ_QQH =     'IC_lowMjj_Jun4_max_score[0.0,0.2,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_ET_LOWMJJ_MISC =    'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,1.0]'
    VAR_ET_LOWMJJ_TT =      'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_ET_LOWMJJ_ZLL =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_ET_LOWMJJ_ZTT =     'IC_lowMjj_Jun4_max_score[0.0,0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'

    VAR_TT_LOWMJJ_GGH =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,1.0]'
    VAR_TT_LOWMJJ_QQH =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_TT_LOWMJJ_MISC =    'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_TT_LOWMJJ_QCD =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_TT_LOWMJJ_ZTT =     'IC_lowMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    VAR_EM_LOWMJJ_GGH =     'IC_lowMjj_Jun15_max_score[0.0,0.3,0.4,0.5,1.0]'
    VAR_EM_LOWMJJ_QQH =     'IC_lowMjj_Jun15_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_EM_LOWMJJ_MISC =    'IC_lowMjj_Jun15_max_score[0.0,0.4,0.5,0.6,0.7,1.0]'
    VAR_EM_LOWMJJ_TT =      'IC_lowMjj_Jun15_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_EM_LOWMJJ_QCD =     'IC_lowMjj_Jun15_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_EM_LOWMJJ_ZTT =     'IC_lowMjj_Jun15_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'

    # HIGHMJJ

    VAR_MT_HIGHMJJ_GGH =     'IC_highMjj_July05_1_max_score,sjdphi[0.0,0.5],(12,-3.2,3.2)'
    VAR_MT_HIGHMJJ_QQH =     'IC_highMjj_July05_1_max_score,sjdphi[0.0,0.4,0.5,0.7],(12,-3.2,3.2)'
    VAR_MT_HIGHMJJ_MISC =    'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_MT_HIGHMJJ_TT =      'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_MT_HIGHMJJ_FAKE =    'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_MT_HIGHMJJ_ZTT =     'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'

    VAR_ET_HIGHMJJ_GGH =     'IC_highMjj_July05_1_max_score,sjdphi[0.0,0.4,0.7],(12,-3.2,3.2)'
    VAR_ET_HIGHMJJ_QQH =     'IC_highMjj_July05_1_max_score,sjdphi[0.0,0.4,0.5,0.7],(12,-3.2,3.2)'
    VAR_ET_HIGHMJJ_MISC =    'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_ET_HIGHMJJ_TT =      'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_ET_HIGHMJJ_FAKE =    'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_ET_HIGHMJJ_ZTT =     'IC_highMjj_July05_1_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'

    VAR_TT_HIGHMJJ_GGH =     'IC_highMjj_July05_1_max_score,sjdphi[0.0,0.6],(12,-3.2,3.2)'
    VAR_TT_HIGHMJJ_QQH =     'IC_highMjj_July05_1_max_score,sjdphi[0.0,0.5,0.6],(12,-3.2,3.2)'
    VAR_TT_HIGHMJJ_MISC =    'IC_highMjj_July05_1_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_TT_HIGHMJJ_QCD =     'IC_highMjj_July05_1_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_TT_HIGHMJJ_ZTT =     'IC_highMjj_July05_1_max_score[0.0,0.4,0.5,0.6,0.7,0.8,1.0]'

    VAR_EM_HIGHMJJ_GGH =     'IC_highMjj_Jun4_max_score,sjdphi[0.0,0.65],(12,-3.2,3.2)'
    VAR_EM_HIGHMJJ_QQH =     'IC_highMjj_Jun4_max_score,sjdphi[0.0,0.5,0.6,0.7],(12,-3.2,3.2)'
    VAR_EM_HIGHMJJ_MISC =    'IC_highMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_EM_HIGHMJJ_TT =      'IC_highMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_EM_HIGHMJJ_QCD =     'IC_highMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_EM_HIGHMJJ_ZTT =     'IC_highMjj_Jun4_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    # define control regions
    # QCD CR for both trainings for et,mt,tt
    # W CR for both trainings for et,mt

    VAR_LT_QCDCR =     'm_vis(4,40,200)'
    VAR_LT_WCR =       'mt_1[80,200]'

    scheme_et = [
        ("21",   "fake_lowMjj",   "fake_lowMjj", VAR_ET_LOWMJJ_FAKE, ' --set_alias="sel:mt_1<50" '),
        ("21",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_ET_LOWMJJ_QQH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_ET_LOWMJJ_GGH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "misc_lowMjj",   "misc_lowMjj", VAR_ET_LOWMJJ_MISC, ' --set_alias="sel:mt_1<50" '),
        ("21",   "tt_lowMjj",     "tt_lowMjj",   VAR_ET_LOWMJJ_TT,   ' --set_alias="sel:mt_1<50" '),
        ("21",   "zll_lowMjj",    "zll_lowMjj",  VAR_ET_LOWMJJ_ZLL,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "ztt_lowMjj",    "ztt_lowMjj",  VAR_ET_LOWMJJ_ZTT,   ' --set_alias="sel:mt_1<50" '),

        ("21",   "fake_highMjj",   "fake_highMjj", VAR_ET_HIGHMJJ_FAKE, ' --set_alias="sel:mt_1<50" '),
        ("21",   "qqh_highMjj",    "qqh_highMjj",  VAR_ET_HIGHMJJ_QQH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "ggh_highMjj",    "ggh_highMjj",  VAR_ET_HIGHMJJ_GGH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "misc_highMjj",   "misc_highMjj", VAR_ET_HIGHMJJ_MISC, ' --set_alias="sel:mt_1<50" '),
        ("21",   "tt_highMjj",     "tt_highMjj",   VAR_ET_HIGHMJJ_TT,   ' --set_alias="sel:mt_1<50" '),
        ("21",   "ztt_highMjj",    "ztt_highMjj",  VAR_ET_HIGHMJJ_ZTT,   ' --set_alias="sel:mt_1<50" '),


        # CRs - using the same cuts as in the other schemes

        # ("21",   "JHU_qcd",    "JHU_qcd_cr",  VAR_JHU_LT_QCDCR, ' --set_alias="sel:mt_1<80" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),
        # ("21",   "powheg_qcd",    "powheg_qcd_cr",  VAR_POWHEG_LT_QCDCR, ' --set_alias="sel:mt_1<80" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),

        # ("21",   "JHU_w",    "JHU_w_cr",  VAR_JHU_LT_WCR, ' --set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),
        # ("21",   "powheg_w",    "powheg_w_cr",  VAR_POWHEG_LT_WCR, ' --set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),

    ]

    scheme_mt = [
        ("21",   "fake_lowMjj",   "fake_lowMjj", VAR_MT_LOWMJJ_FAKE, ' --set_alias="sel:mt_1<50" '),
        ("21",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_MT_LOWMJJ_QQH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_MT_LOWMJJ_GGH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "misc_lowMjj",   "misc_lowMjj", VAR_MT_LOWMJJ_MISC, ' --set_alias="sel:mt_1<50" '),
        ("21",   "tt_lowMjj",     "tt_lowMjj",   VAR_MT_LOWMJJ_TT,   ' --set_alias="sel:mt_1<50" '),
        ("21",   "zll_lowMjj",    "zll_lowMjj",  VAR_MT_LOWMJJ_ZLL,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "ztt_lowMjj",    "ztt_lowMjj",  VAR_MT_LOWMJJ_ZTT,   ' --set_alias="sel:mt_1<50" '),

        ("21",   "fake_highMjj",   "fake_highMjj", VAR_MT_HIGHMJJ_FAKE, ' --set_alias="sel:mt_1<50" '),
        ("21",   "qqh_highMjj",    "qqh_highMjj",  VAR_MT_HIGHMJJ_QQH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "ggh_highMjj",    "ggh_highMjj",  VAR_MT_HIGHMJJ_GGH,  ' --set_alias="sel:mt_1<50" '),
        ("21",   "misc_highMjj",   "misc_highMjj", VAR_MT_HIGHMJJ_MISC, ' --set_alias="sel:mt_1<50" '),
        ("21",   "tt_highMjj",     "tt_highMjj",   VAR_MT_HIGHMJJ_TT,   ' --set_alias="sel:mt_1<50" '),
        ("21",   "ztt_highMjj",    "ztt_highMjj",  VAR_MT_HIGHMJJ_ZTT,   ' --set_alias="sel:mt_1<50" '),

        # CRs - using the same cuts as in the other schemes

        # ("8",   "qcd",    "qcd_cr",  VAR_LT_QCDCR, ' --set_alias="sel:mt_1<80" --set_alias="baseline:(iso_1>0.1 && iso_1<0.3 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0 && pt_2>30 && trg_singleelectron)" --set_alias="qcd_shape:({qcd_shape}&&iso_1>0.1)" '),

        # ("21",   "w",    "w_cr",  VAR_LT_WCR, ' --set_alias="sel:mt_1>80" --set_alias="0jet:({0jet}&&n_bjets==0)" '),

    ]

    scheme_tt = [
        ("8",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_TT_LOWMJJ_GGH,  '  '),
        ("8",   "misc_lowMjj",   "misc_lowMjj", VAR_TT_LOWMJJ_MISC, '  '),
        ("8",   "qcd_lowMjj",    "qcd_lowMjj",  VAR_TT_LOWMJJ_QCD,  '  '),
        ("8",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_TT_LOWMJJ_QQH,  '  '),
        ("8",   "ztt_lowMjj",    "ztt_lowMjj",  VAR_TT_LOWMJJ_ZTT,  '  '),

        ("8",   "ggh_highMjj",    "ggh_highMjj",  VAR_TT_HIGHMJJ_GGH,  '  '),
        ("8",   "misc_highMjj",   "misc_highMjj", VAR_TT_HIGHMJJ_MISC, '  '),
        ("8",   "qcd_highMjj",    "qcd_highMjj",  VAR_TT_HIGHMJJ_QCD,  '  '),
        ("8",   "qqh_highMjj",    "qqh_highMjj",  VAR_TT_HIGHMJJ_QQH,  '  '),
        ("8",   "ztt_highMjj",    "ztt_highMjj",  VAR_TT_HIGHMJJ_ZTT,  '  '),

        # CRs - using the same cuts as in the other schemes

        # ("8",   "JHU_qcd",    "JHU_qcd_cr",  VAR_JHU_TT_QCDCR, ' --do_ss '),
        # ("8",   "powheg_qcd",    "powheg_qcd_cr",  VAR_POWHEG_TT_QCDCR, ' --do_ss '),
    ]
    scheme_em = [

        ("19",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_EM_LOWMJJ_GGH,  '  --set_alias="sel:pzeta>-35" '),
        ("19",   "misc_lowMjj",   "misc_lowMjj", VAR_EM_LOWMJJ_MISC, '  --set_alias="sel:pzeta>-35" '),
        ("19",   "qcd_lowMjj",    "qcd_lowMjj",  VAR_EM_LOWMJJ_QCD,  '  --set_alias="sel:pzeta>-35" '),
        ("19",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_EM_LOWMJJ_QQH,  '  --set_alias="sel:pzeta>-35" '),
        ("19",   "tt_lowMjj",     "tt_lowMjj",   VAR_EM_LOWMJJ_TT,   '  --set_alias="sel:pzeta>-35" '),
        ("19",   "ztt_lowMjj",    "ztt_lowMjj",  VAR_EM_LOWMJJ_ZTT,  '  --set_alias="sel:pzeta>-35" '),

        ("19",   "ggh_highMjj",    "ggh_highMjj",  VAR_EM_HIGHMJJ_GGH,  ' --set_alias="sel:pzeta>-35" '),
        ("19",   "misc_highMjj",   "misc_highMjj", VAR_EM_HIGHMJJ_MISC, ' --set_alias="sel:pzeta>-35" '),
        ("19",   "qcd_highMjj",    "qcd_highMjj",  VAR_EM_HIGHMJJ_QCD,  ' --set_alias="sel:pzeta>-35" '),
        ("19",   "qqh_highMjj",    "qqh_highMjj",  VAR_EM_HIGHMJJ_QQH,  ' --set_alias="sel:pzeta>-35" '),
        ("19",   "tt_highMjj",     "tt_highMjj",   VAR_EM_HIGHMJJ_TT,   ' --set_alias="sel:pzeta>-35" '),
        ("19",   "ztt_highMjj",    "ztt_highMjj",  VAR_EM_HIGHMJJ_ZTT,  ' --set_alias="sel:pzeta>-35" '),

    ]
    bkg_schemes = {
      'et' : 'et_default',
      'mt' : 'mt_with_zmm',
      'em' : 'em_default',
      'tt' : 'tt_default'
    }
    ANA = 'sm'

if SCHEME == 'mlcpsummer16_ff':
    # define signal regions for each training

    # LowMjj
    # with embedding and FF
    VAR_TT_LOWMJJ_GGH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_TT_LOWMJJ_QQH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_TT_LOWMJJ_JETFAKES = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_TT_LOWMJJ_ZTTEMBED = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    VAR_MT_LOWMJJ_JETFAKES = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_LOWMJJ_GGH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_MT_LOWMJJ_QQH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_LOWMJJ_TT =       'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_LOWMJJ_ZLL =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_LOWMJJ_ZTTEMBED = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    VAR_ET_LOWMJJ_JETFAKES = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_ET_LOWMJJ_GGH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_ET_LOWMJJ_QQH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_ET_LOWMJJ_TT =       'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_ET_LOWMJJ_ZLL =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_ET_LOWMJJ_ZTTEMBED = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    VAR_EM_LOWMJJ_GGH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,1.0]'
    VAR_EM_LOWMJJ_QQH =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,1.0]'
    VAR_EM_LOWMJJ_TT =       'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_EM_LOWMJJ_QCD =      'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_EM_LOWMJJ_ZTTEMBED = 'IC_lowMjj_Oct05_max_score[0.0,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    # HighMjj

    # VAR_MT_HIGHMJJ_GGH_LOOSE =       'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.4,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    # VAR_MT_HIGHMJJ_GGH_TIGHT =       'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.4,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    # latest one
    # VAR_MT_HIGHMJJ_GGH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.4,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    # VAR_MT_HIGHMJJ_GGH_BOOSTED =     'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    # VAR_MT_HIGHMJJ_QQH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    # VAR_MT_HIGHMJJ_QQH_BOOSTED =     'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'

    VAR_MT_HIGHMJJ_GGH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.4,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    VAR_MT_HIGHMJJ_QQH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    VAR_MT_HIGHMJJ_TT =              'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_HIGHMJJ_JETFAKES =        'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_MT_HIGHMJJ_ZTTEMBED =        'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    # latest one
    # VAR_ET_HIGHMJJ_GGH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    # VAR_ET_HIGHMJJ_GGH_BOOSTED =     'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    # VAR_ET_HIGHMJJ_QQH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    # VAR_ET_HIGHMJJ_QQH_BOOSTED =     'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.8],(12,-3.2,3.2)'

    VAR_ET_HIGHMJJ_GGH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    VAR_ET_HIGHMJJ_QQH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    VAR_ET_HIGHMJJ_TT =              'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_ET_HIGHMJJ_JETFAKES =        'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_ET_HIGHMJJ_ZTTEMBED =        'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_ET_HIGHMJJ_ZLL =             'IC_highMjj_Oct05_max_score[0.0,1.0]'


    # VAR_TT_HIGHMJJ_GGH_LOOSE = 'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.85],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_GGH_TIGHT = 'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.75,0.9],(12,-3.2,3.2)'
    # latest ones
    # VAR_TT_HIGHMJJ_GGH =         'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.7],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_GGH_BOOSTED = 'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.8,0.9],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_QQH =         'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.8],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_QQH_BOOSTED = 'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.85],(12,-3.2,3.2)'

    # VAR_TT_HIGHMJJ_GGH_LOWBOOST = 'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.7],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_GGH_BOOSTED =  'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.8],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_QQH_LOWBOOST = 'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.8],(12,-3.2,3.2)'
    # VAR_TT_HIGHMJJ_QQH_BOOSTED =  'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.7,0.9],(12,-3.2,3.2)'

    VAR_TT_HIGHMJJ_GGH =         'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.85],(12,-3.2,3.2)'
    VAR_TT_HIGHMJJ_QQH =         'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.8,0.9],(12,-3.2,3.2)'
    VAR_TT_HIGHMJJ_JETFAKES =    'IC_highMjj_Oct05_max_score[0.0,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_TT_HIGHMJJ_ZTTEMBED =    'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    # VAR_EM_HIGHMJJ_GGH_LOOSE =       'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7],(12,-3.2,3.2)'
    # VAR_EM_HIGHMJJ_GGH_TIGHT =       'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.7,0.8],(12,-3.2,3.2)'
    # latest one
    # VAR_EM_HIGHMJJ_GGH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.7],(12,-3.2,3.2)'
    # VAR_EM_HIGHMJJ_GGH_BOOSTED =     'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    # VAR_EM_HIGHMJJ_QQH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.6,0.9],(12,-3.2,3.2)'
    # VAR_EM_HIGHMJJ_QQH_BOOSTED =     'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.8],(12,-3.2,3.2)'

    VAR_EM_HIGHMJJ_GGH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8],(12,-3.2,3.2)'
    VAR_EM_HIGHMJJ_QQH =             'IC_highMjj_Oct05_max_score,sjdphi[0.0,0.5,0.6,0.7,0.8,0.9],(12,-3.2,3.2)'
    VAR_EM_HIGHMJJ_TT =              'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'
    VAR_EM_HIGHMJJ_ZTTEMBED =        'IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]'

    scheme_et = [
        ("17",   "jetFakes_lowMjj",   "jetFakes_lowMjj", VAR_ET_LOWMJJ_JETFAKES,    ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_ET_LOWMJJ_QQH,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_ET_LOWMJJ_GGH,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "tt_lowMjj",     "tt_lowMjj",   VAR_ET_LOWMJJ_TT,      ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "zll_lowMjj",    "zll_lowMjj",  VAR_ET_LOWMJJ_ZLL,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "zttEmbed_lowMjj",    "zttEmbed_lowMjj",  VAR_ET_LOWMJJ_ZTTEMBED,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),

        ("17",   "ggh_highMjj",    "ggh_highMjj",  VAR_ET_HIGHMJJ_GGH,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "qqh_highMjj",    "qqh_highMjj",  VAR_ET_HIGHMJJ_QQH,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "jetFakes_highMjj",   "jetFakes_highMjj", VAR_ET_HIGHMJJ_JETFAKES, ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "tt_highMjj",     "tt_highMjj",   VAR_ET_HIGHMJJ_TT,   ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "zttEmbed_highMjj",    "zttEmbed_highMjj",  VAR_ET_HIGHMJJ_ZTTEMBED,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "zll_highMjj",    "zll_highMjj",  VAR_ET_HIGHMJJ_ZLL,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),

    ]

    scheme_mt = [
        ("17",   "jetFakes_lowMjj",   "jetFakes_lowMjj", VAR_MT_LOWMJJ_JETFAKES,    ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_MT_LOWMJJ_QQH,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_MT_LOWMJJ_GGH,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "tt_lowMjj",     "tt_lowMjj",   VAR_MT_LOWMJJ_TT,      ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "zll_lowMjj",    "zll_lowMjj",  VAR_MT_LOWMJJ_ZLL,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "zttEmbed_lowMjj",    "zttEmbed_lowMjj",  VAR_MT_LOWMJJ_ZTTEMBED,     ' --do_ff_systs --set_alias="sel:mt_1<50" '),

        ("17",   "ggh_highMjj",    "ggh_highMjj",  VAR_MT_HIGHMJJ_GGH,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "qqh_highMjj",    "qqh_highMjj",  VAR_MT_HIGHMJJ_QQH,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "jetFakes_highMjj",   "jetFakes_highMjj", VAR_MT_HIGHMJJ_JETFAKES, ' --do_ff_systs --set_alias="sel:mt_1<50" '),
        ("17",   "tt_highMjj",     "tt_highMjj",   VAR_MT_HIGHMJJ_TT,   ' --do_ff_systs --set_alias="sel:mt_1<50"  '),
        ("17",   "zttEmbed_highMjj",    "zttEmbed_highMjj",  VAR_MT_HIGHMJJ_ZTTEMBED,  ' --do_ff_systs --set_alias="sel:mt_1<50"  '),

    ]

    scheme_tt = [
        ("17",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_TT_LOWMJJ_GGH,  ' --do_ff_systs  '),
        ("17",   "jetFakes_lowMjj",    "jetFakes_lowMjj",  VAR_TT_LOWMJJ_JETFAKES,  ' --do_ff_systs  '),
        ("17",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_TT_LOWMJJ_QQH,  ' --do_ff_systs  '),
        ("17",   "zttEmbed_lowMjj",    "zttEmbed_lowMjj",  VAR_TT_LOWMJJ_ZTTEMBED,  ' --do_ff_systs  '),

        ("17",   "ggh_highMjj",    "ggh_highMjj",  VAR_TT_HIGHMJJ_GGH,  ' --do_ff_systs  '),
        ("17",   "qqh_highMjj",    "qqh_highMjj",  VAR_TT_HIGHMJJ_QQH,  ' --do_ff_systs  '),
        ("17",   "jetFakes_highMjj",    "jetFakes_highMjj",  VAR_TT_HIGHMJJ_JETFAKES,  ' --do_ff_systs  '),
        ("17",   "zttEmbed_highMjj",    "zttEmbed_highMjj",  VAR_TT_HIGHMJJ_ZTTEMBED,  ' --do_ff_systs  '),

    ]
    scheme_em = [

        ("19",   "ggh_lowMjj",    "ggh_lowMjj",  VAR_EM_LOWMJJ_GGH,  '  --set_alias="sel:pzeta>-35" '),
        ("19",   "qcd_lowMjj",    "qcd_lowMjj",  VAR_EM_LOWMJJ_QCD,  '  --set_alias="sel:pzeta>-35" '),
        ("19",   "qqh_lowMjj",    "qqh_lowMjj",  VAR_EM_LOWMJJ_QQH,  '  --set_alias="sel:pzeta>-35" '),
        ("19",   "tt_lowMjj",     "tt_lowMjj",   VAR_EM_LOWMJJ_TT,   '  --set_alias="sel:pzeta>-35" '),
        ("19",   "zttEmbed_lowMjj",  "zttEmbed_lowMjj",  VAR_EM_LOWMJJ_ZTTEMBED,  '  --set_alias="sel:pzeta>-35" '),

        ("19",   "ggh_highMjj",    "ggh_highMjj",  VAR_EM_HIGHMJJ_GGH,  ' --set_alias="sel:pzeta>-10" '),
        ("19",   "qqh_highMjj",    "qqh_highMjj",  VAR_EM_HIGHMJJ_QQH,  ' --set_alias="sel:pzeta>-10" '),
        ("19",   "tt_highMjj",     "tt_highMjj",   VAR_EM_HIGHMJJ_TT,   ' --set_alias="sel:pzeta>-10" '),
        ("19",   "zttEmbed_highMjj",    "zttEmbed_highMjj",  VAR_EM_HIGHMJJ_ZTTEMBED,  ' --set_alias="sel:pzeta>-10" '),

    ]
    bkg_schemes = {
      'et' : 'et_default',
      'mt' : 'mt_with_zmm',
      'em' : 'em_default',
      'tt' : 'tt_default'
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

if SCHEME == 'cards_us':

  VAR_0JET_LT = 'm_vis(20,0,200)'
  #VAR_0JET_EM = 'm_sv[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  #VAR_0JET_TT = 'm_sv[0,50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  #VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[0,50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED = 'pt_tt,m_vis[0,100,150,200,250,300],[0,40,60,70,80,90,100,110,120,130,140,150,160,300]' 
  #VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]'

  #VAR_DIJET = 'm_sv,sjdphi[0,80,100,115,130,150],(12,-3.2,3.2)'
  VAR_DIJET='m_vis,sjdphi[0,40,60,80,100,120,140],(12,-3.2,3.2)'

  scheme_mt = [
    ("8",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --add_wt=wt_quarkmass '),
    ("8",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --add_wt=wt_quarkmass '),
    ("8",   "dijet_loosemjj_lowboost",    "vbf_cat1", VAR_DIJET, ' --set_alias="sel:mt_1<50" --add_wt=wt_quarkmass '),
    ("8",   "dijet_loosemjj_boosted",     "vbf_cat2",  VAR_DIJET, '--set_alias="sel:mt_1<50" --add_wt=wt_quarkmass '),
    ("8",   "dijet_tightmjj_lowboost",    "vbf_cat3", VAR_DIJET, ' --set_alias="sel:mt_1<50" --add_wt=wt_quarkmass '),
    ("8",   "dijet_tightmjj_boosted",     "vbf_cat4",  VAR_DIJET, '--set_alias="sel:mt_1<50" --add_wt=wt_quarkmass ')
  ]
  scheme_et=[]
  scheme_tt=[]
  scheme_em=[]

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


if SCHEME == 'cpsummer16_ff_us':
  
  VAR_0JET_LT = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'
  VAR_0JET_EM = 'm_sv[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300]'

  VAR_0JET_TT = 'm_sv[50,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]' 

  VAR_BOOSTED = 'pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]'
  VAR_BOOSTED_TT = 'pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]' 

  VAR_DIJET = 'm_vis,jdphi[40,60,80,100,120,140],(4,0,3.2)'

  VAR_TT_LO_HI= VAR_DIJET
  VAR_TT_TI_HI= VAR_DIJET

  VAR_ET_LO_HI= VAR_DIJET

  VAR_EM_TI_LO= VAR_DIJET

 # VAR_DIJET_TT_1 = 'm_sv,sjdphi[0,80,100,110,125,135,150],(12,-3.2,3.2)'
 # VAR_DIJET_TT_2 = 'm_sv,sjdphi[0,80,100,110,135],(12,-3.2,3.2)'

 # VAR_DIJET_MT_1 = 'm_sv,sjdphi[0,80,115,145,160],(12,-3.2,3.2)'
 # VAR_DIJET_MT_2 = 'm_sv,sjdphi[0,80,100,115,130,145],(12,-3.2,3.2)'

 # VAR_DIJET_ET_1 = 'm_sv,sjdphi[0,80,100,115,140,160],(12,-3.2,3.2)'
 # VAR_DIJET_ET_2 = 'm_sv,sjdphi[0,80,100,115,130,145],(12,-3.2,3.2)'

 # VAR_DIJET_EM_1 = 'm_sv,sjdphi[0,90,110,150,180],(12,-3.2,3.2)'
 # VAR_DIJET_EM_2 = 'm_sv,sjdphi[0,100,115,130,150,180],(12,-3.2,3.2)'

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
    #("17",   "dijet_lowboost",    "dijet_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    #("17",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_ET_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),

    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_ET_LO_HI, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_ET_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_ET_2, '--set_alias="sel:mt_1<50" --do_ff_systs ')
  ]
  scheme_mt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_LT, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "boosted",    "boosted",  VAR_BOOSTED, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    #("17",   "dijet_lowboost",    "dijet_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    #("17",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),
  
    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_MT_1, ' --set_alias="sel:mt_1<50" --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_DIJET_MT_2, '--set_alias="sel:mt_1<50" --do_ff_systs ')

  ]
  scheme_tt = [
    ("17",   "0jet",    "0jet",  VAR_0JET_TT, ' --do_ff_systs '),
    ("17",   "boosted", "boosted",  VAR_BOOSTED_TT, ' --do_ff_systs '),
    #("17",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET_TT_1, ' --do_ff_systs '),
    #("17",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_TT_2, ' --do_ff_systs '),

    ("17",   "dijet_loosemjj_lowboost",    "dijet_loosemjj_lowboost", VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_loosemjj_boosted",     "dijet_loosemjj_boosted",  VAR_TT_LO_HI, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_lowboost",    "dijet_tightmjj_lowboost", VAR_DIJET_TT_1, ' --do_ff_systs '),
    ("17",   "dijet_tightmjj_boosted",     "dijet_tightmjj_boosted",  VAR_TT_TI_HI, ' --do_ff_systs ')
  ]
  scheme_em = [
    ("19",   "0jet",    "0jet",  VAR_0JET_EM, ' --set_alias="sel:pzeta>-35" '), 
    ("19",   "boosted", "boosted",  VAR_BOOSTED, ' --set_alias="sel:pzeta>-35" '), 
    #("19",   "dijet_lowboost",     "dijet_lowboost",  VAR_DIJET_EM_1, ' --set_alias="sel:pzeta>-10" '),
    #("19",   "dijet_boosted",     "dijet_boosted",  VAR_DIJET_EM_2, ' --set_alias="sel:pzeta>-10" '),
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
                print 'python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s --var="%(var)s" %(extra)s ' % vars()
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra)s --log_y  --ratio --ratio_range 0,2 ' % vars())

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

