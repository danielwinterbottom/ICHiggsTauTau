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
  if cores>1: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -pe hep.pe %(cores)s -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())
  else: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())

years = ["2016","2017","2018"]

categories = ['em_DZetaLtm35','em_Nbtag0','em_Nbtag0_DZetaGt30','em_Nbtag0_DZetaGt30_MHGt250','em_Nbtag0_DZetaLtm35','em_Nbtag0_DZetam10To30','em_Nbtag0_DZetam10To30_MHGt250','em_Nbtag0_DZetam35Tom10','em_Nbtag0_DZetam35Tom10_MHGt250','em_Nbtag1_DZetaGt30','em_Nbtag1_DZetam10To30','em_Nbtag1_DZetam35Tom10','em_NbtagGt1','em_NbtagGt1_DZetaGt30','em_NbtagGt1_DZetaLtm35','em_NbtagGt1_DZetam10To30','em_NbtagGt1_DZetam35Tom10','em_inclusive']

masses = ["60","80","100","120","125","130","140","160","180","200","250","300","350","400","450","500","600","700","800","900","1000","1200","1400","1600","1800","2000","2300","2600","2900","3200","3500"]

for year in years:
  root_file_loc = 'shapes/%(year)s/em' % vars()
  cmd = []
  for cat in categories:
    for mass in masses:
      for higgs in ['h','H','A']:
        for loop in ['t','b','i']:
          for syst in ['Up','Down']:
            print cat, higgs, mass
            cmd.append("rootmv %(root_file_loc)s/htt_all.inputs-mssm-vs-sm-Run%(year)s-mt_tot_puppi.root:%(cat)s/gg%(higgs)s_%(loop)s_%(mass)s_Hdamp_ggH_REWEIGHT%(syst)s %(root_file_loc)s/htt_all.inputs-mssm-vs-sm-Run%(year)s-mt_tot_puppi.root:%(cat)s/gg%(higgs)s_%(loop)s_%(mass)s_Hdamp_ggH_%(loop)s_REWEIGHT%(syst)s" % vars())
  cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')
  CreateBatchJob(cat+mass+year+'.sh',cmssw_base,cmd)
  SubmitBatchJob(cat+mass+year+'.sh',time=600,memory=24,cores=1)
          #os.system(cmd)
         


 
