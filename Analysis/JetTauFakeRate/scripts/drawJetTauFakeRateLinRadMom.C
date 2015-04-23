void drawJetTauFakeRateLinRadMom(){
 TFile *qcd_1 = TFile::Open("Jan29_QCD_15-30.root");
 TFile *qcd_2 = TFile::Open("Jan29_QCD_30-50.root");
 TFile *qcd_3 = TFile::Open("Jan29_QCD_50-80.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));
 TTree *qcd_tree_2 = dynamic_cast<TTree*>(qcd_2->Get("ntuple"));
 TTree *qcd_tree_3 = dynamic_cast<TTree*>(qcd_3->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"0.5","1","1.5","2"};
TH1F* hist1_jetlinradmom1 = new TH1F("hist1_jetlinradmom1","hist1_jetlinradmom1",16,binning);
TH1F* hist2_jetlinradmom1 = new TH1F("hist2_jetlinradmom1","hist2_jetlinradmom1",16,binning);
TH1F* hist3_jetlinradmom1 = new TH1F("hist3_jetlinradmom1","hist3_jetlinradmom1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>hist2_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>hist3_jetlinradmom1",vars);

 TH1F* jetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("hist1_jetlinradmom1");
 TH1F* jetpt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("hist2_jetlinradmom1");
 TH1F* jetpt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("hist3_jetlinradmom1");

 jetpt_1_jetlinradmom1->Sumw2();
 jetpt_2_jetlinradmom1->Sumw2();
 jetpt_3_jetlinradmom1->Sumw2();


 jetpt_1_jetlinradmom1->Scale(988.8/9987968);
 jetpt_2_jetlinradmom1->Scale(66.3/6000000);
 jetpt_3_jetlinradmom1->Scale(8.15/5998860);

 jetpt_1_jetlinradmom1->Add(jetpt_2_jetlinradmom1);
 jetpt_1_jetlinradmom1->Add(jetpt_3_jetlinradmom1);



TH1F* hist11_jetlinradmom1 = new TH1F("hist11_jetlinradmom1","hist11_jetlinradmom1",16,binning);
TH1F* hist21_jetlinradmom1 = new TH1F("hist21_jetlinradmom1","hist21_jetlinradmom1",16,binning);
TH1F* hist31_jetlinradmom1 = new TH1F("hist31_jetlinradmom1","hist31_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>hist21_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>hist31_jetlinradmom1",vars);

 TH1F *taupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("hist11_jetlinradmom1");
 TH1F *taupt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("hist21_jetlinradmom1");
 TH1F *taupt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("hist31_jetlinradmom1");

 taupt_1_jetlinradmom1->Sumw2();
 taupt_2_jetlinradmom1->Sumw2();
 taupt_3_jetlinradmom1->Sumw2();


 taupt_1_jetlinradmom1->Scale(988.8/9987968);
 taupt_2_jetlinradmom1->Scale(66.3/6000000);
 taupt_3_jetlinradmom1->Scale(8.15/5998860);

 



 taupt_1_jetlinradmom1->Add(taupt_2_jetlinradmom1);
 taupt_1_jetlinradmom1->Add(taupt_3_jetlinradmom1);
 


 taupt_1_jetlinradmom1->Divide(jetpt_1_jetlinradmom1);
 taupt_1_jetlinradmom1->SetMarkerStyle(20);
 taupt_1_jetlinradmom1->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom1 = new TH1F("lhist1_jetlinradmom1","lhist1_jetlinradmom1",16,binning);
TH1F* lhist2_jetlinradmom1 = new TH1F("lhist2_jetlinradmom1","lhist2_jetlinradmom1",16,binning);
TH1F* lhist3_jetlinradmom1 = new TH1F("lhist3_jetlinradmom1","lhist3_jetlinradmom1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_jetlinradmom1",vars);

 TH1F* ljetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom1");
 TH1F* ljetpt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist2_jetlinradmom1");
 TH1F* ljetpt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist3_jetlinradmom1");

 ljetpt_1_jetlinradmom1->Sumw2();
 ljetpt_2_jetlinradmom1->Sumw2();
 ljetpt_3_jetlinradmom1->Sumw2();


 ljetpt_1_jetlinradmom1->Scale(988.8/9987968);
 ljetpt_2_jetlinradmom1->Scale(66.3/6000000);
 ljetpt_3_jetlinradmom1->Scale(8.15/5998860);

 ljetpt_1_jetlinradmom1->Add(ljetpt_2_jetlinradmom1);
 ljetpt_1_jetlinradmom1->Add(ljetpt_3_jetlinradmom1);



TH1F* lhist11_jetlinradmom1 = new TH1F("lhist11_jetlinradmom1","lhist11_jetlinradmom1",16,binning);
TH1F* lhist21_jetlinradmom1 = new TH1F("lhist21_jetlinradmom1","lhist21_jetlinradmom1",16,binning);
TH1F* lhist31_jetlinradmom1 = new TH1F("lhist31_jetlinradmom1","lhist31_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_jetlinradmom1",vars);

 TH1F *ltaupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom1");
 TH1F *ltaupt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist21_jetlinradmom1");
 TH1F *ltaupt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("lhist31_jetlinradmom1");

 ltaupt_1_jetlinradmom1->Sumw2();
 ltaupt_2_jetlinradmom1->Sumw2();
 ltaupt_3_jetlinradmom1->Sumw2();


 ltaupt_1_jetlinradmom1->Scale(988.8/9987968);
 ltaupt_2_jetlinradmom1->Scale(66.3/6000000);
 ltaupt_3_jetlinradmom1->Scale(8.15/5998860);

 



 ltaupt_1_jetlinradmom1->Add(ltaupt_2_jetlinradmom1);
 ltaupt_1_jetlinradmom1->Add(ltaupt_3_jetlinradmom1);
 


 ltaupt_1_jetlinradmom1->Divide(ljetpt_1_jetlinradmom1);
 ltaupt_1_jetlinradmom1->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom1->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom1 = new TH1F("bhist1_jetlinradmom1","bhist1_jetlinradmom1",16,binning);
TH1F* bhist2_jetlinradmom1 = new TH1F("bhist2_jetlinradmom1","bhist2_jetlinradmom1",16,binning);
TH1F* bhist3_jetlinradmom1 = new TH1F("bhist3_jetlinradmom1","bhist3_jetlinradmom1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_jetlinradmom1",vars);

 TH1F* bjetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom1");
 TH1F* bjetpt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist2_jetlinradmom1");
 TH1F* bjetpt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist3_jetlinradmom1");

 bjetpt_1_jetlinradmom1->Sumw2();
 bjetpt_2_jetlinradmom1->Sumw2();
 bjetpt_3_jetlinradmom1->Sumw2();


 bjetpt_1_jetlinradmom1->Scale(988.8/9987968);
 bjetpt_2_jetlinradmom1->Scale(66.3/6000000);
 bjetpt_3_jetlinradmom1->Scale(8.15/5998860);

 bjetpt_1_jetlinradmom1->Add(bjetpt_2_jetlinradmom1);
 bjetpt_1_jetlinradmom1->Add(bjetpt_3_jetlinradmom1);



TH1F* bhist11_jetlinradmom1 = new TH1F("bhist11_jetlinradmom1","bhist11_jetlinradmom1",16,binning);
TH1F* bhist21_jetlinradmom1 = new TH1F("bhist21_jetlinradmom1","bhist21_jetlinradmom1",16,binning);
TH1F* bhist31_jetlinradmom1 = new TH1F("bhist31_jetlinradmom1","bhist31_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_jetlinradmom1",vars);

 TH1F *btaupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom1");
 TH1F *btaupt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist21_jetlinradmom1");
 TH1F *btaupt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("bhist31_jetlinradmom1");

 btaupt_1_jetlinradmom1->Sumw2();
 btaupt_2_jetlinradmom1->Sumw2();
 btaupt_3_jetlinradmom1->Sumw2();


 btaupt_1_jetlinradmom1->Scale(988.8/9987968);
 btaupt_2_jetlinradmom1->Scale(66.3/6000000);
 btaupt_3_jetlinradmom1->Scale(8.15/5998860);

 



 btaupt_1_jetlinradmom1->Add(btaupt_2_jetlinradmom1);
 btaupt_1_jetlinradmom1->Add(btaupt_3_jetlinradmom1);
 


 btaupt_1_jetlinradmom1->Divide(bjetpt_1_jetlinradmom1);
 btaupt_1_jetlinradmom1->SetMarkerStyle(20);
 btaupt_1_jetlinradmom1->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom1 = new TH1F("ghist1_jetlinradmom1","ghist1_jetlinradmom1",16,binning);
TH1F* ghist2_jetlinradmom1 = new TH1F("ghist2_jetlinradmom1","ghist2_jetlinradmom1",16,binning);
TH1F* ghist3_jetlinradmom1 = new TH1F("ghist3_jetlinradmom1","ghist3_jetlinradmom1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_jetlinradmom1",vars);

 TH1F* gjetpt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom1");
 TH1F* gjetpt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist2_jetlinradmom1");
 TH1F* gjetpt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist3_jetlinradmom1");

 gjetpt_1_jetlinradmom1->Sumw2();
 gjetpt_2_jetlinradmom1->Sumw2();
 gjetpt_3_jetlinradmom1->Sumw2();


 gjetpt_1_jetlinradmom1->Scale(988.8/9987968);
 gjetpt_2_jetlinradmom1->Scale(66.3/6000000);
 gjetpt_3_jetlinradmom1->Scale(8.15/5998860);

 gjetpt_1_jetlinradmom1->Add(gjetpt_2_jetlinradmom1);
 gjetpt_1_jetlinradmom1->Add(gjetpt_3_jetlinradmom1);



TH1F* ghist11_jetlinradmom1 = new TH1F("ghist11_jetlinradmom1","ghist11_jetlinradmom1",16,binning);
TH1F* ghist21_jetlinradmom1 = new TH1F("ghist21_jetlinradmom1","ghist21_jetlinradmom1",16,binning);
TH1F* ghist31_jetlinradmom1 = new TH1F("ghist31_jetlinradmom1","ghist31_jetlinradmom1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>0&&jetlinradmom<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom1",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_jetlinradmom1",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_jetlinradmom1",vars);

 TH1F *gtaupt_1_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom1");
 TH1F *gtaupt_2_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist21_jetlinradmom1");
 TH1F *gtaupt_3_jetlinradmom1 = (TH1F*)gDirectory->Get("ghist31_jetlinradmom1");

 gtaupt_1_jetlinradmom1->Sumw2();
 gtaupt_2_jetlinradmom1->Sumw2();
 gtaupt_3_jetlinradmom1->Sumw2();


 gtaupt_1_jetlinradmom1->Scale(988.8/9987968);
 gtaupt_2_jetlinradmom1->Scale(66.3/6000000);
 gtaupt_3_jetlinradmom1->Scale(8.15/5998860);

 



 gtaupt_1_jetlinradmom1->Add(gtaupt_2_jetlinradmom1);
 gtaupt_1_jetlinradmom1->Add(gtaupt_3_jetlinradmom1);
 


 gtaupt_1_jetlinradmom1->Divide(gjetpt_1_jetlinradmom1);
 gtaupt_1_jetlinradmom1->SetMarkerStyle(20);

TH1F* hist1_jetlinradmom2 = new TH1F("hist1_jetlinradmom2","hist1_jetlinradmom2",16,binning);
TH1F* hist2_jetlinradmom2 = new TH1F("hist2_jetlinradmom2","hist2_jetlinradmom2",16,binning);
TH1F* hist3_jetlinradmom2 = new TH1F("hist3_jetlinradmom2","hist3_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>hist2_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>hist3_jetlinradmom2",vars);

 TH1F* jetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("hist1_jetlinradmom2");
 TH1F* jetpt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("hist2_jetlinradmom2");
 TH1F* jetpt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("hist3_jetlinradmom2");

 jetpt_1_jetlinradmom2->Sumw2();
 jetpt_2_jetlinradmom2->Sumw2();
 jetpt_3_jetlinradmom2->Sumw2();


 jetpt_1_jetlinradmom2->Scale(988.8/9987968);
 jetpt_2_jetlinradmom2->Scale(66.3/6000000);
 jetpt_3_jetlinradmom2->Scale(8.15/5998860);

 jetpt_1_jetlinradmom2->Add(jetpt_2_jetlinradmom2);
 jetpt_1_jetlinradmom2->Add(jetpt_3_jetlinradmom2);



TH1F* hist11_jetlinradmom2 = new TH1F("hist11_jetlinradmom2","hist11_jetlinradmom2",16,binning);
TH1F* hist21_jetlinradmom2 = new TH1F("hist21_jetlinradmom2","hist21_jetlinradmom2",16,binning);
TH1F* hist31_jetlinradmom2 = new TH1F("hist31_jetlinradmom2","hist31_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>hist21_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>hist31_jetlinradmom2",vars);

 TH1F *taupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("hist11_jetlinradmom2");
 TH1F *taupt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("hist21_jetlinradmom2");
 TH1F *taupt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("hist31_jetlinradmom2");

 taupt_1_jetlinradmom2->Sumw2();
 taupt_2_jetlinradmom2->Sumw2();
 taupt_3_jetlinradmom2->Sumw2();


 taupt_1_jetlinradmom2->Scale(988.8/9987968);
 taupt_2_jetlinradmom2->Scale(66.3/6000000);
 taupt_3_jetlinradmom2->Scale(8.15/5998860);

 



 taupt_1_jetlinradmom2->Add(taupt_2_jetlinradmom2);
 taupt_1_jetlinradmom2->Add(taupt_3_jetlinradmom2);
 


 taupt_1_jetlinradmom2->Divide(jetpt_1_jetlinradmom2);
 taupt_1_jetlinradmom2->SetMarkerStyle(20);
 taupt_1_jetlinradmom2->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom2 = new TH1F("lhist1_jetlinradmom2","lhist1_jetlinradmom2",16,binning);
TH1F* lhist2_jetlinradmom2 = new TH1F("lhist2_jetlinradmom2","lhist2_jetlinradmom2",16,binning);
TH1F* lhist3_jetlinradmom2 = new TH1F("lhist3_jetlinradmom2","lhist3_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_jetlinradmom2",vars);

 TH1F* ljetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom2");
 TH1F* ljetpt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist2_jetlinradmom2");
 TH1F* ljetpt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist3_jetlinradmom2");

 ljetpt_1_jetlinradmom2->Sumw2();
 ljetpt_2_jetlinradmom2->Sumw2();
 ljetpt_3_jetlinradmom2->Sumw2();


 ljetpt_1_jetlinradmom2->Scale(988.8/9987968);
 ljetpt_2_jetlinradmom2->Scale(66.3/6000000);
 ljetpt_3_jetlinradmom2->Scale(8.15/5998860);

 ljetpt_1_jetlinradmom2->Add(ljetpt_2_jetlinradmom2);
 ljetpt_1_jetlinradmom2->Add(ljetpt_3_jetlinradmom2);



TH1F* lhist11_jetlinradmom2 = new TH1F("lhist11_jetlinradmom2","lhist11_jetlinradmom2",16,binning);
TH1F* lhist21_jetlinradmom2 = new TH1F("lhist21_jetlinradmom2","lhist21_jetlinradmom2",16,binning);
TH1F* lhist31_jetlinradmom2 = new TH1F("lhist31_jetlinradmom2","lhist31_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_jetlinradmom2",vars);

 TH1F *ltaupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom2");
 TH1F *ltaupt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist21_jetlinradmom2");
 TH1F *ltaupt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("lhist31_jetlinradmom2");

 ltaupt_1_jetlinradmom2->Sumw2();
 ltaupt_2_jetlinradmom2->Sumw2();
 ltaupt_3_jetlinradmom2->Sumw2();


 ltaupt_1_jetlinradmom2->Scale(988.8/9987968);
 ltaupt_2_jetlinradmom2->Scale(66.3/6000000);
 ltaupt_3_jetlinradmom2->Scale(8.15/5998860);

 



 ltaupt_1_jetlinradmom2->Add(ltaupt_2_jetlinradmom2);
 ltaupt_1_jetlinradmom2->Add(ltaupt_3_jetlinradmom2);
 


 ltaupt_1_jetlinradmom2->Divide(ljetpt_1_jetlinradmom2);
 ltaupt_1_jetlinradmom2->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom2->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom2 = new TH1F("bhist1_jetlinradmom2","bhist1_jetlinradmom2",16,binning);
TH1F* bhist2_jetlinradmom2 = new TH1F("bhist2_jetlinradmom2","bhist2_jetlinradmom2",16,binning);
TH1F* bhist3_jetlinradmom2 = new TH1F("bhist3_jetlinradmom2","bhist3_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_jetlinradmom2",vars);

 TH1F* bjetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom2");
 TH1F* bjetpt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist2_jetlinradmom2");
 TH1F* bjetpt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist3_jetlinradmom2");

 bjetpt_1_jetlinradmom2->Sumw2();
 bjetpt_2_jetlinradmom2->Sumw2();
 bjetpt_3_jetlinradmom2->Sumw2();


 bjetpt_1_jetlinradmom2->Scale(988.8/9987968);
 bjetpt_2_jetlinradmom2->Scale(66.3/6000000);
 bjetpt_3_jetlinradmom2->Scale(8.15/5998860);

 bjetpt_1_jetlinradmom2->Add(bjetpt_2_jetlinradmom2);
 bjetpt_1_jetlinradmom2->Add(bjetpt_3_jetlinradmom2);



TH1F* bhist11_jetlinradmom2 = new TH1F("bhist11_jetlinradmom2","bhist11_jetlinradmom2",16,binning);
TH1F* bhist21_jetlinradmom2 = new TH1F("bhist21_jetlinradmom2","bhist21_jetlinradmom2",16,binning);
TH1F* bhist31_jetlinradmom2 = new TH1F("bhist31_jetlinradmom2","bhist31_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_jetlinradmom2",vars);

 TH1F *btaupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom2");
 TH1F *btaupt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist21_jetlinradmom2");
 TH1F *btaupt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("bhist31_jetlinradmom2");

 btaupt_1_jetlinradmom2->Sumw2();
 btaupt_2_jetlinradmom2->Sumw2();
 btaupt_3_jetlinradmom2->Sumw2();


 btaupt_1_jetlinradmom2->Scale(988.8/9987968);
 btaupt_2_jetlinradmom2->Scale(66.3/6000000);
 btaupt_3_jetlinradmom2->Scale(8.15/5998860);

 



 btaupt_1_jetlinradmom2->Add(btaupt_2_jetlinradmom2);
 btaupt_1_jetlinradmom2->Add(btaupt_3_jetlinradmom2);
 


 btaupt_1_jetlinradmom2->Divide(bjetpt_1_jetlinradmom2);
 btaupt_1_jetlinradmom2->SetMarkerStyle(20);
 btaupt_1_jetlinradmom2->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom2 = new TH1F("ghist1_jetlinradmom2","ghist1_jetlinradmom2",16,binning);
TH1F* ghist2_jetlinradmom2 = new TH1F("ghist2_jetlinradmom2","ghist2_jetlinradmom2",16,binning);
TH1F* ghist3_jetlinradmom2 = new TH1F("ghist3_jetlinradmom2","ghist3_jetlinradmom2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_jetlinradmom2",vars);

 TH1F* gjetpt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom2");
 TH1F* gjetpt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist2_jetlinradmom2");
 TH1F* gjetpt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist3_jetlinradmom2");

 gjetpt_1_jetlinradmom2->Sumw2();
 gjetpt_2_jetlinradmom2->Sumw2();
 gjetpt_3_jetlinradmom2->Sumw2();


 gjetpt_1_jetlinradmom2->Scale(988.8/9987968);
 gjetpt_2_jetlinradmom2->Scale(66.3/6000000);
 gjetpt_3_jetlinradmom2->Scale(8.15/5998860);

 gjetpt_1_jetlinradmom2->Add(gjetpt_2_jetlinradmom2);
 gjetpt_1_jetlinradmom2->Add(gjetpt_3_jetlinradmom2);



TH1F* ghist11_jetlinradmom2 = new TH1F("ghist11_jetlinradmom2","ghist11_jetlinradmom2",16,binning);
TH1F* ghist21_jetlinradmom2 = new TH1F("ghist21_jetlinradmom2","ghist21_jetlinradmom2",16,binning);
TH1F* ghist31_jetlinradmom2 = new TH1F("ghist31_jetlinradmom2","ghist31_jetlinradmom2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[0]+"&&jetlinradmom<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom2",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_jetlinradmom2",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_jetlinradmom2",vars);

 TH1F *gtaupt_1_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom2");
 TH1F *gtaupt_2_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist21_jetlinradmom2");
 TH1F *gtaupt_3_jetlinradmom2 = (TH1F*)gDirectory->Get("ghist31_jetlinradmom2");

 gtaupt_1_jetlinradmom2->Sumw2();
 gtaupt_2_jetlinradmom2->Sumw2();
 gtaupt_3_jetlinradmom2->Sumw2();


 gtaupt_1_jetlinradmom2->Scale(988.8/9987968);
 gtaupt_2_jetlinradmom2->Scale(66.3/6000000);
 gtaupt_3_jetlinradmom2->Scale(8.15/5998860);

 



 gtaupt_1_jetlinradmom2->Add(gtaupt_2_jetlinradmom2);
 gtaupt_1_jetlinradmom2->Add(gtaupt_3_jetlinradmom2);
 


 gtaupt_1_jetlinradmom2->Divide(gjetpt_1_jetlinradmom2);
 gtaupt_1_jetlinradmom2->SetMarkerStyle(20);

TH1F* hist1_jetlinradmom3 = new TH1F("hist1_jetlinradmom3","hist1_jetlinradmom3",16,binning);
TH1F* hist2_jetlinradmom3 = new TH1F("hist2_jetlinradmom3","hist2_jetlinradmom3",16,binning);
TH1F* hist3_jetlinradmom3 = new TH1F("hist3_jetlinradmom3","hist3_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>hist2_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>hist3_jetlinradmom3",vars);

 TH1F* jetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("hist1_jetlinradmom3");
 TH1F* jetpt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("hist2_jetlinradmom3");
 TH1F* jetpt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("hist3_jetlinradmom3");

 jetpt_1_jetlinradmom3->Sumw2();
 jetpt_2_jetlinradmom3->Sumw2();
 jetpt_3_jetlinradmom3->Sumw2();


 jetpt_1_jetlinradmom3->Scale(988.8/9987968);
 jetpt_2_jetlinradmom3->Scale(66.3/6000000);
 jetpt_3_jetlinradmom3->Scale(8.15/5998860);

 jetpt_1_jetlinradmom3->Add(jetpt_2_jetlinradmom3);
 jetpt_1_jetlinradmom3->Add(jetpt_3_jetlinradmom3);



TH1F* hist11_jetlinradmom3 = new TH1F("hist11_jetlinradmom3","hist11_jetlinradmom3",16,binning);
TH1F* hist21_jetlinradmom3 = new TH1F("hist21_jetlinradmom3","hist21_jetlinradmom3",16,binning);
TH1F* hist31_jetlinradmom3 = new TH1F("hist31_jetlinradmom3","hist31_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>hist21_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>hist31_jetlinradmom3",vars);

 TH1F *taupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("hist11_jetlinradmom3");
 TH1F *taupt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("hist21_jetlinradmom3");
 TH1F *taupt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("hist31_jetlinradmom3");

 taupt_1_jetlinradmom3->Sumw2();
 taupt_2_jetlinradmom3->Sumw2();
 taupt_3_jetlinradmom3->Sumw2();


 taupt_1_jetlinradmom3->Scale(988.8/9987968);
 taupt_2_jetlinradmom3->Scale(66.3/6000000);
 taupt_3_jetlinradmom3->Scale(8.15/5998860);

 



 taupt_1_jetlinradmom3->Add(taupt_2_jetlinradmom3);
 taupt_1_jetlinradmom3->Add(taupt_3_jetlinradmom3);
 


 taupt_1_jetlinradmom3->Divide(jetpt_1_jetlinradmom3);
 taupt_1_jetlinradmom3->SetMarkerStyle(20);
 taupt_1_jetlinradmom3->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom3 = new TH1F("lhist1_jetlinradmom3","lhist1_jetlinradmom3",16,binning);
TH1F* lhist2_jetlinradmom3 = new TH1F("lhist2_jetlinradmom3","lhist2_jetlinradmom3",16,binning);
TH1F* lhist3_jetlinradmom3 = new TH1F("lhist3_jetlinradmom3","lhist3_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_jetlinradmom3",vars);

 TH1F* ljetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom3");
 TH1F* ljetpt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist2_jetlinradmom3");
 TH1F* ljetpt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist3_jetlinradmom3");

 ljetpt_1_jetlinradmom3->Sumw2();
 ljetpt_2_jetlinradmom3->Sumw2();
 ljetpt_3_jetlinradmom3->Sumw2();


 ljetpt_1_jetlinradmom3->Scale(988.8/9987968);
 ljetpt_2_jetlinradmom3->Scale(66.3/6000000);
 ljetpt_3_jetlinradmom3->Scale(8.15/5998860);

 ljetpt_1_jetlinradmom3->Add(ljetpt_2_jetlinradmom3);
 ljetpt_1_jetlinradmom3->Add(ljetpt_3_jetlinradmom3);



TH1F* lhist11_jetlinradmom3 = new TH1F("lhist11_jetlinradmom3","lhist11_jetlinradmom3",16,binning);
TH1F* lhist21_jetlinradmom3 = new TH1F("lhist21_jetlinradmom3","lhist21_jetlinradmom3",16,binning);
TH1F* lhist31_jetlinradmom3 = new TH1F("lhist31_jetlinradmom3","lhist31_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_jetlinradmom3",vars);

 TH1F *ltaupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom3");
 TH1F *ltaupt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist21_jetlinradmom3");
 TH1F *ltaupt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("lhist31_jetlinradmom3");

 ltaupt_1_jetlinradmom3->Sumw2();
 ltaupt_2_jetlinradmom3->Sumw2();
 ltaupt_3_jetlinradmom3->Sumw2();


 ltaupt_1_jetlinradmom3->Scale(988.8/9987968);
 ltaupt_2_jetlinradmom3->Scale(66.3/6000000);
 ltaupt_3_jetlinradmom3->Scale(8.15/5998860);

 



 ltaupt_1_jetlinradmom3->Add(ltaupt_2_jetlinradmom3);
 ltaupt_1_jetlinradmom3->Add(ltaupt_3_jetlinradmom3);
 


 ltaupt_1_jetlinradmom3->Divide(ljetpt_1_jetlinradmom3);
 ltaupt_1_jetlinradmom3->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom3->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom3 = new TH1F("bhist1_jetlinradmom3","bhist1_jetlinradmom3",16,binning);
TH1F* bhist2_jetlinradmom3 = new TH1F("bhist2_jetlinradmom3","bhist2_jetlinradmom3",16,binning);
TH1F* bhist3_jetlinradmom3 = new TH1F("bhist3_jetlinradmom3","bhist3_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_jetlinradmom3",vars);

 TH1F* bjetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom3");
 TH1F* bjetpt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist2_jetlinradmom3");
 TH1F* bjetpt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist3_jetlinradmom3");

 bjetpt_1_jetlinradmom3->Sumw2();
 bjetpt_2_jetlinradmom3->Sumw2();
 bjetpt_3_jetlinradmom3->Sumw2();


 bjetpt_1_jetlinradmom3->Scale(988.8/9987968);
 bjetpt_2_jetlinradmom3->Scale(66.3/6000000);
 bjetpt_3_jetlinradmom3->Scale(8.15/5998860);

 bjetpt_1_jetlinradmom3->Add(bjetpt_2_jetlinradmom3);
 bjetpt_1_jetlinradmom3->Add(bjetpt_3_jetlinradmom3);



TH1F* bhist11_jetlinradmom3 = new TH1F("bhist11_jetlinradmom3","bhist11_jetlinradmom3",16,binning);
TH1F* bhist21_jetlinradmom3 = new TH1F("bhist21_jetlinradmom3","bhist21_jetlinradmom3",16,binning);
TH1F* bhist31_jetlinradmom3 = new TH1F("bhist31_jetlinradmom3","bhist31_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_jetlinradmom3",vars);

 TH1F *btaupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom3");
 TH1F *btaupt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist21_jetlinradmom3");
 TH1F *btaupt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("bhist31_jetlinradmom3");

 btaupt_1_jetlinradmom3->Sumw2();
 btaupt_2_jetlinradmom3->Sumw2();
 btaupt_3_jetlinradmom3->Sumw2();


 btaupt_1_jetlinradmom3->Scale(988.8/9987968);
 btaupt_2_jetlinradmom3->Scale(66.3/6000000);
 btaupt_3_jetlinradmom3->Scale(8.15/5998860);

 



 btaupt_1_jetlinradmom3->Add(btaupt_2_jetlinradmom3);
 btaupt_1_jetlinradmom3->Add(btaupt_3_jetlinradmom3);
 


 btaupt_1_jetlinradmom3->Divide(bjetpt_1_jetlinradmom3);
 btaupt_1_jetlinradmom3->SetMarkerStyle(20);
 btaupt_1_jetlinradmom3->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom3 = new TH1F("ghist1_jetlinradmom3","ghist1_jetlinradmom3",16,binning);
TH1F* ghist2_jetlinradmom3 = new TH1F("ghist2_jetlinradmom3","ghist2_jetlinradmom3",16,binning);
TH1F* ghist3_jetlinradmom3 = new TH1F("ghist3_jetlinradmom3","ghist3_jetlinradmom3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_jetlinradmom3",vars);

 TH1F* gjetpt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom3");
 TH1F* gjetpt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist2_jetlinradmom3");
 TH1F* gjetpt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist3_jetlinradmom3");

 gjetpt_1_jetlinradmom3->Sumw2();
 gjetpt_2_jetlinradmom3->Sumw2();
 gjetpt_3_jetlinradmom3->Sumw2();


 gjetpt_1_jetlinradmom3->Scale(988.8/9987968);
 gjetpt_2_jetlinradmom3->Scale(66.3/6000000);
 gjetpt_3_jetlinradmom3->Scale(8.15/5998860);

 gjetpt_1_jetlinradmom3->Add(gjetpt_2_jetlinradmom3);
 gjetpt_1_jetlinradmom3->Add(gjetpt_3_jetlinradmom3);



TH1F* ghist11_jetlinradmom3 = new TH1F("ghist11_jetlinradmom3","ghist11_jetlinradmom3",16,binning);
TH1F* ghist21_jetlinradmom3 = new TH1F("ghist21_jetlinradmom3","ghist21_jetlinradmom3",16,binning);
TH1F* ghist31_jetlinradmom3 = new TH1F("ghist31_jetlinradmom3","ghist31_jetlinradmom3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[1]+"&&jetlinradmom<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom3",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_jetlinradmom3",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_jetlinradmom3",vars);

 TH1F *gtaupt_1_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom3");
 TH1F *gtaupt_2_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist21_jetlinradmom3");
 TH1F *gtaupt_3_jetlinradmom3 = (TH1F*)gDirectory->Get("ghist31_jetlinradmom3");

 gtaupt_1_jetlinradmom3->Sumw2();
 gtaupt_2_jetlinradmom3->Sumw2();
 gtaupt_3_jetlinradmom3->Sumw2();


 gtaupt_1_jetlinradmom3->Scale(988.8/9987968);
 gtaupt_2_jetlinradmom3->Scale(66.3/6000000);
 gtaupt_3_jetlinradmom3->Scale(8.15/5998860);

 



 gtaupt_1_jetlinradmom3->Add(gtaupt_2_jetlinradmom3);
 gtaupt_1_jetlinradmom3->Add(gtaupt_3_jetlinradmom3);
 


 gtaupt_1_jetlinradmom3->Divide(gjetpt_1_jetlinradmom3);
 gtaupt_1_jetlinradmom3->SetMarkerStyle(20);



TH1F* hist1_jetlinradmom4 = new TH1F("hist1_jetlinradmom4","hist1_jetlinradmom4",16,binning);
TH1F* hist2_jetlinradmom4 = new TH1F("hist2_jetlinradmom4","hist2_jetlinradmom4",16,binning);
TH1F* hist3_jetlinradmom4 = new TH1F("hist3_jetlinradmom4","hist3_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>hist2_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>hist3_jetlinradmom4",vars);

 TH1F* jetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("hist1_jetlinradmom4");
 TH1F* jetpt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("hist2_jetlinradmom4");
 TH1F* jetpt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("hist3_jetlinradmom4");

 jetpt_1_jetlinradmom4->Sumw2();
 jetpt_2_jetlinradmom4->Sumw2();
 jetpt_3_jetlinradmom4->Sumw2();


 jetpt_1_jetlinradmom4->Scale(988.8/9987968);
 jetpt_2_jetlinradmom4->Scale(66.3/6000000);
 jetpt_3_jetlinradmom4->Scale(8.15/5998860);

 jetpt_1_jetlinradmom4->Add(jetpt_2_jetlinradmom4);
 jetpt_1_jetlinradmom4->Add(jetpt_3_jetlinradmom4);



TH1F* hist11_jetlinradmom4 = new TH1F("hist11_jetlinradmom4","hist11_jetlinradmom4",16,binning);
TH1F* hist21_jetlinradmom4 = new TH1F("hist21_jetlinradmom4","hist21_jetlinradmom4",16,binning);
TH1F* hist31_jetlinradmom4 = new TH1F("hist31_jetlinradmom4","hist31_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>hist21_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>hist31_jetlinradmom4",vars);

 TH1F *taupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("hist11_jetlinradmom4");
 TH1F *taupt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("hist21_jetlinradmom4");
 TH1F *taupt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("hist31_jetlinradmom4");

 taupt_1_jetlinradmom4->Sumw2();
 taupt_2_jetlinradmom4->Sumw2();
 taupt_3_jetlinradmom4->Sumw2();


 taupt_1_jetlinradmom4->Scale(988.8/9987968);
 taupt_2_jetlinradmom4->Scale(66.3/6000000);
 taupt_3_jetlinradmom4->Scale(8.15/5998860);

 



 taupt_1_jetlinradmom4->Add(taupt_2_jetlinradmom4);
 taupt_1_jetlinradmom4->Add(taupt_3_jetlinradmom4);
 


 taupt_1_jetlinradmom4->Divide(jetpt_1_jetlinradmom4);
 taupt_1_jetlinradmom4->SetMarkerStyle(20);
 taupt_1_jetlinradmom4->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom4 = new TH1F("lhist1_jetlinradmom4","lhist1_jetlinradmom4",16,binning);
TH1F* lhist2_jetlinradmom4 = new TH1F("lhist2_jetlinradmom4","lhist2_jetlinradmom4",16,binning);
TH1F* lhist3_jetlinradmom4 = new TH1F("lhist3_jetlinradmom4","lhist3_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_jetlinradmom4",vars);

 TH1F* ljetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom4");
 TH1F* ljetpt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist2_jetlinradmom4");
 TH1F* ljetpt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist3_jetlinradmom4");

 ljetpt_1_jetlinradmom4->Sumw2();
 ljetpt_2_jetlinradmom4->Sumw2();
 ljetpt_3_jetlinradmom4->Sumw2();


 ljetpt_1_jetlinradmom4->Scale(988.8/9987968);
 ljetpt_2_jetlinradmom4->Scale(66.3/6000000);
 ljetpt_3_jetlinradmom4->Scale(8.15/5998860);

 ljetpt_1_jetlinradmom4->Add(ljetpt_2_jetlinradmom4);
 ljetpt_1_jetlinradmom4->Add(ljetpt_3_jetlinradmom4);



TH1F* lhist11_jetlinradmom4 = new TH1F("lhist11_jetlinradmom4","lhist11_jetlinradmom4",16,binning);
TH1F* lhist21_jetlinradmom4 = new TH1F("lhist21_jetlinradmom4","lhist21_jetlinradmom4",16,binning);
TH1F* lhist31_jetlinradmom4 = new TH1F("lhist31_jetlinradmom4","lhist31_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_jetlinradmom4",vars);

 TH1F *ltaupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom4");
 TH1F *ltaupt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist21_jetlinradmom4");
 TH1F *ltaupt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("lhist31_jetlinradmom4");

 ltaupt_1_jetlinradmom4->Sumw2();
 ltaupt_2_jetlinradmom4->Sumw2();
 ltaupt_3_jetlinradmom4->Sumw2();


 ltaupt_1_jetlinradmom4->Scale(988.8/9987968);
 ltaupt_2_jetlinradmom4->Scale(66.3/6000000);
 ltaupt_3_jetlinradmom4->Scale(8.15/5998860);

 



 ltaupt_1_jetlinradmom4->Add(ltaupt_2_jetlinradmom4);
 ltaupt_1_jetlinradmom4->Add(ltaupt_3_jetlinradmom4);
 


 ltaupt_1_jetlinradmom4->Divide(ljetpt_1_jetlinradmom4);
 ltaupt_1_jetlinradmom4->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom4->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom4 = new TH1F("bhist1_jetlinradmom4","bhist1_jetlinradmom4",16,binning);
TH1F* bhist2_jetlinradmom4 = new TH1F("bhist2_jetlinradmom4","bhist2_jetlinradmom4",16,binning);
TH1F* bhist3_jetlinradmom4 = new TH1F("bhist3_jetlinradmom4","bhist3_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_jetlinradmom4",vars);

 TH1F* bjetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom4");
 TH1F* bjetpt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist2_jetlinradmom4");
 TH1F* bjetpt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist3_jetlinradmom4");

 bjetpt_1_jetlinradmom4->Sumw2();
 bjetpt_2_jetlinradmom4->Sumw2();
 bjetpt_3_jetlinradmom4->Sumw2();


 bjetpt_1_jetlinradmom4->Scale(988.8/9987968);
 bjetpt_2_jetlinradmom4->Scale(66.3/6000000);
 bjetpt_3_jetlinradmom4->Scale(8.15/5998860);

 bjetpt_1_jetlinradmom4->Add(bjetpt_2_jetlinradmom4);
 bjetpt_1_jetlinradmom4->Add(bjetpt_3_jetlinradmom4);



TH1F* bhist11_jetlinradmom4 = new TH1F("bhist11_jetlinradmom4","bhist11_jetlinradmom4",16,binning);
TH1F* bhist21_jetlinradmom4 = new TH1F("bhist21_jetlinradmom4","bhist21_jetlinradmom4",16,binning);
TH1F* bhist31_jetlinradmom4 = new TH1F("bhist31_jetlinradmom4","bhist31_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_jetlinradmom4",vars);

 TH1F *btaupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom4");
 TH1F *btaupt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist21_jetlinradmom4");
 TH1F *btaupt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("bhist31_jetlinradmom4");

 btaupt_1_jetlinradmom4->Sumw2();
 btaupt_2_jetlinradmom4->Sumw2();
 btaupt_3_jetlinradmom4->Sumw2();


 btaupt_1_jetlinradmom4->Scale(988.8/9987968);
 btaupt_2_jetlinradmom4->Scale(66.3/6000000);
 btaupt_3_jetlinradmom4->Scale(8.15/5998860);

 



 btaupt_1_jetlinradmom4->Add(btaupt_2_jetlinradmom4);
 btaupt_1_jetlinradmom4->Add(btaupt_3_jetlinradmom4);
 


 btaupt_1_jetlinradmom4->Divide(bjetpt_1_jetlinradmom4);
 btaupt_1_jetlinradmom4->SetMarkerStyle(20);
 btaupt_1_jetlinradmom4->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom4 = new TH1F("ghist1_jetlinradmom4","ghist1_jetlinradmom4",16,binning);
TH1F* ghist2_jetlinradmom4 = new TH1F("ghist2_jetlinradmom4","ghist2_jetlinradmom4",16,binning);
TH1F* ghist3_jetlinradmom4 = new TH1F("ghist3_jetlinradmom4","ghist3_jetlinradmom4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_jetlinradmom4",vars);

 TH1F* gjetpt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom4");
 TH1F* gjetpt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist2_jetlinradmom4");
 TH1F* gjetpt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist3_jetlinradmom4");

 gjetpt_1_jetlinradmom4->Sumw2();
 gjetpt_2_jetlinradmom4->Sumw2();
 gjetpt_3_jetlinradmom4->Sumw2();


 gjetpt_1_jetlinradmom4->Scale(988.8/9987968);
 gjetpt_2_jetlinradmom4->Scale(66.3/6000000);
 gjetpt_3_jetlinradmom4->Scale(8.15/5998860);

 gjetpt_1_jetlinradmom4->Add(gjetpt_2_jetlinradmom4);
 gjetpt_1_jetlinradmom4->Add(gjetpt_3_jetlinradmom4);



TH1F* ghist11_jetlinradmom4 = new TH1F("ghist11_jetlinradmom4","ghist11_jetlinradmom4",16,binning);
TH1F* ghist21_jetlinradmom4 = new TH1F("ghist21_jetlinradmom4","ghist21_jetlinradmom4",16,binning);
TH1F* ghist31_jetlinradmom4 = new TH1F("ghist31_jetlinradmom4","ghist31_jetlinradmom4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[2]+"&&jetlinradmom<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom4",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_jetlinradmom4",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_jetlinradmom4",vars);

 TH1F *gtaupt_1_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom4");
 TH1F *gtaupt_2_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist21_jetlinradmom4");
 TH1F *gtaupt_3_jetlinradmom4 = (TH1F*)gDirectory->Get("ghist31_jetlinradmom4");

 gtaupt_1_jetlinradmom4->Sumw2();
 gtaupt_2_jetlinradmom4->Sumw2();
 gtaupt_3_jetlinradmom4->Sumw2();


 gtaupt_1_jetlinradmom4->Scale(988.8/9987968);
 gtaupt_2_jetlinradmom4->Scale(66.3/6000000);
 gtaupt_3_jetlinradmom4->Scale(8.15/5998860);

 



 gtaupt_1_jetlinradmom4->Add(gtaupt_2_jetlinradmom4);
 gtaupt_1_jetlinradmom4->Add(gtaupt_3_jetlinradmom4);
 


 gtaupt_1_jetlinradmom4->Divide(gjetpt_1_jetlinradmom4);
 gtaupt_1_jetlinradmom4->SetMarkerStyle(20);

TH1F* hist1_jetlinradmom5 = new TH1F("hist1_jetlinradmom5","hist1_jetlinradmom5",16,binning);
TH1F* hist2_jetlinradmom5 = new TH1F("hist2_jetlinradmom5","hist2_jetlinradmom5",16,binning);
TH1F* hist3_jetlinradmom5 = new TH1F("hist3_jetlinradmom5","hist3_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>hist2_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>hist3_jetlinradmom5",vars);

 TH1F* jetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("hist1_jetlinradmom5");
 TH1F* jetpt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("hist2_jetlinradmom5");
 TH1F* jetpt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("hist3_jetlinradmom5");

 jetpt_1_jetlinradmom5->Sumw2();
 jetpt_2_jetlinradmom5->Sumw2();
 jetpt_3_jetlinradmom5->Sumw2();


 jetpt_1_jetlinradmom5->Scale(988.8/9987968);
 jetpt_2_jetlinradmom5->Scale(66.3/6000000);
 jetpt_3_jetlinradmom5->Scale(8.15/5998860);

 jetpt_1_jetlinradmom5->Add(jetpt_2_jetlinradmom5);
 jetpt_1_jetlinradmom5->Add(jetpt_3_jetlinradmom5);



TH1F* hist11_jetlinradmom5 = new TH1F("hist11_jetlinradmom5","hist11_jetlinradmom5",16,binning);
TH1F* hist21_jetlinradmom5 = new TH1F("hist21_jetlinradmom5","hist21_jetlinradmom5",16,binning);
TH1F* hist31_jetlinradmom5 = new TH1F("hist31_jetlinradmom5","hist31_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>hist21_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>hist31_jetlinradmom5",vars);

 TH1F *taupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("hist11_jetlinradmom5");
 TH1F *taupt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("hist21_jetlinradmom5");
 TH1F *taupt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("hist31_jetlinradmom5");

 taupt_1_jetlinradmom5->Sumw2();
 taupt_2_jetlinradmom5->Sumw2();
 taupt_3_jetlinradmom5->Sumw2();


 taupt_1_jetlinradmom5->Scale(988.8/9987968);
 taupt_2_jetlinradmom5->Scale(66.3/6000000);
 taupt_3_jetlinradmom5->Scale(8.15/5998860);

 



 taupt_1_jetlinradmom5->Add(taupt_2_jetlinradmom5);
 taupt_1_jetlinradmom5->Add(taupt_3_jetlinradmom5);
 


 taupt_1_jetlinradmom5->Divide(jetpt_1_jetlinradmom5);
 taupt_1_jetlinradmom5->SetMarkerStyle(20);
 taupt_1_jetlinradmom5->SetMarkerColor(kRed);


TH1F* lhist1_jetlinradmom5 = new TH1F("lhist1_jetlinradmom5","lhist1_jetlinradmom5",16,binning);
TH1F* lhist2_jetlinradmom5 = new TH1F("lhist2_jetlinradmom5","lhist2_jetlinradmom5",16,binning);
TH1F* lhist3_jetlinradmom5 = new TH1F("lhist3_jetlinradmom5","lhist3_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_jetlinradmom5",vars);

 TH1F* ljetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist1_jetlinradmom5");
 TH1F* ljetpt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist2_jetlinradmom5");
 TH1F* ljetpt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist3_jetlinradmom5");

 ljetpt_1_jetlinradmom5->Sumw2();
 ljetpt_2_jetlinradmom5->Sumw2();
 ljetpt_3_jetlinradmom5->Sumw2();


 ljetpt_1_jetlinradmom5->Scale(988.8/9987968);
 ljetpt_2_jetlinradmom5->Scale(66.3/6000000);
 ljetpt_3_jetlinradmom5->Scale(8.15/5998860);

 ljetpt_1_jetlinradmom5->Add(ljetpt_2_jetlinradmom5);
 ljetpt_1_jetlinradmom5->Add(ljetpt_3_jetlinradmom5);



TH1F* lhist11_jetlinradmom5 = new TH1F("lhist11_jetlinradmom5","lhist11_jetlinradmom5",16,binning);
TH1F* lhist21_jetlinradmom5 = new TH1F("lhist21_jetlinradmom5","lhist21_jetlinradmom5",16,binning);
TH1F* lhist31_jetlinradmom5 = new TH1F("lhist31_jetlinradmom5","lhist31_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_jetlinradmom5",vars);

 TH1F *ltaupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist11_jetlinradmom5");
 TH1F *ltaupt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist21_jetlinradmom5");
 TH1F *ltaupt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("lhist31_jetlinradmom5");

 ltaupt_1_jetlinradmom5->Sumw2();
 ltaupt_2_jetlinradmom5->Sumw2();
 ltaupt_3_jetlinradmom5->Sumw2();


 ltaupt_1_jetlinradmom5->Scale(988.8/9987968);
 ltaupt_2_jetlinradmom5->Scale(66.3/6000000);
 ltaupt_3_jetlinradmom5->Scale(8.15/5998860);

 



 ltaupt_1_jetlinradmom5->Add(ltaupt_2_jetlinradmom5);
 ltaupt_1_jetlinradmom5->Add(ltaupt_3_jetlinradmom5);
 


 ltaupt_1_jetlinradmom5->Divide(ljetpt_1_jetlinradmom5);
 ltaupt_1_jetlinradmom5->SetMarkerStyle(20);
 ltaupt_1_jetlinradmom5->SetMarkerColor(kGreen+3);



TH1F* bhist1_jetlinradmom5 = new TH1F("bhist1_jetlinradmom5","bhist1_jetlinradmom5",16,binning);
TH1F* bhist2_jetlinradmom5 = new TH1F("bhist2_jetlinradmom5","bhist2_jetlinradmom5",16,binning);
TH1F* bhist3_jetlinradmom5 = new TH1F("bhist3_jetlinradmom5","bhist3_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_jetlinradmom5",vars);

 TH1F* bjetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist1_jetlinradmom5");
 TH1F* bjetpt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist2_jetlinradmom5");
 TH1F* bjetpt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist3_jetlinradmom5");

 bjetpt_1_jetlinradmom5->Sumw2();
 bjetpt_2_jetlinradmom5->Sumw2();
 bjetpt_3_jetlinradmom5->Sumw2();


 bjetpt_1_jetlinradmom5->Scale(988.8/9987968);
 bjetpt_2_jetlinradmom5->Scale(66.3/6000000);
 bjetpt_3_jetlinradmom5->Scale(8.15/5998860);

 bjetpt_1_jetlinradmom5->Add(bjetpt_2_jetlinradmom5);
 bjetpt_1_jetlinradmom5->Add(bjetpt_3_jetlinradmom5);



TH1F* bhist11_jetlinradmom5 = new TH1F("bhist11_jetlinradmom5","bhist11_jetlinradmom5",16,binning);
TH1F* bhist21_jetlinradmom5 = new TH1F("bhist21_jetlinradmom5","bhist21_jetlinradmom5",16,binning);
TH1F* bhist31_jetlinradmom5 = new TH1F("bhist31_jetlinradmom5","bhist31_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_jetlinradmom5",vars);

 TH1F *btaupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist11_jetlinradmom5");
 TH1F *btaupt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist21_jetlinradmom5");
 TH1F *btaupt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("bhist31_jetlinradmom5");

 btaupt_1_jetlinradmom5->Sumw2();
 btaupt_2_jetlinradmom5->Sumw2();
 btaupt_3_jetlinradmom5->Sumw2();


 btaupt_1_jetlinradmom5->Scale(988.8/9987968);
 btaupt_2_jetlinradmom5->Scale(66.3/6000000);
 btaupt_3_jetlinradmom5->Scale(8.15/5998860);

 



 btaupt_1_jetlinradmom5->Add(btaupt_2_jetlinradmom5);
 btaupt_1_jetlinradmom5->Add(btaupt_3_jetlinradmom5);
 


 btaupt_1_jetlinradmom5->Divide(bjetpt_1_jetlinradmom5);
 btaupt_1_jetlinradmom5->SetMarkerStyle(20);
 btaupt_1_jetlinradmom5->SetMarkerColor(kBlue);


TH1F* ghist1_jetlinradmom5 = new TH1F("ghist1_jetlinradmom5","ghist1_jetlinradmom5",16,binning);
TH1F* ghist2_jetlinradmom5 = new TH1F("ghist2_jetlinradmom5","ghist2_jetlinradmom5",16,binning);
TH1F* ghist3_jetlinradmom5 = new TH1F("ghist3_jetlinradmom5","ghist3_jetlinradmom5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_jetlinradmom5",vars);

 TH1F* gjetpt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist1_jetlinradmom5");
 TH1F* gjetpt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist2_jetlinradmom5");
 TH1F* gjetpt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist3_jetlinradmom5");

 gjetpt_1_jetlinradmom5->Sumw2();
 gjetpt_2_jetlinradmom5->Sumw2();
 gjetpt_3_jetlinradmom5->Sumw2();


 gjetpt_1_jetlinradmom5->Scale(988.8/9987968);
 gjetpt_2_jetlinradmom5->Scale(66.3/6000000);
 gjetpt_3_jetlinradmom5->Scale(8.15/5998860);

 gjetpt_1_jetlinradmom5->Add(gjetpt_2_jetlinradmom5);
 gjetpt_1_jetlinradmom5->Add(gjetpt_3_jetlinradmom5);



TH1F* ghist11_jetlinradmom5 = new TH1F("ghist11_jetlinradmom5","ghist11_jetlinradmom5",16,binning);
TH1F* ghist21_jetlinradmom5 = new TH1F("ghist21_jetlinradmom5","ghist21_jetlinradmom5",16,binning);
TH1F* ghist31_jetlinradmom5 = new TH1F("ghist31_jetlinradmom5","ghist31_jetlinradmom5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&jetlinradmom>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_jetlinradmom5",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_jetlinradmom5",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_jetlinradmom5",vars);

 TH1F *gtaupt_1_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist11_jetlinradmom5");
 TH1F *gtaupt_2_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist21_jetlinradmom5");
 TH1F *gtaupt_3_jetlinradmom5 = (TH1F*)gDirectory->Get("ghist31_jetlinradmom5");

 gtaupt_1_jetlinradmom5->Sumw2();
 gtaupt_2_jetlinradmom5->Sumw2();
 gtaupt_3_jetlinradmom5->Sumw2();


 gtaupt_1_jetlinradmom5->Scale(988.8/9987968);
 gtaupt_2_jetlinradmom5->Scale(66.3/6000000);
 gtaupt_3_jetlinradmom5->Scale(8.15/5998860);

 



 gtaupt_1_jetlinradmom5->Add(gtaupt_2_jetlinradmom5);
 gtaupt_1_jetlinradmom5->Add(gtaupt_3_jetlinradmom5);
 


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
 TFile *outfile = new TFile("Jan29_qcd_fakerates_jetlinradmom.root","RECREATE");
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
