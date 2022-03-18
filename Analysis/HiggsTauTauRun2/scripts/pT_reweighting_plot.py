import ROOT
import os
import json
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting

### Input options ###

ntuple_name   = "ntuple"
var_input     = "pt_1[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600]"
x_label       = "p_{T} (GeV)"
y_label       = " < Events / GeV >"
do_ratio      = False
ratio_file    = ""
logx          = False
logy          = False
channel       = "tt"
output_name   = "pT_reweighting_plot.pdf"
tanb          = 30
higgs         = "A"
mass          = 1600
under_legend_1= "MSSM M_{h}^{125} scenario" % vars()
under_legend_2= "#phi = %(higgs)s" % vars()
under_legend_3= "m_{%(higgs)s} = %(mass)i GeV" % vars()
under_legend_4= "tan#beta = %(tanb)i" % vars()
title_left    = "13 TeV"
title_right   = "" 
lnN_uncerts   = [[1,1]]
wt_uncerts    = [[]]
turn_uncerts_off = True

uncerts_for = "all"
norm_to_bin_width = True

model_file = "/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/data/mh125_13.root"
reweight_file = "/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/data/higgs_pt_reweighting_fullRun2_v2.root"

mf = ROOT.TFile(model_file)
rescale_b_hist = mf.Get("rescale_gb_%(higgs)s" % vars())
rescale_t_hist = mf.Get("rescale_gt_%(higgs)s" % vars())
yb = rescale_b_hist.Interpolate(float(mass),float(tanb))
yt = rescale_t_hist.Interpolate(float(mass),float(tanb))

rf = ROOT.TFile(reweight_file)
w = rf.Get("w")
w.var("Yb_MSSM_%(higgs)s" % vars()).setVal(yb)
w.var("Yt_MSSM_%(higgs)s" % vars()).setVal(yt)
w.var("m%(higgs)s" % vars()).setVal(mass)
frac = {}
frac["b"] = w.function("gg%(higgs)s_b_MSSM_frac" % vars()).getVal()
frac["t"] = w.function("gg%(higgs)s_t_MSSM_frac" % vars()).getVal()
frac["i"] = w.function("gg%(higgs)s_i_MSSM_frac" % vars()).getVal()


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
  if not minimum == 0: minimum -= total_range
  maximum += 2*total_range
  return minimum, maximum

def SetupCanvas(do_ratio=False,logx=False,logy=False):
  c = ROOT.TCanvas('c','c',700,700)
  if do_ratio:
    c.cd()
    pad1 = ROOT.TPad("pad1","pad1",0,0.4,1,1)
    pad1.SetBottomMargin(0.02)
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
    #c.SetBottomMargin(-0.15)
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


