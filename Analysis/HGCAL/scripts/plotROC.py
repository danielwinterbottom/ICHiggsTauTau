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
parser.add_argument('--eta-range', default='1.479,3.0')
# parser.add_argument('--region', '-r', default='p')
# parser.add_argument('--window', default=0.1, type=float)

# parser.add_argument('--bkg-model', default='Exponential')
# parser.add_argument('--title', default='Muon ID Efficiency')
# parser.add_argument('--postfix', default='')
# parser.add_argument('--plot-dir', '-p', default='./')
# parser.add_argument('--bin-replace', default=None) #(100,2.3,80,2.3)
args = parser.parse_args()


plot.ModTDRStyle(width=700)

def GetEffAndFR(t1, t2, den1, den2, num1, num2):
    t1.Draw("1>>h1_den(1,0,2)", "%s" % den1)
    h1_den = ROOT.gDirectory.Get('h1_den')

    t2.Draw("1>>h2_den(1,0,2)", "%s" % den2)
    h2_den = ROOT.gDirectory.Get('h2_den')

    t1.Draw("1>>h1_num(1,0,2)", "(%s) && (%s)" % (den1, num1))
    h1_num = ROOT.gDirectory.Get('h1_num')

    t2.Draw("1>>h2_num(1,0,2)", "(%s) && (%s)" % (den2, num2))
    h2_num = ROOT.gDirectory.Get('h2_num')

    gr1 = ROOT.TGraphAsymmErrors(h1_num, h1_den)
    gr2 = ROOT.TGraphAsymmErrors(h2_num, h2_den)

    print 'Efficieny of (1): %f +/- %f' % (gr1.GetY()[0], gr1.GetErrorY(0))
    print 'Efficieny of (2): %f +/- %f' % (gr2.GetY()[0], gr2.GetErrorY(0))

    return (gr1.GetY()[0], gr1.GetErrorY(0), gr2.GetY()[0], gr2.GetErrorY(0))


def Scan(t1, t2, den1, den2, num1, num2, start, stop, step):
    lx = []
    ly = []
    lxe = []
    lye = []
    current = start
    while current <= stop:
        snum1 = num1 % {'SCAN': current}
        snum2 = num2 % {'SCAN': current}
        # print snum1, snum2
        results = GetEffAndFR(t1, t2, den1, den2, snum1, snum2)
        lx.append(results[0])
        ly.append(results[2])
        lxe.append(results[1])
        lye.append(results[3])
        current += step
    roc = ROOT.TGraphAsymmErrors(len(lx), array('d', lx), array('d', ly), array('d', lxe), array('d', lxe), array('d', lye), array('d', lye))
    return roc


min_eta = float(args.eta_range.split(',')[0])
max_eta = float(args.eta_range.split(',')[1])

real_den = 'm1_matched && abs(gen_eta) > %g && abs(gen_eta) < %g && gen_vis_pt > 30 && gen_vis_pt < 60' % (min_eta, max_eta)
fake_den = 'm1_matched && abs(gen_eta) > %g && abs(gen_eta) < %g && gen_pt > 30 && gen_pt < 60' % (min_eta, max_eta)




file1 = 'output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_pusub.root'
file2 = 'output/Main/Dijet_agilbert_DiJetFlat20_200_PU140_20171102_pusub.root'
# file1 = 'output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928.root'
# file2 = 'output/Main/Dijet_agilbert_DiJetFlat20_200_20171015.root'
f1 = ROOT.TFile(file1)
f2 = ROOT.TFile(file2)
t1 = f1.Get('taus')
t2 = f2.Get('taus')


real_num = 'rec_nprongs >= 1 && rec_nprongs <= 5 && rec_all_prong_mass < 2.8 && ((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
fake_num = 'rec_nprongs >= 1 && rec_nprongs <= 5 && rec_all_prong_mass < 2.8 && ((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
roc_curve_1 = Scan(t1, t2, real_den, fake_den, real_num, fake_num, 0.01, 0.5, 0.04)


real_num = '((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
fake_num = '((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
roc_curve_2 = Scan(t1, t2, real_den, fake_den, real_num, fake_num, 0.01, 0.5, 0.04)
plot.Set(roc_curve_2, LineColor=4, LineStyle=2, MarkerColor=4, LineWidth=2)


file1 = 'output/Main/Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928.root'
file2 = 'output/Main/Dijet_agilbert_DiJetFlat20_200_20171015.root'
f1 = ROOT.TFile(file1)
f2 = ROOT.TFile(file2)
t1 = f1.Get('taus')
t2 = f2.Get('taus')


real_num = 'rec_nprongs >= 1 && rec_nprongs <= 5 && rec_all_prong_mass < 2.8 && ((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
fake_num = 'rec_nprongs >= 1 && rec_nprongs <= 5 && rec_all_prong_mass < 2.8 && ((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
roc_curve_3 = Scan(t1, t2, real_den, fake_den, real_num, fake_num, 0.01, 0.5, 0.04)
roc_curve_3.SetLineColor(4)


real_num = '((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
fake_num = '((rec_pt_0p2_0p4+rec_pt_0p4_0p6+rec_pt_0p6_0p8)/rec_pt_0p0_0p2) < %(SCAN)f'
roc_curve_4 = Scan(t1, t2, real_den, fake_den, real_num, fake_num, 0.01, 0.5, 0.04)
plot.Set(roc_curve_4, LineColor=4, LineStyle=1, MarkerColor=4, LineWidth=2)



canv = ROOT.TCanvas('roc_curve', '')
pads = plot.OnePad()
pads[0].cd()
# pads[0].SetLogy(True)


roc_curve_2.Draw('APL')
axis = plot.GetAxisHist(pads[0])
axis.GetXaxis().SetLimits(0.5, 1)
axis.SetMinimum(0)
axis.SetMaximum(0.4)
plot.Set(axis.GetXaxis(), Title='#tau_{h} ID efficiency')
plot.Set(axis.GetYaxis(), Title='Jet#rightarrow#tau fake rate')
pads[0].Update()

roc_curve_2.Draw('APL')
roc_curve_4.Draw('PLSAME')

legend = ROOT.TLegend(0.2, 0.75, 0.45, 0.93, '', 'NBNDC')
legend.AddEntry(roc_curve_4, 'PU = 0', 'L')
legend.AddEntry(roc_curve_2, 'PU = 140', 'L')
legend.Draw()


latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextSize(0.03)

text = [
    '30 < p_{T}^{gen} < 60 GeV',
    '%g < |#eta^{gen}| < %g' % (min_eta, max_eta),
]

for i, line in enumerate(text):
    latex.DrawLatex(0.22, 0.6 - i * 0.04, line)

canv.Print('.png')
canv.Print('.pdf')


