import ROOT
import os
import json
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting

### Input options ###

ntuple_name   = "ntuple"
#var_input     = "mt_tot[50,80,100,120,140,160,180,200,250,300,350,400,500,600,700,900,5000]"
var_input     = "mt_tot[50,80,100,120,140,160,180,200,250,300,350,400,500,600,700,900,2000]"
x_label       = "m_{T}^{tot} (GeV)"
y_label       = "< Events / GeV >"
do_ratio      = False
ratio_file    = ""
logx          = True
logy          = False
channel       = "tt"
output_name   = "vlq_signal_plot.pdf"
gU            = 1.5
under_legend_1= "VLQ BM 1"
under_legend_2= "m_{U} = 1 TeV, g_{U} = "+str(gU)
under_legend_3= ""
title_left    = "138 fb^{-1} (13 TeV)"
title_right   = "#tau_{h}#tau_{h}" 
lnN_uncerts   = [[1,1]]
wt_uncerts    = [[]]
#lnN_uncerts   = [[0.96,1.04]]
#wt_uncerts    = [["wt_mur1_muf0p5","wt_mur1_muf2"],["wt_ps_isr_down","wt_ps_isr_up"],["wt_ps_fsr_down","wt_ps_fsr_up"],["wt_vlq_2320","(2-wt_vlq_2320)"],["((0.887*(n_deepbjets==0))+(0.989*(n_deepbjets>0)))","((1.045*(n_deepbjets==0))+(1.038*(n_deepbjets>0)))"]]
turn_uncerts_off = True

uncerts_for = "all"
norm_to_bin_width = True

plotting.ModTDRStyle()
ROOT.TGaxis.SetExponentOffset(-0.06, 0.01, "y");

### Functions for loading in from ntuples ###

def DrawHistFromNtuple(input_folder,files,ntuple_name,var_input,sel,params_file=None,norm_to_bin_width=False,scale=None,change_name=None):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  if change_name != None: hout.SetName(change_name)
  for f_name in files:
    hout_temp = ROOT.TH1D('h','',len(bins)-1, bins)
    f = ROOT.TFile('%(input_folder)s/%(f_name)s' % vars())
    t = f.Get(ntuple_name)
    h = hout_temp.Clone()
    t.Draw('%(var)s>>h' % vars(),sel,'goff')
    hout_temp = t.GetHistogram()
    if params_file != None: 
      with open(params_file) as jsonfile: params = json.load(jsonfile)
      name = "_".join(f_name.split("_")[:-2])
      hout_temp.Scale(params[name]['xs']/params[name]['evt'])
    if scale != None: hout_temp.Scale(scale)
    if norm_to_bin_width: hout_temp.Scale(1.0,"width")
    if "interference" in f_name:
      hout_temp.Scale(gU**2)
    else:
      hout_temp.Scale(gU**4)
    hout.Add(hout_temp.Clone())
    del hout_temp
  return hout

def MakeUncertaintyBandsPlotable(h_key,h_down,h_up):
  for i in range(0,h_key.GetNbinsX()+2):
    h_up.SetBinError(i,(h_up.GetBinContent(i)-h_key.GetBinContent(i))/2)
    h_up.SetBinContent(i,(h_up.GetBinContent(i)+h_key.GetBinContent(i))/2)
    h_down.SetBinError(i,(h_down.GetBinContent(i)-h_key.GetBinContent(i))/2)
    h_down.SetBinContent(i,(h_down.GetBinContent(i)+h_key.GetBinContent(i))/2)
  return h_down, h_up

