#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

FILELIST = './filelists/May9_validation.dat'
SAMPLEDIR = './splitfilelist_gg'
PATCHDIR = './splitfilelist_patches'


os.system('rm -r %(SAMPLEDIR)s' %vars())
os.system('rm -r %(PATCHDIR)s' %vars())
os.system('mkdir %(SAMPLEDIR)s' %vars())
os.system('mkdir %(PATCHDIR)s' %vars())

os.system('split --lines=1 --numeric-suffixes --suffix-length=3 %(FILELIST)s %(SAMPLEDIR)s/' %vars()) 


FILEIT=0
for filename in os.listdir('%(SAMPLEDIR)s' %vars()) :
  json_patch = open('%(PATCHDIR)s//patch.json' %vars(),"w")
  json_patch.write('{"job":{"filelist":"%(SAMPLEDIR)s//%(filename)s", "output_postfix":"%(FILEIT)s"}}'%vars())
  json_patch.close()
#  os.system(''' '{"job":{"filelist":"%(SAMPLEDIR)s//%(filename)s", "output_postfix":"%(FILEIT)s"}}' > %(PATCHDIR)s//patch.json''' %vars())
  os.system('''./bin/HTT --cfg=scripts/config.json '%(PATCHDIR)s//patch.json' ''' %vars())
  #os.system('''./bin/HTT --cfg=scripts/config.json '{"job":{"filelist":"%(SAMPLEDIR)s//%(filename)s"}}' '{"job":{"output_postfix":"%(FILEIT)s"}}' ''' %vars())
  FILEIT=FILEIT+1



