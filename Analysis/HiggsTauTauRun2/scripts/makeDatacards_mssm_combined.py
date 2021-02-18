#!/usr/bin/env python

# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --batch --msv_cut

import sys
from optparse import OptionParser
import os

CHANNELS = ['et', 'mt','tt']

config_files = {'2016':'scripts/plot_mssm_2016.cfg',
                '2017':'scripts/plot_mssm_2017.cfg',
                '2018':'scripts/plot_mssm_2018.cfg'
               }
param_files = {'2016':'scripts/params_mssm_2016.json',
               '2017':'scripts/params_mssm_2017.json',
               '2018':'scripts/params_mssm_2018.json',
              }

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
  os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -pe hep.pe %(cores)s -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())


parser = OptionParser()
parser.add_option("--output_folder", dest="output_folder", type='string', default='mssm_datacards',
                  help="Output folder where plots/datacards will be saved to.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--batch", dest="batch", action='store_true', default=False,
                  help="Submit on batch.")
parser.add_option("--dry_run",dest="dry_run", action='store_true', default=False,
                  help="Just displays code it will run")
parser.add_option("--years", dest="years", type='string', default='2016,2017,2018',
                  help="Year input")
parser.add_option("--no_syst",dest="no_syst", action='store_true', default=False,
                  help="Run without systematics")
parser.add_option("--msv_cut",dest="msv_cut", action='store_true', default=False,
                  help="Add m_sv cut to no btag datacards")


(options, args) = parser.parse_args()
output_folder = options.output_folder
channels = options.channels
years = options.years
no_syst = options.no_syst
msv_cut = options.msv_cut

print 'Processing channels:      %(channels)s' % vars()
print 'Processing years:         %(years)s' % vars()

years = options.years.split(',')

cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')

if not os.path.isdir('%(output_folder)s' % vars()):
  os.system("mkdir %(output_folder)s" % vars())

if not os.path.isdir('%(output_folder)s/jobs' % vars()):
  os.system("mkdir %(output_folder)s/jobs" % vars())

for year in years:

  if not os.path.isdir('%(output_folder)s/%(year)s' % vars()):
    os.system("mkdir %(output_folder)s/%(year)s" % vars())
   
  CFG = config_files[year]

  with open("%(CFG)s"%vars(),"r") as cfgfile:
    lines = cfgfile.readlines()

  configmap={}

  for ind in range(0,len(lines)):
    if len(lines[ind].split("="))>1:
      configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])
  YEAR=year
  FOLDER=configmap["folder"].rstrip('\n')
  PARAMS=configmap["paramfile"].rstrip('\n')

  PARAMS = param_files[year]

  ########## Set up schemes and options ############

  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"

  categories_et = [
                   "inclusive",
                   "nobtag_tightmt",
                   "nobtag_loosemt",
                   "btag_tightmt",
                   "btag_loosemt",
                   ]

  categories_mt = [
                   "inclusive",
                   "nobtag_tightmt",
                   "nobtag_loosemt",
                   "btag_tightmt",
                   "btag_loosemt",
                   ]

  categories_tt = [
                   "inclusive",
                   "btag",
                   "nobtag",
                   ]

  categories_em = [
                   "inclusive",
                   "nobtag_highdzeta",
                   "nobtag_mediumdzeta",
                   "nobtag_lowdzeta",
                   "btag_highdzeta",
                   "btag_mediumdzeta",
                   "btag_lowdzeta",
                   "ttbar_control",
                   ]
 
  ANA = 'mssm-vs-sm-Run%(year)s' % vars()

  cat_schemes = {
                 'et' : categories_et,
                 'mt' : categories_mt,
                 'em' : categories_em,
                 'tt' : categories_tt
                 }

  ### Systematics ###

  common_shape_systematics = [
      ' --syst_tau_id_diff="CMS_eff_t_*_%(year)s"' % vars(), # Tau ID efficiency
      ' --syst_tau_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Tau Trigger efficiency
      ' --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s"' % vars(), # Tau energy scale
      ' --syst_tquark="CMS_htt_ttbarShape"', # Top pT re-weighting
      ' --syst_res_j="CMS_res_j_%(year)s"' % vars(), # Jet energy resolution
      ' --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s"' % vars(), # MET unclustered energy uncertainty
      ' --syst_scale_met="CMS_htt_boson_scale_met_%(year)s"' % vars(), # MET recoil correction uncertainty
      ' --syst_res_met="CMS_htt_boson_reso_met_%(year)s"' % vars(),  # MET recoil correction uncertainty
      ' --syst_scale_j_regrouped="CMS_scale_j_*group"', # Jet energy scale (grouped)
      ' --syst_embedding_tt="CMS_htt_emb_ttbar_%(year)s"' % vars(), # ttbar contamination in embedding
    ]

  if year != '2018': 
    common_shape_systematics += [
      ' --syst_prefire="CMS_prefiring"'
      ]
  if year == '2016':  
    common_shape_systematics += [
      ' --syst_zwt="CMS_htt_dyShape_2016"'
      ]
  else:
    common_shape_systematics += [
      ' --syst_zwt="CMS_htt_dyShape"'
      ]


  et_shape_systematics = [
    ' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_e_scale="CMS_scale_e"', # Election energy scale
    ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
    ' --do_ff_systs' 
  ]

  mt_shape_systematics = [
    ' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
    ' --do_ff_systs'
  ]

  tt_shape_systematics = [
    ' --do_ff_systs'
  ]
   
  em_shape_systematics = ['']

  extra_channel = {
      "em" : common_shape_systematics+em_shape_systematics,
      "et" : common_shape_systematics+et_shape_systematics,
      "mt" : common_shape_systematics+mt_shape_systematics,
      "tt" : common_shape_systematics+tt_shape_systematics,
  }

  var     = 'mt_tot'
  dc_app  = '-mttot_puppi'

  for ch in channels:

    if not os.path.isdir('%(output_folder)s/%(year)s/%(ch)s' % vars()):
      os.system("mkdir %(output_folder)s/%(year)s/%(ch)s" % vars())

    categories = cat_schemes[ch]

    for cat in categories:
      add_cond = '--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\''
      if ch == "em": method = '19'
      else: method = '17'

      if cat.startswith("btag"): bins = BINS
      else: bins = BINS_FINE

      if not no_syst:
        for i in extra_channel[ch]:   
          add_cond += i

      if msv_cut and cat.startswith('nobtag'):
        add_cond += ' --sel=\'m_sv>250\''

      run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var)s%(bins)s" --embedding --doMSSMReWeighting --add_sm_background=125 --no_plot %(add_cond)s' % vars()
      rename_cmd = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var)s_%(cat)s_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s.inputs-%(ANA)s%(dc_app)s.root' % vars()

      if not options.batch:
        print run_cmd
        if not options.dry_run:
          os.system(run_cmd)
          os.system(rename_cmd)
      elif options.batch:
        job_file = '%(output_folder)s/jobs/mssm_datacard_%(cat)s_%(ch)s_%(YEAR)s.sh' % vars()
        CreateBatchJob(job_file,cmssw_base,[run_cmd,rename_cmd])
        if not options.dry_run:
          SubmitBatchJob(job_file,time=180,memory=24,cores=1)
           
    