def GetUpAndDownUncertaintyHistograms(key, h_dict, lnN_uncerts=[[]]):
  # Total squared lnN uncertainty to add
  lnN_up = 0
  lnN_down = 0
  for uncert in lnN_uncerts:
    lnN_up += (max(uncert)-1)**2
    lnN_down += (1-min(uncert))**2

  h_up = h_dict[key].Clone()
  h_down = h_dict[key].Clone()

  h_up.SetName(key+"_up")
  h_down.SetName(key+"_down")

  # Set up and down histograms to current content +/- uncertainty
  for i in range(0,h_dict[key].GetNbinsX()+2):
    h_up.SetBinContent(i,h_dict[key].GetBinContent(i) + ( h_dict[key].GetBinError(i)**2 + lnN_up*(h_dict[key].GetBinContent(i)**2) )**0.5)
    h_down.SetBinContent(i,h_dict[key].GetBinContent(i) - (h_dict[key].GetBinError(i)**2 + lnN_down*(h_dict[key].GetBinContent(i)**2))**0.5 )
    h_up.SetBinError(i,0)
    h_down.SetBinError(i,0)
 
  # add in variations from weights
  for k, v in h_dict.items():
    if key+"_" in k and "_down":
      for i in range(0,h_dict[key].GetNbinsX()+2):
        up_content = max([h_dict[k].GetBinContent(i),h_dict[k.replace("down","up")].GetBinContent(i)])
        down_content = min([h_dict[k].GetBinContent(i),h_dict[k.replace("down","up")].GetBinContent(i)])
        h_up.SetBinContent(i,h_dict[key].GetBinContent(i) + ((h_up.GetBinContent(i)-h_dict[key].GetBinContent(i))**2 + (up_content-h_dict[key].GetBinContent(i))**2)**0.5 )
        h_down.SetBinContent(i,h_dict[key].GetBinContent(i) - ((h_down.GetBinContent(i)-h_dict[key].GetBinContent(i))**2 + (down_content-h_dict[key].GetBinContent(i))**2)**0.5 )

  return h_down, h_up

### Plotting functions ###

def FindMinMaxValForAxis(h_dict):
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
  maximum += 2*total_range
  return minimum, maximum

def SetupCanvas(do_ratio=False,logx=False,logy=False):
  c = ROOT.TCanvas('c','c',700,700)
  if do_ratio:
    c.cd()
    pad1 = ROOT.TPad("pad1","pad1",0,0.4,1,1)
    pad1.SetBottomMargin(0.01)
    if logx: pad1.SetLogx()
    if logy: pad1.SetLogy()
    pad1.Draw()
    c.cd()
    pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.39)
    pad2.SetTopMargin(0.02)
    pad2.SetBottomMargin(0.2)
    if logx: pad2.SetLogx()
    pad2.Draw()
    return c, pad1, pad2
  else:
    c.SetLeftMargin(0.15)
    if logx: c.SetLogx()
    if logy: c.SetLogy()
    return c

def DrawHorizontalLine(pad,horiz_line,var_input,width=2,style=2,color=1):
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  pad.cd()
  line = ROOT.TLine(bins[0],horiz_line,bins[-1],horiz_line)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw()
  pad.Update()


