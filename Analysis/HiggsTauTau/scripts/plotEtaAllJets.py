import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_future_full_tt_0.root"
#infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_UPG_tt_0.root"

vbf_file=ROOT.TFile.Open(infile)
pairtree=vbf_file.Get("jetpvassoc")

xbins = [0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2,4.4,4.6,4.8,5.0]
#xbins = [0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,4.5,4.6,4.7,4.8,4.9,5.0]
vbf_lead_jet = ROOT.TH1D("vbf_lead_jet","vbf_lead_jet",25,np.array(xbins))
#vbf_sublead_jet = ROOT.TH1D("vbf_sublead_jet","vbf_sublead_jet",25,np.array(xbins))
vbf_lead_jet.Sumw2()
#vbf_sublead_jet.Sumw2()


pairtree.Draw("abs(genjet_eta)>>vbf_lead_jet","jet_pt>20&&jet_flav!=0&&jet_flav!=21")
vbf_lead_jet.Scale(1./vbf_lead_jet.Integral())

xvalues = []
yleadvalues = []
ysubleadvalues = []

for i in range(1,26):
  xvalues.append(vbf_lead_jet.GetBinCenter(i))
  yleadvalues.append(vbf_lead_jet.GetBinContent(i))


vbf_lead_jet_graph = ROOT.TGraph(25,np.array(xvalues),np.array(yleadvalues))
vbf_lead_jet_graph.SetLineStyle(1)
vbf_lead_jet_graph.SetLineColor(ROOT.kBlue)
vbf_lead_jet_graph.GetYaxis().SetRangeUser(0,0.1)
vbf_lead_jet.SetLineStyle(1)
vbf_lead_jet.SetLineColor(ROOT.kBlue)
vbf_lead_jet.GetYaxis().SetRangeUser(0,0.1)


plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
vbf_lead_jet_graph.GetXaxis().SetTitle("VBF jet |#eta|")
vbf_lead_jet_graph.GetYaxis().SetTitle("a.u.")
vbf_lead_jet.GetXaxis().SetTitle("VBF jet |#eta|")
vbf_lead_jet.GetYaxis().SetTitle("a.u.")
#legend.AddEntry(vbf_lead_jet_graph,'Leading VBF jet','L')
#legend.AddEntry(vbf_sublead_jet_graph,'Subleading VBF jet','L')

vbf_lead_jet.Draw("L")
#vbf_lead_jet_graph.Draw("LAXIS")
#vbf_sublead_jet_graph.Draw("LSAME")


#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

#legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV, 0 PU", 3)

canv.SaveAs('jetetadist_alljets.pdf')
canv.Print('jetetadist_alljets.png')

