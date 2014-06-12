#include "UserCode/ICHiggsTauTau/plugins/TestModule.hh"
#include <boost/functional/hash.hpp>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


#include "boost/format.hpp"


TestModule::TestModule(const edm::ParameterSet& iConfig) {

}


TestModule::~TestModule() {
}

// ------------ method called to produce the data  ------------
void TestModule::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<edm::View<reco::Muon> > muons;
  iEvent.getByLabel("muons", muons);
  std::cout << "Number of muons: " << muons->size() << "\n";
  for (unsigned i = 0; i < muons->size(); ++i) {
      std::cout << "  " << &(muons->at(i)) << "\n";
      // ptr_vec.push_back(edm::Ptr<reco::Muon>(muons, i));
  }

  edm::Handle<edm::View<reco::Muon> > best_muons;
  iEvent.getByLabel("bestMuons", best_muons);
  std::cout << "Number of bestMuons: " << best_muons->size() << "\n";
  for (unsigned i = 0; i < best_muons->size(); ++i) {
    std::cout << "  " << &(best_muons->at(i)) << "\n";
  }

  edm::Handle<edm::View<reco::Muon> > merge_muons;
  iEvent.getByLabel("mergeMuons", merge_muons);
  std::cout << "Number of mergeMuons: " << merge_muons->size() << "\n";
  for (unsigned i = 0; i < merge_muons->size(); ++i) {
    std::cout << "  " << &(merge_muons->at(i)) << "\n";
  }

}

void TestModule::beginJob() {

}

// ------------ method called once each job just after ending the event loop  ------------
void TestModule::endJob() {

}

// ------------ method called when starting to processes a run  ------------
void TestModule::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void TestModule::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void TestModule::beginLuminosityBlock(edm::LuminosityBlock&,
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void TestModule::endLuminosityBlock(edm::LuminosityBlock&,
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TestModule::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


DEFINE_FWK_MODULE(TestModule);
