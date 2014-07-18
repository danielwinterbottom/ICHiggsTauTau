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
#include "boost/lexical_cast.hpp"

ICTriggerPathProducer::ICTriggerPathProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      include_if_fired_(config.getParameter<bool>("includeAcceptedOnly")),
      save_strings_(config.getParameter<bool>("saveStrings")),
      split_version_(config.getParameter<bool>("splitVersion")) {
  paths_ = new std::vector<ic::TriggerPath>();
}

ICTriggerPathProducer::~ICTriggerPathProducer() { delete paths_; }

void ICTriggerPathProducer::produce(edm::Event& event,
                                    const edm::EventSetup& setup) {
  edm::Handle<pat::TriggerEvent> trig_handle;
  event.getByLabel(input_, trig_handle);
  std::vector<pat::TriggerPath> const* paths = trig_handle->paths();
  paths_->clear();
  paths_->reserve(paths->size());
  for (unsigned i = 0; i < paths->size(); ++i) {
    pat::TriggerPath const& src = paths->at(i);
    if (!src.wasAccept() && include_if_fired_) continue;
    paths_->push_back(ic::TriggerPath());
    ic::TriggerPath & dest = paths_->back();
    dest.set_accept(src.wasAccept());
    dest.set_prescale(src.prescale());
    std::string name = src.name();
    if (split_version_) {
      std::size_t v_pos = name.find_last_of('v');
      if (v_pos != std::string::npos) {
        std::string post_v = name.substr(v_pos+1);
        std::string pre_v = name.substr(0, v_pos+1);
        try {
          unsigned v = boost::lexical_cast<unsigned>(post_v);
          name = pre_v;
          dest.set_version(v);
        }
        catch(boost::bad_lexical_cast const& e) {
        }
      }
    }
    if (save_strings_) dest.set_name(name);
    dest.set_id(CityHash64(name));
  }
}

void ICTriggerPathProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &paths_);
}

void ICTriggerPathProducer::endJob() {
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICTriggerPathProducer);
