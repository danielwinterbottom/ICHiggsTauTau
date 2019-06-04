import sys
import os
import glob
import UserCode.ICHiggsTauTau.plotting as plotting

import ROOT
ROOT.gROOT.SetBatch(True)

plotting.ModTDRStyle()

file_data = ROOT.TFile.Open(os.path.join("./input/pileup/", "Data_Pileup_2018_69p2mb_Cert_314472-325175_100bins.root"), "READ")
hist_data = file_data.Get("pileup")
hist_data.SetLineColor(ROOT.kRed+2)

# +- 4.6% of nominal 69200 mb
file_data_low = ROOT.TFile.Open(os.path.join("./input/pileup/", "Data_Pileup_2018_69p2mb_Cert_314472-325175_100bins_low.root"), "READ")
hist_data_low = file_data_low.Get("pileup")
hist_data_low.SetLineColor(ROOT.kRed-7)

file_data_high = ROOT.TFile.Open(os.path.join("./input/pileup/", "Data_Pileup_2018_69p2mb_Cert_314472-325175_100bins_upp.root"), "READ")
hist_data_high = file_data_high.Get("pileup")
hist_data_high.SetLineColor(ROOT.kRed-7)

file_mc = ROOT.TFile.Open(os.path.join("./input/pileup/2018/", "pileup_2018_DYJetsToLL-LO.root"), "READ")
hist_mc = file_mc.Get("pileup")
hist_mc.SetLineColor(ROOT.kBlue+2)

hists = [hist_data, hist_data_low, hist_data_high, hist_mc]

# normalise bins
for h in hists:
    print h
    scale = h.Integral(-1,-1)
    print scale
    for i in range(1,h.GetNbinsX()+1):
        content = h.GetBinContent(i) 
        error = h.GetBinError(i)
        h.SetBinContent(i,content/scale)
        h.SetBinError(i,error/scale)

stack = ROOT.THStack("stack","")
for hist in hists:
    stack.Add(hist)

# make ratio
h_ratio = hist_data.Clone()
h_ratio.Divide(hist_mc)
h_ratio.SetLineColor(ROOT.kRed+2)

h_ratio_low = hist_data_low.Clone()
h_ratio_low.Divide(hist_mc)
h_ratio_low.SetLineColor(ROOT.kRed-7)

h_ratio_high = hist_data_high.Clone()
h_ratio_high.Divide(hist_mc)
h_ratio_high.SetLineColor(ROOT.kRed-7)

c1 = ROOT.TCanvas()
c1.cd()
pads=plotting.TwoPadSplit(0.29,0.01,0.01)

pads[0].cd()

pads[0].SetBottomMargin(0.3)
stack.Draw("hist,nostack")
stack.GetYaxis().SetTitle("a.u.")
stack.GetXaxis().SetLabelOffset(999)
stack.GetXaxis().SetLabelSize(0)

c1.cd()
pads[1].cd()
pads[1].SetTopMargin(0.7)
h_ratio.SetMinimum(0.0)
h_ratio.SetMaximum(1.99)
h_ratio.Draw("hist")
h_ratio.GetYaxis().SetNdivisions(4)
h_ratio.GetYaxis().SetTitle("Ratio")
h_ratio.GetXaxis().SetTitle("n_TrueInt")

h_ratio_low.SetMinimum(0.0)
h_ratio_low.SetMaximum(1.99)
h_ratio_low.Draw("hist,same")
h_ratio_low.GetYaxis().SetNdivisions(4)
h_ratio_low.GetYaxis().SetTitle("Ratio")

h_ratio_high.SetMinimum(0.0)
h_ratio_high.SetMaximum(1.99)
h_ratio_high.Draw("hist,same")
h_ratio_high.GetYaxis().SetNdivisions(4)
h_ratio_high.GetYaxis().SetTitle("Ratio")

line = ROOT.TLine(0,1,99,1)
line.SetLineColor(ROOT.kBlack)
line.SetLineStyle(2)
line.Draw()

# legend

c1.cd()
leg = ROOT.TLegend(0.65,0.7,0.9,0.85)
leg.SetBorderSize(0)
leg.AddEntry(hist_data,"Data (Nominal)","l")
leg.AddEntry(hist_data_low, "Data (#pm 4.6%)","l")
leg.AddEntry(hist_mc, "MC","l")
leg.Draw()


# label
c1.cd()
txt = ROOT.TLatex()
txt.SetTextFont(42)
txt.SetTextSize(0.04)
txt.SetTextAlign(11)
txt.DrawLatexNDC(c1.GetLeftMargin(),1.02-c1.GetTopMargin(),"#bf{CMS} #it{Preliminary}")
txt2 = ROOT.TLatex()
txt2.SetTextFont(42)
txt2.SetTextSize(0.03)
txt2.SetTextAlign(11)
txt2.DrawLatexNDC(0.88-c1.GetLeftMargin(),1.02-c1.GetTopMargin(),"58.8 fb^{-1} (13 TeV)")

c1.SaveAs("pileup_2018_fromDYLO.pdf");

