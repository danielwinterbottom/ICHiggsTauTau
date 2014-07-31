#include "UserCode/ICHiggsTauTau/plugins/ICTriggerObjectProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "boost/format.hpp"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICTriggerObjectProducer::ICTriggerObjectProducer(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      hlt_path_(config.getParameter<std::string>("hltPath")),
      store_only_if_fired_(config.getParameter<bool>("storeOnlyIfFired")) {
  objects_ = new std::vector<ic::TriggerObject>();
  PrintHeaderWithProduces(config, input_, branch_);
  std::cout << boost::format("%-15s : %-60s\n") % "Path" % hlt_path_;
  PrintOptional(1, store_only_if_fired_, "storeOnlyIfFired");
}

ICTriggerObjectProducer::~ICTriggerObjectProducer() { delete objects_; }

void ICTriggerObjectProducer::produce(edm::Event& event,
                                      const edm::EventSetup& setup) {
  edm::Handle<pat::TriggerEvent> trig_handle;
  event.getByLabel(input_, trig_handle);

  objects_->clear();

  // Get a vector of all HLT paths
  std::vector<pat::TriggerPath> const* paths = trig_handle->paths();

  // Find the full label of the chosen HLT path (i.e. with the version number)
  bool fired = true;
  std::string full_name;
  for (unsigned i = 0; i < paths->size(); ++i) {
    std::string const& name = paths->at(i).name();
    if (name.find(hlt_path_) != name.npos) {
      full_name = name;
      if (store_only_if_fired_ && !(paths->at(i).wasAccept())) fired = false;
      break;  // Stop loop after we find the first match
    }
  }
  if (!fired) return;

  // Get a vector of the objects used in the chosen path
  pat::TriggerObjectRefVector objects =
      trig_handle->pathObjects(full_name, false);
  objects_->resize(objects.size(), ic::TriggerObject());
  for (unsigned i = 0; i < objects.size(); ++i) {
    pat::TriggerObject const& src = *(objects.at(i));
    ic::TriggerObject & dest = objects_->at(i);
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(0);
    dest.set_id(0);
    std::vector<std::size_t> filter_labels;

    // Get the filters this object was used in
    pat::TriggerFilterRefVector filters =
        trig_handle->objectFilters((objects)[i], false);
    for (unsigned k = 0; k < filters.size(); ++k) {
      // Only store the filter label if the filter was used in the chosen path
      if (!trig_handle->filterInPath(filters[k], full_name, false)) continue;
      filter_labels.push_back(CityHash64(filters[k]->label()));
      observed_filters_[filters[k]->label()] = CityHash64(filters[k]->label());
    }
    dest.set_filters(filter_labels);
  }
}

void ICTriggerObjectProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &objects_);
}

void ICTriggerObjectProducer::endJob() {
  std::cout << std::string(78, '-') << "\n";
  std::cout << boost::format("Path: %-50s  %20s\n")
      % hlt_path_ % std::string("Hash Summmary");
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_filters_.begin(); iter != observed_filters_.end();
       ++iter) {
    std::cout << boost::format("%-56s| %020i\n") % iter->first % iter->second;
  }
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICTriggerObjectProducer);
