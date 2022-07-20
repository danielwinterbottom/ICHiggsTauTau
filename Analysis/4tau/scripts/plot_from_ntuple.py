import ROOT
import os
import json
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting
from collections import OrderedDict

### Input options ###

#input_folder  = "/vols/cms/gu18/Offline/output/4tau/2018_gen"
input_folder  = "/vols/cms/gu18/Offline/output/4tau/2018_1307_sigmass"
params_file   = "scripts/params_UL_2018.json"
ntuple_name   = "ntuple"
#var_input     = "mvis_phi[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350,360,370,380,390,400]"
var_input     = "mvis_A[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]"
#var_input     = "pt_4[0,2,4,6,8,10,13,14,16,18,20,22,24,26,28,30,34,38,42,46,50,55,60,65,70,80,90,100,120]"
x_label       = "m_{vis}^{#phi}"
x_label       = "m_{vis}^{A}"
#x_label       = "gen-p_{T}^{#tau_{3}}"
y_label       = "1/N dN/d{}".format(x_label)
z_label       = "Events"
sel           = "({}>0)".format(var_input.split("[")[0])
do_ratio      = False
ratio_file    = "HIG-21-001"
logx          = False
logy          = False
logz          = False
channel       = ""
output_name   = "mphi_dist.pdf"
output_name   = "mA_dist.pdf"
draw_type     = "e2"
draw_type_2   = "hist"
x_offset      = 0
y_offset      = 1.8
z_offset      = 1.8
lumi          = 59740
horiz_line    = None
vert_line     = None
norm          = True
under_legend_1= ""
under_legend_2=""
under_legend_3=""

### Input files ###

#files = {
#         "H(100)A(100)#rightarrow 4#tau (1fb)": ["phi100A100To4Tau_ettt_2018.root","phi100A100To4Tau_mttt_2018.root","phi100A100To4Tau_tttt_2018.root","phi100A100To4Tau_eett_2018.root","phi100A100To4Tau_mmtt_2018.root"],
#         "H(200)A(100)#rightarrow 4#tau (1fb)": ["phi200A100To4Tau_ettt_2018.root","phi200A100To4Tau_mttt_2018.root","phi200A100To4Tau_tttt_2018.root","phi200A100To4Tau_eett_2018.root","phi200A100To4Tau_mmtt_2018.root"],
#         "H(300)A(100)#rightarrow 4#tau (1fb)": ["phi300A100To4Tau_ettt_2018.root","phi300A100To4Tau_mttt_2018.root","phi300A100To4Tau_tttt_2018.root","phi300A100To4Tau_eett_2018.root","phi300A100To4Tau_mmtt_2018.root"],
##         "ZZ#rightarrow 4#tau": ["ZZTo4L_ettt_2018.root"], 
##         "ZZ#rightarrow 4l": ["ZZTo4L_mt_2018.root"],
##         "DY#rightarrow 2#tau": ["DYJetsToLL-LO_mt_2018.root"] 
#        }

files = OrderedDict()

files["H(100)A(60)#rightarrow 4#tau"] = ["phi100A60To4Tau_ettt_2018.root","phi100A60To4Tau_mttt_2018.root","phi100A60To4Tau_tttt_2018.root","phi100A60To4Tau_eett_2018.root","phi100A60To4Tau_mmtt_2018.root"]
files["H(200)A(60)#rightarrow 4#tau"] = ["phi200A60To4Tau_ettt_2018.root","phi200A60To4Tau_mttt_2018.root","phi200A60To4Tau_tttt_2018.root","phi200A60To4Tau_eett_2018.root","phi200A60To4Tau_mmtt_2018.root"]
files["H(300)A(60)#rightarrow 4#tau"] = ["phi300A60To4Tau_ettt_2018.root","phi300A60To4Tau_mttt_2018.root","phi300A60To4Tau_tttt_2018.root","phi300A60To4Tau_eett_2018.root","phi300A60To4Tau_mmtt_2018.root"]

files["H(100)A(100)#rightarrow 4#tau"] = ["phi100A100To4Tau_ettt_2018.root","phi100A100To4Tau_mttt_2018.root","phi100A100To4Tau_tttt_2018.root","phi100A100To4Tau_eett_2018.root","phi100A100To4Tau_mmtt_2018.root"]
files["H(200)A(100)#rightarrow 4#tau"] = ["phi200A100To4Tau_ettt_2018.root","phi200A100To4Tau_mttt_2018.root","phi200A100To4Tau_tttt_2018.root","phi200A100To4Tau_eett_2018.root","phi200A100To4Tau_mmtt_2018.root"]
files["H(300)A(100)#rightarrow 4#tau"] = ["phi300A100To4Tau_ettt_2018.root","phi300A100To4Tau_mttt_2018.root","phi300A100To4Tau_tttt_2018.root","phi300A100To4Tau_eett_2018.root","phi300A100To4Tau_mmtt_2018.root"]

