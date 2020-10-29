import ROOT
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
parser.add_argument('--alt_wp',help= 'Alternative tau ID working point to measure fake factors for', default='vvvloose')
parser.add_argument('--channel',help= 'Channel to get fake factors for (mt or et)', default='mt')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='./')
parser.add_argument('--year', help= 'Name of input year', default='2018')
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
args = parser.parse_args()

wp = args.wp
alt_wp = args.alt_wp
output_folder=args.output_folder
year=args.year
channel = args.channel
draw = int(args.draw) > 0

out_file = '%(output_folder)s/fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root' % vars()
file_ext = '_%(channel)s_%(year)s.root' % vars()

# binning for fake factor determination
njets_bins = {
              'inclusive':'(1)',
              '0jet':'(n_prebjets==0)',
              '1jet':'(n_prebjets>0)',
}

jetpt_bins = {
              #'inclusive':'(1)',
              'jet_pt_low':'(jet_pt_2<1.25*pt_2)',
              'jet_pt_low':'((jet_pt_2>=1.1*pt_2) && (jet_pt_2<1.25*pt_2))',
              'jet_pt_med':'((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.5*pt_2))',
              'jet_pt_high': '(jet_pt_2>=1.5*pt_2)'
}


# get files and cross trigger pT threshold for relevant year
if year == '2018':
  lumi = 58826.8469
  params_file = 'scripts/params_2018.json'
  input_folder = '/vols/cms/gu18/Offline/output/SUSY/Jan24_2018_SUSY/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonA',
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD'
    ]
  elif channel == "et":
    crosstrg_pt = 33
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

elif year == "2017":
  lumi = 41530.
  params_file = 'scripts/params_mssm_2017.json'
  input_folder = '/vols/cms/dw515/Offline/output/MSSM/ff_2017/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF'
    ]
  elif channel == "et":
    crosstrg_pt = 28
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
    ]
 
  ttbar_files = [
    'TTTo2L2Nu', 
    'TTToHadronic', 
    'TTToSemiLeptonic'
  ] 
  
  wjets_files = [
    'WJetsToLNu-LO',
    'WJetsToLNu-LO-ext',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    'EWKWMinus2Jets',
    'EWKWPlus2Jets'
  ]

  other_files = [
    'DYJetsToLL-LO',
    'DYJetsToLL-LO-ext1',
    'DY1JetsToLL-LO',
    'DY1JetsToLL-LO-ext',
    'DY2JetsToLL-LO',
    'DY2JetsToLL-LO-ext',
    'DY3JetsToLL-LO',
    'DY3JetsToLL-LO-ext',
    'DY4JetsToLL-LO',
    'DYJetsToLL_M-10-50-LO',
    'DYJetsToLL_M-10-50-LO-ext1',
    'T-tW', 
    'Tbar-tW',
    'Tbar-t',
    'T-t',
    'WWToLNuQQ',
    'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    'WZTo1L3Nu',
    'WZTo3LNu',
    'ZZTo2L2Nu',
    'WWTo2L2Nu',
    'ZZTo2L2Q',
    'ZZTo4L-ext',
    'ZZTo4L',
    'EWKZ2Jets'
  ]

