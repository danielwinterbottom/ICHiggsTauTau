import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import re
import os
import time
import json
from array import array
import argparse
ROOT.gROOT.SetBatch(1)

ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)

parser = argparse.ArgumentParser()
parser.add_argument('--wp',help= 'Tau ID working point to measure fake factors for', default='medium')
parser.add_argument('--file_ext',help= 'Extension of files names', default='_et_2018.root')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='mvadm_ff_deeptauV2p1_2018_et_ip0p5')
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_2018.json')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default='/vols/cms/dw515/Offline/output/SM/FF_2018/')
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
args = parser.parse_args()

wp = args.wp
file_ext = args.file_ext
output_folder=args.output_folder
params_file=args.params
input_folder=args.input_folder
draw = int(args.draw) > 0
lumi=58826.8469

out_file = '%(output_folder)s/fakefactor_fits_et_%(wp)s_2018.root' % vars()

# read params from json

with open(params_file) as jsonfile:
  params = json.load(jsonfile)

data_files = [
  'EGammaA',
  'EGammaB',
  'EGammaC',
  'EGammaD'
]

ttbar_files = [
  'TTTo2L2Nu',
  'TTToHadronic',
  'TTToSemiLeptonic'
]

wjets_files = [
  'W1JetsToLNu-LO',
  'W2JetsToLNu-LO',
  'W3JetsToLNu-LO',
  'W4JetsToLNu-LO',
  'WJetsToLNu-LO',
  'EWKWMinus2Jets',
  'EWKWPlus2Jets'
]

other_files = [
  'EWKZ2Jets',
  'T-tW-ext1',
  'T-t',
  'DY1JetsToLL-LO',
  'DY2JetsToLL-LO',
  'DY3JetsToLL-LO',
  'DY4JetsToLL-LO',
  'DYJetsToLL-LO',
  'DYJetsToLL_M-10-50-LO',
  'Tbar-tW-ext1',
  'Tbar-t',
  'WWToLNuQQ',
  'WWTo2L2Nu',
  'WZTo1L3Nu',
  'WZTo2L2Q',
  'WZTo3LNu-ext1',
  'WZTo3LNu',
  'ZZTo2L2Nu-ext1',
  'ZZTo2L2Nu-ext2',
  'ZZTo2L2Q',
  'ZZTo4L-ext',
  'ZZTo4L'
]


njets_bins = {
              'inclusive': '(1)',
              'njets0':'n_jets==0 && trg_singleelectron && pt_1>33',
              'njets1':'n_jets==1 && trg_singleelectron & pt_1>33',
              'njets2':'n_jets>1 && trg_singleelectron & pt_1>33',
              'njets0_crosstrg':'n_jets==0 && (trg_singleelectron && pt_1>33)==0',
              'njets1_crosstrg':'n_jets==1 && (trg_singleelectron && pt_1>33)==0',
              'njets2_crosstrg':'n_jets>1 && (trg_singleelectron && pt_1>33)==0'
}
dm_bins = {
              'inclusive': '(1)',
              'dm0':'(tau_decay_mode_2==0)',
              'dm1':'(tau_decay_mode_2==1)',
              'dm10':'(tau_decay_mode_2==10)',
              'dm11':'(tau_decay_mode_2==11)',
              'mvadm0':'(mva_dm_2==0)',
              'mvadm0_sig_gt3':'(mva_dm_2==0&&ip_sig_2>=1.5)',
              'mvadm0_sig_lt3':'(mva_dm_2==0&&ip_sig_2<1.5)',
              'mvadm1':'(mva_dm_2==1)',
              'mvadm2':'(mva_dm_2==2)',
              'mvadm10':'(mva_dm_2==10)',
              'mvadm11':'(mva_dm_2==11)'
}

# choose bins to set to pol1 and pol0 here:
#fit_pol1_qcd   = ['mvadm0_sig_lt3_njets0','mvadm0_sig_lt3_njets2','mvadm1_njets2','dm0_njets2']
#fit_pol1_wjets = ['mvadm11_njets2']
#fit_pol1_ttbar = []
#fit_pol0_qcd   = ['dm10_njets2_crosstrg']
#fit_pol0_wjets = ['mvadm11_njets2','mvadm0_sig_lt3_njets2','mvadm0_sig_lt3_njets2_crosstrg','mvadm0_sig_gt3_njets2_crosstrg','mvadm1_njets2_crosstrg','mvadm2_njets2_crosstrg','dm0_njets1_crosstrg','dm0_njets2_crosstrg','dm1_njets2_crosstrg','dm11_njets2_crosstrg']
#fit_pol0_ttbar = []

fit_pol1_qcd   = [
#'mvadm1_njets1',
#'mvadm1_njets2',
#'mvadm11_njets1',
#'mvadm11_njets2',
]
fit_pol1_wjets = []
fit_pol1_ttbar = []
fit_pol0_qcd   = [
'mvadm0_sig_lt3_njets2_crosstrg',
'mvadm0_sig_lt3_njets2',
'mvadm0_sig_gt3_njets2_crosstrg',
'mvadm0_sig_gt3_njets2',
'mvadm0_sig_gt3_njets1',
'mvadm0_sig_lt3_njets1',
#'mvadm1_njets0_crosstrg',
#'mvadm1_njets2_crosstrg',
#'mvadm2_njets1_crosstrg',
#'mvadm11_njets1_crosstrg',
#'mvadm11_njets2_crosstrg',
]
fit_pol0_wjets = [
'mvadm0_sig_lt3_njets2_crosstrg',
'mvadm0_sig_lt3_njets2',
'mvadm0_sig_gt3_njets2_crosstrg',
'mvadm0_sig_gt3_njets2',
#'mvadm0_njets2_crosstrg',
#'mvadm0_sig_gt3_njets2_crosstrg',
#'mvadm0_sig_lt3_njets2_crosstrg',
#'mvadm1_njets2_crosstrg',
#'mvadm2_njets1_crosstrg',
#'mvadm2_njets2_crosstrg',
#'mvadm11_njets2_crosstrg',
]
fit_pol0_ttbar = []

