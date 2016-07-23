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
  
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"

int main(int argc, char* argv[]){
  if(argc != 2){
      std::cout << "Wrong number of arguments, expected 1 argument." << std::endl;
      return 0;
  }
  int num = std::atoi(argv[1]);
  
  // Create the first tree. 
  std::string filename1;
  std::string filenametemp;
  TChain *tIn = new TChain("icEventProducer/EventTree");
  std::ifstream infile1;
  infile1.open("ZeroBias2Eta3.dat");
 
  int linenumber = 1;
  while(infile1 >> filenametemp){
      if(linenumber == num){
          filename1 = filenametemp;
          break;
      }
      linenumber++;
  }
  infile1.close();

  tIn->Add(filename1.c_str());

  ic::EventInfo  *eventInfo1 = new ic::EventInfo ();
  tIn->SetBranchAddress("eventInfo",&eventInfo1);
  
  TString filename2;
  TChain *chIn2 = new TChain("icEventProducer/EventTree");
  std::ifstream infile2;
  infile2.open("ZeroBias2Eta5.dat");
  while (infile2 >> filename2) chIn2->Add(filename2);
          
  infile2.close();
  
  unsigned nentries2 = chIn2->GetEntries();
  unsigned nentries1 = tIn->GetEntries();
  
  std::cout << "Entries in tree 1: " << nentries1 << std::endl;
  std::cout << "Entries in tree 2: " << nentries2 << std::endl;
  
  std::string outputfilename = Form("/vols/cms02/dw515/l1t-integration-v58pt1/ZeroBias/BothSumRanges/EventTree2_%d.root", num);
  TFile *fOut = new TFile(outputfilename.c_str(),"RECREATE");
  
  std::cout << "Cloaning offline tree." << std::endl;
  TTree *tOut = tIn->CloneTree(0);
  
  std::cout << "Finished cloaning offline tree." << std::endl;
  
  std::cout << "Getting branches from L1 trees." << std::endl;
  

  std::vector<ic::L1TSum> *l1sums = new std::vector<ic::L1TSum>();
  chIn2->SetBranchAddress("L1Sums",&l1sums);
  ic::EventInfo *eventInfo2 = new ic::EventInfo();
  chIn2->SetBranchAddress("eventInfo",&eventInfo2);
  
  std::cout << "Finished getting branches from L1 trees." << std::endl;
  
  std::cout << "Creating new L1 branches on output tree." << std::endl;
  
  std::vector<ic::L1TSum> *l1sumsnew = new std::vector<ic::L1TSum>();
  tOut->Branch("L1SumsEtaRange5pt0", &l1sumsnew);
  
  std::cout << "Finished creating new L1 branches on output tree." << std::endl;

  unsigned n_report = 1000;
  unsigned NEventsMerged=0;
  
  std::cout << "Merging trees..." << std::endl;
  
  
  for(unsigned i=0; i<nentries1; i++){
    
      tIn->GetEntry(i);
      
      for(unsigned j=0; j<nentries2; j++){
          
          chIn2->GetEntry(j);  
          
          // If eventIDs match then fill new branch with copied branches.
          if(eventInfo1->event() == eventInfo2->event()){
      
              l1sumsnew = l1sums;
              
              tOut->Fill();
              NEventsMerged++;
      
              // If a match is found it is likey that the next few preceeding events are also matched so this next part looks to match those events without doing the full loop.
              bool proceed = true;
              unsigned jtemp = j+1;
              unsigned itemp = i+1;
              
              while(proceed){
                  
                  tIn->GetEntry(itemp);
                  chIn2->GetEntry(jtemp);
                  
                  if(eventInfo1->event() == eventInfo2->event()){
                      if(itemp % n_report == 0) std::cout << "Finished merging " << itemp << "th event." << std::endl;
                      //continue filling  
                  
                      l1sumsnew = l1sums;
                      
                      tOut->Fill();
                      NEventsMerged++;
                      itemp++;
                      jtemp++;
                      
                      if(jtemp == nentries2){
                          i = itemp -1;
                          proceed = false;
                      }
                      if(itemp == nentries1){
                          
                          //tOut->Print();
                          fOut->Write();  
                          fOut->Close();
                          
                          std::cout << "Merged " << NEventsMerged << " out of " <<nentries1 << std::endl;
                          std::cout << NEventsMerged/nentries1 << std::endl;
                          return 0;
                      }
                      
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
  
  //tOut->Print();
  fOut->Write();  
  fOut->Close();
  
  std::cout << "Merged " << NEventsMerged << " out of " <<nentries1 << std::endl;
  std::cout << NEventsMerged/nentries1 << std::endl;
  
  return 0;  

}