elif year == "2016":
  lumi = 35920.
  params_file = 'scripts/params_mssm_2016.json'
  input_folder = '/vols/cms/dw515/Offline/output/MSSM/ff_2016/'
  crosstrg_pt = 25 # Not true but fixes pt_1 binning

  if channel == "mt":
    crosstrg_pt = 23
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF',
      'SingleMuonG',
      'SingleMuonH'
    ]
  elif channel == "et":
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
      'SingleElectronG',
      'SingleElectronH'
    ]

  ttbar_files = [
    'TT'
  ]

  wjets_files = [
    'WJetsToLNu-LO', 
    'WJetsToLNu-LO-ext',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W2JetsToLNu-LO-ext',
    'W3JetsToLNu-LO',
    'W3JetsToLNu-LO-ext',
    'W4JetsToLNu-LO',
    'W4JetsToLNu-LO-ext1',
    'W4JetsToLNu-LO-ext2', 
    'EWKWMinus2Jets_WToLNu',
    'EWKWMinus2Jets_WToLNu-ext1',
    'EWKWMinus2Jets_WToLNu-ext2',
    'EWKWPlus2Jets_WToLNu',
    'EWKWPlus2Jets_WToLNu-ext1',
    'EWKWPlus2Jets_WToLNu-ext2'
  ]

  other_files = [
    'DYJetsToLL-LO-ext1',
    'DYJetsToLL-LO-ext2',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    'DYJetsToLL_M-10-50-LO', 
    'T-tW', 
    'Tbar-tW',
    'Tbar-t','T-t',
    'WWTo1L1Nu2Q',
    'WZJToLLLNu',
    'VVTo2L2Nu',
    'VVTo2L2Nu-ext1',
    'ZZTo2L2Q',
    'ZZTo4L-amcat',
    'WZTo2L2Q',
    'WZTo1L3Nu',
    'WZTo1L1Nu2Q',
    'EWKZ2Jets_ZToLL',
    'EWKZ2Jets_ZToLL-ext1',
    'EWKZ2Jets_ZToLL-ext2'
  ]
  

# read params from json
with open(params_file) as jsonfile:
  params = json.load(jsonfile)

# Basline Selection
iso = 'iso_1<0.15'
anti_iso = 'iso_1<0.5 && iso_1>0.25'
deeptau_jet_pass = 'deepTauVsJets_%(wp)s_2>0.5' % vars()
deeptau_jet_fail = 'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_%(alt_wp)s_2>0.5' % vars()
if channel == "mt":
  deeptau_lep = 'deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5'
  crosstrg = '((trg_mutaucross&&pt_1<%(crosstrg_pt)s)||(trg_singlemuon&&pt_1>%(crosstrg_pt)s))' % vars()
elif channel == "et":
  deeptau_lep = "deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5"
  if year == "2016":
    crosstrg = 'trg_singleelectron'
  else:
    crosstrg = "((trg_etaucross&&pt_1<%(crosstrg_pt)s)||(trg_singleelectron&&pt_1>%(crosstrg_pt)s))" % vars() 
other_sel = 'leptonveto==0 && wt<2'


baseline_iso_pass = '(%(iso)s && %(deeptau_jet_pass)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
baseline_iso_fail = '(%(iso)s && %(deeptau_jet_fail)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
baseline_aiso_pass = '(%(anti_iso)s && %(deeptau_jet_pass)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
baseline_aiso_fail = '(%(anti_iso)s && %(deeptau_jet_fail)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()


