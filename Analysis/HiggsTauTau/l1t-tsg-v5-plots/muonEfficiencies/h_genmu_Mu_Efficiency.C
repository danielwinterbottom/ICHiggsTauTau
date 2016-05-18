void h_genmu_Mu_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon May  2 00:36:22 2016) by ROOT version6.06/01
   TCanvas *c1 = new TCanvas("c1", "c1",904,191,850,850);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.125);
   c1->SetRightMargin(0.125);
   c1->SetFrameBorderMode(0);
   
   TH1D *h_genmu_Mu_Efficiency__1 = new TH1D("h_genmu_Mu_Efficiency__1","h_genmu_Mu_Efficiency",50,0,100);
   h_genmu_Mu_Efficiency__1->SetBinContent(1,0.009739735);
   h_genmu_Mu_Efficiency__1->SetBinContent(2,0.1761761);
   h_genmu_Mu_Efficiency__1->SetBinContent(3,0.4373206);
   h_genmu_Mu_Efficiency__1->SetBinContent(4,0.9110022);
   h_genmu_Mu_Efficiency__1->SetBinContent(5,0.9374108);
   h_genmu_Mu_Efficiency__1->SetBinContent(6,0.9465062);
   h_genmu_Mu_Efficiency__1->SetBinContent(7,0.9540266);
   h_genmu_Mu_Efficiency__1->SetBinContent(8,0.9542906);
   h_genmu_Mu_Efficiency__1->SetBinContent(9,0.9583825);
   h_genmu_Mu_Efficiency__1->SetBinContent(10,0.9651189);
   h_genmu_Mu_Efficiency__1->SetBinContent(11,0.9644402);
   h_genmu_Mu_Efficiency__1->SetBinContent(12,0.9650551);
   h_genmu_Mu_Efficiency__1->SetBinContent(13,0.9663568);
   h_genmu_Mu_Efficiency__1->SetBinContent(14,0.9662265);
   h_genmu_Mu_Efficiency__1->SetBinContent(15,0.9692608);
   h_genmu_Mu_Efficiency__1->SetBinContent(16,0.9683911);
   h_genmu_Mu_Efficiency__1->SetBinContent(17,0.9704424);
   h_genmu_Mu_Efficiency__1->SetBinContent(18,0.9678711);
   h_genmu_Mu_Efficiency__1->SetBinContent(19,0.9696453);
   h_genmu_Mu_Efficiency__1->SetBinContent(20,0.9706955);
   h_genmu_Mu_Efficiency__1->SetBinContent(21,0.9694418);
   h_genmu_Mu_Efficiency__1->SetBinContent(22,0.9743435);
   h_genmu_Mu_Efficiency__1->SetBinContent(23,0.9738551);
   h_genmu_Mu_Efficiency__1->SetBinContent(24,0.9728329);
   h_genmu_Mu_Efficiency__1->SetBinContent(25,0.9700079);
   h_genmu_Mu_Efficiency__1->SetBinContent(26,0.9707813);
   h_genmu_Mu_Efficiency__1->SetBinContent(27,0.9676806);
   h_genmu_Mu_Efficiency__1->SetBinContent(28,0.9681182);
   h_genmu_Mu_Efficiency__1->SetBinContent(29,0.9688037);
   h_genmu_Mu_Efficiency__1->SetBinContent(30,0.9774691);
   h_genmu_Mu_Efficiency__1->SetBinContent(31,0.9735898);
   h_genmu_Mu_Efficiency__1->SetBinContent(32,0.9741124);
   h_genmu_Mu_Efficiency__1->SetBinContent(33,0.9737597);
   h_genmu_Mu_Efficiency__1->SetBinContent(34,0.9716689);
   h_genmu_Mu_Efficiency__1->SetBinContent(35,0.9676768);
   h_genmu_Mu_Efficiency__1->SetBinContent(36,0.9692471);
   h_genmu_Mu_Efficiency__1->SetBinContent(37,0.9767164);
   h_genmu_Mu_Efficiency__1->SetBinContent(38,0.9771746);
   h_genmu_Mu_Efficiency__1->SetBinContent(39,0.9784123);
   h_genmu_Mu_Efficiency__1->SetBinContent(40,0.9638041);
   h_genmu_Mu_Efficiency__1->SetBinContent(41,0.9840336);
   h_genmu_Mu_Efficiency__1->SetBinContent(42,0.9721988);
   h_genmu_Mu_Efficiency__1->SetBinContent(43,0.9665392);
   h_genmu_Mu_Efficiency__1->SetBinContent(44,0.98125);
   h_genmu_Mu_Efficiency__1->SetBinContent(45,0.9612591);
   h_genmu_Mu_Efficiency__1->SetBinContent(46,0.9865031);
   h_genmu_Mu_Efficiency__1->SetBinContent(47,0.984701);
   h_genmu_Mu_Efficiency__1->SetBinContent(48,0.9738752);
   h_genmu_Mu_Efficiency__1->SetBinContent(49,0.96);
   h_genmu_Mu_Efficiency__1->SetBinContent(50,0.9741935);
   h_genmu_Mu_Efficiency__1->SetBinContent(51,0.9717189);
   h_genmu_Mu_Efficiency__1->SetEntries(46.11241);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_genmu_Mu_Efficiency__1->SetFillColor(ci);
   h_genmu_Mu_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_genmu_Mu_Efficiency__1->SetLineColor(ci);
   h_genmu_Mu_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_genmu_Mu_Efficiency__1->SetMarkerColor(ci);
   h_genmu_Mu_Efficiency__1->SetMarkerStyle(33);
   h_genmu_Mu_Efficiency__1->SetMarkerSize(1.7);
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetTitle("Gen muon p_{T} GeV");
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_genmu_Mu_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_genmu_Mu_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_genmu_Mu_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen muon |#eta| < 2.4 ");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
