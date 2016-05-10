void h_tau_IsoEG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:44:04 2016) by ROOT version6.06/01
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
   
   TH1D *h_tau_IsoEG_Efficiency__1 = new TH1D("h_tau_IsoEG_Efficiency__1","h_tau_IsoEG_Efficiency",20,0,100);
   h_tau_IsoEG_Efficiency__1->SetBinContent(4,0.09474189);
   h_tau_IsoEG_Efficiency__1->SetBinContent(5,0.1064816);
   h_tau_IsoEG_Efficiency__1->SetBinContent(6,0.1101077);
   h_tau_IsoEG_Efficiency__1->SetBinContent(7,0.1053705);
   h_tau_IsoEG_Efficiency__1->SetBinContent(8,0.1014728);
   h_tau_IsoEG_Efficiency__1->SetBinContent(9,0.105944);
   h_tau_IsoEG_Efficiency__1->SetBinContent(10,0.1107632);
   h_tau_IsoEG_Efficiency__1->SetBinContent(11,0.1170079);
   h_tau_IsoEG_Efficiency__1->SetBinContent(12,0.1282873);
   h_tau_IsoEG_Efficiency__1->SetBinContent(13,0.1452257);
   h_tau_IsoEG_Efficiency__1->SetBinContent(14,0.1597902);
   h_tau_IsoEG_Efficiency__1->SetBinContent(15,0.1717294);
   h_tau_IsoEG_Efficiency__1->SetBinContent(16,0.1799457);
   h_tau_IsoEG_Efficiency__1->SetBinContent(17,0.1843305);
   h_tau_IsoEG_Efficiency__1->SetBinContent(18,0.1838652);
   h_tau_IsoEG_Efficiency__1->SetBinContent(19,0.1835294);
   h_tau_IsoEG_Efficiency__1->SetBinContent(20,0.1830761);
   h_tau_IsoEG_Efficiency__1->SetBinContent(21,0.1690365);
   h_tau_IsoEG_Efficiency__1->SetEntries(2.371669);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_tau_IsoEG_Efficiency__1->SetFillColor(ci);
   h_tau_IsoEG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_tau_IsoEG_Efficiency__1->SetLineColor(ci);
   h_tau_IsoEG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_tau_IsoEG_Efficiency__1->SetMarkerColor(ci);
   h_tau_IsoEG_Efficiency__1->SetMarkerStyle(33);
   h_tau_IsoEG_Efficiency__1->SetMarkerSize(1.7);
   h_tau_IsoEG_Efficiency__1->GetXaxis()->SetTitle("Offline tau p_{T} [GeV]");
   h_tau_IsoEG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_tau_IsoEG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_tau_IsoEG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_tau_IsoEG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_tau_IsoEG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_tau_IsoEG_Efficiency__1->Draw("P");
   
   TH1D *h_tau_IsoEG_Efficiency__2 = new TH1D("h_tau_IsoEG_Efficiency__2","h_tau_IsoEG_Efficiency",20,0,100);
   h_tau_IsoEG_Efficiency__2->SetBinContent(4,0.05462109);
   h_tau_IsoEG_Efficiency__2->SetBinContent(5,0.08205718);
   h_tau_IsoEG_Efficiency__2->SetBinContent(6,0.1010136);
   h_tau_IsoEG_Efficiency__2->SetBinContent(7,0.1025417);
   h_tau_IsoEG_Efficiency__2->SetBinContent(8,0.1004948);
   h_tau_IsoEG_Efficiency__2->SetBinContent(9,0.1055911);
   h_tau_IsoEG_Efficiency__2->SetBinContent(10,0.1104797);
   h_tau_IsoEG_Efficiency__2->SetBinContent(11,0.1168219);
   h_tau_IsoEG_Efficiency__2->SetBinContent(12,0.1281569);
   h_tau_IsoEG_Efficiency__2->SetBinContent(13,0.1449579);
   h_tau_IsoEG_Efficiency__2->SetBinContent(14,0.1594974);
   h_tau_IsoEG_Efficiency__2->SetBinContent(15,0.1716022);
   h_tau_IsoEG_Efficiency__2->SetBinContent(16,0.1798681);
   h_tau_IsoEG_Efficiency__2->SetBinContent(17,0.1841463);
   h_tau_IsoEG_Efficiency__2->SetBinContent(18,0.1836984);
   h_tau_IsoEG_Efficiency__2->SetBinContent(19,0.1834641);
   h_tau_IsoEG_Efficiency__2->SetBinContent(20,0.1829161);
   h_tau_IsoEG_Efficiency__2->SetBinContent(21,0.1688641);
   h_tau_IsoEG_Efficiency__2->SetEntries(2.291928);

   ci = TColor::GetColor("#99ccff");
   h_tau_IsoEG_Efficiency__2->SetFillColor(ci);
   h_tau_IsoEG_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_tau_IsoEG_Efficiency__2->SetLineColor(ci);
   h_tau_IsoEG_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_tau_IsoEG_Efficiency__2->SetMarkerColor(ci);
   h_tau_IsoEG_Efficiency__2->SetMarkerStyle(33);
   h_tau_IsoEG_Efficiency__2->SetMarkerSize(1.7);
   h_tau_IsoEG_Efficiency__2->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
   h_tau_IsoEG_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_tau_IsoEG_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_tau_IsoEG_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_tau_IsoEG_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_tau_IsoEG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_tau_IsoEG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_tau_IsoEG_Efficiency__3 = new TH1D("h_tau_IsoEG_Efficiency__3","h_tau_IsoEG_Efficiency",20,0,100);
   h_tau_IsoEG_Efficiency__3->SetBinContent(4,0.01944039);
   h_tau_IsoEG_Efficiency__3->SetBinContent(5,0.04173598);
   h_tau_IsoEG_Efficiency__3->SetBinContent(6,0.08031513);
   h_tau_IsoEG_Efficiency__3->SetBinContent(7,0.09549092);
   h_tau_IsoEG_Efficiency__3->SetBinContent(8,0.09795386);
   h_tau_IsoEG_Efficiency__3->SetBinContent(9,0.1045104);
   h_tau_IsoEG_Efficiency__3->SetBinContent(10,0.1100416);
   h_tau_IsoEG_Efficiency__3->SetBinContent(11,0.1165119);
   h_tau_IsoEG_Efficiency__3->SetBinContent(12,0.128045);
   h_tau_IsoEG_Efficiency__3->SetBinContent(13,0.1448017);
   h_tau_IsoEG_Efficiency__3->SetBinContent(14,0.1594442);
   h_tau_IsoEG_Efficiency__3->SetBinContent(15,0.1714431);
   h_tau_IsoEG_Efficiency__3->SetBinContent(16,0.1797905);
   h_tau_IsoEG_Efficiency__3->SetBinContent(17,0.1841002);
   h_tau_IsoEG_Efficiency__3->SetBinContent(18,0.1836984);
   h_tau_IsoEG_Efficiency__3->SetBinContent(19,0.1833987);
   h_tau_IsoEG_Efficiency__3->SetBinContent(20,0.1828361);
   h_tau_IsoEG_Efficiency__3->SetBinContent(21,0.1688375);
   h_tau_IsoEG_Efficiency__3->SetEntries(2.183558);

   ci = TColor::GetColor("#99ccff");
   h_tau_IsoEG_Efficiency__3->SetFillColor(ci);
   h_tau_IsoEG_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_tau_IsoEG_Efficiency__3->SetLineColor(ci);
   h_tau_IsoEG_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_tau_IsoEG_Efficiency__3->SetMarkerColor(ci);
   h_tau_IsoEG_Efficiency__3->SetMarkerStyle(33);
   h_tau_IsoEG_Efficiency__3->SetMarkerSize(1.7);
   h_tau_IsoEG_Efficiency__3->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
   h_tau_IsoEG_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_tau_IsoEG_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_tau_IsoEG_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_tau_IsoEG_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_tau_IsoEG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_tau_IsoEG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_tau_IsoEG_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_tau_IsoEG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_tau_IsoEG_Efficiency","L1 IsoEG p_{T} #geq 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_tau_IsoEG_Efficiency","L1 IsoEG p_{T} #geq 15 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_tau_IsoEG_Efficiency","L1 IsoEG p_{T} #geq 20 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   leg->Draw();
   TLatex *   tex = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"Offline tau |#eta| < 2.3");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
