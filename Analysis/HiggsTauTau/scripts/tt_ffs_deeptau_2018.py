import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import re
import os
import time
import json
from array import array
import argparse

ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)

parser = argparse.ArgumentParser()
parser.add_argument('--wp',help= 'Tau ID working point to measure fake factors for', default='tight')
parser.add_argument('--file_ext',help= 'Extension of files names', default='_tt_2018.root')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='mvadm_ff_deeptau_2018')
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_2018_new.json')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default='/vols/cms/dw515/Offline/output/SM/new_2018/')
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
args = parser.parse_args()

wp = args.wp 
file_ext = args.file_ext
output_folder=args.output_folder
params_file=args.params
input_folder=args.input_folder
draw= args.draw > 0
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

def DrawHistsForFractions(var_input, cuts, name, input_folder, file_ext):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_1==6 || gen_match_2==6)'

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
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s*)' % vars(),'goff')
    h = t.GetHistogram()
    data.Add(h)

  # draw wjets when one of tau candidates is a jet fake - this includes VV and DY events also
  for i in wjets_files+other_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)' % vars(),'goff')
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
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s*((%(gen_extra)s)==0)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)

  # subtract other processes to get QCD
  data.Add(bkgs,-1); data.Add(wjets,-1); data.Add(ttbar,-1)

  # fix any negative bins to 0 
  for i in range(1,data.GetNbinsX()+1): 
    if data.GetBinContent() < 0: qcd.SetBinContent(0)
    if ttbar.GetBinContent() < 0: ttbar.SetBinContent(0)
    if wjets.GetBinContent() < 0: wjets.SetBinContent(0)
 
  return (data, wjets, ttbar)


def CalculateFakeFactors(num,denum):
  name = num.GetName().replace('_iso','_ff')
  ff = num.Clone()
  ff.SetName(name)
  ff.Divide(denum)
  return ff

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
    fitresult = h.Fit("f2",'SI')
    rep = int(fitresult) != 0
    if not rep or count>100: 
      ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
      fit = f2
      break
    count+=1
  fit.SetName(h.GetName()+'_fit')
  return fit, h_uncert

def PlotFakeFactor(f, h, name, output_folder, wp):
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
  c1.Print(output_folder+'/tt_'+wp+'_'+name+'_fit.pdf')
  time.sleep(2)

draw_list=[]

# tt plots
baseline_bothiso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
baseline_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()
baseline_aiso2 = 'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsJets_%(wp)s_1>0.5 && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()

var1='pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200]'
var2='pt_2[40,45,50,55,60,65,70,80,90,100,120,140]'

ff_list = {}
to_write = []

for njetbin in njets_bins:
  for dmbin in dm_bins:
    name= dmbin+'_'+njetbin
    cuts=njets_bins[njetbin]+'&&'+dm_bins[dmbin]
    cut_iso = 'wt*(%(baseline_bothiso)s)*(%(cuts)s)' % vars()
    cut_aiso1 = re.sub('X', '1','wt*(%(baseline_aiso1)s)*(%(cuts)s)' % vars())
    cut_aiso2 = re.sub('X', '2','wt*(%(baseline_aiso2)s)*(%(cuts)s)' % vars())
    ff_list[name+'_pt_1'] = (var1, cut_iso, cut_aiso1)
    ff_list[name+'_pt_2'] = (var2, cut_iso, cut_aiso2)

