void h_gene_IsoEG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:37:42 2016) by ROOT version6.06/01
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
   
   TH1D *h_gene_IsoEG_Efficiency__1 = new TH1D("h_gene_IsoEG_Efficiency__1","h_gene_IsoEG_Efficiency",50,0,100);
   h_gene_IsoEG_Efficiency__1->SetBinContent(1,0.08185612);
   h_gene_IsoEG_Efficiency__1->SetBinContent(2,0.253179);
   h_gene_IsoEG_Efficiency__1->SetBinContent(3,0.4854546);
   h_gene_IsoEG_Efficiency__1->SetBinContent(4,0.6242383);
   h_gene_IsoEG_Efficiency__1->SetBinContent(5,0.6795535);
   h_gene_IsoEG_Efficiency__1->SetBinContent(6,0.7109503);
   h_gene_IsoEG_Efficiency__1->SetBinContent(7,0.7348396);
   h_gene_IsoEG_Efficiency__1->SetBinContent(8,0.7553619);
   h_gene_IsoEG_Efficiency__1->SetBinContent(9,0.7673843);
   h_gene_IsoEG_Efficiency__1->SetBinContent(10,0.7876996);
   h_gene_IsoEG_Efficiency__1->SetBinContent(11,0.8017446);
   h_gene_IsoEG_Efficiency__1->SetBinContent(12,0.819358);
   h_gene_IsoEG_Efficiency__1->SetBinContent(13,0.8237006);
   h_gene_IsoEG_Efficiency__1->SetBinContent(14,0.8441597);
   h_gene_IsoEG_Efficiency__1->SetBinContent(15,0.8552886);
   h_gene_IsoEG_Efficiency__1->SetBinContent(16,0.8711228);
   h_gene_IsoEG_Efficiency__1->SetBinContent(17,0.8866496);
   h_gene_IsoEG_Efficiency__1->SetBinContent(18,0.8910571);
   h_gene_IsoEG_Efficiency__1->SetBinContent(19,0.9092877);
   h_gene_IsoEG_Efficiency__1->SetBinContent(20,0.9120785);
   h_gene_IsoEG_Efficiency__1->SetBinContent(21,0.9256355);
   h_gene_IsoEG_Efficiency__1->SetBinContent(22,0.9339175);
   h_gene_IsoEG_Efficiency__1->SetBinContent(23,0.9404297);
   h_gene_IsoEG_Efficiency__1->SetBinContent(24,0.9447554);
   h_gene_IsoEG_Efficiency__1->SetBinContent(25,0.9482056);
   h_gene_IsoEG_Efficiency__1->SetBinContent(26,0.9523705);
   h_gene_IsoEG_Efficiency__1->SetBinContent(27,0.9547692);
   h_gene_IsoEG_Efficiency__1->SetBinContent(28,0.9557702);
   h_gene_IsoEG_Efficiency__1->SetBinContent(29,0.9620499);
   h_gene_IsoEG_Efficiency__1->SetBinContent(30,0.9512195);
   h_gene_IsoEG_Efficiency__1->SetBinContent(31,0.9536242);
   h_gene_IsoEG_Efficiency__1->SetBinContent(32,0.9544285);
   h_gene_IsoEG_Efficiency__1->SetBinContent(33,0.9621951);
   h_gene_IsoEG_Efficiency__1->SetBinContent(34,0.9507979);
   h_gene_IsoEG_Efficiency__1->SetBinContent(35,0.9638142);
   h_gene_IsoEG_Efficiency__1->SetBinContent(36,0.9577922);
   h_gene_IsoEG_Efficiency__1->SetBinContent(37,0.9575688);
   h_gene_IsoEG_Efficiency__1->SetBinContent(38,0.9537789);
   h_gene_IsoEG_Efficiency__1->SetBinContent(39,0.962526);
   h_gene_IsoEG_Efficiency__1->SetBinContent(40,0.9584615);
   h_gene_IsoEG_Efficiency__1->SetBinContent(41,0.9619741);
   h_gene_IsoEG_Efficiency__1->SetBinContent(42,0.9615385);
   h_gene_IsoEG_Efficiency__1->SetBinContent(43,0.9655832);
   h_gene_IsoEG_Efficiency__1->SetBinContent(44,0.965587);
   h_gene_IsoEG_Efficiency__1->SetBinContent(45,0.9619151);
   h_gene_IsoEG_Efficiency__1->SetBinContent(46,0.954181);
   h_gene_IsoEG_Efficiency__1->SetBinContent(47,0.9693095);
   h_gene_IsoEG_Efficiency__1->SetBinContent(48,0.9547511);
   h_gene_IsoEG_Efficiency__1->SetBinContent(49,0.9563863);
   h_gene_IsoEG_Efficiency__1->SetBinContent(50,0.9657534);
   h_gene_IsoEG_Efficiency__1->SetBinContent(51,0.9592107);
   h_gene_IsoEG_Efficiency__1->SetEntries(43.15605);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_gene_IsoEG_Efficiency__1->SetFillColor(ci);
   h_gene_IsoEG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_gene_IsoEG_Efficiency__1->SetLineColor(ci);
   h_gene_IsoEG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_gene_IsoEG_Efficiency__1->SetMarkerColor(ci);
   h_gene_IsoEG_Efficiency__1->SetMarkerStyle(33);
   h_gene_IsoEG_Efficiency__1->SetMarkerSize(1.7);
   h_gene_IsoEG_Efficiency__1->GetXaxis()->SetTitle("Gen electron p_{T} [GeV]");
   h_gene_IsoEG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_gene_IsoEG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_gene_IsoEG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_gene_IsoEG_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_gene_IsoEG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_gene_IsoEG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_gene_IsoEG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_gene_IsoEG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_gene_IsoEG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_gene_IsoEG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_gene_IsoEG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_gene_IsoEG_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen electron |#eta| < 2.4");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
