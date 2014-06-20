#ifndef UserCode_ICHiggsTauTau_ICElectronR9Calculator_h
#define UserCode_ICHiggsTauTau_ICElectronR9Calculator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ICElectronR9Calculator : public edm::EDProducer {
 public:
  explicit ICElectronR9Calculator(const edm::ParameterSet &);
  ~ICElectronR9Calculator();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
};

#endif
