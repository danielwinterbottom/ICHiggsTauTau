import ROOT
import copy
import json
import os
from array import array

folder = "/vols/cms/gu18/Offline/output/4tau/1502_full"
channel = "emtt"
#channel = "mmtt"
#channel = "eett"

data_files = {}
data_files["2016_preVFP"] = [
     "SingleElectronB",
     "SingleElectronC",
     "SingleElectronD",
     "SingleElectronE",
     "SingleElectronF",
     "SingleMuonB",
     "SingleMuonC",
     "SingleMuonD",
     "SingleMuonE",
     "SingleMuonF",
     "TauB",
     "TauC",
     "TauD",
     "TauE",
     "TauF",
]
data_files["2016_postVFP"] = [
     "SingleElectronF",
     "SingleElectronG",
     "SingleElectronH",
     "SingleMuonF",
     "SingleMuonG",
     "SingleMuonH",
     "TauF",
     "TauG",
     "TauH",
]
data_files["2017"] = [
     "SingleElectronB",
     "SingleElectronC",
     "SingleElectronD",
     "SingleElectronE",
     "SingleElectronF",
     "SingleMuonB",
     "SingleMuonC",
     "SingleMuonD",
     "SingleMuonE",
     "SingleMuonF",
     "TauB",
     "TauC",
     "TauD",
     "TauE",
     "TauF",
]
data_files["2018"] = [
     "EGammaA",
     "EGammaB",
     "EGammaC",
     "EGammaD",
     "SingleMuonA",
     "SingleMuonB",
     "SingleMuonC",
     "SingleMuonD",
     "TauA",
     "TauB",
     "TauC",
     "TauD",
]

