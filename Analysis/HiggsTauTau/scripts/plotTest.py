#!/usr/bin/env python
import ROOT
import CombineHarvester.CombineTools.plotting as plot
# from UserCode.ICHiggsTauTau.uncertainties import ufloat
import UserCode.ICHiggsTauTau.MultiDraw

from time import time

# ## Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

file = ROOT.TFile('QCD.root')
tree = file.Get('qcdstudy')

draw = [
    ("flavour_1 >> a(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("flavour_2 >> b(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("pt_1 >> c(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("pt_2 >> d(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("eta_1 >> e(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("eta_2 >> f(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("iso_1 >> g(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
    ("iso_2 >> h(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5")]

start = time()
res = tree.MultiDraw(draw)  # all in one go
print "Total %.2fs" % (time() - start), " " * 20


start = time()
for ele in draw:  # or one at a time
    res = tree.MultiDraw([ele])
print "Total %.2fs" % (time() - start), " " * 20
