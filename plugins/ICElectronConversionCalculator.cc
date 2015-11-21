#include "UserCode/ICHiggsTauTau/plugins/ICElectronConversionCalculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "UserCode/ICHiggsTauTau/plugins/ICFillerStructs.hh"

ICElectronConversionCalculator::ICElectronConversionCalculator(
    const edm::ParameterSet& config)
    : input_((config.getParameter<edm::InputTag>("input"))),
      input_beamspot_((config.getParameter<edm::InputTag>("beamspot"))),
      input_conversions_((config.getParameter<edm::InputTag>("conversions"))) {
      consumes<edm::View<reco::GsfElectron>>(input_);
      consumes<reco::BeamSpot>(input_beamspot_);
      consumes<reco::ConversionCollection>(input_conversions_);
  produces<edm::ValueMap<bool> >();
}

ICElectronConversionCalculator::~ICElectronConversionCalculator() {}

void ICElectronConversionCalculator::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<bool> > product(new edm::ValueMap<bool>());
  // Use an edm::View here so that this will work on a reco::GsfElectron or
  // pat::Electron collection
  edm::Handle<edm::View<reco::GsfElectron> > elecs_handle;
  event.getByLabel(input_, elecs_handle);

  edm::Handle<reco::BeamSpot> beamspot_handle;
  event.getByLabel(input_beamspot_, beamspot_handle);

  edm::Handle<reco::ConversionCollection> conversions_handle;
  event.getByLabel(input_conversions_, conversions_handle);

  // Have to be careful producing a ValueMap from an edm::View. When we call
  // filler.insert(handle,...) the ValueMap will use the ProductID of the handle
  // to create the map keys. If the View points to a concrete collection (i.e.
  // vector<T>) then this ID will correctly refer to this collection. However if
  // the View points to an edm::RefVector<T> we will get the ID of the RefVector
  // not the underlying concrete collection the RefVector points to. Then later
  // when we go to retrieve values from the map we will get a product not found
  // error. The solution is to get a Ref via the View `refAt` method, and use
  // this to create an edm::RefToBaseProd, which will always carry the ID of the
  // concrete collection and can be used in place of the handle in
  // filler.insert(...).
  // 14/11 - as RefToBaseProd doesn't exist in CMSSW_7_6_X anymore,
  // need a different solution. Unfortunately filler.insert(handle,...) calls 
  // handle.id() and handle->size() hence the ridiculous double struct construction - 
  // the first struct stores the size of the RefVector, the second a pointer to the first
  // plus the id of the underlying collection, so that indeed filler.insert can call both
  // .id() and ->size() 


  CollectionSize<reco::GsfElectron> csize;
  CollectionSize<reco::GsfElectron>* csize_ptr;
  CollectionIDAndSize<reco::GsfElectron> c_id_size;
  unsigned n=0;
  if(elecs_handle->size()>0){
    csize.handle_ = elecs_handle;
    csize_ptr = &csize;
    c_id_size.collsizepointer = csize_ptr;
    n=elecs_handle->size();
  }
    std::vector<bool> values(n,false);

  for (unsigned i = 0; i < n; ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    values[i] = ConversionTools::hasMatchedConversion(
        src, conversions_handle, beamspot_handle->position(),
        true, 2.0, 1e-6, 0);
  }

  edm::ValueMap<bool>::Filler filler(*product);
  // We need to pass a CollectionIDAndSize here, only exists
  // if the collection has at least one object (see above)
  if (n > 0) {
    filler.insert(c_id_size, values.begin(), values.end());
    filler.fill();
  }

  event.put(product);
}

void ICElectronConversionCalculator::beginJob() {}

void ICElectronConversionCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronConversionCalculator);
