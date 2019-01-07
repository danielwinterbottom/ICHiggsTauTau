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

file300 = ROOT.TFile.Open("output/mjj300_nowt/datacard_IC_highMjj_July25_1_max_score_ggh_highMjj_tt_2016.root", "READ")
file500 = ROOT.TFile.Open("output/mjj500_nowt/datacard_IC_highMjj_July25_1_max_score_ggh_highMjj_tt_2016.root", "READ")
file700 = ROOT.TFile.Open("output/mjj700_nowt/datacard_IC_highMjj_July25_1_max_score_ggh_highMjj_tt_2016.root", "READ")

directory300 = file300.GetDirectory("tt_ggh_highMjj")
directory500 = file500.GetDirectory("tt_ggh_highMjj")
directory700 = file700.GetDirectory("tt_ggh_highMjj")

hist300 = directory300.Get("ggHsm_jhu_htt125")
hist500 = directory500.Get("ggHsm_jhu_htt125")
hist700 = directory700.Get("ggHsm_jhu_htt125")

if sys.argv[1] == "500":
    hist500.Divide(hist300)
if sys.argv[1] == "700":
    hist700.Divide(hist300)

pads[0].cd()
if sys.argv[1] == "500":
    hist500.Draw()
    hist500.GetXaxis().SetTitle("MVA Score")
    hist500.SetMinimum(0.0)
    hist500.SetMaximum(1.00)
if sys.argv[1] == "700":
    hist700.Draw()
    hist700.GetXaxis().SetTitle("MVA Score")
    hist700.SetMinimum(0.0)
    hist700.SetMaximum(1.00)

if sys.argv[1] == "500":
    c1.SaveAs("ggH_MVAScore500over300_nowt.pdf")
if sys.argv[1] == "700":
    c1.SaveAs("ggH_MVAScore700over300_nowt.pdf")

file300.Close()
file500.Close()
file700.Close()
