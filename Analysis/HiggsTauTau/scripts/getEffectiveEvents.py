import ROOT
import math
import argparse
import json
import sys
import os
import fnmatch
from UserCode.ICHiggsTauTau.analysis import *

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'File where NLO get effective outputs are stored')
args = parser.parse_args()
dirname = args.file
file_names = {}
for file_name in os.listdir(dirname):
    if '_et_2016.root' in file_name: key = file_name.replace('_et_2016.root','')
    if '_mt_2016.root' in file_name: key = file_name.replace('_mt_2016.root','')
    if '_tt_2016.root' in file_name: key = file_name.replace('_tt_2016.root','')
    if '_em_2016.root' in file_name: key = file_name.replace('_em_2016.root','')
    file_names[key] = file_name


out_string=''

for f in file_names:
  input_file = ROOT.TFile(dirname+'/'+file_names[f])
  tree = input_file.Get("effective")
  tree.Draw("rand()>>total_hist",'wt',"goff")
  total_hist = tree.GetHistogram()
  entries = total_hist.Integral(-1,-1) 
  out_string+=f+"  "+str(entries)+"\n"  

print out_string