def Draw2DQCDHist(var_input1, var_input2, cuts, name, input_folder, file_ext,doOS=False,add_wt='1'):
  var1 = var_input1.split('[')[0]
  var2 = var_input2.split('[')[0]
  bins1 = array('f', map(float,var_input1.split('[')[1].split(']')[0].split(',')))
  bins2 = array('f', map(float,var_input2.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH2D('hout','',len(bins1)-1, bins1,len(bins2)-1, bins2)
  hout.GetXaxis().SetTitle(var1)
  hout.GetYaxis().SetTitle(var2)

  data = hout.Clone()
  data.SetName(name+'_qcd')
  bkgs = hout.Clone()
  bkgs.SetName(name+'_bkgs')

  # draw data
  for i in data_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    if not doOS:
      t.Draw('%(var2)s:%(var1)s>>h' % vars(),'((%(cuts)s)*((os==0)))*(%(add_wt)s)' % vars(),'goff')
    else:
      t.Draw('%(var2)s:%(var1)s>>h' % vars(),'((%(cuts)s)*((os==1)))*(%(add_wt)s)' % vars(),'goff')

    h = t.GetHistogram()
    data.Add(h)

  for i in other_files+wjets_files+ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    if not doOS:
      t.Draw('%(var2)s:%(var1)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(add_wt)s)' % vars(),'goff')
    else:
      t.Draw('%(var2)s:%(var1)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(add_wt)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)

  data.Add(bkgs,-1)

  return data

def ZeroNegativeBins(h):
  # if a bin of h1 is equal to the -ve of 
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=True,doTT=True,doIso=False,fullMT=False,lowMT=False):
  if ':' in var_input:
    #pass 2D inputs like x[],y[]
    var_input1 = var_input.split(':')[0]
    var_input2 = var_input.split(':')[1]
    var1 = var_input1.split('[')[0]
    var2 = var_input2.split('[')[0]

    bins1 = array('f', map(float,var_input1.split('[')[1].split(']')[0].split(',')))
    bins2 = array('f', map(float,var_input2.split('[')[1].split(']')[0].split(',')))
    hout = ROOT.TH2D('hout','',len(bins1)-1, bins1,len(bins2)-1, bins2)
    hout.GetXaxis().SetTitle(var1)
    hout.GetYaxis().SetTitle(var2)
    var='%(var2)s:%(var1)s' % vars()
  else:
    var = var_input.split('[')[0]
    bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
    hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_2==6)'

  data_qcd = hout.Clone()
  data_qcd.SetName(name+'_qcd')
  data_w = hout.Clone()
  data_w.SetName(name+'_wjets')
  wjets = hout.Clone()
  wjets.SetName(name+'_wjets_mc')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar_mc')
  bkgs_qcd = hout.Clone()
  bkgs_qcd.SetName(name+'_bkgs_qcd')
  bkgs_w = hout.Clone()
  bkgs_w.SetName(name+'_bkgs_wjets')
  w_qcd_sub = hout.Clone()
  w_qcd_sub.SetName(name+'_qcd_bkgs_wjets')

  w_qcd_sub_mc = hout.Clone()
  w_qcd_sub_mc.SetName(name+'_qcd_mc_bkgs_wjets')

  if doQCD:
    # draw data for QCD
    if not doIso:  qcd_cuts = 'mt_1<50&&iso_1>0.05'
    else: qcd_cuts = 'mt_1<50'
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      if not doOS:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')
      else:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      data_qcd.Add(h)

    # draw all backgrounds to be subtracted from data for QCD estimation
    # only subtract non j->tau fake component
    for i in other_files+wjets_files+ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      if not doOS:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')
      else:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      bkgs_qcd.Add(h)

    bkgs_qcd = ZeroNegativeBins(bkgs_qcd)
    data_qcd.Add(bkgs_qcd,-1)
    data_qcd = ZeroNegativeBins(data_qcd)

  if fullMT: mt_cut='1'
  elif lowMT: mt_cut='mt_1<50'
  else: mt_cut='mt_1>70'

  if doW:
    # draw data for W+jets
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      data_w.Add(h)

    # draw all (non-QCD) backgrounds to be subtracted from data for W estimation
    # only subtract non j->tau fake component
    for i in other_files+ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      bkgs_w.Add(h)


    # draw data for QCD subtraction
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      w_qcd_sub.Add(h)

    # draw MC for QCD subtraction
    for i in other_files+ttbar_files+wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      w_qcd_sub_mc.Add(h)

    w_qcd_sub_mc = ZeroNegativeBins(w_qcd_sub_mc)
    w_qcd_sub.Add(w_qcd_sub_mc,-1)
    w_qcd_sub = ZeroNegativeBins(w_qcd_sub)
    # scale by OS/SS ratio
    w_qcd_sub.Scale(1.1)

    bkgs_w = ZeroNegativeBins(bkgs_w)
    data_w.Add(bkgs_w,-1)
    data_w.Add(w_qcd_sub,-1)
    data_w = ZeroNegativeBins(data_w)

  if doMC:
    # draw wjets when tau candidate is a jet fake
    for i in wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      wjets.Add(h)

    if doTT:
      # draw ttbar when tau candidate is a jet fake 
      for i in ttbar_files:
        f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
        t = f.Get('ntuple')
        h = hout.Clone()
        h.SetName('h')
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(os==1)*(mt_1<50)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
        h = t.GetHistogram()
        scale = lumi*params[i]['xs']/params[i]['evt']
        h.Scale(scale)
        ttbar.Add(h)

    wjets = ZeroNegativeBins(wjets)
    ttbar = ZeroNegativeBins(ttbar)

  return (data_qcd, data_w, wjets, ttbar)

def DrawHistsForFractions(var_input, cuts, name, input_folder, file_ext):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_2==6)'
  data = hout.Clone()
  data.SetName(name+'_qcd')
  wjets = hout.Clone()
  wjets.SetName(name+'_wjets')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar')
  bkgs = hout.Clone()
  bkgs.SetName(name+'_bkgs')

  # draw data
  for i in data_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)' % vars(),'goff')
    h = t.GetHistogram()
    data.Add(h)

  # draw wjets when the tau candidates is a jet fake - this includes VV and DY events also
  for i in wjets_files+other_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    wjets.Add(h)

  # draw ttbar when the tau candidates is a jet fake 
  for i in ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    ttbar.Add(h)

  # draw all backgrounds with no jet fake
  for i in other_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)
  # need to add also the ttbar and W+jet events without any fakes
  for i in wjets_files+ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*((%(gen_extra)s)==0)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)

  # subtract other processes to get QCD
  data.Add(bkgs,-1); data.Add(wjets,-1); data.Add(ttbar,-1)

  # fix any negative bins to 0 
  for i in range(1,data.GetNbinsX()+1):
    if data.GetBinContent(i) < 0: data.SetBinContent(i,0.)
    if ttbar.GetBinContent(i) < 0: ttbar.SetBinContent(i,0.)
    if wjets.GetBinContent(i) < 0: wjets.SetBinContent(i,0.)

  return (data, wjets, ttbar)


def CalculateFakeFactors(num,denum):
  name = num.GetName().replace('_iso','_ff')
  ff = num.Clone()
  ff.SetName(name)
  ff.Divide(denum)
  return ff

def FitFakeFactors(h,usePol1=False,polOnly=None):
  h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
  f1 = ROOT.TF1("f1","landau",20,200)
  f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",20,200)
  if usePol1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",20,200)
  if polOnly is not None:
    if polOnly == 0:
      f1 = ROOT.TF1("f1","pol0",20,200)
      f2 = ROOT.TF1("f2","pol0",20,200)
    if polOnly == 1:
      f1 = ROOT.TF1("f1","pol1",20,200)
      f2 = ROOT.TF1("f2","pol1",20,200)

  # clone histogram and set all bins with >0 content
  h_clone = h.Clone()
  h_clone.Reset()
  for i in range(1,h.GetNbinsX()+1):
    content = h.GetBinContent(i)
    error = h.GetBinError(i)
    if content>0.:
      h_clone.SetBinContent(i,content)
      h_clone.SetBinError(i,error)
  h = h_clone
  if polOnly is None:
    # fit first with landau to get initial values for parameters - pol values set to 0 initially
    h.Fit("f1",'IR')
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
    if usePol1: f2.SetParameter(4,0)
  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    fitresult = h.Fit("f2",'SIR')
    rep = int(fitresult) != 0
    if not rep or count>100:
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')
  return fit, h_uncert, h

def FitCorrection(h, func='pol1',is2D=False):
  if is2D: 
    h_uncert = ROOT.TH2D(h.GetName()+'_uncert',"",100,h.GetXaxis().GetBinLowEdge(1),h.GetXaxis().GetBinLowEdge(h.GetNbinsX()+1),100,h.GetYaxis().GetBinLowEdge(1),h.GetYaxis().GetBinLowEdge(h.GetNbinsY()+1))
    f1 = ROOT.TF2("f1",func)
  else: 
    h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
    f1 = ROOT.TF1("f1",func)

  if 'Erf' in func:
    f1.SetParameter(1,-1.5)
    f1.SetParameter(2,2)

  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    if 'Erf' in func: fitresult = h.Fit("f1",'S')
    else: fitresult = h.Fit("f1",'SI') 
    rep = int(fitresult) != 0
    if not rep or count>100:
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f1
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')
  return fit, h_uncert


def PlotFakeFactor(f, h, name, output_folder, wp):
  c1 = ROOT.TCanvas()
  f.SetMinimum(0)
  if f.GetMaximum() > 1.: f.SetMaximum(1.)
  f.SetStats(0)
  f.GetXaxis().SetTitle('p_{T} (GeV)')
  f.GetYaxis().SetTitle('FF')
  f.SetTitle(name)
  f.SetLineColor(ROOT.kBlack)
  f.Draw()
  h.SetStats(0)
  h.SetFillColor(ROOT.kBlue-10)
  h.Draw("e3 same")
  f.Draw("a sames")
  c1.Print(output_folder+'/et_'+wp+'_'+name+'_fit.pdf')

def PlotFakeFactorCorrection(f, h, name, output_folder, wp,x_title='E_{T}^{miss} (GeV)'):
  c1 = ROOT.TCanvas()
  f.SetMinimum(0)
  f.SetStats(0)
  f.GetXaxis().SetTitle(x_title)
  f.GetYaxis().SetTitle('FF')
  f.SetTitle(name)
  f.SetLineColor(ROOT.kBlack)
  f.Draw()
  h.SetStats(0)
  h.SetFillColor(ROOT.kBlue-10)
  h.Draw("e3 same")
  f.Draw("a sames")
  c1.Print(output_folder+'/et_'+wp+'_'+name+'_fit.pdf')

