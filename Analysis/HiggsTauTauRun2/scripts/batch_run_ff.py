#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import string
import shlex
from subprocess import Popen, PIPE

parser = OptionParser()
parser.add_option("--year", dest="year", type='string', default='all',help="years to run fake factors for")
parser.add_option("--channel", dest="channel", type='string', default='all',help="channels to run fake factors for")
parser.add_option("--output", dest="output", type='string', default='fake_factor_output',help="Name of output folder")
parser.add_option("--workspace", dest="workspace", action='store_true', default=False,help="Run workspaces")
parser.add_option("--dry_run", dest="dry_run", action='store_true',help="will only create only job and will not run")
(options, args) = parser.parse_args()

year = options.year
channel = options.channel
dry_run = options.dry_run
output = options.output
workspace = options.workspace

cmssw_base = "/vols/cms/gu18/CrabCMSSW/CMSSW_10_2_19"

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


if year == "all":
  year_list = ["2016","2017","2018"]
else:
  year_list = year.split(",")

if channel == "all":
  channel_list = ["et","mt","tt"]
else:
  channel_list = channel.split(",")

if not os.path.isdir(output):os.system("mkdir %(output)s" % vars())
if not os.path.isdir('%(output)s/jobs' % vars()):os.system("mkdir %(output)s/jobs" % vars())
if not os.path.isdir('%(output)s/workspaces' % vars()):os.system("mkdir %(output)s/workspaces" % vars())

for ch in channel_list:
  if not os.path.isdir('%(output)s/%(ch)s' % vars()):os.system("mkdir %(output)s/%(ch)s" % vars())  
  for yr in year_list:
    if not os.path.isdir('%(output)s/%(ch)s/%(yr)s' % vars()): os.system("mkdir %(output)s/%(ch)s/%(yr)s" % vars())


for ch in channel_list:
  for yr in year_list:

    if not workspace:
      for mc_shift in ["","Up","Down"]:
    
        if ch in ["et","mt"]:
          run_cmd_ff = "python scripts/mt_et_ffs_deeptau_mssm.py --channel=%(ch)s --year=%(yr)s --output_folder='%(output)s/%(ch)s/%(yr)s/' --mc_shift=%(mc_shift)s" % vars()
        elif ch in ["tt"]:
          run_cmd_ff = "python scripts/tt_ffs_deeptau_2018_jetpt_and_pt.py --year=%(yr)s --output_folder='%(output)s/%(ch)s/%(yr)s/'" % vars()

        if not (ch in ["tt"] and mc_shift in ["Up","Down"]):
          job_file = "%(output)s/jobs/ff_%(ch)s_%(yr)s%(mc_shift)s.sh" % vars()
          CreateBatchJob(job_file,cmssw_base,[run_cmd_ff])
          if not dry_run: SubmitBatchJob(job_file,time=180,memory=24,cores=1)

    else:

      if ch in ["et","mt"]:
        run_cmd_ws = "python scripts/make_ff_workspace_mssm_lt.py --channel=%(ch)s --year=%(yr)s --output='%(output)s/workspaces/' --input='%(output)s/%(ch)s/%(yr)s/'" % vars()
      elif ch in ["tt"]:
        run_cmd_ws = "python scripts/make_ff_workspace_mssm_tt.py --year=%(yr)s --output='%(output)s/workspaces/' --input='%(output)s/%(ch)s/%(yr)s/'" % vars()

      job_file = "%(output)s/jobs/ff_ws_%(ch)s_%(yr)s.sh" % vars()
      CreateBatchJob(job_file,cmssw_base,[run_cmd_ws])
      if not dry_run: SubmitBatchJob(job_file,time=180,memory=24,cores=1)

        



      
  

