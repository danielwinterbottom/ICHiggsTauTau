import argparse
import os
import ROOT
import copy
from array import array
import numpy as np
ROOT.TH1.AddDirectory(0)

parser = argparse.ArgumentParser()
parser.add_argument('--dc_name',help= 'Add the name of the datacard to be processed', default='')
args = parser.parse_args()


def AMS(sig_integral,bkg_integral):
  ''' s/root(b)'''
  AMS = sig_integral/np.sqrt(bkg_integral)
  return AMS

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
  return hout

def FindRebinning_v2(bkg_hist,sig_hist,BinUncertFraction=0.25):

  binning = []
  for i in range(1,bkg_hist.GetNbinsX()+2):
    binning.append(bkg_hist.GetBinLowEdge(i))

  turnOnBin = 0 
  for i in range(1,bkg_hist.GetNbinsX()+1):
    bin_start = bkg_hist.GetBinLowEdge(i)
    bin_end = bkg_hist.GetBinLowEdge(i+1)
    if bkg_hist.GetBinContent(i) > 0 and bkg_hist.GetBinContent(i+1) >0:   
    # Calculate score of bin i
        signal  = sig_hist.GetBinContent(i)
        bkg = bkg_hist.GetBinContent(i)
        AMS_bin = AMS(signal,bkg)
        # Calculate score of bin i + 1
        signal_  = sig_hist.GetBinContent(i+1)
        bkg_ = bkg_hist.GetBinContent(i+1)
        AMS_bin_ = AMS(signal_,bkg_)
        if AMS_bin_ > AMS_bin:
           turnOnBin = bkg_hist.GetBinLowEdge(i+1)
           break
  if binning.index(turnOnBin) != 0:
     del binning[1:binning.index(turnOnBin)]
  bkg_hist = RebinHist(bkg_hist,binning)

  # --------- Rebin on Bin Uncertainty ----------
  # left to right
  finished = False
  k = 0
  while finished == False and k < 1000:
    k += 1

    # Ignore first bin
    for i in range(2,bkg_hist.GetNbinsX()):
      if bkg_hist.GetBinContent(i) > 0: uncert_frac = bkg_hist.GetBinError(i)/bkg_hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      if uncert_frac > BinUncertFraction:
        binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i+1))))
        bkg_hist = RebinHist(bkg_hist,binning)
        break
      elif i+1 == bkg_hist.GetNbinsX():
        finished = True

  # right to left
  finished = False
  k = 0
  while finished == False and k < 1000:
    k+= 1
    for i in reversed(range(3,bkg_hist.GetNbinsX()+1)):
      if bkg_hist.GetBinContent(i) > 0: uncert_frac = bkg_hist.GetBinError(i)/bkg_hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      if uncert_frac > BinUncertFraction:
        binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i))))
        bkg_hist = RebinHist(bkg_hist,binning)
        break
      elif i == 2:
        finished = True
  

  for i in range(1,bkg_hist.GetNbinsX()+2):
    bin_start = bkg_hist.GetBinLowEdge(i)
    bin_end = bkg_hist.GetBinLowEdge(i+1)
    print(bin_start,bin_end,bkg_hist.GetBinContent(i))
  return binning