def DrawHistogramsWithBand(pad_name,h_dict,do_ratio=False,do_options=True, x_label=x_label, y_label=y_label,colours=[4,2,3,94,5,6,7,8,9,10],pad="top",no_uncerts=False):
  minimum, maximum = FindMinMaxValForAxis(h_dict)
  i = 0
  if no_uncerts: h_dict = RemoveUncerts(h_dict)
  #for key, val in h_dict.items():
  for key in ["t + b + t b-interference","b-only","i-only","t-only"]:
    if "_up" not in key and "_down" not in key:
      pad_name.cd()
      if do_options:
        h_dict[key+"_up"].SetStats(0)
        h_dict[key+"_up"].GetYaxis().SetRangeUser(0,maximum)
        h_dict[key+"_up"].GetXaxis().SetRangeUser(0,400)
        if not do_ratio:
          h_dict[key+"_up"].GetXaxis().SetTitle(x_label)
          #h_dict[key+"_up"].GetYaxis().SetTitleOffset(1.5)
          h_dict[key+"_up"].GetYaxis().SetTitleOffset(1.3)
          h_dict[key+"_up"].GetXaxis().SetTitleOffset(1.12)
          h_dict[key+"_up"].GetYaxis().SetTitle(y_label)
          #h_dict[key+"_up"].GetXaxis().SetTitleSize(0.05)
          #h_dict[key+"_up"].GetYaxis().SetTitleSize(0.05)
        else:
          if pad == "top":
            h_dict[key+"_up"].GetXaxis().SetLabelSize(0)
            h_dict[key+"_up"].GetYaxis().SetLabelSize(0.04)
            h_dict[key+"_up"].GetYaxis().SetTitleOffset(0.9)
            h_dict[key+"_up"].GetYaxis().SetTitleSize(0.055)
            h_dict[key+"_up"].GetYaxis().SetTitle(y_label)
            h_dict[key+"_up"].GetXaxis().SetTitleOffset(0)
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
        #h_dict[key+"_up"].SetFillColorAlpha(colours[i],1)
        if no_uncerts:
          #h_dict[key+"_up"].SetLineWidth(2)
          #h_dict[key+"_up"].SetLineColor(colours[i])
          h_dict[key+"_up"].SetLineWidth(2)
          h_dict[key+"_up"].SetLineColor(1)
 
        if not no_uncerts: h_dict[key+"_down"].Draw("e2 same")
        h_dict[key+"_down"].SetLineWidth(0)
        h_dict[key+"_down"].SetMarkerSize(0)
        h_dict[key+"_down"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_down"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_down"].SetFillColorAlpha(colours[i],1)
  
  
      #h_dict[key].SetLineWidth(2)
      #h_dict[key].SetLineColor(colours[i])
      h_dict[key].SetLineWidth(2)
      h_dict[key].SetLineColor(1)
      h_dict[key].SetMarkerSize(0)
      h_dict[key].SetFillColorAlpha(colours[i],0.3)
      #h_dict[key].SetFillColorAlpha(colours[i],1)
      if not no_uncerts: h_dict[key].Draw("hist same")

      if not do_options:
        h_dict[key+"_up"].Draw("e2 same")
        h_dict[key+"_up"].SetLineWidth(0)
        h_dict[key+"_up"].SetMarkerSize(0)
        h_dict[key+"_up"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_up"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_up"].SetFillColorAlpha(colours[i],1)
        if no_uncerts:
          #h_dict[key+"_up"].SetLineWidth(2)
          #h_dict[key+"_up"].SetLineColor(colours[i])
          h_dict[key+"_up"].SetLineWidth(2)
          h_dict[key+"_up"].SetLineColor(1)


  
        if not no_uncerts: h_dict[key+"_down"].Draw("e2 same")
        h_dict[key+"_down"].SetLineWidth(0)
        h_dict[key+"_down"].SetMarkerSize(0)
        h_dict[key+"_down"].SetMarkerColorAlpha(colours[i],0)
        h_dict[key+"_down"].SetFillColorAlpha(colours[i],0.3)
        #h_dict[key+"_down"].SetFillColorAlpha(colours[i],1)

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

input_folder_2016  = "/vols/cms/dw515/outputs/MSSM/mssm_2016/"
input_folder_2017  = "/vols/cms/dw515/outputs/MSSM/mssm_2017/"
input_folder_2018  = "/vols/cms/dw515/outputs/MSSM/mssm_2018/"

params_file_2016   = "scripts/params_mssm_2016.json"
params_file_2017   = "scripts/params_mssm_2017.json"
params_file_2018   = "scripts/params_mssm_2018.json"

sel_2016 = "wt_gg%(higgs)s_LOOP*wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>120 && trg_singletau_1) || (pt_2>120 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)" % vars()
sel_2017 = "wt_gg%(higgs)s_LOOP*wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)" % vars()
sel_2018 = "wt_gg%(higgs)s_LOOP*wt*wt_tau_trg_mssm*wt_tau_id_mssm*(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)" % vars()

files = {
         "t-only":["mssm_pt_weight_inputs.root"],
         "b-only":["mssm_pt_weight_inputs.root"],
         "i-only":["mssm_pt_weight_inputs.root"],
        }

norm_scale = 0
hf = ROOT.TFile("mssm_pt_weight_inputs.root")
for key, val in files.items():
  loop = key[0]
  h_dict[key] = hf.Get("%(higgs)s_m%(mass)s_%(loop)s" % vars())
  h_dict[key].Scale(frac[key[0]])
  norm_scale += h_dict[key].Integral()
  if norm_to_bin_width: h_dict[key].Scale(1.0,"width")
  h_dict[key].Scale(1000.)


#if True:
#  for key, val in files.items():
#    h_dict[key].Scale(1/norm_scale)


### Make up and down histograms ###
h_new_dict = {}
h_plot_dict = {}
for key, val in files.items():
  #h_new_dict[key+"_down"],h_new_dict[key+"_up"] = GetUpAndDownUncertaintyHistograms(key, h_dict, lnN_uncerts=lnN_uncerts)
  h_new_dict[key] = h_dict[key].Clone()
  h_new_dict[key+"_up"] = h_dict[key].Clone()
  h_new_dict[key+"_down"] = h_dict[key].Clone()
  h_plot_dict[key] = h_dict[key].Clone()

  # Make plotable histogram with different up and down uncertainties
  h_plot_dict[key+"_down"], h_plot_dict[key+"_up"] = MakeUncertaintyBandsPlotable(h_dict[key].Clone(),h_new_dict[key+"_down"].Clone(),h_new_dict[key+"_up"].Clone())