mc_files = {}
mc_files["2016_preVFP"] = [
     'DYJetsToLL-LO',
     'DY1JetsToLL-LO',
     'DY2JetsToLL-LO',
     'DY3JetsToLL-LO',
     'DY4JetsToLL-LO',
     'DYJetsToLL_M-10to50-LO',
     'TTTo2L2Nu',
     'TTToHadronic',
     'TTToSemiLeptonic',
     'WZTo1L1Nu2Q',
     'WZTo3LNu',
     'WZTo3LNu',
     'WWTo2L2Nu',
     'ZZTo2L2Nu',
     'ZZTo4L',
     'T-tW',
     'Tbar-tW',
     'Tbar-t',
     'T-t',
     'WWZ',
     'WWZ-ext1',
     'WZZ',
     'WZZ-ext1',
     'WWW',
     'WWW-ext1',
     'ZZZ',
     'ZZZ-ext1',
     'WJetsToLNu-LO',
     'W1JetsToLNu-LO',
     'W2JetsToLNu-LO',
     'W3JetsToLNu-LO',
     'W4JetsToLNu-LO',
     'EWKWMinus2Jets_WToLNu',
     'EWKWPlus2Jets_WToLNu',
     'WGToLNuG',
     'GluGluToContinToZZTo2e2mu',
     'GluGluToContinToZZTo2e2tau',
     'GluGluToContinToZZTo2mu2tau',
     'GluGluToContinToZZTo4e',
     'GluGluToContinToZZTo4mu',
     'GluGluToContinToZZTo4tau',
]
mc_files["2016_postVFP"] = [
     'DYJetsToLL-LO',
     'DY1JetsToLL-LO',
     'DY2JetsToLL-LO',
     'DY3JetsToLL-LO',
     'DY4JetsToLL-LO',
     'DYJetsToLL_M-10to50-LO',
     'TTTo2L2Nu',
     'TTToHadronic',
     'TTToSemiLeptonic',
     'WZTo3LNu',
     'WZTo3LNu',
     'WWTo2L2Nu',
     'ZZTo2L2Nu',
     'ZZTo4L',
     'T-tW',
     'Tbar-tW',
     'Tbar-t',
     'T-t',
     'WWZ',
     'WWZ-ext1',
     'WZZ',
     'WZZ-ext1',
     'WWW',
     'WWW-ext1',
     'ZZZ',
     'ZZZ-ext1',
     'WJetsToLNu-LO',
     'W1JetsToLNu-LO',
     'W2JetsToLNu-LO',
     'W3JetsToLNu-LO',
     'W4JetsToLNu-LO',
     'EWKWMinus2Jets_WToLNu',
     'EWKWPlus2Jets_WToLNu',
     'WGToLNuG',
     'GluGluToContinToZZTo2e2mu',
     'GluGluToContinToZZTo2e2tau',
     'GluGluToContinToZZTo2mu2tau',
     'GluGluToContinToZZTo4e',
     'GluGluToContinToZZTo4mu',
     'GluGluToContinToZZTo4tau',
]
mc_files["2017"] = [
     'DYJetsToLL-LO',
     'DY1JetsToLL-LO',
     'DY2JetsToLL-LO',
     'DY3JetsToLL-LO',
     'DY4JetsToLL-LO',
     'DYJetsToLL_M-10to50-LO',
     'TTTo2L2Nu',
     'TTToHadronic',
     'TTToSemiLeptonic',
     'T-tW',
     'Tbar-tW',
     'Tbar-t',
     'T-t',
     'WZTo3LNu',
     'WWTo2L2Nu',
     'ZZTo2L2Nu',
     'ZZTo4L',
     'WWZ',
     'WWZ-ext1',
     'WZZ',
     'WZZ-ext1',
     'WWW',
     'WWW-ext1',
     'ZZZ',
     'ZZZ-ext1',
     'WJetsToLNu-LO',
     'W1JetsToLNu-LO',
     'W2JetsToLNu-LO',
     'W3JetsToLNu-LO',
     'W4JetsToLNu-LO',
     'EWKWMinus2Jets_WToLNu',
     'EWKWPlus2Jets_WToLNu',
     'WGToLNuG',
     'GluGluToContinToZZTo2e2mu',
     'GluGluToContinToZZTo2e2tau',
     'GluGluToContinToZZTo2mu2tau',
     'GluGluToContinToZZTo4e',
     'GluGluToContinToZZTo4mu',
     'GluGluToContinToZZTo4tau',
]
mc_files["2018"] = [
     'DYJetsToLL-LO',
     'DY1JetsToLL-LO',
     'DY2JetsToLL-LO',
     'DY3JetsToLL-LO',
     'DY4JetsToLL-LO',
     'DYJetsToLL_M-10to50-LO',
     'TTTo2L2Nu',
     'TTToHadronic',
     'TTToSemiLeptonic',
     'WZTo1L1Nu2Q',
     'WZTo3LNu',
     'WWTo1L1Nu2Q',
     'WWTo2L2Nu',
     'ZZTo2L2Nu',
     'ZZTo4L',
     'Tbar-t',
     'Tbar-tW',
     'T-t',
     'T-tW',
     'WWZ',
     'WWZ-ext1',
     'WZZ',
     'WZZ-ext1',
     'WWW',
     'WWW-ext1',
     'ZZZ',
     'ZZZ-ext1',
     'WJetsToLNu-LO',
     'W1JetsToLNu-LO',
     'W2JetsToLNu-LO',
     'W3JetsToLNu-LO',
     'W4JetsToLNu-LO',
     'EWKWMinus2Jets_WToLNu',
     'EWKWPlus2Jets_WToLNu',
     'WGToLNuG',
     'GluGluToContinToZZTo2e2mu',
     'GluGluToContinToZZTo2e2tau',
     'GluGluToContinToZZTo2mu2tau',
     'GluGluToContinToZZTo4e',
     'GluGluToContinToZZTo4mu',
     'GluGluToContinToZZTo4tau',
]

params = {yr:"scripts/params_UL_{}.json".format(yr) for yr in data_files.keys()}

lumis = {}
lumis["2016_preVFP"] = 19520.0 
lumis["2016_postVFP"] = 16810.0
lumis["2017"] = 41530.0
lumis["2018"] = 59830.0

sel = "((((((n_bjets==0 && iso_1<0.15 && iso_2<0.15 && 1 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && 1 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5)*(!((q_1+q_2+q_3+q_4)==0))) && ((pt_1>33 && trg_singleelectron_1) || (pt_2>25 && trg_singlemuon_2) || (pt_3>40 && pt_4>40 && trg_doubletau_34)))))&&((!(isTau && (pt_2>25 && trg_singlemuon_2))) && (!(isSingleElectron && ((pt_1>25 && trg_singlemuon_1) || (pt_3>40 && pt_4>40 && trg_doubletau_34))))))"
#sel = "((((((n_bjets==0 && iso_1<0.15 && iso_2<0.15 && 1 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && 1 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5)*(!((q_1+q_2+q_3+q_4)==0))) && ((pt_1>25 && trg_singlemuon_1) || (pt_2>25 && trg_singlemuon_2) || (pt_3>40 && pt_4>40 && trg_doubletau_34)))))&&((pt_1>25 && trg_singlemuon_1) || (pt_2>25 && trg_singlemuon_2) || (pt_3>40 && pt_4>40 && trg_doubletau_34)))"
#sel = "((((((n_bjets==0 && iso_1<0.15 && iso_2<0.15 && 1 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && 1 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5)*(!((q_1+q_2+q_3+q_4)==0))) && ((pt_1>33 && trg_singleelectron_1) || (pt_2>33 && trg_singleelectron_2) || (pt_3>40 && pt_4>40 && trg_doubletau_34)))))&&((pt_1>25 && trg_singleelectron_1) || (pt_2>25 && trg_singleelectron_2) || (pt_3>40 && pt_4>40 && trg_doubletau_34)))"

