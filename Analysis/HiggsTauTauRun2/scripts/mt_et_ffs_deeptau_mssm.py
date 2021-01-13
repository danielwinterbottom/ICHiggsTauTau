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
parser.add_argument('--cms_label', help= 'Draw cms label', default=False)
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
#              '0jet':'(n_prebjets==0)',
#              '1jet':'(n_prebjets>0)',
}

jetpt_bins = {
#              'inclusive':'(1)',
              'jet_pt_low':'(jet_pt_2<1.25*pt_2)',
              'jet_pt_med':'((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.5*pt_2))',
              'jet_pt_high': '(jet_pt_2>=1.5*pt_2)'
}


# get files and cross trigger pT threshold for relevant year
if year == '2018':
  lumi = 58826.8469
  params_file = 'scripts/params_mssm_2018.json'
  input_folder = '/vols/cms/dw515/Offline/output/MSSM/mssm_2018_jan06/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonA',
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'TauA',
      'TauB',
      'TauC',
      'TauD'
    ]
  elif channel == "et":
    crosstrg_pt = 33
    data_files = [
      'EGammaA',
      'EGammaB',
      'EGammaC',
      'EGammaD',
      'TauA',
      'TauB',
      'TauC',
      'TauD'
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
  input_folder = '/vols/cms/dw515/Offline/output/MSSM/trg_check_2017_v5/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF'
    ]
  elif channel == "et":
    crosstrg_pt = 28
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF'
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
  input_folder = '/vols/cms/dw515/Offline/output/MSSM/trg_check_2016_v5/'

  if channel == "mt":
    crosstrg_pt = 23
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF',
      'SingleMuonG',
      'SingleMuonH',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF',
      'TauG',
      'TauH'
    ]
  elif channel == "et":
    crosstrg_pt = 25
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
      'SingleElectronG',
      'SingleElectronH',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF',
      'TauG',
      'TauH'
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
#iso = 'iso_1<0.15'
#anti_iso = 'iso_1<0.5 && iso_1>0.25'
#deeptau_jet_pass = 'deepTauVsJets_%(wp)s_2>0.5' % vars()
#deeptau_jet_fail = 'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_%(alt_wp)s_2>0.5' % vars()
#if channel == "mt":
#  deeptau_lep = 'deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5'
#  if year == "2016":
#    crosstrg = '((trg_mutaucross&&pt_1<%(crosstrg_pt)s&&pt_2>25)||(trg_singlemuon&&pt_1>%(crosstrg_pt)s))' % vars()
#  else: crosstrg = '((trg_mutaucross&&pt_1<%(crosstrg_pt)s&&pt_2>32)||(trg_singlemuon&&pt_1>%(crosstrg_pt)s))' % vars()
#elif channel == "et":
#  deeptau_lep = "deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5"
#  if year == "2016":
#    crosstrg = 'trg_singleelectron'
#  else:
#    crosstrg = "((trg_etaucross&&pt_1<%(crosstrg_pt)s&&pt_2>35)||(trg_singleelectron&&pt_1>%(crosstrg_pt)s))" % vars() 
#other_sel = 'leptonveto==0 && pt_2>30 && wt<2'


#baseline_iso_pass = '(%(iso)s && %(deeptau_jet_pass)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
#baseline_iso_fail = '(%(iso)s && %(deeptau_jet_fail)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
#baseline_aiso_pass = '(%(anti_iso)s && %(deeptau_jet_pass)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
#baseline_aiso_fail = '(%(anti_iso)s && %(deeptau_jet_fail)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
if year == "2016":
  m_lowpt=23
  e_lowpt=26
  t_highpt=120
  t_lowpt_mt=25
  t_lowpt_et=25
elif year == "2017":
  m_lowpt=25
  e_lowpt=28
  t_highpt=180
  t_lowpt_mt=32
  t_lowpt_et=35
elif year == "2018":
  m_lowpt=25
  e_lowpt=33
  t_highpt=180
  t_lowpt_mt=32
  t_lowpt_et=35

if channel == 'mt':
  baseline_iso_pass = '(iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && !leptonveto && pt_2>30 && ((trg_mutaucross&&pt_2>%(t_lowpt_mt)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(m_lowpt)s)||(trg_singlemuon&&pt_1>=%(m_lowpt)s)||(trg_singletau_2&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
elif channel == 'et':
  baseline_iso_pass = '(iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && pt_2>30 && ((trg_etaucross&&pt_2>%(t_lowpt_et)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(e_lowpt)s)||(trg_singleelectron&&pt_1>=%(e_lowpt)s)||(trg_singletau_2&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
        
baseline_iso_fail = baseline_iso_pass.replace('deepTauVsJets_%(wp)s_2>0.5' % vars(),'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_%(alt_wp)s_2>0.5' % vars())
baseline_aiso_pass = baseline_iso_pass.replace('iso_1<0.15','iso_1<0.5 && iso_1>0.25')
baseline_aiso_fail = baseline_iso_fail.replace('iso_1<0.15','iso_1<0.5 && iso_1>0.25')


def fraction_rebinning(var,cat_name,qcd,wjets,ttbar):
  bins_list = var.split('[')[1].split(']')[0].split(',')
  hist_same = False
  while not hist_same:
    total = qcd.Clone(); total.Add(wjets); total.Add(ttbar)
    qcd_frac = qcd.Clone()
    wjets_frac = wjets.Clone()
    ttbar_frac = ttbar.Clone()

    qcd_frac.Divide(total)
    wjets_frac.Divide(total)
    ttbar_frac.Divide(total)

    # Gets bins to remove from fractions
    rm_list = []
    bin_filled = False
    bin_thesh = 0.1
    for i in range(1,qcd_frac.GetNbinsX()+1):
      if qcd_frac.GetBinContent(i)==0 and wjets_frac.GetBinContent(i)==0 and ttbar_frac.GetBinContent(i)==0:
        rm_list.append(str(int(qcd_frac.GetBinLowEdge(i))))
      elif 'nbjets0' in cat_name and (qcd_frac.GetBinError(i)>bin_thesh or wjets_frac.GetBinError(i)>bin_thesh) and bin_filled: # bin_filled to stop it removing lowest bin if low stat
        rm_list.append(str(int(qcd.GetBinLowEdge(i))))
      elif 'nbjets0' in cat_name and (qcd_frac.GetBinContent(i)==0 and wjets_frac.GetBinError(i)>bin_thesh) and bin_filled:
        rm_list.append(str(int(qcd.GetBinLowEdge(i))))
      elif 'nbjets0' in cat_name and (qcd_frac.GetBinError(i)>bin_thesh and wjets_frac.GetBinContent(i)==0) and bin_filled:
        rm_list.append(str(int(qcd.GetBinLowEdge(i))))
      elif 'nbjets1' in cat_name and (ttbar_frac.GetBinError(i)>bin_thesh or wjets_frac.GetBinError(i)>bin_thesh) and bin_filled:
        rm_list.append(str(int(qcd.GetBinLowEdge(i))))
      elif 'nbjets1' in cat_name and (ttbar_frac.GetBinContent(i)==0 or wjets_frac.GetBinError(i)>bin_thesh) and bin_filled:
        rm_list.append(str(int(qcd.GetBinLowEdge(i))))
      elif 'nbjets1' in cat_name and (ttbar_frac.GetBinError(i)>bin_thesh or wjets_frac.GetBinContent(i)==0) and bin_filled:
        rm_list.append(str(int(qcd.GetBinLowEdge(i))))
      else:
        bin_filled = True

    for i in rm_list: bins_list.remove(i)

    # Set up new histograms with new binning
    new_bins = array('f', map(float,bins_list))
    qcd_nb = ROOT.TH1D("qcd_nb","",len(new_bins)-1, new_bins)
    ttbar_nb = ROOT.TH1D("ttbar_nb","",len(new_bins)-1, new_bins)
    wjets_nb = ROOT.TH1D("wjets_nb","",len(new_bins)-1, new_bins)

    # Get bin content for new histogram
    for i in range(1,qcd.GetNbinsX()+1):
      for j in range(1,qcd_nb.GetNbinsX()+1):
        if qcd.GetBinLowEdge(i) == qcd_nb.GetBinLowEdge(j) and qcd.GetBinLowEdge(i+1) == qcd_nb.GetBinLowEdge(j+1):
          qcd_nb.SetBinContent(j,qcd.GetBinContent(i))
          qcd_nb.SetBinError(j,qcd.GetBinError(i))
          wjets_nb.SetBinContent(j,wjets.GetBinContent(i))
          wjets_nb.SetBinError(j,wjets.GetBinError(i))
          ttbar_nb.SetBinContent(j,ttbar.GetBinContent(i))
          ttbar_nb.SetBinError(j,ttbar.GetBinError(i))
        elif qcd.GetBinLowEdge(i) == qcd_nb.GetBinLowEdge(j):
          k = 0
          new_bin_content_qcd = 0
          new_bin_error2_qcd = 0
          new_bin_content_wjets = 0
          new_bin_error2_wjets = 0
          new_bin_content_ttbar = 0
          new_bin_error2_ttbar = 0
          while qcd.GetBinLowEdge(i+k) <= qcd_nb.GetBinLowEdge(j+1):
            new_bin_content_qcd += qcd.GetBinContent(i+k)
            new_bin_error2_qcd += (qcd.GetBinError(i+k)**2)
            new_bin_content_wjets += wjets.GetBinContent(i+k)
            new_bin_error2_wjets += (wjets.GetBinError(i+k)**2)
            new_bin_content_ttbar += ttbar.GetBinContent(i+k)
            new_bin_error2_ttbar += (ttbar.GetBinError(i+k)**2)
            k += 1
          qcd_nb.SetBinContent(j,new_bin_content_qcd)
          qcd_nb.SetBinError(j,new_bin_error2_qcd**0.5)
          wjets_nb.SetBinContent(j,new_bin_content_wjets)
          wjets_nb.SetBinError(j,new_bin_error2_wjets**0.5)
          ttbar_nb.SetBinContent(j,new_bin_content_ttbar)
          ttbar_nb.SetBinError(j,new_bin_error2_ttbar**0.5)

    hist_same = True
    for i in range(1,qcd_nb.GetNbinsX()+1):
      if qcd.GetBinLowEdge(i) != qcd_nb.GetBinLowEdge(i):
        hist_same = False

    qcd = qcd_nb.Clone()
    wjets = wjets_nb.Clone()
    ttbar = ttbar_nb.Clone()

  return (qcd, wjets, ttbar, new_bins)