def DrawHistogramsWithBand(pad_name,h_dict,do_ratio=False,do_options=True, x_label=x_label, y_label=y_label,colours=[2,3,4,5,6,7,8,9,10],pad="top",no_uncerts=False):
  minimum, maximum = FindMinMaxValForAxis(h_dict)
  i = 0
  if no_uncerts: h_dict = RemoveUncerts(h_dict)
  for key, val in h_dict.items():
    if "_up" not in key and "_down" not in key:
      pad_name.cd()
  
      if do_options:
        h_dict[key+"_up"].SetStats(0)
        h_dict[key+"_up"].GetYaxis().SetRangeUser(minimum,maximum)
        if not do_ratio:
          h_dict[key+"_up"].GetXaxis().SetTitle(x_label)
          h_dict[key+"_up"].GetYaxis().SetTitleOffset(1.3)
          h_dict[key+"_up"].GetXaxis().SetTitleOffset(1.12)
          h_dict[key+"_up"].GetYaxis().SetTitle(y_label)
        else:
          if pad == "top":
            h_dict[key+"_up"].GetXaxis().SetLabelSize(0)
            h_dict[key+"_up"].GetYaxis().SetLabelSize(0.04)
            h_dict[key+"_up"].GetYaxis().SetTitleOffset(1.3)
            h_dict[key+"_up"].GetYaxis().SetTitleSize(0.055)
            h_dict[key+"_up"].GetYaxis().SetTitle(y_label)
            h_dict[key+"_up"].GetXaxis().SetTitleOffset(1.12)
          elif pad == "bottom":
            h_dict[key+"_up"].GetXaxis().SetTitle(x_label)
            h_dict[key+"_up"].GetYaxis().SetTitle("Ratio")
            h_dict[key+"_up"].GetYaxis().SetTitleOffset(0.6)
            h_dict[key+"_up"].GetYaxis().SetTitleSize(0.08)

            h_dict[key+"_up"].GetXaxis().SetTitleOffset(0)
            h_dict[key+"_up"].GetXaxis().SetTitleSize(0.08)

            h_dict[key+"_up"].GetXaxis().SetLabelSize(0.06)
            h_dict[key+"_up"].GetYaxis().SetLabelSize(0.06)
        if logx:
          h_dict[key+"_up"].GetXaxis().SetMoreLogLabels()
          h_dict[key+"_up"].GetXaxis().SetNoExponent()
  
        h_dict[key+"_up"].Draw("e2")
        h_dict[key+"_up"].SetLineWidth(0)
        h_dict[key+"_up"].SetMarkerSize(0)
        h_dict[key+"_up"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_up"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_up"].SetFillColorAlpha(colours[i],1.0)
        if no_uncerts:
          h_dict[key+"_up"].SetLineWidth(2)
          h_dict[key+"_up"].SetLineColor(colours[i])
          h_dict[key+"_up"].SetLineColor(1)

        if not no_uncerts: h_dict[key+"_down"].Draw("e2 same")
        h_dict[key+"_down"].SetLineWidth(0)
        h_dict[key+"_down"].SetMarkerSize(0)
        h_dict[key+"_down"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_down"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_down"].SetFillColorAlpha(colours[i],1.0)
  
      h_dict[key].SetLineWidth(2)
      #h_dict[key].SetLineColor(colours[i])
      h_dict[key].SetLineColor(1)
      h_dict[key].SetMarkerSize(0)
      h_dict[key].SetFillColorAlpha(colours[i],0.3)
      #h_dict[key].SetFillColorAlpha(colours[i],1.0)
      if not no_uncerts: h_dict[key].Draw("hist same")

      if not do_options:
        h_dict[key+"_up"].Draw("e2 same")
        h_dict[key+"_up"].SetLineWidth(0)
        h_dict[key+"_up"].SetMarkerSize(0)
        h_dict[key+"_up"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_up"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_up"].SetFillColorAlpha(colours[i],1.0)
        if no_uncerts:
          h_dict[key+"_up"].SetLineWidth(2)
          #h_dict[key+"_up"].SetLineColor(colours[i])
          h_dict[key+"_up"].SetLineColor(1)

  
        if not no_uncerts: h_dict[key+"_down"].Draw("e2 same")
        h_dict[key+"_down"].SetLineWidth(0)
        h_dict[key+"_down"].SetMarkerSize(0)
        h_dict[key+"_down"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_down"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_down"].SetFillColorAlpha(colours[i],1.0) 
 
      do_options = False
      i+=1
  pad_name.SetTickx(1)
  pad_name.SetTicky(1)
  pad_name.RedrawAxis() 
  #c.Update()

def DrawLegend(pad,h_dict,draw_type="l",posx1=0.6,posy1=0.7,posx2=0.88,posy2=0.88):
  pad.cd()
  l = ROOT.TLegend(posx1,posy1,posx2,posy2)
  l.SetBorderSize(0)
  if not do_ratio:
    l.SetTextSize(0.025)
  else:
    l.SetTextSize(0.04)
  for key, val in h_dict.items():
    if not "_down" in key and not "_up" in key:
      l.AddEntry(h_dict[key],key,draw_type)
  l.Draw()

def GetRatioHists(h_dict,ratio_file):
  h_ratio_dict = {}
  ratio_hist = h_dict[ratio_file].Clone()
  for i in range(0,ratio_hist.GetNbinsX()+2): ratio_hist.SetBinError(i,0)
  for key, val in h_dict.items(): 
    h_ratio_dict[key] = h_dict[key].Clone()
    h_ratio_dict[key].Divide(ratio_hist.Clone())
  return h_ratio_dict

def RemoveUncerts(h_dict):
  for key, val in h_dict.items():
    for i in range(0,val.GetNbinsX()+2):
      h_dict[key].SetBinError(i,0)
      if "_up" in key or "_down" in key:
        h_dict[key].SetBinContent(i,h_dict['_'.join(key.split("_")[:-1])].GetBinContent(i))
  return h_dict

