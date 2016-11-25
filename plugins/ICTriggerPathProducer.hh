#ifndef UserCode_ICHiggsTauTau_ICTriggerPathProducer_h
#define UserCode_ICHiggsTauTau_ICTriggerPathProducer_h

#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

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
  virtual void beginRun(edm::Run const& run, edm::EventSetup const& es);
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
  std::string hlt_process_;
  bool prescale_fallback_;
  std::map<std::string, std::size_t> observed_paths_;

  HLTConfigProvider hlt_config_;
};

#endif
