#ifndef UserCode_ICHiggsTauTau_ICFillerStructs_h
#define UserCode_ICHiggsTauTau_ICFillerStructs_h

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

  template <typename T> 
  struct CollectionSize{
     edm::Handle<edm::View<T> > handle_;
     size_t size() const {return handle_->size();};
  };

  template <typename T>
  struct CollectionIDAndSize{
     CollectionSize<T>* collsizepointer;
     edm::ProductID id() const {return collsizepointer->handle_->refAt(0).id();};
     CollectionSize<T>* operator->() const {
       return collsizepointer;
     }
   }; 

#endif

