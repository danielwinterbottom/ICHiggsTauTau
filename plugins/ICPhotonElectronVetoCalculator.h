#ifndef UserCode_ICHiggsTauTau_ICPhotonElectronVetoCalculator_h
#define UserCode_ICHiggsTauTau_ICPhotonElectronVetoCalculator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

/**
 * @brief Produces an edm::ValueMap<bool> for the photon electron-veto
 *flag
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py PhotonVetoCalculator
 */
class ICPhotonElectronVetoCalculator : public edm::EDProducer {
 public:
  explicit ICPhotonElectronVetoCalculator(const edm::ParameterSet &);
  ~ICPhotonElectronVetoCalculator();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  edm::InputTag input_electrons_;
  edm::InputTag input_beamspot_;
  edm::InputTag input_conversions_;
};

#endif
