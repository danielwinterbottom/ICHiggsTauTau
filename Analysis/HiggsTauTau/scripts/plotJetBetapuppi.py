import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

#infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_pu140_full_tt_0.root"
infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_pu140_puppi_tt_0.root"

vbf_file=ROOT.TFile.Open(infile)
jetpvtree=vbf_file.Get("jetpvassoc")


xbins = [0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0]
beta_jets_central = ROOT.TH1D("beta_jets_central","beta_jets_central",20,np.array(xbins))
beta_jets_around4 = ROOT.TH1D("beta_jets_around4","beta_jets_around4",20,np.array(xbins))

beta_jets_central.Sumw2()
beta_jets_around4.Sumw2()

jetpvtree.Draw("jet_beta>>beta_jets_central","abs(jet_eta)<0.4&&jet_pt>20")
jetpvtree.Draw("jet_beta>>beta_jets_around4","abs(jet_eta)<4.2&&abs(jet_eta)>3.8&&jet_pt>20")


beta_jets_central.Scale(1./beta_jets_central.Integral())
beta_jets_around4.Scale(1./beta_jets_around4.Integral())

beta_jets_central.SetMarkerStyle(20)
beta_jets_central.SetMarkerColor(ROOT.kRed)
beta_jets_central.SetTitle("")
beta_jets_central.SetStats(0)
beta_jets_central.GetYaxis().SetRangeUser(0,1)

beta_jets_around4.SetMarkerStyle(24)
beta_jets_around4.SetMarkerColor(ROOT.kRed)

plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
beta_jets_central.GetXaxis().SetTitle("#beta")
beta_jets_central.GetYaxis().SetTitle("a.u.")
legend.AddEntry(beta_jets_central,'|#eta| < 0.4','P')
legend.AddEntry(beta_jets_around4,'3.8<|#eta| <4.2','P')
beta_jets_central.Draw("EP0")
beta_jets_around4.Draw("EP0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV, 140 PU", 3)

canv.SaveAs('jetbetapuppi.pdf')
canv.Print('jetbetapuppi.png')

