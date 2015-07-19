#include "../Core/interface/Plotting.h"
#include "../Core/interface/Plotting_Style.h"

void plotIsoProfile() {
  ModTDRStyle();
  TH1 * real = (TH1 *)Get("output/toytau_test.root", "ToyTauStudy_toyTaus/all_dm/nh_iso_profile");
  TH1 * real_den = (TH1 *)Get("output/toytau_test.root", "ToyTauStudy_toyTaus/all_dm/nh_iso_profile_den");
  real->Scale(1./real_den->Integral());

  TH1 * fake = (TH1 *)Get("output/toytau_qcd.root", "ToyTauStudy_toyTaus/all_dm_fake/nh_iso_profile");
  TH1 * fake_den = (TH1 *)Get("output/toytau_qcd.root", "ToyTauStudy_toyTaus/all_dm_fake/nh_iso_profile_den");
  fake->Scale(1./fake_den->Integral());

  bool integrate = false;
  if (integrate) {
    for (int i = 1; i <= real->GetNbinsX(); ++i) {
      real->SetBinContent(i, real->Integral(i, real->GetNbinsX()));
    }
    for (int i = 1; i <= fake->GetNbinsX(); ++i) {
      fake->SetBinContent(i, fake->Integral(i, fake->GetNbinsX()));
    }
  }

  TCanvas canv("canv", "canv");

  std::vector<TPad*> pads = OnePad();

  TH1 * axis = CreateAxisHist(real);
  axis->Draw();
  axis->GetXaxis()->SetTitle("#DeltaR from #tau_{h} axis");
  axis->GetYaxis()->SetTitle(TString::Format("Mean I_{nh}/dR (GeV / %g)", real->GetXaxis()->GetBinWidth(1)));

  real->SetMarkerColor(8);
  real->SetLineColor(8);
  real->SetLineWidth(3);
  real->Draw("HISTSAME");
  fake->SetMarkerColor(9);
  fake->SetLineColor(9);
  fake->SetLineWidth(3);
  fake->Draw("HISTSAME");

  pads[0]->SetLogy(integrate ? false : true);
  axis->SetMinimum(1E-1);

  FixTopRange(pads[0], GetPadYMax(pads[0]), 0.1);

  TLegend *legend = PositionedLegend(0.55, 0.15, 3, 0.03);
  legend->AddEntry(real, "Real #tau_{h} (Z'#rightarrow#tau#tau)");
  legend->AddEntry(fake, "Fake #tau_{h} (QCD p_{T} 15-3000 GeV)");
  legend->Draw();

  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);

  canv.SaveAs("iso_profile.pdf");
}