def ZeroNegativeBins(h):
  # if a bin of h1 is equal to 0 ir negative then remove it
  # also remove non zero bins with > 100% errors
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def ZeroLargeErrorBins(h):
  # remove non zero bins with > 100% errors
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) != 0:
      if h.GetBinError(i)/h.GetBinContent(i) > 1.:
        print 'error>1 for, ', h.GetName(), 'bin=', i
        h.SetBinContent(i,0)
        h.SetBinError(i,0)
  return h

def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=True,doTT=True,doIso=True,fullMT=False,lowMT=False,qcdMT='50'):
  add_wt = "(" + add_wt + "*wt_tau_trg_mssm*wt_tau_id_mssm)"
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
      print i
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(os==1)*(mt_1<70)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      h.Print("all")
      scale = lumi*params[i]['xs']/params[i]['evt']
      print scale
      h.Scale(scale)
      ttbar.Add(h)

    ttbar = ZeroNegativeBins(ttbar)

  return (data_qcd, data_w, wjets, ttbar)

def DrawHistsForFractions(var_input, cuts, name, input_folder, file_ext):
  add_wt = "(wt_tau_trg_mssm*wt_tau_id_mssm)"
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
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)*%(add_wt)s' % vars(),'goff')
    h = t.GetHistogram()
    data.Add(h)

  # draw wjets when the tau candidates is a jet fake - this includes VV and DY events also
  for i in wjets_files+other_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*%(add_wt)s*(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'wt*%(add_wt)s*(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'wt*%(add_wt)s*(%(cuts)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'wt*%(add_wt)s*(%(cuts)s)*((%(gen_extra)s)==0)' % vars(),'goff')
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
  ff = ZeroLargeErrorBins(ff)
  return ff

def FitFakeFactors(h,usePol1=False,polOnly=None):

  print "Getting fit for: {}".format(h.GetName())

  #for jet_pt_high bins in et channel set maximum bin to 120 GeV as higher bins often aren't filled and when are filled are often not sensible (large statistical uncertainties etc)
  #if channel == 'et' and 'jet_pt_high' in h.GetName():
  #    'Zeroing bins > 130 for ', h.GetName()
  #    for i in range(1,h.GetNbinsX()+1):
  #      if h.GetBinLowEdge(i)>=130:
  #        h.SetBinContent(h.GetNbinsX(),0)
  #        h.SetBinError(h.GetNbinsX(),0)

  h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
  f1 = ROOT.TF1("f1","landau",20,600)
  just_fit = False
  if h.GetBinContent(h.GetNbinsX()) > 0 and h.GetBinError(h.GetNbinsX())/h.GetBinContent(h.GetNbinsX()) <0.5:
    if 'wjets' in h.GetName() and channel == 'mt' and h.GetBinContent(h.GetNbinsX()-1) > 0 and h.GetBinError(h.GetNbinsX()-1)/h.GetBinContent(h.GetNbinsX()-1)<0.5 and not ('wjets_mc' in h.GetName() and 'pt_low_1jet' in h.GetName() and year == '2016'):
      f2 = ROOT.TF1("f2","((x<140)*([0]*TMath::Landau(x,[1],[2])+[3])) + ([4]*(x>=140 && x<200)) + ([5]*(x>=200))",20,600)
      bin_type = 1
    else:
      f2 = ROOT.TF1("f2","((x<200)*([0]*TMath::Landau(x,[1],[2])+[3])) + ([4]*(x>=200))",20,600)
      bin_type = 2 
  else:
    if 'wjets' in h.GetName() and channel == 'mt' and h.GetBinContent(h.GetNbinsX()-1) > 0 and h.GetBinError(h.GetNbinsX()-1)/h.GetBinContent(h.GetNbinsX()-1)<0.5:
      f2 = ROOT.TF1("f2","((x<140)*([0]*TMath::Landau(x,[1],[2])+[3])) + ([4]*(x>=140))",20,600)
      bin_type = 3
    else:
      just_fit = 	True
      h.SetBinContent(h.GetNbinsX(),0)
      h.SetBinError(h.GetNbinsX(),0)
      f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",20,600)
      bin_type = 0

  if usePol1: 
    if bin_type == 0: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",20,600)
    elif bin_type == 1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x + ([5]*(x>=140 && x<200)) + ([6]*(x>=200))",20,600)
    elif bin_type == 2: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x + ([5]*(x>=200))",20,600)
    elif bin_type == 3: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x + ([5]*(x>=140))",20,600)
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
    status = h.Fit("f1",'SNIR')
    if int(status) != 0:
      print "Doing new fit because other fit failed"
      if bin_type == 0: f2 = ROOT.TF1("f2","[0]+[1]*x",20,600)
      elif bin_type == 1: f2 = ROOT.TF1("f2","([0]+[1]*x)*(x<140) + ([2]*(x>=140 && x<200)) + ([3]*(x>=200))",20,600)
      elif bin_type == 2: f2 = ROOT.TF1("f2","([0]+[1]*x)*(x<200) + ([2]*(x>=200))",20,600)
      elif bin_type == 3: f2 = ROOT.TF1("f2","([0]+[1]*x)*(x<140) + ([2]*(x>=140))",20,600)
    else:
      f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0); f2.SetParLimits(3,0,1)
      if usePol1: f2.SetParameter(4,0)
  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    fitresult = h.Fit("f2",'NSIR')
    rep = int(fitresult) != 0
    if not rep or count>100:
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      f2.Print("all")
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
   
  # Set fit > x_max to fit at x_max
  if just_fit:
    for i in range(h.GetNbinsX()+1,0,-1):
      if h.GetBinContent(i) > 0:
        break
    max_bin = h.GetBinLowEdge(i+1)
    if int(status) != 0:
      p0,p1 = fit.GetParameter(0),fit.GetParameter(1)
      f2 = ROOT.TF1(h.GetName()+'_fit',"%(p0)s+%(p1)s*x" % vars(),20,600)
    else:
      p0,p1,p2,p3 = fit.GetParameter(0),fit.GetParameter(1),fit.GetParameter(2),fit.GetParameter(3)
      fit = ROOT.TF1(h.GetName()+'_fit',"%(p0)s*TMath::Landau(min(x,%(max_bin)s),%(p1)s,%(p2)s)+%(p3)s" % vars(),20,600)
    for j in range(0,h_uncert.GetNbinsX()+1):
      if h_uncert.GetBinLowEdge(j) >= max_bin:
        h_uncert.SetBinContent(j,h_uncert.GetBinContent(h_uncert.GetXaxis().FindBin(max_bin)))
        h_uncert.SetBinError(j,h_uncert.GetBinError(h_uncert.GetXaxis().FindBin(max_bin)))

  return fit, h_uncert, h


def FitCorrection(h, func='pol1',is2D=False,fit_range=None):

  h = ZeroLargeErrorBins(h)

  if is2D: 
    h_uncert = ROOT.TH2D(h.GetName()+'_uncert',"",100,h.GetXaxis().GetBinLowEdge(1),h.GetXaxis().GetBinLowEdge(h.GetNbinsX()+1),100,h.GetYaxis().GetBinLowEdge(1),h.GetYaxis().GetBinLowEdge(h.GetNbinsY()+1))
    f1 = ROOT.TF2("f1",func)
  else: 
    if 'pol0' in func: h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
    else: h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
    if fit_range is not None:
      f1 = ROOT.TF1("f1",func,fit_range[0],fit_range[1])
    else:
      f1 = ROOT.TF1("f1",func)

  if 'Erf' in func:
    f1.SetParameter(1,-1.5)
    f1.SetParameter(2,2)

  if 'Erf((-x-[1])' in func:
    f1.SetParameter(0,0.1)
    f1.SetParameter(1,-40)
    f1.SetParameter(2,10)
    f1.SetParameter(3,1)


  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    extra=''
    if fit_range is not None:
      extra='R'
    if 'Erf' in func: fitresult = h.Fit("f1",'S'+extra)
    else: fitresult = h.Fit("f1",'SI'+extra)
    rep = int(fitresult) != 0
    if not rep or count>100:
      if 'pol0' not in func: ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f1
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')

  if 'pol0' in func:
    c = fit.GetParameter(0)
    e = fit.GetParError(0)
    print 'force pol0 error:'
    print c, e
    for i in range(1,h_uncert.GetNbinsX()+2):
      h_uncert.SetBinContent(i,c)
      h_uncert.SetBinError(i,e)
      print i, h_uncert.GetBinLowEdge(i), c, e

  return fit, h_uncert


