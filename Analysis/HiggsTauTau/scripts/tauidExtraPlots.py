import CombineHarvester.CombineTools.plotting as plot
import ROOT
import argparse
from array import array
# import sys
# from copy import deepcopy

ROOT.TH1.SetDefaultSumw2(True)
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()
plot.SetDeepSeaPalette()


def Getter(file, hist, width=False):
    h = file.Get(hist)
    if width and len(h.GetXaxis().GetXbins()) > 0:
        h.Scale(1., 'width')
    return h


parser = argparse.ArgumentParser()
parser.add_argument('--input', '-i', help='Output of PostFitShapes or PostFitShapesFromWorkspace, specified as FILE:BIN')
parser.add_argument('--output', '-o', default='', help='Output name')
parser.add_argument('--title', default='')

args = parser.parse_args()

output = args.output

filename, folder = args.input.split(':')
file = ROOT.TFile(filename)

doIsoDensity = False
doDeltaBeta = False
doSfFit = True
## The isolation density summary plots

if doIsoDensity:
    for var, label in [('nt', 'Iso #gamma'), ('po', 'Signal #gamma')]:
        canv = ROOT.TCanvas(output + "_%s_isodensity" % var, output + "_%s_isodensity" % var)
        pads = plot.TwoPadSplit(0.27, 0.01, 0.01)
        axis = ROOT.TH1F('axis', '', 5, 0, 0.5)
        axisr = ROOT.TH1F('axisr', '', 5, 0, 0.5)
        plot.Set(axis.GetXaxis(), Title='#DeltaR from #vec{#tau}_{charged}')
        plot.Set(axis.GetYaxis(), Title='%s energy density (GeV)' % label)
        hists_data = []
        hists_mc = []
        axis.Draw()
        graph_data = ROOT.TGraphAsymmErrors(5)
        graph_mc = ROOT.TGraphAsymmErrors(5)
        graph_ratio = ROOT.TGraphAsymmErrors(5)
        for i in xrange(5):
            hists_data.append(Getter(file, '%s/%s_density_0p%i_0p%i/data_obs' % (folder, var, i, i + 1)))
            hists_mc.append(Getter(file, '%s/%s_density_0p%i_0p%i/ZTT' % (folder, var, i, i + 1)))
            for proc in ['ZL', 'ZJ', 'TT', 'W', 'QCD', 'VV']:
                hists_mc[i].Add(Getter(file, '%s/%s_density_0p%i_0p%i/%s' % (folder, var, i, i + 1, proc)))
            graph_data.SetPoint(i, float(i) * 0.1 + 0.05, hists_data[i].GetMean())
            graph_data.SetPointError(i, 0.05, 0.05, hists_data[i].GetMeanError(), hists_data[i].GetMeanError())
            graph_mc.SetPoint(i, float(i) * 0.1 + 0.05, hists_mc[i].GetMean())
            graph_mc.SetPointError(i, 0.05, 0.05, hists_mc[i].GetMeanError(), hists_mc[i].GetMeanError())
            graph_ratio.SetPoint(i, float(i) * 0.1 + 0.05, graph_data.GetY()[i] / graph_mc.GetY()[i])
            graph_ratio.SetPointError(i, 0.05, 0.05, graph_data.GetEYlow()[i] / graph_mc.GetY()[i], graph_data.GetEYhigh()[i] / graph_mc.GetY()[i])

        plot.Set(graph_data, LineColor=ROOT.kRed, MarkerColor=ROOT.kRed)
        graph_data.Draw('SAMEP')
        graph_mc.Draw('SAMEP')
        plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.35)

        pads[1].cd()
        pads[1].SetGrid(0, 1)
        axisr.Draw()
        plot.SetupTwoPadSplitAsRatio(
            pads, plot.GetAxisHist(
                pads[0]), plot.GetAxisHist(pads[1]), 'Data/MC', True, 0.61, 1.39)
        graph_ratio.Draw('SAMEP')
        # CMS logo
        plot.DrawCMSLogo(pads[0], 'CMS', 'Internal', 11, 0.045, 0.05, 1.0, '', 1.0)
        plot.DrawTitle(pads[0], '12.9 fb^{-1} (13 TeV)', 3)

        latex = ROOT.TLatex()
        plot.Set(latex, NDC=None, TextFont=42, TextSize=0.03)
        latex.DrawLatex(0.20, 0.75, args.title)
        # graph_data.Print()
        legend = ROOT.TLegend(0.67, 0.78, 0.90, 0.91, '', 'NBNDC')
        legend.AddEntry(graph_data, 'Data', 'PL')
        legend.AddEntry(graph_mc, 'All MC', 'PL')
        legend.Draw()
        canv.Print('.png')
        canv.Print('.pdf')


