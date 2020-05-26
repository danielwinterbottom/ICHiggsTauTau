import os
import ROOT
import sys
from optparse import OptionParser

ROOT.TH1.AddDirectory(False)

bc_eff_tthad = "TTToHadronic"
bc_eff_ttsemilep = "TTToSemiLeptonic"
bc_eff_ttlep = "TTTo2L2Nu"
bc_eff = "TT"

other_eff_dy = "DYJetsToLL-LO"
other_eff_dyext = "DYJetsToLL-LO-ext1"

bc_effs = [bc_eff_tthad, bc_eff_ttsemilep, bc_eff_ttlep]
other_effs = [other_eff_dy, other_eff_dyext]

#ttbarwt = float(831.76/76628591)
#dywt = float(5765.4/49064064)
ttbarwt = 1
dywt = 6077.22/97525274.

channels = ['mt','tt','et','em']
counter = 0
path = "/vols/cms/akd116/Offline/output/SM/2020/btagEff_2017/"

#channels = ['em']


tt_had_sf = 377.96/41019500.
tt_semilep_sf = 365.34/42076324.
tt_lep_sf = 88.29/8918978.


for ch in channels:
    ftt_had = ROOT.TFile("{}/{}_{}_2017.root".format(path, bc_eff_tthad, ch))
    ftt_semilep = ROOT.TFile("{}/{}_{}_2017.root".format(path, bc_eff_ttsemilep, ch))
    ftt_lep = ROOT.TFile("{}/{}_{}_2017.root".format(path, bc_eff_ttlep, ch))

    #ftt = ROOT.TFile("{}/{}_{}_2017.root".format(path, bc_eff, ch))

    fdy = ROOT.TFile("{}/{}_{}_2017.root".format(path, other_eff_dy, ch))
    # fdyext = ROOT.TFile("{}/{}_{}_2017.root".format(path, other_eff_dyext, ch))

    btag_b_had = ftt_had.Get("BTagCheck/NBtag_bflav")
    tot_b_had = ftt_had.Get("BTagCheck/NTot_bflav")
    btag_c_had = ftt_had.Get("BTagCheck/NBtag_cflav")
    tot_c_had = ftt_had.Get("BTagCheck/NTot_cflav")

    btag_b_semilep = ftt_semilep.Get("BTagCheck/NBtag_bflav")
    tot_b_semilep = ftt_semilep.Get("BTagCheck/NTot_bflav")
    btag_c_semilep = ftt_semilep.Get("BTagCheck/NBtag_cflav")
    tot_c_semilep = ftt_semilep.Get("BTagCheck/NTot_cflav")

    btag_b_lep = ftt_lep.Get("BTagCheck/NBtag_bflav")
    tot_b_lep = ftt_lep.Get("BTagCheck/NTot_bflav")
    btag_c_lep = ftt_lep.Get("BTagCheck/NBtag_cflav")
    tot_c_lep = ftt_lep.Get("BTagCheck/NTot_cflav")

    btag_b_had.Scale(tt_had_sf)
    tot_b_had.Scale(tt_had_sf)
    btag_c_had.Scale(tt_had_sf)
    tot_c_had.Scale(tt_had_sf)

    btag_b_semilep.Scale(tt_semilep_sf)
    tot_b_semilep.Scale(tt_semilep_sf)
    btag_c_semilep.Scale(tt_semilep_sf)
    tot_c_semilep.Scale(tt_semilep_sf)

    btag_b_lep.Scale(tt_lep_sf)
    tot_b_lep.Scale(tt_lep_sf)
    btag_c_lep.Scale(tt_lep_sf)
    tot_c_lep.Scale(tt_lep_sf)

    btag_b = btag_b_had.Clone()
    btag_b.Add(btag_b_semilep)
    btag_b.Add(btag_b_lep)

    tot_b = tot_b_had.Clone()
    tot_b.Add(tot_b_semilep)
    tot_b.Add(tot_b_lep)

    btag_c = btag_c_had.Clone()
    btag_c.Add(btag_c_semilep)
    btag_c.Add(btag_c_lep)

    tot_c = tot_c_had.Clone()
    tot_c.Add(tot_c_semilep)
    tot_c.Add(tot_c_lep)

    btag_oth_had = ftt_had.Get("BTagCheck/NBtag_otherflav")
    tot_oth_had = ftt_had.Get("BTagCheck/NTot_otherflav")

    btag_oth_semilep = ftt_semilep.Get("BTagCheck/NBtag_otherflav")
    tot_oth_semilep = ftt_semilep.Get("BTagCheck/NTot_otherflav")
    
    btag_oth_lep = ftt_lep.Get("BTagCheck/NBtag_otherflav")
    tot_oth_lep = ftt_lep.Get("BTagCheck/NTot_otherflav")

    btag_oth_had.Scale(tt_had_sf)
    tot_oth_had.Scale(tt_had_sf)
    btag_oth_semilep.Scale(tt_semilep_sf)
    tot_oth_semilep.Scale(tt_semilep_sf)
    btag_oth_lep.Scale(tt_lep_sf)
    tot_oth_lep.Scale(tt_lep_sf)

    btag_oth_tt = btag_oth_had.Clone()
    btag_oth_tt.Add(btag_oth_semilep)
    btag_oth_tt.Add(btag_oth_lep)

    tot_oth_tt = tot_oth_had.Clone()
    tot_oth_tt.Add(tot_oth_semilep)
    tot_oth_tt.Add(tot_oth_lep)


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
    ftt_had.Close()
    ftt_semilep.Close()
    ftt_lep.Close()
    fdy.Close()


comb_btag_b.Divide(comb_tot_b)
comb_btag_c.Divide(comb_tot_c)
comb_btag_oth.Divide(comb_tot_oth)

outfile = ROOT.TFile("tagging_efficiencies_deepCSV_2017_loose.root","RECREATE")
outfile.WriteObject(comb_btag_b,"btag_eff_b")
outfile.WriteObject(comb_btag_c,"btag_eff_c")
outfile.WriteObject(comb_btag_oth,"btag_eff_oth")

outfile.Close()


