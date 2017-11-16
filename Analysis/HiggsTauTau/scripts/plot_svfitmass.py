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

#PU = ('200',)
PU = ('0',)
#CHN = ('mt', '#mu_{}#tau_{h}')
CHN = ('et', 'e_{}#tau_{h}')
TAG = 'V2'


infileHTT = "output/%s_VBFHToTauTau_M-125-pu%s_%s_0.root" % (TAG, PU[0], CHN[0])
infileDY = "output/%s_DYJetsToTauTau_M-50-pu%s_%s_0.root" % (TAG, PU[0], CHN[0])

htt_file=ROOT.TFile.Open(infileHTT)
dy_file=ROOT.TFile.Open(infileDY)
htt_tree=htt_file.Get("ntuple")
dy_tree=dy_file.Get("ntuple")

htt_msv = ROOT.TH1D("htt_msv","htt_msv",28,40,180)
dy_msv = ROOT.TH1D("dy_msv","dy_msv",28,40,180)

htt_mvis = ROOT.TH1D("htt_mvis","htt_mvis",28,0,140)
dy_mvis = ROOT.TH1D("dy_mvis","dy_mvis",28,0,140)


htt_tree.Draw("m_sv>>htt_msv")
dy_tree.Draw("m_sv>>dy_msv")
htt_tree.Draw("m_vis>>htt_mvis")
dy_tree.Draw("m_vis>>dy_mvis")


htt_msv.SetLineColor(ROOT.kRed)
dy_msv.SetFillColor(ROOT.TColor.GetColor(248,206,104))
dy_msv.SetFillStyle(1001)

htt_mvis.SetLineColor(ROOT.kRed)
dy_mvis.SetFillColor(ROOT.TColor.GetColor(248,206,104))
dy_msv.SetFillStyle(1001)

htt_mvis.Scale(1./htt_mvis.Integral())
dy_mvis.Scale(1./dy_mvis.Integral())

print 'ZTT nevents/res: %i/%g' % (dy_mvis.GetEntries(), dy_mvis.GetRMS() / dy_mvis.GetMean())
print 'Sig nevents/res: %i/%g' % (htt_mvis.GetEntries(), htt_mvis.GetRMS() / htt_mvis.GetMean())





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

legend_mvis = plot.PositionedLegend(0.23, 0.20, 3, 0.03)
plot.Set(legend_mvis, NColumns=1)
legend_mvis.SetHeader('%s channel' % CHN[1])
dy_mvis.GetXaxis().SetTitle("Visible m_{#tau#tau} (GeV)")
dy_mvis.GetYaxis().SetRangeUser(0,0.22)

formatHisto(dy_mvis)
formatHisto(htt_mvis)

#dy_mvis.GetYaxis().SetTitleOffset(1.4)
dy_mvis.GetYaxis().SetTitle("a.u.")

dy_mvis.Draw("HIST")
htt_mvis.Draw("SAMEHIST")
legend_mvis.AddEntry(dy_mvis,'Z#rightarrow#tau#tau','F')
legend_mvis.AddEntry(htt_mvis,'VBF H#rightarrow#tau#tau','L')
legend_mvis.Draw()

drawCMS()
drawEnPu(PU[0])
#plot.DrawCMSLogo(pads_mvis[0], 'CMS ', 'Simulation', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawCMSLogo(pads[0], 'CMS Phase-2', 'Simulation Preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawTitle(pads_mvis[0], "#mu_{}#tau_{h} channel", 1)
#plot.DrawTitle(pads_mvis[0], "13 TeV, Run 2", 3)

canv_mvis.SaveAs('mvis_comparison_pu%s_%s_%s.pdf' % (PU[0], CHN[0], TAG))
canv_mvis.Print('mvis_comparison_pu%s_%s_%s.png' % (PU[0], CHN[0], TAG))
canv_mvis.SaveAs('mvis_comparison_pu%s_%s_%s.root' % (PU[0], CHN[0], TAG))
canv_mvis.SaveAs('mvis_comparison_pu%s_%s_%s.C' % (PU[0], CHN[0], TAG))