def WriteFunctionTTbar(fout,proc='ttbar_mc',aiso=False):
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for mvadmbin in ['0_sig_lt3','0_sig_gt3','1','2','10','11']:
    fout.cd()
    extra=''
    if aiso: extra = '_aiso2_ss'
    f = fout.Get('mvadm%(mvadmbin)s_inclusive%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
    p = f.GetParameters()
    if f.GetNpar()==1:
      ff_params['mvadm%(mvadmbin)s' % vars()] = ff_pol0.replace('p0','%f' % p[0])
    elif f.GetNpar()==2:
      ff_params['mvadm%(mvadmbin)s' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
    elif f.GetNpar() > 4:
      ff_params['mvadm%(mvadmbin)s' % vars()] = ff_eqn_alt.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
    else:
      ff_params['mvadm%(mvadmbin)s' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) ' % (ff_params['mvadm0_sig_lt3'], ff_params['mvadm0_sig_gt3'], ff_params['mvadm1'], ff_params['mvadm2'], ff_params['mvadm10'], ff_params['mvadm11'])

  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)

  return ff_eqn_tot


def WriteFunctionMVADM2Jets(fout,proc='qcd',aiso=False):
  # this function loops over all njets and dm bins and write the FFs as a function
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin in ['0','1','2','0_crosstrg','1_crosstrg','2_crosstrg']:
    for mvadmbin in [0,1,2,10,11]:
      fout.cd()
      extra=''
      if aiso: extra = '_aiso2_ss'
      f = fout.Get('mvadm%(mvadmbin)i_njets%(njetbin)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
      p = f.GetParameters()
      if f.GetNpar()==1:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)s' % vars()] = ff_pol0.replace('p0','%f' % p[0])
      elif f.GetNpar()==2:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)s' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      elif f.GetNpar() > 4:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)s' % vars()] = ff_eqn_alt.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      else:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)s' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((n_jets==0)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets==1)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets>1)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)))' % (ff_params['mvadm0_njets0'], ff_params['mvadm1_njets0'], ff_params['mvadm2_njets0'], ff_params['mvadm10_njets0'], ff_params['mvadm11_njets0'], ff_params['mvadm0_njets1'], ff_params['mvadm1_njets1'], ff_params['mvadm2_njets1'], ff_params['mvadm10_njets1'], ff_params['mvadm11_njets1'], ff_params['mvadm0_njets2'], ff_params['mvadm1_njets2'], ff_params['mvadm2_njets2'], ff_params['mvadm10_njets2'], ff_params['mvadm11_njets2'])

  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)

  ff_eqn_tot_cross = '((n_jets==0)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets==1)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets>1)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)))' % (ff_params['mvadm0_njets0_crosstrg'], ff_params['mvadm1_njets0_crosstrg'], ff_params['mvadm2_njets0_crosstrg'], ff_params['mvadm10_njets0_crosstrg'], ff_params['mvadm11_njets0_crosstrg'], ff_params['mvadm0_njets1_crosstrg'], ff_params['mvadm1_njets1_crosstrg'], ff_params['mvadm2_njets1_crosstrg'], ff_params['mvadm10_njets1_crosstrg'], ff_params['mvadm11_njets1_crosstrg'], ff_params['mvadm0_njets2_crosstrg'], ff_params['mvadm1_njets2_crosstrg'], ff_params['mvadm2_njets2_crosstrg'], ff_params['mvadm10_njets2_crosstrg'], ff_params['mvadm11_njets2_crosstrg'])
  ff_eqn_tot_cross = re.sub('X', '2', ff_eqn_tot_cross).replace('pt_2','min(pt_2,80.)')

  ff_eqn_tot = '((trg_singleelectron&&pt_1>33)*%(ff_eqn_tot)s + ((trg_singleelectron&&pt_1>33)==0)*%(ff_eqn_tot_cross)s)' % vars()

  return ff_eqn_tot

def WriteFunctionMVADM2JetsIPSig(fout,proc='qcd',aiso=False):
  # this function loops over all njets and dm bins and write the FFs as a function
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin in ['0','1','2','0_crosstrg','1_crosstrg','2_crosstrg']:
    for mvadmbin in ['0_sig_lt3','0_sig_gt3','1','2','10','11']:
      fout.cd()
      extra=''
      if aiso: extra = '_aiso2_ss'
      f = fout.Get('mvadm%(mvadmbin)s_njets%(njetbin)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
      p = f.GetParameters()
      if f.GetNpar()==1:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_pol0.replace('p0','%f' % p[0])
      elif f.GetNpar()==2:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      elif f.GetNpar() > 4:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_eqn_alt.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      else:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((n_jets==0)*((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets==1)*((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets>1)*((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)))' % (ff_params['mvadm0_sig_lt3_njets0'], ff_params['mvadm0_sig_gt3_njets0'], ff_params['mvadm1_njets0'], ff_params['mvadm2_njets0'], ff_params['mvadm10_njets0'], ff_params['mvadm11_njets0'], ff_params['mvadm0_sig_lt3_njets1'], ff_params['mvadm0_sig_gt3_njets1'], ff_params['mvadm1_njets1'], ff_params['mvadm2_njets1'], ff_params['mvadm10_njets1'], ff_params['mvadm11_njets1'], ff_params['mvadm0_sig_lt3_njets2'], ff_params['mvadm0_sig_gt3_njets2'], ff_params['mvadm1_njets2'], ff_params['mvadm2_njets2'], ff_params['mvadm10_njets2'], ff_params['mvadm11_njets2'])

  ff_eqn_tot_cross = '((n_jets==0)*((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets==1)*((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets>1)*((mva_dm_X==0&&ip_sig_2<1.5)*(%s)+(mva_dm_X==0&&ip_sig_2>=1.5)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)))' % (ff_params['mvadm0_sig_lt3_njets0_crosstrg'], ff_params['mvadm0_sig_gt3_njets0_crosstrg'], ff_params['mvadm1_njets0_crosstrg'], ff_params['mvadm2_njets0_crosstrg'], ff_params['mvadm10_njets0_crosstrg'], ff_params['mvadm11_njets0_crosstrg'], ff_params['mvadm0_sig_lt3_njets1_crosstrg'], ff_params['mvadm0_sig_gt3_njets1_crosstrg'], ff_params['mvadm1_njets1_crosstrg'], ff_params['mvadm2_njets1_crosstrg'], ff_params['mvadm10_njets1_crosstrg'], ff_params['mvadm11_njets1_crosstrg'], ff_params['mvadm0_sig_lt3_njets2_crosstrg'], ff_params['mvadm0_sig_gt3_njets2_crosstrg'], ff_params['mvadm1_njets2_crosstrg'], ff_params['mvadm2_njets2_crosstrg'], ff_params['mvadm10_njets2_crosstrg'], ff_params['mvadm11_njets2_crosstrg'])

  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  ff_eqn_tot_cross = re.sub('X', '2', ff_eqn_tot_cross).replace('pt_2','min(pt_2,80.)')

  ff_eqn_tot = '((trg_singleelectron&&pt_1>33)*%(ff_eqn_tot)s + ((trg_singleelectron&&pt_1>33)==0)*%(ff_eqn_tot_cross)s)' % vars()

  return ff_eqn_tot

def WriteFunctionDM2Jets(fout,proc='qcd',aiso=False):
  # this function loops over all njets and dm bins and write the FFs as a function
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin in ['0','1','2','0_crosstrg','1_crosstrg','2_crosstrg']:
    for dmbin in [0,1,10,11]:
      fout.cd()
      extra=''
      if aiso: extra = '_aiso2_ss'
      f = fout.Get('dm%(dmbin)i_njets%(njetbin)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
      p = f.GetParameters()
      if f.GetNpar()==1:
        ff_params['dm%(dmbin)i_njets%(njetbin)s' % vars()] = ff_pol0.replace('p0','%f' % p[0])
      elif f.GetNpar()==2:
        ff_params['dm%(dmbin)i_njets%(njetbin)s' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      elif f.GetNpar() > 4:
        ff_params['dm%(dmbin)i_njets%(njetbin)s' % vars()] = ff_eqn_alt.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      else:
        ff_params['dm%(dmbin)i_njets%(njetbin)s' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((n_jets==0)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)) + (n_jets==1)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)) + (n_jets>1)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)))' % (ff_params['dm0_njets0'], ff_params['dm1_njets0'], ff_params['dm10_njets0'], ff_params['dm11_njets0'], ff_params['dm0_njets1'], ff_params['dm1_njets1'], ff_params['dm10_njets1'], ff_params['dm11_njets1'], ff_params['dm0_njets2'], ff_params['dm1_njets2'], ff_params['dm10_njets2'], ff_params['dm11_njets2'])

  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)

  ff_eqn_tot_cross = '((n_jets==0)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)) + (n_jets==1)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)) + (n_jets>1)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)))' % (ff_params['dm0_njets0_crosstrg'], ff_params['dm1_njets0_crosstrg'], ff_params['dm10_njets0_crosstrg'], ff_params['dm11_njets0_crosstrg'], ff_params['dm0_njets1_crosstrg'], ff_params['dm1_njets1_crosstrg'], ff_params['dm10_njets1_crosstrg'], ff_params['dm11_njets1_crosstrg'], ff_params['dm0_njets2_crosstrg'], ff_params['dm1_njets2_crosstrg'], ff_params['dm10_njets2_crosstrg'], ff_params['dm11_njets2_crosstrg'])

  ff_eqn_tot_cross = re.sub('X', '2', ff_eqn_tot_cross).replace('pt_2','min(pt_2,80.)')

  ff_eqn_tot = '((trg_singleelectron&&pt_1>33)*%(ff_eqn_tot)s + ((trg_singleelectron&&pt_1>33)==0)*%(ff_eqn_tot_cross)s)' % vars()

  return ff_eqn_tot


