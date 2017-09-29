import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser
import argparse
import ConfigParser
import UserCode.ICHiggsTauTau.plotting as plotting
from collections import OrderedDict
import copy

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'Input file containing hitograms to make plots from.')
parser.add_argument('--cp_channel', '-c', help= 'cp_channel', type=int)
parser.add_argument('--output_name', '-o', help= 'Name of output.')
parser.add_argument('--title', '-t', help= 'Title')
args = parser.parse_args()
filename = args.file
cp_channel = args.cp_channel
output_name = args.output_name
if args.title: title = args.title
else: title = ""

f = ROOT.TFile(filename)
h1 = f.Get('even_1')
h2 = f.Get('odd_1')
h3 = f.Get('none_1')
hists=[h1,h2,h3]
plot_name = output_name
if cp_channel>1: plot_name+="_sign_1"
leg_titles=['CP=+1 [SM H(125 GeV)]', 'CP=-1 [SUSY A(120 GeV)]', 'DY']

plotting.CompareHists(hists,
         legend_titles=leg_titles,
         title=title,
         ratio=False,
         log_y=False,
         log_x=False,
         ratio_range="0.7,1.3",
         custom_x_range=False,
         x_axis_max=4000,
         x_axis_min=0,
         custom_y_range=False,
         y_axis_max=4000,
         y_axis_min=0,
         x_title="#phi_{CP}^{*} (radians)",
         y_title="Normalized Events",
         extra_pad=0,
         norm_hists=True,
         plot_name=plot_name,
         label="")

if cp_channel>1:
  h1 = f.Get('even_2')
  h2 = f.Get('odd_2')
  h3 = f.Get('none_2')
  hists=[h1,h2,h3]
  plot_name = output_name
  if cp_channel>1: plot_name+="_sign_2"
  leg_titles=['CP=+1 [SM H(125 GeV)]', 'CP=-1 [SUSY A(120 GeV)]', 'DY']
  
  plotting.CompareHists(hists,
           legend_titles=leg_titles,
           title=title,
           ratio=False,
           log_y=False,
           log_x=False,
           ratio_range="0.7,1.3",
           custom_x_range=False,
           x_axis_max=4000,
           x_axis_min=0,
           custom_y_range=False,
           y_axis_max=4000,
           y_axis_min=0,
           x_title="#phi_{CP}^{*} (radians)",
           y_title="Normalized Events",
           extra_pad=0,
           norm_hists=True,
           plot_name=plot_name,
           label="")