#def PlotFakeFactor(f, h, name, output_folder, wp):
#  c1 = ROOT.TCanvas()
#  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
#  f.SetMinimum(0)
#  if f.GetMaximum() > 1.: f.SetMaximum(1.)
#  f.SetStats(0)
#  f.GetXaxis().SetTitle('p_{T}^{#tau_{h}} (GeV)')
#  f.GetYaxis().SetTitle('F_{F}')
#  f.SetTitle(name)
#  f.SetLineColor(ROOT.kBlack)
#  f.Draw()
#  h.SetLineColor(ROOT.kRed)
#  h.SetStats(0)
#  h.SetFillColor(ROOT.kBlue-10)
#  h.Draw("e3 same")
#  f.Draw("a sames")
#  l.AddEntry(f,"Observed","lep")
#  l.AddEntry(h,"Fit","l")
#  l.AddEntry(h,"Fit Uncertainty","f")
#  l.Draw()

#  c1.Print(output_folder+'/'+channel+'_'+wp+'_'+name+'_fit.pdf')
  #time.sleep(2)

def PlotFakeFactor(h, u, name, output_folder, wp, channel, year):

  #if 'jet_pt_high' not in name:
  #  bin_cap = 200.
  #  high_bin = h.GetBinContent(h.FindBin(250.))
  #  high_bin_err = h.GetBinError(h.FindBin(250.))
  #else:
  #  bin_cap = 600.
  #  high_bin = u.GetBinContent(u.FindBin(599.9))
  #  high_bin_err = u.GetBinError(u.FindBin(599.9))

  #for i in range(1,u.GetNbinsX()+1):
  #  if u.GetBinLowEdge(i) >= bin_cap:
  #    u.SetBinContent(i,high_bin)
  #    u.SetBinError(i,high_bin_err)

  if channel == "mt":
    channel_string = "#tau_{#mu}#tau_{h}"
  elif channel == "et":
    channel_string = "#tau_{e}#tau_{h}"
  elif channel == "tt":
    channel_string = "#tau_{h}#tau_{h}"

  if year == "2016":
    lumi_string = "35.9 fb^{-1}"
  elif year == "2017":
    lumi_string = "41.5 fb^{-1}"
  elif year == "2018":
    lumi_string = "59.7 fb^{-1}"

  if 'qcd_aiso' in name:
    bkg_string = 'QCD Anti-Isolated'
  elif 'qcd' in name:
    bkg_string = "QCD"
  elif 'wjets_mc' in name:
    bkg_string = 'W + Jets MC'
  elif 'wjets' in name:
    bkg_string = 'W + Jets'
  elif 'ttbar' in name:
    bkg_string = 't#bar{t}'

  if '0jet' in name:
    n_prejets_string = 'n_{pre b-jets} = 0'
  elif '1jet' in name:
    n_prejets_string = 'n_{pre b-jets} > 0'
  elif 'inclusive' in name:
    n_prejets_string = ''

  if 'jet_pt_low' in name:
    jet_pt_string = 'p_{T}^{jet}/p_{T}^{#tau_{h}} < 1.25'
  elif 'jet_pt_med' in name:
    jet_pt_string = '1.25 #leq p_{T}^{jet}/p_{T}^{#tau_{h}} < 1.5'
  elif 'jet_pt_high' in name:
    jet_pt_string = 'p_{T}^{jet}/p_{T}^{#tau_{h}} #geq 1.5'
  elif 'inclusive' in name:
    jet_pt_string = ''

  c = ROOT.TCanvas('c','c',700,700)
  u_fit = u.Clone()
  h.SetStats(0)
  u.SetStats(0)
  c.SetLeftMargin(0.15)
  u.Draw('e4')
  c.SetLogx()
  u.GetXaxis().SetMoreLogLabels()
  u.GetXaxis().SetNoExponent()
  h.Draw('a sames')
  h.SetLineColor(1)
  u.SetLineColor(46)
  u.SetFillColorAlpha(46, 0.35)
  u.GetXaxis().SetTitle('p_{T}^{#tau_{h}} (GeV)')
  u.GetXaxis().SetTitleOffset(1.2)
  u.GetYaxis().SetTitleOffset(1.6)
  u.GetYaxis().SetTitle('F_{F}')
  u.SetMaximum(1.7*max(u.GetMaximum(),h.GetMaximum()))
  u.SetMinimum(0)
  h.SetMarkerStyle(8)
  u_fit.Draw("hist L same")
  u_fit.SetLineColor(2)
  u_fit.SetFillColorAlpha(46, 0)
  c.Update()
  l = ROOT.TLegend(0.55,0.75,0.87,0.88)
  l.SetBorderSize(0)
  l.SetTextSize(0.04)
  l.AddEntry(h,'Observed','lep')
  l.AddEntry(u_fit,'Fit','l')
  l.AddEntry(u,'Fit Uncertainty','f')
  l.Draw()
  if args.cms_label:
    DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
  DrawTitle(c, '%(channel_string)s %(bkg_string)s' % vars(), 1, textSize=0.4)
  DrawTitle(c, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.4)
  if args.cms_label:
    DrawTitle(c, jet_pt_string, 4, textSize=0.4,x=0.20,y=0.7)
    DrawTitle(c, n_prejets_string, 4, textSize=0.4,x=0.20,y=0.63)
  else:
    DrawTitle(c, jet_pt_string, 4, textSize=0.4,x=0.20,y=0.83)
    DrawTitle(c, n_prejets_string, 4, textSize=0.4,x=0.20,y=0.76)

  c.Print('%(output_folder)s/ff_fit_%(name)s_%(channel)s_%(year)s.pdf' % vars())


#def PlotFakeFactorCorrection(f, h, name, output_folder, wp,x_title='E_{T}^{miss} (GeV)'):
#  c1 = ROOT.TCanvas()
#  f.SetMinimum(0)
#  f.SetStats(0)
#  f.GetXaxis().SetTitle(x_title)
#  f.GetYaxis().SetTitle('Closure Correction')
#  f.SetTitle(name)
#  f.SetLineColor(ROOT.kBlack)
#  f.Draw()
#  h.SetStats(0)
#  h.SetFillColor(ROOT.kBlue-10)
#  h.Draw("e2 same")
#  f.Draw("a sames")
#  c1.Print(output_folder+'/'+channel+'_'+wp+'_'+name+'_fit.pdf')

def PlotFakeFactorCorrection(h, u, name, output_folder, wp, channel, year, low_bounded_val, high_bounded_val, x_title='E_{T}^{miss} (GeV)',logx=False):

  if channel == "mt":
    channel_string = "#tau_{#mu}#tau_{h}"
  elif channel == "et":
    channel_string = "#tau_{e}#tau_{h}"
  elif channel == "tt":
    channel_string = "#tau_{h}#tau_{h}"

  if year == "2016":
    lumi_string = "35.9 fb^{-1}"
  elif year == "2017":
    lumi_string = "41.5 fb^{-1}"
  elif year == "2018":
    lumi_string = "59.7 fb^{-1}"

  if 'qcd_aiso' in name:
    bkg_string = 'QCD Anti-Isolated'
  elif 'qcd' in name:
    bkg_string = "QCD"
  elif 'wjets_mc' in name:
    bkg_string = 'W + Jets MC'
  elif 'wjets' in name:
    bkg_string = 'W + Jets'
  elif 'ttbar' in name:
    bkg_string = 't#bar{t}'

  high_bin = u.GetBinContent(u.FindBin(high_bounded_val))
  high_bin_err = u.GetBinError(u.FindBin(high_bounded_val))

  for i in range(1,u.GetNbinsX()+1):
    if u.GetBinLowEdge(i) >= high_bounded_val:
      u.SetBinContent(i,high_bin)
      u.SetBinError(i,high_bin_err)

  low_bin = u.GetBinContent(u.FindBin(low_bounded_val))
  low_bin_err = u.GetBinError(u.FindBin(low_bounded_val))

  for i in range(1,u.GetNbinsX()+1):
    if u.GetBinLowEdge(i) <= low_bounded_val:
      u.SetBinContent(i,low_bin)
      u.SetBinError(i,low_bin_err)

  c = ROOT.TCanvas('c','c',700,700)
  u_fit = u.Clone()
  u.SetStats(0)
  c.SetLeftMargin(0.15)
  u.Draw('e3')
  if logx:
    c.SetLogx()
    u.GetXaxis().SetMoreLogLabels()
    u.GetXaxis().SetNoExponent()
  h.Draw('same')
  h.GetFunction("f1").Delete()
  h.SetLineColor(1)
  u.SetLineColor(38)
  u.SetFillColorAlpha(38, 0.35)
  u.GetXaxis().SetTitle(x_title)
  u.GetXaxis().SetTitleOffset(1.2)
  u.GetYaxis().SetTitleOffset(1.6)
  u.GetYaxis().SetTitle('Closure Correction')
  u.SetMaximum(1.5)
  u.SetMinimum(0.6)
  h.SetMarkerStyle(8)
  u_fit.Draw("hist L same")
  u_fit.SetLineColor(4)
  u_fit.SetFillColorAlpha(46, 0)


  # Draw dashed line at y = 1
  line = ROOT.TLine(u.GetBinLowEdge(1),1,u.GetBinLowEdge(u.GetNbinsX()+1),1)
  line.SetLineWidth(1)
  line.SetLineStyle(2)
  line.SetLineColor(ROOT.kBlack)
  line.Draw()

  c.Update()

  l = ROOT.TLegend(0.62,0.75,0.87,0.88)
  l.SetBorderSize(0)
  l.SetTextSize(0.03)
  l.AddEntry(h,'Observed','lep')
  l.AddEntry(u_fit,'Fit','l')
  l.AddEntry(u,'Fit Uncertainty','f')
  l.Draw()

  if args.cms_label:
    DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)

  DrawTitle(c, '%(channel_string)s %(bkg_string)s' % vars(), 1, textSize=0.3)
  DrawTitle(c, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.3)

  c.Print('%(output_folder)s/ff_closure_%(name)s_%(channel)s_%(year)s.pdf' % vars())


def WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',aiso=False):
  # this function loops over all njets and jetpt bins and write the FFs as a function
  ff_eqn_tot = ''
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_1bin = '((pt_X<200)*(p0*TMath::Landau(pt_X,p1,p2)+p3)) + (p4*(pt_X>=200))'
  ff_eqn_2bin = '((pt_X<140)*(p0*TMath::Landau(pt_X,p1,p2)+p3)) + (p4*(pt_X>=140 && pt_X<200)) + (p5*(pt_X>=200))'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin_name,njetbin_cut in njets_bins.items():
    for jetptbin_name,jetptbin_cut in jetpt_bins.items():
      if not (jetptbin_name == 'inclusive' or njetbin_name == 'inclusive'):
        fout.cd()
        extra=''
        if aiso: extra = '_aiso2_ss'
        #f = fout.Get('%(jetptbin_name)s_%(njetbin_name)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
        #p = f.GetParameters()
        #if f.GetNpar()==1:
        #  ff_params = ff_pol0.replace('p0','%f' % p[0])
        #elif f.GetNpar()==2:
        #  ff_params = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
        #elif f.GetNpar() == 5:
        #  ff_params = ff_eqn_1bin.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
        #elif f.GetNpar() == 6:
        #  ff_params = ff_eqn_2bin.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4]).replace('p5','%f' % p[5])
        #else:
        #  ff_params = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])
 
        func = fout.Get('%(jetptbin_name)s_%(njetbin_name)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
        ff = func.GetExpFormula('p')
        ff_params = str(ff).replace('x','min(pt_2,599.)') 
        
        ff_eqn_tot += '(%(njetbin_cut)s*%(jetptbin_cut)s*(%(ff_params)s))+' % vars()
        
  ff_eqn_tot = ff_eqn_tot[:-1]
  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  return ff_eqn_tot

def WriteFakeFactorFunctionTTbar(fout,njets_bins,jetpt_bins,proc='ttbar_mc'):
  # this function loops over all njets and jetpt bins and write the FFs as a function
  ff_eqn_tot = ''
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = '((pt_X<200)*(p0*TMath::Landau(pt_X,p1,p2)+p3)) + (p4*(pt_X>=200))'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for jetptbin_name,jetptbin_cut in jetpt_bins.items():
    if not (jetptbin_name == 'inclusive'):
      fout.cd()
      #f = fout.Get('%(jetptbin_name)s_inclusive_pt_2_ff_%(proc)s_fit' % vars())
      #p = f.GetParameters()
      #if f.GetNpar()==1:
      #  ff_params = ff_pol0.replace('p0','%f' % p[0])
      #elif f.GetNpar()==2:
      #  ff_params = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      #elif f.GetNpar() > 4:
      #  ff_params = ff_eqn_alt.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      #else:
      #  ff_params = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

      func = fout.Get('%(jetptbin_name)s_inclusive_pt_2_ff_%(proc)s_fit' % vars())
      ff = func.GetExpFormula('p')
      ff_params = str(ff).replace('x','min(pt_2,599.)')

      ff_eqn_tot += '(%(jetptbin_cut)s*(%(ff_params)s))+' % vars()

  ff_eqn_tot = ff_eqn_tot[:-1]
  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  return ff_eqn_tot

def DrawCMSLogo(pad, cmsText, extraText, iPosX, relPosX, relPosY, relExtraDY, extraText2='', cmsTextSize=0.8):
    """Blah
    
    Args:
        pad (TYPE): Description
        cmsText (TYPE): Description
        extraText (TYPE): Description
        iPosX (TYPE): Description
        relPosX (TYPE): Description
        relPosY (TYPE): Description
        relExtraDY (TYPE): Description
        extraText2 (str): Description
        cmsTextSize (float): Description
    
    Returns:
        TYPE: Description
    """
    pad.cd()
    cmsTextFont = 62  # default is helvetic-bold

    writeExtraText = len(extraText) > 0
    writeExtraText2 = len(extraText2) > 0
    extraTextFont = 52

    # text sizes and text offsets with respect to the top frame
    # in unit of the top margin size
    lumiTextOffset = 0.2
    # cmsTextSize = 0.8
    # float cmsTextOffset    = 0.1;  // only used in outOfFrame version

    # ratio of 'CMS' and extra text size
    extraOverCmsTextSize = 0.76

    outOfFrame = False
    if iPosX / 10 == 0:
        outOfFrame = True

    alignY_ = 3
    alignX_ = 2
    if (iPosX / 10 == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignY_ = 1
    if (iPosX / 10 == 1):
        alignX_ = 1
    if (iPosX / 10 == 2):
        alignX_ = 2
    if (iPosX / 10 == 3):
        alignX_ = 3
    # if (iPosX == 0): relPosX = 0.14
    align_ = 10 * alignX_ + alignY_

    l = pad.GetLeftMargin()
    t = pad.GetTopMargin()
    r = pad.GetRightMargin()
    b = pad.GetBottomMargin()

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)

    extraTextSize = extraOverCmsTextSize * cmsTextSize
    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if (pad_ratio < 1.):
        pad_ratio = 1.
    if outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextAlign(11)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText)

    posX_ = 0
    if iPosX % 10 <= 1:
        posX_ = l + relPosX * (1 - l - r)
    elif (iPosX % 10 == 2):
        posX_ = l + 0.5 * (1 - l - r)
    elif (iPosX % 10 == 3):
        posX_ = 1 - r - relPosX * (1 - l - r)

    posY_ = 1 - t - relPosY * (1 - t - b)
    if not outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, cmsText)
        if writeExtraText:
            latex.SetTextFont(extraTextFont)
            latex.SetTextAlign(align_)
            latex.SetTextSize(extraTextSize * t * pad_ratio)
            latex.DrawLatex(
                posX_, posY_ - relExtraDY * cmsTextSize * t, extraText)
            if writeExtraText2:
                latex.DrawLatex(
                    posX_, posY_ - 1.8 * relExtraDY * cmsTextSize * t, extraText2)
    elif writeExtraText:
        if iPosX == 0:
            posX_ = l + relPosX * (1 - l - r)
            posY_ = 1 - t + lumiTextOffset * t
        latex.SetTextFont(extraTextFont)
        latex.SetTextSize(extraTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, extraText)

def DrawTitle(pad, text, align, textOffset=0.2,textSize=0.6,x=0,y=0):
    pad_backup = ROOT.gPad
    pad.cd()
    t = pad.GetTopMargin()
    l = pad.GetLeftMargin()
    r = pad.GetRightMargin()

    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if pad_ratio < 1.:
        pad_ratio = 1.

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)
    latex.SetTextFont(42)
    latex.SetTextSize(textSize * t * pad_ratio)

    y_off = 1 - t + textOffset * t
    if align == 1:
        latex.SetTextAlign(11)
        latex.DrawLatex(l, y_off, text)
    elif align == 2:
        latex.SetTextAlign(21)
        latex.DrawLatex(l + (1 - l - r) * 0.5, y_off, text)
    elif align == 3:
        latex.SetTextAlign(31)
        latex.DrawLatex(1 - r, y_off, text)
    else:
        latex.DrawLatex(x,y,text)
    pad_backup.cd()

