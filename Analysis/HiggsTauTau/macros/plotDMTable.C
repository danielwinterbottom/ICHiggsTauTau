#include "../../Core/interface/Plotting.h"
#include "../../Core/interface/Plotting_Style.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"

void plotDMTable(TString file, TString hist, TString output, TString title) {
  ModTDRStyle();
  gStyle->SetPaintTextFormat(".2f");
  // TFile f("output/phys14/Dec7/MC_53X_VBF_HToTauTau_M-125.root");
  TFile f(file);

  // TH2F *dm_hist = (TH2F*)gDirectory->Get("phys14/th_mode_table");
  TH2F *dm_hist = (TH2F*)gDirectory->Get(hist);

  TH2F dm_full(*(TH2F *)dm_hist->Clone());

  TH2F dm_simple("th_mode_table_simple", "", 3, -0.5, 2.5, 3, -0.5, 2.5);

  // Convert the full decay mode table to a simplified one
  // This is the bin assignment:
  // bin 1:  -1: not reconstructed (no match)
  // bin 2:   0: 1-prong 0-neutral
  // bin 3:   1: 1-prong 1-neutral
  // bin 4:   2: 1-prong 2-neutral
  // bin 5:   3: 1-prong 3-neutral
  // bin 6:   4: 1-prong >=4-neutral
  // bin 7:   5: 2-prong 0-neutral
  // bin 8:   6: 2-prong 1-neutral
  // bin 9:   7: 2-prong 2-neutral
  // bin 10:  8: 2-prong 3-neutral
  // bin 11:  9: 2-prong >=4-neutral
  // bin 12: 10: 3-prong 0-neutral
  // bin 13: 11: 3-prong 1-neutral
  // bin 14: 12: 3-prong 2-neutral
  // bin 15: 13: 3-prong 3-neutral
  // bin 16: 14: 3-prong >=4-neutral
  // bin 17: 14: other


  // ----------------------------------------------------------------
  // Full version - including non-reconstructed taus
  // ----------------------------------------------------------------
  TCanvas * canv = new TCanvas("th_mode_table", "th_mode_table");
  canv->cd();
  std::vector<TPad*> pads = OnePad();

  pads[0]->SetBottomMargin(0.17);
  pads[0]->SetLeftMargin(0.17);

  dm_full.SetMarkerSize(1.1);
  dm_full.GetXaxis()->SetLabelSize(0.04);
  dm_full.GetYaxis()->SetLabelSize(0.04);

  dm_full.GetXaxis()->SetBinLabel(1, "none");
  dm_full.GetXaxis()->SetBinLabel(2, "1#pi");
  dm_full.GetXaxis()->SetBinLabel(3, "1#pi + 1#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(4, "1#pi + 2#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(5, "1#pi + 3#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(6, "1#pi + 4#pi^{0}");

  dm_full.GetXaxis()->SetBinLabel(7, "2#pi");
  dm_full.GetXaxis()->SetBinLabel(8, "2#pi + 1#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(9, "2#pi + 2#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(10, "2#pi + 3#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(11, "2#pi + 4#pi^{0}");

  dm_full.GetXaxis()->SetBinLabel(12, "3#pi");
  dm_full.GetXaxis()->SetBinLabel(13, "3#pi + 1#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(14, "3#pi + 2#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(15, "3#pi + 3#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(16, "3#pi + 4#pi^{0}");
  dm_full.GetXaxis()->SetBinLabel(17, "other");

  dm_full.GetYaxis()->SetBinLabel(1, "none");
  dm_full.GetYaxis()->SetBinLabel(2, "1#pi");
  dm_full.GetYaxis()->SetBinLabel(3, "1#pi + 1#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(4, "1#pi + 2#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(5, "1#pi + 3#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(6, "1#pi + 4#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(7, "2#pi");
  dm_full.GetYaxis()->SetBinLabel(8, "2#pi + 1#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(9, "2#pi + 2#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(10, "2#pi + 3#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(11, "2#pi + 4#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(12, "3#pi");
  dm_full.GetYaxis()->SetBinLabel(13, "3#pi + 1#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(14, "3#pi + 2#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(15, "3#pi + 3#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(16, "3#pi + 4#pi^{0}");
  dm_full.GetYaxis()->SetBinLabel(17, "other");

  dm_full.GetXaxis()->SetTitle("Generated #tau_{h} mode");
  dm_full.GetYaxis()->SetTitle("Reconstructed #tau_{h} mode");
  dm_full.GetXaxis()->SetTitleOffset(gStyle->GetTitleXOffset() * 1.6);
  dm_full.GetYaxis()->SetTitleOffset(gStyle->GetTitleYOffset() * 1.15);

  dm_full.GetXaxis()->LabelsOption("v");
  dm_full.GetYaxis()->LabelsOption("h");

  for (int i = 1; i <= dm_full.GetNbinsX(); ++i) {
    float tot = dm_full.Integral(i, i, 1, dm_full.GetNbinsY());
    if (tot == 0) tot = 1.;
    for (int j = 1; j <= dm_full.GetNbinsY(); ++j) {
      dm_full.SetBinContent(i, j, dm_full.GetBinContent(i, j) / tot);
    }
  }


  dm_full.Draw("TEXT");
  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);
  DrawTitle(pads[0], title, 3);
  canv->Print(output+".pdf");


  // ----------------------------------------------------------------
  // Simplified version: 3x3 grid of the accepted modes
  // ----------------------------------------------------------------
  float val_11 = dm_hist->Integral(2, 2, 2, 2);
  float val_12 = dm_hist->Integral(2, 2, 3, 6);
  float val_13 = dm_hist->Integral(2, 2, 12, 12);
  float val_1_sum = val_11 + val_12 + val_13;

  float val_21 = dm_hist->Integral(3, 6, 2, 2);
  float val_22 = dm_hist->Integral(3, 6, 3, 6);
  float val_23 = dm_hist->Integral(3, 6, 12, 12);
  float val_2_sum = val_21 + val_22 + val_23;

  float val_31 = dm_hist->Integral(12, 12, 2, 2);
  float val_32 = dm_hist->Integral(12, 12, 3, 6);
  float val_33 = dm_hist->Integral(12, 12, 12, 12);
  float val_3_sum = val_31 + val_32 + val_33;

  dm_simple.SetBinContent(1, 1, val_11 / val_1_sum);
  dm_simple.SetBinContent(1, 2, val_12 / val_1_sum);
  dm_simple.SetBinContent(1, 3, val_13 / val_1_sum);
  dm_simple.SetBinContent(2, 1, val_21 / val_2_sum);
  dm_simple.SetBinContent(2, 2, val_22 / val_2_sum);
  dm_simple.SetBinContent(2, 3, val_23 / val_2_sum);
  dm_simple.SetBinContent(3, 1, val_31 / val_3_sum);
  dm_simple.SetBinContent(3, 2, val_32 / val_3_sum);
  dm_simple.SetBinContent(3, 3, val_33 / val_3_sum);


  TCanvas *canv_simple =
      new TCanvas("th_mode_table_simple", "th_mode_table_simple");
  canv_simple->cd();
  std::vector<TPad*> pads_simple = OnePad();
  dm_simple.SetMarkerSize(2.5);
  dm_simple.GetXaxis()->SetLabelSize(0.06);
  dm_simple.GetYaxis()->SetLabelSize(0.06);
  dm_simple.GetXaxis()->SetTitle("Generated #tau_{h} mode");
  dm_simple.GetYaxis()->SetTitle("Reconstructed #tau_{h} mode");
  dm_simple.GetXaxis()->SetBinLabel(1, "#pi");
  dm_simple.GetXaxis()->SetBinLabel(2, "#pi#pi^{0}s");
  dm_simple.GetXaxis()->SetBinLabel(3, "#pi#pi#pi");
  dm_simple.GetYaxis()->SetBinLabel(1, "#pi");
  dm_simple.GetYaxis()->SetBinLabel(2, "#pi#pi^{0}s");
  dm_simple.GetYaxis()->SetBinLabel(3, "#pi#pi#pi");
  dm_simple.Draw("TEXT");
  DrawCMSLogo(pads_simple[0], "CMS", "Simulation", 0);
  DrawTitle(pads_simple[0], title, 3);
  canv_simple->Print(output+"_simple.pdf");
}
