void h_qq_DeltaEta()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:01:12 2016) by ROOT version6.06/01
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
   
   TH1D *h_qq_DeltaEta__1 = new TH1D("h_qq_DeltaEta__1","h_qq_DeltaEta",50,0,10);
   h_qq_DeltaEta__1->SetBinContent(1,2751);
   h_qq_DeltaEta__1->SetBinContent(2,2957);
   h_qq_DeltaEta__1->SetBinContent(3,3430);
   h_qq_DeltaEta__1->SetBinContent(4,4181);
   h_qq_DeltaEta__1->SetBinContent(5,5213);
   h_qq_DeltaEta__1->SetBinContent(6,6908);
   h_qq_DeltaEta__1->SetBinContent(7,8730);
   h_qq_DeltaEta__1->SetBinContent(8,11115);
   h_qq_DeltaEta__1->SetBinContent(9,14087);
   h_qq_DeltaEta__1->SetBinContent(10,17434);
   h_qq_DeltaEta__1->SetBinContent(11,21367);
   h_qq_DeltaEta__1->SetBinContent(12,25352);
   h_qq_DeltaEta__1->SetBinContent(13,29940);
   h_qq_DeltaEta__1->SetBinContent(14,34733);
   h_qq_DeltaEta__1->SetBinContent(15,39603);
   h_qq_DeltaEta__1->SetBinContent(16,44106);
   h_qq_DeltaEta__1->SetBinContent(17,48197);
   h_qq_DeltaEta__1->SetBinContent(18,51786);
   h_qq_DeltaEta__1->SetBinContent(19,56200);
   h_qq_DeltaEta__1->SetBinContent(20,58296);
   h_qq_DeltaEta__1->SetBinContent(21,61215);
   h_qq_DeltaEta__1->SetBinContent(22,62330);
   h_qq_DeltaEta__1->SetBinContent(23,63284);
   h_qq_DeltaEta__1->SetBinContent(24,63291);
   h_qq_DeltaEta__1->SetBinContent(25,62276);
   h_qq_DeltaEta__1->SetBinContent(26,61110);
   h_qq_DeltaEta__1->SetBinContent(27,59130);
   h_qq_DeltaEta__1->SetBinContent(28,56819);
   h_qq_DeltaEta__1->SetBinContent(29,54025);
   h_qq_DeltaEta__1->SetBinContent(30,50622);
   h_qq_DeltaEta__1->SetBinContent(31,47157);
   h_qq_DeltaEta__1->SetBinContent(32,43787);
   h_qq_DeltaEta__1->SetBinContent(33,39708);
   h_qq_DeltaEta__1->SetBinContent(34,35854);
   h_qq_DeltaEta__1->SetBinContent(35,31620);
   h_qq_DeltaEta__1->SetBinContent(36,28285);
   h_qq_DeltaEta__1->SetBinContent(37,24623);
   h_qq_DeltaEta__1->SetBinContent(38,21322);
   h_qq_DeltaEta__1->SetBinContent(39,18117);
   h_qq_DeltaEta__1->SetBinContent(40,15202);
   h_qq_DeltaEta__1->SetBinContent(41,12685);
   h_qq_DeltaEta__1->SetBinContent(42,10411);
   h_qq_DeltaEta__1->SetBinContent(43,8491);
   h_qq_DeltaEta__1->SetBinContent(44,6790);
   h_qq_DeltaEta__1->SetBinContent(45,5412);
   h_qq_DeltaEta__1->SetBinContent(46,4314);
   h_qq_DeltaEta__1->SetBinContent(47,3373);
   h_qq_DeltaEta__1->SetBinContent(48,2597);
   h_qq_DeltaEta__1->SetBinContent(49,1921);
   h_qq_DeltaEta__1->SetBinContent(50,1514);
   h_qq_DeltaEta__1->SetBinContent(51,4217);
   h_qq_DeltaEta__1->SetEntries(1447888);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_qq_DeltaEta__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_qq_DeltaEta__1->SetLineColor(ci);
   h_qq_DeltaEta__1->SetLineWidth(3);
   h_qq_DeltaEta__1->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
   h_qq_DeltaEta__1->GetXaxis()->SetRange(1,100);
   h_qq_DeltaEta__1->GetXaxis()->SetLabelSize(0.035);
   h_qq_DeltaEta__1->GetXaxis()->SetTitleSize(0.035);
   h_qq_DeltaEta__1->GetYaxis()->SetTitle("Normalized Entries");
   h_qq_DeltaEta__1->GetYaxis()->SetLabelOffset(100);
   h_qq_DeltaEta__1->GetYaxis()->SetLabelSize(0.035);
   h_qq_DeltaEta__1->GetYaxis()->SetTitleSize(0.035);
   h_qq_DeltaEta__1->GetZaxis()->SetLabelSize(0.035);
   h_qq_DeltaEta__1->GetZaxis()->SetTitleSize(0.035);
   h_qq_DeltaEta__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
