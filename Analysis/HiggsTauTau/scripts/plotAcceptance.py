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
pairtree=vbf_file.Get("geninfo")

xbins = [15.,25.,35.,45.,55.,65.,75.,85.,95.,105.,115.]
xbins_eta = [-5.0,-4.8,-4.6,-4.4,-4.2,-4.0,-3.8,-3.6,-3.4,-3.2,-3.,-2.8,-2.6,-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2,4.4,4.6,4.8,5.0]
tau1_genpt_full = ROOT.TH1D("tau1_genpt_full","tau1_genpt_full",10,np.array(xbins))
tau1_genpt_eta23 = ROOT.TH1D("tau1_genpt_eta23","tau1_genpt_eta23",10,np.array(xbins))
tau1_genpt_eta3 = ROOT.TH1D("tau1_genpt_eta3","tau1_genpt_eta3",10,np.array(xbins))
tau1_genpt_eta4 = ROOT.TH1D("tau1_genpt_eta4","tau1_genpt_eta4",10,np.array(xbins))
tau2_genpt_full = ROOT.TH1D("tau2_genpt_full","tau2_genpt_full",10,np.array(xbins))
tau2_genpt_eta23 = ROOT.TH1D("tau2_genpt_eta23","tau2_genpt_eta23",10,np.array(xbins))
tau2_genpt_eta3 = ROOT.TH1D("tau2_genpt_eta3","tau2_genpt_eta3",10,np.array(xbins))
tau2_genpt_eta4 = ROOT.TH1D("tau2_genpt_eta4","tau2_genpt_eta4",10,np.array(xbins))
tau1_eta_full = ROOT.TH1D("tau1_eta_full","tau1_eta_full",50,np.array(xbins_eta))
tau1_eta_genpt20 = ROOT.TH1D("tau1_eta_genpt20","tau1_eta_genpt20",50,np.array(xbins_eta))
tau2_eta_full = ROOT.TH1D("tau2_eta_full","tau2_eta_full",50,np.array(xbins_eta))
tau2_eta_genpt20 = ROOT.TH1D("tau2_eta_genpt20","tau2_eta_genpt20",50,np.array(xbins_eta))



pairtree.Draw("(tau1_pt)>>tau1_genpt_full","abs(tau1_type)==15&&tau1_pt>15")
pairtree.Draw("(tau1_pt)>>tau1_genpt_eta23","abs(tau1_type)==15&&tau1_pt>15&&abs(tau1_eta)<2.5")
pairtree.Draw("(tau1_pt)>>tau1_genpt_eta3","abs(tau1_type)==15&&tau1_pt>15&&abs(tau1_eta)<3")
pairtree.Draw("(tau1_pt)>>tau1_genpt_eta4","abs(tau1_type)==15&&tau1_pt>15&&abs(tau1_eta)<4")
pairtree.Draw("(tau2_pt)>>tau2_genpt_full","abs(tau2_type)==15&&tau2_pt>15")
pairtree.Draw("(tau2_pt)>>tau2_genpt_eta23","abs(tau2_type)==15&&tau2_pt>15&&abs(tau2_eta)<2.5")
pairtree.Draw("(tau2_pt)>>tau2_genpt_eta3","abs(tau2_type)==15&&tau2_pt>15&&abs(tau2_eta)<3")
pairtree.Draw("(tau2_pt)>>tau2_genpt_eta4","abs(tau2_type)==15&&tau2_pt>15&&abs(tau2_eta)<4")

pairtree.Draw("(tau1_eta)>>tau1_eta_full","abs(tau1_type)==15&&tau1_pt>15")
pairtree.Draw("(tau2_eta)>>tau2_eta_full","abs(tau2_type)==15&&tau2_pt>15")
pairtree.Draw("(tau1_eta)>>tau1_eta_genpt20","abs(tau1_type)==15&&tau1_pt>15&&abs(tau1_eta)<4")
pairtree.Draw("(tau2_eta)>>tau2_eta_genpt20","abs(tau2_type)==15&&tau2_pt>15&&abs(tau2_eta)<4")

tau1_genpt_full.Sumw2()
tau1_genpt_eta23.Sumw2()
tau1_genpt_eta3.Sumw2()
tau1_genpt_eta4.Sumw2()
tau2_genpt_full.Sumw2()
tau2_genpt_eta23.Sumw2()
tau2_genpt_eta3.Sumw2()
tau2_genpt_eta4.Sumw2()

