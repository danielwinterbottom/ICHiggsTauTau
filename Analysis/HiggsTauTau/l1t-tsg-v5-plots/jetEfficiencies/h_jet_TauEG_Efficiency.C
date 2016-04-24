void h_jet_TauEG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:13:39 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_TauEG_Efficiency__1 = new TH1D("h_jet_TauEG_Efficiency__1","h_jet_TauEG_Efficiency",20,0,100);
   h_jet_TauEG_Efficiency__1->SetBinContent(4,0.02497301);
   h_jet_TauEG_Efficiency__1->SetBinContent(5,0.04225615);
   h_jet_TauEG_Efficiency__1->SetBinContent(6,0.06308581);
   h_jet_TauEG_Efficiency__1->SetBinContent(7,0.08294967);
   h_jet_TauEG_Efficiency__1->SetBinContent(8,0.09862836);
   h_jet_TauEG_Efficiency__1->SetBinContent(9,0.1119656);
   h_jet_TauEG_Efficiency__1->SetBinContent(10,0.1220479);
   h_jet_TauEG_Efficiency__1->SetBinContent(11,0.1287711);
   h_jet_TauEG_Efficiency__1->SetBinContent(12,0.1352782);
   h_jet_TauEG_Efficiency__1->SetBinContent(13,0.1368552);
   h_jet_TauEG_Efficiency__1->SetBinContent(14,0.138991);
   h_jet_TauEG_Efficiency__1->SetBinContent(15,0.1389992);
   h_jet_TauEG_Efficiency__1->SetBinContent(16,0.1384445);
   h_jet_TauEG_Efficiency__1->SetBinContent(17,0.1399955);
   h_jet_TauEG_Efficiency__1->SetBinContent(18,0.1362647);
   h_jet_TauEG_Efficiency__1->SetBinContent(19,0.1352952);
   h_jet_TauEG_Efficiency__1->SetBinContent(20,0.1341522);
   h_jet_TauEG_Efficiency__1->SetBinContent(21,0.1224);
   h_jet_TauEG_Efficiency__1->SetEntries(1.908953);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jet_TauEG_Efficiency__1->SetFillColor(ci);
   h_jet_TauEG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_jet_TauEG_Efficiency__1->SetLineColor(ci);
   h_jet_TauEG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_jet_TauEG_Efficiency__1->SetMarkerColor(ci);
   h_jet_TauEG_Efficiency__1->SetMarkerStyle(33);
   h_jet_TauEG_Efficiency__1->SetMarkerSize(1.7);
   h_jet_TauEG_Efficiency__1->GetXaxis()->SetTitle("Offline jet p_{T} [GeV]");
   h_jet_TauEG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_TauEG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_jet_TauEG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_TauEG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_jet_TauEG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_jet_TauEG_Efficiency__1->Draw("P");
   
   TH1D *h_jet_TauEG_Efficiency__2 = new TH1D("h_jet_TauEG_Efficiency__2","h_jet_TauEG_Efficiency",20,0,100);
   h_jet_TauEG_Efficiency__2->SetBinContent(4,0.009509962);
   h_jet_TauEG_Efficiency__2->SetBinContent(5,0.01629588);
   h_jet_TauEG_Efficiency__2->SetBinContent(6,0.03004678);
   h_jet_TauEG_Efficiency__2->SetBinContent(7,0.04741943);
   h_jet_TauEG_Efficiency__2->SetBinContent(8,0.06430971);
   h_jet_TauEG_Efficiency__2->SetBinContent(9,0.08019852);
   h_jet_TauEG_Efficiency__2->SetBinContent(10,0.09565429);
   h_jet_TauEG_Efficiency__2->SetBinContent(11,0.1069689);
   h_jet_TauEG_Efficiency__2->SetBinContent(12,0.1165693);
   h_jet_TauEG_Efficiency__2->SetBinContent(13,0.1227027);
   h_jet_TauEG_Efficiency__2->SetBinContent(14,0.1268168);
   h_jet_TauEG_Efficiency__2->SetBinContent(15,0.1297986);
   h_jet_TauEG_Efficiency__2->SetBinContent(16,0.1303504);
   h_jet_TauEG_Efficiency__2->SetBinContent(17,0.1330189);
   h_jet_TauEG_Efficiency__2->SetBinContent(18,0.1298447);
   h_jet_TauEG_Efficiency__2->SetBinContent(19,0.129321);
   h_jet_TauEG_Efficiency__2->SetBinContent(20,0.1285944);
   h_jet_TauEG_Efficiency__2->SetBinContent(21,0.1180728);
   h_jet_TauEG_Efficiency__2->SetEntries(1.59742);

   ci = TColor::GetColor("#99ccff");
   h_jet_TauEG_Efficiency__2->SetFillColor(ci);
   h_jet_TauEG_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_jet_TauEG_Efficiency__2->SetLineColor(ci);
   h_jet_TauEG_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_jet_TauEG_Efficiency__2->SetMarkerColor(ci);
   h_jet_TauEG_Efficiency__2->SetMarkerStyle(33);
   h_jet_TauEG_Efficiency__2->SetMarkerSize(1.7);
   h_jet_TauEG_Efficiency__2->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
   h_jet_TauEG_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_jet_TauEG_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_jet_TauEG_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_jet_TauEG_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_jet_TauEG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_jet_TauEG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_TauEG_Efficiency__3 = new TH1D("h_jet_TauEG_Efficiency__3","h_jet_TauEG_Efficiency",20,0,100);
   h_jet_TauEG_Efficiency__3->SetBinContent(4,0.003825201);
   h_jet_TauEG_Efficiency__3->SetBinContent(5,0.002527426);
   h_jet_TauEG_Efficiency__3->SetBinContent(6,0.003630923);
   h_jet_TauEG_Efficiency__3->SetBinContent(7,0.008288398);
   h_jet_TauEG_Efficiency__3->SetBinContent(8,0.01769257);
   h_jet_TauEG_Efficiency__3->SetBinContent(9,0.03138485);
   h_jet_TauEG_Efficiency__3->SetBinContent(10,0.04940646);
   h_jet_TauEG_Efficiency__3->SetBinContent(11,0.06730566);
   h_jet_TauEG_Efficiency__3->SetBinContent(12,0.08266134);
   h_jet_TauEG_Efficiency__3->SetBinContent(13,0.09521224);
   h_jet_TauEG_Efficiency__3->SetBinContent(14,0.1047978);
   h_jet_TauEG_Efficiency__3->SetBinContent(15,0.1110149);
   h_jet_TauEG_Efficiency__3->SetBinContent(16,0.114969);
   h_jet_TauEG_Efficiency__3->SetBinContent(17,0.1200483);
   h_jet_TauEG_Efficiency__3->SetBinContent(18,0.1194521);
   h_jet_TauEG_Efficiency__3->SetBinContent(19,0.1201205);
   h_jet_TauEG_Efficiency__3->SetBinContent(20,0.1209021);
   h_jet_TauEG_Efficiency__3->SetBinContent(21,0.1138929);
   h_jet_TauEG_Efficiency__3->SetEntries(1.17324);

   ci = TColor::GetColor("#99ccff");
   h_jet_TauEG_Efficiency__3->SetFillColor(ci);
   h_jet_TauEG_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_jet_TauEG_Efficiency__3->SetLineColor(ci);
   h_jet_TauEG_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_jet_TauEG_Efficiency__3->SetMarkerColor(ci);
   h_jet_TauEG_Efficiency__3->SetMarkerStyle(33);
   h_jet_TauEG_Efficiency__3->SetMarkerSize(1.7);
   h_jet_TauEG_Efficiency__3->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
   h_jet_TauEG_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_jet_TauEG_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_jet_TauEG_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_jet_TauEG_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_jet_TauEG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_TauEG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_TauEG_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_jet_TauEG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.02);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_TauEG_Efficiency","L1 EG, Tau p_{T} #geq 10, 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_TauEG_Efficiency","L1 EG, Tau p_{T} #geq 15, 20 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_TauEG_Efficiency","L1 EG, Tau p_{T} #geq 20, 30 GeV ","pl");
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