def draw_fraction(cat_name,new_bins,output_folder,channel,year,qcd_frac,wjets_frac,ttbar_frac,os_ss):

  if channel == "mt":
    channel_string = "#tau_{#mu}#tau_{h}"
  elif channel == "et":
    channel_string = "#tau_{e}#tau_{h}"
  elif channel == "tt":
    channel_string = "#tau_{h}#tau_{h}"

  if year == "2016":
    lumi_string = "35.9 fb^{-1}"
  elif year == "2017":
    lumi_string = "41.5 fb^{-1}"
  elif year == "2018":
    lumi_string = "59.7 fb^{-1}"

  c = ROOT.TCanvas('c','c',700,700)
  c.SetLogx()
  dummy_hist = ROOT.TH1D("dummy_hist","",len(new_bins)-1, new_bins)
  dummy_hist.SetStats(0)
  dummy_hist.GetXaxis().SetMoreLogLabels()
  dummy_hist.GetXaxis().SetNoExponent()
  dummy_hist.GetXaxis().SetTitle('m_{T}^{tot} (GeV)')
  dummy_hist.GetXaxis().SetTitleOffset(1.2)
  dummy_hist.GetYaxis().SetTitle('Fraction')
  dummy_hist.SetMaximum(1.3)
  dummy_hist.Draw()

  hs = ROOT.THStack("hs","")
  qcd_frac.SetFillColor(ROOT.TColor.GetColor(250,202,255))
  qcd_frac.SetLineColor(1)
  wjets_frac.SetFillColor(ROOT.TColor.GetColor(222,90,106))
  wjets_frac.SetLineColor(1)
  ttbar_frac.SetFillColor(ROOT.TColor.GetColor(155,152,204))
  ttbar_frac.SetLineColor(1)
  hs.Add(qcd_frac,"hist")
  hs.Add(wjets_frac,"hist")
  hs.Add(ttbar_frac,"hist")
  hs.Draw("same")

  c.RedrawAxis()
  l = ROOT.TLegend(0.65,0.75,0.85,0.88)
  l.SetBorderSize(0)
  l.SetTextSize(0.03)
  l.AddEntry(qcd_frac,'QCD','f')
  l.AddEntry(wjets_frac,'W + Jets','f')
  l.AddEntry(ttbar_frac,'t#bar{t}','f')
  l.Draw()
  if args.cms_label:
    DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
  DrawTitle(c, '%(channel_string)s' % vars(), 1, textSize=0.4)
  DrawTitle(c, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.4)

  c.Print('%(output_folder)s/ff_fraction_%(cat_name)s_%(channel)s_%(year)s_%(os_ss)s_rebinning.pdf' % vars())


# variable and binningn used for fitting
var1='pt_2[30,35,40,45,50,55,60,70,80,100,120,140,200,600]'

# set up histograms to draw
ff_list = {}
for njetbin in njets_bins:
  for jetptbin in jetpt_bins:
    name= jetptbin+'_'+njetbin
    cuts="("+njets_bins[njetbin]+'&&'+jetpt_bins[jetptbin]+")"
    # add isolated
    ff_list[name+'_pt_2'] = (var1, baseline_iso_pass+"*"+cuts, baseline_iso_fail+"*"+cuts)
    # add anti-isolated
    ff_list[name+'_aiso2_ss_pt_2'] = (var1, baseline_aiso_pass+"*"+cuts, baseline_aiso_fail+"*"+cuts)

# calculate fake factors
to_write = []
for ff in ff_list:
  print "Calculating fake factors for: {}".format(ff)
  wjets_mc_ff=None
  ttbar_mc_ff=None
  if draw:
    if "inclusive" not in ff:
      (qcd_iso, wjets_iso, wjets_mc_iso, ttbar_mc_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff),doIso=False)
      (qcd_aiso, wjets_aiso, wjets_mc_aiso, ttbar_mc_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff),doIso=False)

      qcd_ff = CalculateFakeFactors(qcd_iso, qcd_aiso)
      to_write.append(qcd_ff)

    if not 'aiso2' in ff:
      if "inclusive" not in ff:
        wjets_ff = CalculateFakeFactors(wjets_iso, wjets_aiso)
        to_write.append(wjets_ff)

        wjets_mc_ff = CalculateFakeFactors(wjets_mc_iso, wjets_mc_aiso)
        to_write.append(wjets_mc_ff)

    if "inclusive" in ff:
      (_, _, _, ttbar_mc_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=False,doW=False,doMC=False,doTT=True,doIso=False)
      (_, _, _, ttbar_mc_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=False,doW=False,doMC=False,doTT=True,doIso=False)

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


  # fit fake factors
  if "inclusive" not in ff:
    (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff)
    to_write.append(qcd_fit)
    to_write.append(qcd_uncert)
    PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp, channel, year)

  if not 'aiso2' in ff:
    if "inclusive" not in ff:
      (wjets_fit, wjets_uncert, wjets_ff) = FitFakeFactors(wjets_ff)
      to_write.append(wjets_fit)
      to_write.append(wjets_uncert)
      PlotFakeFactor(wjets_ff, wjets_uncert, wjets_ff.GetName(), output_folder, wp, channel, year)
      (wjets_mc_fit, wjets_mc_uncert, wjets_mc_ff) = FitFakeFactors(wjets_mc_ff)
      to_write.append(wjets_mc_fit)
      to_write.append(wjets_mc_uncert)
      PlotFakeFactor(wjets_mc_ff, wjets_mc_uncert, wjets_mc_ff.GetName(), output_folder, wp, channel, year)
    if "inclusive" in ff:
      (ttbar_mc_fit, ttbar_mc_uncert, ttbar_mc_ff) = FitFakeFactors(ttbar_mc_ff)
      to_write.append(ttbar_mc_fit)
      to_write.append(ttbar_mc_uncert)
      PlotFakeFactor(ttbar_mc_ff, ttbar_mc_uncert, ttbar_mc_ff.GetName(), output_folder, wp, channel, year)

# make fractions
 
#nbjets_cats = {'nbjets0':'n_deepbjets==0','nbjets1':'n_deepbjets>0'}

#for nbjets_name,nbjets_cut in nbjets_cats.items():
#  var= 'mt_1[0,10,20,30,40,50,60,70,80,90,100,120]'
#  cuts = '(%(baseline_iso_fail)s)*(%(nbjets_cut)s)' % vars()
#  name = '%(channel)s_fracs_%(nbjets_name)s' % vars()
#  qcd_os, wjets_os, ttbar_os = DrawHistsForFractions(var, '%(cuts)s*(os==1)' % vars(), name+'_os', input_folder, file_ext)
#  qcd_ss, wjets_ss, ttbar_ss = DrawHistsForFractions(var, '%(cuts)s*(os==0)' % vars(), name+'_ss', input_folder, file_ext)
#  total_os = qcd_os.Clone(); total_os.Add(wjets_os); total_os.Add(ttbar_os) 
#  total_ss = qcd_ss.Clone(); total_ss.Add(wjets_ss); total_ss.Add(ttbar_ss)
#  qcd_os.Divide(total_os)
#  wjets_os.Divide(total_os)
#  ttbar_os.Divide(total_os)
#  qcd_ss.Divide(total_ss)
#  wjets_ss.Divide(total_ss)
#  ttbar_ss.Divide(total_ss)
#  to_write.append(qcd_os); to_write.append(wjets_os); to_write.append(ttbar_os)
#  to_write.append(qcd_ss); to_write.append(wjets_ss); to_write.append(ttbar_ss)


ana_cats = {
  'tightmt_nbjets0':'n_deepbjets==0 && mt_1<40',
  'tightmt_nbjets1':'n_deepbjets>0 && mt_1<40',
  'loosemt_nbjets0':'n_deepbjets==0 && mt_1>=40 && mt_1<70',
  'loosemt_nbjets1':'n_deepbjets>0 && mt_1>=40 && mt_1<70',
  'control_nbjets0':'n_deepbjets==0 && mt_1>=70',
  'control_nbjets1':'n_deepbjets>0 && mt_1>=70',
  }

for cat_name,cat_cut in ana_cats.items():
  var= 'mt_tot[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]'
  cuts = '(%(baseline_iso_fail)s)*(%(cat_cut)s)' % vars()
  name = '%(channel)s_fracs_%(cat_name)s' % vars()
  qcd_os, wjets_os, ttbar_os = DrawHistsForFractions(var, '%(cuts)s*(os==1)' % vars(), name+'_os', input_folder, file_ext)
  qcd_ss, wjets_ss, ttbar_ss = DrawHistsForFractions(var, '%(cuts)s*(os==0)' % vars(), name+'_ss', input_folder, file_ext)
  qcd_os, wjets_os, ttbar_os, new_bins_os = fraction_rebinning(var,cat_name,qcd_os,wjets_os,ttbar_os)
  qcd_ss, wjets_ss, ttbar_ss, new_bins_ss = fraction_rebinning(var,cat_name,qcd_ss,wjets_ss,ttbar_ss)
  total_os = qcd_os.Clone(); total_os.Add(wjets_os); total_os.Add(ttbar_os) 
  total_ss = qcd_ss.Clone(); total_ss.Add(wjets_ss); total_ss.Add(ttbar_ss)
  qcd_os.Divide(total_os)
  qcd_os.SetName(name+'_os_qcd')
  wjets_os.Divide(total_os)
  wjets_os.SetName(name+'_os_wjets')
  ttbar_os.Divide(total_os)
  ttbar_os.SetName(name+'_os_ttbar')
  qcd_ss.Divide(total_ss)
  qcd_ss.SetName(name+'_ss_qcd')
  wjets_ss.Divide(total_ss)
  wjets_ss.SetName(name+'_ss_wjets')
  ttbar_ss.Divide(total_ss)
  ttbar_ss.SetName(name+'_ss_ttbar')
  to_write.append(qcd_os); to_write.append(wjets_os); to_write.append(ttbar_os)
  to_write.append(qcd_ss); to_write.append(wjets_ss); to_write.append(ttbar_ss)
  draw_fraction(cat_name,new_bins_os,output_folder,channel,year,qcd_os,wjets_os,ttbar_os,'os')
  draw_fraction(cat_name,new_bins_ss,output_folder,channel,year,qcd_ss,wjets_ss,ttbar_ss,'ss')



