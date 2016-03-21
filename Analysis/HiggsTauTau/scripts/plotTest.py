#!/usr/bin/env python
import ROOT
import CombineHarvester.CombineTools.plotting as plot
# from UserCode.ICHiggsTauTau.uncertainties import ufloat
import UserCode.ICHiggsTauTau.MultiDraw

from time import time

def Rate(hist):
    err = ROOT.Double()
    integral = hist.IntegralAndError(0, hist.GetNbinsX()+1, err)
    return ufloat(integral, err)

# ## Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()

file = ROOT.TFile('WJetsNLO.root')
tree = file.Get('qcdstudy')

draw_lo = [("0.5(1,0,1)", "(mt_1<30)*wt*scale_wt_%i" % x) for x in range(9)]
draw_hi = [("0.5(1,0,1)", "(mt_1>70)*wt*scale_wt_%i" % x) for x in range(9)]


# draw = [
#     ("mt_1(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("flavour_2(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("pt_1(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("pt_2(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("eta_1(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("eta_2(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("iso_1(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5"),
#     ("iso_2(25,-0.5,24.5)", "iso_1<0.1 && iso_2<1.5")]

start = time()
res_lo = tree.MultiDraw(draw_lo)  # all in one go
res_hi = tree.MultiDraw(draw_hi)  # all in one go
print "Total %.2fs" % (time() - start), " " * 20

print res

outfile = ROOT.TFile('output.root', 'RECREATE')

for h in res:
    outfile.WriteTObject(h)

    print h.GetName(), h.Integral(h.FindFixBin(0.), h.FindFixBin(30.)) / h.Integral(h.FindFixBin(70.), h.FindFixBin(500.))

outfile.Close()

# start = time()
# for ele in draw:  # or one at a time
#     res = tree.MultiDraw([ele])
# print "Total %.2fs" % (time() - start), " " * 20
