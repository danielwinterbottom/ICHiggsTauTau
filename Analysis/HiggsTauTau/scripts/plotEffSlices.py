#!/usr/bin/env python
import ROOT
import CombineHarvester.CombineTools.plotting as plot
import os
import argparse

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(0)
plot.ModTDRStyle()


parser = argparse.ArgumentParser()

parser.add_argument(
    'input', nargs='+', help="""Input files""")
parser.add_argument(
    '--output', '-o', default='efficiency', help="""Name of the output
    plot without file extension""")
parser.add_argument('--title', default='Muon ID Efficiency')
parser.add_argument('--y-range', default='0,1')
parser.add_argument('--ratio-y-range', default='0.92,1.08')
parser.add_argument('--binned-in', default='#eta')
parser.add_argument('--x-title', default='p_{T} (GeV)')
parser.add_argument('--ratio-to', default=None, type=int)
parser.add_argument('--plot-dir', '-p', default='./')
parser.add_argument('--label-pos', default=1)
args = parser.parse_args()

if args.plot_dir != '':
    os.system('mkdir -p %s' % args.plot_dir)

hists = []

# file = ROOT.TFile('%s.root' % target)

# Process each input argument
for src in args.input:
    splitsrc = src.split(':')
    file = ROOT.TFile(splitsrc[0])
    hists.append(file.Get(splitsrc[1]).Clone())
    file.Close()

print hists

hist = hists[0]

latex = ROOT.TLatex()
latex.SetNDC()

for i in xrange(1, hist.GetNbinsY()+1):
    bin_label = '%s: [%g,%g]' % (args.binned_in, hist.GetYaxis().GetBinLowEdge(i), hist.GetYaxis().GetBinUpEdge(i))
    canv = ROOT.TCanvas('%s_%i' % (args.output, i), args.output)

    if args.ratio_to is not None:
        pads = plot.TwoPadSplit(0.30, 0.01, 0.01)
    else:
        pads = plot.OnePad()
    slices = []

    if args.label_pos == 1:
        text = ROOT.TPaveText(0.55, 0.37, 0.9, 0.50, 'NDC')
        legend = ROOT.TLegend(0.18, 0.37, 0.5, 0.50, '', 'NDC')
    else:
        text = ROOT.TPaveText(0.55, 0.67, 0.9, 0.80, 'NDC')
        legend = ROOT.TLegend(0.18, 0.67, 0.5, 0.80, '', 'NDC')

    for j, src in enumerate(args.input):
        splitsrc = src.split(':')
        slice = hists[j].ProjectionX('%s_projx_%i' % (hists[j].GetName(), j), i, i)
        if len(splitsrc) >= 3:
            settings = {x.split('=')[0]: eval(x.split('=')[1]) for x in splitsrc[2].split(',')}
            plot.Set(slice, **settings)
        slice.Draw('SAME')
        legend.AddEntry(slice)
        slices.append(slice)
    axis = plot.GetAxisHist(pads[0])
    axis.GetYaxis().SetTitle('Efficiency')
    axis.GetXaxis().SetTitle(args.x_title)
    axis.SetMinimum(float(args.y_range.split(',')[0]))
    axis.SetMaximum(float(args.y_range.split(',')[1]))
    pads[0].SetGrid(1, 1)
    latex.SetTextSize(0.04)

    text.AddText(args.title)
    text.AddText(bin_label)
    text.SetTextAlign(13)
    text.SetBorderSize(0)
    text.Draw()
    legend.Draw()

    plot.DrawCMSLogo(pads[0], 'CMS', 'Internal', 0, 0.16, 0.035, 1.2, cmsTextSize=0.9)
    plot.DrawTitle(pads[1], '12.9 fb^{-1} (13 TeV)', 3)

    if args.ratio_to is not None:
        pads[1].cd()
        ratios = []
        for slice in slices:
            ratios.append(slice.Clone())
            ratios[-1].Divide(slices[args.ratio_to])
        ratios[0].Draw('AXIS')
        plot.SetupTwoPadSplitAsRatio(pads,
                                     plot.GetAxisHist(pads[0]),
                                     plot.GetAxisHist(pads[1]),
                                     'Data/MC', True,
                                     float(args.ratio_y_range.split(',')[0]),
                                     float(args.ratio_y_range.split(',')[1]))
        for j, ratio in enumerate(ratios):
            if j == args.ratio_to:
                continue
            ratio.Draw('SAME')
        pads[1].SetGrid(1, 1)
        pads[1].RedrawAxis('g')

    outname = '%s.%s.%g_%g' % (args.output, hist.GetYaxis().GetTitle(), hist.GetYaxis().GetBinLowEdge(i), hist.GetYaxis().GetBinUpEdge(i))
    outname = outname.replace('(', '_')
    outname = outname.replace(')', '_')

    canv.Print('%s/%s.png' % (args.plot_dir, outname))
    canv.Print('%s/%s.pdf' % (args.plot_dir, outname))