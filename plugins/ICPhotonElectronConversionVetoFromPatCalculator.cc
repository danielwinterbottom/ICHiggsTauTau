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
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICPhotonElectronConversionVetoFromPatCalculator::ICPhotonElectronConversionVetoFromPatCalculator(
									     const edm::ParameterSet& config)
  : input_(config.getParameter<edm::InputTag>("input")) {
  consumes<pat::PhotonCollection>(input_);
  produces<edm::ValueMap<bool> >();
}

ICPhotonElectronConversionVetoFromPatCalculator::~ICPhotonElectronConversionVetoFromPatCalculator() {}

void ICPhotonElectronConversionVetoFromPatCalculator::produce(edm::Event& event,
						    const edm::EventSetup& setup) {
  std::unique_ptr<edm::ValueMap<bool> > product(new edm::ValueMap<bool>());
  edm::Handle<pat::PhotonCollection> photons_handle;
  event.getByLabel(input_, photons_handle);
  
  std::vector<bool> values(photons_handle->size(), 0.);
  for (unsigned i = 0; i < photons_handle->size(); ++i) {
#if CMSSW_MAJOR_VERSION >= 7 && CMSSW_MINOR_VERSION >= 0
    pat::Photon const& src = photons_handle->at(i);
    values[i] = src.passElectronVeto();
#else
    throw cms::Exception("IncompatibleCMSSWVersion") << "This module should not be used before CMSSW_7_0_X!!";
#endif
  }

  edm::ValueMap<bool>::Filler filler(*product);
  filler.insert(photons_handle, values.begin(), values.end());
  filler.fill();

  event.put(std::move(product));
}

void ICPhotonElectronConversionVetoFromPatCalculator::beginJob() {}

void ICPhotonElectronConversionVetoFromPatCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPhotonElectronConversionVetoFromPatCalculator);
