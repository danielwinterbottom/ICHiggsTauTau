#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
PREFIXOVERRIDE  = 'root://eoscms//eos/cms/store/user/agilbert/Skims/'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
if "PREFIXOVERRIDE" in os.environ:  PREFIXOVERRIDE  = os.environ["PREFIXOVERRIDE"]  
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT
print "Using input prefix:   " + PREFIXOVERRIDE

SUPPORTED_ERAS_2012 = ['Paper', 'Moriond', 'HCP', 'DOnly']
SUPPORTED_ERAS_2011 = ['Paper', 'Total']
SUPPORTED_ERAS      = [ ] ## Will be assigned from 2011 or 2012

CHANNELS_2012 = ['et', 'mt', 'em', 'etmet', 'mtmet']
CHANNELS_2011 = ['et', 'mt', 'em']
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

#parser.add_option("-p", "--production", dest="production",
#                  help="Specify the ntuple production to use.  The defaults are '%(PRODUCTION_2012)s' for 2012 and '%(PRODUCTION_2011)s' for 2011" % vars())

parser.add_option("-i","--input_prefix", dest="input",
                  help="Specify the path prefix for the skimmed ntuple input. The current prefix is '%(PREFIXOVERRIDE)s'"
                  " Using this option overrides both the default and the environment variable. " % vars())

parser.add_option("-e", "--era", dest="era",
                  help="Specify a data-taking era to process. This option both sets the config files that will be used, "
                  "e.g. scripts/ERA_2012.cfg, and selects the appropriate data filelists. "
                  "The following options are supported by default: For 2012 %(SUPPORTED_ERAS_2012)s, for 2011 %(SUPPORTED_ERAS_2011)s" % vars(), metavar="ERA")

parser.add_option("--data", dest="proc_data", action='store_true', default=False,
                  help="Process data samples (including embedded)")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--sm", dest="proc_sm", action='store_true', default=False,
                  help="Process signal SM mc samples")

parser.add_option("--mssm", dest="proc_mssm", action='store_true', default=False,
                  help="Process signal MSSM mc samples")

parser.add_option("--Hhh", dest="proc_Hhh", action='store_true', default=False,
                  help="Process signal H->hh mc samples")

parser.add_option("--extra_signal_profile", dest="extra_signal_profile", action='store_true', default=False,
                  help="Process extra signal H->hh mc samples for profiling")

parser.add_option("--all", dest="proc_all", action='store_true', default=False,
                  help="Process all samples")

parser.add_option("--short_signal", dest="short_signal", action='store_true', default=False,
                  help="Only process the 125/160 signal samples")

parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of channels to process.  Supported channels: For 2102 %(CHANNELS_2012)s, for 2011 %(CHANNELS_2011)s" % vars())


(options, args) = parser.parse_args()

channels = options.channels

#PRODUCTION = PRODUCTION_2012 if options.production == None else options.production
#PREFIXDATA="--is_data=1 --input_prefix="+PREFIXOVERRIDE+"/"+PRODUCTION+"/Data_53X/"
#PREFIXMC="--is_data=0 --input_prefix="+PREFIXOVERRIDE+"/"+PRODUCTION+"/MC_53X/"
#YR='2012'


#JOB = 'GluGluToHToTauTau_M-125'
#JSONPATCH= r"'{\"job\":{\"channels\":[\"em\",\"et\"]}}'";
#JSONPATCH= r"'{\"job\":{\"channels\":[\"em\",\"et\"]}}'";
#JSONPATCH = "job:channels:em"
#FILELIST = './filelists/May9_validation.dat'
#nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
#nperjob = 1

#for i in range (0,int(math.ceil(nfiles/nperjob))) :
#  os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#  os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())

if options.proc_data or options.proc_all:
  for ch in channels:
   JOB='Data_%s_2012' % (ch)
   FILELIST=('./filelists/June6_Data_53X_Data_Paper_%(ch)s_skim.dat' %vars())
   JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"root://eoscms.cern.ch//eos/cms/store/user/rlane/httskims/June6/Data_53X/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
   nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
   print nfiles
   nperjob = 10
   print (float(nfiles)/float(nperjob))
   for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
    os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())

    if ch in ['et','mt']:
      JOB='Embedded_%s_2012' % (ch)
      FILELIST=('./filelists/June6_Data_53X_Embedded_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"root://eoscms.cern.ch//eos/cms/store/user/rlane/httskims/June6/Data_53X/\",\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true,\"is_data\":true, \"hadronic_tau_selector\":1}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())

    if ch in ['et','mt']:
      JOB='Data_%s_2012' % (ch)
      FILELIST=('./filelists/June6_Data_53X_Special_2_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"root://eoscms.cern.ch//eos/cms/store/user/rlane/httskims/June6/Data_53X/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":3}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/Special_3_%(JOB)s-%(i)d.log" jobs/Special_3_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/Special_3_%(JOB)s-%(i)d.sh' % vars())

    if ch in ['et','mt']:
      JOB='Data_%s_2012' % (ch)
      FILELIST=('./filelists/June6_Data_53X_Special_4_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"root://eoscms.cern.ch//eos/cms/store/user/rlane/httskims/June6/Data_53X/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":4}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/Special_4_%(JOB)s-%(i)d.log" jobs/Special_4_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/Special_4_%(JOB)s-%(i)d.sh' % vars())


#    if ch in ['et', 'mt', 'mtmet']:
#      for sc in scales:
#        JOB='Embedded_%s_%s' % (ch,YR)
#        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s '
#          ' --is_embedded=true --hadronic_tau_selector=1 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
#        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

