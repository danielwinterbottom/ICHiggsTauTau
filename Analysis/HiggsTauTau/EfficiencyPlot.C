{
TString foldername = "ProposalPlots/";
TString histname = "h_jetjet_Mjj_Efficiency";
TString histnameTotal = "h_jetjet_Mjj_Total";

TFile *f1 = new TFile("output/Trigger_2016/v58pt1_Efficiencies.root");

TH1D *htotal = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies1/"+histnameTotal);
TH1D *h1 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies1/"+histname);
TH1D *h2 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies2/"+histname);
TH1D *h3 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies3/"+histname);

int rebinN = 3;
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

TGaxis::SetMaxDigits(3);
gStyle->SetOptStat(0);
gStyle->SetPadGridX(1);
gStyle->SetPadGridY(1);

//h1->GetXaxis()->SetRangeUser(0, 180);
h1->GetXaxis()->SetTitle("offline M_{jj} [GeV]");

h1->SetFillStyle(0);
h1->SetMarkerStyle(33);
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
h2->SetMarkerStyle(33);
h2->SetMarkerSize(1.7);

h3->SetFillStyle(0);
ci = TColor::GetColor("#00cc00");
h3->SetFillStyle(0);
h3->SetMarkerColor(ci);
h3->SetLineColor(ci);
h3->SetLineWidth(2);
h3->SetMarkerStyle(33);
h3->SetMarkerSize(1.7);

//h1->GetYaxis()->SetRangeUser(0,1.1);

h1->Draw("P");
h2->Draw("SAMEP");
h3->Draw("SAMEP");

leg = new TLegend(0.57,0.28,0.85,0.48);
//leg = new TLegend(0.58,0.13,0.85,0.33);
//leg = new TLegend(0.15,0.72,0.38,0.88);
leg->AddEntry(h1,"L1 M_{jj} #geq 400 GeV ","p");
leg->AddEntry(h2,"L1 M_{jj} #geq 500 GeV ","p");
leg->AddEntry(h3,"L1 M_{jj} #geq 600 GeV ","p");

leg->Draw();

TLatex *   tex = new TLatex(0.65,0.92,"l1t-integration-v58.1");
tex->SetNDC();
tex->SetTextFont(44);
tex->SetTextSize(23);
tex->SetLineWidth(2);
tex->Draw();

TLatex *   tex2 = new TLatex(0.15,0.92,"offline jets |#eta| < 4.7, p_{T} > 30 GeV");
tex2->SetNDC();
tex2->SetTextFont(44);
tex2->SetTextSize(23);
tex2->SetLineWidth(2);
tex2->Draw();

c1->Print(foldername+histname+"_v58pt1.C");
c1->Print(foldername+histname+"_v58pt1.png");
c1->Print(foldername+histname+"_v58pt1.pdf");

}
