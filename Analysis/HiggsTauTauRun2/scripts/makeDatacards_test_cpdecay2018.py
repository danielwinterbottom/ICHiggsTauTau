#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

CHANNELS= ['et', 'mt', 'em','tt', 'zmm']

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

def CreateBatchJob(name,cmssw_base,cmd_list):
  if os.path.exists(job_file): os.system('rm %(name)s' % vars())
  os.system('echo "#!/bin/bash" >> %(name)s' % vars())
  os.system('echo "cd %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2" >> %(name)s' % vars())
  os.system('echo "source /vols/grid/cms/setup.sh" >> %(name)s' % vars())
  os.system('echo "export SCRAM_ARCH=slc6_amd64_gcc481" >> %(name)s' % vars())
  os.system('echo "eval \'scramv1 runtime -sh\'" >> %(name)s' % vars())
  os.system('echo "source %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/setup_libs.sh" >> %(name)s' % vars())
  os.system('echo "ulimit -c 0" >> %(name)s' % vars())
  for cmd in cmd_list:
    os.system('echo "%(cmd)s" >> %(name)s' % vars())
  os.system('chmod +x %(name)s' % vars())
  print "Created job:",name

def SubmitBatchJob(name,time=180,memory=24,cores=1):
  error_log = name.replace('.sh','_error.log')
  output_log = name.replace('.sh','_output.log')
  if os.path.exists(error_log): os.system('rm %(error_log)s' % vars())
  if os.path.exists(output_log): os.system('rm %(output_log)s' % vars())
  if cores>1: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -pe hep.pe %(cores)s -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())
  else: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())


parser = OptionParser()
parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',default='scripts/params_2018.json',
                  help="Specify the parameter file containing the luminosity and cross section information - can be used to override config file.")
parser.add_option("--cfg", dest="config", type='string', default='scripts/plot_cpdecays_2018.cfg',
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
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='cpdecay',
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


# cmssw directory
cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')

(options, args) = parser.parse_args()
input_folder = options.folder
output_folder = options.output_folder
channels = options.channels
CFG=options.config
BLIND = options.blind
BLIND = " "
no_shift_systs = options.no_shift_systs
# ------------------------
#       Config file 
with open("%(CFG)s"%vars(),"r") as cfgfile:
  lines = cfgfile.readlines()

configmap={}

for ind in range(0,len(lines)):
  if len(lines[ind].split("="))>1:
    configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])
    
if "signal_scheme" in configmap:
    SCHEME= configmap["signal_scheme"].rstrip('\n')

if "year" in configmap:
    YEAR=configmap["year"].rstrip('\n')
FOLDER=configmap["folder"].rstrip('\n')
PARAMS=configmap["paramfile"].rstrip('\n')
# --------------------------

#Override config file params
if not options.scheme == "":
  SCHEME=options.scheme
if not options.folder ==  "":
  FOLDER=options.folder
if not options.params == "":
  PARAMS=options.params
if not options.year == "":
  YEAR=options.year
  

# check whether path is in an exitisting directory
if not os.path.isdir('%(output_folder)s' % vars()):
  os.system("mkdir %(output_folder)s" % vars())
if not os.path.isdir('%(output_folder)s/test2/jobs' % vars()):
  os.system("mkdir %(output_folder)s/test2/jobs" % vars())
if not os.path.isdir('%(output_folder)s/test2/%(YEAR)s' % vars()):
  os.system("mkdir %(output_folder)s/test2/%(YEAR)s" % vars())


#### Always apply these options:

extra_global = ' '


#### Apply these options for specific channels

# update these to latest shape systematics
extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
  }
jes_systematics=''
if options.split_jes:
    jes_systematics = ' --syst_scale_j_by_source="CMS_scale_j_SOURCE_13TeV" '


common_shape_systematics = (
    ' --syst_zwt="CMS_htt_dyShape_13TeV" '
    ' --syst_tquark="CMS_htt_ttbarShape_13TeV" '
    ' --syst_prefire="CMS_PreFire_13TeV" '
    ' --syst_qcd_scale="CMS_scale_gg_13TeV" '
    ' --syst_ps="CMS_*PS_ggH_13TeV" '
    ' --syst_res_j="CMS_res_j_13TeV" '
)

if not no_shift_systs:
    common_shape_systematics += (
        ' --syst_scale_met_unclustered="CMS_scale_met_unclustered_13TeV" '
        ' --syst_scale_met="CMS_htt_boson_scale_met_13TeV" '
        ' --syst_res_met="CMS_htt_boson_reso_met_13TeV" '
    )

