void h_l1jj_DeltaEta()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:08:22 2016) by ROOT version6.06/01
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
   
   TH1D *h_l1jj_DeltaEta__1 = new TH1D("h_l1jj_DeltaEta__1","h_l1jj_DeltaEta",50,0,10);
   h_l1jj_DeltaEta__1->SetBinContent(1,1632);
   h_l1jj_DeltaEta__1->SetBinContent(2,1541);
   h_l1jj_DeltaEta__1->SetBinContent(3,1568);
   h_l1jj_DeltaEta__1->SetBinContent(4,2839);
   h_l1jj_DeltaEta__1->SetBinContent(5,2816);
   h_l1jj_DeltaEta__1->SetBinContent(6,3946);
   h_l1jj_DeltaEta__1->SetBinContent(7,6713);
   h_l1jj_DeltaEta__1->SetBinContent(8,6106);
   h_l1jj_DeltaEta__1->SetBinContent(9,8623);
   h_l1jj_DeltaEta__1->SetBinContent(10,9454);
   h_l1jj_DeltaEta__1->SetBinContent(11,15343);
   h_l1jj_DeltaEta__1->SetBinContent(12,16518);
   h_l1jj_DeltaEta__1->SetBinContent(13,16217);
   h_l1jj_DeltaEta__1->SetBinContent(14,23870);
   h_l1jj_DeltaEta__1->SetBinContent(15,20811);
   h_l1jj_DeltaEta__1->SetBinContent(16,28391);
   h_l1jj_DeltaEta__1->SetBinContent(17,30779);
   h_l1jj_DeltaEta__1->SetBinContent(18,26287);
   h_l1jj_DeltaEta__1->SetBinContent(19,36342);
   h_l1jj_DeltaEta__1->SetBinContent(20,28865);
   h_l1jj_DeltaEta__1->SetBinContent(21,35868);
   h_l1jj_DeltaEta__1->SetBinContent(22,38571);
   h_l1jj_DeltaEta__1->SetBinContent(23,29055);
   h_l1jj_DeltaEta__1->SetBinContent(24,31153);
   h_l1jj_DeltaEta__1->SetBinContent(25,26838);
   h_l1jj_DeltaEta__1->SetBinContent(26,39079);
   h_l1jj_DeltaEta__1->SetBinContent(27,24459);
   h_l1jj_DeltaEta__1->SetBinContent(28,23284);
   h_l1jj_DeltaEta__1->SetBinContent(29,28324);
   h_l1jj_DeltaEta__1->SetBinContent(30,21666);
   h_l1jj_DeltaEta__1->SetBinContent(31,21895);
   h_l1jj_DeltaEta__1->SetBinContent(32,14564);
   h_l1jj_DeltaEta__1->SetBinContent(33,12664);
   h_l1jj_DeltaEta__1->SetBinContent(34,14561);
   h_l1jj_DeltaEta__1->SetBinContent(35,9983);
   h_l1jj_DeltaEta__1->SetBinContent(36,10553);
   h_l1jj_DeltaEta__1->SetBinContent(37,5116);
   h_l1jj_DeltaEta__1->SetBinContent(38,5233);
   h_l1jj_DeltaEta__1->SetBinContent(39,5014);
   h_l1jj_DeltaEta__1->SetBinContent(40,3909);
   h_l1jj_DeltaEta__1->SetBinContent(41,4848);
   h_l1jj_DeltaEta__1->SetBinContent(42,1327);
   h_l1jj_DeltaEta__1->SetBinContent(43,777);
   h_l1jj_DeltaEta__1->SetBinContent(44,476);
   h_l1jj_DeltaEta__1->SetBinContent(45,156);
   h_l1jj_DeltaEta__1->SetEntries(698034);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_l1jj_DeltaEta__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_l1jj_DeltaEta__1->SetLineColor(ci);
   h_l1jj_DeltaEta__1->SetLineWidth(3);
   h_l1jj_DeltaEta__1->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
   h_l1jj_DeltaEta__1->GetXaxis()->SetRange(1,100);
   h_l1jj_DeltaEta__1->GetXaxis()->SetLabelSize(0.035);
   h_l1jj_DeltaEta__1->GetXaxis()->SetTitleSize(0.035);
   h_l1jj_DeltaEta__1->GetYaxis()->SetTitle("Normalized Entries");
   h_l1jj_DeltaEta__1->GetYaxis()->SetLabelOffset(100);
   h_l1jj_DeltaEta__1->GetYaxis()->SetLabelSize(0.035);
   h_l1jj_DeltaEta__1->GetYaxis()->SetTitleSize(0.035);
   h_l1jj_DeltaEta__1->GetZaxis()->SetLabelSize(0.035);
   h_l1jj_DeltaEta__1->GetZaxis()->SetTitleSize(0.035);
   h_l1jj_DeltaEta__1->Draw("");
   TLatex *   tex = new TLatex(0.65,0.92,"l1t-integration-v58.1");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
