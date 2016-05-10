{
TString foldername = "l1t-tsg-v5-plots/VBFJetsPlots3/";
TString histname = "h_tauEta";
   
TFile *f1 = new TFile("output/Trigger_2016/RateVsEffV5/ZBDiJetAvg40Mjj450DeltaEta3.root");
TH1D * h1 = (TH1D*)f1->Get("ZeroBiasL1TFilter/"+histname+"_Before");

TFile *f2 = new TFile("output/Trigger_2016/RateVsEffV5/ZBDiJetAvg40Mjj450DeltaEta3.root");
TH1D * h2 = (TH1D*)f2->Get("ZeroBiasL1TFilter/"+histname+"_After");


gStyle->SetOptStat(0);

TGaxis::SetMaxDigits(3);
h2->GetXaxis()->SetTitle("L1 di-jet p_{T} [GeV]");
h1->SetFillStyle(0);
h2->SetFillStyle(0);

h1->SetLineColor(kBlue);
h2->SetLineColor(kRed);
h1->Rebin(2);
h2->Rebin(2);

h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));

h2->GetYaxis()->SetLabelOffset(100);
h2->GetYaxis()->SetTitleOffset(100);
h1->GetYaxis()->SetLabelOffset(100);
h1->GetYaxis()->SetTitleOffset(100);
//h1->GetYaxis()->SetRangeUser(0,0.05);

h1->Draw("");
h2->Draw("same");


      TLatex *   tex2 = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex2->SetNDC();
   tex2->SetTextFont(44);
   tex2->SetTextSize(23);
   tex2->SetLineWidth(2);
   tex2->Draw();
   
    TLatex *   tex3 = new TLatex(0.15,0.92,"L1 Iso-taus p_{T} #geq 20 GeV ");
tex3->SetNDC();
   tex3->SetTextFont(44);
   tex3->SetTextSize(23);
   tex3->SetLineWidth(2);
   tex3->Draw();
   
leg = new TLegend(0.54,0.70,0.87,0.87);
leg->SetTextSize(0.023);
leg->AddEntry(h1,"No VBF selections ","l");
leg->AddEntry(h2,"L1 M_{jj} #geq 450 GeV,","l");
leg->AddEntry((TObject*)0,"|#Delta#eta|_{jj} #geq 3, <p_{T}> #geq 40 GeV","");

leg->Draw();

   
c1->Print(foldername+histname+".C");
c1->Print(foldername+histname+".png");
c1->Print(foldername+histname+".pdf");

}
