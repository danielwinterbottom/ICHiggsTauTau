void h_gentau_IsoTau_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:30:18 2016) by ROOT version6.06/01
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
   
   TH1D *h_gentau_IsoTau_Efficiency__1 = new TH1D("h_gentau_IsoTau_Efficiency__1","h_gentau_IsoTau_Efficiency",50,0,100);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(1,0.1589489);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(2,0.245959);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(3,0.4114002);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(4,0.5833532);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(5,0.7234476);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(6,0.8050347);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(7,0.8548089);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(8,0.8768418);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(9,0.882907);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(10,0.883657);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(11,0.8799284);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(12,0.8711042);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(13,0.8633812);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(14,0.8604822);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(15,0.852034);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(16,0.8492616);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(17,0.849322);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(18,0.8511008);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(19,0.8556049);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(20,0.8572376);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(21,0.8634473);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(22,0.8682692);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(23,0.875833);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(24,0.8870205);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(25,0.8969041);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(26,0.9006525);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(27,0.9096697);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(28,0.9220887);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(29,0.9291292);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(30,0.9394237);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(31,0.9381115);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(32,0.9508443);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(33,0.9538047);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(34,0.9594748);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(35,0.965717);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(36,0.9661465);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(37,0.9699108);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(38,0.9776198);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(39,0.9802645);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(40,0.9792793);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(41,0.9852837);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(42,0.9854703);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(43,0.9872213);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(44,0.9851552);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(45,0.9899877);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(46,0.988141);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(47,0.9922801);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(48,0.991994);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(49,0.9925972);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(50,0.9930661);
   h_gentau_IsoTau_Efficiency__1->SetBinContent(51,0.9918872);
   h_gentau_IsoTau_Efficiency__1->SetEntries(43.54062);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_gentau_IsoTau_Efficiency__1->SetFillColor(ci);
   h_gentau_IsoTau_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_gentau_IsoTau_Efficiency__1->SetLineColor(ci);
   h_gentau_IsoTau_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_gentau_IsoTau_Efficiency__1->SetMarkerColor(ci);
   h_gentau_IsoTau_Efficiency__1->SetMarkerStyle(33);
   h_gentau_IsoTau_Efficiency__1->SetMarkerSize(1.7);
   h_gentau_IsoTau_Efficiency__1->GetXaxis()->SetTitle("Gen tau p_{T} [GeV]");
   h_gentau_IsoTau_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_gentau_IsoTau_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_gentau_IsoTau_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_gentau_IsoTau_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_gentau_IsoTau_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_gentau_IsoTau_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_gentau_IsoTau_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_gentau_IsoTau_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_gentau_IsoTau_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_gentau_IsoTau_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_gentau_IsoTau_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_gentau_IsoTau_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen tau |#eta| < 2.3");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
