#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import math
import fnmatch

parser = OptionParser()

parser.add_option("--folder", dest = "folder",
                  help="Specify folder in ./output that contains the output to be hadded")


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder

sample_list = [
  'TTJets',
	'TT',
	'WJetsToLNu',
	'T-tW',
	'Tbar-tW',
  'WWinclusive',
  'WZinclusive',
  'ZZinclusive',
  'WWTo2L2Nu',
  'WWTo4Q',
  'WWToLNuQQ',
  'WZTo1L1Nu2Q',
  'ZZTo4L',
#  'QCDFlat',
#  'QCDMuEnr',
#'DYJetsToTauTau',
'DYJetsToLL',
'DYJetsToLL_M-50-LO',
'DYJetsToLL_M-50_HT100-200',
'WJetsToLNu_HT100-200',
'WJetsToLNu_HT200-400',
'WJetsToLNu_HT400-600',
'WJetsToLNu_HT600-inf',
#'DYJetsToLL-L',
#'DYJetsToLL-J',
#'DYJetsToTauTau-L',
#'DYJetsToTauTau-JJ',
#'ZToLL',
#'ZToTauTau',
  'SingleElectron-2015B-prompt',
  'SingleMuon-2015B-prompt',
  'MuonEG-2015B-prompt',
  'Tau-2015B-prompt',
  'SingleElectron-2015C-prompt',
  'SingleMuon-2015C-prompt',
  'MuonEG-2015C-prompt',
  'Tau-2015C-prompt',
  'SingleElectron-2015D-prompt',
  'SingleMuon-2015D-prompt',
  'MuonEG-2015D-prompt',
  'Tau-2015D-prompt',
#	'DYJetsToTauTau10-50',
#	'DYJetsToLL10-50',
#	'DYJetsToLL10-50-L',
#	'DYJetsToLL10-50-J',
#  'DYJetsToTauTau10-50-L',
#	'DYJetsToTauTau10-50-JJ',
	'SUSYGluGluToHToTauTau_M-130',
  'SUSYGluGluToHToTauTau_M-160',
  'SUSYGluGluToHToTauTau_M-500',
  'SUSYGluGluToHToTauTau_M-1000',
  'GluGluHToTauTau_M-125',
  'GluGluHToTauTau_M-130',
  'VBFHToTauTau_M-120',
  'VBFHToTauTau_M-125',
  'VBFHToTauTau_M-130'
	]

channel = ['em','et','mt','tt','zee','zmm','wmnu']
with open("jobs/files_per_sample.txt","r") as inf:
  lines = inf.readlines()

nfiles={}

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])

for sa in sample_list:
  for ch in channel:
    if os.path.isfile('./output/%(outputf)s/%(sa)s_2015_%(ch)s_0.root'%vars()):
      if len(fnmatch.filter(os.listdir('./output/%(outputf)s'%vars()),'%(sa)s_2015_%(ch)s_*'%vars())) == nfiles["%(sa)s_2015"%vars()]:
        print "Hadding %(sa)s_%(ch)s"%vars()
        os.system('hadd -f ./output/%(outputf)s/%(sa)s_%(ch)s_2015.root ./output/%(outputf)s/%(sa)s_2015_%(ch)s_* &> ./haddout.txt'% vars()) 
        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
        filetext = open("./haddout.txt").read()
        if 'Warning' in filetext or 'Error' in filetext:
          print "Hadd had a problem:"
          print filetext 
        else :
          os.system('rm ./output/%(outputf)s/%(sa)s_2015_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files for sample %(sa)s_2015_%(ch)s!"%vars()
#    if os.path.isfile('./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2015_%(ch)s_0.root'%vars()):
#      if len(fnmatch.filter(os.listdir('./output/%(outputf)s/TSCALE_DOWN'%vars()),'%(sa)s_2015_%(ch)s_*'%vars())) == nfiles["%(sa)s_2015"%vars()]:
#        os.system('hadd ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_%(ch)s_2015.root ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2015_%(ch)s_* &> ./haddout.txt'% vars()) 
#        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
#        filetext = open("./haddout.txt").read()
#        if 'Warning' in filetext or 'Error' in filetext:
#j          print "Hadd had a problem:"
#          print filetext 
#        else :
#          os.system('rm ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2015_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files!"
#    if os.path.isfile('./output/%(outputf)s/TSCALE_UP/%(sa)s_2015_%(ch)s_0.root'%vars()):
#      if len(fnmatch.filter(os.listdir('./output/%(outputf)s/TSCALE_UP'%vars()),'%(sa)s_2015_%(ch)s_*'%vars())) == nfiles["%(sa)s_2015"%vars()]:
#        os.system('hadd ./output/%(outputf)s/TSCALE_UP/%(sa)s_%(ch)s_2015.root ./output/%(outputf)s/TSCALE_UP/%(sa)s_2015_%(ch)s_* &> ./haddout.txt'% vars()) 
#        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
#        filetext = open("./haddout.txt").read()
#        if 'Warning' in filetext or 'Error' in filetext:
#          print "Hadd had a problem:"
#          print filetext 
#        else :
#          os.system('rm ./output/%(outputf)s/TSCALE_UP/%(sa)s_2015_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files!"
#
