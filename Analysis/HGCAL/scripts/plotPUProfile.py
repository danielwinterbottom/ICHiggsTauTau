import argparse
import ROOT
import os
import time
from numpy import mean
from array import array
import CombineHarvester.CombineTools.plotting as plot
# import UserCode.ICHiggsTauTau.MultiDraw

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.WARNING)


parser = argparse.ArgumentParser()
# parser.add_argument('input')
parser.add_argument('--file', '-f', default='output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_pusub_0.root')
# parser.add_argument('--event', '-e', default=1, type=int)
# parser.add_argument('--region', '-r', default='p')
# parser.add_argument('--window', default=0.1, type=float)

# parser.add_argument('--bkg-model', default='Exponential')
# parser.add_argument('--title', default='Muon ID Efficiency')
# parser.add_argument('--postfix', default='')
# parser.add_argument('--plot-dir', '-p', default='./')
# parser.add_argument('--bin-replace', default=None) #(100,2.3,80,2.3)
args = parser.parse_args()


plot.ModTDRStyle(width=700)

infileName = args.file


canv = ROOT.TCanvas('pu_profile', '')

pads = plot.OnePad()
pads[0].cd()
infile = ROOT.TFile(infileName)

legend = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')


gr1 = infile.Get('pu_profile_0')
gr2 = infile.Get('pu_profile_5')
gr3 = infile.Get('pu_profile_10')
gr4 = infile.Get('pu_profile_20')
gr5 = infile.Get('pu_profile_27')
gr6 = infile.Get('pu_profile_22')
gr7 = infile.Get('pu_profile_25')

gr1.Draw("axis")
axis = plot.GetAxisHist(pads[0])
axis.GetXaxis().SetTitle('|#eta|')
axis.GetYaxis().SetTitle('Median rechit energy [GeV]')
axis.SetMaximum(0.15)

pr1 = gr1.ProfileX('gr1', 1, -1, 's')
pr2 = gr2.ProfileX('gr2', 1, -1, 's')
pr3 = gr3.ProfileX('gr3', 1, -1, 's')
pr4 = gr4.ProfileX('gr4', 1, -1, 's')
pr5 = gr5.ProfileX('gr5', 1, -1, 's')
pr6 = gr6.ProfileX('gr6', 1, -1, 's')
pr7 = gr7.ProfileX('gr7', 1, -1, 's')
pr1.SetMarkerSize(0.2)

plot.Set(pr1, MarkerSize=0.5, LineWidth=2, MarkerColor=ROOT.kBlack, LineColor=ROOT.kBlack)
plot.Set(pr2, MarkerSize=0.5, LineWidth=2, MarkerColor=ROOT.kRed, LineColor=ROOT.kRed)
plot.Set(pr3, MarkerSize=0.5, LineWidth=2, MarkerColor=ROOT.kBlue, LineColor=ROOT.kBlue)
plot.Set(pr4, MarkerSize=0.5, LineWidth=2, MarkerColor=ROOT.kGreen, LineColor=ROOT.kGreen)
plot.Set(pr5, MarkerSize=0.5, LineWidth=2, MarkerColor=28, LineColor=28)
plot.Set(pr6, MarkerSize=0.5, LineWidth=2, MarkerColor=6, LineColor=6)
plot.Set(pr7, MarkerSize=0.5, LineWidth=2, MarkerColor=9, LineColor=9)

pr1.Draw('SAMEE')
pr2.Draw('SAMEE')
pr3.Draw('SAMEE')
pr4.Draw('SAMEE')
pr5.Draw('SAMEE')
pr6.Draw('SAMEE')
pr7.Draw('SAMEE')

legend.AddEntry(pr1, 'Layer 1', 'L')
legend.AddEntry(pr2, 'Layer 6', 'L')
legend.AddEntry(pr3, 'Layer 11', 'L')
legend.AddEntry(pr4, 'Layer 21', 'L')
legend.AddEntry(pr6, 'Layer 23', 'L')
legend.AddEntry(pr7, 'Layer 26', 'L')
legend.AddEntry(pr5, 'Layer 28', 'L')
legend.Draw()

canv.Print('.png')
canv.Print('.pdf')

# # infileName = 'output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_0.root'

# setRanges = False
# infoStr = None

# for layer in range(1, 2):

#     nlayers = 52
#     event = args.event
#     region = args.region
#     # layer = 3

#     infile = ROOT.TFile(infileName)

#     graph = infile.Get('event_%i/%s_signif_rechits' % (event, region))
#     if layer == 1:
#         infoStr = graph.GetTitle().split(':')
#     graph.SetMarkerStyle(20)
#     graph.SetMarkerSize(5)

#     pions = infile.Get('event_%i/%s_pions' % (event, region))
#     photons = infile.Get('event_%i/%s_photons' % (event, region))


#     jet_graphs = []
#     jet_graph_idx = 0
#     jet_graph = infile.Get('event_%i/%s_signif_rechit_clusters_%i' % (event, region, jet_graph_idx))
#     while jet_graph != None:
#         jet_graphs.append(jet_graph)
#         jet_graph_idx += 1
#         jet_graph = infile.Get('event_%i/%s_signif_rechit_clusters_%i' % (event, region, jet_graph_idx))


#     ncols = ROOT.gStyle.GetNumberOfColors()

#     if not setRanges:
#         xmin = graph.GetXmin()
#         xmax = graph.GetXmax()
#         ymin = graph.GetYmin()
#         ymax = graph.GetYmax()
#     zmin = 0
#     if graph.GetN():
#         zmax = graph.GetZmax()
#     else:
#         zmax = 1.

