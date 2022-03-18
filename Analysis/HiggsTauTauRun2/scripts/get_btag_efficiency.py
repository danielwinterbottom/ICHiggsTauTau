#!/usr/bin/env python

import os

def CreateBatchJob(name,cmssw_base,cmd_list):
  if os.path.exists(name): os.system('rm %(name)s' % vars())
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


years_dict = {'2016':{'--cfg':'scripts/plot_mssm_2016.cfg','--folder':'/vols/cms/gu18/Offline/output/MSSM/mssm_2016_b_eff'},
              '2017':{'--cfg':'scripts/plot_mssm_2017.cfg','--folder':'/vols/cms/gu18/Offline/output/MSSM/mssm_2017_b_eff'},
              '2018':{'--cfg':'scripts/plot_mssm_2018.cfg','--folder':'/vols/cms/gu18/Offline/output/MSSM/mssm_2018_b_eff'}
              }

channels_dict = {'tt':{'--cat':["Nbtag0","NbtagGt1"]},
                 'mt':{'--cat':["Nbtag0_MTLt40","Nbtag0_MT40To70","NbtagGt1_MTLt40","NbtagGt1_MT40To70"]},
                 'et':{'--cat':["Nbtag0_MTLt40","Nbtag0_MT40To70","NbtagGt1_MTLt40","NbtagGt1_MT40To70"]}
                 }

for channel, cat_list in channels_dict.items():
  for year, year_parsers in years_dict.items():
    for cat in cat_list['--cat']:
      base_cmd = "python scripts/HiggsTauTauPlot.py --method=17 --bbh_nlo_masses='' --ggh_masses='' --embedding --syst_fake_b='CMS_htt_mistag_b_%(year)s' --syst_eff_b='CMS_htt_eff_b_%(year)s' --add_wt='wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire' --no_plot --channel=%(channel)s --cat=%(cat)s --doMSSMReWeighting --var='n_deepbjets[0,1,100]' --add_sm_background=125" % vars()
      for key,val in year_parsers.items():
        base_cmd += ' %(key)s=%(val)s' % vars()
      CreateBatchJob('b_eff_%(channel)s_%(cat)s_%(year)s.sh'%vars(),"/vols/cms/gu18/CrabCMSSW/CMSSW_10_2_19",[base_cmd])
      SubmitBatchJob('b_eff_%(channel)s_%(cat)s_%(year)s.sh'%vars(),time=180,memory=24,cores=1) 
      
    
