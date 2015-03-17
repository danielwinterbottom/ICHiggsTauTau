#include "../../Core/interface/Plotting.h"
#include "../../Core/interface/Plotting_Style.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"

void plotTrackTable(TString file, TString hist, TString output, TString title, TString x_axis) {
  ModTDRStyle(750, 600, 0.06, 0.12, 0.40, 0.04);
  gStyle->SetPaintTextFormat(".3f");
  // TFile f("output/phys14/Dec7/MC_53X_VBF_HToTauTau_M-125.root");
  TFile f(file);

  // TH2F *dm_hist = (TH2F*)gDirectory->Get("phys14/th_mode_table");
  TH2F *h = (TH2F*)gDirectory->Get(hist);

  std::cout << h << "\n";



  // ----------------------------------------------------------------
  // Full version - including non-reconstructed taus
  // ----------------------------------------------------------------
  TCanvas * canv = new TCanvas("track_table", "track_table");
  canv->cd();
  std::vector<TPad*> pads = OnePad();

  h->SetMarkerSize(1.1);
  h->GetXaxis()->SetLabelSize(0.04);
  h->GetYaxis()->SetLabelSize(0.04);

  h->GetYaxis()->SetBinLabel(1, "undefAlgorithm");
  h->GetYaxis()->SetBinLabel(2, "ctf");
  h->GetYaxis()->SetBinLabel(3, "rs");
  h->GetYaxis()->SetBinLabel(4, "cosmics");
  h->GetYaxis()->SetBinLabel(5, "initialStep");
  h->GetYaxis()->SetBinLabel(6, "lowPtTripletStep");
  h->GetYaxis()->SetBinLabel(7, "pixelPairStep");
  h->GetYaxis()->SetBinLabel(8, "detachedTripletStep");
  h->GetYaxis()->SetBinLabel(9, "mixedTripletStep");
  h->GetYaxis()->SetBinLabel(10, "pixelLessStep");
  h->GetYaxis()->SetBinLabel(11, "tobTecStep");
  h->GetYaxis()->SetBinLabel(12, "jetCoreRegionalStep");
  h->GetYaxis()->SetBinLabel(13, "conversionStep");
  h->GetYaxis()->SetBinLabel(14, "muonSeededStepInOut");
  h->GetYaxis()->SetBinLabel(15, "muonSeededStepOutIn");
  h->GetYaxis()->SetBinLabel(16, "outInEcalSeededConv");
  h->GetYaxis()->SetBinLabel(17, "inOutEcalSeededConv");
  h->GetYaxis()->SetBinLabel(18, "nuclInter");
  h->GetYaxis()->SetBinLabel(19, "standAloneMuon");
  h->GetYaxis()->SetBinLabel(20, "globalMuon");
  h->GetYaxis()->SetBinLabel(21, "cosmicStandAloneMuon");
  h->GetYaxis()->SetBinLabel(22, "cosmicGlobalMuon");
  h->GetYaxis()->SetBinLabel(23, "iter1LargeD0");
  h->GetYaxis()->SetBinLabel(24, "iter2LargeD0");
  h->GetYaxis()->SetBinLabel(25, "iter3LargeD0");
  h->GetYaxis()->SetBinLabel(26, "iter4LargeD0");
  h->GetYaxis()->SetBinLabel(27, "iter5LargeD0");
  h->GetYaxis()->SetBinLabel(28, "bTagGhostTracks");
  h->GetYaxis()->SetBinLabel(29, "beamhalo");
  h->GetYaxis()->SetBinLabel(30, "gsf");

  h->GetXaxis()->SetTitle(x_axis);
  h->GetYaxis()->SetTitle("Track algorithm");
  // h->GetXaxis()->SetTitleOffset(gStyle->GetTitleXOffset() * 1.6);
  h->GetYaxis()->SetTitleOffset(gStyle->GetTitleYOffset() * 1);

  // h->GetXaxis()->LabelsOption("v");
  h->GetYaxis()->LabelsOption("h");

  float tot = h->Integral(1, h->GetNbinsX(), 1, h->GetNbinsY());
  if (tot == 0) tot = 1.;
  for (int i = 1; i <= h->GetNbinsX(); ++i) {
    for (int j = 1; j <= h->GetNbinsY(); ++j) {
      h->SetBinContent(i, j, h->GetBinContent(i, j) / tot);
    }
  }


  h->Draw("TEXT");
  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);
  DrawTitle(pads[0], title, 3);
  canv->Print(output+".pdf");
}
