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
infileHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBFHToTauTau-200PU_mt_0.root"
infileHTTpuppi = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBFHToTauTau-200PU-puppi_mt_0.root"
#infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/DYJetsToLL_Summer16_BarrelTDR_mt_0.root"
#infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/GluGluHToTauTau-200PU-tausOrg_mt_0.root"

htt_file=ROOT.TFile.Open(infileHTT)
htt_file_puppi=ROOT.TFile.Open(infileHTTpuppi)
#dy_file=ROOT.TFile.Open(infileDY)
htt_tree=htt_file.Get("ntuple")
htt_tree_puppi=htt_file_puppi.Get("ntuple")
#dy_tree=dy_file.Get("ntuple")

htt_mvis = ROOT.TH1D("htt_mvis","htt_mvis",40,0,200)
htt_msv = ROOT.TH1D("htt_msv","htt_msv",40,0,200)
htt_msv_puppi = ROOT.TH1D("htt_msv_puppi","htt_msv_puppi",40,0,200)


htt_tree.Draw("m_vis>>htt_mvis")
htt_tree.Draw("m_sv>>htt_msv")
htt_tree_puppi.Draw("m_sv>>htt_msv_puppi")


htt_mvis.SetLineColor(ROOT.kRed)
htt_msv.SetLineColor(ROOT.kGreen+3)
htt_msv_puppi.SetLineColor(ROOT.kBlue)

htt_mvis.Scale(1./htt_mvis.Integral())
htt_msv.Scale(1./htt_msv.Integral())
htt_msv_puppi.Scale(1./htt_msv_puppi.Integral())



plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend, NColumns=1)
htt_mvis.GetXaxis().SetTitle("di-tau mass [GeV]")
htt_mvis.GetYaxis().SetTitleOffset(1.4)
htt_mvis.GetYaxis().SetTitle("a.u.")
htt_mvis.GetYaxis().SetRangeUser(0,0.2)
htt_mvis.Draw("")
htt_msv.Draw("SAME")
htt_msv_puppi.Draw("SAME")
legend.AddEntry(htt_mvis,'Visible di-tau mass','L')
legend.AddEntry(htt_msv,'SVFit mass (PF Met)','L')
legend.AddEntry(htt_msv_puppi,'SVFit mass (PUPPI Met)','L')
legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "#mu#tau_{h} channel", 1)
plot.DrawTitle(pads[0], "14 TeV, 200 PU", 3)

canv.SaveAs('mvis_vs_msv_comparison_200PU.pdf')
canv.Print('mvis_vs_msv_comparison_200PU.png')


