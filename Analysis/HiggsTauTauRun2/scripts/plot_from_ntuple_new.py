import ROOT
import os
import json
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting

### Input options ###

input_folder  = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_pdf"
params_file   = "scripts/params_vlq_2018.json"
ntuple_name   = "ntuple"
#var_input     = "n_deepbjets[0,1,2,3,4]"
#x_label        = "N_{b-jets}"
#y_label       = "dN/dN_{b-jets}"
#var_input     = "mt_tot[50,80,100,120,140,160,180,200,250,300,350,400,500,600,700,900,5000]"
var_input     = "mt_tot[120,300]"
x_label       = "m_{T}^{tot} (GeV)"
y_label       = "dN/dm_{T}^{tot} (1/GeV)"
#sel           = "wt*wt_vlq_off_diag_0*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"
sel           = "wt*wt_vlq_off_diag_0*wt_tau_trg_mssm*wt_tau_id_mssm*(n_deepbjets==0)*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"
do_ratio      = True
#ratio_file    = "Matched"
ratio_file    = "HIG-21-001"
logx          = False
logy          = False
channel       = "tt"
output_name   = "exo_sample_comparison.pdf"
draw_type     = "e2"
draw_type_2   = "hist"
lumi          = 59740
horiz_line    = None
norm          = False
under_legend_1= ""
under_legend_2= ""
under_legend_3= ""
title_left    = "59.7 fb^{-1} (13 TeV)"
title_right   = "#tau_{h}#tau_{h}" 
#lnN_uncerts   = [[0.96,1.04],[0.989,1.045]] # nobtag
#lnN_uncerts   = [[0.989,1.045]] # nobtag
#lnN_uncerts = []
#lnN_uncerts   = [[0.96,1.04],[1.038,0.887]] # btag
lnN_uncerts   = [[0.96,1.04]]
#lnN_uncerts   = [[1.038,0.887]] # btag
#wt_uncerts    = [["wt_mur1_muf0p5","wt_mur1_muf2"]]
#wt_uncerts    = [["wt_mur1_muf0p5","wt_mur1_muf2"],["wt_ps_isr_down","wt_ps_isr_up"],["wt_ps_fsr_down","wt_ps_fsr_up"]]
#wt_uncerts    = [["wt_mur1_muf0p5","wt_mur1_muf2"],["wt_ps_isr_down","wt_ps_isr_up"],["wt_ps_fsr_down","wt_ps_fsr_up"],["wt_vlq_2320","(2-wt_vlq_2320)"]]
wt_uncerts    = [["wt_mur1_muf0p5","wt_mur1_muf2"],["wt_ps_isr_down","wt_ps_isr_up"],["wt_ps_fsr_down","wt_ps_fsr_up"],["wt_vlq_2320","(2-wt_vlq_2320)"],["((0.887*(n_deepbjets==0))+(0.989*(n_deepbjets>0)))","((1.045*(n_deepbjets==0))+(1.038*(n_deepbjets>0)))"]]
#wt_uncerts = []
uncerts_for = "HIG-21-001"
#uncerts_for  = "all"
do_bar_for_non_ratio = True


### Input files ###

files = {
         "HIG-21-001":["VectorLQToTauTau_betaRd33_0_mU2000_gU1_matched_tt_2018.root"],
         "EX0-19-016":["exo_vlq_sample_tt_2018.root"]
        }

#files = {
#         "Matched":["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root"],
#         "Unmatched":["VectorLQToTauTau_betaRd33_0_mU1000_gU1_tt_2018.root"]
#        }



### Get histograms from ROOT files ###

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
    t.Draw('%(var)s>>h' % vars(),sel,'goff') 
    h_dict[i_name] = t.GetHistogram()
    scale = lumi*params[i.split("_"+channel)[0]]['xs']/params[i.split("_"+channel)[0]]['evt']
    h_dict[i_name].Scale(scale)
    h_dict[i_name].Scale(1.0,"width")
    if norm: h_dict[i_name].Scale(1/h_dict[i_name].Integral()) 
    h_dict[key].Add(h_dict[i_name].Clone())
  print key, h_dict[key].Integral()

