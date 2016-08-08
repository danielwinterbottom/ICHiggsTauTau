import ROOT
import os
import glob
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat

# h1 = ROOT.TH1F('hist', '', 100, -10, 10)
# h1.FillRandom('gaus', 1000)

# h2 = ROOT.TH1F('hist', '', 100, -10, 10)
# h2.FillRandom('gaus', 100)

# s1 = Shape(h1)
# s2 = Shape(h2)


def AddSamples(ana, dir, tree):
    files = glob.glob(dir)
    for f in files:
        testf = ROOT.TFile(f)
        if testf.Get(tree) != None:
            name = os.path.splitext(os.path.basename(f))[0]
            ana.trees[name] = TTreeEvaluator(tree, f)
        testf.Close()


ana = Analysis()
AddSamples(ana, 'output/HTT2016Studies_July19/*.root', 'Zmm')
for plot in ['m_ll(60,70,130)', 'pt_1(50,0,200)']:
    nodename = plot.split('(')[0]
    ana.nodes.AddNode(ListNode(nodename))
    for sa in ana.trees:
        ana.nodes[nodename].AddNode(BasicNode(sa, sa, plot, '(os && pt_1>20 && pt_2>20 && iso_1<0.15 && iso_2<0.15 && trg_IsoMu22) * wt'))
ana.nodes.PrintTree()
ana.Run()

outfile = ROOT.TFile('test.root', 'RECREATE')

ana.nodes.Output(outfile)

