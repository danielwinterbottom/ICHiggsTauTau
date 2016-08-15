import ROOT
from UserCode.ICHiggsTauTau.analysis import *

ROOT.TH1.SetDefaultSumw2(True)

ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
ana.AddSamples('output/HTT2016Studies_Aug5/*.root', 'ZmtTP')
ana.AddInfo('params_Aug5.json', scaleTo='data_obs')
# missing OS!
sel = '(os && mt_m<40 && tight_t && anti_e_t && anti_m_t%s) * wt'
qcd = '(!os && mt_m<40 && tight_t && anti_e_t && anti_m_t%s) * wt'

vv_samples = ['VVTo2L2Nu', 'WWTo1L1Nu2Q', 'WZJToLLLNu', 'WZTo1L1Nu2Q', 'WZTo1L3Nu', 'WZTo2L2Q', 'ZZTo2L2Q', 'ZZTo4L']

ana.nodes.AddNode(ListNode('pass'))
nodes = ana.nodes['pass']
var = 'm_ll(25,0,250)'
nodes.AddNode(BasicNode('data_obs', 'data_obs', var, sel % ''))
nodes.AddNode(ana.BasicFactory('ZTT', 'DYJetsToLL', var, sel % ' && gen_2==5'))
nodes.AddNode(ana.BasicFactory('ZL', 'DYJetsToLL', var, sel % ' && gen_2<5'))
nodes.AddNode(ana.BasicFactory('ZJ', 'DYJetsToLL', var, sel % ' && gen_2==6'))
nodes.AddNode(ana.BasicFactory('W', 'WJetsToLNu', var, sel % ''))
nodes.AddNode(ana.BasicFactory('TT', 'TT', var, sel % ''))
nodes.AddNode(ana.SummedFactory('VV', vv_samples, var, sel % ''))
nodes.AddNode(SubtractNode('QCD',
    ana.BasicFactory('data_obs', 'data_obs', var, qcd % ''),
    ana.SummedFactory('backgrounds', ['DYJetsToLL', 'WJetsToLNu', 'TT'], var, qcd % '')
  ))

ana.nodes.PrintTree()
ana.Run()

outfile = ROOT.TFile('TauID.root', 'RECREATE')

ana.nodes.Output(outfile)

