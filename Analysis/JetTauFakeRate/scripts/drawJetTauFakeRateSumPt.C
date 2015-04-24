void drawJetTauFakeRateSumPt(){
 TFile *qcd_1 = TFile::Open("Jan29_QCD_15-30.root");
 TFile *qcd_2 = TFile::Open("Jan29_QCD_30-50.root");
 TFile *qcd_3 = TFile::Open("Jan29_QCD_50-80.root");

 TTree *qcd_tree_1 = dynamic_cast<TTree*>(qcd_1->Get("ntuple"));
 TTree *qcd_tree_2 = dynamic_cast<TTree*>(qcd_2->Get("ntuple"));
 TTree *qcd_tree_3 = dynamic_cast<TTree*>(qcd_3->Get("ntuple"));

Float_t binning[17]={0,20,25,30,35,40,45,50,55,60,70,80,90,100,110,130,150};
std::string limits[4]={"30","60","90","120"};
TH1F* hist1_sumpt1 = new TH1F("hist1_sumpt1","hist1_sumpt1",16,binning);
TH1F* hist2_sumpt1 = new TH1F("hist2_sumpt1","hist2_sumpt1",16,binning);
TH1F* hist3_sumpt1 = new TH1F("hist3_sumpt1","hist3_sumpt1",16,binning);

TString vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumpt1",vars);

 TH1F* jetpt_1_sumpt1 = (TH1F*)gDirectory->Get("hist1_sumpt1");
 TH1F* jetpt_2_sumpt1 = (TH1F*)gDirectory->Get("hist2_sumpt1");
 TH1F* jetpt_3_sumpt1 = (TH1F*)gDirectory->Get("hist3_sumpt1");

 jetpt_1_sumpt1->Sumw2();
 jetpt_2_sumpt1->Sumw2();
 jetpt_3_sumpt1->Sumw2();


 jetpt_1_sumpt1->Scale(988.8/9987968);
 jetpt_2_sumpt1->Scale(66.3/6000000);
 jetpt_3_sumpt1->Scale(8.15/5998860);

 jetpt_1_sumpt1->Add(jetpt_2_sumpt1);
 jetpt_1_sumpt1->Add(jetpt_3_sumpt1);



TH1F* hist11_sumpt1 = new TH1F("hist11_sumpt1","hist11_sumpt1",16,binning);
TH1F* hist21_sumpt1 = new TH1F("hist21_sumpt1","hist21_sumpt1",16,binning);
TH1F* hist31_sumpt1 = new TH1F("hist31_sumpt1","hist31_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumpt1",vars);

 TH1F *taupt_1_sumpt1 = (TH1F*)gDirectory->Get("hist11_sumpt1");
 TH1F *taupt_2_sumpt1 = (TH1F*)gDirectory->Get("hist21_sumpt1");
 TH1F *taupt_3_sumpt1 = (TH1F*)gDirectory->Get("hist31_sumpt1");

 taupt_1_sumpt1->Sumw2();
 taupt_2_sumpt1->Sumw2();
 taupt_3_sumpt1->Sumw2();


 taupt_1_sumpt1->Scale(988.8/9987968);
 taupt_2_sumpt1->Scale(66.3/6000000);
 taupt_3_sumpt1->Scale(8.15/5998860);

 



 taupt_1_sumpt1->Add(taupt_2_sumpt1);
 taupt_1_sumpt1->Add(taupt_3_sumpt1);
 


 taupt_1_sumpt1->Divide(jetpt_1_sumpt1);
 taupt_1_sumpt1->SetMarkerStyle(20);
 taupt_1_sumpt1->SetMarkerColor(kRed);


TH1F* lhist1_sumpt1 = new TH1F("lhist1_sumpt1","lhist1_sumpt1",16,binning);
TH1F* lhist2_sumpt1 = new TH1F("lhist2_sumpt1","lhist2_sumpt1",16,binning);
TH1F* lhist3_sumpt1 = new TH1F("lhist3_sumpt1","lhist3_sumpt1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumpt1",vars);

 TH1F* ljetpt_1_sumpt1 = (TH1F*)gDirectory->Get("lhist1_sumpt1");
 TH1F* ljetpt_2_sumpt1 = (TH1F*)gDirectory->Get("lhist2_sumpt1");
 TH1F* ljetpt_3_sumpt1 = (TH1F*)gDirectory->Get("lhist3_sumpt1");

 ljetpt_1_sumpt1->Sumw2();
 ljetpt_2_sumpt1->Sumw2();
 ljetpt_3_sumpt1->Sumw2();


 ljetpt_1_sumpt1->Scale(988.8/9987968);
 ljetpt_2_sumpt1->Scale(66.3/6000000);
 ljetpt_3_sumpt1->Scale(8.15/5998860);

 ljetpt_1_sumpt1->Add(ljetpt_2_sumpt1);
 ljetpt_1_sumpt1->Add(ljetpt_3_sumpt1);



TH1F* lhist11_sumpt1 = new TH1F("lhist11_sumpt1","lhist11_sumpt1",16,binning);
TH1F* lhist21_sumpt1 = new TH1F("lhist21_sumpt1","lhist21_sumpt1",16,binning);
TH1F* lhist31_sumpt1 = new TH1F("lhist31_sumpt1","lhist31_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumpt1",vars);

 TH1F *ltaupt_1_sumpt1 = (TH1F*)gDirectory->Get("lhist11_sumpt1");
 TH1F *ltaupt_2_sumpt1 = (TH1F*)gDirectory->Get("lhist21_sumpt1");
 TH1F *ltaupt_3_sumpt1 = (TH1F*)gDirectory->Get("lhist31_sumpt1");

 ltaupt_1_sumpt1->Sumw2();
 ltaupt_2_sumpt1->Sumw2();
 ltaupt_3_sumpt1->Sumw2();


 ltaupt_1_sumpt1->Scale(988.8/9987968);
 ltaupt_2_sumpt1->Scale(66.3/6000000);
 ltaupt_3_sumpt1->Scale(8.15/5998860);

 



 ltaupt_1_sumpt1->Add(ltaupt_2_sumpt1);
 ltaupt_1_sumpt1->Add(ltaupt_3_sumpt1);
 


 ltaupt_1_sumpt1->Divide(ljetpt_1_sumpt1);
 ltaupt_1_sumpt1->SetMarkerStyle(20);
 ltaupt_1_sumpt1->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt1 = new TH1F("bhist1_sumpt1","bhist1_sumpt1",16,binning);
TH1F* bhist2_sumpt1 = new TH1F("bhist2_sumpt1","bhist2_sumpt1",16,binning);
TH1F* bhist3_sumpt1 = new TH1F("bhist3_sumpt1","bhist3_sumpt1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumpt1",vars);

 TH1F* bjetpt_1_sumpt1 = (TH1F*)gDirectory->Get("bhist1_sumpt1");
 TH1F* bjetpt_2_sumpt1 = (TH1F*)gDirectory->Get("bhist2_sumpt1");
 TH1F* bjetpt_3_sumpt1 = (TH1F*)gDirectory->Get("bhist3_sumpt1");

 bjetpt_1_sumpt1->Sumw2();
 bjetpt_2_sumpt1->Sumw2();
 bjetpt_3_sumpt1->Sumw2();


 bjetpt_1_sumpt1->Scale(988.8/9987968);
 bjetpt_2_sumpt1->Scale(66.3/6000000);
 bjetpt_3_sumpt1->Scale(8.15/5998860);

 bjetpt_1_sumpt1->Add(bjetpt_2_sumpt1);
 bjetpt_1_sumpt1->Add(bjetpt_3_sumpt1);



TH1F* bhist11_sumpt1 = new TH1F("bhist11_sumpt1","bhist11_sumpt1",16,binning);
TH1F* bhist21_sumpt1 = new TH1F("bhist21_sumpt1","bhist21_sumpt1",16,binning);
TH1F* bhist31_sumpt1 = new TH1F("bhist31_sumpt1","bhist31_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumpt1",vars);

 TH1F *btaupt_1_sumpt1 = (TH1F*)gDirectory->Get("bhist11_sumpt1");
 TH1F *btaupt_2_sumpt1 = (TH1F*)gDirectory->Get("bhist21_sumpt1");
 TH1F *btaupt_3_sumpt1 = (TH1F*)gDirectory->Get("bhist31_sumpt1");

 btaupt_1_sumpt1->Sumw2();
 btaupt_2_sumpt1->Sumw2();
 btaupt_3_sumpt1->Sumw2();


 btaupt_1_sumpt1->Scale(988.8/9987968);
 btaupt_2_sumpt1->Scale(66.3/6000000);
 btaupt_3_sumpt1->Scale(8.15/5998860);

 



 btaupt_1_sumpt1->Add(btaupt_2_sumpt1);
 btaupt_1_sumpt1->Add(btaupt_3_sumpt1);
 


 btaupt_1_sumpt1->Divide(bjetpt_1_sumpt1);
 btaupt_1_sumpt1->SetMarkerStyle(20);
 btaupt_1_sumpt1->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt1 = new TH1F("ghist1_sumpt1","ghist1_sumpt1",16,binning);
TH1F* ghist2_sumpt1 = new TH1F("ghist2_sumpt1","ghist2_sumpt1",16,binning);
TH1F* ghist3_sumpt1 = new TH1F("ghist3_sumpt1","ghist3_sumpt1",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumpt1",vars);

 TH1F* gjetpt_1_sumpt1 = (TH1F*)gDirectory->Get("ghist1_sumpt1");
 TH1F* gjetpt_2_sumpt1 = (TH1F*)gDirectory->Get("ghist2_sumpt1");
 TH1F* gjetpt_3_sumpt1 = (TH1F*)gDirectory->Get("ghist3_sumpt1");

 gjetpt_1_sumpt1->Sumw2();
 gjetpt_2_sumpt1->Sumw2();
 gjetpt_3_sumpt1->Sumw2();


 gjetpt_1_sumpt1->Scale(988.8/9987968);
 gjetpt_2_sumpt1->Scale(66.3/6000000);
 gjetpt_3_sumpt1->Scale(8.15/5998860);

 gjetpt_1_sumpt1->Add(gjetpt_2_sumpt1);
 gjetpt_1_sumpt1->Add(gjetpt_3_sumpt1);



TH1F* ghist11_sumpt1 = new TH1F("ghist11_sumpt1","ghist11_sumpt1",16,binning);
TH1F* ghist21_sumpt1 = new TH1F("ghist21_sumpt1","ghist21_sumpt1",16,binning);
TH1F* ghist31_sumpt1 = new TH1F("ghist31_sumpt1","ghist31_sumpt1",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>0&&sumpt<"+limits[0];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt1",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumpt1",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumpt1",vars);

 TH1F *gtaupt_1_sumpt1 = (TH1F*)gDirectory->Get("ghist11_sumpt1");
 TH1F *gtaupt_2_sumpt1 = (TH1F*)gDirectory->Get("ghist21_sumpt1");
 TH1F *gtaupt_3_sumpt1 = (TH1F*)gDirectory->Get("ghist31_sumpt1");

 gtaupt_1_sumpt1->Sumw2();
 gtaupt_2_sumpt1->Sumw2();
 gtaupt_3_sumpt1->Sumw2();


 gtaupt_1_sumpt1->Scale(988.8/9987968);
 gtaupt_2_sumpt1->Scale(66.3/6000000);
 gtaupt_3_sumpt1->Scale(8.15/5998860);

 



 gtaupt_1_sumpt1->Add(gtaupt_2_sumpt1);
 gtaupt_1_sumpt1->Add(gtaupt_3_sumpt1);
 


 gtaupt_1_sumpt1->Divide(gjetpt_1_sumpt1);
 gtaupt_1_sumpt1->SetMarkerStyle(20);

TH1F* hist1_sumpt2 = new TH1F("hist1_sumpt2","hist1_sumpt2",16,binning);
TH1F* hist2_sumpt2 = new TH1F("hist2_sumpt2","hist2_sumpt2",16,binning);
TH1F* hist3_sumpt2 = new TH1F("hist3_sumpt2","hist3_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumpt2",vars);

 TH1F* jetpt_1_sumpt2 = (TH1F*)gDirectory->Get("hist1_sumpt2");
 TH1F* jetpt_2_sumpt2 = (TH1F*)gDirectory->Get("hist2_sumpt2");
 TH1F* jetpt_3_sumpt2 = (TH1F*)gDirectory->Get("hist3_sumpt2");

 jetpt_1_sumpt2->Sumw2();
 jetpt_2_sumpt2->Sumw2();
 jetpt_3_sumpt2->Sumw2();


 jetpt_1_sumpt2->Scale(988.8/9987968);
 jetpt_2_sumpt2->Scale(66.3/6000000);
 jetpt_3_sumpt2->Scale(8.15/5998860);

 jetpt_1_sumpt2->Add(jetpt_2_sumpt2);
 jetpt_1_sumpt2->Add(jetpt_3_sumpt2);



TH1F* hist11_sumpt2 = new TH1F("hist11_sumpt2","hist11_sumpt2",16,binning);
TH1F* hist21_sumpt2 = new TH1F("hist21_sumpt2","hist21_sumpt2",16,binning);
TH1F* hist31_sumpt2 = new TH1F("hist31_sumpt2","hist31_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumpt2",vars);

 TH1F *taupt_1_sumpt2 = (TH1F*)gDirectory->Get("hist11_sumpt2");
 TH1F *taupt_2_sumpt2 = (TH1F*)gDirectory->Get("hist21_sumpt2");
 TH1F *taupt_3_sumpt2 = (TH1F*)gDirectory->Get("hist31_sumpt2");

 taupt_1_sumpt2->Sumw2();
 taupt_2_sumpt2->Sumw2();
 taupt_3_sumpt2->Sumw2();


 taupt_1_sumpt2->Scale(988.8/9987968);
 taupt_2_sumpt2->Scale(66.3/6000000);
 taupt_3_sumpt2->Scale(8.15/5998860);

 



 taupt_1_sumpt2->Add(taupt_2_sumpt2);
 taupt_1_sumpt2->Add(taupt_3_sumpt2);
 


 taupt_1_sumpt2->Divide(jetpt_1_sumpt2);
 taupt_1_sumpt2->SetMarkerStyle(20);
 taupt_1_sumpt2->SetMarkerColor(kRed);


TH1F* lhist1_sumpt2 = new TH1F("lhist1_sumpt2","lhist1_sumpt2",16,binning);
TH1F* lhist2_sumpt2 = new TH1F("lhist2_sumpt2","lhist2_sumpt2",16,binning);
TH1F* lhist3_sumpt2 = new TH1F("lhist3_sumpt2","lhist3_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumpt2",vars);

 TH1F* ljetpt_1_sumpt2 = (TH1F*)gDirectory->Get("lhist1_sumpt2");
 TH1F* ljetpt_2_sumpt2 = (TH1F*)gDirectory->Get("lhist2_sumpt2");
 TH1F* ljetpt_3_sumpt2 = (TH1F*)gDirectory->Get("lhist3_sumpt2");

 ljetpt_1_sumpt2->Sumw2();
 ljetpt_2_sumpt2->Sumw2();
 ljetpt_3_sumpt2->Sumw2();


 ljetpt_1_sumpt2->Scale(988.8/9987968);
 ljetpt_2_sumpt2->Scale(66.3/6000000);
 ljetpt_3_sumpt2->Scale(8.15/5998860);

 ljetpt_1_sumpt2->Add(ljetpt_2_sumpt2);
 ljetpt_1_sumpt2->Add(ljetpt_3_sumpt2);



TH1F* lhist11_sumpt2 = new TH1F("lhist11_sumpt2","lhist11_sumpt2",16,binning);
TH1F* lhist21_sumpt2 = new TH1F("lhist21_sumpt2","lhist21_sumpt2",16,binning);
TH1F* lhist31_sumpt2 = new TH1F("lhist31_sumpt2","lhist31_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumpt2",vars);

 TH1F *ltaupt_1_sumpt2 = (TH1F*)gDirectory->Get("lhist11_sumpt2");
 TH1F *ltaupt_2_sumpt2 = (TH1F*)gDirectory->Get("lhist21_sumpt2");
 TH1F *ltaupt_3_sumpt2 = (TH1F*)gDirectory->Get("lhist31_sumpt2");

 ltaupt_1_sumpt2->Sumw2();
 ltaupt_2_sumpt2->Sumw2();
 ltaupt_3_sumpt2->Sumw2();


 ltaupt_1_sumpt2->Scale(988.8/9987968);
 ltaupt_2_sumpt2->Scale(66.3/6000000);
 ltaupt_3_sumpt2->Scale(8.15/5998860);

 



 ltaupt_1_sumpt2->Add(ltaupt_2_sumpt2);
 ltaupt_1_sumpt2->Add(ltaupt_3_sumpt2);
 


 ltaupt_1_sumpt2->Divide(ljetpt_1_sumpt2);
 ltaupt_1_sumpt2->SetMarkerStyle(20);
 ltaupt_1_sumpt2->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt2 = new TH1F("bhist1_sumpt2","bhist1_sumpt2",16,binning);
TH1F* bhist2_sumpt2 = new TH1F("bhist2_sumpt2","bhist2_sumpt2",16,binning);
TH1F* bhist3_sumpt2 = new TH1F("bhist3_sumpt2","bhist3_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumpt2",vars);

 TH1F* bjetpt_1_sumpt2 = (TH1F*)gDirectory->Get("bhist1_sumpt2");
 TH1F* bjetpt_2_sumpt2 = (TH1F*)gDirectory->Get("bhist2_sumpt2");
 TH1F* bjetpt_3_sumpt2 = (TH1F*)gDirectory->Get("bhist3_sumpt2");

 bjetpt_1_sumpt2->Sumw2();
 bjetpt_2_sumpt2->Sumw2();
 bjetpt_3_sumpt2->Sumw2();


 bjetpt_1_sumpt2->Scale(988.8/9987968);
 bjetpt_2_sumpt2->Scale(66.3/6000000);
 bjetpt_3_sumpt2->Scale(8.15/5998860);

 bjetpt_1_sumpt2->Add(bjetpt_2_sumpt2);
 bjetpt_1_sumpt2->Add(bjetpt_3_sumpt2);



TH1F* bhist11_sumpt2 = new TH1F("bhist11_sumpt2","bhist11_sumpt2",16,binning);
TH1F* bhist21_sumpt2 = new TH1F("bhist21_sumpt2","bhist21_sumpt2",16,binning);
TH1F* bhist31_sumpt2 = new TH1F("bhist31_sumpt2","bhist31_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumpt2",vars);

 TH1F *btaupt_1_sumpt2 = (TH1F*)gDirectory->Get("bhist11_sumpt2");
 TH1F *btaupt_2_sumpt2 = (TH1F*)gDirectory->Get("bhist21_sumpt2");
 TH1F *btaupt_3_sumpt2 = (TH1F*)gDirectory->Get("bhist31_sumpt2");

 btaupt_1_sumpt2->Sumw2();
 btaupt_2_sumpt2->Sumw2();
 btaupt_3_sumpt2->Sumw2();


 btaupt_1_sumpt2->Scale(988.8/9987968);
 btaupt_2_sumpt2->Scale(66.3/6000000);
 btaupt_3_sumpt2->Scale(8.15/5998860);

 



 btaupt_1_sumpt2->Add(btaupt_2_sumpt2);
 btaupt_1_sumpt2->Add(btaupt_3_sumpt2);
 


 btaupt_1_sumpt2->Divide(bjetpt_1_sumpt2);
 btaupt_1_sumpt2->SetMarkerStyle(20);
 btaupt_1_sumpt2->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt2 = new TH1F("ghist1_sumpt2","ghist1_sumpt2",16,binning);
TH1F* ghist2_sumpt2 = new TH1F("ghist2_sumpt2","ghist2_sumpt2",16,binning);
TH1F* ghist3_sumpt2 = new TH1F("ghist3_sumpt2","ghist3_sumpt2",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumpt2",vars);

 TH1F* gjetpt_1_sumpt2 = (TH1F*)gDirectory->Get("ghist1_sumpt2");
 TH1F* gjetpt_2_sumpt2 = (TH1F*)gDirectory->Get("ghist2_sumpt2");
 TH1F* gjetpt_3_sumpt2 = (TH1F*)gDirectory->Get("ghist3_sumpt2");

 gjetpt_1_sumpt2->Sumw2();
 gjetpt_2_sumpt2->Sumw2();
 gjetpt_3_sumpt2->Sumw2();


 gjetpt_1_sumpt2->Scale(988.8/9987968);
 gjetpt_2_sumpt2->Scale(66.3/6000000);
 gjetpt_3_sumpt2->Scale(8.15/5998860);

 gjetpt_1_sumpt2->Add(gjetpt_2_sumpt2);
 gjetpt_1_sumpt2->Add(gjetpt_3_sumpt2);



TH1F* ghist11_sumpt2 = new TH1F("ghist11_sumpt2","ghist11_sumpt2",16,binning);
TH1F* ghist21_sumpt2 = new TH1F("ghist21_sumpt2","ghist21_sumpt2",16,binning);
TH1F* ghist31_sumpt2 = new TH1F("ghist31_sumpt2","ghist31_sumpt2",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[0]+"&&sumpt<"+limits[1];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt2",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumpt2",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumpt2",vars);

 TH1F *gtaupt_1_sumpt2 = (TH1F*)gDirectory->Get("ghist11_sumpt2");
 TH1F *gtaupt_2_sumpt2 = (TH1F*)gDirectory->Get("ghist21_sumpt2");
 TH1F *gtaupt_3_sumpt2 = (TH1F*)gDirectory->Get("ghist31_sumpt2");

 gtaupt_1_sumpt2->Sumw2();
 gtaupt_2_sumpt2->Sumw2();
 gtaupt_3_sumpt2->Sumw2();


 gtaupt_1_sumpt2->Scale(988.8/9987968);
 gtaupt_2_sumpt2->Scale(66.3/6000000);
 gtaupt_3_sumpt2->Scale(8.15/5998860);

 



 gtaupt_1_sumpt2->Add(gtaupt_2_sumpt2);
 gtaupt_1_sumpt2->Add(gtaupt_3_sumpt2);
 


 gtaupt_1_sumpt2->Divide(gjetpt_1_sumpt2);
 gtaupt_1_sumpt2->SetMarkerStyle(20);

TH1F* hist1_sumpt3 = new TH1F("hist1_sumpt3","hist1_sumpt3",16,binning);
TH1F* hist2_sumpt3 = new TH1F("hist2_sumpt3","hist2_sumpt3",16,binning);
TH1F* hist3_sumpt3 = new TH1F("hist3_sumpt3","hist3_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumpt3",vars);

 TH1F* jetpt_1_sumpt3 = (TH1F*)gDirectory->Get("hist1_sumpt3");
 TH1F* jetpt_2_sumpt3 = (TH1F*)gDirectory->Get("hist2_sumpt3");
 TH1F* jetpt_3_sumpt3 = (TH1F*)gDirectory->Get("hist3_sumpt3");

 jetpt_1_sumpt3->Sumw2();
 jetpt_2_sumpt3->Sumw2();
 jetpt_3_sumpt3->Sumw2();


 jetpt_1_sumpt3->Scale(988.8/9987968);
 jetpt_2_sumpt3->Scale(66.3/6000000);
 jetpt_3_sumpt3->Scale(8.15/5998860);

 jetpt_1_sumpt3->Add(jetpt_2_sumpt3);
 jetpt_1_sumpt3->Add(jetpt_3_sumpt3);



TH1F* hist11_sumpt3 = new TH1F("hist11_sumpt3","hist11_sumpt3",16,binning);
TH1F* hist21_sumpt3 = new TH1F("hist21_sumpt3","hist21_sumpt3",16,binning);
TH1F* hist31_sumpt3 = new TH1F("hist31_sumpt3","hist31_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumpt3",vars);

 TH1F *taupt_1_sumpt3 = (TH1F*)gDirectory->Get("hist11_sumpt3");
 TH1F *taupt_2_sumpt3 = (TH1F*)gDirectory->Get("hist21_sumpt3");
 TH1F *taupt_3_sumpt3 = (TH1F*)gDirectory->Get("hist31_sumpt3");

 taupt_1_sumpt3->Sumw2();
 taupt_2_sumpt3->Sumw2();
 taupt_3_sumpt3->Sumw2();


 taupt_1_sumpt3->Scale(988.8/9987968);
 taupt_2_sumpt3->Scale(66.3/6000000);
 taupt_3_sumpt3->Scale(8.15/5998860);

 



 taupt_1_sumpt3->Add(taupt_2_sumpt3);
 taupt_1_sumpt3->Add(taupt_3_sumpt3);
 


 taupt_1_sumpt3->Divide(jetpt_1_sumpt3);
 taupt_1_sumpt3->SetMarkerStyle(20);
 taupt_1_sumpt3->SetMarkerColor(kRed);


TH1F* lhist1_sumpt3 = new TH1F("lhist1_sumpt3","lhist1_sumpt3",16,binning);
TH1F* lhist2_sumpt3 = new TH1F("lhist2_sumpt3","lhist2_sumpt3",16,binning);
TH1F* lhist3_sumpt3 = new TH1F("lhist3_sumpt3","lhist3_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumpt3",vars);

 TH1F* ljetpt_1_sumpt3 = (TH1F*)gDirectory->Get("lhist1_sumpt3");
 TH1F* ljetpt_2_sumpt3 = (TH1F*)gDirectory->Get("lhist2_sumpt3");
 TH1F* ljetpt_3_sumpt3 = (TH1F*)gDirectory->Get("lhist3_sumpt3");

 ljetpt_1_sumpt3->Sumw2();
 ljetpt_2_sumpt3->Sumw2();
 ljetpt_3_sumpt3->Sumw2();


 ljetpt_1_sumpt3->Scale(988.8/9987968);
 ljetpt_2_sumpt3->Scale(66.3/6000000);
 ljetpt_3_sumpt3->Scale(8.15/5998860);

 ljetpt_1_sumpt3->Add(ljetpt_2_sumpt3);
 ljetpt_1_sumpt3->Add(ljetpt_3_sumpt3);



TH1F* lhist11_sumpt3 = new TH1F("lhist11_sumpt3","lhist11_sumpt3",16,binning);
TH1F* lhist21_sumpt3 = new TH1F("lhist21_sumpt3","lhist21_sumpt3",16,binning);
TH1F* lhist31_sumpt3 = new TH1F("lhist31_sumpt3","lhist31_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumpt3",vars);

 TH1F *ltaupt_1_sumpt3 = (TH1F*)gDirectory->Get("lhist11_sumpt3");
 TH1F *ltaupt_2_sumpt3 = (TH1F*)gDirectory->Get("lhist21_sumpt3");
 TH1F *ltaupt_3_sumpt3 = (TH1F*)gDirectory->Get("lhist31_sumpt3");

 ltaupt_1_sumpt3->Sumw2();
 ltaupt_2_sumpt3->Sumw2();
 ltaupt_3_sumpt3->Sumw2();


 ltaupt_1_sumpt3->Scale(988.8/9987968);
 ltaupt_2_sumpt3->Scale(66.3/6000000);
 ltaupt_3_sumpt3->Scale(8.15/5998860);

 



 ltaupt_1_sumpt3->Add(ltaupt_2_sumpt3);
 ltaupt_1_sumpt3->Add(ltaupt_3_sumpt3);
 


 ltaupt_1_sumpt3->Divide(ljetpt_1_sumpt3);
 ltaupt_1_sumpt3->SetMarkerStyle(20);
 ltaupt_1_sumpt3->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt3 = new TH1F("bhist1_sumpt3","bhist1_sumpt3",16,binning);
TH1F* bhist2_sumpt3 = new TH1F("bhist2_sumpt3","bhist2_sumpt3",16,binning);
TH1F* bhist3_sumpt3 = new TH1F("bhist3_sumpt3","bhist3_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumpt3",vars);

 TH1F* bjetpt_1_sumpt3 = (TH1F*)gDirectory->Get("bhist1_sumpt3");
 TH1F* bjetpt_2_sumpt3 = (TH1F*)gDirectory->Get("bhist2_sumpt3");
 TH1F* bjetpt_3_sumpt3 = (TH1F*)gDirectory->Get("bhist3_sumpt3");

 bjetpt_1_sumpt3->Sumw2();
 bjetpt_2_sumpt3->Sumw2();
 bjetpt_3_sumpt3->Sumw2();


 bjetpt_1_sumpt3->Scale(988.8/9987968);
 bjetpt_2_sumpt3->Scale(66.3/6000000);
 bjetpt_3_sumpt3->Scale(8.15/5998860);

 bjetpt_1_sumpt3->Add(bjetpt_2_sumpt3);
 bjetpt_1_sumpt3->Add(bjetpt_3_sumpt3);



TH1F* bhist11_sumpt3 = new TH1F("bhist11_sumpt3","bhist11_sumpt3",16,binning);
TH1F* bhist21_sumpt3 = new TH1F("bhist21_sumpt3","bhist21_sumpt3",16,binning);
TH1F* bhist31_sumpt3 = new TH1F("bhist31_sumpt3","bhist31_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumpt3",vars);

 TH1F *btaupt_1_sumpt3 = (TH1F*)gDirectory->Get("bhist11_sumpt3");
 TH1F *btaupt_2_sumpt3 = (TH1F*)gDirectory->Get("bhist21_sumpt3");
 TH1F *btaupt_3_sumpt3 = (TH1F*)gDirectory->Get("bhist31_sumpt3");

 btaupt_1_sumpt3->Sumw2();
 btaupt_2_sumpt3->Sumw2();
 btaupt_3_sumpt3->Sumw2();


 btaupt_1_sumpt3->Scale(988.8/9987968);
 btaupt_2_sumpt3->Scale(66.3/6000000);
 btaupt_3_sumpt3->Scale(8.15/5998860);

 



 btaupt_1_sumpt3->Add(btaupt_2_sumpt3);
 btaupt_1_sumpt3->Add(btaupt_3_sumpt3);
 


 btaupt_1_sumpt3->Divide(bjetpt_1_sumpt3);
 btaupt_1_sumpt3->SetMarkerStyle(20);
 btaupt_1_sumpt3->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt3 = new TH1F("ghist1_sumpt3","ghist1_sumpt3",16,binning);
TH1F* ghist2_sumpt3 = new TH1F("ghist2_sumpt3","ghist2_sumpt3",16,binning);
TH1F* ghist3_sumpt3 = new TH1F("ghist3_sumpt3","ghist3_sumpt3",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumpt3",vars);

 TH1F* gjetpt_1_sumpt3 = (TH1F*)gDirectory->Get("ghist1_sumpt3");
 TH1F* gjetpt_2_sumpt3 = (TH1F*)gDirectory->Get("ghist2_sumpt3");
 TH1F* gjetpt_3_sumpt3 = (TH1F*)gDirectory->Get("ghist3_sumpt3");

 gjetpt_1_sumpt3->Sumw2();
 gjetpt_2_sumpt3->Sumw2();
 gjetpt_3_sumpt3->Sumw2();


 gjetpt_1_sumpt3->Scale(988.8/9987968);
 gjetpt_2_sumpt3->Scale(66.3/6000000);
 gjetpt_3_sumpt3->Scale(8.15/5998860);

 gjetpt_1_sumpt3->Add(gjetpt_2_sumpt3);
 gjetpt_1_sumpt3->Add(gjetpt_3_sumpt3);



TH1F* ghist11_sumpt3 = new TH1F("ghist11_sumpt3","ghist11_sumpt3",16,binning);
TH1F* ghist21_sumpt3 = new TH1F("ghist21_sumpt3","ghist21_sumpt3",16,binning);
TH1F* ghist31_sumpt3 = new TH1F("ghist31_sumpt3","ghist31_sumpt3",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[1]+"&&sumpt<"+limits[2];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt3",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumpt3",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumpt3",vars);

 TH1F *gtaupt_1_sumpt3 = (TH1F*)gDirectory->Get("ghist11_sumpt3");
 TH1F *gtaupt_2_sumpt3 = (TH1F*)gDirectory->Get("ghist21_sumpt3");
 TH1F *gtaupt_3_sumpt3 = (TH1F*)gDirectory->Get("ghist31_sumpt3");

 gtaupt_1_sumpt3->Sumw2();
 gtaupt_2_sumpt3->Sumw2();
 gtaupt_3_sumpt3->Sumw2();


 gtaupt_1_sumpt3->Scale(988.8/9987968);
 gtaupt_2_sumpt3->Scale(66.3/6000000);
 gtaupt_3_sumpt3->Scale(8.15/5998860);

 



 gtaupt_1_sumpt3->Add(gtaupt_2_sumpt3);
 gtaupt_1_sumpt3->Add(gtaupt_3_sumpt3);
 


 gtaupt_1_sumpt3->Divide(gjetpt_1_sumpt3);
 gtaupt_1_sumpt3->SetMarkerStyle(20);



TH1F* hist1_sumpt4 = new TH1F("hist1_sumpt4","hist1_sumpt4",16,binning);
TH1F* hist2_sumpt4 = new TH1F("hist2_sumpt4","hist2_sumpt4",16,binning);
TH1F* hist3_sumpt4 = new TH1F("hist3_sumpt4","hist3_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumpt4",vars);

 TH1F* jetpt_1_sumpt4 = (TH1F*)gDirectory->Get("hist1_sumpt4");
 TH1F* jetpt_2_sumpt4 = (TH1F*)gDirectory->Get("hist2_sumpt4");
 TH1F* jetpt_3_sumpt4 = (TH1F*)gDirectory->Get("hist3_sumpt4");

 jetpt_1_sumpt4->Sumw2();
 jetpt_2_sumpt4->Sumw2();
 jetpt_3_sumpt4->Sumw2();


 jetpt_1_sumpt4->Scale(988.8/9987968);
 jetpt_2_sumpt4->Scale(66.3/6000000);
 jetpt_3_sumpt4->Scale(8.15/5998860);

 jetpt_1_sumpt4->Add(jetpt_2_sumpt4);
 jetpt_1_sumpt4->Add(jetpt_3_sumpt4);



TH1F* hist11_sumpt4 = new TH1F("hist11_sumpt4","hist11_sumpt4",16,binning);
TH1F* hist21_sumpt4 = new TH1F("hist21_sumpt4","hist21_sumpt4",16,binning);
TH1F* hist31_sumpt4 = new TH1F("hist31_sumpt4","hist31_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumpt4",vars);

 TH1F *taupt_1_sumpt4 = (TH1F*)gDirectory->Get("hist11_sumpt4");
 TH1F *taupt_2_sumpt4 = (TH1F*)gDirectory->Get("hist21_sumpt4");
 TH1F *taupt_3_sumpt4 = (TH1F*)gDirectory->Get("hist31_sumpt4");

 taupt_1_sumpt4->Sumw2();
 taupt_2_sumpt4->Sumw2();
 taupt_3_sumpt4->Sumw2();


 taupt_1_sumpt4->Scale(988.8/9987968);
 taupt_2_sumpt4->Scale(66.3/6000000);
 taupt_3_sumpt4->Scale(8.15/5998860);

 



 taupt_1_sumpt4->Add(taupt_2_sumpt4);
 taupt_1_sumpt4->Add(taupt_3_sumpt4);
 


 taupt_1_sumpt4->Divide(jetpt_1_sumpt4);
 taupt_1_sumpt4->SetMarkerStyle(20);
 taupt_1_sumpt4->SetMarkerColor(kRed);


TH1F* lhist1_sumpt4 = new TH1F("lhist1_sumpt4","lhist1_sumpt4",16,binning);
TH1F* lhist2_sumpt4 = new TH1F("lhist2_sumpt4","lhist2_sumpt4",16,binning);
TH1F* lhist3_sumpt4 = new TH1F("lhist3_sumpt4","lhist3_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumpt4",vars);

 TH1F* ljetpt_1_sumpt4 = (TH1F*)gDirectory->Get("lhist1_sumpt4");
 TH1F* ljetpt_2_sumpt4 = (TH1F*)gDirectory->Get("lhist2_sumpt4");
 TH1F* ljetpt_3_sumpt4 = (TH1F*)gDirectory->Get("lhist3_sumpt4");

 ljetpt_1_sumpt4->Sumw2();
 ljetpt_2_sumpt4->Sumw2();
 ljetpt_3_sumpt4->Sumw2();


 ljetpt_1_sumpt4->Scale(988.8/9987968);
 ljetpt_2_sumpt4->Scale(66.3/6000000);
 ljetpt_3_sumpt4->Scale(8.15/5998860);

 ljetpt_1_sumpt4->Add(ljetpt_2_sumpt4);
 ljetpt_1_sumpt4->Add(ljetpt_3_sumpt4);



TH1F* lhist11_sumpt4 = new TH1F("lhist11_sumpt4","lhist11_sumpt4",16,binning);
TH1F* lhist21_sumpt4 = new TH1F("lhist21_sumpt4","lhist21_sumpt4",16,binning);
TH1F* lhist31_sumpt4 = new TH1F("lhist31_sumpt4","lhist31_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumpt4",vars);

 TH1F *ltaupt_1_sumpt4 = (TH1F*)gDirectory->Get("lhist11_sumpt4");
 TH1F *ltaupt_2_sumpt4 = (TH1F*)gDirectory->Get("lhist21_sumpt4");
 TH1F *ltaupt_3_sumpt4 = (TH1F*)gDirectory->Get("lhist31_sumpt4");

 ltaupt_1_sumpt4->Sumw2();
 ltaupt_2_sumpt4->Sumw2();
 ltaupt_3_sumpt4->Sumw2();


 ltaupt_1_sumpt4->Scale(988.8/9987968);
 ltaupt_2_sumpt4->Scale(66.3/6000000);
 ltaupt_3_sumpt4->Scale(8.15/5998860);

 



 ltaupt_1_sumpt4->Add(ltaupt_2_sumpt4);
 ltaupt_1_sumpt4->Add(ltaupt_3_sumpt4);
 


 ltaupt_1_sumpt4->Divide(ljetpt_1_sumpt4);
 ltaupt_1_sumpt4->SetMarkerStyle(20);
 ltaupt_1_sumpt4->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt4 = new TH1F("bhist1_sumpt4","bhist1_sumpt4",16,binning);
TH1F* bhist2_sumpt4 = new TH1F("bhist2_sumpt4","bhist2_sumpt4",16,binning);
TH1F* bhist3_sumpt4 = new TH1F("bhist3_sumpt4","bhist3_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumpt4",vars);

 TH1F* bjetpt_1_sumpt4 = (TH1F*)gDirectory->Get("bhist1_sumpt4");
 TH1F* bjetpt_2_sumpt4 = (TH1F*)gDirectory->Get("bhist2_sumpt4");
 TH1F* bjetpt_3_sumpt4 = (TH1F*)gDirectory->Get("bhist3_sumpt4");

 bjetpt_1_sumpt4->Sumw2();
 bjetpt_2_sumpt4->Sumw2();
 bjetpt_3_sumpt4->Sumw2();


 bjetpt_1_sumpt4->Scale(988.8/9987968);
 bjetpt_2_sumpt4->Scale(66.3/6000000);
 bjetpt_3_sumpt4->Scale(8.15/5998860);

 bjetpt_1_sumpt4->Add(bjetpt_2_sumpt4);
 bjetpt_1_sumpt4->Add(bjetpt_3_sumpt4);



TH1F* bhist11_sumpt4 = new TH1F("bhist11_sumpt4","bhist11_sumpt4",16,binning);
TH1F* bhist21_sumpt4 = new TH1F("bhist21_sumpt4","bhist21_sumpt4",16,binning);
TH1F* bhist31_sumpt4 = new TH1F("bhist31_sumpt4","bhist31_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumpt4",vars);

 TH1F *btaupt_1_sumpt4 = (TH1F*)gDirectory->Get("bhist11_sumpt4");
 TH1F *btaupt_2_sumpt4 = (TH1F*)gDirectory->Get("bhist21_sumpt4");
 TH1F *btaupt_3_sumpt4 = (TH1F*)gDirectory->Get("bhist31_sumpt4");

 btaupt_1_sumpt4->Sumw2();
 btaupt_2_sumpt4->Sumw2();
 btaupt_3_sumpt4->Sumw2();


 btaupt_1_sumpt4->Scale(988.8/9987968);
 btaupt_2_sumpt4->Scale(66.3/6000000);
 btaupt_3_sumpt4->Scale(8.15/5998860);

 



 btaupt_1_sumpt4->Add(btaupt_2_sumpt4);
 btaupt_1_sumpt4->Add(btaupt_3_sumpt4);
 


 btaupt_1_sumpt4->Divide(bjetpt_1_sumpt4);
 btaupt_1_sumpt4->SetMarkerStyle(20);
 btaupt_1_sumpt4->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt4 = new TH1F("ghist1_sumpt4","ghist1_sumpt4",16,binning);
TH1F* ghist2_sumpt4 = new TH1F("ghist2_sumpt4","ghist2_sumpt4",16,binning);
TH1F* ghist3_sumpt4 = new TH1F("ghist3_sumpt4","ghist3_sumpt4",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumpt4",vars);

 TH1F* gjetpt_1_sumpt4 = (TH1F*)gDirectory->Get("ghist1_sumpt4");
 TH1F* gjetpt_2_sumpt4 = (TH1F*)gDirectory->Get("ghist2_sumpt4");
 TH1F* gjetpt_3_sumpt4 = (TH1F*)gDirectory->Get("ghist3_sumpt4");

 gjetpt_1_sumpt4->Sumw2();
 gjetpt_2_sumpt4->Sumw2();
 gjetpt_3_sumpt4->Sumw2();


 gjetpt_1_sumpt4->Scale(988.8/9987968);
 gjetpt_2_sumpt4->Scale(66.3/6000000);
 gjetpt_3_sumpt4->Scale(8.15/5998860);

 gjetpt_1_sumpt4->Add(gjetpt_2_sumpt4);
 gjetpt_1_sumpt4->Add(gjetpt_3_sumpt4);



TH1F* ghist11_sumpt4 = new TH1F("ghist11_sumpt4","ghist11_sumpt4",16,binning);
TH1F* ghist21_sumpt4 = new TH1F("ghist21_sumpt4","ghist21_sumpt4",16,binning);
TH1F* ghist31_sumpt4 = new TH1F("ghist31_sumpt4","ghist31_sumpt4",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[2]+"&&sumpt<"+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt4",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumpt4",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumpt4",vars);

 TH1F *gtaupt_1_sumpt4 = (TH1F*)gDirectory->Get("ghist11_sumpt4");
 TH1F *gtaupt_2_sumpt4 = (TH1F*)gDirectory->Get("ghist21_sumpt4");
 TH1F *gtaupt_3_sumpt4 = (TH1F*)gDirectory->Get("ghist31_sumpt4");

 gtaupt_1_sumpt4->Sumw2();
 gtaupt_2_sumpt4->Sumw2();
 gtaupt_3_sumpt4->Sumw2();


 gtaupt_1_sumpt4->Scale(988.8/9987968);
 gtaupt_2_sumpt4->Scale(66.3/6000000);
 gtaupt_3_sumpt4->Scale(8.15/5998860);

 



 gtaupt_1_sumpt4->Add(gtaupt_2_sumpt4);
 gtaupt_1_sumpt4->Add(gtaupt_3_sumpt4);
 


 gtaupt_1_sumpt4->Divide(gjetpt_1_sumpt4);
 gtaupt_1_sumpt4->SetMarkerStyle(20);

TH1F* hist1_sumpt5 = new TH1F("hist1_sumpt5","hist1_sumpt5",16,binning);
TH1F* hist2_sumpt5 = new TH1F("hist2_sumpt5","hist2_sumpt5",16,binning);
TH1F* hist3_sumpt5 = new TH1F("hist3_sumpt5","hist3_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist1_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>hist2_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>hist3_sumpt5",vars);

 TH1F* jetpt_1_sumpt5 = (TH1F*)gDirectory->Get("hist1_sumpt5");
 TH1F* jetpt_2_sumpt5 = (TH1F*)gDirectory->Get("hist2_sumpt5");
 TH1F* jetpt_3_sumpt5 = (TH1F*)gDirectory->Get("hist3_sumpt5");

 jetpt_1_sumpt5->Sumw2();
 jetpt_2_sumpt5->Sumw2();
 jetpt_3_sumpt5->Sumw2();


 jetpt_1_sumpt5->Scale(988.8/9987968);
 jetpt_2_sumpt5->Scale(66.3/6000000);
 jetpt_3_sumpt5->Scale(8.15/5998860);

 jetpt_1_sumpt5->Add(jetpt_2_sumpt5);
 jetpt_1_sumpt5->Add(jetpt_3_sumpt5);



TH1F* hist11_sumpt5 = new TH1F("hist11_sumpt5","hist11_sumpt5",16,binning);
TH1F* hist21_sumpt5 = new TH1F("hist21_sumpt5","hist21_sumpt5",16,binning);
TH1F* hist31_sumpt5 = new TH1F("hist31_sumpt5","hist31_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>hist11_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>hist21_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>hist31_sumpt5",vars);

 TH1F *taupt_1_sumpt5 = (TH1F*)gDirectory->Get("hist11_sumpt5");
 TH1F *taupt_2_sumpt5 = (TH1F*)gDirectory->Get("hist21_sumpt5");
 TH1F *taupt_3_sumpt5 = (TH1F*)gDirectory->Get("hist31_sumpt5");

 taupt_1_sumpt5->Sumw2();
 taupt_2_sumpt5->Sumw2();
 taupt_3_sumpt5->Sumw2();


 taupt_1_sumpt5->Scale(988.8/9987968);
 taupt_2_sumpt5->Scale(66.3/6000000);
 taupt_3_sumpt5->Scale(8.15/5998860);

 



 taupt_1_sumpt5->Add(taupt_2_sumpt5);
 taupt_1_sumpt5->Add(taupt_3_sumpt5);
 


 taupt_1_sumpt5->Divide(jetpt_1_sumpt5);
 taupt_1_sumpt5->SetMarkerStyle(20);
 taupt_1_sumpt5->SetMarkerColor(kRed);


TH1F* lhist1_sumpt5 = new TH1F("lhist1_sumpt5","lhist1_sumpt5",16,binning);
TH1F* lhist2_sumpt5 = new TH1F("lhist2_sumpt5","lhist2_sumpt5",16,binning);
TH1F* lhist3_sumpt5 = new TH1F("lhist3_sumpt5","lhist3_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist1_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>lhist2_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>lhist3_sumpt5",vars);

 TH1F* ljetpt_1_sumpt5 = (TH1F*)gDirectory->Get("lhist1_sumpt5");
 TH1F* ljetpt_2_sumpt5 = (TH1F*)gDirectory->Get("lhist2_sumpt5");
 TH1F* ljetpt_3_sumpt5 = (TH1F*)gDirectory->Get("lhist3_sumpt5");

 ljetpt_1_sumpt5->Sumw2();
 ljetpt_2_sumpt5->Sumw2();
 ljetpt_3_sumpt5->Sumw2();


 ljetpt_1_sumpt5->Scale(988.8/9987968);
 ljetpt_2_sumpt5->Scale(66.3/6000000);
 ljetpt_3_sumpt5->Scale(8.15/5998860);

 ljetpt_1_sumpt5->Add(ljetpt_2_sumpt5);
 ljetpt_1_sumpt5->Add(ljetpt_3_sumpt5);



TH1F* lhist11_sumpt5 = new TH1F("lhist11_sumpt5","lhist11_sumpt5",16,binning);
TH1F* lhist21_sumpt5 = new TH1F("lhist21_sumpt5","lhist21_sumpt5",16,binning);
TH1F* lhist31_sumpt5 = new TH1F("lhist31_sumpt5","lhist31_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==1||TMath::Abs(jettype)==2||TMath::Abs(jettype)==3)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>lhist11_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>lhist21_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>lhist31_sumpt5",vars);

 TH1F *ltaupt_1_sumpt5 = (TH1F*)gDirectory->Get("lhist11_sumpt5");
 TH1F *ltaupt_2_sumpt5 = (TH1F*)gDirectory->Get("lhist21_sumpt5");
 TH1F *ltaupt_3_sumpt5 = (TH1F*)gDirectory->Get("lhist31_sumpt5");

 ltaupt_1_sumpt5->Sumw2();
 ltaupt_2_sumpt5->Sumw2();
 ltaupt_3_sumpt5->Sumw2();


 ltaupt_1_sumpt5->Scale(988.8/9987968);
 ltaupt_2_sumpt5->Scale(66.3/6000000);
 ltaupt_3_sumpt5->Scale(8.15/5998860);

 



 ltaupt_1_sumpt5->Add(ltaupt_2_sumpt5);
 ltaupt_1_sumpt5->Add(ltaupt_3_sumpt5);
 


 ltaupt_1_sumpt5->Divide(ljetpt_1_sumpt5);
 ltaupt_1_sumpt5->SetMarkerStyle(20);
 ltaupt_1_sumpt5->SetMarkerColor(kGreen+3);



TH1F* bhist1_sumpt5 = new TH1F("bhist1_sumpt5","bhist1_sumpt5",16,binning);
TH1F* bhist2_sumpt5 = new TH1F("bhist2_sumpt5","bhist2_sumpt5",16,binning);
TH1F* bhist3_sumpt5 = new TH1F("bhist3_sumpt5","bhist3_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist1_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>bhist2_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>bhist3_sumpt5",vars);

 TH1F* bjetpt_1_sumpt5 = (TH1F*)gDirectory->Get("bhist1_sumpt5");
 TH1F* bjetpt_2_sumpt5 = (TH1F*)gDirectory->Get("bhist2_sumpt5");
 TH1F* bjetpt_3_sumpt5 = (TH1F*)gDirectory->Get("bhist3_sumpt5");

 bjetpt_1_sumpt5->Sumw2();
 bjetpt_2_sumpt5->Sumw2();
 bjetpt_3_sumpt5->Sumw2();


 bjetpt_1_sumpt5->Scale(988.8/9987968);
 bjetpt_2_sumpt5->Scale(66.3/6000000);
 bjetpt_3_sumpt5->Scale(8.15/5998860);

 bjetpt_1_sumpt5->Add(bjetpt_2_sumpt5);
 bjetpt_1_sumpt5->Add(bjetpt_3_sumpt5);



TH1F* bhist11_sumpt5 = new TH1F("bhist11_sumpt5","bhist11_sumpt5",16,binning);
TH1F* bhist21_sumpt5 = new TH1F("bhist21_sumpt5","bhist21_sumpt5",16,binning);
TH1F* bhist31_sumpt5 = new TH1F("bhist31_sumpt5","bhist31_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==5)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>bhist11_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>bhist21_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>bhist31_sumpt5",vars);

 TH1F *btaupt_1_sumpt5 = (TH1F*)gDirectory->Get("bhist11_sumpt5");
 TH1F *btaupt_2_sumpt5 = (TH1F*)gDirectory->Get("bhist21_sumpt5");
 TH1F *btaupt_3_sumpt5 = (TH1F*)gDirectory->Get("bhist31_sumpt5");

 btaupt_1_sumpt5->Sumw2();
 btaupt_2_sumpt5->Sumw2();
 btaupt_3_sumpt5->Sumw2();


 btaupt_1_sumpt5->Scale(988.8/9987968);
 btaupt_2_sumpt5->Scale(66.3/6000000);
 btaupt_3_sumpt5->Scale(8.15/5998860);

 



 btaupt_1_sumpt5->Add(btaupt_2_sumpt5);
 btaupt_1_sumpt5->Add(btaupt_3_sumpt5);
 


 btaupt_1_sumpt5->Divide(bjetpt_1_sumpt5);
 btaupt_1_sumpt5->SetMarkerStyle(20);
 btaupt_1_sumpt5->SetMarkerColor(kBlue);


TH1F* ghist1_sumpt5 = new TH1F("ghist1_sumpt5","ghist1_sumpt5",16,binning);
TH1F* ghist2_sumpt5 = new TH1F("ghist2_sumpt5","ghist2_sumpt5",16,binning);
TH1F* ghist3_sumpt5 = new TH1F("ghist3_sumpt5","ghist3_sumpt5",16,binning);

vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist1_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>ghist2_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>ghist3_sumpt5",vars);

 TH1F* gjetpt_1_sumpt5 = (TH1F*)gDirectory->Get("ghist1_sumpt5");
 TH1F* gjetpt_2_sumpt5 = (TH1F*)gDirectory->Get("ghist2_sumpt5");
 TH1F* gjetpt_3_sumpt5 = (TH1F*)gDirectory->Get("ghist3_sumpt5");

 gjetpt_1_sumpt5->Sumw2();
 gjetpt_2_sumpt5->Sumw2();
 gjetpt_3_sumpt5->Sumw2();


 gjetpt_1_sumpt5->Scale(988.8/9987968);
 gjetpt_2_sumpt5->Scale(66.3/6000000);
 gjetpt_3_sumpt5->Scale(8.15/5998860);

 gjetpt_1_sumpt5->Add(gjetpt_2_sumpt5);
 gjetpt_1_sumpt5->Add(gjetpt_3_sumpt5);



TH1F* ghist11_sumpt5 = new TH1F("ghist11_sumpt5","ghist11_sumpt5",16,binning);
TH1F* ghist21_sumpt5 = new TH1F("ghist21_sumpt5","ghist21_sumpt5",16,binning);
TH1F* ghist31_sumpt5 = new TH1F("ghist31_sumpt5","ghist31_sumpt5",16,binning);


vars="jetpt>20&&TMath::Abs(jeteta)<2.3&&jetdz<0.2&&deltaR<0.5&&taudmpass>0.5&&tauloose>0.5&&(TMath::Abs(jettype)==21)&&sumpt>="+limits[3];
 qcd_tree_1->Draw("jetpt>>ghist11_sumpt5",vars);
 qcd_tree_2->Draw("jetpt>>ghist21_sumpt5",vars);
 qcd_tree_3->Draw("jetpt>>ghist31_sumpt5",vars);

 TH1F *gtaupt_1_sumpt5 = (TH1F*)gDirectory->Get("ghist11_sumpt5");
 TH1F *gtaupt_2_sumpt5 = (TH1F*)gDirectory->Get("ghist21_sumpt5");
 TH1F *gtaupt_3_sumpt5 = (TH1F*)gDirectory->Get("ghist31_sumpt5");

 gtaupt_1_sumpt5->Sumw2();
 gtaupt_2_sumpt5->Sumw2();
 gtaupt_3_sumpt5->Sumw2();


 gtaupt_1_sumpt5->Scale(988.8/9987968);
 gtaupt_2_sumpt5->Scale(66.3/6000000);
 gtaupt_3_sumpt5->Scale(8.15/5998860);

 



 gtaupt_1_sumpt5->Add(gtaupt_2_sumpt5);
 gtaupt_1_sumpt5->Add(gtaupt_3_sumpt5);
 


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
 TFile *outfile = new TFile("Jan29_qcd_fakerates_sumpt.root","RECREATE");
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
