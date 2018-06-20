import ROOT

channels = ["mt", "et", "tt"]
path = "/vols/cms/akd116/Offline/output/SM/2018/Jun19_btag_sf"

mt_baseline = "(mt_1<50 && iso_1<0.15 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && pt_1>25 && trg_singlemuon &&pt_2>30)"
et_baseline = "(mt_1<50 && iso_1<0.1 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && pt_1>28 && trg_singleelectron &&pt_2>30)"
tt_baseline = "(pt_1>50 && mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto && trg_doubletau)"

tthad_wt = 377.96/32193696.0
ttsemi_wt = 365.34/40888765.0
ttlep_wt = 88.29/8601887.0

def btag_eff(tree, channel, name, norm):
    c1 = ROOT.TCanvas()
    hist_num_1 = ROOT.TH1F("hist_num_1","hist_num_1",10,0,150)
    hist_denom_1 = ROOT.TH1F("hist_denom_1","hist_denom_1",10,0,150)
    if channel == "mt":
        tree.Draw("(1)>>hist_num_1", "wt*({} & n_bjets==0)".format(mt_baseline))
        tree.Draw("(1)>>hist_denom_1", "wt*{}".format(mt_baseline))
    if channel == "et":
        tree.Draw("(1)>>hist_num_1", "wt*({} & n_bjets==0)".format(et_baseline))
        tree.Draw("(1)>>hist_denom_1", "{}".format(et_baseline))
    if channel == "tt":
        tree.Draw("(1)>>hist_num_1", "wt*({} & n_bjets==0)".format(tt_baseline))
        tree.Draw("(1)>>hist_denom_1", "wt*{}".format(tt_baseline))

    pEff = ROOT.TEfficiency(hist_num_1, hist_denom_1)
    if norm is not "":
        pEff.SetWeight(norm)
    print "\nEfficiency: {}\n".format(pEff.GetEfficiency(1))
    
    pEff.Draw()
    c1.SaveAs("{}_btag_eff_CSV_{}.pdf".format(name, channel))

for channel in channels:
    print "\n{} channel".format(channel)
    f_bkg_tthad = ROOT.TFile("{}/TTToHadronic_{}_2017.root".format(path, channel), "READ")
    tree_bkg_tthad = f_bkg_tthad.Get("ntuple")

    btag_eff(tree_bkg_tthad, channel, "tthad", tthad_wt)

    # if channel == "mt":
    #     num_1 = float(tree_bkg_tthad.GetEntries("{} && n_bjets==0".format(mt_baseline)))
    #     denom_1 = float(tree_bkg_tthad.GetEntries("{}".format(mt_baseline)))
    # elif channel == "et":
    #     num_1 = float(tree_bkg_tthad.GetEntries("{} && n_bjets==0".format(et_baseline)))
    #     denom_1 = float(tree_bkg_tthad.GetEntries("{}".format(et_baseline)))
    # elif channel == "tt":
    #     num_1 = float(tree_bkg_tthad.GetEntries("{} && n_bjets==0".format(tt_baseline)))
    #     denom_1 = float(tree_bkg_tthad.GetEntries("{}".format(tt_baseline)))

    f_bkg_tthad.Close()

    # print num_1 
    # print denom_1
    # efficiency_tthad = float(num_1/denom_1)
    # print "Efficiency for TTToHadronic {} channel: {}".format(channel, efficiency_tthad)

    f_bkg_ttsemilep = ROOT.TFile("{}/TTToSemiLeptonic_{}_2017.root".format(path, channel), "READ")
    tree_bkg_ttsemilep = f_bkg_ttsemilep.Get("ntuple")
    btag_eff(tree_bkg_ttsemilep, channel, "ttsemilep", ttsemi_wt)

    # if channel == "mt":
    #     num_2 = float(tree_bkg_ttsemilep.GetEntries("{} && n_bjets==0".format(mt_baseline)))
    #     denom_2 = float(tree_bkg_ttsemilep.GetEntries("{}".format(mt_baseline)))
    # elif channel == "et":
    #     num_2 = float(tree_bkg_ttsemilep.GetEntries("{} && n_bjets==0".format(et_baseline)))
    #     denom_2 = float(tree_bkg_ttsemilep.GetEntries("{}".format(et_baseline)))
    # elif channel == "tt":
    #     num_2 = float(tree_bkg_ttsemilep.GetEntries("{} && n_bjets==0".format(tt_baseline)))
    #     denom_2 = float(tree_bkg_ttsemilep.GetEntries("{}".format(tt_baseline)))

    f_bkg_ttsemilep.Close()

    # print num_2 
    # print denom_2
    # efficiency_ttsemi = float(num_2/denom_2)
    # print "Efficiency for TTToSemiLeptonic {} channel: {}".format(channel, efficiency_ttsemi)

    f_bkg_ttlep = ROOT.TFile("{}/TTTo2L2Nu_{}_2017.root".format(path, channel), "READ")
    tree_bkg_ttlep = f_bkg_ttlep.Get("ntuple")
    btag_eff(tree_bkg_ttlep, channel, "ttlep", ttlep_wt)

    # if channel == "mt":
    #     num_3 = float(tree_bkg_ttlep.GetEntries("{} && n_bjets==0".format(mt_baseline)))
    #     denom_3 = float(tree_bkg_ttlep.GetEntries("{}".format(mt_baseline)))
    # elif channel == "et":
    #     num_3 = float(tree_bkg_ttlep.GetEntries("{} && n_bjets==0".format(et_baseline)))
    #     denom_3 = float(tree_bkg_ttlep.GetEntries("{}".format(et_baseline)))
    # elif channel == "tt":
    #     num_3 = float(tree_bkg_ttlep.GetEntries("{} && n_bjets==0".format(tt_baseline)))
    #     denom_3 = float(tree_bkg_ttlep.GetEntries("{}".format(tt_baseline)))

    f_bkg_ttlep.Close()

#     print num_3 
#     print denom_3
#     efficiency_ttlep = float(num_3/denom_3)
#     print "Efficiency for TTTo2L2Nu {} channel: {}".format(channel, efficiency_ttlep)

    # now do weighted average of these three values

    # weighted_eff = (efficiency_tthad * tthad_wt + efficiency_ttsemi * ttsemi_wt + efficiency_ttlep * ttlep_wt)/(tthad_wt + ttsemi_wt + ttlep_wt)
    # print "Efficiency for TT samples weighted by xs is: {}".format(weighted_eff)


    f_sig = ROOT.TFile("{}/GluGluHToTauTau_M-125_{}_2017.root".format(path, channel), "READ")
    tree_sig = f_sig.Get("ntuple")

    btag_eff(tree_sig, channel, "ggh", "")

    # if channel == "mt":
    #     num = float(tree_sig.GetEntries("{} && n_bjets==0".format(mt_baseline)))
    #     denom = float(tree_sig.GetEntries("{}".format(mt_baseline)))
    # elif channel == "et":
    #     num = float(tree_sig.GetEntries("{} && n_bjets==0".format(et_baseline)))
    #     denom = float(tree_sig.GetEntries("{}".format(et_baseline)))
    # elif channel == "tt":
    #     num = float(tree_sig.GetEntries("{} && n_bjets==0".format(tt_baseline)))
    #     denom = float(tree_sig.GetEntries("{}".format(tt_baseline)))

    f_sig.Close()
    
    # print num 
    # print denom
    # efficiency = float(num/denom)
    # print "Efficiency for ggH {} channel: {}".format(channel, efficiency)

