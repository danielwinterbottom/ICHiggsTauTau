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
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='./')
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_2018.json')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default='/vols/cms/gu18/Offline/output/SUSY/Jan24_2018_SUSY/')
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

pt_bins = {
              #'inclusive': '(1)',
              'pt0_25':'(pt_2>=0 && pt_2<25)',
              #'pt25_40':'(pt_2>=25 && pt_2<40)',
              #'pt40_50':'(pt_2>=40 && pt_2<50)',
              #'pt50_80':'(pt_2>=50 && pt_2<80)',
              #'pt80_140':'(pt_2>=80 && pt_2<140)',
              #'pt140_9999':'(pt_2>=140 && pt_2<9999)',
}

pt_bins_fit = {
              'qcd_inclusive': 'landau',
              'qcd_aiso_inclusive': 'landau',
              'wjets_inclusive': 'landau',
              'wjets_mc_inclusive': 'landau',
              'ttbar_mc_inclusive': 'landau',

              'qcd_pt0_25': 'landau',
              'qcd_aiso_pt0_25': 'landau',
              'wjets_pt0_25': 'landau',
              'wjets_mc_pt0_25': 'landau',
              'ttbar_mc_pt0_25': 'landau',

              'qcd_pt25_40': 'landau',
              'qcd_aiso_pt25_40': 'landau',
              'wjets_pt25_40': 'landau',
              'wjets_mc_pt25_40': 'landau',
              'ttbar_mc_pt25_40': 'landau',

              'qcd_pt40_50': 'landau_split',
              'qcd_aiso_pt40_50': 'landau_split',
              'wjets_pt40_50': 'landau_split',
              'wjets_mc_pt40_50': 'landau_split',
              'ttbar_mc_pt40_50': 'landau_split',

              'qcd_pt50_80': 'landau_split',
              'qcd_aiso_pt50_80': 'landau_split',
              'wjets_pt50_80': 'landau_split',
              'wjets_mc_pt50_80': 'landau_split',
              'ttbar_mc_pt50_80': 'landau_split',

              'qcd_pt80_140': 'landau_split',
              'qcd_aiso_pt80_140': 'landau_split',
              'wjets_pt80_140': 'landau_split',
              'wjets_mc_pt80_140': 'landau_split',
              'ttbar_mc_pt80_140': 'landau_split',

              'qcd_pt140_9999': 'pol0',
              'qcd_aiso_pt140_9999': 'pol0',
              'wjets_pt140_9999': 'pol0',
              'wjets_mc_pt140_9999': 'landau',
              'ttbar_mc_pt140_9999': 'pol0'}


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
    if not doIso:  qcd_cuts = 'mt_1<70&&iso_1>0.05'
    else: qcd_cuts = 'mt_1<70'
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
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(os==1)*(mt_1<70)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
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

def FitFakeFactors(h,pt_min,pt_max,usePol1=False,polOnly=None,useExp=False,useSplit=False):
  h.SetMaximum(0.6)
  h_uncert = ROOT.TH1D(h.GetName()+'_uncert',"",1000,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
  f1 = ROOT.TF1("f1","landau",0,400)
  f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",0,400)
  f2.SetParLimits(3,0,1)  
  if usePol1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",0,400)
  if useExp: 
    f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+exp([4]+([5]*x))",0,400)
    f3 = ROOT.TF1("f3","exp([0]+[1]*x)",0,400) 
    f2.SetParLimits(5,-100,0)
    f2.SetParLimits(3,0,100)
  if useSplit:
    f2 = ROOT.TF1("f2","[5]*(x<[4]) + (x>[4])*([0]*TMath::Landau(x,[1],[2])+[3])",0,400)
    f2.SetParameter(5,0.1);
    f2.SetParameter(4,float(pt_min))
    f2.SetParLimits(5,0,0.4)
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
    h.Fit("f1",'IR')
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
    if usePol1: f2.SetParameter(4,0)
    if useExp:
      h_exp = h.Clone()
      h_exp.Reset()
      for i in range(1,h.GetNbinsX()+1):
        content = h.GetBinContent(i)
        error = h.GetBinError(i)
        bin_center = h.GetBinCenter(i)
        if bin_center < pt_min or bin_center > pt_max+20:
          h_exp.SetBinContent(i,content)
          h_exp.SetBinError(i,error)
      h_exp.Fit("f3",'IR')
      f2.SetParameter(4,f3.GetParameter(0)); f2.SetParameter(5,f3.GetParameter(1))
 
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

def PlotFakeFactor(f, h, name, output_folder, wp):
  c1 = ROOT.TCanvas()
  f.SetMinimum(0)
  if f.GetMaximum() > 1.: f.SetMaximum(1.)
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
  c1.Print(output_folder+'/et_'+wp+'_'+name+'_fit.pdf')
  #time.sleep(2)

def VariableBinning(ptbin):
  if 'inclusive' not in ptbin:
    x = float(ptbin.split('pt')[1].split("_")[0])
    y = float(ptbin.split('pt')[1].split("_")[1])
    if x == 0 and y == 25:
      binning = [0,10,20,22,24,26,28,30,40,50,60,70,80,90,100,120,160,200]
    elif x == 25 and y == 40:
      binning = [0,10,20,25,30,32,34,36,38,40,45,50,60,70,80,90,100,110,120,140,160,200,240]
    elif x == 40 and y == 50:
      binning = [0,20,40,44,48,50,55,60,70,80,90,100,110,120,140,160,200,240]
    elif x == 50 and y == 80:
      binning = [0,20,40,50,55,60,65,70,74,78,82,90,100,110,120,130,140,160,200,240]
    elif x == 80 and y == 140:
      binning = [0,40,60,80,90,95,100,105,110,115,120,125,130,140,150,160,180,220,260,400]
    elif x == 100 and y == 140:
      binning = [0,20,40,60,80,90,100,105,110,115,120,125,130,135,140,145,150,160,170,180,190,200,220,240,260,300,400]
    else:
      binning = [0,40,80,120,160,200,240,280,320,400]
  else:
    binning = [0,20,40,50,60,70,80,100,120,160,200,240,280,320,400]
  return 'jet_pt_2'+str(binning)

def WriteFunctionForApplyingFakeFactors(fout,pt_bins_fit,pt_bins,proc='qcd',aiso=False):
  ff_eqn_landau = 'p0*TMath::Landau(jet_pt_2,p1,p2)+p3'
  ff_eqn_landau_exp = 'p0*TMath::Landau(jet_pt_2,p1,p2)+p3+exp(p4+p5*jet_pt_2)'
  ff_eqn_pol0 = 'p0'
  ff_eqn_landau_split = '(p5*(jet_pt_2<p4)) + ((jet_pt_2>p4)*(p0*TMath::Landau(jet_pt_2,p1,p2)+p3))'
  ff_params = {}
  ff_eqn_tot = ''
  extra_bin_name = ''
  for ptbin,ptbin_cond in pt_bins.items():
    if ptbin != 'inclusive':
      extra=''
      if aiso:
        extra = '_aiso2_ss'
        extra_bin_name = '_aiso'
      f = fout.Get('%(ptbin)s%(extra)s_jet_pt_2_ff_%(proc)s_fit' % vars())
      p = f.GetParameters()
      if pt_bins_fit[proc+extra_bin_name+'_'+ptbin] == 'landau_exp':
        ff_eqn_new = ff_eqn_landau_exp.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4]).replace('p5','%f' % p[5])
      elif pt_bins_fit[proc+extra_bin_name+'_'+ptbin] == 'landau':
        ff_eqn_new = ff_eqn_landau.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])
      elif pt_bins_fit[proc+extra_bin_name+'_'+ptbin] == 'pol0':
        ff_eqn_new = ff_eqn_pol0.replace('p0','%f' % p[0])
      elif pt_bins_fit[proc+extra_bin_name+'_'+ptbin] == 'landau_split':
        ff_eqn_new = ff_eqn_landau_split.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4]).replace('p5','%f' % p[5])
      ff_eqn_tot += '((%(ptbin_cond)s)*(%(ff_eqn_new)s))+' % vars()
  ff_eqn_tot = ff_eqn_tot[:-1] # Removes final +
  return ff_eqn_tot

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

draw_list=[]

# et plots
baseline_bothiso = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && jet_pt_2>0' % vars()
baseline_aiso1 = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && jet_pt_2>0' % vars()

ff_list = {}
to_write = []

for ptbin in pt_bins:
  name = ptbin
  cuts = pt_bins[ptbin]
  cut_iso1 = re.sub('X', '2', '(%(baseline_bothiso)s)*(%(cuts)s)' % vars())
  cut_aiso1 = re.sub('X', '2','(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
  var1 = VariableBinning(ptbin)
  ff_list[name+'_jet_pt_2'] = (var1, cut_iso1, cut_aiso1)

# add aiso plots

baseline_aiso2_iso = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && jet_pt_2>0' % vars()
baseline_aiso2_aiso1 = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2 && jet_pt_2>0' % vars()

for ptbin in pt_bins:
  name = ptbin
  cuts = pt_bins[ptbin]
  cut_iso1 = re.sub('X', '2', '(%(baseline_aiso2_iso)s)*(%(cuts)s)' % vars())
  cut_aiso1 = re.sub('X', '2','(%(baseline_aiso2_aiso1)s)*(%(cuts)s)' % vars())
  var1 = VariableBinning(ptbin)
  ff_list[name+'_aiso2_ss_jet_pt_2'] = (var1, cut_iso1, cut_aiso1)


for ff in ff_list:
  wjets_mc_ff=None
  ttbar_mc_ff=None
  if draw:
    (qcd_iso, wjets_iso, wjets_mc_iso, ttbar_mc_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
    (qcd_aiso, wjets_aiso, wjets_mc_aiso, ttbar_mc_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
    qcd_ff = CalculateFakeFactors(qcd_iso, qcd_aiso)
    to_write.append(qcd_ff)

    if not 'aiso2' in ff:
      wjets_ff = CalculateFakeFactors(wjets_iso, wjets_aiso)
      to_write.append(wjets_ff)

      wjets_mc_ff = CalculateFakeFactors(wjets_mc_iso, wjets_mc_aiso)
      to_write.append(wjets_mc_ff)

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

  # Get Fit Type
  if 'inclusive' in ff:
    fit_type_qcd = pt_bins_fit['qcd_inclusive']
    fit_type_qcd_aiso = pt_bins_fit['qcd_aiso_inclusive']
    fit_type_wjets = pt_bins_fit['wjets_inclusive']
    fit_type_wjets_mc = pt_bins_fit['wjets_mc_inclusive']
    fit_type_ttbar_mc = pt_bins_fit['ttbar_mc_inclusive']
  else:
    pt_bin = ff.split('_')[0] + '_' + ff.split('_')[1]
    fit_type_qcd = pt_bins_fit['qcd_'+pt_bin]
    fit_type_qcd_aiso = pt_bins_fit['qcd_aiso_'+pt_bin]
    fit_type_wjets = pt_bins_fit['wjets_'+pt_bin]
    fit_type_wjets_mc = pt_bins_fit['wjets_mc_'+pt_bin]
    fit_type_ttbar_mc = pt_bins_fit['ttbar_mc_'+pt_bin]

  # Choose Fit
  useSplitInFit_qcd = False
  useSplitInFit_wjets = False
  useSplitInFit_wjets_mc = False
  useSplitInFit_ttbar_mc = False

  useExpInFit_qcd = False
  useExpInFit_wjets = False
  useExpInFit_wjets_mc = False
  useExpInFit_ttbar_mc = False

  polOnlyInFit_qcd = None
  polOnlyInFit_wjets = None
  polOnlyInFit_wjets_mc = None
  polOnlyInFit_ttbar_mc = None

  if not 'aiso' in ff:
    if 'exp' in fit_type_qcd:
      useExpInFit_qcd = True
    elif 'pol0' in fit_type_qcd:
      polOnlyInFit_qcd = 0
    elif 'split' in fit_type_qcd:
      useSplitInFit_qcd = True
  else:
    if 'exp' in fit_type_qcd_aiso:
      useExpInFit_qcd = True
    elif 'pol0' in fit_type_qcd_aiso:
      polOnlyInFit_qcd = 0
    elif 'split' in fit_type_qcd_aiso:
      useSplitInFit_qcd = True

  if 'exp' in fit_type_wjets:
    useExpInFit_wjets = True
  elif 'pol0' in fit_type_wjets:
    polOnlyInFit_wjets = 0
  elif 'split' in fit_type_wjets:
    useSplitInFit_wjets = True

  if 'exp' in fit_type_wjets_mc:
    useExpInFit_wjets_mc = True
  elif 'pol0' in fit_type_wjets_mc:
    polOnlyInFit_wjets_mc = 0
  elif 'split' in fit_type_wjets_mc:
    useSplitInFit_wjets_mc = True

  if 'exp' in fit_type_ttbar_mc:
    useExpInFit_ttbar_mc = True
  elif 'pol0' in fit_type_ttbar_mc:
    polOnlyInFit_ttbar_mc = 0
  elif 'split' in fit_type_ttbar_mc:
    useSplitInFit_ttbar_mc = True

 
  pt_min = ff.split('pt')[1].split('_')[0]
  pt_max = ff.split('_')[1].split('_')[0]

  # do fitting
  (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff,pt_min,pt_max,useExp=useExpInFit_qcd,polOnly=polOnlyInFit_qcd,useSplit=useSplitInFit_qcd)
  to_write.append(qcd_fit)
  to_write.append(qcd_uncert)
  PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp)

  if not 'aiso2' in ff:
    (wjets_fit, wjets_uncert, wjets_ff) = FitFakeFactors(wjets_ff,pt_min,pt_max,useExp=useExpInFit_wjets,polOnly=polOnlyInFit_wjets,useSplit=useSplitInFit_wjets)
    to_write.append(wjets_fit)
    to_write.append(wjets_uncert)
    PlotFakeFactor(wjets_ff, wjets_uncert, wjets_ff.GetName(), output_folder, wp)

    (wjets_mc_fit, wjets_mc_uncert, wjets_mc_ff) = FitFakeFactors(wjets_mc_ff,pt_min,pt_max,useExp=useExpInFit_wjets_mc,polOnly=polOnlyInFit_wjets_mc,useSplit=useSplitInFit_wjets_mc)
    to_write.append(wjets_mc_fit)
    to_write.append(wjets_mc_uncert)
    PlotFakeFactor(wjets_mc_ff, wjets_mc_uncert, wjets_mc_ff.GetName(), output_folder, wp)
    if ttbar_mc_ff:
      (ttbar_mc_fit, ttbar_mc_uncert, ttbar_mc_ff) = FitFakeFactors(ttbar_mc_ff,pt_min,pt_max,useExp=useExpInFit_ttbar_mc,polOnly=polOnlyInFit_ttbar_mc,useSplit=useSplitInFit_ttbar_mc)
      to_write.append(ttbar_mc_fit)
      to_write.append(ttbar_mc_uncert)
      PlotFakeFactor(ttbar_mc_ff, ttbar_mc_uncert, ttbar_mc_ff.GetName(), output_folder, wp)

# Make Fractions

n_bjets_bins = {'n_bjets0':'n_bjets==0','n_bjets1':'n_bjets>=1'}
for n_bjets_name,n_bjets_cut in n_bjets_bins.items():
  var= 'mt_1[0,10,20,30,40,50,60,70,80,90,100,120]'
  cuts = '(%(baseline_aiso1)s)*(%(n_bjets_cut)s)' % vars()
  name = 'et_fracs_%(n_bjets_name)s' % vars()
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

for i in to_write: 
  print i
  i.Write()

print "QCD FF"
print WriteFunctionForApplyingFakeFactors(fout,pt_bins_fit,pt_bins,proc='qcd',aiso=False)

print "QCD AISO FF"
print WriteFunctionForApplyingFakeFactors(fout,pt_bins_fit,pt_bins,proc='qcd',aiso=True)

print "W+JETS FF"
print WriteFunctionForApplyingFakeFactors(fout,pt_bins_fit,pt_bins,proc='wjets',aiso=False)

print "W+JETS MC FF"
print WriteFunctionForApplyingFakeFactors(fout,pt_bins_fit,pt_bins,proc='wjets_mc',aiso=False)

print "TTBAR MC FF"
print WriteFunctionForApplyingFakeFactors(fout,pt_bins_fit,pt_bins,proc='ttbar_mc',aiso=False)


### Closure Corrections #####


fout.Close()
