import ROOT
from UserCode.ICHiggsTauTau.analysis import *

ROOT.TH1.SetDefaultSumw2(True)



ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
ana.AddSamples('output/HTT2016Studies_Aug16/ZmtTP/*.root', 'ZmtTP')
ana.AddInfo('params_Aug16.json', scaleTo='data_obs')

ana_scale_t_hi = Analysis()
ana_scale_t_hi.remaps = ana.remaps
ana_scale_t_hi.AddSamples('output/HTT2016Studies_Aug16/ZmtTP/scale_t_hi/*.root', 'ZmtTP', fallback='output/HTT2016Studies_Aug16/ZmtTP/*.root')
ana_scale_t_hi.AddInfo('params_Aug16.json', scaleTo='data_obs')

ana_scale_t_lo = Analysis()
ana_scale_t_lo.remaps = ana.remaps
ana_scale_t_lo.AddSamples('output/HTT2016Studies_Aug16/ZmtTP/scale_t_lo/*.root', 'ZmtTP', fallback='output/HTT2016Studies_Aug16/ZmtTP/*.root')
ana_scale_t_lo.AddInfo('params_Aug16.json', scaleTo='data_obs')
# missing OS!


def StandardMT(ana, node, var, sel, pfix='', qcd_os_ss=1.0):
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
    node['QCD' + pfix].subtract_node.AddNode(ana.SummedFactory('VV' + pfix, vv_samples, v, sel(sign='!os')))


def TagAndProbeCats(ana, node, name, var, baseline, probe, pfix=''):
    node.AddNode(ListNode(name+'_pass'))
    node.AddNode(ListNode(name+'_fail'))
    pass_sel = baseline.copy(probe=probe())
    fail_sel = baseline.copy(probe='!(%s)' % probe())
    StandardMT(ana, node[name+'_pass'], var, pass_sel, pfix, qcd_os_ss=1.17)
    StandardMT(ana, node[name+'_fail'], var, fail_sel, pfix, qcd_os_ss=1.00)

sel = Sel(sign='os', baseline='mt_m<40 && anti_e_t && anti_m_t', wt='wt')
for var in [('chiso_t',         'chiso_t(50,0,20)'),
            ('cbiso_t',         'cbiso_t(50,0,20)'),
            ('ntiso_t',         'ntiso_t(50,0,20)'),
            ('puiso_t',         'puiso_t(50,0,20)'),
            ('pho_out_t',       'pho_out_t(50,0,20)'),
            ('pho_out_t_ratio', 'pho_out_t/pt_t(50,0,1)'),
            ('m_ll',            'm_ll(50,0,250)'),
            ('n_iso_ph_0p5',    'n_iso_ph_0p5(20,-0.5,19.5)'),
            ('n_iso_ph_1p0',    'n_iso_ph_1p0(20,-0.5,19.5)'),
            ('n_iso_ph_1p5',    'n_iso_ph_1p5(20,-0.5,19.5)'),
            ('n_iso_ph_2p0',    'n_iso_ph_2p0(20,-0.5,19.5)'),
            ('n_sig_ph_0p5',    'n_sig_ph_0p5(20, 0.5,20.5)'),
            ('n_sig_ph_1p0',    'n_sig_ph_1p0(20, 0.5,20.5)'),
            ('n_sig_ph_1p5',    'n_sig_ph_1p5(20, 0.5,20.5)'),
            ('n_sig_ph_2p0',    'n_sig_ph_2p0(20, 0.5,20.5)'),
            ('n_sum_ph_0p5',    'n_sig_ph_0p5+n_iso_ph_0p5(25,-0.5,24.5)'),
            ('n_sum_ph_1p0',    'n_sig_ph_1p0+n_iso_ph_1p0(25,-0.5,24.5)'),
            ('n_sum_ph_1p5',    'n_sig_ph_1p5+n_iso_ph_1p5(25,-0.5,24.5)'),
            ('n_sum_ph_2p0',    'n_sig_ph_2p0+n_iso_ph_2p0(25,-0.5,24.5)'),
            ]:
    nodename = var[0]
    v = var[1]
    ana.nodes.AddNode(ListNode(nodename))
    StandardMT(ana, ana.nodes[nodename], v, sel)

