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
                  help="A comma separated list of channels to ignore.  Supported channels: For data_2015 %(CHANNELS_2015)s" % vars())

parser.add_option("--tp", dest="proc_tp", action='store_true', default=False,
                  help="Process t&p samples")

parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

channels = options.channels

CONFIG='scripts/Zll_config.json'
#CONFIG='scripts/WMuNu_config.json'

FILELIST='filelists/Nov19_MC_74X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

BACKUPNAME=options.slbackupname

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")

if options.proc_data or options.proc_all:
  data_samples = [
   'SingleMuon-2015D-Oct05',
   'SingleElectron-2015D-Oct05',
   'MuonEG-2015D-Oct05',
   'Tau-2015D-Oct05',
   'SingleMuon-2015D-promptv4',
   'SingleElectron-2015D-promptv4',
   'MuonEG-2015D-promptv4',
   'Tau-2015D-promptv4'
  ]
  DATAFILELIST="./filelists/Nov19_Data_74X"

  for sa in data_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/adewit/Nov19_Data_74X/\",\"sequences\":{\"zee\":[],\"zmm\":[],\"tpzmm\":[],\"tpzee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
      nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 30 
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


if options.proc_bkg or options.proc_all or options.proc_tp:
  central_samples = [
    #'TTJets',
    'TT-ext',
    'WJetsToLNu-LO',
#		'WJetsToLNu',
    #'WWinclusive',
    #'ZZinclusive',
    #'WZinclusive',
    'ZZTo4L',
    'VVTo2L2Nu',
    'ZZTo2L2Q',
    'WWTo1L1Nu2Q',
    'WZJetsTo3LNu',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    'T-t',
    'Tbar-t',
    'T-tW',
    'Tbar-tW',
    #'T-t',
    #'Tbar-t'
#    'WZTo1L1Nu2Q',
#    'WWTo2L2Nu',
#    'WWTo4Q',
#    'WWToLNuQQ',
#    'ZZTo4L',
#    'DYJetsToLL',
    'DYJetsToLL_M-50-LO',
    'DYJetsToLL_M-50_HT100-200',
    'DYJetsToLL_M-50_HT200-400',
    'DYJetsToLL_M-50_HT400-600',
    'DYJetsToLL_M-50_HT600-Inf',
#    'DYJetsToLL_M-50_HT100-200',
#    'WJetsToLNu_HT100-200',
#    'WJetsToLNu_HT200-400',
#    'WJetsToLNu_HT400-600',
#    'WJetsToLNu_HT600-inf'
    'WJetsToLNu_HT100-200',
    'WJetsToLNu_HT200-400',
    'WJetsToLNu_HT400-600',
    'WJetsToLNu_HT600-Inf',
     ]
      
  tandp_samples = [
    'DYJetsToLL_M-50-LO'
     ]

  if not options.proc_tp:
    for sa in central_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"zee\":[],\"zmm\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 30 
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
  else:
    #for sa in tandp_samples:
    for sa in central_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"zee\":[],\"zmm\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 30 
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))



