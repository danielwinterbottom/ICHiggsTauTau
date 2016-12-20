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
  TFile *muLooseIso_ = TFile::Open("MuonIso_Z_RunBCD_prompt80X_7p65_bis.root");
  TFile *muRatios_ = TFile::Open("Muon_ratios_RunBCD_prompt80X_7p65.root");

  double extraIdSyst = sqrt(pow(0.01,2)+pow(0.005,2)); //On top of the "usual" systematcis for ID (1%) from the tag-and-probe method documented here, due to the known effect of HIPs on tracker efficiency it is recommended to add an additinal 0.5% systematic in quadrature.
  double extraIsoSyst = 0.005;
  double extraIsoSyst_tight = 0.01; //For what concerns isolation, the loose isolation working points are rather well modeled in term of pile-up, hence the standard (0.5%) prescription for systematcis holds, whereas it is suggested to increase that value to 1% for tight PF isolation, due to the difference between the sample used to deliver results and the ICHEP dataset.

  TH2F *hist_muon[4][3];
  TGraphAsymmErrors *hist_ratios[1];

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


  muLooseIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[2][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muLooseIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[2][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muLooseIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1");
  hist_muon[2][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");
  
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[3][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[3][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/");
  hist_muon[3][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muRatios_->cd();
  hist_ratios[0] = (TGraphAsymmErrors*)gDirectory->Get("ratio_eta");
  
  const unsigned nPts = hist_ratios[0]->GetN();
  
  double etaMin_ratios[nPts];
  double etaMax_ratios[nPts];
  double Min_ratios[nPts];
  double Max_ratios[nPts];
  
  double etaVal[nPts];
  double val[nPts];

  for (unsigned ie(0);ie<nPts;++ie){
    hist_ratios[0]->GetPoint(ie,etaVal[ie],val[ie]);
    std::cout << " -- central values " << etaVal[ie] << " " << val[ie] << std::endl;
    etaMin_ratios[ie] = etaVal[ie]-hist_ratios[0]->GetErrorXlow(ie);
    etaMax_ratios[ie] = etaVal[ie]+hist_ratios[0]->GetErrorXhigh(ie);
    std::cout << " -- eta min " << etaMin_ratios[ie] << " max " << etaMax_ratios[ie] << std::endl;
    Min_ratios[ie] = hist_ratios[0]->GetErrorYlow(ie);
    Max_ratios[ie] = hist_ratios[0]->GetErrorYhigh(ie);
    std::cout << " -- SF min " << Min_ratios[ie] << " max " << Max_ratios[ie] << std::endl;
  }
  
  std::ostringstream lName_ratios;
  lName_ratios.str("");
  lName_ratios << "Spring16_80X_mu_trackingSF.txt";
  std::ofstream lOut_ratios(lName_ratios.str().c_str());
  for (unsigned ibin(0); ibin<nPts; ++ibin){//loop on eta bin
    lOut_ratios << "10 14000 " << etaMin_ratios[ibin] << " " << etaMax_ratios[ibin] << " " << val[ibin] << " " << Min_ratios[ibin] << " " << Max_ratios[ibin] << std::endl;
  }//loop on eta bin
  

  const unsigned nEta = hist_muon[0][0]->GetXaxis()->GetNbins();
  const unsigned nEtaBis = hist_muon[2][0]->GetXaxis()->GetNbins();

  if (nEtaBis != nEta) std::cout << " WARNING! Different eta binning for Loose Iso, please check!" << std::endl;

  double etaMin[nEta];
  double etaMax[nEta];

  for (unsigned ie(0);ie<nEta;++ie){
    etaMin[ie] = hist_muon[0][0]->GetXaxis()->GetBinLowEdge(ie+1);
    etaMax[ie] = hist_muon[0][0]->GetXaxis()->GetBinLowEdge(ie+2);
    std::cout << "eta min " << etaMin[ie] << " max " << etaMax[ie] << std::endl;
  }
  for (unsigned ie(0);ie<nEtaBis;++ie){
    double etaMinBis = hist_muon[2][0]->GetXaxis()->GetBinLowEdge(ie+1);
    double etaMaxBis = hist_muon[2][0]->GetXaxis()->GetBinLowEdge(ie+2);
    std::cout << "BIS eta min " << etaMinBis << " max " << etaMaxBis << std::endl;
  }

  const unsigned nPt = hist_muon[0][0]->GetYaxis()->GetNbins();
  const unsigned nPtBis = hist_muon[2][0]->GetYaxis()->GetNbins();
  if (nPt != nPtBis) std::cout << " WARNING! Different pT binning for Loose Iso, please check!" << std::endl;

  double ptMin[nPt];
  double ptMax[nPt];

  for (unsigned ie(0);ie<nPt;++ie){
    ptMin[ie] = hist_muon[0][0]->GetYaxis()->GetBinLowEdge(ie+1);
    ptMax[ie] = hist_muon[0][0]->GetYaxis()->GetBinLowEdge(ie+2);
    std::cout << "pt min " << ptMin[ie] << " max " << ptMax[ie] << std::endl;
  }
  for (unsigned ie(0);ie<nPtBis;++ie){
    double ptMinBis = hist_muon[2][0]->GetYaxis()->GetBinLowEdge(ie+1);
    double ptMaxBis = hist_muon[2][0]->GetYaxis()->GetBinLowEdge(ie+2);
    std::cout << "BIS pt min " << ptMinBis << " max " << ptMaxBis << std::endl;
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
      
      //do negative eta
      for (unsigned iEta(nEta); iEta>0; --iEta){//loop on eta bin
	for (unsigned iPt(0); iPt<nPt; ++iPt){//loop on pT bins
	  double val = hist_muon[iWP][iData]->GetBinContent(iEta,iPt+1);
	  double err = hist_muon[iWP][iData]->GetBinError(iEta,iPt+1);
	  if (iWP==2 && iPt==nPt-1){
	    val = hist_muon[iWP][iData]->GetBinContent(iEta,iPt);
	    err = hist_muon[iWP][iData]->GetBinError(iEta,iPt);
	  }
          err = sqrt(pow(err,2)+pow(lSystematic[iWP],2));
	  std::ostringstream lstr;
	  lstr << ptMin[iPt] << " " << ptMax[iPt] << " " << -etaMax[iEta-1] << " " << -etaMin[iEta-1] << " " << val << " " << err << " " << err << std::endl;
	  lOut << lstr.str();
	  if (iWP==2) std::cout << lDataType[iData] << " " << lstr.str();
	}//loop on pT bins
	
      }//loop on eta bin
      
      //do positive eta
      for (unsigned iEta(0); iEta<nEta; ++iEta){//loop on eta bin
	for (unsigned iPt(0); iPt<nPt; ++iPt){//loop on pT bins
	  double val = hist_muon[iWP][iData]->GetBinContent(iEta+1,iPt+1);
	  double err = hist_muon[iWP][iData]->GetBinError(iEta+1,iPt+1);
	  if (iWP==2 && iPt==nPt-1){
	    val = hist_muon[iWP][iData]->GetBinContent(iEta+1,iPt);
	    err = hist_muon[iWP][iData]->GetBinError(iEta+1,iPt);
	  }
          err = sqrt(pow(err,2)+pow(lSystematic[iWP],2));
	  lOut << ptMin[iPt] << " " << ptMax[iPt] << " " << etaMin[iEta] << " " << etaMax[iEta] << " " << val << " " << err << " " << err << std::endl;
	}//loop on pT bins
	
      }//loop on eta bin
      
      lOut.close();
    }//loop on data type
    
  }//loop on WP
  
  muId_->Close(); 
  muLooseIso_->Close();
  muIso_->Close();
  muRatios_->Close();
  return 1;

}//
