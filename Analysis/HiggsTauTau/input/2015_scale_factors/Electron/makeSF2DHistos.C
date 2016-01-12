void makeSF2DHistos(){
  TH1::AddDirectory(kFALSE);
  TFile *fElectron_Ele12_eff = TFile::Open("Electron_Ele12_eff.root");
  TGraphAsymmErrors *ele12_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele12_eff->Get("ZMassEtaLt1p48_MC"));
  TGraphAsymmErrors *ele12_highetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele12_eff->Get("ZMassEtaGt1p48_MC"));
  TGraphAsymmErrors *ele12_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele12_eff->Get("ZMassEtaLt1p48_Data"));
  TGraphAsymmErrors *ele12_highetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele12_eff->Get("ZMassEtaGt1p48_Data"));
  double* Ele12X = ele12_lowetamc->GetX();//-ele12_lowetamc->GetEXlow();
  double* Ele12EX = ele12_lowetamc->GetEXlow();
  int nbins12 = ele12_lowetamc->GetN();
  double ptbins12[nbins12+1];
  for(i=0;i<nbins12;i++){
    ptbins12[i] = Ele12X[i]-Ele12EX[i];
  } 
  ptbins12[nbins12] = 100;
  double etabins[3]= {0,1.48,2.6};
  TH2D *elec_ele12_mc = new TH2D("Electron_Ele12_MC_eff","Electron_Ele12_MC_eff",2,etabins,nbins12,ptbins12);
  TH2D *elec_ele12_data = new TH2D("Electron_Ele12_Data_eff","Electron_Ele12_Data_eff",2,etabins,nbins12,ptbins12);
  double *ele12_lowetamc_y = ele12_lowetamc->GetY();
  double *ele12_highetamc_y = ele12_highetamc->GetY();
  double *ele12_lowetadata_y = ele12_lowetadata->GetY();
  double *ele12_highetadata_y = ele12_highetadata->GetY();
  for(i=0;i<nbins12;i++){
   elec_ele12_mc->Fill(1,Ele12X[i],ele12_lowetamc_y[i]);
   elec_ele12_mc->Fill(2,Ele12X[i],ele12_highetamc_y[i]);
   elec_ele12_data->Fill(1,Ele12X[i],ele12_lowetadata_y[i]);
   elec_ele12_data->Fill(2,Ele12X[i],ele12_highetadata_y[i]);
 }

 fElectron_Ele12_eff->Close();
 
  TFile *fElectron_Ele17_eff = TFile::Open("Electron_Ele17_eff.root");
  TGraphAsymmErrors *ele17_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele17_eff->Get("ZMassEtaLt1p48_MC"));
  TGraphAsymmErrors *ele17_highetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele17_eff->Get("ZMassEtaGt1p48_MC"));
  TGraphAsymmErrors *ele17_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele17_eff->Get("ZMassEtaLt1p48_Data"));
  TGraphAsymmErrors *ele17_highetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_Ele17_eff->Get("ZMassEtaGt1p48_Data"));

  double* Ele17X = ele17_lowetamc->GetX();//-ele17_lowetamc->GetEXlow();
  double* Ele17EX = ele17_lowetamc->GetEXlow();
  int nbins17 = ele17_lowetamc->GetN();
  double ptbins17[nbins17+1];
  for(i=0;i<nbins17;i++){
    ptbins17[i] = Ele17X[i]-Ele17EX[i];
  } 
  ptbins17[nbins17] = 100;



  TH2D *elec_ele17_mc = new TH2D("Electron_Ele17_MC_eff","Electron_Ele17_MC_eff",2,etabins,nbins17,ptbins17);
  TH2D *elec_ele17_data = new TH2D("Electron_Ele17_Data_eff","Electron_Ele17_Data_eff",2,etabins,nbins17,ptbins17);
  double *ele17_lowetamc_y = ele17_lowetamc->GetY();
  double *ele17_highetamc_y = ele17_highetamc->GetY();
  double *ele17_lowetadata_y = ele17_lowetadata->GetY();
  double *ele17_highetadata_y = ele17_highetadata->GetY();
  for(i=0;i<nbins17;i++){
   elec_ele17_mc->Fill(1,Ele17X[i],ele17_lowetamc_y[i]);
   elec_ele17_mc->Fill(2,Ele17X[i],ele17_highetamc_y[i]);
   elec_ele17_data->Fill(1,Ele17X[i],ele17_lowetadata_y[i]);
   elec_ele17_data->Fill(2,Ele17X[i],ele17_highetadata_y[i]);
 }

 fElectron_Ele17_eff->Close();

  TFile *fElectron_EleIdIso0p10_eff = TFile::Open("Electron_IdIso0p10_eff.root");
  TGraphAsymmErrors *eleIdIso0p10_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p10_eff->Get("ZMassEtaLt1p48_MC"));
  TGraphAsymmErrors *eleIdIso0p10_highetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p10_eff->Get("ZMassEtaGt1p48_MC"));
  TGraphAsymmErrors *eleIdIso0p10_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p10_eff->Get("ZMassEtaLt1p48_Data"));
  TGraphAsymmErrors *eleIdIso0p10_highetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p10_eff->Get("ZMassEtaGt1p48_Data"));
  double* EleIdIso0p10X = eleIdIso0p10_lowetamc->GetX();//-eleIdIso0p10_lowetamc->GetEXlow();
  double* EleIdIso0p10EX = eleIdIso0p10_lowetamc->GetEXlow();
  int nbinsIdIso0p10 = eleIdIso0p10_lowetamc->GetN();
  double ptbinsIdIso0p10[nbinsIdIso0p10+1];
  for(i=0;i<nbinsIdIso0p10;i++){
    ptbinsIdIso0p10[i] = EleIdIso0p10X[i]-EleIdIso0p10EX[i];
  } 
  ptbinsIdIso0p10[nbinsIdIso0p10] = 100;

  TH2D *elec_eleIdIso0p10_mc = new TH2D("Electron_IdIso0p10_MC_eff","Electron_IdIso0p10_MC_eff",2,etabins,nbinsIdIso0p10,ptbinsIdIso0p10);
  TH2D *elec_eleIdIso0p10_data = new TH2D("Electron_IdIso0p10_Data_eff","Electron_IdIso0p10_Data_eff",2,etabins,nbinsIdIso0p10,ptbinsIdIso0p10);
  double *eleIdIso0p10_lowetamc_y = eleIdIso0p10_lowetamc->GetY();
  double *eleIdIso0p10_highetamc_y = eleIdIso0p10_highetamc->GetY();
  double *eleIdIso0p10_lowetadata_y = eleIdIso0p10_lowetadata->GetY();
  double *eleIdIso0p10_highetadata_y = eleIdIso0p10_highetadata->GetY();

  for(i=0;i<nbinsIdIso0p10;i++){
   elec_eleIdIso0p10_mc->Fill(1,EleIdIso0p10X[i],eleIdIso0p10_lowetamc_y[i]);
   elec_eleIdIso0p10_mc->Fill(2,EleIdIso0p10X[i],eleIdIso0p10_highetamc_y[i]);
   elec_eleIdIso0p10_data->Fill(1,EleIdIso0p10X[i],eleIdIso0p10_lowetadata_y[i]);
   elec_eleIdIso0p10_data->Fill(2,EleIdIso0p10X[i],eleIdIso0p10_highetadata_y[i]);
 }

 fElectron_EleIdIso0p10_eff->Close();


  TFile *fElectron_EleIdIso0p15_eff = TFile::Open("Electron_IdIso0p15_eff.root");
  TGraphAsymmErrors *eleIdIso0p15_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p15_eff->Get("ZMassEtaLt1p48_MC"));
  TGraphAsymmErrors *eleIdIso0p15_highetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p15_eff->Get("ZMassEtaGt1p48_MC"));
  TGraphAsymmErrors *eleIdIso0p15_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p15_eff->Get("ZMassEtaLt1p48_Data"));
  TGraphAsymmErrors *eleIdIso0p15_highetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleIdIso0p15_eff->Get("ZMassEtaGt1p48_Data"));

  double *eleIdIso0p15_lowetamc_y = eleIdIso0p15_lowetamc->GetY();
  double *eleIdIso0p15_highetamc_y = eleIdIso0p15_highetamc->GetY();
  double *eleIdIso0p15_lowetadata_y = eleIdIso0p15_lowetadata->GetY();
  double *eleIdIso0p15_highetadata_y = eleIdIso0p15_highetadata->GetY();
  double* EleIdIso0p15X = eleIdIso0p15_lowetamc->GetX();//-eleIdIso0p15_lowetamc->GetEXlow();
  double* EleIdIso0p15EX = eleIdIso0p15_lowetamc->GetEXlow();
  int nbinsIdIso0p15 = eleIdIso0p15_lowetamc->GetN();
  double ptbinsIdIso0p15[nbinsIdIso0p15+1];
  for(i=0;i<nbinsIdIso0p15;i++){
    ptbinsIdIso0p15[i] = EleIdIso0p15X[i]-EleIdIso0p15EX[i];
  } 
  ptbinsIdIso0p15[nbinsIdIso0p15] = 100;
  TH2D *elec_eleIdIso0p15_mc = new TH2D("Electron_IdIso0p15_MC_eff","Electron_IdIso0p15_MC_eff",2,etabins,nbinsIdIso0p15,ptbinsIdIso0p15);
  TH2D *elec_eleIdIso0p15_data = new TH2D("Electron_IdIso0p15_Data_eff","Electron_IdIso0p15_Data_eff",2,etabins,nbinsIdIso0p15,ptbinsIdIso0p15);


  for(i=0;i<nbinsIdIso0p15;i++){
   elec_eleIdIso0p15_mc->Fill(1,EleIdIso0p15X[i],eleIdIso0p15_lowetamc_y[i]);
   elec_eleIdIso0p15_mc->Fill(2,EleIdIso0p15X[i],eleIdIso0p15_highetamc_y[i]);
   elec_eleIdIso0p15_data->Fill(1,EleIdIso0p15X[i],eleIdIso0p15_lowetadata_y[i]);
   elec_eleIdIso0p15_data->Fill(2,EleIdIso0p15X[i],eleIdIso0p15_highetadata_y[i]);
 }

 fElectron_EleIdIso0p15_eff->Close();

  TFile *fElectron_EleSingleEle_eff = TFile::Open("Electron_SingleEle_eff.root");
  TGraphAsymmErrors *eleSingleEle_lowetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleSingleEle_eff->Get("ZMassEtaLt1p48_MC"));
  TGraphAsymmErrors *eleSingleEle_highetamc = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleSingleEle_eff->Get("ZMassEtaGt1p48_MC"));
  TGraphAsymmErrors *eleSingleEle_lowetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleSingleEle_eff->Get("ZMassEtaLt1p48_Data"));
  TGraphAsymmErrors *eleSingleEle_highetadata = dynamic_cast<TGraphAsymmErrors*>(fElectron_EleSingleEle_eff->Get("ZMassEtaGt1p48_Data"));

  double *eleSingleEle_lowetamc_y = eleSingleEle_lowetamc->GetY();
  double *eleSingleEle_highetamc_y = eleSingleEle_highetamc->GetY();
  double *eleSingleEle_lowetadata_y = eleSingleEle_lowetadata->GetY();
  double *eleSingleEle_highetadata_y = eleSingleEle_highetadata->GetY();
  double* EleSingleEleX = eleSingleEle_lowetamc->GetX();//-eleSingleEle_lowetamc->GetEXlow();
  double* EleSingleEleEX = eleSingleEle_lowetamc->GetEXlow();
  int nbinsSingleEle = eleSingleEle_lowetamc->GetN();
  double ptbinsSingleEle[nbinsSingleEle+1];
  for(i=0;i<nbinsSingleEle;i++){
    ptbinsSingleEle[i] = EleSingleEleX[i]-EleSingleEleEX[i];
  } 
  ptbinsSingleEle[nbinsSingleEle] = 100;
  TH2D *elec_eleSingleEle_mc = new TH2D("Electron_SingleEle_MC_eff","Electron_SingleEle_MC_eff",2,etabins,nbinsSingleEle,ptbinsSingleEle);
  TH2D *elec_eleSingleEle_data = new TH2D("Electron_SingleEle_Data_eff","Electron_SingleEle_Data_eff",2,etabins,nbinsSingleEle,ptbinsSingleEle);

  for(i=0;i<nbinsSingleEle;i++){
   elec_eleSingleEle_mc->Fill(1,EleSingleEleX[i],eleSingleEle_lowetamc_y[i]);
   elec_eleSingleEle_mc->Fill(2,EleSingleEleX[i],eleSingleEle_highetamc_y[i]);
   elec_eleSingleEle_data->Fill(1,EleSingleEleX[i],eleSingleEle_lowetadata_y[i]);
   elec_eleSingleEle_data->Fill(2,EleSingleEleX[i],eleSingleEle_highetadata_y[i]);
 }

 fElectron_EleSingleEle_eff->Close();


 TFile *Ele_2DHistos = new TFile("Ele_2DHistos.root","RECREATE");
  elec_eleSingleEle_mc->Write();
  elec_eleSingleEle_data->Write();
  elec_eleIdIso0p15_mc->Write();
  elec_eleIdIso0p15_data->Write();
  elec_eleIdIso0p10_mc->Write();
  elec_eleIdIso0p10_data->Write();
  elec_ele12_mc->Write();
  elec_ele12_data->Write();
  elec_ele17_mc->Write();
  elec_ele17_data->Write();
Ele_2DHistos->Close();

}


