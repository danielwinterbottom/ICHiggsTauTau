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
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"

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
  infile2.open("EventTreeAll.dat");
  while (infile2 >> filename2) chIn2->Add(filename2);
  infile2.close();
  
  //unsigned nentries1 = chIn1->GetEntries();
  unsigned nentries1 = 1000;
  //unsigned nentries2 = chIn1->GetEntries();
  unsigned nentries2 = 1000;

  // Create output file and clone the first tree.
  TFile *fOut = new TFile("test/EventTreeMerged.root","RECREATE");
  TTree *tOut = chIn1->CloneTree(nentries1);
  
  // Get the eventInfo for both trees.
  ic::EventInfo *eventInfo1 = new ic::EventInfo();
  tOut->SetBranchAddress("eventInfo",&eventInfo1);
  ic::EventInfo *eventInfo2 = new ic::EventInfo();
  chIn2->SetBranchAddress("eventInfo",&eventInfo2); 
  
  // Get new branches from tree 2.
  std::vector<ic::Tau> *taus = new std::vector<ic::Tau>();
  chIn2->SetBranchAddress("taus",&taus); //delete these after testing!
 
  std::vector<ic::L1TTau> *l1taus = new std::vector<ic::L1TTau>();
  chIn2->SetBranchAddress("L1Taus",&l1taus);
  std::vector<ic::L1TMuon> *l1muons = new std::vector<ic::L1TMuon>();
  chIn2->SetBranchAddress("L1Muon",&l1muons);
  std::vector<ic::L1TEGamma> *l1egammas = new std::vector<ic::L1TEGamma>();
  chIn2->SetBranchAddress("L1EGammas",&l1egammas);
  std::vector<ic::L1TJet> *l1jets = new std::vector<ic::L1TJet>();
  chIn2->SetBranchAddress("L1Jets",&l1jets);
  std::vector<ic::L1TSum> *l1sums = new std::vector<ic::L1TSum>();
  chIn2->SetBranchAddress("L1Sums",&l1sums);

  
  // Create new branches for new tree.
  std::vector<ic::Tau> *tausnew = new std::vector<ic::Tau>();
  TBranch *newBranch0 = tOut->Branch("tausNew", &tausnew);
  ic::EventInfo *eventInfonew = new ic::EventInfo();
  TBranch *newBranch = tOut->Branch("EventInfoNew", &eventInfonew);//delete these after testing!
  
  std::vector<ic::L1TTau> *l1tausnew = new std::vector<ic::L1TTau>();
  TBranch *newBranch1 = tOut->Branch("L1Taus", &l1tausnew);
  std::vector<ic::L1TMuon> *l1muonsnew = new std::vector<ic::L1TMuon>();
  TBranch *newBranch2 = tOut->Branch("L1Muons", &l1muonsnew);
  std::vector<ic::L1TEGamma> *l1egammasnew = new std::vector<ic::L1TEGamma>();
  TBranch *newBranch3 = tOut->Branch("L1EGammas", &l1egammasnew);
  std::vector<ic::L1TJet> *l1jetsnew = new std::vector<ic::L1TJet>();
  TBranch *newBranch4 = tOut->Branch("L1Jets", &l1jetsnew);
  std::vector<ic::L1TSum> *l1sumsnew = new std::vector<ic::L1TSum>();
  TBranch *newBranch5 = tOut->Branch("L1Sums", &l1sumsnew);
  
  for(unsigned i=0; i<nentries1; i++){
      
      tOut->GetEntry(i);
      
      for(unsigned j=0; j<nentries2; j++){
          
          chIn2->GetEntry(j);
          // If eventIDs match then fill new branch with copied branches.
          if(eventInfo1->event() == eventInfo2->event()){
              tausnew = taus;
              eventInfonew = eventInfo2;
              newBranch0->Fill();
              newBranch->Fill();//delete these after testing!
              
              l1tausnew = l1taus;
              l1muonsnew = l1muons;
              l1egammasnew = l1egammas;
              l1jetsnew = l1jets;
              l1sumsnew = l1sums;
              newBranch1->Fill();
              newBranch2->Fill();
              newBranch3->Fill();
              newBranch4->Fill();
              newBranch5->Fill();
              
              break;
          }
          
      }
  }

  fOut->Write();  
  fOut->Close();
  
  return 0;  

}
