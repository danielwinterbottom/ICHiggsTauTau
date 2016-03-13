#!/usr/bin/env python

import fnmatch
import os
import sys
import subprocess
from optparse import OptionParser


parser = OptionParser()
parser.add_option("-i","--input_folder", dest="input",
                  help="Scan the specified folder recursively looking for svfit input files." % vars())


(options, args) = parser.parse_args()

dcache_path = ""#Set this to the dcache folder you want to copy files to

for root, dirnames, filenames in os.walk(options.input):
  for filename in fnmatch.filter(filenames, '*svfit_*_input.root'):
    if len(root.split('/'))>1:
      dcache_sdir = root.split('/')[-1]
      total_path = '%(dcache_path)s/%(dcache_sdir)s/'%vars()
    else:
      total_path = dcache_path
    fullfile = os.path.join(root, filename)
    os.system('lcg-cp %(fullfile)s %(total_path)s/%(filename)s'%vars())
    #try:
    #  output  = subprocess.check_output('lcg-cp %(fullfile)s %(total_path)s/%(filename)s'%vars(),stderr=subprocess.STDOUT,shell=True)
    #except subprocess.CalledProcessError as grepexc:
    #  print "error code", grepexc.returncode, grepexc.output
    #  while 'zero' in grepexc.output:
    #    try:
    #      output  = subprocess.check_output('lcg-cp %(fullfile)s %(total_path)s/%(filename)s'%vars(),stderr=subprocess.STDOUT,shell=True)
    #    except subprocess.CalledProcessError as grepexc:
    #      print "error code", grepexc.returncode, grepexc.output