for ff in ff_list:
  wjets_ff=None
  ttbar_ff=None
  if draw:
    (qcd_iso, wjets_iso, ttbar_iso) = DrawHists(ff_list[ff][0], ff_list[ff][1], ff+'_iso',input_folder,file_ext)
    (qcd_aiso, wjets_aiso, ttbar_aiso) = DrawHists(ff_list[ff][0], ff_list[ff][2], ff+'_aiso',input_folder,file_ext)

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
      print ff+'_ff_wjets_mc'
      wjets_ff = fin.Get(ff+'_ff_wjets_mc')
      wjets_ff.SetDirectory(0)
      ttbar_ff = fin.Get(ff+'_ff_ttbar_mc')
      ttbar_ff.SetDirectory(0)
      to_write.append(wjets_ff)
      to_write.append(ttbar_ff)
    fin.Close()

  # do fitting
  (qcd_fit, qcd_uncert) = FitFakeFactors(qcd_ff)
  to_write.append(qcd_fit)
  to_write.append(qcd_uncert)
  PlotFakeFactor(qcd_ff, qcd_uncert, qcd_ff.GetName(), output_folder, wp)

  if wjets_ff:
    (wjets_fit, wjets_uncert) = FitFakeFactors(wjets_ff)
    to_write.append(wjets_fit)
    to_write.append(wjets_uncert)
    PlotFakeFactor(wjets_ff, wjets_uncert, wjets_ff.GetName(), output_folder, wp)
  if ttbar_ff:
    (ttbar_fit, ttbar_uncert) = FitFakeFactors(ttbar_ff)
    to_write.append(ttbar_fit)
    to_write.append(ttbar_uncert)
    PlotFakeFactor(ttbar_ff, ttbar_uncert, ttbar_ff.GetName(), output_folder, wp)


## fractions
#

baseline_eitheraiso = 'deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvvloose_2>0.5 && (deepTauVsJets_%(wp)s_1<0.5 || deepTauVsJets_%(wp)s_2<0.5) && deepTauVsEle_vvvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau && tau_decay_mode_1!=5 && tau_decay_mode_1!=6 && tau_decay_mode_2!=5 && tau_decay_mode_2!=6' % vars()

