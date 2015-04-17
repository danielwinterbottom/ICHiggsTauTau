#ifndef UserCode_ICHiggsTauTau_ICTriggerPathProducer_h
#define UserCode_ICHiggsTauTau_ICTriggerPathProducer_h

#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"

/**
 * @brief See documentation [here](\ref objs-trig-path)
 */
class ICTriggerPathProducer : public edm::EDProducer {
 public:
  explicit ICTriggerPathProducer(const edm::ParameterSet &);
  ~ICTriggerPathProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  void SetNameInfo(std::string name, ic::TriggerPath *path);

  std::vector<ic::TriggerPath> *paths_;
  edm::InputTag input_;
  std::string branch_;
  bool include_if_fired_;
  bool save_strings_;
  bool split_version_;
  bool input_is_standalone_;
  edm::InputTag input_prescales_;

};

#endif
