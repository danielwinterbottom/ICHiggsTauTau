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
infileHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBFHToTauTau-0PU-tau_resolution_mt_0.root"
infileTTbar1000 = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/TTbar-relval-1000-tau_resolution_mt_0.root"
infileTTbar3000 = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/TTbar-relval-3000-tau_resolution_mt_0.root"
infileTTbar300 = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/TTbar-relval-300-tau_resolution_mt_0.root"
#infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/DYJetsToLL_Summer16_BarrelTDR_mt_0.root"

htt_file=ROOT.TFile.Open(infileHTT)
ttbar1000_file=ROOT.TFile.Open(infileTTbar1000)
ttbar3000_file=ROOT.TFile.Open(infileTTbar3000)
ttbar300_file=ROOT.TFile.Open(infileTTbar300)
htt_tree=htt_file.Get("tauidinfo")
ttbar1000_tree=ttbar1000_file.Get("tauidinfo")
ttbar3000_tree=ttbar3000_file.Get("tauidinfo")
ttbar300_tree=ttbar300_file.Get("tauidinfo")


htt_resolution_dm0=ROOT.TH1D("htt_resolution_dm0","htt_resolution_dm0",20,-5,5)
ttbar1000_resolution_dm0=ROOT.TH1D("ttbar1000_resolution_dm0","ttbar1000_resolution_dm0",20,-5,5)
ttbar3000_resolution_dm0=ROOT.TH1D("ttbar3000_resolution_dm0","ttbar3000_resolution_dm0",20,-5,5)
ttbar300_resolution_dm0=ROOT.TH1D("ttbar300_resolution_dm0","ttbar300_resolution_dm0",20,-5,5)

htt_resolution_dm1=ROOT.TH1D("htt_resolution_dm1","htt_resolution_dm1",20,-5,5)
ttbar1000_resolution_dm1=ROOT.TH1D("ttbar1000_resolution_dm1","ttbar1000_resolution_dm1",20,-5,5)
ttbar3000_resolution_dm1=ROOT.TH1D("ttbar3000_resolution_dm1","ttbar3000_resolution_dm1",20,-5,5)
ttbar300_resolution_dm1=ROOT.TH1D("ttbar300_resolution_dm1","ttbar300_resolution_dm1",20,-5,5)

htt_resolution_dm10=ROOT.TH1D("htt_resolution_dm10","htt_resolution_dm10",20,-5,5)
ttbar1000_resolution_dm10=ROOT.TH1D("ttbar1000_resolution_dm10","ttbar1000_resolution_dm10",20,-5,5)
ttbar3000_resolution_dm10=ROOT.TH1D("ttbar3000_resolution_dm10","ttbar3000_resolution_dm10",20,-5,5)
ttbar300_resolution_dm10=ROOT.TH1D("ttbar300_resolution_dm10","ttbar300_resolution_dm10",20,-5,5)


htt_tree.Draw("tau_resolution>>htt_resolution_dm0","tau_dm==0")
htt_tree.Draw("tau_resolution>>htt_resolution_dm1","tau_dm==1")
htt_tree.Draw("tau_resolution>>htt_resolution_dm10","tau_dm==10")

ttbar1000_tree.Draw("tau_resolution>>ttbar1000_resolution_dm0","tau_dm==0")
ttbar1000_tree.Draw("tau_resolution>>ttbar1000_resolution_dm1","tau_dm==1")
ttbar1000_tree.Draw("tau_resolution>>ttbar1000_resolution_dm10","tau_dm==10")

ttbar3000_tree.Draw("tau_resolution>>ttbar3000_resolution_dm0","tau_dm==0")
ttbar3000_tree.Draw("tau_resolution>>ttbar3000_resolution_dm1","tau_dm==1")
ttbar3000_tree.Draw("tau_resolution>>ttbar3000_resolution_dm10","tau_dm==10")