draw_list=[]

# mt plots
baseline_bothiso = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && n_bjets==0 && mva_dm_2>=0' % vars()
baseline_aiso1 = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && n_bjets==0 && mva_dm_2>=0' % vars()

var1='pt_2[20,25,30,35,40,45,50,55,60,70,80,100]'
#var2='pt_2[20,30,40,50,60,80]'
var2='pt_2[20,80]'


ff_list = {}
to_write = []

for njetbin in njets_bins:
  for dmbin in dm_bins:
    name= dmbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+dm_bins[dmbin]
    cut_iso1 = re.sub('X', '2', '(%(baseline_bothiso)s)*(%(cuts)s)' % vars())
    cut_aiso1 = re.sub('X', '2','(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
    if 'crosstrg' in name: ff_list[name+'_pt_2'] = (var2, cut_iso1, cut_aiso1)
    else: ff_list[name+'_pt_2'] = (var1, cut_iso1, cut_aiso1)


# add aiso plots

baseline_aiso2_iso = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && n_bjets==0 && mva_dm_2>=0' % vars()
baseline_aiso2_aiso1 = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && n_bjets==0 && mva_dm_2>=0' % vars()

for njetbin in njets_bins:
  for dmbin in dm_bins:
    name= dmbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+dm_bins[dmbin]
    cut_iso1 = re.sub('X', '2', '(%(baseline_aiso2_iso)s)*(%(cuts)s)' % vars())
    cut_aiso1 = re.sub('X', '2','(%(baseline_aiso2_aiso1)s)*(%(cuts)s)' % vars())
    if 'crosstrg' in name: ff_list[name+'_aiso2_ss_pt_2'] = (var2, cut_iso1, cut_aiso1)
    else: ff_list[name+'_aiso2_ss_pt_2'] = (var1, cut_iso1, cut_aiso1)

for ff in ff_list:
  wjets_mc_ff=None
  ttbar_mc_ff=None
  if draw:
    (qcd_iso, wjets_iso, wjets_mc_iso, ttbar_mc_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
    (qcd_aiso, wjets_aiso, wjets_mc_aiso, ttbar_mc_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))

    if 'crosstrg' in ff:
      cuts_new_iso  = ff_list[ff][1].replace('(trg_singleelectron && pt_1>28)==0','trg_etaucross')
      cuts_new_aiso = ff_list[ff][2].replace('(trg_singleelectron && pt_1>28)==0','trg_etaucross')
      # if the cross trigger data is empty then we have to use events without cross-triggers applied
      if qcd_iso.Integral()<=0 or qcd_aiso.Integral()<=0:
        print 'QCD cross trigger empty for ', ff
        (qcd_iso, dummy, dummy, dummy) = DrawHists(ff_list[ff][0], cuts_new_iso, ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso3' in ff))
        (qcd_aiso, dummy, dummy, dummy) = DrawHists(ff_list[ff][0], cuts_new_aiso, ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
      if wjets_iso.Integral()<=0 or wjets_aiso.Integral()<=0:
        print 'W+jets cross trigger empty for ', ff
        (dummy, wjets_iso, dummy, dummy) = DrawHists(ff_list[ff][0], cuts_new_iso, ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso3' in ff))
        (dummy, wjets_aiso, dummy, dummy) = DrawHists(ff_list[ff][0], cuts_new_aiso, ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
      if wjets_mc_iso.Integral()<=0 or wjets_mc_aiso.Integral()<=0:
        print 'W+jets MC cross trigger empty for ', ff
        (dummy, dummy, wjets_mc_iso, dummy) = DrawHists(ff_list[ff][0], cuts_new_iso, ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso3' in ff))
        (dummy, dummy, wjets_mc_aiso, dummy) = DrawHists(ff_list[ff][0], cuts_new_aiso, ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
      if ttbar_mc_iso.Integral()<=0 or ttbar_mc_aiso.Integral()<=0:
        (dummy, dummy, dummy, ttbar_mc_iso) = DrawHists(ff_list[ff][0], cuts_new_iso, ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso3' in ff))
        (dummy, dummy, dummy, ttbar_mc_aiso) = DrawHists(ff_list[ff][0], cuts_new_aiso, ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))

    qcd_ff = CalculateFakeFactors(qcd_iso, qcd_aiso)
    to_write.append(qcd_ff)

    if not 'aiso2' in ff:
      wjets_ff = CalculateFakeFactors(wjets_iso, wjets_aiso)
      to_write.append(wjets_ff)

      wjets_mc_ff = CalculateFakeFactors(wjets_mc_iso, wjets_mc_aiso)
      to_write.append(wjets_mc_ff)

      #if 'inclusive_inclusive' in ff:
      ttbar_mc_ff = CalculateFakeFactors(ttbar_mc_iso, ttbar_mc_aiso)
      to_write.append(ttbar_mc_ff)
  else:
    # if not drawing histogram then retrieve them from the old output folder
    fin = ROOT.TFile(out_file)
    qcd_ff = fin.Get(ff+'_ff_qcd')
    qcd_ff.SetDirectory(0)
    to_write.append(qcd_ff)
    if not 'aiso2' in ff:
      wjets_ff = fin.Get(ff+'_ff_wjets')
      wjets_ff.SetDirectory(0)
      to_write.append(wjets_ff)
      wjets_mc_ff = fin.Get(ff+'_ff_wjets_mc')
      wjets_mc_ff.SetDirectory(0)
      to_write.append(wjets_mc_ff)
      ttbar_mc_ff = fin.Get(ff+'_ff_ttbar_mc')
      ttbar_mc_ff.SetDirectory(0)
      to_write.append(ttbar_mc_ff)
    fin.Close()

  usePol=None
  usePol=1
  if 'crosstrg' in ff: usePol=0

  usePolQCD = usePol
  usePolW = usePol
  usePolTT = usePol

  if ((True in [ff.startswith(x) for x in fit_pol1_qcd] and 'crosstrg' not in ff) or (True in [x in ff and 'crosstrg' in x for x in fit_pol1_qcd])): usePolQCD=1
  if ((True in [ff.startswith(x) for x in fit_pol1_wjets] and 'crosstrg' not in ff) or (True in [x in ff and 'crosstrg' in x for x in fit_pol1_wjets])): usePolW=1
  if ((True in [ff.startswith(x) for x in fit_pol1_ttbar] and 'crosstrg' not in ff) or (True in [x in ff and 'crosstrg' in x for x in fit_pol1_ttbar])): usePolTT=1
  if ((True in [ff.startswith(x) for x in fit_pol0_qcd] and 'crosstrg' not in ff) or (True in [x in ff and 'crosstrg' in x for x in fit_pol0_qcd])): usePolQCD=0
  if ((True in [ff.startswith(x) for x in fit_pol0_wjets] and 'crosstrg' not in ff) or (True in [x in ff and 'crosstrg' in x for x in fit_pol0_wjets])): usePolW=0
  if ((True in [ff.startswith(x) for x in fit_pol0_ttbar] and 'crosstrg' not in ff) or (True in [x in ff and 'crosstrg' in x for x in fit_pol0_ttbar])): usePolTT=0

  # do fitting
  (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff,polOnly=usePolQCD)
  to_write.append(qcd_fit)
  to_write.append(qcd_uncert)
  PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp)

  if not 'aiso2' in ff:
    (wjets_fit, wjets_uncert, wjets_ff) = FitFakeFactors(wjets_ff,polOnly=usePolW)
    to_write.append(wjets_fit)
    to_write.append(wjets_uncert)
    PlotFakeFactor(wjets_ff, wjets_uncert, wjets_ff.GetName(), output_folder, wp)

    (wjets_mc_fit, wjets_mc_uncert, wjets_mc_ff) = FitFakeFactors(wjets_mc_ff,polOnly=usePolW)
    to_write.append(wjets_mc_fit)
    to_write.append(wjets_mc_uncert)
    PlotFakeFactor(wjets_mc_ff, wjets_mc_uncert, wjets_mc_ff.GetName(), output_folder, wp)
    if ttbar_mc_ff:
      (ttbar_mc_fit, ttbar_mc_uncert, ttbar_mc_ff) = FitFakeFactors(ttbar_mc_ff,polOnly=usePolTT)
      to_write.append(ttbar_mc_fit)
      to_write.append(ttbar_mc_uncert)
      PlotFakeFactor(ttbar_mc_ff, ttbar_mc_uncert, ttbar_mc_ff.GetName(), output_folder, wp)

# make fractions
 
njets_cuts = ['n_jets==0','n_jets==1','n_jets>=2']

for njets in [0,1,2]:
  njets_cut = njets_cuts[njets]
  var= 'mt_1[0,10,20,30,40,50,60,70,80,90,100,120]'
  cuts = '(%(baseline_aiso1)s)*(%(njets_cut)s)' % vars()
  name = 'et_fracs_njets%(njets)s' % vars()
  qcd_os, wjets_os, ttbar_os = DrawHistsForFractions(var, '%(cuts)s*(os==1)' % vars(), name+'_os', input_folder, file_ext)
  qcd_ss, wjets_ss, ttbar_ss = DrawHistsForFractions(var, '%(cuts)s*(os==0)' % vars(), name+'_ss', input_folder, file_ext)
  total_os = qcd_os.Clone(); total_os.Add(wjets_os); total_os.Add(ttbar_os) 
  total_ss = qcd_ss.Clone(); total_ss.Add(wjets_ss); total_ss.Add(ttbar_ss)
  qcd_os.Divide(total_os)
  wjets_os.Divide(total_os)
  ttbar_os.Divide(total_os)
  qcd_ss.Divide(total_ss)
  wjets_ss.Divide(total_ss)
  ttbar_ss.Divide(total_ss)
  to_write.append(qcd_os); to_write.append(wjets_os); to_write.append(ttbar_os)
  to_write.append(qcd_ss); to_write.append(wjets_ss); to_write.append(ttbar_ss)

# write everything to the output file
fout = ROOT.TFile(out_file, 'RECREATE')

for i in to_write: i.Write()

## write strings needed for closure tests
tau_qcd_dm_string = WriteFunctionDM2Jets(fout,proc='qcd').replace('pt_2','min(pt_2,100.)')
tau_qcd_mvadm_string = WriteFunctionMVADM2Jets(fout,proc='qcd').replace('pt_2','min(pt_2,100.)')
tau_qcd_mvadm_string_ipsig = WriteFunctionMVADM2JetsIPSig(fout,proc='qcd').replace('pt_2','min(pt_2,100.)')

tau_qcd_dm_string_aiso = WriteFunctionDM2Jets(fout,proc='qcd',aiso=True).replace('pt_2','min(pt_2,100.)')
tau_qcd_mvadm_string_aiso = WriteFunctionMVADM2Jets(fout,proc='qcd',aiso=True).replace('pt_2','min(pt_2,100.)')
tau_qcd_mvadm_string_ipsig_aiso = WriteFunctionMVADM2JetsIPSig(fout,proc='qcd',aiso=True).replace('pt_2','min(pt_2,100.)')

tau_wjets_dm_string = WriteFunctionDM2Jets(fout,proc='wjets').replace('pt_2','min(pt_2,100.)')
tau_wjets_mvadm_string = WriteFunctionMVADM2Jets(fout,proc='wjets').replace('pt_2','min(pt_2,100.)')
tau_wjets_mvadm_string_ipsig = WriteFunctionMVADM2JetsIPSig(fout,proc='wjets').replace('pt_2','min(pt_2,100.)')

tau_wjets_mc_dm_string = WriteFunctionDM2Jets(fout,proc='wjets_mc').replace('pt_2','min(pt_2,100.)')
tau_wjets_mc_mvadm_string = WriteFunctionMVADM2Jets(fout,proc='wjets_mc').replace('pt_2','min(pt_2,100.)')
tau_wjets_mc_mvadm_string_ipsig = WriteFunctionMVADM2JetsIPSig(fout,proc='wjets_mc').replace('pt_2','min(pt_2,100.)')

print '\n'
print "QCD isolated region dm binning:"
print tau_qcd_dm_string

print '\n'
print "QCD isolated region mva-dm binning:"
print tau_qcd_mvadm_string

print '\n'
print "QCD isolated region mva-dm binning:"
print tau_qcd_mvadm_string_ipsig

print '\n'
print "QCD anti-isolated region dm binning:"
print tau_qcd_dm_string_aiso

print '\n'
print "QCD anti-isolated region mva-dm binning:"
print tau_qcd_mvadm_string_aiso

print '\n'
print "QCD anti-isolated region mva-dm binning:"
print tau_qcd_mvadm_string_ipsig_aiso

print '\n'
print "W+jets isolated region dm binning:"
print tau_wjets_dm_string

print '\n'
print "W+jets isolated region mva-dm binning:"
print tau_wjets_mvadm_string

print '\n'
print "W+jets isolated region mva-dm binning:"
print tau_wjets_mvadm_string_ipsig

print '\n'
print "W+jets (MC) isolated region dm binning:"
print tau_wjets_mc_dm_string

print '\n'
print "W+jets (MC) isolated region mva-dm binning:"
print tau_wjets_mc_mvadm_string

print '\n'
print "W+jets (MC) isolated region mva-dm binning:"
print tau_wjets_mc_mvadm_string_ipsig

print '\n'
print "ttbar (MC) isolated region mva-dm binning:"
tau_ttbar_mvadm_string=WriteFunctionTTbar(fout,proc='ttbar_mc')
print tau_ttbar_mvadm_string.replace('pt_2','min(pt_2,140.)')


# for W+jets, first non-closure correction is to correct the MET distribution, this is correlated to the tau pT so needs to be derved using a 2D histogram
# only for events with n_jets<2 as no non closure observed for 2 jet bin
for i in ['mvadm','mvadm_nosig','dm']:

  met_2d_corr='('
  for njet in ['0','1','2']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets==1'
    if njet =='2': cut = 'n_jets>=2'

    var='newmet*cos(newmet_dphi_2)/pt_2[-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000]'
    
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=False,doW=True,doMC=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_wjets_mvadm_string_ipsig,doQCD=False,doW=True,doMC=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_wjets_mvadm_string,doQCD=False,doW=True,doMC=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_wjets_dm_string,doQCD=False,doW=True,doMC=False)

    fout.cd()
    wjets_data.Divide(wjets_pred)
    wjets_met_corr_fit, wjets_met_corr_uncert = FitCorrection(wjets_data, func='[0]*TMath::Erf((x-[1])/[2])+[3]',)
    wjets_data.Write()
    wjets_met_corr_fit.Write()
    wjets_met_corr_uncert.Write()

    met_2d_corr_temp=str(wjets_met_corr_fit.GetExpFormula('p')).replace('x','max(min(newmet*cos(newmet_dphi_2)/pt_2,5),-5)').replace(',false','')
    met_2d_corr += '('+cut+')*(%(met_2d_corr_temp)s)' % vars()
    if njet != '2': met_2d_corr+=' + '

    PlotFakeFactorCorrection(wjets_data, wjets_met_corr_uncert, wjets_data.GetName(), output_folder, wp,x_title='E_{T}^{miss}^{*}cos(#Delta#phi^{*})/p_{T}')

    print "W met correction:"
    print met_2d_corr

  met_2d_corr+=')'
  # now the pt_1 correction

  pt_1_2d_corr='((pt_1>33)*('

  for njet in ['0','1','2']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets==1'
    if njet =='2': cut = 'n_jets>=2'

    #var='pt_1[20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    var='pt_1[33,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=False,doW=True,doMC=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mvadm_string_ipsig)s*%(met_2d_corr)s' % vars(),doQCD=False,doW=True,doMC=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mvadm_string)s*%(met_2d_corr)s' % vars(),doQCD=False,doW=True,doMC=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_dm_string)s*%(met_2d_corr)s' % vars() ,doQCD=False,doW=True,doMC=False)
  
  
    fout.cd()
    wjets_data.Divide(wjets_pred)
    if njet == '0': pol='pol3'
    else: pol = 'pol1' 
    wjets_pt_1_corr_fit, wjets_pt_1_corr_uncert = FitCorrection(wjets_data, func=pol)
    wjets_data.Write()
    wjets_pt_1_corr_fit.Write()
    wjets_pt_1_corr_uncert.Write()
  
    pt_1_2d_corr_temp=str(wjets_pt_1_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,90.)').replace(',false','')
    pt_1_2d_corr += '('+cut+')*(%(pt_1_2d_corr_temp)s)' % vars()
    if njet != '2': pt_1_2d_corr+=' + '

    PlotFakeFactorCorrection(wjets_data, wjets_pt_1_corr_uncert, wjets_data.GetName(), output_folder, wp,x_title='p_{T}^{l} (GeV)')

  pt_1_2d_corr+=') + (pt_1<33))'
  print 'The W non-closure function for decay mode binning %(i)s:' % vars()
  print '%(met_2d_corr)s*%(pt_1_2d_corr)s' % vars()


  # now we need to do exactly the same for the W+jets MC in order to derive the high mT -> low mT extrapolation uncertainty

  met_2d_corr_mc='('
  for njet in ['0','1','2']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets==1'
    if njet =='2': cut = 'n_jets>=2'

    var='newmet*cos(newmet_dphi_2)/pt_2[-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000]'
    
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=False,doW=False,doMC=True,doTT=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_wjets_mc_mvadm_string_ipsig,doQCD=False,doW=False,doMC=True,doTT=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_wjets_mc_mvadm_string,doQCD=False,doW=False,doMC=True,doTT=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_wjets_mc_dm_string,doQCD=False,doW=False,doMC=True,doTT=False)

    fout.cd()
    wjets_mc_data.Divide(wjets_mc_pred)
    wjets_mc_met_corr_fit, wjets_mc_met_corr_uncert = FitCorrection(wjets_mc_data, func='[0]*TMath::Erf((x-[1])/[2])+[3]',)
    wjets_mc_data.Write()
    wjets_mc_met_corr_fit.Write()
    wjets_mc_met_corr_uncert.Write()

    met_2d_corr_mc_temp=str(wjets_mc_met_corr_fit.GetExpFormula('p')).replace('x','max(min(newmet*cos(newmet_dphi_2)/pt_2,5),-5)').replace(',false','')
    met_2d_corr_mc += '('+cut+')*(%(met_2d_corr_mc_temp)s)' % vars()
    if njet != '2': met_2d_corr_mc+=' + '

    PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_met_corr_uncert, wjets_mc_data.GetName(), output_folder, wp,x_title='E_{T}^{miss}^{*}cos(#Delta#phi^{*})/p_{T}')

  met_2d_corr_mc+=')'
  print "W MC met correction:"
  print met_2d_corr_mc

  # now the pt_1 correction

  pt_1_2d_corr_mc='((pt_1>33)*('

  for njet in ['0','1','2']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets==1'
    if njet =='2': cut = 'n_jets>=2'

    var='pt_1[33,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=False,doW=False,doMC=True,doTT=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_mvadm_string_ipsig)s*%(met_2d_corr_mc)s' % vars(),doQCD=False,doW=False,doMC=True,doTT=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_mvadm_string)s*%(met_2d_corr_mc)s' % vars(),doQCD=False,doW=False,doMC=True,doTT=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_dm_string)s*%(met_2d_corr_mc)s' % vars() ,doQCD=False,doW=False,doMC=True,doTT=False)
  
  
    fout.cd()
    wjets_mc_data.Divide(wjets_mc_pred)
    if njet == '0': pol='pol3'
    else: pol = 'pol1' 
    wjets_mc_pt_1_corr_fit, wjets_mc_pt_1_corr_uncert = FitCorrection(wjets_mc_data, func=pol,)
    wjets_mc_data.Write()
    wjets_mc_pt_1_corr_fit.Write()
    wjets_mc_pt_1_corr_uncert.Write()
  
    pt_1_2d_corr_mc_temp=str(wjets_mc_pt_1_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,90.)').replace(',false','')
    pt_1_2d_corr_mc += '('+cut+')*(%(pt_1_2d_corr_mc_temp)s)' % vars()
    if njet != '2': pt_1_2d_corr_mc+=' + '

    PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_pt_1_corr_uncert, wjets_mc_data.GetName(), output_folder, wp,x_title='p_{T}^{l} (GeV)')

  pt_1_2d_corr_mc+=') + (pt_1<33))'
  print 'The W MC non-closure function for decay mode binning %(i)s:' % vars()
  print '%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars()


  # now derive mT corrections for MC (full mT range) and data (mT>70 GeV only)

  print "Deriving mt_1 corrections"

  # for MC

  var='mt_1[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140]'
  (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')', '%(i)s_mt_corr' % vars(),input_folder,file_ext,False,doQCD=False,doW=False,doMC=True,doTT=False,fullMT=True)
  if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')', '%(i)s_mt_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_mvadm_string_ipsig)s*%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars(),doQCD=False,doW=False,doMC=True,doTT=False,fullMT=True)
  if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')', '%(i)s_mt_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_mvadm_string)s*%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars(),doQCD=False,doW=False,doMC=True,doTT=False,fullMT=True)
  if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')', '%(i)s_mt_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_dm_string)s*%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars() ,doQCD=False,doW=False,doMC=True,doTT=False,fullMT=True)

  fout.cd()
  wjets_mc_data.Divide(wjets_mc_pred)
  wjets_mc_data_fit, wjets_mc_data_uncert =  FitCorrection(wjets_mc_data, func='pol3')
  wjets_mc_data.Write()
  wjets_mc_data_fit.Write()
  wjets_mc_data_uncert.Write()
  PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_data_uncert, wjets_mc_data.GetName(), output_folder, wp, x_title='m_{T} (GeV)')

