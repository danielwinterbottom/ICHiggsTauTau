import ROOT
import copy
import json
import os
from array import array

folder = "/vols/cms/gu18/Offline/output/4tau/1502_full"
channel = "ttt"

data_files = {}
data_files["2016_preVFP"] = [
     "TauB",
     "TauC",
     "TauD",
     "TauE",
     "TauF",
]
data_files["2016_postVFP"] = [
     "TauF",
     "TauG",
     "TauH",
]
data_files["2017"] = [
     "TauB",
     "TauC",
     "TauD",
     "TauE",
     "TauF",
]
data_files["2018"] = [
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

sel = "(fabs(q_sum)!=1 && deepTauVsJets_iso_1>0.1 && deepTauVsJets_iso_2>0.1 && deepTauVsJets_iso_3>0.1 && deepTauVsMu_vloose_1>0.5 && deepTauVsEle_vvloose_1>0.5 && deepTauVsMu_vloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && ((pt_1>40 && pt_2>40 && trg_doubletau_12==1) || (pt_1>40 && pt_3>40 && trg_doubletau_13==1) || (pt_2>40 && pt_3>40 && trg_doubletau_23==1)))"

f = "(deepTauVsJets_loose_X<0.5 && deepTauVsJets_iso_X>0.1)"
p = "(deepTauVsJets_loose_X>0.5)"
o = "(deepTauVsJets_iso_X>0.1)"

f1 = f.replace("X","1")
f2 = f.replace("X","2")
f3 = f.replace("X","3")
p1 = p.replace("X","1")
p2 = p.replace("X","2")
p3 = p.replace("X","3")
o1 = o.replace("X","1")
o2 = o.replace("X","2")
o3 = o.replace("X","3")


get_sels = {}

get_sels["fail_123"] = "({f1} * {f2} * {f3})"
get_sels["pass_123"] = "({p1} * {p2} * {p3})"

get_sels["fail_12"] = "({f1} * {f2} * {o3})"
get_sels["pass_12"] = "({p1} * {p2} * {o3})"

get_sels["fail_13"] = "({f1} * {o2} * {f3})"
get_sels["pass_13"] = "({p1} * {o2} * {p3})"

get_sels["fail_23"] = "({o1} * {f2} * {f3})"
get_sels["pass_23"] = "({o1} * {p2} * {p3})"

get_sels["fail_1"] = "({f1} * {o2} * {o3})"
get_sels["pass_1"] = "({p1} * {o2} * {o3})"

get_sels["fail_2"] = "({o1} * {f2} * {o3})"
get_sels["pass_2"] = "({o1} * {p2} * {o3})"

get_sels["fail_3"] = "({o1} * {o2} * {f3})"
get_sels["pass_3"] = "({o1} * {o2} * {p3})"

for k,v in get_sels.items(): get_sels[k] = get_sels[k].format(f1=f1,f2=f2,f3=f3,p1=p1,p2=p2,p3=p3,o1=o1,o2=o2,o3=o3)

mc_sels = {}
for k,v in get_sels.items():
  mc_sels[k] = "("
  for ind, num in enumerate(k.split("_")[1]):
    if ind != 0: mc_sels[k] += "*"
    mc_sels[k] += "(gen_match_{} != 6)".format(num)
  mc_sels[k] += ")"

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
    ratio[k.replace("pass","")] = v.Clone()
    ratio[k.replace("pass","")].Divide(yields[k.replace("pass","fail")])
    print k.replace("pass","fail")
    print ratio[k.replace("pass","")].Print("all")

