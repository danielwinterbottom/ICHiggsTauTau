void h_genjj_Mqq()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:05:55 2016) by ROOT version6.06/01
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
   
   TH1D *h_genjj_Mqq__1 = new TH1D("h_genjj_Mqq__1","h_genjj_Mqq",50,0,2000);
   h_genjj_Mqq__1->SetBinContent(1,2785);
   h_genjj_Mqq__1->SetBinContent(2,20366);
   h_genjj_Mqq__1->SetBinContent(3,39497);
   h_genjj_Mqq__1->SetBinContent(4,50596);
   h_genjj_Mqq__1->SetBinContent(5,56482);
   h_genjj_Mqq__1->SetBinContent(6,57840);
   h_genjj_Mqq__1->SetBinContent(7,57131);
   h_genjj_Mqq__1->SetBinContent(8,54604);
   h_genjj_Mqq__1->SetBinContent(9,51977);
   h_genjj_Mqq__1->SetBinContent(10,48544);
   h_genjj_Mqq__1->SetBinContent(11,45251);
   h_genjj_Mqq__1->SetBinContent(12,42578);
   h_genjj_Mqq__1->SetBinContent(13,39418);
   h_genjj_Mqq__1->SetBinContent(14,36460);
   h_genjj_Mqq__1->SetBinContent(15,33997);
   h_genjj_Mqq__1->SetBinContent(16,30749);
   h_genjj_Mqq__1->SetBinContent(17,28675);
   h_genjj_Mqq__1->SetBinContent(18,26990);
   h_genjj_Mqq__1->SetBinContent(19,25072);
   h_genjj_Mqq__1->SetBinContent(20,23360);
   h_genjj_Mqq__1->SetBinContent(21,21466);
   h_genjj_Mqq__1->SetBinContent(22,19936);
   h_genjj_Mqq__1->SetBinContent(23,18783);
   h_genjj_Mqq__1->SetBinContent(24,17568);
   h_genjj_Mqq__1->SetBinContent(25,16170);
   h_genjj_Mqq__1->SetBinContent(26,15076);
   h_genjj_Mqq__1->SetBinContent(27,14017);
   h_genjj_Mqq__1->SetBinContent(28,13269);
   h_genjj_Mqq__1->SetBinContent(29,12270);
   h_genjj_Mqq__1->SetBinContent(30,11642);
   h_genjj_Mqq__1->SetBinContent(31,10728);
   h_genjj_Mqq__1->SetBinContent(32,10112);
   h_genjj_Mqq__1->SetBinContent(33,9669);
   h_genjj_Mqq__1->SetBinContent(34,8974);
   h_genjj_Mqq__1->SetBinContent(35,8609);
   h_genjj_Mqq__1->SetBinContent(36,7911);
   h_genjj_Mqq__1->SetBinContent(37,7464);
   h_genjj_Mqq__1->SetBinContent(38,6979);
   h_genjj_Mqq__1->SetBinContent(39,6588);
   h_genjj_Mqq__1->SetBinContent(40,6216);
   h_genjj_Mqq__1->SetBinContent(41,5850);
   h_genjj_Mqq__1->SetBinContent(42,5579);
   h_genjj_Mqq__1->SetBinContent(43,5208);
   h_genjj_Mqq__1->SetBinContent(44,4970);
   h_genjj_Mqq__1->SetBinContent(45,4673);
   h_genjj_Mqq__1->SetBinContent(46,4264);
   h_genjj_Mqq__1->SetBinContent(47,4041);
   h_genjj_Mqq__1->SetBinContent(48,3936);
   h_genjj_Mqq__1->SetBinContent(49,3669);
   h_genjj_Mqq__1->SetBinContent(50,3473);
   h_genjj_Mqq__1->SetBinContent(51,59609);
   h_genjj_Mqq__1->SetEntries(1121091);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_genjj_Mqq__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_genjj_Mqq__1->SetLineColor(ci);
   h_genjj_Mqq__1->SetLineWidth(3);
   h_genjj_Mqq__1->GetXaxis()->SetTitle("m_{jj} [GeV]");
   h_genjj_Mqq__1->GetXaxis()->SetRange(1,100);
   h_genjj_Mqq__1->GetXaxis()->SetLabelSize(0.035);
   h_genjj_Mqq__1->GetXaxis()->SetTitleSize(0.035);
   h_genjj_Mqq__1->GetYaxis()->SetTitle("Normalized Entries");
   h_genjj_Mqq__1->GetYaxis()->SetLabelOffset(100);
   h_genjj_Mqq__1->GetYaxis()->SetLabelSize(0.035);
   h_genjj_Mqq__1->GetYaxis()->SetTitleSize(0.035);
   h_genjj_Mqq__1->GetZaxis()->SetLabelSize(0.035);
   h_genjj_Mqq__1->GetZaxis()->SetTitleSize(0.035);
   h_genjj_Mqq__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
