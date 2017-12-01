import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)


def formatHisto(hist):
    hist.GetXaxis().SetTitleSize(0.0462874993682)
    hist.GetXaxis().SetTitleOffset(1.4)
    hist.GetXaxis().SetLabelSize(0.0462874993682)
    hist.GetXaxis().SetLabelOffset(0.0100567853078)

    hist.GetYaxis().SetTitleSize(0.0462874993682)
    hist.GetYaxis().SetTitleOffset(1.62249999046)
    hist.GetYaxis().SetLabelSize(0.0462874993682)
    hist.GetYaxis().SetLabelOffset(0.005)

    hist.GetZaxis().SetTitleSize(0.0462874993682)
    hist.GetZaxis().SetTitleOffset(1.14999997616)
    hist.GetZaxis().SetLabelSize(0.0462874993682)

def drawCMS(onTop=False):
    #text='Phase-2 Simulation'
    #textLower = ''
    text='Phase-2'
    textLower = 'Simulation Preliminary'
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextFont(62)
    latex.SetTextSize(0.05175)
    latex.DrawLatex(0.18, 0.85 if not onTop else 0.94, "CMS")
    latex.SetTextSize(0.0414)
    latex.SetTextFont(52)
    latex.DrawLatex(0.26625, 0.85 if not onTop else 0.94, text)
    latex.DrawLatex(0.18, 0.80 if not onTop else 0.94, textLower)
    
def drawEnPu(pileup=None, lumi=None):
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.046)
    latex.SetTextColor(1)
    latex.SetTextFont(42)
    latex.SetTextAlign(31)
    tex = '14 TeV'
    if pileup: tex += ', {0} PU'.format(pileup)
    if lumi: tex = '{0}, '.format(lumi) + tex
    latex.DrawLatex(0.95, 0.94, tex)

PU = ('0',)
CHN = ('et', 'e_{}#tau_{h}')
#CHN = ('mt', '#mu_{}#tau_{h}')
TAG = 'V2'


infileHTT0 = "output/%s_VBFHToTauTau_M-125-run2_%s_0.root" % (TAG, CHN[0])
infileHTT1 = "output/%s_VBFHToTauTau_M-125-pu0_%s_0.root" % (TAG, CHN[0])
infileHTT2 = "output/%s_VBFHToTauTau_M-125-pu200_%s_0.root" % (TAG, CHN[0])

htt0_file=ROOT.TFile.Open(infileHTT0)
htt1_file=ROOT.TFile.Open(infileHTT1)
htt2_file=ROOT.TFile.Open(infileHTT2)
htt0_tree=htt0_file.Get("ntuple")
htt1_tree=htt1_file.Get("ntuple")
htt2_tree=htt2_file.Get("ntuple")

htt0_mvis = ROOT.TH1D("htt0_mvis","htt_mvis",28,0,140)
htt1_mvis = ROOT.TH1D("htt1_mvis","htt_mvis",28,0,140)
htt2_mvis = ROOT.TH1D("htt2_mvis","htt_mvis",28,0,140)

htt0_tree.Draw("m_vis>>htt0_mvis")
htt1_tree.Draw("m_vis>>htt1_mvis")
htt2_tree.Draw("m_vis>>htt2_mvis")


htt0_mvis.SetLineColor(ROOT.kGray+1)
htt0_mvis.SetFillColor(ROOT.kGray+0)
htt0_mvis.SetFillStyle(1001)
htt1_mvis.SetLineColor(ROOT.kBlue)
htt2_mvis.SetLineColor(ROOT.kRed)
htt1_mvis.SetLineWidth(2)
htt2_mvis.SetLineWidth(2)


htt0_mvis.Scale(1./htt0_mvis.Integral())
htt1_mvis.Scale(1./htt1_mvis.Integral())
htt2_mvis.Scale(1./htt2_mvis.Integral())

print 'Sig res0: %g' % (htt0_mvis.GetRMS() / htt0_mvis.GetMean())
print 'Sig res1: %g' % (htt1_mvis.GetRMS() / htt1_mvis.GetMean())
print 'Sig res2: %g' % (htt2_mvis.GetRMS() / htt2_mvis.GetMean())





plot.ModTDRStyle(height=600, width=800)
ROOT.gStyle.SetFrameLineWidth(2)
canv_mvis=ROOT.TCanvas()
pads_mvis = plot.OnePad()


pads_mvis[0].SetTopMargin(0.069)
pads_mvis[0].SetRightMargin(0.046)
pads_mvis[0].SetLeftMargin(0.138)
pads_mvis[0].SetBottomMargin(0.15)

for padx in pads_mvis:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend_mvis = plot.PositionedLegend(0.28, 0.25, 3, 0.03)
plot.Set(legend_mvis, NColumns=1)
legend_mvis.SetHeader('%s channel, VBF H#rightarrow#tau#tau' % CHN[1])
htt0_mvis.GetXaxis().SetTitle("Visible m_{#tau#tau} (GeV)")
htt0_mvis.GetYaxis().SetRangeUser(0,0.22)

formatHisto(htt0_mvis)
formatHisto(htt1_mvis)
formatHisto(htt2_mvis)

#dy_mvis.GetYaxis().SetTitleOffset(1.4)
htt0_mvis.GetYaxis().SetTitle("a.u.")

htt0_mvis.Draw("HIST")
htt1_mvis.Draw("SAMEHIST")
htt2_mvis.Draw("SAMEHIST")
legend_mvis.AddEntry(htt0_mvis,'Run 2, 13 TeV', 'F')
legend_mvis.AddEntry(htt1_mvis,'Phase-2, 14 TeV, 0 PU','L')
legend_mvis.AddEntry(htt2_mvis,'Phase-2, 14 TeV, 200 PU','L')
legend_mvis.Draw()

drawCMS()
drawEnPu(PU[0])
#plot.DrawCMSLogo(pads_mvis[0], 'CMS ', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawTitle(pads_mvis[0], "#mu_{}#tau_{h} channel", 1)
#plot.DrawTitle(pads_mvis[0], "13 TeV, Run 2", 3)

canv_mvis.SaveAs('mvis_comparison_signals_%s_%s.pdf' % (CHN[0], TAG))
canv_mvis.Print('mvis_comparison_signals_%s_%s.png' % (CHN[0], TAG))
canv_mvis.SaveAs('mvis_comparison_signals_%s_%s.root' % (CHN[0], TAG))
canv_mvis.SaveAs('mvis_comparison_signals_%s_%s.C' % (CHN[0], TAG))


