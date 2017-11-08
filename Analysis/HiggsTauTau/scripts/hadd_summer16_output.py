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


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
samplelist = options.samplelist
ignore = options.ignore


sample_list = [
   'GluGluTpHToTauTau_M-120',
   'GluGluToHToTauTau_M-125',
   'GluGluToHToTauTau_M-130',
   'TTHToTauTau_M-120',
   'TTHToTauTau_M-125',
   'TTHToTauTau_M-130',
   'VBFHToTauTau_M-120',
   'VBFHToTauTau_M-125',
   'VBFHToTauTau_M-130',
   'WminusHToTauTau_M-120',
   'WminusHToTauTau_M-125',
   'WminusHToTauTau_M-130',
   'WplusHToTauTau_M-120',
   'WplusHToTauTau_M-125',
   'WplusHToTauTau_M-130',
   'ZHToTauTau_M-120',
   'ZHToTauTau_M-125',
   'ZHToTauTau_M-130',
   'QCDMuEnrichedPt15',
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
   'SingleMuonBReReco',
   'SingleElectronBReReco',
   'MuonEGBReReco',
   'TauBReReco',
   'SingleMuonCReReco',
   'SingleElectronCReReco',
   'MuonEGCReReco',
   'TauCReReco',
   'SingleMuonDReReco',
   'SingleElectronDReReco',
   'MuonEGDReReco',
   'TauDReReco',
   'SingleMuonEReReco',
   'SingleElectronEReReco',
   'MuonEGEReReco',
   'TauEReReco',
   'SingleMuonFReReco',
   'SingleElectronFReReco',
   'MuonEGFReReco',
   'TauFReReco',
   'SingleMuonGReReco',
   'SingleElectronGReReco',
   'MuonEGGReReco',
   'TauGReReco',
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
   'SUSYGluGluToBBHToTauTau_M-80',
   'SUSYGluGluToBBHToTauTau_M-90',
   'SUSYGluGluToBBHToTauTau_M-100',
   'SUSYGluGluToBBHToTauTau_M-110',
   'SUSYGluGluToBBHToTauTau_M-120',
   'SUSYGluGluToBBHToTauTau_M-130',
   'SUSYGluGluToBBHToTauTau_M-140',
   'SUSYGluGluToBBHToTauTau_M-160',
   'SUSYGluGluToBBHToTauTau_M-180',
   'SUSYGluGluToBBHToTauTau_M-200',
   'SUSYGluGluToBBHToTauTau_M-250',
   'SUSYGluGluToBBHToTauTau_M-300',
   'SUSYGluGluToBBHToTauTau_M-350',
   'SUSYGluGluToBBHToTauTau_M-400',
   'SUSYGluGluToBBHToTauTau_M-450',
   'SUSYGluGluToBBHToTauTau_M-500',
   'SUSYGluGluToBBHToTauTau_M-600',
   'SUSYGluGluToBBHToTauTau_M-700',
   'SUSYGluGluToBBHToTauTau_M-800',
   'SUSYGluGluToBBHToTauTau_M-900',
   'SUSYGluGluToBBHToTauTau_M-1000',
   'SUSYGluGluToBBHToTauTau_M-1200',
   'SUSYGluGluToBBHToTauTau_M-1400',
   'SUSYGluGluToBBHToTauTau_M-1500',
   'SUSYGluGluToBBHToTauTau_M-1600',
   'SUSYGluGluToBBHToTauTau_M-1800',
   'SUSYGluGluToBBHToTauTau_M-2000',
   'SUSYGluGluToBBHToTauTau_M-2300',
   'SUSYGluGluToBBHToTauTau_M-2600',
   'SUSYGluGluToBBHToTauTau_M-2900',
   'SUSYGluGluToBBHToTauTau_M-3200',
   'SUSYGluGluToHToTauTau_M-80',
   'SUSYGluGluToHToTauTau_M-90',
   'SUSYGluGluToHToTauTau_M-100',
   'SUSYGluGluToHToTauTau_M-110',
   'SUSYGluGluToHToTauTau_M-120',
   'SUSYGluGluToHToTauTau_M-130',
   'SUSYGluGluToHToTauTau_M-140',
   'SUSYGluGluToHToTauTau_M-160',
   'SUSYGluGluToHToTauTau_M-180',
   'SUSYGluGluToHToTauTau_M-200',
   'SUSYGluGluToHToTauTau_M-250',
   'SUSYGluGluToHToTauTau_M-300',
   'SUSYGluGluToHToTauTau_M-350',
   'SUSYGluGluToHToTauTau_M-400',
   'SUSYGluGluToHToTauTau_M-450',
   'SUSYGluGluToHToTauTau_M-500',
   'SUSYGluGluToHToTauTau_M-600',
   'SUSYGluGluToHToTauTau_M-700',
   'SUSYGluGluToHToTauTau_M-800',
   'SUSYGluGluToHToTauTau_M-900',
   'SUSYGluGluToHToTauTau_M-1000',
   'SUSYGluGluToHToTauTau_M-1200',
   'SUSYGluGluToHToTauTau_M-1400',
   'SUSYGluGluToHToTauTau_M-1500',
   'SUSYGluGluToHToTauTau_M-1600',
   'SUSYGluGluToHToTauTau_M-1800',
   'SUSYGluGluToHToTauTau_M-2000',
   'SUSYGluGluToHToTauTau_M-2300',
   'SUSYGluGluToHToTauTau_M-2600',
   'SUSYGluGluToHToTauTau_M-2900',
   'SUSYGluGluToHToTauTau_M-3200',
   'SUSYGluGluToBBHToTauTau_M-100-NLO',
  'SUSYGluGluToBBHToTauTau_M-1000-NLO',
  'SUSYGluGluToBBHToTauTau_M-110-NLO',
  'SUSYGluGluToBBHToTauTau_M-120-NLO',
  'SUSYGluGluToBBHToTauTau_M-1200-NLO',
  'SUSYGluGluToBBHToTauTau_M-130-NLO',
  'SUSYGluGluToBBHToTauTau_M-140-NLO',
  'SUSYGluGluToBBHToTauTau_M-1400-NLO',
  'SUSYGluGluToBBHToTauTau_M-160-NLO',
  'SUSYGluGluToBBHToTauTau_M-1600-NLO',
  'SUSYGluGluToBBHToTauTau_M-180-NLO',
  'SUSYGluGluToBBHToTauTau_M-1800-NLO',
  'SUSYGluGluToBBHToTauTau_M-200-NLO',
  'SUSYGluGluToBBHToTauTau_M-2000-NLO',
  'SUSYGluGluToBBHToTauTau_M-2300-NLO',
  'SUSYGluGluToBBHToTauTau_M-250-NLO',
  'SUSYGluGluToBBHToTauTau_M-2600-NLO',
  'SUSYGluGluToBBHToTauTau_M-2900-NLO',
  'SUSYGluGluToBBHToTauTau_M-3200-NLO',
  'SUSYGluGluToBBHToTauTau_M-350-NLO',
  'SUSYGluGluToBBHToTauTau_M-400-NLO',
  'SUSYGluGluToBBHToTauTau_M-450-NLO',
  'SUSYGluGluToBBHToTauTau_M-500-NLO',
  'SUSYGluGluToBBHToTauTau_M-600-NLO',
  'SUSYGluGluToBBHToTauTau_M-700-NLO',
  'SUSYGluGluToBBHToTauTau_M-80-NLO',
  'SUSYGluGluToBBHToTauTau_M-800-NLO',
  'SUSYGluGluToBBHToTauTau_M-90-NLO',
  'SUSYGluGluToBBHToTauTau_M-900-NLO',
   'SUSYGluGluToBBHToTauTau_M-80-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-130-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-200-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-350-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-700-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-1200-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-1800-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-3200-NLO-QshUp',
   'SUSYGluGluToBBHToTauTau_M-80-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-130-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-200-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-350-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-700-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-1200-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-1800-NLO-QshDown',
   'SUSYGluGluToBBHToTauTau_M-3200-NLO-QshDown',
   'EWKWMinus2Jets_WToLNu-ext1',
   'EWKWMinus2Jets_WToLNu-ext2',
   'EWKWMinus2Jets_WToLNu',
   'EWKWPlus2Jets_WToLNu-ext1',
   'EWKWPlus2Jets_WToLNu-ext2',
   'EWKWPlus2Jets_WToLNu',
   'EWKZ2Jets_ZToLL-ext',
   'EWKZ2Jets_ZToLL',
   'EWKZ2Jets_ZToNuNu-ext',
   'EWKZ2Jets_ZToNuNu'
	]

channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm']
with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

subdirs = ['TSCALE_DOWN','TSCALE_UP','TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN','TSCALE3PRONG_UP','TSCALE3PRONG_DOWN','JES_UP','JES_DOWN', 'BTAG_UP','BTAG_DOWN','BFAKE_UP','BFAKE_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 'EFAKE0PI_UP', 'EFAKE0PI_DOWN', 'EFAKE1PI_UP', 'EFAKE1PI_DOWN','JES_1_UP','JES_1_DOWN','JES_2_UP','JES_2_DOWN','JES_3_UP','JES_3_DOWN','JES_4_UP','JES_4_DOWN','JES_5_UP','JES_5_DOWN','JES_6_UP','JES_6_DOWN','JES_7_UP','JES_7_DOWN','JES_8_UP','JES_8_DOWN','JES_9_UP','JES_9_DOWN','JES_10_UP','JES_10_DOWN','JES_11_UP','JES_11_DOWN','JES_12_UP','JES_12_DOWN','JES_13_UP','JES_13_DOWN','JES_14_UP','JES_14_DOWN','JES_15_UP','JES_15_DOWN','JES_16_UP','JES_16_DOWN','JES_17_UP','JES_17_DOWN','JES_18_UP','JES_18_DOWN','JES_19_UP','JES_19_DOWN','JES_20_UP','JES_20_DOWN','JES_21_UP','JES_21_DOWN','JES_22_UP','JES_22_DOWN','JES_23_UP','JES_23_DOWN','JES_24_UP','JES_24_DOWN','JES_25_UP','JES_25_DOWN','JES_26_UP','JES_26_DOWN','JES_27_UP','JES_27_DOWN']

nfiles={}

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])

