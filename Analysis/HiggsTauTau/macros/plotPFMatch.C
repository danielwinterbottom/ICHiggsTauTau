#include "../../Core/interface/Plotting.h"
#include "../../Core/interface/Plotting_Style.h"

void plotPFMatch(TString file, TString hist, TString output, TString title,
                 TString xaxis) {
  ModTDRStyle();
  TFile f(file);

  TH1F *h_all = (TH1F*)gDirectory->Get(hist+"_all");
  TH1F *h_hadron = (TH1F*)gDirectory->Get(hist+"_hadron");
  TH1F *h_electron = (TH1F*)gDirectory->Get(hist+"_electron");
  TH1F *h_muon = (TH1F*)gDirectory->Get(hist+"_muon");
  TH1F *h_photon = (TH1F*)gDirectory->Get(hist+"_photon");
  TH1F *h_none = (TH1F*)gDirectory->Get(hist+"_none");

  // Convert hists to ratios
  h_hadron->Divide(h_all);
  h_electron->Divide(h_all);
  h_muon->Divide(h_all);
  h_photon->Divide(h_all);
  h_none->Divide(h_all);

  h_hadron->SetFillColor(17);
  h_electron->SetFillColor(38);
  h_muon->SetFillColor(30);
  h_photon->SetFillColor(46);
  h_none->SetFillColor(40);

  THStack stack;
  stack.Add(h_hadron);
  stack.Add(h_photon);
  stack.Add(h_electron);
  stack.Add(h_muon);
  stack.Add(h_none);

  TCanvas * canv = new TCanvas("th_mode_table", "th_mode_table");
  canv->cd();
  std::vector<TPad*> pads = OnePad();

  // Do the axis hist
  TH1 *axis = CreateAxisHist(h_all);
  axis->GetXaxis()->SetTitle(xaxis);
  axis->GetYaxis()->SetTitle("Fraction");
  axis->Draw();

  // Draw the stack
  stack.Draw("HISTSAME");

  // Fix the y-axis range
  FixTopRange(pads[0], 1.0, 0.0);

  // Make the legend
  TLegend *leg = PositionedLegend(0.3, 0.3, 6, 0.025);
  leg->AddEntry(h_none, "None", "F");
  leg->AddEntry(h_muon, "Muons", "F");
  leg->AddEntry(h_electron, "Electrons", "F");
  leg->AddEntry(h_photon, "Photons", "F");
  leg->AddEntry(h_hadron, "Ch. Hadrons", "F");
  leg->Draw();

  // Draw labels and text
  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);
  DrawTitle(pads[0], title, 3);

  // Redraw the axis
  FixOverlay();

  // Print!
  canv->Print(output+".pdf");
}
