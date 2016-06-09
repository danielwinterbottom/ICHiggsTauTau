void h_tau_EG_Efficiency_v58pt1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu Jun  9 15:46:06 2016) by ROOT version6.06/01
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
   
   TH1D *h_tau_EG_Efficiency__1 = new TH1D("h_tau_EG_Efficiency__1","h_tau_EG_Efficiency",20,0,100);
   h_tau_EG_Efficiency__1->SetBinContent(4,0.2543212);
   h_tau_EG_Efficiency__1->SetBinContent(5,0.295823);
   h_tau_EG_Efficiency__1->SetBinContent(6,0.3098398);
   h_tau_EG_Efficiency__1->SetBinContent(7,0.2964738);
   h_tau_EG_Efficiency__1->SetBinContent(8,0.2726355);
   h_tau_EG_Efficiency__1->SetBinContent(9,0.2603471);
   h_tau_EG_Efficiency__1->SetBinContent(10,0.2428735);
   h_tau_EG_Efficiency__1->SetBinContent(11,0.2266198);
   h_tau_EG_Efficiency__1->SetBinContent(12,0.2159742);
   h_tau_EG_Efficiency__1->SetBinContent(13,0.2082031);
   h_tau_EG_Efficiency__1->SetBinContent(14,0.2017333);
   h_tau_EG_Efficiency__1->SetBinContent(15,0.1961173);
   h_tau_EG_Efficiency__1->SetBinContent(16,0.1946033);
   h_tau_EG_Efficiency__1->SetBinContent(17,0.1953743);
   h_tau_EG_Efficiency__1->SetBinContent(18,0.1861867);
   h_tau_EG_Efficiency__1->SetBinContent(19,0.1837246);
   h_tau_EG_Efficiency__1->SetBinContent(20,0.1847053);
   h_tau_EG_Efficiency__1->SetBinContent(21,0.1673055);
   h_tau_EG_Efficiency__1->SetEntries(3.925556);

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
   h_tau_EG_Efficiency__1->GetXaxis()->SetTitle("offline tau p_{T} [GeV]");
   h_tau_EG_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_tau_EG_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__1->GetYaxis()->SetTitle("Efficiency");
   h_tau_EG_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_tau_EG_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__1->Draw("P");
   
   TH1D *h_tau_EG_Efficiency__2 = new TH1D("h_tau_EG_Efficiency__2","h_tau_EG_Efficiency",20,0,100);
   h_tau_EG_Efficiency__2->SetBinContent(4,0.127095);
   h_tau_EG_Efficiency__2->SetBinContent(5,0.2028355);
   h_tau_EG_Efficiency__2->SetBinContent(6,0.2660289);
   h_tau_EG_Efficiency__2->SetBinContent(7,0.2779658);
   h_tau_EG_Efficiency__2->SetBinContent(8,0.2647319);
   h_tau_EG_Efficiency__2->SetBinContent(9,0.257036);
   h_tau_EG_Efficiency__2->SetBinContent(10,0.2411477);
   h_tau_EG_Efficiency__2->SetBinContent(11,0.2257529);
   h_tau_EG_Efficiency__2->SetBinContent(12,0.2155862);
   h_tau_EG_Efficiency__2->SetBinContent(13,0.2076644);
   h_tau_EG_Efficiency__2->SetBinContent(14,0.2013043);
   h_tau_EG_Efficiency__2->SetBinContent(15,0.1956599);
   h_tau_EG_Efficiency__2->SetBinContent(16,0.1941059);
   h_tau_EG_Efficiency__2->SetBinContent(17,0.1951526);
   h_tau_EG_Efficiency__2->SetBinContent(18,0.1858321);
   h_tau_EG_Efficiency__2->SetBinContent(19,0.1832029);
   h_tau_EG_Efficiency__2->SetBinContent(20,0.1843242);
   h_tau_EG_Efficiency__2->SetBinContent(21,0.1670107);
   h_tau_EG_Efficiency__2->SetEntries(3.625426);

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
   h_tau_EG_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_tau_EG_Efficiency__3 = new TH1D("h_tau_EG_Efficiency__3","h_tau_EG_Efficiency",20,0,100);
   h_tau_EG_Efficiency__3->SetBinContent(4,0.04388468);
   h_tau_EG_Efficiency__3->SetBinContent(5,0.09167779);
   h_tau_EG_Efficiency__3->SetBinContent(6,0.1858781);
   h_tau_EG_Efficiency__3->SetBinContent(7,0.238726);
   h_tau_EG_Efficiency__3->SetBinContent(8,0.2461763);
   h_tau_EG_Efficiency__3->SetBinContent(9,0.2481531);
   h_tau_EG_Efficiency__3->SetBinContent(10,0.237218);
   h_tau_EG_Efficiency__3->SetBinContent(11,0.2236725);
   h_tau_EG_Efficiency__3->SetBinContent(12,0.2148699);
   h_tau_EG_Efficiency__3->SetBinContent(13,0.2071616);
   h_tau_EG_Efficiency__3->SetBinContent(14,0.2010039);
   h_tau_EG_Efficiency__3->SetBinContent(15,0.1954058);
   h_tau_EG_Efficiency__3->SetBinContent(16,0.1938572);
   h_tau_EG_Efficiency__3->SetBinContent(17,0.1950048);
   h_tau_EG_Efficiency__3->SetBinContent(18,0.1856548);
   h_tau_EG_Efficiency__3->SetBinContent(19,0.1827856);
   h_tau_EG_Efficiency__3->SetBinContent(20,0.1840701);
   h_tau_EG_Efficiency__3->SetBinContent(21,0.1668001);
   h_tau_EG_Efficiency__3->SetEntries(3.2752);

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
   h_tau_EG_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_tau_EG_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_tau_EG_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_tau_EG_Efficiency","L1 EG #geq 10 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_tau_EG_Efficiency","L1 EG #geq 15 GeV","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_tau_EG_Efficiency","L1 EG #geq 20 GeV","pl");
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
      tex = new TLatex(0.15,0.92,"offline tau |#eta| < 2.3");
tex->SetNDC();
   tex->SetTextFont(44);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
