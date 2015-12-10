#ifndef UserCode_ICHiggsTauTau_ICFillerStructs_h
#define UserCode_ICHiggsTauTau_ICFillerStructs_h

#include "DataFormats/Provenance/interface/ProductID.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

//These structs make sure the correct ID is passed to filler.insert(handle,...)
//when producing a ValueMap from an edm::View.
//In case the edm::View points to an edm::RefVector, passing a CollectionIDAndSize
//to filler.insert(handle,...) instead of the handle itself ensures the ValueMap
//is linked to the ID of the underlying collection, not the edm::RefVector
//For an example interpretation, see ICElectronConversionCalculator.cc

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

