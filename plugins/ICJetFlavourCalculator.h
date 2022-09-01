#ifndef UserCode_ICHiggsTauTau_ICJetFlavourCalculator_h
#define UserCode_ICHiggsTauTau_ICJetFlavourCalculator_h

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ICJetFlavourCalculator : public edm::one::EDProducer {
 public:
  explicit ICJetFlavourCalculator(const edm::ParameterSet &);
  ~ICJetFlavourCalculator();

 private:
  virtual void beginJob();
  //virtual void produce(edm::Event &, const edm::EventSetup &)
  void produce(edm::Event &, const edm::EventSetup &); override;
  virtual void endJob();

  edm::InputTag input_;
  edm::InputTag input_jet_flavour_;
};

#endif
