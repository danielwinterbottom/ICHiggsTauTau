import ROOT
import argparse
from UserCode.ICHiggsTauTau.analysis import *
import CombineHarvester.CombineTools.plotting as plot
from numpy import std

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

def ComputeRMS(nominal, variations):
    resultLo = nominal.Clone()
    resultHi = nominal.Clone()
    for b in xrange(1, nominal.GetNbinsX() + 1):
        vals = []
        for v in variations:
            vals.append(v.GetBinContent(b))
        stddev = std(vals)
            # vals.append(abs(v.GetBinContent(b) - nominal.GetBinContent(b)))
        resultLo.SetBinContent(b, nominal.GetBinContent(b) - stddev)
        resultHi.SetBinContent(b, nominal.GetBinContent(b) + stddev)
    return (resultLo, resultHi)


def ComputeEnvelope(nominal, variations):
    resultLo = nominal.Clone()
    resultHi = nominal.Clone()
    for b in xrange(1, nominal.GetNbinsX() + 1):
        vals = []
        for v in variations:
            vals.append(v.GetBinContent(b))
            # vals.append(abs(v.GetBinContent(b) - nominal.GetBinContent(b)))
        resultLo.SetBinContent(b, min(vals))
        resultHi.SetBinContent(b, max(vals))
    return (resultLo, resultHi)

ROOT.TH1.SetDefaultSumw2(True)

renorm = False

ana = Analysis()

path = 'output/HTT2016Studies_Feb6/Theory'
ana.AddSamples('%s/*.root' % path, 'Theory')
ana.AddInfo('scripts/params_Feb6.json', scaleTo='data_obs')

varlist = [
    ('pt_h', 'pt_h[0:50:5,50:100:10,100:200:20]', ('p_{T}^{H} (GeV)', 'd#sigma/dp_{T}^{H} (pb/GeV)')),
    ('njets', 'njets(6,-0.5,5.5)', ('Number of jets', '#sigma (pb)')),
]

for name, var, axes in varlist:
    ana.nodes.AddNode(ListNode(name))

    ana.nodes[name].AddNode(ana.BasicFactory('Nominal', 'Nominal', var, '1'))
    ana.nodes[name].AddNode(ana.BasicFactory('Herwigpp', 'Herwigpp', var, '1'))
    ana.nodes[name].AddNode(ana.BasicFactory('HerwigppRW', 'Herwigpp', var, 'herwig_wt'))
    ana.nodes[name].AddNode(ana.BasicFactory('amcatnlo', 'amcatnlo', var, 'wt'))

    for i in [0, 1, 2, 3, 4, 6, 8]:
        ana.nodes[name].AddNode(ana.BasicFactory('scale_wt_%i' % i, 'Nominal', var, '1*scale_wt_%i' % i))

    for i in xrange(100):
        ana.nodes[name].AddNode(ana.BasicFactory('pdf_wt_%i' % i, 'Nominal', var, '1*pdf_wt_%i' % i))


    ana.nodes[name].AddNode(ana.BasicFactory('PyScaleDown', 'PyScaleDown', var, '1'))
    ana.nodes[name].AddNode(ana.BasicFactory('PyScaleUp', 'PyScaleUp', var, '1'))

    ana.nodes[name].AddNode(ana.BasicFactory('PyUEDown', 'PyUEDown', var, '1'))
    ana.nodes[name].AddNode(ana.BasicFactory('PyUEUp', 'PyUEUp', var, '1'))


ana.nodes.PrintTree()

ana.Run()

