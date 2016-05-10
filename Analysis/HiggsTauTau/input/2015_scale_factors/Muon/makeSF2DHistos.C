void makeSF2DHistos(){
  TH1::AddDirectory(kFALSE);
  TFile *fMuon_Mu8_eff = TFile::Open("Muon_Mu8_fall15.root");
  TGraphAsymmErrors *mu12_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu8_eff->Get("ZMassEtaLt0p9_MC"));
  TGraphAsymmErrors *mu12_medetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu8_eff->Get("ZMassEta0p9to1p2_MC"));
  TGraphAsymmErrors *mu12_highetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu8_eff->Get("ZMassEtaGt1p2_MC"));
  TGraphAsymmErrors *mu12_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu8_eff->Get("ZMassEtaLt0p9_Data"));
  TGraphAsymmErrors *mu12_medetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu8_eff->Get("ZMassEta0p9to1p2_Data"));
  TGraphAsymmErrors *mu12_highetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu8_eff->Get("ZMassEtaGt1p2_Data"));
  double* Mu8X = mu12_lowetamc->GetX();//-mu12_lowetamc->GetEXlow();
  double* Mu8EX = mu12_lowetamc->GetEXlow();
  int nbins12 = mu12_lowetamc->GetN();
  double ptbins12[nbins12+1];
  for(i=0;i<nbins12;i++){
    ptbins12[i] = Mu8X[i]-Mu8EX[i];
  } 
  ptbins12[nbins12] = 150;
  int nbinseta=3;
  double etabins[nbinseta+1];//
  etabins[0] = 0;
  etabins[1]= 0.9;
  etabins[2] = 1.2;
  etabins[3] = 2.6;
  TH2D *muon_mu12_mc = new TH2D("Muon_Mu8_MC_eff","Muon_Mu8_MC_eff",nbinseta,etabins,nbins12,ptbins12);
  TH2D *muon_mu12_data = new TH2D("Muon_Mu8_Data_eff","Muon_Mu8_Data_eff",nbinseta,etabins,nbins12,ptbins12);
  double *mu12_lowetamc_y = mu12_lowetamc->GetY();
  double *mu12_medetamc_y = mu12_medetamc->GetY();
  double *mu12_highetamc_y = mu12_highetamc->GetY();
  double *mu12_lowetadata_y = mu12_lowetadata->GetY();
  double *mu12_medetadata_y = mu12_medetadata->GetY();
  double *mu12_highetadata_y = mu12_highetadata->GetY();
  for(i=0;i<nbins12;i++){
   muon_mu12_mc->Fill(0.6,Mu8X[i],mu12_lowetamc_y[i]);
   muon_mu12_mc->Fill(1.1,Mu8X[i],mu12_medetamc_y[i]);
   muon_mu12_mc->Fill(2,Mu8X[i],mu12_highetamc_y[i]);
   muon_mu12_data->Fill(0.6,Mu8X[i],mu12_lowetadata_y[i]);
   muon_mu12_data->Fill(1,Mu8X[i],mu12_medetadata_y[i]);
   muon_mu12_data->Fill(2,Mu8X[i],mu12_highetadata_y[i]);
 }

 fMuon_Mu8_eff->Close();
 
  TFile *fMuon_Mu17_eff = TFile::Open("Muon_Mu17_fall15.root");
  TGraphAsymmErrors *mu17_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu17_eff->Get("ZMassEtaLt0p9_MC"));
  TGraphAsymmErrors *mu17_medetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu17_eff->Get("ZMassEta0p9to1p2_MC"));
  TGraphAsymmErrors *mu17_highetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu17_eff->Get("ZMassEtaGt1p2_MC"));
  TGraphAsymmErrors *mu17_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu17_eff->Get("ZMassEtaLt0p9_Data"));
  TGraphAsymmErrors *mu17_medetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu17_eff->Get("ZMassEta0p9to1p2_Data"));
  TGraphAsymmErrors *mu17_highetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_Mu17_eff->Get("ZMassEtaGt1p2_Data"));

  double* Mu17X = mu17_lowetamc->GetX();//-mu17_lowetamc->GetEXlow();
  double* Mu17EX = mu17_lowetamc->GetEXlow();
  int nbins17 = mu17_lowetamc->GetN();
  double ptbins17[nbins17+1];
  for(i=0;i<nbins17;i++){
    ptbins17[i] = Mu17X[i]-Mu17EX[i];
  } 
  ptbins17[nbins17] = 150;



  TH2D *muon_mu17_mc = new TH2D("Muon_Mu17_MC_eff","Muon_Mu17_MC_eff",nbinseta,etabins,nbins17,ptbins17);
  TH2D *muon_mu17_data = new TH2D("Muon_Mu17_Data_eff","Muon_Mu17_Data_eff",nbinseta,etabins,nbins17,ptbins17);
  double *mu17_lowetamc_y = mu17_lowetamc->GetY();
  double *mu17_medetamc_y = mu17_medetamc->GetY();
  double *mu17_highetamc_y = mu17_highetamc->GetY();
  double *mu17_lowetadata_y = mu17_lowetadata->GetY();
  double *mu17_medetadata_y = mu17_medetadata->GetY();
  double *mu17_highetadata_y = mu17_highetadata->GetY();
  for(i=0;i<nbins17;i++){
   muon_mu17_mc->Fill(0.6,Mu17X[i],mu17_lowetamc_y[i]);
   muon_mu17_mc->Fill(1,Mu17X[i],mu17_medetamc_y[i]);
   muon_mu17_mc->Fill(2,Mu17X[i],mu17_highetamc_y[i]);
   muon_mu17_data->Fill(0.6,Mu17X[i],mu17_lowetadata_y[i]);
   muon_mu17_data->Fill(1,Mu17X[i],mu17_medetadata_y[i]);
   muon_mu17_data->Fill(2,Mu17X[i],mu17_highetadata_y[i]);
 }

 fMuon_Mu17_eff->Close();

  TFile *fMuon_MuonIdIso0p10_eff = TFile::Open("Muon_IdIso0p1_fall15.root");
  TGraphAsymmErrors *muIdIso0p10_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p10_eff->Get("ZMassEtaLt0p9_MC"));
  TGraphAsymmErrors *muIdIso0p10_medetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p10_eff->Get("ZMassEta0p9to1p2_MC"));
  TGraphAsymmErrors *muIdIso0p10_highetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p10_eff->Get("ZMassEtaGt1p2_MC"));
  TGraphAsymmErrors *muIdIso0p10_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p10_eff->Get("ZMassEtaLt0p9_Data"));
  TGraphAsymmErrors *muIdIso0p10_medetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p10_eff->Get("ZMassEta0p9to1p2_Data"));
  TGraphAsymmErrors *muIdIso0p10_highetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p10_eff->Get("ZMassEtaGt1p2_Data"));
  double* MuonIdIso0p10X = muIdIso0p10_lowetamc->GetX();//-muIdIso0p10_lowetamc->GetEXlow();
  double* MuonIdIso0p10EX = muIdIso0p10_lowetamc->GetEXlow();
  int nbinsIdIso0p10 = muIdIso0p10_lowetamc->GetN();
  double ptbinsIdIso0p10[nbinsIdIso0p10+1];
  for(i=0;i<nbinsIdIso0p10;i++){
    ptbinsIdIso0p10[i] = MuonIdIso0p10X[i]-MuonIdIso0p10EX[i];
  } 
  ptbinsIdIso0p10[nbinsIdIso0p10] = 100;

  TH2D *muon_muIdIso0p10_mc = new TH2D("Muon_IdIso0p10_MC_eff","Muon_IdIso0p10_MC_eff",nbinseta,etabins,nbinsIdIso0p10,ptbinsIdIso0p10);
  TH2D *muon_muIdIso0p10_data = new TH2D("Muon_IdIso0p10_Data_eff","Muon_IdIso0p10_Data_eff",nbinseta,etabins,nbinsIdIso0p10,ptbinsIdIso0p10);
  double *muIdIso0p10_lowetamc_y = muIdIso0p10_lowetamc->GetY();
  double *muIdIso0p10_medetamc_y = muIdIso0p10_medetamc->GetY();
  double *muIdIso0p10_highetamc_y = muIdIso0p10_highetamc->GetY();
  double *muIdIso0p10_lowetadata_y = muIdIso0p10_lowetadata->GetY();
  double *muIdIso0p10_medetadata_y = muIdIso0p10_medetadata->GetY();
  double *muIdIso0p10_highetadata_y = muIdIso0p10_highetadata->GetY();

  for(i=0;i<nbinsIdIso0p10;i++){
   muon_muIdIso0p10_mc->Fill(0.6,MuonIdIso0p10X[i],muIdIso0p10_lowetamc_y[i]);
   muon_muIdIso0p10_mc->Fill(1,MuonIdIso0p10X[i],muIdIso0p10_medetamc_y[i]);
   muon_muIdIso0p10_mc->Fill(2,MuonIdIso0p10X[i],muIdIso0p10_highetamc_y[i]);
   muon_muIdIso0p10_data->Fill(0.6,MuonIdIso0p10X[i],muIdIso0p10_lowetadata_y[i]);
   muon_muIdIso0p10_data->Fill(1,MuonIdIso0p10X[i],muIdIso0p10_medetadata_y[i]);
   muon_muIdIso0p10_data->Fill(2,MuonIdIso0p10X[i],muIdIso0p10_highetadata_y[i]);
 }

 fMuon_MuonIdIso0p10_eff->Close();


  TFile *fMuon_MuonIdIso0p15_eff = TFile::Open("Muon_IdIso0p15_fall15.root");
  TGraphAsymmErrors *muIdIso0p15_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p15_eff->Get("ZMassEtaLt0p9_MC"));
  TGraphAsymmErrors *muIdIso0p15_medetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p15_eff->Get("ZMassEta0p9to1p2_MC"));
  TGraphAsymmErrors *muIdIso0p15_highetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p15_eff->Get("ZMassEtaGt1p2_MC"));
  TGraphAsymmErrors *muIdIso0p15_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p15_eff->Get("ZMassEtaLt0p9_Data"));
  TGraphAsymmErrors *muIdIso0p15_medetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p15_eff->Get("ZMassEta0p9to1p2_Data"));
  TGraphAsymmErrors *muIdIso0p15_highetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonIdIso0p15_eff->Get("ZMassEtaGt1p2_Data"));

  double *muIdIso0p15_lowetamc_y = muIdIso0p15_lowetamc->GetY();
  double *muIdIso0p15_medetamc_y = muIdIso0p15_medetamc->GetY();
  double *muIdIso0p15_highetamc_y = muIdIso0p15_highetamc->GetY();
  double *muIdIso0p15_lowetadata_y = muIdIso0p15_lowetadata->GetY();
  double *muIdIso0p15_medetadata_y = muIdIso0p15_medetadata->GetY();
  double *muIdIso0p15_highetadata_y = muIdIso0p15_highetadata->GetY();
  double* MuonIdIso0p15X = muIdIso0p15_lowetamc->GetX();//-muIdIso0p15_lowetamc->GetEXlow();
  double* MuonIdIso0p15EX = muIdIso0p15_lowetamc->GetEXlow();
  int nbinsIdIso0p15 = muIdIso0p15_lowetamc->GetN();
  double ptbinsIdIso0p15[nbinsIdIso0p15+1];
  for(i=0;i<nbinsIdIso0p15;i++){
    ptbinsIdIso0p15[i] = MuonIdIso0p15X[i]-MuonIdIso0p15EX[i];
  } 
  ptbinsIdIso0p15[nbinsIdIso0p15] = 100;
  TH2D *muon_muIdIso0p15_mc = new TH2D("Muon_IdIso0p15_MC_eff","Muon_IdIso0p15_MC_eff",nbinseta,etabins,nbinsIdIso0p15,ptbinsIdIso0p15);
  TH2D *muon_muIdIso0p15_data = new TH2D("Muon_IdIso0p15_Data_eff","Muon_IdIso0p15_Data_eff",nbinseta,etabins,nbinsIdIso0p15,ptbinsIdIso0p15);


  for(i=0;i<nbinsIdIso0p15;i++){
   muon_muIdIso0p15_mc->Fill(0.6,MuonIdIso0p15X[i],muIdIso0p15_lowetamc_y[i]);
   muon_muIdIso0p15_mc->Fill(1,MuonIdIso0p15X[i],muIdIso0p15_medetamc_y[i]);
   muon_muIdIso0p15_mc->Fill(2,MuonIdIso0p15X[i],muIdIso0p15_highetamc_y[i]);
   muon_muIdIso0p15_data->Fill(0.6,MuonIdIso0p15X[i],muIdIso0p15_lowetadata_y[i]);
   muon_muIdIso0p15_data->Fill(1,MuonIdIso0p15X[i],muIdIso0p15_medetadata_y[i]);
   muon_muIdIso0p15_data->Fill(2,MuonIdIso0p15X[i],muIdIso0p15_highetadata_y[i]);
 }

 fMuon_MuonIdIso0p15_eff->Close();

  TFile *fMuon_MuonSingleMu_eff = TFile::Open("Muon_IsoMu18_fall15.root");
  TGraphAsymmErrors *muSingleMu_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonSingleMu_eff->Get("ZMassEtaLt0p9_MC"));
  TGraphAsymmErrors *muSingleMu_medetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonSingleMu_eff->Get("ZMassEta0p9to1p2_MC"));
  TGraphAsymmErrors *muSingleMu_highetamc = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonSingleMu_eff->Get("ZMassEtaGt1p2_MC"));
  TGraphAsymmErrors *muSingleMu_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonSingleMu_eff->Get("ZMassEtaLt0p9_Data"));
  TGraphAsymmErrors *muSingleMu_medetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonSingleMu_eff->Get("ZMassEta0p9to1p2_Data"));
  TGraphAsymmErrors *muSingleMu_highetadata = dynamic_cast<TGraphAsymmErrors*>(fMuon_MuonSingleMu_eff->Get("ZMassEtaGt1p2_Data"));

  double *muSingleMu_lowetamc_y = muSingleMu_lowetamc->GetY();
  double *muSingleMu_medetamc_y = muSingleMu_medetamc->GetY();
  double *muSingleMu_highetamc_y = muSingleMu_highetamc->GetY();
  double *muSingleMu_lowetadata_y = muSingleMu_lowetadata->GetY();
  double *muSingleMu_medetadata_y = muSingleMu_medetadata->GetY();
  double *muSingleMu_highetadata_y = muSingleMu_highetadata->GetY();
  double* MuonSingleMuX = muSingleMu_lowetamc->GetX();//-muSingleMu_lowetamc->GetEXlow();
  double* MuonSingleMuEX = muSingleMu_lowetamc->GetEXlow();
  int nbinsSingleMu = muSingleMu_lowetamc->GetN();
  double ptbinsSingleMu[nbinsSingleMu+1];
  for(i=0;i<nbinsSingleMu;i++){
    ptbinsSingleMu[i] = MuonSingleMuX[i]-MuonSingleMuEX[i];
  } 
  ptbinsSingleMu[nbinsSingleMu] = 150;
  TH2D *muon_muSingleMu_mc = new TH2D("Muon_SingleMu_MC_eff","Muon_SingleMu_MC_eff",nbinseta,etabins,nbinsSingleMu,ptbinsSingleMu);
  TH2D *muon_muSingleMu_data = new TH2D("Muon_SingleMu_Data_eff","Muon_SingleMu_Data_eff",nbinseta,etabins,nbinsSingleMu,ptbinsSingleMu);

  for(i=0;i<nbinsSingleMu;i++){
   muon_muSingleMu_mc->Fill(0.6,MuonSingleMuX[i],muSingleMu_lowetamc_y[i]);
   muon_muSingleMu_mc->Fill(1,MuonSingleMuX[i],muSingleMu_medetamc_y[i]);
   muon_muSingleMu_mc->Fill(2,MuonSingleMuX[i],muSingleMu_highetamc_y[i]);
   muon_muSingleMu_data->Fill(0.6,MuonSingleMuX[i],muSingleMu_lowetadata_y[i]);
   muon_muSingleMu_data->Fill(1,MuonSingleMuX[i],muSingleMu_medetadata_y[i]);
   muon_muSingleMu_data->Fill(2,MuonSingleMuX[i],muSingleMu_highetadata_y[i]);
 }

 fMuon_MuonSingleMu_eff->Close();


 TFile *Muon_2DHistos = new TFile("Muon_2DHistos_fall15.root","RECREATE");
  muon_muSingleMu_mc->Write();
  muon_muSingleMu_data->Write();
  muon_muIdIso0p15_mc->Write();
  muon_muIdIso0p15_data->Write();
  muon_muIdIso0p10_mc->Write();
  muon_muIdIso0p10_data->Write();
  muon_mu12_mc->Write();
  muon_mu12_data->Write();
  muon_mu17_mc->Write();
  muon_mu17_data->Write();
Muon_2DHistos->Close();

}


