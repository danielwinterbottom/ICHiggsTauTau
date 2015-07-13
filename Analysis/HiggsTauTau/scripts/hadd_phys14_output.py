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
	'WZJetsTo3LNu',
	'WJetsToLNu',
	'T-tW',
	'Tbar-tW',
	'DYJetsToTauTau',
	'DYJetsToLL',
	'DYJetsToLL-L',
	'DYJetsToLL-J',
  'DYJetsToTauTau-L',
	'DYJetsToTauTau-JJ',
	'GluGluToHToTauTau_M-125',
	'VBF_HToTauTau_M-125'
	]

channel = ['em','et','mt','tt']
with open("jobs/files_per_sample.txt","r") as inf:
  lines = inf.readlines()

nfiles={}

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])

for sa in sample_list:
  for ch in channel:
    if os.path.isfile('./output/%(outputf)s/%(sa)s_2014_%(ch)s_0.root'%vars()):
      if len(fnmatch.filter(os.listdir('./output/%(outputf)s'%vars()),'%(sa)s_2014_%(ch)s_*'%vars())) == nfiles["%(sa)s_2014"%vars()]:
        os.system('hadd ./output/%(outputf)s/%(sa)s_%(ch)s_2014.root ./output/%(outputf)s/%(sa)s_2014_%(ch)s_* &> ./haddout.txt'% vars()) 
        filetext = open("./haddout.txt").read()
        if 'Warning' in filetext or 'Error' in filetext:
          print "Hadd had a problem:"
          print filetext 
        else :
          os.system('rm ./output/%(outputf)s/%(sa)s_2014_%(ch)s_*' %vars())
      else :
        print "Incorrect number of files for sample %(sa)s_2014_%(ch)s!"%vars()
    if os.path.isfile('./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2014_%(ch)s_0.root'%vars()):
      if len(fnmatch.filter(os.listdir('./output/%(outputf)s/TSCALE_DOWN'%vars()),'%(sa)s_2014_%(ch)s_*'%vars())) == nfiles["%(sa)s_2014"%vars()]:
        os.system('hadd ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_%(ch)s_2014.root ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2014_%(ch)s_* &> ./haddout.txt'% vars()) 
        filetext = open("./haddout.txt").read()
        if 'Warning' in filetext or 'Error' in filetext:
          print "Hadd had a problem:"
          print filetext 
        else :
          os.system('rm ./output/%(outputf)s/TSCALE_DOWN/%(sa)s_2014_%(ch)s_*' %vars())
      else :
        print "Incorrect number of files!"
    if os.path.isfile('./output/%(outputf)s/TSCALE_UP/%(sa)s_2014_%(ch)s_0.root'%vars()):
      if len(fnmatch.filter(os.listdir('./output/%(outputf)s/TSCALE_UP'%vars()),'%(sa)s_2014_%(ch)s_*'%vars())) == nfiles["%(sa)s_2014"%vars()]:
        os.system('hadd ./output/%(outputf)s/TSCALE_UP/%(sa)s_%(ch)s_2014.root ./output/%(outputf)s/TSCALE_UP/%(sa)s_2014_%(ch)s_* &> ./haddout.txt'% vars()) 
        filetext = open("./haddout.txt").read()
        if 'Warning' in filetext or 'Error' in filetext:
          print "Hadd had a problem:"
          print filetext 
        else :
          os.system('rm ./output/%(outputf)s/TSCALE_UP/%(sa)s_2014_%(ch)s_*' %vars())
      else :
        print "Incorrect number of files!"

