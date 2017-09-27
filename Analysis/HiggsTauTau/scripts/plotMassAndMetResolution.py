import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

infileHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/puppi_GluGlu_HToTauTau_Summer16_BarrelTDR_mt_0.root"
infilenormHTT = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/GluGlu_HToTauTau_Summer16_BarrelTDR_mt_0.root"
infileDY = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/puppi_DYJetsToLL_Summer16_BarrelTDR_mt_0.root"

htt_file=ROOT.TFile.Open(infileHTT)
htt_file_norm=ROOT.TFile.Open(infilenormHTT)
dy_file=ROOT.TFile.Open(infileDY)
htt_tree=htt_file.Get("ntuple")
htt_tree_norm=htt_file_norm.Get("ntuple")
dy_tree=dy_file.Get("ntuple")

htt_msv_res10 = ROOT.TH1D("htt_msv_res10","htt_msv_res10",28,40,180)
htt_met_res = ROOT.TH1D("htt_met_res","htt_met_res",100,0,1000)
htt_met_res_norm = ROOT.TH1D("htt_met_res_norm","htt_met_res_norm",100,0,1000)
htt_norm_msv_res10 = ROOT.TH1D("htt_norm_msv_res10","htt_norm_msv_res10",28,40,180)
htt_msv_res = ROOT.TH1D("htt_msv_res","htt_msv_res",28,40,180)
htt_norm_msv_res = ROOT.TH1D("htt_norm_msv_res","htt_norm_msv_res",28,40,180)
dy_msv_res10 = ROOT.TH1D("dy_msv_res10","dy_msv_res10",28,40,180)
htt_msv_res20 = ROOT.TH1D("htt_msv_res20","htt_msv_res20",28,40,180)
htt_norm_msv_res20 = ROOT.TH1D("htt_norm_msv_res20","htt_norm_msv_res20",28,40,180)
dy_msv_res20 = ROOT.TH1D("dy_msv_res20","dy_msv_res20",28,40,180)
htt_msv_res30 = ROOT.TH1D("htt_msv_res30","htt_msv_res30",28,40,180)
htt_norm_msv_res30 = ROOT.TH1D("htt_norm_msv_res30","htt_norm_msv_res30",28,40,180)
dy_msv_res30 = ROOT.TH1D("dy_msv_res30","dy_msv_res30",28,40,180)
htt_msv_res40 = ROOT.TH1D("htt_msv_res40","htt_msv_res40",28,40,180)
htt_norm_msv_res40 = ROOT.TH1D("htt_norm_msv_res40","htt_norm_msv_res40",28,40,180)
dy_msv_res40 = ROOT.TH1D("dy_msv_res40","dy_msv_res40",28,40,180)
htt_msv_res50 = ROOT.TH1D("htt_msv_res50","htt_msv_res50",28,40,180)
htt_norm_msv_res50 = ROOT.TH1D("htt_norm_msv_res50","htt_norm_msv_res50",28,40,180)
dy_msv_res50 = ROOT.TH1D("dy_msv_res50","dy_msv_res50",28,40,180)
htt_msv_res60 = ROOT.TH1D("htt_msv_res60","htt_msv_res60",28,40,180)
htt_norm_msv_res60 = ROOT.TH1D("htt_norm_msv_res60","htt_norm_msv_res60",28,40,180)
dy_msv_res60 = ROOT.TH1D("dy_msv_res60","dy_msv_res60",28,40,180)

htt_mvis = ROOT.TH1D("htt_mvis","htt_mvis",28,0,140)
dy_mvis = ROOT.TH1D("dy_mvis","dy_mvis",28,0,140)

htt_msvres_metres = ROOT.TH1D("htt_msvres_metres","htt_msvres_metres",6,0,60)
htt_msvres_metres_fine = ROOT.TH1D("htt_msvres_metres_fine","htt_msvres_metres_fine",60,0,60)


htt_tree.Draw("abs(pfmet_par_res)>>htt_met_res")
htt_tree_norm.Draw("abs(pfmet_par_res)>>htt_met_res_norm")
htt_tree.Draw("m_sv>>htt_msv_res","abs(eta_2)<1.479")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res","abs(eta_2)<1.479")
htt_tree.Draw("m_sv>>htt_msv_res10","abs(pfmet_par_res)<10")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res10","abs(pfmet_par_res)<10")
dy_tree.Draw("m_sv>>dy_msv_res10","abs(pfmet_par_res)<10")
htt_tree.Draw("m_sv>>htt_msv_res20","abs(pfmet_par_res)<20&&abs(pfmet_par_res)>10")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res20","abs(pfmet_par_res)<20&&abs(pfmet_par_res)>10")
dy_tree.Draw("m_sv>>dy_msv_res20","abs(pfmet_par_res)<20&&abs(pfmet_par_res)>10")
htt_tree.Draw("m_sv>>htt_msv_res30","abs(pfmet_par_res)<30&&abs(pfmet_par_res)>20")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res30","abs(pfmet_par_res)<30&&abs(pfmet_par_res)>20")
dy_tree.Draw("m_sv>>dy_msv_res30","abs(pfmet_par_res)<30&&abs(pfmet_par_res)>20")
htt_tree.Draw("m_sv>>htt_msv_res40","abs(pfmet_par_res)<40&&abs(pfmet_par_res)>30")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res40","abs(pfmet_par_res)<40&&abs(pfmet_par_res)>30")
dy_tree.Draw("m_sv>>dy_msv_res40","abs(pfmet_par_res)<40&&abs(pfmet_par_res)>30")
htt_tree.Draw("m_sv>>htt_msv_res50","abs(pfmet_par_res)<50&&abs(pfmet_par_res)>40")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res50","abs(pfmet_par_res)<50&&abs(pfmet_par_res)>40")
dy_tree.Draw("m_sv>>dy_msv_res50","abs(pfmet_par_res)<50&&abs(pfmet_par_res)>40")
htt_tree.Draw("m_sv>>htt_msv_res60","abs(pfmet_par_res)<60&&abs(pfmet_par_res)>50")
htt_tree_norm.Draw("m_sv>>htt_norm_msv_res60","abs(pfmet_par_res)<60&&abs(pfmet_par_res)>50")
dy_tree.Draw("m_sv>>dy_msv_res60","abs(pfmet_par_res)<60&&abs(pfmet_par_res)>50")

