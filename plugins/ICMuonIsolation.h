#ifndef UserCode_ICHiggsTauTau_ICMuonIsolation_h
#define UserCode_ICHiggsTauTau_ICMuonIsolation_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ICMuonIsolation : public edm::EDProducer {
 public:
  explicit ICMuonIsolation(const edm::ParameterSet &);
  ~ICMuonIsolation();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  std::string input_isolation_type_;
};

#endif
