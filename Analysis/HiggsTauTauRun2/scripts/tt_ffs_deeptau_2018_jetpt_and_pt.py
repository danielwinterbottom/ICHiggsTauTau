import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import re
import os
import time
import json
from array import array
import argparse
import math
ROOT.gROOT.SetBatch(1)

ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)

parser = argparse.ArgumentParser()
parser.add_argument('--wp',help= 'Tau ID working point to measure fake factors for', default='medium')
parser.add_argument('--file_ext',help= 'Extension of files names', default='_tt_2018.root')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='./')
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_2018.json')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default='/vols/cms/gu18/Offline/output/SUSY/Jan24_2018_SUSY/')
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
parser.add_argument('--year','-y', help= 'year', default=2018)
args = parser.parse_args()

wp = args.wp
file_ext = args.file_ext
output_folder=args.output_folder
params_file=args.params
input_folder=args.input_folder
draw = int(args.draw) > 0
lumi=58826.8469

out_file = '%(output_folder)s/fakefactor_fits_tt_%(wp)s_2018.root' % vars()

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

if year == 2017:

  lumi=41530.

  out_file = '%(output_folder)s/fakefactor_fits_tt_%(wp)s_2017.root' % vars()

  # read params from json

  with open('scripts/params_2017.json') as jsonfile:
    params = json.load(jsonfile)

  data_files = [
    'TauB',
    'TauC',
    'TauD',
    'TauE',
    'TauF',
  ]
  
  ttbar_files = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic']
  
  wjets_files = ['WJetsToLNu-LO','WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
  
  # Add MC sample names   
  other_files = ['DYJetsToLL-LO','DYJetsToLL-LO-ext1','DY1JetsToLL-LO','DY1JetsToLL-LO-ext','DY2JetsToLL-LO','DY2JetsToLL-LO-ext','DY3JetsToLL-LO','DY3JetsToLL-LO-ext','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO-ext1','T-tW', 'Tbar-tW','Tbar-t','T-t','WWToLNuQQ','WZTo2L2Q','WZTo1L1Nu2Q','WZTo1L3Nu','WZTo3LNu','ZZTo2L2Nu','WWTo2L2Nu','ZZTo2L2Q','ZZTo4L-ext','ZZTo4L','EWKZ2Jets']

if year == 2016:  

  lumi=35920.

  out_file = '%(output_folder)s/fakefactor_fits_tt_%(wp)s_2016.root' % vars()

  # read params from json

  with open('scripts/params_leg2016.json') as jsonfile:
    params = json.load(jsonfile)

  data_files = [
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
  
  wjets_files = ['WJetsToLNu-LO', 'WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W2JetsToLNu-LO-ext','W3JetsToLNu-LO','W3JetsToLNu-LO-ext','W4JetsToLNu-LO','W4JetsToLNu-LO-ext1','W4JetsToLNu-LO-ext2', 'EWKWMinus2Jets_WToLNu','EWKWMinus2Jets_WToLNu-ext1','EWKWMinus2Jets_WToLNu-ext2','EWKWPlus2Jets_WToLNu','EWKWPlus2Jets_WToLNu-ext1','EWKWPlus2Jets_WToLNu-ext2']
  
  # Add MC sample names   
  other_files = ['DYJetsToLL-LO-ext1','DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO', 'T-tW', 'Tbar-tW','Tbar-t','T-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','VVTo2L2Nu-ext1','ZZTo2L2Q','ZZTo4L-amcat','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','EWKZ2Jets_ZToLL','EWKZ2Jets_ZToLL-ext1','EWKZ2Jets_ZToLL-ext2']
  
  
string_0jet = '(n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_1)<2.4) || (n_lowpt_jets==2&&fabs(jeta_1)<2.4&&fabs(jeta_2)<2.4) )'
string_1jet = '(n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_1)<2.4) || (n_lowpt_jets==2&&fabs(jeta_1)<2.4&&fabs(jeta_2)<2.4) )==0'

pt_bins = {

              '0jet': '%(string_0jet)s' % vars(),
              '1jet': '%(string_1jet)s' % vars(),

              'jet_pt_low_0jet': '%(string_0jet)s && jet_pt_1<1.25*pt_1' % vars(),
              'jet_pt_med_0jet': '%(string_0jet)s && jet_pt_1>=1.25*pt_1&&jet_pt_1<1.5*pt_1' % vars(),
              'jet_pt_high_0jet': '%(string_0jet)s &&jet_pt_1>=1.5*pt_1' % vars(),

              'jet_pt_low_1jet': '%(string_1jet)s&&jet_pt_1<1.25*pt_1' % vars(),
              'jet_pt_med_1jet': '%(string_1jet)s&&jet_pt_1>=1.25*pt_1&&jet_pt_1<1.5*pt_1' % vars(),
              'jet_pt_high_1jet': '%(string_1jet)s&&jet_pt_1>=1.5*pt_1' % vars(),
}

pt_bins_fit = {
}



def ZeroNegativeBins(h):
  # if a bin of h1 is equal to the -ve of 
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1'):
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

  data_qcd = hout.Clone()
  data_qcd.SetName(name+'_qcd')
  bkgs_qcd = hout.Clone()
  bkgs_qcd.SetName(name+'_bkgs_qcd')

  # draw data for QCD
  for i in data_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    if not doOS:
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(add_wt)s)' % vars(),'goff')
    else:
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(add_wt)s)' % vars(),'goff')

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
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(add_wt)s)' % vars(),'goff')
    else:
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(add_wt)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs_qcd.Add(h)

  bkgs_qcd = ZeroNegativeBins(bkgs_qcd)
  data_qcd.Add(bkgs_qcd,-1)
  data_qcd = ZeroNegativeBins(data_qcd)

  return data_qcd

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