htt_tree.Draw("m_vis>>htt_mvis","abs(eta_2)<1.479")
dy_tree.Draw("m_vis>>dy_mvis","abs(eta_2)<1.479")

metres=htt_met_res.GetMean(1)
metres_norm=htt_met_res_norm.GetMean(1)

msvres = htt_msv_res.GetStdDev(1)/htt_msv_res.GetMean(1)
msvres_norm = htt_norm_msv_res.GetStdDev(1)/htt_norm_msv_res.GetMean(1)
msvres_10 = htt_msv_res10.GetStdDev(1)/htt_msv_res10.GetMean(1)
msvres_20 = htt_msv_res20.GetStdDev(1)/htt_msv_res20.GetMean(1)
msvres_30 = htt_msv_res30.GetStdDev(1)/htt_msv_res30.GetMean(1)
msvres_40 = htt_msv_res40.GetStdDev(1)/htt_msv_res40.GetMean(1)
msvres_50 = htt_msv_res50.GetStdDev(1)/htt_msv_res50.GetMean(1)
msvres_60 = htt_msv_res60.GetStdDev(1)/htt_msv_res60.GetMean(1)
msvres_norm_10 = htt_norm_msv_res10.GetStdDev(1)/htt_norm_msv_res10.GetMean(1)
msvres_norm_20 = htt_norm_msv_res20.GetStdDev(1)/htt_norm_msv_res20.GetMean(1)
msvres_norm_30 = htt_norm_msv_res30.GetStdDev(1)/htt_norm_msv_res30.GetMean(1)
msvres_norm_40 = htt_norm_msv_res40.GetStdDev(1)/htt_norm_msv_res40.GetMean(1)
msvres_norm_50 = htt_norm_msv_res50.GetStdDev(1)/htt_norm_msv_res50.GetMean(1)
msvres_norm_60 = htt_norm_msv_res60.GetStdDev(1)/htt_norm_msv_res60.GetMean(1)

htt_msvres_metres.SetBinContent(1,msvres_norm_10)
htt_msvres_metres.SetBinContent(2,msvres_norm_20)
htt_msvres_metres.SetBinContent(3,msvres_norm_30)
htt_msvres_metres.SetBinContent(4,msvres_norm_40)
htt_msvres_metres.SetBinContent(5,msvres_norm_50)
htt_msvres_metres.SetBinContent(6,msvres_norm_60)

mvisres = htt_mvis.GetStdDev(1)/htt_mvis.GetMean(1)


print "mvis ", mvisres
print "msv ", msvres_norm
print msvres_norm,mvisres
print metres_norm,metres
print msvres_10,msvres_norm_10
print msvres_20,msvres_norm_20
print msvres_30,msvres_norm_30
print msvres_40,msvres_norm_40
print msvres_50,msvres_norm_50
print msvres_60,msvres_norm_60



htt_msvres_metres.SetMarkerColor(ROOT.kRed)
htt_msvres_metres.SetMarkerStyle(20)


plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.12, 3, 0.015)
plot.Set(legend, NColumns=1)
htt_msvres_metres.GetYaxis().SetRangeUser(0.1,0.3)
htt_msvres_metres.GetXaxis().SetTitle("#sigma(MET,perp)")
htt_msvres_metres.GetYaxis().SetTitleOffset(1.4)
htt_msvres_metres.GetYaxis().SetTitle("#sigma(m_{#tau,#tau})/< m_{#tau,#tau}>")
htt_msvres_metres.Draw("P")
linemvis = ROOT.TLine()
linemvis.SetLineColor(ROOT.kBlue)
linePFMET = ROOT.TLine()
linePFMET.SetLineColor(ROOT.kGreen+3)
linePFMET.SetLineStyle(2)
plot.DrawHorizontalLine(pads[0],linemvis,mvisres)
plot.DrawVerticalLine(pads[0],linePFMET,metres_norm)

legend.AddEntry(htt_msvres_metres,'m_{#tau,#tau} resolution ','P')
legend.AddEntry(linemvis,'m_{vis} resolution ', 'L')
legend.AddEntry(linePFMET,'PF Met resolution ', 'L')
legend.Draw()


plot.DrawCMSLogo(pads[0], 'CMS ', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "#mu#tau_{h} channel", 1)
plot.DrawTitle(pads[0], "13 TeV, Run 2", 3)

canv.SaveAs('msvres_metres_summer16.pdf')
canv.Print('msvres_metres_summer16.png')

'''
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

'''