# now we add a correction as a function of WpT for the high->lowMT extrapolation in bins of njets

  for njet in ['0','1','2']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets==1'
    if njet =='2': cut = 'n_jets>=2'

    var='newmet[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200]'
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_WpT_corr_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=False,doW=False,doMC=True,doTT=False,lowMT=True)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_WpT_pred_njet%(njet)s' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_mvadm_string_ipsig)s*%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars(),doQCD=False,doW=False,doMC=True,doTT=False,lowMT=True)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_WpT_pred_njet%(njet)s' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_mvadm_string)s*%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars(),doQCD=False,doW=False,doMC=True,doTT=False,lowMT=True)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_WpT_pred_njet%(njet)s' % vars(),input_folder,file_ext,False,add_wt='%(tau_wjets_mc_dm_string)s*%(met_2d_corr_mc)s*%(pt_1_2d_corr_mc)s' % vars() ,doQCD=False,doW=False,doMC=True,doTT=False,lowMT=True)
  
    fout.cd()
    wjets_mc_data.Divide(wjets_mc_pred)
    wjets_mc_data_fit, wjets_mc_data_uncert =  FitCorrection(wjets_mc_data, func='pol1')
    wjets_mc_data.Write()
    wjets_mc_data_fit.Write()
    wjets_mc_data_uncert.Write()
    PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_data_uncert, wjets_mc_data.GetName(), output_folder, wp, x_title='p_{T}^{W} (GeV)')


