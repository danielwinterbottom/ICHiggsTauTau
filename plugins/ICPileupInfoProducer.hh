#ifndef UserCode_ICHiggsTauTau_ICPileupInfoProducer_h
#define UserCode_ICHiggsTauTau_ICPileupInfoProducer_h
#include <memory>
#include <vector>
#include <string>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"

class ICPileupInfoProducer : public edm::EDProducer {
 public:
  explicit ICPileupInfoProducer(const edm::ParameterSet&);
  ~ICPileupInfoProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::PileupInfo> *info_;
  edm::InputTag input_;
  std::string branch_;
};

#endif