def ZeroNegativeBins(h):
  # if a bin of h1 is equal to the -ve of
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=True,doTT=True,doIso=True,fullMT=False,lowMT=False,qcdMT='50'):
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
    if not doIso:  qcd_cuts = 'mt_1<%(qcdMT)s&&iso_1>0.05' % vars()
    else: qcd_cuts = 'mt_1<%(qcdMT)s' % vars()
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
  elif lowMT: mt_cut='mt_1<70'
  else: mt_cut='mt_1>70'

  if doW:
    # draw data for W+jets
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((n_deepbjets==0)*(%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      data_w.Add(h)

    # draw all (non-QCD) backgrounds to be subtracted from data for W estimation
    # only subtract non j->tau fake component
    for i in other_files+ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((n_deepbjets==0)*(%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
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
      t.Draw('%(var)s>>h' % vars(),'((n_deepbjets==0)*(%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      w_qcd_sub.Add(h)

    # draw MC for QCD subtraction
    for i in other_files+ttbar_files+wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((n_deepbjets==0)*(%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
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

    wjets = ZeroNegativeBins(wjets)

  if doTT:
    # draw ttbar when tau candidate is a jet fake 
    for i in ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(os==1)*(mt_1<70)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      ttbar.Add(h)

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
  f1 = ROOT.TF1("f1","landau",20,600)
  if h.GetBinContent(h.GetNbinsX()) > 0 and h.GetBinError(h.GetNbinsX())/h.GetBinContent(h.GetNbinsX()) <0.5:
    if 'wjets' in h.GetName() and channel == 'mt' and h.GetBinContent(h.GetNbinsX()-1) > 0 and h.GetBinError(h.GetNbinsX()-1)/h.GetBinContent(h.GetNbinsX()-1)<0.5:
      f2 = ROOT.TF1("f2","((x<140)*([0]*TMath::Landau(x,[1],[2])+[3])) + ([4]*(x>=140 && x<200)) + ([5]*(x>=200))",20,600)
    else:
      f2 = ROOT.TF1("f2","((x<200)*([0]*TMath::Landau(x,[1],[2])+[3])) + ([4]*(x>=200))",20,600)
  else:
    if 'wjets' in h.GetName() and channel == 'mt' and h.GetBinContent(h.GetNbinsX()-1) > 0 and h.GetBinError(h.GetNbinsX()-1)/h.GetBinContent(h.GetNbinsX()-1)<0.5:
      f2 = ROOT.TF1("f2","((x<140)*([0]*TMath::Landau(x,[1],[2])+[3])) + ([4]*(x>=140))",20,600)
    else:
      f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",20,600)

  if usePol1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",20,400)
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
    h.Fit("f1",'IRQ')
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0); f2.SetParLimits(3,0,1)
    if usePol1: f2.SetParameter(4,0)
  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    fitresult = h.Fit("f2",'SIRQ')
    rep = int(fitresult) != 0
    if not rep or count>100:
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')

  # Set binned value error to binned value error
  for i in range(0,h_uncert.GetNbinsX()+1):
    if h.GetBinContent(h.GetNbinsX()) > 0 and h.GetBinError(h.GetNbinsX())/h.GetBinContent(h.GetNbinsX()) <0.5:
      if 'wjets' in h.GetName() and channel == 'mt' and h.GetBinContent(h.GetNbinsX()-1) > 0 and h.GetBinError(h.GetNbinsX()-1)/h.GetBinContent(h.GetNbinsX()-1)<0.5:
        if h_uncert.GetBinLowEdge(i) >= 140 and (h_uncert.GetBinLowEdge(i) + h_uncert.GetBinWidth(i)) <= 200:
          h_uncert.SetBinError(i,h.GetBinError(h.GetNbinsX()-1))
        elif h_uncert.GetBinLowEdge(i) >= 200:
          h_uncert.SetBinError(i,h.GetBinError(h.GetNbinsX()))
      else:
        if h_uncert.GetBinLowEdge(i) >= 200:
          h_uncert.SetBinError(i,h.GetBinError(h.GetNbinsX()))

  return fit, h_uncert, h


def FitCorrection(h, func='pol1',is2D=False,fit_range=None):
  if is2D: 
    h_uncert = ROOT.TH2D(h.GetName()+'_uncert',"",100,h.GetXaxis().GetBinLowEdge(1),h.GetXaxis().GetBinLowEdge(h.GetNbinsX()+1),100,h.GetYaxis().GetBinLowEdge(1),h.GetYaxis().GetBinLowEdge(h.GetNbinsY()+1))
    f1 = ROOT.TF2("f1",func)
  else: 
    h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
    if fit_range is not None:
      f1 = ROOT.TF1("f1",func,fit_range[0],fit_range[1])
    else:
      f1 = ROOT.TF1("f1",func)

  if 'Erf' in func:
    f1.SetParameter(1,-1.5)
    f1.SetParameter(2,2)

  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    extra=''
    if fit_range is not None:
      extra='R'
    if 'Erf' in func: fitresult = h.Fit("f1",'SQ'+extra)
    else: fitresult = h.Fit("f1",'SIQ'+extra)
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
  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  f.SetMinimum(0)
  if f.GetMaximum() > 1.: f.SetMaximum(1.)
  f.SetStats(0)
  f.GetXaxis().SetTitle('p_{T}^{#tau_{h}} (GeV)')
  f.GetYaxis().SetTitle('F_{F}')
  f.SetTitle(name)
  f.SetLineColor(ROOT.kBlack)
  f.Draw()
  h.SetLineColor(ROOT.kRed)
  h.SetStats(0)
  h.SetFillColor(ROOT.kBlue-10)
  h.Draw("e3 same")
  f.Draw("a sames")
  l.AddEntry(f,"Observed","lep")
  l.AddEntry(h,"Fit","l")
  l.AddEntry(h,"Fit Uncertainty","f")
  l.Draw()

  c1.Print(output_folder+'/'+channel+'_'+year+'_'+wp+'_'+name+'_fit.pdf')
  #time.sleep(2)

def PlotFakeFactorCorrection(f, h, name, output_folder, wp,x_title='E_{T}^{miss} (GeV)'):
  c1 = ROOT.TCanvas()
  f.SetMinimum(0)
  f.SetStats(0)
  f.GetXaxis().SetTitle(x_title)
  f.GetYaxis().SetTitle('Closure Correction')
  f.SetTitle(name)
  f.SetLineColor(ROOT.kBlack)
  f.Draw()
  h.SetStats(0)
  h.SetFillColor(ROOT.kBlue-10)
  h.Draw("e3 same")
  f.Draw("a sames")
  c1.Print(output_folder+'/'+channel+'_'+wp+'_'+name+'_fit.pdf')

def WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',ss=False):
  # this function loops over all njets and jetpt bins and write the FFs as a function
  ff_eqn_tot = '('
  ff_params = {}
  for njetbin_name,njetbin_cut in njets_bins.items():
    for jetptbin_name,jetptbin_cut in jetpt_bins.items():
      if not (jetptbin_name == 'inclusive' or njetbin_name == 'inclusive'):
        fout.cd()
        extra=''
        if proc=='qcd': 
          if ss: extra = '_aiso_ss'
          else: extra = '_aiso_os'
        func = fout.Get('%(jetptbin_name)s_%(njetbin_name)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
        ff = func.GetExpFormula('p')
        ff_params = str(ff).replace('x','min(pt_2,599.)') 
        
        ff_eqn_tot += '(%(njetbin_cut)s*%(jetptbin_cut)s*(%(ff_params)s))+' % vars()
        
  ff_eqn_tot = ff_eqn_tot[:-1]+')'
  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  return ff_eqn_tot

def WriteFakeFactorFunctionTTbar(fout,njets_bins,jetpt_bins,proc='ttbar_mc'):
  # this function loops over all njets and jetpt bins and write the FFs as a function
  ff_eqn_tot = ''
  ff_params = {}
  for jetptbin_name,jetptbin_cut in jetpt_bins.items():
    if not (jetptbin_name == 'inclusive'):
      fout.cd()
      func = fout.Get('%(jetptbin_name)s_inclusive_pt_2_ff_%(proc)s_fit' % vars())
      ff = func.GetExpFormula('p')
      ff_params = str(ff).replace('x','min(pt_2,599.)')

      ff_eqn_tot += '(%(jetptbin_cut)s*(%(ff_params)s))+' % vars()

  ff_eqn_tot = ff_eqn_tot[:-1]
  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  return ff_eqn_tot

def ClosureCorrection(fout,closure_dict,x_title,ff,proc='wjets',completing_bin=None,qcd_mt='50',do_iso_qcd=False,low_mt=False):
  corr_string = "*(" 
 
  for closure_name, _ in closure_dict.items():

    bin_cut = closure_dict[closure_name][0]
    var = closure_dict[closure_name][1]
    fit_type = closure_dict[closure_name][2]
    
    if closure_dict[closure_name][3] == None:
      replace_var = var.split('[')[0]
    else:
      replace_var = "min({},{})".format(var.split('[')[0],closure_dict[closure_name][3])

    corr_name = var.split('[')[0] + '_' + closure_name

    if proc == 'wjets':
      (_,data_hist,_,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+bin_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=True,doQCD=False,doTT=False,lowMT=low_mt)
      (_,pred_hist,_,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+bin_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=False,doW=True,doQCD=False,doTT=False,lowMT=low_mt)
    elif proc == 'wjets_mc':
      if not low_mt:
        (_,_,data_hist,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+bin_cut+'))', corr_name+'_highmt_closure' % vars(),input_folder,file_ext,doMC=True,doW=False,doQCD=False,doTT=False,lowMT=False)
        (_,_,pred_hist,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+bin_cut+'))', corr_name+'_highmt_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=True,doW=False,doQCD=False,doTT=False,lowMT=False)
      else:
        (_,_,data_hist,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+bin_cut+'))', corr_name+'_lowmt_closure' % vars(),input_folder,file_ext,doMC=True,doW=False,doQCD=False,doTT=False,lowMT=True)
        (_,_,pred_hist,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+bin_cut+'))', corr_name+'_lowmt_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=True,doW=False,doQCD=False,doTT=False,lowMT=True)
    elif proc == 'ttbar_mc':
      (_,_,_,data_hist) = DrawHists(var, '(('+baseline_iso_pass+')*('+bin_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=False,doTT=True)
      (_,_,_,pred_hist) = DrawHists(var, '(('+baseline_iso_fail+')*('+bin_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=False,doW=False,doQCD=False,doTT=True)
    elif proc == 'qcd_os':
      (data_hist,_,_,_) = DrawHists(var, '(('+baseline_aiso_pass+')*('+bin_cut+'))', corr_name+'_os_aiso_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False,qcdMT=qcd_mt,doOS=True)
      (pred_hist,_,_,_) = DrawHists(var, '(('+baseline_aiso_fail+')*('+bin_cut+'))', corr_name+'_os_aiso_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=False,doW=False,doQCD=True,doTT=False,qcdMT=qcd_mt,doOS=True)
    elif proc == 'qcd_ss':
      if not do_iso_qcd:
        (data_hist,_,_,_) = DrawHists(var, '(('+baseline_aiso_pass+')*('+bin_cut+'))', corr_name+'_ss_aiso_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False,qcdMT=qcd_mt,doOS=False)
        (pred_hist,_,_,_) = DrawHists(var, '(('+baseline_aiso_fail+')*('+bin_cut+'))', corr_name+'_ss_aiso_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=False,doW=False,doQCD=True,doTT=False,qcdMT=qcd_mt,doOS=False)
      else:
        (data_hist,_,_,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+bin_cut+'))', corr_name+'_ss_iso_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False,qcdMT=qcd_mt,doOS=False)
        (pred_hist,_,_,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+bin_cut+'))', corr_name+'_ss_iso_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff+")",doMC=False,doW=False,doQCD=True,doTT=False,qcdMT=qcd_mt,doOS=False)

    fout.cd()
    data_hist.Divide(pred_hist)

    data_fit, data_uncert =  FitCorrection(data_hist,func=fit_type)

    data_hist.Write()
    data_fit.Write()
    data_uncert.Write()
    PlotFakeFactorCorrection(data_hist, data_uncert, data_hist.GetName(), output_folder, wp, x_title=x_title)

    if 'qcd' in proc:
      if do_iso_qcd:
        corr_string += '((%s)*(%s))+' % (bin_cut,str(fout.Get(corr_name+'_'+proc.split('_')[1]+'_iso_closure_qcd_fit').GetExpFormula('p')).replace('x',replace_var))
      else:
        corr_string += '((%s)*(%s))+' % (bin_cut,str(fout.Get(corr_name+'_'+proc.split('_')[1]+'_aiso_closure_qcd_fit').GetExpFormula('p')).replace('x',replace_var))
    elif 'wjets_mc' in proc:
      if low_mt:
        corr_string += '((%s)*(%s))+' % (bin_cut,str(fout.Get(corr_name+'_lowmt_closure_wjets_mc_fit').GetExpFormula('p')).replace('x',replace_var))
      else:
        corr_string += '((%s)*(%s))+' % (bin_cut,str(fout.Get(corr_name+'_highmt_closure_wjets_mc_fit').GetExpFormula('p')).replace('x',replace_var))
    else:
      corr_string += '((%s)*(%s))+' % (bin_cut,str(fout.Get(corr_name+'_closure_'+proc+'_fit').GetExpFormula('p')).replace('x',replace_var))
  
  if completing_bin == None:
    corr_string = corr_string[:-1] + ')'
  else:
    corr_string += '('+completing_bin+'))' # For DR->AR closures 

  return corr_string

# variable and binning used for fitting
var1='pt_2[20,25,30,35,40,45,50,55,60,70,80,100,120,140,200,600]'

# set up histograms to draw
ff_list = {}
for njetbin in njets_bins:
  for jetptbin in jetpt_bins:
    name= jetptbin+'_'+njetbin
    cuts="("+njets_bins[njetbin]+'&&'+jetpt_bins[jetptbin]+")"
    # add isolated
    ff_list[name+'_pt_2'] = (var1, baseline_iso_pass+"*"+cuts, baseline_iso_fail+"*"+cuts)
    # add anti-isolated same sign
    ff_list[name+'_aiso_ss_pt_2'] = (var1, baseline_aiso_pass+"*"+cuts, baseline_aiso_fail+"*"+cuts)
    # add anti-isolated opposite sign
    ff_list[name+'_aiso_os_pt_2'] = (var1, baseline_aiso_pass+"*"+cuts, baseline_aiso_fail+"*"+cuts)


# calculate fake factors
to_write = []
for ff in ff_list:
  if draw:
    if 'aiso' in ff:
      (qcd_pass, _, _, _) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('_os_' in ff),doQCD=True,doW=False,doMC=False,doTT=False)
      (qcd_fail, _, _, _) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('_os_' in ff),doQCD=True,doW=False,doMC=False,doTT=False)

      qcd_ff = CalculateFakeFactors(qcd_pass, qcd_fail)
      to_write.append(qcd_ff)

    else:
      (_, wjets_pass, wjets_mc_pass, ttbar_mc_pass) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=True,doQCD=False,doW=True,doMC=True,doTT=True)
      (_, wjets_fail, wjets_mc_fail, ttbar_mc_fail) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=True,doQCD=False,doW=True,doMC=True,doTT=True)

      wjets_ff = CalculateFakeFactors(wjets_pass, wjets_fail)
      to_write.append(wjets_ff)

      wjets_mc_ff = CalculateFakeFactors(wjets_mc_pass, wjets_mc_fail)
      to_write.append(wjets_mc_ff)

      ttbar_mc_ff = CalculateFakeFactors(ttbar_mc_pass, ttbar_mc_fail)
      to_write.append(ttbar_mc_ff)
  else:
    # if not drawing histogram then retrieve them from the old output folder
    fin = ROOT.TFile(out_file)
    qcd_ff = fin.Get(ff+'_ff_qcd')
    qcd_ff.SetDirectory(0)
    to_write.append(qcd_ff)
    if not 'aiso' in ff:
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


  # fit fake factors
  if 'aiso' in ff:
    (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff)
    to_write.append(qcd_fit)
    to_write.append(qcd_uncert)
    PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp)
  else:
    (wjets_fit, wjets_uncert, wjets_ff) = FitFakeFactors(wjets_ff)
    to_write.append(wjets_fit)
    to_write.append(wjets_uncert)
    PlotFakeFactor(wjets_ff, wjets_uncert, wjets_ff.GetName(), output_folder, wp)

    (wjets_mc_fit, wjets_mc_uncert, wjets_mc_ff) = FitFakeFactors(wjets_mc_ff)
    to_write.append(wjets_mc_fit)
    to_write.append(wjets_mc_uncert)
    PlotFakeFactor(wjets_mc_ff, wjets_mc_uncert, wjets_mc_ff.GetName(), output_folder, wp)

    (ttbar_mc_fit, ttbar_mc_uncert, ttbar_mc_ff) = FitFakeFactors(ttbar_mc_ff)
    to_write.append(ttbar_mc_fit)
    to_write.append(ttbar_mc_uncert)
    PlotFakeFactor(ttbar_mc_ff, ttbar_mc_uncert, ttbar_mc_ff.GetName(), output_folder, wp)

# make fractions
nbjets_cats = {'nbjets0':'n_deepbjets==0','nbjets1':'n_deepbjets>0'}

for nbjets_name,nbjets_cut in nbjets_cats.items():
  var= 'mt_1[0,10,20,30,40,50,60,70,80,90,100,120]'
  cuts = '(%(baseline_iso_fail)s)*(%(nbjets_cut)s)' % vars()
  name = '%(channel)s_fracs_%(nbjets_name)s' % vars()
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

# Print fake factor strings before closures
print "---------------------------------------------------------------------------------------------------"
print "Fake Factor Strings Before Closure Corrections"
print "---------------------------------------------------------------------------------------------------"
ff_qcd_os = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',ss=False)
print "QCD OS Fake Factors:"
print "ff_string='(" + ff_qcd_os + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_qcd_ss = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',ss=True)
print "QCD SS Fake Factors:"
print "ff_string='(" + ff_qcd_ss + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_wjets = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='wjets')
print "W + Jets Fake Factors:"
print "ff_string='(" + ff_wjets + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_wjets_mc = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='wjets_mc')
print "W + Jets MC Fake Factors:"
print "ff_string='(" + ff_wjets_mc + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_ttbar_mc = WriteFakeFactorFunctionTTbar(fout,njets_bins,jetpt_bins,proc='ttbar_mc')
print "ttbar Fake Factors:"
print "ff_string='(" + ff_ttbar_mc + ")'"
print "---------------------------------------------------------------------------------------------------"


############ Do closures ##############

# Dictionaries for closures set up like:
# {bin_name:[bin_condition,correction_var_and_binning,fit_type,maximum_fit_value]}

# W + Jets pt_1 correction
if channel == "et" and year == "2016":
  closure_dict = {'0jet':['n_prebjets==0','pt_1[20,25,40,60,80,100,120,160,200,300]','pol1','160'],
                  '1jet':['n_prebjets>0 ','pt_1[20,25,40,60,80,100,120,160,200,300]','pol3','200']}
else:
  closure_dict = {'low_pt_1_0jet ':['(pt_1<=%(crosstrg_pt)s && n_prebjets==0)' % vars(),'pt_1[0,%(crosstrg_pt)s]' % vars()        ,'pol0',None ],
                  'low_pt_1_1jet ':['(pt_1<=%(crosstrg_pt)s && n_prebjets>0) ' % vars(),'pt_1[0,%(crosstrg_pt)s]' % vars()        ,'pol0',None ],
                  'high_pt_1_0jet':['(pt_1>%(crosstrg_pt)s && n_prebjets==0)' % vars() ,'pt_1[20,25,40,60,80,100,120,160,200,300]','pol1','160'],
                  'high_pt_1_1jet':['(pt_1>%(crosstrg_pt)s && n_prebjets>0)' % vars()  ,'pt_1[20,25,40,60,80,100,120,160,200,300]','pol3','200']}


w_corr_string    = ClosureCorrection(fout,closure_dict,'p_{T}^{#mu} (GeV)',ff_wjets   ,proc='wjets'   ,completing_bin=None)
w_mc_corr_string = ClosureCorrection(fout,closure_dict,'p_{T}^{#mu} (GeV)',ff_wjets_mc,proc='wjets_mc',completing_bin=None)

# W + Jets MET correction
closure_dict = {'0jet':['n_prebjets==0','met[0,30,45,60,80,100,120,160,200,300]','pol3','250'],
                '1jet':['n_prebjets>0 ','met[0,30,45,60,80,100,120,160,200,300]','pol3','250']}

w_corr_string    += ClosureCorrection(fout,closure_dict,'MET (GeV)','(' + ff_wjets + w_corr_string + ')'      ,proc='wjets'   ,completing_bin=None)
w_mc_corr_string += ClosureCorrection(fout,closure_dict,'MET (GeV)','(' + ff_wjets_mc + w_mc_corr_string + ')',proc='wjets_mc',completing_bin=None)

# W + Jets pt_1 DR -> AR correction
closure_dict = {'nbjets0_tightmt':['(n_deepbjets==0 && mt_1<50)'           ,'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol1',None],
                'nbjets0_loosemt':['(n_deepbjets==0 && mt_1>50 && mt_1<70)','pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol1',None],
                'nbjets1_tightmt':['(n_deepbjets>0 && mt_1<50)'            ,'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol0',None],
                'nbjets1_loosemt':['(n_deepbjets>0 && mt_1>50 && mt_1<70)' ,'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol0',None]}

highmt_to_lowmt_corr = ClosureCorrection(fout,closure_dict,'p_{T}^{#mu} (GeV)','(' + ff_wjets_mc + w_mc_corr_string + ')',proc='wjets_mc',completing_bin='mt_1>70',low_mt=True) 
w_corr_string += highmt_to_lowmt_corr
w_mc_corr_string += highmt_to_lowmt_corr

# QCD MET correction
closure_dict = {'0jet':['n_prebjets==0','met[0,30,45,60,80,100,120,160,200,300]','pol3','140'],
                '1jet':['n_prebjets>0 ','met[0,30,45,60,80,100,120,160,200,300]','pol3','140']}

qcd_ss_corr_string = ClosureCorrection(fout,closure_dict,'MET (GeV)',ff_qcd_ss,proc='qcd_ss',completing_bin=None)
qcd_os_corr_string = ClosureCorrection(fout,closure_dict,'MET (GeV)',ff_qcd_os,proc='qcd_os',completing_bin=None)

# QCD pt_1 DR -> AR correction
closure_dict = {'nbjets0_tightmt':['(n_deepbjets==0 && mt_1<50)'           ,'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol3','250'],
                'nbjets0_loosemt':['(n_deepbjets==0 && mt_1>50 && mt_1<70)','pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol3','250'],
                'nbjets1_tightmt':['(n_deepbjets>0 && mt_1<50)'            ,'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol3','250'],
                'nbjets1_loosemt':['(n_deepbjets>0 && mt_1>50 && mt_1<70)' ,'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200,300]' % vars(),'pol3','250']}

aiso_to_iso_corr = ClosureCorrection(fout,closure_dict,'p_{T}^{#mu} (GeV)','(' + ff_qcd_ss + qcd_ss_corr_string + ')',proc='qcd_ss',qcd_mt='70',do_iso_qcd=True)

qcd_ss_corr_string += aiso_to_iso_corr
qcd_os_corr_string += aiso_to_iso_corr

# ttbar pt_1 correction
if channel == "et" and year == "2016":
  closure_dict = {'tightmt':['mt_1<50 ','pt_1[20,25,40,60,80,100,120,160,200,300]','pol1','300'],
                  'loosemt':['mt_1>=50','pt_1[20,25,40,60,80,100,120,160,200,300]','pol1','300']}
else:
  closure_dict = {'low_pt_1_tightmt ':['(pt_1<=%(crosstrg_pt)s && mt_1<50)' % vars() ,'pt_1[0,%(crosstrg_pt)s]' % vars()        ,'pol0',None ],
                  'low_pt_1_loosemt ':['(pt_1<=%(crosstrg_pt)s && mt_1>=50)' % vars(),'pt_1[0,%(crosstrg_pt)s]' % vars()        ,'pol0',None ],
                  'high_pt_1_tightmt':['(pt_1>%(crosstrg_pt)s && mt_1<50)' % vars()  ,'pt_1[20,25,40,60,80,100,120,160,200,300]','pol1','300'],
                  'high_pt_1_loosemt':['(pt_1>%(crosstrg_pt)s && mt_1>=50)' % vars() ,'pt_1[20,25,40,60,80,100,120,160,200,300]','pol1','300']}

ttbar_mc_corr_string = ClosureCorrection(fout,closure_dict,'p_{T}^{#mu} (GeV)',ff_ttbar_mc,proc='ttbar_mc',completing_bin=None) 


fout.Close()

# print fake factor strings after closures
print "---------------------------------------------------------------------------------------------------"
print "Fake Factor Strings After Closure Corrections"
print "---------------------------------------------------------------------------------------------------"
print "QCD OS Fake Factors:"
print "ff_string='((" + ff_qcd_os + ")" + qcd_os_corr_string + ")'"
print "---------------------------------------------------------------------------------------------------"
print "QCD SS Fake Factors:"
print "ff_string='((" + ff_qcd_ss + ")" + qcd_ss_corr_string + ")'"
print "---------------------------------------------------------------------------------------------------"
print "W + Jets Fake Factors:"
print "ff_string='((" + ff_wjets + ")" + w_corr_string + ")'"
print "---------------------------------------------------------------------------------------------------"
print "W + Jets MC Fake Factors:"
print "ff_string='((" + ff_wjets_mc + ")" + w_mc_corr_string + ")'"
print "---------------------------------------------------------------------------------------------------"
print "ttbar Fake Factors:"
print "ff_string='((" + ff_ttbar_mc + ")" + ttbar_mc_corr_string + ")'"
print "---------------------------------------------------------------------------------------------------"

