#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT


CHANNELS_2015 = ['zee', 'zmm','wmnu','tp-zee','tp-zmm']
#CHANNELS_2011 = ['et', 'mt', 'em']
CHANNELS      = [ ] ## will be assigned from 2011 or 2012

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))


parser = OptionParser()
parser.add_option("--wrapper", dest="wrapper",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--submit", dest="submit",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --submit option overrides both the default and the environment variable. " % vars())


#parser.add_option("-e", "--era", dest="era",
#                  help="Specify a data-taking era to process. This option both sets the config files that will be used, "
#                  "e.g. scripts/ERA_2012.cfg, and selects the appropriate data filelists. "
#                  "The following options are supported by default: For 2012 %(SUPPORTED_ERAS_2012)s, for 2011 %(SUPPORTED_ERAS_2011)s" % vars(), metavar="ERA")

parser.add_option("--data", dest="proc_data", action='store_true', default=False,
                  help="Process data samples (including embedded)")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--all", dest="proc_all", action='store_true', default=False,
                  help="Process all samples")

parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of channels to ignore.  Supported channels: For data_2015 %(CHANNELS_2015)s" % vars())


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

channels = options.channels

CONFIG='scripts/Zll_config.json'
#CONFIG='scripts/WMuNu_config.json'

FILELIST='filelists/Oct15_MC_74X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("rm ./jobs/files_per_sample.txt")

file_persamp = open("./jobs/files_per_sample.txt", "w")

if options.proc_data or options.proc_all:
  data_samples = [
   'SingleMuon-2015D-prompt',
   'MuonEG-2015D-prompt',
   'SingleElectron-2015D-prompt',
   'Tau-2015D-prompt'
  ]
  DATAFILELIST="./filelists/Sep22_Data_74X"

  for sa in data_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/adewit/Sep22_Data_74X/\",\"sequences\":{\"zee\":[],\"zmm\":[],\"tpzmm\":[],\"tpzee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
      nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 30 
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


if options.proc_bkg or options.proc_all:
  central_samples = [
#    'TTJets',
    'TT',
    'WJetsToLNu',
    'WWinclusive',
    'ZZinclusive',
    'WZinclusive',
   # 'QCDFlat',
#    'QCDMuEnr',
    'T-tW',
    'Tbar-tW',
#    'WZTo1L1Nu2Q',
#    'WWTo2L2Nu',
#    'WWTo4Q',
#    'WWToLNuQQ',
#    'ZZTo4L',
    'DYJetsToLL'
     ]
      
  tandp_samples = [
    'DYJetsToLL'
     ]

for channel in channels:
      if 'tp' in channel:  
          for sa in tandp_samples:
              JOB='%s_2015' % (sa)
              JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"tpzee\":[],\"tpzmm\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
              nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
              nperjob = 30 
              for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
                os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
              file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      else:
          for sa in central_samples:
              JOB='%s_2015' % (sa)
              JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"zee\":[],\"zmm\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
              nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
              nperjob = 30 
              for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
                os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
              file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))



