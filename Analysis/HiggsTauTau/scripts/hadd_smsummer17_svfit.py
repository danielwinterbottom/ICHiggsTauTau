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


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
batch = options.batch

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0"'

sample_list = [
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY2JetsToLL-LO-ext',
    'DY3JetsToLL-LO',
    'DY3JetsToLL-LO-ext',
    'DY4JetsToLL-LO',
    'DYJetsToLL',
    'DYJetsToLL-LO',
    'DYJetsToLL-LO-ext1',
    'DYJetsToLL-ext',
    'DYJetsToLL_M-10-50-LO',
    'DYJetsToLL_M-10-50-LO-ext1',
    'EWKWMinus2Jets',
    'EWKWPlus2Jets',
    'EWKZ2Jets',
    'EmbeddingElElB',
    'EmbeddingElElC',
    'EmbeddingElElD',
    'EmbeddingElElE',
    'EmbeddingElElF',
    'EmbeddingElMuB',
    'EmbeddingElMuC',
    'EmbeddingElMuD',
    'EmbeddingElMuE',
    'EmbeddingElMuF',
    'EmbeddingElTauB',
    'EmbeddingElTauC',
    'EmbeddingElTauD',
    'EmbeddingElTauE',
    'EmbeddingElTauF',
    'EmbeddingMuMuB',
    'EmbeddingMuMuC',
    'EmbeddingMuMuD',
    'EmbeddingMuMuE',
    'EmbeddingMuMuF',
    'EmbeddingMuTauB',
    'EmbeddingMuTauC',
    'EmbeddingMuTauD',
    'EmbeddingMuTauE',
    'EmbeddingMuTauF',
    'EmbeddingTauTauB',
    'EmbeddingTauTauC',
    'EmbeddingTauTauD',
    'EmbeddingTauTauE',
    'EmbeddingTauTauF',
    'GluGluHToTauTau_M-125',
    'GluGluHToTauTau_M-125-ext',
    'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToHToTauTau_M125_amcatnloFXFX',
    'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
    'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
    'MuonEGB',
    'MuonEGC',
    'MuonEGD',
    'MuonEGE',
    'MuonEGF',
    'SingleElectronB',
    'SingleElectronC',
    'SingleElectronD',
    'SingleElectronE',
    'SingleElectronF',
    'SingleMuonB',
    'SingleMuonC',
    'SingleMuonD',
    'SingleMuonE',
    'SingleMuonF',
    'T-t',
    'T-tW',
    'TTTo2L2Nu',
    'TTToHadronic',
    'TTToSemiLeptonic',
    'TauB',
    'TauC',
    'TauD',
    'TauE',
    'TauF',
    'Tbar-t',
    'Tbar-tW',
    'VBFHToTauTau_M-125',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    'WGToLNuG',
    'WJetsToLNu-LO',
    'WJetsToLNu-LO-ext',
    'WWTo1L1Nu2Q',
    'WWTo2L2Nu',
    'WWToLNuQQ',
    'WWToLNuQQ-ext',
    'WZTo1L1Nu2Q',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo3LNu',
    'WminusHToTauTau_M-125',
    'WplusHToTauTau_M-125',
    'ZHToTauTau_M-125',
    'ZZTo2L2Nu',
    'ZZTo2L2Q',
    'ZZTo2Q2Nu',
    'ZZTo4L',
    'ZZTo4L-ext',
    'ttHToTauTau_M-125',
	]


channel = ['em','et','mt','tt']