files["H(100)A(150)#rightarrow 4#tau"] = ["phi100A150To4Tau_ettt_2018.root","phi100A150To4Tau_mttt_2018.root","phi100A150To4Tau_tttt_2018.root","phi100A150To4Tau_eett_2018.root","phi100A150To4Tau_mmtt_2018.root"]
files["H(200)A(150)#rightarrow 4#tau"] = ["phi200A150To4Tau_ettt_2018.root","phi200A150To4Tau_mttt_2018.root","phi200A150To4Tau_tttt_2018.root","phi200A150To4Tau_eett_2018.root","phi200A150To4Tau_mmtt_2018.root"]
files["H(300)A(150)#rightarrow 4#tau"] = ["phi300A150To4Tau_ettt_2018.root","phi300A150To4Tau_mttt_2018.root","phi300A150To4Tau_tttt_2018.root","phi300A150To4Tau_eett_2018.root","phi300A150To4Tau_mmtt_2018.root"]

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

for key, val in files.iteritems():

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
#    scale = lumi*params[i.split("_")[0]]['xs']/params[i.split("_")[0]]['evt']
    #scale = lumi*params[i.split("_"+channel)[0]]['xs']/params[i.split("_"+channel)[0]]['evt']
    scale = lumi*1.0/params[i.split("_")[0]]['evt']
    h_dict[i_name].Scale(scale)
    if norm: h_dict[i_name].Scale(1/h_dict[i_name].Integral())
    h_dict[i_name].Scale(1.0,"width")
    h_dict[key].Add(h_dict[i_name].Clone())
  h_dict[key].Print("all")


### Get ratio ###
if do_ratio:
  for key, val in files.iteritems():
    h_dict[key].Divide(h_dict[ratio_file].Clone())

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

print minimum, maximum  
total_range = (maximum - minimum)/10

if round(minimum,5) != 0: minimum -= total_range
maximum += total_range

#minimum = -0.003
#maximum = 0.5

### Plot histograms ###

colours = [2,3,4,5,6,7,8,9,11]

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
for key, val in files.iteritems():

  if draw_type_2 != None: hist_2[key] = h_dict[key].Clone()
  
  if do_options:
    h_dict[key].SetStats(0)
    h_dict[key].GetXaxis().SetTitle(x_label)
    h_dict[key].GetYaxis().SetTitle(y_label)
    h_dict[key].GetXaxis().SetTitleOffset(x_offset)
    h_dict[key].GetYaxis().SetTitleOffset(y_offset)
    #h_dict[key].GetYaxis().SetRangeUser(minimum,maximum)
    h_dict[key].GetYaxis().SetRangeUser(0,maximum)
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
  #l = ROOT.TLegend(0.65,0.7,0.88,0.88)
#  l = ROOT.TLegend(0.6,0.7,0.88,0.88)
  l = ROOT.TLegend(0.55,0.5,0.88,0.88)
  l.SetBorderSize(0)
  l.SetTextSize(0.025)
  for key, val in files.iteritems():
    l.AddEntry(h_dict[key],key,'l')
  l.Draw()

if horiz_line != None:
  line = ROOT.TLine(bins[0],horiz_line,bins[-1],horiz_line)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw()
if vert_line != None:
  line = ROOT.TLine(vert_line,minimum,vert_line,maximum)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw()
 
plotting.DrawTitle(c, '59.7 fb^{-1} (13 TeV)' % vars(), 3, scale=0.6)
#plotting.DrawTitle(c, 'e#tau_{h}#tau_{h}#tau_{h}' % vars(), 1, scale=0.6)
#plotting.DrawTitle(c, '#mu#tau_{h}' % vars(), 1, scale=0.6)

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextAngle(0)
latex.SetTextAlign(12)
latex.SetTextSize(0.025)
latex.SetTextColor(1)
latex.DrawLatex(0.62,0.65,under_legend_1)
latex.DrawLatex(0.62,0.6,under_legend_2)
latex.DrawLatex(0.62,0.55,under_legend_3)

c.Print(output_name)

os.system("display "+output_name)
