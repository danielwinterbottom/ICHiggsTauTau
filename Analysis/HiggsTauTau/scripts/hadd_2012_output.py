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
          'WJetsToLNuSoup',
          'TTJetsFullLept',
          'TTJetsSemiLept',
          'TTJetsHadronicExt',
          'WWJetsTo2L2Nu',
          'WZJetsTo2L2Q',
          'WZJetsTo3LNu',
          'ZZJetsTo2L2Nu',
          'ZZJetsTo2L2Q',
          'ZZJetsTo4L',
          'T-tW',
          'Tbar-tW',
          'DYJetsToLL-JSoup',
          'DYJetsToLL-LSoup',
          'DYJetsToTauTauSoup',
          'DYJetsToTauTau-JJSoup',
          'DYJetsToLLSoup',
          'DYJetsToTauTau-LSoup',
          'GluGluToHToTauTau_M-125',
          'VBF_HToTauTau_M-125', 
          'WH_ZH_TTH_HToTauTau_M-125',
          'VBF_HToWWTo2LAndTau2Nu_M-125',
          'WH_HToTauTau_M-125',
          'ZH_HToTauTau_M-125',
          'Data',
          'Special_3_Data',
          'Special_4_Data',
          'Special_5_WJetsToLNuSoup',
          'Embedded'
	]

channel = ['em','et','mt','tt']

for sa in sample_list:
  for ch in channel:
    if os.path.isfile('./output/%(outputf)s/%(sa)s_2012_%(ch)s_0.root'%vars()):
      print "Hadding %(sa)s_%(ch)s"%vars()
      os.system('hadd -f ./output/%(outputf)s/%(sa)s_%(ch)s_2012.root ./output/%(outputf)s/%(sa)s_2012_%(ch)s_* &> ./haddout.txt'% vars()) 
      filetext = open("./haddout.txt").read()
      if 'Warning' in filetext or 'Error' in filetext:
        print "Hadd had a problem:"
        print filetext 
      else :
        os.system('rm ./output/%(outputf)s/%(sa)s_2012_%(ch)s_*' %vars())
    if os.path.isfile('./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2012_%(ch)s_0.root'%vars()):
      os.system('hadd ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_%(ch)s_2012.root ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2012_%(ch)s_* &> ./haddout.txt'% vars()) 
      filetext = open("./haddout.txt").read()
      if 'Warning' in filetext or 'Error' in filetext:
        print "Hadd had a problem:"
        print filetext 
      else :
        os.system('rm ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2012_%(ch)s_*' %vars())
    if os.path.isfile('./output/%(outputf)s/TSCALE_UP/%(sa)s_2012_%(ch)s_0.root'%vars()):
      os.system('hadd ./output/%(outputf)s/TSCALE_UP/%(sa)s_%(ch)s_2012.root ./output/%(outputf)s/TSCALE_UP/%(sa)s_2012_%(ch)s_* &> ./haddout.txt'% vars()) 
      filetext = open("./haddout.txt").read()
      if 'Warning' in filetext or 'Error' in filetext:
        print "Hadd had a problem:"
        print filetext 
      else :
        os.system('rm ./output/%(outputf)s/TSCALE_UP/%(sa)s_2012_%(ch)s_*' %vars())

