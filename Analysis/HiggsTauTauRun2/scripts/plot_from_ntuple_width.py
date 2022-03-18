import ROOT
import os
import json
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting

### Input options ###

input_folder  = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_width_check"
params_file   = "scripts/params_vlq_2018.json"
ntuple_name   = "ntuple"
#var_input     = "chi[1,3,5,7,9,11,13,15,17,19,21]"
#var_input     = "mt_tot[60,80,100,125,150,200,300,400,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900]"
var_input     = "mt_tot[600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900]"
x_label       = "m_{T}^{tot}"
y_label       = "dN/dm_{T}^{tot}"
z_label       = "Events"
sel           = "wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"
do_ratio      = True
ratio_file    = "1.44 x g_{U}=1.0"
logx          = True
logy          = False
logz          = False
channel       = "tt"
gU            = 1
output_name   = "vlq_signal_betaR33_0_best_fit_zoomed_MU_1_gU_{}_tt_2018.pdf".format(str(gU).replace(".","p"))
draw_type     = "e2"
draw_type_2   = "hist"
x_offset      = 0
y_offset      = 2.1
#y_offset      = 0
z_offset      = 1.8
lumi          = 59740
horiz_line    = 1.
under_legend_1= "M_{U} = 1 TeV" % vars()
#under_legend_1= ""
under_legend_2= "#bf{#beta}_{R}^{b#bf{#tau}}=0 (best fit)"
#under_legend_3= "N_{b-jets} = 0"
under_legend_3=""

### Input files ###

files = {
         "1.44 x g_{U}=1.0": ["VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2018.root", "VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2018.root"],
         "g_{U}=1.2": ["VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2018.root", "VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2018.root"],
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

for key, val in files.items():

  with open(params_file) as jsonfile:
    params = json.load(jsonfile)

  if type(val) == str: val = [val] 

  h_dict[key] = hout.Clone()
  h_dict[key].SetName(key)
  for i in val:
    i_name = i+"_"+key
    f[i_name] = ROOT.TFile('%(input_folder)s/%(i)s' % vars())
    t = f[i_name].Get(ntuple_name)
    h_dict[i_name] = hout.Clone()
    h_dict[i_name].SetName("h")
    if " x " not in key: 
      new_sel = "wt_vlq_gU_{}*({})".format(key.split("=")[1].replace(".","p"),sel)
    else:
      new_sel = sel
    print new_sel
    t.Draw('%(var)s>>h' % vars(),new_sel,'goff')
    h_dict[i_name] = t.GetHistogram()
    scale = lumi*params[i.split("_"+channel)[0]]['xs']/params[i.split("_"+channel)[0]]['evt']
    h_dict[i_name].Scale(scale)
    if " x " in key: 
      h_dict[i_name].Scale(float(key.split(" x ")[0]))
      print "Scaling by ", float(key.split(" x ")[0])
    h_dict[i_name].Scale(1.0,"width")
    h_dict[key].Add(h_dict[i_name].Clone())
  h_dict[key].Print("all")


### Get ratio ###
if do_ratio:
  for key, val in files.items():
    if key != ratio_file:
      h_dict[key].Divide(h_dict[ratio_file].Clone())
  h_dict[ratio_file].Divide(h_dict[ratio_file].Clone())

### Find maximum and minimum entries ###
minimum = None
maximum = None
for key, val in h_dict.items():
  if minimum == None:
    minimum, maximum = val.GetBinContent(val.GetMaximumBin()), val.GetBinContent(val.GetMinimumBin())
  else:
    if val.GetBinContent(val.GetMaximumBin()) > maximum:
      maximum = val.GetBinContent(val.GetMaximumBin())
    if val.GetBinContent(val.GetMinimumBin()) < minimum:
      minimum = val.GetBinContent(val.GetMinimumBin())

total_range = (maximum - minimum)/10

minimum -= total_range
maximum += total_range

minimum = 0
maximum = 2

### Plot histograms ###

colours = [2,3,4,5,6,7,8,9,10]

c = ROOT.TCanvas('c','c',700,700)
#c.SetRightMargin(0.18)
#c.SetMargin(0.1, 0.2, 0.1, 0.1)
c.SetLeftMargin(0.15)
if logx: c.SetLogx()
if logy: c.SetLogy()
if logz: c.SetLogz()
if ":" in var_input: c.SetRightMargin(0.15)

do_options = True
i = 0
hist_2 = {}
for key, val in files.items():

  if draw_type_2 != None: hist_2[key] = h_dict[key].Clone()
  
  if do_options:
    h_dict[key].SetStats(0)
    h_dict[key].GetXaxis().SetTitle(x_label)
    h_dict[key].GetYaxis().SetTitle(y_label)
    h_dict[key].GetXaxis().SetTitleOffset(x_offset)
    h_dict[key].GetYaxis().SetTitleOffset(y_offset)
    h_dict[key].GetYaxis().SetRangeUser(minimum,maximum)
    if logx:
      h_dict[key].GetXaxis().SetMoreLogLabels()
      h_dict[key].GetXaxis().SetNoExponent()
    h_dict[key].Draw(draw_type)
    do_options = False
  if ":" not in var_input:
    h_dict[key].SetLineColor(colours[i])
    h_dict[key].SetMarkerColor(colours[i])
    h_dict[key].SetLineWidth(2)
    h_dict[key].SetMarkerSize(0)
    h_dict[key].SetFillColorAlpha(colours[i],0.3)
    if draw_type_2 != None:
      hist_2[key].SetMarkerSize(0)
      hist_2[key].SetLineWidth(2)
      hist_2[key].SetLineColor(colours[i])
      hist_2[key].Draw(draw_type_2+" same")
    if not do_options: h_dict[key].Draw(draw_type+" same")
  else: 
    h_dict[key].GetZaxis().SetTitle(z_label)
    h_dict[key].GetZaxis().SetTitleOffset(z_offset)

  i+=1

c.Update()

if ":" not in var_input:
  l = ROOT.TLegend(0.65,0.7,0.88,0.88)
  l.SetBorderSize(0)
  l.SetTextSize(0.025)
  for key, val in files.items():
    l.AddEntry(h_dict[key],key,'l')
  l.Draw()

if horiz_line != None:
  line = ROOT.TLine(bins[0],horiz_line,bins[-1],horiz_line)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw()
 
plotting.DrawTitle(c, '59.7 fb^{-1} (13 TeV)' % vars(), 3, scale=0.6)
plotting.DrawTitle(c, '#tau_{h}#tau_{h}' % vars(), 1, scale=0.6)

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextAngle(0)
latex.SetTextAlign(12)
latex.SetTextSize(0.025)
latex.SetTextColor(1)
latex.DrawLatex(0.65,0.65,under_legend_1)
latex.DrawLatex(0.65,0.6,under_legend_2)
latex.DrawLatex(0.65,0.55,under_legend_3)

c.Print(output_name)

os.system("display "+output_name)
