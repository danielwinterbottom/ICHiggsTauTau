void h_jet_Tau_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 14:58:52 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_Tau_Efficiency__1 = new TH1D("h_jet_Tau_Efficiency__1","h_jet_Tau_Efficiency",20,0,100);
   h_jet_Tau_Efficiency__1->SetBinContent(4,0.2310784);
   h_jet_Tau_Efficiency__1->SetBinContent(5,0.3395356);
   h_jet_Tau_Efficiency__1->SetBinContent(6,0.4359237);
   h_jet_Tau_Efficiency__1->SetBinContent(7,0.505855);
   h_jet_Tau_Efficiency__1->SetBinContent(8,0.5502081);
   h_jet_Tau_Efficiency__1->SetBinContent(9,0.5836454);
   h_jet_Tau_Efficiency__1->SetBinContent(10,0.6123429);
   h_jet_Tau_Efficiency__1->SetBinContent(11,0.6354208);
   h_jet_Tau_Efficiency__1->SetBinContent(12,0.6572629);
   h_jet_Tau_Efficiency__1->SetBinContent(13,0.6739329);
   h_jet_Tau_Efficiency__1->SetBinContent(14,0.6928889);
   h_jet_Tau_Efficiency__1->SetBinContent(15,0.7126435);
   h_jet_Tau_Efficiency__1->SetBinContent(16,0.7269186);
   h_jet_Tau_Efficiency__1->SetBinContent(17,0.7416968);
   h_jet_Tau_Efficiency__1->SetBinContent(18,0.7560743);
   h_jet_Tau_Efficiency__1->SetBinContent(19,0.7708562);
   h_jet_Tau_Efficiency__1->SetBinContent(20,0.7842126);
   h_jet_Tau_Efficiency__1->SetBinContent(21,0.8688707);
   h_jet_Tau_Efficiency__1->SetEntries(10.4105);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jet_Tau_Efficiency__1->SetFillColor(ci);
   h_jet_Tau_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_jet_Tau_Efficiency__1->SetLineColor(ci);
   h_jet_Tau_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_jet_Tau_Efficiency__1->SetMarkerColor(ci);
   h_jet_Tau_Efficiency__1->SetMarkerStyle(33);
   h_jet_Tau_Efficiency__1->SetMarkerSize(1.7);
   h_jet_Tau_Efficiency__1->GetXaxis()->SetTitle("Offline jet p_{T} [GeV]");
   h_jet_Tau_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_Tau_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_jet_Tau_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_Tau_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_jet_Tau_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_jet_Tau_Efficiency__1->Draw("P");
   
   TH1D *h_jet_Tau_Efficiency__2 = new TH1D("h_jet_Tau_Efficiency__2","h_jet_Tau_Efficiency",20,0,100);
   h_jet_Tau_Efficiency__2->SetBinContent(4,0.0697834);
   h_jet_Tau_Efficiency__2->SetBinContent(5,0.1170619);
   h_jet_Tau_Efficiency__2->SetBinContent(6,0.2147567);
   h_jet_Tau_Efficiency__2->SetBinContent(7,0.3285442);
   h_jet_Tau_Efficiency__2->SetBinContent(8,0.4276855);
   h_jet_Tau_Efficiency__2->SetBinContent(9,0.5027601);
   h_jet_Tau_Efficiency__2->SetBinContent(10,0.5608886);
   h_jet_Tau_Efficiency__2->SetBinContent(11,0.6032166);
   h_jet_Tau_Efficiency__2->SetBinContent(12,0.637214);
   h_jet_Tau_Efficiency__2->SetBinContent(13,0.6616445);
   h_jet_Tau_Efficiency__2->SetBinContent(14,0.6847255);
   h_jet_Tau_Efficiency__2->SetBinContent(15,0.7075821);
   h_jet_Tau_Efficiency__2->SetBinContent(16,0.7237414);
   h_jet_Tau_Efficiency__2->SetBinContent(17,0.7398439);
   h_jet_Tau_Efficiency__2->SetBinContent(18,0.7547395);
   h_jet_Tau_Efficiency__2->SetBinContent(19,0.7698458);
   h_jet_Tau_Efficiency__2->SetBinContent(20,0.7835683);
   h_jet_Tau_Efficiency__2->SetBinContent(21,0.8685624);
   h_jet_Tau_Efficiency__2->SetEntries(9.287602);

   ci = TColor::GetColor("#99ccff");
   h_jet_Tau_Efficiency__2->SetFillColor(ci);
   h_jet_Tau_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_jet_Tau_Efficiency__2->SetLineColor(ci);
   h_jet_Tau_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_jet_Tau_Efficiency__2->SetMarkerColor(ci);
   h_jet_Tau_Efficiency__2->SetMarkerStyle(33);
   h_jet_Tau_Efficiency__2->SetMarkerSize(1.7);
   h_jet_Tau_Efficiency__2->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_Tau_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_jet_Tau_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_jet_Tau_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_jet_Tau_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_jet_Tau_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_jet_Tau_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_Tau_Efficiency__3 = new TH1D("h_jet_Tau_Efficiency__3","h_jet_Tau_Efficiency",20,0,100);
   h_jet_Tau_Efficiency__3->SetBinContent(4,0.02509157);
   h_jet_Tau_Efficiency__3->SetBinContent(5,0.01488172);
   h_jet_Tau_Efficiency__3->SetBinContent(6,0.01490216);
   h_jet_Tau_Efficiency__3->SetBinContent(7,0.02945725);
   h_jet_Tau_Efficiency__3->SetBinContent(8,0.06866196);
   h_jet_Tau_Efficiency__3->SetBinContent(9,0.1434801);
   h_jet_Tau_Efficiency__3->SetBinContent(10,0.2442346);
   h_jet_Tau_Efficiency__3->SetBinContent(11,0.3556979);
   h_jet_Tau_Efficiency__3->SetBinContent(12,0.452944);
   h_jet_Tau_Efficiency__3->SetBinContent(13,0.5303906);
   h_jet_Tau_Efficiency__3->SetBinContent(14,0.588395);
   h_jet_Tau_Efficiency__3->SetBinContent(15,0.639106);
   h_jet_Tau_Efficiency__3->SetBinContent(16,0.6756685);
   h_jet_Tau_Efficiency__3->SetBinContent(17,0.7048205);
   h_jet_Tau_Efficiency__3->SetBinContent(18,0.7302833);
   h_jet_Tau_Efficiency__3->SetBinContent(19,0.7522602);
   h_jet_Tau_Efficiency__3->SetBinContent(20,0.7706806);
   h_jet_Tau_Efficiency__3->SetBinContent(21,0.8658265);
   h_jet_Tau_Efficiency__3->SetEntries(6.740956);

   ci = TColor::GetColor("#99ccff");
   h_jet_Tau_Efficiency__3->SetFillColor(ci);
   h_jet_Tau_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_jet_Tau_Efficiency__3->SetLineColor(ci);
   h_jet_Tau_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_jet_Tau_Efficiency__3->SetMarkerColor(ci);
   h_jet_Tau_Efficiency__3->SetMarkerStyle(33);
   h_jet_Tau_Efficiency__3->SetMarkerSize(1.7);
   h_jet_Tau_Efficiency__3->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_Tau_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_jet_Tau_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_jet_Tau_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_jet_Tau_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_jet_Tau_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_Tau_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_Tau_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_jet_Tau_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_Tau_Efficiency","L1 Tau p_{T} #geq 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_Tau_Efficiency","L1 Tau p_{T} #geq 20 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_Tau_Efficiency","L1 Tau p_{T} #geq 30 GeV ","pl");
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
