#include "../Core/interface/Plotting.h"
#include "../Core/interface/Plotting_Style.h"

TString output = "output/";
TString file_sig = "toytau_test.root";
TString file_bkg = "toytau_qcd.root";
TString dir_rec = "ToyTauStudy_taus";
TString dir_toy = "ToyTauStudy_toyTaus";

void test(TString ) {
  ModTDRStyle();
  TGraphAsymmErrors roc1 = RocCurveFrom1DHists(
      (TH1 *)Get("output/toytau_test.root", "ToyTauStudy_toyTaus/all_dm/cmb_iso"),
      (TH1 *)Get("output/toytau_qcd.root", "ToyTauStudy_toyTaus/all_dm_fake/cmb_iso"),
      false);
  TGraphAsymmErrors roc2 = RocCurveFrom1DHists(
      (TH1 *)Get("output/toytau_test.root", "ToyTauStudy_toyTaus/all_dm/cmb_recalc_iso"),
      (TH1 *)Get("output/toytau_qcd.root", "ToyTauStudy_toyTaus/all_dm_fake/cmb_recalc_iso"),
      false);
  TCanvas canv("canv", "canv");

  std::vector<TPad*> pads = OnePad();

  TH1 * axis = CreateAxisHist(&roc1, false);
  axis->Draw();
  axis->GetXaxis()->SetLimits(0.2, 1);
  axis->GetYaxis()->SetRangeUser(0, 0.003);
  axis->GetXaxis()->SetTitle("#tau_{h} Efficiency");
  axis->GetYaxis()->SetTitle("Jet#rightarrow#tau_{h} Fake Rate");

  roc1.SetMarkerColor(2);
  roc1.SetLineColor(2);
  roc1.Draw("PSAME");
  roc2.SetMarkerColor(4);
  roc2.SetLineColor(4);
  roc2.Draw("PSAME");

  TLegend *legend = PositionedLegend(0.2, 0.2, 1, 0.03);
  legend->AddEntry(&roc1, "Reco #tau_{h}");
  legend->AddEntry(&roc2, "Toy #tau_{h}");
  legend->Draw();

  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);

  canv.SaveAs("test.pdf");
}