for var in [('mt_m',         'mt_m(40,0,200)', '1'),
            ('pzeta',                 'pzeta(60,-200,100)', '1'),
            ('pzeta_after_mt',        'pzeta(60,-200,100)', 'mt_m<40'),
            ]:
    nodename = var[0]
    v = var[1]
    ana.nodes.AddNode(ListNode(nodename))
    StandardMT(ana, ana.nodes[nodename], v, Sel(sign='os', baseline='anti_e_t && anti_m_t && n_bjets==0', wt='wt', extra=var[2]))

for var in [('cmb_l',  'm_ll(32,40,200)', 'cmb_l_t', '1'),
            ('cmb_m',  'm_ll(32,40,200)', 'cmb_m_t', '1'),
            ('cmb_t',  'm_ll(32,40,200)', 'cmb_t_t', '1'),
            ('mva_vl',  'm_ll(32,40,200)', 'mva_t_t', '1'),
            ('mva_l',   'm_ll(32,40,200)', 'mva_l_t', '1'),
            ('mva_m',   'm_ll(32,40,200)', 'mva_m_t', '1'),
            ('mva_t',   'm_ll(32,40,200)', 'mva_t_t', '1'),
            ('mva_vt',  'm_ll(32,40,200)', 'mva_vt_t', '1'),
            ('mva_vvt', 'm_ll(32,40,200)', 'mva_vvt_t', '1'),
            ('mva_t_pt_20_30', 'm_ll(32,40,200)',  'mva_t_t', 'pt_t>20 && pt_t<=30'),
            ('mva_t_pt_30_40', 'm_ll(32,40,200)',  'mva_t_t', 'pt_t>30 && pt_t<=40'),
            ('mva_t_pt_40_60', 'm_ll(32,40,200)',  'mva_t_t', 'pt_t>40 && pt_t<=60'),
            ('mva_t_pt_60_100', 'm_ll(32,40,200)', 'mva_t_t', 'pt_t>60 && pt_t<=100'),
            ('cmb_t_pt_20_30', 'm_ll(32,40,200)',  'cmb_t_t', 'pt_t>20 && pt_t<=30'),
            ('cmb_t_pt_30_40', 'm_ll(32,40,200)',  'cmb_t_t', 'pt_t>30 && pt_t<=40'),
            ('cmb_t_pt_40_60', 'm_ll(32,40,200)',  'cmb_t_t', 'pt_t>40 && pt_t<=60'),
            ('cmb_t_pt_60_100', 'm_ll(32,40,200)', 'cmb_t_t', 'pt_t>60 && pt_t<=100'),
            ('mva_t_dm0', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==0'),
            ('mva_t_dm1', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==1'),
            ('mva_t_dm10', 'm_ll(32,40,200)',  'mva_t_t', 'dm_t==10'),
            ('cmb_t_dm0', 'm_ll(32,40,200)',  'cmb_t_t', 'dm_t==0'),
            ('cmb_t_dm1', 'm_ll(32,40,200)',  'cmb_t_t', 'dm_t==1'),
            ('cmb_t_dm10', 'm_ll(32,40,200)',  'cmb_t_t', 'dm_t==10'),
            ]:
    nodename = var[0]
    v = var[1]
    probe = Sel(probe=var[2])
    dosel = sel
    if len(var) >= 4:
        dosel = sel.copy(extra=var[3])
    TagAndProbeCats(ana, ana.nodes, nodename, v, dosel, probe)
    TagAndProbeCats(ana_scale_t_hi, ana_scale_t_hi.nodes, nodename, v, dosel, probe, pfix='_CMS_scale_tUp')
    TagAndProbeCats(ana_scale_t_lo, ana_scale_t_lo.nodes, nodename, v, dosel, probe, pfix='_CMS_scale_tDown')

outfile = ROOT.TFile('TauID_Basic.root', 'RECREATE')

for a in [ana, ana_scale_t_hi, ana_scale_t_lo]:
  a.nodes.PrintTree()
  a.compiled = True
  a.Run()
  a.nodes.Output(outfile)

outfile.Close()
