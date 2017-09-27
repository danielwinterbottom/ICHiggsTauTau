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
#infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/ggH_future_full_tt_0.root"

vbf_file=ROOT.TFile.Open(infile)
pairtree=vbf_file.Get("geninfo")

xbins = [-5.0,-4.8,-4.6,-4.4,-4.2,-4.0,-3.8,-3.6,-3.4,-3.2,-3.,-2.8,-2.6,-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2,4.4,4.6,4.8,5.0]
#muon_eta = ROOT.TH1D("muon_eta","muon_eta",50,np.array(xbins))
#electron_eta = ROOT.TH1D("electron_eta","electron_eta",50,np.array(xbins))
tau1_eta = ROOT.TH1D("tau1_eta","tau1_eta",50,np.array(xbins))
tau2_eta = ROOT.TH1D("tau2_eta","tau2_eta",50,np.array(xbins))


#pairtree.Draw("(tau1_eta)>>muon_eta","abs(tau1_type)==13&&tau1_pt>20")
#pairtree.Draw("(tau1_eta)>>electron_eta","abs(tau1_type)==11&&tau1_pt>20")
pairtree.Draw("(tau1_eta)>>tau1_eta","abs(tau1_type)==15&&tau1_pt>20")
pairtree.Draw("(tau2_eta)>>tau2_eta","abs(tau2_type)==15&&tau2_pt>20")
tau1_eta.Sumw2()
tau2_eta.Sumw2()

tau1_eta.Add(tau2_eta)

tau1_eta.Scale(1./tau1_eta.Integral())

xvalues = []
ymuonvalues = []
yelectronvalues = []
ytauvalues = []

#for i in range(1,51):
#  xvalues.append(muon_eta.GetBinCenter(i))
#  ymuonvalues.append(muon_eta.GetBinContent(i))
#  yelectronvalues.append(electron_eta.GetBinContent(i))
#  ytauvalues.append(tau_eta.GetBinContent(i))

#print xvalues
#print ymuonvalues
#print yelectronvalues
#print ytauvalues

#muon_eta_graph = ROOT.TGraph(50,np.array(xvalues),np.array(ymuonvalues))
#electron_eta_graph = ROOT.TGraph(50,np.array(xvalues),np.array(yelectronvalues))
#tau_eta_graph = ROOT.TGraph(50,np.array(xvalues),np.array(ytauvalues))
#muon_eta_graph.SetLineStyle(1)
#muon_eta_graph.SetLineColor(ROOT.kBlue)
#electron_eta_graph.SetLineStyle(1)
#electron_eta_graph.SetLineColor(ROOT.kRed)
#tau_eta_graph.SetLineStyle(1)
#tau_eta_graph.SetLineColor(ROOT.kGreen+3)
tau1_eta.SetLineStyle(1)
tau1_eta.SetLineColor(ROOT.kGreen+3)


plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
#canv=ROOT.TCanvas()
#pads = plot.OnePad()
#for padx in pads:
    # Use tick marks on oppsite axis edges
#    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

#legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
#plot.Set(legend, NColumns=2)
#muon_eta_graph.GetXaxis().SetTitle("Muon #eta")
#muon_eta_graph.GetYaxis().SetTitle("a.u.")
#muon_eta_graph.Draw("LAXIS")

#plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
#plot.DrawTitle(pads[0], "14 TeV, 0 PU", 3)

#canv.SaveAs('muonetadist.pdf')
#canv.Print('muonetadist.png')

#canv2=ROOT.TCanvas()
#pads2 = plot.OnePad()
#for padx in pads2:
    # Use tick marks on oppsite axis edges
#    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

#legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
#plot.Set(legend, NColumns=2)
#electron_eta_graph.GetXaxis().SetTitle("Electron #eta")
#electron_eta_graph.GetYaxis().SetTitle("a.u.")
#electron_eta_graph.Draw("LAXIS")

#plot.DrawCMSLogo(pads2[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawTitle(pads2[0], "VBF H#rightarrow#tau#tau", 1)
#plot.DrawTitle(pads2[0], "14 TeV, 0 PU", 3)

#canv2.SaveAs('electronetadist.pdf')
#canv2.Print('electronetadist.png')

canv3=ROOT.TCanvas()
pads3 = plot.OnePad()
for padx in pads3:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=2)
tau1_eta.GetXaxis().SetTitle("#tau_{h} #eta")
tau1_eta.GetYaxis().SetTitle("a.u.")
tau1_eta.Draw("L")

plot.DrawCMSLogo(pads3[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads3[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads3[0], "#sqrt{s}=14 TeV, 0 PU", 3)

canv3.SaveAs('tauetadistVBFFull.pdf')
canv3.Print('tauetadistVBFFull.png')



