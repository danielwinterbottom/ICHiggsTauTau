#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import math
import fnmatch

parser = OptionParser()

parser.add_option("--folder", dest = "folder",
                  help="Specify folder that contains the output to be hadded")
parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample.txt",
                  help="list of files per sample you want to use for hadding")


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
samplelist = options.samplelist


sample_list = [
  'QCD_Ht100to200',
  'QCD_Ht200to300',
  'QCD_Ht300to500',
  'QCD_Ht500to700',
  'QCD_Ht700to1000',
  'QCD_Ht1000to1500',
  'QCD_Ht1500to2000',
  'QCD_Ht2000toInf',
  'TTJets',
	'TT',
  'TT-ext',
	'WJetsToLNu',
	'WJetsToLNu-LO',
	'T-tW',
	'Tbar-tW',
  'T-t',
  'Tbar-t',
  'WWinclusive',
  'WZinclusive',
  'ZZinclusive',
  'WZJetsTo3LNu',
  'VVTo2L2Nu',
  'WWTo1L1Nu2Q',
  'WWTo2L2Nu',
  'WWTo4Q',
  'WWToLNuQQ',
  'WZTo1L1Nu2Q',
  'WZTo2L2Q',
  'WZTo3LNu',
  'WZTo1L3Nu',
  'ZZTo4L',
  'ZZTo2L2Nu',
  'ZZTo2L2Q',
#  'QCDFlat',
#  'QCDMuEnr',
#'DYJetsToTauTau',
'DYJetsToLL',
'DY1JetsToLL_M-50-LO',
'DY2JetsToLL_M-50-LO',
'DY3JetsToLL_M-50-LO',
'DY4JetsToLL_M-50-LO',
'DYJetsToLL_M-50-LO',
'DYJetsToLL_M-5-LO',
'DYJetsToLL_M-50_HT100-200',
'WJetsToLNu_HT100-200',
'WJetsToLNu_HT200-400',
'WJetsToLNu_HT400-600',
'WJetsToLNu_HT600-Inf',
'DYJetsToLL_M-50_HT100-200',
'DYJetsToLL_M-50_HT200-400',
'DYJetsToLL_M-50_HT400-600',
'DYJetsToLL_M-50_HT600-Inf',
'DYJetsToLL_M-5_HT100-200',
'DYJetsToLL_M-5_HT200-400',
'DYJetsToLL_M-5_HT400-600',
'DYJetsToLL_M-5_HT600-Inf',
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
#  'SingleElectron-2015C-prompt',
#  'SingleMuon-2015C-prompt',
#  'MuonEG-2015C-prompt',
#  'Tau-2015C-prompt',
  'SingleElectron-2015D-prompt',
  'SingleMuon-2015D-prompt',
  'MuonEG-2015D-prompt',
  'Tau-2015D-prompt',
  'SingleElectron-2015D-promptv4',
  'SingleMuon-2015D-promptv4',
  'MuonEG-2015D-promptv4',
  'Tau-2015D-promptv4',
  'SingleElectron-2015D-Oct05',
  'SingleMuon-2015D-Oct05',
  'MuonEG-2015D-Oct05',
  'Tau-2015D-Oct05',
  'SingleElectron-2015D',
  'SingleMuon-2015D',
  'MuonEG-2015D',
  'Tau-2015D',
#	'DYJetsToTauTau10-50',
#	'DYJetsToLL10-50',
#	'DYJetsToLL10-50-L',
#	'DYJetsToLL10-50-J',
#  'DYJetsToTauTau10-50-L',
#	'DYJetsToTauTau10-50-JJ',
	#'SUSYGluGluToHToTauTau_M-130',
  'GluGluToRadionToHHTo2B2Tau_M-250',
  'GluGluToRadionToHHTo2B2Tau_M-260',
  'GluGluToRadionToHHTo2B2Tau_M-270',
  'GluGluToRadionToHHTo2B2Tau_M-280',
  'GluGluToRadionToHHTo2B2Tau_M-300',
  'GluGluToRadionToHHTo2B2Tau_M-320',
  'GluGluToRadionToHHTo2B2Tau_M-340',
  'GluGluToRadionToHHTo2B2Tau_M-350',
  'GluGluToRadionToHHTo2B2Tau_M-400',
  'GluGluToRadionToHHTo2B2Tau_M-450',
  'GluGluToRadionToHHTo2B2Tau_M-500',
  'GluGluToRadionToHHTo2B2Tau_M-550',
  'GluGluToRadionToHHTo2B2Tau_M-600',
  'GluGluToRadionToHHTo2B2Tau_M-650',
  'GluGluToRadionToHHTo2B2Tau_M-700',
  'GluGluToRadionToHHTo2B2Tau_M-800',
  'GluGluToRadionToHHTo2B2Tau_M-900',
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
  'GluGluHToTauTau_M-125',
  'GluGluHToTauTau_M-130',
  'GluGluHToTauTau_M-120',
  'VBFHToTauTau_M-120',
  'VBFHToTauTau_M-125',
  'VBFHToTauTau_M-130'
	]

channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm']
with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

nfiles={}

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])

for sa in sample_list:
  for ch in channel:
    if os.path.isfile('./%(outputf)s/%(sa)s_2015_%(ch)s_0.root'%vars()):
      if "%(sa)s_2015"%vars() in nfiles:
        if len(fnmatch.filter(os.listdir('./%(outputf)s'%vars()),'%(sa)s_2015_%(ch)s_*'%vars())) == nfiles["%(sa)s_2015"%vars()]:
          print "Hadding %(sa)s_%(ch)s"%vars()
          os.system('hadd -f ./%(outputf)s/%(sa)s_%(ch)s_2015.root ./%(outputf)s/%(sa)s_2015_%(ch)s_* &> ./haddout.txt'% vars()) 
          os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
          filetext = open("./haddout.txt").read()
          if 'Warning' in filetext or 'Error' in filetext:
            print "Hadd had a problem:"
            print filetext 
          else :
            os.system('rm ./%(outputf)s/%(sa)s_2015_%(ch)s_*' %vars())
        else :
          print "Incorrect number of files for sample %(sa)s_2015_%(ch)s!"%vars()
#        print "Incorrect number of files for sample %(sa)s_2015_%(ch)s!"%vars()
#    if os.path.isfile('./%(outputf)s/TSCALE_DOWN/%(sa)s_2015_%(ch)s_0.root'%vars()):
#      if len(fnmatch.filter(os.listdir('./%(outputf)s/TSCALE_DOWN'%vars()),'%(sa)s_2015_%(ch)s_*'%vars())) == nfiles["%(sa)s_2015"%vars()]:
#        os.system('hadd ./%(outputf)s/TSCALE_DOWN/%(sa)s_%(ch)s_2015.root ./%(outputf)s/TSCALE_DOWN/%(sa)s_2015_%(ch)s_* &> ./haddout.txt'% vars()) 
#        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
#        filetext = open("./haddout.txt").read()
#        if 'Warning' in filetext or 'Error' in filetext:
#j          print "Hadd had a problem:"
#          print filetext 
#        else :
#          os.system('rm ./%(outputf)s/TSCALE_DOWN/%(sa)s_2015_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files!"
#    if os.path.isfile('./%(outputf)s/TSCALE_UP/%(sa)s_2015_%(ch)s_0.root'%vars()):
#      if len(fnmatch.filter(os.listdir('./%(outputf)s/TSCALE_UP'%vars()),'%(sa)s_2015_%(ch)s_*'%vars())) == nfiles["%(sa)s_2015"%vars()]:
#        os.system('hadd ./%(outputf)s/TSCALE_UP/%(sa)s_%(ch)s_2015.root ./%(outputf)s/TSCALE_UP/%(sa)s_2015_%(ch)s_* &> ./haddout.txt'% vars()) 
#        os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
#        filetext = open("./haddout.txt").read()
#        if 'Warning' in filetext or 'Error' in filetext:
#          print "Hadd had a problem:"
#          print filetext 
#        else :
#          os.system('rm ./%(outputf)s/TSCALE_UP/%(sa)s_2015_%(ch)s_*' %vars())
#      else :
#        print "Incorrect number of files!"
#
