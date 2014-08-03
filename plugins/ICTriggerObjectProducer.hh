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

#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"

/**
 * @brief Produces an ic::TriggerObject collection for a specific trigger path
 *using the pat::Trigger input
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py TriggerObject
 */
class ICTriggerObjectProducer : public edm::EDProducer {
 public:
  explicit ICTriggerObjectProducer(const edm::ParameterSet&);
  ~ICTriggerObjectProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::TriggerObject>* objects_;
  edm::InputTag input_;
  std::string branch_;
  std::string hlt_path_;
  bool store_only_if_fired_;
  std::map<std::string, std::size_t> observed_filters_;
};

#endif
