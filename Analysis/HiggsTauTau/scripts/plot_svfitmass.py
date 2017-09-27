import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

infileHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/GluGlu_HToTauTau_Summer16_BarrelTDR_mt_0.root"
infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/DYJetsToLL_Summer16_BarrelTDR_mt_0.root"

htt_file=ROOT.TFile.Open(infileHTT)
dy_file=ROOT.TFile.Open(infileDY)
htt_tree=htt_file.Get("ntuple")
dy_tree=dy_file.Get("ntuple")

htt_msv = ROOT.TH1D("htt_msv","htt_msv",28,40,180)
dy_msv = ROOT.TH1D("dy_msv","dy_msv",28,40,180)

htt_mvis = ROOT.TH1D("htt_mvis","htt_mvis",28,0,140)
dy_mvis = ROOT.TH1D("dy_mvis","dy_mvis",28,0,140)


htt_tree.Draw("m_sv>>htt_msv")
dy_tree.Draw("m_sv>>dy_msv")
htt_tree.Draw("m_vis>>htt_mvis")
dy_tree.Draw("m_vis>>dy_mvis")


htt_msv.SetLineColor(ROOT.kRed)
dy_msv.SetFillColor(ROOT.TColor.GetColor(248,206,104))
dy_msv.SetFillStyle(1001)

htt_mvis.SetLineColor(ROOT.kRed)
dy_mvis.SetFillColor(ROOT.TColor.GetColor(248,206,104))
dy_msv.SetFillStyle(1001)

htt_msv.Scale(1./htt_msv.Integral())
htt_mvis.Scale(1./htt_mvis.Integral())
dy_msv.Scale(1./dy_msv.Integral())
dy_mvis.Scale(1./dy_mvis.Integral())





plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend, NColumns=1)
dy_msv.GetXaxis().SetTitle("m_{#tau#tau} [GeV]")
dy_msv.GetYaxis().SetTitleOffset(1.4)
dy_msv.GetYaxis().SetTitle("a.u.")
dy_msv.GetYaxis().SetRangeUser(0,0.14)
dy_msv.Draw("")
htt_msv.Draw("SAME")
legend.AddEntry(dy_msv,'Z#rightarrow#tau#tau','F')
legend.AddEntry(htt_msv,'GluGlu H#rightarrow#tau#tau','L')
legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS ', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "#mu#tau_{h} channel", 1)
plot.DrawTitle(pads[0], "13 TeV, Run 2", 3)

canv.SaveAs('msv_comparison_summer16.pdf')
canv.Print('msv_comparison_summer16.png')

canv_mvis=ROOT.TCanvas()
pads_mvis = plot.OnePad()
for padx in pads_mvis:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend_mvis = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend_mvis, NColumns=1)
dy_mvis.GetXaxis().SetTitle("Visible m_{#tau#tau} [GeV]")
dy_mvis.GetYaxis().SetRangeUser(0,0.22)
dy_mvis.GetYaxis().SetTitleOffset(1.4)
dy_mvis.GetYaxis().SetTitle("a.u.")

dy_mvis.Draw("")
htt_mvis.Draw("SAME")
legend_mvis.AddEntry(dy_mvis,'Z#rightarrow#tau#tau','F')
legend_mvis.AddEntry(htt_mvis,'GluGlu H#rightarrow#tau#tau','L')
legend_mvis.Draw()

plot.DrawCMSLogo(pads_mvis[0], 'CMS ', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads_mvis[0], "#mu#tau_{h} channel", 1)
plot.DrawTitle(pads_mvis[0], "13 TeV, Run 2", 3)

canv_mvis.SaveAs('mvis_comparison_summer16.pdf')
canv_mvis.Print('mvis_comparison_summer16.png')


