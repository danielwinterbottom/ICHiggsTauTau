void h_gentau_Tau_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:31:22 2016) by ROOT version6.06/01
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
   
   TH1D *h_gentau_Tau_Efficiency__1 = new TH1D("h_gentau_Tau_Efficiency__1","h_gentau_Tau_Efficiency",50,0,100);
   h_gentau_Tau_Efficiency__1->SetBinContent(1,0.1692037);
   h_gentau_Tau_Efficiency__1->SetBinContent(2,0.25749);
   h_gentau_Tau_Efficiency__1->SetBinContent(3,0.4247896);
   h_gentau_Tau_Efficiency__1->SetBinContent(4,0.6009638);
   h_gentau_Tau_Efficiency__1->SetBinContent(5,0.7467158);
   h_gentau_Tau_Efficiency__1->SetBinContent(6,0.8388825);
   h_gentau_Tau_Efficiency__1->SetBinContent(7,0.8986591);
   h_gentau_Tau_Efficiency__1->SetBinContent(8,0.9365443);
   h_gentau_Tau_Efficiency__1->SetBinContent(9,0.9562871);
   h_gentau_Tau_Efficiency__1->SetBinContent(10,0.9692691);
   h_gentau_Tau_Efficiency__1->SetBinContent(11,0.9792504);
   h_gentau_Tau_Efficiency__1->SetBinContent(12,0.9829189);
   h_gentau_Tau_Efficiency__1->SetBinContent(13,0.9881398);
   h_gentau_Tau_Efficiency__1->SetBinContent(14,0.9892773);
   h_gentau_Tau_Efficiency__1->SetBinContent(15,0.9911524);
   h_gentau_Tau_Efficiency__1->SetBinContent(16,0.9911018);
   h_gentau_Tau_Efficiency__1->SetBinContent(17,0.9926668);
   h_gentau_Tau_Efficiency__1->SetBinContent(18,0.9933064);
   h_gentau_Tau_Efficiency__1->SetBinContent(19,0.9942471);
   h_gentau_Tau_Efficiency__1->SetBinContent(20,0.9929858);
   h_gentau_Tau_Efficiency__1->SetBinContent(21,0.9937783);
   h_gentau_Tau_Efficiency__1->SetBinContent(22,0.9930888);
   h_gentau_Tau_Efficiency__1->SetBinContent(23,0.9932574);
   h_gentau_Tau_Efficiency__1->SetBinContent(24,0.9930518);
   h_gentau_Tau_Efficiency__1->SetBinContent(25,0.9932886);
   h_gentau_Tau_Efficiency__1->SetBinContent(26,0.9931992);
   h_gentau_Tau_Efficiency__1->SetBinContent(27,0.9927717);
   h_gentau_Tau_Efficiency__1->SetBinContent(28,0.9931102);
   h_gentau_Tau_Efficiency__1->SetBinContent(29,0.9945234);
   h_gentau_Tau_Efficiency__1->SetBinContent(30,0.9944563);
   h_gentau_Tau_Efficiency__1->SetBinContent(31,0.9926003);
   h_gentau_Tau_Efficiency__1->SetBinContent(32,0.9950453);
   h_gentau_Tau_Efficiency__1->SetBinContent(33,0.9949593);
   h_gentau_Tau_Efficiency__1->SetBinContent(34,0.9941565);
   h_gentau_Tau_Efficiency__1->SetBinContent(35,0.9946081);
   h_gentau_Tau_Efficiency__1->SetBinContent(36,0.9941161);
   h_gentau_Tau_Efficiency__1->SetBinContent(37,0.9931278);
   h_gentau_Tau_Efficiency__1->SetBinContent(38,0.9933724);
   h_gentau_Tau_Efficiency__1->SetBinContent(39,0.9930716);
   h_gentau_Tau_Efficiency__1->SetBinContent(40,0.9926802);
   h_gentau_Tau_Efficiency__1->SetBinContent(41,0.9937603);
   h_gentau_Tau_Efficiency__1->SetBinContent(42,0.993245);
   h_gentau_Tau_Efficiency__1->SetBinContent(43,0.9948341);
   h_gentau_Tau_Efficiency__1->SetBinContent(44,0.9931024);
   h_gentau_Tau_Efficiency__1->SetBinContent(45,0.9946087);
   h_gentau_Tau_Efficiency__1->SetBinContent(46,0.9923167);
   h_gentau_Tau_Efficiency__1->SetBinContent(47,0.9942549);
   h_gentau_Tau_Efficiency__1->SetBinContent(48,0.9942283);
   h_gentau_Tau_Efficiency__1->SetBinContent(49,0.9950648);
   h_gentau_Tau_Efficiency__1->SetBinContent(50,0.9954496);
   h_gentau_Tau_Efficiency__1->SetBinContent(51,0.9935672);
   h_gentau_Tau_Efficiency__1->SetEntries(46.50698);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_gentau_Tau_Efficiency__1->SetFillColor(ci);
   h_gentau_Tau_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_gentau_Tau_Efficiency__1->SetLineColor(ci);
   h_gentau_Tau_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_gentau_Tau_Efficiency__1->SetMarkerColor(ci);
   h_gentau_Tau_Efficiency__1->SetMarkerStyle(33);
   h_gentau_Tau_Efficiency__1->SetMarkerSize(1.7);
   h_gentau_Tau_Efficiency__1->GetXaxis()->SetTitle("Gen tau p_{T} [GeV]");
   h_gentau_Tau_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_gentau_Tau_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_gentau_Tau_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_gentau_Tau_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_gentau_Tau_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_gentau_Tau_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_gentau_Tau_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_gentau_Tau_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_gentau_Tau_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_gentau_Tau_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_gentau_Tau_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_gentau_Tau_Efficiency__1->Draw("P");
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