### Get histograms from ROOT files ###
h_dict = {}

h_dict_2016 = {}
h_dict_2017 = {}
h_dict_2018 = {}

lumi_2016 = 59740 
lumi_2017 = 41530 
lumi_2018 = 36330 

input_folder_2016  = "/vols/cms/gu18/Offline/output/MSSM/vlq_2016_pdf_full"
input_folder_2017  = "/vols/cms/gu18/Offline/output/MSSM/vlq_2017_pdf_full"
input_folder_2018  = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_pdf_full"

params_file_2016   = "scripts/params_vlq_2016.json"
params_file_2017   = "scripts/params_vlq_2017.json"
params_file_2018   = "scripts/params_vlq_2018.json"

sel_2016 = "wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>120 && trg_singletau_1) || (pt_2>120 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"
sel_2017 = "wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"
sel_2018 = "wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"

files_2016 = {
         "Signal":["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2016.root"],
         "Interference":["VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2016.root","VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2016.root"],
         "Combined"    :["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2016.root","VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2016.root","VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2016.root"]
        }
files_2017 = {
         "Signal":["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2017.root"],
         "Interference":["VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2017.root","VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2017.root"],
         "Combined"    :["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2017.root","VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2017.root","VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2017.root"]
        }
files_2018 = {
         "Signal":["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root"],
         "Interference":["VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2018.root","VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2018.root"],
         "Combined"    :["VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root","VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference_tt_2018.root","VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference_tt_2018.root"]
        }

norm_scale = 0
for key, val in files_2016.items():
  h_dict_2016[key] = DrawHistFromNtuple(input_folder_2016,files_2016[key],ntuple_name,var_input,sel_2016,params_file=params_file_2016,norm_to_bin_width=norm_to_bin_width,scale=lumi_2016,change_name=key)
  h_dict_2017[key] = DrawHistFromNtuple(input_folder_2017,files_2017[key],ntuple_name,var_input,sel_2017,params_file=params_file_2017,norm_to_bin_width=norm_to_bin_width,scale=lumi_2017,change_name=key)
  h_dict_2018[key] = DrawHistFromNtuple(input_folder_2018,files_2018[key],ntuple_name,var_input,sel_2018,params_file=params_file_2018,norm_to_bin_width=norm_to_bin_width,scale=lumi_2018,change_name=key)
  h_dict[key] = h_dict_2016[key].Clone()
  h_dict[key].Add(h_dict_2017[key].Clone())
  h_dict[key].Add(h_dict_2018[key].Clone())
  if key != "Combined": norm_scale += h_dict[key].Integral()
 
### Get additional uncertainty histogram ###

for wt_list in wt_uncerts:
  for wt in wt_list:
    for key, val in files_2016.items():
      if uncerts_for == "all" or key in uncerts_for:
        h_dict_2016[key+"_"+wt] = DrawHistFromNtuple(input_folder_2016,files_2016[key],ntuple_name,var_input,wt+"*("+sel_2016+")",params_file=params_file_2016,norm_to_bin_width=norm_to_bin_width,scale=lumi_2016,change_name=key+"_"+wt)
        h_dict_2017[key+"_"+wt] = DrawHistFromNtuple(input_folder_2017,files_2017[key],ntuple_name,var_input,wt+"*("+sel_2017+")",params_file=params_file_2017,norm_to_bin_width=norm_to_bin_width,scale=lumi_2017,change_name=key+"_"+wt)
        h_dict_2018[key+"_"+wt] = DrawHistFromNtuple(input_folder_2018,files_2018[key],ntuple_name,var_input,wt+"*("+sel_2018+")",params_file=params_file_2018,norm_to_bin_width=norm_to_bin_width,scale=lumi_2018,change_name=key+"_"+wt)
        h_dict[key+"_"+wt] = h_dict_2016[key+"_"+wt].Clone()
        h_dict[key+"_"+wt].Add(h_dict_2017[key+"_"+wt].Clone())
        h_dict[key+"_"+wt].Add(h_dict_2018[key+"_"+wt].Clone())

#if True:
#  for key, val in h_dict.items():
#    h_dict[key].Scale(1/norm_scale)



