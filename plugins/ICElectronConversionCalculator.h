#ifndef UserCode_ICHiggsTauTau_ICElectronConversionCalculator_h
#define UserCode_ICHiggsTauTau_ICElectronConversionCalculator_h

#include "DataFormats/Provenance/interface/ProductID.h"
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

  struct CollectionSize{
     edm::Handle<edm::View<reco::GsfElectron> > handle_;
     size_t size() const {return handle_->size();};
  };

  struct CollectionIDAndSize{
     CollectionSize* collsizepointer;
     edm::ProductID id() const {return collsizepointer->handle_->refAt(0).id();};
     CollectionSize* operator->() const {
       return collsizepointer;
     }
   }; 

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();



//  edm::InputTag input_;
  edm::InputTag input_; 
  edm::InputTag input_beamspot_;
  edm::InputTag input_conversions_;
};

#endif
