import ROOT
#import os
#import glob
#import json
#from UserCode.ICHiggsTauTau.analysis import *
#from UserCode.ICHiggsTauTau.uncertainties import ufloat
#from optparse import OptionParser
#import argparse
#import ConfigParser
import UserCode.ICHiggsTauTau.plotting as plotting
#from collections import OrderedDict
import copy

dirname='/vols/cms/dw515/Offline/output/MSSM/gen_comps/'

samples={"official":"bbH700_official_mt_2016.root", "private":"bbH700_private_mt_2016.root", "OldAcc":"bbH700_OldAcc_mt_2016.root", "NewAcc":"bbH700_NewAcc_mt_2016.root"}

#var='n_bjets'
#binning='(4,0,4)'
#cuts='1'
#in_str='n_bjets:(4,0,4):(1):N_{b-jets}:n_bjets'
#in_str='mt_1:(10,0,100):(higgsDecay==\"et\"||higgsDecay==\"mt\"):m_{T} (GeV):mt_1'
#in_str='pt_1:(20,0,200):(higgsDecay==\"et\"||higgsDecay==\"mt\"):P_{T}^{e/#mu} (GeV):pt_1_l'
#in_str='pt_2:(20,0,400):(higgsDecay==\"et\"||higgsDecay==\"mt\"):P_{T}^{#tau} (GeV):pt_2_t_semileptonic'
#in_str='pt_1:(20,0,400):(higgsDecay==\"tt\"):P_{T}^{#tau_{1}} (GeV):pt_1_t_hardonic'
#in_str='pt_2:(20,0,400):(higgsDecay==\"tt\"):P_{T}^{#tau_{2}} (GeV):pt_2_t_hardonic'
#in_str='mt_tot:(20,0,800):(higgsDecay==\"tt\"):m_{T}^{tot} (GeV):mt_tot_hardonic'
#in_str='mt_tot:(20,0,800):(higgsDecay==\"mt\"||higgsDecay==\"et\"):m_{T}^{tot} (GeV):mt_tot_semileptonic'
#in_str='met:(20,0,200):(1):MET (GeV):met'
#in_str='eta_1:(20,-3,3):(higgsDecay==\"mt\"||higgsDecay==\"et\"):#eta^{e/#mu}:eta_1_l_semileptonic'
#in_str='eta_2:(20,-3,3):(higgsDecay==\"mt\"||higgsDecay==\"et\"):#eta^{#tau}:eta_2_t_semileptonic'
#in_str='eta_1:(20,-3,3):(higgsDecay==\"tt\"):#eta^{#tau_{1}}:eta_1_t_hadronic'
#in_str='eta_2:(20,-3,3):(higgsDecay==\"tt\"):#eta^{#tau_{2}}:eta_2_t_hadronic'
#in_str='(wt):(2,-1,1):(1):MC weight:weight'
var=in_str.split(':')[0]
binning=in_str.split(':')[1]
cuts=in_str.split(':')[2]
x_title=in_str.split(':')[3]
out_name=in_str.split(':')[4]

hists = []
legend_titles = []

#sf = {}
#
#for samp in samples:
#  file_name=dirname+samples[samp]
#  f = ROOT.TFile(file_name)
#  t = f.Get('gen_ntuple')
#  t.Draw(var+'>>h'+binning,'wt','goff')
#  h = ROOT.gDirectory.Get("h")
  
for samp in samples:
  file_name=dirname+samples[samp]
  f = ROOT.TFile(file_name)
  t = f.Get('gen_ntuple')
  t.Draw(var+'>>h'+binning,'wt*('+cuts+')','goff')
  #
  #print "sample", "N +ve", "N -ve", "Ratio"
  pos = t.GetEntries("wt>0")
  neg = t.GetEntries("wt<0")
  import math
  error = math.sqrt(1./pos + 1./neg)
  #print samp, pos,"+/-", math.sqrt(pos), neg, "+/-", math.sqrt(pos), float(neg)/pos,"+/-", error
  print samp, float(neg)/pos,"+/-", error*float(neg)/pos

  continue
  #
  h = ROOT.gDirectory.Get("h")
  if samp == "NewAcc":
    h.SetName("NewAcc_uncert")
    hist = h.Clone()
    hists.append(copy.deepcopy(hist))
    legend_titles.append("NewAcc_uncert")
    h.SetName(samp)
    hist = h.Clone()
    for i in range(1,hist.GetNbinsX()+1): hist.SetBinError(i,0.0001)
    hists.append(copy.deepcopy(hist))
    legend_titles.append(samp)
    
  else:     
    h.SetName(samp)
    hist = h.Clone()
    hists.append(copy.deepcopy(hist))
    legend_titles.append(samp)

plotting.CompareNHists(hists=hists,
             legend_titles=legend_titles,
             title="",
             ratio=True,
             log_y=False,
             log_x=False,
             ratio_range="0.85,1.15",
             custom_x_range=False,
             x_axis_max=4000,
             x_axis_min=0,
             custom_y_range=False,
             y_axis_max=4000,
             y_axis_min=0,
             x_title=x_title,
             y_title="Normalized Events",
             extra_pad=0.5,
             norm_hists=True,
             plot_name="gen_comps/"+out_name,
             label="",
             draw_options=None)
