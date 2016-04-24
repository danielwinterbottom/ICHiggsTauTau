void h_jet_IsoTauIsoEG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:16:54 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_IsoTauIsoEG_Efficiency__1 = new TH1D("h_jet_IsoTauIsoEG_Efficiency__1","h_jet_IsoTauIsoEG_Efficiency",20,0,100);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(4,0.008386738);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(5,0.01366615);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(6,0.01703695);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(7,0.01885193);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(8,0.01837685);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(9,0.01711783);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(10,0.01723013);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(11,0.01592429);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(12,0.01636622);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(13,0.01692139);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(14,0.01765367);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(15,0.01881742);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(16,0.02160949);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(17,0.02553413);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(18,0.02995439);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(19,0.03625244);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(20,0.04283125);
   h_jet_IsoTauIsoEG_Efficiency__1->SetBinContent(21,0.07396183);
   h_jet_IsoTauIsoEG_Efficiency__1->SetEntries(0.3525313);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jet_IsoTauIsoEG_Efficiency__1->SetFillColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_jet_IsoTauIsoEG_Efficiency__1->SetLineColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_jet_IsoTauIsoEG_Efficiency__1->SetMarkerColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__1->SetMarkerStyle(33);
   h_jet_IsoTauIsoEG_Efficiency__1->SetMarkerSize(1.7);
   h_jet_IsoTauIsoEG_Efficiency__1->GetXaxis()->SetTitle("Offline jet p_{T} [GeV]");
   h_jet_IsoTauIsoEG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_IsoTauIsoEG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_jet_IsoTauIsoEG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoTauIsoEG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_jet_IsoTauIsoEG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_jet_IsoTauIsoEG_Efficiency__1->Draw("P");
   
   TH1D *h_jet_IsoTauIsoEG_Efficiency__2 = new TH1D("h_jet_IsoTauIsoEG_Efficiency__2","h_jet_IsoTauIsoEG_Efficiency",20,0,100);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(4,0.003169987);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(5,0.006384758);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(6,0.01019497);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(7,0.01348358);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(8,0.01510958);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(9,0.01508339);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(10,0.0161194);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(11,0.01531153);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(12,0.01602098);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(13,0.01664993);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(14,0.01751188);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(15,0.01870494);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(16,0.02152124);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(17,0.02537971);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(18,0.02987494);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(19,0.03611062);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(20,0.0427507);
   h_jet_IsoTauIsoEG_Efficiency__2->SetBinContent(21,0.07389836);
   h_jet_IsoTauIsoEG_Efficiency__2->SetEntries(0.3193821);

   ci = TColor::GetColor("#99ccff");
   h_jet_IsoTauIsoEG_Efficiency__2->SetFillColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_jet_IsoTauIsoEG_Efficiency__2->SetLineColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_jet_IsoTauIsoEG_Efficiency__2->SetMarkerColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__2->SetMarkerStyle(33);
   h_jet_IsoTauIsoEG_Efficiency__2->SetMarkerSize(1.7);
   h_jet_IsoTauIsoEG_Efficiency__2->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
   h_jet_IsoTauIsoEG_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_jet_IsoTauIsoEG_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoTauIsoEG_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoTauIsoEG_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_jet_IsoTauIsoEG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_jet_IsoTauIsoEG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_IsoTauIsoEG_Efficiency__3 = new TH1D("h_jet_IsoTauIsoEG_Efficiency__3","h_jet_IsoTauIsoEG_Efficiency",20,0,100);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(4,0.001360349);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(5,0.0008785813);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(6,0.001419253);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(7,0.003349994);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(8,0.006540702);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(9,0.009511345);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(10,0.01252343);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(11,0.01340515);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(12,0.01493592);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(13,0.0160346);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(14,0.01708649);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(15,0.0184125);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(16,0.02143298);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(17,0.02523934);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(18,0.02968425);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(19,0.03603971);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(20,0.04271043);
   h_jet_IsoTauIsoEG_Efficiency__3->SetBinContent(21,0.07384622);
   h_jet_IsoTauIsoEG_Efficiency__3->SetEntries(0.270565);

   ci = TColor::GetColor("#99ccff");
   h_jet_IsoTauIsoEG_Efficiency__3->SetFillColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_jet_IsoTauIsoEG_Efficiency__3->SetLineColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_jet_IsoTauIsoEG_Efficiency__3->SetMarkerColor(ci);
   h_jet_IsoTauIsoEG_Efficiency__3->SetMarkerStyle(33);
   h_jet_IsoTauIsoEG_Efficiency__3->SetMarkerSize(1.7);
   h_jet_IsoTauIsoEG_Efficiency__3->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
   h_jet_IsoTauIsoEG_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_jet_IsoTauIsoEG_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoTauIsoEG_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoTauIsoEG_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_jet_IsoTauIsoEG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoTauIsoEG_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_jet_IsoTauIsoEG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.02);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_IsoTauIsoEG_Efficiency","L1 IsoEG, IsoTau p_{T} #geq 10, 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoTauIsoEG_Efficiency","L1 IsoEG, IsoTau p_{T} #geq 15, 20 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoTauIsoEG_Efficiency","L1 IsoEG, IsoTau p_{T} #geq 20, 30 GeV ","pl");
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
