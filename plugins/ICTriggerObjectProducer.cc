#include "UserCode/ICHiggsTauTau/plugins/ICTriggerObjectProducer.hh"
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

#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"
#include "boost/regex.hpp"


ICTriggerObjectProducer::ICTriggerObjectProducer(const edm::ParameterSet& iConfig) {
  hlt_path_ = iConfig.getUntrackedParameter<std::string>("hltPath");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  store_only_if_fired_ = iConfig.getUntrackedParameter<bool>("StoreOnlyIfFired");
  objects_ = new std::vector<ic::TriggerObject>();
}


ICTriggerObjectProducer::~ICTriggerObjectProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete objects_;
}



// ------------ method called to produce the data  ------------
void ICTriggerObjectProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
 
  // Get the PAT TriggerEvent
  edm::Handle< pat::TriggerEvent > triggerEvent;
  iEvent.getByLabel( "patTriggerEvent", triggerEvent );
  
  objects_->resize(0);

  // Get a vector of all HLT paths
  std::vector<pat::TriggerPath> const* paths = triggerEvent->paths();

  // Find the full label of the chosen HLT path (i.e. with the version number)
  bool fired = true;
  std::string full_name;
  for (unsigned i = 0; i < paths->size(); ++i) {
    std::string name = paths->at(i).name();
    if (name.find(hlt_path_) != name.npos) {
      full_name = name;
      if (store_only_if_fired_ && !(paths->at(i).wasAccept())) fired = false;
      break; // Stop loop after we find the first match
    }
  }

  if (!fired) return;

  //std::cout << "Found Path: " << full_name << std::endl;
  // Get a vector of the objects used in the chosen path
  pat::TriggerObjectRefVector pathObjects = triggerEvent->pathObjects(full_name, false);
  objects_->reserve(pathObjects.size());
  for (unsigned j = 0; j < pathObjects.size(); ++j) {
    objects_->push_back(ic::TriggerObject());
    ic::TriggerObject & obj = objects_->back();
    obj.set_pt(pathObjects[j]->pt());
    obj.set_eta(pathObjects[j]->eta());
    obj.set_phi(pathObjects[j]->phi());
    obj.set_energy(pathObjects[j]->energy());
    obj.set_charge(0);
    obj.set_id(0);
    std::vector<std::size_t> filter_labels;
    //std::cout << j << "\t" << (pathObjects)[j]->collection() << "\tpt: " << (pathObjects)[j]->pt() << "\teta: " << (pathObjects)[j]->eta() << std::endl;
    
    // Get the filters this object was used in
    pat::TriggerFilterRefVector filters = triggerEvent->objectFilters((pathObjects)[j], false);
    for (unsigned k = 0; k < filters.size(); ++k) {
      // Only store the filter label if the filter was used in the chosen path
      if (!triggerEvent->filterInPath(filters[k],full_name, false)) continue;
      //std::cout << "--" << filters[k]->label() << std::endl;
      filter_labels.push_back(CityHash64(filters[k]->label()));
      observed_filters_[filters[k]->label()] = CityHash64(filters[k]->label());
    }
    obj.set_filters(filter_labels);
  }

}

// ------------ method called once each job just before starting event loop  ------------
void ICTriggerObjectProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str(),&objects_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICTriggerObjectProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICTriggerObjectProducer>: EndJob Summary for path: " << hlt_path_ << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "HLT Filters Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_filters_.begin(); iter != observed_filters_.end(); ++iter) {
    std::cout << boost::format("%-70s %-20s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICTriggerObjectProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICTriggerObjectProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICTriggerObjectProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICTriggerObjectProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICTriggerObjectProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICTriggerObjectProducer);