for name, var, axes in varlist:
    canv = ROOT.TCanvas('output_%s' % name, 'output_%s' % var)
    if renorm:
        canv.SetName(canv.GetName()+'_renorm')
    pads = plot.TwoPadSplit(0.45, 0.01, 0.01)
    legend = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')
    h_nominal = ana.nodes[name]['Nominal'].shape.hist
    h_herwigpp = ana.nodes[name]['Herwigpp'].shape.hist
    h_herwigppRW = ana.nodes[name]['HerwigppRW'].shape.hist
    h_amcatnlo = ana.nodes[name]['amcatnlo'].shape.hist

    if renorm:
        h_nominal.Scale(1. / h_nominal.Integral())
        h_herwigpp.Scale(1. / h_herwigpp.Integral())
        h_herwigppRW.Scale(1. / h_herwigppRW.Integral())
        h_amcatnlo.Scale(1. / h_amcatnlo.Integral())

    h_scales = []
    for i in [0, 1, 2, 3, 4, 6, 8]:
        h_scales.append(ana.nodes[name]['scale_wt_%i' % i].shape.hist)
        if renorm:
            h_scales[-1].Scale(1. / h_scales[-1].Integral())
    h_scaleLo, h_scaleHi = ComputeEnvelope(h_nominal, h_scales)

    h_pdfs = []
    for i in xrange(100):
        h_pdfs.append(ana.nodes[name]['pdf_wt_%i' % i].shape.hist)
        if renorm:
            h_pdfs[-1].Scale(1. / h_pdfs[-1].Integral())
    h_pdfLo, h_pdfHi = ComputeRMS(h_nominal, h_pdfs)

    h_pyscaleLo = ana.nodes[name]['PyScaleDown'].shape.hist
    h_pyscaleHi = ana.nodes[name]['PyScaleUp'].shape.hist

    h_pyueLo = ana.nodes[name]['PyUEDown'].shape.hist
    h_pyueHi = ana.nodes[name]['PyUEUp'].shape.hist

    if renorm:
        h_pyscaleLo.Scale(1. / h_pyscaleLo.Integral())
        h_pyscaleHi.Scale(1. / h_pyscaleHi.Integral())
        h_pyueLo.Scale(1. / h_pyueLo.Integral())
        h_pyueHi.Scale(1. / h_pyueHi.Integral())

    h_nominal.Scale(1, 'width')
    h_herwigpp.Scale(1, 'width')
    h_herwigppRW.Scale(1, 'width')
    h_amcatnlo.Scale(1, 'width')
    h_scaleLo.Scale(1, 'width')
    h_scaleHi.Scale(1, 'width')
    h_pdfLo.Scale(1, 'width')
    h_pdfHi.Scale(1, 'width')
    h_pyscaleLo.Scale(1, 'width')
    h_pyscaleHi.Scale(1, 'width')
    h_pyueLo.Scale(1, 'width')
    h_pyueHi.Scale(1, 'width')


    plot.Set(h_nominal, LineColor=ROOT.kBlack, LineWidth=3)
    plot.Set(h_herwigpp, LineColor=ROOT.kGreen, LineWidth=3)
    plot.Set(h_herwigppRW, LineColor=ROOT.kGreen+3, LineWidth=3, LineStyle=7)
    plot.Set(h_amcatnlo, LineColor=ROOT.kMagenta, LineWidth=3)
    plot.Set(h_scaleLo, LineColor=ROOT.kRed, LineWidth=2, LineStyle=2)
    plot.Set(h_scaleHi, LineColor=ROOT.kRed, LineWidth=2, LineStyle=2)
    plot.Set(h_pdfLo, LineColor=ROOT.kBlue, LineWidth=2, LineStyle=2)
    plot.Set(h_pdfHi, LineColor=ROOT.kBlue, LineWidth=2, LineStyle=2)
    plot.Set(h_pyscaleLo, LineColor=ROOT.kOrange+2, LineWidth=2, LineStyle=7)
    plot.Set(h_pyscaleHi, LineColor=ROOT.kOrange+2, LineWidth=2, LineStyle=9)
    plot.Set(h_pyueLo, LineColor=ROOT.kGray+2, LineWidth=2, LineStyle=7)
    plot.Set(h_pyueHi, LineColor=ROOT.kGray+2, LineWidth=2, LineStyle=9)
    legend.AddEntry(h_nominal, 'powheg+pythia8', 'L')
    legend.AddEntry(h_herwigpp, 'powheg+herwig++', 'L')
    legend.AddEntry(h_herwigppRW, 'powheg+herwig++ (RW)', 'L')
    legend.AddEntry(h_amcatnlo, 'aMC@NLO+pythia8', 'L')
    legend.AddEntry(h_scaleLo, 'QCD scale', 'L')
    legend.AddEntry(h_pdfLo, 'PDF (NNPDF 3.0)', 'L')
    legend.AddEntry(h_pyscaleLo, 'pythia8 PS scale (Up)', 'L')
    legend.AddEntry(h_pyscaleHi, 'pythia8 PS scale (Down)', 'L')
    legend.AddEntry(h_pyueLo, 'pythia8 UE tune (Up)', 'L')
    legend.AddEntry(h_pyueHi, 'pythia8 UE tune (Down)', 'L')
    h_nominal.Draw('SAME')
    h_herwigpp.Draw('SAMEHIST')
    h_herwigppRW.Draw('SAMEHIST')
    h_amcatnlo.Draw('SAMEHIST')
    h_scaleLo.Draw('SAMEHIST')
    h_scaleHi.Draw('SAMEHIST')
    h_pdfLo.Draw('SAMEHIST')
    h_pdfHi.Draw('SAMEHIST')
    h_pyscaleLo.Draw('SAMEHIST')
    h_pyscaleHi.Draw('SAMEHIST')
    h_pyueLo.Draw('SAMEHIST')
    h_pyueHi.Draw('SAMEHIST')
    h_nominal.Draw('SAME')
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.30)
    legend.Draw()

    axis = plot.GetAxisHist(pads[0])
    plot.Set(axis.GetXaxis(), Title=axes[0])
    plot.Set(axis.GetYaxis(), Title=axes[1])

    if renorm:
        plot.Set(axis.GetYaxis(), Title="a.u")

    pads[1].cd()
    pads[1].SetGrid(0, 1)
    ratio_hist = h_nominal.Clone()
    ratio_hist.Reset()
    # ratio_hist.Draw('SAME')

    h_amcatnloRatio = h_amcatnlo.Clone()
    h_amcatnloRatio.Divide(h_nominal)
    h_amcatnloRatio.Draw('SAMEHIST')

    h_herwigppRatio = h_herwigpp.Clone()
    h_herwigppRatio.Divide(h_nominal)
    print 'Herwigpp:'
    h_herwigppRatio.Print('range')
    h_herwigppRatio.Draw('SAMEHIST')


    h_herwigppRWRatio = h_herwigppRW.Clone()
    h_herwigppRWRatio.Divide(h_nominal)
    h_herwigppRWRatio.Draw('SAMEHIST')


    ## Ratios for the QCD scale envelope
    h_scaleLoRatio = h_scaleLo.Clone()
    h_scaleLoRatio.Divide(h_nominal)
    h_scaleHiRatio = h_scaleHi.Clone()
    h_scaleHiRatio.Divide(h_nominal)
    h_scaleLoRatio.Draw('SAMEHIST')
    h_scaleHiRatio.Draw('SAMEHIST')

    ## Ratios for the pdf variation
    h_pdfLoRatio = h_pdfLo.Clone()
    h_pdfLoRatio.Divide(h_nominal)
    h_pdfHiRatio = h_pdfHi.Clone()
    h_pdfHiRatio.Divide(h_nominal)
    h_pdfLoRatio.Draw('SAMEHIST')
    h_pdfHiRatio.Draw('SAMEHIST')


    h_pyscaleLoRatio = h_pyscaleLo.Clone()
    h_pyscaleLoRatio.Divide(h_nominal)
    h_pyscaleHiRatio = h_pyscaleHi.Clone()
    h_pyscaleHiRatio.Divide(h_nominal)
    h_pyscaleLoRatio.Draw('SAMEHIST')
    h_pyscaleHiRatio.Draw('SAMEHIST')

    print 'PyScale Lo/Hi:'

    h_pyscaleLoRatio.Print('range')
    h_pyscaleHiRatio.Print('range')

    h_pyueLoRatio = h_pyueLo.Clone()
    h_pyueLoRatio.Divide(h_nominal)
    h_pyueHiRatio = h_pyueHi.Clone()
    h_pyueHiRatio.Divide(h_nominal)
    h_pyueLoRatio.Draw('SAMEHIST')
    h_pyueHiRatio.Draw('SAMEHIST')

    print 'PyUE Lo/Hi:'

    h_pyueLoRatio.Print('range')
    h_pyueHiRatio.Print('range')

    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Ratio/Nominal', True, 0.5, 1.5)

    plot.DrawTitle(pads[0], 'GluGluToHToTauTau', 3)
    canv.Print('.pdf')
    canv.Print('.png')


outfile = ROOT.TFile('output.root', 'RECREATE')

ana.nodes.Output(outfile)

outfile.Close()

