#ifndef UserCode_ICHiggsTauTau_ICTriggerObjectProducer_h
#define UserCode_ICHiggsTauTau_ICTriggerObjectProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"

/**
 * @brief See documentation [here](\ref objs-trig-obj)
 */
class ICTriggerObjectProducer : public edm::EDProducer {
 public:
  explicit ICTriggerObjectProducer(const edm::ParameterSet&);
  ~ICTriggerObjectProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void beginRun(edm::Run const& run, edm::EventSetup const& es);
  virtual void endJob();

  std::vector<ic::TriggerObject>* objects_;
  edm::InputTag input_;
  edm::InputTag input_trigres_;
  std::string branch_;
  std::string hlt_path_;
  bool store_only_if_fired_;
  bool input_is_standalone_;
  std::map<std::string, std::size_t> observed_filters_;
  HLTConfigProvider hlt_config_;
};

#endif
