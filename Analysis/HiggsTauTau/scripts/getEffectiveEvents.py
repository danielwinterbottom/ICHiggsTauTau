import ROOT
import sys
import os

file_names = {}
channels = ['mt']
years = ['2016','2017','2018']
for file_name in os.listdir(sys.argv[1]):
    if '.root' not in file_name:
        continue
    for year in years:
        for channel in channels:
            if "_{}_{}.root".format(channel, year) in file_name:
                key = file_name.replace("_{}_{}.root".format(channel, year),"")
                file_names[key] = file_name

out_string=''

for f in file_names:
    if f in ["SingleMuonA","SingleMuonB","SingleMuonC","SingleMuonD"]:
        input_file = ROOT.TFile(sys.argv[1]+'/'+file_names[f])
        tree = input_file.Get("effective")
        entries = tree.GetEntries()
        print("{}  {}".format(f,entries))
    else:
        input_file = ROOT.TFile(sys.argv[1]+'/'+file_names[f])
        tree = input_file.Get("effective")
        tree.Draw("rand()>>total_hist",'wt',"goff")
        total_hist = tree.GetHistogram()
        entries = total_hist.Integral(-1,-1)
        print("{}  {}".format(f,entries))
