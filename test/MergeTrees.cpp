
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
#include <memory>
#include <map>

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

int main(){

//tchain!!
  TFile *fIn1 = new TFile("test/EventTree.root", "READ");
  TFile *fIn2 = new TFile("test/EventTree2.root", "READ");
  ///TFile *fOut = new TFile("test/EventTreeMerged.root","RECREATE");

  TTree *tIn1 = (TTree*)fIn1->Get("EventTree");
  //int nentries = tIn1->GetEntries();
  //TTree *tOut = tIn1->CloneTree(-1);
  TTree *tIn2 = (TTree*)fIn2->Get("EventTree");
  
  ic::EventInfo *eventInfo = new ic::EventInfo();
  tIn2->SetBranchAddress("eventInfo",&eventInfo);
  //TBranch *neweventBranch = tIn1->Branch("new", &new,"new/F");
  tIn2->GetEntry(1);
  tOut->GetEntry(1);
  std::cout << eventInfo->event() << std::endl;
  //fOut->Write();    

}
