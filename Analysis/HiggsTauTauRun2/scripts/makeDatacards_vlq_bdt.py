#!/usr/bin/env python

# python scripts/makeDatacards_vlq_bdt.py --years='2016,2017,2018' --channels='tt,mt,et,em' --output_folder='vlq_dc' --batch

# after running all jobs hadd them using this commnd inside the output folder:
#for ch in tt et mt em; do for year in 2016 2017 2018; do eval "hadd -f ${year}/${ch}/vlq.inputs-mssm-vs-sm-Run${year}-mt_tot_puppi.root ${year}/${ch}/*.root"; done; done

# to copy them to the right combine directory
#for ch in tt et mt em; do for year in 2016 2017 2018; do cp ${year}/${ch}/vlq.inputs-mssm-vs-sm-Run${year}-mt_tot_puppi.root /vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/${year}/${ch}/; done; done

import sys
from optparse import OptionParser
import os

CHANNELS = ['et', 'mt','tt']

config_files = {'2016':'scripts/plot_vlq_2016.cfg',
                '2017':'scripts/plot_vlq_2017.cfg',
                '2018':'scripts/plot_vlq_2018.cfg'
               }
param_files = {'2016':'scripts/params_vlq_2016.json',
               '2017':'scripts/params_vlq_2017.json',
               '2018':'scripts/params_vlq_2018.json',
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
  if cores>1: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -pe hep.pe %(cores)s -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())
  else: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())


