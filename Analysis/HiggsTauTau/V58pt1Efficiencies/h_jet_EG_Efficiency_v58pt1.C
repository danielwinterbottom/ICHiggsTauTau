void h_jet_EG_Efficiency_v58pt1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 15:58:15 2016) by ROOT version6.06/01
   TCanvas *c1 = new TCanvas("c1", "c1",904,191,850,850);
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
   h_jet_EG_Efficiency__1->SetBinContent(4,0.02518147);
   h_jet_EG_Efficiency__1->SetBinContent(5,0.04245194);
   h_jet_EG_Efficiency__1->SetBinContent(6,0.06306182);
   h_jet_EG_Efficiency__1->SetBinContent(7,0.08305638);
   h_jet_EG_Efficiency__1->SetBinContent(8,0.09876721);
   h_jet_EG_Efficiency__1->SetBinContent(9,0.1123573);
   h_jet_EG_Efficiency__1->SetBinContent(10,0.1220746);
   h_jet_EG_Efficiency__1->SetBinContent(11,0.128824);
   h_jet_EG_Efficiency__1->SetBinContent(12,0.1352029);
   h_jet_EG_Efficiency__1->SetBinContent(13,0.1369004);
   h_jet_EG_Efficiency__1->SetBinContent(14,0.1389835);
   h_jet_EG_Efficiency__1->SetBinContent(15,0.1392496);
   h_jet_EG_Efficiency__1->SetBinContent(16,0.1390015);
   h_jet_EG_Efficiency__1->SetBinContent(17,0.1400322);
   h_jet_EG_Efficiency__1->SetBinContent(18,0.1364772);
   h_jet_EG_Efficiency__1->SetBinContent(19,0.1354672);
   h_jet_EG_Efficiency__1->SetBinContent(20,0.134182);
   h_jet_EG_Efficiency__1->SetBinContent(21,0.1225103);
   h_jet_EG_Efficiency__1->SetEntries(1.911271);

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
   h_jet_EG_Efficiency__1->GetXaxis()->SetTitle("offline jet p_{T} [GeV]");
   h_jet_EG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_EG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_jet_EG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_EG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__1->Draw("P");
   
   TH1D *h_jet_EG_Efficiency__2 = new TH1D("h_jet_EG_Efficiency__2","h_jet_EG_Efficiency",20,0,100);
   h_jet_EG_Efficiency__2->SetBinContent(4,0.009695405);
   h_jet_EG_Efficiency__2->SetBinContent(5,0.01643655);
   h_jet_EG_Efficiency__2->SetBinContent(6,0.03024161);
   h_jet_EG_Efficiency__2->SetBinContent(7,0.04747895);
   h_jet_EG_Efficiency__2->SetBinContent(8,0.06448892);
   h_jet_EG_Efficiency__2->SetBinContent(9,0.08038477);
   h_jet_EG_Efficiency__2->SetBinContent(10,0.09569459);
   h_jet_EG_Efficiency__2->SetBinContent(11,0.1070722);
   h_jet_EG_Efficiency__2->SetBinContent(12,0.1165988);
   h_jet_EG_Efficiency__2->SetBinContent(13,0.1227415);
   h_jet_EG_Efficiency__2->SetBinContent(14,0.126873);
   h_jet_EG_Efficiency__2->SetBinContent(15,0.1299807);
   h_jet_EG_Efficiency__2->SetBinContent(16,0.1308317);
   h_jet_EG_Efficiency__2->SetBinContent(17,0.133042);
   h_jet_EG_Efficiency__2->SetBinContent(18,0.1300305);
   h_jet_EG_Efficiency__2->SetBinContent(19,0.1294859);
   h_jet_EG_Efficiency__2->SetBinContent(20,0.1286651);
   h_jet_EG_Efficiency__2->SetBinContent(21,0.1181757);
   h_jet_EG_Efficiency__2->SetEntries(1.599742);

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
   h_jet_EG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_EG_Efficiency__3 = new TH1D("h_jet_EG_Efficiency__3","h_jet_EG_Efficiency",20,0,100);
   h_jet_EG_Efficiency__3->SetBinContent(4,0.004822388);
   h_jet_EG_Efficiency__3->SetBinContent(5,0.00537513);
   h_jet_EG_Efficiency__3->SetBinContent(6,0.01136384);
   h_jet_EG_Efficiency__3->SetBinContent(7,0.02216249);
   h_jet_EG_Efficiency__3->SetBinContent(8,0.03619027);
   h_jet_EG_Efficiency__3->SetBinContent(9,0.05002201);
   h_jet_EG_Efficiency__3->SetBinContent(10,0.066288);
   h_jet_EG_Efficiency__3->SetBinContent(11,0.08017573);
   h_jet_EG_Efficiency__3->SetBinContent(12,0.09250419);
   h_jet_EG_Efficiency__3->SetBinContent(13,0.1019756);
   h_jet_EG_Efficiency__3->SetBinContent(14,0.1097438);
   h_jet_EG_Efficiency__3->SetBinContent(15,0.1149773);
   h_jet_EG_Efficiency__3->SetBinContent(16,0.1180336);
   h_jet_EG_Efficiency__3->SetBinContent(17,0.1222505);
   h_jet_EG_Efficiency__3->SetBinContent(18,0.1211017);
   h_jet_EG_Efficiency__3->SetBinContent(19,0.1213852);
   h_jet_EG_Efficiency__3->SetBinContent(20,0.121861);
   h_jet_EG_Efficiency__3->SetBinContent(21,0.1141467);
   h_jet_EG_Efficiency__3->SetEntries(1.300233);

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
   h_jet_EG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_EG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_EG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_EG_Efficiency","L1 EG #geq 10 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_EG_Efficiency","L1 EG #geq 15 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_EG_Efficiency","L1 EG #geq 20 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   leg->Draw();
   TLatex *   tex = new TLatex(0.65,0.92,"l1t-integration-v58.1");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.15,0.92,"offline jet |#eta| < 4.7");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
