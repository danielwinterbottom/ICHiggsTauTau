#ifndef UserCode_ICHiggsTauTau_ICL1EtMissProducer_h
#define UserCode_ICHiggsTauTau_ICL1EtMissProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

class ICL1EtMissProducer : public edm::EDProducer {
 public:
  explicit ICL1EtMissProducer(const edm::ParameterSet &);
  ~ICL1EtMissProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::Candidate> *candidates_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<l1extra::L1EtMissParticle const*> l1_hasher_;
};

#endif
