#ifndef UserCode_ICHiggsTauTau_ICElectronProducer_h
#define UserCode_ICHiggsTauTau_ICElectronProducer_h

#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/**
 * @brief Handles the creation of the ntuple output, and must always be included
 *after the other IC object producers.
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py Event
 */
class ICEventProducer : public edm::EDProducer {
 public:
  explicit ICEventProducer(const edm::ParameterSet&);
  ~ICEventProducer();

 private:
  unsigned processed_;
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();
};

#endif
