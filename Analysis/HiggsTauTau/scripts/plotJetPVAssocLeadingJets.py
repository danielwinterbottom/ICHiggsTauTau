import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
import UserCode.ICHiggsTauTau.plotting as plot
ROOT.gROOT.SetBatch(ROOT.kTRUE)

#infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_future_full_tt_0.root"
pu200_infile="/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_pu200_partial_tt_0.root"
#current_infile = "/afs/cern.ch/work/a/adewit/private/CMSSW_8_2_0/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/VBF_current_tt_0.root"

#vbf_file=ROOT.TFile.Open(infile)
#jetpvtree=vbf_file.Get("jetpvassoc")

pu200_file = ROOT.TFile.Open(pu200_infile)
jetpvtree_pu200=pu200_file.Get("jetpvassoc")
jetpvtree_leadsublead=pu200_file.Get("leadsubleadjet")

#vbfcurrent_file=ROOT.TFile.Open(current_infile)
#jetpvtree_current=vbfcurrent_file.Get("jetpvassoc")


xbins = [0.,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2,4.4,4.6,4.8,5.0]
#vbf_all_jets = ROOT.TH1D("vbf_all_jets","vbf_all_jets",25,np.array(xbins))
#vbf_pvassoc_jets = ROOT.TH1D("vbf_pvassoc_jets","vbf_pvassoc_jets",25,np.array(xbins))
vbf_all_jets_pu200 = ROOT.TH1D("vbf_all_jets_pu200","vbf_all_jets_pu200",25,np.array(xbins))
vbf_pvassoc_jets_pu200 = ROOT.TH1D("vbf_pvassoc_jets_pu200","vbf_pvassoc_jets_pu200",25,np.array(xbins))
vbf_lsubl_pu200 = ROOT.TH1D("vbf_lsubl_pu200","vbf_lsubl_pu200",25,np.array(xbins))
vbf_lsubl_pvassoc_pu200 = ROOT.TH1D("vbf_lsubl_pvassoc_pu200","vbf_lsubl_pvassoc_pu200",25,np.array(xbins))
vbf_lsubl_pu200_j2 = ROOT.TH1D("vbf_lsubl_pu200_j2","vbf_lsubl_pu200_j2",25,np.array(xbins))
vbf_lsubl_pvassoc_pu200_j2 = ROOT.TH1D("vbf_lsubl_pvassoc_pu200_j2","vbf_lsubl_pvassoc_pu200_j2",25,np.array(xbins))
#vbf_all_jets_current = ROOT.TH1D("vbf_all_jets_current","vbf_all_jets_current",25,np.array(xbins))
#vbf_pvassoc_jets_current = ROOT.TH1D("vbf_pvassoc_jets_current","vbf_pvassoc_jets_current",25,np.array(xbins))
#vbf_lead_jet = ROOT.TH1D("vbf_lead_jet","vbf_lead_jet",25,np.array(xbins))
#vbf_beta_jets = ROOT.TH1D("vbf_beta_jets","vbf_beta_jets",25,np.array(xbins))
#vbf_betaneg_jets = ROOT.TH1D("vbf_betaneg_jets","vbf_betaneg_jets",25,np.array(xbins))

#vbf_all_jets.Sumw2()
#vbf_pvassoc_jets.Sumw2()
vbf_all_jets_pu200.Sumw2()
vbf_pvassoc_jets_pu200.Sumw2()
vbf_lsubl_pu200.Sumw2()
vbf_lsubl_pvassoc_pu200.Sumw2()
vbf_lsubl_pu200_j2.Sumw2()
vbf_lsubl_pvassoc_pu200_j2.Sumw2()


jetpvtree_pu200.Draw("abs(jet_eta)>>vbf_all_jets_pu200","jet_flav!=0&&jet_flav!=21&&jet_pt>20")
jetpvtree_pu200.Draw("abs(jet_eta)>>vbf_pvassoc_jets_pu200","jet_flav!=0&&jet_flav!=21&&jet_beta>0.1&&jet_pt>20")
jetpvtree_leadsublead.Draw("abs(jet_eta1)>>vbf_lsubl_pu200","")
jetpvtree_leadsublead.Draw("abs(jet_eta1)>>vbf_lsubl_pvassoc_pu200","jet_beta1>0.1")
jetpvtree_leadsublead.Draw("abs(jet_eta2)>>vbf_lsubl_pu200_j2","")
jetpvtree_leadsublead.Draw("abs(jet_eta2)>>vbf_lsubl_pvassoc_pu200_j2","jet_beta2>0.1")