### Add additional uncertainty histogram ###

for wt_list in wt_uncerts:
  for wt in wt_list:
    for key, val in files.items():

      if uncerts_for == "all" or key in uncerts_for:
    
        with open(params_file) as jsonfile:
          params = json.load(jsonfile)
      
        if type(val) == str: val = [val]
      
        h_dict[key+'_'+wt] = hout.Clone()
        h_dict[key+'_'+wt].SetName(key+'_'+wt)
        for i in val:
          i_name = i+"_"+key+"_"+wt
          f[i_name] = ROOT.TFile('%(input_folder)s/%(i)s' % vars())
          t = f[i_name].Get(ntuple_name)
          h_dict[i_name] = hout.Clone()
          h_dict[i_name].SetName("h")
          t.Draw('%(var)s>>h' % vars(),wt+"*("+sel+")",'goff')
          h_dict[i_name] = t.GetHistogram()
          scale = lumi*params[i.split("_"+channel)[0]]['xs']/params[i.split("_"+channel)[0]]['evt']
          h_dict[i_name].Scale(scale)
          h_dict[i_name].Scale(1.0,"width")
          if norm: h_dict[i_name].Scale(1/h_dict[i_name].Integral())
          h_dict[key+'_'+wt].Add(h_dict[i_name].Clone())

# Total squared lnN uncertainty to add
lnN_up = 0
lnN_down = 0
for uncert in lnN_uncerts:
  lnN_up += (max(uncert)-1)**2
  lnN_down += (1-min(uncert))**2

for key, val in files.items():

  h_dict[key+"_up"] = h_dict[key].Clone()
  h_dict[key+"_down"] = h_dict[key].Clone()

  if uncerts_for == "all" or key in uncerts_for:

    # Set up and down histograms to current content +/- uncertainty
    for i in range(0,h_dict[key].GetNbinsX()+2):
      h_dict[key+"_up"].SetBinContent(i,h_dict[key].GetBinContent(i) + ( h_dict[key].GetBinError(i)**2 + lnN_up*(h_dict[key].GetBinContent(i)**2) )**0.5)
      h_dict[key+"_down"].SetBinContent(i,h_dict[key].GetBinContent(i) - (h_dict[key].GetBinError(i)**2 + lnN_down*(h_dict[key].GetBinContent(i)**2))**0.5 )
      h_dict[key+"_up"].SetBinError(i,0)
      h_dict[key+"_down"].SetBinError(i,0)

    # add in variations from weights
    for wt_list in wt_uncerts:
      for i in range(0,h_dict[key].GetNbinsX()+2):
        up_content = max([h_dict[key+"_"+wt_list[0]].GetBinContent(i),h_dict[key+"_"+wt_list[1]].GetBinContent(i)])
        down_content = min([h_dict[key+"_"+wt_list[0]].GetBinContent(i),h_dict[key+"_"+wt_list[1]].GetBinContent(i)])
        h_dict[key+"_up"].SetBinContent(i,h_dict[key].GetBinContent(i) + ((h_dict[key+"_up"].GetBinContent(i)-h_dict[key].GetBinContent(i))**2 + (up_content-h_dict[key].GetBinContent(i))**2)**0.5 )
        h_dict[key+"_down"].SetBinContent(i,h_dict[key].GetBinContent(i) - ((h_dict[key+"_down"].GetBinContent(i)-h_dict[key].GetBinContent(i))**2 + (down_content-h_dict[key].GetBinContent(i))**2)**0.5 )


    # Set up and down variations so they are plot able
    for i in range(0,h_dict[key].GetNbinsX()+2):
      h_dict[key+"_up"].SetBinError(i,(h_dict[key+"_up"].GetBinContent(i)-h_dict[key].GetBinContent(i))/2)
      h_dict[key+"_up"].SetBinContent(i,(h_dict[key+"_up"].GetBinContent(i)+h_dict[key].GetBinContent(i))/2)
      h_dict[key+"_down"].SetBinError(i,(h_dict[key+"_down"].GetBinContent(i)-h_dict[key].GetBinContent(i))/2)
      h_dict[key+"_down"].SetBinContent(i,(h_dict[key+"_down"].GetBinContent(i)+h_dict[key].GetBinContent(i))/2)

  else:
    for i in range(0,h_dict[key].GetNbinsX()+2):
      h_dict[key+"_up"].SetBinError(i,(h_dict[key].GetBinError(i)/2))
      h_dict[key+"_up"].SetBinContent(i,(2*h_dict[key].GetBinContent(i)+h_dict[key].GetBinError(i))/2)
      h_dict[key+"_down"].SetBinError(i,(h_dict[key].GetBinError(i)/2))
      h_dict[key+"_down"].SetBinContent(i,(2*h_dict[key].GetBinContent(i)-h_dict[key].GetBinError(i))/2)



