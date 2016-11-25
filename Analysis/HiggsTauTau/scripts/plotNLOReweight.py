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
parser.add_argument('--tree', default='ntuple')
parser.add_argument('--selection', default='1')
parser.add_argument('--x-title', default='m_{ll}^{gen} (GeV)')
parser.add_argument('--ratio-range', default='0.69,1.31')
parser.add_argument('--title-right', default='')
parser.add_argument('--title-left', default='')
parser.add_argument('--output', default='mt_dist')
parser.add_argument('--file', default='')
# parser.add_argument('--norm', action='store_true')
# parser.add_argument('--type', default=0, type=int)



args = parser.parse_args()


files = [args.file]
    # 'output/Mar05-postpreapp/SUSYGluGluToHToTauTau_M-500_mt_0']


ROOT.TH1.SetDefaultSumw2(True) # Why do I need to do this here?


chain = ROOT.TChain(args.tree)
for f in files:
    chain.Add('%s.root' % f)


res = chain.MultiDraw([(args.var, '(%s)' % args.selection), (args.var, 'wt_nlo_pt*(%s)' % args.selection)])


canv = ROOT.TCanvas(args.output, args.output)
pads = plot.TwoPadSplit(0.30, 0.01, 0.01)

legend = ROOT.TLegend(0.50, 0.75, 0.93, 0.93, '', 'NBNDC')

plot.Set(res[0], LineColor=ROOT.kBlack, LineWidth=3)
plot.Set(res[1], LineColor=ROOT.kRed, LineWidth=3)
legend.AddEntry(res[0], 'Unweighted', 'L')
legend.AddEntry(res[1], 'Weighted', 'L')
res[0].Draw('HISTESAME')
res[1].Draw('HISTESAME')

plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.40)

delta_acc = Rate(res[1]) / Rate(res[0])
legend.SetHeader('Rel. Acc.: %.3f #pm %.3f' % (delta_acc.n, delta_acc.s))
legend.Draw()

axis = plot.GetAxisHist(pads[0])
plot.Set(axis.GetXaxis(), Title=args.x_title)
plot.Set(axis.GetYaxis(), Title='a.u.')

rmin = float(args.ratio_range.split(',')[0])
rmax = float(args.ratio_range.split(',')[1])
pads[1].cd()
pads[1].SetGrid(0, 1)

ratio_hist = res[1].Clone()
ratio_hist.Divide(res[0])
ratio_hist.Draw('HISTESAME')
plot.SetupTwoPadSplitAsRatio(
    pads, plot.GetAxisHist(
        pads[0]), plot.GetAxisHist(pads[1]), 'Ratio', True, rmin, rmax)

plot.DrawTitle(pads[0], args.title_left, 1)
plot.DrawTitle(pads[0], args.title_right, 3)

res[0].Print()

canv.Print('.pdf')
canv.Print('.png')
