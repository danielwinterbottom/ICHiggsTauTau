import ROOT
import json
from array import array


### Input options ###

input_folder  = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_matched_v2"
params_file   = "scripts/params_vlq_2017.json"
ntuple_name   = "ntuple"
var_input     = "chi[1,3,5,7,9,11,13,15,17,19,21]"
x_label       = "#chi"
y_label       = "g_{U}"
z_label       = "Events"
sel           = "wt*wt_vlq_off_diag_0*(n_deepbjets==0 && m_vis>600 && deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"
do_ratio      = False
ratio_file    = "Nominal"
logx          = False
logy          = False
logz          = False
channel       = "tt"
output_name   = "test_plot.pdf"
draw_type     = "colz"
x_offset      = 0
y_offset      = 0
z_offset      = 1.8
lumi          = 59740

### Input files ###

files = {
         "": ["VectorLQToTauTau_betaRd33_0_mU2000_gU1_matched_tt_2018.root","VectorLQToTauTau_betaRd33_0_lowM_mU2000_gU1_matched_interference_tt_2018.root", "VectorLQToTauTau_betaRd33_0_highM_mU2000_gU1_matched_interference_tt_2018.root"],
         #"": ["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root"], 
        }

### Get histograms from ROOT files ###

if ":" in var_input:
  var_input1 = var_input.split(':')[0]
  var_input2 = var_input.split(':')[1]
  var1 = var_input1.split('[')[0]
  var2 = var_input2.split('[')[0]
  bins1 = array('f', map(float,var_input1.split('[')[1].split(']')[0].split(',')))
  bins2 = array('f', map(float,var_input2.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH2D('hout','',len(bins1)-1, bins1,len(bins2)-1, bins2)
  var='%(var2)s:%(var1)s' % vars()
else:
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)

h_dict = {}
f = {}

bins2_array = [0.8,0.9,1.0,1.2,1.3,1.4,1.5,1.6]
bins2 = array('f',map(float,bins2_array))

for gU in bins2_array:
  for key, val in files.items():
  
    with open(params_file) as jsonfile:
      params = json.load(jsonfile)
  
    if type(val) == str: val = [val] 
  
    h_dict[key+str(gU)] = hout.Clone()
    h_dict[key+str(gU)].SetName(key+str(gU))
    for i in val:
      f[i+str(gU)] = ROOT.TFile('%(input_folder)s/%(i)s' % vars())
      t = f[i+str(gU)].Get(ntuple_name)
      h_dict[i+str(gU)] = hout.Clone()
      h_dict[i+str(gU)].SetName("h")
      t.Draw('%(var)s>>h' % vars(),sel,'goff')
      h_dict[i+str(gU)] = t.GetHistogram()
      scale = lumi*params[i.split("_"+channel)[0]]['xs']/params[i.split("_"+channel)[0]]['evt']
      h_dict[i+str(gU)].Scale(scale)
      if "interference" in i: h_dict[i+str(gU)].Scale(gU**2)
      else: h_dict[i+str(gU)].Scale(gU**4)
      h_dict[i+str(gU)].Scale(1.0,"width")
      if gU == 1.0:
        h_dict[i+str(gU)].Print("all")
      h_dict[key+str(gU)].Add(h_dict[i+str(gU)].Clone())

# Combine TH1 to TH2
hout = ROOT.TH2D('hout','',len(bins)-1, bins,len(bins2)-1, bins2)
for i in range(0,len(bins2_array)):
  for j in range(0,len(bins)):
    hout.SetBinContent(j,i+1,h_dict[str(bins2_array[i])].GetBinContent(j))

var_input = ":"
hout.Print("all")

files = {"1":""}
h_dict["1"] = hout.Clone()

### Get ratio ###
if do_ratio:
  for key, val in files.items():
    h_dict[key].Divide(h_dict[ratio_file].Clone())


### Plot histograms ###

colours = [2,3,4,5,6,7,8,9,10]

c = ROOT.TCanvas('c','c',700,700)
#c.SetRightMargin(0.18)
#c.SetMargin(0.1, 0.2, 0.1, 0.1)
#c.SetLeftMargin(0.15)
if logx: c.SetLogx()
if logy: c.SetLogy()
if logz: c.SetLogz()
if ":" in var_input: c.SetRightMargin(0.15)

do_options = True
i = 0
for key, val in files.items():
  if do_options:
    h_dict[key].SetStats(0)
    h_dict[key].GetXaxis().SetTitle(x_label)
    h_dict[key].GetYaxis().SetTitle(y_label)
    h_dict[key].GetXaxis().SetTitleOffset(x_offset)
    h_dict[key].GetYaxis().SetTitleOffset(y_offset)
    h_dict[key].Draw(draw_type)
    if ":" not in var_input:
      h_dict[key].SetLineColor(colours[i])
      h_dict[key].SetMarkerColor(colours[i])
    else:
      h_dict[key].GetZaxis().SetTitle(z_label)
      h_dict[key].GetZaxis().SetTitleOffset(z_offset)
    do_options = False
  else:
    if ":" not in var_input:
      h_dict[key].SetLineColor(colours[i])
      h_dict[key].SetMarkerColor(colours[i]) 
      h_dict[key].Draw(draw_type+" same")

  i+=1

c.Update()

if ":" not in var_input:
  l = ROOT.TLegend(0.7,0.7,0.88,0.88)
  l.SetBorderSize(0)
  l.SetTextSize(0.04)
  for key, val in files.items():
    l.AddEntry(h_dict[key],key,'lep')
  l.Draw()

c.Print(output_name)
