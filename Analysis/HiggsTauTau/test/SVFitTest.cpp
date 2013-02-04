#include <iostream>
#include <string>
#include <stdlib.h>  // for size_t.
#include <stdint.h>

#include "TFile.h"
#include "TTree.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SVFitService.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"

#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"


int main(int argc, char* argv[]){
  typedef ROOT::Math::PtEtaPhiEVector Vector;

  if (argc != 2){
    std::cerr << "Need 1 arg: <input>" << std::endl;
    exit(1);
  }

  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  std::string input_file = argv[1];
  std::string output_file = input_file;
  if (output_file.find("input.root") != input_file.npos) {
    std::size_t pos = output_file.find("input.root");
    output_file.replace(pos, std::string("input.root").length(), "output.root");
  } else {
    std::cerr << "The input file is not named correctly" << std::endl;
    return 1;
  }
  TFile *input = new TFile(input_file.c_str());
  if (!input) {
    std::cerr << "The input file could not be opened" << std::endl;
    return 1;
  }
  TTree *itree = dynamic_cast<TTree *>(input->Get("svfit"));
  if (!itree) {
    std::cerr << "The input tree could not be found" << std::endl;
    return 1;
  }

  ic::Candidate *c1 = NULL;
  ic::Candidate *c2 = NULL;
  ic::Met *met = NULL;
  uint64_t event_hash = 0;
  uint64_t objects_hash = 0;
  unsigned mode = 0;
  double svfit_mass;

  itree->SetBranchAddress("event_hash", &event_hash);
  itree->SetBranchAddress("objects_hash", &objects_hash);
  itree->SetBranchAddress("lepton1", &c1);
  itree->SetBranchAddress("lepton2", &c2);
  itree->SetBranchAddress("met", &met);
  itree->SetBranchAddress("decay_mode", &mode);

  TFile *output = new TFile(output_file.c_str(),"RECREATE");
  TTree *otree = new TTree("svfit","svfit");
  otree->Branch("event_hash", &event_hash, "event_hash/l");
  otree->Branch("objects_hash", &objects_hash, "objects_hash/l");
  otree->Branch("svfit_mass", &svfit_mass);


  ic::SVFitService svfit_service;

  for (unsigned i = 0; i < itree->GetEntries(); ++i) {
    itree->GetEntry(i);
    if (mode == 0) {
      svfit_mass = svfit_service.SVFitMassLepHad(c1, c2, met);
    } else {
      svfit_mass = svfit_service.SVFitMassLepLep(c1, c2, met);
    }
    otree->Fill();
  }

  output->Write();
  delete otree;
  output->Close();
  delete output;

  input->Close();
  delete input;

  return 0;
}