### Find maximum and minimum entries ###
minimum = None
maximum = None
for key, val in h_dict.items():
  if "ratio" not in key:
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

### Plot histograms ###

colours = [2,1,3,4,5,6,7,8,9,10]
#colours = [2,3,4,5,6,7,8,9,10]

c = ROOT.TCanvas('c','c',700,700)
if do_ratio:
  pad1 = ROOT.TPad("pad1","pad1",0,0.4,1,1)
  pad1.SetBottomMargin(0.01)
  if logx: pad1.SetLogx()
  if logy: pad1.SetLogy()
  pad1.Draw()
  pad1.cd()
else:
  c.SetLeftMargin(0.15)
  if logx: c.SetLogx()
  if logy: c.SetLogy()

do_options = True
i = 0
for key, val in files.items():
   
  if do_options:
    h_dict[key+"_up"].SetStats(0)
    if not do_ratio: 
      h_dict[key+"_up"].GetXaxis().SetTitle(x_label)
    else:
      h_dict[key+"_up"].GetXaxis().SetLabelSize(0)
    h_dict[key+"_up"].GetYaxis().SetTitle(y_label)
    h_dict[key+"_up"].GetXaxis().SetTitleOffset(0)
    if not do_ratio:
      h_dict[key+"_up"].GetYaxis().SetTitleOffset(1.8)
    else:
      h_dict[key+"_up"].GetYaxis().SetTitleOffset(0.9)
      h_dict[key+"_up"].GetYaxis().SetTitleSize(0.055)
    h_dict[key+"_up"].GetYaxis().SetRangeUser(minimum,maximum)
    if logx:
      h_dict[key+"_up"].GetXaxis().SetMoreLogLabels()
      h_dict[key+"_up"].GetXaxis().SetNoExponent()

    if not do_bar_for_non_ratio or key == ratio_file:
      h_dict[key+"_up"].Draw(draw_type)
      h_dict[key+"_up"].SetLineWidth(0)
      h_dict[key+"_up"].SetMarkerSize(0)
      h_dict[key+"_up"].SetMarkerColorAlpha(colours[i],0)
      h_dict[key+"_up"].SetFillColorAlpha(colours[i],0.3)

      h_dict[key+"_down"].Draw(draw_type+" same")
      h_dict[key+"_down"].SetLineWidth(0)
      h_dict[key+"_down"].SetMarkerSize(0)
      h_dict[key+"_down"].SetMarkerColorAlpha(colours[i],0)
      h_dict[key+"_down"].SetFillColorAlpha(colours[i],0.3)

  if draw_type_2 != None:
    h_dict[key].SetLineWidth(2)
    h_dict[key].SetLineColor(colours[i])
    if not do_bar_for_non_ratio or key == ratio_file:
      h_dict[key].SetMarkerSize(0)
      h_dict[key].Draw(draw_type_2+" same")
    else:
      h_dict[key].SetMarkerColor(colours[i])
      h_dict[key].SetMarkerStyle(8)
      h_dict[key].Draw("E same")
  if not do_options and (not do_bar_for_non_ratio or key == ratio_file): 
    h_dict[key+"_up"].Draw(draw_type+" same")
    h_dict[key+"_up"].SetLineWidth(0)
    h_dict[key+"_up"].SetMarkerSize(0)
    h_dict[key+"_up"].SetMarkerColorAlpha(colours[i],0)
    h_dict[key+"_up"].SetFillColorAlpha(colours[i],0.3)

    h_dict[key+"_down"].Draw(draw_type+" same")
    h_dict[key+"_down"].SetLineWidth(0)
    h_dict[key+"_down"].SetMarkerSize(0)
    h_dict[key+"_down"].SetMarkerColorAlpha(colours[i],0)
    h_dict[key+"_down"].SetFillColorAlpha(colours[i],0.3)


  do_options = False

  i+=1

#c.Update()

#l = ROOT.TLegend(0.65,0.7,0.88,0.88)
l = ROOT.TLegend(0.6,0.7,0.88,0.88)
l.SetBorderSize(0)
if not do_ratio:
  l.SetTextSize(0.025)
else:
  l.SetTextSize(0.04)
for key, val in files.items():
  if not do_bar_for_non_ratio or key == ratio_file:
    l.AddEntry(h_dict[key],key,'l')
  else:
    l.AddEntry(h_dict[key],key,'lep')
l.Draw()

if horiz_line != None:
  line = ROOT.TLine(bins[0],horiz_line,bins[-1],horiz_line)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw()
 
if not do_ratio:
  plotting.DrawTitle(c, title_right, 3, scale=0.6)
  plotting.DrawTitle(c, title_left, 1, scale=0.6)
else:
  plotting.DrawTitle(pad1, title_right, 3, scale=1)
  plotting.DrawTitle(pad1, title_left, 1, scale=1)

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextAngle(0)
latex.SetTextAlign(12)
latex.SetTextSize(0.025)
latex.SetTextColor(1)
latex.DrawLatex(0.62,0.65,under_legend_1)
latex.DrawLatex(0.62,0.6,under_legend_2)
latex.DrawLatex(0.62,0.55,under_legend_3)

# Draw ratio

