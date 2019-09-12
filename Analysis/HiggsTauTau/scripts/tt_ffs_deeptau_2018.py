import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import re
import os
import time
import json
from array import array

ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)

# set some of these under option eventually
wp = 'tight'
file_ext='_tt_2018.root'
output_folder='mvadm_ff_deeptau_2018'
params_file='scripts/params_2018_new.json'
input_folder='/vols/cms/dw515/Offline/output/SM/new_2018/'
draw=True
fit=True
lumi=58826.8469
###########################################

# read params from json

with open(params_file) as jsonfile:
  params = json.load(jsonfile)

data_files = [
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
  'WWTo1L1Nu2Q',
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
              #'njets0':'n_jets==0',
              #'njets1':'n_jets>0'
}
dm_bins = {
              'inclusive': '(1)',
              #'mvadm0':'(mva_dm_X==0)',
              #'mvadm1':'(mva_dm_X==1)',
              #'mvadm2':'(mva_dm_X==2)',
              #'mvadm10':'(mva_dm_X==10)',
              #'mvadm11':'(mva_dm_X==11)'
}

def DrawHists(var_input, cuts, name, input_folder, file_ext):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_1==6)'
  if var == 'pt_2': gen_extra='(gen_match_2==6)'

  data = hout.Clone()
  data.SetName(name+'_qcd')
  wjets = hout.Clone()
  wjets.SetName(name+'_wjets')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar')
  bkgs = hout.Clone()
  bkgs.SetName(name+'_bkgs')

  c1 = ROOT.TCanvas()

  # draw data
  for i in data_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple') 
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s*((os==0)))' % vars(),'goff')
    h = t.GetHistogram()
    data.Add(h)

  # draw wjets when tau candidate is a jet fake
  for i in wjets_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    wjets.Add(h)

  # draw ttbar when tau candidate is a jet fake 
  for i in ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    ttbar.Add(h)

  # draw all backgrounds to be subtracted from data for QCD estimation
  for i in other_files+wjets_files+ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s*((os==0)))' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)

  data.Add(bkgs,-1)
  
  return (data, wjets, ttbar)


def FitFakeFactors(h,usePol1=False):
  h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
  f1 = ROOT.TF1("f1","landau")
  f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]")
  if usePol1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x")
  # fit first with landau to get initial values for parameters - pol values set to 0 initially
  h.Fit("f1",'I')
  f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
  if usePol1: f2.SetParameter(4,0)
  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    fitresult = f.Fit("f2",'SI')
    rep = int(fitresult) != 0
    if not rep or count>100: 
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')
  return fit, h_uncert

def PlotFakeFactor(f, h, name, output_folder):
  c1 = ROOT.TCanvas() 
  f.SetMinimum(0)
  if f.GetMaximum() > 0.5: f.SetMaximum(0.5)
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
  c1.Print(output_folder+'/'+i[0]+'_'+i[1]+'_fit.pdf')
  time.sleep(2)

draw_list=[]

# tt plots
baseline_bothiso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
baseline_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
baseline_aiso2 = 'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsJets_%(wp)s_1>0.5 && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()

var1='pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200]'
var2='pt_2[40,45,50,55,60,65,70,80,90,100,120,140]'

ff_list = {}

for njetbin in njets_bins:
  for dmbin in dm_bins:
    name= dmbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+dm_bins[dmbin]
    cut_iso = 'wt*(%(baseline_bothiso)s)*(%(cuts)s)' % vars()
    cut_aiso1 = re.sub('X', '1','wt*(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
    cut_aiso2 = re.sub('X', '2','wt*(%(baseline_aiso2)s)*(%(cuts)s)' % vars())
    ff_list[name+'_pt_1'] = (var1, cut_iso, cut_aiso1)
    ff_list[name+'_pt_2'] = (var2, cut_iso, cut_aiso2)


DrawHists(ff_list['inclusive_inclusive_pt_1'][0], ff_list['inclusive_inclusive_pt_1'][1], 'inclusive_inclusive_pt_1_iso',input_folder,file_ext)

