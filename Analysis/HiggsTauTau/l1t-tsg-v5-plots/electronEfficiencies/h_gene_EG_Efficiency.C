void h_gene_EG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon May  2 00:34:09 2016) by ROOT version6.06/01
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
   
   TH1D *h_gene_EG_Efficiency__1 = new TH1D("h_gene_EG_Efficiency__1","h_gene_EG_Efficiency",50,0,100);
   h_gene_EG_Efficiency__1->SetBinContent(1,0.06313009);
   h_gene_EG_Efficiency__1->SetBinContent(2,0.2652939);
   h_gene_EG_Efficiency__1->SetBinContent(3,0.6696086);
   h_gene_EG_Efficiency__1->SetBinContent(4,0.8606323);
   h_gene_EG_Efficiency__1->SetBinContent(5,0.9074591);
   h_gene_EG_Efficiency__1->SetBinContent(6,0.9233482);
   h_gene_EG_Efficiency__1->SetBinContent(7,0.9323548);
   h_gene_EG_Efficiency__1->SetBinContent(8,0.9351041);
   h_gene_EG_Efficiency__1->SetBinContent(9,0.9400293);
   h_gene_EG_Efficiency__1->SetBinContent(10,0.9433286);
   h_gene_EG_Efficiency__1->SetBinContent(11,0.9471736);
   h_gene_EG_Efficiency__1->SetBinContent(12,0.9497969);
   h_gene_EG_Efficiency__1->SetBinContent(13,0.9466232);
   h_gene_EG_Efficiency__1->SetBinContent(14,0.9497329);
   h_gene_EG_Efficiency__1->SetBinContent(15,0.9512016);
   h_gene_EG_Efficiency__1->SetBinContent(16,0.9526376);
   h_gene_EG_Efficiency__1->SetBinContent(17,0.9549446);
   h_gene_EG_Efficiency__1->SetBinContent(18,0.9564213);
   h_gene_EG_Efficiency__1->SetBinContent(19,0.9604062);
   h_gene_EG_Efficiency__1->SetBinContent(20,0.9583539);
   h_gene_EG_Efficiency__1->SetBinContent(21,0.961919);
   h_gene_EG_Efficiency__1->SetBinContent(22,0.9610632);
   h_gene_EG_Efficiency__1->SetBinContent(23,0.9606504);
   h_gene_EG_Efficiency__1->SetBinContent(24,0.9596251);
   h_gene_EG_Efficiency__1->SetBinContent(25,0.9586473);
   h_gene_EG_Efficiency__1->SetBinContent(26,0.9618819);
   h_gene_EG_Efficiency__1->SetBinContent(27,0.9614306);
   h_gene_EG_Efficiency__1->SetBinContent(28,0.9645749);
   h_gene_EG_Efficiency__1->SetBinContent(29,0.9671543);
   h_gene_EG_Efficiency__1->SetBinContent(30,0.9579238);
   h_gene_EG_Efficiency__1->SetBinContent(31,0.9619342);
   h_gene_EG_Efficiency__1->SetBinContent(32,0.9594446);
   h_gene_EG_Efficiency__1->SetBinContent(33,0.9682731);
   h_gene_EG_Efficiency__1->SetBinContent(34,0.9573251);
   h_gene_EG_Efficiency__1->SetBinContent(35,0.9695946);
   h_gene_EG_Efficiency__1->SetBinContent(36,0.9644588);
   h_gene_EG_Efficiency__1->SetBinContent(37,0.9613636);
   h_gene_EG_Efficiency__1->SetBinContent(38,0.9580506);
   h_gene_EG_Efficiency__1->SetBinContent(39,0.9643592);
   h_gene_EG_Efficiency__1->SetBinContent(40,0.9611872);
   h_gene_EG_Efficiency__1->SetBinContent(41,0.9666667);
   h_gene_EG_Efficiency__1->SetBinContent(42,0.9690987);
   h_gene_EG_Efficiency__1->SetBinContent(43,0.9736098);
   h_gene_EG_Efficiency__1->SetBinContent(44,0.9699399);
   h_gene_EG_Efficiency__1->SetBinContent(45,0.9614973);
   h_gene_EG_Efficiency__1->SetBinContent(46,0.9633028);
   h_gene_EG_Efficiency__1->SetBinContent(47,0.9723965);
   h_gene_EG_Efficiency__1->SetBinContent(48,0.9582712);
   h_gene_EG_Efficiency__1->SetBinContent(49,0.9644513);
   h_gene_EG_Efficiency__1->SetBinContent(50,0.9662162);
   h_gene_EG_Efficiency__1->SetBinContent(51,0.966316);
   h_gene_EG_Efficiency__1->SetEntries(45.87389);

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
   h_gene_EG_Efficiency__1->GetXaxis()->SetTitle("Gen electron p_{T} GeV");
   h_gene_EG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_gene_EG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_gene_EG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_gene_EG_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_gene_EG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_gene_EG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_gene_EG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_gene_EG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_gene_EG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_gene_EG_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen electron |#eta| < 2.4 ");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
