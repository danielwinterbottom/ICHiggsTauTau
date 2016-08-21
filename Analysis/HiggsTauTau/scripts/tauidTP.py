import ROOT
from UserCode.ICHiggsTauTau.analysis import *

ROOT.TH1.SetDefaultSumw2(True)



ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
ana.AddSamples('output/HTT2016Studies_Aug16/ZmtTP/*.root', 'ZmtTP')
ana.AddInfo('params_Aug16.json', scaleTo='data_obs')
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


def TagAndProbeCats(ana, node, name, var, baseline, probe):
    node.AddNode(ListNode(name+'_pass'))
    node.AddNode(ListNode(name+'_fail'))
    pass_sel = baseline.copy(probe=probe())
    fail_sel = baseline.copy(probe='!(%s)' % probe())
    StandardMT(ana, node[name+'_pass'], var, pass_sel, qcd_os_ss=1.17)
    StandardMT(ana, node[name+'_fail'], var, fail_sel, qcd_os_ss=1.10)

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

# for var in [('mt_m',         'mt_m(40,0,200)'),
#             ]:
#     nodename = var[0]
#     v = var[1]
#     ana.nodes.AddNode(ListNode(nodename))
#     StandardMT(ana, ana.nodes[nodename], v, Sel(sign='os', baseline='anti_e_t && anti_m_t && n_bjets==0 && tight_t', wt='wt'))

for var in [('mva_t', 'm_ll(50,0,250)', 'mva_t_t'),
            ('mva_vt', 'm_ll(50,0,250)', 'mva_vt_t'),
            ]:
    nodename = var[0]
    v = var[1]
    probe = Sel(probe=var[2])
    dosel = sel
    if len(var) >= 4:
        dosel = sel.copy(extra=var[3])
    TagAndProbeCats(ana, ana.nodes, nodename, v, dosel, probe)


ana.nodes.PrintTree()
ana.Run()

outfile = ROOT.TFile('TauID_Basic.root', 'RECREATE')

ana.nodes.Output(outfile)
