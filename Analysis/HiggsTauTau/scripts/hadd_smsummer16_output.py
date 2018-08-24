#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import math
import fnmatch

parser = OptionParser()

parser.add_option("--folder", dest = "folder",
                  help="Specify folder that contains the output to be hadded")
parser.add_option("--ignore_nfiles", dest= "ignore", default=False, action='store_true',
                  help="Ignore number of files per sample")
parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample.txt",
                  help="list of files per sample you want to use for hadding")
parser.add_option("--batch", dest= "batch", default=False, action='store_true',
                  help="Submit as batch jobs")


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
samplelist = options.samplelist
ignore = options.ignore
batch = options.batch

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0"'


sample_list = [
   'GluGluToHToTauTau_M-110',
   'GluGluToHToTauTau_M-120',
   'GluGluToHToTauTau_M-125',
   'GluGluToHToTauTau_M-130',
   'GluGluToHToTauTau_M-140',
   'TTHToTauTau_M-120',
   'TTHToTauTau_M-125',
   'TTHToTauTau_M-130',
   'VBFHToTauTau_M-110',
   'VBFHToTauTau_M-120',
   'VBFHToTauTau_M-125',
   'VBFHToTauTau_M-130',
   'VBFHToTauTau_M-140',
   'WminusHToTauTau_M-110',
   'WminusHToTauTau_M-120',
   'WminusHToTauTau_M-125',
   'WminusHToTauTau_M-130',
   'WminusHToTauTau_M-140',
   'WplusHToTauTau_M-110',
   'WplusHToTauTau_M-120',
   'WplusHToTauTau_M-125',
   'WplusHToTauTau_M-130',
   'WplusHToTauTau_M-140',
   'ZHToTauTau_M-110',
   'ZHToTauTau_M-120',
   'ZHToTauTau_M-125',
   'ZHToTauTau_M-130',
   'ZHToTauTau_M-140',
   'GluGluHToWWTo2L2Nu_M-125',
   'VBFHToWWTo2L2Nu_M-125',
   'QCDMuEnrichedPt15',
   'QCDEM_Pt120to170',
   'QCDEM_Pt170to300',
   'QCDEM_Pt20to30',
   'QCDEM_Pt300toInf',
   'QCDEM_Pt30to50',
   'QCDEM_Pt50to80',
   'QCDEM_Pt80to120',
   'QCDMu5_Pt1000toInf',
   'QCDMu5_Pt120to170',
   'QCDMu5_Pt15to20',
   'QCDMu5_Pt170to130',
   'QCDMu5_Pt20to30',
   'QCDMu5_Pt300to470',
   'QCDMu5_Pt30to50',
   'QCDMu5_Pt470to600',
   'QCDMu5_Pt50to80',
   'QCDMu5_Pt600to800',
   'QCDMu5_Pt800to1000',
   'QCDMu5_Pt80to120',
   'TT',
   'WJetsToLNu',
   'WJetsToLNu-LO',
   'WJetsToLNu-LO-ext',
   'VVTo2L2Nu',
   'VVTo2L2Nu-ext1',
   'ZZTo2L2Q',
   'ZZTo4L',
   'ZZTo4L-amcat',
   'WWTo1L1Nu2Q',
   'WWToLNuQQ',
   'WWToLNuQQ-ext',
   'WZJToLLLNu',
   'WZTo1L3Nu',
   'WZTo2L2Q',
   'WZTo1L1Nu2Q',
   'T-t',
   'Tbar-t',
   'T-tW',
   'Tbar-tW',
   'DYJetsToLL',
   'DYJetsToLL-LO',
   'DYJetsToLL-LO-ext1',
   'DYJetsToLL-LO-ext2',
   'DYJetsToLL_M-10to50-ext',
   'DYJetsToLL_M-10to50',
   'DYJetsToLL_M-10-50-LO',
   'DY1JetsToLL_M-10-50-LO',
   'DY2JetsToLL_M-10-50-LO',
   'DY3JetsToLL_M-10-50-LO',
   'DYJetsToLL_M-150-LO',
   'DY1JetsToLL-LO',
   'DY2JetsToLL-LO',
   'DY3JetsToLL-LO',
   'DY4JetsToLL-LO',
   'W1JetsToLNu-LO',
   'W2JetsToLNu-LO',
   'W2JetsToLNu-LO-ext',
   'W3JetsToLNu-LO',
   'W3JetsToLNu-LO-ext',
   'W4JetsToLNu-LO',
   'W4JetsToLNu-LO-ext1',
   'W4JetsToLNu-LO-ext2',
   'WGToLNuG',
   'WGToLNuG-ext',
   'WGstarToLNuEE',
   'WGstarToLNuMuMu',
   'SingleMuonB',
   'SingleElectronB',
   'MuonEGB',
   'TauB',
   'SingleMuonC',
   'SingleElectronC',
   'MuonEGC',
   'TauC',
   'SingleMuonD',
   'SingleElectronD',
   'MuonEGD',
   'TauD',
   'SingleMuonE',
   'SingleElectronE',
   'MuonEGE',
   'TauE',
   'SingleMuonF',
   'SingleElectronF',
   'MuonEGF',
   'TauF',
   'SingleMuonG',
   'SingleElectronG',
   'MuonEGG',
   'TauG',
   'SingleMuonHv2',
   'SingleElectronHv2',
   'MuonEGHv2',
   'TauHv2',
   'SingleMuonHv3',
   'SingleElectronHv3',
   'MuonEGHv3',
   'TauHv3',
   'EWKWMinus2Jets_WToLNu-ext1',
   'EWKWMinus2Jets_WToLNu-ext2',
   'EWKWMinus2Jets_WToLNu',
   'EWKWPlus2Jets_WToLNu-ext1',
   'EWKWPlus2Jets_WToLNu-ext2',
   'EWKWPlus2Jets_WToLNu',
   'EWKZ2Jets_ZToLL-ext',
   'EWKZ2Jets_ZToLL',
   'EWKZ2Jets_ZToNuNu-ext',
   'EWKZ2Jets_ZToNuNu',
   'GluGluH2JetsToTauTau_M125_CPmixing_pseudoscalar',
   'GluGluH2JetsToTauTau_M125_CPmixing_maxmix',
   'GluGluH2JetsToTauTau_M125_CPmixing_sm',
   'VBFHiggs0M_M-125',
   'VBFHiggs0Mf05ph0_M-125',
   'VBFHiggs0PM_M-125',
   'GluGluToHToTauTau_amcNLO_M-125',
   'VBFHToTauTau_amcNLO_M-125',
   'EmbeddingMuTauB',
   'EmbeddingMuTauC',
   'EmbeddingMuTauD',
   'EmbeddingMuTauE',
   'EmbeddingMuTauF',
   'EmbeddingMuTauG',
   'EmbeddingMuTauH',
   'EmbeddingElMuB',
   'EmbeddingElMuC',
   'EmbeddingElMuD',
   'EmbeddingElMuE',
   'EmbeddingElMuF',
   'EmbeddingElMuG',
   'EmbeddingElMuH',
   'EmbeddingElTauB',
   'EmbeddingElTauC',
   'EmbeddingElTauD',
   'EmbeddingElTauE',
   'EmbeddingElTauF',
   'EmbeddingElTauG',
   'EmbeddingElTauH',
   'EmbeddingTauTauB',
   'EmbeddingTauTauC',
   'EmbeddingTauTauD',
   'EmbeddingTauTauE',
   'EmbeddingTauTauF',
   'EmbeddingTauTauG',
   'EmbeddingTauTauH',
   'EmbeddingElElB',
   'EmbeddingElElC',
   'EmbeddingElElD',
   'EmbeddingElElE',
   'EmbeddingElElF',
   'EmbeddingElElG',
   'EmbeddingElElH',
   'EmbeddingMuMuB',
   'EmbeddingMuMuC',
   'EmbeddingMuMuD',
   'EmbeddingMuMuE',
   'EmbeddingMuMuF',
   'EmbeddingMuMuG',
   'EmbeddingMuMuH',
   'ZHiggs0M_M-125',
   'ZHiggs0Mf05ph0_M-125',
   'ZHiggs0PM_M-125',
   'WHiggs0M_M-125',
   'WHiggs0Mf05ph0_M-125',
   'WHiggs0PM_M-125',
   'GluGluHToTauTau_M125_NNLOPS',
   'GluGluToHToTauTau_M125_MG5_PSDown',
   'GluGluToHToTauTau_M125_MG5_PSUp',
   'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
   'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
   'GluGluToHToTauTau_M125_amcatnloFXFX',
   'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
   'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
   'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX'
	]

channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm','tpmt','tpem']
with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

subdirs = ['TSCALE_DOWN','TSCALE_UP','TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN','TSCALE3PRONG_UP','TSCALE3PRONG_DOWN','JES_UP','JES_DOWN', 'BTAG_UP','BTAG_DOWN','BFAKE_UP','BFAKE_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 'EFAKE0PI_UP', 'EFAKE0PI_DOWN', 'EFAKE1PI_UP', 'EFAKE1PI_DOWN','MUFAKE0PI_UP','MUFAKE0PI_DOWN','MUFAKE1PI_UP','MUFAKE1PI_DOWN','METUNCL_UP','METUNCL_DOWN','METCL_UP','METCL_DOWN','MUSCALE_UP','MUSCALE_DOWN','ESCALE_UP','ESCALE_DOWN','JESFULL_DOWN','JESFULL_UP','JESCENT_UP','JESCENT_DOWN','JESHF_UP','JESHF_DOWN','JESRBAL_UP','JESRBAL_DOWN','MET_SCALE_NJETS0_DOWN','MET_SCALE_NJETS0_UP','MET_SCALE_NJETS1_DOWN','MET_SCALE_NJETS1_UP','MET_SCALE_NJETS2_DOWN','MET_SCALE_NJETS2_UP','MET_RES_NJETS0_DOWN','MET_RES_NJETS0_UP','MET_RES_NJETS1_DOWN','MET_RES_NJETS1_UP','MET_RES_NJETS2_DOWN','MET_RES_NJETS2_UP']


nfiles={}

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])
for sa in sample_list:
  command=''
  if batch:
    JOB='jobs/hadd_%s.sh' % sa
    os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
  for ch in channel:
    if os.path.isfile('%(outputf)s/%(sa)s_2016_%(ch)s_0.root'%vars()):
      if "%(sa)s_2016"%vars() in nfiles or ignore==True:
        if ignore==True or len(fnmatch.filter(os.listdir('%(outputf)s'%vars()),'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]:
          if not batch:  
            print "Hadding %(sa)s_%(ch)s"%vars()
            os.system('hadd -f %(outputf)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
            os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
            filetext = open("./haddout.txt").read()
            if 'Warning' in filetext or 'Error' in filetext:
              print "Hadd had a problem:"
              print filetext 
            else :
              os.system('rm %(outputf)s/%(sa)s_2016_%(ch)s_*' %vars())
          else:
            haddout='haddout_%s.txt' % sa 
            command+="echo \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sa)s_2016_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\nif [ \"$(cat %(haddout)s | grep -e Warning -e Error)\" != \"\" ]; then echo \"Hadd had a problem:\"\ncat %(haddout)s ; else \nrm %(outputf)s/%(sa)s_2016_%(ch)s_*; fi\n" % vars()
        else :
          print "Incorrect number of files for sample %(sa)s_2016_%(ch)s!"%vars()
    for sdir in subdirs:
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_0.root'%vars()):
        if "%(sa)s_2016"%vars() in nfiles or ignore==True:
          if ignore ==True or len(fnmatch.filter(os.listdir('%(outputf)s/%(sdir)s'%vars()),'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]:
            if not batch:  
              print "Hadding in subdir %(sdir)s"%vars()
              print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
              os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
              os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
              filetext = open("./haddout.txt").read()
              if 'Warning' in filetext or 'Error' in filetext:
                print "Hadd had a problem:"
                print filetext 
              else :
                os.system('rm %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_*' %vars())
            else:
              haddout='haddout_%s_%s.txt' % (sa,sdir)  
              command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\nif [ \"$(cat %(haddout)s | grep -e Warning -e Error)\" != \"\" ]; then echo \"Hadd had a problem:\"\ncat %(haddout)s ;\nelse rm %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_*; fi\n" % vars()     
          else :
            print "Incorrect number of files for sample %(sa)s_2016_%(ch)s! in %(sdir)s"%vars()

  if batch and command:
    with open(JOB, "a") as file: file.write("\n%s" % command)
    os.system('%(JOBSUBMIT)s %(JOB)s' % vars())

