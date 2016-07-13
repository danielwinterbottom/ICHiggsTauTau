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
    'TT',
    'WJetsToLNu-LO',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    #'ZZTo4L',
    'VVTo2L2Nu',
    'ZZTo2L2Q',
    'WWTo1L1Nu2Q',
    #'WZJetsToLLLNu',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    #'T-t',
    'Tbar-t',
    'T-tW',
    'Tbar-tW',
    #'DYJetsToLL',
    #'DYJetsToLL_M-10-ext',
    #'DYJetsToLL_M-150-LO',
    'DYJetsToLL-LO',
    #'DYJetsToLL_M-50-LO',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    #'DYJetsToLL-ext4',
    'DYJetsToLL',
    #'DYJetsToLL_M-50_HT100-200',
    #'DYJetsToLL_M-50_HT200-400',
    #'DYJetsToLL_M-50_HT400-600',
    #'DYJetsToLL_M-50_HT600-Inf',
    #'WJetsToLNu_HT100-200',
    #'WJetsToLNu_HT200-400',
    #'WJetsToLNu_HT400-600',
    #'WJetsToLNu_HT600-Inf',
   'SingleMuonB',
   'SingleElectronB',
   'MuonEGB',
   'TauB',
   'SingleMuonC',
   'SingleElectronC',
   'MuonEGC',
   'TauC',
   'SUSYGluGluToHToTauTau_M-160',
   'SUSYGluGluToBBHToTauTau_M-160'
   'SUSYGluGluToHToTauTau_M-500',
   'SUSYGluGluToBBHToTauTau_M-500'
	]

channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm']
with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

subdirs = ['TSCALE_DOWN','TSCALE_UP','JES_UP','JES_DOWN', 'BTAG_UP','BTAG_DOWN','BFAKE_UP','BFAKE_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN']

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
