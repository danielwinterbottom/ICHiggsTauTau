void h_genmu_Mu_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:33:05 2016) by ROOT version6.06/01
   TCanvas *c1 = new TCanvas("c1", "c1",516,52,850,850);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetGridx();
   c1->SetGridy();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.125);
   c1->SetRightMargin(0.125);
   c1->SetFrameBorderMode(0);
   
   TH1D *h_genmu_Mu_Efficiency__1 = new TH1D("h_genmu_Mu_Efficiency__1","h_genmu_Mu_Efficiency",50,0,100);
   h_genmu_Mu_Efficiency__1->SetBinContent(1,0.01514728);
   h_genmu_Mu_Efficiency__1->SetBinContent(2,0.1921001);
   h_genmu_Mu_Efficiency__1->SetBinContent(3,0.4478247);
   h_genmu_Mu_Efficiency__1->SetBinContent(4,0.9120434);
   h_genmu_Mu_Efficiency__1->SetBinContent(5,0.9371825);
   h_genmu_Mu_Efficiency__1->SetBinContent(6,0.9468014);
   h_genmu_Mu_Efficiency__1->SetBinContent(7,0.9539451);
   h_genmu_Mu_Efficiency__1->SetBinContent(8,0.9549207);
   h_genmu_Mu_Efficiency__1->SetBinContent(9,0.95883);
   h_genmu_Mu_Efficiency__1->SetBinContent(10,0.9649389);
   h_genmu_Mu_Efficiency__1->SetBinContent(11,0.964129);
   h_genmu_Mu_Efficiency__1->SetBinContent(12,0.9653242);
   h_genmu_Mu_Efficiency__1->SetBinContent(13,0.9665474);
   h_genmu_Mu_Efficiency__1->SetBinContent(14,0.966185);
   h_genmu_Mu_Efficiency__1->SetBinContent(15,0.9691558);
   h_genmu_Mu_Efficiency__1->SetBinContent(16,0.9687667);
   h_genmu_Mu_Efficiency__1->SetBinContent(17,0.9705596);
   h_genmu_Mu_Efficiency__1->SetBinContent(18,0.967944);
   h_genmu_Mu_Efficiency__1->SetBinContent(19,0.9696832);
   h_genmu_Mu_Efficiency__1->SetBinContent(20,0.9703985);
   h_genmu_Mu_Efficiency__1->SetBinContent(21,0.9698129);
   h_genmu_Mu_Efficiency__1->SetBinContent(22,0.9752505);
   h_genmu_Mu_Efficiency__1->SetBinContent(23,0.9740304);
   h_genmu_Mu_Efficiency__1->SetBinContent(24,0.9728026);
   h_genmu_Mu_Efficiency__1->SetBinContent(25,0.9705355);
   h_genmu_Mu_Efficiency__1->SetBinContent(26,0.9711785);
   h_genmu_Mu_Efficiency__1->SetBinContent(27,0.9678734);
   h_genmu_Mu_Efficiency__1->SetBinContent(28,0.9680879);
   h_genmu_Mu_Efficiency__1->SetBinContent(29,0.9685335);
   h_genmu_Mu_Efficiency__1->SetBinContent(30,0.9778816);
   h_genmu_Mu_Efficiency__1->SetBinContent(31,0.9736061);
   h_genmu_Mu_Efficiency__1->SetBinContent(32,0.9735371);
   h_genmu_Mu_Efficiency__1->SetBinContent(33,0.9730849);
   h_genmu_Mu_Efficiency__1->SetBinContent(34,0.9706943);
   h_genmu_Mu_Efficiency__1->SetBinContent(35,0.9688139);
   h_genmu_Mu_Efficiency__1->SetBinContent(36,0.968984);
   h_genmu_Mu_Efficiency__1->SetBinContent(37,0.9758162);
   h_genmu_Mu_Efficiency__1->SetBinContent(38,0.9754408);
   h_genmu_Mu_Efficiency__1->SetBinContent(39,0.9775753);
   h_genmu_Mu_Efficiency__1->SetBinContent(40,0.9635714);
   h_genmu_Mu_Efficiency__1->SetBinContent(41,0.9838022);
   h_genmu_Mu_Efficiency__1->SetBinContent(42,0.9716738);
   h_genmu_Mu_Efficiency__1->SetBinContent(43,0.9661509);
   h_genmu_Mu_Efficiency__1->SetBinContent(44,0.9811123);
   h_genmu_Mu_Efficiency__1->SetBinContent(45,0.9607362);
   h_genmu_Mu_Efficiency__1->SetBinContent(46,0.9863354);
   h_genmu_Mu_Efficiency__1->SetBinContent(47,0.984375);
   h_genmu_Mu_Efficiency__1->SetBinContent(48,0.9748521);
   h_genmu_Mu_Efficiency__1->SetBinContent(49,0.96);
   h_genmu_Mu_Efficiency__1->SetBinContent(50,0.9752066);
   h_genmu_Mu_Efficiency__1->SetBinContent(51,0.9723502);
   h_genmu_Mu_Efficiency__1->SetEntries(46.14378);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_genmu_Mu_Efficiency__1->SetFillColor(ci);
   h_genmu_Mu_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_genmu_Mu_Efficiency__1->SetLineColor(ci);
   h_genmu_Mu_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_genmu_Mu_Efficiency__1->SetMarkerColor(ci);
   h_genmu_Mu_Efficiency__1->SetMarkerStyle(33);
   h_genmu_Mu_Efficiency__1->SetMarkerSize(1.7);
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetTitle("Gen muon p_{T} [GeV]");
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_genmu_Mu_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_genmu_Mu_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_genmu_Mu_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_genmu_Mu_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_genmu_Mu_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_genmu_Mu_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_genmu_Mu_Efficiency__1->Draw("P");
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Gen muon |#eta| < 2.4");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
