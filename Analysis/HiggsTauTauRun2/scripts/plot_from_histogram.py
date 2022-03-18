import ROOT
import os
import json
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting

### Input options ###

input_folder  = ""
folder_name   = "mt_NbtagGt1_MTLt40"
hist_name     = {"Non-resonant":"VLQ_betaRd33_0_matched_M_2000","Interference":"VLQ_betaRd33_0_matched_interference_M_2000"}
x_label       = "m_{T}^{tot}"
y_label       = "Events"
z_label       = ""
do_ratio      = False
ratio_file    = "Nominal"
x_min         = 600
x_max         = 4000
y_min         = None
y_max         = None
logx          = True
logy          = False
logz          = False
output_name   = "test_plot.pdf"
draw_type     = "e2"
draw_type_2   = "hist"
x_offset      = 0
y_offset      = 1.2
z_offset      = 1.8
horiz_line    = None
under_legend_1= ""
under_legend_2= ""
under_legend_3= ""
gU = 2.5
### Input files ###


#files = {
#         "Non-interference":"vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/unblinded_0211_betaRd33_0/datacards_vector_leptoquark/combined/cmb/postfit_shapes_MH=1000,gU=1.21.root",
#         "Interference":"vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/unblinded_0211_betaRd33_0/datacards_vector_leptoquark/combined/cmb/postfit_shapes_MH=1000,gU=1.21.root",
#        }

files = {
         "Non-resonant":"vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2016/mt/vlq.inputs-mssm-vs-sm-Run2016-mt_tot_puppi.root",
         "Interference":"vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2016/mt/vlq.inputs-mssm-vs-sm-Run2016-mt_tot_puppi.root",
        }


### Get histograms from ROOT files ###

h_dict = {}
f = {}

for key, val in files.items():

  if type(val) == str: val = [val] 

  first_loop = True
  for i in val:
    i_name = i+"_"+key
    f[i_name] = ROOT.TFile('%(input_folder)s/%(i)s' % vars())
    print '%(input_folder)s/%(i)s' % vars()
    if type(hist_name) == dict:
      h_dict[i_name] = f[i_name].Get(folder_name+"/"+hist_name[key])
    else:
      h_dict[i_name] = f[i_name].Get(folder_name+"/"+hist_name)
    if first_loop:
      h_dict[key] = h_dict[i_name].Clone()
      h_dict[key].SetName(key)
      first_loop = False
    else:
      h_dict[key].Add(h_dict[i_name].Clone())
  h_dict[key].Print("all")
h_dict["Non-resonant"].Scale(gU**4)
h_dict["Interference"].Scale(gU**2)

h_dict["Combined"] = h_dict["Non-resonant"].Clone()
h_dict["Combined"].Add(h_dict["Interference"].Clone())

files["Combined"] = ""

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
  if key in files:
    if minimum == None:
      minimum, maximum = val.GetBinContent(val.GetMinimumBin()), val.GetBinContent(val.GetMaximumBin())
    else:
      if val.GetBinContent(val.GetMaximumBin()) > maximum:
        maximum = val.GetBinContent(val.GetMaximumBin())
      if val.GetBinContent(val.GetMinimumBin()) < minimum:
        minimum = val.GetBinContent(val.GetMinimumBin())

total_range = (maximum - minimum)/10
minimum -= total_range
maximum += total_range

if y_min != None: minimum = y_min
if y_max != None: maximum = y_max

### Plot histograms ###

colours = [2,3,4,5,6,7,8,9,10]

c = ROOT.TCanvas('c','c',700,700)
#c.SetRightMargin(0.18)
#c.SetMargin(0.1, 0.2, 0.1, 0.1)
#c.SetLeftMargin(0.15)
if logx: c.SetLogx()
if logy: c.SetLogy()
if logz: c.SetLogz()

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
    h_dict[key].GetXaxis().SetRangeUser(x_min,x_max)
    h_dict[key].Draw(draw_type)
    do_options = False
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
  i+=1

c.Update()

l = ROOT.TLegend(0.55,0.7,0.88,0.88)
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
 
#plotting.DrawTitle(c, '59.7 fb^{-1} (13 TeV)' % vars(), 3, scale=0.6)
#plotting.DrawTitle(c, '#tau_{h}#tau_{h}' % vars(), 1, scale=0.6)

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextAngle(0)
latex.SetTextAlign(12)
latex.SetTextSize(0.025)
latex.SetTextColor(1)
latex.DrawLatex(0.55,0.65,under_legend_1)
latex.DrawLatex(0.55,0.6,under_legend_2)
latex.DrawLatex(0.55,0.55,under_legend_3)

c.Print(output_name)

os.system("display "+output_name)
