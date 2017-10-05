#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math
import json

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT


def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))


parser = OptionParser()
parser.add_option("--wrapper", dest="wrapper",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--submit", dest="submit",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --submit option overrides both the default and the environment variable. " % vars())



(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

flatjsonlistdysig = []
flatjsonlistdysig.append("job:sequences:all:^default^")

FLATJSONPATCHDYSIG = ''.join(flatjsonlistdysig)
 

CONFIG='scripts/config2016.json'
  
FILELIST='filelists/Oct04_MC_80X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]


file_persamp = open("./jobs/files_per_sample.txt", "w")


signal_mc = [
   'bbH700_private',
   'bbH700_official',
   'bbH700_NewAcc',
   'bbH700_OldAcc'
   ]
  

for sa in signal_mc:
  JOB='%s_2016' % (sa)
  JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
  FLATJSONPATCH=FLATJSONPATCHDYSIG
  if os.path.exists('%(FILELIST)s_%(sa)s.dat' %vars()):
    nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
    nperjob = 50
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      
      



