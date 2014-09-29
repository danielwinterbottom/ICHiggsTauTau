#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TLine.h"
#include "TBox.h"
#include "TFile.h"
#include "TFrame.h"
#include "TASImage.h"
#include "TGAxis.h"
#include "TROOT.h"
#include "TLegend.h"

#include "../interface/Plotting.h"

// Function prototypes
TCanvas* example_plot(int iPos, bool do_ratio);

int main() {
  example_plot(11, true);
  return 0;
}

TCanvas* example_plot(int iPos, bool do_ratio) {
  TH1::AddDirectory(0);
  modTDRStyle();
  gROOT->ForceStyle();

  TString canvName = "FigExample";
  TCanvas* canv = new TCanvas(canvName, canvName);
  canv->cd();

  std::vector<TPad*> pads =
      do_ratio ? TwoPadSplit(0.29, 0.008, 0.025) : OnePad();

  pads[0]->SetLogy(true);

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
  TH1F* h     = reinterpret_cast<TH1F*>(data->Clone());
  h->Reset();
  h->SetAxisRange(70, 119.9);
  TH1F* hl    = reinterpret_cast<TH1F*>(h->Clone());
  if (do_ratio) {
    h->GetXaxis()->SetTitle("");
    h->GetXaxis()->SetLabelSize(0);
    StandardAxes(hl->GetXaxis(), h->GetYaxis(), "m_{e^{+}e^{-}}", "GeV");
  } else {
    h->GetXaxis()->SetTitleOffset(1.0);
    StandardAxes(h->GetXaxis(), h->GetYaxis(), "m_{e^{+}e^{-}}", "GeV");
  }
  double upper_h = pads[0]->GetHNDC();
  double lower_h = pads[1]->GetHNDC();
  hl->GetXaxis()->SetTickLength(gStyle->GetTickLength() * upper_h / lower_h);
  hl->GetYaxis()->SetTickLength(gStyle->GetTickLength() * (1. + lower_h/upper_h));
  hl->GetYaxis()->SetTitle("Obs/Exp");
  hl->GetYaxis()->CenterTitle(true);
  hl->SetMinimum(0.65);
  hl->SetMaximum(1.35);

  // Can draw main axis now
  h->Draw("axis");

  TColor* adapt = gROOT->GetColor(12);
  int new_idx = gROOT->GetListOfColors()->GetSize() + 1;
  TColor* trans = new TColor(new_idx, adapt->GetRed(), adapt->GetGreen(),
                             adapt->GetBlue());
  trans->SetAlpha(0.3);

  err->SetFillColor(new_idx);
  // err->SetMarkerSize(0);

  MC->SetFillColor(kAzure + 1);


  MC->Draw("histsame");
  err->Draw("e2same");
  data->Draw("esame");

  TH1F *ratio = reinterpret_cast<TH1F*>(MakeRatioHist(data, MC, true, false));
  TH1F *ratio_err = reinterpret_cast<TH1F*>(MakeRatioHist(err, err, true, false));

  // StyleHistogram(fill_color, fill_style, line_color, line_style, marker_color, marker_style, marker_size)

  double sc = 0.2;
  h->SetMaximum((GetPadYMax(pads[0]) - sc * h->GetMinimum()) / (1. - sc));
  if (pads[0]->GetLogy()) {
    h->SetMinimum(0.09);
    double pt = GetPadYMax(pads[0]);
    double low = h->GetMinimum();
    if (low == 0.) low = 1E-6;
    double max = (std::log10(pt) - sc * std::log10(low)) / (1 - sc);
    max = std::pow(10, max);
    h->SetMaximum(max);
  }

  // writing the lumi information and the CMS "logo"
  drawCMSLogo(pads[0], "CMS", "Preliminary", iPos, 0.045, 0.035, 1.2);
  drawTitle(pads[0], "19.7 fb^{-1} (8 TeV) + 4.9 fb^{-1} (7 TeV)", 3);
  drawTitle(pads[0], "H#rightarrow#tau#tau", 1);

  if (do_ratio) {
    pads[1]->cd();
    hl->Draw("axis");
    ratio->Draw("esamex0");
    ratio_err->Draw("e2same");
  }

  pads[0]->cd();
  double l_from_b = 0.05;
  TLegend *legend = new TLegend(0.66, 0.7, 1 - gPad->GetRightMargin() - l_from_b,
                                1 - gPad->GetTopMargin() - l_from_b, "", "NBNDC");
  legend->AddEntry(data, "Observed", "pe");
  legend->AddEntry(MC, "Background", "f");
  legend->AddEntry(err, "Uncertainty", "f");
  legend->SetMargin(0.35);
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

  return canv;
}


