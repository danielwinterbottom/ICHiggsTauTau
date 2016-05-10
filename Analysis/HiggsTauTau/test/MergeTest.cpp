// ROOT includes
#include "TChain.h"
#include "TMath.h"
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

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

int main(){

  // Create the first tree. 
  TString filename1;
  TChain *chIn1 = new TChain("icEventProducer/EventTree");
  std::ifstream infile1;
  infile1.open("EventTreeAll.dat");
  while (infile1 >> filename1) chIn1->Add(filename1);
  infile1.close();
  
  // Create the second tree. 
  TString filename2;
  TChain *chIn2 = new TChain("icEventProducer/EventTree");
  std::ifstream infile2;
  infile2.open("L1ObjectsAll.dat");
  while (infile2 >> filename2) chIn2->Add(filename2);
  infile2.close();
  
  TChain *chIn3 = new TChain("EventTree");
  chIn3->Add("test/EventTreeMergedFast.root");
  
  // Get the eventInfo for both trees.
  ic::EventInfo *eventInfo1 = new ic::EventInfo();
  chIn1->SetBranchAddress("eventInfo",&eventInfo1);
  ic::EventInfo *eventInfo2 = new ic::EventInfo();
  chIn2->SetBranchAddress("eventInfo",&eventInfo2);
  ic::EventInfo *eventInfo3 = new ic::EventInfo();
  chIn3->SetBranchAddress("eventInfo",&eventInfo3);
  
  // Get new branches from tree 2. 
  std::vector<ic::Tau> *tausold = new std::vector<ic::Tau>();
  chIn1->SetBranchAddress("taus",&tausold);
  
  // Get new branches from tree 2. 
  std::vector<ic::L1TTau> *l1tausold = new std::vector<ic::L1TTau>();
  chIn2->SetBranchAddress("L1Taus",&l1tausold);

  
  std::vector<ic::Tau> *tausnew = new std::vector<ic::Tau>();
  chIn3->SetBranchAddress("taus",&tausnew);
  
  // Get new branches from tree 2. 
  std::vector<ic::L1TTau> *l1tausnew = new std::vector<ic::L1TTau>();
  chIn3->SetBranchAddress("L1Taus",&l1tausnew);

  
//unsigned nentries1 = chIn1->GetEntries();
  unsigned nentries1 = 5;
  unsigned nentries2 = chIn2->GetEntries();
  
    for(unsigned i=995; i<995+nentries1; i++){
      
      std::cout << "New Test Event." << std::endl;
      
      chIn1->GetEntry(i);
      chIn3->GetEntry(i);
      for(unsigned j=0; j<nentries2; j++){

          chIn2->GetEntry(j);
          
          // If eventIDs match then fill new branch with copied branches.
          if(eventInfo1->event() == eventInfo2->event()){
              
              std::cout << "Offline Tree Event " << eventInfo1->event()  << ", Tau Size = " << tausold->size() << ". L1 Tree Event " << eventInfo2->event() <<  ", L1TTau Size = " << l1tausold->size() << std::endl;
  std::cout << "Merged Tree Event " << eventInfo3->event()  << ", Tau Size = " << tausnew->size()  <<  ", L1TTau Size = " << l1tausnew->size() << std::endl;

              break;
              }
              
              
              
          }
          
      
  }

}