# write everything to the output file
fout = ROOT.TFile(out_file, 'RECREATE')
for i in to_write: i.Write()

# Print fake factor strings before closures
print "---------------------------------------------------------------------------------------------------"
print "Fake Factor Strings Before Closure Corrections"
print "---------------------------------------------------------------------------------------------------"
ff_qcd = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',aiso=False)
print "QCD Fake Factors:"
print "ff_string='(" + ff_qcd + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_qcd_aiso = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',aiso=True)
print "QCD AISO Fake Factors:"
print "ff_string='(" + ff_qcd_aiso + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_wjets = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='wjets',aiso=False)
print "W + Jets Fake Factors:"
print "ff_string='(" + ff_wjets + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_wjets_mc = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='wjets_mc',aiso=False)
print "W + Jets MC Fake Factors:"
print "ff_string='(" + ff_wjets_mc + ")'"
print "---------------------------------------------------------------------------------------------------"
ff_ttbar_mc = WriteFakeFactorFunctionTTbar(fout,njets_bins,jetpt_bins,proc='ttbar_mc')
print "ttbar Fake Factors:"
print "ff_string='(" + ff_ttbar_mc + ")'"
print "---------------------------------------------------------------------------------------------------"


############ Do closures ##############

###### W + Jets Closure #######

#### data ####

var='newmet*cos(newmet_dphi_2)/pt_2[-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000]'

w_corr_string = "*("
for njets_name, njets_cut in njets_bins.items():
  if "inclusive" not in njets_name:
    corr_cut = njets_cut
    corr_name = 'met_' + njets_name

    (_,wjets_data,_,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=True,doQCD=False,doTT=False)
    (_,wjets_pred,_,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff_wjets+")",doMC=False,doW=True,doQCD=False,doTT=False)
    fout.cd()
    wjets_data.Divide(wjets_pred)

    wjets_data_fit, wjets_data_uncert =  FitCorrection(wjets_data,func='[0]*TMath::Erf((x-[1])/[2])+[3]')

    wjets_data.Write()
    wjets_data_fit.Write()
    wjets_data_uncert.Write()
    PlotFakeFactorCorrection(wjets_data, wjets_data_uncert, wjets_data.GetName(), output_folder, wp, channel, year, -2.2, 1, x_title='c_{W}',logx=False)

    w_corr_string += '((%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_closure_wjets_fit').GetExpFormula('p')).replace('x','max(min(newmet*cos(newmet_dphi_2)/pt_2,1.0),-2.2)'))

w_corr_string = w_corr_string[:-1] + ')'
init_w_corr_string = w_corr_string[:]

# l_pt correction
var = 'pt_1[20,%(crosstrg_pt)s,35,40,45,50,60,70,80,100,120,140,160,180,200]' % vars()
w_corr_string += "*("
for add_name, corr_cut in njets_bins.items():
  if "inclusive" not in add_name:
    corr_name = 'l_pt_' + add_name
    (_,wjets_data,_,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=True,doQCD=False,doTT=False)
    (_,wjets_pred,_,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_wjets+")"+init_w_corr_string+")",doMC=False,doW=True,doQCD=False,doTT=False)
    fout.cd()
    wjets_data.Divide(wjets_pred)

    wjets_data_fit, wjets_data_uncert =  FitCorrection(wjets_data,func='pol1',fit_range=[crosstrg_pt,300.])

    wjets_data.Write()
    wjets_data_fit.Write()
    wjets_data_uncert.Write()
    if '0jet' in add_name: top_bin = 100
    else: top_bin = 200
    PlotFakeFactorCorrection(wjets_data, wjets_data_uncert, wjets_data.GetName(), output_folder, wp, channel, year, 0, top_bin, x_title='p_{T}^{#mu} (GeV)',logx=True)

    low_pt_corr_cut = corr_cut+'&&pt_1<%(crosstrg_pt)s' % vars()
    high_pt_corr_cut = corr_cut+'&&pt_1>=%(crosstrg_pt)s' % vars()
    low_pt_corr = wjets_data.GetBinContent(1)
    if '0jet' in add_name: w_corr_string += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_wjets_fit').GetExpFormula('p')).replace('x','min(pt_1,100)'),low_pt_corr_cut, low_pt_corr)
    else: w_corr_string += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_wjets_fit').GetExpFormula('p')).replace('x','min(pt_1,200)'),low_pt_corr_cut, low_pt_corr)

w_corr_string = w_corr_string[:-1] + ')'

#### mc ####

w_mc_corr_string = "*("

var='newmet*cos(newmet_dphi_2)/pt_2[-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000]'
for njets_name, njets_cut in njets_bins.items():
  if "inclusive" not in njets_name:
    corr_cut = njets_cut
    corr_name = 'met_' + njets_name

    (_,_,wjets_mc_data,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=True,doW=False,doQCD=False,doTT=False)
    (_,_,wjets_mc_pred,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff_wjets_mc+")",doMC=True,doW=False,doQCD=False,doTT=False)
    fout.cd()
    wjets_mc_data.Divide(wjets_mc_pred)

    wjets_mc_data_fit, wjets_mc_data_uncert =  FitCorrection(wjets_mc_data,func='[0]*TMath::Erf((x-[1])/[2])+[3]')
    wjets_mc_data.Write()
    wjets_mc_data_fit.Write()
    wjets_mc_data_uncert.Write()
    PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_data_uncert, wjets_mc_data.GetName(), output_folder, wp, channel, year, -2.2, 1, x_title='c_{W}',logx=False)

    w_mc_corr_string += '((%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_closure_wjets_mc_fit').GetExpFormula('p')).replace('x','max(min(newmet*cos(newmet_dphi_2)/pt_2,1.0),-2.2)'))
w_mc_corr_string = w_mc_corr_string[:-1] + ')'
init_w_mc_corr_string = w_mc_corr_string[:]

# l_pt correction
var = 'pt_1[20,%(crosstrg_pt)s,35,40,45,50,60,70,80,100,120,140,160,180,200]' % vars()
w_mc_corr_string += "*("
for add_name, corr_cut in njets_bins.items():
  if "inclusive" not in add_name:
    corr_name = 'l_pt_' + add_name
    (_,_,wjets_mc_data,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=True,doW=False,doQCD=False,doTT=False)
    (_,_,wjets_mc_pred,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_wjets_mc+")"+init_w_mc_corr_string+")",doMC=True,doW=False,doQCD=False,doTT=False)
    fout.cd()
    wjets_mc_data.Divide(wjets_mc_pred)

    wjets_mc_data_fit, wjets_mc_data_uncert =  FitCorrection(wjets_mc_data,func='pol1',fit_range=[crosstrg_pt,200.])

    wjets_mc_data.Write()
    wjets_mc_data_fit.Write()
    wjets_mc_data_uncert.Write()
    if '0jet' in add_name: top_bin = 100
    else: top_bin = 200
    PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_data_uncert, wjets_mc_data.GetName(), output_folder, wp, channel, year, 0, top_bin, x_title='p_{T}^{#mu} (GeV)',logx=True)

    low_pt_corr_cut = corr_cut+'&&pt_1<%(crosstrg_pt)s' % vars()
    high_pt_corr_cut = corr_cut+'&&pt_1>=%(crosstrg_pt)s' % vars()
    low_pt_corr = wjets_mc_data.GetBinContent(1)
    if '0jet' in add_name: w_mc_corr_string += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_wjets_mc_fit').GetExpFormula('p')).replace('x','min(pt_1,100)'),low_pt_corr_cut, low_pt_corr)
    else: w_mc_corr_string += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_wjets_mc_fit').GetExpFormula('p')).replace('x','min(pt_1,200)'),low_pt_corr_cut, low_pt_corr)

w_mc_corr_string = w_mc_corr_string[:-1] + ')'

# High-mT -> Low-mT MET corrections
w_mc_corr_ff = w_mc_corr_string[:]
w_mc_corr_string += "*("
w_corr_string += "*("
categories = {
              'nbjets0_tightmt':"(n_deepbjets==0 && mt_1<40)",
              'nbjets0_loosemt':"(n_deepbjets==0 && mt_1>40 && mt_1<70)",
              'nbjets1_tightmt':"(n_deepbjets>0 && mt_1<40)",
              'nbjets1_loosemt':"(n_deepbjets>0 && mt_1>40 && mt_1<70)"
}
var = 'pt_1[%(crosstrg_pt)s,35,40,45,50,60,70,80,100,120,140,160,180,200]' % vars()
for add_name, corr_cut in categories.items():
  if 'nbjets1' in add_name: var = 'pt_1[20,%(crosstrg_pt)s,200]' % vars()
  else: var = 'pt_1[20,%(crosstrg_pt)s,40,60,80,100,120,160,200]' % vars()
  corr_name = 'pt_1_' + add_name + '_dr_to_ar'
  (_,_,wjets_mc_data,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=True,doW=False,doQCD=False,doTT=False,lowMT=True)
  (_,_,wjets_mc_pred,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_wjets_mc+")"+w_mc_corr_ff+")",doMC=True,doW=False,doQCD=False,doTT=False,lowMT=True)
  fout.cd()
  wjets_mc_data.Divide(wjets_mc_pred)

  if wjets_mc_data.GetBinContent(1) <=0: 
    wjets_mc_data.SetBinContent(1,1.)

  if 'nbjets1' in add_name: wjets_mc_data_fit, wjets_mc_data_uncert =  FitCorrection(wjets_mc_data,func='pol0',fit_range=[crosstrg_pt,300.])
  else: wjets_mc_data_fit, wjets_mc_data_uncert =  FitCorrection(wjets_mc_data,func='pol1',fit_range=[crosstrg_pt,300.])

  wjets_mc_data.Write()
  wjets_mc_data_fit.Write()
  wjets_mc_data_uncert.Write()
  PlotFakeFactorCorrection(wjets_mc_data, wjets_mc_data_uncert, wjets_mc_data.GetName(), output_folder, wp, channel, year, 0, 200, x_title='p_{T}^{#mu} (GeV)',logx=True)

  low_pt_corr_cut = corr_cut+'&&pt_1<%(crosstrg_pt)s' % vars()
  high_pt_corr_cut = corr_cut+'&&pt_1>=%(crosstrg_pt)s' % vars()
  low_pt_corr = wjets_mc_data.GetBinContent(1)
  w_mc_corr_string += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_wjets_mc_fit').GetExpFormula('p')).replace('x','min(pt_1,200)'),low_pt_corr_cut, low_pt_corr)
  w_corr_string    += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_wjets_mc_fit').GetExpFormula('p')).replace('x','min(pt_1,200)'),low_pt_corr_cut, low_pt_corr)

w_mc_corr_string += '(mt_1>70))'
w_corr_string += '(mt_1>70))'

###### QCD Closure #######
#### Isolated ####

# MET correction
var='met*cos(met_dphi_2)/pt_2[-3.50,-3.25,-3.00,-2.75,-2.50,-2.25,-2.00,-1.75,-1.50,-1.25,-1.00,-0.75,-0.50,-0.25,0.00,0.25,0.50,0.75,1.00,1.25,1.50]'
qcd_corr_string = "*("
for add_name, corr_cut in njets_bins.items():
  if "inclusive" not in add_name:
    corr_name = 'met_' + add_name
    (qcd_data,_,_,_) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False,doIso=False)
    (qcd_pred,_,_,_) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff_qcd+")",doMC=False,doW=False,doQCD=True,doTT=False,doIso=False)
    fout.cd()
    qcd_data.Divide(qcd_pred)

    qcd_data_fit, qcd_data_uncert =  FitCorrection(qcd_data,func='[0]*TMath::Erf((x-[1])/[2])+[3]')

    qcd_data.Write()
    qcd_data_fit.Write()
    qcd_data_uncert.Write()
    PlotFakeFactorCorrection(qcd_data, qcd_data_uncert, qcd_data.GetName(), output_folder, wp, channel, year, -1.8, 1.5, x_title='c_{QCD}',logx=False)

    qcd_corr_string += '((%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_closure_qcd_fit').GetExpFormula('p')).replace('x','max(min(met*cos(met_dphi_2)/pt_2,1.5),-1.8)'))
qcd_corr_string = qcd_corr_string[:-1] + ')'

# cross trigger correction
init_qcd_corr_string = qcd_corr_string[:]
var = "pt_1[20,%(crosstrg_pt)s,200]" % vars()

qcd_corr_string += "*("

corr_name = 'l_pt'
(qcd_data,_,_,_) = DrawHists(var, '('+baseline_iso_pass+')', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False,doIso=False)
(qcd_pred,_,_,_) = DrawHists(var, '('+baseline_iso_fail+')', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_qcd+")"+init_qcd_corr_string+")",doMC=False,doW=False,doQCD=True,doTT=False,doIso=False)
fout.cd()
qcd_data.Divide(qcd_pred)

qcd_data.Write()
low = qcd_data.GetBinContent(1)
high = qcd_data.GetBinContent(2)

qcd_corr_string += '((%(low).5f)*(pt_1<%(crosstrg_pt)s) + (%(high).5f)*(pt_1>=%(crosstrg_pt)s))+' % vars()
qcd_corr_string = qcd_corr_string[:-1] + ')'

# iso>0.05 cut correction
init_qcd_corr_string = qcd_corr_string[:]
var = "iso_1[0.0,0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1,0.105,0.11,0.115,0.12,0.125,0.13,0.135,0.14,0.145,0.15]"

qcd_corr_string += "*("

print '!!!!!!!!'

corr_name = 'iso_' + add_name
(qcd_data,_,_,_) = DrawHists(var, '('+baseline_iso_pass+')', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False)
(qcd_pred,_,_,_) = DrawHists(var, '('+baseline_iso_fail+')', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_qcd+")"+init_qcd_corr_string+")",doMC=False,doW=False,doQCD=True,doTT=False)
fout.cd()

print "(("+ff_qcd+")"+init_qcd_corr_string+")"

qcd_data.Divide(qcd_pred)

qcd_data_alt = qcd_data.Clone()
qcd_data.SetName(str(qcd_data.GetName())+'_alt')

qcd_data_fit_alt, qcd_data_uncert_alt =  FitCorrection(qcd_data_alt,func='pol1')
qcd_data_fit, qcd_data_uncert =  FitCorrection(qcd_data,func='pol1',fit_range=[0.05,0.15])


qcd_data.Write()
qcd_data_fit.Write()
qcd_data_uncert.Write()
qcd_data_fit_alt.Write()
qcd_data_uncert_alt.Write()
PlotFakeFactorCorrection(qcd_data, qcd_data_uncert, qcd_data.GetName(), output_folder, wp, channel, year, 0, 0.5, x_title='Iso',logx=False)
PlotFakeFactorCorrection(qcd_data_alt, qcd_data_uncert_alt, qcd_data_alt.GetName(), output_folder, wp, channel, year, 0, 0.5, x_title='Iso',logx=False)

qcd_corr_string += '(%s)+' % (str(fout.Get(corr_name+'_closure_qcd_fit').GetExpFormula('p')).replace('x','min(iso_1,0.5)'))
qcd_corr_string = qcd_corr_string[:-1] + ')'

#### Anti-Isolated ####

# MET correction
var='met*cos(met_dphi_2)/pt_2[-3.50,-3.25,-3.00,-2.75,-2.50,-2.25,-2.00,-1.75,-1.50,-1.25,-1.00,-0.75,-0.50,-0.25,0.00,0.25,0.50,0.75,1.00,1.25,1.50]'
qcd_aiso_corr_string = "*("
for add_name, corr_cut in njets_bins.items():
  if "inclusive" not in add_name:
    corr_name = 'met_' + add_name
    (qcd_aiso_data,_,_,_) = DrawHists(var, '(('+baseline_aiso_pass+')*('+corr_cut+'))', corr_name+'_aiso_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False)
    (qcd_aiso_pred,_,_,_) = DrawHists(var, '(('+baseline_aiso_fail+')*('+corr_cut+'))', corr_name+'_aiso_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff_qcd_aiso+")",doMC=False,doW=False,doQCD=True,doTT=False)
    fout.cd()
    qcd_aiso_data.Divide(qcd_aiso_pred)
    qcd_aiso_data_fit, qcd_aiso_data_uncert =  FitCorrection(qcd_aiso_data,func='[0]*TMath::Erf((x-[1])/[2])+[3]')

    qcd_aiso_data.Write()
    qcd_aiso_data_fit.Write()
    qcd_aiso_data_uncert.Write()
    PlotFakeFactorCorrection(qcd_aiso_data, qcd_aiso_data_uncert, qcd_aiso_data.GetName(), output_folder, wp,channel, year, -1.8, 1, x_title='c_{QCD}',logx=False)

    qcd_aiso_corr_string += '((%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_aiso_closure_qcd_fit').GetExpFormula('p')).replace('x','max(min(met*cos(met_dphi_2)/pt_2,1.5),-1.8)'))
qcd_aiso_corr_string = qcd_aiso_corr_string[:-1] + ')'

# cross trigger correction
init_qcd_aiso_corr_string = qcd_aiso_corr_string[:]
var = "pt_1[20,%(crosstrg_pt)s,200]" % vars()

qcd_aiso_corr_string += "*("

