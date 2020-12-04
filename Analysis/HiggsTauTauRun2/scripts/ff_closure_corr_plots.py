import ROOT
import re
import os
import time
import json
from array import array
import argparse

# python scripts/ff_closure_corr_plots.py --year=2016 --channel=tt --bin=os_closure_nbjet0 --bkg=qcd --xlabel='#Delta R' --low_bounded_val=0.5 --high_bounded_val=5

parser = argparse.ArgumentParser()
parser.add_argument('--year',help= 'Year for correction', default='2017')
parser.add_argument('--channel',help= 'Channel for correction', default='tt')
parser.add_argument('--bin',help= 'Closure correction name', default='os_closure_nbjet0')
parser.add_argument('--bkg',help= 'Background to draw fits', default='qcd')
parser.add_argument('--xlabel',help= 'X axis label', default='p_{T}^{#tau_{h}} (GeV)')
parser.add_argument('--low_bounded_val',help= '', default='0')
parser.add_argument('--high_bounded_val',help= '', default='10000.0')
parser.add_argument('--logx',help= '', default=False)
args = parser.parse_args()

year = args.year
channel = args.channel
bin = args.bin
bkg = args.bkg
xlabel = args.xlabel
low_bounded_val = float(args.low_bounded_val)
high_bounded_val = float(args.high_bounded_val)
logx = args.logx

file = 'fakefactor_fits_%(channel)s_medium_%(year)s.root' % vars()


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

print "%(bin)s_%(bkg)s" % vars()
h = rf.Get("%(bin)s_%(bkg)s" % vars())
f = rf.Get("%(bin)s_%(bkg)s_fit" % vars())
u = rf.Get("%(bin)s_%(bkg)s_uncert" % vars())


if channel == "mt":
  channel_string = "#tau_{#mu}#tau_{h}"
elif channel == "et":
  channel_string = "#tau_{e}#tau_{h}"
elif channel == "tt":
  channel_string = "#tau_{h}#tau_{h}"

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

#high_bounded_val = 5.0
high_bin = u.GetBinContent(u.FindBin(high_bounded_val))
high_bin_err = u.GetBinError(u.FindBin(high_bounded_val))

for i in range(1,u.GetNbinsX()+1):
  if u.GetBinLowEdge(i) >= high_bounded_val:
    u.SetBinContent(i,high_bin)
    u.SetBinError(i,high_bin_err)

#low_bounded_val = 0.5
low_bin = u.GetBinContent(u.FindBin(low_bounded_val))
low_bin_err = u.GetBinError(u.FindBin(low_bounded_val))

for i in range(1,u.GetNbinsX()+1):
  if u.GetBinLowEdge(i) <= low_bounded_val:
    u.SetBinContent(i,low_bin)
    u.SetBinError(i,low_bin_err)

f.SetRange(low_bounded_val,high_bounded_val)

f1 = ROOT.TF1('f1',"%(high_bin)s + 0*x" % vars(),high_bounded_val,u.GetBinLowEdge(u.GetNbinsX()+1))
f2 = ROOT.TF1('f1',"%(low_bin)s + 0*x" % vars(),u.GetBinLowEdge(1),low_bounded_val)


c = ROOT.TCanvas('c','c',700,700)
u.SetStats(0)
c.SetLeftMargin(0.15)
u.Draw('e3')
if logx:
  c.SetLogx()
  u.GetXaxis().SetMoreLogLabels()
  u.GetXaxis().SetNoExponent()
h.Draw('same')
h.GetFunction("f1").Delete()
f.Print("all")
#f.SetRange(0,u.GetBinLowEdge(u.GetNbinsX()+1))
f.Draw('same')
f1.Draw('same')
f2.Draw('same')
f.SetLineColor(4)
f1.SetLineColor(4)
f2.SetLineColor(4)
h.SetLineColor(1)
u.SetLineColor(38)
u.SetFillColorAlpha(38, 0.35)
u.GetXaxis().SetTitle(xlabel)
u.GetXaxis().SetTitleOffset(1.2)
u.GetYaxis().SetTitleOffset(1.6)
u.GetYaxis().SetTitle('Closure Correction')
u.SetMaximum(1.5)
u.SetMinimum(0.6)
h.SetMarkerStyle(8)

# Draw dashed line at y = 1
line = ROOT.TLine(u.GetBinLowEdge(1),1,u.GetBinLowEdge(u.GetNbinsX()+1),1)
line.SetLineWidth(1)
line.SetLineStyle(2)
line.SetLineColor(ROOT.kBlack)
line.Draw()

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

#DrawTitle(c, jet_pt_string, 4, textSize=0.3,x=0.20,y=0.72)
#DrawTitle(c, n_prejets_string, 4, textSize=0.3,x=0.20,y=0.67)

c.Print('ff_closure_%(bin)s_%(bkg)s_%(channel)s_%(year)s.pdf' % vars())

