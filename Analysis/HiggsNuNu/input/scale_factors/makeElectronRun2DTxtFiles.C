#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"

#include <iostream>
#include <sstream>
#include <fstream>



int makeElectronRun2DTxtFiles(){//main

  const unsigned nP = 3;
  std::string prefix = "Spring16_80X_";
  std::string lFileName[nP] = {"ele_veto_id","ele_tight_id","gsf_id"};//,"ele_loose_iso","ele_tight_iso"};
  std::string lDataType[3] = {"data_eff","mc_eff","SF"};

  TFile *eleId[nP];
  eleId[0] = TFile::Open("CutBasedID_VetoWP_80X_SF2D.root");
  eleId[1] = TFile::Open("CutBasedID_TightWP_80X_SF2D.root");
  eleId[2] = TFile::Open("CutBasedID_GsfID_80X_SF2D.root");

  TFile *eleEff[nP];
  eleEff[0] = TFile::Open("CutBasedID_VetoWP_80X_eff.root");
  //no need for tight one....
  eleEff[2] = TFile::Open("GsfID_80X_eff.root");

  double extraIdSyst = 0;
  double extraGsfSyst = 0.03;//for pT<20 GeV due to gsfID

  TH2F *hist_elec[nP][3];
  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP
    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      hist_elec[iWP][iData]=0;
    }
  }


  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP
    eleId[iWP]->cd();
    hist_elec[iWP][2] = (TH2F*)gDirectory->Get("EGamma_SF2D");
    if (iWP!=1){
      eleEff[iWP]->cd();
      hist_elec[iWP][0] = (TH2F*)gDirectory->Get("EGamma_EffData2D");
      hist_elec[iWP][1] = (TH2F*)gDirectory->Get("EGamma_EffMC2D");
    }
  }

  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP

    const unsigned nEta = hist_elec[iWP][2]->GetXaxis()->GetNbins();
    double etaMin[nEta];
    double etaMax[nEta];
    
    for (unsigned ie(0);ie<nEta;++ie){
      etaMin[ie] = hist_elec[iWP][2]->GetXaxis()->GetBinLowEdge(ie+1);
      etaMax[ie] = hist_elec[iWP][2]->GetXaxis()->GetBinLowEdge(ie+2);
      std::cout << "eta min " << etaMin[ie] << " max " << etaMax[ie] << std::endl;
    }

    const unsigned nPt = hist_elec[iWP][2]->GetYaxis()->GetNbins();
    
    double ptMin[nPt];
    double ptMax[nPt];
    
    for (unsigned ie(0);ie<nPt;++ie){
      ptMin[ie] = hist_elec[iWP][2]->GetYaxis()->GetBinLowEdge(ie+1);
      ptMax[ie] = hist_elec[iWP][2]->GetYaxis()->GetBinLowEdge(ie+2);
      std::cout << "pt min " << ptMin[ie] << " max " << ptMax[ie] << std::endl;
    }
    
    
    std::ostringstream lName;
    
    double valcheck[3][nEta*nPt];

    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      if (!hist_elec[iWP][iData]) continue;
      lName.str("");
      lName << prefix << lFileName[iWP] << "_" << lDataType[iData] << ".txt";
      std::ofstream lOut(lName.str().c_str());
      for (unsigned iEta(0); iEta<nEta; ++iEta){//loop on eta bin
	for (unsigned iPt(0); iPt<nPt; ++iPt){//loop on pT bins
	  double val = hist_elec[iWP][iData]->GetBinContent(iEta+1,iPt+1);
	  double err = hist_elec[iWP][iData]->GetBinError(iEta+1,iPt+1);
	  if (iData!=1){
	    //apply extra systs only to SF or data
	    if (iWP<2) err = sqrt(pow(err,2)+pow(extraIdSyst,2));
	    else if (ptMin[iPt]<20) err = sqrt(pow(err,2)+pow(extraGsfSyst,2));
	  }
	  lOut << ptMin[iPt] << " " << ptMax[iPt] << " " << etaMin[iEta] << " " << etaMax[iEta] << " " << val << " " << err << " " << err << std::endl;
	  unsigned iBin = nPt*iEta+iPt;
	  if (iWP!=1) valcheck[iData][iBin] = val;
	}//loop on pT bins
	
      }//loop on eta bin
      
      lOut.close();
    }//loop on data type

    if (iWP!=1){
      std::cout << " Check values for WP " << iWP << std::endl;
      for (unsigned iBin(0); iBin<nEta*nPt; ++iBin){
	std::cout << " bin " << iBin << " SF " << valcheck[2][iBin] << " data/MC " << valcheck[0][iBin]/valcheck[1][iBin] << std::endl;
      }
    }

  }//loop on WP
  
  
  return 0;

}//
