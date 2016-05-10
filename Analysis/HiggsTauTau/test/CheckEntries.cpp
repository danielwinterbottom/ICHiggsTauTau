// ROOT includes
#include "TChain.h"
//#include "TMath.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TBranch.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

int main(){
  
  //Create the first tree. 
  TString filename1;
  TChain *chIn1 = new TChain("icEventProducer/EventTree");
  std::ifstream infile1;
  infile1.open("L1ZJets.dat");
  //int count=0;
  while (infile1 >> filename1 ){
      std::cout << "Adding File: " << filename1 << std::endl;
      chIn1->Add(filename1);
      //count++;
  }
  
  infile1.close();
  
  int nentries1 = chIn1->GetEntries();
  
  ic::EventInfo *eventInfo1 = new ic::EventInfo();
  chIn1->SetBranchAddress("eventInfo",&eventInfo1);
  
  std::cout << "nentries = " << nentries1 << std::endl;

  //std::ofstream outfile1;
  //outfile1.open("TauTauEventIDs.dat");
  
  double max = -1;
  double min = 2000000000;
  
  for(int i=0; i<nentries1; i++){

      chIn1->GetEntry(i);
      //outfile1 << eventInfo1->event() << '\n';
      if(eventInfo1->event() > max) max = eventInfo1->event(); 
      if(eventInfo1->event() < min) min = eventInfo1->event();
  }
  std::cout << "Min " << min << std::endl;
  std::cout << "Max " << max << std::endl;
  //outfile1.close();
  
  return 0;  

}
