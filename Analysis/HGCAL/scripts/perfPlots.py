import ROOT
import argparse
from UserCode.ICHiggsTauTau.analysis import *
import CombineHarvester.CombineTools.plotting as plot

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

parser = argparse.ArgumentParser()

args = parser.parse_args()
ROOT.TH1.SetDefaultSumw2(True)


def MakePlot(c):
    canv = ROOT.TCanvas('output_%s' % (c['name']), 'output_%s' % (c['name']))
    pads = plot.OnePad()
    legend = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')

    hists = []
    for entry in c['inputs']:
        hists.append(ana.nodes[c['name']][entry['name']].shape.hist)
        plot.Set(hists[-1], LineColor=entry['col'], LineWidth=2)
        legend.AddEntry(hists[-1], entry['leg'], 'L')

    for hist in hists:
        if c['scaled']:
            hist.Scale(1. / hist.Integral())
        hist.Draw('SAMEHIST')

    plot.GetAxisHist(pads[0]).SetMinimum(0)
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.30)
    legend.Draw()

    axis = plot.GetAxisHist(pads[0])
    plot.Set(axis.GetXaxis(), Title=c['xtitle'])
    plot.Set(axis.GetYaxis(), Title=c['ytitle'])

    # plot.DrawTitle(pads[0], 'Boson: %s, ^{}m_{%s}=%s GeV, tan#beta=%s' % (partx, partx, mA, tanb), 3)
    canv.Print('.pdf')
    canv.Print('.png')

ana = Analysis()

path = 'output/Main'
sa = {
    'tau_0pu': 'Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013',
    'jet_0pu': 'Dijet_agilbert_DiJetFlat20_200_20171015'
}
labels = {
    'tau_0pu': '#tau_{h}#rightarrow#pi^{+}#pi^{0} (0 PU)',
    'jet_0pu': 'Jet#rightarrow#tau_{h} fakes (0 PU)'
}
for name, s in sa.iteritems():
    ana.AddSamples('%s/%s.root' % (path, s), 'taus')

cols = {
    'tau_0pu': 2,
    'jet_0pu': 4
}

jet_sel = '(m1_matched && abs(gen_eta) > 1.479 && abs(gen_eta) < 3 && rec_jet_pt > 20)'

plotlist = [
{
    'name': 'rec_jet_pt', 'scaled': False,
    'inputs': [
        {'name': 'tau_0pu', 'sa': sa['tau_0pu'], 'var': 'rec_jet_pt(50,0,200)', 'sel': 'm1_matched', 'leg': labels['tau_0pu'], 'col': cols['tau_0pu']},
    ],
    'xtitle': 'Reco. jet p_{T}(GeV)',
    'ytitle': 'Events'
},
{
    'name': 'fake_gen_pt', 'scaled': False,
    'inputs': [
        {'name': 'jet_0pu', 'sa': sa['jet_0pu'], 'var': 'gen_pt(50,0,200)', 'sel': '1', 'leg': labels['jet_0pu'], 'col': cols['jet_0pu']},
    ],
    'xtitle': 'Gen jet p_{T}(GeV)',
    'ytitle': 'Events'
},
{
    'name': 'fake_gen_eta', 'scaled': False,
    'inputs': [
        {'name': 'jet_0pu', 'sa': sa['jet_0pu'], 'var': 'gen_eta(50,-5,5)', 'sel': '1', 'leg': labels['jet_0pu'], 'col': cols['jet_0pu']},
    ],
    'xtitle': 'Gen jet #eta',
    'ytitle': 'Events'
},
{
    'name': 'rec_nprongs_pi_reached', 'scaled': True,
    'inputs': [
        {'name': 'tau_0pu', 'sa': sa['tau_0pu'], 'var': 'rec_nprongs(15,-0.5,14.5)', 'sel': 'm1_matched && m1_pi_reached_ee', 'leg': labels['tau_0pu'], 'col': cols['tau_0pu']},
        {'name': 'jet_0pu', 'sa': sa['jet_0pu'], 'var': 'rec_nprongs(15,-0.5,14.5)', 'sel': '%s' % jet_sel, 'leg': labels['jet_0pu'], 'col': cols['jet_0pu']},
    ],
    'xtitle': 'Reco. nProngs',
    'ytitle': 'a.u.'
},
{
    'name': 'rec_nprongs_all_reached', 'scaled': True,
    'inputs': [
        {'name': 'tau_0pu', 'sa': sa['tau_0pu'], 'var': 'rec_nprongs(15,-0.5,14.5)', 'sel': 'm1_matched && m1_all_reached_ee', 'leg': labels['tau_0pu'], 'col': cols['tau_0pu']},
        {'name': 'jet_0pu', 'sa': sa['jet_0pu'], 'var': 'rec_nprongs(15,-0.5,14.5)', 'sel': '%s' % jet_sel, 'leg': labels['jet_0pu'], 'col': cols['jet_0pu']},
    ],
    'xtitle': 'Reco. nProngs',
    'ytitle': 'a.u.'
},
{
    'name': 'rec_all_prong_mass', 'scaled': True,
    'inputs': [
        {'name': 'tau_0pu', 'sa': sa['tau_0pu'], 'var': 'rec_all_prong_mass(20,0,6)', 'sel': 'm1_matched && m1_pi_reached_ee && rec_nprongs <= 3 && rec_nprongs >= 2', 'leg': labels['tau_0pu'], 'col': 4},
        {'name': 'jet_0pu', 'sa': sa['jet_0pu'], 'var': 'rec_all_prong_mass(20,0,6)', 'sel': '%s  && rec_nprongs <= 3 && rec_nprongs >= 2' % jet_sel, 'leg': labels['jet_0pu'], 'col': 8},
    ],
    'xtitle': '#tau_{h} Mass (GeV)',
    'ytitle': 'a.u.'
},
{
    'name': 'm1_pi_bestmatch_hfrac_all', 'scaled': True,
    'inputs': [
        {'name': 'tau_0pu_rec', 'sa': sa['tau_0pu'], 'var': 'm1_pi_bestmatch_hfrac(30,0,1)', 'sel': 'm1_matched && m1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0', 'leg': 'Reco level', 'col': 2},
        {'name': 'tau_0pu_gen', 'sa': sa['tau_0pu'], 'var': 'm1_pi_hfrac(30,0,1)', 'sel': 'm1_matched && m1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0', 'leg': 'Gen level', 'col': 8},
    ],
    'xtitle': 'E_{H} fraction for #pi^{+} match',
    'ytitle': 'a.u.'
},
{
    'name': 'm1_p1_bestmatch_hfrac_all', 'scaled': True,
    'inputs': [
        {'name': 'tau_0pu_rec', 'sa': sa['tau_0pu'], 'var': 'm1_p1_bestmatch_hfrac(30,0,1)', 'sel': 'm1_matched && m1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0', 'leg': 'Reco level', 'col': 2},
        {'name': 'tau_0pu_gen', 'sa': sa['tau_0pu'], 'var': 'm1_p1_hfrac(30,0,1)', 'sel': 'm1_matched && m1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0', 'leg': 'Gen level', 'col': 8},
    ],
    'xtitle': 'E_{H} fraction for #gamma_{1} match',
    'ytitle': 'a.u.'
},
{
    'name': 'm1_p2_bestmatch_hfrac_all', 'scaled': True,
    'inputs': [
        {'name': 'tau_0pu_rec', 'sa': sa['tau_0pu'], 'var': 'm1_p2_bestmatch_hfrac(30,0,1)', 'sel': 'm1_matched && m1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0', 'leg': 'Reco level', 'col': 2},
        {'name': 'tau_0pu_gen', 'sa': sa['tau_0pu'], 'var': 'm1_p2_hfrac(30,0,1)', 'sel': 'm1_matched && m1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0', 'leg': 'Gen level', 'col': 8},
    ],
    'xtitle': 'E_{H} fraction for #gamma_{1} match',
    'ytitle': 'a.u.'
}
]


#     (
#       'm1_pi_bestmatch_hfrac_all',
#       'm1_pi_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
#       ('E_{H} fraction for #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_p1_bestmatch_hfrac_all',
#       'm1_p1_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
#       ('E_{H} fraction for #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p2_bestmatch_hfrac_all',
#       'm1_p2_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
#       ('E_{H} fraction for #gamma_{2} match', 'Events')
#     ),

for c in plotlist:
    ana.nodes.AddNode(ListNode(c['name']))
    for p in c['inputs']:
        ana.nodes[c['name']].AddNode(ana.BasicFactory(p['name'], p['sa'], p['var'], p['sel'], scaleToLumi=False))

ana.Run()


for p in plotlist:
    MakePlot(p)

# varlist = [
#     (
#       'rec_jet_pt',
#       'rec_jet_pt(50,0,200)', 'm1_matched',
#       ('Reco. jet p_{T}(GeV)', 'Events')
#     ),
#     (
#       'gen_pt',
#       'gen_pt(50,0,200)', '1',
#       ('Gen. #tau p_{T}(GeV)', 'Events')
#     ),
#     (
#       'gen_vis_pt',
#       'gen_vis_pt(50,0,200)', '1',
#       ('Gen. #tau_{h} p_{T}(GeV)', 'Events')
#     ),
#     (
#       'rec_jet_eta',
#       'rec_jet_eta(50,-5,5)', 'm1_matched',
#       ('Reco. jet #eta', 'Events')
#     ),
#     (
#       'gen_eta',
#       'gen_eta(50,-5,5)', '1',
#       ('Gen. #tau #eta', 'Events')
#     ),
#     (
#       'gen_vis_eta',
#       'gen_vis_eta(50,-5,5)', '1',
#       ('Gen. #tau_{h} #eta', 'Events')
#     ),
#     (
#       'm1_pi_reached_ee',
#       'm1_pi_reached_ee(2,-0.5,1.5)', '1',
#       ('#pi^{+} reached EE', 'Events')
#     ),
#     (
#       'm1_all_reached_ee',
#       'm1_all_reached_ee(2,-0.5,1.5)', '1',
#       ('#pi^{+} + 2#gamma reached EE', 'Events')
#     ),
#     (
#       'm1_pi_e',
#       'm1_pi_e(20,0,400)', '1',
#       ('#pi^{+} Energy (GeV)', 'Events')
#     ),
#     (
#       'm1_p1_e',
#       'm1_p1_e(20,0,400)', '1',
#       ('#gamma_{1} Energy (GeV)', 'Events')
#     ),
#     (
#       'm1_p2_e',
#       'm1_p2_e(20,0,400)', '1',
#       ('#gamma_{2} Energy (GeV)', 'Events')
#     ),
#     (
#       'rec_nprongs_pi_reached',
#       'rec_nprongs(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
#       ('Reco. nProngs', 'a.u.')
#     ),
#     (
#       'rec_nprongs_all_reached',
#       'rec_nprongs(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
#       ('Reco. nProngs', 'a.u.')
#     ),
#     (
#       'rec_all_prong_mass',
#       'rec_all_prong_mass(50,0,5)', 'm1_matched && m1_pi_reached_ee && rec_nprongs <= 3 && rec_nprongs >= 2',
#       ('#tau_{h} Mass (GeV)', 'Events')
#     ),
#     (
#       'm1_mass',
#       'm1_mass(50,0,5)', 'm1_matched && m1_pi_reached_ee && rec_nprongs <= 3 && rec_nprongs >= 2',
#       ('#tau_{h} Mass (GeV)', 'Events')
#     ),
#     (
#       'pi_over_p1_he',
#       'm1_pi_bestmatch_hfrac/m1_p1_bestmatch_hfrac(50,0,5)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0 && m1_nmatched_to_p1 > 0',
#       ('#pi^{+} / #gamma_{1} ratio for H/E', 'Events')
#     ),
#     (
#       'm1_nmatched_to_pi',
#       'm1_nmatched_to_pi(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
#       ('nProngs matched to #pi^{+}', 'Events')
#     ),
#     (
#       'm1_nmatched_to_p1',
#       'm1_nmatched_to_p1(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
#       ('nProngs matched to #gamma_{1}', 'Events')
#     ),
#     (
#       'm1_nmatched_to_p2',
#       'm1_nmatched_to_p2(10,-0.5,9.5)', 'm1_matched && m1_pi_reached_ee',
#       ('nProngs matched to #gamma_{2}', 'Events')
#     ),
#     (
#       'm1_pi_bestmatch_dr',
#       'm1_pi_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0',
#       ('#DeltaR to best #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_p1_bestmatch_dr',
#       'm1_p1_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p1 > 0',
#       ('#DeltaR to best #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p2_bestmatch_dr',
#       'm1_p2_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p2 > 0',
#       ('#DeltaR to best #gamma_{2} match', 'Events')
#     ),


#     (
#       'm1_pi_bestmatch_hfrac',
#       'm1_pi_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0',
#       ('E_{H} fraction for #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_p1_bestmatch_hfrac',
#       'm1_p1_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p1 > 0',
#       ('E_{H} fraction for #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p2_bestmatch_hfrac',
#       'm1_p2_bestmatch_hfrac(30,0,1)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p2 > 0',
#       ('E_{H} fraction for #gamma_{2} match', 'Events')
#     ),


#     (
#       'm1_nmatched_to_pi_all',
#       'm1_nmatched_to_pi(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
#       ('nProngs matched to #pi^{+}', 'Events')
#     ),
#     (
#       'm1_nmatched_to_p1_all',
#       'm1_nmatched_to_p1(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
#       ('nProngs matched to #gamma_{1}', 'Events')
#     ),
#     (
#       'm1_nmatched_to_p2_all',
#       'm1_nmatched_to_p2(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee',
#       ('nProngs matched to #gamma_{2}', 'Events')
#     ),
#     (
#       'm1_pi_bestmatch_dr_all',
#       'm1_pi_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
#       ('#DeltaR to best #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_p1_bestmatch_dr_all',
#       'm1_p1_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
#       ('#DeltaR to best #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p2_bestmatch_dr_all',
#       'm1_p2_bestmatch_dr(30,0,0.2)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
#       ('#DeltaR to best #gamma_{2} match', 'Events')
#     ),



#     (
#       'm1_pi_hfrac_all',
#       'm1_pi_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
#       ('True E_{H} fraction for #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_p1_hfrac_all',
#       'm1_p1_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
#       ('True E_{H} fraction for #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p2_hfrac_all',
#       'm1_p2_hfrac(30,0,1)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
#       ('True E_{H} fraction for #gamma_{2} match', 'Events')
#     ),

#     (
#       'm1_pi_n_ecal_hits',
#       'm1_pi_n_ecal_hits(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
#       ('Number of clustered ECAL hits for #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_pi_n_hcal_hits',
#       'm1_pi_n_hcal_hits(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_pi > 0',
#       ('Number of clustered HCAL hits for #pi^{+} match', 'Events')
#     ),
#     (
#       'm1_p1_n_ecal_hits',
#       'm1_p1_n_ecal_hits(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
#       ('Number of clustered ECAL hits for #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p1_n_hcal_hits',
#       'm1_p1_n_hcal_hits(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p1 > 0',
#       ('Number of clustered HCAL hits for #gamma_{1} match', 'Events')
#     ),
#     (
#       'm1_p2_n_ecal_hits',
#       'm1_p2_n_ecal_hits(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
#       ('Number of clustered ECAL hits for #gamma_{2} match', 'Events')
#     ),
#     (
#       'm1_p2_n_hcal_hits',
#       'm1_p2_n_hcal_hits(10,-0.5,9.5)', 'm1_matched && m1_all_reached_ee && m1_nmatched_to_p2 > 0',
#       ('Number of clustered HCAL hits for #gamma_{2} match', 'Events')
#     ),

#     (
#       'pi_energy_resp',
#       'm1_pi_bestmatch_e/m1_pi_e(30,0,3)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0',
#       ('#pi^{+} Energy response', 'Events')
#     ),
#     (
#       'p1_energy_resp',
#       'm1_p1_bestmatch_e/m1_p1_e(30,0,3)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p1 > 0',
#       ('#gamma_{1} Energy response', 'Events')
#     ),
#     (
#       'p2_energy_resp',
#       'm1_p2_bestmatch_e/m1_p2_e(30,0,3)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_p2 > 0',
#       ('#gamma_{2} Energy response', 'Events')
#     ),
#     (
#       'pi_over_p1_he',
#       'm1_pi_bestmatch_hfrac/m1_p1_bestmatch_hfrac(50,0,5)', 'm1_matched && m1_pi_reached_ee && m1_nmatched_to_pi > 0 && m1_nmatched_to_p1 > 0',
#       ('#pi^{+} / #gamma_{1} ratio for H/E', 'Events')
#     ),
#   ]

# for name, var, sel, axes in varlist:
#     ana.nodes.AddNode(ListNode(name))

#     ana.nodes[name].AddNode(ana.BasicFactory(sa, sa, var, sel, scaleToLumi=False))
#     # ana.nodes[name].AddNode(ana.BasicFactory('pythia_check', 'SUSYGluGluToHToTauTau_M-%s' % args.mass, var, 'wt*wt_2HDM_%s' % hdict[args.higgs], scaleToLumi=False))
#     # ana.nodes[name].AddNode(ana.BasicFactory('t_t', '%s_t_t' % X, var, 'wt', scaleToLumi=False))
#     # ana.nodes[name].AddNode(ana.BasicFactory('b_b', '%s_b_b' % X, var, 'wt', scaleToLumi=False))
#     # ana.nodes[name].AddNode(ana.BasicFactory('tb_tb', '%s_tb_tb' % X, var, 'wt', scaleToLumi=False))
#     # ana.nodes[name].AddNode(ana.BasicFactory('t_tb', '%s_t_tb' % X, var, 'wt', scaleToLumi=False))
#     # ana.nodes[name].AddNode(ana.BasicFactory('b_tb', '%s_b_tb' % X, var, 'wt', scaleToLumi=False))




# for name, var, sel, axes in varlist:
#     # for h in ana.nodes[name].SubNodes():
#         # h.shape.hist.Scale(1. / h.shape.hist.Integral())
#         # if h.name in ['t_t', 'b_b', 'tb_tb', 't_tb', 'b_tb']:
#         #     h.shape.hist.Scale(xsec['%s_%s' % (X, h.name)][0])
#         # if h.name in ['pythia', 'pythia_check']:
#         #     h.shape.hist.Scale(total_xs)
#         # h.shape.hist.Scale(1., 'width')

#     canv = ROOT.TCanvas('output_%s' % (name), 'output_%s' % (var))
#     pads = plot.OnePad()
#     legend = ROOT.TLegend(0.55, 0.71, 0.95, 0.93, '', 'NBNDC')


#     plot.Set(ana.nodes[name][sa].shape.hist, LineColor=2, LineWidth=2)




#     # legend.AddEntry(h_t_t, 'Pure top (Qt)', 'L')
#     # legend.AddEntry(h_b_b, 'Pure bottom (Qb)', 'L')
#     # legend.AddEntry(h_int, 'Interference (Qtb)', 'L')
#     # legend.AddEntry(h_tot, 'Powheg total', 'L')

#     ana.nodes[name][sa].shape.hist.Draw('SAMEHIST')

#     if name == 'm1_mass':
#         plot.Set(ana.nodes['rec_all_prong_mass'][sa].shape.hist, LineColor=4, LineWidth=2)
#         legend.AddEntry(ana.nodes['m1_mass'][sa].shape.hist, 'Gen. level', 'L')
#         legend.AddEntry(ana.nodes['rec_all_prong_mass'][sa].shape.hist, 'Reco. level', 'L')
#         ana.nodes['rec_all_prong_mass'][sa].shape.hist.Draw('SAMEHIST')

#     if 'rec_nprongs' in name:
#         ana.nodes[name][sa].shape.hist.Scale(1. / ana.nodes[name][sa].shape.hist.Integral())

#     # h_t_t.Draw('SAMEHIST')
#     # h_b_b.Draw('SAMEHIST')
#     # h_int.Draw('SAMEHIST')
#     # h_tot.Draw('SAMEHIST')
#     # h_pythia_check.Draw('SAMEHIST')

#     # min_val = 0.
#     # if int_xs < 0.:
#     #     min_val = h_int.GetMinimum()

#     plot.GetAxisHist(pads[0]).SetMinimum(0)
#     plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.30)
#     legend.Draw()

#     axis = plot.GetAxisHist(pads[0])
#     plot.Set(axis.GetXaxis(), Title=axes[0])
#     plot.Set(axis.GetYaxis(), Title=axes[1])

#     # plot.DrawTitle(pads[0], 'Boson: %s, ^{}m_{%s}=%s GeV, tan#beta=%s' % (partx, partx, mA, tanb), 3)
#     canv.Print('.pdf')
#     canv.Print('.png')


outfile = ROOT.TFile('output_test.root', 'RECREATE')

ana.nodes.Output(outfile)

outfile.Close()
