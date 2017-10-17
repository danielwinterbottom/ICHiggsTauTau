import ROOT
import argparse
from UserCode.ICHiggsTauTau.analysis import *
import CombineHarvester.CombineTools.plotting as plot
# from numpy import std

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

parser = argparse.ArgumentParser()

parser.add_argument('-m', '--mass', default='500')
parser.add_argument('-t', '--tanb', default='15')
parser.add_argument('-H', '--higgs', default='H')
args = parser.parse_args()

# def ComputeRMS(nominal, variations):
#     resultLo = nominal.Clone()
#     resultHi = nominal.Clone()
#     for b in xrange(1, nominal.GetNbinsX() + 1):
#         vals = []
#         for v in variations:
#             vals.append(v.GetBinContent(b))
#         stddev = std(vals)
#             # vals.append(abs(v.GetBinContent(b) - nominal.GetBinContent(b)))
#         resultLo.SetBinContent(b, nominal.GetBinContent(b) - stddev)
#         resultHi.SetBinContent(b, nominal.GetBinContent(b) + stddev)
#     return (resultLo, resultHi)


# def ComputeEnvelope(nominal, variations):
#     resultLo = nominal.Clone()
#     resultHi = nominal.Clone()
#     for b in xrange(1, nominal.GetNbinsX() + 1):
#         vals = []
#         for v in variations:
#             vals.append(v.GetBinContent(b))
#             # vals.append(abs(v.GetBinContent(b) - nominal.GetBinContent(b)))
#         resultLo.SetBinContent(b, min(vals))
#         resultHi.SetBinContent(b, max(vals))
#     return (resultLo, resultHi)
hdict = {
    's': 'h',
    'H': 'H',
    'A': 'A'
}

ROOT.TH1.SetDefaultSumw2(True)

ana = Analysis()

path = 'output/HTT2016Studies_test/Theory'
ana.AddSamples('%s/*.root' % path, 'Theory')

X = '%s_%s_%s' % (args.higgs, args.mass, args.tanb)

varlist = [
    ('pt_h1', 'pt_h[0:100:10,100:200:20,200:600:100]', ('p_{T}^{H} (GeV)', 'd#sigma/dp_{T}^{H} (pb/GeV)')),
    ('pt_h2', 'pt_h[0:100:25,100:200:50,200:600:400]', ('p_{T}^{H} (GeV)', 'd#sigma/dp_{T}^{H} (pb/GeV)')),
    ('pt_h3', 'pt_h[0:100:50,100:200:100,200:600:400]', ('p_{T}^{H} (GeV)', 'd#sigma/dp_{T}^{H} (pb/GeV)')),
    ('njets', 'njets(6,-0.5,5.5)', ('Number of jets', '#sigma (pb)')),
]

with open('xsec.json') as jsonfile:
    xsec = json.load(jsonfile)

for name, var, axes in varlist:
    ana.nodes.AddNode(ListNode(name))

    ana.nodes[name].AddNode(ana.BasicFactory('pythia', 'SUSYGluGluToHToTauTau_M-%s' % args.mass, var, 'wt', scaleToLumi=False))
    ana.nodes[name].AddNode(ana.BasicFactory('pythia_check', 'SUSYGluGluToHToTauTau_M-%s' % args.mass, var, 'wt*wt_2HDM_%s' % hdict[args.higgs], scaleToLumi=False))
    ana.nodes[name].AddNode(ana.BasicFactory('t_t', '%s_t_t' % X, var, 'wt', scaleToLumi=False))
    ana.nodes[name].AddNode(ana.BasicFactory('b_b', '%s_b_b' % X, var, 'wt', scaleToLumi=False))
    ana.nodes[name].AddNode(ana.BasicFactory('tb_tb', '%s_tb_tb' % X, var, 'wt', scaleToLumi=False))
    ana.nodes[name].AddNode(ana.BasicFactory('t_tb', '%s_t_tb' % X, var, 'wt', scaleToLumi=False))
    ana.nodes[name].AddNode(ana.BasicFactory('b_tb', '%s_b_tb' % X, var, 'wt', scaleToLumi=False))


ana.Run()

total_xs = 0.
int_xs = xsec['%s_tb_tb' % X][0] - xsec['%s_t_tb' % X][0] - xsec['%s_b_tb' % X][0]
total_xs = xsec['%s_t_t' % X][0] + xsec['%s_b_b' % X][0] + int_xs

