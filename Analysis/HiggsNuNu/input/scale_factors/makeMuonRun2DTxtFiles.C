#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"

#include <iostream>
#include <sstream>
#include <fstream>



int makeMuonRun2DTxtFiles(){//main

  //TFile *muId_ = new TFile("MuonID_Z_RunCD_Reco76X_Feb15.root");
  //TFile *muIso_ = new TFile("MuonIso_Z_RunCD_Reco76X_Feb15.root");
  TFile *muId_ = TFile::Open("MuonID_Z_RunBCD_prompt80X_7p65.root");
  TFile *muIso_ = TFile::Open("MuonIso_Z_RunBCD_prompt80X_7p65.root");
  TFile *muRatios_ = TFile::Open("Muon_ratios_RunBCD_prompt80X_7p65.root");

  double extraIdSyst = sqrt(pow(0.01,2)+pow(0.005,2)); //On top of the "usual" systematcis for ID (1%) from the tag-and-probe method documented here, due to the known effect of HIPs on tracker efficiency it is recommended to add an additinal 0.5% systematic in quadrature.
  double extraIsoSyst = 0.005;
  double extraIsoSyst_tight = 0.01; //For what concerns isolation, the loose isolation working points are rather well modeled in term of pile-up, hence the standard (0.5%) prescription for systematcis holds, whereas it is suggested to increase that value to 1% for tight PF isolation, due to the difference between the sample used to deliver results and the ICHEP dataset.

  TH2F *hist_muon[4][3];
  TH2F *hist_ratios[1];

  muId_->cd("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[0][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muId_->cd("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[0][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muId_->cd("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/");
  hist_muon[0][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muId_->cd("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[1][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muId_->cd("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[1][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muId_->cd("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/");
  hist_muon[1][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");


//   muIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/efficienciesDATA/");
//   hist_muon[2][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
//   muIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/efficienciesMC/");
//   hist_muon[2][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
//   muIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1");
//   hist_muon[2][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muIso_->cd("MC_NUM_LooseRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[2][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muIso_->cd("MC_NUM_LooseRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[2][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muIso_->cd("MC_NUM_LooseRelIso_DEN_TightID_PAR_pt_spliteta_bin1");
  hist_muon[2][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[3][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[3][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/");
  hist_muon[3][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muRatios_->cd();
  hist_ratios[0] = (TH2F*)gDirectory->Get("ratio_eta");
  
  const unsigned nEtaRatios = hist_ratios[0]->GetXaxis()->GetNbins();
  
  double etaMin_ratios[nEtaRatios];
  double etaMax_ratios[nEtaRatios];
  
  for (unsigned ie(0);ie<nEtaRatios;++ie){
    etaMin_ratios[ie] = hist_ratios[0]->GetXaxis()->GetBinLowEdge(ie+1);
    etaMax_ratios[ie] = hist_ratios[0]->GetXaxis()->GetBinLowEdge(ie+2);
    std::cout << " -- For ratios: eta min " << etaMin_ratios[ie] << " max " << etaMax_ratios[ie] << std::endl;
  }
  
  const unsigned nRatios = hist_ratios[0]->GetYaxis()->GetNbins();
  
  double Min_ratios[nRatios];
  double Max_ratios[nRatios];
  
  for (unsigned ie(0);ie<nRatios;++ie){
    Min_ratios[ie] = hist_ratios[0]->GetYaxis()->GetBinLowEdge(ie+1);
    Max_ratios[ie] = hist_ratios[0]->GetYaxis()->GetBinLowEdge(ie+2);
    std::cout << " -- For ratios: min " << Min_ratios[ie] << " max " << Max_ratios[ie] << std::endl;
  }

  std::ostringstream lName_ratios;
  lName_ratios.str("");
  lName_ratios << "Spring16_80X_" << "mu_Ratios" << ".txt";
  std::ofstream lOut_ratios(lName_ratios.str().c_str());
  for (unsigned iEta_ratios(0); iEta_ratios<nEtaRatios; ++iEta_ratios){//loop on eta bin
    for (unsigned i_ratios(0); i_ratios<nRatios; ++i_ratios){//loop on pT bins
      double val_ratios = hist_ratios[0]->GetBinContent(iEta_ratios+1,i_ratios+1);
      double err_ratios = hist_ratios[0]->GetBinError(iEta_ratios+1,i_ratios+1);
      lOut_ratios << Min_ratios[i_ratios] << " " << Max_ratios[i_ratios] << " " << etaMin_ratios[iEta_ratios] << " " << etaMax_ratios[iEta_ratios] << " " << val_ratios << " " << err_ratios << std::endl;
    }//loop on pT bins
    
  }//loop on eta bin
  

  const unsigned nEta = hist_muon[0][0]->GetXaxis()->GetNbins();

  double etaMin[nEta];
  double etaMax[nEta];

  for (unsigned ie(0);ie<nEta;++ie){
    etaMin[ie] = hist_muon[0][0]->GetXaxis()->GetBinLowEdge(ie+1);
    etaMax[ie] = hist_muon[0][0]->GetXaxis()->GetBinLowEdge(ie+2);
    std::cout << "eta min " << etaMin[ie] << " max " << etaMax[ie] << std::endl;
  }

  const unsigned nPt = hist_muon[0][0]->GetYaxis()->GetNbins();

  double ptMin[nPt];
  double ptMax[nPt];

  for (unsigned ie(0);ie<nPt;++ie){
    ptMin[ie] = hist_muon[0][0]->GetYaxis()->GetBinLowEdge(ie+1);
    ptMax[ie] = hist_muon[0][0]->GetYaxis()->GetBinLowEdge(ie+2);
    std::cout << "pt min " << ptMin[ie] << " max " << ptMax[ie] << std::endl;
  }

  const unsigned nP = 4;
  //std::string prefix = "Fall15_76X_";
  std::string prefix = "Spring16_80X_";
  std::string lFileName[nP] = {"mu_loose_id","mu_tight_id","mu_loose_iso","mu_tight_iso"};
  double lSystematic[nP]    = {extraIdSyst,extraIdSyst,extraIsoSyst,extraIsoSyst_tight};
  std::string lDataType[3] = {"data_eff","mc_eff","SF"};
  

  std::ostringstream lName;

  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP
    
    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      lName.str("");
      lName << prefix << lFileName[iWP] << "_" << lDataType[iData] << ".txt";
      std::ofstream lOut(lName.str().c_str());
      
      for (unsigned iEta(0); iEta<nEta; ++iEta){//loop on eta bin
	for (unsigned iPt(0); iPt<nPt; ++iPt){//loop on pT bins
	  double val = hist_muon[iWP][iData]->GetBinContent(iEta+1,iPt+1);
	  double err = hist_muon[iWP][iData]->GetBinError(iEta+1,iPt+1);
          err = sqrt(pow(err,2)+pow(lSystematic[iWP],2));
	  lOut << ptMin[iPt] << " " << ptMax[iPt] << " " << etaMin[iEta] << " " << etaMax[iEta] << " " << val << " " << err << " " << err << std::endl;
	}//loop on pT bins
	
      }//loop on eta bin
      
      lOut.close();
    }//loop on data type
    
  }//loop on WP
  
  muId_->Close(); 
  muIso_->Close();
  muRatios_->Close();
  return 1;

}//
