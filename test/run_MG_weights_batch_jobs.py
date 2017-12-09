import os
import subprocess

cwd = os.getcwd()
CMSSW_BASE = subprocess.check_output("echo $CMSSW_BASE", shell=True).replace('\n','/')
nperjob=400
processes=['ggH','qqH']
ggH_total=5791488
vbf_total=1998200
ggH_option='MGsignalGF=True'
vbf_option='MGsignalVBF=True'

for process in processes:
  job_count=1  
  if process is 'ggH':
    dir_name  = 'GluGluToHToTauTau_amcNLO_M-125'
    total=ggH_total
    proc_option = ggH_option
  else: 
    dir_name = 'VBFHToTauTau_amcNLO_M-125'
    total=vbf_total
    proc_option = vbf_option
  splitting=range(1,total+1,nperjob)
  outdir='/vols/cms/dw515/Nov20_MC_80X/%s/' % dir_name
  os.system('mkdir -p %s' % outdir)
  for offset in splitting:
    outfile='%sEventTree_%i.root' % (outdir,job_count)
    if splitting.index(offset) == len(splitting)-1: maxevents=-1
    else: maxevents = nperjob
    options='release=80XMINIAOD isData=0 doHT=1 globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v8 isReHLT=1 LHEWeights=True %s maxevents=%i firstevent=%i cmsswbase=%s outfile=%s' % (proc_option,maxevents,offset,CMSSW_BASE,outfile) 
    job_string='cd %s\n' % cwd
    job_string+='source /vols/grid/cms/setup.sh\n'
    job_string+='export SCRAM_ARCH=slc6_amd64_gcc481\n'
    job_string+='eval `scramv1 runtime -sh`\n'
    job_string+='ulimit -c 0\n'
    job_string+='cmsRun %s/src/UserCode/ICHiggsTauTau/test/higgstautau_cfg_80X_Nov17_batch.py %s' % (CMSSW_BASE,options)
    job = 'job_%s_%i.sh' % (process,job_count)
    job_file = open(job, "w")
    job_file.write(job_string)
    job_file.close()
    os.system('chmod 755 %s' % job)    
    job_count+=1

  parajob_count=1
  for i in range(1,job_count,2000):
    parajob = 'parajob_%s_%i.sh' % (process,parajob_count)
    job_string='cd %s\n' % cwd
    job_string+='./job_%s_${SGE_TASK_ID}.sh' % process
    parajob_file = open(parajob, "w")
    parajob_file.write(job_string)
    parajob_file.close()
    os.system('chmod 755 %s' % parajob) 
    maxj= min(i+2000-1,job_count-1)
    os.system('qsub -q hep.q -l h_rt=0:180:0 -cwd -t %i-%i:1 %s' % (i,maxj,parajob))
    parajob_count+=1

