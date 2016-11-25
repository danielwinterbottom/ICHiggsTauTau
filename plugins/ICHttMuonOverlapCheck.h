#ifndef UserCode_ICHiggsTauTau_ICHttMuonOverlapCheck_h
#define UserCode_ICHiggsTauTau_ICHttMuonOverlapCheck_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ICHttMuonOverlapCheck : public edm::EDProducer {
 public:
  explicit ICHttMuonOverlapCheck(const edm::ParameterSet &);
  ~ICHttMuonOverlapCheck();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  edm::InputTag input_muons_;
};

#endif
