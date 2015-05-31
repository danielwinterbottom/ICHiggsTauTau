void drawJetTauFakeRateWJetsLinRadMom(){
 TFile *qcd_1 = TFile::Open("Jan29_WJets.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"0.5","1","1.5","2"};
TH1F* hist1_jetlinradmom1 = new TH1F("hist1_jetlinradmom1","hist1_jetlinradmom1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom1",vars);

 TH1F* jetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("hist1_jetlinradmom1");

 jetpt_1_jetlinradmom1->Sumw2();



TH1F* hist11_jetlinradmom1 = new TH1F("hist11_jetlinradmom1","hist11_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>0&&jetlinradmom<"+limits[0];

 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom1",vars);

 TH1F *taupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("hist11_jetlinradmom1");

 taupt_1_jetlinradmom1->Sumw2();


 

 taupt_1_jetlinradmom1->Divide(jetpt_1_jetlinradmom1);
 taupt_1_jetlinradmom1->SetMarkerStyle(20);
 taupt_1_jetlinradmom1->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom1 = new TH1F("lhist1_jetlinradmom1","lhist1_jetlinradmom1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom1",vars);

 TH1F* ljetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom1");

 ljetpt_1_jetlinradmom1->Sumw2();


TH1F* lhist11_jetlinradmom1 = new TH1F("lhist11_jetlinradmom1","lhist11_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom1",vars);

 TH1F *ltaupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom1");

 ltaupt_1_jetlinradmom1->Sumw2();



 ltaupt_1_jetlinradmom1->Divide(ljetpt_1_jetlinradmom1);
 ltaupt_1_jetlinradmom1->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom1->SetMarkerColor(kGreen+3);


TH1F* bhist1_jetlinradmom1 = new TH1F("bhist1_jetlinradmom1","bhist1_jetlinradmom1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom1",vars);

 TH1F* bjetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom1");

 bjetpt_1_jetlinradmom1->Sumw2();


TH1F* bhist11_jetlinradmom1 = new TH1F("bhist11_jetlinradmom1","bhist11_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom1",vars);

 TH1F *btaupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom1");

 btaupt_1_jetlinradmom1->Sumw2();


 btaupt_1_jetlinradmom1->Divide(bjetpt_1_jetlinradmom1);
 btaupt_1_jetlinradmom1->SetMarkerStyle(20);
 btaupt_1_jetlinradmom1->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom1 = new TH1F("ghist1_jetlinradmom1","ghist1_jetlinradmom1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom1",vars);

 TH1F* gjetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom1");

 gjetpt_1_jetlinradmom1->Sumw2();

TH1F* ghist11_jetlinradmom1 = new TH1F("ghist11_jetlinradmom1","ghist11_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom1",vars);

 TH1F *gtaupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom1");

 gtaupt_1_jetlinradmom1->Sumw2();


 gtaupt_1_jetlinradmom1->Divide(gjetpt_1_jetlinradmom1);
 gtaupt_1_jetlinradmom1->SetMarkerStyle(20);

TH1F* hist1_jetlinradmom2 = new TH1F("hist1_jetlinradmom2","hist1_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom2",vars);

 TH1F* jetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("hist1_jetlinradmom2");

 jetpt_1_jetlinradmom2->Sumw2();




TH1F* hist11_jetlinradmom2 = new TH1F("hist11_jetlinradmom2","hist11_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom2",vars);

 TH1F *taupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("hist11_jetlinradmom2");

 taupt_1_jetlinradmom2->Sumw2();



 taupt_1_jetlinradmom2->Divide(jetpt_1_jetlinradmom2);
 taupt_1_jetlinradmom2->SetMarkerStyle(20);
 taupt_1_jetlinradmom2->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom2 = new TH1F("lhist1_jetlinradmom2","lhist1_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom2",vars);

 TH1F* ljetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom2");

 ljetpt_1_jetlinradmom2->Sumw2();



TH1F* lhist11_jetlinradmom2 = new TH1F("lhist11_jetlinradmom2","lhist11_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom2",vars);

 TH1F *ltaupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom2");

 ltaupt_1_jetlinradmom2->Sumw2();


 ltaupt_1_jetlinradmom2->Divide(ljetpt_1_jetlinradmom2);
 ltaupt_1_jetlinradmom2->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom2->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom2 = new TH1F("bhist1_jetlinradmom2","bhist1_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom2",vars);

 TH1F* bjetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom2");

 bjetpt_1_jetlinradmom2->Sumw2();



TH1F* bhist11_jetlinradmom2 = new TH1F("bhist11_jetlinradmom2","bhist11_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom2",vars);

 TH1F *btaupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom2");

 btaupt_1_jetlinradmom2->Sumw2();



 btaupt_1_jetlinradmom2->Divide(bjetpt_1_jetlinradmom2);
 btaupt_1_jetlinradmom2->SetMarkerStyle(20);
 btaupt_1_jetlinradmom2->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom2 = new TH1F("ghist1_jetlinradmom2","ghist1_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom2",vars);

 TH1F* gjetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom2");

 gjetpt_1_jetlinradmom2->Sumw2();



TH1F* ghist11_jetlinradmom2 = new TH1F("ghist11_jetlinradmom2","ghist11_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom2",vars);

 TH1F *gtaupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom2");

 gtaupt_1_jetlinradmom2->Sumw2();



 gtaupt_1_jetlinradmom2->Divide(gjetpt_1_jetlinradmom2);
 gtaupt_1_jetlinradmom2->SetMarkerStyle(20);

TH1F* hist1_jetlinradmom3 = new TH1F("hist1_jetlinradmom3","hist1_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom3",vars);

 TH1F* jetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("hist1_jetlinradmom3");

 jetpt_1_jetlinradmom3->Sumw2();


TH1F* hist11_jetlinradmom3 = new TH1F("hist11_jetlinradmom3","hist11_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom3",vars);

 TH1F *taupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("hist11_jetlinradmom3");

 taupt_1_jetlinradmom3->Sumw2();





 taupt_1_jetlinradmom3->Divide(jetpt_1_jetlinradmom3);
 taupt_1_jetlinradmom3->SetMarkerStyle(20);
 taupt_1_jetlinradmom3->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom3 = new TH1F("lhist1_jetlinradmom3","lhist1_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom3",vars);

 TH1F* ljetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom3");

 ljetpt_1_jetlinradmom3->Sumw2();


TH1F* lhist11_jetlinradmom3 = new TH1F("lhist11_jetlinradmom3","lhist11_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom3",vars);

 TH1F *ltaupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom3");

 ltaupt_1_jetlinradmom3->Sumw2();



 ltaupt_1_jetlinradmom3->Divide(ljetpt_1_jetlinradmom3);
 ltaupt_1_jetlinradmom3->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom3->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom3 = new TH1F("bhist1_jetlinradmom3","bhist1_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom3",vars);

 TH1F* bjetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom3");

 bjetpt_1_jetlinradmom3->Sumw2();



TH1F* bhist11_jetlinradmom3 = new TH1F("bhist11_jetlinradmom3","bhist11_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom3",vars);

 TH1F *btaupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom3");

 btaupt_1_jetlinradmom3->Sumw2();



 btaupt_1_jetlinradmom3->Divide(bjetpt_1_jetlinradmom3);
 btaupt_1_jetlinradmom3->SetMarkerStyle(20);
 btaupt_1_jetlinradmom3->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom3 = new TH1F("ghist1_jetlinradmom3","ghist1_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom3",vars);

 TH1F* gjetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom3");

 gjetpt_1_jetlinradmom3->Sumw2();




TH1F* ghist11_jetlinradmom3 = new TH1F("ghist11_jetlinradmom3","ghist11_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom3",vars);

 TH1F *gtaupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom3");

 gtaupt_1_jetlinradmom3->Sumw2();





 gtaupt_1_jetlinradmom3->Divide(gjetpt_1_jetlinradmom3);
 gtaupt_1_jetlinradmom3->SetMarkerStyle(20);



TH1F* hist1_jetlinradmom4 = new TH1F("hist1_jetlinradmom4","hist1_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom4",vars);

 TH1F* jetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("hist1_jetlinradmom4");

 jetpt_1_jetlinradmom4->Sumw2();



TH1F* hist11_jetlinradmom4 = new TH1F("hist11_jetlinradmom4","hist11_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom4",vars);

 TH1F *taupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("hist11_jetlinradmom4");

 taupt_1_jetlinradmom4->Sumw2();



 taupt_1_jetlinradmom4->Divide(jetpt_1_jetlinradmom4);
 taupt_1_jetlinradmom4->SetMarkerStyle(20);
 taupt_1_jetlinradmom4->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom4 = new TH1F("lhist1_jetlinradmom4","lhist1_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom4",vars);

 TH1F* ljetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom4");

 ljetpt_1_jetlinradmom4->Sumw2();



TH1F* lhist11_jetlinradmom4 = new TH1F("lhist11_jetlinradmom4","lhist11_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom4",vars);

 TH1F *ltaupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom4");

 ltaupt_1_jetlinradmom4->Sumw2();



 ltaupt_1_jetlinradmom4->Divide(ljetpt_1_jetlinradmom4);
 ltaupt_1_jetlinradmom4->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom4->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom4 = new TH1F("bhist1_jetlinradmom4","bhist1_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom4",vars);

 TH1F* bjetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom4");

 bjetpt_1_jetlinradmom4->Sumw2();


TH1F* bhist11_jetlinradmom4 = new TH1F("bhist11_jetlinradmom4","bhist11_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom4",vars);

 TH1F *btaupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom4");

 btaupt_1_jetlinradmom4->Sumw2();


 btaupt_1_jetlinradmom4->Divide(bjetpt_1_jetlinradmom4);
 btaupt_1_jetlinradmom4->SetMarkerStyle(20);
 btaupt_1_jetlinradmom4->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom4 = new TH1F("ghist1_jetlinradmom4","ghist1_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom4",vars);

 TH1F* gjetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom4");

 gjetpt_1_jetlinradmom4->Sumw2();




TH1F* ghist11_jetlinradmom4 = new TH1F("ghist11_jetlinradmom4","ghist11_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom4",vars);

 TH1F *gtaupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom4");

 gtaupt_1_jetlinradmom4->Sumw2();


 gtaupt_1_jetlinradmom4->Divide(gjetpt_1_jetlinradmom4);
 gtaupt_1_jetlinradmom4->SetMarkerStyle(20);

TH1F* hist1_jetlinradmom5 = new TH1F("hist1_jetlinradmom5","hist1_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom5",vars);

 TH1F* jetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("hist1_jetlinradmom5");

 jetpt_1_jetlinradmom5->Sumw2();



TH1F* hist11_jetlinradmom5 = new TH1F("hist11_jetlinradmom5","hist11_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom5",vars);

 TH1F *taupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("hist11_jetlinradmom5");

 taupt_1_jetlinradmom5->Sumw2();

 taupt_1_jetlinradmom5->Divide(jetpt_1_jetlinradmom5);
 taupt_1_jetlinradmom5->SetMarkerStyle(20);
 taupt_1_jetlinradmom5->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom5 = new TH1F("lhist1_jetlinradmom5","lhist1_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom5",vars);

 TH1F* ljetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom5");

 ljetpt_1_jetlinradmom5->Sumw2();


TH1F* lhist11_jetlinradmom5 = new TH1F("lhist11_jetlinradmom5","lhist11_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom5",vars);

 TH1F *ltaupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom5");

 ltaupt_1_jetlinradmom5->Sumw2();



 ltaupt_1_jetlinradmom5->Divide(ljetpt_1_jetlinradmom5);
 ltaupt_1_jetlinradmom5->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom5->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom5 = new TH1F("bhist1_jetlinradmom5","bhist1_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom5",vars);

 TH1F* bjetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom5");

 bjetpt_1_jetlinradmom5->Sumw2();


TH1F* bhist11_jetlinradmom5 = new TH1F("bhist11_jetlinradmom5","bhist11_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom5",vars);

 TH1F *btaupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom5");

 btaupt_1_jetlinradmom5->Sumw2();



 btaupt_1_jetlinradmom5->Divide(bjetpt_1_jetlinradmom5);
 btaupt_1_jetlinradmom5->SetMarkerStyle(20);
 btaupt_1_jetlinradmom5->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom5 = new TH1F("ghist1_jetlinradmom5","ghist1_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom5",vars);

 TH1F* gjetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom5");

 gjetpt_1_jetlinradmom5->Sumw2();



TH1F* ghist11_jetlinradmom5 = new TH1F("ghist11_jetlinradmom5","ghist11_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom5",vars);

 TH1F *gtaupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom5");

 gtaupt_1_jetlinradmom5->Sumw2();





 gtaupt_1_jetlinradmom5->Divide(gjetpt_1_jetlinradmom5);
 gtaupt_1_jetlinradmom5->SetMarkerStyle(20);


/* TCanvas *c2 =new TCanvas("c2","c2",0,0,1200,800);
 c2->SetLogy();
 taupt_1->GetYaxis()->SetRangeUser(0.00001,0.5);
 taupt_1->Draw("EP");
 ltaupt_1->Draw("EPSAME");
 btaupt_1->Draw("EPSAME");
 gtaupt_1->Draw("EPSAME");
 */
 TFile *outfile = new TFile("Jan29_wjets_fakerates_jetlinradmom.root","RECREATE");
 taupt_1_jetlinradmom1->Write();
 ltaupt_1_jetlinradmom1->Write();
 btaupt_1_jetlinradmom1->Write();
 gtaupt_1_jetlinradmom1->Write();
 taupt_1_jetlinradmom2->Write();
 ltaupt_1_jetlinradmom2->Write();
 btaupt_1_jetlinradmom2->Write();
 gtaupt_1_jetlinradmom2->Write();
  taupt_1_jetlinradmom3->Write();
 ltaupt_1_jetlinradmom3->Write();
 btaupt_1_jetlinradmom3->Write();
 gtaupt_1_jetlinradmom3->Write();
 taupt_1_jetlinradmom4->Write();
 ltaupt_1_jetlinradmom4->Write();
 btaupt_1_jetlinradmom4->Write();
 gtaupt_1_jetlinradmom4->Write();
 taupt_1_jetlinradmom5->Write();
 ltaupt_1_jetlinradmom5->Write();
 btaupt_1_jetlinradmom5->Write();
 gtaupt_1_jetlinradmom5->Write();





outfile->Close();


}
