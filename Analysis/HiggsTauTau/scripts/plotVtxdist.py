import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_pu200_partial_forvertex_tt_0.root"
infilenopu = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_pu0__forvertex_tt_0.root"

vbf_file=ROOT.TFile.Open(infile)
gentree=vbf_file.Get("geninfo")

vbf_filenopu=ROOT.TFile.Open(infilenopu)
gentreenopu=vbf_filenopu.Get("geninfo")



xbins = [0.,0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.2]
dz_200 = ROOT.TH1D("dz_200","dz_200",20,np.array(xbins))
dz_0 = ROOT.TH1D("dz_0","dz_0",20,np.array(xbins))
dy_200 = ROOT.TH1D("dy_200","dy_200",20,np.array(xbins))
dy_0 = ROOT.TH1D("dy_0","dy_0",20,np.array(xbins))
dx_200 = ROOT.TH1D("dx_200","dx_200",20,np.array(xbins))
dx_0 = ROOT.TH1D("dx_0","dx_0",20,np.array(xbins))


dz_200.Sumw2()
dz_0.Sumw2()
dy_200.Sumw2()
dy_0.Sumw2()
dx_200.Sumw2()
dx_0.Sumw2()

gentree.Draw("vtxdz>>dz_200","")
gentree.Draw("vtxdy>>dy_200","")
gentree.Draw("vtxdx>>dx_200","")
gentreenopu.Draw("vtxdz>>dz_0","")
gentreenopu.Draw("vtxdy>>dy_0","")
gentreenopu.Draw("vtxdx>>dx_0","")

print dz_200.Integral(0,21)
dz_200.Scale(1./dz_200.Integral(0,21))
dz_0.Scale(1./dz_0.Integral(0,21))
dy_200.Scale(1./dy_200.Integral(0,21))
dy_0.Scale(1./dy_0.Integral(0,21))
dx_200.Scale(1./dx_200.Integral(0,21))
dx_0.Scale(1./dx_0.Integral(0,21))

dz_200.SetMarkerStyle(20)
dz_200.SetMarkerColor(ROOT.kRed)
dz_200.SetTitle("")
dz_200.SetStats(0)
dz_200.GetYaxis().SetRangeUser(0,1)

dz_0.SetMarkerStyle(24)
dz_0.SetMarkerColor(ROOT.kRed)

dy_200.SetMarkerStyle(20)
dy_200.SetMarkerColor(ROOT.kRed)
dy_200.SetTitle("")
dy_200.SetStats(0)
dy_200.GetYaxis().SetRangeUser(0,1)

dy_0.SetMarkerStyle(24)
dy_0.SetMarkerColor(ROOT.kRed)

dx_200.SetMarkerStyle(20)
dx_200.SetMarkerColor(ROOT.kRed)
dx_200.SetTitle("")
dx_200.SetStats(0)
dx_200.GetYaxis().SetRangeUser(0,1)

dx_0.SetMarkerStyle(24)
dx_0.SetMarkerColor(ROOT.kRed)


plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
dz_200.GetXaxis().SetTitle("#Delta z (vtx-gen vtx) (cm)")
dz_200.GetYaxis().SetTitle("a.u.")
legend.AddEntry(dz_200,'200 PU','P')
legend.AddEntry(dz_0,'0 PU','P')
dz_200.Draw("EP0")
dz_0.Draw("EP0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV", 3)
canv.SetLogy()

canv.SaveAs('vtxdz.pdf')
canv.Print('vtxdz.png')

canv2=ROOT.TCanvas()
pads2 = plot.OnePad()
for padx in pads2:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

dy_200.GetXaxis().SetTitle("#Delta y (vtx-gen vtx) (cm)")
dy_200.GetYaxis().SetTitle("a.u.")
dy_200.Draw("EP0")
dy_0.Draw("EP0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV", 3)

canv2.SaveAs('vtxdy.pdf')
canv2.Print('vtxdy.png')

canv3=ROOT.TCanvas()
pads3 = plot.OnePad()
for padx in pads3:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

dx_200.GetXaxis().SetTitle("#Delta x (vtx-gen vtx) (cm)")
dx_200.GetYaxis().SetTitle("a.u.")
dx_200.Draw("EP0")
dx_0.Draw("EP0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV", 3)

canv2.SaveAs('vtxdx.pdf')
canv2.Print('vtxdx.png')