def FitFakeFactors(h,pt_min,pt_max,usePol1=False,polOnly=None):
  h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
  maxx=h.GetBinLowEdge(h.GetNbinsX()+1)
  maxx=200.
  f1 = ROOT.TF1("f1","landau",40,maxx)
  f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",40,maxx)
  #f2.SetParLimits(3,0,1)
  if usePol1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",40,maxx)
  if polOnly is not None:
    if polOnly == 0:
      f1 = ROOT.TF1("f1","pol0",0,400)
      f2 = ROOT.TF1("f2","pol0",0,400)
    if polOnly == 1:
      f1 = ROOT.TF1("f1","pol1",0,400)
      f2 = ROOT.TF1("f2","pol1",0,400)

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
    h.Fit("f1",'R')
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
    # to get initial value for pol0 take last bin content
    for i in range(h.GetNbinsX(),0,-1): 
      if h.GetBinContent(i)>0.: 
        f2.SetParameter(3,h.GetBinContent(i))
        break
    if usePol1: f2.SetParameter(4,0)
 
  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    fitresult = h.Fit("f2",'SR')
    rep = int(fitresult) != 0
    if not rep or count>100:
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')
  return fit, h_uncert, h

def FitCorrection(h, func='pol1'):
  h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
  f1 = ROOT.TF1("f1",func)
  # now fit with the full functions
  # repeat fit up to 100 times until the fit converges properly
  rep = True
  count = 0
  while rep:
    fitresult = h.Fit("f1",'SI')
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
  #if f.GetMaximum() > 1.: f.SetMaximum(1.)
  f.SetStats(0)
  f.GetXaxis().SetTitle('Jet-p_{T} (GeV)')
  f.GetYaxis().SetTitle('F_{F}')
  f.SetTitle(name)
  f.SetLineColor(ROOT.kBlack)
  f.Draw()
  h.SetStats(0)
  h.SetFillColor(ROOT.kBlue-10)
  h.Draw("e3 same")
  f.Draw("a sames")
  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')
  #time.sleep(2)

def VariableBinning(ptbin):
  binning = [40,45,50,55,60,65,70,80,90,100,120,140,200,400]
  return 'pt_1'+str(binning)

def WriteFunctionForApplyingFakeFactors(fout,inclusive=False,aiso=False):
  outstring=''
  extra=''
  if aiso: extra = '_aiso2'
  ff_bins = []
  if not inclusive:
    ff_bins = ['jet_pt_low_0jet', 'jet_pt_med_0jet', 'jet_pt_high_0jet', 'jet_pt_low_1jet', 'jet_pt_med_1jet', 'jet_pt_high_1jet']
  else: 
    ff_bins = ['0jet', '1jet']

  first=True
  for i in ff_bins:
    if not first: outstring+=' + ' 
    func = fout.Get(i+extra+'_pt_1_ff_qcd_fit')
    ff = func.GetExpFormula('p')
    hist = fout.Get(i+extra+'_pt_1_ff_qcd')
    if 'high' not in i:
      high_bin = hist.GetBinContent(hist.FindBin(250.)) 
      ff = '(pt_1<200)*(%(ff)s) + (pt_1>=200)*%(high_bin).5f' % vars() 
    outstring += '('+pt_bins[i]+')*('+str(ff)+')'
    first=False
  outstring=outstring.replace('x','min(pt_1,199.)')

  return '('+outstring+')'


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
  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')

draw_list=[]

# tt plots
baseline_bothiso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
baseline_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()

ff_list = {}
to_write = []

