import ROOT
import sys
import os

file_names = {}
channels = ['tttt']
years = ['2016','2017','2018']
for file_name in os.listdir(sys.argv[1]):
    if '.root' not in file_name:
        continue
    for year in years:
        for channel in channels:
            if "_{}_{}.root".format(channel, year) in file_name:
                key = file_name.replace("_{}_{}.root".format(channel, year),"")
                file_names[key] = file_name

for f in file_names:
    if "SingleMuon" in f or "SingleElectron" in f or "EGamma" in f or f in "MuonEG" or\
            "\bTau" in f: 
        continue
        input_file = ROOT.TFile(sys.argv[1]+'/'+file_names[f])
        tree = input_file.Get("ntuple")
        entries = tree.GetEntries()
        print("{}  {}".format(f,entries))
    else:
        input_file = ROOT.TFile(sys.argv[1]+'/'+file_names[f])
        tree = input_file.Get("effective")
        if 'JJH' in f and 'Higgs' not in f: entries = tree.GetEntries('wt>0&&wt_cp_prod_sm!=0') - tree.GetEntries('wt<0&&wt_cp_prod_sm!=0') 
        else: entries = tree.GetEntries('wt>0') - tree.GetEntries('wt<0')
        print("{}  {}".format(f,entries))
