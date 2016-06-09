void h_jet_IsoEG_Efficiency_v58pt1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 16:00:08 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_IsoEG_Efficiency__1 = new TH1D("h_jet_IsoEG_Efficiency__1","h_jet_IsoEG_Efficiency",20,0,100);
   h_jet_IsoEG_Efficiency__1->SetBinContent(4,0.008689158);
   h_jet_IsoEG_Efficiency__1->SetBinContent(5,0.01441096);
   h_jet_IsoEG_Efficiency__1->SetBinContent(6,0.01786432);
   h_jet_IsoEG_Efficiency__1->SetBinContent(7,0.02032823);
   h_jet_IsoEG_Efficiency__1->SetBinContent(8,0.02000087);
   h_jet_IsoEG_Efficiency__1->SetBinContent(9,0.0186408);
   h_jet_IsoEG_Efficiency__1->SetBinContent(10,0.01905305);
   h_jet_IsoEG_Efficiency__1->SetBinContent(11,0.01779553);
   h_jet_IsoEG_Efficiency__1->SetBinContent(12,0.01822923);
   h_jet_IsoEG_Efficiency__1->SetBinContent(13,0.01859108);
   h_jet_IsoEG_Efficiency__1->SetBinContent(14,0.01913051);
   h_jet_IsoEG_Efficiency__1->SetBinContent(15,0.02051007);
   h_jet_IsoEG_Efficiency__1->SetBinContent(16,0.02303906);
   h_jet_IsoEG_Efficiency__1->SetBinContent(17,0.02659415);
   h_jet_IsoEG_Efficiency__1->SetBinContent(18,0.03087982);
   h_jet_IsoEG_Efficiency__1->SetBinContent(19,0.03709092);
   h_jet_IsoEG_Efficiency__1->SetBinContent(20,0.04348093);
   h_jet_IsoEG_Efficiency__1->SetBinContent(21,0.0742982);
   h_jet_IsoEG_Efficiency__1->SetEntries(0.3743287);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jet_IsoEG_Efficiency__1->SetFillColor(ci);
   h_jet_IsoEG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_jet_IsoEG_Efficiency__1->SetLineColor(ci);
   h_jet_IsoEG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_jet_IsoEG_Efficiency__1->SetMarkerColor(ci);
   h_jet_IsoEG_Efficiency__1->SetMarkerStyle(33);
   h_jet_IsoEG_Efficiency__1->SetMarkerSize(1.7);
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetTitle("offline jet p_{T} [GeV]");
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoEG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__1->Draw("P");
   
   TH1D *h_jet_IsoEG_Efficiency__2 = new TH1D("h_jet_IsoEG_Efficiency__2","h_jet_IsoEG_Efficiency",20,0,100);
   h_jet_IsoEG_Efficiency__2->SetBinContent(4,0.003341497);
   h_jet_IsoEG_Efficiency__2->SetBinContent(5,0.006778399);
   h_jet_IsoEG_Efficiency__2->SetBinContent(6,0.01072819);
   h_jet_IsoEG_Efficiency__2->SetBinContent(7,0.01454697);
   h_jet_IsoEG_Efficiency__2->SetBinContent(8,0.01637685);
   h_jet_IsoEG_Efficiency__2->SetBinContent(9,0.01636081);
   h_jet_IsoEG_Efficiency__2->SetBinContent(10,0.01775094);
   h_jet_IsoEG_Efficiency__2->SetBinContent(11,0.01706715);
   h_jet_IsoEG_Efficiency__2->SetBinContent(12,0.01786265);
   h_jet_IsoEG_Efficiency__2->SetBinContent(13,0.01825156);
   h_jet_IsoEG_Efficiency__2->SetBinContent(14,0.01891498);
   h_jet_IsoEG_Efficiency__2->SetBinContent(15,0.02039606);
   h_jet_IsoEG_Efficiency__2->SetBinContent(16,0.02296235);
   h_jet_IsoEG_Efficiency__2->SetBinContent(17,0.02645179);
   h_jet_IsoEG_Efficiency__2->SetBinContent(18,0.03079923);
   h_jet_IsoEG_Efficiency__2->SetBinContent(19,0.03692927);
   h_jet_IsoEG_Efficiency__2->SetBinContent(20,0.04339919);
   h_jet_IsoEG_Efficiency__2->SetBinContent(21,0.07423155);
   h_jet_IsoEG_Efficiency__2->SetEntries(0.3389179);

   ci = TColor::GetColor("#99ccff");
   h_jet_IsoEG_Efficiency__2->SetFillColor(ci);
   h_jet_IsoEG_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_jet_IsoEG_Efficiency__2->SetLineColor(ci);
   h_jet_IsoEG_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_jet_IsoEG_Efficiency__2->SetMarkerColor(ci);
   h_jet_IsoEG_Efficiency__2->SetMarkerStyle(33);
   h_jet_IsoEG_Efficiency__2->SetMarkerSize(1.7);
   h_jet_IsoEG_Efficiency__2->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_IsoEG_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_jet_IsoEG_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoEG_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoEG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_IsoEG_Efficiency__3 = new TH1D("h_jet_IsoEG_Efficiency__3","h_jet_IsoEG_Efficiency",20,0,100);
   h_jet_IsoEG_Efficiency__3->SetBinContent(4,0.001639106);
   h_jet_IsoEG_Efficiency__3->SetBinContent(5,0.002178118);
   h_jet_IsoEG_Efficiency__3->SetBinContent(6,0.004767416);
   h_jet_IsoEG_Efficiency__3->SetBinContent(7,0.008923112);
   h_jet_IsoEG_Efficiency__3->SetBinContent(8,0.0120905);
   h_jet_IsoEG_Efficiency__3->SetBinContent(9,0.01338432);
   h_jet_IsoEG_Efficiency__3->SetBinContent(10,0.0157661);
   h_jet_IsoEG_Efficiency__3->SetBinContent(11,0.01572539);
   h_jet_IsoEG_Efficiency__3->SetBinContent(12,0.01699618);
   h_jet_IsoEG_Efficiency__3->SetBinContent(13,0.01764593);
   h_jet_IsoEG_Efficiency__3->SetBinContent(14,0.01855577);
   h_jet_IsoEG_Efficiency__3->SetBinContent(15,0.02013385);
   h_jet_IsoEG_Efficiency__3->SetBinContent(16,0.02283449);
   h_jet_IsoEG_Efficiency__3->SetBinContent(17,0.02632366);
   h_jet_IsoEG_Efficiency__3->SetBinContent(18,0.03060583);
   h_jet_IsoEG_Efficiency__3->SetBinContent(19,0.0368215);
   h_jet_IsoEG_Efficiency__3->SetBinContent(20,0.04335833);
   h_jet_IsoEG_Efficiency__3->SetBinContent(21,0.07417869);
   h_jet_IsoEG_Efficiency__3->SetEntries(0.3077496);

   ci = TColor::GetColor("#99ccff");
   h_jet_IsoEG_Efficiency__3->SetFillColor(ci);
   h_jet_IsoEG_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_jet_IsoEG_Efficiency__3->SetLineColor(ci);
   h_jet_IsoEG_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_jet_IsoEG_Efficiency__3->SetMarkerColor(ci);
   h_jet_IsoEG_Efficiency__3->SetMarkerStyle(33);
   h_jet_IsoEG_Efficiency__3->SetMarkerSize(1.7);
   h_jet_IsoEG_Efficiency__3->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_IsoEG_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_jet_IsoEG_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoEG_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoEG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_IsoEG_Efficiency","L1 IsoEG #geq 10 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoEG_Efficiency","L1 IsoEG #geq 15 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoEG_Efficiency","L1 IsoEG #geq 20 GeV","pl");
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
      tex = new TLatex(0.25,0.92,"offline jet |#eta| < 4.7");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