for ptbin in pt_bins:
  name = ptbin
  cuts = pt_bins[ptbin]
  cut_iso1 = re.sub('X', '1', '(%(baseline_bothiso)s)*(%(cuts)s)' % vars())
  cut_aiso1 = re.sub('X', '1','(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
  var1 = VariableBinning(ptbin)
  ff_list[name+'_pt_1'] = (var1, cut_iso1, cut_aiso1)

# add aiso plots

baseline_aiso2_iso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
baseline_aiso2_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()

for ptbin in pt_bins:
  name = ptbin
  cuts = pt_bins[ptbin]
  cut_iso1 = re.sub('X', '2', '(%(baseline_aiso2_iso)s)*(%(cuts)s)' % vars())
  cut_aiso1 = re.sub('X', '2','(%(baseline_aiso2_aiso1)s)*(%(cuts)s)' % vars())
  var1 = VariableBinning(ptbin)
  ff_list[name+'_aiso2_pt_1'] = (var1, cut_iso1, cut_aiso1)


for ff in ff_list:
  wjets_mc_ff=None
  ttbar_mc_ff=None
  if draw:
    qcd_iso = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff))
    qcd_aiso = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff))

    qcd_ff = CalculateFakeFactors(qcd_iso, qcd_aiso)
    to_write.append(qcd_ff)

  else:
    # if not drawing histogram then retrieve them from the old output folder
    fin = ROOT.TFile(out_file)
    qcd_ff = fin.Get(ff+'_ff_qcd')
    qcd_ff.SetDirectory(0)
    to_write.append(qcd_ff)
    fin.Close()

  # Get Fit Type
  if 'inclusive' in ff:
    fit_type_qcd = pt_bins_fit['qcd_inclusive']
    fit_type_qcd_aiso = pt_bins_fit['qcd_aiso_inclusive']
  else:
    pt_bin = ff.split('_')[0] + '_' + ff.split('_')[1]
    if 'qcd_'+pt_bin in pt_bins_fit: fit_type_qcd = pt_bins_fit['qcd_'+pt_bin]
    else: fit_type_qcd = 'landau'
    if 'qcd_aiso_'+pt_bin in pt_bins_fit: fit_type_qcd_aiso = pt_bins_fit['qcd_aiso_'+pt_bin]
    else: fit_type_qcd_aiso = 'landau'

  # Choose Fit
  polOnlyInFit_qcd = None

  if not 'aiso' in ff:
    if 'pol0' in fit_type_qcd:
      polOnlyInFit_qcd = 0
  else:
    if 'pol0' in fit_type_qcd_aiso:
      polOnlyInFit_qcd = 0
  
  pt_min = ff.split('pt')[1].split('_')[0]
  pt_max = ff.split('_')[1].split('_')[0]

  # do fitting
  print 'Fitting for ', ff
  (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff,pt_min,pt_max,polOnly=polOnlyInFit_qcd)
  to_write.append(qcd_fit)
  to_write.append(qcd_uncert)
  PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp)


# write raw fake factors

fout = ROOT.TFile(out_file, 'RECREATE')

for i in to_write: i.Write()

#print "QCD FF"
tau1_string =  WriteFunctionForApplyingFakeFactors(fout)
tau1_aiso_string =  WriteFunctionForApplyingFakeFactors(fout,aiso=True)
print tau1_string, '\n\n', tau1_aiso_string


### Closure Corrections #####
### same sign closure corrections

var='dR[0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0]'
for nbjet in ['0','1']:
  cut = ''
  if nbjet =='0': cut = 'n_bjets==0'
  if nbjet =='1': cut = 'n_bjets>0'

  # ss region
  qcd_ss_data = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', 'ss_closure_nbjet%(nbjet)s' % vars(),input_folder,file_ext,False)
  qcd_ss_pred = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', 'ss_closure_nbjet%(nbjet)s_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_string)
  fout.cd()
  qcd_ss_data.Divide(qcd_ss_pred)

  # ss aiso2 region
  qcd_ss_data_aiso2 = DrawHists(var, '('+baseline_aiso2_iso+')*('+cut+')', 'ss_aiso2_closure_nbjet%(nbjet)s' % vars(),input_folder,file_ext,False)
  qcd_ss_pred_aiso2 = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', 'ss_closure_nbjet%(nbjet)s_aiso2_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_aiso_string)
  fout.cd()
  qcd_ss_data_aiso2.Divide(qcd_ss_pred_aiso2)

  polfit = 'pol3'

  qcd_ss_data_fit, qcd_ss_data_uncert =  FitCorrection(qcd_ss_data, func=polfit)

  qcd_ss_data.Write()
  qcd_ss_data_fit.Write()
  qcd_ss_data_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data, qcd_ss_data_uncert, qcd_ss_data.GetName(), output_folder, wp, x_title='#Delta R')

  fout.cd()
  qcd_ss_data_aiso2_fit, qcd_ss_data_aiso2_uncert =  FitCorrection(qcd_ss_data_aiso2, func=polfit)

  qcd_ss_data_aiso2.Write()
  qcd_ss_data_aiso2_fit.Write()
  qcd_ss_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data_aiso2, qcd_ss_data_aiso2_uncert, qcd_ss_data_aiso2.GetName(), output_folder, wp, x_title='#Delta R')

