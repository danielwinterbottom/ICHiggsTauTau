#ifndef UserCode_ICHiggsTauTau_ICElectronConversionFromPatCalculator_h
#define UserCode_ICHiggsTauTau_ICElectronConversionFromPatCalculator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

/**
 * @brief Produces an edm::ValueMap<bool> for the electron conversion-rejection
 *flag. The flag is extracted directly from the input PAT candidate 
 *
 */
class ICElectronConversionFromPatCalculator : public edm::EDProducer {
 public:
  explicit ICElectronConversionFromPatCalculator(const edm::ParameterSet &);
  ~ICElectronConversionFromPatCalculator();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
};

#endif
