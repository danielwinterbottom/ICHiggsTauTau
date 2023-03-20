import argparse
import os
import ROOT
import copy
from array import array

ROOT.TH1.AddDirectory(0)

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Channel to run for', default='mmtt')
parser.add_argument('--extra_name',help= 'Extra name to add to datacards', default='combined')
parser.add_argument('--cat',help= 'Category to run for', default='inclusive')
parser.add_argument('--options',help= 'Options to add to 4tauPlot', default='')
parser.add_argument('--combined_options',help= 'Options to add to 4tauPlot when running combined year', default='')
parser.add_argument('--outputfolder',help= 'Options folder to store plots/datacards', default='output')
parser.add_argument('--run_datacards', help= 'Run datacards', action='store_true')
parser.add_argument('--add_stat_to_syst', help= 'Add statistics to systematics', action='store_true')
parser.add_argument("--auto_rebinning", action='store_true',help="Do auto rebinning")
parser.add_argument("--bin_threshold", type=float, default=100000.0, help="Threshold for bin auto rebin value")
parser.add_argument("--bin_uncert_fraction", type=float, default=0.5, help="Threshold for bin auto rebin fractional uncertainty")
parser.add_argument("--rebin_with_data", action='store_true', help="Use data in the rebinning algorithm")
parser.add_argument("--zero_negative_bins", action='store_true', help="Zero negative bins")
args = parser.parse_args()

if "--datacard" in args.options or "--cfg" in args.options or "--outputname" in args.options:
  print "ERROR: Cannot provide this option here"
  exit()

### functions ###

def TotalUnc(h0, hists=[]):
  #sum in quadrature several systematic uncertainties to form total uncertainty band
  hout = copy.deepcopy(h0)
  hup = copy.deepcopy(h0)
  hdown = copy.deepcopy(h0)
  hout.SetName(h0.GetName()+'_custom_uncerts')
  hup.SetName(h0.GetName()+'_custom_uncerts_up')
  hdown.SetName(h0.GetName()+'_custom_uncerts_down')
  for i in range(1,h0.GetNbinsX()+2):
    x0 = h0.GetBinContent(i)
    uncerts_up = [0.]
    uncerts_down = [0.]
    for h in hists:
      x = h.GetBinContent(i)
      if x>x0: uncerts_up.append(x-x0)
      if x<x0: uncerts_down.append(x0-x)
    up = 0.
    down = 0.
    for u in uncerts_up: up+=u**2
    for u in uncerts_down: down+=u**2
    up = up**.5
    down = down**.5

    hup.SetBinContent(i,x0+up)
    hdown.SetBinContent(i,x0-down)
    c = (x0+up + x0-down)/2
    u = (up+down)/2
    hout.SetBinContent(i,c)
    hout.SetBinError(i,u)
  return (hout, hup, hdown)

def FindRebinning(hist,BinThreshold=100,BinUncertFraction=0.5):

  #hist.Print("all")
  # getting binning
  binning = []
  for i in range(1,hist.GetNbinsX()+2):
    binning.append(hist.GetBinLowEdge(i))

  # remove outer 0 bins
  still_zero = True
  for i in range(1,hist.GetNbinsX()):
    if not (hist.GetBinContent(i) == 0 and hist.GetBinError(i) == 0): still_zero = False
    if still_zero:
      binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i))))
    else:
      break
  hist = RebinHist(hist,binning)

  still_zero = True
  for i in reversed(range(2,hist.GetNbinsX()+1)):
    if not (hist.GetBinContent(i) == 0 and hist.GetBinError(i) == 0): still_zero = False
    if still_zero:
      binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i+1))))
    else:
      break
  hist = RebinHist(hist,binning)

  # left to right
  finished = False
  k = 0
  while finished == False and k < 1000:
    k += 1
    for i in range(1,hist.GetNbinsX()):
      if hist.GetBinContent(i) > 0: uncert_frac = hist.GetBinError(i)/hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      #print hist.GetBinLowEdge(i), uncert_frac, BinUncertFraction,  hist.GetBinContent(i), BinThreshold
      if uncert_frac > BinUncertFraction and hist.GetBinContent(i) < BinThreshold:
        #binning.remove(hist.GetBinLowEdge(i+1))
        binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i+1))))
        hist = RebinHist(hist,binning)
        break
      elif i+1 == hist.GetNbinsX():
        finished = True

  # right to left
  finished = False
  k = 0
  while finished == False and k < 1000:
    k+= 1
    for i in reversed(range(2,hist.GetNbinsX()+1)):
      if hist.GetBinContent(i) > 0: uncert_frac = hist.GetBinError(i)/hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      #print hist.GetBinLowEdge(i), uncert_frac, BinUncertFraction,  hist.GetBinContent(i), BinThreshold
      if uncert_frac > BinUncertFraction and hist.GetBinContent(i) < BinThreshold:
#        binning.remove(hist.GetBinLowEdge(i))
        binning.remove(min(binning, key=lambda x:abs(x-hist.GetBinLowEdge(i))))
        hist = RebinHist(hist,binning)
        break
      elif i == 2:
        finished = True

#  hist.Print("all")
  return binning