def FindRebinning(bkg_hist,sig_hist,BinUncertFraction=0.25):
  binning = []
  for i in range(1,bkg_hist.GetNbinsX()+2):
    binning.append(bkg_hist.GetBinLowEdge(i))

  # --------- First Rebin on significance ----------
  # left to right
  finished = False
  k = 0
  while finished == False and k < 1000:
    k += 1
    for i in range(1,bkg_hist.GetNbinsX()):
      if bkg_hist.GetBinContent(i) > 0 and bkg_hist.GetBinContent(i+1) >0: 
       
        # Calculate score of bin i
        signal  = sig_hist.GetBinContent(i)
        bkg = bkg_hist.GetBinContent(i)
        AMS_bin = AMS(signal,bkg)
     
        # Calculate score of bin i + 1
        signal_  = sig_hist.GetBinContent(i+1)
        bkg_ = bkg_hist.GetBinContent(i+1)
        AMS_bin_ = AMS(signal_,bkg_)

        # Total score by merging bins i & i + 1
        total_signal = signal + signal_
        total_bkg = bkg + bkg_
        total_AMS = AMS(total_signal,total_bkg)
        
        # Scores of bins i & i + 1 added in Quadrature
        total_dp = np.sqrt(AMS_bin**2 + AMS_bin_**2)

        # Removing bin if score of merged bins is better than the quadrature sum
        if total_AMS >= total_dp:
           binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i+1))))
           bkg_hist = RebinHist(bkg_hist,binning)
           sig_hist = RebinHist(sig_hist,binning)
           break
        elif i+1 == bkg_hist.GetNbinsX():
           finished = True
      
      else:
        binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i+1))))
        bkg_hist = RebinHist(bkg_hist,binning)
        sig_hist = RebinHist(sig_hist,binning)
        break
        if i+1 == bkg_hist.GetNbinsX():
          finished = True

  # right to left
  finished = False
  k = 0
  while finished == False and k < 1000:
    k+= 1
    for i in reversed(range(2,bkg_hist.GetNbinsX()+1)):
      if bkg_hist.GetBinContent(i) > 0 and bkg_hist.GetBinContent(i-1) >0:

        # Calculate score of bin i
        signal  = sig_hist.GetBinContent(i)
        bkg = bkg_hist.GetBinContent(i)
        AMS_bin = AMS(signal,bkg)

        # Calculate score of bin i - 1
        signal_  = sig_hist.GetBinContent(i-1)
        bkg_ = bkg_hist.GetBinContent(i-1)
        AMS_bin_ = AMS(signal_,bkg_)

        # Total score by merging bins i & i - 1
        total_signal = signal + signal_
        total_bkg = bkg + bkg_
        total_AMS = AMS(total_signal,total_bkg)

        # Scores of bins i & i - 1 added in Quadrature
        total_dp = np.sqrt(AMS_bin**2 + AMS_bin_**2)

        # Removing bin if score of merged bins is better than the quadrature sum
        if total_AMS >= total_dp:
           binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i))))
           bkg_hist = RebinHist(bkg_hist,binning)
           sig_hist = RebinHist(sig_hist,binning)
           break
        elif i == 2:
           finished = True

      else:
        binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i))))
        bkg_hist = RebinHist(bkg_hist,binning)
        sig_hist = RebinHist(sig_hist,binning)
        break
        if i == 2:
          finished = True

  
  # --------- Rebin on Bin Uncertainty ----------
  # left to right
  finished = False
  k = 0
  while finished == False and k < 1000:
    k += 1

    # Ignore first bin
    for i in range(2,bkg_hist.GetNbinsX()):
      if bkg_hist.GetBinContent(i) > 0: uncert_frac = bkg_hist.GetBinError(i)/bkg_hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      if uncert_frac > BinUncertFraction:
        binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i+1))))
        bkg_hist = RebinHist(bkg_hist,binning)
        break
      elif i+1 == bkg_hist.GetNbinsX():
        finished = True

  # right to left
  finished = False
  k = 0
  while finished == False and k < 1000:
    k+= 1
    for i in reversed(range(2,bkg_hist.GetNbinsX()+1)):
      if bkg_hist.GetBinContent(i) > 0: uncert_frac = bkg_hist.GetBinError(i)/bkg_hist.GetBinContent(i)
      else: uncert_frac = BinUncertFraction+1
      if uncert_frac > BinUncertFraction:
        binning.remove(min(binning, key=lambda x:abs(x-bkg_hist.GetBinLowEdge(i))))
        bkg_hist = RebinHist(bkg_hist,binning)
        break
      elif i == 2:
        finished = True


#  print()
#  for i in range(1,bkg_hist.GetNbinsX()+1):
#     AMS_bin = 0
#     AMS_bin_ = 0
# 
#     total_signal = sig_hist.GetBinContent(i)+sig_hist.GetBinContent(i+1)
#     total_bkg = bkg_hist.GetBinContent(i)+bkg_hist.GetBinContent(i+1)
#     total_AMS = 0
#     if total_bkg !=0: total_AMS = AMS(total_signal,total_bkg)
#     binUncertFraction = bkg_hist.GetBinError(i)/bkg_hist.GetBinContent(i)
#     if bkg_hist.GetBinContent(i) != 0: AMS_bin = AMS(sig_hist.GetBinContent(i),bkg_hist.GetBinContent(i))
#     if bkg_hist.GetBinContent(i+1) != 0: AMS_bin_ = AMS(sig_hist.GetBinContent(i+1),bkg_hist.GetBinContent(i+1))
#
#     total_dp = np.sqrt(AMS_bin**2 + AMS_bin_**2)
#     print("BIN:",bkg_hist.GetBinLowEdge(i),bkg_hist.GetBinLowEdge(i+1),"SIG:",sig_hist.GetBinContent(i),"BKG:",bkg_hist.GetBinContent(i),"AMS1:",AMS_bin,"AMS2:",AMS_bin_,"AMS_TOT",total_AMS,"AMS_DP:",total_dp,"BIN_unc_fraction:",binUncertFraction)
#
#  print
#  print("Background OverFlowBin",bkg_hist.GetBinContent(bkg_hist.GetNbinsX()+1))
#  print("Signal OverFlowBin",sig_hist.GetBinContent(sig_hist.GetNbinsX()+1))
#  print(binning)
#  print(len(binning))
  return binning

f = ROOT.TFile("{}".format(args.dc_name))
hist_dict = {}

for dkey in f.GetListOfKeys():
  d = f.Get(dkey.GetName())
  fnames = []
  first_hist = True
  for hkey in d.GetListOfKeys():
    if ".subnodes" not in hkey.GetName() and hkey.GetName() not in fnames:
      fnames.append(hkey.GetName())
      hist = d.Get(hkey.GetName())
      if hkey.GetName() not in hist_dict.keys():          
        hist_dict[hkey.GetName()] = copy.deepcopy(copy.deepcopy(hist))
        if "_norm" in hkey.GetName():
          if first_hist==True:
             hist_dict["total_sig"] = copy.deepcopy(copy.deepcopy(hist))
             hist_dict["total_sig"].SetName("total_sig")
             first_hist=False
	  else:
             hist_dict["total_sig"].Add(copy.deepcopy(copy.deepcopy(hist)))
        
total_bkghist = copy.deepcopy(hist_dict["total_bkg"])
sig_hist = copy.deepcopy(hist_dict["A60phi100_norm"])
total_bkghist.Scale(1/total_bkghist.Integral())
sig_hist.Scale(1/sig_hist.Integral())
binning = FindRebinning_v2(total_bkghist,sig_hist)
print("Binning")
print(binning)


