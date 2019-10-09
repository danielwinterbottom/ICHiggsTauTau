#!/usr/bin/env python
import ROOT
import argparse
import shlex
from subprocess import Popen, PIPE
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

fin = ROOT.TFile("input/scale_factors/htt_scalefactors_legacy_2018.root")
w = fin.Get('w')

def run_command(command):
    print command
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("--use", action="store", default="m", 
            choices=["m", "e"], help="Decide which object to use")

    return parser.parse_args()

def compare_functions(w, output, fn1, fn2, bins, var, other_vars= {}, line_pos=None, 
        add_fn=None, add_name=None):
    label_vars = []
    for key, val in other_vars.iteritems():
        w.var(key).setVal(val)
        label_vars.append('%s=%g' % (key, val))
    if add_fn is not None:
        w.factory('expr::{}("@0*@1", {}, {})'.format(add_name, fn1, add_fn))
        h1 = w.function(add_name).createHistogram(
                add_name, w.var(var),
                ROOT.RooFit.Binning(*bins),
                ROOT.RooFit.Scaling(False)
                )
    else:
        h1 = w.function(fn1).createHistogram(
                fn1, w.var(var),
                ROOT.RooFit.Binning(*bins),
                ROOT.RooFit.Scaling(False)
                )
    h2 = w.function(fn2).createHistogram(
            fn2, w.var(var),
            ROOT.RooFit.Binning(*bins),
            ROOT.RooFit.Scaling(False)
            )
    canv = ROOT.TCanvas(output, output)
    pads = plot.TwoPadSplit(0.30, 0.01, 0.01)
    pads[0].cd()
    pads[0].SetGrid(1, 1)
    plot.Set(h1, LineColor=ROOT.kBlack, LineWidth=2)
    if "ratio" in fn1:
        plot.Set(h1.GetYaxis(), Title='SF')
    else:
        plot.Set(h1.GetYaxis(), Title='Efficiency')
    plot.Set(h2, LineColor=ROOT.kRed, LineWidth=2)

    for i in xrange(1, h1.GetNbinsX()+1):
        h1.SetBinError(i, 0.)
    for i in xrange(1, h2.GetNbinsX()+1):
        h2.SetBinError(i, 0.)
    h1.Draw('L')
    h2.Draw('LSAME')
    ratio = h1.Clone()
    ratio.Divide(h2)
    ratio.GetYaxis().SetNdivisions(4)


    if "data" in fn1 or "mc" in fn1:
        legend = plot.PositionedLegend(0.37,0.15,6,0.03)
    else:
        legend = plot.PositionedLegend(0.37,0.15,3,0.03)
    if add_fn is not None: 
        legend.AddEntry(h1, add_name, 'L')
    else: 
        legend.AddEntry(h1, fn1, 'L')
    legend.AddEntry(h2, fn2, 'L')
    legend.Draw()
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.25)
    if "trg_ratio" in fn1:
        plot.FixTopRange(pads[0], 1.5, 0.25)
    plot.DrawTitle(pads[0], ','.join(label_vars), 1)

    line = ROOT.TLine()
    plot.Set(line, LineColor=12, LineStyle=4, LineWidth=2)
    if line_pos is not None:
        plot.DrawVerticalLine(pads[0], line, line_pos)

    pads[1].cd()
    pads[1].SetGrid(1, 1)
    ratio.Draw('L')
    plot.SetupTwoPadSplitAsRatio(
            pads, plot.GetAxisHist(
                pads[0]), plot.GetAxisHist(pads[1]), 'Ratio', True, 0.91, 1.09
            )
    if line_pos is not None:
        plot.DrawVerticalLine(pads[1], line, line_pos)
    canv.Print('.pdf')
    canv.Print('.png')


if __name__ == "__main__":

    args = parse_arguments()

    # bins in eta to use
    eta_bins = []

    # muons
    if args.use == "m":
        eta_bins = [0.0, 0.9, 1.2, 2.1]
        trg_threshold = 24.
        # single_trg_desy = "m_trgIsoMu24orIsoMu27_desy"
        single_trg_desy = "m_trg24_27_kit"
    elif args.use == "e":
        eta_bins = [0.0, 2.0, 2.4]
        trg_threshold = 32.
        single_trg_desy = "e_trgEle32orEle35_desy"

    for eta_bin in eta_bins:
        # idiso ratio, same syntax for both muons and electrons
        # compare_functions(w, 'compare_ic_kis_{}_idiso_ratio_eta_{}'.format(args.use, eta_bin).replace(".","p"),
        #         '{}_id_ratio'.format(args.use),
        #         '{}_idiso_desy_ratio'.format(args.use),
        #         bins=[100, 0, 100],
        #         var='{}_pt'.format(args.use),
        #         other_vars={'{}_eta'.format(args.use):eta_bin},
        #         line_pos=trg_threshold,
        #         add_fn="{}_iso_ratio".format(args.use), # need these for idiso
        #         add_name="{}_idiso_ratio".format(args.use),
        #         )

        # trg comparisons, for data, mc and ratio/SFs
        for t in ["data", "mc", "ratio"]:
            compare_functions(w,
                    'compare_ic_kit_{}_trg_{}_eta_{}'.format(args.use, t, eta_bin).replace(".","p"),
                    '{}_trg_{}'.format(args.use, t),
                    '{}_{}'.format(single_trg_desy, t),
                    bins=[100, 0, 100],
                    var='{}_pt'.format(args.use),
                    other_vars={'{}_eta'.format(args.use):eta_bin},
                    line_pos=trg_threshold,
                    )