for name, var, axes in varlist:
    for h in ana.nodes[name].SubNodes():
        h.shape.hist.Scale(1. / h.shape.hist.Integral())
        if h.name in ['t_t', 'b_b', 'tb_tb', 't_tb', 'b_tb']:
            h.shape.hist.Scale(xsec['%s_%s' % (X, h.name)][0])
        if h.name in ['pythia', 'pythia_check']:
            h.shape.hist.Scale(total_xs)
        h.shape.hist.Scale(1., 'width')

    canv = ROOT.TCanvas('output_%s_%s' % (name, X), 'output_%s_%s' % (var,X))
    pads = plot.OnePad()
    legend = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')
    h_pythia = ana.nodes[name]['pythia'].shape.hist
    h_pythia_check = ana.nodes[name]['pythia_check'].shape.hist
    h_t_t = ana.nodes[name]['t_t'].shape.hist
    h_b_b = ana.nodes[name]['b_b'].shape.hist
    h_tb_tb = ana.nodes[name]['tb_tb'].shape.hist
    h_t_tb = ana.nodes[name]['t_tb'].shape.hist
    h_b_tb = ana.nodes[name]['b_tb'].shape.hist
    h_int = h_tb_tb.Clone()
    h_int.Add(h_t_tb, -1.)
    h_int.Add(h_b_tb, -1.)

    h_tot = h_t_t.Clone()
    h_tot.Add(h_b_b)
    h_tot.Add(h_int)

    plot.Set(h_pythia, LineColor=28, LineWidth=3, MarkerSize=0.0)
    plot.Set(h_pythia_check, LineColor=28, LineWidth=3, MarkerSize=0.0, LineStyle=2)
    plot.Set(h_t_t, LineColor=ROOT.kRed, LineWidth=2, LineStyle=2, MarkerSize=0.0)
    plot.Set(h_b_b, LineColor=ROOT.kBlue, LineWidth=2, LineStyle=2, MarkerSize=0.0)
    plot.Set(h_int, LineColor=ROOT.kBlack, LineWidth=2, LineStyle=2, MarkerSize=0.0)
    plot.Set(h_tot, LineColor=ROOT.kMagenta, LineWidth=3, MarkerSize=0.0)

    legend.AddEntry(h_pythia, 'Pythia', 'L')
    legend.AddEntry(h_pythia_check, 'Pythia (weighted)', 'L')
    legend.AddEntry(h_t_t, 'Pure top (Qt)', 'L')
    legend.AddEntry(h_b_b, 'Pure bottom (Qb)', 'L')
    legend.AddEntry(h_int, 'Interference (Qtb)', 'L')
    legend.AddEntry(h_tot, 'Powheg total', 'L')

    h_pythia.Draw('SAMEHIST')
    h_t_t.Draw('SAMEHIST')
    h_b_b.Draw('SAMEHIST')
    h_int.Draw('SAMEHIST')
    h_tot.Draw('SAMEHIST')
    h_pythia_check.Draw('SAMEHIST')

    min_val = 0.
    if int_xs < 0.:
        min_val = h_int.GetMinimum()

    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.30)
    plot.GetAxisHist(pads[0]).SetMinimum(min_val)
    legend.Draw()

    axis = plot.GetAxisHist(pads[0])
    plot.Set(axis.GetXaxis(), Title=axes[0])
    plot.Set(axis.GetYaxis(), Title=axes[1])

    part, mA, tanb = X.split('_')
    partx = part if part is not 's' else 'h'
    plot.DrawTitle(pads[0], 'Boson: %s, ^{}m_{%s}=%s GeV, tan#beta=%s' % (partx, partx, mA, tanb), 3)
    canv.Print('.pdf')
    canv.Print('.png')

    if int_xs < 0.:
        print 'Interference cross section is negative: %g, will invert histogram...' % int_xs
        h_int.Scale(-1.)

    # The interference part can have negative contributions
    h_int_unmod = h_int.Clone()
    print 'Int xsec: %g, int hist integral: %g' % (int_xs, h_int.Integral('width'))
    int_pos = 0.
    int_neg = 0.
    for i in xrange(1, h_int.GetNbinsX() + 1):
        if h_int.GetBinContent(i) < 0.:
            int_neg += (h_int.GetBinContent(i) * -1. * h_int.GetBinWidth(i))
            # print 'Bin %i is negative after inversion: %g' % (i, h_int.GetBinContent(i))
            h_int.SetBinContent(i, 0.)
        else:
            int_pos += (h_int.GetBinContent(i) * +1. * h_int.GetBinWidth(i))
    print 'Negative fraction: %g' % (int_neg / (int_pos + int_neg))

    canv2 = ROOT.TCanvas('output_norm_%s_%s' % (name, X), 'output_norm_%s_%s' % (var,X))
    pads2 = plot.OnePad()
    legend2 = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')

    ###### Draw normalised plot
    h_t_t_norm = h_t_t.Clone()
    h_b_b_norm = h_b_b.Clone()
    h_int_norm = h_int.Clone()
    pythia_norm = h_pythia.Clone()

    for h in [h_t_t_norm, h_b_b_norm, h_int_norm, pythia_norm]:
        h.Scale(1. / h.Integral('width'))

    legend2.AddEntry(pythia_norm, 'Pythia', 'L')
    legend2.AddEntry(h_t_t_norm, 'Pure top (Qt)', 'L')
    legend2.AddEntry(h_b_b_norm, 'Pure bottom (Qb)', 'L')
    legend2.AddEntry(h_int_norm, 'Interference (Qtb)', 'L')

    pythia_norm.Draw('SAMEHISTE')
    h_t_t_norm.Draw('SAMEHISTE')
    h_b_b_norm.Draw('SAMEHISTE')
    h_int_norm.Draw('SAMEHISTE')

    plot.FixTopRange(pads2[0], plot.GetPadYMax(pads2[0]), 0.30)
    legend2.Draw()

    axis2 = plot.GetAxisHist(pads2[0])
    plot.Set(axis2.GetXaxis(), Title=axes[0])
    plot.Set(axis2.GetYaxis(), Title=axes[1])

    plot.DrawTitle(pads2[0], 'Boson: %s, ^{}m_{%s}=%s GeV, tan#beta=%s' % (partx, partx, mA, tanb), 3)
    canv2.Print('.pdf')
    canv2.Print('.png')
    ######

    ana.nodes[name].AddNode(BasicNode('int', '', var, 'wt'))
    ana.nodes[name]['int'].shape = Shape(h_int)

    ana.nodes[name].AddNode(BasicNode('int_unmod', '', var, 'wt'))
    ana.nodes[name]['int_unmod'].shape = Shape(h_int_unmod)

    ana.nodes[name].AddNode(BasicNode('tot', '', var, 'wt'))
    ana.nodes[name]['tot'].shape = Shape(h_tot)


outfile = ROOT.TFile('output_%s.root' % X, 'RECREATE')

ana.nodes.Output(outfile)

outfile.Close()
