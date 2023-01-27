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
parser.add_argument('--ch',help= 'Channel input', default='mt')
parser.add_argument('--yr',help= 'Year input', default='2018')
parser.add_argument('--output_folder','-o', help= 'Name of output directory', default='./')
parser.add_argument('--params',help= 'Parmaters file contaaining cross sections and event numbers', default='scripts/params_2018.json')
parser.add_argument('--input_folder','-i', help= 'Name of output directory', default='/vols/cms/gu18/Offline/output/SUSY/Jan24_2018_SUSY/')
parser.add_argument('--var_to_plot', help= 'Variable to plot', default='mt_tot')
parser.add_argument('--draw','-d', help= 'Draw histograms, if >0 then histograms will be redrawn. Else the histograms will be loaded from the file named the same as the output folder', default=1)
args = parser.parse_args()

ch = args.ch
yr = args.yr
wp = args.wp
output_folder=args.output_folder
params_file=args.params
input_folder=args.input_folder
var_to_plot = args.var_to_plot
draw = int(args.draw) > 0
lumi=58826.8469

file_ext = '_%(ch)s_%(yr)s.root' % vars()


ff = '((((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))*(jet_pt_2>=1.6*pt_2)*(((pt_2<200)*(0.717281*TMath::Landau(pt_2,-66.801669,21.054607)+0.017612)) + (0.152016*(pt_2>=200))))+((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))*(jet_pt_2<1.25*pt_2)*(((pt_2<200)*(29.443407*TMath::Landau(pt_2,-43.644018,5.194610)+0.135811)) + (0.257569*(pt_2>=200))))+((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(((pt_2<200)*(14.164830*TMath::Landau(pt_2,-8.161417,1.828772)+0.085891)) + (0.113565*(pt_2>=200))))+(((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))==0)*(jet_pt_2>=1.6*pt_2)*(((pt_2<200)*(0.239768*TMath::Landau(pt_2,28.263996,23.144426)+0.000000)) + (0.005327*(pt_2>=200))))+(((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))==0)*(jet_pt_2<1.25*pt_2)*(((pt_2<200)*(133.035447*TMath::Landau(pt_2,-251.616847,11.860534)+0.000000)) + (0.251395*(pt_2>=200))))+(((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))==0)*((jet_pt_2>=1.25*pt_2) && (jet_pt_2<1.6*pt_2))*(((pt_2<200)*(4.480947*TMath::Landau(pt_2,-1.317808,2.377057)+0.080496)) + (0.097342*(pt_2>=200)))))*(((pt_1<=25&&(n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4)))*(1.45883))+((pt_1>25&&(n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4)))*(1.12979+-0.00234044*pow(min(pt_1,250),1)+-1.27746e-05*pow(min(pt_1,250),2)+8.66708e-08*pow(min(pt_1,250),3)))+((pt_1<=25&&((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))==0))*(1.39124))+((pt_1>25&&((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))==0))*(1.08929+-0.00122025*pow(min(pt_1,250),1)+-1.16198e-05*pow(min(pt_1,250),2)+4.53552e-08*pow(min(pt_1,250),3))))*((((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4)))*(0.839493+0.00669195*pow(min(met,250),1)+-7.12353e-05*pow(min(met,250),2)+1.761e-07*pow(min(met,250),3)))+((((n_lowpt_jets==0 || (n_lowpt_jets==1&&fabs(jeta_2)<2.4) || (n_lowpt_jets==2&&fabs(jeta_2)<2.4))==0))*(0.846989+0.00769891*pow(min(met,250),1)+-9.25346e-05*pow(min(met,250),2)+2.35269e-07*pow(min(met,250),3)))))'

if ch == "mt":
  baseline_bothiso = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
  baseline_aiso1 = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
  baseline_aiso2_iso = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
  baseline_aiso2_aiso1 = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
elif ch == "et":
  baseline_bothiso = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2' % vars()
  baseline_aiso1 = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2' % vars()
  baseline_aiso2_iso = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2' % vars()
  baseline_aiso2_aiso1 = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && leptonveto==0 && ((trg_etaucross&&pt_1<33)||(trg_singleelectron&&pt_1>33)) && wt<2' % vars()
elif ch == "tt":
  baseline_bothiso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
  baseline_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
  baseline_aiso2_iso = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
  baseline_aiso2_aiso1 = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()

with open(params_file) as jsonfile:
  params = json.load(jsonfile)

if ch == "mt":
  data_files = [
    'SingleMuonA',
    'SingleMuonB',
    'SingleMuonC',
    'SingleMuonD'
  ]
