#include "UserCode/ICHiggsTauTau/plugins/ICPhotonElectronConversionVetoFromPatCalculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

ICPhotonElectronConversionVetoFromPatCalculator::ICPhotonElectronConversionVetoFromPatCalculator(
									     const edm::ParameterSet& config)
  : input_(config.getParameter<edm::InputTag>("input")) {
  produces<edm::ValueMap<bool> >();
}

ICPhotonElectronConversionVetoFromPatCalculator::~ICPhotonElectronConversionVetoFromPatCalculator() {}

void ICPhotonElectronConversionVetoFromPatCalculator::produce(edm::Event& event,
						    const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<bool> > product(new edm::ValueMap<bool>());
  edm::Handle<pat::PhotonCollection> photons_handle;
  event.getByLabel(input_, photons_handle);
  
  std::vector<bool> values(photons_handle->size(), 0.);
  for (unsigned i = 0; i < photons_handle->size(); ++i) {
    pat::Photon const& src = photons_handle->at(i);
    // We want true = is matched to a conversion, so flip the logic
    values[i] = src.passElectronVeto();
  }

  edm::ValueMap<bool>::Filler filler(*product);
  filler.insert(photons_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICPhotonElectronConversionVetoFromPatCalculator::beginJob() {}

void ICPhotonElectronConversionVetoFromPatCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPhotonElectronConversionVetoFromPatCalculator);