for sa in sample_list:
  for ch in channel:
    if os.path.isfile('%(outputf)s/%(sa)s_2016_%(ch)s_0.root'%vars()):
      if "%(sa)s_2016"%vars() in nfiles or ignore==True:
        if ignore==True or len(fnmatch.filter(os.listdir('%(outputf)s'%vars()),'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]:
          print "Hadding %(sa)s_%(ch)s"%vars()
          os.system('hadd -f %(outputf)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
          os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
          filetext = open("./haddout.txt").read()
          if 'Warning' in filetext or 'Error' in filetext:
            print "Hadd had a problem:"
            print filetext 
          else :
            os.system('rm %(outputf)s/%(sa)s_2016_%(ch)s_*' %vars())
        else :
          print "Incorrect number of files for sample %(sa)s_2016_%(ch)s!"%vars()
    for sdir in subdirs:
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_0.root'%vars()):
        print "Hadding in subdir %(sdir)s"%vars()
        if "%(sa)s_2016"%vars() in nfiles or ignore==True:
          if ignore ==True or len(fnmatch.filter(os.listdir('%(outputf)s/%(sdir)s'%vars()),'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]:
            print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
            os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
            os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
            filetext = open("./haddout.txt").read()
            if 'Warning' in filetext or 'Error' in filetext:
              print "Hadd had a problem:"
              print filetext 
            else :
              os.system('rm %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_*' %vars())
          else :
            print "Incorrect number of files for sample %(sa)s_2016_%(ch)s! in %(sdir)s"%vars()
#        print "Incorrect number of files for sample %(sa)s_2016_%(ch)s!"%vars()
#    if os.path.isfile('./%(outputf)s/TSCALE_DOWN/%(sa)s_2016_%(ch)s_0.root'%vars()):
#      if len(fnmatch.filter(os.listdir('./%(outputf)s/TSCALE_DOWN'%vars()),'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]:
#        os.system('hadd ./%(outputf)s/TSCALE_DOWN/%(sa)s_%(ch)s_2016.root ./%(outputf)s/TSCALE_DOWN/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
#        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
#        filetext = open("./haddout.txt").read()
#        if 'Warning' in filetext or 'Error' in filetext:
#j          print "Hadd had a problem:"
#          print filetext 
#        else :
#          os.system('rm ./%(outputf)s/TSCALE_DOWN/%(sa)s_2016_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files!"
#    if os.path.isfile('./%(outputf)s/TSCALE_UP/%(sa)s_2016_%(ch)s_0.root'%vars()):
#      if len(fnmatch.filter(os.listdir('./%(outputf)s/TSCALE_UP'%vars()),'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]:
#        os.system('hadd ./%(outputf)s/TSCALE_UP/%(sa)s_%(ch)s_2016.root ./%(outputf)s/TSCALE_UP/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
#        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
#        filetext = open("./haddout.txt").read()
#        if 'Warning' in filetext or 'Error' in filetext:
#          print "Hadd had a problem:"
#          print filetext 
#        else :
#          os.system('rm ./%(outputf)s/TSCALE_UP/%(sa)s_2016_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files!"
#
