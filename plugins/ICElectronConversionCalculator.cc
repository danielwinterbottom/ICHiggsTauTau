#include "UserCode/ICHiggsTauTau/plugins/ICElectronConversionCalculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

ICElectronConversionCalculator::ICElectronConversionCalculator(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_beamspot_(config.getParameter<edm::InputTag>("beamspot")),
      input_conversions_(config.getParameter<edm::InputTag>("conversions")) {
  produces<edm::ValueMap<bool> >();
}

ICElectronConversionCalculator::~ICElectronConversionCalculator() {}

void ICElectronConversionCalculator::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<bool> > product(new edm::ValueMap<bool>());
  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);

  edm::Handle<reco::BeamSpot> beamspot_handle;
  event.getByLabel(input_beamspot_, beamspot_handle);

  edm::Handle<reco::ConversionCollection> conversions_handle;
  event.getByLabel(input_conversions_, conversions_handle);

  std::vector<bool> values(elecs_handle->size(), 0.);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    values[i] = ConversionTools::hasMatchedConversion(
        src, conversions_handle, beamspot_handle->position(),
        true, 2.0, 1e-6, 0);
  }

  edm::ValueMap<bool>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICElectronConversionCalculator::beginJob() {}

void ICElectronConversionCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronConversionCalculator);
