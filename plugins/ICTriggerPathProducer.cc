#include "UserCode/ICHiggsTauTau/plugins/ICTriggerPathProducer.hh"
#include <string>
#include <vector>
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/ICHashTreeProducer.hh"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

#if CMSSW_MAJOR_VERSION >= 7
// In MiniAOD the prescales must be extracted from a new
// "pat::PackedTriggerPrescales" object, but this class was only introduced in
// CMSSW_7_0_5
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#endif

#if CMSSW_MAJOR_VERSION >= 8
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#endif


ICTriggerPathProducer::ICTriggerPathProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      include_if_fired_(config.getParameter<bool>("includeAcceptedOnly")),
      save_strings_(config.getParameter<bool>("saveStrings")),
      split_version_(config.getParameter<bool>("splitVersion")),
      input_is_standalone_(config.getParameter<bool>("inputIsStandAlone")),
      input_prescales_(config.getParameter<edm::InputTag>("inputPrescales")),
      hlt_process_(config.getParameter<std::string>("hltProcess")),
      prescale_fallback_(config.getParameter<bool>("prescaleFallback"))
 {
  if(!input_is_standalone_){
    consumes<pat::TriggerEvent>(input_);
  } else {
    consumes<edm::TriggerResults>(input_);
  }
#if CMSSW_MAJOR_VERSION >= 7
   consumes<pat::PackedTriggerPrescales>(input_prescales_);
#endif
#if CMSSW_MAJOR_VERSION >= 8
  if (prescale_fallback_) {
    consumes<BXVector<GlobalAlgBlk>>(edm::InputTag("gtStage2Digis"));
  }
#endif
  paths_ = new std::vector<ic::TriggerPath>();
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, include_if_fired_, "includeAcceptedOnly");
  PrintOptional(1, save_strings_, "saveStrings");
  PrintOptional(1, split_version_, "splitVersion");
  PrintOptional(1, input_is_standalone_, "inputIsStandAlone");
}

ICTriggerPathProducer::~ICTriggerPathProducer() { delete paths_; }

void ICTriggerPathProducer::produce(edm::Event& event,
                                    const edm::EventSetup& setup) {
  paths_->clear();

  if (!input_is_standalone_) {
    edm::Handle<pat::TriggerEvent> trig_handle;
    event.getByLabel(input_, trig_handle);
    std::vector<pat::TriggerPath> const* paths = trig_handle->paths();
    paths_->reserve(paths->size());
    for (unsigned i = 0; i < paths->size(); ++i) {
      pat::TriggerPath const& src = paths->at(i);
      if (!src.wasAccept() && include_if_fired_) continue;
      paths_->push_back(ic::TriggerPath());
      ic::TriggerPath & dest = paths_->back();
      dest.set_accept(src.wasAccept());
      dest.set_prescale(src.prescale());
      std::string name = src.name();
      SetNameInfo(name, &dest);
    }
  } else {  // i.e. MiniAOD
    edm::Handle<edm::TriggerResults> trigres_handle;
    event.getByLabel(input_, trigres_handle);

#if CMSSW_MAJOR_VERSION >= 7
    edm::Handle<pat::PackedTriggerPrescales> prescales_handle;
    event.getByLabel(input_prescales_, prescales_handle);
#endif

#if CMSSW_MAJOR_VERSION >= 8
    edm::Handle<BXVector<GlobalAlgBlk>> l1algo_handle;
    if (prescale_fallback_) {
      event.getByLabel(edm::InputTag("gtStage2Digis"), l1algo_handle);
    }
#endif

    edm::TriggerNames const& names = event.triggerNames(*trigres_handle);
    paths_->reserve(trigres_handle->size());
    for (unsigned int i = 0, n = trigres_handle->size(); i < n; ++i) {
      if (!trigres_handle->accept(i) && include_if_fired_) continue;
      paths_->push_back(ic::TriggerPath());
      ic::TriggerPath & dest = paths_->back();
      dest.set_accept(trigres_handle->accept(i));
#if CMSSW_MAJOR_VERSION >= 7
      dest.set_prescale(prescales_handle->getPrescaleForIndex(i));
  #if CMSSW_MAJOR_VERSION >= 8
      if (prescale_fallback_) {
        unsigned column = l1algo_handle->at(0, 0).getPreScColumn();
        unsigned other_prescale = hlt_config_.prescaleValue(column, names.triggerName(i));
        // if (dest.prescale() != other_prescale) {
        //   std::cout << names.triggerName(i) << "\t" << hlt_config_.tableName() << "\t" << dest.prescale() << "\t" << other_prescale << "\n";
        // }
        dest.set_prescale(other_prescale);
      }
  #endif
#else
      dest.set_prescale(0);
#endif
      std::string name = names.triggerName(i);
      SetNameInfo(name, &dest);
    }
  }
}

void ICTriggerPathProducer::SetNameInfo(std::string name,
                                        ic::TriggerPath* path) {
  if (split_version_) {
    std::size_t v_pos = name.find_last_of('v');
    if (v_pos != std::string::npos) {
      std::string post_v = name.substr(v_pos+1);
      std::string pre_v = name.substr(0, v_pos+1);
      try {
        unsigned v = boost::lexical_cast<unsigned>(post_v);
        name = pre_v;
        path->set_version(v);
      }
      catch(boost::bad_lexical_cast const& e) {
      }
    }
  }
  //std::cout<<name<<std::endl;
  std::size_t hash = CityHash64(name);
  if (save_strings_) {
    path->set_name(name);
  } else {
    if (!observed_paths_.count(name)) {
      observed_paths_[name] = hash;
    }
  }
  path->set_id(hash);
}

void ICTriggerPathProducer::beginRun(edm::Run const& run,
                                       edm::EventSetup const& es) {
  bool changed = true;
  bool res = hlt_config_.init(run, es, hlt_process_, changed);
  if (!res)
    throw std::runtime_error(
        "HLTConfigProvider did not initialise correctly");
}

void ICTriggerPathProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &paths_);
}

void ICTriggerPathProducer::endJob() {
  // If the trigger path strings were not saved print a summary
  // of the string hashes
  if (!save_strings_) {
    std::cout << std::string(78, '-') << "\n";
    std::cout << boost::format("%-56s  %20s\n")
        % "HLT Paths" % std::string("Hash Summmary");
    std::map<std::string, std::size_t>::const_iterator iter;
    for (iter = observed_paths_.begin(); iter != observed_paths_.end();
         ++iter) {
      ICHashTreeProducer::Add(iter->second, iter->first);
      std::cout << boost::format("%-56s| %020i\n") % iter->first % iter->second;
    }
  }
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICTriggerPathProducer);