tau1_eta_full.Sumw2()
tau2_eta_full.Sumw2()
tau1_eta_genpt20.Sumw2()
tau2_eta_genpt20.Sumw2()

tau1_genpt_full.Add(tau2_genpt_full)
tau1_genpt_eta23.Add(tau2_genpt_eta23)
tau1_genpt_eta3.Add(tau2_genpt_eta3)
tau1_genpt_eta4.Add(tau2_genpt_eta4)
tau1_eta_full.Add(tau2_eta_full)
tau1_eta_genpt20.Add(tau2_eta_genpt20)
print "total: ",tau1_eta_full.Integral()
print "with |eta|<2.5: ",tau1_eta_genpt20.Integral()

tau1_genpt_eta23.Divide(tau1_genpt_full)
tau1_genpt_eta3.Divide(tau1_genpt_full)
tau1_genpt_eta4.Divide(tau1_genpt_full)
tau1_eta_genpt20.Divide(tau1_eta_full)

for i in range(0,tau1_genpt_eta23.GetNbinsX()):
  print "bin center: ", tau1_genpt_eta23.GetBinCenter(i+1), "acceptance |eta|<2.3: ", tau1_genpt_eta23.GetBinContent(i+1)
  print "bin center: ", tau1_genpt_eta3.GetBinCenter(i+1), "acceptance |eta|<3: ", tau1_genpt_eta3.GetBinContent(i+1)
  print "bin center: ", tau1_genpt_eta4.GetBinCenter(i+1), "acceptance |eta|<4: ", tau1_genpt_eta4.GetBinContent(i+1)

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
tau1_genpt_eta23.SetMarkerStyle(23)
tau1_genpt_eta23.SetMarkerColor(ROOT.kRed)
tau1_genpt_eta3.SetMarkerStyle(24)
tau1_genpt_eta3.SetMarkerColor(ROOT.kBlue)
tau1_genpt_eta4.SetMarkerStyle(25)
tau1_genpt_eta4.SetMarkerColor(ROOT.kGreen+3)
tau1_eta_genpt20.SetMarkerStyle(20)
tau1_eta_genpt20.SetMarkerColor(ROOT.kRed)


plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend, NColumns=1)
tau1_genpt_eta23.GetXaxis().SetTitleOffset(1.2)
tau1_genpt_eta23.GetXaxis().SetTitle("Visible generator #tau_{h} p_{T} (GeV)")
tau1_genpt_eta23.GetYaxis().SetTitleOffset(1.2)
tau1_genpt_eta23.GetYaxis().SetTitle("#tau_{h} acceptance")
tau1_genpt_eta23.GetYaxis().SetRangeUser(0.6,1.2)
tau1_genpt_eta23.Draw("P")
legend.AddEntry(tau1_genpt_eta23,'#tau_{h} generator |#eta| < 2.5','P')
tau1_genpt_eta3.Draw("PSAME")
legend.AddEntry(tau1_genpt_eta3,'#tau_{h} generator |#eta| < 3','P')
tau1_genpt_eta4.Draw("PSAME")
legend.AddEntry(tau1_genpt_eta4,'#tau_{h} generator |#eta| < 4','P')
legend.Draw()


plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "14 TeV, 0 PU", 3)

canv.SaveAs('tau_acceptance_VBF_FULL_noprelim.pdf')
canv.Print('tau_acceptance_VBF_FULL_noprelim.png')

canveta=ROOT.TCanvas()
padseta = plot.OnePad()
for padx in padseta:
    plot.Set(padx, Tickx=1, Ticky=1,Logx=False)

legendeta = plot.PositionedLegend(0.45,0.10,3,0.015)
plot.Set(legendeta,NColumns=1)
tau1_eta_genpt20.GetXaxis().SetTitle("Gen #tau_{h} #eta")
tau1_eta_genpt20.GetYaxis().SetTitle("#tau_{h} acceptance")
tau1_eta_genpt20.GetYaxis().SetRangeUser(0.,1.0)
legendeta.AddEntry(tau1_eta_genpt20,"#tau_{h} vis gen p_{T} > 20 GeV",'P')
tau1_eta_genpt20.Draw("P")
legendeta.Draw()

plot.DrawCMSLogo(padseta[0], 'CMS Phase 2', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(padseta[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(padseta[0], "#sqrt{s}=14 TeV, 0 PU", 3)

canveta.SaveAs('tau_acceptance_eta_VBF_FULL.pdf')
canveta.Print('tau_acceptance_eta_VBF_FULL.png')


