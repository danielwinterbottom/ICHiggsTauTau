import ROOT
import math
import argparse
import sys
import os
import numpy as np
import fnmatch

ROOT.gROOT.SetBatch(ROOT.kTRUE)

def calcSoverRootB(filename,stringtomatch,sig_sample,mass,use_ztt_sig):
  sob_x = []
  sob_x_err = []
  sob_y = []
  sob_y_err = []
  x_point = {'%(stringtomatch)svloose'%vars():1,'%(stringtomatch)sloose'%vars():2,'%(stringtomatch)smedium'%vars():3,'%(stringtomatch)stight'%vars():4,'%(stringtomatch)svtight'%vars():5,'%(stringtomatch)svvtight'%vars():6}
  for key in filename.GetListOfKeys():
    tdir = filename.Get(key.GetName())
    if isinstance(tdir,ROOT.TDirectory) and stringtomatch in key.GetName():
      ztt_hist = tdir.Get("ZTT")
      vv_hist = tdir.Get("VV")
      zll_hist = tdir.Get("ZLL")
      tt_hist = tdir.Get("TT")
      qcd_hist = tdir.Get("QCD")
      w_hist = tdir.Get("W")
      sigsamp = tdir.Get("%(sig_sample)s%(mass)s"%vars()) 
      sig_error = ROOT.Double(0.0)
      bkg_error = ROOT.Double(0.0)
      bkg = ROOT.Double(0.0)
      if use_ztt_sig == True:
        ztt_hist.IntegralAndError(0,ztt_hist.GetNbinsX()+1,sig_error)
        signal = ztt_hist.Integral()
        bkglist = {vv_hist,zll_hist,tt_hist,qcd_hist,w_hist}
        for histo in bkglist:
          dummy_bkg_error = ROOT.Double(0.0)
          histo.IntegralAndError(0,histo.GetNbinsX()+1,dummy_bkg_error)
          bkg_error += dummy_bkg_error
          bkg += histo.Integral()
      else :
         sigsamp.IntegralAndError(0,sigsamp.GetNbinsX()+1,sig_error)
         signal = sigsamp.Integral()
         bkglist = {vv_hist,zll_hist,tt_hist,qcd_hist,w_hist,ztt_hist}
         for histo in bkglist:
           dummy_bkg_error = ROOT.Double(0.0)
           histo.IntegralAndError(0,histo.GetNbinsX()+1,dummy_bkg_error)
           bkg_error += dummy_bkg_error
           bkg += histo.Integral()
      sob_err = math.sqrt((sig_error/signal)*(sig_error/signal)+(0.5*bkg_error/bkg)*(0.5*bkg_error/bkg))*signal/math.sqrt(bkg);
      sob_x.append(x_point[tdir.GetName()])
      sob_x_err.append(0.)
      sob_y.append(signal/math.sqrt(bkg))
      sob_y_err.append(sob_err)
  SOBGraph = ROOT.TGraphErrors(len(sob_x))
  for i in range(0,len(sob_x)):
    SOBGraph.SetPoint(i,sob_x[i],sob_y[i])
    SOBGraph.SetPointError(i,sob_x_err[i],sob_y_err[i])
  return SOBGraph
    

datacard_file = ROOT.TFile("./datacards-mvaiso/htt_mt.inputs-mssm-13TeV-mttot-TauIsoOpt.root")
endgraph = calcSoverRootB(datacard_file,"mt_mvadbnew","ggH","180",True)
endgraph.SetName("ztt_sig")

outfile = ROOT.TFile("output_graphs.root","RECREATE")
outfile.cd()
endgraph.Write()
outfile.Close()
 
   
      
  
