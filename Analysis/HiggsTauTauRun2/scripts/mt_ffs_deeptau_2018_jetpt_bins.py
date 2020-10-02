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
parser.add_argument('--file_ext',help= 'Extension of files names', default='_mt_2018.root')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='./')
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_2018.json')
parser.add_argument('--var_to_plot',help= 'Variable to plot', default='mt_tot')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default='/vols/cms/gu18/Offline/output/SUSY/Jan24_2018_SUSY/')
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
parser.add_argument('--only_closures', help= 'Draw closures only from ff function string', default=False)
args = parser.parse_args()

wp = args.wp
file_ext = args.file_ext
output_folder=args.output_folder
params_file=args.params
input_folder=args.input_folder
only_closures = args.only_closures
var_to_plot = args.var_to_plot
draw = int(args.draw) > 0
lumi=58826.8469

out_file = '%(output_folder)s/fakefactor_fits_mt_%(wp)s_2018.root' % vars()

# read params from json

with open(params_file) as jsonfile:
  params = json.load(jsonfile)

data_files = [
  'SingleMuonA',
  'SingleMuonB',
  'SingleMuonC',
  'SingleMuonD'
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
              'njets0':'((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)',
              'njets1':'(n_lowpt_jets>0&&fabs(jeta_1)<2.4)',
}

jetpt_bins = {
              'inclusive': '(1)',
              'low':'(jet_pt_2<1.25*pt_2)',
              'medium':'((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))',
              'high':'(jet_pt_2>=1.6*pt_2)',
}


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
  c1.Print(output_folder+'/mt_'+wp+'_'+name+'_fit.pdf')
  #time.sleep(2)

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
  c1.Print(output_folder+'/mt_'+wp+'_'+name+'_fit.pdf')


def WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',aiso=False):
  # this function loops over all njets and jetpt bins and write the FFs as a function
  ff_eqn_tot = ''
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin_name,njetbin_cut in njets_bins.items():
    for jetptbin_name,jetptbin_cut in jetpt_bins.items():
      if not (jetptbin_name == 'inclusive' or njetbin_name == 'inclusive'):
        fout.cd()
        extra=''
        if aiso: extra = '_aiso2_ss'
        f = fout.Get('%(jetptbin_name)s_%(njetbin_name)s%(extra)s_pt_2_ff_%(proc)s_fit' % vars())
        p = f.GetParameters()
        if f.GetNpar()==1:
          ff_params = ff_pol0.replace('p0','%f' % p[0])
        elif f.GetNpar()==2:
          ff_params = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
        elif f.GetNpar() > 4:
          ff_params = ff_eqn_alt.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
        else:
          ff_params = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])
          
        ff_eqn_tot += '(%(njetbin_cut)s*%(jetptbin_cut)s*(%(ff_params)s))+' % vars()
        
  ff_eqn_tot = ff_eqn_tot[:-1]
  ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  return ff_eqn_tot

def PlotDistributionComparison(var,x_label,dist_1,dist_1_name,dist_2,dist_2_name,act_dist,output_folder,save_name):
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(act_dist)

  dist_2_ratio = dist_2.Clone()
  dist_2_ratio.Divide(act_dist)

  c = ROOT.TCanvas('c','c',1400,700)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  pad1.Draw()
  pad1.cd()

  dist_1.Draw()
  dist_1.SetStats(0)
  dist_1.SetMarkerColor(2)
  dist_1.SetLineColor(2)
  dist_1.SetMarkerStyle(5)
  dist_1.GetXaxis().SetTitle(x_label)
  dist_1.GetYaxis().SetTitle('Events')
  dist_1.GetYaxis().SetTitleOffset(0.6)
  dist_1.GetXaxis().SetLabelSize(0)

  act_dist.Draw("SAME")
  act_dist.SetMarkerColor(1)
  act_dist.SetLineColor(1)
  act_dist.SetMarkerStyle(4)

  dist_2.Draw("SAME")
  dist_2.SetMarkerColor(4)
  dist_2.SetLineColor(4)
  dist_2.SetMarkerStyle(2)

  l = ROOT.TLegend(0.5,0.75,0.9,0.9);
  l.AddEntry(dist_1,dist_1_name,"p")
  l.AddEntry(dist_2,dist_2_name,"p")
  l.AddEntry(act_dist,"Actual Distribution","p");
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  pad2.Draw()
  pad2.cd()

  dist_1_ratio.SetAxisRange(0.6,1.4,'Y');
  dist_1_ratio.Draw()
  dist_1_ratio.SetStats(0)
  dist_1_ratio.SetMarkerColor(2)
  dist_1_ratio.SetLineColor(2)
  dist_1_ratio.SetMarkerStyle(5)
  dist_1_ratio.GetXaxis().SetLabelSize(0.08)
  dist_1_ratio.GetYaxis().SetLabelSize(0.08)
  dist_1_ratio.GetXaxis().SetTitle(x_label)
  dist_1_ratio.GetYaxis().SetTitle("Ratio")
  dist_1_ratio.GetYaxis().SetTitleSize(0.1)
  dist_1_ratio.GetYaxis().SetTitleOffset(0.2)
  dist_1_ratio.GetXaxis().SetTitleSize(0.1)
  dist_1_ratio.GetXaxis().SetTitleOffset(0.8)

  dist_2_ratio.Draw("SAME")
  dist_2_ratio.SetMarkerColor(4)
  dist_2_ratio.SetLineColor(4)
  dist_2_ratio.SetMarkerStyle(2)

  line = ROOT.TLine(min(bins),1,max(bins),1)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw("SAME")

  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()
  

def DrawDistributionComparisons(var,xlabel,baseline_aiso1,baseline_bothiso,baseline_aiso2_aiso1,baseline_aiso2_iso,jetpt_qcd,jetpt_qcd_aiso,jetpt_wjets,jetpt_wjets_mc,jetpt_ttbar_mc,doAR=True,doDR=True,doW=True,doQCD=True,doTT=True):

  njets_qcd = '(n_jets==0)*(0.314347*TMath::Landau(pt_2,46.699016,12.132896)+0.061950) + (n_jets==1)*(0.077204*TMath::Landau(pt_2,45.769425,12.940029)+0.086458) + (n_jets>1)*(0.520224*TMath::Landau(pt_2,256.454857,230.252681)+0.000001)'
  njets_qcd_aiso = '(n_jets==0)*(0.804763*TMath::Landau(pt_2,66.850246,31.750701)+-0.023313) + (n_jets==1)*(7.588977*TMath::Landau(pt_2,105.902273,212.812360)+-1.276283) + (n_jets>1)*(0.454884*TMath::Landau(pt_2,114.674679,112.377438)+-0.000003)'
  njets_wjets = '(n_jets==0)*(0.788945*TMath::Landau(pt_2,24.554684,100.802156)+0.000006) + (n_jets==1)*(0.184461*TMath::Landau(pt_2,42.815839,22.677562)+0.086080) + (n_jets>1)*(0.402916*TMath::Landau(pt_2,28.523134,6.887628)+0.027908)'
  njets_wjets_mc = '(n_jets==0)*(0.821322*TMath::Landau(pt_2,810.261195,740.962106)+0.012498) + (n_jets==1)*(0.719553*TMath::Landau(pt_2,97.404285,73.158490)+0.000000) + (n_jets>1)*(0.079655*TMath::Landau(pt_2,110.329192,26.793263)+0.092029)'
  njets_ttbar_mc = '(n_jets==0)*(5.169081*TMath::Landau(pt_2,335.106696,351.782114)+-0.777435) + (n_jets==1)*(0.360254*TMath::Landau(pt_2,94.284400,48.252774)+0.029325) + (n_jets>1)*(1.191239*TMath::Landau(pt_2,138.822952,144.428177)+-0.117314)'
  
  jetpt_string = 'F_{F} Method Using n_{jets}-like and Jet-p_{T} Binning and p_{T} Fitting'
  njet_string = 'F_{F} Method Using n_{jets} Binning and p_{T} Fitting'
  
  if doDR:
    if doW:
      (_,wjets_pred_jetpt_dr,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_wjets,doW=True,doQCD=False,doMC=False,doTT=False)
      (_,wjets_pred_njets_dr,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_wjets,doW=True,doQCD=False,doMC=False,doTT=False)
      (_,wjets_act_dr       ,_,_) = DrawHists(var, baseline_bothiso, 'wjets_act_dr' % vars(),input_folder,file_ext,doW=True,doQCD=False,doMC=False,doTT=False)
      PlotDistributionComparison(var,xlabel,wjets_pred_jetpt_dr,jetpt_string,wjets_pred_njets_dr,njet_string,wjets_act_dr,output_folder,'wjets_dr')

      (_,_,wjets_mc_pred_jetpt_dr,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_wjets_mc,doMC=True,doW=False,doQCD=False,doTT=False)
      (_,_,wjets_mc_pred_njets_dr,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_wjets_mc,doMC=True,doW=False,doQCD=False,doTT=False)
      (_,_,wjets_mc_act_dr       ,_) = DrawHists(var, baseline_bothiso, 'wjets_mc_act_dr' % vars(),input_folder,file_ext,doMC=True,doW=False,doQCD=False,doTT=False)
      PlotDistributionComparison(var,xlabel,wjets_mc_pred_jetpt_dr,jetpt_string,wjets_mc_pred_njets_dr,njet_string,wjets_mc_act_dr,output_folder,'wjets_mc_dr')
    if doQCD:
      (qcd_pred_jetpt_dr,_,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'qcd_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_qcd,doQCD=True,doMC=False,doW=False,doTT=False)
      (qcd_pred_njets_dr,_,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'qcd_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_qcd,doQCD=True,doMC=False,doW=False,doTT=False)
      (qcd_act_dr       ,_,_,_) = DrawHists(var, baseline_bothiso, 'qcd_act_dr' % vars(),input_folder,file_ext,doQCD=True,doMC=False,doW=False,doTT=False)
      PlotDistributionComparison(var,xlabel,qcd_pred_jetpt_dr,jetpt_string,qcd_pred_njets_dr,njet_string,qcd_act_dr,output_folder,'qcd_dr')

      (qcd_aiso_pred_jetpt_dr,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_qcd_aiso,doQCD=True,doMC=False,doW=False,doTT=False)
      (qcd_aiso_pred_njets_dr,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_qcd_aiso,doQCD=True,doMC=False,doW=False,doTT=False)
      (qcd_aiso_act_dr       ,_,_,_) = DrawHists(var, baseline_aiso2_iso, 'qcd_aiso_act_dr' % vars(),input_folder,file_ext,doQCD=True,doMC=False,doW=False,doTT=False)
      PlotDistributionComparison(var,xlabel,qcd_aiso_pred_jetpt_dr,jetpt_string,qcd_aiso_pred_njets_dr,njet_string,qcd_aiso_act_dr,output_folder,'qcd_aiso_dr')
      
  if doAR:
    if doW:
      (_,_,wjets_mc_pred_jetpt_ar,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_jetpt_ar' % vars(),input_folder,file_ext,add_wt=jetpt_wjets_mc,doMC=True,lowMT=True,doQCD=False,doW=False,doTT=False)
      (_,_,wjets_mc_pred_njets_ar,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_njets_ar' % vars(),input_folder,file_ext,add_wt=njets_wjets_mc,doMC=True,lowMT=True,doQCD=False,doW=False,doTT=False)
      (_,_,wjets_mc_act_ar       ,_) = DrawHists(var, baseline_bothiso, 'wjets_mc_act_ar' % vars(),input_folder,file_ext,doMC=True,lowMT=True,doQCD=False,doW=False,doTT=False)
      PlotDistributionComparison(var,xlabel,wjets_mc_pred_jetpt_ar,jetpt_string,wjets_mc_pred_njets_ar,njet_string,wjets_mc_act_ar,output_folder,'wjets_mc_ar')
    if doQCD:
      (qcd_aiso_pred_jetpt_ar,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_jetpt_ar' % vars(),input_folder,file_ext,add_wt=jetpt_qcd_aiso,doQCD=True,doOS=True,doW=False,doMC=False,doTT=False)
      (qcd_aiso_pred_njets_ar,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_njets_ar' % vars(),input_folder,file_ext,add_wt=njets_qcd_aiso,doQCD=True,doOS=True,doW=False,doMC=False,doTT=False)
      (qcd_aiso_act_ar       ,_,_,_) = DrawHists(var, baseline_aiso2_iso, 'qcd_aiso_act_ar' % vars(),input_folder,file_ext,doQCD=True,doOS=True,doW=False,doMC=False,doTT=False)
      PlotDistributionComparison(var,xlabel,qcd_aiso_pred_jetpt_ar,jetpt_string,qcd_aiso_pred_njets_ar,njet_string,qcd_aiso_act_ar,output_folder,'qcd_aiso_ar')
      
  if doTT:
    (_,_,_,ttbar_mc_pred_jetpt) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'ttbar_mc_pred_jetpt' % vars(),input_folder,file_ext,add_wt=jetpt_ttbar_mc,doTT=True,doQCD=False,doW=False,doMC=True)
    (_,_,_,ttbar_mc_pred_njets) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'ttbar_mc_pred_njets' % vars(),input_folder,file_ext,add_wt=njets_ttbar_mc,doTT=True,doQCD=False,doW=False,doMC=True)
    (_,_,_,ttbar_mc_act       ) = DrawHists(var, baseline_bothiso, 'ttbar_mc_act' % vars(),input_folder,file_ext,doTT=True,doQCD=False,doW=False,doMC=True)
    PlotDistributionComparison(var,xlabel,ttbar_mc_pred_jetpt,jetpt_string,ttbar_mc_pred_njets,njet_string,ttbar_mc_act,output_folder,'ttbar_mc')


draw_list=[]

# mt plots
baseline_bothiso = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
baseline_aiso1 = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()

var1='pt_2[20,25,30,35,40,45,50,55,60,70,80,100,140]'
var2='pt_2[20,30,40,50,60,80]'


ff_list = {}
to_write = []

for njetbin in njets_bins:
  for jetptbin in jetpt_bins:
    name= jetptbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+jetpt_bins[jetptbin]
    cut_iso1 = re.sub('X', '2', '(%(baseline_bothiso)s)*(%(cuts)s)' % vars())
    cut_aiso1 = re.sub('X', '2','(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
    if 'crosstrg' in name: ff_list[name+'_pt_2'] = (var2, cut_iso1, cut_aiso1)
    else: ff_list[name+'_pt_2'] = (var1, cut_iso1, cut_aiso1)


# add aiso plots

baseline_aiso2_iso = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
baseline_aiso2_aiso1 = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()

for njetbin in njets_bins:
  for jetptbin in jetpt_bins:
    name= jetptbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+jetpt_bins[jetptbin]
    cut_iso1 = re.sub('X', '2', '(%(baseline_aiso2_iso)s)*(%(cuts)s)' % vars())
    cut_aiso1 = re.sub('X', '2','(%(baseline_aiso2_aiso1)s)*(%(cuts)s)' % vars())
    if 'crosstrg' in name: ff_list[name+'_aiso2_ss_pt_2'] = (var2, cut_iso1, cut_aiso1)
    else: ff_list[name+'_aiso2_ss_pt_2'] = (var1, cut_iso1, cut_aiso1)

for ff in ff_list:
  wjets_mc_ff=None
  ttbar_mc_ff=None
  if draw and not only_closures:
    (qcd_iso, wjets_iso, wjets_mc_iso, ttbar_mc_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))
    (qcd_aiso, wjets_aiso, wjets_mc_aiso, ttbar_mc_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff),doQCD=True,doW=(not 'aiso2' in ff),doMC=(not 'aiso2' in ff))

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
  if 'crosstrg' in ff: usePol=1

  usePolQCD = usePol
  usePolW = usePol
  usePolTT = usePol

  # do fitting
  if not only_closures:
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
 
#njets_cuts = ['n_jets==0','n_jets==1','n_jets>=2']

#for njets in [0,1,2]:
#  njets_cut = njets_cuts[njets]
#  var= 'mt_1[0,10,20,30,40,50,60,70,80,90,100,120]'
#  cuts = '(%(baseline_aiso1)s)*(%(njets_cut)s)' % vars()
#  name = 'mt_fracs_njets%(njets)s' % vars()
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

# write everything to the output file
if not only_closures:
  fout = ROOT.TFile(out_file, 'RECREATE')
  for i in to_write: i.Write()

  jetpt_qcd = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',aiso=False)
  jetpt_qcd_aiso = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='qcd',aiso=True)
  jetpt_wjets = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='wjets',aiso=False)
  jetpt_wjets_mc = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='wjets_mc',aiso=False)
  jetpt_ttbar_mc = WriteFakeFactorFunction(fout,njets_bins,jetpt_bins,proc='ttbar_mc',aiso=False)
else:
  jetpt_qcd = '(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2>=1.6*pt_2)*(0.063478*TMath::Landau(pt_2,32.683483,6.225066)+0.029216))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(0.302339*TMath::Landau(pt_2,29.344503,9.975768)+0.068037))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2<1.25*pt_2)*(0.632684*TMath::Landau(pt_2,37.063674,8.577969)+0.160853))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2>=1.6*pt_2)*(140.734560*TMath::Landau(pt_2,312.354019,1159.427407)+-25.387771))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(88.208352*TMath::Landau(pt_2,-15.269539,0.876855)+0.073106))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2<1.25*pt_2)*(6.345729*TMath::Landau(pt_2,-109.015924,21.307761)+0.089926))' 
  jetpt_qcd_aiso = '(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2>=1.6*pt_2)*(0.248129*TMath::Landau(pt_2,46.627659,25.264572)+-0.006042))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(1.611464*TMath::Landau(pt_2,-275.040959,93.882250)+0.000001))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2<1.25*pt_2)*(0.688626*TMath::Landau(pt_2,34.952798,9.977941)+0.153191))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2>=1.6*pt_2)*(0.481543*TMath::Landau(pt_2,-83.506053,23.113688)+0.012093))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(2.941613*TMath::Landau(pt_2,-5.203319,2.972576)+0.077272))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2<1.25*pt_2)*(0.580519*TMath::Landau(pt_2,32.420871,9.216656)+0.152861))'
  jetpt_wjets = '(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2>=1.6*pt_2)*(0.880862*TMath::Landau(pt_2,-56.405282,72.369890)+-0.066399))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(1.454207*TMath::Landau(pt_2,-3.982308,6.759866)+0.084528))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2<1.25*pt_2)*(31.290730*TMath::Landau(pt_2,-106.281911,11.500190)+0.050104))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2>=1.6*pt_2)*(2.826269*TMath::Landau(pt_2,-22.267586,3.864132)+0.024318))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(13.963525*TMath::Landau(pt_2,-31.058443,4.063350)+0.054912))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2<1.25*pt_2)*(1.118834*TMath::Landau(pt_2,27.980704,7.700504)+0.097890))'
  jetpt_wjets_mc = '(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2>=1.6*pt_2)*(0.556352*TMath::Landau(pt_2,-98.957416,51.370185)+0.000002))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(8.854800*TMath::Landau(pt_2,-1.816146,1.787908)+0.091413))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2<1.25*pt_2)*(16.002292*TMath::Landau(pt_2,-18.536415,3.726180)+0.190846))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2>=1.6*pt_2)*(18.747000*TMath::Landau(pt_2,-4379.143550,3577.058207)+-2.496043))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(4.474868*TMath::Landau(pt_2,-3.254466,2.483118)+0.076736))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2<1.25*pt_2)*(21.044820*TMath::Landau(pt_2,-62.219866,7.446410)+0.114955))'
  jetpt_ttbar_mc = '(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2>=1.6*pt_2)*(0.315336*TMath::Landau(pt_2,9.556944,2.966951)+0.021339))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(3.728516*TMath::Landau(pt_2,-10.691586,3.523235)+0.058748))+(((n_lowpt_jets>0&&fabs(jeta_1)<2.4)==0)*(jet_pt_2<1.25*pt_2)*(0.578189*TMath::Landau(pt_2,26.750001,3.176015)+0.177640))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2>=1.6*pt_2)*(4.648823*TMath::Landau(pt_2,-51.391879,2.750762)+0.021446))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(0.973938*TMath::Landau(pt_2,-3.061339,4.849313)+0.063293))+((n_lowpt_jets>0&&fabs(jeta_1)<2.4)*(jet_pt_2<1.25*pt_2)*(5.984535*TMath::Landau(pt_2,-52.212071,9.991455)+0.129559))'


plotted_vars = {'mt_tot[0,30,45,60,70,80,90,100,110,120,130,140,150,160,180,220,320,600]':'m_{T}^{tot}/GeV',
                'm_vis[0,30,45,60,70,80,90,100,110,120,130,140,150,160,180,220,320,600]':'m_{vis}/GeV',
                'mt_1[0,10,20,30,40,50,60,70,80]':'m_{T}^{#mu}/GeV',
                'mt_2[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150]':'m_{T}^{#tau_{h}}/GeV',
                'pt_1[20,40,60,80,100,120,160,200,300]':'p_{T}^{#mu}/GeV',
                'pt_2[20,40,60,80,100,120,160,200,300]':'p_{T}^{#tau_{h}}/GeV',
                'n_jets[0,1,2,3,4]':'n_{jets}',
                'n_bjets[0,1,5]':'n_{b-jets}',
                'met[0,30,45,60,80,100,120,160,200,300]': 'MET/GeV'}

# Get variables to plot
for var,_ in plotted_vars.items():
  if var_to_plot in var:
    var_to_plot_dict = {var:plotted_vars[var]}
  elif var_to_plot == 'all':
    var_to_plot_dict = plotted_vars


for var,xlabel in var_to_plot_dict.items():
  DrawDistributionComparisons(var,xlabel,baseline_aiso1,baseline_bothiso,baseline_aiso2_aiso1,baseline_aiso2_iso,jetpt_qcd,jetpt_qcd_aiso,jetpt_wjets,jetpt_wjets_mc,jetpt_ttbar_mc,doAR=True,doDR=True,doW=True,doQCD=True,doTT=True)

fout.Close()
