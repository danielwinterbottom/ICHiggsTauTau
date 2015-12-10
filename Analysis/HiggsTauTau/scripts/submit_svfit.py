#!/usr/bin/env python
import fnmatch
import os
import ROOT
import sys
from optparse import OptionParser

JOBWRAPPER 			= './scripts/generate_job.sh'
JOBSUBMIT 			= 'true'
if "JOBWRAPPER" in os.environ: 			JOBWRAPPER 			= os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ: 			JOBSUBMIT 			= os.environ["JOBSUBMIT"]
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT

parser = OptionParser()
parser.add_option("--jobwrap", dest="wrap",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--jobsub", dest="sub",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --jobsub option overrides both the default and the environment variable. " % vars())

parser.add_option("-i","--input_folder", dest="input",
                  help="Scan the specified folder recursively looking for svfit input files." % vars())

parser.add_option("--submit", dest="submit", action='store_true', default=False,
                  help="Generate and submit jobs")

parser.add_option("--verify", dest="verify", action='store_true', default=False,
                  help="Run verification of output, if --submit is also set then only jobs failing verification will be resubmitted.")

parser.add_option("--run_legacy", dest="run_legacy", action='store_true', default=False,
                  help="Use legacy SVFit code (off by default)")


(options, args) = parser.parse_args()


if options.wrap: JOBWRAPPER=options.wrap
if options.sub: 	JOBSUBMIT=options.sub

ROOT.gSystem.Load("libFWCoreFWLite")
ROOT.gSystem.Load("libUserCodeICHiggsTauTau")
ROOT.AutoLibraryLoader.enable()

filesSeen = 0
filesVerified = 0


for root, dirnames, filenames in os.walk(options.input):
  for filename in fnmatch.filter(filenames, '*svfit_*_input.root'):
    fullfile = os.path.join(root, filename)
    outfile = fullfile.replace('input.root','output.root')
    print 'Found input file: '+fullfile
    filesSeen += 1
    submitTask = True
    if options.verify:
      if (os.path.isfile(outfile)):
        print 'Found output file: '+outfile
        fin =  ROOT.TFile(fullfile)
        tin = fin.Get("svfit")
        fout =  ROOT.TFile(outfile)
        tout = fout.Get("svfit")
        if fout and tout:
          if tin.GetEntries() == tout.GetEntries():
            print 'Both input and output trees have ' + str(tin.GetEntries()) + ' entries, passed verification'
            submitTask = False
            filesVerified += 1
          else:
            print 'Failed verification, input and output trees with differet numbers of entries!'
          fin.Close()
          fout.Close()
        else:
            print 'Failed verification, unable to open output file'
            fin.Close()
      else:
        print 'Failed verification, output file not found!'

    if submitTask and options.submit:
      job = fullfile.replace('_input.root','.sh')
      if options.run_legacy:
        os.system('%(JOBWRAPPER)s "./bin/SVFitTest %(fullfile)s 1" %(job)s' % vars())
      else:
        os.system('%(JOBWRAPPER)s "./bin/SVFitTest %(fullfile)s" %(job)s' %vars())
      os.system('%(JOBSUBMIT)s %(job)s' % vars())

print 'TOTAL SVFIT FILES:    '+str(filesSeen)
print 'VERIFIED SVFIT FILES: '+str(filesVerified)