### derive QCD non-closure corrections
# first we need to correct the MET for n_jets=0 events - as with the W+jets this needs to be done using a 2D distribution as a function also of pt_1

for i in ['mvadm','mvadm_nosig','dm']:
  
  met_2d_corr='('
  for njet in ['0','1']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets>=1'

    var='met*cos(met_dphi_2)/pt_2[-3.50,-3.25,-3.00,-2.75,-2.50,-2.25,-2.00,-1.75,-1.50,-1.25,-1.00,-0.75,-0.50,-0.25,0.00,0.25,0.50,0.75,1.00,1.25,1.50]'
    
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=True,doW=False,doMC=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_qcd_mvadm_string_ipsig,doQCD=True,doW=False,doMC=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_qcd_mvadm_string,doQCD=True,doW=False,doMC=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau_qcd_dm_string,doQCD=True,doW=False,doMC=False)

    fout.cd()
    qcd_data.Divide(qcd_pred)
    qcd_met_corr_fit, qcd_met_corr_uncert = FitCorrection(qcd_data, func='pol1',)
    qcd_data.Write()
    qcd_met_corr_fit.Write()
    qcd_met_corr_uncert.Write()

    met_2d_corr_temp=str(qcd_met_corr_fit.GetExpFormula('p')).replace('x','max(min(met*cos(met_dphi_2)/pt_2,1.5),-2.5)').replace(',false','')
    met_2d_corr += '('+cut+')*(%(met_2d_corr_temp)s)' % vars()
    if njet != '1': met_2d_corr+=' + '

    PlotFakeFactorCorrection(qcd_data, qcd_met_corr_uncert, qcd_data.GetName(), output_folder, wp,x_title='E_{T}^{miss}cos(#Delta#phi)/p_{T}')

  met_2d_corr+=')'
  print "QCD met correction:"
  print met_2d_corr

  # now the pt_1 correction

  pt_1_2d_corr='((pt_1>33)*('

  for njet in ['0','1']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets>=1'

    #var='pt_1[20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    var='pt_1[33,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s' % vars(),input_folder,file_ext,False,doQCD=True,doW=False,doMC=False,doTT=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_qcd_mvadm_string_ipsig)s*%(met_2d_corr)s' % vars(),doQCD=True,doW=False,doMC=False,doTT=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_qcd_mvadm_string)s*%(met_2d_corr)s' % vars(),doQCD=True,doW=False,doMC=False,doTT=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,False,add_wt='%(tau_qcd_dm_string)s*%(met_2d_corr)s' % vars() ,doQCD=True,doW=False,doMC=False,doTT=False)
  
  
    fout.cd()
    qcd_data.Divide(qcd_pred)
    if njet == '0': pol='landau'
    else: pol = 'pol1' 
    qcd_pt_1_corr_fit, qcd_pt_1_corr_uncert = FitCorrection(qcd_data, func=pol,)
    qcd_data.Write()
    qcd_pt_1_corr_fit.Write()
    qcd_pt_1_corr_uncert.Write()
  
    if njet=='0': pt_1_2d_corr_temp=str(qcd_pt_1_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,50.)').replace(',false','')
    else: pt_1_2d_corr_temp=str(qcd_pt_1_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,70.)').replace(',false','')
    pt_1_2d_corr += '('+cut+')*(%(pt_1_2d_corr_temp)s)' % vars()
    if njet != '1': pt_1_2d_corr+=' + '
  
    PlotFakeFactorCorrection(qcd_data, qcd_pt_1_corr_uncert, qcd_data.GetName(), output_folder, wp,x_title='p_{T}^{l} (GeV)')
  
  pt_1_2d_corr+=') + (pt_1<33))'
  print "QCD pt_1 correction:"
  print pt_1_2d_corr

  ## apply all closure corrections in SS region and then derive the aiso->iso correction

  var='iso_1[0.00,0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.10,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.20,0.21,0.22,0.23,0.24,0.25,0.26,0.27,0.28,0.29,0.30,0.31,0.32,0.33,0.34,0.35,0.36,0.37,0.38,0.39,0.40,0.41,0.42,0.43,0.44,0.45,0.46,0.47,0.48,0.49,0.50]'

  # ss region for events triggered by single muon trigger
  (qcd_ss_data, wjets_ss_data, wjets_mc_data, ttbar_ss_data) = DrawHists(var, '('+baseline_bothiso.replace('iso_1<0.15','1')+')', '%(i)s_iso_closure' % vars(),input_folder,file_ext,False,doQCD=True,doW=False,doMC=False,doIso=True)
  if i=='mvadm': (qcd_ss_pred, wjets_ss_pred, wjets_ss_mc_pred, ttbar_ss_pred_mvadm) = DrawHists(var, '('+baseline_aiso1.replace('iso_1<0.15','1')+')', '%(i)s_iso_closure_pred' % vars(),input_folder,file_ext,False,add_wt='(%(tau_qcd_mvadm_string_ipsig)s)*(%(pt_1_2d_corr)s)*(%(met_2d_corr)s)' % vars(),doQCD=True,doW=False,doMC=False,doIso=True)
  if i=='mvadm_nosig': (qcd_ss_pred, wjets_ss_pred, wjets_ss_mc_pred, ttbar_ss_pred) = DrawHists(var, '('+baseline_aiso1.replace('iso_1<0.15','1')+')', '%(i)s_iso_closure_pred' % vars(),input_folder,file_ext,False,add_wt='(%(tau_qcd_mvadm_string)s)*(%(pt_1_2d_corr)s)*(%(met_2d_corr)s)' % vars(),doQCD=True,doW=False,doMC=False,doIso=True)
  if i=='dm': (qcd_ss_pred, wjets_ss_pred, wjets_ss_mc_pred, ttbar_ss_pred) = DrawHists(var, '('+baseline_aiso1.replace('iso_1<0.15','1')+')', '%(i)s_iso_closure_pred' % vars(),input_folder,file_ext,False,add_wt='(%(tau_qcd_dm_string)s)*(%(pt_1_2d_corr)s)*(%(met_2d_corr)s)' % vars(),doQCD=True,doW=False,doMC=False,doIso=True)
  fout.cd()
  qcd_ss_data.Divide(qcd_ss_pred)

  qcd_ss_data_fit, qcd_ss_data_uncert =  FitCorrection(qcd_ss_data, func='pol3')

  qcd_ss_data.Write()
  qcd_ss_data_fit.Write()
  qcd_ss_data_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data, qcd_ss_data_uncert, qcd_ss_data.GetName(), output_folder, wp,x_title="I^{#mu}_{rel}")

  iso_corr_fit = fout.Get('%(i)s_iso_closure_qcd_fit' % vars())
  p = iso_corr_fit.GetParameters()
  iso_corr = '*(%f+ %f*min(iso_1,0.5) + %f*min(iso_1,0.5)*min(iso_1,0.5) + %f*min(iso_1,0.5)*min(iso_1,0.5)*min(iso_1,0.5))' % (p[0], p[1], p[2], p[3])

  print '\n'
  print 'iso_1 correction for %s' % i
  print iso_corr

