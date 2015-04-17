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
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICTriggerObjectProducer::ICTriggerObjectProducer(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_trigres_(config.getParameter<edm::InputTag>("inputTriggerResults")),
      branch_(config.getParameter<std::string>("branch")),
      hlt_path_(config.getParameter<std::string>("hltPath")),
      store_only_if_fired_(config.getParameter<bool>("storeOnlyIfFired")),
      input_is_standalone_(config.getParameter<bool>("inputIsStandAlone")) {
  objects_ = new std::vector<ic::TriggerObject>();
  PrintHeaderWithProduces(config, input_, branch_);
  std::cout << boost::format("%-15s : %-60s\n") % "Path" % hlt_path_;
  PrintOptional(1, store_only_if_fired_, "storeOnlyIfFired");
  PrintOptional(1, input_is_standalone_, "inputIsStandAlone");
}

ICTriggerObjectProducer::~ICTriggerObjectProducer() { delete objects_; }

void ICTriggerObjectProducer::produce(edm::Event& event,
                                      const edm::EventSetup& setup) {
  objects_->clear();

  if (!input_is_standalone_) {
    edm::Handle<pat::TriggerEvent> trig_handle;
    event.getByLabel(input_, trig_handle);


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
  } else {  // i.e. MiniAOD
    // We need to figure out the full HLT path name (typically hlt_path_ doesn't
    // contain the version number at the end). We don't have access to the full
    // pat::TriggerEvent here, but we can get the same information from the
    // edm::TriggerResults instead.
    // This code was inspired by the example here:
    //  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Trigger
    edm::Handle<edm::TriggerResults> trigres_handle;
    event.getByLabel(input_trigres_, trigres_handle);
    edm::TriggerNames const& names = event.triggerNames(*trigres_handle);

    bool fired = true;
    std::string full_name;

    for (unsigned int i = 0, n = trigres_handle->size(); i < n; ++i) {
      std::string const& name = names.triggerName(i);
      // std::cout << i << "\t" << name << "\n";
      if (name.find(hlt_path_) != name.npos) {
        full_name = name;
        if (store_only_if_fired_ && !(trigres_handle->accept(i))) fired = false;
        break;  // Stop loop after we find the first match
      }
    }
    if (!fired) return;

    // Have to use the HLTConfigProvider to get the list of object-producing
    // filter modules that were run in this path
    std::set<std::string> path_filters;
    std::vector<std::string> const& filt_vec =
        hlt_config_.saveTagsModules(full_name);
    for (unsigned i = 0; i < filt_vec.size(); ++i)
      path_filters.insert(filt_vec[i]);

    edm::Handle<pat::TriggerObjectStandAloneCollection> trigobj_handle;
    event.getByLabel(input_, trigobj_handle);
    for (unsigned i = 0; i < trigobj_handle->size(); ++i) {
      pat::TriggerObjectStandAlone src = trigobj_handle->at(i);
      src.unpackPathNames(names);
      std::vector<std::string> const& pathnames = src.pathNames();
      bool obj_in_path = false;
      for (unsigned j = 0; j < pathnames.size(); ++j) {
        if (full_name == pathnames[j]) {
          obj_in_path = true;
          break;
        }
      }
      if (!obj_in_path) continue;

      // Ok, so this object was used in the path - now we can add it to the
      // output collection
      objects_->push_back(ic::TriggerObject());
      ic::TriggerObject & dest = objects_->back();
      dest.set_pt(src.pt());
      dest.set_eta(src.eta());
      dest.set_phi(src.phi());
      dest.set_energy(src.energy());
      dest.set_charge(0);
      dest.set_id(0);
      // Get the filters this object was used in
      std::vector<std::string> const& filters = src.filterLabels();
      std::vector<std::size_t> filter_labels;
      for (unsigned k = 0; k < filters.size(); ++k) {
        // Using the info we got from the HLTConfigProvider we can check if this
        // filter module was actually used in the path we are interested in
        if (!path_filters.count(filters[k])) continue;
        filter_labels.push_back(CityHash64(filters[k]));
        observed_filters_[filters[k]] = CityHash64(filters[k]);
      }
      dest.set_filters(filter_labels);
    }
  }
}

void ICTriggerObjectProducer::beginRun(edm::Run const& run,
                                       edm::EventSetup const& es) {
  if (input_is_standalone_) {
    std::string proc =
        input_trigres_.process() != "" ? input_trigres_.process() : "HLT";
    bool changed = true;
    bool res = hlt_config_.init(run, es, proc, changed);
    if (!res)
      throw std::runtime_error(
          "HLTConfigProvider did not initialise correctly");
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
