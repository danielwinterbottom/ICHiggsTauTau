void h_jet_EG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:10:11 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_EG_Efficiency__1 = new TH1D("h_jet_EG_Efficiency__1","h_jet_EG_Efficiency",20,0,100);
   h_jet_EG_Efficiency__1->SetBinContent(4,0.02517894);
   h_jet_EG_Efficiency__1->SetBinContent(5,0.04237048);
   h_jet_EG_Efficiency__1->SetBinContent(6,0.06315677);
   h_jet_EG_Efficiency__1->SetBinContent(7,0.08305715);
   h_jet_EG_Efficiency__1->SetBinContent(8,0.09871467);
   h_jet_EG_Efficiency__1->SetBinContent(9,0.1120758);
   h_jet_EG_Efficiency__1->SetBinContent(10,0.1221451);
   h_jet_EG_Efficiency__1->SetBinContent(11,0.1288619);
   h_jet_EG_Efficiency__1->SetBinContent(12,0.1353522);
   h_jet_EG_Efficiency__1->SetBinContent(13,0.1368914);
   h_jet_EG_Efficiency__1->SetBinContent(14,0.139072);
   h_jet_EG_Efficiency__1->SetBinContent(15,0.1391566);
   h_jet_EG_Efficiency__1->SetBinContent(16,0.1385453);
   h_jet_EG_Efficiency__1->SetBinContent(17,0.1400938);
   h_jet_EG_Efficiency__1->SetBinContent(18,0.1364077);
   h_jet_EG_Efficiency__1->SetBinContent(19,0.1353661);
   h_jet_EG_Efficiency__1->SetBinContent(20,0.1342126);
   h_jet_EG_Efficiency__1->SetBinContent(21,0.1224726);
   h_jet_EG_Efficiency__1->SetEntries(1.910659);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jet_EG_Efficiency__1->SetFillColor(ci);
   h_jet_EG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_jet_EG_Efficiency__1->SetLineColor(ci);
   h_jet_EG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_jet_EG_Efficiency__1->SetMarkerColor(ci);
   h_jet_EG_Efficiency__1->SetMarkerStyle(33);
   h_jet_EG_Efficiency__1->SetMarkerSize(1.7);
   h_jet_EG_Efficiency__1->GetXaxis()->SetTitle("Offline jet p_{T} [GeV]");
   h_jet_EG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_EG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_jet_EG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_EG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_jet_EG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_jet_EG_Efficiency__1->Draw("P");
   
   TH1D *h_jet_EG_Efficiency__2 = new TH1D("h_jet_EG_Efficiency__2","h_jet_EG_Efficiency",20,0,100);
   h_jet_EG_Efficiency__2->SetBinContent(4,0.009722127);
   h_jet_EG_Efficiency__2->SetBinContent(5,0.0164042);
   h_jet_EG_Efficiency__2->SetBinContent(6,0.03012957);
   h_jet_EG_Efficiency__2->SetBinContent(7,0.04752094);
   h_jet_EG_Efficiency__2->SetBinContent(8,0.06439602);
   h_jet_EG_Efficiency__2->SetBinContent(9,0.08025683);
   h_jet_EG_Efficiency__2->SetBinContent(10,0.09569594);
   h_jet_EG_Efficiency__2->SetBinContent(11,0.1070369);
   h_jet_EG_Efficiency__2->SetBinContent(12,0.1166268);
   h_jet_EG_Efficiency__2->SetBinContent(13,0.1227389);
   h_jet_EG_Efficiency__2->SetBinContent(14,0.1268978);
   h_jet_EG_Efficiency__2->SetBinContent(15,0.129956);
   h_jet_EG_Efficiency__2->SetBinContent(16,0.1304512);
   h_jet_EG_Efficiency__2->SetBinContent(17,0.1331172);
   h_jet_EG_Efficiency__2->SetBinContent(18,0.1299878);
   h_jet_EG_Efficiency__2->SetBinContent(19,0.129392);
   h_jet_EG_Efficiency__2->SetBinContent(20,0.1286549);
   h_jet_EG_Efficiency__2->SetBinContent(21,0.1181521);
   h_jet_EG_Efficiency__2->SetEntries(1.598985);

   ci = TColor::GetColor("#99ccff");
   h_jet_EG_Efficiency__2->SetFillColor(ci);
   h_jet_EG_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_jet_EG_Efficiency__2->SetLineColor(ci);
   h_jet_EG_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_jet_EG_Efficiency__2->SetMarkerColor(ci);
   h_jet_EG_Efficiency__2->SetMarkerStyle(33);
   h_jet_EG_Efficiency__2->SetMarkerSize(1.7);
   h_jet_EG_Efficiency__2->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_EG_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_jet_EG_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_jet_EG_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_jet_EG_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_jet_EG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_jet_EG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_EG_Efficiency__3 = new TH1D("h_jet_EG_Efficiency__3","h_jet_EG_Efficiency",20,0,100);
   h_jet_EG_Efficiency__3->SetBinContent(4,0.004867304);
   h_jet_EG_Efficiency__3->SetBinContent(5,0.005373788);
   h_jet_EG_Efficiency__3->SetBinContent(6,0.01132446);
   h_jet_EG_Efficiency__3->SetBinContent(7,0.02217803);
   h_jet_EG_Efficiency__3->SetBinContent(8,0.0360201);
   h_jet_EG_Efficiency__3->SetBinContent(9,0.04996696);
   h_jet_EG_Efficiency__3->SetBinContent(10,0.06635196);
   h_jet_EG_Efficiency__3->SetBinContent(11,0.08018126);
   h_jet_EG_Efficiency__3->SetBinContent(12,0.09248436);
   h_jet_EG_Efficiency__3->SetBinContent(13,0.102017);
   h_jet_EG_Efficiency__3->SetBinContent(14,0.1098113);
   h_jet_EG_Efficiency__3->SetBinContent(15,0.1148953);
   h_jet_EG_Efficiency__3->SetBinContent(16,0.1177679);
   h_jet_EG_Efficiency__3->SetBinContent(17,0.1222943);
   h_jet_EG_Efficiency__3->SetBinContent(18,0.1211047);
   h_jet_EG_Efficiency__3->SetBinContent(19,0.1211487);
   h_jet_EG_Efficiency__3->SetBinContent(20,0.1217076);
   h_jet_EG_Efficiency__3->SetBinContent(21,0.1141287);
   h_jet_EG_Efficiency__3->SetEntries(1.299495);

   ci = TColor::GetColor("#99ccff");
   h_jet_EG_Efficiency__3->SetFillColor(ci);
   h_jet_EG_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_jet_EG_Efficiency__3->SetLineColor(ci);
   h_jet_EG_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_jet_EG_Efficiency__3->SetMarkerColor(ci);
   h_jet_EG_Efficiency__3->SetMarkerStyle(33);
   h_jet_EG_Efficiency__3->SetMarkerSize(1.7);
   h_jet_EG_Efficiency__3->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_EG_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_jet_EG_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_jet_EG_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_jet_EG_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_jet_EG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_jet_EG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_EG_Efficiency","L1 EG p_{T} #geq 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_EG_Efficiency","L1 EG p_{T} #geq 15 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_EG_Efficiency","L1 EG p_{T} #geq 20 GeV ","pl");
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
      tex = new TLatex(0.15,0.92,"Offline jet |#eta| < 4.7");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
