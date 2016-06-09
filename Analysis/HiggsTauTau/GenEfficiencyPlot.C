{
TString foldername = "V58pt1MissFires/";
TString histname = "h_genjet_Jet_Efficiency";
TString histnameTotal = "h_genjet_Total";

TFile *f1 = new TFile("output/Trigger_2016/V58pt1Efficiencies_MVAMet.root");

TH1D *htotal = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies4/"+histnameTotal);
TH1D *h1 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies4/"+histname);
TH1D *h2 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies4/h_genjet_IsoEG_Efficiency");
TH1D *h3 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies4/h_genjet_IsoTau_Efficiency");
TH1D *h4 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies4/h_genjet_IsoTauIsoEG_Efficiency");

int rebinN = 4;
htotal->Rebin(rebinN,"htotalnew");

h1->Multiply(htotal);
h1->Rebin(rebinN);
h1->Divide(htotalnew);

h2->Multiply(htotal);
h2->Rebin(rebinN);
h2->Divide(htotalnew);

h3->Multiply(htotal);
h3->Rebin(rebinN);
h3->Divide(htotalnew);

h4->Multiply(htotal);
h4->Rebin(rebinN);
h4->Divide(htotalnew);

TGaxis::SetMaxDigits(3);
gStyle->SetOptStat(0);
gStyle->SetPadGridX(1);
gStyle->SetPadGridY(1);

h1->GetYaxis()->SetRangeUser(0, 1);
h1->GetXaxis()->SetTitle("gen jet p_{T} [GeV]");

h1->SetFillStyle(0);
h1->SetMarkerStyle(21);
h1->SetMarkerSize(1.7);
ci = TColor::GetColor("#0000cc");
h1->SetLineColor(ci);
h1->SetLineWidth(2);
h1->SetMarkerColor(ci);
h1->GetYaxis()->SetTitleOffset(1.4);

h2->SetFillStyle(0);
ci = TColor::GetColor("#cc0000");
h2->SetMarkerColor(ci);
h2->SetLineColor(ci);
h2->SetLineWidth(2);
h2->SetMarkerStyle(20);
h2->SetMarkerSize(1.8);

h3->SetFillStyle(0);
ci = TColor::GetColor("#00cc00");
h3->SetFillStyle(0);
h3->SetMarkerColor(ci);
h3->SetLineColor(ci);
h3->SetLineWidth(2);
h3->SetMarkerStyle(22);
h3->SetMarkerSize(1.9);

h4->SetFillStyle(0);
h4->SetFillStyle(0);
h4->SetMarkerColor(kViolet-1);
h4->SetLineColor(kViolet-1);
h4->SetLineWidth(2);
h4->SetMarkerStyle(33);
h4->SetMarkerSize(1.7);

h1->Draw("P");
h2->Draw("SAMEP");
h3->Draw("SAMEP");
h4->Draw("SAMEP");

//leg = new TLegend(0.47,0.31,0.85,0.51);
leg = new TLegend(0.44,0.54,0.78,0.74);
//leg = new TLegend(0.15,0.72,0.38,0.88);
leg->AddEntry(h1,"L1 Jet ","pl");
leg->AddEntry(h2,"L1 Jet + IsoEG ","pl");
leg->AddEntry(h3,"L1 Jet + IsoTau ","pl");
leg->AddEntry(h4,"L1 Jet + IsoEG + IsoTau ","pl");

leg->Draw();

TLatex *   tex = new TLatex(0.65,0.92,"l1t-integration-v58.1");
tex->SetNDC();
tex->SetTextFont(44);
tex->SetTextSize(23);
tex->SetLineWidth(2);
tex->Draw();

TLatex *   tex2 = new TLatex(0.15,0.92,"gen jet |#eta| < 4.7");
tex2->SetNDC();
tex2->SetTextFont(44);
tex2->SetTextSize(23);
tex2->SetLineWidth(2);
tex2->Draw();
histname = "h_genjet_Iso_MissFire";
c1->Print(foldername+histname+"_v58pt1.C");
c1->Print(foldername+histname+"_v58pt1.png");
c1->Print(foldername+histname+"_v58pt1.pdf");

}
