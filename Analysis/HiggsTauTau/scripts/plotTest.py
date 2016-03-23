#!/usr/bin/env python
import ROOT
import CombineHarvester.CombineTools.plotting as plot
from UserCode.ICHiggsTauTau.uncertainties import ufloat
import UserCode.ICHiggsTauTau.MultiDraw

from time import time

def Rate(hist):
    err = ROOT.Double()
    integral = hist.IntegralAndError(0, hist.GetNbinsX()+1, err)
    return ufloat(integral, err)

# ## Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

target = 'WJetsToLNuNLO'
wt_name = '(n_jets>=1)*pdf_wt'
n_points = 99
# wt_name = '(n_jets>=1)*scale_wt'
# n_points = 9
do_mt_plot = True

legends = [
    '#mu_{R}=1.0 #mu_{F}=1.0',
    '#mu_{R}=1.0 #mu_{F}=2.0',
    '#mu_{R}=1.0 #mu_{F}=0.5',
    '#mu_{R}=2.0 #mu_{F}=1.0',
    '#mu_{R}=2.0 #mu_{F}=2.0',
    '#mu_{R}=2.0 #mu_{F}=0.5',
    '#mu_{R}=0.5 #mu_{F}=1.0',
    '#mu_{R}=0.5 #mu_{F}=2.0',
    '#mu_{R}=0.5 #mu_{F}=0.5'
]

file = ROOT.TFile('%s.root' % target)
tree = file.Get('wjetsstudy')

draw_lo = [("0.5(1,0,1)", "(mt_1<30)*wt*%s_%i" % (wt_name, x)) for x in range(n_points)]
draw_hi = [("0.5(1,0,1)", "(mt_1>70)*wt*%s_%i" % (wt_name, x)) for x in range(n_points)]

start = time()
res_lo = tree.MultiDraw(draw_lo)
res_hi = tree.MultiDraw(draw_hi)
print "Total %.2fs" % (time() - start), " " * 20


print res_lo
print res_hi

outfile = ROOT.TFile('%s_output.root' % target, 'RECREATE')

hratio = ROOT.TH1F('ratio', 'ratio', 400, 0.2, 0.4)

unc_ratios = []

for i, h in enumerate(zip(res_lo, res_hi)):
    hratio.Fill((Rate(h[0])/Rate(h[1])).nominal_value)
    print ('%i : ' % i) + str(Rate(h[0])/Rate(h[1]))
    unc_ratios.append((Rate(h[0])/Rate(h[1])))
    # outfile.WriteTObject(h)

for i in xrange(1, len(unc_ratios)):
    unc_ratios[i] = unc_ratios[i]/unc_ratios[0]


cols = [1, 2, 4, 6, 8, 9, 30, 40, 46, 48, 28]

if do_mt_plot:
    draw_mt = [("mt_1(30,0,150)", "wt*%s_%i" % (wt_name, x)) for x in range(n_points)]
    mt_hists = tree.MultiDraw(draw_mt)

    canv = ROOT.TCanvas('mt_dist', 'mt_dist')
    pads = plot.TwoPadSplit(0.30, 0.01, 0.01)

    legend = ROOT.TLegend(0.65, 0.71, 0.93, 0.92, '', 'NBNDC')

    # pads = plot.TwoPadSplit 
    for i, h in enumerate(mt_hists):
        if i in [5, 7]: continue
        h.SetLineColor(cols.pop())
        # h.Scale(1/h.Integral(0,h.GetNbinsX()+1))
        h.Draw('SAMEHIST')
        legend.AddEntry(h, legends[i] +': %s' % unc_ratios[i], 'L')
        # print h.Integral(1,6) / h.Integral(15,31)
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.3)
    legend.Draw()

    # latex = ROOT.TLatex()
    # latex.SetNDC()
    # latex.SetTextSize(0.04)
    # latex.DrawLatex(0.2, 0.85, "OS/SS = " + AsTex(os_ss_ratio))

    # axis = plot.GetAxisHist(pads[0])
    # plot.Set(axis.GetXaxis(), Title=draw[2])
    # plot.Set(axis.GetYaxis(), Title='Events')

    pads[1].cd()
    pads[1].SetGrid(0, 1)
    ratio_hists = [x.Clone() for x in mt_hists]
    # ratio_hists[0].Draw('')
    for i, h in enumerate(ratio_hists):
        h.Divide(mt_hists[0])
        if i in [5, 7]:
            continue
        h.Draw('SAMEHIST')

    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Ratio', True, 0.91, 1.09)

    # # plot.DrawTitle(pads[0], "OS/SS = " + AsTex(os_ss_ratio), 1)
    # plot.DrawTitle(pads[0], 'QCD_Pt_20_MuEnrichedPt_15, 8 TeV', 3)

    canv.Print('.pdf')
    canv.Print('.png')

    for i, h in enumerate(mt_hists):
        outfile.WriteTObject(h)


outfile.WriteTObject(hratio)



outfile.Close()
