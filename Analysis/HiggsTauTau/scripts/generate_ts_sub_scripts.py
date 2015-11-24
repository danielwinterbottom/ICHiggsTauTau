#!/usr/bin/env python
import subprocess
import sys
import os
from optparse import OptionParser
import math
import fnmatch

MACHINE1=""#First machine you want to submit to
MACHINE2=""#Second machine you want to submit to

parser = OptionParser()

parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample.txt",
                  help="list of job names/nr of jobs per name that you want to submit")

(options,args) = parser.parse_args()
samplelist=options.samplelist

with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

nfiles={}

njobs=0;
nfiles={}
for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])
  njobs+=int(lines[ind].split()[1])


print "Number of jobs is %(njobs)d"%vars()

cwd=os.getcwd()

nsubmitted=0
if os.path.isfile("./jobs/submit_to_machine_1.sh"):
  os.system("rm ./jobs/submit_to_machine_1.sh")
if os.path.isfile("./jobs/submit_to_machine_2.sh"):
  os.system("rm ./jobs/submit_to_machine_2.sh")

machine1jobs=open("./jobs/submit_to_machine_1.sh","w")
machine2jobs=open("./jobs/submit_to_machine_2.sh","w")
n_machine1=int(math.ceil(float(njobs)/float(2)))
n_machine2=njobs-n_machine1
switched=0
machine1jobs.write('cd %(cwd)s \n'%vars())
machine1jobs.write('source /vols/cms/grid/setup.sh \n')
machine1jobs.write('export PATH=/home/hep/amd12/bin/:$PATH \n')
machine1jobs.write('ts -S 7 \n')
machine2jobs.write('cd %(cwd)s \n'%vars())
machine2jobs.write('source /vols/cms/grid/setup.sh \n')
machine2jobs.write('export PATH=/home/hep/amd12/bin/:$PATH \n')
machine2jobs.write('ts -S 7 \n')

for ind in range(0, len(lines)):
  for i in range (0, nfiles[lines[ind].split()[0]]) :
    jobname=lines[ind].split()[0]
    if nsubmitted<n_machine1:
      machine1jobs.write("eval '%(cwd)s/scripts/submit_ts_job.sh jobs/%(jobname)s-%(i)d.sh' \n"%vars())
    else :
      machine2jobs.write("eval '%(cwd)s/scripts/submit_ts_job.sh jobs/%(jobname)s-%(i)d.sh' \n"%vars())
    nsubmitted+=1

machine1jobs.write('exit')
machine2jobs.write('exit')
machine1jobs.close()
machine2jobs.close()
os.system('chmod +x ./jobs/submit_to_machine_1.sh')
os.system('chmod +x ./jobs/submit_to_machine_2.sh')


out_1 = subprocess.check_output(["ssh","%(MACHINE1)s"%vars(),"%(cwd)s/jobs/submit_to_machine_1.sh"%vars()])
print out_1
out_2 = subprocess.check_output(["ssh","%(MACHINE2)s"%vars(),"%(cwd)s/jobs/submit_to_machine_2.sh"%vars()])
print out_2
