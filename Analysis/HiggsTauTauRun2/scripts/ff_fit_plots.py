import ROOT
import re
import os
import time
import json
from array import array
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--year',help= 'Year to draw fits', default='2017')
parser.add_argument('--channel',help= 'Channel to draw fits', default='et')
parser.add_argument('--n_prebjets_bin',help= 'n_pre b-jets bin to draw fits', default='1jet')
parser.add_argument('--jet_pt_bin',help= 'Jet-pt bin to draw fits', default='med')
parser.add_argument('--bkg',help= 'Background to draw fits', default='ttbar_mc')
args = parser.parse_args()

year = args.year
channel = args.channel
n_prejets_bin = args.n_prebjets_bin
jet_pt_bin = args.jet_pt_bin
bkg = args.bkg

file = 'fakefactor_fits/fakefactor_fits_%(channel)s_medium_%(year)s.root' % vars()

def DrawTitle(pad, text, align, textOffset=0.2,textSize=0.6,x=0,y=0):
    pad_backup = ROOT.gPad
    pad.cd()
    t = pad.GetTopMargin()
    l = pad.GetLeftMargin()
    r = pad.GetRightMargin()

    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if pad_ratio < 1.:
        pad_ratio = 1.

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)
    latex.SetTextFont(42)
    latex.SetTextSize(textSize * t * pad_ratio)

    y_off = 1 - t + textOffset * t
    if align == 1:
        latex.SetTextAlign(11)
        latex.DrawLatex(l, y_off, text)
    elif align == 2:
        latex.SetTextAlign(21)
        latex.DrawLatex(l + (1 - l - r) * 0.5, y_off, text)
    elif align == 3:
        latex.SetTextAlign(31)
        latex.DrawLatex(1 - r, y_off, text)
    else:
        latex.DrawLatex(x,y,text)
    pad_backup.cd()

def DrawCMSLogo(pad, cmsText, extraText, iPosX, relPosX, relPosY, relExtraDY, extraText2='', cmsTextSize=0.8):
    """Blah
    
    Args:
        pad (TYPE): Description
        cmsText (TYPE): Description
        extraText (TYPE): Description
        iPosX (TYPE): Description
        relPosX (TYPE): Description
        relPosY (TYPE): Description
        relExtraDY (TYPE): Description
        extraText2 (str): Description
        cmsTextSize (float): Description
    
    Returns:
        TYPE: Description
    """
    pad.cd()
    cmsTextFont = 62  # default is helvetic-bold

    writeExtraText = len(extraText) > 0
    writeExtraText2 = len(extraText2) > 0
    extraTextFont = 52

    # text sizes and text offsets with respect to the top frame
    # in unit of the top margin size
    lumiTextOffset = 0.2
    # cmsTextSize = 0.8
    # float cmsTextOffset    = 0.1;  // only used in outOfFrame version

    # ratio of 'CMS' and extra text size
    extraOverCmsTextSize = 0.76

    outOfFrame = False
    if iPosX / 10 == 0:
        outOfFrame = True

    alignY_ = 3
    alignX_ = 2
    if (iPosX / 10 == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignY_ = 1
    if (iPosX / 10 == 1):
        alignX_ = 1
    if (iPosX / 10 == 2):
        alignX_ = 2
    if (iPosX / 10 == 3):
        alignX_ = 3
    # if (iPosX == 0): relPosX = 0.14
    align_ = 10 * alignX_ + alignY_

    l = pad.GetLeftMargin()
    t = pad.GetTopMargin()
    r = pad.GetRightMargin()
    b = pad.GetBottomMargin()

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)

    extraTextSize = extraOverCmsTextSize * cmsTextSize
    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if (pad_ratio < 1.):
        pad_ratio = 1.
    if outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextAlign(11)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText)

    posX_ = 0
    if iPosX % 10 <= 1:
        posX_ = l + relPosX * (1 - l - r)
    elif (iPosX % 10 == 2):
        posX_ = l + 0.5 * (1 - l - r)
    elif (iPosX % 10 == 3):
        posX_ = 1 - r - relPosX * (1 - l - r)

    posY_ = 1 - t - relPosY * (1 - t - b)
    if not outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, cmsText)
        if writeExtraText:
            latex.SetTextFont(extraTextFont)
            latex.SetTextAlign(align_)
            latex.SetTextSize(extraTextSize * t * pad_ratio)
            latex.DrawLatex(
                posX_, posY_ - relExtraDY * cmsTextSize * t, extraText)
            if writeExtraText2:
                latex.DrawLatex(
                    posX_, posY_ - 1.8 * relExtraDY * cmsTextSize * t, extraText2)
    elif writeExtraText:
        if iPosX == 0:
            posX_ = l + relPosX * (1 - l - r)
            posY_ = 1 - t + lumiTextOffset * t
        latex.SetTextFont(extraTextFont)
        latex.SetTextSize(extraTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, extraText)

