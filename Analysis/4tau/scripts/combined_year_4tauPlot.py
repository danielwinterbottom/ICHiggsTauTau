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
parser.add_argument("--batch", action='store_true', help="Run each year separetely on the batch")
args = parser.parse_args()

if "--datacard" in args.options or "--cfg" in args.options or "--outputname" in args.options:
  print "ERROR: Cannot provide this option here"
  exit()

### functions ###

def CreateBatchJob(name,cmssw_base,cmd_list):
  if os.path.exists(job_file): os.system('rm %(name)s' % vars())
  os.system('echo "#!/bin/bash" >> %(name)s' % vars())
  os.system('echo "cd %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/4tau" >> %(name)s' % vars())
  os.system('echo "source /vols/grid/cms/setup.sh" >> %(name)s' % vars())
  os.system('echo "export SCRAM_ARCH=slc6_amd64_gcc481" >> %(name)s' % vars())
  os.system('echo "eval \'scramv1 runtime -sh\'" >> %(name)s' % vars())
  os.system('echo "source %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/4tau/scripts/setup_libs.sh" >> %(name)s' % vars())
  os.system('echo "ulimit -c 0" >> %(name)s' % vars())
  for cmd in cmd_list:
    os.system('echo "%(cmd)s" >> %(name)s' % vars())
  os.system('chmod +x %(name)s' % vars())
  print "Created job:",name

def SubmitBatchJob(name,time=180,memory=24,cores=1):
  error_log = name.replace('.sh','_error.log')
  output_log = name.replace('.sh','_output.log')
  if os.path.exists(error_log): os.system('rm %(error_log)s' % vars())
  if os.path.exists(output_log): os.system('rm %(output_log)s' % vars())
  if cores>1: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -pe hep.pe %(cores)s -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())
  else: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())

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


