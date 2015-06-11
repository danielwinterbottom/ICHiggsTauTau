#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TFrame.h"
#include "TLegend.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "../../Core/interface/Plotting.h"
#include "../../Core/interface/Plotting_Style.h"

TGraphAsymmErrors Divide(TGraphAsymmErrors const& g1,
                         TGraphAsymmErrors const& g2) {
  TGraphAsymmErrors res = g1;
  for (int i = 0; i < g1.GetN(); ++i) {
    res.GetY()[i] = g1.GetY()[i]/g2.GetY()[i];
    res.GetEYhigh()[i] = g1.GetEYhigh()[i]/g2.GetY()[i];
    res.GetEYlow()[i] = g1.GetEYlow()[i]/g2.GetY()[i];
  }
  return res;
}

void plotEff() {
  // Always set the global style options first
  TH1::AddDirectory(0);
  ModTDRStyle();
  gStyle->SetEndErrorSize(0);

  // Open files, extract plotting objects
  TFile f("toytau_test.root");
  TH1F *h1_num = (TH1F*)gDirectory->Get("ToyTauStudy_taus/rec_dm_chiso0p8_eff_vs_pt_pass");
  TH1F *h1_den = (TH1F*)gDirectory->Get("ToyTauStudy_taus/rec_dm_chiso0p8_eff_vs_pt_all");
  TH1F *h2_num = (TH1F*)gDirectory->Get("ToyTauStudy_toyTaus/rec_dm_chiso0p8_eff_vs_pt_pass");
  TH1F *h2_den = (TH1F*)gDirectory->Get("ToyTauStudy_toyTaus/rec_dm_chiso0p8_eff_vs_pt_all");

  // Build higher-level objects if necessary
  TGraphAsymmErrors g1(h1_num, h1_den);
  TGraphAsymmErrors g2(h2_num, h2_den);

  g1.SetLineColor(1);
  g1.SetMarkerColor(1);
  g1.SetMarkerStyle(20);

  g2.SetLineColor(2);
  g2.SetMarkerColor(2);
  g2.SetMarkerStyle(21);

  TGraphAsymmErrors ratio = Divide(g1, g2);

  // Create canvas, pads, "axis" histograms (in that order)
  TCanvas * canv = new TCanvas("test", "test");
  canv->cd();
  std::vector<TPad*> pads = TwoPadSplit(0.29, 0.005, 0.005);
  std::vector<TH1*> h_axes = CreateAxisHists(pads.size(), &g1, false);
  SetupTwoPadSplitAsRatio(pads, h_axes[0], h_axes[1], "Ratio", true, 0.05, 1.95);

  // Draw on first pad
  pads[0]->cd();
  pads[0]->SetGridx(true);
  pads[0]->SetGridy(true);
  h_axes[0]->Draw("axis");
  h_axes[0]->SetMinimum(0.0);
  h_axes[0]->SetMaximum(1.0);
  h_axes[0]->GetYaxis()->SetTitle("Efficiency");
  g1.Draw("PSAME");
  g2.Draw("PSAME");

  TLegend *legend = PositionedLegend(0.25, 0.1, 6, 0.03);
  legend->AddEntry(&g1, "Graph1");
  legend->AddEntry(&g2, "Graph2");
  legend->Draw();
  
  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);
  DrawTitle(pads[0], "Some Title", 3);
  FixOverlay();

  // Draw on second pad
  pads[1]->cd();
  pads[1]->SetGridx(true);
  pads[1]->SetGridy(true);
  h_axes[1]->Draw("axis");
  h_axes[1]->GetXaxis()->SetTitle("Tau p_{T} [GeV]");
  ratio.Draw("PSAME");

  canv->Print(".pdf");
}
