#ifndef UserCode_ICHiggsTauTau_ICElectronConversionCalculator_h
#define UserCode_ICHiggsTauTau_ICElectronConversionCalculator_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
/**
 * @brief Produces an edm::ValueMap<bool> for the electron conversion-rejection
 *flag
 */
class ICElectronConversionCalculator : public edm::EDProducer {
 public:
  explicit ICElectronConversionCalculator(const edm::ParameterSet &);
  ~ICElectronConversionCalculator();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_; 
  edm::InputTag input_beamspot_;
  edm::InputTag input_conversions_;
};

#endif
