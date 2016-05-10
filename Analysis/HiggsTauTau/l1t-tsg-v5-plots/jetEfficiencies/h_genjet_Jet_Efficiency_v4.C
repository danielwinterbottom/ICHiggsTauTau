void h_genjet_Jet_Efficiency_v4()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:41:08 2016) by ROOT version6.06/01
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
   h_genjet_Jet_Efficiency__1->SetBinContent(5,0.2663593);
   h_genjet_Jet_Efficiency__1->SetBinContent(6,0.3491091);
   h_genjet_Jet_Efficiency__1->SetBinContent(7,0.4398199);
   h_genjet_Jet_Efficiency__1->SetBinContent(8,0.5330256);
   h_genjet_Jet_Efficiency__1->SetBinContent(9,0.6155516);
   h_genjet_Jet_Efficiency__1->SetBinContent(10,0.6912462);
   h_genjet_Jet_Efficiency__1->SetBinContent(11,0.7526744);
   h_genjet_Jet_Efficiency__1->SetBinContent(12,0.8022824);
   h_genjet_Jet_Efficiency__1->SetBinContent(13,0.8450281);
   h_genjet_Jet_Efficiency__1->SetBinContent(14,0.8759546);
   h_genjet_Jet_Efficiency__1->SetBinContent(15,0.9016647);
   h_genjet_Jet_Efficiency__1->SetBinContent(16,0.9216484);
   h_genjet_Jet_Efficiency__1->SetBinContent(17,0.9353913);
   h_genjet_Jet_Efficiency__1->SetBinContent(18,0.9464497);
   h_genjet_Jet_Efficiency__1->SetBinContent(19,0.954544);
   h_genjet_Jet_Efficiency__1->SetBinContent(20,0.9618544);
   h_genjet_Jet_Efficiency__1->SetBinContent(21,0.9681867);
   h_genjet_Jet_Efficiency__1->SetBinContent(22,0.9722184);
   h_genjet_Jet_Efficiency__1->SetBinContent(23,0.9756741);
   h_genjet_Jet_Efficiency__1->SetBinContent(24,0.9784673);
   h_genjet_Jet_Efficiency__1->SetBinContent(25,0.9801006);
   h_genjet_Jet_Efficiency__1->SetBinContent(26,0.9816515);
   h_genjet_Jet_Efficiency__1->SetBinContent(27,0.9834104);
   h_genjet_Jet_Efficiency__1->SetBinContent(28,0.984675);
   h_genjet_Jet_Efficiency__1->SetBinContent(29,0.9852148);
   h_genjet_Jet_Efficiency__1->SetBinContent(30,0.9873221);
   h_genjet_Jet_Efficiency__1->SetBinContent(31,0.9876975);
   h_genjet_Jet_Efficiency__1->SetBinContent(32,0.9889616);
   h_genjet_Jet_Efficiency__1->SetBinContent(33,0.9895769);
   h_genjet_Jet_Efficiency__1->SetBinContent(34,0.990752);
   h_genjet_Jet_Efficiency__1->SetBinContent(35,0.9903208);
   h_genjet_Jet_Efficiency__1->SetBinContent(36,0.9898269);
   h_genjet_Jet_Efficiency__1->SetBinContent(37,0.9911098);
   h_genjet_Jet_Efficiency__1->SetBinContent(38,0.9920358);
   h_genjet_Jet_Efficiency__1->SetBinContent(39,0.9914032);
   h_genjet_Jet_Efficiency__1->SetBinContent(40,0.9920083);
   h_genjet_Jet_Efficiency__1->SetBinContent(41,0.9925951);
   h_genjet_Jet_Efficiency__1->SetBinContent(42,0.9923141);
   h_genjet_Jet_Efficiency__1->SetBinContent(43,0.9929953);
   h_genjet_Jet_Efficiency__1->SetBinContent(44,0.9932459);
   h_genjet_Jet_Efficiency__1->SetBinContent(45,0.9926247);
   h_genjet_Jet_Efficiency__1->SetBinContent(46,0.9927092);
   h_genjet_Jet_Efficiency__1->SetBinContent(47,0.9940845);
   h_genjet_Jet_Efficiency__1->SetBinContent(48,0.9946019);
   h_genjet_Jet_Efficiency__1->SetBinContent(49,0.9939258);
   h_genjet_Jet_Efficiency__1->SetBinContent(50,0.9936909);
   h_genjet_Jet_Efficiency__1->SetBinContent(51,0.9957587);
   h_genjet_Jet_Efficiency__1->SetEntries(41.426);

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
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v4");
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