if doDeltaBeta:
    for var in ['ntiso_t', 'pho_out_t']:
        canvColz = ROOT.TCanvas(output + "_dbeta_%s" % var, output + "_dbeta_%s" % var)
        pads = plot.OnePad()
        pads[0].SetRightMargin(0.22)
        hist = Getter(file, '%s/%s_vs_puiso_t/TT' % (folder, var))
        plot.Set(hist.GetZaxis(), Title='Events', TitleOffset=1.5)
        plot.Set(hist.GetXaxis(), Title=hist.GetXaxis().GetTitle().replace(':GeV', ' (GeV)'))
        plot.Set(hist.GetYaxis(), Title=hist.GetYaxis().GetTitle().replace(':GeV', ' (GeV)'))
        hist.SetContour(255)
        hist.Draw('COLZ')
        plot.DrawTitle(pads[0], '12.9 fb^{-1} (13 TeV)', 3)
        plot.DrawTitle(pads[0], args.title, 1)
        prof = hist.ProfileX('_pfx', -1, -1)
        plot.Set(prof, LineColor=ROOT.kBlack, MarkerColor=ROOT.kBlack, MarkerSize=0.2, LineWidth=2)
        prof.Draw('SAME')
        res = prof.Fit('pol1', 'S0', '', 0, 100)
        fn = prof.GetFunction("pol1")
        # fn.Print()
        plot.Set(fn, LineColor=ROOT.kRed, LineWidth=2)
        fn.Draw('SAME')
        print res.Parameter(0)
        print res.Parameter(1)
        text = ROOT.TPaveText(0.2, 0.8, 0.5, 0.9, 'NDC')
        text.AddText('p0 = %.3g #pm %.3g' % (res.Parameter(0), res.ParError(0)))
        text.AddText('p1 = %.3g #pm %.3g' % (res.Parameter(1), res.ParError(1)))
        plot.Set(text, BorderSize=0, TextAlign=12)
        text.Draw()
        canvColz.Print('.png')
        canvColz.Print('.pdf')

if doSfFit:
    x_vals = [25, 35, 50, 70, 150]
    y_vals = [0.87, 0.86, 0.90, 0.93, 0.85]
    x_err =  [0, 0, 0, 0, 0]
    y_err =  [0.05, 0.04, 0.07, 0.09, 0.12]
    gr = ROOT.TGraphErrors(len(x_vals), array('d', x_vals), array('d', y_vals), array('d', x_err), array('d', y_err))
    canv = ROOT.TCanvas(output + "_sf_fit", output + "_sf_fit")
    pads = plot.OnePad()
    axis = plot.CreateAxisHist(gr, False)
    axis.GetXaxis().SetTitle('#tau_{h} p_{T} (GeV)')
    axis.GetYaxis().SetTitle('Scale Factor')
    pads[0].cd()
    axis.Draw()
    gr.Draw('PSAME')
    gr.Fit('pol1', '', '3')
    # text.Draw()
    canv.Print('.png')
    canv.Print('.pdf')


