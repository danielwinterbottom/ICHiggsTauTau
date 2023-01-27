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
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_mssm_2018.json')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default=None)
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
parser.add_argument('--year','-y', help= 'year', type=str, default='2018')
parser.add_argument("--singletau", dest="singletau", action='store_true', help="Use single tau trigger in OR")
parser.add_argument("--cms_label", help="If true will draw CMS label")
parser.add_argument('--force_pol1', help= 'force all fits to be pol1 up to 100 GeV and flat above',  action='store_true')
args = parser.parse_args()

wp = args.wp
file_ext = args.file_ext

file_ext=file_ext.replace('2018',args.year)

output_folder=args.output_folder
params_file=args.params
input_folder=args.input_folder
draw = int(args.draw) > 0
year = args.year
lumi=58826.8469

if year == '2018':

  if input_folder is None:
    input_folder = '/vols/cms/dw515/outputs/MSSM/mssm_2018/' 

out_file = '%(output_folder)s/fakefactor_fits_tt_%(wp)s_2018.root' % vars()

# read params from json

with open('scripts/params_mssm_2018.json') as jsonfile:
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


if year == '2017':

  lumi=41530.

  if input_folder is None:
    input_folder = '/vols/cms/dw515/outputs/MSSM/mssm_2017/'

  out_file = '%(output_folder)s/fakefactor_fits_tt_%(wp)s_2017.root' % vars()

  # read params from json

  with open('scripts/params_mssm_2017.json') as jsonfile:
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

if year == '2016':  

  lumi=35920.

  if input_folder is None:
    input_folder = '/vols/cms/dw515/outputs/MSSM/mssm_2016/'

  out_file = '%(output_folder)s/fakefactor_fits_tt_%(wp)s_2016.root' % vars()

  # read params from json

  with open('scripts/params_mssm_2016.json') as jsonfile:
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
  
  
string_0jet = 'n_prebjets==0'
string_1jet = 'n_prebjets>0'

