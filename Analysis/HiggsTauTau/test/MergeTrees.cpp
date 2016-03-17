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

int main(int argc, char* argv[]){
  if(argc != 2){
      std::cout << "Wrong number of arguments, expected 1 argument." << std::endl;
      return 0;
  }
  int num = std::atoi(argv[1]);
  
  // Create the first tree. 
  std::string filename1;
  std::string filenametemp;
  TChain *chIn1 = new TChain("icEventProducer/EventTree");
  std::ifstream infile1;
  infile1.open("EventTreeAll.dat");
 
  int linenumber = 1;
  while(infile1 >> filenametemp){
      if(linenumber == num){
          filename1 = filenametemp;
          break;
      }
      linenumber++;
  }

  chIn1->Add(filename1.c_str());
  infile1.close();
  
  // Create the second tree. 
  TString filename2;
  TChain *chIn2 = new TChain("icEventProducer/EventTree");
  std::ifstream infile2;
  infile2.open("L1ObjectsAll.dat");
  while (infile2 >> filename2) chIn2->Add(filename2);
          
  infile2.close();
  
  unsigned nentries2 = chIn2->GetEntries();
  
  std::cout << "Cloning offline tree..." << std::endl;

  // Create output file and clone the first tree.
  std::string outputfilename = Form("/vols/cms02/dw515/EventTreeMerged_%d.root", num);
  TFile *fOut = new TFile(outputfilename.c_str(),"RECREATE");
  TTree *tOut = chIn1->CloneTree();
  unsigned nentries1 = tOut->GetEntries();
  std::cout << "Finished cloning offline tree. Merging with L1 tree..." << std::endl;
  
  // Get the eventInfo for both trees.
  ic::EventInfo *eventInfo1 = new ic::EventInfo();
  tOut->SetBranchAddress("eventInfo",&eventInfo1);
  ic::EventInfo *eventInfo2 = new ic::EventInfo();
  chIn2->SetBranchAddress("eventInfo",&eventInfo2);
  
  // Get new branches from tree 2. 
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
  
  // Create new braches on new tree.
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
  
  unsigned n_report = 10000;

  
  for(unsigned i=0; i<nentries1; i++){

      tOut->GetEntry(i);

      for(unsigned j=0; j<nentries2; j++){

          chIn2->GetEntry(j);          
          
          // If eventIDs match then fill new branch with copied branches.
          if(eventInfo1->event() == eventInfo2->event()){
              if(i % n_report == 0) std::cout << "Finished merging " << i << "th event." << std::endl;
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
              
              // If a match is found it is likey that the next few preceeding events are also matched so this next part looks to match those events without doing the full loop.
              bool proceed = true;
              unsigned jtemp = j+1;
              unsigned itemp = i+1;
              
              while(proceed){
                  
                  tOut->GetEntry(itemp);
                  chIn2->GetEntry(jtemp);
                  
                  if(eventInfo1->event() == eventInfo2->event()){
                      if(itemp % n_report == 0) std::cout << "Finished merging " << itemp << "th event." << std::endl;
                      //continue filling  
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
                      itemp++;
                      jtemp++;
                  }
                  else{
                      i = itemp-1;
                      j = jtemp-1;
                      proceed = false;
                  }
              }
              
              break;
          }
          
      }
  }

  fOut->Write();  
  fOut->Close();
  
  return 0;  

}
