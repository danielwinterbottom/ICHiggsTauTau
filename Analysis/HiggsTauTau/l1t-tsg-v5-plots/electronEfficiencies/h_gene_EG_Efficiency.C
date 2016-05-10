void h_gene_EG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:36:23 2016) by ROOT version6.06/01
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
   
   TH1D *h_gene_EG_Efficiency__1 = new TH1D("h_gene_EG_Efficiency__1","h_gene_EG_Efficiency",50,0,100);
   h_gene_EG_Efficiency__1->SetBinContent(1,0.0988462);
   h_gene_EG_Efficiency__1->SetBinContent(2,0.3061088);
   h_gene_EG_Efficiency__1->SetBinContent(3,0.5928453);
   h_gene_EG_Efficiency__1->SetBinContent(4,0.7674878);
   h_gene_EG_Efficiency__1->SetBinContent(5,0.8322106);
   h_gene_EG_Efficiency__1->SetBinContent(6,0.8631487);
   h_gene_EG_Efficiency__1->SetBinContent(7,0.8850514);
   h_gene_EG_Efficiency__1->SetBinContent(8,0.8970956);
   h_gene_EG_Efficiency__1->SetBinContent(9,0.9072204);
   h_gene_EG_Efficiency__1->SetBinContent(10,0.9150591);
   h_gene_EG_Efficiency__1->SetBinContent(11,0.922907);
   h_gene_EG_Efficiency__1->SetBinContent(12,0.9287783);
   h_gene_EG_Efficiency__1->SetBinContent(13,0.9286681);
   h_gene_EG_Efficiency__1->SetBinContent(14,0.9347456);
   h_gene_EG_Efficiency__1->SetBinContent(15,0.9363109);
   h_gene_EG_Efficiency__1->SetBinContent(16,0.9412844);
   h_gene_EG_Efficiency__1->SetBinContent(17,0.9446494);
   h_gene_EG_Efficiency__1->SetBinContent(18,0.9447981);
   h_gene_EG_Efficiency__1->SetBinContent(19,0.9499715);
   h_gene_EG_Efficiency__1->SetBinContent(20,0.9484979);
   h_gene_EG_Efficiency__1->SetBinContent(21,0.9509194);
   h_gene_EG_Efficiency__1->SetBinContent(22,0.9540296);
   h_gene_EG_Efficiency__1->SetBinContent(23,0.9527995);
   h_gene_EG_Efficiency__1->SetBinContent(24,0.9528796);
   h_gene_EG_Efficiency__1->SetBinContent(25,0.9518914);
   h_gene_EG_Efficiency__1->SetBinContent(26,0.9550044);
   h_gene_EG_Efficiency__1->SetBinContent(27,0.9557066);
   h_gene_EG_Efficiency__1->SetBinContent(28,0.9575496);
   h_gene_EG_Efficiency__1->SetBinContent(29,0.9620499);
   h_gene_EG_Efficiency__1->SetBinContent(30,0.951837);
   h_gene_EG_Efficiency__1->SetBinContent(31,0.9536242);
   h_gene_EG_Efficiency__1->SetBinContent(32,0.9555311);
   h_gene_EG_Efficiency__1->SetBinContent(33,0.9630081);
   h_gene_EG_Efficiency__1->SetBinContent(34,0.9516844);
   h_gene_EG_Efficiency__1->SetBinContent(35,0.9643032);
   h_gene_EG_Efficiency__1->SetBinContent(36,0.9588745);
   h_gene_EG_Efficiency__1->SetBinContent(37,0.9587156);
   h_gene_EG_Efficiency__1->SetBinContent(38,0.9537789);
   h_gene_EG_Efficiency__1->SetBinContent(39,0.96322);
   h_gene_EG_Efficiency__1->SetBinContent(40,0.9584615);
   h_gene_EG_Efficiency__1->SetBinContent(41,0.9627832);
   h_gene_EG_Efficiency__1->SetBinContent(42,0.9624126);
   h_gene_EG_Efficiency__1->SetBinContent(43,0.9684512);
   h_gene_EG_Efficiency__1->SetBinContent(44,0.9665992);
   h_gene_EG_Efficiency__1->SetBinContent(45,0.9619151);
   h_gene_EG_Efficiency__1->SetBinContent(46,0.9564719);
   h_gene_EG_Efficiency__1->SetBinContent(47,0.9693095);
   h_gene_EG_Efficiency__1->SetBinContent(48,0.9547511);
   h_gene_EG_Efficiency__1->SetBinContent(49,0.9595016);
   h_gene_EG_Efficiency__1->SetBinContent(50,0.9657534);
   h_gene_EG_Efficiency__1->SetBinContent(51,0.9639033);
   h_gene_EG_Efficiency__1->SetEntries(45.1995);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_gene_EG_Efficiency__1->SetFillColor(ci);
   h_gene_EG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_gene_EG_Efficiency__1->SetLineColor(ci);
   h_gene_EG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_gene_EG_Efficiency__1->SetMarkerColor(ci);
   h_gene_EG_Efficiency__1->SetMarkerStyle(33);
   h_gene_EG_Efficiency__1->SetMarkerSize(1.7);
   h_gene_EG_Efficiency__1->GetXaxis()->SetTitle("Gen electron p_{T} [GeV]");
   h_gene_EG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_gene_EG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_gene_EG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_gene_EG_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_gene_EG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_gene_EG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_gene_EG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_gene_EG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_gene_EG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_gene_EG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_gene_EG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_gene_EG_Efficiency__1->Draw("P");
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
