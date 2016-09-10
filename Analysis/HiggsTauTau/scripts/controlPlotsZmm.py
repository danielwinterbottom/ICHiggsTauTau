import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat

ROOT.TH1.SetDefaultSumw2(True)
# h1 = ROOT.TH1F('hist', '', 100, -10, 10)
# h1.FillRandom('gaus', 1000)

# h2 = ROOT.TH1F('hist', '', 100, -10, 10)
# h2.FillRandom('gaus', 100)

# s1 = Shape(h1)
# s2 = Shape(h2)

ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
ana.AddSamples('output/HTT2016Studies_Aug16/Zmm/*.root', 'Zmm')
ana.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')
vv_samples = ['VVTo2L2Nu', 'WWTo1L1Nu2Q', 'WZJToLLLNu',
     'WZTo1L1Nu2Q', 'WZTo1L3Nu', 'WZTo2L2Q', 'ZZTo2L2Q', 'ZZTo4L']

sel = '(os && pt_1>23 && pt_2>20 && iso_1<0.15 && iso_2<0.15 && trg_IsoMu22) * wt'
for plot in ['m_ll(60,70,130)', 'pt_1(50,0,200)', 'pt_2(50,0,200)', 'eta_1(50,-2.4,2.4)', 'eta_2(50,-2.4,2.4)', 'mvamet_et(40,0,400)']:
    nodename = plot.split('(')[0]
    ana.nodes.AddNode(ListNode(nodename))
    ana.nodes[nodename].AddNode(ana.BasicFactory('data_obs', 'data_obs', plot, sel))
    ana.nodes[nodename].AddNode(ana.BasicFactory('ZLL', 'DYJetsToLL', plot, sel))
    ana.nodes[nodename].AddNode(ana.BasicFactory('TT', 'TT', plot, sel))
    ana.nodes[nodename].AddNode(ana.SummedFactory('VV', vv_samples, plot, sel))
ana.Run()
ana.nodes.PrintTree()

outfile = ROOT.TFile('test.root', 'RECREATE')

ana.nodes.Output(outfile)

