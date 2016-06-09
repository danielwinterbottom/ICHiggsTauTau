void h_qq_Mqq()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:00:37 2016) by ROOT version6.06/01
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
   
   TH1D *h_qq_Mqq__1 = new TH1D("h_qq_Mqq__1","h_qq_Mqq",50,0,2000);
   h_qq_Mqq__1->SetBinContent(1,15850);
   h_qq_Mqq__1->SetBinContent(2,42755);
   h_qq_Mqq__1->SetBinContent(3,62344);
   h_qq_Mqq__1->SetBinContent(4,73122);
   h_qq_Mqq__1->SetBinContent(5,76622);
   h_qq_Mqq__1->SetBinContent(6,76434);
   h_qq_Mqq__1->SetBinContent(7,73718);
   h_qq_Mqq__1->SetBinContent(8,69999);
   h_qq_Mqq__1->SetBinContent(9,65141);
   h_qq_Mqq__1->SetBinContent(10,61281);
   h_qq_Mqq__1->SetBinContent(11,56783);
   h_qq_Mqq__1->SetBinContent(12,52871);
   h_qq_Mqq__1->SetBinContent(13,48570);
   h_qq_Mqq__1->SetBinContent(14,44789);
   h_qq_Mqq__1->SetBinContent(15,41626);
   h_qq_Mqq__1->SetBinContent(16,38572);
   h_qq_Mqq__1->SetBinContent(17,35538);
   h_qq_Mqq__1->SetBinContent(18,32546);
   h_qq_Mqq__1->SetBinContent(19,30532);
   h_qq_Mqq__1->SetBinContent(20,28480);
   h_qq_Mqq__1->SetBinContent(21,26157);
   h_qq_Mqq__1->SetBinContent(22,24334);
   h_qq_Mqq__1->SetBinContent(23,22382);
   h_qq_Mqq__1->SetBinContent(24,21231);
   h_qq_Mqq__1->SetBinContent(25,19610);
   h_qq_Mqq__1->SetBinContent(26,18320);
   h_qq_Mqq__1->SetBinContent(27,17113);
   h_qq_Mqq__1->SetBinContent(28,16103);
   h_qq_Mqq__1->SetBinContent(29,14828);
   h_qq_Mqq__1->SetBinContent(30,13731);
   h_qq_Mqq__1->SetBinContent(31,13143);
   h_qq_Mqq__1->SetBinContent(32,12311);
   h_qq_Mqq__1->SetBinContent(33,11509);
   h_qq_Mqq__1->SetBinContent(34,10561);
   h_qq_Mqq__1->SetBinContent(35,10187);
   h_qq_Mqq__1->SetBinContent(36,9486);
   h_qq_Mqq__1->SetBinContent(37,8968);
   h_qq_Mqq__1->SetBinContent(38,8381);
   h_qq_Mqq__1->SetBinContent(39,7851);
   h_qq_Mqq__1->SetBinContent(40,7529);
   h_qq_Mqq__1->SetBinContent(41,6999);
   h_qq_Mqq__1->SetBinContent(42,6786);
   h_qq_Mqq__1->SetBinContent(43,6213);
   h_qq_Mqq__1->SetBinContent(44,5865);
   h_qq_Mqq__1->SetBinContent(45,5431);
   h_qq_Mqq__1->SetBinContent(46,5250);
   h_qq_Mqq__1->SetBinContent(47,4927);
   h_qq_Mqq__1->SetBinContent(48,4634);
   h_qq_Mqq__1->SetBinContent(49,4324);
   h_qq_Mqq__1->SetBinContent(50,4268);
   h_qq_Mqq__1->SetBinContent(51,71883);
   h_qq_Mqq__1->SetEntries(1447888);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_qq_Mqq__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_qq_Mqq__1->SetLineColor(ci);
   h_qq_Mqq__1->SetLineWidth(3);
   h_qq_Mqq__1->GetXaxis()->SetTitle("m_{qq} [GeV]");
   h_qq_Mqq__1->GetXaxis()->SetRange(1,100);
   h_qq_Mqq__1->GetXaxis()->SetLabelSize(0.035);
   h_qq_Mqq__1->GetXaxis()->SetTitleSize(0.035);
   h_qq_Mqq__1->GetYaxis()->SetTitle("Normalized Entries");
   h_qq_Mqq__1->GetYaxis()->SetLabelOffset(100);
   h_qq_Mqq__1->GetYaxis()->SetLabelSize(0.035);
   h_qq_Mqq__1->GetYaxis()->SetTitleSize(0.035);
   h_qq_Mqq__1->GetZaxis()->SetLabelSize(0.035);
   h_qq_Mqq__1->GetZaxis()->SetTitleSize(0.035);
   h_qq_Mqq__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
