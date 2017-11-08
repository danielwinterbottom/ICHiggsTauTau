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
# parser.add_argument('--file', '-f', default='output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_pusub_0.root')
# parser.add_argument('--event', '-e', default=1, type=int)
# parser.add_argument('--region', '-r', default='p')
# parser.add_argument('--window', default=0.1, type=float)

# parser.add_argument('--bkg-model', default='Exponential')
# parser.add_argument('--title', default='Muon ID Efficiency')
# parser.add_argument('--postfix', default='')
# parser.add_argument('--plot-dir', '-p', default='./')
# parser.add_argument('--bin-replace', default=None) #(100,2.3,80,2.3)
args = parser.parse_args()


plot.ModTDRStyle(width=700, r=0.16)

for x in [0, 1]:
    if x == 0:
        file1 = 'output/Test/Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928.root'
        canv = ROOT.TCanvas('2d_pt_response_taus', '')
    if x == 1:
        file1 = 'output/Test/Dijet_agilbert_DiJetFlat20_200_20171015.root'
        canv = ROOT.TCanvas('2d_pt_response_jets', '')

    pads = plot.OnePad()
    pads[0].cd()
    f1 = ROOT.TFile(file1)

# legend = ROOT.TLegend(0.55, 0.6, 0.95, 0.7, '', 'NBNDC')

    t1 = f1.Get('taus')

    if x == 0:
        t1.Draw("rec_jet_pt_r0p4/gen_vis_pt:rec_jet_pt/gen_vis_pt>>h1(40,0,2,40,0,2)", "m1_matched && abs(gen_eta) > 1.479 && abs(gen_eta) < 3 && gen_vis_pt > 30 && gen_vis_pt < 60", "COLZ")
        h1 = ROOT.gDirectory.Get('h1')
        h1.GetYaxis().SetTitle("Reco. p_{T} (#DeltaR=0.4) / Gen. p_{T} (#tau_{vis})")
        h1.GetXaxis().SetTitle("Reco. p_{T} (#DeltaR=0.2) / Gen. p_{T} (#tau_{vis})")

    if x == 1:
        t1.Draw("rec_jet_pt_r0p4/gen_pt_r0p4:rec_jet_pt/gen_pt_r0p4>>h1(40,0,2,40,0,2)", "m1_matched && abs(gen_eta) > 1.479 && abs(gen_eta) < 3 && gen_pt > 30 && gen_pt < 60", "COLZ")
        h1 = ROOT.gDirectory.Get('h1')
        h1.GetYaxis().SetTitle("Reco. p_{T} (#DeltaR=0.4) / Gen. p_{T} (#DeltaR=0.4)")
        h1.GetXaxis().SetTitle("Reco. p_{T} (#DeltaR=0.2) / Gen. p_{T} (#DeltaR=0.4)")



# h2 = f2.Get('jet_energy_cont_profile')


    h1.DrawNormalized("COLZ")
    axis = plot.GetAxisHist(pads[0])

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.04)
    if x == 0:
        latex.DrawLatex(0.5, 0.85, '30 < p_{T}^{gen. tau} < 60 GeV')
    if x == 1:
        latex.DrawLatex(0.5, 0.85, '30 < p_{T}^{gen. jet} < 60 GeV')


    canv.Print('.png')
    canv.Print('.pdf')


