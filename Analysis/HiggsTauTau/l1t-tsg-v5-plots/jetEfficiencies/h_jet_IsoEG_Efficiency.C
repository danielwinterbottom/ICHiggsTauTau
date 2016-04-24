void h_jet_IsoEG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:05:28 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_IsoEG_Efficiency__1 = new TH1D("h_jet_IsoEG_Efficiency__1","h_jet_IsoEG_Efficiency",20,0,100);
   h_jet_IsoEG_Efficiency__1->SetBinContent(4,0.00845538);
   h_jet_IsoEG_Efficiency__1->SetBinContent(5,0.01375642);
   h_jet_IsoEG_Efficiency__1->SetBinContent(6,0.01720845);
   h_jet_IsoEG_Efficiency__1->SetBinContent(7,0.01924007);
   h_jet_IsoEG_Efficiency__1->SetBinContent(8,0.01883303);
   h_jet_IsoEG_Efficiency__1->SetBinContent(9,0.0175908);
   h_jet_IsoEG_Efficiency__1->SetBinContent(10,0.01795904);
   h_jet_IsoEG_Efficiency__1->SetBinContent(11,0.01650679);
   h_jet_IsoEG_Efficiency__1->SetBinContent(12,0.01684299);
   h_jet_IsoEG_Efficiency__1->SetBinContent(13,0.01734669);
   h_jet_IsoEG_Efficiency__1->SetBinContent(14,0.01802842);
   h_jet_IsoEG_Efficiency__1->SetBinContent(15,0.01906487);
   h_jet_IsoEG_Efficiency__1->SetBinContent(16,0.02179861);
   h_jet_IsoEG_Efficiency__1->SetBinContent(17,0.02563239);
   h_jet_IsoEG_Efficiency__1->SetBinContent(18,0.03001796);
   h_jet_IsoEG_Efficiency__1->SetBinContent(19,0.03627016);
   h_jet_IsoEG_Efficiency__1->SetBinContent(20,0.04285139);
   h_jet_IsoEG_Efficiency__1->SetBinContent(21,0.07402756);
   h_jet_IsoEG_Efficiency__1->SetEntries(0.3574035);

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
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetTitle("Offline jet p_{T} [GeV]");
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoEG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_jet_IsoEG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_jet_IsoEG_Efficiency__1->Draw("P");
   
   TH1D *h_jet_IsoEG_Efficiency__2 = new TH1D("h_jet_IsoEG_Efficiency__2","h_jet_IsoEG_Efficiency",20,0,100);
   h_jet_IsoEG_Efficiency__2->SetBinContent(4,0.003226149);
   h_jet_IsoEG_Efficiency__2->SetBinContent(5,0.006420864);
   h_jet_IsoEG_Efficiency__2->SetBinContent(6,0.01027776);
   h_jet_IsoEG_Efficiency__2->SetBinContent(7,0.01376423);
   h_jet_IsoEG_Efficiency__2->SetBinContent(8,0.01544863);
   h_jet_IsoEG_Efficiency__2->SetBinContent(9,0.01543974);
   h_jet_IsoEG_Efficiency__2->SetBinContent(10,0.01673724);
   h_jet_IsoEG_Efficiency__2->SetBinContent(11,0.01578055);
   h_jet_IsoEG_Efficiency__2->SetBinContent(12,0.0164813);
   h_jet_IsoEG_Efficiency__2->SetBinContent(13,0.01702093);
   h_jet_IsoEG_Efficiency__2->SetBinContent(14,0.01785624);
   h_jet_IsoEG_Efficiency__2->SetBinContent(15,0.01894114);
   h_jet_IsoEG_Efficiency__2->SetBinContent(16,0.02171035);
   h_jet_IsoEG_Efficiency__2->SetBinContent(17,0.02546394);
   h_jet_IsoEG_Efficiency__2->SetBinContent(18,0.0299385);
   h_jet_IsoEG_Efficiency__2->SetBinContent(19,0.03612835);
   h_jet_IsoEG_Efficiency__2->SetBinContent(20,0.04277084);
   h_jet_IsoEG_Efficiency__2->SetBinContent(21,0.07396863);
   h_jet_IsoEG_Efficiency__2->SetEntries(0.3234068);

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
   h_jet_IsoEG_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_jet_IsoEG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_jet_IsoEG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_IsoEG_Efficiency__3 = new TH1D("h_jet_IsoEG_Efficiency__3","h_jet_IsoEG_Efficiency",20,0,100);
   h_jet_IsoEG_Efficiency__3->SetBinContent(4,0.001597474);
   h_jet_IsoEG_Efficiency__3->SetBinContent(5,0.002118223);
   h_jet_IsoEG_Efficiency__3->SetBinContent(6,0.004683536);
   h_jet_IsoEG_Efficiency__3->SetBinContent(7,0.008485456);
   h_jet_IsoEG_Efficiency__3->SetBinContent(8,0.01139229);
   h_jet_IsoEG_Efficiency__3->SetBinContent(9,0.01260188);
   h_jet_IsoEG_Efficiency__3->SetBinContent(10,0.01485595);
   h_jet_IsoEG_Efficiency__3->SetBinContent(11,0.01460798);
   h_jet_IsoEG_Efficiency__3->SetBinContent(12,0.01568395);
   h_jet_IsoEG_Efficiency__3->SetBinContent(13,0.0164961);
   h_jet_IsoEG_Efficiency__3->SetBinContent(14,0.017522);
   h_jet_IsoEG_Efficiency__3->SetBinContent(15,0.01870494);
   h_jet_IsoEG_Efficiency__3->SetBinContent(16,0.02159688);
   h_jet_IsoEG_Efficiency__3->SetBinContent(17,0.02535164);
   h_jet_IsoEG_Efficiency__3->SetBinContent(18,0.02979548);
   h_jet_IsoEG_Efficiency__3->SetBinContent(19,0.03605744);
   h_jet_IsoEG_Efficiency__3->SetBinContent(20,0.04273057);
   h_jet_IsoEG_Efficiency__3->SetBinContent(21,0.07391876);
   h_jet_IsoEG_Efficiency__3->SetEntries(0.2942818);

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
   h_jet_IsoEG_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_jet_IsoEG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoEG_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_jet_IsoEG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_IsoEG_Efficiency","L1 IsoEG p_{T} #geq 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoEG_Efficiency","L1 IsoEG p_{T} #geq 15 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoEG_Efficiency","L1 IsoEG p_{T} #geq 20 GeV ","pl");
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
      tex = new TLatex(0.23,0.92,"Offline jet |#eta| < 4.7");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