### derive QCD non-closure corrections for anti-siolated data
# first we need to correct the MET 

for i in ['mvadm','mvadm_nosig','dm']:
  met_2d_corr_aiso='('
  for njet in ['0','1']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets>=1'

    var='met*cos(met_dphi_2)/pt_2[-3.50,-3.25,-3.00,-2.75,-2.50,-2.25,-2.00,-1.75,-1.50,-1.25,-1.00,-0.75,-0.50,-0.25,0.00,0.25,0.50,0.75,1.00,1.25,1.50]'
    
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_aiso2_iso+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_aiso' % vars(),input_folder,file_ext,False,doQCD=True,doW=False,doMC=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred_aiso' % vars(),input_folder,file_ext,False,add_wt=tau_qcd_mvadm_string_ipsig_aiso,doQCD=True,doW=False,doMC=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred_aiso' % vars(),input_folder,file_ext,False,add_wt=tau_qcd_mvadm_string_aiso,doQCD=True,doW=False,doMC=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_met_closure_njet%(njet)s_pred_aiso' % vars(),input_folder,file_ext,False,add_wt=tau_qcd_dm_string_aiso,doQCD=True,doW=False,doMC=False)

    fout.cd()
    qcd_data.Divide(qcd_pred)
    qcd_met_corr_fit, qcd_met_corr_uncert = FitCorrection(qcd_data, func='pol1',)
    qcd_data.Write()
    qcd_met_corr_fit.Write()
    qcd_met_corr_uncert.Write()

    met_2d_corr_aiso_temp=str(qcd_met_corr_fit.GetExpFormula('p')).replace('x','max(min(met*cos(met_dphi_2)/pt_2,1.5),-2.5)').replace(',false','')
    met_2d_corr_aiso += '('+cut+')*(%(met_2d_corr_aiso_temp)s)' % vars()
    if njet != '1': met_2d_corr_aiso+=' + '

    PlotFakeFactorCorrection(qcd_data, qcd_met_corr_uncert, qcd_data.GetName(), output_folder, wp,x_title='E_{T}^{miss}cos(#Delta#phi)/p_{T}')

  met_2d_corr_aiso+=')'
  print "QCD met correction (anti isolated):"
  print met_2d_corr_aiso

  # now the pt_1 correction

  pt_1_2d_corr_aiso='((pt_1>33)*('

  for njet in ['0','1']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets>=1'

    #var='pt_1[20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    var='pt_1[33,40,45,50,55,60,65,70,75,80,85,90,95,100]'
    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_aiso2_iso+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_aiso' % vars(),input_folder,file_ext,False,doQCD=True,doW=False,doMC=False,doTT=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred_aiso' % vars(),input_folder,file_ext,False,add_wt='%(tau_qcd_mvadm_string_ipsig_aiso)s*%(met_2d_corr_aiso)s' % vars(),doQCD=True,doW=False,doMC=False,doTT=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred_aiso' % vars(),input_folder,file_ext,False,add_wt='%(tau_qcd_mvadm_string_aiso)s*%(met_2d_corr_aiso)s' % vars(),doQCD=True,doW=False,doMC=False,doTT=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_pt_1_closure_njet%(njet)s_pred_aiso' % vars(),input_folder,file_ext,False,add_wt='%(tau_qcd_dm_string_aiso)s*%(met_2d_corr_aiso)s' % vars() ,doQCD=True,doW=False,doMC=False,doTT=False)
  
  
    fout.cd()
    qcd_data.Divide(qcd_pred)
    if njet == '0': pol='landau'
    else: pol = 'pol1' 
    qcd_pt_1_corr_fit, qcd_pt_1_corr_uncert = FitCorrection(qcd_data, func=pol,)
    qcd_data.Write()
    qcd_pt_1_corr_fit.Write()
    qcd_pt_1_corr_uncert.Write()
  
    if njet=='0': pt_1_2d_corr_aiso_temp=str(qcd_pt_1_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,50.)').replace(',false','')
    else: pt_1_2d_corr_aiso_temp=str(qcd_pt_1_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,50.)').replace(',false','')
    pt_1_2d_corr_aiso += '('+cut+')*(%(pt_1_2d_corr_aiso_temp)s)' % vars()
    if njet != '1': pt_1_2d_corr_aiso+=' + '
  
    PlotFakeFactorCorrection(qcd_data, qcd_pt_1_corr_uncert, qcd_data.GetName(), output_folder, wp,x_title='p_{T}^{l} (GeV)')

  pt_1_2d_corr_aiso+=') + (pt_1<33))'
  print "QCD pt_1 correction (anti isolated):"
  print pt_1_2d_corr_aiso


  if i=='dm':
    aiso_ff_string='((%(tau_qcd_dm_string_aiso)s)*(%(met_2d_corr_aiso)s)*(%(pt_1_2d_corr_aiso)s))' % vars()
  if i=='mvadm_nosig':
    aiso_ff_string='((%(tau_qcd_mvadm_string_aiso)s)*(%(met_2d_corr_aiso)s)*(%(pt_1_2d_corr_aiso)s))' % vars()
  if i=='mvadm':
    aiso_ff_string='((%(tau_qcd_mvadm_string_ipsig_aiso)s)*(%(met_2d_corr_aiso)s)*(%(pt_1_2d_corr_aiso)s))' % vars()

  print 'QCD aiso region string:'
  print aiso_ff_string

  for njet in ['0','1']:
    cut = ''
    if njet =='0': cut = 'n_jets==0'
    if njet =='1': cut = 'n_jets>=1'
 
    var='pt_1[25,30,35,40,50,60,70,80,90,100]'

    (qcd_data, wjets_data, wjets_mc_data, ttbar_data) = DrawHists(var, '('+baseline_aiso2_iso+')*('+cut+')', '%(i)s_osss_closure_njet%(njet)s' % vars(),input_folder,file_ext,doOS=True,doQCD=True,doW=False,doMC=False,doIso=False)
    if i=='mvadm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred_mvadm) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_osss_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=aiso_ff_string % vars(),doQCD=True,doW=False,doMC=False,doIso=False)
    if i=='mvadm_nosig': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_osss_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=aiso_ff_string % vars(),doQCD=True,doW=False,doMC=False,doIso=False)
    if i=='dm': (qcd_pred, wjets_pred, wjets_mc_pred, ttbar_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', '%(i)s_osss_closure_njet%(njet)s_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=aiso_ff_string % vars(),doQCD=True,doW=False,doMC=False,doIso=False)
    fout.cd()
    qcd_data.Divide(qcd_pred)

    qcd_data_fit, qcd_data_uncert =  FitCorrection(qcd_data, func='pol3')

    qcd_data.Write()
    qcd_data_fit.Write()
    qcd_data_uncert.Write()
    PlotFakeFactorCorrection(qcd_data, qcd_data_uncert, qcd_data.GetName(), output_folder, wp,x_title="p_{T}^{l} (GeV)")

    osss_corr_fit = fout.Get('%(i)s_osss_closure_njet%(njet)s_qcd_fit' % vars())
    osss_corr = str(osss_corr_fit.GetExpFormula('p')).replace('x','min(pt_1,100.)').replace(',false','')

    print '\n'
    print 'OS/SS correction for %s' % i
    print osss_corr

