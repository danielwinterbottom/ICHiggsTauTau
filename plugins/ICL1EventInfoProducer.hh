#ifndef UserCode_ICHiggsTauTau_ICL1EventInfoProducer_h
#define UserCode_ICHiggsTauTau_ICL1EventInfoProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

/**
 * @brief Produces an ic::EventInfo object
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py EventInfo
 */
class ICL1EventInfoProducer : public edm::EDProducer {
 public:
  explicit ICL1EventInfoProducer(const edm::ParameterSet &);
  ~ICL1EventInfoProducer();

 private:
  virtual void beginJob();
  virtual void endRun(edm::Run const& run, edm::EventSetup const& es);
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  ic::EventInfo *info_;
  std::string branch_;

};

#endif
