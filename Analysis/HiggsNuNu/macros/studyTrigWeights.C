#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"
#include "TFile.h"
#include "TH1F.h" 
#include "TGraphErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"

int studyTrigWeights(){//main

  //TFile * fData;

  std::vector<std::string> files;
  files.push_back("MC_TTJets");
  files.push_back("MC_T-tW");
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_SingleT-s-powheg-tauola");
  files.push_back("MC_SingleTBar-s-powheg-tauola");
  files.push_back("MC_SingleT-t-powheg-tauola");
  files.push_back("MC_SingleTBar-t-powheg-tauola");
  files.push_back("MC_WW-pythia6-tauola");
  files.push_back("MC_WZ-pythia6-tauola");
  files.push_back("MC_ZZ-pythia6-tauola");
  files.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");
  files.push_back("MC_DYJetsToLL");
  files.push_back("MC_DY1JetsToLL");
  files.push_back("MC_DY2JetsToLL");
  files.push_back("MC_DY3JetsToLL");
  files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_WJetsToLNu-v1_enu");
  files.push_back("MC_WJetsToLNu-v2_enu");
  files.push_back("MC_W1JetsToLNu_enu");
  files.push_back("MC_W2JetsToLNu_enu");
  files.push_back("MC_W3JetsToLNu_enu");
  files.push_back("MC_W4JetsToLNu_enu");
  files.push_back("MC_WJetsToLNu-v1_munu");
  files.push_back("MC_WJetsToLNu-v2_munu");
  files.push_back("MC_W1JetsToLNu_munu");
  files.push_back("MC_W2JetsToLNu_munu");
  files.push_back("MC_W3JetsToLNu_munu");
  files.push_back("MC_W4JetsToLNu_munu");
  files.push_back("MC_WJetsToLNu-v1_taunu");
  files.push_back("MC_WJetsToLNu-v2_taunu");
  files.push_back("MC_W1JetsToLNu_taunu");
  files.push_back("MC_W2JetsToLNu_taunu");
  files.push_back("MC_W3JetsToLNu_taunu");
  files.push_back("MC_W4JetsToLNu_taunu");

  std::string folder[2] = {"output_noVetoSF","output"};
  std::string suffix = "";

  const unsigned nFiles = files.size();

  TFile *fBkg[nFiles];
  //TFile *output = new TFile(("BkgForQCDEstimation"+suffix+".root").c_str(),"RECREATE");

  double lLumi = 19619.;
  
  int lColor[nFiles];
  for (unsigned iN(0); iN<7;++iN){
    lColor[iN] = 5;
  }
  lColor[7] = 4;
  lColor[8] = 4;
  lColor[9] = 4;
  for (unsigned iN(10); iN<20;++iN){
    lColor[iN] = 3;
  }

  int lStyle = 1001;
  double normalisation[nFiles];
  normalisation[0] = 234./6923750.0;
  normalisation[1] = 11.1/497658.0;
  normalisation[2] = 11.1/493460.0;
  normalisation[3] = 3.79/259961.0;
  normalisation[4] = 1.76/139974.0;
  normalisation[5] = 56.4/99876.0;
  normalisation[6] = 30.7/1935072.0;
  normalisation[7] = 54.838/9970431.0;
  normalisation[8] = 33.21/10000283.0;
  normalisation[9] = 17.654/9799908.0;
  normalisation[10] = (0.70/0.101*0.2)/510501.0;
  normalisation[11] = 3503.7/30459503.0;
  normalisation[12] = 3503.7/30459503.0;
  normalisation[13] = 3503.7/30459503.0;
  normalisation[14] = 3503.7/30459503.0;
  normalisation[15] = 3503.7/30459503.0;
  normalisation[16] = 381.2/4040980.0;
  normalisation[17] = 160.3/4416646.0;
  normalisation[18] = 41.49/5055885.0;
  normalisation[19] = 5.274/1006928.0;
  for (unsigned iN(20); iN<nFiles;++iN){
    normalisation[iN] = 37509/76102995.0;
    lColor[iN] = 6;
  }

  const unsigned nVar = 4;
  TCanvas *myc[nVar];
  for (unsigned iV(0); iV<nVar;++iV){
    std::ostringstream lName;
    lName.str("");
    lName << "myc" << iV;
    myc[iV] = new TCanvas(lName.str().c_str(),"",1);
    gStyle->SetOptStat(0);
  }


  TH1F *hist[nVar][nFiles][2];

  TH1F *totHist[nVar][2];

  std::ostringstream lPath;
  std::string lHistName[nVar] = {
    "met_noMuons",
    "mjj",
    "jpt_1",
    "jpt_2"
  };

  std::string lSelection = "DPhiSIGNAL";

  for (unsigned iV(0); iV<nVar;++iV){//loop on variables

    for (unsigned iS(0); iS<2; ++iS){//loop on folders
      std::cout << " -- Processing folder " << folder[iS]  << std::endl;
      //get histograms
      bool first = true;
      for (unsigned iBkg(26); iBkg<32; ++iBkg){//loop on bkg files
	lPath.str("");
	lPath << "../" << folder[iS] << "/munu/MET130/" << files[iBkg] << ".root";
	fBkg[iBkg] = TFile::Open(lPath.str().c_str());
	//fBkg[iBkg]->cd((lSelection[iS]+"/weights/").c_str());
	fBkg[iBkg]->cd((lSelection+"/").c_str());
	hist[iV][iBkg][iS] = (TH1F*)gDirectory->Get(lHistName[iV].c_str());
	//hist[iV][iBkg][iS]->Sumw2();
	hist[iV][iBkg][iS]->Scale(lLumi*normalisation[iBkg]);
	if (first) {
	  totHist[iV][iS] = (TH1F*)hist[iV][iBkg][iS]->Clone();
	  first=false;
	}
	else totHist[iV][iS]->Add(hist[iV][iBkg][iS]);
      }//loop on bkg files
      
      double nBkg = 0;
      double nBkgErr = 0;
      unsigned binMin = 0;
      unsigned binMax = hist[iV][26][iS]->GetNbinsX();
      for (unsigned iBkg(26); iBkg<32; ++iBkg){
	double lBkgErr = 0;
	double lBkg = hist[iV][iBkg][iS]->IntegralAndError(binMin,binMax,lBkgErr);
	if (lBkg != lBkg) lBkg=0;
	if (lBkgErr != lBkgErr) lBkgErr=0;
	
	nBkg += lBkg;
	nBkgErr += lBkgErr*lBkgErr;
	//std::cout << " - " << files[iBkg] << " = " <<  lBkg << " +/- " << lBkgErr << std::endl;
      }
      
      std::cout << " -- Variable " << lHistName[iV] << " -- Total bkg = " << nBkg << " +/- " << sqrt(nBkgErr) << std::endl;
      
      myc[iV]->cd();
      totHist[iV][iS]->Rebin(20);
      totHist[iV][iS]->SetLineColor(iS+1);
      if (iS==0) totHist[iV][iS]->Draw();
      else totHist[iV][iS]->Draw("same");
      myc[iV]->Update();
      //myc->Print(("bkgQCDSasha"+suffix+"_"+lSelection[iS]+".pdf").c_str());
      
    }//loop on folders
  }//loop on variables
  
  return 0;

}//main