#
#cmd_0jet='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="m_vis(15,0,300)"  --ratio --cat=inclusive --datacard=njets0 --set_alias="inclusive:(n_jets==0)" --set_alias="baseline:(%(anti_iso_either)s)" --outputfolder=%(output_folder)s ' % vars()
#cmd_1jet='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="m_vis(15,0,300)"  --ratio --cat=inclusive --datacard=njets1 --set_alias="inclusive:(n_jets==1)" --set_alias="baseline:(%(anti_iso_either)s)" --outputfolder=%(output_folder)s --outputfolder=%(output_folder)s ' % vars()
#cmd_2jet='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="m_vis(15,0,300)"  --ratio --cat=inclusive --datacard=njets2 --set_alias="inclusive:(n_jets>=2)" --set_alias="baseline:(%(anti_iso_either)s)" --outputfolder=%(output_folder)s ' % vars()
#
#os.system(cmd_0jet)
#os.system(cmd_1jet)
#os.system(cmd_2jet)
#
#
#for njets in [0,1,2]:
#  f0 = ROOT.TFile('%(output_folder)s/datacard_m_vis_njets%(njets)i_tt_2018.root' % vars())
# 
#  w = f0.Get('tt_njets%i/W' % njets)
#  vvj = f0.Get('tt_njets%i/VVJ' % njets)
#  zj = f0.Get('tt_njets%i/ZJ' % njets)
#  ttj = f0.Get('tt_njets%i/TTJ' % njets)
#  ztt = f0.Get('tt_njets%i/ZTT' % njets)
#  zl = f0.Get('tt_njets%i/ZL' % njets)
#  ttt = f0.Get('tt_njets%i/TTT' % njets) 
#  vvt = f0.Get('tt_njets%i/VVT' % njets)
#  qcd = f0.Get('tt_njets%i/data_obs' % njets)
#
#  real = ztt
#  real.Add(vvt)
#  real.Add(ttt)
#  real.Add(zl)
#  w.Add(zj)
#  w.Add(vvj)
#
#
#  qcd.Add(w,-1)
#  qcd.Add(ttj,-1)
#  qcd.Add(real,-1)
#
#  total_fake = qcd.Clone()
#  total_fake.Add(w)
#  total_fake.Add(ttj)
#
#  for i in range(1,total_fake.GetNbinsX()+2):
#    tot = total_fake.GetBinContent(i)
#    if tot==0: 
#      qcd.SetBinContent(i,1)
#      w.SetBinContent(i,0)
#      ttj.SetBinContent(i,0)
#    else:
#      qcd.SetBinContent(i,qcd.GetBinContent(i)/tot)
#      w.SetBinContent(i,w.GetBinContent(i)/tot)
#      ttj.SetBinContent(i,ttj.GetBinContent(i)/tot)
#
#  fout.cd()
#
#  qcd.Write('ff_fracs_qcd_njets%i' % njets)
#  w.Write('ff_fracs_w_njets%i' % njets)
#  ttj.Write('ff_fracs_ttj_njets%i' % njets)
#
## same sign fractions
#
#cmd_0jet='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="m_vis(15,0,300)"  --ratio --cat=inclusive --datacard=njets0_ss --set_alias="inclusive:(n_jets==0)" --set_alias="baseline:(%(anti_iso_either)s)" --outputfolder=%(output_folder)s --do_ss ' % vars()
#cmd_1jet='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="m_vis(15,0,300)"  --ratio --cat=inclusive --datacard=njets1_ss --set_alias="inclusive:(n_jets==1)" --set_alias="baseline:(%(anti_iso_either)s)" --outputfolder=%(output_folder)s --outputfolder=%(output_folder)s --do_ss ' % vars()
#cmd_2jet='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="m_vis(15,0,300)"  --ratio --cat=inclusive --datacard=njets2_ss --set_alias="inclusive:(n_jets>=2)" --set_alias="baseline:(%(anti_iso_either)s)" --outputfolder=%(output_folder)s --do_ss ' % vars()
#
#os.system(cmd_0jet)
#os.system(cmd_1jet)
#os.system(cmd_2jet)
#
#for njets in [0,1,2]:
#  f0 = ROOT.TFile('%(output_folder)s/datacard_m_vis_njets%(njets)i_ss_tt_2018.root' % vars())
#
#  w = f0.Get('tt_njets%i_ss/W' % njets)
#  vvj = f0.Get('tt_njets%i_ss/VVJ' % njets)
#  zj = f0.Get('tt_njets%i_ss/ZJ' % njets)
#  ttj = f0.Get('tt_njets%i_ss/TTJ' % njets)
#  ztt = f0.Get('tt_njets%i_ss/ZTT' % njets)
#  zl = f0.Get('tt_njets%i_ss/ZL' % njets)
#  ttt = f0.Get('tt_njets%i_ss/TTT' % njets)
#  vvt = f0.Get('tt_njets%i_ss/VVT' % njets)
#  qcd = f0.Get('tt_njets%i_ss/data_obs' % njets)
#
#  real = ztt
#  real.Add(vvt)
#  real.Add(ttt)
#  real.Add(zl)
#  w.Add(zj)
#  w.Add(vvj)
#
#
#  qcd.Add(w,-1)
#  qcd.Add(ttj,-1)
#  qcd.Add(real,-1)
#
#  total_fake = qcd.Clone()
#  total_fake.Add(w)
#  total_fake.Add(ttj)
#
#  for i in range(1,total_fake.GetNbinsX()+2):
#    tot = total_fake.GetBinContent(i)
#    if tot==0:
#      qcd.SetBinContent(i,1)
#      w.SetBinContent(i,0)
#      ttj.SetBinContent(i,0)
#    else:
#      qcd.SetBinContent(i,qcd.GetBinContent(i)/tot)
#      w.SetBinContent(i,w.GetBinContent(i)/tot)
#      ttj.SetBinContent(i,ttj.GetBinContent(i)/tot)
#
#  fout.cd()
#
#  qcd.Write('ff_fracs_qcd_njets%i_ss' % njets)
#  w.Write('ff_fracs_w_njets%i_ss' % njets)
#  ttj.Write('ff_fracs_ttj_njets%i_ss' % njets


# write everything to the output file
fout = ROOT.TFile(out_file, 'RECREATE')
for i in to_write: i.Write()
fout.Close()