subdirs = ['TSCALE_DOWN','TSCALE_UP','TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN','TSCALE3PRONG_UP','TSCALE3PRONG_DOWN','JES_UP','JES_DOWN', 'BTAG_UP','BTAG_DOWN','BFAKE_UP','BFAKE_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 'EFAKE0PI_UP', 'EFAKE0PI_DOWN', 'EFAKE1PI_UP', 'EFAKE1PI_DOWN','MUFAKE0PI_UP','MUFAKE0PI_DOWN','MUFAKE1PI_UP','MUFAKE1PI_DOWN','METUNCL_UP','METUNCL_DOWN','METCL_UP','METCL_DOWN','MUSCALE_UP','MUSCALE_DOWN','ESCALE_UP','ESCALE_DOWN','JESFULL_DOWN','JESFULL_UP','JESCENT_UP','JESCENT_DOWN','JESHF_UP','JESHF_DOWN','JESRBAL_UP','JESRBAL_DOWN','JESRSAMP_UP','JESRSAMP_DOWN','MET_SCALE_NJETS0_DOWN','MET_SCALE_NJETS0_UP','MET_SCALE_NJETS1_DOWN','MET_SCALE_NJETS1_UP','MET_SCALE_NJETS2_DOWN','MET_SCALE_NJETS2_UP','MET_RES_NJETS0_DOWN','MET_RES_NJETS0_UP','MET_RES_NJETS1_DOWN','MET_RES_NJETS1_UP','MET_RES_NJETS2_DOWN','MET_RES_NJETS2_UP','JES_CORR_UP','JES_CORR_DOWN','JES_UNCORR_UP','JES_UNCORR_DOWN','JESFULL_CORR_DOWN','JESFULL_CORR_UP','JESCENT_CORR_UP','JESCENT_CORR_DOWN','JESHF_CORR_UP','JESHF_CORR_DOWN','JESFULL_UNCORR_DOWN','JESFULL_UNCORR_UP','JESCENT_UNCORR_UP','JESCENT_UNCORR_DOWN','JESHF_UNCORR_UP','JESHF_UNCORR_DOWN']
subdirs=['JESFULL_CORR_DOWN','JESFULL_CORR_UP','JESCENT_CORR_UP','JESCENT_CORR_DOWN','JESHF_CORR_UP','JESHF_CORR_DOWN','JESFULL_UNCORR_DOWN','JESFULL_UNCORR_UP','JESCENT_UNCORR_UP','JESCENT_UNCORR_DOWN','JESHF_UNCORR_UP','JESHF_UNCORR_DOWN']

for sa in sample_list:
  sa = 'svfit_'+sa
  command=''
  if batch:
    JOB='jobs/hadd_%s.sh' % sa
    os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
  for ch in channel:
    if glob.glob('%(outputf)s/%(sa)s_2017_%(ch)s_*_*_input.root'%vars()):
      if not batch:  
        print "Hadding %(sa)s_%(ch)s"%vars()
        os.system('hadd -f %(outputf)s/%(sa)s_%(ch)s_2017_input.root %(outputf)s/%(sa)s_2017_%(ch)s_*input.root &> ./haddout.txt'% vars()) 
        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
        filetext = open("./haddout.txt").read()
        if 'Warning' in filetext or 'Error' in filetext:
          print "Hadd had a problem:"
          print filetext 
        else :
          os.system('rm %(outputf)s/%(sa)s_2017_%(ch)s_*input.root' %vars())
      else:
        haddout='haddout_%s.txt' % sa 
        command+="echo \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sa)s_%(ch)s_2017_input.root %(outputf)s/%(sa)s_2017_%(ch)s_*input.root &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\nif [ \"$(cat %(haddout)s | grep -e Warning -e Error)\" != \"\" ]; then echo \"Hadd had a problem:\"\ncat %(haddout)s ; else \nrm %(outputf)s/%(sa)s_2017_%(ch)s_*input.root; fi\n" % vars()
    for sdir in subdirs:
      if glob.glob('%(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*_*_input.root'%vars()):
        if not batch:  
          print "Hadding in subdir %(sdir)s"%vars()
          print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
          os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2017_input.root %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*input.root &> ./haddout.txt'% vars()) 
          os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
          filetext = open("./haddout.txt").read()
          if 'Warning' in filetext or 'Error' in filetext:
            print "Hadd had a problem:"
            print filetext 
          else :
            os.system('rm %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*input.root' %vars())
        else:
          haddout='haddout_%s_%s.txt' % (sa,sdir)  
          command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2017_input.root %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*input.root &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\nif [ \"$(cat %(haddout)s | grep -e Warning -e Error)\" != \"\" ]; then echo \"Hadd had a problem:\"\ncat %(haddout)s ;\nelse rm %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*input.root; fi\n" % vars()     

  if batch and command:
    with open(JOB, "a") as file: file.write("\n%s" % command)
    os.system('%(JOBSUBMIT)s %(JOB)s' % vars())