if do_ratio:

  ratio_file_no_error = h_dict[ratio_file].Clone()
  for i in range(0,ratio_file_no_error.GetNbinsX()+2): ratio_file_no_error.SetBinError(i,0)

  for key, val in files.items():
     

    h_dict[key+"_ratio"] = h_dict[key].Clone()
    h_dict[key+"_ratio"].Divide(ratio_file_no_error.Clone())

    h_dict[key+"_up_ratio"] = h_dict[key].Clone()
    h_dict[key+"_down_ratio"] = h_dict[key].Clone()

    for i in range(0,h_dict[key].GetNbinsX()+2):
      up_num_uncert = 2*h_dict[key+"_up"].GetBinError(i) 
      down_num_uncert = 2*h_dict[key+"_down"].GetBinError(i)
      num_val = h_dict[key+"_up"].GetBinContent(i) - h_dict[key+"_up"].GetBinError(i)

      up_denom_uncert = 2*h_dict[ratio_file+"_up"].GetBinError(i)
      down_denom_uncert = 2*h_dict[ratio_file+"_down"].GetBinError(i)
      denom_val = h_dict[ratio_file+"_up"].GetBinContent(i) - h_dict[ratio_file+"_up"].GetBinError(i)

      if num_val != 0 and denom_val != 0:
		      #up_uncert = ( (up_num_uncert/num_val)**2 + (up_denom_uncert/denom_val)**2 )**0.5
          up_uncert = up_num_uncert/num_val
		      #down_uncert = ( (down_num_uncert/num_val)**2 + (down_denom_uncert/denom_val)**2 )**0.5
          down_uncert = down_num_uncert/num_val
      else:
        up_uncert = 0
        down_uncert = 0
      nom_val = h_dict[key+"_ratio"].GetBinContent(i)

      h_dict[key+"_up_ratio"].SetBinContent(i,nom_val+(up_uncert/2))
      h_dict[key+"_down_ratio"].SetBinContent(i,nom_val-(down_uncert/2))
      h_dict[key+"_up_ratio"].SetBinError(i,up_uncert/2)
      h_dict[key+"_down_ratio"].SetBinError(i,down_uncert/2)


  ### Find maximum and minimum entries ###
  minimum = None
  maximum = None
  for key, val in h_dict.items():
    if "ratio" in key:
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


  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.39)
  pad2.SetTopMargin(0.02)
  pad2.SetBottomMargin(0.2)
  if logx: pad2.SetLogx()
  pad2.Draw()
  pad2.cd()

  do_options = True
  i = 0
  for key, val in files.items():
  
    if do_options:
      h_dict[key+"_up_ratio"].SetStats(0)
      h_dict[key+"_up_ratio"].GetXaxis().SetTitle(x_label)
      h_dict[key+"_up_ratio"].GetYaxis().SetTitle("Ratio")
      h_dict[key+"_up_ratio"].GetYaxis().SetTitleOffset(0.5)
      h_dict[key+"_up_ratio"].GetYaxis().SetTitleSize(0.08)

      h_dict[key+"_up_ratio"].GetXaxis().SetTitleOffset(0)
      h_dict[key+"_up_ratio"].GetXaxis().SetTitleSize(0.08)
      h_dict[key+"_up_ratio"].GetYaxis().SetRangeUser(minimum,maximum)

      h_dict[key+"_up_ratio"].GetXaxis().SetLabelSize(0.06)
      h_dict[key+"_up_ratio"].GetYaxis().SetLabelSize(0.06)

      if logx:
        h_dict[key+"_up_ratio"].GetXaxis().SetMoreLogLabels()
        h_dict[key+"_up_ratio"].GetXaxis().SetNoExponent()

      if not do_bar_for_non_ratio or key == ratio_file:
        h_dict[key+"_up_ratio"].Draw(draw_type)
        h_dict[key+"_up_ratio"].SetLineWidth(0)
        h_dict[key+"_up_ratio"].SetMarkerSize(0)
        h_dict[key+"_up_ratio"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_up_ratio"].SetFillColorAlpha(colours[i],0.3)
  
        h_dict[key+"_down_ratio"].Draw(draw_type+"_same")
        h_dict[key+"_down_ratio"].SetLineWidth(0)
        h_dict[key+"_down_ratio"].SetMarkerSize(0)
        h_dict[key+"_down_ratio"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_down_ratio"].SetFillColorAlpha(colours[i],0.3)

    #h_dict[key+"_ratio"].SetLineColor(colours[i])
    #h_dict[key+"_ratio"].SetMarkerColor(colours[i])
    #h_dict[key+"_ratio"].SetLineWidth(2)
    #h_dict[key+"_ratio"].SetMarkerSize(0)
    #_dict[key+"_ratio"].SetFillColorAlpha(colours[i],0.3)
    if draw_type_2 != None:
      h_dict[key+"_ratio"].SetLineWidth(2)
      h_dict[key+"_ratio"].SetLineColor(colours[i])
      if not do_bar_for_non_ratio or key == ratio_file:
        h_dict[key+"_ratio"].SetMarkerSize(0)
        h_dict[key+"_ratio"].Draw(draw_type_2+" same")
      else:
        h_dict[key+"_ratio"].SetMarkerColor(colours[i])
        h_dict[key+"_ratio"].SetMarkerStyle(8)
        h_dict[key+"_ratio"].Draw("E same")
    if not do_options and (not do_bar_for_non_ratio or key == ratio_file):
      h_dict[key+"_up_ratio"].Draw(draw_type+" same")
      h_dict[key+"_up_ratio"].SetLineWidth(0)
      h_dict[key+"_up_ratio"].SetMarkerSize(0)
      h_dict[key+"_up_ratio"].SetMarkerColorAlpha(colours[i],0)
      h_dict[key+"_up_ratio"].SetFillColorAlpha(colours[i],0.3)
  
      h_dict[key+"_down_ratio"].Draw(draw_type+" same")
      h_dict[key+"_down_ratio"].SetLineWidth(0)
      h_dict[key+"_down_ratio"].SetMarkerSize(0)
      h_dict[key+"_down_ratio"].SetMarkerColorAlpha(colours[i],0)
      h_dict[key+"_down_ratio"].SetFillColorAlpha(colours[i],0.3)

      #h_dict[key+"_ratio"].Draw(draw_type+" same")
    do_options = False
    i+=1

#c.Update()

c.Print(output_name)
