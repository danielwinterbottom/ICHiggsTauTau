#include "UserCode/ICHiggsTauTau/plugins/ICTriggerPathProducer.hh"
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
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"
#include "boost/regex.hpp"


ICTriggerPathProducer::ICTriggerPathProducer(const edm::ParameterSet& iConfig) {
  paths_ = new std::vector<ic::TriggerPath>();
}


ICTriggerPathProducer::~ICTriggerPathProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete paths_;
}



// ------------ method called to produce the data  ------------
void ICTriggerPathProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle< pat::TriggerEvent > triggerEvent;
  iEvent.getByLabel( "patTriggerEvent", triggerEvent );
  paths_->resize(0);
  std::vector<pat::TriggerPath> const* paths = triggerEvent->paths();
  paths_->reserve(paths->size());
  for (unsigned i = 0; i < paths->size(); ++i) {
    if (paths->at(i).wasAccept()) {
      paths_->push_back(ic::TriggerPath());
      ic::TriggerPath & path = paths_->back();
      path.set_accept(paths->at(i).wasAccept());
      path.set_name(paths->at(i).name());
      path.set_prescale(paths->at(i).prescale());
      path.set_id(CityHash64(paths->at(i).name()));
    }
  }
  // pat::TriggerObjectRefVector pathObjects = triggerEvent->pathObjects("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v3",false);
  // for (unsigned j = 0; j < pathObjects.size(); ++j) {
  //   std::cout << j << "\t" << (pathObjects)[j]->collection() << "\tpt: " << (pathObjects)[j]->pt() << "\teta: " << (pathObjects)[j]->eta() << std::endl;
  //   pat::TriggerFilterRefVector filters = triggerEvent->objectFilters((pathObjects)[j], false);
  //   for (unsigned k = 0; k < filters.size(); ++k) {
  //     std::cout << "--" << filters[k]->label() << "\t" << filters[k]->type() << std::endl;
  //   }
  // }

}

// ------------ method called once each job just before starting event loop  ------------
void ICTriggerPathProducer::beginJob() {
 ic::StaticTree::tree_->Branch("triggerPaths",&paths_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICTriggerPathProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICTriggerPathProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICTriggerPathProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICTriggerPathProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICTriggerPathProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICTriggerPathProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICTriggerPathProducer);
