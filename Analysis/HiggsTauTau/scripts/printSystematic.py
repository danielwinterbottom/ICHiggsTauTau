#!/usr/bin/env python
import os
import ROOT
import sys
import math
from optparse import OptionParser

def split_callback(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("-i", dest="input",
                  help="The datacard input file")

parser.add_option("-s", dest="systematic",
                  help="Name of the systematic uncertainty (no leading underscore)")

parser.add_option("-p", dest="processes", type='string', action='callback', callback=split_callback,
                  help="Which processes to look for")


(options, args) = parser.parse_args()

f = ROOT.TFile(options.input)
if f:
  for key in f.GetListOfKeys():
    if key.IsFolder(): 
      print '\n'
      print key.GetName()
      ROOT.gDirectory.cd("/"+key.GetName())
      print '%-10s %-19s %-19s' % ("Process","Down","Up")
      for pr in options.processes:
        h_central = ROOT.gDirectory.Get(pr)
        if not h_central: continue
        y_central = h_central.Integral(0, h_central.GetNbinsX()+1)
        e_central = ROOT.Double(0.0)
        h_central.IntegralAndError(0, h_central.GetNbinsX()+1, e_central)
        neff_central = (y_central/e_central)**2

        h_down = ROOT.gDirectory.Get(pr+"_"+options.systematic+"Down")
        if not h_down: continue
        y_down = h_down.Integral(0, h_down.GetNbinsX()+1)

        h_up = ROOT.gDirectory.Get(pr+"_"+options.systematic+"Up")
        if not h_up: continue
        y_up = h_up.Integral(0, h_up.GetNbinsX()+1)

        up_ratio = y_up/y_central
        up_err = ((math.fabs(y_up-y_central)*(neff_central/y_central))**0.5)/neff_central;
        down_ratio = y_down/y_central;
        down_err = ((math.fabs(y_down-y_central)*(neff_central/y_central))**0.5)/neff_central;
        print '%-10s %-5.2f +/- %-5.2f     %-5.2f +/- %-5.2f' % (pr,down_ratio,down_err,up_ratio,up_err)










