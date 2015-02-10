void drawJetTauFakeRateWJetsSumEt(){
 TFile *qcd_1 = TFile::Open("Jan29_WJets.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"500","1000","1500","2000"};
TH1F* hist1_sumet1 = new TH1F("hist1_sumet1","hist1_sumet1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_sumet1",vars);

 TH1F* jetpt_1_sumet1 = (TH1F*)gDirectory->Get("hist1_sumet1");

 jetpt_1_sumet1->Sumw2();



TH1F* hist11_sumet1 = new TH1F("hist11_sumet1","hist11_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>0&&sumet<"+limits[0];

 qcd_tree_1->Draw("jetpt>>hist11_sumet1",vars);

 TH1F *taupt_1_sumet1 = (TH1F*)gDirectory->Get("hist11_sumet1");

 taupt_1_sumet1->Sumw2();


 

 taupt_1_sumet1->Divide(jetpt_1_sumet1);
 taupt_1_sumet1->SetMarkerStyle(20);
 taupt_1_sumet1->SetMarkerColor(kRed);


TH1F* lhist1_sumet1 = new TH1F("lhist1_sumet1","lhist1_sumet1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet1",vars);

 TH1F* ljetpt_1_sumet1 = (TH1F*)gDirectory->Get("lhist1_sumet1");

 ljetpt_1_sumet1->Sumw2();


TH1F* lhist11_sumet1 = new TH1F("lhist11_sumet1","lhist11_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet1",vars);

 TH1F *ltaupt_1_sumet1 = (TH1F*)gDirectory->Get("lhist11_sumet1");

 ltaupt_1_sumet1->Sumw2();



 ltaupt_1_sumet1->Divide(ljetpt_1_sumet1);
 ltaupt_1_sumet1->SetMarkerStyle(20);
 ltaupt_1_sumet1->SetMarkerColor(kGreen+3);


TH1F* bhist1_sumet1 = new TH1F("bhist1_sumet1","bhist1_sumet1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet1",vars);

 TH1F* bjetpt_1_sumet1 = (TH1F*)gDirectory->Get("bhist1_sumet1");

 bjetpt_1_sumet1->Sumw2();


TH1F* bhist11_sumet1 = new TH1F("bhist11_sumet1","bhist11_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet1",vars);

 TH1F *btaupt_1_sumet1 = (TH1F*)gDirectory->Get("bhist11_sumet1");

 btaupt_1_sumet1->Sumw2();


 btaupt_1_sumet1->Divide(bjetpt_1_sumet1);
 btaupt_1_sumet1->SetMarkerStyle(20);
 btaupt_1_sumet1->SetMarkerColor(kBlue);


TH1F* ghist1_sumet1 = new TH1F("ghist1_sumet1","ghist1_sumet1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet1",vars);

 TH1F* gjetpt_1_sumet1 = (TH1F*)gDirectory->Get("ghist1_sumet1");

 gjetpt_1_sumet1->Sumw2();

TH1F* ghist11_sumet1 = new TH1F("ghist11_sumet1","ghist11_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet1",vars);

 TH1F *gtaupt_1_sumet1 = (TH1F*)gDirectory->Get("ghist11_sumet1");

 gtaupt_1_sumet1->Sumw2();


 gtaupt_1_sumet1->Divide(gjetpt_1_sumet1);
 gtaupt_1_sumet1->SetMarkerStyle(20);

TH1F* hist1_sumet2 = new TH1F("hist1_sumet2","hist1_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_sumet2",vars);

 TH1F* jetpt_1_sumet2 = (TH1F*)gDirectory->Get("hist1_sumet2");

 jetpt_1_sumet2->Sumw2();




TH1F* hist11_sumet2 = new TH1F("hist11_sumet2","hist11_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_sumet2",vars);

 TH1F *taupt_1_sumet2 = (TH1F*)gDirectory->Get("hist11_sumet2");

 taupt_1_sumet2->Sumw2();



 taupt_1_sumet2->Divide(jetpt_1_sumet2);
 taupt_1_sumet2->SetMarkerStyle(20);
 taupt_1_sumet2->SetMarkerColor(kRed);


TH1F* lhist1_sumet2 = new TH1F("lhist1_sumet2","lhist1_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet2",vars);

 TH1F* ljetpt_1_sumet2 = (TH1F*)gDirectory->Get("lhist1_sumet2");

 ljetpt_1_sumet2->Sumw2();



TH1F* lhist11_sumet2 = new TH1F("lhist11_sumet2","lhist11_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet2",vars);

 TH1F *ltaupt_1_sumet2 = (TH1F*)gDirectory->Get("lhist11_sumet2");

 ltaupt_1_sumet2->Sumw2();


 ltaupt_1_sumet2->Divide(ljetpt_1_sumet2);
 ltaupt_1_sumet2->SetMarkerStyle(20);
 ltaupt_1_sumet2->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet2 = new TH1F("bhist1_sumet2","bhist1_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet2",vars);

 TH1F* bjetpt_1_sumet2 = (TH1F*)gDirectory->Get("bhist1_sumet2");

 bjetpt_1_sumet2->Sumw2();



TH1F* bhist11_sumet2 = new TH1F("bhist11_sumet2","bhist11_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet2",vars);

 TH1F *btaupt_1_sumet2 = (TH1F*)gDirectory->Get("bhist11_sumet2");

 btaupt_1_sumet2->Sumw2();



 btaupt_1_sumet2->Divide(bjetpt_1_sumet2);
 btaupt_1_sumet2->SetMarkerStyle(20);
 btaupt_1_sumet2->SetMarkerColor(kBlue);


TH1F* ghist1_sumet2 = new TH1F("ghist1_sumet2","ghist1_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet2",vars);

 TH1F* gjetpt_1_sumet2 = (TH1F*)gDirectory->Get("ghist1_sumet2");

 gjetpt_1_sumet2->Sumw2();



TH1F* ghist11_sumet2 = new TH1F("ghist11_sumet2","ghist11_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet2",vars);

 TH1F *gtaupt_1_sumet2 = (TH1F*)gDirectory->Get("ghist11_sumet2");

 gtaupt_1_sumet2->Sumw2();



 gtaupt_1_sumet2->Divide(gjetpt_1_sumet2);
 gtaupt_1_sumet2->SetMarkerStyle(20);

TH1F* hist1_sumet3 = new TH1F("hist1_sumet3","hist1_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_sumet3",vars);

 TH1F* jetpt_1_sumet3 = (TH1F*)gDirectory->Get("hist1_sumet3");

 jetpt_1_sumet3->Sumw2();


TH1F* hist11_sumet3 = new TH1F("hist11_sumet3","hist11_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_sumet3",vars);

 TH1F *taupt_1_sumet3 = (TH1F*)gDirectory->Get("hist11_sumet3");

 taupt_1_sumet3->Sumw2();





 taupt_1_sumet3->Divide(jetpt_1_sumet3);
 taupt_1_sumet3->SetMarkerStyle(20);
 taupt_1_sumet3->SetMarkerColor(kRed);


TH1F* lhist1_sumet3 = new TH1F("lhist1_sumet3","lhist1_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet3",vars);

 TH1F* ljetpt_1_sumet3 = (TH1F*)gDirectory->Get("lhist1_sumet3");

 ljetpt_1_sumet3->Sumw2();


TH1F* lhist11_sumet3 = new TH1F("lhist11_sumet3","lhist11_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet3",vars);

 TH1F *ltaupt_1_sumet3 = (TH1F*)gDirectory->Get("lhist11_sumet3");

 ltaupt_1_sumet3->Sumw2();



 ltaupt_1_sumet3->Divide(ljetpt_1_sumet3);
 ltaupt_1_sumet3->SetMarkerStyle(20);
 ltaupt_1_sumet3->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet3 = new TH1F("bhist1_sumet3","bhist1_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet3",vars);

 TH1F* bjetpt_1_sumet3 = (TH1F*)gDirectory->Get("bhist1_sumet3");

 bjetpt_1_sumet3->Sumw2();



TH1F* bhist11_sumet3 = new TH1F("bhist11_sumet3","bhist11_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet3",vars);

 TH1F *btaupt_1_sumet3 = (TH1F*)gDirectory->Get("bhist11_sumet3");

 btaupt_1_sumet3->Sumw2();



 btaupt_1_sumet3->Divide(bjetpt_1_sumet3);
 btaupt_1_sumet3->SetMarkerStyle(20);
 btaupt_1_sumet3->SetMarkerColor(kBlue);


TH1F* ghist1_sumet3 = new TH1F("ghist1_sumet3","ghist1_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet3",vars);

 TH1F* gjetpt_1_sumet3 = (TH1F*)gDirectory->Get("ghist1_sumet3");

 gjetpt_1_sumet3->Sumw2();




TH1F* ghist11_sumet3 = new TH1F("ghist11_sumet3","ghist11_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet3",vars);

 TH1F *gtaupt_1_sumet3 = (TH1F*)gDirectory->Get("ghist11_sumet3");

 gtaupt_1_sumet3->Sumw2();





 gtaupt_1_sumet3->Divide(gjetpt_1_sumet3);
 gtaupt_1_sumet3->SetMarkerStyle(20);



TH1F* hist1_sumet4 = new TH1F("hist1_sumet4","hist1_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumet4",vars);

 TH1F* jetpt_1_sumet4 = (TH1F*)gDirectory->Get("hist1_sumet4");

 jetpt_1_sumet4->Sumw2();



TH1F* hist11_sumet4 = new TH1F("hist11_sumet4","hist11_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumet4",vars);

 TH1F *taupt_1_sumet4 = (TH1F*)gDirectory->Get("hist11_sumet4");

 taupt_1_sumet4->Sumw2();



 taupt_1_sumet4->Divide(jetpt_1_sumet4);
 taupt_1_sumet4->SetMarkerStyle(20);
 taupt_1_sumet4->SetMarkerColor(kRed);


TH1F* lhist1_sumet4 = new TH1F("lhist1_sumet4","lhist1_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet4",vars);

 TH1F* ljetpt_1_sumet4 = (TH1F*)gDirectory->Get("lhist1_sumet4");

 ljetpt_1_sumet4->Sumw2();



TH1F* lhist11_sumet4 = new TH1F("lhist11_sumet4","lhist11_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet4",vars);

 TH1F *ltaupt_1_sumet4 = (TH1F*)gDirectory->Get("lhist11_sumet4");

 ltaupt_1_sumet4->Sumw2();



 ltaupt_1_sumet4->Divide(ljetpt_1_sumet4);
 ltaupt_1_sumet4->SetMarkerStyle(20);
 ltaupt_1_sumet4->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet4 = new TH1F("bhist1_sumet4","bhist1_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet4",vars);

 TH1F* bjetpt_1_sumet4 = (TH1F*)gDirectory->Get("bhist1_sumet4");

 bjetpt_1_sumet4->Sumw2();


TH1F* bhist11_sumet4 = new TH1F("bhist11_sumet4","bhist11_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet4",vars);

 TH1F *btaupt_1_sumet4 = (TH1F*)gDirectory->Get("bhist11_sumet4");

 btaupt_1_sumet4->Sumw2();


 btaupt_1_sumet4->Divide(bjetpt_1_sumet4);
 btaupt_1_sumet4->SetMarkerStyle(20);
 btaupt_1_sumet4->SetMarkerColor(kBlue);


TH1F* ghist1_sumet4 = new TH1F("ghist1_sumet4","ghist1_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet4",vars);

 TH1F* gjetpt_1_sumet4 = (TH1F*)gDirectory->Get("ghist1_sumet4");

 gjetpt_1_sumet4->Sumw2();




TH1F* ghist11_sumet4 = new TH1F("ghist11_sumet4","ghist11_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet4",vars);

 TH1F *gtaupt_1_sumet4 = (TH1F*)gDirectory->Get("ghist11_sumet4");

 gtaupt_1_sumet4->Sumw2();


 gtaupt_1_sumet4->Divide(gjetpt_1_sumet4);
 gtaupt_1_sumet4->SetMarkerStyle(20);

TH1F* hist1_sumet5 = new TH1F("hist1_sumet5","hist1_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumet5",vars);

 TH1F* jetpt_1_sumet5 = (TH1F*)gDirectory->Get("hist1_sumet5");

 jetpt_1_sumet5->Sumw2();



TH1F* hist11_sumet5 = new TH1F("hist11_sumet5","hist11_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumet5",vars);

 TH1F *taupt_1_sumet5 = (TH1F*)gDirectory->Get("hist11_sumet5");

 taupt_1_sumet5->Sumw2();

 taupt_1_sumet5->Divide(jetpt_1_sumet5);
 taupt_1_sumet5->SetMarkerStyle(20);
 taupt_1_sumet5->SetMarkerColor(kRed);


TH1F* lhist1_sumet5 = new TH1F("lhist1_sumet5","lhist1_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet5",vars);

 TH1F* ljetpt_1_sumet5 = (TH1F*)gDirectory->Get("lhist1_sumet5");

 ljetpt_1_sumet5->Sumw2();


TH1F* lhist11_sumet5 = new TH1F("lhist11_sumet5","lhist11_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet5",vars);

 TH1F *ltaupt_1_sumet5 = (TH1F*)gDirectory->Get("lhist11_sumet5");

 ltaupt_1_sumet5->Sumw2();



 ltaupt_1_sumet5->Divide(ljetpt_1_sumet5);
 ltaupt_1_sumet5->SetMarkerStyle(20);
 ltaupt_1_sumet5->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet5 = new TH1F("bhist1_sumet5","bhist1_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet5",vars);

 TH1F* bjetpt_1_sumet5 = (TH1F*)gDirectory->Get("bhist1_sumet5");

 bjetpt_1_sumet5->Sumw2();


TH1F* bhist11_sumet5 = new TH1F("bhist11_sumet5","bhist11_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet5",vars);

 TH1F *btaupt_1_sumet5 = (TH1F*)gDirectory->Get("bhist11_sumet5");

 btaupt_1_sumet5->Sumw2();



 btaupt_1_sumet5->Divide(bjetpt_1_sumet5);
 btaupt_1_sumet5->SetMarkerStyle(20);
 btaupt_1_sumet5->SetMarkerColor(kBlue);


TH1F* ghist1_sumet5 = new TH1F("ghist1_sumet5","ghist1_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet5",vars);

 TH1F* gjetpt_1_sumet5 = (TH1F*)gDirectory->Get("ghist1_sumet5");

 gjetpt_1_sumet5->Sumw2();



TH1F* ghist11_sumet5 = new TH1F("ghist11_sumet5","ghist11_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet5",vars);

 TH1F *gtaupt_1_sumet5 = (TH1F*)gDirectory->Get("ghist11_sumet5");

 gtaupt_1_sumet5->Sumw2();





 gtaupt_1_sumet5->Divide(gjetpt_1_sumet5);
 gtaupt_1_sumet5->SetMarkerStyle(20);


/* TCanvas *c2 =new TCanvas("c2","c2",0,0,1200,800);
 c2->SetLogy();
 taupt_1->GetYaxis()->SetRangeUser(0.00001,0.5);
 taupt_1->Draw("EP");
 ltaupt_1->Draw("EPSAME");
 btaupt_1->Draw("EPSAME");
 gtaupt_1->Draw("EPSAME");
 */
 TFile *outfile = new TFile("Jan29_wjets_fakerates_sumet.root","RECREATE");
 taupt_1_sumet1->Write();
 ltaupt_1_sumet1->Write();
 btaupt_1_sumet1->Write();
 gtaupt_1_sumet1->Write();
 taupt_1_sumet2->Write();
 ltaupt_1_sumet2->Write();
 btaupt_1_sumet2->Write();
 gtaupt_1_sumet2->Write();
  taupt_1_sumet3->Write();
 ltaupt_1_sumet3->Write();
 btaupt_1_sumet3->Write();
 gtaupt_1_sumet3->Write();
 taupt_1_sumet4->Write();
 ltaupt_1_sumet4->Write();
 btaupt_1_sumet4->Write();
 gtaupt_1_sumet4->Write();
 taupt_1_sumet5->Write();
 ltaupt_1_sumet5->Write();
 btaupt_1_sumet5->Write();
 gtaupt_1_sumet5->Write();





outfile->Close();


}
