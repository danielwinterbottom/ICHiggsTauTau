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
        tree.Draw("(1)>>hist_num_1", "wt*({} && n_bjets==0)".format(mt_baseline))
        tree.Draw("(1)>>hist_denom_1", "wt*{}".format(mt_baseline))
    if channel == "et":
        tree.Draw("(1)>>hist_num_1", "wt*({} && n_bjets==0)".format(et_baseline))
        tree.Draw("(1)>>hist_denom_1", "wt*{}".format(et_baseline))
    if channel == "tt":
        tree.Draw("(1)>>hist_num_1", "wt*({} && n_bjets==0)".format(tt_baseline))
        tree.Draw("(1)>>hist_denom_1", "wt*{}".format(tt_baseline))

    if norm is not "":
        hist_num_1.Scale(norm)
        hist_denom_1.Scale(norm)

    numerator = hist_num_1.Integral(-1,-1)
    denominator = hist_denom_1.Integral(-1,-1)

    basic_eff = float(numerator/denominator)
    print "Division of integrals: {}".format(basic_eff)
        
    pEff = ROOT.TEfficiency(hist_num_1, hist_denom_1)
    print "\nEfficiency: {}\n".format(pEff.GetEfficiency(1))
    
    pEff.Draw()
    c1.SaveAs("{}_btag_eff_CSV_{}.pdf".format(name, channel))

    return [numerator, denominator]

for channel in channels:
    print "\n{} channel".format(channel)

    # TT Hadronic
    f_bkg_tthad = ROOT.TFile("{}/TTToHadronic_{}_2017.root".format(path, channel), "READ")
    tree_bkg_tthad = f_bkg_tthad.Get("ntuple")

    tthad_nums = btag_eff(tree_bkg_tthad, channel, "tthad", tthad_wt)

    f_bkg_tthad.Close()

    # TT SemiLeptonic
    f_bkg_ttsemilep = ROOT.TFile("{}/TTToSemiLeptonic_{}_2017.root".format(path, channel), "READ")
    tree_bkg_ttsemilep = f_bkg_ttsemilep.Get("ntuple")
    
    ttsemilep_nums = btag_eff(tree_bkg_ttsemilep, channel, "ttsemilep", ttsemi_wt)

    f_bkg_ttsemilep.Close()

    # TT Leptonic
    f_bkg_ttlep = ROOT.TFile("{}/TTTo2L2Nu_{}_2017.root".format(path, channel), "READ")
    tree_bkg_ttlep = f_bkg_ttlep.Get("ntuple")
    
    ttlep_nums = btag_eff(tree_bkg_ttlep, channel, "ttlep", ttlep_wt)

    f_bkg_ttlep.Close()

    
    # Average of TT split by decay
    scaled_numerator = tthad_nums[0] + ttsemilep_nums[0] + ttlep_nums[0]
    scaled_denominator = tthad_nums[1] + ttsemilep_nums[1] + ttlep_nums[1]

    scaled_tt_eff = float(scaled_numerator/scaled_denominator)
    print "TT efficiency: {}".format(scaled_tt_eff)

    # ggH 
    f_sig = ROOT.TFile("{}/GluGluHToTauTau_M-125_{}_2017.root".format(path, channel), "READ")
    tree_sig = f_sig.Get("ntuple")

    btag_eff(tree_sig, channel, "ggh", "")

    f_sig.Close()
    
