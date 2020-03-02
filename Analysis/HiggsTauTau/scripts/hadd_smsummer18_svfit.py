#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import math
import fnmatch
import glob

parser = OptionParser()

parser.add_option("--folder", dest = "folder",
                  help="Specify folder that contains the output to be hadded")
parser.add_option("--batch", dest= "batch", default=False, action='store_true',
                  help="Submit as batch jobs")

def list_paths(path):
    directories = []
    for item in os.listdir(path):
      if os.path.isdir(os.path.join(path, item)):
        directories.append(item)
    return directories


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
batch = options.batch

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0"'

sample_list = [
    'DYJetsToLL-2018',
    'SingleMuonA',
    'SingleMuonB',
    'SingleMuonC',
    'SingleMuonD',
    'TauA',
    'TauB',
    'TauC',
    'TauD',
    'EGammaA',
    'EGammaB',
    'EGammaC',
    'EGammaD',
    'MuonEGA',
    'MuonEGB',
    'MuonEGC',
    'MuonEGD',
    'DYJetsToLL',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    'DYJetsToLL-LO',
    'DYJetsToLL_M-10-50-LO',
    'EWKWMinus2Jets',
    'EWKWPlus2Jets',
    'EWKZ2Jets',#new
    'T-t',
    'T-tW-ext1',
    'TTTo2L2Nu',
    'TTToHadronic',
    'TTToSemiLeptonic',
    'Tbar-t',
    'Tbar-tW-ext1',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    'WGToLNuG',
    'WJetsToLNu-LO',
    'WWTo1L1Nu2Q',
    'WWTo2L2Nu',#new
    'WWTo4Q',#new
    'WWToLNuQQ',#new
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo3LNu',#new
    'WZTo3LNu-ext1',
    'ZZTo2L2Nu-ext1',
    'ZZTo2L2Nu-ext2',
    'ZZTo2L2Q',
    'ZZTo4L',
    'ZZTo4L-ext',
    'EmbeddingElElB',
    'EmbeddingElElC',
    'EmbeddingElElD',
    'EmbeddingElElA',
    'EmbeddingElMuB',
    'EmbeddingElMuC',
    'EmbeddingElMuD',
    'EmbeddingElMuA',
    'EmbeddingElTauB',
    'EmbeddingElTauC',
    'EmbeddingElTauD',
    'EmbeddingElTauA',
    'EmbeddingMuMuB',
    'EmbeddingMuMuC',
    'EmbeddingMuMuD',
    'EmbeddingMuMuA',
    'EmbeddingMuTauB',
    'EmbeddingMuTauC',
    'EmbeddingMuTauD',
    'EmbeddingMuTauA',
    'EmbeddingTauTauB',
    'EmbeddingTauTauC',
    'EmbeddingTauTauD',
    'EmbeddingTauTauA',

    'GluGluHToTauTauUncorrelatedDecay',
    'GluGluHToTauTauUncorrelatedDecay_Filtered',
    'GluGluHToTauTau_M-125',
    'GluGluHToWWTo2L2Nu_M-125',
    'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToHToTauTau_M-125-nospinner-filter',
    'GluGluToHToTauTau_M-125-nospinner',
    'GluGluToHToTauTau_M125_amcatnloFXFX',
    'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
    'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
    'JJH0MToTauTauPlusOneJets',
    'JJH0MToTauTauPlusOneJets_Filtered',
    'JJH0MToTauTauPlusTwoJets',
    'JJH0MToTauTauPlusTwoJets_Filtered',
    'JJH0MToTauTauPlusZeroJets',
    'JJH0MToTauTauPlusZeroJets_Filtered',
    'JJH0Mf05ph0ToTauTauPlusOneJets.dat',
    'JJH0Mf05ph0ToTauTauPlusOneJets_Filtered',
    'JJH0Mf05ph0ToTauTauPlusTwoJets',
    'JJH0Mf05ph0ToTauTauPlusTwoJets_Filtered',
    'JJH0Mf05ph0ToTauTauPlusZeroJets',
    'JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered',
    'JJH0PMToTauTauPlusOneJets',
    'JJH0PMToTauTauPlusOneJets_Filtered',
    'JJH0PMToTauTauPlusTwoJets',
    'JJH0PMToTauTauPlusTwoJets_Filtered',
    'JJH0PMToTauTauPlusZeroJets',
    'JJH0PMToTauTauPlusZeroJets_Filtered',
    'JJHiggs0MToTauTau',
    'JJHiggs0Mf05ph0ToTauTau',
    'JJHiggs0PMToTauTau',
    'VBFHToTauTauUncorrelatedDecay',
    'VBFHToTauTauUncorrelatedDecay_Filtered',
    'VBFHToTauTau_M-125-MM-filter',
    'VBFHToTauTau_M-125-PS-filter',
    'VBFHToTauTau_M-125-SM-filter',
    'VBFHToTauTau_M-125-ext1',
    'VBFHToTauTau_M-125-nospinner-filter',
    'VBFHToTauTau_M-126-nospinner',
    'VBFHToWWTo2L2Nu_M-125',
    'VBFHiggs0L1ToTauTau',
    'VBFHiggs0L1ZgToTauTau',
    'VBFHiggs0L1Zgf05ph0ToTauTau',
    'VBFHiggs0L1f05ph0ToTauTau',
    'VBFHiggs0MToTauTau',
    'VBFHiggs0Mf05ph0ToTauTau',
    'VBFHiggs0PHToTauTau',
    'VBFHiggs0PHf05ph0ToTauTau',
    'VBFHiggs0PMToTauTau',
    'WHiggs0L1ToTauTau',
    'WHiggs0L1f05ph0ToTauTau',
    'WHiggs0MToTauTau',
    'WHiggs0Mf05ph0ToTauTau',
    'WHiggs0PHToTauTau',
    'WHiggs0PHf05ph0ToTauTau',
    'WHiggs0PMToTauTau.dat',
    'WminusHToTauTauUncorrelatedDecay',
    'WminusHToTauTauUncorrelatedDecay_Filtered',
    'WminusHToTauTau_M-125',
    'WplusHToTauTauUncorrelatedDecay',
    'WplusHToTauTauUncorrelatedDecay_Filtered',
    'WplusHToTauTau_M-125',
    'ZHToTauTauUncorrelatedDecay_Filtered',
    'ZHToTauTau_M-125',
    'ZHiggs0L1ToTauTau',
    'ZHiggs0L1ZgToTauTau',
    'ZHiggs0L1Zgf05ph0ToTauTau',
    'ZHiggs0L1f05ph0ToTauTau',
    'ZHiggs0MToTauTau',
    'ZHiggs0Mf05ph0ToTauTau',
    'ZHiggs0PHToTauTau',
    'ZHiggs0PHf05ph0ToTauTau',
    'ZHiggs0PMToTauTau',
    'ttHiggs0MToTauTau',
    'ttHiggs0Mf05ph0ToTauTau',
    'ttHiggs0PMToTauTau',

	]

