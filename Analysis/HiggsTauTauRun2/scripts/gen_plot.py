import ROOT
import json
from array import array

input_folder = '/vols/cms/gu18/Offline/output/MSSM/'
#input_folder = '/vols/cms/gu18/LHE_NTuples'
files = [
         "VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_down_tt_2017.root",
         "VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_down_tt_2018.root",
         "VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_up_tt_2017.root",
         "VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_up_tt_2018.root",
         "VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2017.root",
         "VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root",
         ]
#files = [
#         "combined.root",
#         "lowM.root",
#         "highM.root"
#         ]

#sel = "wt"
#sel = "eventWeight/abs(eventWeight)"
sel = "wt"

file_ext = ""

#var_input = "mt_tot[0,80,160,240,320,400,480,560,640,720,800,880,1000]"
var_input = "n_bjets[0,1,5]"
#var_input = "pt_tt[0,40,80,120,160,200,240,280,320,360,400,440,480,520,560,600]"
#var_input = "mtt[400,440,480,520,560,600,640,680,720,760,800,840,880,920,960,1000]"
#var_input = "mtt[300,320,340,360,380,400,420,440,460,480,500]"
#var_input = "wt[-1,0,1]"
#var_input = "eventWeight[-1,-0.5,0,0.5,1]"
var = var_input.split('[')[0]
bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
hout = ROOT.TH1D('hout','',len(bins)-1, bins)

h_dict = {}
f = {}
for i in files:
  if "2017" in i:
    params_file = 'scripts/params_vlq_2017.json'
    input_folder = '/vols/cms/gu18/Offline/output/MSSM/vlq_2017_matched_gen'
  elif "2018" in i:
    params_file = 'scripts/params_vlq_2018.json'
    input_folder = '/vols/cms/gu18/Offline/output/MSSM/vlq_2018_matched_gen'

  with open(params_file) as jsonfile:
    params = json.load(jsonfile)

  #if "highM" in i: sel += "*(mtt_mass>400)"
  #if "combined" in i: sel += "*(mtt>400)"
  #print '%(input_folder)s/%(i)s%(file_ext)s' % vars()
  f[i] = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
  t = f[i].Get('gen_ntuple')
  #t = f[i].Get('ntuple')
  h_dict[i] = hout.Clone()
  h_dict[i].SetName('h')
  t.Draw('%(var)s>>h' % vars(),sel,'goff')
  h_dict[i] = t.GetHistogram()
  scale = 1
  scale = params[i.split("_tt")[0]]['xs']/params[i.split("_tt")[0]]['evt']
  #scale = 1./abs(int(t.GetEntries("eventWeight<0"))- int(t.GetEntries("eventWeight>0")))
  #if "lowM" in i: scale = "*"

  h_dict[i].Scale(scale)
  #h_dict[i].Print("all")
  #print h_dict[i].Integral()

print "down", (h_dict[files[0]].Integral()+h_dict[files[1]].Integral())/(h_dict[files[4]].Integral()+h_dict[files[5]].Integral())
print "up", (h_dict[files[2]].Integral()+h_dict[files[3]].Integral())/(h_dict[files[4]].Integral()+h_dict[files[5]].Integral())

h_dict[files[0]].Divide(h_dict[files[2]].Clone())
h_dict[files[1]].Divide(h_dict[files[2]].Clone())
h_dict[files[2]].Divide(h_dict[files[2]].Clone())

h_dict[files[0]].Print("all")
h_dict[files[1]].Print("all")
h_dict[files[2]].Print("all")

c = ROOT.TCanvas('c','c',700,700)
c.SetLeftMargin(0.15)

h_dict[files[0]].SetStats(0)

h_dict[files[0]].Draw('hist')
h_dict[files[1]].Draw('histsame')

h_dict[files[0]].SetMarkerColor(1)
h_dict[files[0]].SetMarkerStyle(ROOT.kFullCircle)
h_dict[files[1]].SetMarkerColor(2)
h_dict[files[1]].SetLineColor(2)
h_dict[files[1]].SetMarkerStyle(ROOT.kFullSquare)
h_dict[files[0]].GetXaxis().SetTitle(var)
h_dict[files[0]].GetXaxis().SetTitleOffset(1.2)
h_dict[files[0]].GetYaxis().SetTitleOffset(1.6)
h_dict[files[0]].GetYaxis().SetTitle('Events')
h_dict[files[0]].GetYaxis().SetRangeUser(0.75,1.25)

h_dict[files[2]].Draw('histsame')
h_dict[files[2]].SetMarkerColor(3)
h_dict[files[2]].SetLineColor(3)
h_dict[files[2]].SetMarkerStyle(ROOT.kFullSquare)

h_dict[files[0]].Print("all")
h_dict[files[1]].Print("all")
h_dict[files[2]].Print("all")

c.Update()
l = ROOT.TLegend(0.35,0.75,0.87,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.04)
l.AddEntry(h_dict[files[0]],'non-interference down','lep')
l.AddEntry(h_dict[files[1]],'non-interference up','lep')
l.AddEntry(h_dict[files[2]],'non-interference nominal','lep')
l.Draw()

c.Print('norm_check.pdf' % vars())
