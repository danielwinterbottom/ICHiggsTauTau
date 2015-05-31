void drawJetTauFakeRateSumEt(){
 TFile *qcd_1 = TFile::Open("Jan29_QCD_15-30.root");
 TFile *qcd_2 = TFile::Open("Jan29_QCD_30-50.root");
 TFile *qcd_3 = TFile::Open("Jan29_QCD_50-80.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));
 TTree *qcd_tree_2 = dynamic_cast<TTree*>(qcd_2->Get("ntuple"));
 TTree *qcd_tree_3 = dynamic_cast<TTree*>(qcd_3->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"500","1000","1500","2000"};
TH1F* hist1_sumet1 = new TH1F("hist1_sumet1","hist1_sumet1",16,binning);
TH1F* hist2_sumet1 = new TH1F("hist2_sumet1","hist2_sumet1",16,binning);
TH1F* hist3_sumet1 = new TH1F("hist3_sumet1","hist3_sumet1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumet1",vars);

 TH1F* jetpt_1_sumet1 = (TH1F*)gDirectory->Get("hist1_sumet1");
 TH1F* jetpt_2_sumet1 = (TH1F*)gDirectory->Get("hist2_sumet1");
 TH1F* jetpt_3_sumet1 = (TH1F*)gDirectory->Get("hist3_sumet1");

 jetpt_1_sumet1->Sumw2();
 jetpt_2_sumet1->Sumw2();
 jetpt_3_sumet1->Sumw2();


 jetpt_1_sumet1->Scale(988.8/9987968);
 jetpt_2_sumet1->Scale(66.3/6000000);
 jetpt_3_sumet1->Scale(8.15/5998860);

 jetpt_1_sumet1->Add(jetpt_2_sumet1);
 jetpt_1_sumet1->Add(jetpt_3_sumet1);



TH1F* hist11_sumet1 = new TH1F("hist11_sumet1","hist11_sumet1",16,binning);
TH1F* hist21_sumet1 = new TH1F("hist21_sumet1","hist21_sumet1",16,binning);
TH1F* hist31_sumet1 = new TH1F("hist31_sumet1","hist31_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist11_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumet1",vars);

 TH1F *taupt_1_sumet1 = (TH1F*)gDirectory->Get("hist11_sumet1");
 TH1F *taupt_2_sumet1 = (TH1F*)gDirectory->Get("hist21_sumet1");
 TH1F *taupt_3_sumet1 = (TH1F*)gDirectory->Get("hist31_sumet1");

 taupt_1_sumet1->Sumw2();
 taupt_2_sumet1->Sumw2();
 taupt_3_sumet1->Sumw2();


 taupt_1_sumet1->Scale(988.8/9987968);
 taupt_2_sumet1->Scale(66.3/6000000);
 taupt_3_sumet1->Scale(8.15/5998860);

 



 taupt_1_sumet1->Add(taupt_2_sumet1);
 taupt_1_sumet1->Add(taupt_3_sumet1);
 


 taupt_1_sumet1->Divide(jetpt_1_sumet1);
 taupt_1_sumet1->SetMarkerStyle(20);
 taupt_1_sumet1->SetMarkerColor(kRed);


TH1F* lhist1_sumet1 = new TH1F("lhist1_sumet1","lhist1_sumet1",16,binning);
TH1F* lhist2_sumet1 = new TH1F("lhist2_sumet1","lhist2_sumet1",16,binning);
TH1F* lhist3_sumet1 = new TH1F("lhist3_sumet1","lhist3_sumet1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumet1",vars);

 TH1F* ljetpt_1_sumet1 = (TH1F*)gDirectory->Get("lhist1_sumet1");
 TH1F* ljetpt_2_sumet1 = (TH1F*)gDirectory->Get("lhist2_sumet1");
 TH1F* ljetpt_3_sumet1 = (TH1F*)gDirectory->Get("lhist3_sumet1");

 ljetpt_1_sumet1->Sumw2();
 ljetpt_2_sumet1->Sumw2();
 ljetpt_3_sumet1->Sumw2();


 ljetpt_1_sumet1->Scale(988.8/9987968);
 ljetpt_2_sumet1->Scale(66.3/6000000);
 ljetpt_3_sumet1->Scale(8.15/5998860);

 ljetpt_1_sumet1->Add(ljetpt_2_sumet1);
 ljetpt_1_sumet1->Add(ljetpt_3_sumet1);



TH1F* lhist11_sumet1 = new TH1F("lhist11_sumet1","lhist11_sumet1",16,binning);
TH1F* lhist21_sumet1 = new TH1F("lhist21_sumet1","lhist21_sumet1",16,binning);
TH1F* lhist31_sumet1 = new TH1F("lhist31_sumet1","lhist31_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumet1",vars);

 TH1F *ltaupt_1_sumet1 = (TH1F*)gDirectory->Get("lhist11_sumet1");
 TH1F *ltaupt_2_sumet1 = (TH1F*)gDirectory->Get("lhist21_sumet1");
 TH1F *ltaupt_3_sumet1 = (TH1F*)gDirectory->Get("lhist31_sumet1");

 ltaupt_1_sumet1->Sumw2();
 ltaupt_2_sumet1->Sumw2();
 ltaupt_3_sumet1->Sumw2();


 ltaupt_1_sumet1->Scale(988.8/9987968);
 ltaupt_2_sumet1->Scale(66.3/6000000);
 ltaupt_3_sumet1->Scale(8.15/5998860);

 



 ltaupt_1_sumet1->Add(ltaupt_2_sumet1);
 ltaupt_1_sumet1->Add(ltaupt_3_sumet1);
 


 ltaupt_1_sumet1->Divide(ljetpt_1_sumet1);
 ltaupt_1_sumet1->SetMarkerStyle(20);
 ltaupt_1_sumet1->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet1 = new TH1F("bhist1_sumet1","bhist1_sumet1",16,binning);
TH1F* bhist2_sumet1 = new TH1F("bhist2_sumet1","bhist2_sumet1",16,binning);
TH1F* bhist3_sumet1 = new TH1F("bhist3_sumet1","bhist3_sumet1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumet1",vars);

 TH1F* bjetpt_1_sumet1 = (TH1F*)gDirectory->Get("bhist1_sumet1");
 TH1F* bjetpt_2_sumet1 = (TH1F*)gDirectory->Get("bhist2_sumet1");
 TH1F* bjetpt_3_sumet1 = (TH1F*)gDirectory->Get("bhist3_sumet1");

 bjetpt_1_sumet1->Sumw2();
 bjetpt_2_sumet1->Sumw2();
 bjetpt_3_sumet1->Sumw2();


 bjetpt_1_sumet1->Scale(988.8/9987968);
 bjetpt_2_sumet1->Scale(66.3/6000000);
 bjetpt_3_sumet1->Scale(8.15/5998860);

 bjetpt_1_sumet1->Add(bjetpt_2_sumet1);
 bjetpt_1_sumet1->Add(bjetpt_3_sumet1);



TH1F* bhist11_sumet1 = new TH1F("bhist11_sumet1","bhist11_sumet1",16,binning);
TH1F* bhist21_sumet1 = new TH1F("bhist21_sumet1","bhist21_sumet1",16,binning);
TH1F* bhist31_sumet1 = new TH1F("bhist31_sumet1","bhist31_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumet1",vars);

 TH1F *btaupt_1_sumet1 = (TH1F*)gDirectory->Get("bhist11_sumet1");
 TH1F *btaupt_2_sumet1 = (TH1F*)gDirectory->Get("bhist21_sumet1");
 TH1F *btaupt_3_sumet1 = (TH1F*)gDirectory->Get("bhist31_sumet1");

 btaupt_1_sumet1->Sumw2();
 btaupt_2_sumet1->Sumw2();
 btaupt_3_sumet1->Sumw2();


 btaupt_1_sumet1->Scale(988.8/9987968);
 btaupt_2_sumet1->Scale(66.3/6000000);
 btaupt_3_sumet1->Scale(8.15/5998860);

 



 btaupt_1_sumet1->Add(btaupt_2_sumet1);
 btaupt_1_sumet1->Add(btaupt_3_sumet1);
 


 btaupt_1_sumet1->Divide(bjetpt_1_sumet1);
 btaupt_1_sumet1->SetMarkerStyle(20);
 btaupt_1_sumet1->SetMarkerColor(kBlue);


TH1F* ghist1_sumet1 = new TH1F("ghist1_sumet1","ghist1_sumet1",16,binning);
TH1F* ghist2_sumet1 = new TH1F("ghist2_sumet1","ghist2_sumet1",16,binning);
TH1F* ghist3_sumet1 = new TH1F("ghist3_sumet1","ghist3_sumet1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumet1",vars);

 TH1F* gjetpt_1_sumet1 = (TH1F*)gDirectory->Get("ghist1_sumet1");
 TH1F* gjetpt_2_sumet1 = (TH1F*)gDirectory->Get("ghist2_sumet1");
 TH1F* gjetpt_3_sumet1 = (TH1F*)gDirectory->Get("ghist3_sumet1");

 gjetpt_1_sumet1->Sumw2();
 gjetpt_2_sumet1->Sumw2();
 gjetpt_3_sumet1->Sumw2();


 gjetpt_1_sumet1->Scale(988.8/9987968);
 gjetpt_2_sumet1->Scale(66.3/6000000);
 gjetpt_3_sumet1->Scale(8.15/5998860);

 gjetpt_1_sumet1->Add(gjetpt_2_sumet1);
 gjetpt_1_sumet1->Add(gjetpt_3_sumet1);



TH1F* ghist11_sumet1 = new TH1F("ghist11_sumet1","ghist11_sumet1",16,binning);
TH1F* ghist21_sumet1 = new TH1F("ghist21_sumet1","ghist21_sumet1",16,binning);
TH1F* ghist31_sumet1 = new TH1F("ghist31_sumet1","ghist31_sumet1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>0&&sumet<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet1",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumet1",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumet1",vars);

 TH1F *gtaupt_1_sumet1 = (TH1F*)gDirectory->Get("ghist11_sumet1");
 TH1F *gtaupt_2_sumet1 = (TH1F*)gDirectory->Get("ghist21_sumet1");
 TH1F *gtaupt_3_sumet1 = (TH1F*)gDirectory->Get("ghist31_sumet1");

 gtaupt_1_sumet1->Sumw2();
 gtaupt_2_sumet1->Sumw2();
 gtaupt_3_sumet1->Sumw2();


 gtaupt_1_sumet1->Scale(988.8/9987968);
 gtaupt_2_sumet1->Scale(66.3/6000000);
 gtaupt_3_sumet1->Scale(8.15/5998860);

 



 gtaupt_1_sumet1->Add(gtaupt_2_sumet1);
 gtaupt_1_sumet1->Add(gtaupt_3_sumet1);
 


 gtaupt_1_sumet1->Divide(gjetpt_1_sumet1);
 gtaupt_1_sumet1->SetMarkerStyle(20);

TH1F* hist1_sumet2 = new TH1F("hist1_sumet2","hist1_sumet2",16,binning);
TH1F* hist2_sumet2 = new TH1F("hist2_sumet2","hist2_sumet2",16,binning);
TH1F* hist3_sumet2 = new TH1F("hist3_sumet2","hist3_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumet2",vars);

 TH1F* jetpt_1_sumet2 = (TH1F*)gDirectory->Get("hist1_sumet2");
 TH1F* jetpt_2_sumet2 = (TH1F*)gDirectory->Get("hist2_sumet2");
 TH1F* jetpt_3_sumet2 = (TH1F*)gDirectory->Get("hist3_sumet2");

 jetpt_1_sumet2->Sumw2();
 jetpt_2_sumet2->Sumw2();
 jetpt_3_sumet2->Sumw2();


 jetpt_1_sumet2->Scale(988.8/9987968);
 jetpt_2_sumet2->Scale(66.3/6000000);
 jetpt_3_sumet2->Scale(8.15/5998860);

 jetpt_1_sumet2->Add(jetpt_2_sumet2);
 jetpt_1_sumet2->Add(jetpt_3_sumet2);



TH1F* hist11_sumet2 = new TH1F("hist11_sumet2","hist11_sumet2",16,binning);
TH1F* hist21_sumet2 = new TH1F("hist21_sumet2","hist21_sumet2",16,binning);
TH1F* hist31_sumet2 = new TH1F("hist31_sumet2","hist31_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumet2",vars);

 TH1F *taupt_1_sumet2 = (TH1F*)gDirectory->Get("hist11_sumet2");
 TH1F *taupt_2_sumet2 = (TH1F*)gDirectory->Get("hist21_sumet2");
 TH1F *taupt_3_sumet2 = (TH1F*)gDirectory->Get("hist31_sumet2");

 taupt_1_sumet2->Sumw2();
 taupt_2_sumet2->Sumw2();
 taupt_3_sumet2->Sumw2();


 taupt_1_sumet2->Scale(988.8/9987968);
 taupt_2_sumet2->Scale(66.3/6000000);
 taupt_3_sumet2->Scale(8.15/5998860);

 



 taupt_1_sumet2->Add(taupt_2_sumet2);
 taupt_1_sumet2->Add(taupt_3_sumet2);
 


 taupt_1_sumet2->Divide(jetpt_1_sumet2);
 taupt_1_sumet2->SetMarkerStyle(20);
 taupt_1_sumet2->SetMarkerColor(kRed);


TH1F* lhist1_sumet2 = new TH1F("lhist1_sumet2","lhist1_sumet2",16,binning);
TH1F* lhist2_sumet2 = new TH1F("lhist2_sumet2","lhist2_sumet2",16,binning);
TH1F* lhist3_sumet2 = new TH1F("lhist3_sumet2","lhist3_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumet2",vars);

 TH1F* ljetpt_1_sumet2 = (TH1F*)gDirectory->Get("lhist1_sumet2");
 TH1F* ljetpt_2_sumet2 = (TH1F*)gDirectory->Get("lhist2_sumet2");
 TH1F* ljetpt_3_sumet2 = (TH1F*)gDirectory->Get("lhist3_sumet2");

 ljetpt_1_sumet2->Sumw2();
 ljetpt_2_sumet2->Sumw2();
 ljetpt_3_sumet2->Sumw2();


 ljetpt_1_sumet2->Scale(988.8/9987968);
 ljetpt_2_sumet2->Scale(66.3/6000000);
 ljetpt_3_sumet2->Scale(8.15/5998860);

 ljetpt_1_sumet2->Add(ljetpt_2_sumet2);
 ljetpt_1_sumet2->Add(ljetpt_3_sumet2);



TH1F* lhist11_sumet2 = new TH1F("lhist11_sumet2","lhist11_sumet2",16,binning);
TH1F* lhist21_sumet2 = new TH1F("lhist21_sumet2","lhist21_sumet2",16,binning);
TH1F* lhist31_sumet2 = new TH1F("lhist31_sumet2","lhist31_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumet2",vars);

 TH1F *ltaupt_1_sumet2 = (TH1F*)gDirectory->Get("lhist11_sumet2");
 TH1F *ltaupt_2_sumet2 = (TH1F*)gDirectory->Get("lhist21_sumet2");
 TH1F *ltaupt_3_sumet2 = (TH1F*)gDirectory->Get("lhist31_sumet2");

 ltaupt_1_sumet2->Sumw2();
 ltaupt_2_sumet2->Sumw2();
 ltaupt_3_sumet2->Sumw2();


 ltaupt_1_sumet2->Scale(988.8/9987968);
 ltaupt_2_sumet2->Scale(66.3/6000000);
 ltaupt_3_sumet2->Scale(8.15/5998860);

 



 ltaupt_1_sumet2->Add(ltaupt_2_sumet2);
 ltaupt_1_sumet2->Add(ltaupt_3_sumet2);
 


 ltaupt_1_sumet2->Divide(ljetpt_1_sumet2);
 ltaupt_1_sumet2->SetMarkerStyle(20);
 ltaupt_1_sumet2->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet2 = new TH1F("bhist1_sumet2","bhist1_sumet2",16,binning);
TH1F* bhist2_sumet2 = new TH1F("bhist2_sumet2","bhist2_sumet2",16,binning);
TH1F* bhist3_sumet2 = new TH1F("bhist3_sumet2","bhist3_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumet2",vars);

 TH1F* bjetpt_1_sumet2 = (TH1F*)gDirectory->Get("bhist1_sumet2");
 TH1F* bjetpt_2_sumet2 = (TH1F*)gDirectory->Get("bhist2_sumet2");
 TH1F* bjetpt_3_sumet2 = (TH1F*)gDirectory->Get("bhist3_sumet2");

 bjetpt_1_sumet2->Sumw2();
 bjetpt_2_sumet2->Sumw2();
 bjetpt_3_sumet2->Sumw2();


 bjetpt_1_sumet2->Scale(988.8/9987968);
 bjetpt_2_sumet2->Scale(66.3/6000000);
 bjetpt_3_sumet2->Scale(8.15/5998860);

 bjetpt_1_sumet2->Add(bjetpt_2_sumet2);
 bjetpt_1_sumet2->Add(bjetpt_3_sumet2);



TH1F* bhist11_sumet2 = new TH1F("bhist11_sumet2","bhist11_sumet2",16,binning);
TH1F* bhist21_sumet2 = new TH1F("bhist21_sumet2","bhist21_sumet2",16,binning);
TH1F* bhist31_sumet2 = new TH1F("bhist31_sumet2","bhist31_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumet2",vars);

 TH1F *btaupt_1_sumet2 = (TH1F*)gDirectory->Get("bhist11_sumet2");
 TH1F *btaupt_2_sumet2 = (TH1F*)gDirectory->Get("bhist21_sumet2");
 TH1F *btaupt_3_sumet2 = (TH1F*)gDirectory->Get("bhist31_sumet2");

 btaupt_1_sumet2->Sumw2();
 btaupt_2_sumet2->Sumw2();
 btaupt_3_sumet2->Sumw2();


 btaupt_1_sumet2->Scale(988.8/9987968);
 btaupt_2_sumet2->Scale(66.3/6000000);
 btaupt_3_sumet2->Scale(8.15/5998860);

 



 btaupt_1_sumet2->Add(btaupt_2_sumet2);
 btaupt_1_sumet2->Add(btaupt_3_sumet2);
 


 btaupt_1_sumet2->Divide(bjetpt_1_sumet2);
 btaupt_1_sumet2->SetMarkerStyle(20);
 btaupt_1_sumet2->SetMarkerColor(kBlue);


TH1F* ghist1_sumet2 = new TH1F("ghist1_sumet2","ghist1_sumet2",16,binning);
TH1F* ghist2_sumet2 = new TH1F("ghist2_sumet2","ghist2_sumet2",16,binning);
TH1F* ghist3_sumet2 = new TH1F("ghist3_sumet2","ghist3_sumet2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumet2",vars);

 TH1F* gjetpt_1_sumet2 = (TH1F*)gDirectory->Get("ghist1_sumet2");
 TH1F* gjetpt_2_sumet2 = (TH1F*)gDirectory->Get("ghist2_sumet2");
 TH1F* gjetpt_3_sumet2 = (TH1F*)gDirectory->Get("ghist3_sumet2");

 gjetpt_1_sumet2->Sumw2();
 gjetpt_2_sumet2->Sumw2();
 gjetpt_3_sumet2->Sumw2();


 gjetpt_1_sumet2->Scale(988.8/9987968);
 gjetpt_2_sumet2->Scale(66.3/6000000);
 gjetpt_3_sumet2->Scale(8.15/5998860);

 gjetpt_1_sumet2->Add(gjetpt_2_sumet2);
 gjetpt_1_sumet2->Add(gjetpt_3_sumet2);



TH1F* ghist11_sumet2 = new TH1F("ghist11_sumet2","ghist11_sumet2",16,binning);
TH1F* ghist21_sumet2 = new TH1F("ghist21_sumet2","ghist21_sumet2",16,binning);
TH1F* ghist31_sumet2 = new TH1F("ghist31_sumet2","ghist31_sumet2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[0]+"&&sumet<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet2",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumet2",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumet2",vars);

 TH1F *gtaupt_1_sumet2 = (TH1F*)gDirectory->Get("ghist11_sumet2");
 TH1F *gtaupt_2_sumet2 = (TH1F*)gDirectory->Get("ghist21_sumet2");
 TH1F *gtaupt_3_sumet2 = (TH1F*)gDirectory->Get("ghist31_sumet2");

 gtaupt_1_sumet2->Sumw2();
 gtaupt_2_sumet2->Sumw2();
 gtaupt_3_sumet2->Sumw2();


 gtaupt_1_sumet2->Scale(988.8/9987968);
 gtaupt_2_sumet2->Scale(66.3/6000000);
 gtaupt_3_sumet2->Scale(8.15/5998860);

 



 gtaupt_1_sumet2->Add(gtaupt_2_sumet2);
 gtaupt_1_sumet2->Add(gtaupt_3_sumet2);
 


 gtaupt_1_sumet2->Divide(gjetpt_1_sumet2);
 gtaupt_1_sumet2->SetMarkerStyle(20);

TH1F* hist1_sumet3 = new TH1F("hist1_sumet3","hist1_sumet3",16,binning);
TH1F* hist2_sumet3 = new TH1F("hist2_sumet3","hist2_sumet3",16,binning);
TH1F* hist3_sumet3 = new TH1F("hist3_sumet3","hist3_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumet3",vars);

 TH1F* jetpt_1_sumet3 = (TH1F*)gDirectory->Get("hist1_sumet3");
 TH1F* jetpt_2_sumet3 = (TH1F*)gDirectory->Get("hist2_sumet3");
 TH1F* jetpt_3_sumet3 = (TH1F*)gDirectory->Get("hist3_sumet3");

 jetpt_1_sumet3->Sumw2();
 jetpt_2_sumet3->Sumw2();
 jetpt_3_sumet3->Sumw2();


 jetpt_1_sumet3->Scale(988.8/9987968);
 jetpt_2_sumet3->Scale(66.3/6000000);
 jetpt_3_sumet3->Scale(8.15/5998860);

 jetpt_1_sumet3->Add(jetpt_2_sumet3);
 jetpt_1_sumet3->Add(jetpt_3_sumet3);



TH1F* hist11_sumet3 = new TH1F("hist11_sumet3","hist11_sumet3",16,binning);
TH1F* hist21_sumet3 = new TH1F("hist21_sumet3","hist21_sumet3",16,binning);
TH1F* hist31_sumet3 = new TH1F("hist31_sumet3","hist31_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumet3",vars);

 TH1F *taupt_1_sumet3 = (TH1F*)gDirectory->Get("hist11_sumet3");
 TH1F *taupt_2_sumet3 = (TH1F*)gDirectory->Get("hist21_sumet3");
 TH1F *taupt_3_sumet3 = (TH1F*)gDirectory->Get("hist31_sumet3");

 taupt_1_sumet3->Sumw2();
 taupt_2_sumet3->Sumw2();
 taupt_3_sumet3->Sumw2();


 taupt_1_sumet3->Scale(988.8/9987968);
 taupt_2_sumet3->Scale(66.3/6000000);
 taupt_3_sumet3->Scale(8.15/5998860);

 



 taupt_1_sumet3->Add(taupt_2_sumet3);
 taupt_1_sumet3->Add(taupt_3_sumet3);
 


 taupt_1_sumet3->Divide(jetpt_1_sumet3);
 taupt_1_sumet3->SetMarkerStyle(20);
 taupt_1_sumet3->SetMarkerColor(kRed);


TH1F* lhist1_sumet3 = new TH1F("lhist1_sumet3","lhist1_sumet3",16,binning);
TH1F* lhist2_sumet3 = new TH1F("lhist2_sumet3","lhist2_sumet3",16,binning);
TH1F* lhist3_sumet3 = new TH1F("lhist3_sumet3","lhist3_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumet3",vars);

 TH1F* ljetpt_1_sumet3 = (TH1F*)gDirectory->Get("lhist1_sumet3");
 TH1F* ljetpt_2_sumet3 = (TH1F*)gDirectory->Get("lhist2_sumet3");
 TH1F* ljetpt_3_sumet3 = (TH1F*)gDirectory->Get("lhist3_sumet3");

 ljetpt_1_sumet3->Sumw2();
 ljetpt_2_sumet3->Sumw2();
 ljetpt_3_sumet3->Sumw2();


 ljetpt_1_sumet3->Scale(988.8/9987968);
 ljetpt_2_sumet3->Scale(66.3/6000000);
 ljetpt_3_sumet3->Scale(8.15/5998860);

 ljetpt_1_sumet3->Add(ljetpt_2_sumet3);
 ljetpt_1_sumet3->Add(ljetpt_3_sumet3);



TH1F* lhist11_sumet3 = new TH1F("lhist11_sumet3","lhist11_sumet3",16,binning);
TH1F* lhist21_sumet3 = new TH1F("lhist21_sumet3","lhist21_sumet3",16,binning);
TH1F* lhist31_sumet3 = new TH1F("lhist31_sumet3","lhist31_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumet3",vars);

 TH1F *ltaupt_1_sumet3 = (TH1F*)gDirectory->Get("lhist11_sumet3");
 TH1F *ltaupt_2_sumet3 = (TH1F*)gDirectory->Get("lhist21_sumet3");
 TH1F *ltaupt_3_sumet3 = (TH1F*)gDirectory->Get("lhist31_sumet3");

 ltaupt_1_sumet3->Sumw2();
 ltaupt_2_sumet3->Sumw2();
 ltaupt_3_sumet3->Sumw2();


 ltaupt_1_sumet3->Scale(988.8/9987968);
 ltaupt_2_sumet3->Scale(66.3/6000000);
 ltaupt_3_sumet3->Scale(8.15/5998860);

 



 ltaupt_1_sumet3->Add(ltaupt_2_sumet3);
 ltaupt_1_sumet3->Add(ltaupt_3_sumet3);
 


 ltaupt_1_sumet3->Divide(ljetpt_1_sumet3);
 ltaupt_1_sumet3->SetMarkerStyle(20);
 ltaupt_1_sumet3->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet3 = new TH1F("bhist1_sumet3","bhist1_sumet3",16,binning);
TH1F* bhist2_sumet3 = new TH1F("bhist2_sumet3","bhist2_sumet3",16,binning);
TH1F* bhist3_sumet3 = new TH1F("bhist3_sumet3","bhist3_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumet3",vars);

 TH1F* bjetpt_1_sumet3 = (TH1F*)gDirectory->Get("bhist1_sumet3");
 TH1F* bjetpt_2_sumet3 = (TH1F*)gDirectory->Get("bhist2_sumet3");
 TH1F* bjetpt_3_sumet3 = (TH1F*)gDirectory->Get("bhist3_sumet3");

 bjetpt_1_sumet3->Sumw2();
 bjetpt_2_sumet3->Sumw2();
 bjetpt_3_sumet3->Sumw2();


 bjetpt_1_sumet3->Scale(988.8/9987968);
 bjetpt_2_sumet3->Scale(66.3/6000000);
 bjetpt_3_sumet3->Scale(8.15/5998860);

 bjetpt_1_sumet3->Add(bjetpt_2_sumet3);
 bjetpt_1_sumet3->Add(bjetpt_3_sumet3);



TH1F* bhist11_sumet3 = new TH1F("bhist11_sumet3","bhist11_sumet3",16,binning);
TH1F* bhist21_sumet3 = new TH1F("bhist21_sumet3","bhist21_sumet3",16,binning);
TH1F* bhist31_sumet3 = new TH1F("bhist31_sumet3","bhist31_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumet3",vars);

 TH1F *btaupt_1_sumet3 = (TH1F*)gDirectory->Get("bhist11_sumet3");
 TH1F *btaupt_2_sumet3 = (TH1F*)gDirectory->Get("bhist21_sumet3");
 TH1F *btaupt_3_sumet3 = (TH1F*)gDirectory->Get("bhist31_sumet3");

 btaupt_1_sumet3->Sumw2();
 btaupt_2_sumet3->Sumw2();
 btaupt_3_sumet3->Sumw2();


 btaupt_1_sumet3->Scale(988.8/9987968);
 btaupt_2_sumet3->Scale(66.3/6000000);
 btaupt_3_sumet3->Scale(8.15/5998860);

 



 btaupt_1_sumet3->Add(btaupt_2_sumet3);
 btaupt_1_sumet3->Add(btaupt_3_sumet3);
 


 btaupt_1_sumet3->Divide(bjetpt_1_sumet3);
 btaupt_1_sumet3->SetMarkerStyle(20);
 btaupt_1_sumet3->SetMarkerColor(kBlue);


TH1F* ghist1_sumet3 = new TH1F("ghist1_sumet3","ghist1_sumet3",16,binning);
TH1F* ghist2_sumet3 = new TH1F("ghist2_sumet3","ghist2_sumet3",16,binning);
TH1F* ghist3_sumet3 = new TH1F("ghist3_sumet3","ghist3_sumet3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumet3",vars);

 TH1F* gjetpt_1_sumet3 = (TH1F*)gDirectory->Get("ghist1_sumet3");
 TH1F* gjetpt_2_sumet3 = (TH1F*)gDirectory->Get("ghist2_sumet3");
 TH1F* gjetpt_3_sumet3 = (TH1F*)gDirectory->Get("ghist3_sumet3");

 gjetpt_1_sumet3->Sumw2();
 gjetpt_2_sumet3->Sumw2();
 gjetpt_3_sumet3->Sumw2();


 gjetpt_1_sumet3->Scale(988.8/9987968);
 gjetpt_2_sumet3->Scale(66.3/6000000);
 gjetpt_3_sumet3->Scale(8.15/5998860);

 gjetpt_1_sumet3->Add(gjetpt_2_sumet3);
 gjetpt_1_sumet3->Add(gjetpt_3_sumet3);



TH1F* ghist11_sumet3 = new TH1F("ghist11_sumet3","ghist11_sumet3",16,binning);
TH1F* ghist21_sumet3 = new TH1F("ghist21_sumet3","ghist21_sumet3",16,binning);
TH1F* ghist31_sumet3 = new TH1F("ghist31_sumet3","ghist31_sumet3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[1]+"&&sumet<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet3",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumet3",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumet3",vars);

 TH1F *gtaupt_1_sumet3 = (TH1F*)gDirectory->Get("ghist11_sumet3");
 TH1F *gtaupt_2_sumet3 = (TH1F*)gDirectory->Get("ghist21_sumet3");
 TH1F *gtaupt_3_sumet3 = (TH1F*)gDirectory->Get("ghist31_sumet3");

 gtaupt_1_sumet3->Sumw2();
 gtaupt_2_sumet3->Sumw2();
 gtaupt_3_sumet3->Sumw2();


 gtaupt_1_sumet3->Scale(988.8/9987968);
 gtaupt_2_sumet3->Scale(66.3/6000000);
 gtaupt_3_sumet3->Scale(8.15/5998860);

 



 gtaupt_1_sumet3->Add(gtaupt_2_sumet3);
 gtaupt_1_sumet3->Add(gtaupt_3_sumet3);
 


 gtaupt_1_sumet3->Divide(gjetpt_1_sumet3);
 gtaupt_1_sumet3->SetMarkerStyle(20);



TH1F* hist1_sumet4 = new TH1F("hist1_sumet4","hist1_sumet4",16,binning);
TH1F* hist2_sumet4 = new TH1F("hist2_sumet4","hist2_sumet4",16,binning);
TH1F* hist3_sumet4 = new TH1F("hist3_sumet4","hist3_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumet4",vars);

 TH1F* jetpt_1_sumet4 = (TH1F*)gDirectory->Get("hist1_sumet4");
 TH1F* jetpt_2_sumet4 = (TH1F*)gDirectory->Get("hist2_sumet4");
 TH1F* jetpt_3_sumet4 = (TH1F*)gDirectory->Get("hist3_sumet4");

 jetpt_1_sumet4->Sumw2();
 jetpt_2_sumet4->Sumw2();
 jetpt_3_sumet4->Sumw2();


 jetpt_1_sumet4->Scale(988.8/9987968);
 jetpt_2_sumet4->Scale(66.3/6000000);
 jetpt_3_sumet4->Scale(8.15/5998860);

 jetpt_1_sumet4->Add(jetpt_2_sumet4);
 jetpt_1_sumet4->Add(jetpt_3_sumet4);



TH1F* hist11_sumet4 = new TH1F("hist11_sumet4","hist11_sumet4",16,binning);
TH1F* hist21_sumet4 = new TH1F("hist21_sumet4","hist21_sumet4",16,binning);
TH1F* hist31_sumet4 = new TH1F("hist31_sumet4","hist31_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumet4",vars);

 TH1F *taupt_1_sumet4 = (TH1F*)gDirectory->Get("hist11_sumet4");
 TH1F *taupt_2_sumet4 = (TH1F*)gDirectory->Get("hist21_sumet4");
 TH1F *taupt_3_sumet4 = (TH1F*)gDirectory->Get("hist31_sumet4");

 taupt_1_sumet4->Sumw2();
 taupt_2_sumet4->Sumw2();
 taupt_3_sumet4->Sumw2();


 taupt_1_sumet4->Scale(988.8/9987968);
 taupt_2_sumet4->Scale(66.3/6000000);
 taupt_3_sumet4->Scale(8.15/5998860);

 



 taupt_1_sumet4->Add(taupt_2_sumet4);
 taupt_1_sumet4->Add(taupt_3_sumet4);
 


 taupt_1_sumet4->Divide(jetpt_1_sumet4);
 taupt_1_sumet4->SetMarkerStyle(20);
 taupt_1_sumet4->SetMarkerColor(kRed);


TH1F* lhist1_sumet4 = new TH1F("lhist1_sumet4","lhist1_sumet4",16,binning);
TH1F* lhist2_sumet4 = new TH1F("lhist2_sumet4","lhist2_sumet4",16,binning);
TH1F* lhist3_sumet4 = new TH1F("lhist3_sumet4","lhist3_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumet4",vars);

 TH1F* ljetpt_1_sumet4 = (TH1F*)gDirectory->Get("lhist1_sumet4");
 TH1F* ljetpt_2_sumet4 = (TH1F*)gDirectory->Get("lhist2_sumet4");
 TH1F* ljetpt_3_sumet4 = (TH1F*)gDirectory->Get("lhist3_sumet4");

 ljetpt_1_sumet4->Sumw2();
 ljetpt_2_sumet4->Sumw2();
 ljetpt_3_sumet4->Sumw2();


 ljetpt_1_sumet4->Scale(988.8/9987968);
 ljetpt_2_sumet4->Scale(66.3/6000000);
 ljetpt_3_sumet4->Scale(8.15/5998860);

 ljetpt_1_sumet4->Add(ljetpt_2_sumet4);
 ljetpt_1_sumet4->Add(ljetpt_3_sumet4);



TH1F* lhist11_sumet4 = new TH1F("lhist11_sumet4","lhist11_sumet4",16,binning);
TH1F* lhist21_sumet4 = new TH1F("lhist21_sumet4","lhist21_sumet4",16,binning);
TH1F* lhist31_sumet4 = new TH1F("lhist31_sumet4","lhist31_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumet4",vars);

 TH1F *ltaupt_1_sumet4 = (TH1F*)gDirectory->Get("lhist11_sumet4");
 TH1F *ltaupt_2_sumet4 = (TH1F*)gDirectory->Get("lhist21_sumet4");
 TH1F *ltaupt_3_sumet4 = (TH1F*)gDirectory->Get("lhist31_sumet4");

 ltaupt_1_sumet4->Sumw2();
 ltaupt_2_sumet4->Sumw2();
 ltaupt_3_sumet4->Sumw2();


 ltaupt_1_sumet4->Scale(988.8/9987968);
 ltaupt_2_sumet4->Scale(66.3/6000000);
 ltaupt_3_sumet4->Scale(8.15/5998860);

 



 ltaupt_1_sumet4->Add(ltaupt_2_sumet4);
 ltaupt_1_sumet4->Add(ltaupt_3_sumet4);
 


 ltaupt_1_sumet4->Divide(ljetpt_1_sumet4);
 ltaupt_1_sumet4->SetMarkerStyle(20);
 ltaupt_1_sumet4->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet4 = new TH1F("bhist1_sumet4","bhist1_sumet4",16,binning);
TH1F* bhist2_sumet4 = new TH1F("bhist2_sumet4","bhist2_sumet4",16,binning);
TH1F* bhist3_sumet4 = new TH1F("bhist3_sumet4","bhist3_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumet4",vars);

 TH1F* bjetpt_1_sumet4 = (TH1F*)gDirectory->Get("bhist1_sumet4");
 TH1F* bjetpt_2_sumet4 = (TH1F*)gDirectory->Get("bhist2_sumet4");
 TH1F* bjetpt_3_sumet4 = (TH1F*)gDirectory->Get("bhist3_sumet4");

 bjetpt_1_sumet4->Sumw2();
 bjetpt_2_sumet4->Sumw2();
 bjetpt_3_sumet4->Sumw2();


 bjetpt_1_sumet4->Scale(988.8/9987968);
 bjetpt_2_sumet4->Scale(66.3/6000000);
 bjetpt_3_sumet4->Scale(8.15/5998860);

 bjetpt_1_sumet4->Add(bjetpt_2_sumet4);
 bjetpt_1_sumet4->Add(bjetpt_3_sumet4);



TH1F* bhist11_sumet4 = new TH1F("bhist11_sumet4","bhist11_sumet4",16,binning);
TH1F* bhist21_sumet4 = new TH1F("bhist21_sumet4","bhist21_sumet4",16,binning);
TH1F* bhist31_sumet4 = new TH1F("bhist31_sumet4","bhist31_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumet4",vars);

 TH1F *btaupt_1_sumet4 = (TH1F*)gDirectory->Get("bhist11_sumet4");
 TH1F *btaupt_2_sumet4 = (TH1F*)gDirectory->Get("bhist21_sumet4");
 TH1F *btaupt_3_sumet4 = (TH1F*)gDirectory->Get("bhist31_sumet4");

 btaupt_1_sumet4->Sumw2();
 btaupt_2_sumet4->Sumw2();
 btaupt_3_sumet4->Sumw2();


 btaupt_1_sumet4->Scale(988.8/9987968);
 btaupt_2_sumet4->Scale(66.3/6000000);
 btaupt_3_sumet4->Scale(8.15/5998860);

 



 btaupt_1_sumet4->Add(btaupt_2_sumet4);
 btaupt_1_sumet4->Add(btaupt_3_sumet4);
 


 btaupt_1_sumet4->Divide(bjetpt_1_sumet4);
 btaupt_1_sumet4->SetMarkerStyle(20);
 btaupt_1_sumet4->SetMarkerColor(kBlue);


TH1F* ghist1_sumet4 = new TH1F("ghist1_sumet4","ghist1_sumet4",16,binning);
TH1F* ghist2_sumet4 = new TH1F("ghist2_sumet4","ghist2_sumet4",16,binning);
TH1F* ghist3_sumet4 = new TH1F("ghist3_sumet4","ghist3_sumet4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumet4",vars);

 TH1F* gjetpt_1_sumet4 = (TH1F*)gDirectory->Get("ghist1_sumet4");
 TH1F* gjetpt_2_sumet4 = (TH1F*)gDirectory->Get("ghist2_sumet4");
 TH1F* gjetpt_3_sumet4 = (TH1F*)gDirectory->Get("ghist3_sumet4");

 gjetpt_1_sumet4->Sumw2();
 gjetpt_2_sumet4->Sumw2();
 gjetpt_3_sumet4->Sumw2();


 gjetpt_1_sumet4->Scale(988.8/9987968);
 gjetpt_2_sumet4->Scale(66.3/6000000);
 gjetpt_3_sumet4->Scale(8.15/5998860);

 gjetpt_1_sumet4->Add(gjetpt_2_sumet4);
 gjetpt_1_sumet4->Add(gjetpt_3_sumet4);



TH1F* ghist11_sumet4 = new TH1F("ghist11_sumet4","ghist11_sumet4",16,binning);
TH1F* ghist21_sumet4 = new TH1F("ghist21_sumet4","ghist21_sumet4",16,binning);
TH1F* ghist31_sumet4 = new TH1F("ghist31_sumet4","ghist31_sumet4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[2]+"&&sumet<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet4",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumet4",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumet4",vars);

 TH1F *gtaupt_1_sumet4 = (TH1F*)gDirectory->Get("ghist11_sumet4");
 TH1F *gtaupt_2_sumet4 = (TH1F*)gDirectory->Get("ghist21_sumet4");
 TH1F *gtaupt_3_sumet4 = (TH1F*)gDirectory->Get("ghist31_sumet4");

 gtaupt_1_sumet4->Sumw2();
 gtaupt_2_sumet4->Sumw2();
 gtaupt_3_sumet4->Sumw2();


 gtaupt_1_sumet4->Scale(988.8/9987968);
 gtaupt_2_sumet4->Scale(66.3/6000000);
 gtaupt_3_sumet4->Scale(8.15/5998860);

 



 gtaupt_1_sumet4->Add(gtaupt_2_sumet4);
 gtaupt_1_sumet4->Add(gtaupt_3_sumet4);
 


 gtaupt_1_sumet4->Divide(gjetpt_1_sumet4);
 gtaupt_1_sumet4->SetMarkerStyle(20);

TH1F* hist1_sumet5 = new TH1F("hist1_sumet5","hist1_sumet5",16,binning);
TH1F* hist2_sumet5 = new TH1F("hist2_sumet5","hist2_sumet5",16,binning);
TH1F* hist3_sumet5 = new TH1F("hist3_sumet5","hist3_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumet5",vars);

 TH1F* jetpt_1_sumet5 = (TH1F*)gDirectory->Get("hist1_sumet5");
 TH1F* jetpt_2_sumet5 = (TH1F*)gDirectory->Get("hist2_sumet5");
 TH1F* jetpt_3_sumet5 = (TH1F*)gDirectory->Get("hist3_sumet5");

 jetpt_1_sumet5->Sumw2();
 jetpt_2_sumet5->Sumw2();
 jetpt_3_sumet5->Sumw2();


 jetpt_1_sumet5->Scale(988.8/9987968);
 jetpt_2_sumet5->Scale(66.3/6000000);
 jetpt_3_sumet5->Scale(8.15/5998860);

 jetpt_1_sumet5->Add(jetpt_2_sumet5);
 jetpt_1_sumet5->Add(jetpt_3_sumet5);



TH1F* hist11_sumet5 = new TH1F("hist11_sumet5","hist11_sumet5",16,binning);
TH1F* hist21_sumet5 = new TH1F("hist21_sumet5","hist21_sumet5",16,binning);
TH1F* hist31_sumet5 = new TH1F("hist31_sumet5","hist31_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumet5",vars);

 TH1F *taupt_1_sumet5 = (TH1F*)gDirectory->Get("hist11_sumet5");
 TH1F *taupt_2_sumet5 = (TH1F*)gDirectory->Get("hist21_sumet5");
 TH1F *taupt_3_sumet5 = (TH1F*)gDirectory->Get("hist31_sumet5");

 taupt_1_sumet5->Sumw2();
 taupt_2_sumet5->Sumw2();
 taupt_3_sumet5->Sumw2();


 taupt_1_sumet5->Scale(988.8/9987968);
 taupt_2_sumet5->Scale(66.3/6000000);
 taupt_3_sumet5->Scale(8.15/5998860);

 



 taupt_1_sumet5->Add(taupt_2_sumet5);
 taupt_1_sumet5->Add(taupt_3_sumet5);
 


 taupt_1_sumet5->Divide(jetpt_1_sumet5);
 taupt_1_sumet5->SetMarkerStyle(20);
 taupt_1_sumet5->SetMarkerColor(kRed);


TH1F* lhist1_sumet5 = new TH1F("lhist1_sumet5","lhist1_sumet5",16,binning);
TH1F* lhist2_sumet5 = new TH1F("lhist2_sumet5","lhist2_sumet5",16,binning);
TH1F* lhist3_sumet5 = new TH1F("lhist3_sumet5","lhist3_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumet5",vars);

 TH1F* ljetpt_1_sumet5 = (TH1F*)gDirectory->Get("lhist1_sumet5");
 TH1F* ljetpt_2_sumet5 = (TH1F*)gDirectory->Get("lhist2_sumet5");
 TH1F* ljetpt_3_sumet5 = (TH1F*)gDirectory->Get("lhist3_sumet5");

 ljetpt_1_sumet5->Sumw2();
 ljetpt_2_sumet5->Sumw2();
 ljetpt_3_sumet5->Sumw2();


 ljetpt_1_sumet5->Scale(988.8/9987968);
 ljetpt_2_sumet5->Scale(66.3/6000000);
 ljetpt_3_sumet5->Scale(8.15/5998860);

 ljetpt_1_sumet5->Add(ljetpt_2_sumet5);
 ljetpt_1_sumet5->Add(ljetpt_3_sumet5);



TH1F* lhist11_sumet5 = new TH1F("lhist11_sumet5","lhist11_sumet5",16,binning);
TH1F* lhist21_sumet5 = new TH1F("lhist21_sumet5","lhist21_sumet5",16,binning);
TH1F* lhist31_sumet5 = new TH1F("lhist31_sumet5","lhist31_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumet5",vars);

 TH1F *ltaupt_1_sumet5 = (TH1F*)gDirectory->Get("lhist11_sumet5");
 TH1F *ltaupt_2_sumet5 = (TH1F*)gDirectory->Get("lhist21_sumet5");
 TH1F *ltaupt_3_sumet5 = (TH1F*)gDirectory->Get("lhist31_sumet5");

 ltaupt_1_sumet5->Sumw2();
 ltaupt_2_sumet5->Sumw2();
 ltaupt_3_sumet5->Sumw2();


 ltaupt_1_sumet5->Scale(988.8/9987968);
 ltaupt_2_sumet5->Scale(66.3/6000000);
 ltaupt_3_sumet5->Scale(8.15/5998860);

 



 ltaupt_1_sumet5->Add(ltaupt_2_sumet5);
 ltaupt_1_sumet5->Add(ltaupt_3_sumet5);
 


 ltaupt_1_sumet5->Divide(ljetpt_1_sumet5);
 ltaupt_1_sumet5->SetMarkerStyle(20);
 ltaupt_1_sumet5->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumet5 = new TH1F("bhist1_sumet5","bhist1_sumet5",16,binning);
TH1F* bhist2_sumet5 = new TH1F("bhist2_sumet5","bhist2_sumet5",16,binning);
TH1F* bhist3_sumet5 = new TH1F("bhist3_sumet5","bhist3_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumet5",vars);

 TH1F* bjetpt_1_sumet5 = (TH1F*)gDirectory->Get("bhist1_sumet5");
 TH1F* bjetpt_2_sumet5 = (TH1F*)gDirectory->Get("bhist2_sumet5");
 TH1F* bjetpt_3_sumet5 = (TH1F*)gDirectory->Get("bhist3_sumet5");

 bjetpt_1_sumet5->Sumw2();
 bjetpt_2_sumet5->Sumw2();
 bjetpt_3_sumet5->Sumw2();


 bjetpt_1_sumet5->Scale(988.8/9987968);
 bjetpt_2_sumet5->Scale(66.3/6000000);
 bjetpt_3_sumet5->Scale(8.15/5998860);

 bjetpt_1_sumet5->Add(bjetpt_2_sumet5);
 bjetpt_1_sumet5->Add(bjetpt_3_sumet5);



TH1F* bhist11_sumet5 = new TH1F("bhist11_sumet5","bhist11_sumet5",16,binning);
TH1F* bhist21_sumet5 = new TH1F("bhist21_sumet5","bhist21_sumet5",16,binning);
TH1F* bhist31_sumet5 = new TH1F("bhist31_sumet5","bhist31_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumet5",vars);

 TH1F *btaupt_1_sumet5 = (TH1F*)gDirectory->Get("bhist11_sumet5");
 TH1F *btaupt_2_sumet5 = (TH1F*)gDirectory->Get("bhist21_sumet5");
 TH1F *btaupt_3_sumet5 = (TH1F*)gDirectory->Get("bhist31_sumet5");

 btaupt_1_sumet5->Sumw2();
 btaupt_2_sumet5->Sumw2();
 btaupt_3_sumet5->Sumw2();


 btaupt_1_sumet5->Scale(988.8/9987968);
 btaupt_2_sumet5->Scale(66.3/6000000);
 btaupt_3_sumet5->Scale(8.15/5998860);

 



 btaupt_1_sumet5->Add(btaupt_2_sumet5);
 btaupt_1_sumet5->Add(btaupt_3_sumet5);
 


 btaupt_1_sumet5->Divide(bjetpt_1_sumet5);
 btaupt_1_sumet5->SetMarkerStyle(20);
 btaupt_1_sumet5->SetMarkerColor(kBlue);


TH1F* ghist1_sumet5 = new TH1F("ghist1_sumet5","ghist1_sumet5",16,binning);
TH1F* ghist2_sumet5 = new TH1F("ghist2_sumet5","ghist2_sumet5",16,binning);
TH1F* ghist3_sumet5 = new TH1F("ghist3_sumet5","ghist3_sumet5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumet5",vars);

 TH1F* gjetpt_1_sumet5 = (TH1F*)gDirectory->Get("ghist1_sumet5");
 TH1F* gjetpt_2_sumet5 = (TH1F*)gDirectory->Get("ghist2_sumet5");
 TH1F* gjetpt_3_sumet5 = (TH1F*)gDirectory->Get("ghist3_sumet5");

 gjetpt_1_sumet5->Sumw2();
 gjetpt_2_sumet5->Sumw2();
 gjetpt_3_sumet5->Sumw2();


 gjetpt_1_sumet5->Scale(988.8/9987968);
 gjetpt_2_sumet5->Scale(66.3/6000000);
 gjetpt_3_sumet5->Scale(8.15/5998860);

 gjetpt_1_sumet5->Add(gjetpt_2_sumet5);
 gjetpt_1_sumet5->Add(gjetpt_3_sumet5);



TH1F* ghist11_sumet5 = new TH1F("ghist11_sumet5","ghist11_sumet5",16,binning);
TH1F* ghist21_sumet5 = new TH1F("ghist21_sumet5","ghist21_sumet5",16,binning);
TH1F* ghist31_sumet5 = new TH1F("ghist31_sumet5","ghist31_sumet5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumet>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumet5",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumet5",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumet5",vars);

 TH1F *gtaupt_1_sumet5 = (TH1F*)gDirectory->Get("ghist11_sumet5");
 TH1F *gtaupt_2_sumet5 = (TH1F*)gDirectory->Get("ghist21_sumet5");
 TH1F *gtaupt_3_sumet5 = (TH1F*)gDirectory->Get("ghist31_sumet5");

 gtaupt_1_sumet5->Sumw2();
 gtaupt_2_sumet5->Sumw2();
 gtaupt_3_sumet5->Sumw2();


 gtaupt_1_sumet5->Scale(988.8/9987968);
 gtaupt_2_sumet5->Scale(66.3/6000000);
 gtaupt_3_sumet5->Scale(8.15/5998860);

 



 gtaupt_1_sumet5->Add(gtaupt_2_sumet5);
 gtaupt_1_sumet5->Add(gtaupt_3_sumet5);
 


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
 TFile *outfile = new TFile("Jan29_qcd_fakerates_sumet.root","RECREATE");
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
