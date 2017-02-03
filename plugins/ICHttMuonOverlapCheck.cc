#include "UserCode/ICHiggsTauTau/plugins/ICHttMuonOverlapCheck.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICHttMuonOverlapCheck::ICHttMuonOverlapCheck(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_muons_(config.getParameter<edm::InputTag>("muons")) {
  consumes<reco::GsfElectronCollection>(input_);
  consumes<edm::View<reco::Muon>>(input_muons_);
  produces<edm::ValueMap<float> >();
}

ICHttMuonOverlapCheck::~ICHttMuonOverlapCheck() {}

void ICHttMuonOverlapCheck::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::unique_ptr<edm::ValueMap<float> > product(new edm::ValueMap<float>());
  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);
  edm::Handle<edm::View<reco::Muon> > muons_handle;
  event.getByLabel(input_muons_, muons_handle);

  std::vector<float> values(elecs_handle->size(), 0.);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    values[i] = 0.;
    for (unsigned mu = 0; mu < muons_handle->size(); ++mu) {
      if (muons_handle->at(mu).pt() > 3.) {
        if (reco::deltaR(muons_handle->at(mu), src) < 0.3) {
          values[i] = 1.;
          break;
        }
      }
    }
  }

  edm::ValueMap<float>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(std::move(product));
}

void ICHttMuonOverlapCheck::beginJob() {}

void ICHttMuonOverlapCheck::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICHttMuonOverlapCheck);
