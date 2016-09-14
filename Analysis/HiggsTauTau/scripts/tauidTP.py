import ROOT
import argparse
from UserCode.ICHiggsTauTau.analysis import *

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)

parser = argparse.ArgumentParser()
parser.add_argument('--type', '-t', help='control, tp')
parser.add_argument('--output', '-o', default='TauID_Basic', help='Output name')
parser.add_argument('--qcd_os_ss', default=1.0, type=float)
parser.add_argument('--extra', default=None)

args = parser.parse_args()


ROOT.TH1.SetDefaultSumw2(True)


ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
path = 'output/HTT2016Studies_Aug16/ZmtTP'
ana.AddSamples('%s/*.root' % path, 'ZmtTP')
ana.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')

ana_scale_t_hi = Analysis()
ana_scale_t_hi.remaps = ana.remaps
ana_scale_t_hi.AddSamples('%s/scale_t_hi/*.root' % path, 'ZmtTP', fallback='%s/*.root' % path)
ana_scale_t_hi.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')

ana_scale_t_lo = Analysis()
ana_scale_t_lo.remaps = ana.remaps
ana_scale_t_lo.AddSamples('%s/scale_t_lo/*.root' % path, 'ZmtTP', fallback='%s/*.root' % path)
ana_scale_t_lo.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')
# missing OS!


def StandardMT(ana, node, v, sel, pfix='', qcd_os_ss=1.0):
    vv_samples = ['VVTo2L2Nu', 'WWTo1L1Nu2Q', 'WZJToLLLNu',
        'WZTo1L1Nu2Q', 'WZTo1L3Nu', 'WZTo2L2Q', 'ZZTo2L2Q', 'ZZTo4L']

    node.AddNode(BasicNode('data_obs' + pfix, 'data_obs', v, sel()))

    node.AddNode(ana.BasicFactory(
        'ZTT' + pfix, 'DYJetsToLL', v, sel(extra='gen_2==5')))

    node.AddNode(ana.BasicFactory(
        'ZL' + pfix, 'DYJetsToLL', v, sel(extra='gen_2<5')))

    node.AddNode(ana.BasicFactory(
        'ZJ' + pfix, 'DYJetsToLL', v, sel(extra='gen_2==6')))

    node.AddNode(ana.BasicFactory('W' + pfix, 'WJetsToLNu', v, sel()))

    node.AddNode(ana.BasicFactory('TT' + pfix, 'TT', v, sel()))

    node.AddNode(ana.SummedFactory('VV' + pfix, vv_samples, v, sel()))

    node.AddNode(HttQCDNode('QCD' + pfix,
        ana.BasicFactory('data_obs' + pfix, 'data_obs', v, sel(sign='!os')),
        ana.SummedFactory('backgrounds' + pfix,
            ['DYJetsToLL', 'WJetsToLNu', 'TT'], v, sel(sign='!os')),
        qcd_os_ss))
    node['QCD' + pfix].subtract_node.add_output_prefix = False
    node['QCD' + pfix].subtract_node.AddNode(ana.SummedFactory('VV' + pfix, vv_samples, v, sel(sign='!os')))


def TagAndProbeCats(ana, node, name, var_pass, var_fail, baseline, probe, pfix=''):
    node.AddNode(ListNode(name+'_pass'))
    node.AddNode(ListNode(name+'_fail'))
    pass_sel = baseline.copy(probe=probe())
    fail_sel = baseline.copy(probe='!(%s)' % probe())
    StandardMT(ana, node[name+'_pass'], var_pass, pass_sel, pfix, qcd_os_ss=1.17)
    StandardMT(ana, node[name+'_fail'], var_fail, fail_sel, pfix, qcd_os_ss=1.00)

sel = Sel(sign='os', baseline='mt_m<40 && anti_e_t && anti_m_t', wt='wt')

if args.type == 'control':
    ana_list = [ana]
    for selection in [('baseline',    '1'),
                      ('chiso_lt2',   'chiso_t<2.0'),
                      ('chiso_lt0p8', 'chiso_t<0.8'),
                      ('cbiso_lt0p8', 'cbiso_t<0.8')]:
        ana.nodes.AddNode(ListNode(selection[0]))
        for var in [
                    ('rho',             'rho(30,0,30);#rho:GeV'),
                    ('dm_t',            'dm_t(12,-0.5,11.5);#tau_{h} decay mode:'),
                    ('chiso_t',         'chiso_t(50,0,20);Charged Iso:GeV'),
                    ('cbiso_t',         'cbiso_t(50,0,20);Combined Iso:GeV'),
                    ('cbiso_0p5_t',     'cbiso_0p5_t(50,0,20);Combined Iso (E_{T}^{#gamma}>0.5 GeV):GeV'),
                    ('cbiso_1p0_t',     'cbiso_1p0_t(50,0,20);Combined Iso (E_{T}^{#gamma}>1.0 GeV):GeV'),
                    ('cbiso_1p5_t',     'cbiso_1p5_t(50,0,20);Combined Iso (E_{T}^{#gamma}>1.5 GeV):GeV'),
                    ('cbiso_2p0_t',     'cbiso_2p0_t(50,0,20);Combined Iso (E_{T}^{#gamma}>2.0 GeV):GeV'),
                    ('ntiso_t',         'ntiso_t(50,0,20);Neutral Iso:GeV'),
                    ('ntiso_0p5_t',     'ntiso_0p5_t(50,0,20);Neutral Iso (E_{T}^{#gamma}>0.5 GeV):GeV'),
                    ('ntiso_1p0_t',     'ntiso_1p0_t(50,0,20);Neutral Iso (E_{T}^{#gamma}>1.0 GeV):GeV'),
                    ('ntiso_1p5_t',     'ntiso_1p5_t(50,0,20);Neutral Iso (E_{T}^{#gamma}>1.5 GeV):GeV'),
                    ('ntiso_2p0_t',     'ntiso_2p0_t(50,0,20);Neutral Iso (E_{T}^{#gamma}>2.0 GeV):GeV'),
                    ('puiso_t',         'puiso_t(50,0,20); Pileup Iso:GeV'),
                    ('ntiso_t_vs_puiso_t',  'ntiso_t,puiso_t(30,0,30),(50,0,100);Neutral Iso:GeV;Pileup Iso:GeV'),
                    ('pho_out_t_vs_puiso_t',  'pho_out_t,puiso_t(50,0,10),(50,0,100);PhotonOutside Iso:GeV;Pileup Iso:GeV'),
                    ('pho_out_t',       'pho_out_t(50,0,20);PhotonOutside Iso:GeV'),
                    ('pho_out_0p5_t',   'pho_out_0p5_t(50,0,20);PhotonOutside Iso (E_{T}^{#gamma}>0.5 GeV):GeV'),
                    ('pho_out_1p0_t',   'pho_out_1p0_t(50,0,20);PhotonOutside Iso (E_{T}^{#gamma}>1.0 GeV):GeV'),
                    ('pho_out_1p5_t',   'pho_out_1p5_t(50,0,20);PhotonOutside Iso (E_{T}^{#gamma}>1.5 GeV):GeV'),
                    ('pho_out_2p0_t',   'pho_out_2p0_t(50,0,20);PhotonOutside Iso (E_{T}^{#gamma}>2.0 GeV):GeV'),
                    ('pho_out_t_ratio', 'pho_out_t/pt_t(50,0,1);PhotonOutside/p_{T}^{#tau}:'),
                    ('pho_out_0p5_t_ratio', 'pho_out_0p5_t/pt_t(50,0,1);PhotonOutside/p_{T}^{#tau} (E_{T}^{#gamma}>0.5 GeV):'),
                    ('pho_out_1p0_t_ratio', 'pho_out_1p0_t/pt_t(50,0,1);PhotonOutside/p_{T}^{#tau} (E_{T}^{#gamma}>1.0 GeV):'),
                    ('pho_out_1p5_t_ratio', 'pho_out_1p5_t/pt_t(50,0,1);PhotonOutside/p_{T}^{#tau} (E_{T}^{#gamma}>1.5 GeV):'),
                    ('pho_out_2p0_t_ratio', 'pho_out_2p0_t/pt_t(50,0,1);PhotonOutside/p_{T}^{#tau} (E_{T}^{#gamma}>2.0 GeV):'),
                    ('ntiso_db_t',      'TMath::Max(0.,ntiso_t-0.2*puiso_t)(50,0,20);Neutral Iso #Delta#beta:GeV'),
                    ('m_ll',            'm_ll(50,0,250);m_{#tau#tau}^{vis}:GeV'),
                    ('n_iso_ph_0p5',    'n_iso_ph_0p5(20,-0.5,19.5);nIsoPhotons (E_{T}^{#gamma}>0.5 GeV):'),
                    ('n_iso_ph_1p0',    'n_iso_ph_1p0(20,-0.5,19.5);nIsoPhotons (E_{T}^{#gamma}>1.0 GeV):'),
                    ('n_iso_ph_1p5',    'n_iso_ph_1p5(20,-0.5,19.5);nIsoPhotons (E_{T}^{#gamma}>1.5 GeV):'),
                    ('n_iso_ph_2p0',    'n_iso_ph_2p0(20,-0.5,19.5);nIsoPhotons (E_{T}^{#gamma}>2.0 GeV):'),
                    ('n_sig_ph_0p5',    'n_sig_ph_0p5(20, 0.5,20.5);nSigPhotons (E_{T}^{#gamma}>0.5 GeV):'),
                    ('n_sig_ph_1p0',    'n_sig_ph_1p0(20, 0.5,20.5);nSigPhotons (E_{T}^{#gamma}>1.0 GeV)'),
                    ('n_sig_ph_1p5',    'n_sig_ph_1p5(20, 0.5,20.5);nSigPhotons (E_{T}^{#gamma}>1.5 GeV)'),
                    ('n_sig_ph_2p0',    'n_sig_ph_2p0(20, 0.5,20.5);nSigPhotons (E_{T}^{#gamma}>2.0 GeV)'),
                    ('n_sum_ph_0p5',    'n_sig_ph_0p5+n_iso_ph_0p5(25,-0.5,24.5);nSumPhotons (E_{T}^{#gamma}>0.5 GeV)'),
                    ('n_sum_ph_1p0',    'n_sig_ph_1p0+n_iso_ph_1p0(25,-0.5,24.5);nSumPhotons (E_{T}^{#gamma}>1.0 GeV)'),
                    ('n_sum_ph_1p5',    'n_sig_ph_1p5+n_iso_ph_1p5(25,-0.5,24.5);nSumPhotons (E_{T}^{#gamma}>1.5 GeV)'),
                    ('n_sum_ph_2p0',    'n_sig_ph_2p0+n_iso_ph_2p0(25,-0.5,24.5);nSumPhotons (E_{T}^{#gamma}>2.0 GeV)'),
                    ('n_sum_ph_2p0',    'n_sig_ph_2p0+n_iso_ph_2p0(25,-0.5,24.5);nSumPhotons (E_{T}^{#gamma}>2.0 GeV)'),
                    ('nt_density_0p0_0p1', 'nt_density_0p0_0p1[0:10:2,10:20:5,20:100:10];Photon p_{T} density #DeltaR_{0.0}^{0.1}:GeV'),
                    ('nt_density_0p1_0p2', 'nt_density_0p1_0p2[0:10:2,10:20:5,20:100:10];Photon p_{T} density #DeltaR_{0.1}^{0.2}:GeV'),
                    ('nt_density_0p2_0p3', 'nt_density_0p2_0p3[0:10:2,10:20:5,20:100:10];Photon p_{T} density #DeltaR_{0.2}^{0.3}:GeV'),
                    ('nt_density_0p3_0p4', 'nt_density_0p3_0p4[0:10:2,10:20:5,20:100:10];Photon p_{T} density #DeltaR_{0.3}^{0.4}:GeV'),
                    ('nt_density_0p4_0p5', 'nt_density_0p4_0p5[0:10:2,10:20:5,20:100:10];Photon p_{T} density #DeltaR_{0.4}^{0.5}:GeV'),
                    ('po_density_0p0_0p1', 'po_density_0p0_0p1[0:10:2,10:20:5,20:100:10];PhotonOutside p_{T} density #DeltaR_{0.0}^{0.1}:GeV'),
                    ('po_density_0p1_0p2', 'po_density_0p1_0p2[0:10:2,10:20:5,20:100:10];PhotonOutside p_{T} density #DeltaR_{0.1}^{0.2}:GeV'),
                    ('po_density_0p2_0p3', 'po_density_0p2_0p3[0:10:2,10:20:5,20:100:10];PhotonOutside p_{T} density #DeltaR_{0.2}^{0.3}:GeV'),
                    ('po_density_0p3_0p4', 'po_density_0p3_0p4[0:10:2,10:20:5,20:100:10];PhotonOutside p_{T} density #DeltaR_{0.3}^{0.4}:GeV'),
                    ('po_density_0p4_0p5', 'po_density_0p4_0p5[0:10:2,10:20:5,20:100:10];PhotonOutside p_{T} density #DeltaR_{0.4}^{0.5}:GeV'),
                    ('tot_density_0p0_0p1', 'po_density_0p0_0p1+nt_density_0p0_0p1[0:10:2,10:20:5,20:100:10];Total Photon p_{T} density #DeltaR_{0.0}^{0.1}:GeV'),
                    ('tot_density_0p1_0p2', 'po_density_0p1_0p2+nt_density_0p1_0p2[0:10:2,10:20:5,20:100:10];Total Photon p_{T} density #DeltaR_{0.1}^{0.2}:GeV'),
                    ('tot_density_0p2_0p3', 'po_density_0p2_0p3+nt_density_0p2_0p3[0:10:2,10:20:5,20:100:10];Total Photon p_{T} density #DeltaR_{0.2}^{0.3}:GeV'),
                    ('tot_density_0p3_0p4', 'po_density_0p3_0p4+nt_density_0p3_0p4[0:10:2,10:20:5,20:100:10];Total Photon p_{T} density #DeltaR_{0.3}^{0.4}:GeV'),
                    ('tot_density_0p4_0p5', 'po_density_0p4_0p5+nt_density_0p4_0p5[0:10:2,10:20:5,20:100:10];Total Photon p_{T} density #DeltaR_{0.4}^{0.5}:GeV'),
                  ]:
            nodename = var[0]
            v = var[1]
            dosel = sel.copy(extra_selection=selection[1])
            if args.extra is not None:
                dosel = dosel.copy(extra_baseline=args.extra)
            ana.nodes[selection[0]].AddNode(ListNode(nodename))
            StandardMT(ana, ana.nodes[selection[0]][nodename], v, dosel, qcd_os_ss=args.qcd_os_ss)

