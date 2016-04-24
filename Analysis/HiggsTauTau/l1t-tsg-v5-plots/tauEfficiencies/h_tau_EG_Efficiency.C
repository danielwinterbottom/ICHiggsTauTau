void h_tau_EG_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:40:46 2016) by ROOT version6.06/01
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
   
   TH1D *h_tau_EG_Efficiency__1 = new TH1D("h_tau_EG_Efficiency__1","h_tau_EG_Efficiency",20,0,100);
   h_tau_EG_Efficiency__1->SetBinContent(4,0.2578652);
   h_tau_EG_Efficiency__1->SetBinContent(5,0.3002519);
   h_tau_EG_Efficiency__1->SetBinContent(6,0.3128612);
   h_tau_EG_Efficiency__1->SetBinContent(7,0.3006422);
   h_tau_EG_Efficiency__1->SetBinContent(8,0.2777437);
   h_tau_EG_Efficiency__1->SetBinContent(9,0.2629246);
   h_tau_EG_Efficiency__1->SetBinContent(10,0.2457897);
   h_tau_EG_Efficiency__1->SetBinContent(11,0.2296914);
   h_tau_EG_Efficiency__1->SetBinContent(12,0.2189999);
   h_tau_EG_Efficiency__1->SetBinContent(13,0.2130632);
   h_tau_EG_Efficiency__1->SetBinContent(14,0.2052279);
   h_tau_EG_Efficiency__1->SetBinContent(15,0.1996565);
   h_tau_EG_Efficiency__1->SetBinContent(16,0.1994957);
   h_tau_EG_Efficiency__1->SetBinContent(17,0.1984708);
   h_tau_EG_Efficiency__1->SetBinContent(18,0.1920939);
   h_tau_EG_Efficiency__1->SetBinContent(19,0.1917647);
   h_tau_EG_Efficiency__1->SetBinContent(20,0.1879549);
   h_tau_EG_Efficiency__1->SetBinContent(21,0.173732);
   h_tau_EG_Efficiency__1->SetEntries(3.994497);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_tau_EG_Efficiency__1->SetFillColor(ci);
   h_tau_EG_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_tau_EG_Efficiency__1->SetLineColor(ci);
   h_tau_EG_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_tau_EG_Efficiency__1->SetMarkerColor(ci);
   h_tau_EG_Efficiency__1->SetMarkerStyle(33);
   h_tau_EG_Efficiency__1->SetMarkerSize(1.7);
   h_tau_EG_Efficiency__1->GetXaxis()->SetTitle("Offline tau p_{T} [GeV]");
   h_tau_EG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_tau_EG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_tau_EG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_tau_EG_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_tau_EG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_tau_EG_Efficiency__1->Draw("P");
   
   TH1D *h_tau_EG_Efficiency__2 = new TH1D("h_tau_EG_Efficiency__2","h_tau_EG_Efficiency",20,0,100);
   h_tau_EG_Efficiency__2->SetBinContent(4,0.1314626);
   h_tau_EG_Efficiency__2->SetBinContent(5,0.2071252);
   h_tau_EG_Efficiency__2->SetBinContent(6,0.2678013);
   h_tau_EG_Efficiency__2->SetBinContent(7,0.2808831);
   h_tau_EG_Efficiency__2->SetBinContent(8,0.2691238);
   h_tau_EG_Efficiency__2->SetBinContent(9,0.2588002);
   h_tau_EG_Efficiency__2->SetBinContent(10,0.2433801);
   h_tau_EG_Efficiency__2->SetBinContent(11,0.2282189);
   h_tau_EG_Efficiency__2->SetBinContent(12,0.2177884);
   h_tau_EG_Efficiency__2->SetBinContent(13,0.2117689);
   h_tau_EG_Efficiency__2->SetBinContent(14,0.2038969);
   h_tau_EG_Efficiency__2->SetBinContent(15,0.1986386);
   h_tau_EG_Efficiency__2->SetBinContent(16,0.1984484);
   h_tau_EG_Efficiency__2->SetBinContent(17,0.1975036);
   h_tau_EG_Efficiency__2->SetBinContent(18,0.1913155);
   h_tau_EG_Efficiency__2->SetBinContent(19,0.1906536);
   h_tau_EG_Efficiency__2->SetBinContent(20,0.1871551);
   h_tau_EG_Efficiency__2->SetBinContent(21,0.1727637);
   h_tau_EG_Efficiency__2->SetEntries(3.683964);

   ci = TColor::GetColor("#99ccff");
   h_tau_EG_Efficiency__2->SetFillColor(ci);
   h_tau_EG_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_tau_EG_Efficiency__2->SetLineColor(ci);
   h_tau_EG_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_tau_EG_Efficiency__2->SetMarkerColor(ci);
   h_tau_EG_Efficiency__2->SetMarkerStyle(33);
   h_tau_EG_Efficiency__2->SetMarkerSize(1.7);
   h_tau_EG_Efficiency__2->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
   h_tau_EG_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_tau_EG_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_tau_EG_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_tau_EG_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_tau_EG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_tau_EG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_tau_EG_Efficiency__3 = new TH1D("h_tau_EG_Efficiency__3","h_tau_EG_Efficiency",20,0,100);
   h_tau_EG_Efficiency__3->SetBinContent(4,0.04842097);
   h_tau_EG_Efficiency__3->SetBinContent(5,0.09603395);
   h_tau_EG_Efficiency__3->SetBinContent(6,0.1873441);
   h_tau_EG_Efficiency__3->SetBinContent(7,0.2403626);
   h_tau_EG_Efficiency__3->SetBinContent(8,0.2500623);
   h_tau_EG_Efficiency__3->SetBinContent(9,0.2495037);
   h_tau_EG_Efficiency__3->SetBinContent(10,0.2386512);
   h_tau_EG_Efficiency__3->SetBinContent(11,0.2257235);
   h_tau_EG_Efficiency__3->SetBinContent(12,0.2166701);
   h_tau_EG_Efficiency__3->SetBinContent(13,0.2109433);
   h_tau_EG_Efficiency__3->SetBinContent(14,0.2033113);
   h_tau_EG_Efficiency__3->SetBinContent(15,0.1977798);
   h_tau_EG_Efficiency__3->SetBinContent(16,0.1977502);
   h_tau_EG_Efficiency__3->SetBinContent(17,0.197043);
   h_tau_EG_Efficiency__3->SetBinContent(18,0.1909263);
   h_tau_EG_Efficiency__3->SetBinContent(19,0.1896732);
   h_tau_EG_Efficiency__3->SetBinContent(20,0.1865952);
   h_tau_EG_Efficiency__3->SetBinContent(21,0.1721801);
   h_tau_EG_Efficiency__3->SetEntries(3.326795);

   ci = TColor::GetColor("#99ccff");
   h_tau_EG_Efficiency__3->SetFillColor(ci);
   h_tau_EG_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_tau_EG_Efficiency__3->SetLineColor(ci);
   h_tau_EG_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_tau_EG_Efficiency__3->SetMarkerColor(ci);
   h_tau_EG_Efficiency__3->SetMarkerStyle(33);
   h_tau_EG_Efficiency__3->SetMarkerSize(1.7);
   h_tau_EG_Efficiency__3->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
   h_tau_EG_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_tau_EG_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_tau_EG_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_tau_EG_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_tau_EG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_tau_EG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_tau_EG_Efficiency","L1 EG p_{T} #geq 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_tau_EG_Efficiency","L1 EG p_{T} #geq 15 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_tau_EG_Efficiency","L1 EG p_{T} #geq 20 GeV ","pl");
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