def RemoveSameDirectionShift(nom_hist, up_hist, down_hist):
  for i in range(0,nom_hist.GetNbinsX()+2):
    up_bin = up_hist.GetBinContent(i) - nom_hist.GetBinContent(i)
    down_bin = down_hist.GetBinContent(i) - nom_hist.GetBinContent(i)
    if up_bin == 0 or down_bin == 0: continue
    if up_bin/abs(up_bin) == down_bin/abs(down_bin):
      if abs(up_bin) > abs(down_bin):
        down_hist.SetBinContent(i, nom_hist.GetBinContent(i))
      elif abs(up_bin) <= abs(down_bin):
        up_hist.SetBinContent(i, nom_hist.GetBinContent(i))
  return up_hist, down_hist



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
    cmd = "python scripts/4tauPlot.py --outputfolder={} --replace_name={}_{} --cfg={} --channel={} --cat={} {}".format(args.outputfolder,dc_name,year,config,args.channel,args.cat,args.options)
    if not args.batch:
      print cmd
      os.system(cmd)
    else:
      job_file = "{}/jobs/{}_{}.sh".format(args.outputfolder,dc_name,year)
      if not os.path.isdir('{}/jobs'.format(args.outputfolder)): os.system("mkdir {}/jobs".format(args.outputfolder))
      CreateBatchJob(job_file,os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/4tau',''),[cmd])
      SubmitBatchJob(job_file,time=180,memory=24,cores=1)

if args.batch: exit()

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
          #print i.GetName()
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

# capping jetfakes if higher numbers have a greater value of events when better stats
print "Capping jetFakes yields"

def ShiftYields(check,minimum,check_scale=1.0,minimum_scale=1.0):
  cf.cd(dir_name)

  min_hist = cf.Get(dir_name+'/'+minimum)
  min_hist.Scale(minimum_scale)

  for c in check:

    hist = cf.Get(dir_name+'/'+c)
    new_hist = copy.deepcopy(hist)

    new_hist.Scale(check_scale)

    for i in range(0,new_hist.GetNbinsX()+1):
      if abs(min_hist.GetBinContent(i)) > abs(new_hist.GetBinContent(i)):
        if new_hist.Integral() > 0:
          new_hist.SetBinContent(i,abs(min_hist.GetBinContent(i)))
        else:
          new_hist.SetBinContent(i,-abs(min_hist.GetBinContent(i)))

    new_hist.Scale(1.0/check_scale)

    directory.Delete(c+';1')
    new_hist.Write()
    shift = new_hist.Clone()
    shift.Add(hist,-1)

    shifted = []
    for i in directory.GetListOfKeys():
      if c+"_" in i.GetName():
        print i.GetName()
        new_shift = copy.deepcopy(directory.Get(i.GetName()))
        new_shift.Add(shift)
        shifted.append(new_shift)

    for i in shifted:
      directory.Delete(i.GetName()+';1')
      i.Write()


if args.channel in ["tttt"]:
  ShiftYields(["jetFakes123","jetFakes124","jetFakes134","jetFakes234"],"jetFakes1234",minimum_scale=1.0/3.0,check_scale=1.0/2.0)
  ShiftYields(["jetFakes12","jetFakes13","jetFakes23"],"jetFakes123",minimum_scale=1.0/2.0)
  ShiftYields(["jetFakes12","jetFakes14","jetFakes24"],"jetFakes124",minimum_scale=1.0/2.0)
  ShiftYields(["jetFakes13","jetFakes14","jetFakes34"],"jetFakes134",minimum_scale=1.0/2.0)
  ShiftYields(["jetFakes23","jetFakes24","jetFakes34"],"jetFakes234",minimum_scale=1.0/2.0)

if args.channel in ["ettt","mttt"]:
  ShiftYields(["jetFakes23","jetFakes24","jetFakes34"],"jetFakes234")
  ShiftYields(["jetFakes2","jetFakes3"],"jetFakes23")
  ShiftYields(["jetFakes2","jetFakes4"],"jetFakes24")
  ShiftYields(["jetFakes3","jetFakes4"],"jetFakes34")


if args.channel in ["ttt"]:
  ShiftYields(["jetFakes12","jetFakes13","jetFakes23"],"jetFakes123")
  ShiftYields(["jetFakes1","jetFakes2"],"jetFakes12")
  ShiftYields(["jetFakes1","jetFakes3"],"jetFakes13")
  ShiftYields(["jetFakes2","jetFakes3"],"jetFakes23")

if args.channel in ["eett","mmtt","emtt"]:
  ShiftYields(["jetFakes3","jetFakes4"],"jetFakes34")



#recalculate total
proc_names = ["Higgs","VVV","ZR","TTR","VVR","WR","ZLF","TTLF","VVLF","WLF","jetFakes","MC_jetFakes","jetFakes1","jetFakes2","jetFakes3","jetFakes4","jetFakes12","jetFakes13","jetFakes14","jetFakes23","jetFakes24","jetFakes34","jetFakes123","jetFakes124","jetFakes134","jetFakes234","jetFakes1234"]
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

  #for uncert_name in sorted(uncerts_needed): 
  #  temp_hist = copy.deepcopy(h0)
  #  for p in proc_names:
  #    if p+"_"+uncert_name in directory.GetListOfKeys():
  #      temp_hist.Add(directory.Get(p),-1)
  #      temp_hist.Add(directory.Get(p+"_"+uncert_name))
  #  print uncert_name, temp_hist.Integral() - h0.Integral(), h0.Integral()
  #  hists.append(temp_hist)

  for uncert_name in sorted(uncerts_needed): 
    if "Up" not in uncert_name: continue
    temp_hist = copy.deepcopy(h0)
    up_shift = temp_hist.Clone()
    down_shift = temp_hist.Clone()
    for p in proc_names:
      if p+"_"+uncert_name in directory.GetListOfKeys():
        nom_hist = directory.Get(p)
        up_hist = directory.Get(p+"_"+uncert_name)
        down_hist = directory.Get(p+"_"+uncert_name.replace("Up","Down"))
        # Get up and down
        up_shift.Add(nom_hist,-1)
        up_shift.Add(up_hist)
        down_shift.Add(nom_hist,-1)
        down_shift.Add(down_hist)
        print p, uncert_name.replace("Up",""), nom_hist.Integral(), "+"+str(abs(max(up_hist.Integral()-nom_hist.Integral(),down_hist.Integral()-nom_hist.Integral()))), "-"+str(abs(min(up_hist.Integral()-nom_hist.Integral(),down_hist.Integral()-nom_hist.Integral())))
    nup, ndown = RemoveSameDirectionShift(h0, up_shift, down_shift)
    hists.append(nup)
    hists.append(ndown)
    print uncert_name.replace("Up",""), h0.Integral(), "+"+str(abs(max(nup.Integral()-h0.Integral(),ndown.Integral()-h0.Integral()))), "-"+str(abs(min(nup.Integral()-h0.Integral(),ndown.Integral()-h0.Integral())))


  (uncert, up, down) = TotalUnc(h0, hists)

  if args.zero_negative_bins:
    up, _ = ZeroNegativeBins(copy.deepcopy(up))
    down, _ = ZeroNegativeBins(copy.deepcopy(down))

  cf.cd(dir_name)
  uncert.Write()
  up.Write()
  down.Write()

  args.combined_options += " --add_stat_to_syst"

cf.Close()
cf.Write()

print "python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --replace_name={}_all  --channel={} --cat={} {} {}".format(args.outputfolder,output_name,config,dc_name,args.channel,args.cat,args.options,args.combined_options)
os.system("python scripts/4tauPlot.py --outputfolder={} --plot_from_dc='{}' --cfg={} --lumi='138 fb^{{-1}} (13 TeV)' --replace_name={}_all  --channel={} --cat={} {} {}".format(args.outputfolder,output_name,config,dc_name,args.channel,args.cat,args.options,args.combined_options))