# get ttbar met non closure correction


var='newmet*cos(newmet_dphi_2)/pt_2[-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000]'

(qcd_data, wjets_data, wjets_mc_data, ttbar_mc_data) = DrawHists(var, '('+baseline_bothiso+')', 'met_closure' % vars(),input_folder,file_ext,False,doQCD=False,doW=False,doMC=True,doTT=True)
(qcd_pred, wjets_pred, wjets_mc_pred, ttbar_mc_pred) = DrawHists(var, '('+baseline_aiso1+')', 'met_closure' % vars(),input_folder,file_ext,False,add_wt=tau_ttbar_mvadm_string,doQCD=False,doW=False,doMC=True,doTT=True)

fout.cd()
ttbar_mc_data.Divide(ttbar_mc_pred)
ttbar_mc_met_corr_fit, ttbar_mc_met_corr_uncert = FitCorrection(ttbar_mc_data, func='[0]*TMath::Erf((x-[1])/[2])+[3]',)
ttbar_mc_data.Write()
ttbar_mc_met_corr_fit.Write()
ttbar_mc_met_corr_uncert.Write()

met_2d_corr_mc_temp=str(ttbar_mc_met_corr_fit.GetExpFormula('p')).replace('x','max(min(newmet*cos(newmet_dphi_2)/pt_2,5),-5)').replace(',false','')
met_2d_corr_mc = '(%(met_2d_corr_mc_temp)s)' % vars()

PlotFakeFactorCorrection(ttbar_mc_data, ttbar_mc_met_corr_uncert, ttbar_mc_data.GetName(), output_folder, wp,x_title='E_{T}^{miss}^{*}cos(#Delta#phi^{*})/p_{T}')

print "ttbar MC met correction:"
print met_2d_corr_mc


fout.Close()