get_sels = {}
get_sels["double_fail"] = "((deepTauVsJets_loose_3<0.5 && deepTauVsJets_iso_3>0.1) * (deepTauVsJets_loose_4<0.5 && deepTauVsJets_iso_4>0.1))"
get_sels["double_pass"] = " ((deepTauVsJets_loose_3>0.5) * (deepTauVsJets_loose_4>0.5))"

#get_sels["single_fail_1"] = "((deepTauVsJets_loose_3>0.5) * (deepTauVsJets_loose_4<0.5 && deepTauVsJets_iso_4>0.1))"
#get_sels["single_fail_2"] = "((deepTauVsJets_loose_3<0.5 && deepTauVsJets_iso_3>0.1) * (deepTauVsJets_loose_4>0.5))"

#get_sels["single_pass_1"] = "((deepTauVsJets_loose_3>0.5) * (deepTauVsJets_loose_4>0.5))"
#get_sels["single_pass_2"] = "((deepTauVsJets_loose_3>0.5) * (deepTauVsJets_loose_4>0.5))"

get_sels["single_fail_1"] = "((deepTauVsJets_iso_3>0.1) * (deepTauVsJets_loose_4<0.5 && deepTauVsJets_iso_4>0.1))"
get_sels["single_fail_2"] = "((deepTauVsJets_loose_3<0.5 && deepTauVsJets_iso_3>0.1) * (deepTauVsJets_iso_4>0.1))"

get_sels["single_pass_1"] = "((deepTauVsJets_iso_3>0.1) * (deepTauVsJets_loose_4>0.5))"
get_sels["single_pass_2"] = "((deepTauVsJets_loose_3>0.5) * (deepTauVsJets_iso_4>0.1))"


mc_sels = {}
mc_sels["double_fail"] = "((gen_match_3 != 6) * (gen_match_4 != 6))"
mc_sels["single_fail_1"] = "(gen_match_4 != 6)"
mc_sels["single_fail_2"] = " (gen_match_3 != 6)"
mc_sels["double_pass"] = "((gen_match_3 != 6) * (gen_match_4 != 6))"
mc_sels["single_pass_1"] = "(gen_match_4 != 6)"
mc_sels["single_pass_2"] = " (gen_match_3 != 6)"

var_input = "mt_tot[0,5000]"

var = var_input.split('[')[0]
bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
hout = ROOT.TH1D('hout','',len(bins)-1, bins)


yields = {}

for sel_type, sel_iso in get_sels.items():
  data = copy.deepcopy(hout)
  mc = copy.deepcopy(hout)
  
  data_sel = "(" + sel_iso + ") * (" + sel + ")"
  for year, files in data_files.items():
    file_ext = "_{}_{}.root".format(channel,year)
    for i in files:
      if not os.path.isfile('%(folder)s/%(i)s%(file_ext)s' % vars()): continue
      f = ROOT.TFile('%(folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(), data_sel, 'goff')
      h = t.GetHistogram()
      data.Add(h)

  mc_sel = "(" + mc_sels[sel_type]  + ") * (" + sel_iso + ") * (" + sel + ")"
  for year, files in mc_files.items():
    file_ext = "_{}_{}.root".format(channel,year)
    with open(params[year]) as jsonfile: param = json.load(jsonfile)

    for i in files:
      f = ROOT.TFile('%(folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(), mc_sel, 'goff')
      h = t.GetHistogram()
      if type(h) != ROOT.TH1D: continue
      scale = lumis[year]*param[i]['xs']/param[i]['evt']
      h.Scale(scale)
      mc.Add(h)

  del f,t,h,i
  yields[sel_type] = data.Clone()
  yields[sel_type].Add(mc,-1)

ratio = {}
for k, v in yields.items():
  if "pass" in k:
    ratio[k.replace("_pass","")] = v.Clone()
    ratio[k.replace("_pass","")].Divide(yields[k.replace("pass","fail")])

single = ratio["single_1"].Clone()
single.Multiply(ratio["single_2"].Clone())

print "single"
single.Print("all")

print "double"
ratio["double"].Print("all")


uncert = ratio["double"].Clone()
uncert.Divide(single)

print "Uncert"
uncert.Print("all")