corr_name = 'l_pt'
(qcd_aiso_data,_,_,_) = DrawHists(var, '('+baseline_aiso_pass+')', corr_name+'_aiso_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False)
(qcd_aiso_pred,_,_,_) = DrawHists(var, '('+baseline_aiso_fail+')', corr_name+'_aiso_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_qcd_aiso+")"+init_qcd_aiso_corr_string+")",doMC=False,doW=False,doQCD=True,doTT=False)
fout.cd()
qcd_aiso_data.Divide(qcd_aiso_pred)

qcd_aiso_data.Write()
low = qcd_aiso_data.GetBinContent(1)
high = qcd_aiso_data.GetBinContent(2)

qcd_aiso_corr_string += '((%(low).5f)*(pt_1<%(crosstrg_pt)s) + (%(high).5f)*(pt_1>=%(crosstrg_pt)s))+' % vars()
qcd_aiso_corr_string = qcd_aiso_corr_string[:-1] + ')'

# SS to OS correction

var = 'pt_1[20,%(crosstrg_pt)s,35,40,50,60,80,100,120,160]' % vars()

qcd_aiso_corr_ff = qcd_aiso_corr_string[:]
qcd_aiso_corr_string += "*("
qcd_corr_string += "*("

print qcd_aiso_corr_ff

categories = {
              'nbjets0':"(n_deepbjets==0 && mt_1<70)",
              'nbjets1':"(n_deepbjets>0 && mt_1<70)",
}
for add_name, corr_cut in categories.items():
  #if 'nbjets0' in add_name and channel == 'et': 
  #  if year == '2016': var = 'pt_1[25,30,35,40,80]' % vars()
  #  if year == '2017': var = 'pt_1[20,%(crosstrg_pt)s,35,40,80]' % vars()
  #  if year == '2018': var = 'pt_1[20,%(crosstrg_pt)s,35,40,80]' % vars()
  if 'nbjets0' in add_name and channel == 'mt': 
    if year == '2016': var = 'pt_1[20,%(crosstrg_pt)s,25,27,30,35,40,50,60,80,100,120,160]' % vars()
    else: var = 'pt_1[20,%(crosstrg_pt)s,35,40,50,60,80,100,120,160]' % vars()
  else: var = 'pt_1[20,%(crosstrg_pt)s,160]' % vars()
  corr_name = 'pt_1_' + add_name + '_dr_to_ar'
  (qcd_aiso_data,_,_,_) = DrawHists(var, '(('+baseline_aiso_pass+')*('+corr_cut+'))', corr_name+'_aiso_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=True,doTT=False,doOS=True,qcdMT='70')
  (qcd_aiso_pred,_,_,_) = DrawHists(var, '(('+baseline_aiso_fail+')*('+corr_cut+'))', corr_name+'_aiso_closure_pred' % vars(),input_folder,file_ext,add_wt="(("+ff_qcd_aiso+")"+qcd_aiso_corr_ff+")",doMC=False,doW=False,doQCD=True,doTT=False,doOS=True,qcdMT='70')
  fout.cd()
  qcd_aiso_data.Divide(qcd_aiso_pred)

  if qcd_aiso_data.GetBinContent(1) <=0:
    qcd_aiso_data.SetBinContent(1,1.)

  if 'nbjets0' in add_name and channel == 'mt': 
    #qcd_aiso_data_fit, qcd_aiso_data_uncert =  FitCorrection(qcd_aiso_data,func='pol3')#,fit_range=[crosstrg_pt,300.])
    qcd_aiso_data_fit, qcd_aiso_data_uncert =  FitCorrection(qcd_aiso_data,func='[0]*TMath::Erf((-x-[1])/[2])+[3]')#,fit_range=[crosstrg_pt,300.])
  else: 
    qcd_aiso_data_fit, qcd_aiso_data_uncert =  FitCorrection(qcd_aiso_data,func='pol0')#,fit_range=[crosstrg_pt,300.])

  qcd_aiso_data.Write()
  qcd_aiso_data_fit.Write()
  qcd_aiso_data_uncert.Write()
  PlotFakeFactorCorrection(qcd_aiso_data, qcd_aiso_data_uncert, qcd_aiso_data.GetName(), output_folder, wp, channel, year, 0, 100, x_title='p_{T}^{#mu} (GeV)',logx=True)

  qcd_aiso_corr_string += '((os==1)*(%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_aiso_closure_qcd_fit').GetExpFormula('p')).replace('x','min(pt_1,100)'))
  qcd_corr_string += '((os==1)*(%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_aiso_closure_qcd_fit').GetExpFormula('p')).replace('x','min(pt_1,100)'))

qcd_aiso_corr_string += '(os==0))'
qcd_corr_string += '(os==0))'

# ttbar corrections

# MET correction
var='newmet*cos(newmet_dphi_2)/pt_2[-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000]'
ttbar_mc_corr_string = "*("
#mt_1_regions = {'tightmT':'mt_1<50' % vars(),'loosemT':'mt_1>=50' % vars()}
corr_name = 'met'
corr_cut = '1'
(_,_,_,ttbar_mc_data) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=False,doTT=True)
(_,_,_,ttbar_mc_pred) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff_ttbar_mc+")",doMC=False,doW=False,doQCD=False,doTT=True)
fout.cd()
ttbar_mc_data.Divide(ttbar_mc_pred)
ttbar_mc_data_fit, ttbar_mc_data_uncert =  FitCorrection(ttbar_mc_data,func='pol3')
ttbar_mc_data.Write()
ttbar_mc_data_fit.Write()
ttbar_mc_data_uncert.Write()
PlotFakeFactorCorrection(ttbar_mc_data, ttbar_mc_data_uncert, ttbar_mc_data.GetName(), output_folder, wp, channel, year, -2.2, 1, x_title='c_{W}',logx=False)

ttbar_mc_corr_string += '((%s)*(%s))+' % (corr_cut,str(fout.Get(corr_name+'_closure_ttbar_mc_fit').GetExpFormula('p')).replace('x','max(min(newmet*cos(newmet_dphi_2)/pt_2,1.0),-2.2)'))
ttbar_mc_corr_string = ttbar_mc_corr_string[:-1] + ')'

init_ttbar_mc_corr_string = ttbar_mc_corr_string[:]

# pt_1 correction
var = 'pt_1[20,%(crosstrg_pt)s,35,40,45,50,60,70,80,100,120,140,160,180,200]' % vars()
mt_1_regions = {'tightmT':'mt_1<40' % vars(),'loosemT':'mt_1>=40' % vars()}
ttbar_mc_corr_string += "*("
for add_name, add_cut in mt_1_regions.items():
  pol_to_use = 'pol1'
  corr_cut = add_cut
  corr_name = 'l_pt_'+add_name
  (_,_,_,ttbar_mc_data) = DrawHists(var, '(('+baseline_iso_pass+')*('+corr_cut+'))', corr_name+'_closure' % vars(),input_folder,file_ext,doMC=False,doW=False,doQCD=False,doTT=True)
  (_,_,_,ttbar_mc_pred) = DrawHists(var, '(('+baseline_iso_fail+')*('+corr_cut+'))', corr_name+'_closure_pred' % vars(),input_folder,file_ext,add_wt="("+ff_ttbar_mc+")",doMC=False,doW=False,doQCD=False,doTT=True)
  fout.cd()
  ttbar_mc_data.Divide(ttbar_mc_pred)

  ttbar_mc_data_fit, ttbar_mc_data_uncert =  FitCorrection(ttbar_mc_data,func=pol_to_use,fit_range=[crosstrg_pt,200.])

  ttbar_mc_data.Write()
  ttbar_mc_data_fit.Write()
  ttbar_mc_data_uncert.Write()
  PlotFakeFactorCorrection(ttbar_mc_data, ttbar_mc_data_uncert, ttbar_mc_data.GetName(), output_folder, wp, channel, year, 0, 200, x_title='p_{T}^{#mu} (GeV)',logx=True)

  low_pt_corr_cut = corr_cut+'&&pt_1<%(crosstrg_pt)s' % vars()
  high_pt_corr_cut = corr_cut+'&&pt_1>=%(crosstrg_pt)s' % vars()
  low_pt_corr = ttbar_mc_data.GetBinContent(1) 
  ttbar_mc_corr_string += '((%s)*(%s)+(%s)*(%s))+' % (high_pt_corr_cut,str(fout.Get(corr_name+'_closure_ttbar_mc_fit').GetExpFormula('p')).replace('x','min(pt_1,200)'),low_pt_corr_cut, low_pt_corr)
ttbar_mc_corr_string = ttbar_mc_corr_string[:-1] + ')'

fout.Close()

# print fake factor strings after closures
print "---------------------------------------------------------------------------------------------------"
print "Fake Factor Strings After Closure Corrections"
print "---------------------------------------------------------------------------------------------------"
print "QCD Fake Factors:"
print "ff_qcd='((" + ff_qcd + ")" + qcd_corr_string + ")'"
print "---------------------------------------------------------------------------------------------------"
print "QCD AISO Fake Factors:"
print "ff_qcd_aiso='((" + ff_qcd_aiso + ")" + qcd_aiso_corr_string + ")'"
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


