void h_l1jj_l1Mjj()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:07:51 2016) by ROOT version6.06/01
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
   
   TH1D *h_l1jj_l1Mjj__1 = new TH1D("h_l1jj_l1Mjj__1","h_l1M_Mjj",50,0,2000);
   h_l1jj_l1Mjj__1->SetBinContent(1,719);
   h_l1jj_l1Mjj__1->SetBinContent(2,5758);
   h_l1jj_l1Mjj__1->SetBinContent(3,14014);
   h_l1jj_l1Mjj__1->SetBinContent(4,21320);
   h_l1jj_l1Mjj__1->SetBinContent(5,26427);
   h_l1jj_l1Mjj__1->SetBinContent(6,28983);
   h_l1jj_l1Mjj__1->SetBinContent(7,30097);
   h_l1jj_l1Mjj__1->SetBinContent(8,30397);
   h_l1jj_l1Mjj__1->SetBinContent(9,29854);
   h_l1jj_l1Mjj__1->SetBinContent(10,28652);
   h_l1jj_l1Mjj__1->SetBinContent(11,27142);
   h_l1jj_l1Mjj__1->SetBinContent(12,26168);
   h_l1jj_l1Mjj__1->SetBinContent(13,24634);
   h_l1jj_l1Mjj__1->SetBinContent(14,23238);
   h_l1jj_l1Mjj__1->SetBinContent(15,21984);
   h_l1jj_l1Mjj__1->SetBinContent(16,20287);
   h_l1jj_l1Mjj__1->SetBinContent(17,19347);
   h_l1jj_l1Mjj__1->SetBinContent(18,18008);
   h_l1jj_l1Mjj__1->SetBinContent(19,16676);
   h_l1jj_l1Mjj__1->SetBinContent(20,15604);
   h_l1jj_l1Mjj__1->SetBinContent(21,14708);
   h_l1jj_l1Mjj__1->SetBinContent(22,13698);
   h_l1jj_l1Mjj__1->SetBinContent(23,13064);
   h_l1jj_l1Mjj__1->SetBinContent(24,12024);
   h_l1jj_l1Mjj__1->SetBinContent(25,11405);
   h_l1jj_l1Mjj__1->SetBinContent(26,10756);
   h_l1jj_l1Mjj__1->SetBinContent(27,10019);
   h_l1jj_l1Mjj__1->SetBinContent(28,9489);
   h_l1jj_l1Mjj__1->SetBinContent(29,8907);
   h_l1jj_l1Mjj__1->SetBinContent(30,8240);
   h_l1jj_l1Mjj__1->SetBinContent(31,7792);
   h_l1jj_l1Mjj__1->SetBinContent(32,7509);
   h_l1jj_l1Mjj__1->SetBinContent(33,7058);
   h_l1jj_l1Mjj__1->SetBinContent(34,6577);
   h_l1jj_l1Mjj__1->SetBinContent(35,6186);
   h_l1jj_l1Mjj__1->SetBinContent(36,5949);
   h_l1jj_l1Mjj__1->SetBinContent(37,5612);
   h_l1jj_l1Mjj__1->SetBinContent(38,5319);
   h_l1jj_l1Mjj__1->SetBinContent(39,4988);
   h_l1jj_l1Mjj__1->SetBinContent(40,4634);
   h_l1jj_l1Mjj__1->SetBinContent(41,4366);
   h_l1jj_l1Mjj__1->SetBinContent(42,4216);
   h_l1jj_l1Mjj__1->SetBinContent(43,4032);
   h_l1jj_l1Mjj__1->SetBinContent(44,3853);
   h_l1jj_l1Mjj__1->SetBinContent(45,3554);
   h_l1jj_l1Mjj__1->SetBinContent(46,3492);
   h_l1jj_l1Mjj__1->SetBinContent(47,3198);
   h_l1jj_l1Mjj__1->SetBinContent(48,3066);
   h_l1jj_l1Mjj__1->SetBinContent(49,2963);
   h_l1jj_l1Mjj__1->SetBinContent(50,2818);
   h_l1jj_l1Mjj__1->SetBinContent(51,59233);
   h_l1jj_l1Mjj__1->SetEntries(698034);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_l1jj_l1Mjj__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_l1jj_l1Mjj__1->SetLineColor(ci);
   h_l1jj_l1Mjj__1->SetLineWidth(3);
   h_l1jj_l1Mjj__1->GetXaxis()->SetTitle("m_{jj} [GeV]");
   h_l1jj_l1Mjj__1->GetXaxis()->SetRange(1,100);
   h_l1jj_l1Mjj__1->GetXaxis()->SetLabelSize(0.035);
   h_l1jj_l1Mjj__1->GetXaxis()->SetTitleSize(0.035);
   h_l1jj_l1Mjj__1->GetYaxis()->SetTitle("Normalized Entries");
   h_l1jj_l1Mjj__1->GetYaxis()->SetLabelOffset(100);
   h_l1jj_l1Mjj__1->GetYaxis()->SetLabelSize(0.035);
   h_l1jj_l1Mjj__1->GetYaxis()->SetTitleSize(0.035);
   h_l1jj_l1Mjj__1->GetZaxis()->SetLabelSize(0.035);
   h_l1jj_l1Mjj__1->GetZaxis()->SetTitleSize(0.035);
   h_l1jj_l1Mjj__1->Draw("");
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
