import ROOT
import json
from array import array

params_file = 'scripts/params_vlq_2018.json'
input_folder = '/vols/cms/gu18/Offline/output/MSSM/vlq_2018_matched_gen/'
#input_folder = '/vols/cms/gu18/LHE_NTuples'
files = [
         "VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference",
         "VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference",
         ]
#files = [
#         "combined.root",
#         "lowM.root",
#         "highM.root"
#         ]

file_ext = "_tt_2018.root"
#file_ext = ""
sel = "wt"
#sel = "eventWeight/abs(eventWeight)"
#sel = "(1)"


var_input = "mtt_mass[0,40,80,120,160,200,240,280,320,360,400,440,480,520,560,600,640,680,720,760,800,840,880,920,960,1000]"
#var_input = "mtt[0,40,80,120,160,200,240,280,320,360,400,440,480,520,560,600,640,680,720,760,800,840,880,920,960,1000]"
#var_input = "mtt[400,440,480,520,560,600,640,680,720,760,800,840,880,920,960,1000]"
#var_input = "mtt[300,320,340,360,380,400,420,440,460,480,500]"
#var_input = "wt[-1,0,1]"
#var_input = "eventWeight[-1,-0.5,0,0.5,1]"
var = var_input.split('[')[0]
bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
hout = ROOT.TH1D('hout','',len(bins)-1, bins)

with open(params_file) as jsonfile:
  params = json.load(jsonfile)

h_dict = {}
f = {}
for i in files:
  if "highM" in i: sel += "*(mtt_mass>400)"
  #if "combined" in i: sel += "*(mtt>400)"
  print '%(input_folder)s/%(i)s%(file_ext)s' % vars()
  f[i] = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
  t = f[i].Get('gen_ntuple')
  #t = f[i].Get('ntuple')
  print t
  h_dict[i] = hout.Clone()
  h_dict[i].SetName('h')
  t.Draw('%(var)s>>h' % vars(),sel,'goff')
  h_dict[i] = t.GetHistogram()
  scale = 1
  scale = params[i]['xs']/params[i]['evt']
  #scale = 1./abs(int(t.GetEntries("eventWeight<0"))- int(t.GetEntries("eventWeight>0")))
  #if "lowM" in i: scale = "*"

  h_dict[i].Scale(scale)
  h_dict[i].Print("all")


c = ROOT.TCanvas('c','c',700,700)
c.SetLeftMargin(0.15)

h_dict[files[0]].SetStats(0)

h_dict[files[0]].Draw('PLC')
h_dict[files[1]].Draw('PLC same')

h_dict[files[0]].SetMarkerColor(1)
h_dict[files[0]].SetMarkerStyle(ROOT.kFullCircle)
h_dict[files[1]].SetMarkerColor(2)
h_dict[files[1]].SetLineColor(2)
h_dict[files[1]].SetMarkerStyle(ROOT.kFullSquare)
h_dict[files[0]].GetXaxis().SetTitle('m_{#tau#tau} (GeV)')
h_dict[files[0]].GetXaxis().SetTitleOffset(1.2)
#h_dict[files[0]].GetYaxis().SetTitleOffset(1.6)
#h_dict[files[0]].GetYaxis().SetTitle('Events')

#h_dict[files[2]].Draw('PLC same')
#h_dict[files[2]].SetMarkerColor(3)
#h_dict[files[2]].SetLineColor(3)
#h_dict[files[2]].SetMarkerStyle(ROOT.kFullSquare)

c.Update()
l = ROOT.TLegend(0.55,0.75,0.87,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.04)
#l.AddEntry(h_dict[files[0]],'combined','lep')
l.AddEntry(h_dict[files[0]],'lowM','lep')
l.AddEntry(h_dict[files[1]],'highM','lep')
#l.Draw()

c.Print('norm_check.pdf' % vars())