### Make up and down histograms ###
h_new_dict = {}
h_plot_dict = {}
for key, val in files_2016.items():
  h_new_dict[key+"_down"],h_new_dict[key+"_up"] = GetUpAndDownUncertaintyHistograms(key, h_dict, lnN_uncerts=lnN_uncerts)
  h_new_dict[key] = h_dict[key].Clone()
  h_plot_dict[key] = h_dict[key].Clone()

  # Make plotable histogram with different up and down uncertainties
  h_plot_dict[key+"_down"], h_plot_dict[key+"_up"] = MakeUncertaintyBandsPlotable(h_dict[key].Clone(),h_new_dict[key+"_down"].Clone(),h_new_dict[key+"_up"].Clone())

if do_ratio:
  c, pad1, pad2 = SetupCanvas(do_ratio=do_ratio,logx=logx,logy=logy)
else:
  pad1 = SetupCanvas(do_ratio=do_ratio,logx=logx,logy=logy)

#if turn_uncerts_off: h_plot_dict = RemoveUncerts(h_plot_dict)

#DrawHistogramsWithBand(pad1,h_plot_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label,pad="top",no_uncerts=turn_uncerts_off,colours=[ROOT.TColor.GetColor(100, 192, 232),ROOT.TColor.GetColor(155, 152, 204),ROOT.TColor.GetColor(248, 206, 104),ROOT.TColor.GetColor(222, 90, 106),ROOT.TColor.GetColor(250, 202, 255),ROOT.TColor.GetColor(222, 90, 106),ROOT.TColor.GetColor(192, 232, 100)])
DrawHistogramsWithBand(pad1,h_plot_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label,pad="top",no_uncerts=turn_uncerts_off)

#DrawHorizontalLine(pad1,0,var_input,width=2,style=2,color=1)
bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
line = ROOT.TLine(bins[0],0,bins[-1],0)
line.SetLineWidth(2)
line.SetLineStyle(2)
line.SetLineColor(1)
line.Draw()


#DrawLegend(pad1,h_plot_dict,draw_type="l",posx1=0.6,posy1=0.7,posx2=0.88,posy2=0.88)
l = ROOT.TLegend(0.62,0.2,0.95,0.4)
l.SetBorderSize(0)
if not do_ratio:
  l.SetTextSize(0.025)
else:
  l.SetTextSize(0.04)
for key, val in h_plot_dict.items():
  if not "_down" in key and not "_up" in key:
    if key != "Combined":
      l.AddEntry(val,key,"f")
    else:
      l.AddEntry(val,"Signal + interference","f")
l.Draw()

# Draw titles and text on plot
 
if not do_ratio:
  plotting.DrawTitle(pad1, title_right, 1, scale=1)
  plotting.DrawTitle(pad1, title_left, 3, scale=1)
else:
  plotting.DrawTitle(pad1, title_right, 1, scale=1)
  plotting.DrawTitle(pad1, title_left, 3, scale=1)

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextAngle(0)
latex.SetTextAlign(12)
latex.SetTextSize(0.03)
latex.SetTextColor(1)
latex.DrawLatex(0.65,0.88,under_legend_1)
latex.DrawLatex(0.65,0.83,under_legend_2)
latex.DrawLatex(0.65,0.78,under_legend_3)

plotting.DrawCMSLogo(pad1, 'CMS', 'Simulation', 11, 0.045, 0.03, 1.0, '', 1.0)

# Draw ratio

if do_ratio:

  h_ratio_dict = GetRatioHists(h_new_dict,ratio_file)
  h_plot_ratio_dict = {}
  for key, val in files_2016.items():
    h_plot_ratio_dict[key] = h_ratio_dict[key].Clone()
    h_plot_ratio_dict[key+"_down"], h_plot_ratio_dict[key+"_up"] = MakeUncertaintyBandsPlotable(h_ratio_dict[key].Clone(),h_ratio_dict[key+"_down"].Clone(),h_ratio_dict[key+"_up"].Clone())

  DrawHistogramsWithBand(pad2,h_plot_ratio_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label, pad="bottom")

if do_ratio:
  c.Print(output_name)
else:
  pad1.Print(output_name)