parser = OptionParser()
parser.add_option("--output_folder", dest="output_folder", type='string', default='vlq_datacards',
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


(options, args) = parser.parse_args()
output_folder = options.output_folder
channels = options.channels
years = options.years
no_syst = options.no_syst

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

  FOLDER = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_bdt_v2"

  ########## Set up schemes and options ############

  # my attempt at a compromise with the binning
  # merge mt_tot<50 bins since this region was tricking the past and seems to only impact the lowest mass point  (M=60)
  # use slightly finer bins for mt_tot between 350 and 1000
  #BINS_FINE="[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"
  BINS_FINE = "[0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.71,0.72,0.73,0.74,0.75,0.76,0.77,0.78,0.79,0.8,0.81,0.82,0.83,0.84,0.85,0.86,0.87,0.88,0.89,0.9,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.975,0.98,0.985,0.99,0.995,0.996,0.997,0.998,0.999,0.9995,1.0]"
  #BINS_FINE = "[0.2,0.4,0.6,0.8,1.0],[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.95,1.0]"

  # for btag category bins merge mt_tot<60 for same reason as above
  # add a slightly finer binning between 500 and 1000
  #BINS="[0,60,80,100,120,140,160,180,200,250,300,350,400,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"
  BINS = BINS_FINE

  categories_et = [
                   "Nbtag0_MTLt40",
                   "Nbtag0_MT40To70",
                   "NbtagGt1_MTLt40",
                   "NbtagGt1_MT40To70",
                   "MTLt40",
                   "MT40To70",
                   ]

  categories_mt = [
                   "Nbtag0_MTLt40",
                   "Nbtag0_MT40To70",
                   "NbtagGt1_MTLt40",
                   "NbtagGt1_MT40To70",
                   "MTLt40",
                   "MT40To70",
                   ]

  categories_tt = [
                   "Nbtag0",
                   "NbtagGt1",
                   #"inclusive",
                   #"Nbtag0_interference",
                   #"Nbtag0_signal",
                   #"NbtagGt1_interference",
                   #"NbtagGt1_signal",
                   ]

  categories_em = [
                   "inclusive",
                   "Nbtag0_DZetaGt30",
                   "Nbtag0_DZetam10To30",
                   "Nbtag0_DZetam35Tom10",
                   "NbtagGt1_DZetaGt30",
                   "NbtagGt1_DZetam10To30",
                   "NbtagGt1_DZetam35Tom10",
                   ]
 
  ANA = 'vlq-Run%(year)s' % vars()

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
      ' --syst_tquark="CMS_htt_ttbarShape"', # Top pT re-weighting
      ' --syst_embedding_tt="CMS_htt_emb_ttbar_%(year)s"' % vars(), # ttbar contamination in embedding
      ' --syst_mssm_ggh ' # ggH theory uncertainties
      ' --syst_eff_b=CMS_htt_eff_b_%(year)s' % vars(),
      ' --syst_fake_b=CMS_htt_mistag_b_%(year)s'% vars(),
    ]

  common_sep_shape_systematics = [
      ' --syst_res_j="CMS_res_j_%(year)s"' % vars(), # Jet energy resolution
      ' --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s"' % vars(), # MET unclustered energy uncertainty
      ' --syst_scale_met="CMS_htt_boson_scale_met_%(year)s"' % vars(), # MET recoil correction uncertainty
      ' --syst_res_met="CMS_htt_boson_res_met_%(year)s"' % vars(),  # MET recoil correction uncertainty
      ' --syst_scale_j_regrouped="CMS_scale_j_*group"', # Jet energy scale (grouped)
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
    ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
  ]

  et_sep_shape_systematics = [
    ' --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s"' % vars(), # Tau energy scale
    ' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_e_scale="CMS_scale_e"', # Election energy scale
    ' --do_ff_systs_1',
    ' --do_ff_systs_2',
  ]

  mt_shape_systematics = [
    ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
  ]

  mt_sep_shape_systematics = [
    ' --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s"' % vars(), # Tau energy scale
    ' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --do_ff_systs_1',
    ' --do_ff_systs_2',
  ]

  tt_shape_systematics = [
  ]
  
  tt_sep_shape_systematics = [
    ' --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s"' % vars(), # Tau energy scale
    ' --do_ff_systs'
  ]
 
  em_shape_systematics = [
   ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency
  ]

  em_sep_shape_systematics = [
   ' --syst_e_scale="CMS_scale_e"', # Election energy scale
  ]

  extra_channel = {
      "em" : common_shape_systematics+em_shape_systematics,
      "et" : common_shape_systematics+et_shape_systematics,
      "mt" : common_shape_systematics+mt_shape_systematics,
      "tt" : common_shape_systematics+tt_shape_systematics,
  }

  sep_systs_channel = {
      "em" : common_sep_shape_systematics+em_sep_shape_systematics,
      "et" : common_sep_shape_systematics+et_sep_shape_systematics,
      "mt" : common_sep_shape_systematics+mt_sep_shape_systematics,
      "tt" : common_sep_shape_systematics+tt_sep_shape_systematics,
  }


  #var     = 'vlq_bdt_score'
  #var     = 'vlq_i_bdt_score,vlq_s_bdt_score'
  #var_dict = {"interference":"vlq_i_bdt_binary", "signal":"vlq_s_bdt_binary"}
  var      = "vlq_s_bdt_multiclass"
  dc_app  = '-mt_tot_puppi'

  for ch in channels:

    if not os.path.isdir('%(output_folder)s/%(year)s/%(ch)s' % vars()):
      os.system("mkdir %(output_folder)s/%(year)s/%(ch)s" % vars())

    categories = cat_schemes[ch]

    for cat in categories:
    
      #var = var_dict[cat.split("_")[1]] 
    
      add_cond = '--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\''
      if ch == "em": method = '19'
      else: method = '17'

      if cat.startswith("NbtagGt1"): bins = BINS
      else: bins = BINS_FINE


      add_cond_nosysts = add_cond

      if not no_syst:
        for i in extra_channel[ch]:   
          add_cond += i

      run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var)s%(bins)s" --embedding --add_sm_background=125 --no_plot %(add_cond)s --embed_folder="" --signal_folder=""' % vars()
      rename_cmd = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var)s_%(cat)s_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s.inputs-%(ANA)s%(dc_app)s.root' % vars()

      if not options.batch:
        print run_cmd
        if not options.dry_run:
          os.system(run_cmd)
          os.system(rename_cmd)
      elif options.batch:
        job_file = '%(output_folder)s/jobs/vlq_datacard_%(cat)s_%(ch)s_%(YEAR)s.sh' % vars()
        CreateBatchJob(job_file,cmssw_base,[run_cmd,rename_cmd])
        if not options.dry_run:
          SubmitBatchJob(job_file,time=180,memory=24,cores=1)
  
      
      # run systematics that involve drawing from different trees in parallel     
      if not no_syst: 
        for syst in sep_systs_channel[ch]:
          syst_name=syst.split('=')[0].split('--')[1]
          dc='%(cat)s_%(syst_name)s' % vars()
          run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --extra_name=%(syst_name)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var)s%(bins)s" --embedding --add_sm_background=125 --no_plot %(add_cond_nosysts)s --no_default %(syst)s --embed_folder="" --signal_folder=""' % vars()
          rename_cmd = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var)s_%(dc)s_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(dc)s.inputs-%(ANA)s%(dc_app)s.root' % vars()
  
          if not options.batch:
            print run_cmd
            if not options.dry_run:
              os.system(run_cmd)
              os.system(rename_cmd)
          elif options.batch:
            job_file = '%(output_folder)s/jobs/vlq_datacard_%(dc)s_%(ch)s_%(YEAR)s.sh' % vars()
            CreateBatchJob(job_file,cmssw_base,[run_cmd,rename_cmd])
            if not options.dry_run:
              SubmitBatchJob(job_file,time=180,memory=24,cores=1)    