"""
for var in [('mt_m',         'mt_m(40,0,200)', '1'),
            ('pzeta',                 'pzeta(60,-200,100)', '1'),
            ('pzeta_after_mt',        'pzeta(60,-200,100)', 'mt_m<40'),
            ]:
    nodename = var[0]
    v = var[1]
    ana.nodes.AddNode(ListNode(nodename))
    StandardMT(ana, ana.nodes[nodename], v, Sel(sign='os', baseline='anti_e_t && anti_m_t && n_bjets==0', wt='wt', extra_baseline=var[2]))
"""

if args.type == 'tp':
    ana_list = [ana, ana_scale_t_hi, ana_scale_t_lo]
    for var in [
            ('cmb_m_2p5',  'm_ll(32,40,200)', 'cbiso_t < 2.5', '1'),
            ('cmb_m_1p5',  'm_ll(32,40,200)', 'cbiso_t < 1.5', '1'),
            ('cmb_t_0p8',  'm_ll(32,40,200)', 'cbiso_t < 0.8', '1'),
            ('ch_m_2p5',  'm_ll(32,40,200)', 'chiso_t < 2.5', '1'),
            ('ch_m_1p5',  'm_ll(32,40,200)', 'chiso_t < 1.5', '1'),
            ('ch_t_0p8',  'm_ll(32,40,200)', 'chiso_t < 0.8', '1'),
            ('cmb_l',  'm_ll(32,40,200)', 'cmb_l_t', '1'),
            ('cmb_m',  'm_ll(32,40,200)', 'cmb_m_t', '1'),
            ('cmb_t',  'm_ll(32,40,200)', 'cmb_t_t', '1'),
            ('mva_vl',  'm_ll(32,40,200)', 'mva_vl_t', '1'),
            ('mva_l',   'm_ll(32,40,200)', 'mva_l_t', '1'),
            ('mva_m',   'm_ll(32,40,200)', 'mva_m_t', '1'),
            ('mva_t',   'm_ll(32,40,200)', 'mva_t_t', '1'),
            ('mva_vt',  'm_ll(32,40,200)', 'mva_vt_t', '1'),
            ('mva_vvt', 'm_ll(32,40,200)', 'mva_vvt_t', '1'),
            ('mva_t_pt20_30', 'm_ll(32,40,200)',  'mva_t_t', 'pt_t>20 && pt_t<=30'),
            ('mva_t_pt30_40', 'm_ll(32,40,200)',  'mva_t_t', 'pt_t>30 && pt_t<=40'),
            ('mva_t_pt40_60', 'm_ll(16,40,200)',  'mva_t_t', 'pt_t>40 && pt_t<=60'),
            ('mva_t_pt60_100', 'm_ll(8,40,200)', 'mva_t_t', 'pt_t>60 && pt_t<=100'),
            ('mva_m_pt20_30', 'm_ll(32,40,200)',  'mva_m_t', 'pt_t>20 && pt_t<=30'),
            ('mva_m_pt30_40', 'm_ll(32,40,200)',  'mva_m_t', 'pt_t>30 && pt_t<=40'),
            ('mva_m_pt40_60', 'm_ll(16,40,200)',  'mva_m_t', 'pt_t>40 && pt_t<=60'),
            ('mva_m_pt60_100', 'm_ll(8,40,200)', 'mva_m_t', 'pt_t>60 && pt_t<=100'),
            ('pt_mva_t_pt20_30',  'pt_t(40,20,100)',  'mva_t_t', 'pt_t>20 && pt_t<=30'),
            ('pt_mva_t_pt30_40',  'pt_t(40,20,100)',  'mva_t_t', 'pt_t>30 && pt_t<=40'),
            ('pt_mva_t_pt40_60',  'pt_t(40,20,100)',  'mva_t_t', 'pt_t>40 && pt_t<=60'),
            ('pt_mva_t_pt60_100', 'pt_t(40,20,100)',  'mva_t_t', 'pt_t>60 && pt_t<=100'),
            ('pt_mva_m_pt20_30',  'pt_t(40,20,100)',  'mva_m_t', 'pt_t>20 && pt_t<=30'),
            ('pt_mva_m_pt30_40',  'pt_t(40,20,100)',  'mva_m_t', 'pt_t>30 && pt_t<=40'),
            ('pt_mva_m_pt40_60',  'pt_t(40,20,100)',  'mva_m_t', 'pt_t>40 && pt_t<=60'),
            ('pt_mva_m_pt60_100', 'pt_t(40,20,100)',  'mva_m_t', 'pt_t>60 && pt_t<=100'),
            ('cmb_t_pt20_30', 'm_ll(32,40,200)',  'cmb_t_t', 'pt_t>20 && pt_t<=30'),
            ('cmb_t_pt30_40', 'm_ll(32,40,200)',  'cmb_t_t', 'pt_t>30 && pt_t<=40'),
            ('cmb_t_pt40_60', 'm_ll(16,40,200)',  'cmb_t_t', 'pt_t>40 && pt_t<=60'),
            ('cmb_t_pt60_100', 'm_ll(8,40,200)', 'cmb_t_t', 'pt_t>60 && pt_t<=100'),
            ('mva_t_dm0', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==0'),
            ('mva_t_dm1', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==1'),
            ('mva_t_dm10', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==10'),
            ('mva_m_dm0_pt20_30', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==0 && pt_t>20 && pt_t<=30'),
            ('mva_m_dm1_pt20_30', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==1 && pt_t>20 && pt_t<=30'),
            ('mva_m_dm10_pt20_30', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==10 && pt_t>20 && pt_t<=30'),
            ('mva_m_pt30', 'm_ll(32,40,200)',  'mva_m_t', 'pt_t>30'),
            ('mva_m_dm0_pt30', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==0 && pt_t>30'),
            ('mva_m_dm1_pt30', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==1 && pt_t>30'),
            ('mva_m_dm10_pt30', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==10 && pt_t>30'),
            ('mva_t_pt40_eta2p1', 'm_ll(32,40,200)',  'mva_t_t', 'pt_t>40 && abs(eta_t)<2.1'),
            ('mva_t_dm0_pt40_eta2p1', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==0 && pt_t>40 && abs(eta_t)<2.1'),
            ('mva_t_dm1_pt40_eta2p1', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==1 && pt_t>40 && abs(eta_t)<2.1'),
            ('mva_t_dm10_pt40_eta2p1', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==10 && pt_t>40 && abs(eta_t)<2.1'),
            ('mva_m_dm0_pt30_b', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==0 && pt_t>30 && abs(eta_t)<1.5'),
            ('mva_m_dm1_pt30_b', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==1 && pt_t>30 && abs(eta_t)<1.5'),
            ('mva_m_dm10_pt30_b', 'm_ll(32,40,200)',  'mva_m_t', 'dm_t==10 && pt_t>30 && abs(eta_t)<1.5'),
            ('mva_m_dm0_pt30_e', 'm_ll(16,40,200)',  'mva_m_t', 'dm_t==0 && pt_t>30 && abs(eta_t)>=1.5'),
            ('mva_m_dm1_pt30_e', 'm_ll(16,40,200)',  'mva_m_t', 'dm_t==1 && pt_t>30 && abs(eta_t)>=1.5'),
            ('mva_m_dm10_pt30_e', 'm_ll(16,40,200)',  'mva_m_t', 'dm_t==10 && pt_t>30 && abs(eta_t)>=1.5'),
            ('mva_t_dm0_pt40_b', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==0 && pt_t>40 && abs(eta_t)<1.5'),
            ('mva_t_dm1_pt40_b', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==1 && pt_t>40 && abs(eta_t)<1.5'),
            ('mva_t_dm10_pt40_b', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==10 && pt_t>40 && abs(eta_t)<1.5'),
            ('mva_t_dm0_pt40_e', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==0 && pt_t>40 && abs(eta_t)>=1.5'),
            ('mva_t_dm1_pt40_e', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==1 && pt_t>40 && abs(eta_t)>=1.5'),
            ('mva_t_dm10_pt40_e', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==10 && pt_t>40 && abs(eta_t)>=1.5'),
            ('mva_t_dm0_b', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==0 && abs(eta_t)<1.5'),
            ('mva_t_dm1_b', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==1 && abs(eta_t)<1.5'),
            ('mva_t_dm10_b', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==10 && abs(eta_t)<1.5'),
            ('mva_t_dm0_e', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==0 && abs(eta_t)>=1.5'),
            ('mva_t_dm1_e', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==1 && abs(eta_t)>=1.5'),
            ('mva_t_dm10_e', 'm_ll(16,40,200)',  'mva_t_t', 'dm_t==10 && abs(eta_t)>=1.5'),
            ('cmb_t_dm0', 'm_ll(32,40,200)',  'cmb_t_t', 'dm_t==0'),
            ('cmb_t_dm1', 'm_ll(32,40,200)',  'cmb_t_t', 'dm_t==1'),
            ('cmb_t_dm10', 'm_ll(32,40,200)',  'cmb_t_t', 'dm_t==10'),
            ('cmb_0p5_t',  'm_ll(32,40,200)', 'cbiso_0p5_t<0.8 && (pho_out_0p5_t/pt_t)<0.1', '1'),
            ('cmb_1p0_t',  'm_ll(32,40,200)', 'cbiso_1p0_t<0.8 && (pho_out_1p0_t/pt_t)<0.1', '1'),
            ('cmb_1p5_t',  'm_ll(32,40,200)', 'cbiso_1p5_t<0.8 && (pho_out_1p5_t/pt_t)<0.1', '1'),
            ('cmb_2p0_t',  'm_ll(32,40,200)', 'cbiso_2p0_t<0.8 && (pho_out_2p0_t/pt_t)<0.1', '1'),
            ]:
        nodename = var[0]
        varnames = var[1].split(';')
        v_pass = varnames[0]
        v_fail = varnames[1] if len(varnames) > 1 else varnames[0]
        probe = Sel(probe=var[2])
        dosel = sel
        if len(var) >= 4:
            dosel = sel.copy(extra_baseline=var[3])
        TagAndProbeCats(ana, ana.nodes, nodename, v_pass, v_fail, dosel, probe)
        TagAndProbeCats(ana_scale_t_hi, ana_scale_t_hi.nodes, nodename, v_pass, v_fail, dosel, probe, pfix='_CMS_scale_tUp')
        TagAndProbeCats(ana_scale_t_lo, ana_scale_t_lo.nodes, nodename, v_pass, v_fail, dosel, probe, pfix='_CMS_scale_tDown')

outfile = ROOT.TFile('%s.root' % args.output, 'RECREATE')

for a in ana_list:
  a.nodes.PrintTree()
  a.compiled = True
  a.Run()
  a.nodes.Output(outfile)

outfile.Close()
"""            
            ('nch_mva_t_dm0', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==0'),
            ('nch_mva_t_dm1', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==1'),
            ('nch_mva_t_dm10', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==10'),
            ('nch_mva_m_dm0_pt30', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==0 && pt_t>30'),
            ('nch_mva_m_dm1_pt30', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==1 && pt_t>30'),
            ('nch_mva_m_dm10_pt30', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==10 && pt_t>30'),
            ('nch_mva_t_dm0_pt40', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==0 && pt_t>40'),
            ('nch_mva_t_dm1_pt40', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==1 && pt_t>40'),
            ('nch_mva_t_dm10_pt40', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==10 && pt_t>40'),
            ('nch_mva_m_dm0_pt30_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==0 && pt_t>30 && abs(eta_t)<1.5'),
            ('nch_mva_m_dm1_pt30_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==1 && pt_t>30 && abs(eta_t)<1.5'),
            ('nch_mva_m_dm10_pt30_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==10 && pt_t>30 && abs(eta_t)<1.5'),
            ('nch_mva_m_dm0_pt30_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==0 && pt_t>30 && abs(eta_t)>=1.5'),
            ('nch_mva_m_dm1_pt30_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==1 && pt_t>30 && abs(eta_t)>=1.5'),
            ('nch_mva_m_dm10_pt30_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_m_t', 'dm_t==10 && pt_t>30 && abs(eta_t)>=1.5'),
            ('nch_mva_t_dm0_pt40_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==0 && pt_t>40 && abs(eta_t)<1.5'),
            ('nch_mva_t_dm1_pt40_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==1 && pt_t>40 && abs(eta_t)<1.5'),
            ('nch_mva_t_dm10_pt40_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==10 && pt_t>40 && abs(eta_t)<1.5'),
            ('nch_mva_t_dm0_pt40_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==0 && pt_t>40 && abs(eta_t)>=1.5'),
            ('nch_mva_t_dm1_pt40_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==1 && pt_t>40 && abs(eta_t)>=1.5'),
            ('nch_mva_t_dm10_pt40_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==10 && pt_t>40 && abs(eta_t)>=1.5'),
            ('nch_mva_t_dm0_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==0 && abs(eta_t)<1.5'),
            ('nch_mva_t_dm1_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==1 && abs(eta_t)<1.5'),
            ('nch_mva_t_dm10_b', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==10 && abs(eta_t)<1.5'),
            ('nch_mva_t_dm0_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==0 && abs(eta_t)>=1.5'),
            ('nch_mva_t_dm1_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==1 && abs(eta_t)>=1.5'),
            ('nch_mva_t_dm10_e', 'tot_ch_t(8,0.5,8.5);tot_ch_t(20,0.5,20.5)',  'mva_t_t', 'dm_t==10 && abs(eta_t)>=1.5'),
"""
