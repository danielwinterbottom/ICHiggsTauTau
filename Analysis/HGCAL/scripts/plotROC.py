import argparse
import ROOT
from numpy import mean
from array import array
import CombineHarvester.CombineTools.plotting as plot

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


file1 = 'output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_pusub.root'
file2 = 'output/Main/Dijet_agilbert_DiJetFlat20_200_PU140_20171102_pusub.root'
        # canv = ROOT.TCanvas('2d_pt_response_jets', '')
f1 = ROOT.TFile(file1)
f2 = ROOT.TFile(file2)
t1 = f1.Get('taus')
t2 = f2.Get('taus')


real_den = 'm1_matched && abs(gen_eta) > 1.479 && abs(gen_eta) < 3 && gen_vis_pt > 30 && gen_vis_pt < 60'
fake_den = 'm1_matched && abs(gen_eta) > 1.479 && abs(gen_eta) < 3 && gen_pt > 30 && gen_pt < 60'

real_num = 'rec_nprongs >= 1 && rec_nprongs <= 4 && rec_all_prong_mass < 2.0 && (rec_pt_0p2_0p4/rec_pt_0p0_0p2) < 0.15 && (rec_pt_0p4_0p6+rec_pt_0p6_0p8) < 0.5'
fake_num = 'rec_nprongs >= 1 && rec_nprongs <= 4 && rec_all_prong_mass < 2.0 && (rec_pt_0p2_0p4/rec_pt_0p0_0p2) < 0.15 && (rec_pt_0p4_0p6+rec_pt_0p6_0p8) < 0.5'

t1.Draw("1>>h1_den(1,0,2)", "%s" % real_den)
h1_den = ROOT.gDirectory.Get('h1_den')

t2.Draw("1>>h2_den(1,0,2)", "%s" % fake_den)
h2_den = ROOT.gDirectory.Get('h2_den')

t1.Draw("1>>h1_num(1,0,2)", "(%s) && (%s)" % (real_den, real_num))
h1_num = ROOT.gDirectory.Get('h1_num')

t2.Draw("1>>h2_num(1,0,2)", "(%s) && (%s)" % (fake_den, fake_num))
h2_num = ROOT.gDirectory.Get('h2_num')


print 'Efficieny of (1): %f' % (h1_num.Integral()/h1_den.Integral())
print 'Efficieny of (2): %f' % (h2_num.Integral()/h2_den.Integral())


# h2 = f2.Get('jet_energy_cont_profile')


    # h1.DrawNormalized("COLZ")
    # axis = plot.GetAxisHist(pads[0])

    # latex = ROOT.TLatex()
    # latex.SetNDC()
    # latex.SetTextSize(0.04)
    # if x == 0:
    #     latex.DrawLatex(0.5, 0.85, '30 < p_{T}^{gen. tau} < 60 GeV')
    # if x == 1:
    #     latex.DrawLatex(0.5, 0.85, '30 < p_{T}^{gen. jet} < 60 GeV')


    # canv.Print('.png')
    # canv.Print('.pdf')


