void h_jj_Mjj()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:04:06 2016) by ROOT version6.06/01
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
   
   TH1D *h_jj_Mjj__1 = new TH1D("h_jj_Mjj__1","h_jj_Mjj",50,0,2000);
   h_jj_Mjj__1->SetBinContent(1,857);
   h_jj_Mjj__1->SetBinContent(2,10652);
   h_jj_Mjj__1->SetBinContent(3,26195);
   h_jj_Mjj__1->SetBinContent(4,37749);
   h_jj_Mjj__1->SetBinContent(5,44667);
   h_jj_Mjj__1->SetBinContent(6,46845);
   h_jj_Mjj__1->SetBinContent(7,47720);
   h_jj_Mjj__1->SetBinContent(8,46798);
   h_jj_Mjj__1->SetBinContent(9,44673);
   h_jj_Mjj__1->SetBinContent(10,42639);
   h_jj_Mjj__1->SetBinContent(11,39828);
   h_jj_Mjj__1->SetBinContent(12,37553);
   h_jj_Mjj__1->SetBinContent(13,35365);
   h_jj_Mjj__1->SetBinContent(14,32758);
   h_jj_Mjj__1->SetBinContent(15,30266);
   h_jj_Mjj__1->SetBinContent(16,28427);
   h_jj_Mjj__1->SetBinContent(17,25956);
   h_jj_Mjj__1->SetBinContent(18,24398);
   h_jj_Mjj__1->SetBinContent(19,22857);
   h_jj_Mjj__1->SetBinContent(20,21396);
   h_jj_Mjj__1->SetBinContent(21,19949);
   h_jj_Mjj__1->SetBinContent(22,18574);
   h_jj_Mjj__1->SetBinContent(23,17045);
   h_jj_Mjj__1->SetBinContent(24,15975);
   h_jj_Mjj__1->SetBinContent(25,15097);
   h_jj_Mjj__1->SetBinContent(26,13985);
   h_jj_Mjj__1->SetBinContent(27,13132);
   h_jj_Mjj__1->SetBinContent(28,12163);
   h_jj_Mjj__1->SetBinContent(29,11651);
   h_jj_Mjj__1->SetBinContent(30,10599);
   h_jj_Mjj__1->SetBinContent(31,10145);
   h_jj_Mjj__1->SetBinContent(32,9471);
   h_jj_Mjj__1->SetBinContent(33,8843);
   h_jj_Mjj__1->SetBinContent(34,8290);
   h_jj_Mjj__1->SetBinContent(35,7665);
   h_jj_Mjj__1->SetBinContent(36,7304);
   h_jj_Mjj__1->SetBinContent(37,6966);
   h_jj_Mjj__1->SetBinContent(38,6479);
   h_jj_Mjj__1->SetBinContent(39,6178);
   h_jj_Mjj__1->SetBinContent(40,5703);
   h_jj_Mjj__1->SetBinContent(41,5366);
   h_jj_Mjj__1->SetBinContent(42,5041);
   h_jj_Mjj__1->SetBinContent(43,4867);
   h_jj_Mjj__1->SetBinContent(44,4421);
   h_jj_Mjj__1->SetBinContent(45,4187);
   h_jj_Mjj__1->SetBinContent(46,4044);
   h_jj_Mjj__1->SetBinContent(47,3668);
   h_jj_Mjj__1->SetBinContent(48,3560);
   h_jj_Mjj__1->SetBinContent(49,3374);
   h_jj_Mjj__1->SetBinContent(50,3150);
   h_jj_Mjj__1->SetBinContent(51,50194);
   h_jj_Mjj__1->SetEntries(964685);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jj_Mjj__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_jj_Mjj__1->SetLineColor(ci);
   h_jj_Mjj__1->SetLineWidth(3);
   h_jj_Mjj__1->GetXaxis()->SetTitle("m_{jj} [GeV]");
   h_jj_Mjj__1->GetXaxis()->SetRange(1,100);
   h_jj_Mjj__1->GetXaxis()->SetLabelSize(0.035);
   h_jj_Mjj__1->GetXaxis()->SetTitleSize(0.035);
   h_jj_Mjj__1->GetYaxis()->SetTitle("Normalized Entries");
   h_jj_Mjj__1->GetYaxis()->SetLabelOffset(100);
   h_jj_Mjj__1->GetYaxis()->SetLabelSize(0.035);
   h_jj_Mjj__1->GetYaxis()->SetTitleSize(0.035);
   h_jj_Mjj__1->GetZaxis()->SetLabelSize(0.035);
   h_jj_Mjj__1->GetZaxis()->SetTitleSize(0.035);
   h_jj_Mjj__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