elif ch == "et":
  data_files = [
    'EGammaA',
    'EGammaB',
    'EGammaC',
    'EGammaD'
  ]
elif ch == "tt":
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


def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1',doQCD=False,doW=False,doMC=False,doTT=False,doIso=False,fullMT=False,lowMT=False):
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

def ZeroNegativeBins(h):
  # if a bin of h1 is equal to the -ve of 
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def PlotDistributionComparison(var,x_label,dist_1,dist_1_name,act_dist,output_folder,save_name):
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  act_dist_ratio = act_dist.Clone()
  act_dist_ratio.Divide(act_dist)

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(act_dist)

  c = ROOT.TCanvas('c','c',1400,700)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  if 'mt_tot' in var or 'm_vis' in var:
    pad1.SetLogx()
  pad1.Draw()
  pad1.cd()

  act_dist.Draw("BAR")
  act_dist.SetStats(0)
  act_dist.SetFillColor(38)
  act_dist.GetXaxis().SetTitle(x_label)
  act_dist.GetYaxis().SetTitle('Events')
  act_dist.GetYaxis().SetTitleOffset(0.8)
  act_dist.GetYaxis().SetTitleSize(0.04)
  act_dist.GetXaxis().SetLabelSize(0)

  dist_1.Draw("E SAME")
  dist_1.SetMarkerColor(1)
  dist_1.SetLineColor(1)
  dist_1.SetMarkerStyle(19)

  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  l.AddEntry(dist_1,"F_{F} x N_{fail}","lep")
  l.AddEntry(act_dist,"N_{pass}","f");
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  if 'mt_tot' in var or 'm_vis' in var:
    pad2.SetLogx()
  pad2.Draw()
  pad2.cd()
 
  act_dist_ratio.SetFillColor(16)
  act_dist_ratio.Draw("E3")
  act_dist_ratio.SetAxisRange(0.6,1.4,'Y')
  act_dist_ratio.SetStats(0)
  act_dist_ratio.GetXaxis().SetLabelSize(0.08)
  act_dist_ratio.GetYaxis().SetLabelSize(0.08)
  act_dist_ratio.GetXaxis().SetTitle(x_label)
  act_dist_ratio.GetYaxis().SetTitle("Obs/Pred")
  act_dist_ratio.GetYaxis().SetTitleSize(0.1)
  act_dist_ratio.GetYaxis().SetTitleOffset(0.2)
  act_dist_ratio.GetXaxis().SetTitleSize(0.1)
  act_dist_ratio.GetXaxis().SetTitleOffset(0.9)

  line = ROOT.TLine(min(bins),1,max(bins),1)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw("SAME")
 
  dist_1_ratio.Draw("E SAME")
  dist_1_ratio.SetMarkerColor(1)
  dist_1_ratio.SetLineColor(1)
  dist_1_ratio.SetMarkerStyle(19)


  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()

plotted_vars = {'mt_tot[80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]':'m_{T}^{tot}/GeV',
                'm_vis[0,30,45,60,70,80,90,100,110,120,130,140,150,160,180,220,320,600]':'m_{vis}/GeV',
                'mt_1[0,10,20,30,40,50,60,70,80]':'m_{T}^{#mu}/GeV',
                'mt_2[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150]':'m_{T}^{#tau_{h}}/GeV',
                'pt_1[20,40,60,80,100,120,160,200,300]':'p_{T}^{#mu}/GeV',
                'pt_2[20,40,60,80,100,120,160,200,300]':'p_{T}^{#tau_{h}}/GeV',
                'n_jets[0,1,2,3,4]':'n_{jets}',
                'n_bjets[0,1,5]':'n_{b-jets}',
                'met[0,30,45,60,80,100,120,160,200,300]': 'MET/GeV'}

if var_to_plot != "all":
  for var,_ in plotted_vars.items():
    if var_to_plot in var:
      var_to_plot_dict = {var:plotted_vars[var]}
else:
  var_to_plot_dict = plotted_vars

