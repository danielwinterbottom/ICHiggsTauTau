void h_l1j1_Pt()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 18:07:10 2016) by ROOT version6.06/01
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
   
   TH1D *h_l1j1_Pt__1 = new TH1D("h_l1j1_Pt__1","h_l1j1_Pt",37,0,296);
   h_l1j1_Pt__1->SetBinContent(1,1);
   h_l1j1_Pt__1->SetBinContent(2,162);
   h_l1j1_Pt__1->SetBinContent(3,2199);
   h_l1j1_Pt__1->SetBinContent(4,8407);
   h_l1j1_Pt__1->SetBinContent(5,16771);
   h_l1j1_Pt__1->SetBinContent(6,27337);
   h_l1j1_Pt__1->SetBinContent(7,39892);
   h_l1j1_Pt__1->SetBinContent(8,46599);
   h_l1j1_Pt__1->SetBinContent(9,50916);
   h_l1j1_Pt__1->SetBinContent(10,53752);
   h_l1j1_Pt__1->SetBinContent(11,52028);
   h_l1j1_Pt__1->SetBinContent(12,49091);
   h_l1j1_Pt__1->SetBinContent(13,43653);
   h_l1j1_Pt__1->SetBinContent(14,39486);
   h_l1j1_Pt__1->SetBinContent(15,37154);
   h_l1j1_Pt__1->SetBinContent(16,31057);
   h_l1j1_Pt__1->SetBinContent(17,27542);
   h_l1j1_Pt__1->SetBinContent(18,23737);
   h_l1j1_Pt__1->SetBinContent(19,20549);
   h_l1j1_Pt__1->SetBinContent(20,17576);
   h_l1j1_Pt__1->SetBinContent(21,15184);
   h_l1j1_Pt__1->SetBinContent(22,12751);
   h_l1j1_Pt__1->SetBinContent(23,11079);
   h_l1j1_Pt__1->SetBinContent(24,9485);
   h_l1j1_Pt__1->SetBinContent(25,8082);
   h_l1j1_Pt__1->SetBinContent(26,7219);
   h_l1j1_Pt__1->SetBinContent(27,6216);
   h_l1j1_Pt__1->SetBinContent(28,5534);
   h_l1j1_Pt__1->SetBinContent(29,4368);
   h_l1j1_Pt__1->SetBinContent(30,3870);
   h_l1j1_Pt__1->SetBinContent(31,3353);
   h_l1j1_Pt__1->SetBinContent(32,2965);
   h_l1j1_Pt__1->SetBinContent(33,2414);
   h_l1j1_Pt__1->SetBinContent(34,2179);
   h_l1j1_Pt__1->SetBinContent(35,1877);
   h_l1j1_Pt__1->SetBinContent(36,1678);
   h_l1j1_Pt__1->SetBinContent(37,1371);
   h_l1j1_Pt__1->SetBinContent(38,10500);
   h_l1j1_Pt__1->SetEntries(698034);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_l1j1_Pt__1->SetFillColor(ci);

   ci = TColor::GetColor("#000099");
   h_l1j1_Pt__1->SetLineColor(ci);
   h_l1j1_Pt__1->SetLineWidth(3);
   h_l1j1_Pt__1->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
   h_l1j1_Pt__1->GetXaxis()->SetRange(1,150);
   h_l1j1_Pt__1->GetXaxis()->SetLabelSize(0.035);
   h_l1j1_Pt__1->GetXaxis()->SetTitleSize(0.035);
   h_l1j1_Pt__1->GetYaxis()->SetTitle("Normalized Entries");
   h_l1j1_Pt__1->GetYaxis()->SetLabelOffset(100);
   h_l1j1_Pt__1->GetYaxis()->SetLabelSize(0.035);
   h_l1j1_Pt__1->GetYaxis()->SetTitleSize(0.035);
   h_l1j1_Pt__1->GetZaxis()->SetLabelSize(0.035);
   h_l1j1_Pt__1->GetZaxis()->SetTitleSize(0.035);
   h_l1j1_Pt__1->Draw("");
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
