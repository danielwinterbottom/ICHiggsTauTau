void h_genjet_Jet_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:39:30 2016) by ROOT version6.06/01
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
   
   TH1D *h_genjet_Jet_Efficiency__1 = new TH1D("h_genjet_Jet_Efficiency__1","h_genjet_Jet_Efficiency",50,0,100);
   h_genjet_Jet_Efficiency__1->SetBinContent(5,0.08045029);
   h_genjet_Jet_Efficiency__1->SetBinContent(6,0.1063197);
   h_genjet_Jet_Efficiency__1->SetBinContent(7,0.1415102);
   h_genjet_Jet_Efficiency__1->SetBinContent(8,0.1851518);
   h_genjet_Jet_Efficiency__1->SetBinContent(9,0.2352307);
   h_genjet_Jet_Efficiency__1->SetBinContent(10,0.2943575);
   h_genjet_Jet_Efficiency__1->SetBinContent(11,0.3585174);
   h_genjet_Jet_Efficiency__1->SetBinContent(12,0.4245359);
   h_genjet_Jet_Efficiency__1->SetBinContent(13,0.4943255);
   h_genjet_Jet_Efficiency__1->SetBinContent(14,0.557311);
   h_genjet_Jet_Efficiency__1->SetBinContent(15,0.6178344);
   h_genjet_Jet_Efficiency__1->SetBinContent(16,0.6703269);
   h_genjet_Jet_Efficiency__1->SetBinContent(17,0.7163336);
   h_genjet_Jet_Efficiency__1->SetBinContent(18,0.7570944);
   h_genjet_Jet_Efficiency__1->SetBinContent(19,0.791109);
   h_genjet_Jet_Efficiency__1->SetBinContent(20,0.8209657);
   h_genjet_Jet_Efficiency__1->SetBinContent(21,0.8462781);
   h_genjet_Jet_Efficiency__1->SetBinContent(22,0.8664718);
   h_genjet_Jet_Efficiency__1->SetBinContent(23,0.8881512);
   h_genjet_Jet_Efficiency__1->SetBinContent(24,0.9025325);
   h_genjet_Jet_Efficiency__1->SetBinContent(25,0.9139124);
   h_genjet_Jet_Efficiency__1->SetBinContent(26,0.9257121);
   h_genjet_Jet_Efficiency__1->SetBinContent(27,0.9331715);
   h_genjet_Jet_Efficiency__1->SetBinContent(28,0.9409601);
   h_genjet_Jet_Efficiency__1->SetBinContent(29,0.9477693);
   h_genjet_Jet_Efficiency__1->SetBinContent(30,0.9548767);
   h_genjet_Jet_Efficiency__1->SetBinContent(31,0.9595774);
   h_genjet_Jet_Efficiency__1->SetBinContent(32,0.9639572);
   h_genjet_Jet_Efficiency__1->SetBinContent(33,0.9681089);
   h_genjet_Jet_Efficiency__1->SetBinContent(34,0.9701889);
   h_genjet_Jet_Efficiency__1->SetBinContent(35,0.9735011);
   h_genjet_Jet_Efficiency__1->SetBinContent(36,0.9748299);
   h_genjet_Jet_Efficiency__1->SetBinContent(37,0.9786735);
   h_genjet_Jet_Efficiency__1->SetBinContent(38,0.9803238);
   h_genjet_Jet_Efficiency__1->SetBinContent(39,0.9809662);
   h_genjet_Jet_Efficiency__1->SetBinContent(40,0.9845668);
   h_genjet_Jet_Efficiency__1->SetBinContent(41,0.9850692);
   h_genjet_Jet_Efficiency__1->SetBinContent(42,0.9859357);
   h_genjet_Jet_Efficiency__1->SetBinContent(43,0.9874987);
   h_genjet_Jet_Efficiency__1->SetBinContent(44,0.988909);
   h_genjet_Jet_Efficiency__1->SetBinContent(45,0.9885109);
   h_genjet_Jet_Efficiency__1->SetBinContent(46,0.9895074);
   h_genjet_Jet_Efficiency__1->SetBinContent(47,0.9905595);
   h_genjet_Jet_Efficiency__1->SetBinContent(48,0.9913546);
   h_genjet_Jet_Efficiency__1->SetBinContent(49,0.9908661);
   h_genjet_Jet_Efficiency__1->SetBinContent(50,0.990861);
   h_genjet_Jet_Efficiency__1->SetBinContent(51,0.9953929);
   h_genjet_Jet_Efficiency__1->SetEntries(35.99498);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_genjet_Jet_Efficiency__1->SetFillColor(ci);
   h_genjet_Jet_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_genjet_Jet_Efficiency__1->SetLineColor(ci);
   h_genjet_Jet_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_genjet_Jet_Efficiency__1->SetMarkerColor(ci);
   h_genjet_Jet_Efficiency__1->SetMarkerStyle(33);
   h_genjet_Jet_Efficiency__1->SetMarkerSize(1.7);
   h_genjet_Jet_Efficiency__1->GetXaxis()->SetTitle("Gen jet p_{T} [GeV]");
   h_genjet_Jet_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_genjet_Jet_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_genjet_Jet_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_genjet_Jet_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_genjet_Jet_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_genjet_Jet_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_genjet_Jet_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_genjet_Jet_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_genjet_Jet_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_genjet_Jet_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_genjet_Jet_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_genjet_Jet_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen jet |#eta| < 5.0");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
