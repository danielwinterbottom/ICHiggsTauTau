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
parser.add_argument('--qsh', help= 'Do parameters for Qsh samples as well',action='store_true')
args = parser.parse_args()
dirname = args.file
file_names={}
bbhmasses = ['80','130','200','350','700','1200','1800']

for mass in bbhmasses :
  file_names['SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO'] = dirname+'/SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO_mt_2016.root'

if args.qsh:
  for mass in bbhmasses :
    file_names['SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshDown'] = dirname+'/SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshUp_mt_2016.root'
    file_names['SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshDown'] = dirname+'/SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshDown_mt_2016.root'

out_string=''
weights=['wt','wt_mur1_muf1','wt_mur1_muf2','wt_mur1_muf0p5','wt_mur2_muf1','wt_mur2_muf2','wt_mur2_muf0p5','wt_mur0p5_muf1','wt_mur0p5_muf2','wt_mur0p5_muf0p5']

for f in file_names:
  out_string+='  \"'+f+'\": {\n    \"xs\": 1,\n'
  for wt in weights:
    input_file = ROOT.TFile(file_names[f])
    tree = input_file.Get("effective")
    tree.Draw("rand()>>total_hist",wt,"goff")
    total_hist = tree.GetHistogram()
    entries = total_hist.Integral(-1,-1) 
    if wt is 'wt': out_string+='    \"evt\": '+str(entries)+'\n'
    else: out_string+='    \"evt_'+wt+'\": '+str(entries)+'\n'
  out_string+='},\n'

print out_string
