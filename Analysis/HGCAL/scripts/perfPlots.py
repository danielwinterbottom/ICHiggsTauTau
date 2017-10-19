import ROOT
import argparse
from UserCode.ICHiggsTauTau.analysis import *
import CombineHarvester.CombineTools.plotting as plot
# from numpy import std

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

parser = argparse.ArgumentParser()

# parser.add_argument('-m', '--mass', default='500')
# parser.add_argument('-t', '--tanb', default='15')
# parser.add_argument('-H', '--higgs', default='H')
args = parser.parse_args()


ROOT.TH1.SetDefaultSumw2(True)

ana = Analysis()

path = 'output/Main'
sa = 'Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928_0'
ana.AddSamples('%s/%s.root' % (path, sa), 'taus')

# X = '%s_%s_%s' % (args.higgs, args.mass, args.tanb)

varlist = [
    (
      'rec_jet_pt',
      'rec_jet_pt(50,0,200)', 'm1_matched',
      ('Reco. jet p_{T}(GeV)', 'Events')
    ),
    (
      'gen_pt',
      'gen_pt(50,0,200)', '1',
      ('Gen. #tau p_{T}(GeV)', 'Events')
    ),
    (
      'gen_vis_pt',
      'gen_vis_pt(50,0,200)', '1',
      ('Gen. #tau_{h} p_{T}(GeV)', 'Events')
    ),
    (
      'rec_jet_eta',
      'rec_jet_eta(50,-5,5)', 'm1_matched',
      ('Reco. jet #eta', 'Events')
    ),
    (
      'gen_eta',
      'gen_eta(50,-5,5)', '1',
      ('Gen. #tau #eta', 'Events')
    ),
    (
      'gen_vis_eta',
      'gen_vis_eta(50,-5,5)', '1',
      ('Gen. #tau_{h} #eta', 'Events')
    ),
    (
      'm1_pi_reached_ee',
      'm1_pi_reached_ee(2,-0.5,1.5)', '1',
      ('#pi^{+} reached EE', 'Events')
    ),
    (
      'm1_all_reached_ee',
      'm1_all_reached_ee(2,-0.5,1.5)', '1',
      ('#pi^{+} + 2#gamma reached EE', 'Events')
    ),
    (
      'm1_pi_e',
      'm1_pi_e(20,0,400)', '1',
      ('#pi^{+} Energy (GeV)', 'Events')
    ),
    (
      'm1_p1_e',
      'm1_p1_e(20,0,400)', '1',
      ('#gamma_{1} Energy (GeV)', 'Events')
    ),
    (
      'm1_p2_e',
      'm1_p2_e(20,0,400)', '1',
      ('#gamma_{2} Energy (GeV)', 'Events')
    ),
    (
      'rec_nprongs_pi_reached',
      'rec_nprongs(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
      ('Reco. nProngs', 'a.u.')
    ),
    (
      'rec_nprongs_all_reached',
      'rec_nprongs(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
      ('Reco. nProngs', 'a.u.')
    ),
    (
      'rec_all_prong_mass',
      'rec_all_prong_mass(50,0,5)', 'm1_matched && m1_pi_reached_ee && rec_nprongs <= 3 && rec_nprongs >= 2',
      ('#tau_{h} Mass (GeV)', 'Events')
    ),
    (
      'm1_mass',
      'm1_mass(50,0,5)', 'm1_matched && m1_pi_reached_ee && rec_nprongs <= 3 && rec_nprongs >= 2',
      ('#tau_{h} Mass (GeV)', 'Events')
    ),
    (
      'pi_over_p1_he',
      'm1_pi_bestmatch_hfrac/m1_p1_bestmatch_hfrac(50,0,5)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0 && m1_nmatched_to_p1 > 0',
      ('#pi^{+} / #gamma_{1} ratio for H/E', 'Events')
    ),
    (
      'm1_nmatched_to_pi',
      'm1_nmatched_to_pi(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
      ('nProngs matched to #pi^{+}', 'Events')
    ),
    (
      'm1_nmatched_to_p1',
      'm1_nmatched_to_p1(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
      ('nProngs matched to #gamma_{1}', 'Events')
    ),
    (
      'm1_nmatched_to_p2',
      'm1_nmatched_to_p2(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
      ('nProngs matched to #gamma_{2}', 'Events')
    ),
    (
      'm1_pi_bestmatch_dr',
      'm1_pi_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0',
      ('#DeltaR to best #pi^{+} match', 'Events')
    ),
    (
      'm1_p1_bestmatch_dr',
      'm1_p1_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p1 > 0',
      ('#DeltaR to best #gamma_{1} match', 'Events')
    ),
    (
      'm1_p2_bestmatch_dr',
      'm1_p2_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p2 > 0',
      ('#DeltaR to best #gamma_{2} match', 'Events')
    ),


    (
      'm1_pi_bestmatch_hfrac',
      'm1_pi_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0',
      ('E_{H} fraction for #pi^{+} match', 'Events')
    ),
    (
      'm1_p1_bestmatch_hfrac',
      'm1_p1_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p1 > 0',
      ('E_{H} fraction for #gamma_{1} match', 'Events')
    ),
    (
      'm1_p2_bestmatch_hfrac',
      'm1_p2_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p2 > 0',
      ('E_{H} fraction for #gamma_{2} match', 'Events')
    ),


    (
      'm1_nmatched_to_pi_all',
      'm1_nmatched_to_pi(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
      ('nProngs matched to #pi^{+}', 'Events')
    ),
    (
      'm1_nmatched_to_p1_all',
      'm1_nmatched_to_p1(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
      ('nProngs matched to #gamma_{1}', 'Events')
    ),
    (
      'm1_nmatched_to_p2_all',
      'm1_nmatched_to_p2(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
      ('nProngs matched to #gamma_{2}', 'Events')
    ),
    (
      'm1_pi_bestmatch_dr_all',
      'm1_pi_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
      ('#DeltaR to best #pi^{+} match', 'Events')
    ),
    (
      'm1_p1_bestmatch_dr_all',
      'm1_p1_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
      ('#DeltaR to best #gamma_{1} match', 'Events')
    ),
    (
      'm1_p2_bestmatch_dr_all',
      'm1_p2_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
      ('#DeltaR to best #gamma_{2} match', 'Events')
    ),

    (
      'm1_pi_bestmatch_hfrac_all',
      'm1_pi_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
      ('E_{H} fraction for #pi^{+} match', 'Events')
    ),
    (
      'm1_p1_bestmatch_hfrac_all',
      'm1_p1_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
      ('E_{H} fraction for #gamma_{1} match', 'Events')
    ),
    (
      'm1_p2_bestmatch_hfrac_all',
      'm1_p2_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
      ('E_{H} fraction for #gamma_{2} match', 'Events')
    ),

    (
      'pi_energy_resp',
      'm1_pi_bestmatch_e/m1_pi_e(30,0,3)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0',
      ('#pi^{+} Energy response', 'Events')
    ),
    (
      'p1_energy_resp',
      'm1_p1_bestmatch_e/m1_p1_e(30,0,3)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p1 > 0',
      ('#gamma_{1} Energy response', 'Events')
    ),
    (
      'p2_energy_resp',
      'm1_p2_bestmatch_e/m1_p2_e(30,0,3)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p2 > 0',
      ('#gamma_{2} Energy response', 'Events')
    ),
    (
      'pi_over_p1_he',
      'm1_pi_bestmatch_hfrac/m1_p1_bestmatch_hfrac(50,0,5)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0 && m1_nmatched_to_p1 > 0',
      ('#pi^{+} / #gamma_{1} ratio for H/E', 'Events')
    ),
  ]

