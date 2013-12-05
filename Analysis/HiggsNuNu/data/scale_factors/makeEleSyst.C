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

  bool doTight = false;

  std::ifstream lInput;
  if (doTight) lInput.open("ele_tight_id.txt");
  else lInput.open("ele_veto_id_data_eff.txt");
  if(!lInput.is_open()){
    std::cerr << "Unable to open file. " << std::endl;
    return 1;
  }

  std::ofstream lOutput;
  if (!doTight) lOutput.open("ele_veto_id_data_eff_with_syst.txt");
  else lOutput.open("ele_tight_id_with_syst.txt");
 
  double syst[5][6];
  if (doTight) {
    //eta 0-0.8
    syst[0][0] = 0.042;
    syst[0][1] = 0.044;
    syst[0][2] = 0.002;
    syst[0][3] = 0.002;
    syst[0][4] = 0.001;
    syst[0][5] = 0.001;
    //eta 0.8-1.442
    syst[1][0] = 0.024;
    syst[1][1] = 0.008;
    syst[1][2] = 0.005;
    syst[1][3] = 0.005;
    syst[1][4] = 0.001;
    syst[1][5] = 0.004;
    //eta 1.442-1.556
    syst[2][0] = 0.203;
    syst[2][1] = 0.049;
    syst[2][2] = 0.003;
    syst[2][3] = 0.002;
    syst[2][4] = 0.001;
    syst[2][5] = 0.004;
    //eta 1.556-2.0
    syst[3][0] = 0.031;
    syst[3][1] = 0.019;
    syst[3][2] = 0.008;
    syst[3][3] = 0.002;
    syst[3][4] = 0.001;
    syst[3][5] = 0.001;
    //eta 2.0-2.5
    syst[4][0] = 0.027;
    syst[4][1] = 0.037;
    syst[4][2] = 0.006;
    syst[4][3] = 0.001;
    syst[4][4] = 0.002;
    syst[4][5] = 0.001;
  }
  else {
    //this is the systematics on the ratio data/MC, 
    //applied as absolute value on eff(data)
    //eta 0-0.8
    syst[0][0] = 0.033;
    syst[0][1] = 0.024;
    syst[0][2] = 0.003;
    syst[0][3] = 0.002;
    syst[0][4] = 0.001;
    syst[0][5] = 0.001;
    //eta 0.8-1.442
    syst[1][0] = 0.062;
    syst[1][1] = 0.023;
    syst[1][2] = 0.007;
    syst[1][3] = 0.001;
    syst[1][4] = 0.004;
    syst[1][5] = 0.001;
    //eta 1.442-1.556
    syst[2][0] = 0.131;
    syst[2][1] = 0.079;
    syst[2][2] = 0.011;
    syst[2][3] = 0.002;
    syst[2][4] = 0.004;
    syst[2][5] = 0.003;
    //eta 1.556-2.0
    syst[3][0] = 0.052;
    syst[3][1] = 0.036;
    syst[3][2] = 0.003;
    syst[3][3] = 0.001;
    syst[3][4] = 0.001;
    syst[3][5] = 0.001;
    //eta 2.0-2.5
    syst[4][0] = 0.051;
    syst[4][1] = 0.020;
    syst[4][2] = 0.003;
    syst[4][3] = 0.001;
    syst[4][4] = 0.001;
    syst[4][5] = 0.001;
  }

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
    
    std::cout << pTmin << "-" << pTmax << " "
	      << etaMin << "-" << etaMax << " "
	      << SF << " "
	      << SFerrPlus << " (" 
	      << SFerrPlus/SF*100 << "%) " ;

    unsigned etaBin=0;
    unsigned pTbin=0;
    findElectronPtEtaBin((pTmin+pTmax)/2.,(etaMin+etaMax)/2.,pTbin,etaBin);

    //std::cout << "[" << syst[etaBin][pTbin] << " (" 
    //<< syst[etaBin][pTbin]/SF*100 << "%)] " ;
    //SF=a/b, dSF/SF=da/a ++ db/b
    //SFveto=(1-a)/(1-b), dSFV/SFV=da/(1-a) ++ db(1-b)
    //dSFV/SFV=da/a*a/(1-a) ++ db/b*b/(1-b)
    //using the approximation a/1-a ~ b/1-b
    //dSFV/SFV=a/(1-a) * dSF/SF
    //so dSFV=b/(1-b)*dSF
    //transferring everything to the numerator 
    //d(1-a) = da = dSF*a/(1-a)
    if (!doTight && SF != 1) syst[etaBin][pTbin] = syst[etaBin][pTbin]*SF/(1-SF);

    std::cout << syst[etaBin][pTbin] << " (" 
	      << syst[etaBin][pTbin]/SF*100 << "%) " ;

    SFerrPlus = sqrt(pow(SFerrPlus,2)+pow(syst[etaBin][pTbin],2));
    SFerrMinus = sqrt(pow(SFerrMinus,2)+pow(syst[etaBin][pTbin],2));

    std::cout << SFerrPlus << " (" 
	      << SFerrPlus/SF*100 << "%)"  
	      << std::endl ;


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