for var,x_label in var_to_plot_dict.items():

  # Draw distribution for qcd (iao,aiso) and w+jets (data,mc) in DR
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))

  #qcd_pred_sum_dr = ROOT.TH1D('qcd_pred_sum_dr','',len(bins)-1, bins)
  #(qcd_pred_sum_dr,_,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'qcd_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=ff,doQCD=True)
  #(qcd_act_dr       ,_,_,_) = DrawHists(var, baseline_bothiso, 'qcd_act_dr' % vars(),input_folder,file_ext,doQCD=True)
  #PlotDistributionComparison(var,x_label,qcd_pred_sum_dr,'F_{F} Method',qcd_act_dr,output_folder,'qcd_dr')

  #qcd_aiso_pred_sum_dr = ROOT.TH1D('qcd_aiso_pred_sum_dr','',len(bins)-1, bins)
  #(qcd_aiso_pred_sum_dr,_,_,_) = DrawHists(var, baseline_aiso2_aiso1, 'qcd_aiso_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=ff,doQCD=True)
  #(qcd_aiso_act_dr       ,_,_,_) = DrawHists(var, baseline_aiso2_iso, 'qcd_aiso_act_dr' % vars(),input_folder,file_ext,doQCD=True)
  #PlotDistributionComparison(var,x_label,qcd_aiso_pred_sum_dr,'F_{F} Method',qcd_aiso_act_dr,output_folder,'qcd_aiso_dr')

  #wjets_pred_sum_dr = ROOT.TH1D('wjets_pred_sum_dr','',len(bins)-1, bins)
  #(_,wjets_pred_sum_dr,_,_) = DrawHists(var, baseline_aiso1+'&& n_bjets==0', 'wjets_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=ff,doW=True)
  #(_,wjets_act_dr       ,_,_) = DrawHists(var, baseline_bothiso+'&& n_bjets==0', 'wjets_act_dr' % vars(),input_folder,file_ext,doW=True)
  #PlotDistributionComparison(var,x_label,wjets_pred_sum_dr,'F_{F} Method',wjets_act_dr,output_folder,'wjets_dr')

  wjets_mc_pred_sum_dr = ROOT.TH1D('wjets_mc_pred_sum_dr','',len(bins)-1, bins)
  (_,_,wjets_mc_pred_sum_dr,_) = DrawHists(var, baseline_aiso1+'&& n_bjets==0', 'wjets_mc_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=ff,doMC=True)
  (_,_,wjets_mc_act_dr       ,_) = DrawHists(var, baseline_bothiso+'&& n_bjets==0', 'wjets_mc_act_dr' % vars(),input_folder,file_ext,doMC=True)
  PlotDistributionComparison(var,x_label,wjets_mc_pred_sum_dr,'F_{F} Method',wjets_mc_act_dr,output_folder,'wjets_mc_dr')

  # Draw distribution for qcd (aiso), wjets (mc), ttbar mc in AR

  #qcd_aiso_pred_sum_ar = ROOT.TH1D('qcd_aiso_pred_sum_ar','',len(bins)-1, bins)
  #(qcd_aiso_pred_sum_ar,_,_,_) = DrawHists(var, baseline_aiso2_aiso1, 'qcd_aiso_pred_jetpt_ar' % vars(),input_folder,file_ext,add_wt=ff,doQCD=True,doOS=True)
  #(qcd_aiso_act_ar       ,_,_,_) = DrawHists(var, baseline_aiso2_iso, 'qcd_aiso_act_ar' % vars(),input_folder,file_ext,doQCD=True,doOS=True)
  #PlotDistributionComparison(var,x_label,qcd_aiso_pred_sum_ar,'F_{F} Method',qcd_aiso_act_ar,output_folder,'qcd_aiso_ar')

  wjets_mc_pred_sum_ar = ROOT.TH1D('wjets_mc_pred_sum_ar','',len(bins)-1, bins)
  (_,_,wjets_mc_pred_sum_ar,_) = DrawHists(var, baseline_aiso1+' && n_bjets==0', 'wjets_mc_pred_jetpt_ar' % vars(),input_folder,file_ext,add_wt=ff,doMC=True,lowMT=True)
  (_,_,wjets_mc_act_ar       ,_) = DrawHists(var, baseline_bothiso+' && n_bjets==0', 'wjets_mc_act_ar' % vars(),input_folder,file_ext,doMC=True,lowMT=True)
  PlotDistributionComparison(var,x_label,wjets_mc_pred_sum_ar,'F_{F} Method',wjets_mc_act_ar,output_folder,'wjets_mc_ar')

  #ttbar_mc_pred_sum = ROOT.TH1D('ttbar_mc_pred_sum','',len(bins)-1, bins)
  #(_,_,_,ttbar_mc_pred_sum) = DrawHists(var, baseline_aiso1, 'ttbar_mc_pred_jetpt' % vars(),input_folder,file_ext,add_wt=ff,doTT=True)
  #(_,_,_,ttbar_mc_act       ) = DrawHists(var, baseline_bothiso, 'ttbar_mc_act' % vars(),input_folder,file_ext,doTT=True)
  #PlotDistributionComparison(var,x_label,ttbar_mc_pred_sum,'F_{F} Method',ttbar_mc_act,output_folder,'ttbar_mc')


  
 
