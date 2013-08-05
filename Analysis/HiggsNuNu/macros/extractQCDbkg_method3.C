#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"

using std::showpos;
using std::noshowpos;

//enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL,DPhiQCD};
//enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJV,DPhiSIGNAL,DPhiQCD};
enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJVfail,DPhiSIGNAL_CJVfail,DPhiQCD_CJVfail,CJVpass,DPhiSIGNAL_CJVpass,DPhiQCD_CJVpass};
enum Sample {QCD,GJets,Top,TTbar,SingleTop,TW,WJets,WJets_enu,WJets_munu,WJets_taunu,ZJets,ZJets_ll,ZJets_nunu,ZJets_vbf,VV,Data,Signal};


struct events {
  std::string sample;
  double number;
  double error;

  void operator+=(const events & rhs){
    this->number += rhs.number;
    this->error = sqrt(pow(this->error,2)+pow(rhs.error,2));
  }

  void operator-=(const events & rhs){
    this->number -= rhs.number;
    this->error = sqrt(pow(this->error,2)+pow(rhs.error,2));
  }

};

double sumSqErr(events evt1, events evt2){
  if (evt1.number==0 || evt2.number==0) return 0;
  return sqrt(pow(evt1.error/evt1.number,2)+pow(evt2.error/evt2.number,2));
}

struct efficiency{
  std::string name;
  events num;
  events den;

  double eff() const{
    if (den.number==0) return 0;
    return num.number/den.number;
  }

  double error() const{
    if (den.number==0) return 0;
    return 1/den.number*sqrt(num.number*(1-eff()));
  }

};

std::ostream & operator<<(std::ostream & aOs, const events & evt){
  aOs //<< "$" << evt.sample << " $ & "
      << "$" <<  evt.number
      << " \\pm " << evt.error
      << "$";// \\\\"
  //<< std::endl;
  return aOs;
}

std::ostream & operator<<(std::ostream & aOs, const efficiency & eff){
  aOs //<< "$\\" << eff.name << " $ & " 
      << "$" <<  eff.eff()
      << " \\pm " << eff.error()
      << "$ ";//\\\\"
  //<< std::endl;
  return aOs;
}

std::istream & operator>>(std::istream & is, events & evt){
  is>>evt.sample; 
  is>>evt.number;
  is>>evt.error; 
  return is; 
}

typedef std::vector < events > evtsArray;

std::istream & operator>>(std::istream & is, evtsArray & evtVec){

  events temp; 
  while(1){
    is>>temp; 
    if(is.eof()){
      break; 
    }
    evtVec.push_back(temp); 

  }
  return is; 
}

int extractWJetsBkg(){//main

  std::vector<std::string> lSelVecSignal;
  lSelVecSignal.push_back("HLTMetClean");
  lSelVecSignal.push_back("LeptonVeto");
  lSelVecSignal.push_back("JetPair");
  lSelVecSignal.push_back("DEta");
  lSelVecSignal.push_back("MET");
  lSelVecSignal.push_back("TightMjj");
  lSelVecSignal.push_back("DPhiSIGNAL_noCJV");
  lSelVecSignal.push_back("DPhiQCD_noCJV");
  lSelVecSignal.push_back("CJVfail");
  lSelVecSignal.push_back("DPhiSIGNAL_fail");
  lSelVecSignal.push_back("DPhiQCD_fail");
  lSelVecSignal.push_back("CJVpass");
  lSelVecSignal.push_back("DPhiSIGNAL_pass");
  lSelVecSignal.push_back("DPhiQCD_pass");

  //std::string lSuffix = "";
  const unsigned nWeights = 6;
  std::string lSuffix[nWeights] = {"","_pu","_puUp","_puDown","_pu_trig","_pu_trig_idiso"};

  bool doWeights = false;

  //std::string TOPDIR = "../TABLES_mjj1200/";
  std::string TOPDIR = "../TABLES/";

  const unsigned nSteps = lSelVecSignal.size();

  for (unsigned iW(0); iW<(doWeights?nWeights:1); ++iW){//loop on different weights

    std::cout << " -- Processing weight " << lSuffix[iW] << std::endl;

    std::ostringstream lFolder;
    lFolder << "MET0/";
      
    evtsArray lSel[nSteps];
    
    for (unsigned iS(0); iS<nSteps; ++iS){//loop on steps
	  
      std::ifstream lTable;
      std::ostringstream lName;
      
      
      lName << TOPDIR << "/nunu/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << lSuffix[iW] << ".dat";
      lTable.open(lName.str().c_str());
      if(!lTable.is_open()){
	cerr<<"Unable to open file: "<<lName.str()<<endl;
	return 1; 
      }
      lTable>>lSel[iS];
      lTable.close();
    }//loop on steps
	
	
    const unsigned nSel = 4;
    std::string lSelection[nSel] = {"DPhiQCD_CJVfail","DPhiQCD_CJVpass","DPhiSIGNAL_CJVfail","DPhiSIGNAL_CJVpass"};
    std::string lSelectionName[nSel] = {"#Delta#phi_{jj}>2.6, fail CJV","#Delta#phi_{jj}>2.6, pass CJV","#Delta#phi_{jj}<1.0, fail CJV","#Delta#phi_{jj}<1.0, pass CJV"};
  
    Selection lEnumName[nSel] = {};

    for (unsigned int iS(0); iS<nSel; ++iS){//selection

      std::cout << lSelectionName[iS] << std::endl;

      std::cout << " Data : " << std::endl;
      std::cout << " A = " << lSel[]


    }//selection


  }//loop on weights

  return 0;
}//main
  