tau1_string+='*((n_bjets==0)*(%s) + (n_bjets>0)*(%s))' % (str(fout.Get('ss_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'), str(fout.Get('ss_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'))

tau1_aiso_string+='*((n_bjets==0)*(%s) + (n_bjets>0)*(%s))' % (str(fout.Get('ss_aiso2_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'), str(fout.Get('ss_aiso2_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'))

print 'iso FF'
print tau1_string
print '`n'
print 'aiso FF'
print tau1_aiso_string

# same sign correction from aiso data

var='dR[0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0]'
var_alt='pt_1[40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200]'
for nbjet in ['0','1']:
  cut = ''
  if nbjet =='0': cut = 'n_bjets==0'
  if nbjet =='1': cut = 'n_bjets>0'

  # os aiso2 region
  qcd_os_data_aiso2 = DrawHists(var, '('+baseline_aiso2_iso+')*('+cut+')', 'os_closure_nbjet%(nbjet)s' % vars(),input_folder,file_ext,doOS=True)
  qcd_os_pred_aiso2 = DrawHists(var, '('+baseline_aiso2_aiso1+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_aiso2_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=tau1_aiso_string)
  fout.cd()

  # obtain a flat correction as well
  flat_corr = qcd_os_data_aiso2.Clone()
  flat_corr_denom = qcd_os_pred_aiso2.Clone()
  n_bins = flat_corr.GetNbinsX()
  flat_corr.Rebin(n_bins)
  flat_corr_denom.Rebin(n_bins)
  flat_corr.Divide(flat_corr_denom)
  flat_corr.SetName('os_closure_nbjet%(nbjet)s_flat' % vars())
  fout.cd()
  flat_corr.Write()

  qcd_os_data_aiso2.Divide(qcd_os_pred_aiso2)

  polfit = 'pol3'

  fout.cd()
  qcd_os_data_aiso2_fit, qcd_os_data_aiso2_uncert =  FitCorrection(qcd_os_data_aiso2, func=polfit)

  qcd_os_data_aiso2.Write()
  qcd_os_data_aiso2_fit.Write()
  qcd_os_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_os_data_aiso2, qcd_os_data_aiso2_uncert, qcd_os_data_aiso2.GetName(), output_folder, wp, x_title='#Delta R')


  #alternative parameterisation 

  qcd_os_data_aiso2 = DrawHists(var_alt, '('+baseline_aiso2_iso+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_alt' % vars(),input_folder,file_ext,doOS=True)
  qcd_os_pred_aiso2 = DrawHists(var_alt, '('+baseline_aiso2_aiso1+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_aiso2_alt_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=tau1_aiso_string)
  fout.cd()
  qcd_os_data_aiso2.Divide(qcd_os_pred_aiso2)

  polfit = 'pol3'

  fout.cd()
  qcd_os_data_aiso2_fit, qcd_os_data_aiso2_uncert =  FitCorrection(qcd_os_data_aiso2, func=polfit)

  qcd_os_data_aiso2.Write()
  qcd_os_data_aiso2_fit.Write()
  qcd_os_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_os_data_aiso2, qcd_os_data_aiso2_uncert, qcd_os_data_aiso2.GetName(), output_folder, wp, x_title='#Delta R')

tau1_string+='*((n_bjets==0)*(%s) + (n_bjets>0)*(%s))' % (str(fout.Get('os_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'), str(fout.Get('os_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'))
tau1_aiso_string+='*((n_bjets==0)*(%s) + (n_bjets>0)*(%s))' % (str(fout.Get('os_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'), str(fout.Get('os_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','min(dR,5.)'))


## Make Fractions
#
#n_bjets_bins = {'n_bjets0':'n_bjets==0','n_bjets1':'n_bjets>=1'}
#for n_bjets_name,n_bjets_cut in n_bjets_bins.items():
#  var= 'mt_1[0,10,20,30,40,50,60,70,80,90,100,120]'
#  cuts = '(%(baseline_aiso1)s)*(%(n_bjets_cut)s)' % vars()
#  name = 'tt_fracs_%(n_bjets_name)s' % vars()
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




fout.Close()
