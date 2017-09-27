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

vbf_file=ROOT.TFile.Open(infile)
pairtree=vbf_file.Get("tauidinfo")

xbins_eta = [-5.0,-4.8,-4.6,-4.4,-4.2,-4.0,-3.8,-3.6,-3.4,-3.2,-3.,-2.8,-2.6,-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2,4.4,4.6,4.8,5.0]
tau_eta_full = ROOT.TH1D("tau_eta_full","tau_eta_full",50,np.array(xbins_eta))
tau_eta_dmfinding = ROOT.TH1D("tau_eta_dmfinding","tau_eta_dmfinding",50,np.array(xbins_eta))
tau_eta_mvaisop9 = ROOT.TH1D("tau_eta_mvaisop9","tau_eta_mvaisop9",50,np.array(xbins_eta))
tau_eta_full_dm0_dmf = ROOT.TH1D("tau_eta_full_dm0_dmf","tau_eta_full_dm0_dmf",50,np.array(xbins_eta))
tau_eta_full_dm1_dmf = ROOT.TH1D("tau_eta_full_dm1_dmf","tau_eta_full_dm1_dmf",50,np.array(xbins_eta))
tau_eta_full_dm10_dmf = ROOT.TH1D("tau_eta_full_dm10_dmf","tau_eta_full_dm10_dmf",50,np.array(xbins_eta))

pairtree.Draw("(tau_eta)>>tau_eta_full","")
pairtree.Draw("(tau_eta)>>tau_eta_dmfinding","tau_dm_finding>0.5")
pairtree.Draw("(tau_eta)>>tau_eta_mvaisop9","tau_mvaiso_raw>0.9&&tau_dm_finding>0.5")
pairtree.Draw("(tau_eta)>>tau_eta_full_dm0_dmf","tau_dm==0&&tau_dm_finding>0.5")
pairtree.Draw("(tau_eta)>>tau_eta_full_dm1_dmf","tau_dm==1&&tau_dm_finding>0.5")
pairtree.Draw("(tau_eta)>>tau_eta_full_dm10_dmf","tau_dm==10&&tau_dm_finding>0.5")

tau_eta_full.Sumw2()
tau_eta_dmfinding.Sumw2()
tau_eta_full_dm0_dmf.Sumw2()
tau_eta_full_dm1_dmf.Sumw2()
tau_eta_full_dm10_dmf.Sumw2()
tau_eta_mvaisop9.Sumw2()

tau_eta_full_dm0_dmf.Divide(tau_eta_dmfinding)
tau_eta_full_dm1_dmf.Divide(tau_eta_dmfinding)
tau_eta_full_dm10_dmf.Divide(tau_eta_dmfinding)
tau_eta_dmfinding.Divide(tau_eta_full)
tau_eta_mvaisop9.Divide(tau_eta_full)

#xvalues = []
#ymuonvalues = []
#yelectronvalues = []
#ytauvalues = []

#for i in range(1,51):
#  xvalues.append(tau1_genpt_eta23.GetBinCenter(i))
#  ymuonvalues.append(muon_eta.GetBinContent(i))
#  yelectronvalues.append(electron_eta.GetBinContent(i))
#  ytauvalues.append(tau_eta.GetBinContent(i))

#muon_eta_graph = ROOT.TGraph(50,np.array(xvalues),np.array(ymuonvalues))
#electron_eta_graph = ROOT.TGraph(50,np.array(xvalues),np.array(yelectronvalues))
#tau_eta_graph = ROOT.TGraph(50,np.array(xvalues),np.array(ytauvalues))
#muon_eta_graph.SetLineStyle(1)
#muon_eta_graph.SetLineColor(ROOT.kBlue)
#electron_eta_graph.SetLineStyle(1)
#electron_eta_graph.SetLineColor(ROOT.kRed)
#tau_eta_graph.SetLineStyle(1)
#tau_eta_graph.SetLineColor(ROOT.kGreen+3)
tau_eta_dmfinding.SetMarkerStyle(20)
tau_eta_dmfinding.SetMarkerColor(ROOT.kRed)
tau_eta_mvaisop9.SetMarkerStyle(21)
tau_eta_mvaisop9.SetMarkerColor(ROOT.kGreen+3)

tau_eta_full_dm0_dmf.SetMarkerStyle(20)
tau_eta_full_dm0_dmf.SetMarkerColor(ROOT.kRed)
tau_eta_full_dm1_dmf.SetMarkerStyle(21)
tau_eta_full_dm1_dmf.SetMarkerColor(ROOT.kGreen+3)
tau_eta_full_dm10_dmf.SetMarkerStyle(22)
tau_eta_full_dm10_dmf.SetMarkerColor(ROOT.kBlue)

plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
tau_eta_dmfinding.GetXaxis().SetTitle("#tau_{h} #eta ")
tau_eta_dmfinding.GetYaxis().SetTitle("Efficiency ")
tau_eta_dmfinding.GetYaxis().SetRangeUser(0.0,1.2)
tau_eta_dmfinding.Draw("P")
legend.AddEntry(tau_eta_dmfinding,'DM finding','P')
tau_eta_mvaisop9.Draw("PSAME")
legend.AddEntry(tau_eta_mvaisop9,'MVA iso > 0.9','P')
legend.Draw()


plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV, 0 PU", 3)

canv.SaveAs('tau_efficiency_VBF.pdf')
canv.Print('tau_efficiency_VBF.png')

canveta=ROOT.TCanvas()
padseta = plot.OnePad()
for padx in padseta:
    plot.Set(padx, Tickx=1, Ticky=1,Logx=False)

legendeta = plot.PositionedLegend(0.45,0.10,3,0.015)
plot.Set(legendeta,NColumns=1)
tau_eta_full_dm0_dmf.GetXaxis().SetTitle("#tau_{h} #eta")
tau_eta_full_dm0_dmf.GetYaxis().SetTitle("DM fraction")
tau_eta_full_dm0_dmf.GetYaxis().SetRangeUser(0.,1.0)
legendeta.AddEntry(tau_eta_full_dm0_dmf,"1prong0pi0",'P')
legendeta.AddEntry(tau_eta_full_dm1_dmf,"1prong1pi0",'P')
legendeta.AddEntry(tau_eta_full_dm10_dmf,"3prong0pi0",'P')
tau_eta_full_dm0_dmf.Draw("P")
tau_eta_full_dm1_dmf.Draw("PSAME")
tau_eta_full_dm10_dmf.Draw("PSAME")
legendeta.Draw()

plot.DrawCMSLogo(padseta[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(padseta[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(padseta[0], "#sqrt{s}=14 TeV, 0 PU", 3)

canveta.SaveAs('tau_dmsplit_VBF.pdf')
canveta.Print('tau_dmsplit_VBF.png')