rf = ROOT.TFile.Open(file,"read")
h = rf.Get("jet_pt_%(jet_pt_bin)s_%(n_prejets_bin)s_pt_2_ff_%(bkg)s" % vars())
f = rf.Get("jet_pt_%(jet_pt_bin)s_%(n_prejets_bin)s_pt_2_ff_%(bkg)s_fit" % vars())
u = rf.Get("jet_pt_%(jet_pt_bin)s_%(n_prejets_bin)s_pt_2_ff_%(bkg)s_uncert" % vars())

if channel == "mt":
  channel_string = "#mu#tau_{h}"
elif channel == "et":
  channel_string = "e#tau_{h}"

if year == "2016":
  lumi_string = "35.9 fb^{-1}"
elif year == "2017":
  lumi_string = "41.5 fb^{-1}"
elif year == "2018":
  lumi_string = "59.7 fb^{-1}"

if bkg == "qcd":
  bkg_string = "QCD"
elif bkg == "wjets":
  bkg_string = "W + Jets"
elif bkg == "ttbar_mc":
  bkg_string = "t#bar{t}"
elif bkg == 'wjets_mc':
  bkg_string = 'W + Jets MC'
elif bkg == 'qcd_aiso':
  bkg_string = 'QCD Anti-Isolated'

if n_prejets_bin == '0jet':
  n_prejets_string = 'n_{pre b-jets} = 0'
elif n_prejets_bin == '1jet':
  n_prejets_string = 'n_{pre b-jets} > 0'
elif n_prejets_bin == 'inclusive':
  n_prejets_string = 'Inclusive'

if jet_pt_bin == 'low':
  jet_pt_string = 'Jet-p_{T}/Tau-p_{T} < 1.25'
elif jet_pt_bin == 'med':
  jet_pt_string = '1.25 #leq Jet-p_{T}/Tau-p_{T} < 1.5'
elif jet_pt_bin == 'high':
  jet_pt_string = 'Jet-p_{T}/Tau-p_{T} #geq 1.5'

c = ROOT.TCanvas('c','c',700,700)
h.SetStats(0)
c.SetLeftMargin(0.15)
u.Draw('e3')
c.SetLogx()
u.GetXaxis().SetMoreLogLabels()
u.GetXaxis().SetNoExponent()
h.Draw('same')
f.Draw('same')
h.SetLineColor(1)
u.SetLineColor(46)
u.SetFillColorAlpha(46, 0.35)
u.GetXaxis().SetTitle('Tau-p_{T} (GeV)')
u.GetXaxis().SetTitleOffset(1.2)
u.GetYaxis().SetTitleOffset(1.6)
u.GetYaxis().SetTitle('F_{F}')
u.SetMaximum(1.7*max(u.GetMaximum(),h.GetMaximum()))
u.SetMinimum(0)
h.SetMarkerStyle(8)

c.Update()

l = ROOT.TLegend(0.62,0.75,0.87,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.03)
l.AddEntry(h,'Observed','lep')
l.AddEntry(f,'Fit','l')
l.AddEntry(u,'Fit Uncertainty','f')
l.Draw()

DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
DrawTitle(c, '%(channel_string)s %(bkg_string)s' % vars(), 1, textSize=0.3)
DrawTitle(c, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.3)

DrawTitle(c, jet_pt_string, 4, textSize=0.25,x=0.20,y=0.72)
DrawTitle(c, n_prejets_string, 4, textSize=0.25,x=0.20,y=0.67)

c.Print('ff_fit_plots/%(year)s/%(channel)s/%(bkg)s/ff_fit_%(jet_pt_bin)s_%(n_prejets_bin)s_%(bkg)s_%(channel)s_%(year)s.pdf' % vars())

