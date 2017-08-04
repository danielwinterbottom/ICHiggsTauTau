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
parser.add_argument('--pdf', help= 'Do parameters for PDF weighted samples',action='store_true')
args = parser.parse_args()
dirname = args.file
file_names={}
bbhmasses = ['100','1000','110','120','1200','130','140','1400','160','1600','180','1800','200','2000','2300','250','2600','2900','3200','350','400','450','500','600','700','80','800','90','900']

for mass in bbhmasses :
  file_names['SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO'] = dirname+'/SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO_mt_2016.root'

if args.qsh:
  for mass in bbhmasses :
    file_names['SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshUp'] = dirname+'/SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshUp_mt_2016.root'
    file_names['SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshDown'] = dirname+'/SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshDown_mt_2016.root'

out_string=''
weights=['wt','wt_mur1_muf1','wt_mur1_muf2','wt_mur1_muf0p5','wt_mur2_muf1','wt_mur2_muf2','wt_mur2_muf0p5','wt_mur0p5_muf1','wt_mur0p5_muf2','wt_mur0p5_muf0p5']

for f in file_names:
  out_string+='  \"'+f+'\": {\n'
  input_file = ROOT.TFile(file_names[f])
  tree = input_file.Get("effective")
  for wt in weights:
    if wt is not 'wt' and 'Qsh' in f: continue
    tree.Draw("rand()>>total_hist",wt,"goff")
    total_hist = tree.GetHistogram()
    entries = total_hist.Integral(-1,-1) 
    if wt is 'wt': out_string+='    \"evt\": '+str(entries)+',\n'
    else: out_string+='    \"evt_'+wt+'\": '+str(entries)+',\n'
  if args.pdf:
    for i in range(1, 101): 
      wt = 'wt_pdf_'+str(i) 
      tree.Draw("rand()>>total_hist",'wt*'+wt,"goff")
      total_hist = tree.GetHistogram()
      entries = total_hist.Integral(-1,-1) 
      out_string+='    \"evt_'+wt+'\": '+str(entries)+',\n'
    wt='wt_alphasdown'
    tree.Draw("rand()>>total_hist",'wt*'+wt,"goff")
    total_hist = tree.GetHistogram()
    entries = total_hist.Integral(-1,-1)   
    out_string+='    \"evt_'+wt+'\": '+str(entries)+',\n' 
    wt='wt_alphasup'
    tree.Draw("rand()>>total_hist",'wt*'+wt,"goff")
    total_hist = tree.GetHistogram()
    entries = total_hist.Integral(-1,-1)   
    out_string+='    \"evt_'+wt+'\": '+str(entries)+',\n' 
  out_string+='    \"xs\": 1.0\n'
  out_string+='  },\n'

print out_string
