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

##### Fake Factor Strings ########
jetpt_qcd = '(((pt_2>=50 && pt_2<80))*(1.200729*TMath::Landau(jet_pt_2,56.306786,6.473258)+0.025160))+(((pt_2>=140 && pt_2<9999))*(33569.374669*TMath::Landau(jet_pt_2,498.644725,65.228696)+0.099494))+(((pt_2>=80 && pt_2<140))*(0.908373*TMath::Landau(jet_pt_2,88.554970,16.020250)+0.022461))+(((pt_2>=40 && pt_2<50))*(1.501551*TMath::Landau(jet_pt_2,44.241673,6.083739)+-0.007730))+(((pt_2>=0 && pt_2<25))*(1.919042*TMath::Landau(jet_pt_2,24.712901,2.005622)+0.004407))+(((pt_2>=25 && pt_2<40))*(1.158920*TMath::Landau(jet_pt_2,30.034642,6.249130)+-0.004769))'

jetpt_qcd_aiso = '(((pt_2>=50 && pt_2<80))*(1.018026*TMath::Landau(jet_pt_2,56.017615,11.188485)+0.003135))+(((pt_2>=140 && pt_2<9999))*(19.964153*TMath::Landau(jet_pt_2,199.798982,0.344514)+0.102864))+(((pt_2>=80 && pt_2<140))*(0.944295*TMath::Landau(jet_pt_2,97.136185,21.958842)+-0.017409))+(((pt_2>=40 && pt_2<50))*(1.421630*TMath::Landau(jet_pt_2,47.609143,3.880187)+0.012878))+(((pt_2>=0 && pt_2<25))*(1.552234*TMath::Landau(jet_pt_2,24.037834,2.440050)+0.003144))+(((pt_2>=25 && pt_2<40))*(1.282319*TMath::Landau(jet_pt_2,29.341010,5.361961)+-0.000315))'

jetpt_wjets = '(((pt_2>=50 && pt_2<80))*(1.649885*TMath::Landau(jet_pt_2,40.221268,15.103273)+-0.023654))+(((pt_2>=140 && pt_2<9999))*(0.741913*TMath::Landau(jet_pt_2,225.565236,12603.542805)+0.000142))+(((pt_2>=80 && pt_2<140))*(5.753494*TMath::Landau(jet_pt_2,-328.021617,140.646061)+-0.297678))+(((pt_2>=40 && pt_2<50))*(1.694481*TMath::Landau(jet_pt_2,43.368827,5.748568)+-0.000765))+(((pt_2>=0 && pt_2<25))*(2.220945*TMath::Landau(jet_pt_2,23.533964,2.561129)+0.006234))+(((pt_2>=25 && pt_2<40))*(1.593126*TMath::Landau(jet_pt_2,29.210422,5.239049)+0.004674))'

jetpt_wjets_mc = '(((pt_2>=50 && pt_2<80))*(1.514669*TMath::Landau(jet_pt_2,50.071797,11.436736)+-0.000991))+(((pt_2>=140 && pt_2<9999))*(8.905064*TMath::Landau(jet_pt_2,401.408010,341.135443)+-1.421123))+(((pt_2>=80 && pt_2<140))*(1.629080*TMath::Landau(jet_pt_2,61.407605,21.927440)+-0.009120))+(((pt_2>=40 && pt_2<50))*(2.111221*TMath::Landau(jet_pt_2,43.617052,3.164197)+0.026121))+(((pt_2>=0 && pt_2<25))*(2.597108*TMath::Landau(jet_pt_2,22.172599,2.502718)+-0.002499))+(((pt_2>=25 && pt_2<40))*(1.533886*TMath::Landau(jet_pt_2,29.043714,5.451369)+-0.004209))'

jetpt_ttbar_mc = '(((pt_2>=50 && pt_2<80))*(1.389707*TMath::Landau(jet_pt_2,48.321228,9.965364)+0.001623))+(((pt_2>=140 && pt_2<9999))*(1.324990*TMath::Landau(jet_pt_2,-1429.002133,669.422193)+0.000007))+(((pt_2>=80 && pt_2<140))*(0.939720*TMath::Landau(jet_pt_2,89.058680,13.987755)+0.018998))+(((pt_2>=40 && pt_2<50))*(1.779675*TMath::Landau(jet_pt_2,43.264190,3.868491)+0.005780))+(((pt_2>=0 && pt_2<25))*(1.808911*TMath::Landau(jet_pt_2,22.580480,2.398923)+-0.000223))+(((pt_2>=25 && pt_2<40))*(1.346542*TMath::Landau(jet_pt_2,28.689396,4.515645)+0.002505))'

