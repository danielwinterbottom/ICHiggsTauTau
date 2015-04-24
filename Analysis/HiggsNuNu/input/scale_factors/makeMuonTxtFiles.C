#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <sstream>
#include <fstream>



int makeMuonTxtFiles(){//main

  TFile *muIdIso_ = new TFile("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.root");

  TGraphAsymmErrors *hist_muon[4][3][3];
 
  hist_muon[0][0][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_Loose_pt_abseta<0.9_2012ABCD");
  hist_muon[0][0][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_Loose_pt_abseta<0.9_2012ABCD");
  hist_muon[0][0][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_Loose_pt_abseta<0.9_2012ABCD");
  hist_muon[0][1][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_Loose_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[0][1][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_Loose_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[0][1][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_Loose_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[0][2][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_Loose_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[0][2][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_Loose_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[0][2][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_Loose_pt_abseta1.2-2.1_2012ABCD");

  hist_muon[1][0][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[1][0][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[1][0][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[1][1][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[1][1][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[1][1][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[1][2][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_Tight_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[1][2][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_Tight_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[1][2][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_Tight_pt_abseta1.2-2.1_2012ABCD");
  
  hist_muon[2][0][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_combRelIsoPF04dBeta<02_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[2][0][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_combRelIsoPF04dBeta<02_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[2][0][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_combRelIsoPF04dBeta<02_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[2][1][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_combRelIsoPF04dBeta<02_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[2][1][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_combRelIsoPF04dBeta<02_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[2][1][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_combRelIsoPF04dBeta<02_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[2][2][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_combRelIsoPF04dBeta<02_Tight_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[2][2][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_combRelIsoPF04dBeta<02_Tight_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[2][2][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_combRelIsoPF04dBeta<02_Tight_pt_abseta1.2-2.1_2012ABCD");

  hist_muon[3][0][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_combRelIsoPF04dBeta<012_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[3][0][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_combRelIsoPF04dBeta<012_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[3][0][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta<0.9_2012ABCD");
  hist_muon[3][1][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_combRelIsoPF04dBeta<012_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[3][1][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_combRelIsoPF04dBeta<012_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[3][1][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta0.9-1.2_2012ABCD");
  hist_muon[3][2][0] = (TGraphAsymmErrors*)gDirectory->Get("DATA_combRelIsoPF04dBeta<012_Tight_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[3][2][1] = (TGraphAsymmErrors*)gDirectory->Get("MC_combRelIsoPF04dBeta<012_Tight_pt_abseta1.2-2.1_2012ABCD");
  hist_muon[3][2][2] = (TGraphAsymmErrors*)gDirectory->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta1.2-2.1_2012ABCD");

  double etaMin[3] = {0,0.9,1.2};
  double etaMax[3] = {0.9,1.2,2.1};

  std::string lFileName[4] = {"mu_loose_id","mu_tight_id","mu_loose_iso","mu_tight_iso"};
  std::string lDataType[3] = {"data_eff","mc_eff","SF"};
  
  std::ostringstream lName;
  
  for (unsigned iWP(0);iWP<4;++iWP){//loop on WP
    
    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      lName.str("");
      lName << lFileName[iWP] << "_" << lDataType[iData] << ".txt";
      std::ofstream lOut(lName.str().c_str());
      
      for (unsigned iEta(0); iEta<3; ++iEta){//loop on eta bin
	
	const unsigned nBins = hist_muon[iWP][iEta][iData]->GetN();
	for (unsigned iPt(0); iPt<nBins; ++iPt){//loop on pT bins
	  double x = 0;
	  double y = 0;
	  hist_muon[iWP][iEta][iData]->GetPoint(iPt,x,y);
	  double xMax = x+hist_muon[iWP][iEta][iData]->GetErrorXhigh(iPt);
	  double xMin = x-hist_muon[iWP][iEta][iData]->GetErrorXlow(iPt);
	  double yErrPlus = hist_muon[iWP][iEta][iData]->GetErrorYhigh(iPt);
	  double yErrMinus = hist_muon[iWP][iEta][iData]->GetErrorYlow(iPt);
	  
	  lOut << xMin << " " << xMax << " " << etaMin[iEta] << " " << etaMax[iEta] << " " << y << " " << yErrMinus << " " << yErrPlus << std::endl;
	}//loop on pT bins
	
      }//loop on eta bin
      
      lOut.close();
    }//loop on data type
    
  }//loop on WP
  
  
  return 1;
}//
