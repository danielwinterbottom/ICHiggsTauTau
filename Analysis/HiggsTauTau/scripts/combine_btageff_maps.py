import os
import ROOT
import sys
import math
from optparse import OptionParser

ROOT.TH1.AddDirectory(False)

bc_eff = "TT"
other_eff = "DYJetsToLL-LO-ext2"

#ttbarwt = float(831.76/76628591)
#dywt = float(5765.4/49064064)
ttbarwt = 1
dywt = 1

channels = ['mt','tt','et']
counter = 0
for ch in channels:
  ftt = ROOT.TFile("/vols/cms/dw515/BTagEffs/Feb17/"+bc_eff+"_"+ch+"_2016.root")
  fdy = ROOT.TFile("/vols/cms/dw515/BTagEffs/Feb17/"+other_eff+"_"+ch+"_2016.root")
  btag_b = ftt.Get("BTagCheck/NBtag_bflav")
  tot_b = ftt.Get("BTagCheck/NTot_bflav")
  btag_c = ftt.Get("BTagCheck/NBtag_cflav")
  tot_c = ftt.Get("BTagCheck/NTot_cflav")
  btag_oth_tt = ftt.Get("BTagCheck/NBtag_otherflav")
  btag_oth_tt.Scale(ttbarwt)
  tot_oth_tt = ftt.Get("BTagCheck/NTot_otherflav")
  tot_oth_tt.Scale(ttbarwt)
  btag_oth = fdy.Get("BTagCheck/NBtag_otherflav")
  btag_oth.Scale(dywt)
  tot_oth = fdy.Get("BTagCheck/NTot_otherflav")
  tot_oth.Scale(dywt)
  if counter<1:
    comb_btag_b = btag_b.Clone("btag_eff_b")
    comb_tot_b = tot_b.Clone("btag_eff_b_denom")
    comb_btag_c = btag_c.Clone("btag_eff_c")
    comb_tot_c = tot_c.Clone("btag_eff_c_denom")
    comb_btag_oth = btag_oth.Clone("btag_eff_oth")
    comb_tot_oth = tot_oth.Clone("btag_eff_oth_denom")
    comb_btag_b.SetNameTitle("btag_eff_b","btag_eff_b")
    comb_btag_c.SetNameTitle("btag_eff_c","btag_eff_c")
    comb_btag_oth.SetNameTitle("btag_eff_oth","btag_eff_oth")
    comb_btag_oth.Add(btag_oth_tt)
    comb_tot_oth.Add(tot_oth_tt)
  else:
    comb_btag_b.Add(btag_b)
    comb_tot_b.Add(tot_b)
    comb_btag_c.Add(btag_c)
    comb_tot_c.Add(tot_c)
    comb_btag_oth.Add(btag_oth)
    comb_tot_oth.Add(tot_oth)
    comb_btag_oth.Add(btag_oth_tt)
    comb_tot_oth.Add(tot_oth_tt)
  counter +=1
  ftt.Close()
  fdy.Close()


comb_btag_b.Divide(comb_tot_b)
comb_btag_c.Divide(comb_tot_c)
comb_btag_oth.Divide(comb_tot_oth)

outfile = ROOT.TFile("tagging_efficiencies_Moriond2017.root","RECREATE")
outfile.WriteObject(comb_btag_b,"btag_eff_b")
outfile.WriteObject(comb_btag_c,"btag_eff_c")
outfile.WriteObject(comb_btag_oth,"btag_eff_oth")

outfile.Close()


