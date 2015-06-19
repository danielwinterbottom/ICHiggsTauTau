#ifndef UserCode_ICHiggsTauTau_ICElectronIsolation_h
#define UserCode_ICHiggsTauTau_ICElectronIsolation_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ICElectronIsolation : public edm::EDProducer {
 public:
  explicit ICElectronIsolation(const edm::ParameterSet &);
  ~ICElectronIsolation();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  std::string input_isolation_type_;
};

#endif