ttbar300_tree.Draw("tau_resolution>>ttbar300_resolution_dm0","tau_dm==0")
ttbar300_tree.Draw("tau_resolution>>ttbar300_resolution_dm1","tau_dm==1")
ttbar300_tree.Draw("tau_resolution>>ttbar300_resolution_dm10","tau_dm==10")


htt_resolution_dm0.SetLineColor(ROOT.kRed)
htt_resolution_dm1.SetLineColor(ROOT.kRed)
htt_resolution_dm10.SetLineColor(ROOT.kRed)

ttbar1000_resolution_dm0.SetLineColor(ROOT.kBlue)
ttbar1000_resolution_dm1.SetLineColor(ROOT.kBlue)
ttbar1000_resolution_dm10.SetLineColor(ROOT.kBlue)

ttbar3000_resolution_dm0.SetLineColor(ROOT.kGreen+3)
ttbar3000_resolution_dm1.SetLineColor(ROOT.kGreen+3)
ttbar3000_resolution_dm10.SetLineColor(ROOT.kGreen+3)

ttbar300_resolution_dm0.SetLineColor(1)
ttbar300_resolution_dm1.SetLineColor(1)
ttbar300_resolution_dm10.SetLineColor(1)


htt_resolution_dm0.Scale(1./htt_resolution_dm0.Integral())
htt_resolution_dm1.Scale(1./htt_resolution_dm1.Integral())
htt_resolution_dm10.Scale(1./htt_resolution_dm10.Integral())

ttbar1000_resolution_dm0.Scale(1./ttbar1000_resolution_dm0.Integral())
ttbar1000_resolution_dm1.Scale(1./ttbar1000_resolution_dm1.Integral())
ttbar1000_resolution_dm10.Scale(1./ttbar1000_resolution_dm10.Integral())

ttbar3000_resolution_dm0.Scale(1./ttbar3000_resolution_dm0.Integral())
ttbar3000_resolution_dm1.Scale(1./ttbar3000_resolution_dm1.Integral())
ttbar3000_resolution_dm10.Scale(1./ttbar3000_resolution_dm10.Integral())

ttbar300_resolution_dm0.Scale(1./ttbar300_resolution_dm0.Integral())
ttbar300_resolution_dm1.Scale(1./ttbar300_resolution_dm1.Integral())
ttbar300_resolution_dm10.Scale(1./ttbar300_resolution_dm10.Integral())

htt_res_dm0 = htt_resolution_dm0.GetStdDev(1)
htt_res_dm1 = htt_resolution_dm1.GetStdDev(1)
htt_res_dm10 = htt_resolution_dm10.GetStdDev(1)

ttbar300_res_dm0 = ttbar300_resolution_dm0.GetStdDev(1)
ttbar300_res_dm1 = ttbar300_resolution_dm1.GetStdDev(1)
ttbar300_res_dm10 = ttbar300_resolution_dm10.GetStdDev(1)

ttbar1000_res_dm0 = ttbar1000_resolution_dm0.GetStdDev(1)
ttbar1000_res_dm1 = ttbar1000_resolution_dm1.GetStdDev(1)
ttbar1000_res_dm10 = ttbar1000_resolution_dm10.GetStdDev(1)


ttbar3000_res_dm0 = ttbar3000_resolution_dm0.GetStdDev(1)
ttbar3000_res_dm1 = ttbar3000_resolution_dm1.GetStdDev(1)
ttbar3000_res_dm10 = ttbar3000_resolution_dm10.GetStdDev(1)

print "HTT", htt_res_dm0,htt_res_dm1,htt_res_dm10
print "TTbar,aged300", ttbar300_res_dm0,ttbar300_res_dm1,ttbar300_res_dm10
print "TTbar,aged1000", ttbar1000_res_dm0,ttbar1000_res_dm1,ttbar1000_res_dm10
print "TTbar,aged3000", ttbar3000_res_dm0,ttbar3000_res_dm1,ttbar3000_res_dm10


plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend, NColumns=1)
htt_resolution_dm0.GetXaxis().SetTitle("Reco tau p_{T} - Gen tau p_{T} [GeV]")
htt_resolution_dm0.GetYaxis().SetTitleOffset(1.4)
htt_resolution_dm0.GetYaxis().SetTitle("a.u.")
htt_resolution_dm0.GetYaxis().SetRangeUser(0,0.5)
htt_resolution_dm0.Draw("")
ttbar1000_resolution_dm0.Draw("SAME")
ttbar3000_resolution_dm0.Draw("SAME")
ttbar300_resolution_dm0.Draw("SAME")
legend.AddEntry(htt_resolution_dm0,'Signal sample','L')
legend.AddEntry(ttbar300_resolution_dm0,'ttbar relval (aging 300)','L')
legend.AddEntry(ttbar1000_resolution_dm0,'ttbar relval (aging 1000)','L')
legend.AddEntry(ttbar3000_resolution_dm0,'ttbar relval (aging 3000)','L')
legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "1prong0pi0",1)
plot.DrawTitle(pads[0], "14 TeV, 0 PU", 3)

canv.SaveAs('resolution_comparison_dm0.pdf')
canv.Print('resolution_comparison_dm0.png')



canv2=ROOT.TCanvas()
pads2 = plot.OnePad()
for padx in pads2:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend2 = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend2, NColumns=1)
htt_resolution_dm1.GetXaxis().SetTitle("Reco tau p_{T} - gen tau p_{T} [GeV]")
htt_resolution_dm1.GetYaxis().SetTitleOffset(1.4)
htt_resolution_dm1.GetYaxis().SetTitle("a.u.")
htt_resolution_dm1.GetYaxis().SetRangeUser(0,0.5)
htt_resolution_dm1.Draw("")
ttbar1000_resolution_dm1.Draw("SAME")
ttbar3000_resolution_dm1.Draw("SAME")
ttbar300_resolution_dm1.Draw("SAME")
legend2.AddEntry(htt_resolution_dm1,'Signal sample','L')
legend2.AddEntry(ttbar300_resolution_dm1,'ttbar relval (aging 300)','L')
legend2.AddEntry(ttbar1000_resolution_dm1,'ttbar relval (aging 1000)','L')
legend2.AddEntry(ttbar3000_resolution_dm1,'ttbar relval (aging 3000)','L')
legend2.Draw()

plot.DrawCMSLogo(pads2[0], 'CMS Phase-2', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads2[0], "1prong1pi0",1)
plot.DrawTitle(pads2[0], "14 TeV, 0 PU", 3)

canv2.SaveAs('resolution_comparison_dm1.pdf')
canv2.Print('resolution_comparison_dm1.png')

canv3=ROOT.TCanvas()
pads3 = plot.OnePad()
for padx in pads3:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend3 = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend3, NColumns=1)
htt_resolution_dm10.GetXaxis().SetTitle("Reco tau p_{T} - gen tau p_{T} [GeV]")
htt_resolution_dm10.GetYaxis().SetTitleOffset(1.4)
htt_resolution_dm10.GetYaxis().SetTitle("a.u.")
htt_resolution_dm10.GetYaxis().SetRangeUser(0,0.7)
htt_resolution_dm10.Draw("")
ttbar1000_resolution_dm10.Draw("SAME")
ttbar3000_resolution_dm10.Draw("SAME")
ttbar300_resolution_dm10.Draw("SAME")
legend3.AddEntry(htt_resolution_dm10,'Signal sample','L')
legend3.AddEntry(ttbar300_resolution_dm10,'ttbar relval (aging 300)','L')
legend3.AddEntry(ttbar1000_resolution_dm10,'ttbar relval (aging 1000)','L')
legend3.AddEntry(ttbar3000_resolution_dm10,'ttbar relval (aging 3000)','L')
legend3.Draw()

plot.DrawCMSLogo(pads3[0], 'CMS Phase-2', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads3[0], "3prong0pi0",1)
plot.DrawTitle(pads3[0], "14 TeV, 0 PU", 3)

canv3.SaveAs('resolution_comparison_dm10.pdf')
canv3.Print('resolution_comparison_dm10.png')

