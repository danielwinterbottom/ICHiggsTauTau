import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import sys

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)
plotting.ModTDRStyle()

c1 = ROOT.TCanvas()
c1.cd()
pads=plotting.OnePad()

file_ = ROOT.TFile.Open("output/datacard_IC_binary_Oct11_score_ggh_highMjj_mt_2016.root", "READ")

directory = file_.GetDirectory("mt_ggh_highMjj")

hist_ggh = directory.Get("ggHsm_htt125")
hist_qqh = directory.Get("qqH_htt125")
hist_bkg = directory.Get("total_bkg")

pads[0].cd()
print "total yield in ggH/qqH: {}/{}".format(hist_ggh.Integral(-1,-1),hist_qqh.Integral(-1,-1))

for bin_ in range(1,hist_ggh.GetNbinsX()+1):
    print "ggH/qqH content in first {} bins: {:.2f}/{:.2f}".format(
            bin_,hist_ggh.Integral(0,bin_),hist_qqh.Integral(0,bin_),
            )
    print "ggH/qqH content in bin {}: {:.2f}/{:.2f}".format(
            bin_,hist_ggh.Integral(bin_,bin_),hist_qqh.Integral(bin_,bin_),
            )
    if hist_bkg.Integral(bin_,bin_) != 0.0:
        print "ggH acceptance: {:.2f}/{:.2f} = {:.2f}".format(
                hist_ggh.Integral(0,bin_),hist_ggh.Integral(-1,-1),
                hist_ggh.Integral(0,bin_)/hist_ggh.Integral(-1,-1),
                )

# hist.Draw()

# c1.SaveAs("binaryMVA.pdf")

file_.Close()
