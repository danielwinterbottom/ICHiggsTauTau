import ROOT
import math
import argparse
import json
import sys
import os
import fnmatch
from UserCode.ICHiggsTauTau.analysis import *

parser = argparse.ArgumentParser()
parser.add_argument('--dirname', '-d',
        help= 'Directory where NLO get effective outputs are stored')
args = parser.parse_args()
file_names = {}
channels = ['et','mt','tt','em','zmm']
years = ['2016','2017']
for file_name in os.listdir(args.dirname):
    if '.root' not in file_name:
        continue
    for year in years:
        for channel in channels:
            if "_{}_{}.root".format(channel, year) in file_name:
                key = file_name.replace("_{}_{}.root".format(channel, year),"")
                file_names[key] = file_name

out_string=''

for f in file_names:
    input_file = ROOT.TFile(args.dirname+'/'+file_names[f])
    tree = input_file.Get("effective")
    tree.Draw("rand()>>total_hist",'wt',"goff")
    total_hist = tree.GetHistogram()
    entries = total_hist.Integral(-1,-1)
    out_string+=f+"  "+str(entries)+"\n"

print out_string
