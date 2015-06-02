#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math

FILELIST = './filelists/May9_validation.dat'

nfiles = sum(1 for line in open('./filelists/May9_validation.dat'))

nperjob = 1

for i in range (0,int(math.ceil(nfiles/nperjob))) :
  os.system("./bin/HTT --cfg=scripts/config.json --offset=%(i)d --nlines=%(nperjob)d" %vars())

