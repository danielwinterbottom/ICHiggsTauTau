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
  eleId[0] = new TFile("CutBasedID_VetoWP_80X_SF2D.root");
  eleId[1] = new TFile("CutBasedID_TightWP_80X_SF2D.root");
  eleId[2] = new TFile("CutBasedID_GsfID_80X_SF2D.root");

  double extraIdSyst = 0;//for pT<20 GeV due to gsfID
  double extraGsfSyst = 0.03;//0.005;

  TH2F *hist_elec[nP][3];
  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP
    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      hist_elec[iWP][iData]=0;
    }
  }


  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP
    eleId[iWP]->cd();
    hist_elec[iWP][2] = (TH2F*)gDirectory->Get("EGamma_SF2D");
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
    
    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      if (!hist_elec[iWP][iData]) continue;
      lName.str("");
      lName << prefix << lFileName[iWP] << "_" << lDataType[iData] << ".txt";
      std::ofstream lOut(lName.str().c_str());
      for (unsigned iEta(0); iEta<nEta; ++iEta){//loop on eta bin
	for (unsigned iPt(0); iPt<nPt; ++iPt){//loop on pT bins
	  double val = hist_elec[iWP][iData]->GetBinContent(iEta+1,iPt+1);
	  double err = hist_elec[iWP][iData]->GetBinError(iEta+1,iPt+1);
	  if (iWP<2) err = sqrt(pow(err,2)+pow(extraIdSyst,2));
	  else if (ptMin[iPt]<20) err = sqrt(pow(err,2)+pow(extraGsfSyst,2));
	  lOut << ptMin[iPt] << " " << ptMax[iPt] << " " << etaMin[iEta] << " " << etaMax[iEta] << " " << val << " " << err << " " << err << std::endl;
	}//loop on pT bins
	
      }//loop on eta bin
      
      lOut.close();
    }//loop on data type
    
  }//loop on WP
  
  
  return 0;

}//