pt_bins = {

              'inclusive': '1' % vars(),
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
    if t.GetEntries() == 0: continue
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
    if t.GetEntries() == 0: continue
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
    if t.GetEntries() == 0: continue
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

  if args.force_pol1 and not polOnly == 0:
      f1 = ROOT.TF1("f1","[0]+[1]*min(100.,x)",0,100)
      f2 = ROOT.TF1("f2","[0]+[1]*min(100.,x)",0,100)

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
    h.Fit("f1",'NR')
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
    fitresult = h.Fit("f2",'NSR')
    rep = int(fitresult) != 0
    if not rep or count>100:
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')
  fit.Print("all")

  islandau = 'landau' in str(fit.GetExpFormula('p')).lower()

  if islandau:
    if 'high' in h.GetName():
      # find minimum bin value over 80 GeV
      minbin = h.FindBin(80.1)
      minval = h.GetBinContent(minbin)
      maxbin_to_check = h.GetNbinsX()

      for i in range(minbin+1,maxbin_to_check+1):
        bin_val = h.GetBinContent(i)
        if bin_val != 0 and bin_val < minval and h_uncert.GetBinContent(h_uncert.FindBin(h.GetBinCenter(i)))>=bin_val: # stop it choosing the lowest value way where fit way below values
          minbin = i
          minval = bin_val

      # if no bins above 80 GeV filled find the largest filled bin
      if minval == 0:
        for i in range(h.GetNbinsX()+1,0,-1):
          if h.GetBinContent(i) > 0:
            break
        minbin_val = h.GetBinLowEdge(i)
      else:
        minbin_val = h.GetBinLowEdge(minbin)

      p0,p1,p2,p3 = fit.GetParameter(0),fit.GetParameter(1),fit.GetParameter(2),fit.GetParameter(3)
      if not usePol1:
        fit = ROOT.TF1(h.GetName()+'_fit',"%(p0)s*TMath::Landau(min(x,%(minbin_val)s),%(p1)s,%(p2)s)+%(p3)s" % vars(),20,600)
      else:
        p4 = fit.GetParameter(4)
        fit = ROOT.TF1(h.GetName()+'_fit',"%(p0)s*TMath::Landau(min(x,%(minbin_val)s),%(p1)s,%(p2)s)+%(p3)s+(%(p4)s*x)" % vars(),20,600)
      for j in range(0,h_uncert.GetNbinsX()+1):
        if h_uncert.GetBinLowEdge(j) >= minbin_val:
          h_uncert.SetBinContent(j,h_uncert.GetBinContent(h_uncert.GetXaxis().FindBin(minbin_val)))
          h_uncert.SetBinError(j,h_uncert.GetBinError(h_uncert.GetXaxis().FindBin(minbin_val)))
    else:
      p0,p1,p2,p3 = fit.GetParameter(0),fit.GetParameter(1),fit.GetParameter(2),fit.GetParameter(3)
      if not usePol1:
        fit = ROOT.TF1(h.GetName()+'_fit',"%(p0)s*TMath::Landau(min(x,170.),%(p1)s,%(p2)s)+%(p3)s" % vars(),20,600)
      else:
        p4 = fit.GetParameter(4)
        fit = ROOT.TF1(h.GetName()+'_fit',"%(p0)s*TMath::Landau(min(x,170.),%(p1)s,%(p2)s)+%(p3)s+(%(p4)s*min(x,170.))" % vars(),20,600)

      for j in range(0,h_uncert.GetNbinsX()+1):
        if h_uncert.GetBinLowEdge(j) >= 170 and h_uncert.GetBinLowEdge(j) < 200:
          h_uncert.SetBinContent(j,h_uncert.GetBinContent(h_uncert.GetXaxis().FindBin(170)))
          h_uncert.SetBinError(j,h_uncert.GetBinError(h_uncert.GetXaxis().FindBin(170)))


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

#def PlotFakeFactor(f, h, name, output_folder, wp):
#  c1 = ROOT.TCanvas()
#  f.SetMinimum(0)
#  #if f.GetMaximum() > 1.: f.SetMaximum(1.)
#  f.SetStats(0)
#  f.GetXaxis().SetTitle('Jet-p_{T} (GeV)')
#  f.GetYaxis().SetTitle('F_{F}')
#  f.SetTitle(name)
#  f.SetLineColor(ROOT.kBlack)
#  f.Draw()
#  h.SetStats(0)
#  h.SetFillColor(ROOT.kBlue-10)
#  h.Draw("e3 same")
#  f.Draw("a sames")
#  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')
  #time.sleep(2)

def PlotFakeFactor(h, u, name, output_folder, wp, year):
  print name
  if 'jet_pt_high' not in name:
    bin_cap = 200.
    high_bin = h.GetBinContent(h.FindBin(250.))
    high_bin_err = h.GetBinError(h.FindBin(250.))
  else:
    bin_cap = 600.
    high_bin = u.GetBinContent(u.FindBin(599.9))
    high_bin_err = u.GetBinError(u.FindBin(599.9))

  for i in range(1,u.GetNbinsX()+1):
    if u.GetBinLowEdge(i) >= bin_cap:
      u.SetBinContent(i,high_bin)
      u.SetBinError(i,high_bin_err)

  channel_string = "#tau_{h}#tau_{h}"

  if year == "2016":
    lumi_string = "35.9 fb^{-1}"
  elif year == "2017":
    lumi_string = "41.5 fb^{-1}"
  elif year == "2018":
    lumi_string = "59.7 fb^{-1}"

  if 'aiso2' in name:
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
  else:
    n_prejets_string = ''

  if 'jet_pt_low' in name:
    jet_pt_string = 'p_{T}^{jet}/p_{T}^{#tau_{h}} < 1.25'
  elif 'jet_pt_med' in name:
    jet_pt_string = '1.25 #leq p_{T}^{jet}/p_{T}^{#tau_{h}} < 1.5'
  elif 'jet_pt_high' in name:
    jet_pt_string = 'p_{T}^{jet}/p_{T}^{#tau_{h}} #geq 1.5'
  else:
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
  c.Print('%(output_folder)s/ff_fit_%(name)s_tt_%(year)s.pdf' % vars())


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
  outstring=outstring.replace('x','pt_1')

  return '('+outstring+')'

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

def PlotDistributionComparison(var,x_label,dist_1,dist_1_name,dist_2,dist_2_name,output_folder,save_name):
  # ratio is dist_1/dist_2
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  dist_2_ratio = dist_2.Clone()
  dist_2_ratio.Divide(dist_2)

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(dist_2)

  c = ROOT.TCanvas('c','c',1400,700)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  if 'mt_tot' in var or 'm_vis' in var:
    pad1.SetLogx()
  pad1.Draw()
  pad1.cd()

  dist_2.Draw("BAR")
  dist_2.SetStats(0)
  dist_2.SetFillColor(38)
  dist_2.GetXaxis().SetTitle(x_label)
  dist_2.GetYaxis().SetTitle('Events')
  dist_2.GetYaxis().SetTitleOffset(0.8)
  dist_2.GetYaxis().SetTitleSize(0.04)
  dist_2.GetXaxis().SetLabelSize(0)

  dist_1.Draw("E SAME")
  dist_1.SetMarkerColor(1)
  dist_1.SetLineColor(1)
  dist_1.SetMarkerStyle(19)

  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  l.AddEntry(dist_1,dist_1_name,"lep")
  l.AddEntry(dist_2,dist_2_name,"f");
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  if 'mt_tot' in var or 'm_vis' in var:
    pad2.SetLogx()
  pad2.Draw()
  pad2.cd()

  dist_2_ratio.SetFillColor(16)
  dist_2_ratio.Draw("E3")
  dist_2_ratio.SetAxisRange(0.6,1.4,'Y')
  dist_2_ratio.SetStats(0)
  dist_2_ratio.GetXaxis().SetLabelSize(0.08)
  dist_2_ratio.GetYaxis().SetLabelSize(0.08)
  dist_2_ratio.GetXaxis().SetTitle(x_label)
  dist_2_ratio.GetYaxis().SetTitle("Obs/Pred")
  dist_2_ratio.GetYaxis().SetTitleSize(0.1)
  dist_2_ratio.GetYaxis().SetTitleOffset(0.2)
  dist_2_ratio.GetXaxis().SetTitleSize(0.1)
  dist_2_ratio.GetXaxis().SetTitleOffset(0.9)

  dist_1_ratio.Draw("E SAME")
  dist_1_ratio.SetMarkerColor(1)
  dist_1_ratio.SetLineColor(1)
  dist_1_ratio.SetMarkerStyle(19)

  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()


#def PlotFakeFactorCorrection(f, h, name, output_folder, wp,x_title='E_{T}^{miss} (GeV)'):
#  c1 = ROOT.TCanvas()
#  f.SetMinimum(0)
#  f.SetStats(0)
#  f.GetXaxis().SetTitle(x_title)
#  f.GetYaxis().SetTitle('FF')
#  f.SetTitle(name)
#  f.SetLineColor(ROOT.kBlack)
#  f.Draw()
#  h.SetStats(0)
#  h.SetFillColor(ROOT.kBlue-10)
#  h.Draw("e3 same")
#  f.Draw("a sames")
#  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')

def PlotFakeFactorCorrection(h, u, name, output_folder, wp, year, low_bounded_val, high_bounded_val, x_title='E_{T}^{miss} (GeV)',logx=False):

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

  DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
  DrawTitle(c, '%(channel_string)s %(bkg_string)s' % vars(), 1, textSize=0.3)
  DrawTitle(c, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.3)

  c.Print('%(output_folder)s/ff_closure_%(name)s_tt_%(year)s.pdf' % vars())


draw_list=[]

# tt plots
baseline_bothiso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
baseline_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()


#if args.singletau:
#  baseline_bothiso = baseline_bothiso.replace('trg_doubletau','(trg_doubletau || (pt_1>200 && trg_singletau_1) || (pt_2>200 && trg_singletau_2))')
#  baseline_aiso1 = baseline_aiso1.replace('trg_doubletau','(trg_doubletau || (pt_1>200 && trg_singletau_1) || (pt_2>200 && trg_singletau_2))')

if year == "2016":
  baseline_bothiso = '(deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>120 && trg_singletau_1) || (pt_2>120 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)' % vars()
else:
  baseline_bothiso = '(deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)' % vars()

baseline_aiso1 = baseline_bothiso.replace("deepTauVsJets_%(wp)s_1>0.5" % vars(),"deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5" % vars())
baseline_aiso2_iso = baseline_bothiso.replace("deepTauVsJets_%(wp)s_2>0.5" % vars(),"deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5")
baseline_aiso2_aiso1 = baseline_aiso1.replace("deepTauVsJets_%(wp)s_2>0.5" % vars(),"deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5")


ff_list = {}
to_write = []

for ptbin in pt_bins:
  name = ptbin
  cuts = pt_bins[ptbin]
  cut_iso1 = re.sub('X', '1', '(%(baseline_bothiso)s)*(%(cuts)s)' % vars())
  cut_aiso1 = re.sub('X', '1','(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
  var1 = VariableBinning(ptbin)
  if not ('high' in ptbin or 'med' in ptbin or 'low' in ptbin): var1='pt_1[40,45,50,55,60,65,70,80,90,100,120,140,160,200,240,300,400]'
  ff_list[name+'_pt_1'] = (var1, cut_iso1, cut_aiso1)

# add aiso plots

#baseline_aiso2_iso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
#baseline_aiso2_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()

#if args.singletau:
#  baseline_aiso2_iso = baseline_aiso2_iso.replace('trg_doubletau','(trg_doubletau || (pt_1>200 && trg_singletau_1) || (pt_2>200 && trg_singletau_2))')
#  baseline_aiso2_aiso1 = baseline_aiso2_aiso1.replace('trg_doubletau','(trg_doubletau || (pt_1>200 && trg_singletau_1) || (pt_2>200 && trg_singletau_2))')

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
  #if 'inclusive' in ff:
  #  fit_type_qcd = pt_bins_fit['qcd_inclusive']
  #  fit_type_qcd_aiso = pt_bins_fit['qcd_aiso_inclusive']
  #else:
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
  if "low" not in ff:
    (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff,pt_min,pt_max,polOnly=polOnlyInFit_qcd)
  else:
    (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff,pt_min,pt_max,polOnly=polOnlyInFit_qcd,usePol1=True)
  to_write.append(qcd_fit)
  to_write.append(qcd_uncert)
  PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp, year)


# write raw fake factors
if args.force_pol1: out_file=out_file.replace('.root','_forcepol1.root')
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
  if nbjet =='0': cut = 'n_deepbjets==0'
  if nbjet =='1': cut = 'n_deepbjets>0'

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

  if nbjet == '0':
    polfit = 'pol3'
  else:
    polfit = 'pol1'


  qcd_ss_data_fit, qcd_ss_data_uncert =  FitCorrection(qcd_ss_data, func=polfit)

  qcd_ss_data.Write()
  qcd_ss_data_fit.Write()
  qcd_ss_data_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data, qcd_ss_data_uncert, qcd_ss_data.GetName(), output_folder, wp, year, 0.5, 4.5, x_title='#Delta R')

  fout.cd()
  qcd_ss_data_aiso2_fit, qcd_ss_data_aiso2_uncert =  FitCorrection(qcd_ss_data_aiso2, func=polfit)

  qcd_ss_data_aiso2.Write()
  qcd_ss_data_aiso2_fit.Write()
  qcd_ss_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data_aiso2, qcd_ss_data_aiso2_uncert, qcd_ss_data_aiso2.GetName(), output_folder, wp, year, 0.5, 4.5, x_title='#Delta R')

tau1_string+='*((n_deepbjets==0)*(%s) + (n_deepbjets>0)*(%s))' % (str(fout.Get('ss_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(dR,4.5),0.5)'), str(fout.Get('ss_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(dR,4.5),0.5)'))

tau1_aiso_string+='*((n_deepbjets==0)*(%s) + (n_deepbjets>0)*(%s))' % (str(fout.Get('ss_aiso2_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(0.5,min(dR,4.5))'), str(fout.Get('ss_aiso2_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(0.5,min(dR,4.5))'))

print 'iso FF'
print tau1_string
print '`n'
print 'aiso FF'
print tau1_aiso_string

# for an additional uncertainty due to the non-closure on the pt_2 distribution derive additional non-closure here

var='pt_2[40,45,50,55,60,65,70,80,90,100,120,140]'
cut = '1'

# ss region
qcd_ss_data = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', 'ss_pt_2_closure' % vars(),input_folder,file_ext,False)
qcd_ss_pred = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', 'ss_pt_2_closure_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_string)
fout.cd()
qcd_ss_data.Divide(qcd_ss_pred)

polfit = 'pol1'

qcd_ss_data_fit, qcd_ss_data_uncert =  FitCorrection(qcd_ss_data, func=polfit)

qcd_ss_data.Write()
qcd_ss_data_fit.Write()
qcd_ss_data_uncert.Write()
PlotFakeFactorCorrection(qcd_ss_data, qcd_ss_data_uncert, qcd_ss_data.GetName(), output_folder, wp, year, 0.5, 4.5, x_title='p_{T} (GeV)')

var='met[0,10,20,30,40,50,60,70,80,100,120,140,160,200]'
cut = '1'

# ss region
qcd_ss_data = DrawHists(var, '('+baseline_bothiso+')*('+cut+')', 'ss_met_closure' % vars(),input_folder,file_ext,False)
qcd_ss_pred = DrawHists(var, '('+baseline_aiso1+')*('+cut+')', 'ss_met_closure_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_string)
fout.cd()
qcd_ss_data.Divide(qcd_ss_pred)

polfit = 'pol1'

qcd_ss_data_fit, qcd_ss_data_uncert =  FitCorrection(qcd_ss_data, func=polfit)

qcd_ss_data.Write()
qcd_ss_data_fit.Write()
qcd_ss_data_uncert.Write()
PlotFakeFactorCorrection(qcd_ss_data, qcd_ss_data_uncert, qcd_ss_data.GetName(), output_folder, wp, year, 0.5, 4.5, x_title='MET (GeV)')

# same sign correction from aiso data

var='dR[0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0]'
var_alt='pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200,400]'
for nbjet in ['0','1']:
  cut = ''
  if nbjet =='0': cut = 'n_deepbjets==0'
  if nbjet =='1': cut = 'n_deepbjets>0'

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

  if nbjet == '0':
    polfit = 'pol3'
  else:
    polfit = 'pol1'

  fout.cd()
  qcd_os_data_aiso2_fit, qcd_os_data_aiso2_uncert =  FitCorrection(qcd_os_data_aiso2, func=polfit)

  qcd_os_data_aiso2.Write()
  qcd_os_data_aiso2_fit.Write()
  qcd_os_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_os_data_aiso2, qcd_os_data_aiso2_uncert, qcd_os_data_aiso2.GetName(), output_folder, wp, year, 0.5, 4.5, x_title='#Delta R')


  #alternative parameterisation 

  qcd_os_data_aiso2 = DrawHists(var_alt, '('+baseline_aiso2_iso+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_alt' % vars(),input_folder,file_ext,doOS=True)
  qcd_os_pred_aiso2 = DrawHists(var_alt, '('+baseline_aiso2_aiso1+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_aiso2_alt_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=tau1_aiso_string)
  fout.cd()
  qcd_os_data_aiso2.Divide(qcd_os_pred_aiso2)

  polfit = 'pol1'

  fout.cd()
  qcd_os_data_aiso2_fit, qcd_os_data_aiso2_uncert =  FitCorrection(qcd_os_data_aiso2, func=polfit)

  qcd_os_data_aiso2.Write()
  qcd_os_data_aiso2_fit.Write()
  qcd_os_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_os_data_aiso2, qcd_os_data_aiso2_uncert, qcd_os_data_aiso2.GetName(), output_folder, wp, year, 0, 200, x_title='p_{T} (GeV)',logx=True)

tau1_string+='*((n_deepbjets==0)*(%s) + (n_deepbjets>0)*(%s))' % (str(fout.Get('os_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(dR,4.5),0.5)'), str(fout.Get('os_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(dR,4.5),0.5)'))
tau1_aiso_string+='*((n_deepbjets==0)*(%s) + (n_deepbjets>0)*(%s))' % (str(fout.Get('os_closure_nbjet0_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(dR,4.5),0.5)'), str(fout.Get('os_closure_nbjet1_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(dR,4.5),0.5)'))

print "fake_factor_wt_string_1 = \"" + tau1_string + "\""
# Adding additional closure correction
#var_alt = 'mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]'
#for nbjet in ['0','1']:
#  cut = ''
#  if nbjet =='0': cut = 'n_deepbjets==0'
#  if nbjet =='1': cut = 'n_deepbjets>0'
#
#  qcd_os_data_aiso2 = DrawHists(var_alt, '('+baseline_aiso2_iso+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_corr2' % vars(),input_folder,file_ext,doOS=True)
#  qcd_os_pred_aiso2 = DrawHists(var_alt, '('+baseline_aiso2_aiso1+')*('+cut+')', 'os_closure_nbjet%(nbjet)s_aiso2_corr2_pred' % vars(),input_folder,file_ext,doOS=True,add_wt=tau1_aiso_string)
#  fout.cd()
#  qcd_os_data_aiso2.Divide(qcd_os_pred_aiso2)
#
#  polfit = 'pol3'
#
#  fout.cd()
#  qcd_os_data_aiso2_fit, qcd_os_data_aiso2_uncert =  FitCorrection(qcd_os_data_aiso2, func=polfit)
#
#  qcd_os_data_aiso2.Write()
#  qcd_os_data_aiso2_fit.Write()
#  qcd_os_data_aiso2_uncert.Write()
#  PlotFakeFactorCorrection(qcd_os_data_aiso2, qcd_os_data_aiso2_uncert, qcd_os_data_aiso2.GetName(), output_folder, wp, year, 0, 800, x_title='m_{vis} (GeV)',logx=True)
#
#tau1_string+='*((n_deepbjets==0)*(%s) + (n_deepbjets>0)*(%s))' % (str(fout.Get('os_closure_nbjet0_corr2_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(mt_tot,800.),20.)'), str(fout.Get('os_closure_nbjet1_corr2_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(mt_tot,800.),20.)'))
#tau1_aiso_string+='*((n_deepbjets==0)*(%s) + (n_deepbjets>0)*(%s))' % (str(fout.Get('os_closure_nbjet0_corr2_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(mt_tot,800.),20.)'), str(fout.Get('os_closure_nbjet1_corr2_qcd_fit' % vars()).GetExpFormula('p')).replace('x','max(min(mt_tot,800.),20.)'))

#print "fake_factor_wt_string_1 = \"" + tau1_string + "\""

# Make Fractions

to_write=[]

n_deepbjets_bins = {'n_deepbjets0':'n_deepbjets==0','n_deepbjets1':'n_deepbjets>=1'}
for n_deepbjets_name,n_deepbjets_cut in n_deepbjets_bins.items():
  var= 'pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200,400]'
  cuts = '(%(baseline_aiso1)s)*(%(n_deepbjets_cut)s)' % vars()
  name = 'tt_fracs_%(n_deepbjets_name)s' % vars()
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

fout.cd()
for i in to_write: i.Write()

fout.Close()
