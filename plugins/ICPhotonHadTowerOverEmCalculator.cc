#include "UserCode/ICHiggsTauTau/plugins/ICPhotonHadTowerOverEmCalculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

ICPhotonHadTowerOverEmCalculator::ICPhotonHadTowerOverEmCalculator(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")) {
  produces<edm::ValueMap<float> >();
}

ICPhotonHadTowerOverEmCalculator::~ICPhotonHadTowerOverEmCalculator() {}

void ICPhotonHadTowerOverEmCalculator::produce(edm::Event& event,
                                             const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<float> > product(new edm::ValueMap<float>());

  edm::Handle<reco::PhotonCollection> photons_handle;
  event.getByLabel(input_, photons_handle);

  std::vector<float> values(photons_handle->size(), 0.);
  for (unsigned i = 0; i < photons_handle->size(); ++i) {
    reco::Photon const& src = photons_handle->at(i);
    values[i] = src.hadTowOverEm();
  }

  edm::ValueMap<float>::Filler filler(*product);
  filler.insert(photons_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICPhotonHadTowerOverEmCalculator::beginJob() {}

void ICPhotonHadTowerOverEmCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPhotonHadTowerOverEmCalculator);
