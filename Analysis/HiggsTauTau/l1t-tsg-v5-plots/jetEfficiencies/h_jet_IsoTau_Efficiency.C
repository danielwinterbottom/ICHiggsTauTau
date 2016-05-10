void h_jet_IsoTau_Efficiency()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Apr 24 15:02:05 2016) by ROOT version6.06/01
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
   
   TH1D *h_jet_IsoTau_Efficiency__1 = new TH1D("h_jet_IsoTau_Efficiency__1","h_jet_IsoTau_Efficiency",20,0,100);
   h_jet_IsoTau_Efficiency__1->SetBinContent(4,0.1823055);
   h_jet_IsoTau_Efficiency__1->SetBinContent(5,0.2626236);
   h_jet_IsoTau_Efficiency__1->SetBinContent(6,0.3125551);
   h_jet_IsoTau_Efficiency__1->SetBinContent(7,0.3241253);
   h_jet_IsoTau_Efficiency__1->SetBinContent(8,0.3156983);
   h_jet_IsoTau_Efficiency__1->SetBinContent(9,0.3079719);
   h_jet_IsoTau_Efficiency__1->SetBinContent(10,0.3040264);
   h_jet_IsoTau_Efficiency__1->SetBinContent(11,0.3111478);
   h_jet_IsoTau_Efficiency__1->SetBinContent(12,0.3241597);
   h_jet_IsoTau_Efficiency__1->SetBinContent(13,0.3475401);
   h_jet_IsoTau_Efficiency__1->SetBinContent(14,0.376561);
   h_jet_IsoTau_Efficiency__1->SetBinContent(15,0.4199219);
   h_jet_IsoTau_Efficiency__1->SetBinContent(16,0.4706053);
   h_jet_IsoTau_Efficiency__1->SetBinContent(17,0.5219265);
   h_jet_IsoTau_Efficiency__1->SetBinContent(18,0.5770948);
   h_jet_IsoTau_Efficiency__1->SetBinContent(19,0.625616);
   h_jet_IsoTau_Efficiency__1->SetBinContent(20,0.6708417);
   h_jet_IsoTau_Efficiency__1->SetBinContent(21,0.8374105);
   h_jet_IsoTau_Efficiency__1->SetEntries(6.654721);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   h_jet_IsoTau_Efficiency__1->SetFillColor(ci);
   h_jet_IsoTau_Efficiency__1->SetFillStyle(0);

   ci = TColor::GetColor("#0000cc");
   h_jet_IsoTau_Efficiency__1->SetLineColor(ci);
   h_jet_IsoTau_Efficiency__1->SetLineWidth(2);

   ci = TColor::GetColor("#0000cc");
   h_jet_IsoTau_Efficiency__1->SetMarkerColor(ci);
   h_jet_IsoTau_Efficiency__1->SetMarkerStyle(33);
   h_jet_IsoTau_Efficiency__1->SetMarkerSize(1.7);
   h_jet_IsoTau_Efficiency__1->GetXaxis()->SetTitle("Offline jet p_{T} [GeV]");
   h_jet_IsoTau_Efficiency__1->GetXaxis()->SetRange(1,100);
   h_jet_IsoTau_Efficiency__1->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__1->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__1->GetYaxis()->SetTitle("Probability");
   h_jet_IsoTau_Efficiency__1->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__1->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__1->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoTau_Efficiency__1->GetZaxis()->SetLabelFont(42);
   h_jet_IsoTau_Efficiency__1->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__1->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__1->GetZaxis()->SetTitleFont(42);
   h_jet_IsoTau_Efficiency__1->Draw("P");
   
   TH1D *h_jet_IsoTau_Efficiency__2 = new TH1D("h_jet_IsoTau_Efficiency__2","h_jet_IsoTau_Efficiency",20,0,100);
   h_jet_IsoTau_Efficiency__2->SetBinContent(4,0.04854823);
   h_jet_IsoTau_Efficiency__2->SetBinContent(5,0.08705176);
   h_jet_IsoTau_Efficiency__2->SetBinContent(6,0.1548583);
   h_jet_IsoTau_Efficiency__2->SetBinContent(7,0.2148893);
   h_jet_IsoTau_Efficiency__2->SetBinContent(8,0.2489844);
   h_jet_IsoTau_Efficiency__2->SetBinContent(9,0.2678856);
   h_jet_IsoTau_Efficiency__2->SetBinContent(10,0.2791878);
   h_jet_IsoTau_Efficiency__2->SetBinContent(11,0.2939601);
   h_jet_IsoTau_Efficiency__2->SetBinContent(12,0.3115912);
   h_jet_IsoTau_Efficiency__2->SetBinContent(13,0.337532);
   h_jet_IsoTau_Efficiency__2->SetBinContent(14,0.3673443);
   h_jet_IsoTau_Efficiency__2->SetBinContent(15,0.4121273);
   h_jet_IsoTau_Efficiency__2->SetBinContent(16,0.4639359);
   h_jet_IsoTau_Efficiency__2->SetBinContent(17,0.5161992);
   h_jet_IsoTau_Efficiency__2->SetBinContent(18,0.5724706);
   h_jet_IsoTau_Efficiency__2->SetBinContent(19,0.6218046);
   h_jet_IsoTau_Efficiency__2->SetBinContent(20,0.6674184);
   h_jet_IsoTau_Efficiency__2->SetBinContent(21,0.8365491);
   h_jet_IsoTau_Efficiency__2->SetEntries(5.865789);

   ci = TColor::GetColor("#99ccff");
   h_jet_IsoTau_Efficiency__2->SetFillColor(ci);
   h_jet_IsoTau_Efficiency__2->SetFillStyle(0);

   ci = TColor::GetColor("#cc0000");
   h_jet_IsoTau_Efficiency__2->SetLineColor(ci);
   h_jet_IsoTau_Efficiency__2->SetLineWidth(2);

   ci = TColor::GetColor("#cc0000");
   h_jet_IsoTau_Efficiency__2->SetMarkerColor(ci);
   h_jet_IsoTau_Efficiency__2->SetMarkerStyle(33);
   h_jet_IsoTau_Efficiency__2->SetMarkerSize(1.7);
   h_jet_IsoTau_Efficiency__2->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_IsoTau_Efficiency__2->GetXaxis()->SetRange(1,100);
   h_jet_IsoTau_Efficiency__2->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__2->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__2->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoTau_Efficiency__2->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__2->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__2->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoTau_Efficiency__2->GetZaxis()->SetLabelFont(42);
   h_jet_IsoTau_Efficiency__2->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__2->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__2->GetZaxis()->SetTitleFont(42);
   h_jet_IsoTau_Efficiency__2->Draw("SAMEP");
   
   TH1D *h_jet_IsoTau_Efficiency__3 = new TH1D("h_jet_IsoTau_Efficiency__3","h_jet_IsoTau_Efficiency",20,0,100);
   h_jet_IsoTau_Efficiency__3->SetBinContent(4,0.01837719);
   h_jet_IsoTau_Efficiency__3->SetBinContent(5,0.01065731);
   h_jet_IsoTau_Efficiency__3->SetBinContent(6,0.01101104);
   h_jet_IsoTau_Efficiency__3->SetBinContent(7,0.02274532);
   h_jet_IsoTau_Efficiency__3->SetBinContent(8,0.05207903);
   h_jet_IsoTau_Efficiency__3->SetBinContent(9,0.1041648);
   h_jet_IsoTau_Efficiency__3->SetBinContent(10,0.1649427);
   h_jet_IsoTau_Efficiency__3->SetBinContent(11,0.2241051);
   h_jet_IsoTau_Efficiency__3->SetBinContent(12,0.2711318);
   h_jet_IsoTau_Efficiency__3->SetBinContent(13,0.3134439);
   h_jet_IsoTau_Efficiency__3->SetBinContent(14,0.3533469);
   h_jet_IsoTau_Efficiency__3->SetBinContent(15,0.4033991);
   h_jet_IsoTau_Efficiency__3->SetBinContent(16,0.4575312);
   h_jet_IsoTau_Efficiency__3->SetBinContent(17,0.5118336);
   h_jet_IsoTau_Efficiency__3->SetBinContent(18,0.5690063);
   h_jet_IsoTau_Efficiency__3->SetBinContent(19,0.6192519);
   h_jet_IsoTau_Efficiency__3->SetBinContent(20,0.6652839);
   h_jet_IsoTau_Efficiency__3->SetBinContent(21,0.8358691);
   h_jet_IsoTau_Efficiency__3->SetEntries(4.772311);

   ci = TColor::GetColor("#99ccff");
   h_jet_IsoTau_Efficiency__3->SetFillColor(ci);
   h_jet_IsoTau_Efficiency__3->SetFillStyle(0);

   ci = TColor::GetColor("#00cc00");
   h_jet_IsoTau_Efficiency__3->SetLineColor(ci);
   h_jet_IsoTau_Efficiency__3->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   h_jet_IsoTau_Efficiency__3->SetMarkerColor(ci);
   h_jet_IsoTau_Efficiency__3->SetMarkerStyle(33);
   h_jet_IsoTau_Efficiency__3->SetMarkerSize(1.7);
   h_jet_IsoTau_Efficiency__3->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
   h_jet_IsoTau_Efficiency__3->GetXaxis()->SetRange(1,100);
   h_jet_IsoTau_Efficiency__3->GetXaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__3->GetXaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__3->GetYaxis()->SetTitle("Efficiency");
   h_jet_IsoTau_Efficiency__3->GetYaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__3->GetYaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__3->GetYaxis()->SetTitleOffset(1.4);
   h_jet_IsoTau_Efficiency__3->GetZaxis()->SetLabelFont(42);
   h_jet_IsoTau_Efficiency__3->GetZaxis()->SetLabelSize(0.035);
   h_jet_IsoTau_Efficiency__3->GetZaxis()->SetTitleSize(0.035);
   h_jet_IsoTau_Efficiency__3->GetZaxis()->SetTitleFont(42);
   h_jet_IsoTau_Efficiency__3->Draw("SAMEP");
   
   TLegend *leg = new TLegend(-2.353437e-185,-2.353437e-185,-2.353437e-185,-2.353437e-185,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(3);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h_jet_IsoTau_Efficiency","L1 IsoTau p_{T} #geq 10 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoTau_Efficiency","L1 IsoTau p_{T} #geq 20 GeV ","pl");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("h_jet_IsoTau_Efficiency","L1 IsoTau p_{T} #geq 30 GeV ","pl");
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
