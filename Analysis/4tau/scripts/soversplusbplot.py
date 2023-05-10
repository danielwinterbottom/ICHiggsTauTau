import ROOT
import math
from array import array
import numpy as np

folder = "0105"
show_data = False
signal = "A100phi100"
bins = 10

folder = folder+"/CHANNEL/mt_tot_signal_CAT_all.root"
cats = ['tttt_inclusive','ttt_inclusive','ettt_nobtag','mttt_nobtag','eett_z_control_nobtag','eett_2l2t_sig_nobtag','mmtt_z_control_nobtag','mmtt_2l2t_sig_nobtag','emtt_z_control_nobtag','emtt_2l2t_sig_nobtag']

soversplusb = []
sig_wt = []
bkg_wt = []
data_wt = []
for k in cats:
  name = folder.replace("CAT",k).replace("CHANNEL",k.split("_")[0])
  f = ROOT.TFile(name)
  tb = f.Get(k+"/total_bkg")
  s = f.Get(k+"/"+signal)
  if show_data: d = f.Get(k+"/data_obs")
  for i in range(1,tb.GetNbinsX()+1):
    if s.GetBinContent(i) == 0: continue
    soversplusb.append(math.log10(s.GetBinContent(i)/(s.GetBinContent(i)+tb.GetBinContent(i))))
    sig_wt.append(s.GetBinContent(i))
    bkg_wt.append(tb.GetBinContent(i))
    if show_data: data_wt.append(d.GetBinContent(i))

min_val = min(soversplusb)
max_val = max(soversplusb)

order = -int(math.floor(math.log10(abs(max_val))))
scale_factor = 10**order
max_val = math.ceil(max_val * scale_factor) / scale_factor

order = -int(math.floor(math.log10(abs(min_val))))
scale_factor = 10**order
min_val = math.floor(min_val * scale_factor) / scale_factor

binning = array('f',list(np.linspace(min_val,max_val,num=bins)))
hist = ROOT.TH1F("h", "", len(binning)-1, binning)

s_hist = hist.Clone()
s_hist.SetName("Signal")
b_hist = hist.Clone()
b_hist.SetName("Background")
if show_data:
  d_hist = hist.Clone()
  d_hist.SetName("Data")

for ind, i in enumerate(soversplusb):
  s_hist.Fill(i,sig_wt[ind])
  b_hist.Fill(i,bkg_wt[ind])
  if show_data: d_hist.Fill(i,data_wt[ind])

s_hist.Print("all")
b_hist.Print("all")
if show_data: d_hist.Print("all")