if options.total_jes:
    common_shape_systematics += ' --syst_scale_j="CMS_scale_j_13TeV" '
elif not no_shift_systs: 
    common_shape_systematics += ' --syst_scale_j_regrouped="CMS_scale_j_*group_13TeV"'

et_shape_systematics = (
  ' --syst_tau_id_diff="CMS_eff_t_*MVADM_13TeV" '
  ' --syst_tau_trg_diff="CMS_eff_Xtrigger_mt_*MVADM_13TeV" '
  ' --syst_eff_b_weights="CMS_eff_b_13TeV" '
)
if not no_shift_systs:
    et_shape_systematics += (
        ' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_13TeV" '
        ' --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_13TeV" '
        ' --syst_e_scale="CMS_scale_e_13TeV" '
        ' --syst_tau_scale_grouped="CMS_scale_t_*group_13TeV" '
    )

mt_shape_systematics = (
    ' --syst_tau_id_diff="CMS_eff_t_*MVADM_13TeV" '
    ' --syst_tau_trg_diff="CMS_eff_Xtrigger_mt_*MVADM_13TeV" '
    ' --syst_eff_b_weights="CMS_eff_b_13TeV" '
)
if not no_shift_systs:
    mt_shape_systematics += (
        ' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_13TeV" '
        ' --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_13TeV" ' 
        ' --syst_mu_scale="CMS_scale_mu_13TeV" '
        ' --syst_tau_scale_grouped="CMS_scale_t_*group_13TeV" '
    )

tt_shape_systematics = (
    ' --syst_tau_id_diff="CMS_eff_t_*MVADM_13TeV" '
    ' --syst_tau_trg_diff="CMS_eff_t_trg_*MVADM_13TeV" '
)
if not no_shift_systs:
    tt_shape_systematics += ' --syst_tau_scale_grouped="CMS_scale_t_*group_13TeV" '

if options.embedding:
    common_shape_systematics+=' --syst_embedding_tt="CMS_ttbar_embeded_13TeV" '


extra_channel = {
      "et" : ' '+common_shape_systematics+ ' '+et_shape_systematics,
      "mt" : ' '+common_shape_systematics+ ' '+mt_shape_systematics,
      "tt" : ' '+common_shape_systematics+ ' '+tt_shape_systematics,
  }

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
  }
  
if SCHEME == 'cpdecay':

  # TT variables

