#include <iostream>
#include <string>
#include <stdlib.h>  // for size_t.
#include <stdint.h>

#include "TFile.h"
#include "TTree.h"
#include "HiggsTauTau/interface/SVFitService.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"

#include "TSystem.h"
// #include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"


int main(int argc, char* argv[]){
  // typedef ROOT::Math::PtEtaPhiEVector Vector;

  if (!(argc == 2 || argc== 3)){
    std::cout<<argc<<std::endl;
    std::cerr << "Need 1 or 2 args: <input> [<run_legacy_svfit>]" << std::endl;
    exit(1);
  }

  // Load necessary libraries for ROOT I/O of custom classes
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();

  std::string input_file = argv[1];
  bool run_legacy = false;
  if(argc == 3){
     run_legacy = atoi(argv[2]);
    } 
  std::string output_file = input_file;
  bool MC=true; // Set to true to use Markov-Chain integration
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
  unsigned event = 0;
  unsigned lumi = 0;
  unsigned run = 0;
  ULong64_t objects_hash = 0;
  unsigned mode = 0;
  int dm1 = -1;
  int dm2 = -1;
  double svfit_mass;
  ic::Candidate *svfit_vector = NULL;

  TH1::AddDirectory(kFALSE);

 if(!run_legacy){
  itree->SetBranchAddress("event", &event);
  itree->SetBranchAddress("lumi", &lumi);
  itree->SetBranchAddress("run", &run);
  itree->SetBranchAddress("objects_hash", &objects_hash);
  itree->SetBranchAddress("lepton1", &c1);
  itree->SetBranchAddress("dm1", &dm1);
  itree->SetBranchAddress("lepton2", &c2);
  itree->SetBranchAddress("dm2", &dm2);
  itree->SetBranchAddress("met", &met);
  itree->SetBranchAddress("decay_mode", &mode);

  TFile *output = new TFile(output_file.c_str(),"RECREATE");
  TTree *otree = new TTree("svfit","svfit");
  otree->Branch("event", &event, "event/i");
  otree->Branch("lumi", &lumi, "lumi/i");
  otree->Branch("run", &run, "run/i");
  otree->Branch("objects_hash", &objects_hash, "objects_hash/l");
  otree->Branch("svfit_mass", &svfit_mass);
  otree->Branch("svfit_vector", &svfit_vector);

  ic::SVFitService svfit_service;


  for (unsigned i = 0; i < itree->GetEntries(); ++i) {
    itree->GetEntry(i);
    std::pair<ic::Candidate, double> result;
    if (mode == 0) {
      result = svfit_service.SVFitCandidateMuHad(c1, c2,dm2, met, MC);
    } else if (mode == 1){
      result = svfit_service.SVFitCandidateEleMu(c1, c2, met, MC);
    } else if (mode == 2){
      result = svfit_service.SVFitCandidateEleHad(c1, c2, dm2, met, MC);
    } else if (mode == 3){
      result = svfit_service.SVFitCandidateHadHad(c1,dm1, c2, dm2,met, MC);
    } else{
      std::cout<<"Mode "<<mode<<" not valid"<<std::endl;
      exit(1);
    }
    svfit_mass = result.second;
    svfit_vector = &(result.first);
    svfit_vector->set_id(objects_hash);
    std::cout << "Mass: " << svfit_mass << "\tVector Mass: " << svfit_vector->M() << "\tVector pT: " << svfit_vector->pt() << std::endl;
    otree->Fill();
  }
  output->Write();
  delete otree;
  output->Close();
  delete output;

 }else{
  itree->SetBranchAddress("event", &event);
  itree->SetBranchAddress("lumi", &lumi);
  itree->SetBranchAddress("run", &run);
  itree->SetBranchAddress("objects_hash", &objects_hash);
  itree->SetBranchAddress("lepton1", &c1);
  itree->SetBranchAddress("lepton2", &c2);
  itree->SetBranchAddress("met", &met);
  itree->SetBranchAddress("decay_mode", &mode);

  TFile *output = new TFile(output_file.c_str(),"RECREATE");
  TTree *otree = new TTree("svfit","svfit");
  otree->Branch("event", &event, "event/i");
  otree->Branch("lumi", &lumi, "lumi/i");
  otree->Branch("run", &run, "run/i");
  otree->Branch("objects_hash", &objects_hash, "objects_hash/l");
  otree->Branch("svfit_mass", &svfit_mass);
  otree->Branch("svfit_vector", &svfit_vector);

  ic::SVFitService svfit_service;

  for (unsigned i = 0; i < itree->GetEntries(); ++i) {
    itree->GetEntry(i);
    std::pair<ic::Candidate, double> result;
    if (mode == 0) {
      result = svfit_service.SVFitCandidateLepHad(c1, c2, met, MC);
    } else {
      result = svfit_service.SVFitCandidateLepLep(c1, c2, met, MC);
    }
    svfit_mass = result.second;
    svfit_vector = &(result.first);
    svfit_vector->set_id(objects_hash);
    //std::cout << "Mass: " << svfit_mass << "\tVector Mass: " << svfit_vector->M() << "\tVector pT: " << svfit_vector->pt() << std::endl;
    otree->Fill();
  }
  output->Write();
  delete otree;
  output->Close();
  delete output;
}

  input->Close();
  delete input;

  return 0;
}




