void h_jj_DeltaEta()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:04:29 2016) by ROOT version6.06/01
   TCanvas *c1 = new TCanvas("c1", "c1",904,191,850,850);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.125);
   c1->SetRightMargin(0.125);
   c1->SetFrameBorderMode(0);
   
   TH1D *h_jj_DeltaEta__1 = new TH1D("h_jj_DeltaEta__1","h_jj_DeltaEta",50,0,10);
   h_jj_DeltaEta__1->SetBinContent(1,1795);
   h_jj_DeltaEta__1->SetBinContent(2,1998);
   h_jj_DeltaEta__1->SetBinContent(3,2358);
   h_jj_DeltaEta__1->SetBinContent(4,2903);
   h_jj_DeltaEta__1->SetBinContent(5,3818);
   h_jj_DeltaEta__1->SetBinContent(6,5026);
   h_jj_DeltaEta__1->SetBinContent(7,6618);
   h_jj_DeltaEta__1->SetBinContent(8,8490);
   h_jj_DeltaEta__1->SetBinContent(9,10815);
   h_jj_DeltaEta__1->SetBinContent(10,13501);
   h_jj_DeltaEta__1->SetBinContent(11,16574);
   h_jj_DeltaEta__1->SetBinContent(12,19841);
   h_jj_DeltaEta__1->SetBinContent(13,23290);
   h_jj_DeltaEta__1->SetBinContent(14,27413);
   h_jj_DeltaEta__1->SetBinContent(15,30633);
   h_jj_DeltaEta__1->SetBinContent(16,33883);
   h_jj_DeltaEta__1->SetBinContent(17,36806);
   h_jj_DeltaEta__1->SetBinContent(18,39788);
   h_jj_DeltaEta__1->SetBinContent(19,42479);
   h_jj_DeltaEta__1->SetBinContent(20,43847);
   h_jj_DeltaEta__1->SetBinContent(21,45238);
   h_jj_DeltaEta__1->SetBinContent(22,45626);
   h_jj_DeltaEta__1->SetBinContent(23,45469);
   h_jj_DeltaEta__1->SetBinContent(24,44995);
   h_jj_DeltaEta__1->SetBinContent(25,43742);
   h_jj_DeltaEta__1->SetBinContent(26,42259);
   h_jj_DeltaEta__1->SetBinContent(27,40083);
   h_jj_DeltaEta__1->SetBinContent(28,37682);
   h_jj_DeltaEta__1->SetBinContent(29,34760);
   h_jj_DeltaEta__1->SetBinContent(30,32240);
   h_jj_DeltaEta__1->SetBinContent(31,29555);
   h_jj_DeltaEta__1->SetBinContent(32,26291);
   h_jj_DeltaEta__1->SetBinContent(33,23093);
   h_jj_DeltaEta__1->SetBinContent(34,20102);
   h_jj_DeltaEta__1->SetBinContent(35,17350);
   h_jj_DeltaEta__1->SetBinContent(36,14741);
   h_jj_DeltaEta__1->SetBinContent(37,12094);
   h_jj_DeltaEta__1->SetBinContent(38,9935);
   h_jj_DeltaEta__1->SetBinContent(39,7809);
   h_jj_DeltaEta__1->SetBinContent(40,6070);
   h_jj_DeltaEta__1->SetBinContent(41,4612);
   h_jj_DeltaEta__1->SetBinContent(42,3331);
   h_jj_DeltaEta__1->SetBinContent(43,2311);
   h_jj_DeltaEta__1->SetBinContent(44,1496);
   h_jj_DeltaEta__1->SetBinContent(45,911);
   h_jj_DeltaEta__1->SetBinContent(46,522);
   h_jj_DeltaEta__1->SetBinContent(47,329);
   h_jj_DeltaEta__1->SetBinContent(48,119);
   h_jj_DeltaEta__1->SetBinContent(49,37);
   h_jj_DeltaEta__1->SetBinContent(50,7);
   h_jj_DeltaEta__1->SetEntries(964685);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jj_DeltaEta__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_jj_DeltaEta__1->SetLineColor(ci);
   h_jj_DeltaEta__1->SetLineWidth(3);
   h_jj_DeltaEta__1->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
   h_jj_DeltaEta__1->GetXaxis()->SetRange(1,100);
   h_jj_DeltaEta__1->GetXaxis()->SetLabelSize(0.035);
   h_jj_DeltaEta__1->GetXaxis()->SetTitleSize(0.035);
   h_jj_DeltaEta__1->GetYaxis()->SetTitle("Normalized Entries");
   h_jj_DeltaEta__1->GetYaxis()->SetLabelOffset(100);
   h_jj_DeltaEta__1->GetYaxis()->SetLabelSize(0.035);
   h_jj_DeltaEta__1->GetYaxis()->SetTitleSize(0.035);
   h_jj_DeltaEta__1->GetZaxis()->SetLabelSize(0.035);
   h_jj_DeltaEta__1->GetZaxis()->SetTitleSize(0.035);
   h_jj_DeltaEta__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
