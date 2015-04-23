void drawJetTauFakeRateNtracks(){
 TFile *qcd_1 = TFile::Open("Jan29_QCD_15-30.root");
 TFile *qcd_2 = TFile::Open("Jan29_QCD_30-50.root");
 TFile *qcd_3 = TFile::Open("Jan29_QCD_50-80.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));
 TTree *qcd_tree_2 = dynamic_cast<TTree*>(qcd_2->Get("ntuple"));
 TTree *qcd_tree_3 = dynamic_cast<TTree*>(qcd_3->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"20","30","40","50"};
TH1F* hist1_ntracks1 = new TH1F("hist1_ntracks1","hist1_ntracks1",16,binning);
TH1F* hist2_ntracks1 = new TH1F("hist2_ntracks1","hist2_ntracks1",16,binning);
TH1F* hist3_ntracks1 = new TH1F("hist3_ntracks1","hist3_ntracks1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>hist2_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>hist3_ntracks1",vars);

 TH1F* jetpt_1_ntracks1 = (TH1F*)gDirectory->Get("hist1_ntracks1");
 TH1F* jetpt_2_ntracks1 = (TH1F*)gDirectory->Get("hist2_ntracks1");
 TH1F* jetpt_3_ntracks1 = (TH1F*)gDirectory->Get("hist3_ntracks1");

 jetpt_1_ntracks1->Sumw2();
 jetpt_2_ntracks1->Sumw2();
 jetpt_3_ntracks1->Sumw2();


 jetpt_1_ntracks1->Scale(988.8/9987968);
 jetpt_2_ntracks1->Scale(66.3/6000000);
 jetpt_3_ntracks1->Scale(8.15/5998860);

 jetpt_1_ntracks1->Add(jetpt_2_ntracks1);
 jetpt_1_ntracks1->Add(jetpt_3_ntracks1);



TH1F* hist11_ntracks1 = new TH1F("hist11_ntracks1","hist11_ntracks1",16,binning);
TH1F* hist21_ntracks1 = new TH1F("hist21_ntracks1","hist21_ntracks1",16,binning);
TH1F* hist31_ntracks1 = new TH1F("hist31_ntracks1","hist31_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>hist21_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>hist31_ntracks1",vars);

 TH1F *taupt_1_ntracks1 = (TH1F*)gDirectory->Get("hist11_ntracks1");
 TH1F *taupt_2_ntracks1 = (TH1F*)gDirectory->Get("hist21_ntracks1");
 TH1F *taupt_3_ntracks1 = (TH1F*)gDirectory->Get("hist31_ntracks1");

 taupt_1_ntracks1->Sumw2();
 taupt_2_ntracks1->Sumw2();
 taupt_3_ntracks1->Sumw2();


 taupt_1_ntracks1->Scale(988.8/9987968);
 taupt_2_ntracks1->Scale(66.3/6000000);
 taupt_3_ntracks1->Scale(8.15/5998860);

 



 taupt_1_ntracks1->Add(taupt_2_ntracks1);
 taupt_1_ntracks1->Add(taupt_3_ntracks1);
 


 taupt_1_ntracks1->Divide(jetpt_1_ntracks1);
 taupt_1_ntracks1->SetMarkerStyle(20);
 taupt_1_ntracks1->SetMarkerColor(kRed);


TH1F* lhist1_ntracks1 = new TH1F("lhist1_ntracks1","lhist1_ntracks1",16,binning);
TH1F* lhist2_ntracks1 = new TH1F("lhist2_ntracks1","lhist2_ntracks1",16,binning);
TH1F* lhist3_ntracks1 = new TH1F("lhist3_ntracks1","lhist3_ntracks1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_ntracks1",vars);

 TH1F* ljetpt_1_ntracks1 = (TH1F*)gDirectory->Get("lhist1_ntracks1");
 TH1F* ljetpt_2_ntracks1 = (TH1F*)gDirectory->Get("lhist2_ntracks1");
 TH1F* ljetpt_3_ntracks1 = (TH1F*)gDirectory->Get("lhist3_ntracks1");

 ljetpt_1_ntracks1->Sumw2();
 ljetpt_2_ntracks1->Sumw2();
 ljetpt_3_ntracks1->Sumw2();


 ljetpt_1_ntracks1->Scale(988.8/9987968);
 ljetpt_2_ntracks1->Scale(66.3/6000000);
 ljetpt_3_ntracks1->Scale(8.15/5998860);

 ljetpt_1_ntracks1->Add(ljetpt_2_ntracks1);
 ljetpt_1_ntracks1->Add(ljetpt_3_ntracks1);



TH1F* lhist11_ntracks1 = new TH1F("lhist11_ntracks1","lhist11_ntracks1",16,binning);
TH1F* lhist21_ntracks1 = new TH1F("lhist21_ntracks1","lhist21_ntracks1",16,binning);
TH1F* lhist31_ntracks1 = new TH1F("lhist31_ntracks1","lhist31_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_ntracks1",vars);

 TH1F *ltaupt_1_ntracks1 = (TH1F*)gDirectory->Get("lhist11_ntracks1");
 TH1F *ltaupt_2_ntracks1 = (TH1F*)gDirectory->Get("lhist21_ntracks1");
 TH1F *ltaupt_3_ntracks1 = (TH1F*)gDirectory->Get("lhist31_ntracks1");

 ltaupt_1_ntracks1->Sumw2();
 ltaupt_2_ntracks1->Sumw2();
 ltaupt_3_ntracks1->Sumw2();


 ltaupt_1_ntracks1->Scale(988.8/9987968);
 ltaupt_2_ntracks1->Scale(66.3/6000000);
 ltaupt_3_ntracks1->Scale(8.15/5998860);

 



 ltaupt_1_ntracks1->Add(ltaupt_2_ntracks1);
 ltaupt_1_ntracks1->Add(ltaupt_3_ntracks1);
 


 ltaupt_1_ntracks1->Divide(ljetpt_1_ntracks1);
 ltaupt_1_ntracks1->SetMarkerStyle(20);
 ltaupt_1_ntracks1->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks1 = new TH1F("bhist1_ntracks1","bhist1_ntracks1",16,binning);
TH1F* bhist2_ntracks1 = new TH1F("bhist2_ntracks1","bhist2_ntracks1",16,binning);
TH1F* bhist3_ntracks1 = new TH1F("bhist3_ntracks1","bhist3_ntracks1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_ntracks1",vars);

 TH1F* bjetpt_1_ntracks1 = (TH1F*)gDirectory->Get("bhist1_ntracks1");
 TH1F* bjetpt_2_ntracks1 = (TH1F*)gDirectory->Get("bhist2_ntracks1");
 TH1F* bjetpt_3_ntracks1 = (TH1F*)gDirectory->Get("bhist3_ntracks1");

 bjetpt_1_ntracks1->Sumw2();
 bjetpt_2_ntracks1->Sumw2();
 bjetpt_3_ntracks1->Sumw2();


 bjetpt_1_ntracks1->Scale(988.8/9987968);
 bjetpt_2_ntracks1->Scale(66.3/6000000);
 bjetpt_3_ntracks1->Scale(8.15/5998860);

 bjetpt_1_ntracks1->Add(bjetpt_2_ntracks1);
 bjetpt_1_ntracks1->Add(bjetpt_3_ntracks1);



TH1F* bhist11_ntracks1 = new TH1F("bhist11_ntracks1","bhist11_ntracks1",16,binning);
TH1F* bhist21_ntracks1 = new TH1F("bhist21_ntracks1","bhist21_ntracks1",16,binning);
TH1F* bhist31_ntracks1 = new TH1F("bhist31_ntracks1","bhist31_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_ntracks1",vars);

 TH1F *btaupt_1_ntracks1 = (TH1F*)gDirectory->Get("bhist11_ntracks1");
 TH1F *btaupt_2_ntracks1 = (TH1F*)gDirectory->Get("bhist21_ntracks1");
 TH1F *btaupt_3_ntracks1 = (TH1F*)gDirectory->Get("bhist31_ntracks1");

 btaupt_1_ntracks1->Sumw2();
 btaupt_2_ntracks1->Sumw2();
 btaupt_3_ntracks1->Sumw2();


 btaupt_1_ntracks1->Scale(988.8/9987968);
 btaupt_2_ntracks1->Scale(66.3/6000000);
 btaupt_3_ntracks1->Scale(8.15/5998860);

 



 btaupt_1_ntracks1->Add(btaupt_2_ntracks1);
 btaupt_1_ntracks1->Add(btaupt_3_ntracks1);
 


 btaupt_1_ntracks1->Divide(bjetpt_1_ntracks1);
 btaupt_1_ntracks1->SetMarkerStyle(20);
 btaupt_1_ntracks1->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks1 = new TH1F("ghist1_ntracks1","ghist1_ntracks1",16,binning);
TH1F* ghist2_ntracks1 = new TH1F("ghist2_ntracks1","ghist2_ntracks1",16,binning);
TH1F* ghist3_ntracks1 = new TH1F("ghist3_ntracks1","ghist3_ntracks1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_ntracks1",vars);

 TH1F* gjetpt_1_ntracks1 = (TH1F*)gDirectory->Get("ghist1_ntracks1");
 TH1F* gjetpt_2_ntracks1 = (TH1F*)gDirectory->Get("ghist2_ntracks1");
 TH1F* gjetpt_3_ntracks1 = (TH1F*)gDirectory->Get("ghist3_ntracks1");

 gjetpt_1_ntracks1->Sumw2();
 gjetpt_2_ntracks1->Sumw2();
 gjetpt_3_ntracks1->Sumw2();


 gjetpt_1_ntracks1->Scale(988.8/9987968);
 gjetpt_2_ntracks1->Scale(66.3/6000000);
 gjetpt_3_ntracks1->Scale(8.15/5998860);

 gjetpt_1_ntracks1->Add(gjetpt_2_ntracks1);
 gjetpt_1_ntracks1->Add(gjetpt_3_ntracks1);



TH1F* ghist11_ntracks1 = new TH1F("ghist11_ntracks1","ghist11_ntracks1",16,binning);
TH1F* ghist21_ntracks1 = new TH1F("ghist21_ntracks1","ghist21_ntracks1",16,binning);
TH1F* ghist31_ntracks1 = new TH1F("ghist31_ntracks1","ghist31_ntracks1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>0&&ntracks<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks1",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_ntracks1",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_ntracks1",vars);

 TH1F *gtaupt_1_ntracks1 = (TH1F*)gDirectory->Get("ghist11_ntracks1");
 TH1F *gtaupt_2_ntracks1 = (TH1F*)gDirectory->Get("ghist21_ntracks1");
 TH1F *gtaupt_3_ntracks1 = (TH1F*)gDirectory->Get("ghist31_ntracks1");

 gtaupt_1_ntracks1->Sumw2();
 gtaupt_2_ntracks1->Sumw2();
 gtaupt_3_ntracks1->Sumw2();


 gtaupt_1_ntracks1->Scale(988.8/9987968);
 gtaupt_2_ntracks1->Scale(66.3/6000000);
 gtaupt_3_ntracks1->Scale(8.15/5998860);

 



 gtaupt_1_ntracks1->Add(gtaupt_2_ntracks1);
 gtaupt_1_ntracks1->Add(gtaupt_3_ntracks1);
 


 gtaupt_1_ntracks1->Divide(gjetpt_1_ntracks1);
 gtaupt_1_ntracks1->SetMarkerStyle(20);

TH1F* hist1_ntracks2 = new TH1F("hist1_ntracks2","hist1_ntracks2",16,binning);
TH1F* hist2_ntracks2 = new TH1F("hist2_ntracks2","hist2_ntracks2",16,binning);
TH1F* hist3_ntracks2 = new TH1F("hist3_ntracks2","hist3_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>hist2_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>hist3_ntracks2",vars);

 TH1F* jetpt_1_ntracks2 = (TH1F*)gDirectory->Get("hist1_ntracks2");
 TH1F* jetpt_2_ntracks2 = (TH1F*)gDirectory->Get("hist2_ntracks2");
 TH1F* jetpt_3_ntracks2 = (TH1F*)gDirectory->Get("hist3_ntracks2");

 jetpt_1_ntracks2->Sumw2();
 jetpt_2_ntracks2->Sumw2();
 jetpt_3_ntracks2->Sumw2();


 jetpt_1_ntracks2->Scale(988.8/9987968);
 jetpt_2_ntracks2->Scale(66.3/6000000);
 jetpt_3_ntracks2->Scale(8.15/5998860);

 jetpt_1_ntracks2->Add(jetpt_2_ntracks2);
 jetpt_1_ntracks2->Add(jetpt_3_ntracks2);



TH1F* hist11_ntracks2 = new TH1F("hist11_ntracks2","hist11_ntracks2",16,binning);
TH1F* hist21_ntracks2 = new TH1F("hist21_ntracks2","hist21_ntracks2",16,binning);
TH1F* hist31_ntracks2 = new TH1F("hist31_ntracks2","hist31_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>hist21_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>hist31_ntracks2",vars);

 TH1F *taupt_1_ntracks2 = (TH1F*)gDirectory->Get("hist11_ntracks2");
 TH1F *taupt_2_ntracks2 = (TH1F*)gDirectory->Get("hist21_ntracks2");
 TH1F *taupt_3_ntracks2 = (TH1F*)gDirectory->Get("hist31_ntracks2");

 taupt_1_ntracks2->Sumw2();
 taupt_2_ntracks2->Sumw2();
 taupt_3_ntracks2->Sumw2();


 taupt_1_ntracks2->Scale(988.8/9987968);
 taupt_2_ntracks2->Scale(66.3/6000000);
 taupt_3_ntracks2->Scale(8.15/5998860);

 



 taupt_1_ntracks2->Add(taupt_2_ntracks2);
 taupt_1_ntracks2->Add(taupt_3_ntracks2);
 


 taupt_1_ntracks2->Divide(jetpt_1_ntracks2);
 taupt_1_ntracks2->SetMarkerStyle(20);
 taupt_1_ntracks2->SetMarkerColor(kRed);


TH1F* lhist1_ntracks2 = new TH1F("lhist1_ntracks2","lhist1_ntracks2",16,binning);
TH1F* lhist2_ntracks2 = new TH1F("lhist2_ntracks2","lhist2_ntracks2",16,binning);
TH1F* lhist3_ntracks2 = new TH1F("lhist3_ntracks2","lhist3_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_ntracks2",vars);

 TH1F* ljetpt_1_ntracks2 = (TH1F*)gDirectory->Get("lhist1_ntracks2");
 TH1F* ljetpt_2_ntracks2 = (TH1F*)gDirectory->Get("lhist2_ntracks2");
 TH1F* ljetpt_3_ntracks2 = (TH1F*)gDirectory->Get("lhist3_ntracks2");

 ljetpt_1_ntracks2->Sumw2();
 ljetpt_2_ntracks2->Sumw2();
 ljetpt_3_ntracks2->Sumw2();


 ljetpt_1_ntracks2->Scale(988.8/9987968);
 ljetpt_2_ntracks2->Scale(66.3/6000000);
 ljetpt_3_ntracks2->Scale(8.15/5998860);

 ljetpt_1_ntracks2->Add(ljetpt_2_ntracks2);
 ljetpt_1_ntracks2->Add(ljetpt_3_ntracks2);



TH1F* lhist11_ntracks2 = new TH1F("lhist11_ntracks2","lhist11_ntracks2",16,binning);
TH1F* lhist21_ntracks2 = new TH1F("lhist21_ntracks2","lhist21_ntracks2",16,binning);
TH1F* lhist31_ntracks2 = new TH1F("lhist31_ntracks2","lhist31_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_ntracks2",vars);

 TH1F *ltaupt_1_ntracks2 = (TH1F*)gDirectory->Get("lhist11_ntracks2");
 TH1F *ltaupt_2_ntracks2 = (TH1F*)gDirectory->Get("lhist21_ntracks2");
 TH1F *ltaupt_3_ntracks2 = (TH1F*)gDirectory->Get("lhist31_ntracks2");

 ltaupt_1_ntracks2->Sumw2();
 ltaupt_2_ntracks2->Sumw2();
 ltaupt_3_ntracks2->Sumw2();


 ltaupt_1_ntracks2->Scale(988.8/9987968);
 ltaupt_2_ntracks2->Scale(66.3/6000000);
 ltaupt_3_ntracks2->Scale(8.15/5998860);

 



 ltaupt_1_ntracks2->Add(ltaupt_2_ntracks2);
 ltaupt_1_ntracks2->Add(ltaupt_3_ntracks2);
 


 ltaupt_1_ntracks2->Divide(ljetpt_1_ntracks2);
 ltaupt_1_ntracks2->SetMarkerStyle(20);
 ltaupt_1_ntracks2->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks2 = new TH1F("bhist1_ntracks2","bhist1_ntracks2",16,binning);
TH1F* bhist2_ntracks2 = new TH1F("bhist2_ntracks2","bhist2_ntracks2",16,binning);
TH1F* bhist3_ntracks2 = new TH1F("bhist3_ntracks2","bhist3_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_ntracks2",vars);

 TH1F* bjetpt_1_ntracks2 = (TH1F*)gDirectory->Get("bhist1_ntracks2");
 TH1F* bjetpt_2_ntracks2 = (TH1F*)gDirectory->Get("bhist2_ntracks2");
 TH1F* bjetpt_3_ntracks2 = (TH1F*)gDirectory->Get("bhist3_ntracks2");

 bjetpt_1_ntracks2->Sumw2();
 bjetpt_2_ntracks2->Sumw2();
 bjetpt_3_ntracks2->Sumw2();


 bjetpt_1_ntracks2->Scale(988.8/9987968);
 bjetpt_2_ntracks2->Scale(66.3/6000000);
 bjetpt_3_ntracks2->Scale(8.15/5998860);

 bjetpt_1_ntracks2->Add(bjetpt_2_ntracks2);
 bjetpt_1_ntracks2->Add(bjetpt_3_ntracks2);



TH1F* bhist11_ntracks2 = new TH1F("bhist11_ntracks2","bhist11_ntracks2",16,binning);
TH1F* bhist21_ntracks2 = new TH1F("bhist21_ntracks2","bhist21_ntracks2",16,binning);
TH1F* bhist31_ntracks2 = new TH1F("bhist31_ntracks2","bhist31_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_ntracks2",vars);

 TH1F *btaupt_1_ntracks2 = (TH1F*)gDirectory->Get("bhist11_ntracks2");
 TH1F *btaupt_2_ntracks2 = (TH1F*)gDirectory->Get("bhist21_ntracks2");
 TH1F *btaupt_3_ntracks2 = (TH1F*)gDirectory->Get("bhist31_ntracks2");

 btaupt_1_ntracks2->Sumw2();
 btaupt_2_ntracks2->Sumw2();
 btaupt_3_ntracks2->Sumw2();


 btaupt_1_ntracks2->Scale(988.8/9987968);
 btaupt_2_ntracks2->Scale(66.3/6000000);
 btaupt_3_ntracks2->Scale(8.15/5998860);

 



 btaupt_1_ntracks2->Add(btaupt_2_ntracks2);
 btaupt_1_ntracks2->Add(btaupt_3_ntracks2);
 


 btaupt_1_ntracks2->Divide(bjetpt_1_ntracks2);
 btaupt_1_ntracks2->SetMarkerStyle(20);
 btaupt_1_ntracks2->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks2 = new TH1F("ghist1_ntracks2","ghist1_ntracks2",16,binning);
TH1F* ghist2_ntracks2 = new TH1F("ghist2_ntracks2","ghist2_ntracks2",16,binning);
TH1F* ghist3_ntracks2 = new TH1F("ghist3_ntracks2","ghist3_ntracks2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_ntracks2",vars);

 TH1F* gjetpt_1_ntracks2 = (TH1F*)gDirectory->Get("ghist1_ntracks2");
 TH1F* gjetpt_2_ntracks2 = (TH1F*)gDirectory->Get("ghist2_ntracks2");
 TH1F* gjetpt_3_ntracks2 = (TH1F*)gDirectory->Get("ghist3_ntracks2");

 gjetpt_1_ntracks2->Sumw2();
 gjetpt_2_ntracks2->Sumw2();
 gjetpt_3_ntracks2->Sumw2();


 gjetpt_1_ntracks2->Scale(988.8/9987968);
 gjetpt_2_ntracks2->Scale(66.3/6000000);
 gjetpt_3_ntracks2->Scale(8.15/5998860);

 gjetpt_1_ntracks2->Add(gjetpt_2_ntracks2);
 gjetpt_1_ntracks2->Add(gjetpt_3_ntracks2);



TH1F* ghist11_ntracks2 = new TH1F("ghist11_ntracks2","ghist11_ntracks2",16,binning);
TH1F* ghist21_ntracks2 = new TH1F("ghist21_ntracks2","ghist21_ntracks2",16,binning);
TH1F* ghist31_ntracks2 = new TH1F("ghist31_ntracks2","ghist31_ntracks2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[0]+"&&ntracks<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks2",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_ntracks2",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_ntracks2",vars);

 TH1F *gtaupt_1_ntracks2 = (TH1F*)gDirectory->Get("ghist11_ntracks2");
 TH1F *gtaupt_2_ntracks2 = (TH1F*)gDirectory->Get("ghist21_ntracks2");
 TH1F *gtaupt_3_ntracks2 = (TH1F*)gDirectory->Get("ghist31_ntracks2");

 gtaupt_1_ntracks2->Sumw2();
 gtaupt_2_ntracks2->Sumw2();
 gtaupt_3_ntracks2->Sumw2();


 gtaupt_1_ntracks2->Scale(988.8/9987968);
 gtaupt_2_ntracks2->Scale(66.3/6000000);
 gtaupt_3_ntracks2->Scale(8.15/5998860);

 



 gtaupt_1_ntracks2->Add(gtaupt_2_ntracks2);
 gtaupt_1_ntracks2->Add(gtaupt_3_ntracks2);
 


 gtaupt_1_ntracks2->Divide(gjetpt_1_ntracks2);
 gtaupt_1_ntracks2->SetMarkerStyle(20);

TH1F* hist1_ntracks3 = new TH1F("hist1_ntracks3","hist1_ntracks3",16,binning);
TH1F* hist2_ntracks3 = new TH1F("hist2_ntracks3","hist2_ntracks3",16,binning);
TH1F* hist3_ntracks3 = new TH1F("hist3_ntracks3","hist3_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>hist2_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>hist3_ntracks3",vars);

 TH1F* jetpt_1_ntracks3 = (TH1F*)gDirectory->Get("hist1_ntracks3");
 TH1F* jetpt_2_ntracks3 = (TH1F*)gDirectory->Get("hist2_ntracks3");
 TH1F* jetpt_3_ntracks3 = (TH1F*)gDirectory->Get("hist3_ntracks3");

 jetpt_1_ntracks3->Sumw2();
 jetpt_2_ntracks3->Sumw2();
 jetpt_3_ntracks3->Sumw2();


 jetpt_1_ntracks3->Scale(988.8/9987968);
 jetpt_2_ntracks3->Scale(66.3/6000000);
 jetpt_3_ntracks3->Scale(8.15/5998860);

 jetpt_1_ntracks3->Add(jetpt_2_ntracks3);
 jetpt_1_ntracks3->Add(jetpt_3_ntracks3);



TH1F* hist11_ntracks3 = new TH1F("hist11_ntracks3","hist11_ntracks3",16,binning);
TH1F* hist21_ntracks3 = new TH1F("hist21_ntracks3","hist21_ntracks3",16,binning);
TH1F* hist31_ntracks3 = new TH1F("hist31_ntracks3","hist31_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>hist21_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>hist31_ntracks3",vars);

 TH1F *taupt_1_ntracks3 = (TH1F*)gDirectory->Get("hist11_ntracks3");
 TH1F *taupt_2_ntracks3 = (TH1F*)gDirectory->Get("hist21_ntracks3");
 TH1F *taupt_3_ntracks3 = (TH1F*)gDirectory->Get("hist31_ntracks3");

 taupt_1_ntracks3->Sumw2();
 taupt_2_ntracks3->Sumw2();
 taupt_3_ntracks3->Sumw2();


 taupt_1_ntracks3->Scale(988.8/9987968);
 taupt_2_ntracks3->Scale(66.3/6000000);
 taupt_3_ntracks3->Scale(8.15/5998860);

 



 taupt_1_ntracks3->Add(taupt_2_ntracks3);
 taupt_1_ntracks3->Add(taupt_3_ntracks3);
 


 taupt_1_ntracks3->Divide(jetpt_1_ntracks3);
 taupt_1_ntracks3->SetMarkerStyle(20);
 taupt_1_ntracks3->SetMarkerColor(kRed);


TH1F* lhist1_ntracks3 = new TH1F("lhist1_ntracks3","lhist1_ntracks3",16,binning);
TH1F* lhist2_ntracks3 = new TH1F("lhist2_ntracks3","lhist2_ntracks3",16,binning);
TH1F* lhist3_ntracks3 = new TH1F("lhist3_ntracks3","lhist3_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_ntracks3",vars);

 TH1F* ljetpt_1_ntracks3 = (TH1F*)gDirectory->Get("lhist1_ntracks3");
 TH1F* ljetpt_2_ntracks3 = (TH1F*)gDirectory->Get("lhist2_ntracks3");
 TH1F* ljetpt_3_ntracks3 = (TH1F*)gDirectory->Get("lhist3_ntracks3");

 ljetpt_1_ntracks3->Sumw2();
 ljetpt_2_ntracks3->Sumw2();
 ljetpt_3_ntracks3->Sumw2();


 ljetpt_1_ntracks3->Scale(988.8/9987968);
 ljetpt_2_ntracks3->Scale(66.3/6000000);
 ljetpt_3_ntracks3->Scale(8.15/5998860);

 ljetpt_1_ntracks3->Add(ljetpt_2_ntracks3);
 ljetpt_1_ntracks3->Add(ljetpt_3_ntracks3);



TH1F* lhist11_ntracks3 = new TH1F("lhist11_ntracks3","lhist11_ntracks3",16,binning);
TH1F* lhist21_ntracks3 = new TH1F("lhist21_ntracks3","lhist21_ntracks3",16,binning);
TH1F* lhist31_ntracks3 = new TH1F("lhist31_ntracks3","lhist31_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_ntracks3",vars);

 TH1F *ltaupt_1_ntracks3 = (TH1F*)gDirectory->Get("lhist11_ntracks3");
 TH1F *ltaupt_2_ntracks3 = (TH1F*)gDirectory->Get("lhist21_ntracks3");
 TH1F *ltaupt_3_ntracks3 = (TH1F*)gDirectory->Get("lhist31_ntracks3");

 ltaupt_1_ntracks3->Sumw2();
 ltaupt_2_ntracks3->Sumw2();
 ltaupt_3_ntracks3->Sumw2();


 ltaupt_1_ntracks3->Scale(988.8/9987968);
 ltaupt_2_ntracks3->Scale(66.3/6000000);
 ltaupt_3_ntracks3->Scale(8.15/5998860);

 



 ltaupt_1_ntracks3->Add(ltaupt_2_ntracks3);
 ltaupt_1_ntracks3->Add(ltaupt_3_ntracks3);
 


 ltaupt_1_ntracks3->Divide(ljetpt_1_ntracks3);
 ltaupt_1_ntracks3->SetMarkerStyle(20);
 ltaupt_1_ntracks3->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks3 = new TH1F("bhist1_ntracks3","bhist1_ntracks3",16,binning);
TH1F* bhist2_ntracks3 = new TH1F("bhist2_ntracks3","bhist2_ntracks3",16,binning);
TH1F* bhist3_ntracks3 = new TH1F("bhist3_ntracks3","bhist3_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_ntracks3",vars);

 TH1F* bjetpt_1_ntracks3 = (TH1F*)gDirectory->Get("bhist1_ntracks3");
 TH1F* bjetpt_2_ntracks3 = (TH1F*)gDirectory->Get("bhist2_ntracks3");
 TH1F* bjetpt_3_ntracks3 = (TH1F*)gDirectory->Get("bhist3_ntracks3");

 bjetpt_1_ntracks3->Sumw2();
 bjetpt_2_ntracks3->Sumw2();
 bjetpt_3_ntracks3->Sumw2();


 bjetpt_1_ntracks3->Scale(988.8/9987968);
 bjetpt_2_ntracks3->Scale(66.3/6000000);
 bjetpt_3_ntracks3->Scale(8.15/5998860);

 bjetpt_1_ntracks3->Add(bjetpt_2_ntracks3);
 bjetpt_1_ntracks3->Add(bjetpt_3_ntracks3);



TH1F* bhist11_ntracks3 = new TH1F("bhist11_ntracks3","bhist11_ntracks3",16,binning);
TH1F* bhist21_ntracks3 = new TH1F("bhist21_ntracks3","bhist21_ntracks3",16,binning);
TH1F* bhist31_ntracks3 = new TH1F("bhist31_ntracks3","bhist31_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_ntracks3",vars);

 TH1F *btaupt_1_ntracks3 = (TH1F*)gDirectory->Get("bhist11_ntracks3");
 TH1F *btaupt_2_ntracks3 = (TH1F*)gDirectory->Get("bhist21_ntracks3");
 TH1F *btaupt_3_ntracks3 = (TH1F*)gDirectory->Get("bhist31_ntracks3");

 btaupt_1_ntracks3->Sumw2();
 btaupt_2_ntracks3->Sumw2();
 btaupt_3_ntracks3->Sumw2();


 btaupt_1_ntracks3->Scale(988.8/9987968);
 btaupt_2_ntracks3->Scale(66.3/6000000);
 btaupt_3_ntracks3->Scale(8.15/5998860);

 



 btaupt_1_ntracks3->Add(btaupt_2_ntracks3);
 btaupt_1_ntracks3->Add(btaupt_3_ntracks3);
 


 btaupt_1_ntracks3->Divide(bjetpt_1_ntracks3);
 btaupt_1_ntracks3->SetMarkerStyle(20);
 btaupt_1_ntracks3->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks3 = new TH1F("ghist1_ntracks3","ghist1_ntracks3",16,binning);
TH1F* ghist2_ntracks3 = new TH1F("ghist2_ntracks3","ghist2_ntracks3",16,binning);
TH1F* ghist3_ntracks3 = new TH1F("ghist3_ntracks3","ghist3_ntracks3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_ntracks3",vars);

 TH1F* gjetpt_1_ntracks3 = (TH1F*)gDirectory->Get("ghist1_ntracks3");
 TH1F* gjetpt_2_ntracks3 = (TH1F*)gDirectory->Get("ghist2_ntracks3");
 TH1F* gjetpt_3_ntracks3 = (TH1F*)gDirectory->Get("ghist3_ntracks3");

 gjetpt_1_ntracks3->Sumw2();
 gjetpt_2_ntracks3->Sumw2();
 gjetpt_3_ntracks3->Sumw2();


 gjetpt_1_ntracks3->Scale(988.8/9987968);
 gjetpt_2_ntracks3->Scale(66.3/6000000);
 gjetpt_3_ntracks3->Scale(8.15/5998860);

 gjetpt_1_ntracks3->Add(gjetpt_2_ntracks3);
 gjetpt_1_ntracks3->Add(gjetpt_3_ntracks3);



TH1F* ghist11_ntracks3 = new TH1F("ghist11_ntracks3","ghist11_ntracks3",16,binning);
TH1F* ghist21_ntracks3 = new TH1F("ghist21_ntracks3","ghist21_ntracks3",16,binning);
TH1F* ghist31_ntracks3 = new TH1F("ghist31_ntracks3","ghist31_ntracks3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[1]+"&&ntracks<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks3",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_ntracks3",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_ntracks3",vars);

 TH1F *gtaupt_1_ntracks3 = (TH1F*)gDirectory->Get("ghist11_ntracks3");
 TH1F *gtaupt_2_ntracks3 = (TH1F*)gDirectory->Get("ghist21_ntracks3");
 TH1F *gtaupt_3_ntracks3 = (TH1F*)gDirectory->Get("ghist31_ntracks3");

 gtaupt_1_ntracks3->Sumw2();
 gtaupt_2_ntracks3->Sumw2();
 gtaupt_3_ntracks3->Sumw2();


 gtaupt_1_ntracks3->Scale(988.8/9987968);
 gtaupt_2_ntracks3->Scale(66.3/6000000);
 gtaupt_3_ntracks3->Scale(8.15/5998860);

 



 gtaupt_1_ntracks3->Add(gtaupt_2_ntracks3);
 gtaupt_1_ntracks3->Add(gtaupt_3_ntracks3);
 


 gtaupt_1_ntracks3->Divide(gjetpt_1_ntracks3);
 gtaupt_1_ntracks3->SetMarkerStyle(20);



TH1F* hist1_ntracks4 = new TH1F("hist1_ntracks4","hist1_ntracks4",16,binning);
TH1F* hist2_ntracks4 = new TH1F("hist2_ntracks4","hist2_ntracks4",16,binning);
TH1F* hist3_ntracks4 = new TH1F("hist3_ntracks4","hist3_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>hist2_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>hist3_ntracks4",vars);

 TH1F* jetpt_1_ntracks4 = (TH1F*)gDirectory->Get("hist1_ntracks4");
 TH1F* jetpt_2_ntracks4 = (TH1F*)gDirectory->Get("hist2_ntracks4");
 TH1F* jetpt_3_ntracks4 = (TH1F*)gDirectory->Get("hist3_ntracks4");

 jetpt_1_ntracks4->Sumw2();
 jetpt_2_ntracks4->Sumw2();
 jetpt_3_ntracks4->Sumw2();


 jetpt_1_ntracks4->Scale(988.8/9987968);
 jetpt_2_ntracks4->Scale(66.3/6000000);
 jetpt_3_ntracks4->Scale(8.15/5998860);

 jetpt_1_ntracks4->Add(jetpt_2_ntracks4);
 jetpt_1_ntracks4->Add(jetpt_3_ntracks4);



TH1F* hist11_ntracks4 = new TH1F("hist11_ntracks4","hist11_ntracks4",16,binning);
TH1F* hist21_ntracks4 = new TH1F("hist21_ntracks4","hist21_ntracks4",16,binning);
TH1F* hist31_ntracks4 = new TH1F("hist31_ntracks4","hist31_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>hist21_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>hist31_ntracks4",vars);

 TH1F *taupt_1_ntracks4 = (TH1F*)gDirectory->Get("hist11_ntracks4");
 TH1F *taupt_2_ntracks4 = (TH1F*)gDirectory->Get("hist21_ntracks4");
 TH1F *taupt_3_ntracks4 = (TH1F*)gDirectory->Get("hist31_ntracks4");

 taupt_1_ntracks4->Sumw2();
 taupt_2_ntracks4->Sumw2();
 taupt_3_ntracks4->Sumw2();


 taupt_1_ntracks4->Scale(988.8/9987968);
 taupt_2_ntracks4->Scale(66.3/6000000);
 taupt_3_ntracks4->Scale(8.15/5998860);

 



 taupt_1_ntracks4->Add(taupt_2_ntracks4);
 taupt_1_ntracks4->Add(taupt_3_ntracks4);
 


 taupt_1_ntracks4->Divide(jetpt_1_ntracks4);
 taupt_1_ntracks4->SetMarkerStyle(20);
 taupt_1_ntracks4->SetMarkerColor(kRed);


TH1F* lhist1_ntracks4 = new TH1F("lhist1_ntracks4","lhist1_ntracks4",16,binning);
TH1F* lhist2_ntracks4 = new TH1F("lhist2_ntracks4","lhist2_ntracks4",16,binning);
TH1F* lhist3_ntracks4 = new TH1F("lhist3_ntracks4","lhist3_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_ntracks4",vars);

 TH1F* ljetpt_1_ntracks4 = (TH1F*)gDirectory->Get("lhist1_ntracks4");
 TH1F* ljetpt_2_ntracks4 = (TH1F*)gDirectory->Get("lhist2_ntracks4");
 TH1F* ljetpt_3_ntracks4 = (TH1F*)gDirectory->Get("lhist3_ntracks4");

 ljetpt_1_ntracks4->Sumw2();
 ljetpt_2_ntracks4->Sumw2();
 ljetpt_3_ntracks4->Sumw2();


 ljetpt_1_ntracks4->Scale(988.8/9987968);
 ljetpt_2_ntracks4->Scale(66.3/6000000);
 ljetpt_3_ntracks4->Scale(8.15/5998860);

 ljetpt_1_ntracks4->Add(ljetpt_2_ntracks4);
 ljetpt_1_ntracks4->Add(ljetpt_3_ntracks4);



TH1F* lhist11_ntracks4 = new TH1F("lhist11_ntracks4","lhist11_ntracks4",16,binning);
TH1F* lhist21_ntracks4 = new TH1F("lhist21_ntracks4","lhist21_ntracks4",16,binning);
TH1F* lhist31_ntracks4 = new TH1F("lhist31_ntracks4","lhist31_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_ntracks4",vars);

 TH1F *ltaupt_1_ntracks4 = (TH1F*)gDirectory->Get("lhist11_ntracks4");
 TH1F *ltaupt_2_ntracks4 = (TH1F*)gDirectory->Get("lhist21_ntracks4");
 TH1F *ltaupt_3_ntracks4 = (TH1F*)gDirectory->Get("lhist31_ntracks4");

 ltaupt_1_ntracks4->Sumw2();
 ltaupt_2_ntracks4->Sumw2();
 ltaupt_3_ntracks4->Sumw2();


 ltaupt_1_ntracks4->Scale(988.8/9987968);
 ltaupt_2_ntracks4->Scale(66.3/6000000);
 ltaupt_3_ntracks4->Scale(8.15/5998860);

 



 ltaupt_1_ntracks4->Add(ltaupt_2_ntracks4);
 ltaupt_1_ntracks4->Add(ltaupt_3_ntracks4);
 


 ltaupt_1_ntracks4->Divide(ljetpt_1_ntracks4);
 ltaupt_1_ntracks4->SetMarkerStyle(20);
 ltaupt_1_ntracks4->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks4 = new TH1F("bhist1_ntracks4","bhist1_ntracks4",16,binning);
TH1F* bhist2_ntracks4 = new TH1F("bhist2_ntracks4","bhist2_ntracks4",16,binning);
TH1F* bhist3_ntracks4 = new TH1F("bhist3_ntracks4","bhist3_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_ntracks4",vars);

 TH1F* bjetpt_1_ntracks4 = (TH1F*)gDirectory->Get("bhist1_ntracks4");
 TH1F* bjetpt_2_ntracks4 = (TH1F*)gDirectory->Get("bhist2_ntracks4");
 TH1F* bjetpt_3_ntracks4 = (TH1F*)gDirectory->Get("bhist3_ntracks4");

 bjetpt_1_ntracks4->Sumw2();
 bjetpt_2_ntracks4->Sumw2();
 bjetpt_3_ntracks4->Sumw2();


 bjetpt_1_ntracks4->Scale(988.8/9987968);
 bjetpt_2_ntracks4->Scale(66.3/6000000);
 bjetpt_3_ntracks4->Scale(8.15/5998860);

 bjetpt_1_ntracks4->Add(bjetpt_2_ntracks4);
 bjetpt_1_ntracks4->Add(bjetpt_3_ntracks4);



TH1F* bhist11_ntracks4 = new TH1F("bhist11_ntracks4","bhist11_ntracks4",16,binning);
TH1F* bhist21_ntracks4 = new TH1F("bhist21_ntracks4","bhist21_ntracks4",16,binning);
TH1F* bhist31_ntracks4 = new TH1F("bhist31_ntracks4","bhist31_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_ntracks4",vars);

 TH1F *btaupt_1_ntracks4 = (TH1F*)gDirectory->Get("bhist11_ntracks4");
 TH1F *btaupt_2_ntracks4 = (TH1F*)gDirectory->Get("bhist21_ntracks4");
 TH1F *btaupt_3_ntracks4 = (TH1F*)gDirectory->Get("bhist31_ntracks4");

 btaupt_1_ntracks4->Sumw2();
 btaupt_2_ntracks4->Sumw2();
 btaupt_3_ntracks4->Sumw2();


 btaupt_1_ntracks4->Scale(988.8/9987968);
 btaupt_2_ntracks4->Scale(66.3/6000000);
 btaupt_3_ntracks4->Scale(8.15/5998860);

 



 btaupt_1_ntracks4->Add(btaupt_2_ntracks4);
 btaupt_1_ntracks4->Add(btaupt_3_ntracks4);
 


 btaupt_1_ntracks4->Divide(bjetpt_1_ntracks4);
 btaupt_1_ntracks4->SetMarkerStyle(20);
 btaupt_1_ntracks4->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks4 = new TH1F("ghist1_ntracks4","ghist1_ntracks4",16,binning);
TH1F* ghist2_ntracks4 = new TH1F("ghist2_ntracks4","ghist2_ntracks4",16,binning);
TH1F* ghist3_ntracks4 = new TH1F("ghist3_ntracks4","ghist3_ntracks4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_ntracks4",vars);

 TH1F* gjetpt_1_ntracks4 = (TH1F*)gDirectory->Get("ghist1_ntracks4");
 TH1F* gjetpt_2_ntracks4 = (TH1F*)gDirectory->Get("ghist2_ntracks4");
 TH1F* gjetpt_3_ntracks4 = (TH1F*)gDirectory->Get("ghist3_ntracks4");

 gjetpt_1_ntracks4->Sumw2();
 gjetpt_2_ntracks4->Sumw2();
 gjetpt_3_ntracks4->Sumw2();


 gjetpt_1_ntracks4->Scale(988.8/9987968);
 gjetpt_2_ntracks4->Scale(66.3/6000000);
 gjetpt_3_ntracks4->Scale(8.15/5998860);

 gjetpt_1_ntracks4->Add(gjetpt_2_ntracks4);
 gjetpt_1_ntracks4->Add(gjetpt_3_ntracks4);



TH1F* ghist11_ntracks4 = new TH1F("ghist11_ntracks4","ghist11_ntracks4",16,binning);
TH1F* ghist21_ntracks4 = new TH1F("ghist21_ntracks4","ghist21_ntracks4",16,binning);
TH1F* ghist31_ntracks4 = new TH1F("ghist31_ntracks4","ghist31_ntracks4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[2]+"&&ntracks<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks4",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_ntracks4",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_ntracks4",vars);

 TH1F *gtaupt_1_ntracks4 = (TH1F*)gDirectory->Get("ghist11_ntracks4");
 TH1F *gtaupt_2_ntracks4 = (TH1F*)gDirectory->Get("ghist21_ntracks4");
 TH1F *gtaupt_3_ntracks4 = (TH1F*)gDirectory->Get("ghist31_ntracks4");

 gtaupt_1_ntracks4->Sumw2();
 gtaupt_2_ntracks4->Sumw2();
 gtaupt_3_ntracks4->Sumw2();


 gtaupt_1_ntracks4->Scale(988.8/9987968);
 gtaupt_2_ntracks4->Scale(66.3/6000000);
 gtaupt_3_ntracks4->Scale(8.15/5998860);

 



 gtaupt_1_ntracks4->Add(gtaupt_2_ntracks4);
 gtaupt_1_ntracks4->Add(gtaupt_3_ntracks4);
 


 gtaupt_1_ntracks4->Divide(gjetpt_1_ntracks4);
 gtaupt_1_ntracks4->SetMarkerStyle(20);

TH1F* hist1_ntracks5 = new TH1F("hist1_ntracks5","hist1_ntracks5",16,binning);
TH1F* hist2_ntracks5 = new TH1F("hist2_ntracks5","hist2_ntracks5",16,binning);
TH1F* hist3_ntracks5 = new TH1F("hist3_ntracks5","hist3_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>hist2_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>hist3_ntracks5",vars);

 TH1F* jetpt_1_ntracks5 = (TH1F*)gDirectory->Get("hist1_ntracks5");
 TH1F* jetpt_2_ntracks5 = (TH1F*)gDirectory->Get("hist2_ntracks5");
 TH1F* jetpt_3_ntracks5 = (TH1F*)gDirectory->Get("hist3_ntracks5");

 jetpt_1_ntracks5->Sumw2();
 jetpt_2_ntracks5->Sumw2();
 jetpt_3_ntracks5->Sumw2();


 jetpt_1_ntracks5->Scale(988.8/9987968);
 jetpt_2_ntracks5->Scale(66.3/6000000);
 jetpt_3_ntracks5->Scale(8.15/5998860);

 jetpt_1_ntracks5->Add(jetpt_2_ntracks5);
 jetpt_1_ntracks5->Add(jetpt_3_ntracks5);



TH1F* hist11_ntracks5 = new TH1F("hist11_ntracks5","hist11_ntracks5",16,binning);
TH1F* hist21_ntracks5 = new TH1F("hist21_ntracks5","hist21_ntracks5",16,binning);
TH1F* hist31_ntracks5 = new TH1F("hist31_ntracks5","hist31_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>hist21_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>hist31_ntracks5",vars);

 TH1F *taupt_1_ntracks5 = (TH1F*)gDirectory->Get("hist11_ntracks5");
 TH1F *taupt_2_ntracks5 = (TH1F*)gDirectory->Get("hist21_ntracks5");
 TH1F *taupt_3_ntracks5 = (TH1F*)gDirectory->Get("hist31_ntracks5");

 taupt_1_ntracks5->Sumw2();
 taupt_2_ntracks5->Sumw2();
 taupt_3_ntracks5->Sumw2();


 taupt_1_ntracks5->Scale(988.8/9987968);
 taupt_2_ntracks5->Scale(66.3/6000000);
 taupt_3_ntracks5->Scale(8.15/5998860);

 



 taupt_1_ntracks5->Add(taupt_2_ntracks5);
 taupt_1_ntracks5->Add(taupt_3_ntracks5);
 


 taupt_1_ntracks5->Divide(jetpt_1_ntracks5);
 taupt_1_ntracks5->SetMarkerStyle(20);
 taupt_1_ntracks5->SetMarkerColor(kRed);


TH1F* lhist1_ntracks5 = new TH1F("lhist1_ntracks5","lhist1_ntracks5",16,binning);
TH1F* lhist2_ntracks5 = new TH1F("lhist2_ntracks5","lhist2_ntracks5",16,binning);
TH1F* lhist3_ntracks5 = new TH1F("lhist3_ntracks5","lhist3_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_ntracks5",vars);

 TH1F* ljetpt_1_ntracks5 = (TH1F*)gDirectory->Get("lhist1_ntracks5");
 TH1F* ljetpt_2_ntracks5 = (TH1F*)gDirectory->Get("lhist2_ntracks5");
 TH1F* ljetpt_3_ntracks5 = (TH1F*)gDirectory->Get("lhist3_ntracks5");

 ljetpt_1_ntracks5->Sumw2();
 ljetpt_2_ntracks5->Sumw2();
 ljetpt_3_ntracks5->Sumw2();


 ljetpt_1_ntracks5->Scale(988.8/9987968);
 ljetpt_2_ntracks5->Scale(66.3/6000000);
 ljetpt_3_ntracks5->Scale(8.15/5998860);

 ljetpt_1_ntracks5->Add(ljetpt_2_ntracks5);
 ljetpt_1_ntracks5->Add(ljetpt_3_ntracks5);



TH1F* lhist11_ntracks5 = new TH1F("lhist11_ntracks5","lhist11_ntracks5",16,binning);
TH1F* lhist21_ntracks5 = new TH1F("lhist21_ntracks5","lhist21_ntracks5",16,binning);
TH1F* lhist31_ntracks5 = new TH1F("lhist31_ntracks5","lhist31_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_ntracks5",vars);

 TH1F *ltaupt_1_ntracks5 = (TH1F*)gDirectory->Get("lhist11_ntracks5");
 TH1F *ltaupt_2_ntracks5 = (TH1F*)gDirectory->Get("lhist21_ntracks5");
 TH1F *ltaupt_3_ntracks5 = (TH1F*)gDirectory->Get("lhist31_ntracks5");

 ltaupt_1_ntracks5->Sumw2();
 ltaupt_2_ntracks5->Sumw2();
 ltaupt_3_ntracks5->Sumw2();


 ltaupt_1_ntracks5->Scale(988.8/9987968);
 ltaupt_2_ntracks5->Scale(66.3/6000000);
 ltaupt_3_ntracks5->Scale(8.15/5998860);

 



 ltaupt_1_ntracks5->Add(ltaupt_2_ntracks5);
 ltaupt_1_ntracks5->Add(ltaupt_3_ntracks5);
 


 ltaupt_1_ntracks5->Divide(ljetpt_1_ntracks5);
 ltaupt_1_ntracks5->SetMarkerStyle(20);
 ltaupt_1_ntracks5->SetMarkerColor(kGreen+3);



TH1F* bhist1_ntracks5 = new TH1F("bhist1_ntracks5","bhist1_ntracks5",16,binning);
TH1F* bhist2_ntracks5 = new TH1F("bhist2_ntracks5","bhist2_ntracks5",16,binning);
TH1F* bhist3_ntracks5 = new TH1F("bhist3_ntracks5","bhist3_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_ntracks5",vars);

 TH1F* bjetpt_1_ntracks5 = (TH1F*)gDirectory->Get("bhist1_ntracks5");
 TH1F* bjetpt_2_ntracks5 = (TH1F*)gDirectory->Get("bhist2_ntracks5");
 TH1F* bjetpt_3_ntracks5 = (TH1F*)gDirectory->Get("bhist3_ntracks5");

 bjetpt_1_ntracks5->Sumw2();
 bjetpt_2_ntracks5->Sumw2();
 bjetpt_3_ntracks5->Sumw2();


 bjetpt_1_ntracks5->Scale(988.8/9987968);
 bjetpt_2_ntracks5->Scale(66.3/6000000);
 bjetpt_3_ntracks5->Scale(8.15/5998860);

 bjetpt_1_ntracks5->Add(bjetpt_2_ntracks5);
 bjetpt_1_ntracks5->Add(bjetpt_3_ntracks5);



TH1F* bhist11_ntracks5 = new TH1F("bhist11_ntracks5","bhist11_ntracks5",16,binning);
TH1F* bhist21_ntracks5 = new TH1F("bhist21_ntracks5","bhist21_ntracks5",16,binning);
TH1F* bhist31_ntracks5 = new TH1F("bhist31_ntracks5","bhist31_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_ntracks5",vars);

 TH1F *btaupt_1_ntracks5 = (TH1F*)gDirectory->Get("bhist11_ntracks5");
 TH1F *btaupt_2_ntracks5 = (TH1F*)gDirectory->Get("bhist21_ntracks5");
 TH1F *btaupt_3_ntracks5 = (TH1F*)gDirectory->Get("bhist31_ntracks5");

 btaupt_1_ntracks5->Sumw2();
 btaupt_2_ntracks5->Sumw2();
 btaupt_3_ntracks5->Sumw2();


 btaupt_1_ntracks5->Scale(988.8/9987968);
 btaupt_2_ntracks5->Scale(66.3/6000000);
 btaupt_3_ntracks5->Scale(8.15/5998860);

 



 btaupt_1_ntracks5->Add(btaupt_2_ntracks5);
 btaupt_1_ntracks5->Add(btaupt_3_ntracks5);
 


 btaupt_1_ntracks5->Divide(bjetpt_1_ntracks5);
 btaupt_1_ntracks5->SetMarkerStyle(20);
 btaupt_1_ntracks5->SetMarkerColor(kBlue);


TH1F* ghist1_ntracks5 = new TH1F("ghist1_ntracks5","ghist1_ntracks5",16,binning);
TH1F* ghist2_ntracks5 = new TH1F("ghist2_ntracks5","ghist2_ntracks5",16,binning);
TH1F* ghist3_ntracks5 = new TH1F("ghist3_ntracks5","ghist3_ntracks5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_ntracks5",vars);

 TH1F* gjetpt_1_ntracks5 = (TH1F*)gDirectory->Get("ghist1_ntracks5");
 TH1F* gjetpt_2_ntracks5 = (TH1F*)gDirectory->Get("ghist2_ntracks5");
 TH1F* gjetpt_3_ntracks5 = (TH1F*)gDirectory->Get("ghist3_ntracks5");

 gjetpt_1_ntracks5->Sumw2();
 gjetpt_2_ntracks5->Sumw2();
 gjetpt_3_ntracks5->Sumw2();


 gjetpt_1_ntracks5->Scale(988.8/9987968);
 gjetpt_2_ntracks5->Scale(66.3/6000000);
 gjetpt_3_ntracks5->Scale(8.15/5998860);

 gjetpt_1_ntracks5->Add(gjetpt_2_ntracks5);
 gjetpt_1_ntracks5->Add(gjetpt_3_ntracks5);



TH1F* ghist11_ntracks5 = new TH1F("ghist11_ntracks5","ghist11_ntracks5",16,binning);
TH1F* ghist21_ntracks5 = new TH1F("ghist21_ntracks5","ghist21_ntracks5",16,binning);
TH1F* ghist31_ntracks5 = new TH1F("ghist31_ntracks5","ghist31_ntracks5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&ntracks>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_ntracks5",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_ntracks5",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_ntracks5",vars);

 TH1F *gtaupt_1_ntracks5 = (TH1F*)gDirectory->Get("ghist11_ntracks5");
 TH1F *gtaupt_2_ntracks5 = (TH1F*)gDirectory->Get("ghist21_ntracks5");
 TH1F *gtaupt_3_ntracks5 = (TH1F*)gDirectory->Get("ghist31_ntracks5");

 gtaupt_1_ntracks5->Sumw2();
 gtaupt_2_ntracks5->Sumw2();
 gtaupt_3_ntracks5->Sumw2();


 gtaupt_1_ntracks5->Scale(988.8/9987968);
 gtaupt_2_ntracks5->Scale(66.3/6000000);
 gtaupt_3_ntracks5->Scale(8.15/5998860);

 



 gtaupt_1_ntracks5->Add(gtaupt_2_ntracks5);
 gtaupt_1_ntracks5->Add(gtaupt_3_ntracks5);
 


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
 TFile *outfile = new TFile("Jan29_qcd_fakerates_ntracks.root","RECREATE");
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
