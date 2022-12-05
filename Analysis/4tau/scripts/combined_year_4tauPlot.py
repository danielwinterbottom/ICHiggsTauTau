import argparse
import os
import ROOT
import copy

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Channel to run for', default='mmtt')
parser.add_argument('--extra_name',help= 'Extra name to add to datacards', default='combined')
parser.add_argument('--cat',help= 'Category to run for', default='inclusive')
parser.add_argument('--options',help= 'Options to add to 4tauPlot', default='')
parser.add_argument('--combined_options',help= 'Options to add to 4tauPlot when running combined year', default='')
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

if args.extra_name != "": args.extra_name =  args.extra_name + "_"
dc_name = args.extra_name + args.channel + "_" + args.cat


if args.run_datacards:
  for year, config in year_cfg.items():
    print "python scripts/4tauPlot.py --outputfolder={} --replace_name={}_{} --cfg={} --channel={} --cat={} {}".format(args.outputfolder,dc_name,year,config,args.channel,args.cat,args.options)
    os.system("python scripts/4tauPlot.py --outputfolder={} --replace_name={}_{} --cfg={} --channel={} --cat={} {}".format(args.outputfolder,dc_name,year,config,args.channel,args.cat,args.options))

dir_name = "{}_{}".format(args.channel,args.cat)

rfn = "{}/{}_all.root".format(args.outputfolder,dc_name) 
if os.path.exists(rfn): os.system("rm {}".format(rfn))
cf = ROOT.TFile(rfn,"RECREATE")
cf.mkdir(dir_name)

hist_dict = {}
for year, config in year_cfg.items():
  f = ROOT.TFile("{}/{}_{}.root".format(args.outputfolder,dc_name,year))
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

print "python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}/{}_all.root' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --replace_name={}_all  --channel={} --cat={} {} {}".format(args.outputfolder,args.outputfolder,dc_name,config,dc_name,args.channel,args.cat,args.options,args.combined_options)
os.system("python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}/{}_all.root' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --replace_name={}_all  --channel={} --cat={} {} {}".format(args.outputfolder,args.outputfolder,dc_name,config,dc_name,args.channel,args.cat,args.options,args.combined_options))