vbf_lsubl_pu200.Add(vbf_lsubl_pu200_j2)
vbf_lsubl_pvassoc_pu200.Add(vbf_lsubl_pvassoc_pu200_j2)

vbf_pvassoc_jets_pu200.Divide(vbf_all_jets_pu200)
vbf_lsubl_pvassoc_pu200.Divide(vbf_lsubl_pu200)

vbf_pvassoc_jets_pu200.SetMarkerStyle(20)
vbf_pvassoc_jets_pu200.SetMarkerColor(ROOT.kRed)
vbf_pvassoc_jets_pu200.SetTitle("")
vbf_pvassoc_jets_pu200.SetStats(0)
vbf_pvassoc_jets_pu200.GetYaxis().SetRangeUser(0,1.4)

vbf_lsubl_pvassoc_pu200.SetMarkerStyle(21)
vbf_lsubl_pvassoc_pu200.SetMarkerColor(ROOT.kGreen+3)

plot.ModTDRStyle(r=0.06, l=0.12)
ROOT.gStyle.SetFrameLineWidth(2)
canv=ROOT.TCanvas()
pads = plot.OnePad()
for padx in pads:
    # Use tick marks on oppsite axis edges
    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
plot.Set(legend, NColumns=1)
vbf_pvassoc_jets_pu200.GetXaxis().SetTitle("VBF jet generator |#eta|")
vbf_pvassoc_jets_pu200.GetYaxis().SetTitle("jet PV association efficiency")
legend.AddEntry(vbf_pvassoc_jets_pu200,'With tracker extension, 200 PU','P')
legend.AddEntry(vbf_lsubl_pvassoc_pu200,'With tracker extension, 200 PU (leading+subleading jet only)','P')
vbf_pvassoc_jets_pu200.Draw("EP0")
vbf_lsubl_pvassoc_pu200.Draw("P0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

legend.Draw()

plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV", 3)

canv.SaveAs('jetpvassoc_with200_comp_all_leadsublead.pdf')
canv.Print('jetpvassoc_with200_comp_all_leadsublead.png')

#canv2=ROOT.TCanvas()
#pads2 = plot.OnePad()
#for padx in pads2:
    # Use tick marks on oppsite axis edges
#    plot.Set(padx, Tickx=1, Ticky=1, Logx=False)

#legend = plot.PositionedLegend(0.45, 0.10, 3, 0.015)
#plot.Set(legend, NColumns=1)
#vbf_beta_jets.GetXaxis().SetTitle("VBF jet |#eta|")
#vbf_beta_jets.GetYaxis().SetTitle("#beta")
#legend.AddEntry(vbf_pvassoc_jets,'With tracker extension','P')
#legend.AddEntry(vbf_pvassoc_jets_current,'Without tracker extension (current)','P')
#vbf_pvassoc_jets.Draw("EP0")
#vbf_beta_jets.Draw("L")
#vbf_pvassoc_jets_current.Draw("P0SAME")

#box = ROOT.TPave(pads[0].GetLeftMargin(), 0.81, 1-pads[0].GetRightMargin(), 1-pads[0].GetTopMargin(), 1, 'NDC')
#box.Draw()

#legend.Draw()

#plot.DrawCMSLogo(pads[0], 'CMS', 'Simulation preliminary', 11, 0.045, 0.035, 1.2, '', 0.8)
#plot.DrawTitle(pads[0], "VBF H#rightarrow#tau#tau", 1)
#plot.DrawTitle(pads[0], "#sqrt{s}=14 TeV, 0 PU", 3)

#canv2.SaveAs('jetbeta.pdf')
#canv2.Print('jetbeta.png')

