void h_l1j2_Pt()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:07:26 2016) by ROOT version6.06/01
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
   
   TH1D *h_l1j2_Pt__1 = new TH1D("h_l1j2_Pt__1","h_l1j2_Pt",37,0,296);
   h_l1j2_Pt__1->SetBinContent(1,1694);
   h_l1j2_Pt__1->SetBinContent(2,16535);
   h_l1j2_Pt__1->SetBinContent(3,55742);
   h_l1j2_Pt__1->SetBinContent(4,81701);
   h_l1j2_Pt__1->SetBinContent(5,86247);
   h_l1j2_Pt__1->SetBinContent(6,85043);
   h_l1j2_Pt__1->SetBinContent(7,80766);
   h_l1j2_Pt__1->SetBinContent(8,65964);
   h_l1j2_Pt__1->SetBinContent(9,53720);
   h_l1j2_Pt__1->SetBinContent(10,42542);
   h_l1j2_Pt__1->SetBinContent(11,31998);
   h_l1j2_Pt__1->SetBinContent(12,24107);
   h_l1j2_Pt__1->SetBinContent(13,17842);
   h_l1j2_Pt__1->SetBinContent(14,13307);
   h_l1j2_Pt__1->SetBinContent(15,10255);
   h_l1j2_Pt__1->SetBinContent(16,7441);
   h_l1j2_Pt__1->SetBinContent(17,5716);
   h_l1j2_Pt__1->SetBinContent(18,4135);
   h_l1j2_Pt__1->SetBinContent(19,3116);
   h_l1j2_Pt__1->SetBinContent(20,2334);
   h_l1j2_Pt__1->SetBinContent(21,1799);
   h_l1j2_Pt__1->SetBinContent(22,1292);
   h_l1j2_Pt__1->SetBinContent(23,1035);
   h_l1j2_Pt__1->SetBinContent(24,770);
   h_l1j2_Pt__1->SetBinContent(25,625);
   h_l1j2_Pt__1->SetBinContent(26,500);
   h_l1j2_Pt__1->SetBinContent(27,394);
   h_l1j2_Pt__1->SetBinContent(28,290);
   h_l1j2_Pt__1->SetBinContent(29,213);
   h_l1j2_Pt__1->SetBinContent(30,151);
   h_l1j2_Pt__1->SetBinContent(31,160);
   h_l1j2_Pt__1->SetBinContent(32,102);
   h_l1j2_Pt__1->SetBinContent(33,92);
   h_l1j2_Pt__1->SetBinContent(34,80);
   h_l1j2_Pt__1->SetBinContent(35,54);
   h_l1j2_Pt__1->SetBinContent(36,53);
   h_l1j2_Pt__1->SetBinContent(37,27);
   h_l1j2_Pt__1->SetBinContent(38,192);
   h_l1j2_Pt__1->SetEntries(698034);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_l1j2_Pt__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_l1j2_Pt__1->SetLineColor(ci);
   h_l1j2_Pt__1->SetLineWidth(3);
   h_l1j2_Pt__1->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
   h_l1j2_Pt__1->GetXaxis()->SetRange(1,150);
   h_l1j2_Pt__1->GetXaxis()->SetLabelSize(0.035);
   h_l1j2_Pt__1->GetXaxis()->SetTitleSize(0.035);
   h_l1j2_Pt__1->GetYaxis()->SetTitle("Normalized Entries");
   h_l1j2_Pt__1->GetYaxis()->SetLabelOffset(100);
   h_l1j2_Pt__1->GetYaxis()->SetLabelSize(0.035);
   h_l1j2_Pt__1->GetYaxis()->SetTitleSize(0.035);
   h_l1j2_Pt__1->GetZaxis()->SetLabelSize(0.035);
   h_l1j2_Pt__1->GetZaxis()->SetTitleSize(0.035);
   h_l1j2_Pt__1->Draw("");
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