#  VAR1 ="IC_Nov13_tauspinner_max_score,aco_angle_1[0.,0.7,0.8,0.9],(14,0,6.28319)"
#  VAR5 ="IC_Nov13_tauspinner_max_score,aco_angle_5[0.,0.7,0.8,0.9],(14,0,6.28319)"
#  VAR6 ="IC_Nov13_tauspinner_max_score,aco_angle_6[0.,0.7,0.8,0.9],(14,0,6.28319)"
#  VAR_H_TT_Other  = "IC_Nov13_tauspinner_max_score[0.,0.7,0.8,0.9]"
#  VAR_ZTTEMBED_TT = "IC_Nov13_tauspinner_max_score[0.,0.7,0.8,0.9]"
#  VAR_JETFAKES_TT = "IC_Nov13_tauspinner_max_score[0.,0.7,0.8,0.9]"

  VAR1 ="aco_angle_1[0.,0.7,0.8,0.9]"
  VAR5 ="aco_angle_5[0.,0.7,0.8,0.9]"
  VAR6 ="aco_angle_6[0.,0.7,0.8,0.9]"
  #VAR_H_TT_Other  = "IC_01Jun2020_max_score[0.,0.7,0.8,0.9,1.0]"
  #VAR_H_TT        = "IC_01Jun2020_max_score(7,0.3,1.0)"
  #VAR_ZTTEMBED_TT = "IC_01Jun2020_max_score(7,0.3,1.0)"
  #VAR_JETFAKES_TT = "IC_01Jun2020_max_score(7,0.3,1.0)"

  # merge bins after seeing SM/PS stat unc.(28 April 2020) 
  VAR_PIPI ="aco_angle_6[0.,0.7,0.8]"
  VAR_PIRHO ="aco_angle_5[0.,0.7,0.8,0.9]"
  VAR_PIA1 ="aco_angle_5[0.,0.7,0.8,0.9]"
  VAR_PI0A1 ="aco_angle_5[0.,0.7,0.8,0.9]"
  VAR_RHORHO ="aco_angle_1[0.,0.7,0.8,0.9]"
  VAR_A1RHO ="aco_angle_1[0.,0.7,0.8,0.9]"
  VAR_A1A1 ="aco_angle_1[0.,0.7,0.8]"
  VAR_A1A1_PolVec ="pv_angle[0.,0.7,0.8]"
  VAR_0A1RHO ="aco_angle_1[0.,0.7,0.8,0.9]"
  VAR_0A1A1 ="aco_angle_1[0.,0.7,0.8]"

  ADD_STRING_MT = ' --set_alias "sel:(mt_1<50)" '

  scheme_et = []
  scheme_mt = []
  scheme_tt = [
    ("17",   "inclusive_mvarhorho",    "2018_higgs_Rho_Rho",  VAR_RHORHO, ' '),
    ("17",   "inclusive_mvarho0a1",    "2018_higgs_0A1_Rho_and_0A1_0A1",  VAR_0A1RHO, ' '),
   # ("17",   "inclusive_mvaa1rho",    "2018_higgs_A1_Rho",  VAR_A1RHO, ' '),
   # ("17",   "inclusive_mvaa1a1",    "2018_higgs_A1_A1",  VAR_A1A1, ' '),
   # ("17",   "inclusive_mvaa1a1",    "2018_higgs_A1_A1_PolVec",  VAR_A1A1_PolVec, ' '),
   # ("17",   "inclusive_mvapipi",    "2018_higgs_Pi_Pi",  VAR_PIPI, ' '),
   # ("17",   "inclusive_mvapirho",    "2018_higgs_Pi_Rho_Mixed",  VAR_PIRHO, ' '),
   # ("17",   "inclusive_mvapi0a1",    "2018_higgs_Pi_0A1_Mixed",  VAR_PI0A1, ' '),
   # ("17",   "inclusive_mvaa1pi",    "2018_higgs_Pi_A1_Mixed",  VAR_PIA1, ' '),
   # ("17",   "inclusive_mvaa10a1",    "2018_higgs_A1_0A1",  VAR_0A1A1, ' '),

    # ("17",   "higgs_mvaother",    "2018_higgs_other",  VAR_H_TT_Other, ' '),
    #("17",   "higgs",       "2018_higgs",     VAR_H_TT, ' '),
    #("17",   "zttEmbed",    "2018_zttEmbed",  VAR_ZTTEMBED_TT, ' '),
    #("17",   "jetFakes",    "2018_jetFakes",  VAR_JETFAKES_TT, ' '),

    #("17",   "higgs_mvarhorho",    "2018_higgs_Rho_Rho_ss",  VAR_RHORHO, ' --do_ss --no_systs '),
    #("17",   "higgs_mvarho0a1",    "2018_higgs_0A1_Rho_and_0A1_0A1_ss",  VAR_0A1RHO, ' --do_ss --no_systs '),
    #("17",   "higgs_mvaa1rho",    "2018_higgs_A1_Rho_ss",  VAR_A1RHO, ' --do_ss --no_systs '),
    #("17",   "higgs_mvaa1a1",    "2018_higgs_A1_A1_ss",  VAR_A1A1, ' --do_ss --no_systs '),
    #("17",   "higgs_mvaa1a1",    "2018_higgs_A1_A1_PolVec_ss",  VAR_A1A1_PolVec, ' --do_ss --no_systs '),
    #("17",   "higgs_mvapipi",    "2018_higgs_Pi_Pi_ss",  VAR_PIPI, ' --do_ss --no_systs '),
    #("17",   "higgs_mvapirho",    "2018_higgs_Pi_Rho_Mixed_ss",  VAR_PIRHO, ' --do_ss --no_systs '),
    #("17",   "higgs_mvapi0a1",    "2018_higgs_Pi_0A1_Mixed_ss",  VAR_PI0A1, ' --do_ss --no_systs '),
    #("17",   "higgs_mvaa1pi",    "2018_higgs_Pi_A1_Mixed_ss",  VAR_PIA1, ' --do_ss --no_systs '),
    #("17",   "higgs_mvaa10a1",    "2018_higgs_A1_0A1_ss",  VAR_0A1A1, ' --do_ss --no_systs '),
    #("17",   "higgs",       "2018_higgs_ss",     VAR_H_TT, ' --do_ss --no_systs '),
    #("17",   "zttEmbed",    "2018_zttEmbed_ss",  VAR_ZTTEMBED_TT, ' --do_ss --no_systs '),
    #("17",   "jetFakes",    "2018_jetFakes_ss",  VAR_JETFAKES_TT, ' --do_ss --no_systs '),

    #("17",   "zttEmbed_mvarhorho",    "2018_zttEmbed_Rho_Rho",  VAR_RHORHO, ' '),
    #("17",   "zttEmbed_mvarho0a1",    "2018_zttEmbed_0A1_Rho_and_0A1_0A1",  VAR_0A1RHO, ' '),
    #("17",   "zttEmbed_mvaa1rho",    "2018_zttEmbed_A1_Rho",  VAR_A1RHO, ' '),
    #("17",   "zttEmbed_mvaa1a1",    "2018_zttEmbed_A1_A1",  VAR_A1A1, ' '),
    #("17",   "zttEmbed_mvaa1a1",    "2018_zttEmbed_A1_A1_PolVec",  VAR_A1A1_PolVec, ' '),
    #("17",   "zttEmbed_mvapipi",    "2018_zttEmbed_Pi_Pi",  VAR_PIPI, ' '),
    #("17",   "zttEmbed_mvapirho",    "2018_zttEmbed_Pi_Rho_Mixed",  VAR_PIRHO, ' '),
    #("17",   "zttEmbed_mvapi0a1",    "2018_zttEmbed_Pi_0A1_Mixed",  VAR_PI0A1, ' '),
    #("17",   "zttEmbed_mvaa1pi",    "2018_zttEmbed_Pi_A1_Mixed",  VAR_PIA1, ' '),
    #("17",   "zttEmbed_mvaa10a1",    "2018_zttEmbed_A1_0A1",  VAR_0A1A1, ' '),
  
    #("17",   "jetFakes_mvarhorho",    "2018_jetFakes_Rho_Rho",  VAR_RHORHO, ' '),
    #("17",   "jetFakes_mvarho0a1",    "2018_jetFakes_0A1_Rho_and_0A1_0A1",  VAR_0A1RHO, ' '),
    #("17",   "jetFakes_mvaa1rho",    "2018_jetFakes_A1_Rho",  VAR_A1RHO, ' '),
    #("17",   "jetFakes_mvaa1a1",    "2018_jetFakes_A1_A1",  VAR_A1A1, ' '),
    #("17",   "jetFakes_mvaa1a1",    "2018_jetFakes_A1_A1_PolVec",  VAR_A1A1_PolVec, ' '),
    #("17",   "jetFakes_mvapipi",    "2018_jetFakes_Pi_Pi",  VAR_PIPI, ' '),
    #("17",   "jetFakes_mvapirho",    "2018_jetFakes_Pi_Rho_Mixed",  VAR_PIRHO, ' '),
    #("17",   "jetFakes_mvapi0a1",    "2018_jetFakes_Pi_0A1_Mixed",  VAR_PI0A1, ' '),
    #("17",   "jetFakes_mvaa1pi",    "2018_jetFakes_Pi_A1_Mixed",  VAR_PIA1, ' '),
    #("17",   "jetFakes_mvaa10a1",    "2018_jetFakes_A1_0A1",  VAR_0A1A1, ' '),
  ]
  ANA = 'sm'

cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'tt' : scheme_tt
}

num = 0

for ch in channels:
    if not os.path.isdir('%(output_folder)s/test2/%(YEAR)s/%(ch)s' % vars()):
        os.system("mkdir %(output_folder)s/test2/%(YEAR)s/%(ch)s" % vars())
    scheme = cat_schemes[ch]
    for x in scheme:
        num = num + 1
	cat_num = x[0]
        cat_str = x[1]
        dc      = x[2]
        var     = x[3]
        opts    = x[4]
        extra = options.extra + ' ' + extra_global + ' ' + extra_channel[ch] + ' ' + opts
        if options.embedding: extra+=' --embedding'
        if ch in ['em','et','mt']: extra+=' --add_wt=\"wt_btag*wt_tau_trg_mvadm*wt_tau_id_mvadm\" '
        if ch in ['tt']: extra+=' --add_wt=\"wt_tau_trg_mvadm*wt_tau_id_mvadm\" '
        if ch in ['et','mt','tt'] and cat_num in ['17','18'] and SCHEME != 'ip_uncert': extra+=' --do_ff_systs '
        extra_jes = options.extra + ' ' + extra_global + ' ' + jes_systematics + ' ' + opts + ' --no_default '

        run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/test2/%(YEAR)s/%(ch)s --analysis=mssmrun2 --ggh_masses_powheg='' --bbh_masses_powheg='' --datacard=%(dc)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s --var="%(var)s" %(extra)s --ratio_range 0,2 --custom_y_range --y_axis_max=12000' % vars()
        job_file = '%(output_folder)s/test2/jobs/aco_%(num)s.sh' % vars()
	CreateBatchJob(job_file,cmssw_base,[run_cmd])
        SubmitBatchJob(job_file,time=180,memory=24,cores=1)






