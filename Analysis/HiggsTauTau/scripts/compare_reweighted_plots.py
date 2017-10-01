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
parser.add_argument('--file2', help= 'Input file containing reweighted histograms to compare to.')
parser.add_argument('--cp_channel', '-c', help= 'cp_channel', type=int)
parser.add_argument('--output_name', '-o', help= 'Name of output.')
parser.add_argument('--title', '-t', help= 'Title')
args = parser.parse_args()
filename = args.file
filename2 = args.file2
cp_channel = args.cp_channel
output_name = args.output_name
if args.title: title = args.title
else: title = ""

f = ROOT.TFile(filename)
f2 = ROOT.TFile(filename2)
hist_names = ['odd_1', 'none_1']
if cp_channel>1: hist_names = ['odd_1', 'none_1', 'odd_2', 'none_2']
leg_titles=['CP=+1 [SM H(125 GeV)]', 'CP=-1 [SUSY A(120 GeV)]']
for name in hist_names:
  h1 = f.Get(name)    
  h2 = f2.Get(name)
  hists=[h1,h2]
  if name in ['odd_1', 'odd_2']: leg_titles = ['CP=-1 [SUSY A(120 GeV)]', 'CP=-1 [SM H(125 GeV)]']
  if name in ['none_1', 'none_2']: leg_titles = ['CP=0 [DY]', 'CP=0 [SM H(125 GeV)]']
  
  plot_name = output_name
  if name in ['odd_1', 'odd_2']: plot_name += "_CPodd_comp"
  if name in ['none_1', 'none_2']: plot_name += "_CPnone_comp"
  if name in ['odd_1', 'none_1']: plot_name+="_sign_1"
  elif name in ['odd_2', 'none_2']: plot_name+="_sign_2"

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
         label="",
         draw_options=['p E2','l'])
