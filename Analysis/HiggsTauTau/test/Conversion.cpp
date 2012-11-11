#include <iostream>
#include <vector>
#include <string>
#include "TSystem.h"
#include "TKey.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

#include "TH1F.h"
#include "TObjArray.h"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "UserCode/AGilbert/interface/Objects.hh"
#include "UserCode/AGilbert/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"

int main(int argc, char* argv[]){


  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeAGilbert.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  std::vector<agilbert::Electron> *ag_electrons = NULL;
  std::vector<ic::Electron> *ic_electrons = NULL;

  std::vector<agilbert::Muon> *ag_muonsPFlow = NULL;
  std::vector<ic::Muon> *ic_muonsPFlow = NULL;

  std::vector<agilbert::PFJet> *ag_pfJetsPFlow = NULL;
  std::vector<ic::PFJet> *ic_pfJetsPFlow = NULL;

  std::vector<agilbert::Met> *ag_pfMVAMetVector = NULL;
  std::vector<ic::Met> *ic_pfMVAMetVector = NULL;

  agilbert::Met *ag_pfMet = NULL;
  ic::Met *ic_pfMet = NULL;

  agilbert::Met *ag_pfMVAMet = NULL;
  ic::Met *ic_pfMVAMet = NULL;

  agilbert::EventInfo *ag_eventInfo = NULL;
  ic::EventInfo *ic_eventInfo = NULL;

  std::vector<agilbert::Tau> *ag_taus = NULL;
  std::vector<ic::Tau> *ic_taus = NULL;

  std::vector<agilbert::Vertex> *ag_vertices = NULL;
  std::vector<ic::Vertex> *ic_vertices = NULL;

  std::vector<agilbert::GenParticle> *ag_genParticles = NULL;
  std::vector<ic::GenParticle> *ic_genParticles = NULL;

  std::vector<agilbert::TriggerPath> *ag_triggerPaths = NULL;
  std::vector<ic::TriggerPath> *ic_triggerPaths = NULL;

  std::map<std::string, std::vector<agilbert::TriggerObject> * > ag_trig_obj;
  std::map<std::string, std::vector<ic::TriggerObject> * > ic_trig_obj;

  for (int i = 1; i < argc; ++i){
    std::cout << i << "\t" << argv[i] << std::endl;
    TFile *fin = new TFile(argv[i]);
    fin->cd();
    gDirectory->cd("agEventProducer");
    TTree *tin = dynamic_cast<TTree*>(gDirectory->Get("EventTree"));
    TFile *fout = new TFile("Test.root","RECREATE");
    fout->cd();
    gDirectory->mkdir("icEventProducer");
    gDirectory->cd("icEventProducer");
    TTree *tout = new TTree("EventTree","EventTree");
    TObjArray *arr = tin->GetListOfBranches();
    for (unsigned j = 0; j < unsigned(arr->GetSize()); ++j) {
      std::string name = arr->At(j)->GetName();
      std::cout << "-- " << name << std::endl;
      if (name == "electrons") {
        tin->SetBranchAddress("electrons", &ag_electrons);
        tout->Branch("electrons", &ic_electrons);
        continue;
      }

      if (name == "muonsPFlow") {
        tin->SetBranchAddress("muonsPFlow", &ag_muonsPFlow);
        tout->Branch("muonsPFlow", &ic_muonsPFlow);
        continue;
      }

      if (name == "pfJetsPFlow") {
        tin->SetBranchAddress("pfJetsPFlow", &ag_pfJetsPFlow);
        tout->Branch("pfJetsPFlow", &ic_pfJetsPFlow);
        continue;
      }

      if (name == "pfMVAMetVector") {
        tin->SetBranchAddress("pfMVAMetVector", &ag_pfMVAMetVector);
        tout->Branch("pfMVAMetVector", &ic_pfMVAMetVector);
        continue;
      }

      if (name == "pfMet") {
        tin->SetBranchAddress("pfMet", &ag_pfMet);
        tout->Branch("pfMet", &ic_pfMet);
        continue;
      }

      if (name == "eventInfo") {
        tin->SetBranchAddress("eventInfo", &ag_eventInfo);
        tout->Branch("eventInfo", &ic_eventInfo);
        continue;
      }

      if (name == "pfMVAMet") {
        tin->SetBranchAddress("pfMVAMet", &ag_pfMVAMet);
        tout->Branch("pfMVAMet", &ic_pfMVAMet);
        continue;
      }

      if (name == "taus") {
        tin->SetBranchAddress("taus", &ag_taus);
        tout->Branch("taus", &ic_taus);
        continue;
      }

      if (name == "vertices") {
        tin->SetBranchAddress("vertices", &ag_vertices);
        tout->Branch("vertices", &ic_vertices);
        continue;
      }

      if (name == "triggerPaths") {
        tin->SetBranchAddress("triggerPaths", &ag_triggerPaths);
        tout->Branch("triggerPaths", &ic_triggerPaths);
        continue;
      }

      if (name == "genParticles") {
        tin->SetBranchAddress("genParticles", &ag_genParticles);
        tout->Branch("genParticles", &ic_genParticles);
        continue;
      }

      if (name.find("triggerObjects") != name.npos) {
        tin->SetBranchAddress(name.c_str(), &(ag_trig_obj[name]));
        tout->Branch(name.c_str(), &(ic_trig_obj[name]));
        continue;
      }

      std::cout << "Error, don't know what to do with branch " << name << ", quitting!" << std::endl;
      exit(1);
    }

    for (unsigned j = 0; j < tin->GetEntries(); ++j) {
      tin->GetEntry(j);
      ic_electrons = reinterpret_cast<std::vector<ic::Electron>*>(ag_electrons);
      ic_muonsPFlow = reinterpret_cast<std::vector<ic::Muon>*>(ag_muonsPFlow);
      ic_pfJetsPFlow = reinterpret_cast<std::vector<ic::PFJet>*>(ag_pfJetsPFlow);
      ic_pfMVAMetVector = reinterpret_cast<std::vector<ic::Met>*>(ag_pfMVAMetVector);
      ic_pfMet = reinterpret_cast<ic::Met *>(ag_pfMet);
      ic_pfMVAMet = reinterpret_cast<ic::Met *>(ag_pfMVAMet);
      ic_eventInfo = reinterpret_cast<ic::EventInfo *>(ag_eventInfo);
      ic_taus = reinterpret_cast<std::vector<ic::Tau>*>(ag_taus);
      ic_vertices = reinterpret_cast<std::vector<ic::Vertex>*>(ag_vertices);
      ic_triggerPaths = reinterpret_cast<std::vector<ic::TriggerPath>*>(ag_triggerPaths);
      ic_genParticles = reinterpret_cast<std::vector<ic::GenParticle>*>(ag_genParticles);


      std::map<std::string, std::vector<ic::TriggerObject> * >::iterator it;
      for (it = ic_trig_obj.begin(); it != ic_trig_obj.end(); ++it) {
        it->second = reinterpret_cast<std::vector<ic::TriggerObject>*>(ag_trig_obj[it->first]); 
      }

      tout->Fill();
    }

    tout->Write(0,TObject::kOverwrite);  
    fout->Close();
    delete fout;
    fin->Close();
    delete fin;
  }



 
 
  
  return 0;
}




