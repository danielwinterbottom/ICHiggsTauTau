#include "UserCode/ICHiggsTauTau/plugins/ICEventProducer.hh"
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

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "TTree.h"


ICEventProducer::ICEventProducer(const edm::ParameterSet& iConfig)
{
   edm::Service<TFileService> lFileService;
   ic::StaticTree::tree_ = lFileService->make<TTree>("EventTree","EventTree");
}


ICEventProducer::~ICEventProducer() {
}


// ------------ method called to produce the data  ------------
void ICEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   ic::StaticTree::tree_->Fill();
   static unsigned processed = 0;
   ++processed;
   if (processed == 500) ic::StaticTree::tree_->OptimizeBaskets();
}

// ------------ method called once each job just before starting event loop  ------------
void ICEventProducer::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void ICEventProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICEventProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICEventProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICEventProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICEventProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ICEventProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICEventProducer);
