void h_genmu_IsoMu_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon May  2 00:35:40 2016) by ROOT version6.06/01
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
   
   TH1D *h_genmu_IsoMu_Efficiency__1 = new TH1D("h_genmu_IsoMu_Efficiency__1","h_genmu_IsoMu_Efficiency",50,0,100);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(1,0.00557711);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(2,0.08750514);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(3,0.2038278);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(4,0.3418394);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(5,0.4299588);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(6,0.541792);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(7,0.6856462);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(8,0.83077);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(9,0.9055781);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(10,0.9273632);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(11,0.9288805);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(12,0.9364581);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(13,0.936182);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(14,0.9389041);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(15,0.942425);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(16,0.9424806);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(17,0.9420515);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(18,0.9410092);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(19,0.9417917);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(20,0.9418941);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(21,0.9430938);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(22,0.948687);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(23,0.9470441);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(24,0.9367292);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(25,0.9463299);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(26,0.9434681);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(27,0.9420152);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(28,0.9339036);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(29,0.9419004);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(30,0.9506173);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(31,0.9471797);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(32,0.9497041);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(33,0.9434194);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(34,0.9464365);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(35,0.95);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(36,0.9384942);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(37,0.9546269);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(38,0.9463294);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(39,0.9470752);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(40,0.941093);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(41,0.9546218);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(42,0.9393429);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(43,0.9407266);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(44,0.9520833);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(45,0.9249395);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(46,0.9656442);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(47,0.945758);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(48,0.9404935);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(49,0.9312);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(50,0.9419355);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(51,0.9419687);
   h_genmu_IsoMu_Efficiency__1->SetEntries(42.68683);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_genmu_IsoMu_Efficiency__1->SetFillColor(ci);
   h_genmu_IsoMu_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_genmu_IsoMu_Efficiency__1->SetLineColor(ci);
   h_genmu_IsoMu_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_genmu_IsoMu_Efficiency__1->SetMarkerColor(ci);
   h_genmu_IsoMu_Efficiency__1->SetMarkerStyle(33);
   h_genmu_IsoMu_Efficiency__1->SetMarkerSize(1.7);
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetTitle("Gen muon p_{T} GeV");
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_genmu_IsoMu_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_genmu_IsoMu_Efficiency__1->Draw("P");
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
