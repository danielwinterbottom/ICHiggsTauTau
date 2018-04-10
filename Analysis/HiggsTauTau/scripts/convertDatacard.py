#!/usr/bin/env python
#This script drops unnecessary subdirectories from the shape files - the original
#input file is saved under the same name but with _full_file appended
import ROOT
import math
import argparse
import json
import sys
import os
import fnmatch
from UserCode.ICHiggsTauTau.analysis import *
ROOT.TH1.AddDirectory(False)

def getHistogramAndWriteToFile(infile,outfile,dirname,write_dirname):
    directory = infile.Get(dirname)
    for key in directory.GetListOfKeys():
        histo = directory.Get(key.GetName())
        if not isinstance(histo,ROOT.TDirectory):
            WriteToTFile(histo, outfile, write_dirname+"/"+key.GetName())

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'File from which subdirectories need to be dropped')
args = parser.parse_args()
filename = args.file
newfilename=filename.replace('.root','_full_file.root')

os.system('mv %(filename)s %(newfilename)s'%vars())

original_file = ROOT.TFile(newfilename)
output_file = ROOT.TFile(filename,"RECREATE")

for key in original_file.GetListOfKeys():
    if isinstance(original_file.Get(key.GetName()),ROOT.TDirectory):
        if(key.GetName()=="em_ttbar_cr"): 
            dirname="ttbar_cr"
        else :
            dirname=key.GetName()
        getHistogramAndWriteToFile(original_file,output_file,key.GetName(),dirname)
