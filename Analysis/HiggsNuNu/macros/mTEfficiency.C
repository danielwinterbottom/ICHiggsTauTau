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
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"


int mTEfficiency(){//main

  TFile * fWJets[2];
  TFile * fData[2];

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

  const unsigned nFiles = files.size();

  TFile *fBkg[nFiles];

  double lLumi = 19500.;
  
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
  normalisation[10] = 0.45/510501.0;
  normalisation[11] = 3503.7/30459503.0;
  normalisation[12] = 3503.7/30459503.0;
  normalisation[13] = 3503.7/30459503.0;
  normalisation[14] = 3503.7/30459503.0;
  normalisation[15] = 3503.7/30459503.0;
  normalisation[16] = 381.2/4040980.0;
  normalisation[17] = 160.3/4416646.0;
  normalisation[18] = 41.49/5055885.0;
  normalisation[19] = 5.274/1006928.0;

  TCanvas *myc = new TCanvas("myc","",1);
  gStyle->SetOptStat(0);


  TH1F *mT[nFiles+2][2];
  TH1F *nCdataOverMC[2];

  std::ostringstream lPath,lHistName;
  std::string lFlavour[2] = {"enu","munu"};
  


  for (unsigned iF(0); iF<2; ++iF){//loop on flavour

    lHistName.str("");
    lHistName << "mt_" << lFlavour[iF] ;

    lPath.str("");
    lPath << "../output/" << lFlavour[iF] << "/MET130/MC_WJetsToLNu_" << lFlavour[iF] << ".root";
    fWJets[iF] = TFile::Open(lPath.str().c_str());

    lPath.str("");
    lPath << "../output/" << lFlavour[iF] << "/MET130/Data.root";
    fData[iF] = TFile::Open(lPath.str().c_str());

    //lPath.str("");
    //lPath << "output/" << lFlavour[iF] << "/MET130/MC_ZandVV.root";
    //fZandVV[iF] = TFile::Open(lPath.str().c_str());


    fWJets[iF]->cd("DPhiSIGNAL");
    mT[0][iF] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
    mT[0][iF]->Sumw2();
    mT[0][iF]->Scale(lLumi*37509/76102995.0);

    fData[iF]->cd("DPhiSIGNAL");
    mT[1][iF] = (TH1F*)gDirectory->Get(lHistName.str().c_str()); 
    mT[1][iF]->Sumw2();


    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      lPath.str("");
      lPath << "../output/" << lFlavour[iF] << "/MET130/" << files[iBkg] << ".root";
      fBkg[iBkg] = TFile::Open(lPath.str().c_str());
      fBkg[iBkg]->cd("DPhiSIGNAL");
      mT[2+iBkg][iF] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      mT[2+iBkg][iF]->Sumw2();
      mT[2+iBkg][iF]->Scale(lLumi*normalisation[iBkg]);
    }//loop on bkg files

    std::ostringstream lName;
    lName.str("");
    lName << "nCDataOverMC_" << lFlavour[iF];
    nCdataOverMC[iF] = new TH1F(lName.str().c_str(),";min mT cut (GeV);n^{C}_{W-data}/n^{C}_{W-MC}",8,0,80);
    nCdataOverMC[iF]->Sumw2();

    unsigned binMin = 0;
    unsigned binMax = mT[0][iF]->GetNbinsX() + 1;
    for (unsigned iB(0); iB<8; iB++){

      if (iB==7){
	for (unsigned iX(0); iX<mT[0][iF]->GetNbinsX(); ++iX){
	  if (mT[0][iF]->GetBinLowEdge(iX+1) >= 120) {
	    binMax = iX;
	    break;
	  }
	}
      }
      else if (iB==0) {
	binMin = 0;
      }
      else {
	for (unsigned iX(0); iX<mT[0][iF]->GetNbinsX(); ++iX){
	  if (mT[0][iF]->GetBinLowEdge(iX+1) >= 10*iB) {
	    binMin = iX+1;
	    break;
	  }
	}
      }
      std::cout << " Bin " << iB 
		<< " min = " <<  mT[0][iF]->GetBinLowEdge(binMin)
		<< " max = " <<  mT[0][iF]->GetXaxis()->GetBinUpEdge(binMax)
		<< std::endl;

      double nCdataErr = 0;
      double nCdata = mT[1][iF]->IntegralAndError(binMin,binMax,nCdataErr);
      std::cout << " -- data = " << nCdata << " +/- " << nCdataErr << std::endl;

      double nBkgErr = 0;
      double nBkg = 0;
      for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
	double lErr = 0;
	nBkg += mT[2+iBkg][iF]->IntegralAndError(binMin,binMax,lErr);
	nBkgErr += lErr*lErr;
      }
      nCdata -= nBkg;
      nCdataErr = sqrt(pow(nCdataErr,2)+nBkgErr);

      std::cout << " -- bkg = " << nBkg << " +/- " << sqrt(nBkgErr) << std::endl;
      std::cout << " -- data-bkg = " << nCdata << " +/- " << nCdataErr << std::endl;


      double nCMCErr = 0;
      double nCMC = mT[0][iF]->IntegralAndError(binMin,binMax,nCMCErr);
      
      std::cout << " -- MC W = " << nCMC << " +/- " << nCMCErr << std::endl;


      nCdataOverMC[iF]->SetBinContent(iB+1,nCdata/nCMC);
      double lErr = nCdata/nCMC*sqrt(pow(nCdataErr/nCdata,2)+pow(nCMCErr/nCMC,2));
      nCdataOverMC[iF]->SetBinError(iB+1,lErr);
    }
    myc->cd();
    nCdataOverMC[iF]->Draw("PE");
    myc->Print( (lName.str()+".pdf").c_str());
    
  }

  return 0;

}//main