channel = ['em','et','mt','tt']

subdirs=['']
subdirs+=list_paths(outputf)

new_subdirs=[]
for d in subdirs:
  infi=os.listdir('%(outputf)s/%(d)s' % vars())
  if infi: new_subdirs.append((d,infi))
subdirs=new_subdirs

print subdirs

failed = []

for sa in sample_list:
  remove=True
  to_remove=[]
  hadd_dirs=[]
  sa = 'svfit_'+sa
  command=''
  if batch:
    JOB='jobs/hadd_%s.sh' % sa
    os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
  for ch in channel:
    for jsdir in subdirs:
      sdir = jsdir[0]
      infiles=jsdir[1]
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_0_0_input.root'%vars()):
        if not batch:  
          print "Hadding in subdir %(sdir)s"%vars()
          print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
          os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2018_input.root %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*input.root &> ./haddout.txt'% vars()) 
          os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
          filetext = open("./haddout.txt").read()
          if 'Warning' in filetext or 'Error' in filetext:
            print "Hadd had a problem:"
            print filetext
            remove=False 
            failed.append(sa) 
          else :
            to_remove.append('rm %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*input.root' %vars())
        else:
          haddout='haddout_%s_%s_%s.txt' % (sa,ch,sdir) 
          command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2018_input.root %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*input.root &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\nif [ \"$(cat %(haddout)s | grep -e Warning -e Error)\"  == \"\" ]; then rm %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*input.root; fi\n" % vars()    

  if batch and command:
    with open(JOB, "a") as file: 
      file.write("\n%s" % command)
      file.write('\necho End of job &> jobs/hadd_svfit_%(sa)s.log' % vars())
    os.system('%(JOBSUBMIT)s %(JOB)s' % vars())