njets_qcd = '(n_jets==0)*(0.314347*TMath::Landau(pt_2,46.699016,12.132896)+0.061950) + (n_jets==1)*(0.077204*TMath::Landau(pt_2,45.769425,12.940029)+0.086458) + (n_jets>1)*(0.520224*TMath::Landau(pt_2,256.454857,230.252681)+0.000001)'
njets_qcd_aiso = '(n_jets==0)*(0.804763*TMath::Landau(pt_2,66.850246,31.750701)+-0.023313) + (n_jets==1)*(7.588977*TMath::Landau(pt_2,105.902273,212.812360)+-1.276283) + (n_jets>1)*(0.454884*TMath::Landau(pt_2,114.674679,112.377438)+-0.000003)'
njets_wjets = '(n_jets==0)*(0.788945*TMath::Landau(pt_2,24.554684,100.802156)+0.000006) + (n_jets==1)*(0.184461*TMath::Landau(pt_2,42.815839,22.677562)+0.086080) + (n_jets>1)*(0.402916*TMath::Landau(pt_2,28.523134,6.887628)+0.027908)'
njets_wjets_mc = '(n_jets==0)*(0.821322*TMath::Landau(pt_2,810.261195,740.962106)+0.012498) + (n_jets==1)*(0.719553*TMath::Landau(pt_2,97.404285,73.158490)+0.000000) + (n_jets>1)*(0.079655*TMath::Landau(pt_2,110.329192,26.793263)+0.092029)'
njets_ttbar_mc = '(n_jets==0)*(5.169081*TMath::Landau(pt_2,335.106696,351.782114)+-0.777435) + (n_jets==1)*(0.360254*TMath::Landau(pt_2,94.284400,48.252774)+0.029325) + (n_jets>1)*(1.191239*TMath::Landau(pt_2,138.822952,144.428177)+-0.117314)'

# mt plots
baseline_bothiso = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
baseline_aiso1 = 'iso_1<0.15 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()


baseline_aiso2_iso = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()
baseline_aiso2_aiso1 = 'iso_1<0.5&&iso_1>0.25 && deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && ((trg_mutaucross&&pt_1<25)||(trg_singlemuon&&pt_1>25)) && wt<2' % vars()


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

