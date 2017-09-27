import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_current_withunchsjets_tt_0.root"
current_infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_current_tt_0.root"

vbf_file=ROOT.TFile.Open(infile)
jetpvtree=vbf_file.Get("jetpvassoc")

vbfcurrent_file=ROOT.TFile.Open(current_infile)
jetpvtree_current=vbfcurrent_file.Get("jetpvassoc")


xbins = [0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2,4.4,4.6,4.8,5.0]
vbf_all_jets = ROOT.TH1D("vbf_all_jets","vbf_all_jets",25,np.array(xbins))
vbf_pvassoc_jets = ROOT.TH1D("vbf_pvassoc_jets","vbf_pvassoc_jets",25,np.array(xbins))
vbf_all_jets_current = ROOT.TH1D("vbf_all_jets_current","vbf_all_jets_current",25,np.array(xbins))
vbf_pvassoc_jets_current = ROOT.TH1D("vbf_pvassoc_jets_current","vbf_pvassoc_jets_current",25,np.array(xbins))
vbf_beta_jets = ROOT.TH1D("vbf_beta_jets","vbf_beta_jets",25,np.array(xbins))
vbf_betaneg_jets = ROOT.TH1D("vbf_betaneg_jets","vbf_betaneg_jets",25,np.array(xbins))

vbf_all_jets.Sumw2()
vbf_pvassoc_jets.Sumw2()
vbf_all_jets_current.Sumw2()
vbf_pvassoc_jets_current.Sumw2()
vbf_beta_jets.Sumw2()
vbf_betaneg_jets.Sumw2()


jetpvtree.Draw("abs(jet_eta)>>vbf_all_jets","jet_flav!=0&&jet_flav!=21&&jet_pt>20")
jetpvtree.Draw("abs(jet_eta)>>vbf_pvassoc_jets","jet_flav!=0&&jet_flav!=21&&jet_beta>0.1&&jet_pt>20")
jetpvtree.Draw("jet_beta>>vbf_beta_jets","jet_flav!=0&&jet_flav!=21&&jet_pt>20&&jet_beta>-1")
jetpvtree.Draw("(jet_beta+1)>>vbf_betaneg_jets","jet_flav!=0&&jet_flav!=21&&jet_pt>20&&jet_beta<0")
jetpvtree_current.Draw("abs(jet_eta)>>vbf_all_jets_current","jet_flav!=0&&jet_flav!=21&&jet_pt>20")
jetpvtree_current.Draw("abs(jet_eta)>>vbf_pvassoc_jets_current","jet_flav!=0&&jet_flav!=21&&jet_beta>0.1&&jet_pt>20")

vbf_beta_jets.Add(vbf_betaneg_jets)
vbf_beta_jets.Scale(1./vbf_beta_jets.Integral())

vbf_pvassoc_jets.Divide(vbf_all_jets)
vbf_pvassoc_jets_current.Divide(vbf_all_jets_current)

vbf_pvassoc_jets.SetMarkerStyle(20)
vbf_pvassoc_jets.SetMarkerColor(ROOT.kRed)
vbf_pvassoc_jets.SetTitle("")
vbf_pvassoc_jets.SetStats(0)
vbf_pvassoc_jets.GetYaxis().SetRangeUser(0,1.4)

vbf_pvassoc_jets_current.SetMarkerStyle(24)
vbf_pvassoc_jets_current.SetMarkerColor(ROOT.kRed)

plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
vbf_pvassoc_jets.GetXaxis().SetTitle("VBF jet |#eta|")
vbf_pvassoc_jets.GetYaxis().SetTitle("jet PV association efficiency")
legend.AddEntry(vbf_pvassoc_jets,'Current tracker, jets without CHS','P')
legend.AddEntry(vbf_pvassoc_jets_current,'Current tracker, jets with CHS','P')
vbf_pvassoc_jets.Draw("EP0")
vbf_pvassoc_jets_current.Draw("P0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=13 TeV, Moriond17 PU", 3)

canv.SaveAs('jetpvassoc_CHStest.pdf')
canv.Print('jetpvassoc_CHStest.png')

#canv2=ROOT.TCanvas()
#pads2 = plot.OnePad()
#for padx in pads2:
    # Use tick marks on oppsite axis edges
#    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

#legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
#plot.Set(legend, NColumns=1)
#vbf_beta_jets.GetXaxis().SetTitle("VBF jet |#eta|")
#vbf_beta_jets.GetYaxis().SetTitle("#beta")
#legend.AddEntry(vbf_pvassoc_jets,'With tracker extension','P')
#legend.AddEntry(vbf_pvassoc_jets_current,'Without tracker extension (current)','P')
#vbf_pvassoc_jets.Draw("EP0")
#vbf_beta_jets.Draw("L")
#vbf_pvassoc_jets_current.Draw("P0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

#legend.Draw()

#plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
#plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV, 0 PU", 3)

#canv2.SaveAs('jetbeta.pdf')
#canv2.Print('jetbeta.png')