#     if not setRanges and pions.GetN() > 0:
#         xvals = []
#         yvals = []
#         for i in xrange(pions.GetN()):
#             # theColor = int(((graph.GetZ()[i] - zmin) / (zmax - zmin)) * (ncols - 1))
#             x = ROOT.Double()
#             y = ROOT.Double()
#             z = ROOT.Double()
#             pions.GetPoint(i, x, y, z)
#             xvals.append(x)
#             yvals.append(y)
#         xmean = mean(xvals)
#         ymean = mean(yvals)
#         xmin = xmean - args.window
#         xmax = xmean + args.window
#         ymin = ymean - args.window
#         ymax = ymean + args.window
#         setRanges = True


#     axis = ROOT.TH2F("axis", "", 1, xmin, xmax, 1, ymin, ymax)
#     axis.SetBinContent(1, 1, 0)

#     axis.SetMinimum(zmin)
#     axis.SetMaximum(zmax)
#     axis.GetXaxis().SetTitle('#eta')
#     axis.GetYaxis().SetTitle('#phi')
#     axis.GetZaxis().SetTitle('E [GeV]')
#     axis.GetZaxis().SetTitleOffset(axis.GetZaxis().GetTitleOffset() * 1.3)
#     canv = ROOT.TCanvas('evt_%i_%s_signifs' % (event, region), '')
#     # view = ROOT.TView.CreateView(1)

#     pads = plot.OnePad()
#     pads[0].cd()

#     axis.Draw('colz')

#     # help(graph)

#     markers = ROOT.TPolyMarker(1)
#     markers.SetMarkerStyle(20)
#     markers.SetMarkerSize(0.8)

#     jmarkers = ROOT.TPolyMarker(1)
#     jmarkers.SetMarkerStyle(4)
#     jmarkers.SetMarkerSize(0.8)

#     latex = ROOT.TLatex()
#     # latex.SetNDC()
#     latex.SetTextSize(0.02)
#     # latex.SetTextFont(42)
#     # latex.DrawLatex(0.7, 0.84, 'Mean = %.3f' % wsp.var('mean').getVal())
#     # latex.DrawLatex(0.7, 0.79, 'Sigma = %.3f' % wsp.var('sigma').getVal())

#     for i in xrange(graph.GetN()):
#         theColor = int(((graph.GetZ()[i] - zmin) / (zmax - zmin)) * (ncols - 1))
#         markers.SetMarkerColor(ROOT.gStyle.GetColorPalette(theColor))
#         markers.DrawPolyMarker(1, array('d', [graph.GetX()[i]]), array('d', [graph.GetY()[i]]))


#     jetcols = [1, 2, 4, 6, 7, 8, 9, 28, 46]
#     jetcol = 0
#     for i, jet_graph in enumerate(jet_graphs):
#         theColor = jetcols[jetcol]
#         jetcol += 1
#         if jetcol == len(jetcols):
#             jetcol = 0
#         for j in xrange(jet_graph.GetN()):
#             jmarkers.SetMarkerColor(theColor)
#             jmarkers.DrawPolyMarker(1, array('d', [jet_graph.GetX()[j]]), array('d', [jet_graph.GetY()[j]]))


#     print 'Pions: %i' % pions.GetN()
#     for i in xrange(pions.GetN()):
#         # theColor = int(((graph.GetZ()[i] - zmin) / (zmax - zmin)) * (ncols - 1))
#         markers.SetMarkerStyle(25)
#         markers.SetMarkerSize(1)
#         markers.SetMarkerColor(ROOT.kRed)
#         x = ROOT.Double()
#         y = ROOT.Double()
#         z = ROOT.Double()
#         pions.GetPoint(i, x, y, z)
#         markers.DrawPolyMarker(1, array('d', [x]), array('d', [y]))
#         # latex.DrawLatex(x+1, y+1, 'E = %.1f GeV' % float(infoStr[i+1]))

#     print 'Photons: %i' % photons.GetN()
#     for i in xrange(photons.GetN()):
#         # theColor = int(((graph.GetZ()[i] - zmin) / (zmax - zmin)) * (ncols - 1))
#         markers.SetMarkerStyle(25)
#         markers.SetMarkerSize(1)
#         markers.SetMarkerColor(ROOT.kBlue)
#         x = ROOT.Double()
#         y = ROOT.Double()
#         z = ROOT.Double()
#         photons.GetPoint(i, x, y, z)
#         markers.DrawPolyMarker(1, array('d', [x]), array('d', [y]))
#         latex.SetTextColor(ROOT.kBlue)
#         # latex.DrawLatex(x+1, y+1, 'E = %.1f GeV' % float(infoStr[i+1+pions.GetN()]))

#     # plot.DrawTitle(pads[0], 'layer = %i' % layer, 1)
#     # plot.DrawTitle(pads[0], infoStr[0], 3)
#     # time.sleep(2)
#     # graph.Draw('pcolz')
#     # pads[0].Update()

#     # pads[0].GetView()
#     # pads[0].GetView().TopView()

#     # axis = plot.GetAxisHist(pads[0])
#     # pads[0].GetListOfPrimitives().Print()
#     # Get a top-down view
#     # canv.Print('.png')
#     # canv.Print('.pdf')
#     fout = 'evt_%i_%s_signifs' % (event, region)
#     # canv.Print('%s.png' % fout)
#     canv.Print('%s.pdf' % (fout))

