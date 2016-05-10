{
TFile *f1 = new TFile("output/Trigger_2016/RateVsEffV5/DiJetAvg60Mjj855.root");
TFile *f2 = new TFile("output/Trigger_2016/RateVsEffV5/DoubleIsoTau28er.root");
TFile *f3 = new TFile("output/Trigger_2016/RateVsEffV5/SingleMu20er_em.root");
TFile *f4 = new TFile("output/Trigger_2016/RateVsEffV5/SingleMu22_em.root");
TFile *f5 = new TFile("output/Trigger_2016/RateVsEffV5/Mu20_EG15.root");
TFile *f6 = new TFile("output/Trigger_2016/RateVsEffV5/Mu5_EG20.root");
TFile *f7 = new TFile("output/Trigger_2016/RateVsEffV5/Mu5_IsoEG18.root");
TString histname = "h_L1Filter_Efficiency";

TH1D * h1 = (TH1D*)f1->Get("L1TFilter/"+histname);
TH1D * h2 = (TH1D*)f2->Get("L1TFilter/"+histname);
TH1D * h3 = (TH1D*)f3->Get("L1TFilter/"+histname);
TH1D * h4 = (TH1D*)f4->Get("L1TFilter/"+histname);
TH1D * h5 = (TH1D*)f5->Get("L1TFilter/"+histname);
TH1D * h6 = (TH1D*)f6->Get("L1TFilter/"+histname);
TH1D * h7 = (TH1D*)f7->Get("L1TFilter/"+histname);
cout << h1->GetEntries() << endl;
cout << h2->GetEntries() << endl;
//cout << h3->GetEntries() << endl;
h1->Add(h2);
//h1->Add(h3);
//h1->Add(h4);
//h1->Add(h5);
//h1->Add(h6);
//h1->Add(h7);
int Passcount = 0;
for(int i=1; i<= h1->GetNbinsX(); i++){
    if(h1->GetBinContent(i) > 0) Passcount++;         
}
double Total = 952407.;

double SignalEfficiency = Passcount/Total;

cout << Passcount << " out of " << Total << endl;
cout << SignalEfficiency << endl;
}

//mt total 0.0464054
//em total 0.0527955