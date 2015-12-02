#include "UserCode/ICHiggsTauTau/plugins/ICPhotonElectronVetoCalculator.h"
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
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICPhotonElectronVetoCalculator::ICPhotonElectronVetoCalculator(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_electrons_(config.getParameter<edm::InputTag>("electrons")),
      input_beamspot_(config.getParameter<edm::InputTag>("beamspot")),
      input_conversions_(config.getParameter<edm::InputTag>("conversions")) {
  consumes<reco::PhotonCollection>(input_);
  consumes<reco::GsfElectronCollection>(input_electrons_);
  consumes<reco::BeamSpot>(input_beamspot_);
  consumes<reco::ConversionCollection>(input_conversions_);
  produces<edm::ValueMap<bool> >();
}

ICPhotonElectronVetoCalculator::~ICPhotonElectronVetoCalculator() {}

void ICPhotonElectronVetoCalculator::produce(edm::Event& event,
                                             const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<bool> > product(new edm::ValueMap<bool>());

  edm::Handle<reco::PhotonCollection> photons_handle;
  event.getByLabel(input_, photons_handle);

  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel(input_electrons_, elecs_handle);

  edm::Handle<reco::BeamSpot> beamspot_handle;
  event.getByLabel(input_beamspot_, beamspot_handle);

  edm::Handle<reco::ConversionCollection> conversions_handle;
  event.getByLabel(input_conversions_, conversions_handle);

  std::vector<bool> values(photons_handle->size(), false);
  for (unsigned i = 0; i < photons_handle->size(); ++i) {
    reco::Photon const& src = photons_handle->at(i);
    values[i] = !ConversionTools::hasMatchedPromptElectron(
                     src.superCluster(), elecs_handle, conversions_handle,
                     beamspot_handle->position());
  }

  edm::ValueMap<bool>::Filler filler(*product);
  filler.insert(photons_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICPhotonElectronVetoCalculator::beginJob() {}

void ICPhotonElectronVetoCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPhotonElectronVetoCalculator);
