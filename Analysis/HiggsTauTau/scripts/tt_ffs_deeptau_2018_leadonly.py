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
parser.add_argument('--file_ext',help= 'Extension of files names', default='_tt_2018.root')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='mvadm_ff_deeptauV2p1_2018')
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


njets_bins = { 
              'inclusive': '(1)',
              'njets0':'n_jets==0',
              'njets1':'n_jets==1',
              'njets2':'n_jets>1'
}
dm_bins = {
              'inclusive': '(1)',
              'dm0':'(tau_decay_mode_X==0)',
              'dm1':'(tau_decay_mode_X==1)',
              'dm10':'(tau_decay_mode_X==10)',
              'dm11':'(tau_decay_mode_X==11)',
              'mvadm0':'(mva_dm_X==0)',
              'mvadm0_sig_gt3':'(mva_dm_X==0&&ip_sig_1>=1)',
              'mvadm0_sig_lt3':'(mva_dm_X==0&&ip_sig_1<1)',
              'mvadm1':'(mva_dm_X==1)',
              'mvadm2':'(mva_dm_X==2)',
              'mvadm10':'(mva_dm_X==10)',
              'mvadm11':'(mva_dm_X==11)'
}

fit_pol1 = []

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

def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1'):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_1==6)'

  data = hout.Clone()
  data.SetName(name+'_qcd')
  wjets = hout.Clone()
  wjets.SetName(name+'_wjets_mc')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar_mc')
  bkgs = hout.Clone()
  bkgs.SetName(name+'_bkgs')


  # draw data
  for i in data_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple') 
    h = hout.Clone()
    h.SetName('h')
    if not doOS: 
      t.Draw('%(var)s>>h' % vars(),'((%(cuts)s)*((os==0)))*(%(add_wt)s)' % vars(),'goff')
    else: 
      t.Draw('%(var)s>>h' % vars(),'((%(cuts)s)*((os==1)))*(%(add_wt)s)' % vars(),'goff')

    h = t.GetHistogram()
    data.Add(h)

  # draw wjets when tau candidate is a jet fake
  for i in wjets_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
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
    if not doOS: 
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(add_wt)s)' % vars(),'goff')
    else: 
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(add_wt)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)

  data.Add(bkgs,-1)
 
  return (data, wjets, ttbar)

def DrawHistsForFractions(var_input, cuts, name, input_folder, file_ext):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_1==6 || gen_match_2==6)'
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

  # draw wjets when one of tau candidates is a jet fake - this includes VV and DY events also
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

  # draw ttbar when one of tau candidates is a jet fake 
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

  # draw all backgrounds with no jet fakes
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
    if data.GetBinContent(i) < 0: qcd.SetBinContent(i,0.)
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
  f1 = ROOT.TF1("f1","landau",40,200)
  f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",40,200)
  if usePol1: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",40,200)
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
    if content>0:
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
  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')
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
  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')


def WriteFunctionMVADM2Jets(fout, subtau=False, aiso=False):
  # this function loops over all njets and dm bins and write the FFs as a function
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin in [0,1,2]:
    for mvadmbin in [0,1,2,10,11]:
      fout.cd()
      extra=''
      if aiso: extra = '_aiso2_ss'
      if subtau: f = fout.Get('mvadm%(mvadmbin)i_njets%(njetbin)i%(extra)s_pt_2_ff_qcd_fit' % vars())
      else: f = fout.Get('mvadm%(mvadmbin)i_njets%(njetbin)i%(extra)s_pt_1_ff_qcd_fit' % vars())
      p = f.GetParameters()
      if f.GetNpar()==1:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)i' % vars()] = ff_pol0.replace('p0','%f' % p[0])
      elif f.GetNpar()==2:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)i' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      elif f.GetNpar() > 4:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)i' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      else:
        ff_params['mvadm%(mvadmbin)i_njets%(njetbin)i' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((n_jets==0)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets==1)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets>1)*((mva_dm_X==0)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)))' % (ff_params['mvadm0_njets0'], ff_params['mvadm1_njets0'], ff_params['mvadm2_njets0'], ff_params['mvadm10_njets0'], ff_params['mvadm11_njets0'], ff_params['mvadm0_njets1'], ff_params['mvadm1_njets1'], ff_params['mvadm2_njets1'], ff_params['mvadm10_njets1'], ff_params['mvadm11_njets1'], ff_params['mvadm0_njets2'], ff_params['mvadm1_njets2'], ff_params['mvadm2_njets2'], ff_params['mvadm10_njets2'], ff_params['mvadm11_njets2'])

  if subtau: ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  else:      ff_eqn_tot = re.sub('X', '1', ff_eqn_tot)

  return ff_eqn_tot

def WriteFunctionMVADM2JetsIPSig(fout, subtau=False, aiso=False):
  # this function loops over all njets and dm bins and write the FFs as a function
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin in [0,1,2]:
    for mvadmbin in ['0_sig_lt3','0_sig_gt3','1','2','10','11']:
      fout.cd()
      extra=''
      if aiso: extra = '_aiso2_ss'
      if subtau: f = fout.Get('mvadm%(mvadmbin)s_njets%(njetbin)i%(extra)s_pt_2_ff_qcd_fit' % vars())
      else: f = fout.Get('mvadm%(mvadmbin)s_njets%(njetbin)i%(extra)s_pt_1_ff_qcd_fit' % vars())
      p = f.GetParameters()
      if f.GetNpar()==1:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_pol0.replace('p0','%f' % p[0])
      elif f.GetNpar()==2:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      elif f.GetNpar() > 4:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      else:
        ff_params['mvadm%(mvadmbin)s_njets%(njetbin)s' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((n_jets==0)*((mva_dm_X==0&&ip_sig_1<1)*(%s)+(mva_dm_X==0&&ip_sig_1>=1)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets==1)*((mva_dm_X==0&&ip_sig_1<1)*(%s)+(mva_dm_X==0&&ip_sig_1>=1)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)) + (n_jets>1)*((mva_dm_X==0&&ip_sig_1<1)*(%s)+(mva_dm_X==0&&ip_sig_1>=1)*(%s)+(mva_dm_X==1)*(%s)+(mva_dm_X==2)*(%s)+(mva_dm_X==10)*(%s)+(mva_dm_X==11)*(%s)))' % (ff_params['mvadm0_sig_lt3_njets0'], ff_params['mvadm0_sig_gt3_njets0'], ff_params['mvadm1_njets0'], ff_params['mvadm2_njets0'], ff_params['mvadm10_njets0'], ff_params['mvadm11_njets0'], ff_params['mvadm0_sig_lt3_njets1'], ff_params['mvadm0_sig_gt3_njets1'], ff_params['mvadm1_njets1'], ff_params['mvadm2_njets1'], ff_params['mvadm10_njets1'], ff_params['mvadm11_njets1'], ff_params['mvadm0_sig_lt3_njets2'], ff_params['mvadm0_sig_gt3_njets2'], ff_params['mvadm1_njets2'], ff_params['mvadm2_njets2'], ff_params['mvadm10_njets2'], ff_params['mvadm11_njets2'])

  if subtau: ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  else:      ff_eqn_tot = re.sub('X', '1', ff_eqn_tot)

  return ff_eqn_tot


def WriteFunctionDM2Jets(fout, subtau=False,aiso=False):
  # this function loops over all njets and dm bins and write the FFs as a function
  ff_eqn = 'p0*TMath::Landau(pt_X,p1,p2)+p3'
  ff_eqn_alt = 'p0*TMath::Landau(pt_X,p1,p2)+p3+p4*pt_X'
  ff_pol0 = 'p0'
  ff_pol1 = 'p0+p1*pt_X'
  ff_params = {}
  for njetbin in [0,1,2]:
    for dmbin in [0,1,10,11]:
      fout.cd()
      extra=''
      if aiso: extra = '_aiso2_ss'
      if subtau: f = fout.Get('dm%(dmbin)i_njets%(njetbin)i%(extra)s_pt_2_ff_qcd_fit' % vars())
      else: f = fout.Get('dm%(dmbin)i_njets%(njetbin)i%(extra)s_pt_1_ff_qcd_fit' % vars())
      p = f.GetParameters()
      if f.GetNpar()==1:
        ff_params['dm%(dmbin)i_njets%(njetbin)i' % vars()] = ff_pol0.replace('p0','%f' % p[0])
      elif f.GetNpar()==2:
        ff_params['dm%(dmbin)i_njets%(njetbin)i' % vars()] = ff_pol1.replace('p0','%f' % p[0]).replace('p1','%f' % p[1])
      elif f.GetNpar() > 4:
        ff_params['dm%(dmbin)i_njets%(njetbin)i' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3]).replace('p4','%f' % p[4])
      else:
        ff_params['dm%(dmbin)i_njets%(njetbin)i' % vars()] = ff_eqn.replace('p0','%f' % p[0]).replace('p1','%f' % p[1]).replace('p2','%f' % p[2]).replace('p3','%f' % p[3])

  ff_eqn_tot = '((n_jets==0)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)) + (n_jets==1)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)) + (n_jets>1)*((tau_decay_mode_X==0)*(%s)+(tau_decay_mode_X==1)*(%s)+(tau_decay_mode_X==10)*(%s)+(tau_decay_mode_X==11)*(%s)))' % (ff_params['dm0_njets0'], ff_params['dm1_njets0'], ff_params['dm10_njets0'], ff_params['dm11_njets0'], ff_params['dm0_njets1'], ff_params['dm1_njets1'], ff_params['dm10_njets1'], ff_params['dm11_njets1'], ff_params['dm0_njets2'], ff_params['dm1_njets2'], ff_params['dm10_njets2'], ff_params['dm11_njets2'])

  if subtau: ff_eqn_tot = re.sub('X', '2', ff_eqn_tot)
  else:      ff_eqn_tot = re.sub('X', '1', ff_eqn_tot)

  return ff_eqn_tot

draw_list=[]

# tt plots
baseline_bothiso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
baseline_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
#baseline_aiso2 = 'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsJets_%(wp)s_1>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()

var1='pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200]'
#var2='pt_2[40,45,50,55,60,65,70,80,90,100,120,140]'

ff_list = {}
to_write = []

for njetbin in njets_bins:
  for dmbin in dm_bins:
    name= dmbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+dm_bins[dmbin]
    cut_iso1 = re.sub('X', '1', '(%(baseline_bothiso)s)*(%(cuts)s)' % vars())
    cut_aiso1 = re.sub('X', '1','(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
    ff_list[name+'_pt_1'] = (var1, cut_iso1, cut_aiso1)

# use measurments in tau2 anti iso region to define uncertainty
#baseline_aiso2_iso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
#baseline_aiso2_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()

baseline_aiso2_iso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
baseline_aiso2_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()

for njetbin in njets_bins:
  for dmbin in dm_bins:
    name= dmbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+dm_bins[dmbin]
    cut_iso1 = re.sub('X', '1', '(%(baseline_aiso2_iso)s)*(%(cuts)s)' % vars())
    cut_aiso1 = re.sub('X', '1','(%(baseline_aiso2_aiso1)s)*(%(cuts)s)' % vars())
    ff_list[name+'_aiso2_ss_pt_1'] = (var1, cut_iso1, cut_aiso1)


for ff in ff_list:
  wjets_ff=None
  ttbar_ff=None
  if draw:
    (qcd_iso, wjets_iso, ttbar_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext,doOS=('aiso2_os' in ff))
    (qcd_aiso, wjets_aiso, ttbar_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext,doOS=('aiso2_os' in ff))

    qcd_ff = CalculateFakeFactors(qcd_iso, qcd_aiso)
    to_write.append(qcd_ff)

    if 'inclusive_inclusive' in ff:
      wjets_ff = CalculateFakeFactors(wjets_iso, wjets_aiso)
      ttbar_ff = CalculateFakeFactors(ttbar_iso, ttbar_aiso)
      to_write.append(wjets_ff)
      to_write.append(ttbar_ff)
  else:
    # if not drawing histogram then retrieve them from the old output folder
    fin = ROOT.TFile(out_file)
    qcd_ff = fin.Get(ff+'_ff_qcd')
    qcd_ff.SetDirectory(0)
    to_write.append(qcd_ff)
    if 'inclusive_inclusive' in ff:
      wjets_ff = fin.Get(ff+'_ff_wjets_mc')
      wjets_ff.SetDirectory(0)
      ttbar_ff = fin.Get(ff+'_ff_ttbar_mc')
      ttbar_ff.SetDirectory(0)
      to_write.append(wjets_ff)
      to_write.append(ttbar_ff)
    fin.Close()

  usePol=None
  if True in [ff.startswith(x) for x in fit_pol1]: usePol=1

  # do fitting
  (qcd_fit, qcd_uncert, qcd_ff) = FitFakeFactors(qcd_ff,polOnly=usePol)
  to_write.append(qcd_fit)
  to_write.append(qcd_uncert)
  PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp)

  if wjets_ff:
    (wjets_fit, wjets_uncert, wjets_ff) = FitFakeFactors(wjets_ff,polOnly=usePol)
    to_write.append(wjets_fit)
    to_write.append(wjets_uncert)
    PlotFakeFactor(wjets_ff, wjets_uncert, wjets_ff.GetName(), output_folder, wp)
  if ttbar_ff:
    (ttbar_fit, ttbar_uncert, ttbar_ff) = FitFakeFactors(ttbar_ff,polOnly=usePol)
    to_write.append(ttbar_fit)
    to_write.append(ttbar_uncert)
    PlotFakeFactor(ttbar_ff, ttbar_uncert, ttbar_ff.GetName(), output_folder, wp)