plotted_vars = {'mt_tot[0,30,45,60,70,80,90,100,110,120,130,140,150,160,180,220,320,600]':'m_{T}^{tot}/GeV',
                'm_vis[0,30,45,60,70,80,90,100,110,120,130,140,150,160,180,220,320,600]':'m_{vis}/GeV',
                'mt_1[0,10,20,30,40,50,60,70,80]':'m_{T}^{#mu}/GeV',
                'mt_2[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150]':'m_{T}^{#tau_{h}}/GeV',
                'pt_1[20,40,60,80,100,120,160,200,300]':'p_{T}^{#mu}/GeV',
                'pt_2[20,40,60,80,100,120,160,200,300]':'p_{T}^{#tau_{h}}/GeV',
                'n_jets[0,1,2,3,4]':'n_{jets}',
                'n_bjets[0,1,5]':'n_{b-jets}',
                'met[0,30,45,60,80,100,120,160,200,300]': 'MET/GeV',

for var,x_label in plotted_vars.items():

  # Draw distribution for qcd (iao,aiso) and w+jets (data,mc) in DR

  (qcd_pred_jetpt_dr,_,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'qcd_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_qcd,doQCD=True)
  (qcd_pred_njets_dr,_,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'qcd_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_qcd,doQCD=True)
  (qcd_act_dr       ,_,_,_) = DrawHists(var, baseline_bothiso, 'qcd_act_dr' % vars(),input_folder,file_ext,doQCD=True)
  PlotDistributionComparison(var,x_label,qcd_pred_jetpt_dr,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',qcd_pred_njets_dr,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',qcd_act_dr,output_folder,'qcd_dr')

  (qcd_aiso_pred_jetpt_dr,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_qcd_aiso,doQCD=True)
  (qcd_aiso_pred_njets_dr,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_qcd_aiso,doQCD=True)
  (qcd_aiso_act_dr       ,_,_,_) = DrawHists(var, baseline_aiso2_iso, 'qcd_aiso_act_dr' % vars(),input_folder,file_ext,doQCD=True)
  PlotDistributionComparison(var,x_label,qcd_aiso_pred_jetpt_dr,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',qcd_aiso_pred_njets_dr,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',qcd_aiso_act_dr,output_folder,'qcd_aiso_dr')


  (_,wjets_pred_jetpt_dr,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_wjets,doW=True)
  (_,wjets_pred_njets_dr,_,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_wjets,doW=True)
  (_,wjets_act_dr       ,_,_) = DrawHists(var, baseline_bothiso, 'wjets_act_dr' % vars(),input_folder,file_ext,doW=True)
  PlotDistributionComparison(var,x_label,wjets_pred_jetpt_dr,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',wjets_pred_njets_dr,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',wjets_act_dr,output_folder,'wjets_dr')

  (_,_,wjets_mc_pred_jetpt_dr,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_jetpt_dr' % vars(),input_folder,file_ext,add_wt=jetpt_wjets_mc,doMC=True)
  (_,_,wjets_mc_pred_njets_dr,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_njets_dr' % vars(),input_folder,file_ext,add_wt=njets_wjets_mc,doMC=True)
  (_,_,wjets_mc_act_dr       ,_) = DrawHists(var, baseline_bothiso, 'wjets_mc_act_dr' % vars(),input_folder,file_ext,doMC=True)
  PlotDistributionComparison(var,x_label,wjets_mc_pred_jetpt_dr,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',wjets_mc_pred_njets_dr,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',wjets_mc_act_dr,output_folder,'wjets_mc_dr')

  # Draw distribution for qcd (aiso), wjets (mc), ttbar mc in AR

  (qcd_aiso_pred_jetpt_ar,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_jetpt_ar' % vars(),input_folder,file_ext,add_wt=jetpt_qcd_aiso,doQCD=True,doOS=True)
  (qcd_aiso_pred_njets_ar,_,_,_) = DrawHists(var, baseline_aiso2_aiso1+' && jet_pt_2>0', 'qcd_aiso_pred_njets_ar' % vars(),input_folder,file_ext,add_wt=njets_qcd_aiso,doQCD=True,doOS=True)
  (qcd_aiso_act_ar       ,_,_,_) = DrawHists(var, baseline_aiso2_iso, 'qcd_aiso_act_ar' % vars(),input_folder,file_ext,doQCD=True,doOS=True)
  PlotDistributionComparison(var,x_label,qcd_aiso_pred_jetpt_ar,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',qcd_aiso_pred_njets_ar,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',qcd_aiso_act_ar,output_folder,'qcd_aiso_ar')

  (_,_,wjets_mc_pred_jetpt_ar,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_jetpt_ar' % vars(),input_folder,file_ext,add_wt=jetpt_wjets_mc,doMC=True,lowMT=True)
  (_,_,wjets_mc_pred_njets_ar,_) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'wjets_mc_pred_njets_ar' % vars(),input_folder,file_ext,add_wt=njets_wjets_mc,doMC=True,lowMT=True)
  (_,_,wjets_mc_act_ar       ,_) = DrawHists(var, baseline_bothiso, 'wjets_mc_act_ar' % vars(),input_folder,file_ext,doMC=True,lowMT=True)
  PlotDistributionComparison(var,x_label,wjets_mc_pred_jetpt_ar,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',wjets_mc_pred_njets_ar,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',wjets_mc_act_ar,output_folder,'wjets_mc_ar')

  (_,_,_,ttbar_mc_pred_jetpt) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'ttbar_mc_pred_jetpt' % vars(),input_folder,file_ext,add_wt=jetpt_ttbar_mc,doTT=True)
  (_,_,_,ttbar_mc_pred_njets) = DrawHists(var, baseline_aiso1+' && jet_pt_2>0', 'ttbar_mc_pred_njets' % vars(),input_folder,file_ext,add_wt=njets_ttbar_mc,doTT=True)
  (_,_,_,ttbar_mc_act       ) = DrawHists(var, baseline_bothiso, 'ttbar_mc_act' % vars(),input_folder,file_ext,doTT=True)
  PlotDistributionComparison(var,x_label,ttbar_mc_pred_jetpt,'F_{F} Method Using p_{T} Binning and Jet-p_{T} Fitting',ttbar_mc_pred_njets,'F_{F} Method Using n_{jets} Binning and p_{T} Fitting',ttbar_mc_act,output_folder,'ttbar_mc')


  
  
