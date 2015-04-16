void drawJetTauFakeRateWJetsNtracks(){
 TFile *qcd_1 = TFile::Open("Jan29_WJets.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"15","25","35","45"};
TH1F* hist1_ntracks1 = new TH1F("hist1_ntracks1","hist1_ntracks1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks1",vars);

 TH1F* jetpt_1_ntracks1 = (TH1F*)gDirectory->Get("hist1_ntracks1");

 jetpt_1_ntracks1->Sumw2();



TH1F* hist11_ntracks1 = new TH1F("hist11_ntracks1","hist11_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>0&&ntracks<"+limits[0];

 qcd_tree_1->Draw("jetpt>>hist11_ntracks1",vars);

 TH1F *taupt_1_ntracks1 = (TH1F*)gDirectory->Get("hist11_ntracks1");

 taupt_1_ntracks1->Sumw2();


 

 taupt_1_ntracks1->Divide(jetpt_1_ntracks1);
 taupt_1_ntracks1->SetMarkerStyle(20);
 taupt_1_ntracks1->SetMarkerColor(kRed);


TH1F* lhist1_ntracks1 = new TH1F("lhist1_ntracks1","lhist1_ntracks1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks1",vars);

 TH1F* ljetpt_1_ntracks1 = (TH1F*)gDirectory->Get("lhist1_ntracks1");

 ljetpt_1_ntracks1->Sumw2();


TH1F* lhist11_ntracks1 = new TH1F("lhist11_ntracks1","lhist11_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks1",vars);

 TH1F *ltaupt_1_ntracks1 = (TH1F*)gDirectory->Get("lhist11_ntracks1");

 ltaupt_1_ntracks1->Sumw2();



 ltaupt_1_ntracks1->Divide(ljetpt_1_ntracks1);
 ltaupt_1_ntracks1->SetMarkerStyle(20);
 ltaupt_1_ntracks1->SetMarkerColor(kGreen+3);


TH1F* bhist1_ntracks1 = new TH1F("bhist1_ntracks1","bhist1_ntracks1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks1",vars);

 TH1F* bjetpt_1_ntracks1 = (TH1F*)gDirectory->Get("bhist1_ntracks1");

 bjetpt_1_ntracks1->Sumw2();


TH1F* bhist11_ntracks1 = new TH1F("bhist11_ntracks1","bhist11_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks1",vars);

 TH1F *btaupt_1_ntracks1 = (TH1F*)gDirectory->Get("bhist11_ntracks1");

 btaupt_1_ntracks1->Sumw2();


 btaupt_1_ntracks1->Divide(bjetpt_1_ntracks1);
 btaupt_1_ntracks1->SetMarkerStyle(20);
 btaupt_1_ntracks1->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks1 = new TH1F("ghist1_ntracks1","ghist1_ntracks1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks1",vars);

 TH1F* gjetpt_1_ntracks1 = (TH1F*)gDirectory->Get("ghist1_ntracks1");

 gjetpt_1_ntracks1->Sumw2();

TH1F* ghist11_ntracks1 = new TH1F("ghist11_ntracks1","ghist11_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks1",vars);

 TH1F *gtaupt_1_ntracks1 = (TH1F*)gDirectory->Get("ghist11_ntracks1");

 gtaupt_1_ntracks1->Sumw2();


 gtaupt_1_ntracks1->Divide(gjetpt_1_ntracks1);
 gtaupt_1_ntracks1->SetMarkerStyle(20);

TH1F* hist1_ntracks2 = new TH1F("hist1_ntracks2","hist1_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks2",vars);

 TH1F* jetpt_1_ntracks2 = (TH1F*)gDirectory->Get("hist1_ntracks2");

 jetpt_1_ntracks2->Sumw2();




TH1F* hist11_ntracks2 = new TH1F("hist11_ntracks2","hist11_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks2",vars);

 TH1F *taupt_1_ntracks2 = (TH1F*)gDirectory->Get("hist11_ntracks2");

 taupt_1_ntracks2->Sumw2();



 taupt_1_ntracks2->Divide(jetpt_1_ntracks2);
 taupt_1_ntracks2->SetMarkerStyle(20);
 taupt_1_ntracks2->SetMarkerColor(kRed);


TH1F* lhist1_ntracks2 = new TH1F("lhist1_ntracks2","lhist1_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks2",vars);

 TH1F* ljetpt_1_ntracks2 = (TH1F*)gDirectory->Get("lhist1_ntracks2");

 ljetpt_1_ntracks2->Sumw2();



TH1F* lhist11_ntracks2 = new TH1F("lhist11_ntracks2","lhist11_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks2",vars);

 TH1F *ltaupt_1_ntracks2 = (TH1F*)gDirectory->Get("lhist11_ntracks2");

 ltaupt_1_ntracks2->Sumw2();


 ltaupt_1_ntracks2->Divide(ljetpt_1_ntracks2);
 ltaupt_1_ntracks2->SetMarkerStyle(20);
 ltaupt_1_ntracks2->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks2 = new TH1F("bhist1_ntracks2","bhist1_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks2",vars);

 TH1F* bjetpt_1_ntracks2 = (TH1F*)gDirectory->Get("bhist1_ntracks2");

 bjetpt_1_ntracks2->Sumw2();



TH1F* bhist11_ntracks2 = new TH1F("bhist11_ntracks2","bhist11_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks2",vars);

 TH1F *btaupt_1_ntracks2 = (TH1F*)gDirectory->Get("bhist11_ntracks2");

 btaupt_1_ntracks2->Sumw2();



 btaupt_1_ntracks2->Divide(bjetpt_1_ntracks2);
 btaupt_1_ntracks2->SetMarkerStyle(20);
 btaupt_1_ntracks2->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks2 = new TH1F("ghist1_ntracks2","ghist1_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks2",vars);

 TH1F* gjetpt_1_ntracks2 = (TH1F*)gDirectory->Get("ghist1_ntracks2");

 gjetpt_1_ntracks2->Sumw2();



TH1F* ghist11_ntracks2 = new TH1F("ghist11_ntracks2","ghist11_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks2",vars);

 TH1F *gtaupt_1_ntracks2 = (TH1F*)gDirectory->Get("ghist11_ntracks2");

 gtaupt_1_ntracks2->Sumw2();



 gtaupt_1_ntracks2->Divide(gjetpt_1_ntracks2);
 gtaupt_1_ntracks2->SetMarkerStyle(20);

TH1F* hist1_ntracks3 = new TH1F("hist1_ntracks3","hist1_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks3",vars);

 TH1F* jetpt_1_ntracks3 = (TH1F*)gDirectory->Get("hist1_ntracks3");

 jetpt_1_ntracks3->Sumw2();


TH1F* hist11_ntracks3 = new TH1F("hist11_ntracks3","hist11_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks3",vars);

 TH1F *taupt_1_ntracks3 = (TH1F*)gDirectory->Get("hist11_ntracks3");

 taupt_1_ntracks3->Sumw2();





 taupt_1_ntracks3->Divide(jetpt_1_ntracks3);
 taupt_1_ntracks3->SetMarkerStyle(20);
 taupt_1_ntracks3->SetMarkerColor(kRed);


TH1F* lhist1_ntracks3 = new TH1F("lhist1_ntracks3","lhist1_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks3",vars);

 TH1F* ljetpt_1_ntracks3 = (TH1F*)gDirectory->Get("lhist1_ntracks3");

 ljetpt_1_ntracks3->Sumw2();


TH1F* lhist11_ntracks3 = new TH1F("lhist11_ntracks3","lhist11_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks3",vars);

 TH1F *ltaupt_1_ntracks3 = (TH1F*)gDirectory->Get("lhist11_ntracks3");

 ltaupt_1_ntracks3->Sumw2();



 ltaupt_1_ntracks3->Divide(ljetpt_1_ntracks3);
 ltaupt_1_ntracks3->SetMarkerStyle(20);
 ltaupt_1_ntracks3->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks3 = new TH1F("bhist1_ntracks3","bhist1_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks3",vars);

 TH1F* bjetpt_1_ntracks3 = (TH1F*)gDirectory->Get("bhist1_ntracks3");

 bjetpt_1_ntracks3->Sumw2();



TH1F* bhist11_ntracks3 = new TH1F("bhist11_ntracks3","bhist11_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks3",vars);

 TH1F *btaupt_1_ntracks3 = (TH1F*)gDirectory->Get("bhist11_ntracks3");

 btaupt_1_ntracks3->Sumw2();



 btaupt_1_ntracks3->Divide(bjetpt_1_ntracks3);
 btaupt_1_ntracks3->SetMarkerStyle(20);
 btaupt_1_ntracks3->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks3 = new TH1F("ghist1_ntracks3","ghist1_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks3",vars);

 TH1F* gjetpt_1_ntracks3 = (TH1F*)gDirectory->Get("ghist1_ntracks3");

 gjetpt_1_ntracks3->Sumw2();




TH1F* ghist11_ntracks3 = new TH1F("ghist11_ntracks3","ghist11_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks3",vars);

 TH1F *gtaupt_1_ntracks3 = (TH1F*)gDirectory->Get("ghist11_ntracks3");

 gtaupt_1_ntracks3->Sumw2();





 gtaupt_1_ntracks3->Divide(gjetpt_1_ntracks3);
 gtaupt_1_ntracks3->SetMarkerStyle(20);



TH1F* hist1_ntracks4 = new TH1F("hist1_ntracks4","hist1_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks4",vars);

 TH1F* jetpt_1_ntracks4 = (TH1F*)gDirectory->Get("hist1_ntracks4");

 jetpt_1_ntracks4->Sumw2();



TH1F* hist11_ntracks4 = new TH1F("hist11_ntracks4","hist11_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks4",vars);

 TH1F *taupt_1_ntracks4 = (TH1F*)gDirectory->Get("hist11_ntracks4");

 taupt_1_ntracks4->Sumw2();



 taupt_1_ntracks4->Divide(jetpt_1_ntracks4);
 taupt_1_ntracks4->SetMarkerStyle(20);
 taupt_1_ntracks4->SetMarkerColor(kRed);


TH1F* lhist1_ntracks4 = new TH1F("lhist1_ntracks4","lhist1_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks4",vars);

 TH1F* ljetpt_1_ntracks4 = (TH1F*)gDirectory->Get("lhist1_ntracks4");

 ljetpt_1_ntracks4->Sumw2();



TH1F* lhist11_ntracks4 = new TH1F("lhist11_ntracks4","lhist11_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks4",vars);

 TH1F *ltaupt_1_ntracks4 = (TH1F*)gDirectory->Get("lhist11_ntracks4");

 ltaupt_1_ntracks4->Sumw2();



 ltaupt_1_ntracks4->Divide(ljetpt_1_ntracks4);
 ltaupt_1_ntracks4->SetMarkerStyle(20);
 ltaupt_1_ntracks4->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks4 = new TH1F("bhist1_ntracks4","bhist1_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks4",vars);

 TH1F* bjetpt_1_ntracks4 = (TH1F*)gDirectory->Get("bhist1_ntracks4");

 bjetpt_1_ntracks4->Sumw2();


TH1F* bhist11_ntracks4 = new TH1F("bhist11_ntracks4","bhist11_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks4",vars);

 TH1F *btaupt_1_ntracks4 = (TH1F*)gDirectory->Get("bhist11_ntracks4");

 btaupt_1_ntracks4->Sumw2();


 btaupt_1_ntracks4->Divide(bjetpt_1_ntracks4);
 btaupt_1_ntracks4->SetMarkerStyle(20);
 btaupt_1_ntracks4->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks4 = new TH1F("ghist1_ntracks4","ghist1_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks4",vars);

 TH1F* gjetpt_1_ntracks4 = (TH1F*)gDirectory->Get("ghist1_ntracks4");

 gjetpt_1_ntracks4->Sumw2();




TH1F* ghist11_ntracks4 = new TH1F("ghist11_ntracks4","ghist11_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks4",vars);

 TH1F *gtaupt_1_ntracks4 = (TH1F*)gDirectory->Get("ghist11_ntracks4");

 gtaupt_1_ntracks4->Sumw2();


 gtaupt_1_ntracks4->Divide(gjetpt_1_ntracks4);
 gtaupt_1_ntracks4->SetMarkerStyle(20);

TH1F* hist1_ntracks5 = new TH1F("hist1_ntracks5","hist1_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks5",vars);

 TH1F* jetpt_1_ntracks5 = (TH1F*)gDirectory->Get("hist1_ntracks5");

 jetpt_1_ntracks5->Sumw2();



TH1F* hist11_ntracks5 = new TH1F("hist11_ntracks5","hist11_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks5",vars);

 TH1F *taupt_1_ntracks5 = (TH1F*)gDirectory->Get("hist11_ntracks5");

 taupt_1_ntracks5->Sumw2();

 taupt_1_ntracks5->Divide(jetpt_1_ntracks5);
 taupt_1_ntracks5->SetMarkerStyle(20);
 taupt_1_ntracks5->SetMarkerColor(kRed);


TH1F* lhist1_ntracks5 = new TH1F("lhist1_ntracks5","lhist1_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks5",vars);

 TH1F* ljetpt_1_ntracks5 = (TH1F*)gDirectory->Get("lhist1_ntracks5");

 ljetpt_1_ntracks5->Sumw2();


TH1F* lhist11_ntracks5 = new TH1F("lhist11_ntracks5","lhist11_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks5",vars);

 TH1F *ltaupt_1_ntracks5 = (TH1F*)gDirectory->Get("lhist11_ntracks5");

 ltaupt_1_ntracks5->Sumw2();



 ltaupt_1_ntracks5->Divide(ljetpt_1_ntracks5);
 ltaupt_1_ntracks5->SetMarkerStyle(20);
 ltaupt_1_ntracks5->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks5 = new TH1F("bhist1_ntracks5","bhist1_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks5",vars);

 TH1F* bjetpt_1_ntracks5 = (TH1F*)gDirectory->Get("bhist1_ntracks5");

 bjetpt_1_ntracks5->Sumw2();


TH1F* bhist11_ntracks5 = new TH1F("bhist11_ntracks5","bhist11_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks5",vars);

 TH1F *btaupt_1_ntracks5 = (TH1F*)gDirectory->Get("bhist11_ntracks5");

 btaupt_1_ntracks5->Sumw2();



 btaupt_1_ntracks5->Divide(bjetpt_1_ntracks5);
 btaupt_1_ntracks5->SetMarkerStyle(20);
 btaupt_1_ntracks5->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks5 = new TH1F("ghist1_ntracks5","ghist1_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks5",vars);

 TH1F* gjetpt_1_ntracks5 = (TH1F*)gDirectory->Get("ghist1_ntracks5");

 gjetpt_1_ntracks5->Sumw2();



TH1F* ghist11_ntracks5 = new TH1F("ghist11_ntracks5","ghist11_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks5",vars);

 TH1F *gtaupt_1_ntracks5 = (TH1F*)gDirectory->Get("ghist11_ntracks5");

 gtaupt_1_ntracks5->Sumw2();





 gtaupt_1_ntracks5->Divide(gjetpt_1_ntracks5);
 gtaupt_1_ntracks5->SetMarkerStyle(20);


/* TCanvas *c2 =new TCanvas("c2","c2",0,0,1200,800);
 c2->SetLogy();
 taupt_1->GetYaxis()->SetRangeUser(0.00001,0.5);
 taupt_1->Draw("EP");
 ltaupt_1->Draw("EPSAME");
 btaupt_1->Draw("EPSAME");
 gtaupt_1->Draw("EPSAME");
 */
 TFile *outfile = new TFile("Jan29_wjets_fakerates_ntracks_down.root","RECREATE");
 taupt_1_ntracks1->Write();
 ltaupt_1_ntracks1->Write();
 btaupt_1_ntracks1->Write();
 gtaupt_1_ntracks1->Write();
 taupt_1_ntracks2->Write();
 ltaupt_1_ntracks2->Write();
 btaupt_1_ntracks2->Write();
 gtaupt_1_ntracks2->Write();
  taupt_1_ntracks3->Write();
 ltaupt_1_ntracks3->Write();
 btaupt_1_ntracks3->Write();
 gtaupt_1_ntracks3->Write();
 taupt_1_ntracks4->Write();
 ltaupt_1_ntracks4->Write();
 btaupt_1_ntracks4->Write();
 gtaupt_1_ntracks4->Write();
 taupt_1_ntracks5->Write();
 ltaupt_1_ntracks5->Write();
 btaupt_1_ntracks5->Write();
 gtaupt_1_ntracks5->Write();





outfile->Close();


}
