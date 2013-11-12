#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <iomanip>



void findElectronPtEtaBin(const double pt, const double eta, unsigned & ptBin, unsigned & etaBin){
  
  if (fabs(eta) < 0.8) etaBin=0;
  else if (fabs(eta) < 1.442) etaBin=1;
  else if (fabs(eta) < 1.556) etaBin=2;
  else if (fabs(eta) < 2.0) etaBin=3;
  else etaBin=4;
  if (pt<15) ptBin=0;
  else if (pt<20) ptBin=1;
  else if (pt<30) ptBin=2;
  else if (pt<40) ptBin=3;
  else if (pt<50) ptBin=4;
  else ptBin=5;
    
}

int makeEleSyst(){//main

  std::ifstream lInput;
  //lInput.open("ele_tight_id.txt");
  lInput.open("ele_veto_id_data_eff.txt");
  if(!lInput.is_open()){
    std::cerr << "Unable to open file. " << std::endl;
    return 1;
  }

  std::ofstream lOutput;
  lOutput.open("ele_veto_id_data_eff_with_syst.txt");
  
  double syst[5][6];
  for (unsigned iEta(0); iEta<5;++iEta){//loop on eta bins
    syst[iEta][0] = 0.043;
    syst[iEta][1] = 0.042;
    syst[iEta][2] = 0.014;
    syst[iEta][3] = 0.0043;
    syst[iEta][4] = 0.0028;
    syst[iEta][5] = 0.0045;
  }//loop on eta bins
  //correct gap bin:
  syst[2][1] = 0.046;
  syst[2][2] = 0.027;
  syst[2][3] = 0.015;
  syst[2][5] = 0.0051;


  while(1){
    double pTmin = 0;
    double pTmax = 0;
    double etaMin = 0;
    double etaMax = 0;
    double SF = 0;
    double SFerrPlus = 0;
    double SFerrMinus = 0;
    lInput>>pTmin>>pTmax>>etaMin>>etaMax>>SF>>SFerrMinus>>SFerrPlus;
    //protect against blank line at the end of the file
    

    unsigned etaBin=0;
    unsigned pTbin=0;
    findElectronPtEtaBin((pTmin+pTmax)/2.,(etaMin+etaMax)/2.,pTbin,etaBin);
    SFerrPlus = sqrt(pow(SFerrPlus,2)+pow(syst[etaBin][pTbin]*SF,2));
    SFerrMinus = sqrt(pow(SFerrMinus,2)+pow(syst[etaBin][pTbin]*SF,2));


    if (pTmin > 1) lOutput //<< std::setprecision(2)
      << static_cast<unsigned>(pTmin*1000+0.5)/1000. << " " 
			   << static_cast<unsigned>(pTmax*1000+0.5)/1000.  << " " 
			   << static_cast<unsigned>(etaMin*1000+0.5)/1000.  << " " 
			   << static_cast<unsigned>(etaMax*1000+0.5)/1000.  << " " 
			   << static_cast<unsigned>(SF*1000+0.5)/1000.  << " " 
			   << static_cast<unsigned>(SFerrMinus*1000+0.5)/1000.  << " " 
			   << static_cast<unsigned>(SFerrPlus*1000+0.5)/1000.
			   << std::endl;


    if(lInput.eof()){
      break; 
    }
  }

  lInput.close();
  lOutput.close();

  return 0;


}
