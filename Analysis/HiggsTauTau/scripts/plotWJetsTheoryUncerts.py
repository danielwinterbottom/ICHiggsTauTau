#!/usr/bin/env python
import ROOT
import CombineHarvester.CombineTools.plotting as plot
from UserCode.ICHiggsTauTau.uncertainties import ufloat
import UserCode.ICHiggsTauTau.MultiDraw
import argparse

from time import time

"""
Examples:

python scripts/plotWJetsTheoryUncerts.py --sample WJetsToLNuNLO \\ 
    --title-right 'WJetsToLNu (NLO)' --title-left 'QCD scale' \\
    --text '#splitline{W#rightarrow#mu#nu + #geq 1 jet}{selection}' \\
    --output mt_dist_WJetsToLNuNLO_1jet --selection 'n_jets>=1'

python scripts/plotWJetsTheoryUncerts.py --sample WJetsToLNuNLO --mode 2 \\
    --title-right 'WJetsToLNu (NLO)' --title-left '#alpha_{s}' \\
    --text '#splitline{W#rightarrow#mu#nu + 2 jets}{selection}' \\
    --output mt_as_WJetsToLNuNLO_2jet --selection 'n_jets>=2'
"""


def Rate(hist):
    err = ROOT.Double()
    integral = hist.IntegralAndError(0, hist.GetNbinsX() + 1, err)
    return ufloat(integral, err)

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()


parser = argparse.ArgumentParser()

parser.add_argument('--sample', default=None)
parser.add_argument(
    '--mode', type=int, default=0, help='0=scale, 1=pdf, 2=alpha_s')
parser.add_argument('--selection', default='1')
parser.add_argument('--title-right', default='')
parser.add_argument('--title-left', default='')
parser.add_argument('--text', default='')
parser.add_argument('--output', default='mt_dist')


args = parser.parse_args()

target = args.sample
if args.mode == 1:
    wt_name = '(%s)*pdf_wt' % args.selection
    points = range(100)
    do_mt_plot = False
elif args.mode == 0:
    wt_name = '(%s)*scale_wt' % args.selection
    points = [0, 1, 2, 3, 4, 6, 8]
    do_mt_plot = True
elif args.mode == 2:
    wt_name = '(%s)*pdf_wt' % args.selection
    points = [0, 100, 101]
    do_mt_plot = False


scale_legends = {
    0: '#mu_{R}=1.0 #mu_{F}=1.0',
    1: '#mu_{R}=1.0 #mu_{F}=2.0',
    2: '#mu_{R}=1.0 #mu_{F}=0.5',
    3: '#mu_{R}=2.0 #mu_{F}=1.0',
    4: '#mu_{R}=2.0 #mu_{F}=2.0',
    5: '#mu_{R}=2.0 #mu_{F}=0.5',
    6: '#mu_{R}=0.5 #mu_{F}=1.0',
    7: '#mu_{R}=0.5 #mu_{F}=2.0',
    8: '#mu_{R}=0.5 #mu_{F}=0.5'
}

file = ROOT.TFile('%s.root' % target)
tree = file.Get('wjetsstudy')

draw_lo = [("0.5(1,0,1)", "(mt_1<30)*wt*%s_%i" % (wt_name, x))
           for x in points]
draw_hi = [("0.5(1,0,1)", "(mt_1>70)*wt*%s_%i" % (wt_name, x))
           for x in points]

start = time()
res_lo = tree.MultiDraw(draw_lo)
res_hi = tree.MultiDraw(draw_hi)
print "Total %.2fs" % (time() - start), " " * 20

ratio_list = []
unc_ratio_list = []
for i, h in enumerate(zip(res_lo, res_hi)):
    ratio_list.append((Rate(h[0]) / Rate(h[1])).nominal_value)
    unc_ratio_list.append((Rate(h[0]) / Rate(h[1])))
    # hratio.Fill((Rate(h[0]) / Rate(h[1])).nominal_value)
    # print ('%i : ' % i) + str(Rate(h[0]) / Rate(h[1]))
    # outfile.WriteTObject(h)

outfile = ROOT.TFile('%s_output.root' % target, 'RECREATE')

min_ratio = min(ratio_list)
max_ratio = max(ratio_list)
hratio = ROOT.TH1F('ratio', 'ratio', 100,
                   min_ratio - 0.1 * (max_ratio - min_ratio),
                   max_ratio + 0.1 * (max_ratio - min_ratio))

for r in ratio_list:
    hratio.Fill(r)

canv = ROOT.TCanvas(args.output+'_dist', args.output+'_dist')
pads = plot.OnePad()
plot.Set(hratio.GetXaxis(), Title='(m_{T} < 30 / m_{T} > 70) ratio')
plot.Set(hratio.GetYaxis(), Title='a.u.')
plot.Set(hratio, LineColor=ROOT.kAzure, FillColor=ROOT.kAzure-9)
hratio.Draw()
plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.30)
plot.DrawTitle(pads[0], args.title_left, 1)
plot.DrawTitle(pads[0], args.title_right, 3)
latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextSize(0.03)
latex.DrawLatex(0.2, 0.85, '#splitline{Mean: %.4f}{Std. Dev.: %.4f}' % (hratio.GetMean(), hratio.GetStdDev()))

canv.Print('.png')
canv.Print('.pdf')

# for i in xrange(1, len(unc_ratio_list)):
#     unc_ratio_list[i] = unc_ratio_list[i] / unc_ratio_list[0]

cols = [28, 2, 4, 6, 8, 9, 30, 40, 46, 48, 1]

if do_mt_plot:
    draw_mt = [("mt_1(30,0,150)", "wt*%s_%i" % (wt_name, x))
               for x in points]
    mt_hists = tree.MultiDraw(draw_mt)

    canv = ROOT.TCanvas(args.output, args.output)
    pads = plot.TwoPadSplit(0.30, 0.01, 0.01)

    legend = ROOT.TLegend(0.50, 0.64, 0.93, 0.93, '', 'NBNDC')
    legend.SetHeader('(m_{T} < 30 / m_{T} > 70) / relative to nominal')

    # pads = plot.TwoPadSplit
    for i, h in enumerate(mt_hists):
        plot.Set(h, LineColor=cols.pop(), LineWidth=3)
        h.Scale(1 / h.Integral(0, h.GetNbinsX() + 1))
        h.Draw('SAMEHIST')
        legend.AddEntry(h, scale_legends[points[i]] + ': %s / %.3f' %
                        (unc_ratio_list[i],
                         (unc_ratio_list[i] / unc_ratio_list[0]).nominal_value), 'L')
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.50)
    legend.Draw()

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.2, 0.85, args.text)

    axis = plot.GetAxisHist(pads[0])
    plot.Set(axis.GetXaxis(), Title='m_{T} (GeV)')
    plot.Set(axis.GetYaxis(), Title='a.u.')

    pads[1].cd()
    pads[1].SetGrid(0, 1)
    ratio_hists = [x.Clone() for x in mt_hists]
    # ratio_hists[0].Draw('')
    for i, h in enumerate(ratio_hists):
        h.Divide(mt_hists[0])
        h.Draw('SAMEHIST')

    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Ratio', True, 0.91, 1.09)

    plot.DrawTitle(pads[0], args.title_left, 1)
    plot.DrawTitle(pads[0], args.title_right, 3)

    canv.Print('.pdf')
    canv.Print('.png')

    for i, h in enumerate(mt_hists):
        outfile.WriteTObject(h)


outfile.WriteTObject(hratio)


outfile.Close()
