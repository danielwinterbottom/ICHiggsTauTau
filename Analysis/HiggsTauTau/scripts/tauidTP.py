import ROOT
from UserCode.ICHiggsTauTau.analysis import *

ROOT.TH1.SetDefaultSumw2(True)

sel = Sel(sign='os', baseline='mt_m<40 && anti_e_t && anti_m_t && chiso_t < 1.0 && dm_t==0', wt='wt')
qcd = sel.copy(sign='!os')

print sel()
print qcd()

ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
ana.AddSamples('output/HTT2016Studies_Aug16/*.root', 'ZmtTP')
ana.AddInfo('params_Aug16.json', scaleTo='data_obs')
# missing OS!

vv_samples = ['VVTo2L2Nu', 'WWTo1L1Nu2Q', 'WZJToLLLNu',
    'WZTo1L1Nu2Q', 'WZTo1L3Nu', 'WZTo2L2Q', 'ZZTo2L2Q', 'ZZTo4L']

for var in ['chiso_t(50,0,20)',
            'cbiso_t(50,0,20)',
            'ntiso_t(50,0,20)',
            'puiso_t(50,0,20)',
            'pho_out_t(50,0,20)',
            'pho_out_t/pt_t(50,0,1)',
            'm_ll(50,0,250)',
            'n_iso_ph_0p5(20,-0.5,19.5)',
            'n_iso_ph_1p0(20,-0.5,19.5)',
            'n_iso_ph_1p5(20,-0.5,19.5)',
            'n_iso_ph_2p0(20,-0.5,19.5)',
            'n_sig_ph_0p5(20, 0.5,20.5)',
            'n_sig_ph_1p0(20, 0.5,20.5)',
            'n_sig_ph_1p5(20, 0.5,20.5)',
            'n_sig_ph_2p0(20, 0.5,20.5)',
            ('n_sig_ph_0p5+n_iso_ph_0p5(25,-0.5,24.5)', 'n_sum_ph_0p5'),
            ('n_sig_ph_1p0+n_iso_ph_1p0(25,-0.5,24.5)', 'n_sum_ph_1p0'),
            ('n_sig_ph_1p5+n_iso_ph_1p5(25,-0.5,24.5)', 'n_sum_ph_1p5'),
            ('n_sig_ph_2p0+n_iso_ph_2p0(25,-0.5,24.5)', 'n_sum_ph_2p0'),
            ]:
    if isinstance(var, tuple) and len(var) == 1:
        nodename = var[0].split('(')[0]
        v = var[0]
    elif isinstance(var, tuple) and len(var) == 2:
        nodename = var[1]
        v = var[0]
    else:
        nodename = var.split('(')[0]
        v = var
    ana.nodes.AddNode(ListNode(nodename))


    nodes = ana.nodes[nodename]
    nodes.AddNode(BasicNode('data_obs', 'data_obs', v, sel()))
    nodes.AddNode(ana.BasicFactory(
        'ZTT', 'DYJetsToLL', v, sel(extra='gen_2==5')))
    nodes.AddNode(ana.BasicFactory(
        'ZL', 'DYJetsToLL', v, sel(extra='gen_2<5')))
    nodes.AddNode(ana.BasicFactory(
        'ZJ', 'DYJetsToLL', v, sel(extra='gen_2==6')))
    nodes.AddNode(ana.BasicFactory('W', 'WJetsToLNu', v, sel()))
    nodes.AddNode(ana.BasicFactory('TT', 'TT', v, sel()))
    nodes.AddNode(ana.SummedFactory('VV', vv_samples, v, sel()))
    nodes.AddNode(SubtractNode('QCD',
        ana.BasicFactory('data_obs', 'data_obs', v, sel(sign='!os')),
        ana.SummedFactory(
            'backgrounds', ['DYJetsToLL', 'WJetsToLNu', 'TT'], v, sel(sign='!os'))
      ))
    nodes['QCD'].subtract_node.AddNode(ana.SummedFactory('VV', vv_samples, v, sel(sign='!os')))

ana.nodes.PrintTree()
ana.Run()

outfile = ROOT.TFile('TauID_w_chiso_lt_1p0_dm0.root', 'RECREATE')

ana.nodes.Output(outfile)
