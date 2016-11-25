#include "../../Core/interface/Plotting.h"
#include "../../Core/interface/Plotting_Style.h"

void plotRatios(TString type = "trk") {
  ModTDRStyle();


  TFile f_53X("output/phys14/Jan19/MC_53X_VBF_HToTauTau_M-125.root");
  TH1F *frac_ch_53X = (TH1F*)gDirectory->Get("phys14/"+type+"_pt_frac_ch");
  frac_ch_53X->SetLineWidth(2);
  frac_ch_53X->SetLineColor(kBlue);
  frac_ch_53X->Scale(1. / frac_ch_53X->Integral());
  
  TFile f_72X("output/phys14/Jan19/MC_72X_VBF_HToTauTau_M-125_PU40bx25.root");
  TH1F *frac_ch_72X = (TH1F*)gDirectory->Get("phys14/"+type+"_pt_frac_ch");
  TH1F *frac_em_72X = (TH1F*)gDirectory->Get("phys14/"+type+"_pt_frac_em");
  TH1F *frac_tot_72X = (TH1F*)frac_ch_72X->Clone();
  frac_tot_72X->Add(frac_em_72X);
  frac_ch_72X->SetFillColor(17);
  frac_tot_72X->SetFillColor(46);
  double sf_72X = 1. / frac_tot_72X->Integral();
  frac_tot_72X->Scale(sf_72X);
  frac_ch_72X->Scale(sf_72X);



  TCanvas * canv = new TCanvas("pf_ratios", "pf_ratios");
  canv->cd();
  std::vector<TPad*> pads = OnePad();
  if (type == "trk") pads[0]->SetLogy(true);
  if (type == "th") pads[0]->SetLogy(false);


  // Do the axis hist
  TH1 *axis = CreateAxisHist(frac_ch_53X);
  if (type == "trk")
    axis->GetXaxis()->SetTitle("PFCandidate p_{T} / Gen. #pi^{+} p_{T}");
  if (type == "th")
    axis->GetXaxis()->SetTitle("PFCandidate p_{T} / Gen. #tau_{h} p_{T}");
  axis->GetYaxis()->SetTitle("A.U.");
  axis->Draw();
  axis->SetMinimum(1E-4);
  if (type == "trk") axis->SetMaximum(1.);
  if (type == "th") axis->SetMaximum(0.15);

  frac_tot_72X->Draw("histsame");
  frac_ch_72X->Draw("histsame");
  frac_ch_53X->Draw("histsame");



  // Fix the y-axis range
  // FixTopRange(pads[0], 1.0, 0.0);

  // Make the legend
  TLegend *leg = PositionedLegend(0.4, 0.2, 3, 0.025);
  leg->AddEntry(frac_ch_53X, "53X Ch. Hadrons", "L");
  leg->AddEntry(frac_ch_72X, "72X Ch. Hadrons", "F");
  leg->AddEntry(frac_tot_72X, "72X Photons", "F");
  leg->Draw();

  // Draw labels and text
  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);
  // DrawTitle(pads[0], title, 3);

  // Redraw the axis
  // FixOverlay();

  // Print!
  canv->Print(type+"_pt_ratio.pdf");
}
