void drawJetTauFakeRateWJetsSumPt(){
 TFile *qcd_1 = TFile::Open("Jan29_WJets.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"30","60","90","120"};
TH1F* hist1_sumpt1 = new TH1F("hist1_sumpt1","hist1_sumpt1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt1",vars);

 TH1F* jetpt_1_sumpt1 = (TH1F*)gDirectory->Get("hist1_sumpt1");

 jetpt_1_sumpt1->Sumw2();



TH1F* hist11_sumpt1 = new TH1F("hist11_sumpt1","hist11_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>0&&sumpt<"+limits[0];

 qcd_tree_1->Draw("jetpt>>hist11_sumpt1",vars);

 TH1F *taupt_1_sumpt1 = (TH1F*)gDirectory->Get("hist11_sumpt1");

 taupt_1_sumpt1->Sumw2();


 

 taupt_1_sumpt1->Divide(jetpt_1_sumpt1);
 taupt_1_sumpt1->SetMarkerStyle(20);
 taupt_1_sumpt1->SetMarkerColor(kRed);


TH1F* lhist1_sumpt1 = new TH1F("lhist1_sumpt1","lhist1_sumpt1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt1",vars);

 TH1F* ljetpt_1_sumpt1 = (TH1F*)gDirectory->Get("lhist1_sumpt1");

 ljetpt_1_sumpt1->Sumw2();


TH1F* lhist11_sumpt1 = new TH1F("lhist11_sumpt1","lhist11_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt1",vars);

 TH1F *ltaupt_1_sumpt1 = (TH1F*)gDirectory->Get("lhist11_sumpt1");

 ltaupt_1_sumpt1->Sumw2();



 ltaupt_1_sumpt1->Divide(ljetpt_1_sumpt1);
 ltaupt_1_sumpt1->SetMarkerStyle(20);
 ltaupt_1_sumpt1->SetMarkerColor(kGreen+3);


TH1F* bhist1_sumpt1 = new TH1F("bhist1_sumpt1","bhist1_sumpt1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt1",vars);

 TH1F* bjetpt_1_sumpt1 = (TH1F*)gDirectory->Get("bhist1_sumpt1");

 bjetpt_1_sumpt1->Sumw2();


TH1F* bhist11_sumpt1 = new TH1F("bhist11_sumpt1","bhist11_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt1",vars);

 TH1F *btaupt_1_sumpt1 = (TH1F*)gDirectory->Get("bhist11_sumpt1");

 btaupt_1_sumpt1->Sumw2();


 btaupt_1_sumpt1->Divide(bjetpt_1_sumpt1);
 btaupt_1_sumpt1->SetMarkerStyle(20);
 btaupt_1_sumpt1->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt1 = new TH1F("ghist1_sumpt1","ghist1_sumpt1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt1",vars);

 TH1F* gjetpt_1_sumpt1 = (TH1F*)gDirectory->Get("ghist1_sumpt1");

 gjetpt_1_sumpt1->Sumw2();

TH1F* ghist11_sumpt1 = new TH1F("ghist11_sumpt1","ghist11_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt1",vars);

 TH1F *gtaupt_1_sumpt1 = (TH1F*)gDirectory->Get("ghist11_sumpt1");

 gtaupt_1_sumpt1->Sumw2();


 gtaupt_1_sumpt1->Divide(gjetpt_1_sumpt1);
 gtaupt_1_sumpt1->SetMarkerStyle(20);

TH1F* hist1_sumpt2 = new TH1F("hist1_sumpt2","hist1_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt2",vars);

 TH1F* jetpt_1_sumpt2 = (TH1F*)gDirectory->Get("hist1_sumpt2");

 jetpt_1_sumpt2->Sumw2();




TH1F* hist11_sumpt2 = new TH1F("hist11_sumpt2","hist11_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt2",vars);

 TH1F *taupt_1_sumpt2 = (TH1F*)gDirectory->Get("hist11_sumpt2");

 taupt_1_sumpt2->Sumw2();



 taupt_1_sumpt2->Divide(jetpt_1_sumpt2);
 taupt_1_sumpt2->SetMarkerStyle(20);
 taupt_1_sumpt2->SetMarkerColor(kRed);


TH1F* lhist1_sumpt2 = new TH1F("lhist1_sumpt2","lhist1_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt2",vars);

 TH1F* ljetpt_1_sumpt2 = (TH1F*)gDirectory->Get("lhist1_sumpt2");

 ljetpt_1_sumpt2->Sumw2();



TH1F* lhist11_sumpt2 = new TH1F("lhist11_sumpt2","lhist11_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt2",vars);

 TH1F *ltaupt_1_sumpt2 = (TH1F*)gDirectory->Get("lhist11_sumpt2");

 ltaupt_1_sumpt2->Sumw2();


 ltaupt_1_sumpt2->Divide(ljetpt_1_sumpt2);
 ltaupt_1_sumpt2->SetMarkerStyle(20);
 ltaupt_1_sumpt2->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt2 = new TH1F("bhist1_sumpt2","bhist1_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt2",vars);

 TH1F* bjetpt_1_sumpt2 = (TH1F*)gDirectory->Get("bhist1_sumpt2");

 bjetpt_1_sumpt2->Sumw2();



TH1F* bhist11_sumpt2 = new TH1F("bhist11_sumpt2","bhist11_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt2",vars);

 TH1F *btaupt_1_sumpt2 = (TH1F*)gDirectory->Get("bhist11_sumpt2");

 btaupt_1_sumpt2->Sumw2();



 btaupt_1_sumpt2->Divide(bjetpt_1_sumpt2);
 btaupt_1_sumpt2->SetMarkerStyle(20);
 btaupt_1_sumpt2->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt2 = new TH1F("ghist1_sumpt2","ghist1_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt2",vars);

 TH1F* gjetpt_1_sumpt2 = (TH1F*)gDirectory->Get("ghist1_sumpt2");

 gjetpt_1_sumpt2->Sumw2();



TH1F* ghist11_sumpt2 = new TH1F("ghist11_sumpt2","ghist11_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt2",vars);

 TH1F *gtaupt_1_sumpt2 = (TH1F*)gDirectory->Get("ghist11_sumpt2");

 gtaupt_1_sumpt2->Sumw2();



 gtaupt_1_sumpt2->Divide(gjetpt_1_sumpt2);
 gtaupt_1_sumpt2->SetMarkerStyle(20);

TH1F* hist1_sumpt3 = new TH1F("hist1_sumpt3","hist1_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt3",vars);

 TH1F* jetpt_1_sumpt3 = (TH1F*)gDirectory->Get("hist1_sumpt3");

 jetpt_1_sumpt3->Sumw2();


TH1F* hist11_sumpt3 = new TH1F("hist11_sumpt3","hist11_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt3",vars);

 TH1F *taupt_1_sumpt3 = (TH1F*)gDirectory->Get("hist11_sumpt3");

 taupt_1_sumpt3->Sumw2();





 taupt_1_sumpt3->Divide(jetpt_1_sumpt3);
 taupt_1_sumpt3->SetMarkerStyle(20);
 taupt_1_sumpt3->SetMarkerColor(kRed);


TH1F* lhist1_sumpt3 = new TH1F("lhist1_sumpt3","lhist1_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt3",vars);

 TH1F* ljetpt_1_sumpt3 = (TH1F*)gDirectory->Get("lhist1_sumpt3");

 ljetpt_1_sumpt3->Sumw2();


TH1F* lhist11_sumpt3 = new TH1F("lhist11_sumpt3","lhist11_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt3",vars);

 TH1F *ltaupt_1_sumpt3 = (TH1F*)gDirectory->Get("lhist11_sumpt3");

 ltaupt_1_sumpt3->Sumw2();



 ltaupt_1_sumpt3->Divide(ljetpt_1_sumpt3);
 ltaupt_1_sumpt3->SetMarkerStyle(20);
 ltaupt_1_sumpt3->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt3 = new TH1F("bhist1_sumpt3","bhist1_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt3",vars);

 TH1F* bjetpt_1_sumpt3 = (TH1F*)gDirectory->Get("bhist1_sumpt3");

 bjetpt_1_sumpt3->Sumw2();



TH1F* bhist11_sumpt3 = new TH1F("bhist11_sumpt3","bhist11_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt3",vars);

 TH1F *btaupt_1_sumpt3 = (TH1F*)gDirectory->Get("bhist11_sumpt3");

 btaupt_1_sumpt3->Sumw2();



 btaupt_1_sumpt3->Divide(bjetpt_1_sumpt3);
 btaupt_1_sumpt3->SetMarkerStyle(20);
 btaupt_1_sumpt3->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt3 = new TH1F("ghist1_sumpt3","ghist1_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt3",vars);

 TH1F* gjetpt_1_sumpt3 = (TH1F*)gDirectory->Get("ghist1_sumpt3");

 gjetpt_1_sumpt3->Sumw2();




TH1F* ghist11_sumpt3 = new TH1F("ghist11_sumpt3","ghist11_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt3",vars);

 TH1F *gtaupt_1_sumpt3 = (TH1F*)gDirectory->Get("ghist11_sumpt3");

 gtaupt_1_sumpt3->Sumw2();





 gtaupt_1_sumpt3->Divide(gjetpt_1_sumpt3);
 gtaupt_1_sumpt3->SetMarkerStyle(20);



TH1F* hist1_sumpt4 = new TH1F("hist1_sumpt4","hist1_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt4",vars);

 TH1F* jetpt_1_sumpt4 = (TH1F*)gDirectory->Get("hist1_sumpt4");

 jetpt_1_sumpt4->Sumw2();



TH1F* hist11_sumpt4 = new TH1F("hist11_sumpt4","hist11_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt4",vars);

 TH1F *taupt_1_sumpt4 = (TH1F*)gDirectory->Get("hist11_sumpt4");

 taupt_1_sumpt4->Sumw2();



 taupt_1_sumpt4->Divide(jetpt_1_sumpt4);
 taupt_1_sumpt4->SetMarkerStyle(20);
 taupt_1_sumpt4->SetMarkerColor(kRed);


TH1F* lhist1_sumpt4 = new TH1F("lhist1_sumpt4","lhist1_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt4",vars);

 TH1F* ljetpt_1_sumpt4 = (TH1F*)gDirectory->Get("lhist1_sumpt4");

 ljetpt_1_sumpt4->Sumw2();



TH1F* lhist11_sumpt4 = new TH1F("lhist11_sumpt4","lhist11_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt4",vars);

 TH1F *ltaupt_1_sumpt4 = (TH1F*)gDirectory->Get("lhist11_sumpt4");

 ltaupt_1_sumpt4->Sumw2();



 ltaupt_1_sumpt4->Divide(ljetpt_1_sumpt4);
 ltaupt_1_sumpt4->SetMarkerStyle(20);
 ltaupt_1_sumpt4->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt4 = new TH1F("bhist1_sumpt4","bhist1_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt4",vars);

 TH1F* bjetpt_1_sumpt4 = (TH1F*)gDirectory->Get("bhist1_sumpt4");

 bjetpt_1_sumpt4->Sumw2();


TH1F* bhist11_sumpt4 = new TH1F("bhist11_sumpt4","bhist11_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt4",vars);

 TH1F *btaupt_1_sumpt4 = (TH1F*)gDirectory->Get("bhist11_sumpt4");

 btaupt_1_sumpt4->Sumw2();


 btaupt_1_sumpt4->Divide(bjetpt_1_sumpt4);
 btaupt_1_sumpt4->SetMarkerStyle(20);
 btaupt_1_sumpt4->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt4 = new TH1F("ghist1_sumpt4","ghist1_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt4",vars);

 TH1F* gjetpt_1_sumpt4 = (TH1F*)gDirectory->Get("ghist1_sumpt4");

 gjetpt_1_sumpt4->Sumw2();




TH1F* ghist11_sumpt4 = new TH1F("ghist11_sumpt4","ghist11_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt4",vars);

 TH1F *gtaupt_1_sumpt4 = (TH1F*)gDirectory->Get("ghist11_sumpt4");

 gtaupt_1_sumpt4->Sumw2();


 gtaupt_1_sumpt4->Divide(gjetpt_1_sumpt4);
 gtaupt_1_sumpt4->SetMarkerStyle(20);

TH1F* hist1_sumpt5 = new TH1F("hist1_sumpt5","hist1_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt5",vars);

 TH1F* jetpt_1_sumpt5 = (TH1F*)gDirectory->Get("hist1_sumpt5");

 jetpt_1_sumpt5->Sumw2();



TH1F* hist11_sumpt5 = new TH1F("hist11_sumpt5","hist11_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt5",vars);

 TH1F *taupt_1_sumpt5 = (TH1F*)gDirectory->Get("hist11_sumpt5");

 taupt_1_sumpt5->Sumw2();

 taupt_1_sumpt5->Divide(jetpt_1_sumpt5);
 taupt_1_sumpt5->SetMarkerStyle(20);
 taupt_1_sumpt5->SetMarkerColor(kRed);


TH1F* lhist1_sumpt5 = new TH1F("lhist1_sumpt5","lhist1_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt5",vars);

 TH1F* ljetpt_1_sumpt5 = (TH1F*)gDirectory->Get("lhist1_sumpt5");

 ljetpt_1_sumpt5->Sumw2();


TH1F* lhist11_sumpt5 = new TH1F("lhist11_sumpt5","lhist11_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt5",vars);

 TH1F *ltaupt_1_sumpt5 = (TH1F*)gDirectory->Get("lhist11_sumpt5");

 ltaupt_1_sumpt5->Sumw2();



 ltaupt_1_sumpt5->Divide(ljetpt_1_sumpt5);
 ltaupt_1_sumpt5->SetMarkerStyle(20);
 ltaupt_1_sumpt5->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt5 = new TH1F("bhist1_sumpt5","bhist1_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt5",vars);

 TH1F* bjetpt_1_sumpt5 = (TH1F*)gDirectory->Get("bhist1_sumpt5");

 bjetpt_1_sumpt5->Sumw2();


TH1F* bhist11_sumpt5 = new TH1F("bhist11_sumpt5","bhist11_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt5",vars);

 TH1F *btaupt_1_sumpt5 = (TH1F*)gDirectory->Get("bhist11_sumpt5");

 btaupt_1_sumpt5->Sumw2();



 btaupt_1_sumpt5->Divide(bjetpt_1_sumpt5);
 btaupt_1_sumpt5->SetMarkerStyle(20);
 btaupt_1_sumpt5->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt5 = new TH1F("ghist1_sumpt5","ghist1_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt5",vars);

 TH1F* gjetpt_1_sumpt5 = (TH1F*)gDirectory->Get("ghist1_sumpt5");

 gjetpt_1_sumpt5->Sumw2();



TH1F* ghist11_sumpt5 = new TH1F("ghist11_sumpt5","ghist11_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt5",vars);

 TH1F *gtaupt_1_sumpt5 = (TH1F*)gDirectory->Get("ghist11_sumpt5");

 gtaupt_1_sumpt5->Sumw2();





 gtaupt_1_sumpt5->Divide(gjetpt_1_sumpt5);
 gtaupt_1_sumpt5->SetMarkerStyle(20);


/* TCanvas *c2 =new TCanvas("c2","c2",0,0,1200,800);
 c2->SetLogy();
 taupt_1->GetYaxis()->SetRangeUser(0.00001,0.5);
 taupt_1->Draw("EP");
 ltaupt_1->Draw("EPSAME");
 btaupt_1->Draw("EPSAME");
 gtaupt_1->Draw("EPSAME");
 */
 TFile *outfile = new TFile("Jan29_wjets_fakerates_sumpt.root","RECREATE");
 taupt_1_sumpt1->Write();
 ltaupt_1_sumpt1->Write();
 btaupt_1_sumpt1->Write();
 gtaupt_1_sumpt1->Write();
 taupt_1_sumpt2->Write();
 ltaupt_1_sumpt2->Write();
 btaupt_1_sumpt2->Write();
 gtaupt_1_sumpt2->Write();
  taupt_1_sumpt3->Write();
 ltaupt_1_sumpt3->Write();
 btaupt_1_sumpt3->Write();
 gtaupt_1_sumpt3->Write();
 taupt_1_sumpt4->Write();
 ltaupt_1_sumpt4->Write();
 btaupt_1_sumpt4->Write();
 gtaupt_1_sumpt4->Write();
 taupt_1_sumpt5->Write();
 ltaupt_1_sumpt5->Write();
 btaupt_1_sumpt5->Write();
 gtaupt_1_sumpt5->Write();





outfile->Close();


}