for name, var, sel, axes in varlist:
    ana.nodes.AddNode(ListNode(name))

    ana.nodes[name].AddNode(ana.BasicFactory(sa, sa, var, sel, scaleToLumi=False))
    # ana.nodes[name].AddNode(ana.BasicFactory('pythia_check', 'SUSYGluGluToHToTauTau_M-%s' % args.mass, var, 'wt*wt_2HDM_%s' % hdict[args.higgs], scaleToLumi=False))
    # ana.nodes[name].AddNode(ana.BasicFactory('t_t', '%s_t_t' % X, var, 'wt', scaleToLumi=False))
    # ana.nodes[name].AddNode(ana.BasicFactory('b_b', '%s_b_b' % X, var, 'wt', scaleToLumi=False))
    # ana.nodes[name].AddNode(ana.BasicFactory('tb_tb', '%s_tb_tb' % X, var, 'wt', scaleToLumi=False))
    # ana.nodes[name].AddNode(ana.BasicFactory('t_tb', '%s_t_tb' % X, var, 'wt', scaleToLumi=False))
    # ana.nodes[name].AddNode(ana.BasicFactory('b_tb', '%s_b_tb' % X, var, 'wt', scaleToLumi=False))


ana.Run()


for name, var, sel, axes in varlist:
    # for h in ana.nodes[name].SubNodes():
        # h.shape.hist.Scale(1. / h.shape.hist.Integral())
        # if h.name in ['t_t', 'b_b', 'tb_tb', 't_tb', 'b_tb']:
        #     h.shape.hist.Scale(xsec['%s_%s' % (X, h.name)][0])
        # if h.name in ['pythia', 'pythia_check']:
        #     h.shape.hist.Scale(total_xs)
        # h.shape.hist.Scale(1., 'width')

    canv = ROOT.TCanvas('output_%s' % (name), 'output_%s' % (var))
    pads = plot.OnePad()
    legend = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')


    plot.Set(ana.nodes[name][sa].shape.hist, LineColor=2, LineWidth=2)




    # legend.AddEntry(h_t_t, 'Pure top (Qt)', 'L')
    # legend.AddEntry(h_b_b, 'Pure bottom (Qb)', 'L')
    # legend.AddEntry(h_int, 'Interference (Qtb)', 'L')
    # legend.AddEntry(h_tot, 'Powheg total', 'L')

    ana.nodes[name][sa].shape.hist.Draw('SAMEHIST')

    if name == 'm1_mass':
        plot.Set(ana.nodes['rec_all_prong_mass'][sa].shape.hist, LineColor=4, LineWidth=2)
        legend.AddEntry(ana.nodes['m1_mass'][sa].shape.hist, 'Gen. level', 'L')
        legend.AddEntry(ana.nodes['rec_all_prong_mass'][sa].shape.hist, 'Reco. level', 'L')
        ana.nodes['rec_all_prong_mass'][sa].shape.hist.Draw('SAMEHIST')

    if 'rec_nprongs' in name:
        ana.nodes[name][sa].shape.hist.Scale(1. / ana.nodes[name][sa].shape.hist.Integral())

    # h_t_t.Draw('SAMEHIST')
    # h_b_b.Draw('SAMEHIST')
    # h_int.Draw('SAMEHIST')
    # h_tot.Draw('SAMEHIST')
    # h_pythia_check.Draw('SAMEHIST')

    # min_val = 0.
    # if int_xs < 0.:
    #     min_val = h_int.GetMinimum()

    plot.GetAxisHist(pads[0]).SetMinimum(0)
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.30)
    legend.Draw()

    axis = plot.GetAxisHist(pads[0])
    plot.Set(axis.GetXaxis(), Title=axes[0])
    plot.Set(axis.GetYaxis(), Title=axes[1])

    # plot.DrawTitle(pads[0], 'Boson: %s, ^{}m_{%s}=%s GeV, tan#beta=%s' % (partx, partx, mA, tanb), 3)
    canv.Print('.pdf')
    canv.Print('.png')


outfile = ROOT.TFile('output_test.root', 'RECREATE')

ana.nodes.Output(outfile)

outfile.Close()
