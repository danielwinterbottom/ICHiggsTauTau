void h_genmu_IsoMu_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:34:38 2016) by ROOT version6.06/01
   TCanvas *c1 = new TCanvas("c1", "c1",516,52,850,850);
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
   h_genmu_IsoMu_Efficiency__1->SetBinContent(1,0.007560125);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(2,0.07821794);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(3,0.1827083);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(4,0.3145443);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(5,0.405116);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(6,0.523817);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(7,0.6754144);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(8,0.8264076);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(9,0.9030852);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(10,0.9241791);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(11,0.9258002);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(12,0.935802);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(13,0.9355039);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(14,0.9371359);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(15,0.9413961);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(16,0.9419061);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(17,0.9411191);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(18,0.940346);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(19,0.940734);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(20,0.940038);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(21,0.942904);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(22,0.9494382);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(23,0.9460371);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(24,0.9367795);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(25,0.9462473);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(26,0.9434244);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(27,0.9410213);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(28,0.93356);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(29,0.9408199);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(30,0.9507788);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(31,0.9472121);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(32,0.948565);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(33,0.9428571);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(34,0.9449955);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(35,0.9509202);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(36,0.9374332);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(37,0.9540508);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(38,0.9439547);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(39,0.9460406);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(40,0.9392857);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(41,0.9556692);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(42,0.9381974);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(43,0.9400387);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(44,0.9517314);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(45,0.9239264);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(46,0.9639752);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(47,0.9446023);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(48,0.9408284);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(49,0.9328);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(50,0.9404959);
   h_genmu_IsoMu_Efficiency__1->SetBinContent(51,0.9420849);
   h_genmu_IsoMu_Efficiency__1->SetEntries(42.53942);

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
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetTitle("Gen muon p_{T} [GeV]");
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_genmu_IsoMu_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_genmu_IsoMu_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_genmu_IsoMu_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_genmu_IsoMu_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen muon |#eta| < 2.4");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
