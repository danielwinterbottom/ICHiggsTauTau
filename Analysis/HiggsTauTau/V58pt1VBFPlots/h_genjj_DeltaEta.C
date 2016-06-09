void h_genjj_DeltaEta()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:03:12 2016) by ROOT version6.06/01
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
   
   TH1D *h_genjj_DeltaEta__1 = new TH1D("h_genjj_DeltaEta__1","h_genjj_DeltaEta",50,0,10);
   h_genjj_DeltaEta__1->SetBinContent(1,2053);
   h_genjj_DeltaEta__1->SetBinContent(2,2286);
   h_genjj_DeltaEta__1->SetBinContent(3,2732);
   h_genjj_DeltaEta__1->SetBinContent(4,3261);
   h_genjj_DeltaEta__1->SetBinContent(5,4232);
   h_genjj_DeltaEta__1->SetBinContent(6,5671);
   h_genjj_DeltaEta__1->SetBinContent(7,7336);
   h_genjj_DeltaEta__1->SetBinContent(8,9375);
   h_genjj_DeltaEta__1->SetBinContent(9,12054);
   h_genjj_DeltaEta__1->SetBinContent(10,14763);
   h_genjj_DeltaEta__1->SetBinContent(11,18264);
   h_genjj_DeltaEta__1->SetBinContent(12,21994);
   h_genjj_DeltaEta__1->SetBinContent(13,25868);
   h_genjj_DeltaEta__1->SetBinContent(14,30158);
   h_genjj_DeltaEta__1->SetBinContent(15,34223);
   h_genjj_DeltaEta__1->SetBinContent(16,38092);
   h_genjj_DeltaEta__1->SetBinContent(17,41253);
   h_genjj_DeltaEta__1->SetBinContent(18,44595);
   h_genjj_DeltaEta__1->SetBinContent(19,47883);
   h_genjj_DeltaEta__1->SetBinContent(20,49601);
   h_genjj_DeltaEta__1->SetBinContent(21,51315);
   h_genjj_DeltaEta__1->SetBinContent(22,52234);
   h_genjj_DeltaEta__1->SetBinContent(23,52158);
   h_genjj_DeltaEta__1->SetBinContent(24,51918);
   h_genjj_DeltaEta__1->SetBinContent(25,50614);
   h_genjj_DeltaEta__1->SetBinContent(26,49099);
   h_genjj_DeltaEta__1->SetBinContent(27,47099);
   h_genjj_DeltaEta__1->SetBinContent(28,44149);
   h_genjj_DeltaEta__1->SetBinContent(29,41444);
   h_genjj_DeltaEta__1->SetBinContent(30,38354);
   h_genjj_DeltaEta__1->SetBinContent(31,35043);
   h_genjj_DeltaEta__1->SetBinContent(32,31652);
   h_genjj_DeltaEta__1->SetBinContent(33,28373);
   h_genjj_DeltaEta__1->SetBinContent(34,24790);
   h_genjj_DeltaEta__1->SetBinContent(35,21515);
   h_genjj_DeltaEta__1->SetBinContent(36,18513);
   h_genjj_DeltaEta__1->SetBinContent(37,15277);
   h_genjj_DeltaEta__1->SetBinContent(38,12967);
   h_genjj_DeltaEta__1->SetBinContent(39,10310);
   h_genjj_DeltaEta__1->SetBinContent(40,8252);
   h_genjj_DeltaEta__1->SetBinContent(41,6281);
   h_genjj_DeltaEta__1->SetBinContent(42,4676);
   h_genjj_DeltaEta__1->SetBinContent(43,3402);
   h_genjj_DeltaEta__1->SetBinContent(44,2306);
   h_genjj_DeltaEta__1->SetBinContent(45,1553);
   h_genjj_DeltaEta__1->SetBinContent(46,1009);
   h_genjj_DeltaEta__1->SetBinContent(47,623);
   h_genjj_DeltaEta__1->SetBinContent(48,309);
   h_genjj_DeltaEta__1->SetBinContent(49,130);
   h_genjj_DeltaEta__1->SetBinContent(50,32);
   h_genjj_DeltaEta__1->SetEntries(1121091);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_genjj_DeltaEta__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_genjj_DeltaEta__1->SetLineColor(ci);
   h_genjj_DeltaEta__1->SetLineWidth(3);
   h_genjj_DeltaEta__1->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
   h_genjj_DeltaEta__1->GetXaxis()->SetRange(1,100);
   h_genjj_DeltaEta__1->GetXaxis()->SetLabelSize(0.035);
   h_genjj_DeltaEta__1->GetXaxis()->SetTitleSize(0.035);
   h_genjj_DeltaEta__1->GetYaxis()->SetTitle("Normalized Entries");
   h_genjj_DeltaEta__1->GetYaxis()->SetLabelOffset(100);
   h_genjj_DeltaEta__1->GetYaxis()->SetLabelSize(0.035);
   h_genjj_DeltaEta__1->GetYaxis()->SetTitleSize(0.035);
   h_genjj_DeltaEta__1->GetZaxis()->SetLabelSize(0.035);
   h_genjj_DeltaEta__1->GetZaxis()->SetTitleSize(0.035);
   h_genjj_DeltaEta__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
