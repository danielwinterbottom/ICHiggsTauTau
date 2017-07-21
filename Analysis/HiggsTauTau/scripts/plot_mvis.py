import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

#infileHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/GluGlu_HToTauTau_Summer16_BarrelTDR_mt_0.root"
infileHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/GluGluHToTauTau-200PU_mt_0.root"
#infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/DYJetsToLL_Summer16_BarrelTDR_mt_0.root"
infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/GluGluHToTauTau-200PU-tausOrg_mt_0.root"

htt_file=ROOT.TFile.Open(infileHTT)
dy_file=ROOT.TFile.Open(infileDY)
htt_tree=htt_file.Get("ntuple")
dy_tree=dy_file.Get("ntuple")

htt_mvis = ROOT.TH1D("htt_mvis","htt_mvis",32,0,160)
httorg_mvis = ROOT.TH1D("httorg_mvis","httorg_mvis",32,0,160)


htt_tree.Draw("m_vis>>htt_mvis")
dy_tree.Draw("m_vis>>httorg_mvis")



htt_mvis.SetLineColor(ROOT.kRed)
httorg_mvis.SetLineColor(ROOT.kBlue)

htt_mvis.Scale(1./htt_mvis.Integral())
httorg_mvis.Scale(1./httorg_mvis.Integral())





plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend, NColumns=1)
htt_mvis.GetXaxis().SetTitle("Visible mass [GeV]")
htt_mvis.GetYaxis().SetTitleOffset(1.4)
htt_mvis.GetYaxis().SetTitle("a.u.")
htt_mvis.GetYaxis().SetRangeUser(0,0.2)
htt_mvis.Draw("")
httorg_mvis.Draw("SAME")
legend.AddEntry(htt_mvis,'Gluon fusion H#rightarrow#tau#tau (after fix)','L')
legend.AddEntry(httorg_mvis,'Gluon fusion H#rightarrow#tau#tau (before fix)','L')
legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS ', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "#mu#tau_{h} channel", 1)
plot.DrawTitle(pads[0], "14 TeV, 200 PU", 3)

canv.SaveAs('mvis_comparison_stripfix.pdf')
canv.Print('mvis_comparison_stripfix.png')


