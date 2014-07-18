#ifndef UserCode_ICHiggsTauTau_ICPhotonHadTowerOverEmCalculator_h
#define UserCode_ICHiggsTauTau_ICPhotonHadTowerOverEmCalculator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ICPhotonHadTowerOverEmCalculator : public edm::EDProducer {
 public:
  explicit ICPhotonHadTowerOverEmCalculator(const edm::ParameterSet &);
  ~ICPhotonHadTowerOverEmCalculator();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
};

#endif