def RebinHist(hist,binning):
  # getting initial binning
  initial_binning = []
  for i in range(1,hist.GetNbinsX()+2):
    initial_binning.append(hist.GetBinLowEdge(i))

  new_binning = array('f', map(float,binning))
  hout = ROOT.TH1D(hist.GetName(),'',len(new_binning)-1, new_binning)
  for i in range(1,hout.GetNbinsX()+1):
    for j in range(1,hist.GetNbinsX()+1):
      if hist.GetBinCenter(j) > hout.GetBinLowEdge(i) and hist.GetBinCenter(j) < hout.GetBinLowEdge(i+1):
        new_content = hout.GetBinContent(i)+hist.GetBinContent(j)
        new_error = (hout.GetBinError(i)**2+hist.GetBinError(j)**2)**0.5
        hout.SetBinContent(i,new_content)
        hout.SetBinError(i,new_error)
  #hout.Print("all")
  return hout

def ZeroNegativeBins(hist):
  neg_bins = False
  for i in range(0,hist.GetNbinsX()+1):
    if hist.GetBinContent(i) < 0:
      neg_bins = True
      hist.SetBinContent(i,0)
  return hist, neg_bins

###


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
          hist_dict[hkey.GetName()] = copy.deepcopy(copy.deepcopy(hist))
        else:
          hist_dict[hkey.GetName()].Add(copy.deepcopy(copy.deepcopy(hist)))

for key, val in hist_dict.items():
  cd = cf.Get(dir_name)
  cd.cd()
  val.Write()

output_name = args.outputfolder + "/" + dc_name + "_all.root"
directory = cf.Get(dir_name)
keys = [i.GetName() for i in directory.GetListOfKeys()]

if args.auto_rebinning:
  print "Doing rebinning"
  #outfile_rebin = ROOT.TFile(output_name.replace(".root","_rebinned.root"), 'RECREATE')
  #output_name = output_name.replace(".root","_rebinned.root")
  #outfile_rebin.mkdir(dir_name)
  #outfile_rebin.cd(dir_name)
  total_bkghist = copy.deepcopy(cf.Get(dir_name+'/total_bkg'))
  binning = FindRebinning(total_bkghist,BinThreshold=args.bin_threshold,BinUncertFraction=args.bin_uncert_fraction)
  if args.rebin_with_data:
    data_obs = copy.deepcopy(cf.Get(dir_name+'/data_obs'))
    data_obs = RebinHist(data_obs,binning)
    binning = FindRebinning(data_obs,BinThreshold=args.bin_threshold,BinUncertFraction=args.bin_uncert_fraction)


  print "New binning:", binning
  hists_done = []
  for i in  cf.Get(dir_name).GetListOfKeys():
      if i.GetName() not in hists_done:
        if ".subnodes" not in i.GetName():
          print i.GetName()
          new_hist = RebinHist(copy.deepcopy(cf.Get(dir_name+'/'+i.GetName())),binning)
          hists_done.append(i.GetName())
          directory.Delete(i.GetName()+';1')
          new_hist.Write()
  
  #cf.Close()
  #cf = outfile_rebin

if args.zero_negative_bins:
  print "Zeroing negative bins"
  files_done = []
  cf.cd(dir_name)
  for i in keys:
    if ".subnodes" not in i:
      print i
      hist = cf.Get(dir_name+'/'+i)
      scale = False
      if hist.Integral() < 0:  
        scale = True
        hist.Scale(-1)
      new_hist, neg_bins = ZeroNegativeBins(copy.deepcopy(hist))
      if scale: new_hist.Scale(-1)
      if neg_bins:
        directory.Delete(i+';1')
        new_hist.Write()

#recalculate total
proc_names = ["VVR","VVLF","jetFakes","MC_jetFakes","jetFakes1","jetFakes2","jetFakes3","jetFakes4","jetFakes12","jetFakes13","jetFakes14","jetFakes23","jetFakes24","jetFakes34","jetFakes123","jetFakes124","jetFakes234","jetFakes1234"]
first = True
for i in proc_names:
  if i in directory.GetListOfKeys():
    if first:
      total_bkg = copy.deepcopy(directory.Get(i))
      total_bkg.SetName('total_bkg')
      first = False
    else:
      total_bkg.Add(directory.Get(i))

directory.Delete('total_bkg;1')
total_bkg.Write()
  

if args.add_stat_to_syst:
  print "Getting total systematic uncertainty"
  custom_uncerts_up_name = 'total_bkg_uncerts_custom_up'
  custom_uncerts_down_name = 'total_bkg_uncerts_custom_down'
  directory = cf.Get(dir_name)
  h0 = directory.Get('total_bkg')
  hists=[]
  uncerts_needed = []
  for hist in directory.GetListOfKeys():
    if ".subnodes" in hist.GetName(): continue
    if hist.GetName().endswith("Up") or hist.GetName().endswith("Down"):
      for p in proc_names:
        if p+"_" in hist.GetName():
          uncert_name = hist.GetName().split(p+"_")[1]
          if uncert_name not in uncerts_needed: uncerts_needed.append(uncert_name)
          break

  for uncert_name in uncerts_needed: 
    temp_hist = copy.deepcopy(h0)
    for p in proc_names:
      if p+"_"+uncert_name in directory.GetListOfKeys():
        temp_hist.Add(directory.Get(p),-1)
        temp_hist.Add(directory.Get(p+"_"+uncert_name))
    hists.append(temp_hist)

  (uncert, up, down) = TotalUnc(h0, hists)
  cf.cd(dir_name)
  uncert.Write()
  up.Write()
  down.Write()

  args.combined_options += " --add_stat_to_syst"

cf.Close()
cf.Write()

print "python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --replace_name={}_all  --channel={} --cat={} {} {}".format(args.outputfolder,output_name,config,dc_name,args.channel,args.cat,args.options,args.combined_options)
os.system("python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --replace_name={}_all  --channel={} --cat={} {} {}".format(args.outputfolder,output_name,config,dc_name,args.channel,args.cat,args.options,args.combined_options))