total_name = "t + b + t b-interference"
h_plot_dict[total_name] = h_dict["t-only"].Clone()
h_plot_dict[total_name].Add(h_dict["b-only"].Clone())
h_plot_dict[total_name].Add(h_dict["i-only"].Clone())

h_plot_dict[total_name+"_down"], h_plot_dict[total_name+"_up"] = MakeUncertaintyBandsPlotable(h_plot_dict[key].Clone(),h_plot_dict[key].Clone(),h_plot_dict[key].Clone())

if do_ratio:
  c, pad1, pad2 = SetupCanvas(do_ratio=do_ratio,logx=logx,logy=logy)
else:
  pad1 = SetupCanvas(do_ratio=do_ratio,logx=logx,logy=logy)


#DrawHistogramsWithBand(pad1,h_plot_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label,pad="top",no_uncerts=turn_uncerts_off,colours=[ROOT.TColor.GetColor(248, 206, 104),ROOT.TColor.GetColor(100, 192, 232),ROOT.TColor.GetColor(155, 152, 204),ROOT.TColor.GetColor(222, 90, 106),ROOT.TColor.GetColor(250, 202, 255),ROOT.TColor.GetColor(222, 90, 106),ROOT.TColor.GetColor(192, 232, 100)])
DrawHistogramsWithBand(pad1,h_plot_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label,pad="top",no_uncerts=turn_uncerts_off)


l = ROOT.TLegend(0.6,0.25,0.94,0.52)
l.SetBorderSize(0)
if not do_ratio:
  l.SetTextSize(0.025)
else:
  l.SetTextSize(0.04)
#for key, val in h_plot_dict.items():
#  if not "_down" in key and not "_up" in key:
#    if key[0] == "t":
#      l.AddEntry(val,"t quark only","f")
#    if key[0] == "b":
#      l.AddEntry(val,"b quark only","f")
#    if key[0] == "i":
#      l.AddEntry(val,"t b-interference","f")

l.AddEntry(h_plot_dict["t-only"],"t quark only","f")
l.AddEntry(h_plot_dict["b-only"],"b quark only","f")
l.AddEntry(h_plot_dict["i-only"],"t b-interference","f")
l.AddEntry(h_plot_dict[total_name],total_name,"f")
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
latex.DrawLatex(0.65,0.73,under_legend_4)
plotting.DrawCMSLogo(pad1, 'CMS', 'Simulation', 11, 0.045, 0.03, 1.0, '', 1.0)

# Draw 10^-3
latex1 = ROOT.TLatex()
latex1.SetNDC()
latex1.SetTextAngle(0)
latex1.SetTextAlign(12)
latex1.SetTextSize(0.032)
latex1.SetTextColor(1)
latex1.SetTextFont(42)
latex1.DrawLatex(0.09,0.97,"#times10^{-3}")

# Draw ratio

if do_ratio:

  h_ratio_dict = GetRatioHists(h_new_dict,ratio_file)
  h_plot_ratio_dict = {}
  for key, val in files_2016.items():
    h_plot_ratio_dict[key] = h_ratio_dict[key].Clone()
    h_plot_ratio_dict[key+"_down"], h_plot_ratio_dict[key+"_up"] = MakeUncertaintyBandsPlotable(h_ratio_dict[key].Clone(),h_ratio_dict[key+"_down"].Clone(),h_ratio_dict[key+"_up"].Clone())

#  DrawHistogramsWithBand(pad2,h_plot_ratio_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label, pad="bottom", colours=[ROOT.R.TColor.GetColor(248, 206, 104),ROOT.TColor.GetColor(100, 192, 232),ROOT.TColor.GetColor(155, 152, 204),ROOT.TColor.GetColor(222, 90, 106),ROOT.TColor.GetColor(250, 202, 255),ROOT.TColor.GetColor(222, 90, 106),ROOT.TColor.GetColor(192, 232, 100)])
  DrawHistogramsWithBand(pad2,h_plot_ratio_dict,do_ratio=do_ratio,do_options=True, x_label=x_label, y_label=y_label, pad="bottom")

if do_ratio:
  c.Print(output_name)
else:
  pad1.Print(output_name)
