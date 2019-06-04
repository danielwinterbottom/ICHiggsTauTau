import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import numpy as np

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

plotting.ModTDRStyle(r=0.16, l=0.14)
ROOT.TGaxis.SetExponentOffset(-0.06, 0.01, "y")

c1 = ROOT.TCanvas()
c1.cd()
pads=plotting.OnePad()
lumi = "2018"


file_ = ROOT.TFile("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root", "READ")

hist = file_.Get("btag_eff_b")

x_bins = hist.GetXaxis().GetNbins()
y_bins = hist.GetYaxis().GetNbins()

content = []
for x_bin in np.arange(0,x_bins):
    for y_bin in np.arange(0,y_bins):
        content.append(hist.GetBinContent(x_bin, y_bin))
        
values = [x for x in content if x!=0.0]

min_value = np.min(values)
print min_value
        

plotting.SetBirdPalette()
pads[0].cd()
hist.Draw("colz")

plotting.DrawCMSLogo(pads[0], 'CMS', 'Simulation', 11, 0.045, 0.05, 1.0, '', 0.8)
plotting.DrawTitle(pads[0], lumi, 3)

hist.GetXaxis().SetTitle("P_{T} (GeV)")
hist.GetYaxis().SetTitle("#eta")
hist.GetZaxis().SetTitle("Efficiency")
hist.SetMinimum(min_value)
hist.SetMaximum(1.0)


c1.SaveAs("btag_eff_b_deepCSV_2018.pdf")
file_.Close()


