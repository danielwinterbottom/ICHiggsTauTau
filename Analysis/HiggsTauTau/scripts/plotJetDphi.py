import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_full_forjetdphi_tt_0.root"
current_infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_current_forjetdphi_tt_0.root"

vbf_file=ROOT.TFile.Open(infile)
jetpvtree=vbf_file.Get("leadsubleadjet")

vbfcurrent_file=ROOT.TFile.Open(current_infile)
jetpvtree_current=vbfcurrent_file.Get("leadsubleadjet")


xbins = [0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2]
dphi_fut_all = ROOT.TH1D("dphi_fut_all","dphi_fut_all",16,np.array(xbins))
dphi_fut_assoc = ROOT.TH1D("dphi_fut_assoc","dphi_fut_assoc",16,np.array(xbins))
dphi_current_all = ROOT.TH1D("dphi_current_all","dphi_current_all",16,np.array(xbins))
dphi_current_assoc = ROOT.TH1D("dphi_current_assoc","dphi_current_assoc",16,np.array(xbins))

dphi_fut_all.Sumw2()
dphi_fut_assoc.Sumw2()
dphi_current_all.Sumw2()
dphi_current_assoc.Sumw2()

jetpvtree.Draw("vbf_dphi>>dphi_fut_all","")
jetpvtree.Draw("vbf_dphi>>dphi_fut_assoc","jet_beta1>0.1&&jet_beta2>0.1")
jetpvtree_current.Draw("vbf_dphi>>dphi_current_all","")
jetpvtree_current.Draw("vbf_dphi>>dphi_current_assoc","jet_beta1>0.1&&jet_beta2>0.1")

dphi_fut_assoc.Divide(dphi_fut_all)
dphi_current_assoc.Divide(dphi_current_all)

dphi_fut_assoc.SetMarkerStyle(20)
dphi_fut_assoc.SetMarkerColor(ROOT.kRed)
dphi_fut_assoc.SetTitle("")
dphi_fut_assoc.SetStats(0)
dphi_fut_assoc.GetYaxis().SetRangeUser(0,1.4)

dphi_current_assoc.SetMarkerStyle(24)
dphi_current_assoc.SetMarkerColor(ROOT.kRed)

plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
dphi_fut_assoc.GetXaxis().SetTitle("VBF jet #Delta#phi")
dphi_fut_assoc.GetYaxis().SetTitle("PV association efficiency of both jets")
legend.AddEntry(dphi_fut_assoc,'With tracker extension','P')
legend.AddEntry(dphi_current_assoc,'Without tracker extension (current)','P')
dphi_fut_assoc.Draw("EP0")
dphi_current_assoc.Draw("EP0SAME")

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV, 0 PU", 3)

canv.SaveAs('dphi_association.pdf')
canv.Print('dphi_association.png')

