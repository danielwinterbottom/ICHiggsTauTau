#!/usr/bin/env python
import ROOT
import CombineHarvester.CombineTools.plotting as plot
from UserCode.ICHiggsTauTau.uncertainties import ufloat
import UserCode.ICHiggsTauTau.MultiDraw
import argparse


def Rate(hist):
    err = ROOT.Double()
    integral = hist.IntegralAndError(0, hist.GetNbinsX() + 1, err)
    return ufloat(integral, err)

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

parser = argparse.ArgumentParser()

parser.add_argument('--var', default='mll(20,0,500)')
parser.add_argument('--selection', default='1')
parser.add_argument('--x-title', default='m_{ll}^{gen} (GeV)')
parser.add_argument('--ratio-range', default='0.69,1.31')
parser.add_argument('--title-right', default='')
parser.add_argument('--title-left', default='')
parser.add_argument('--output', default='mt_dist')
parser.add_argument('--norm', action='store_true')
parser.add_argument('--type', default=0, type=int)



args = parser.parse_args()

path = 'results'

files = [
    'DYJetsToLL_M-50-LO-ext',
    'DY1JetsToLL_M-50-LO',
    'DY2JetsToLL_M-50-LO',
    'DY3JetsToLL_M-50-LO',
    'DY4JetsToLL_M-50-LO',
    'DYJetsToLL_M-150-LO'
]

# Uncomment to just print the sample yields
# for f in files:
#     fin = ROOT.TFile('%s/%s.root' % (path, f))
#     t = fin.Get('genweights')
#     print '%s: %i' % (f, t.GetEntries())


ROOT.TH1.SetDefaultSumw2(True) # Why do I need to do this here?

if args.type == 0:
    ch_stitched = ROOT.TChain('genweights')
    for f in files:
        ch_stitched.Add('%s/%s.root' % (path, f))

    ch_inc = ROOT.TChain('genweights')
    ch_inc.Add('%s/%s.root' % (path, 'DYJetsToLL_M-50-LO-ext'))

    res_stitched = ch_stitched.MultiDraw([(args.var, 'wt*(%s)' % args.selection)])
    res_inc = ch_inc.MultiDraw([(args.var, '1*(%s)' % args.selection)])
elif args.type == 1:
    res = []
    for f in files:
        fin = ROOT.TFile('%s/%s.root' % (path, f))
        t = fin.Get('genweights')
        res.extend(t.MultiDraw([(args.var, '1*(%s)' % args.selection)]))

canv = ROOT.TCanvas(args.output, args.output)
pads = plot.TwoPadSplit(0.30, 0.01, 0.01)

legend = ROOT.TLegend(0.50, 0.75, 0.93, 0.93, '', 'NBNDC')

if args.type == 0:
    plot.Set(res_inc[0], LineColor=ROOT.kBlack, LineWidth=3)
    plot.Set(res_stitched[0], LineColor=ROOT.kRed, LineWidth=3)
    legend.AddEntry(res_inc[0], 'Inclusive', 'L')
    legend.AddEntry(res_stitched[0], 'Stitched', 'L')
    res_inc[0].Draw('SAME')
    res_stitched[0].Draw('SAME')
elif args.type == 1:
    cols = [28, 2, 4, 6, 8, 9, 30, 40, 46, 48, 1]
    for i, f in enumerate(files):
        plot.Set(res[i], LineColor=cols.pop(), LineWidth=3)
        if args.norm and res[i].Integral(0, res[i].GetNbinsX() + 1) > 0.:
            res[i].Scale(1 / res[i].Integral(0, res[i].GetNbinsX() + 1))
        legend.AddEntry(res[i], f, 'L')
        res[i].Draw('SAME')

plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.40)
legend.Draw()

axis = plot.GetAxisHist(pads[0])
plot.Set(axis.GetXaxis(), Title=args.x_title)
plot.Set(axis.GetYaxis(), Title='Events')

rmin = float(args.ratio_range.split(',')[0])
rmax = float(args.ratio_range.split(',')[1])
pads[1].cd()
pads[1].SetGrid(0, 1)
if args.type == 0:
    ratio_hist = res_stitched[0].Clone()
    ratio_hist.Divide(res_inc[0])
    ratio_hist.Draw('SAME')
    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Stitched/Inc', True, rmin, rmax)
elif args.type == 1:
    ratios = []
    for i, f in enumerate(files):
        if i == 0:
            continue
        ratios.append(res[i].Clone())
        ratios[-1].Divide(res[0])
        ratios[-1].Draw('SAME')
    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Ratio/Inc', True, rmin, rmax)

plot.DrawTitle(pads[0], args.title_left, 1)
plot.DrawTitle(pads[0], args.title_right, 3)

canv.Print('.pdf')
canv.Print('.png')
