import os
cwd = os.getcwd()

processes=['ggH','qqH']
ggH_total=
vbf_total=0

for process in processes:
  if process is 'ggH': 
    dir_name  = 'GluGluToHToTauTau_amcNLO_M-125'
    total=ggH_total
  else: 
    dir_name = 'VBFHToTauTau_amcNLO_M-125'
    total=vbf_total
  outfile='/vols/cms/dw515/Nov20_MC_80X/%s/' % dir_name
  maxEvents=100
  firstEvent=1
  
  job_string=''
  job_string+='cd %s\n' % cwd
  job_string+='export SCRAM_ARCH=slc6_amd64_gcc481\n'
  job_string+='eval \`scramv1 runtime -sh\`\n'
  job_string+='ulimit -c 0\n'
