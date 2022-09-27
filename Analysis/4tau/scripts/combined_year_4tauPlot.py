import argparse
import os
import ROOT
import copy

parser = argparse.ArgumentParser()
parser.add_argument('--options',help= 'Options to add to 4tauPlot', default='')
parser.add_argument('--outputfolder',help= 'Options folder to store plots/datacards', default='output')
parser.add_argument('--run_datacards', help= 'Run datacards', action='store_true')
args = parser.parse_args()

if "--datacard" in args.options or "--cfg" in args.options or "--outputname" in args.options:
  print "ERROR: Cannot provide this option here"
  exit()

if not os.path.isdir(args.outputfolder): os.system("mkdir "+args.outputfolder)

year_cfg = {
            "2016_preVFP" :"scripts/plot_UL_2016_preVFP.cfg",
            "2016_postVFP":"scripts/plot_UL_2016_postVFP.cfg",
            "2017"        :"scripts/plot_UL_2017.cfg",
            "2018"        :"scripts/plot_UL_2018.cfg",
            }

if args.run_datacards:
  for year, config in year_cfg.items():
    os.system("python scripts/4tauPlot.py --outputfolder={} --datacard=combined_year_{} --cfg={} {}".format(args.outputfolder,year,config,args.options))

ch_name = "mmtt"
cat_name = "inclusive"
dir_name = "{}_{}".format(ch_name,cat_name)

cf = ROOT.TFile("{}/combined_year_all.root".format(args.outputfolder),"RECREATE")
cf.mkdir(dir_name)

hist_dict = {}
for year, config in year_cfg.items():
  f = ROOT.TFile("{}/combined_year_{}.root".format(args.outputfolder,year))
  for dkey in f.GetListOfKeys():
    d = f.Get(dkey.GetName())
    fnames = []
    for hkey in d.GetListOfKeys():
      if ".subnodes" not in hkey.GetName() and hkey.GetName() not in fnames:
        fnames.append(hkey.GetName())
        hist = d.Get(hkey.GetName())

        if hkey.GetName() not in hist_dict.keys():          
          hist_dict[hkey.GetName()] = copy.deepcopy(hist.Clone())
        else:
          hist_dict[hkey.GetName()].Add(copy.deepcopy(hist.Clone()))

for key, val in hist_dict.items():
  cd = cf.Get(dir_name)
  cd.cd()
  val.Write()

cf.Close()
cf.Write()

os.system("python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}/combined_year_all.root' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --datacard={} --extra_name={}_combined_year_all {}".format(args.outputfolder,args.outputfolder,config,cat_name,ch_name,args.options))

