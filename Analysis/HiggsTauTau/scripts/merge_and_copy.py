#!/usr/bin/env python
import fnmatch
import os
import ROOT
import sys
from optparse import OptionParser
from subprocess import call
import time


parser = OptionParser()
parser.add_option("--prefix-in", dest="prefix_in",
                  help="Input filelist prefix")

parser.add_option("--copy-out", dest="copy_out",
                  help="copy location")

parser.add_option("--prefix-out", dest="prefix_out",
                  help="copy location")

parser.add_option("--files-in", dest="files_in",
                  help="Input filelist")

parser.add_option("--files-out", dest="files_out",
                  help="output filelist")

parser.add_option("--max-size", dest="max_size", type="float",
                  help="max size")

parser.add_option("--local", dest="local",
                  help="Local folder where the hadding should take place")

parser.add_option("--remote", dest="remote", action='store_true', default=False,
                  help="Files are remote and should be copied first")


(options, args) = parser.parse_args()

if options.local:
  os.environ["TMPDIR"] = options.local

if "TMPDIR" not in os.environ:
  os.environ["TMPDIR"] = os.environ["PWD"]

TMPDIR = os.environ["TMPDIR"]

print "-" * 78
print "Merge-and-Copy"
print "Input prefix:          "+options.prefix_in
print "Input filelist:        "+options.files_in
print "Output location(1):    "+options.copy_out
print "Output location(2):    "+options.prefix_out
print "Output filelist:       "+options.files_out
print "Input is remote:       "+str(options.remote)
print "Local merge location:  "+TMPDIR
print "-" * 78

files_in = [ ]

with open(options.files_in,'r') as f:
  for l in f:
    files_in.append(l.strip())

#print files_in

current_size = 0.0

merge_lists = [ ]

merge_list = [ ]

for idx, val in enumerate(files_in):
  if idx % 50 == 0:
    print "Give xrootd a break for a few seconds..."
    time.sleep(10)
  #if idx == 5: break
  #print idx, val
  print "File ["+str(idx+1)+"/"+str(len(files_in))+"]: " + options.prefix_in+val
  fin = ROOT.TFile.Open(options.prefix_in + val)
  if not fin:
    print "File opening failed!"
    break
  size = float(fin.GetSize()) / 1E6
  fin.Close()
  if ((current_size + size) > options.max_size):
    print "flushing with size: " + str(current_size)
    current_size = 0.
    merge_lists.append(merge_list)
    merge_list = [ ]
  merge_list.append(options.prefix_in+val)
  current_size += size

if len(merge_list) > 0:
  print "flushing with size: " + str(current_size)
  merge_lists.append(merge_list)

fout = open(options.files_out, 'w')

for idx, val in enumerate(merge_lists):
  merger = ROOT.TFileMerger(options.remote)
  merger.SetPrintLevel(0)
  outfile = "EventTree_"+str(idx+1)+".root"
  full_tmp = os.path.join(TMPDIR, outfile)
  merger.OutputFile(full_tmp)
  for x in val:
    merger.AddFile(x)
  merger.Merge()
  print ["lcg-cp", "-v", full_tmp, options.copy_out + options.prefix_out + outfile]
  copy_result = call(["lcg-cp", "-v", full_tmp, options.copy_out + options.prefix_out + outfile])
  print "Copy result: " + str(copy_result)
  print ["rm", full_tmp]
  del_result = call(["rm", full_tmp])
  print "delete result: " + str(del_result)
  fout.write(options.prefix_out+outfile+"\n")

fout.close()