# make fractions

if draw or True:
  #baseline_eitheraiso = 'deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvvloose_2>0.5 && (deepTauVsJets_%(wp)s_1<0.5 || deepTauVsJets_%(wp)s_2<0.5) && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
  # fractions are currently been produced assuming only lead tau will be anti isolated 
 
  njets_cuts = ['n_jets==0','n_jets==1','n_jets>=2']
  
  for njets in [0,1,2]:
    njets_cut = njets_cuts[njets]
    var= 'm_vis[0,40,60,80,100,120,140,160,180,200,220,240,260,280,300]'
    cuts = '(%(baseline_aiso1)s)*(%(njets_cut)s)' % vars()
    name = 'tt_fracs_njets%(njets)s' % vars()
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
else:
  fout = ROOT.TFile(out_file) 
  njets_cuts = ['n_jets==0','n_jets==1','n_jets>=2']

  for njets in [0,1,2]:
    njets_cut = njets_cuts[njets]
    qcd_os = fout.Get('tt_fracs_njets%(njets)s_os_qcd' % vars())
    wjets_os = fout.Get('tt_fracs_njets%(njets)s_os_wjets' % vars())
    ttbar_os = fout.Get('tt_fracs_njets%(njets)s_os_ttbar' % vars())
    qcd_ss = fout.Get('tt_fracs_njets%(njets)s_ss_qcd' % vars())
    wjets_ss = fout.Get('tt_fracs_njets%(njets)s_ss_wjets' % vars())
    ttbar_ss = fout.Get('tt_fracs_njets%(njets)s_ss_ttbar' % vars())
    to_write.append(qcd_os); to_write.append(wjets_os); to_write.append(ttbar_os)
    to_write.append(qcd_ss); to_write.append(wjets_ss); to_write.append(ttbar_ss)

# write everything to the output file
fout = ROOT.TFile(out_file, 'RECREATE')

for i in to_write: i.Write()

# write strings needed for closure tests
tau1_dm_string = WriteFunctionDM2Jets(fout, subtau=False)
tau1_mvadm_string = WriteFunctionMVADM2Jets(fout, subtau=False)
tau1_dm_aiso_string = WriteFunctionDM2Jets(fout, subtau=False,aiso=True)
tau1_mvadm_aiso_string = WriteFunctionMVADM2Jets(fout, subtau=False,aiso=True)


tau1_mvadm_string_ipsig = WriteFunctionMVADM2JetsIPSig(fout, subtau=False)
tau1_mvadm_aiso_string_ipsig = WriteFunctionMVADM2JetsIPSig(fout, subtau=False,aiso=True)

print '\n'
print "isolated region dm binning:"
print tau1_dm_string

print '\n'
print "isolated region mva-dm binning:"
print tau1_mvadm_string

print '\n'
print "isolated region mva-dm binning:"
print tau1_mvadm_string_ipsig

print '\n'
print "anti-isolated region dm binning:"
print tau1_dm_aiso_string

print '\n'
print "anti-isolated region mva-dm binning:"
print tau1_mvadm_aiso_string

print '\n'
print "anti-isolated region mva-dm binning:"
print tau1_mvadm_aiso_string_ipsig

# derive non-closure corrections for n_jets=0 events as a function of MET
var='met[0,10,20,30,40,50,60,70,80,90,100]'
for i in ['mvadm_nosig','mvadm','dm']:
  # ss region
  (qcd_ss_data, wjets_ss_data, ttbar_ss_data) = DrawHists(var, '('+baseline_bothiso+')*(n_jets==0)', '%(i)s_met_ss_closure' % vars(),input_folder,file_ext,False)
  if i=='mvadm': (qcd_ss_pred, wjets_ss_pred_mvadm, ttbar_ss_pred_mvadm) = DrawHists(var, '('+baseline_aiso1+')*(n_jets==0)', '%(i)s_met_ss_closure_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_mvadm_string_ipsig)
  if i=='mvadm_nosig': (qcd_ss_pred, wjets_ss_pred, ttbar_ss_pred) = DrawHists(var, '('+baseline_aiso1+')*(n_jets==0)', '%(i)s_met_ss_closure_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_mvadm_string)
  if i=='dm': (qcd_ss_pred, wjets_ss_pred, ttbar_ss_pred) = DrawHists(var, '('+baseline_aiso1+')*(n_jets==0)', '%(i)s_met_ss_closure_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_dm_string)
  fout.cd()
  qcd_ss_data.Divide(qcd_ss_pred)

  # ss aiso2 region
  (qcd_ss_data_aiso2, wjets_ss_data, ttbar_ss_data) = DrawHists(var, '('+baseline_aiso2_iso+')*(n_jets==0)', '%(i)s_met_ss_aiso2_closure' % vars(),input_folder,file_ext,False)
  if i=='mvadm': (qcd_ss_pred_aiso2, wjets_ss_pred_mvadm, ttbar_ss_pred_mvadm) = DrawHists(var, '('+baseline_aiso2_aiso1+')*(n_jets==0)', '%(i)s_met_ss_aiso2_closure_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_mvadm_aiso_string_ipsig)
  if i=='mvadm_nosig': (qcd_ss_pred_aiso2, wjets_ss_pred, ttbar_ss_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*(n_jets==0)', '%(i)s_met_ss_closure_aiso2_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_mvadm_aiso_string)
  if i=='dm': (qcd_ss_pred_aiso2, wjets_ss_pred, ttbar_ss_pred) = DrawHists(var, '('+baseline_aiso2_aiso1+')*(n_jets==0)', '%(i)s_met_ss_closure_aiso2_pred' % vars(),input_folder,file_ext,False,add_wt=tau1_dm_aiso_string)
  fout.cd()
  qcd_ss_data_aiso2.Divide(qcd_ss_pred_aiso2)

  qcd_ss_data_fit, qcd_ss_data_uncert =  FitCorrection(qcd_ss_data, func='pol3')

  qcd_ss_data.Write()
  qcd_ss_data_fit.Write()
  qcd_ss_data_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data, qcd_ss_data_uncert, qcd_ss_data.GetName(), output_folder, wp)

  fout.cd()
  qcd_ss_data_aiso2_fit, qcd_ss_data_aiso2_uncert =  FitCorrection(qcd_ss_data_aiso2, func='pol3')

  qcd_ss_data_aiso2.Write()
  qcd_ss_data_aiso2_fit.Write()
  qcd_ss_data_aiso2_uncert.Write()
  PlotFakeFactorCorrection(qcd_ss_data_aiso2, qcd_ss_data_aiso2_uncert, qcd_ss_data_aiso2.GetName(), output_folder, wp)

# closure comparrison to define os/ss corrections and uncertainties
# pt_2 dependent closure correction
var='pt_2[40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140]'
baseline_aiso2_iso = '(mva_dm_1>=0&&mva_dm_2>=0)*(deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6)' % vars()
baseline_aiso2_aiso1 = '(mva_dm_1>=0&&mva_dm_2>=0)*(deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6)' % vars()

for i in ['mvadm_nosig','mvadm','dm']:
  met_corr_fit = fout.Get('%(i)s_met_ss_aiso2_closure_qcd_fit' % vars())
  p = met_corr_fit.GetParameters()
  met_corr = '*((n_jets==0)*(%f+ %f*min(met,100.) + %f*min(met,100.)*min(met,100.) + %f*min(met,100.)*min(met,100.)*min(met,100.))+(n_jets>0))' % (p[0], p[1], p[2], p[3])
  (qcd_os_data, wjets_os_data, ttbar_os_data) = DrawHists(var, baseline_aiso2_iso, '%(i)s_pt_2_os_closure' % vars(),input_folder,file_ext,True)
  if i=='mvadm': (qcd_os_pred, wjets_os_pred_mvadm, ttbar_os_pred_mvadm) = DrawHists(var, baseline_aiso2_aiso1, '%(i)s_pt_2_os_closure_pred' % vars(),input_folder,file_ext,True,add_wt=tau1_mvadm_aiso_string_ipsig+met_corr)
  if i=='mvadm_nosig': (qcd_os_pred, wjets_os_pred, ttbar_os_pred) = DrawHists(var, baseline_aiso2_aiso1, '%(i)s_pt_2_os_closure_pred' % vars(),input_folder,file_ext,True,add_wt=tau1_mvadm_aiso_string+met_corr)
  if i=='dm': (qcd_os_pred, wjets_os_pred, ttbar_os_pred) = DrawHists(var, baseline_aiso2_aiso1, '%(i)s_pt_2_os_closure_pred' % vars(),input_folder,file_ext,True,add_wt=tau1_dm_aiso_string+met_corr)
  fout.cd()
  qcd_os_data.Divide(qcd_os_pred)

  qcd_os_data_fit, qcd_os_data_uncert =  FitCorrection(qcd_os_data, func='pol3')

  qcd_os_data.Write()
  qcd_os_data_fit.Write()
  qcd_os_data_uncert.Write()
  PlotFakeFactorCorrection(qcd_os_data, qcd_os_data_uncert, qcd_os_data.GetName(), output_folder, wp)

  ## apply pt_2 closure correction then define uncertainty as 2D data/pred differences in dm variables
  p = qcd_os_data_fit.GetParameters()
  pt2_corr_str='(%f+ %f*min(pt_2,140.) + %f*min(pt_2,140.)*min(pt_2,140.) + %f*min(pt_2,140.)*min(pt_2,140.)*min(pt_2,140.))' % (p[0], p[1], p[2], p[3])
  var1 = 'mva_dm_1[0,1,2,3,10,11,12]'
  var2 = 'mva_dm_2[0,1,2,3,10,11,12]'
  qcd_os_data = Draw2DQCDHist(var1, var2, baseline_aiso2_iso, '%(i)s_os_dm_uncert' % vars(),input_folder,file_ext,True, add_wt='%(pt2_corr_str)s' % vars())
  if i=='mvadm': qcd_os_pred  = Draw2DQCDHist(var1, var2, baseline_aiso2_aiso1, '%(i)s_os_dm_uncert_pred' % vars(),input_folder,file_ext,True,add_wt='(%(tau1_mvadm_aiso_string_ipsig)s)*%(pt2_corr_str)s%(met_corr)s' % vars())
  if i=='mvadm_nosig': qcd_os_pred = Draw2DQCDHist(var1, var2, baseline_aiso2_aiso1, '%(i)s_os_dm_uncert_pred' % vars(),input_folder,file_ext,True,add_wt='(%(tau1_mvadm_aiso_string)s)*%(pt2_corr_str)s%(met_corr)s' % vars())
  if i=='dm': qcd_os_pred = Draw2DQCDHist(var1, var2, baseline_aiso2_aiso1, '%(i)s_os_dm_uncert_pred' % vars(),input_folder,file_ext,True,add_wt='(%(tau1_dm_aiso_string)s)*%(pt2_corr_str)s%(met_corr)s' % vars())
  fout.cd()
  qcd_os_data.Divide(qcd_os_pred)
  qcd_os_data.Write()
 

fout.Close()
