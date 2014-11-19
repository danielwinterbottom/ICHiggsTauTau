#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TFrame.h"
#include "TLegend.h"

#include "../interface/Plotting.h"
#include "../interface/Plotting_Style.h"

// Function prototypes

int main() {
  bool do_ratio = true;
  bool do_logy = true;

  TH1::AddDirectory(0);
  ModTDRStyle();

  TString canvName = "FigExample";
  TCanvas* canv = new TCanvas(canvName, canvName);
  canv->cd();

  std::vector<TPad*> pads =
      do_ratio ? TwoPadSplit(0.29, 0.00, 0.00) : OnePad();

  pads[0]->SetLogy(do_logy);

  // Source histograms
  TFile file_("histo.root", "READ");
  TH1F* data  = reinterpret_cast<TH1F*>(file_.Get("data")->Clone());
  TH1F* MC    = reinterpret_cast<TH1F*>(file_.Get("MC")->Clone());
  file_.Close();

  // Source binning
  MC->Rebin(2);
  data->Rebin(2);

  // Derived histograms
  TH1F* err   = reinterpret_cast<TH1F*>(MC->Clone());

  // Axis histogram
  std::vector<TH1*> h = CreateAxisHists(2, data, 70, 119.9);
  h[0]->Draw("axis");

  if (do_ratio) {
    pads[1]->cd();
    h[1]->Draw("axis");
    SetupTwoPadSplitAsRatio(pads, "Obs/Exp", true, 0.65, 1.35);
    StandardAxes(h[1]->GetXaxis(), h[0]->GetYaxis(), "m_{e^{+}e^{-}}", "GeV");
  } else {
    h[0]->GetXaxis()->SetTitleOffset(1.0);
    StandardAxes(h[0]->GetXaxis(), h[0]->GetYaxis(), "m_{e^{+}e^{-}}", "GeV");
  }
  pads[0]->cd();

  // Can draw main axis now

  int new_idx = CreateTransparentColor(12, 0.2);
  err->SetFillColor(new_idx);
  err->SetMarkerSize(0);

  MC->SetFillColor(kAzure + 1);

  MC->Draw("histsame");
  err->Draw("e2same");
  data->Draw("esamex0");

  TH1F *ratio = reinterpret_cast<TH1F*>(MakeRatioHist(data, MC, true, false));
  TH1F *ratio_err = reinterpret_cast<TH1F*>(MakeRatioHist(err, err, true, false));

  if (pads[0]->GetLogy()) h[0]->SetMinimum(0.09);

  FixTopRange(pads[0], GetPadYMax(pads[0]), 0.15);
  DrawCMSLogo(pads[0], "CMS", "Preliminary", 11, 0.045, 0.035, 1.2);
  DrawTitle(pads[0], "19.7 fb^{-1} (8 TeV) + 4.9 fb^{-1} (7 TeV)", 3);
  DrawTitle(pads[0], "Z#rightarrowee", 1);

  if (do_ratio) {
    pads[1]->cd();
    h[1]->Draw("axis");
    ratio_err->Draw("e2same");
    ratio->Draw("esamex0");
  }

  pads[0]->cd();
  // pos = 1, 2, 3
  TLegend *legend = PositionedLegend(0.25, 0.18, 3, 0.03);
  legend->SetTextFont(42);
  FixBoxPadding(pads[0], legend, 0.05);
  legend->AddEntry(data, "Observed", "pe");
  legend->AddEntry(MC, "Background", "f");
  legend->AddEntry(err, "Uncertainty", "f");
  legend->Draw();

  canv->Update();
  pads[0]->RedrawAxis();
  pads[0]->GetFrame()->Draw();
  if (do_ratio) {
    pads[1]->cd();
    pads[1]->RedrawAxis();
    pads[1]->GetFrame()->Draw();
  }

  canv->Print(".pdf");
  canv->Print(".png");

  return 0;
}
