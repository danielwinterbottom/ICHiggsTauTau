#include "UserCode/ICHiggsTauTau/plugins/ICHttElecIsoCheck.h"
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
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICHttElecIsoCheck::ICHttElecIsoCheck(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_pf_charged_(config.getParameter<edm::InputTag>("pfChargedAll")) {
  consumes<reco::GsfElectronCollection>(input_);
  consumes<edm::View<reco::PFCandidate>>(input_pf_charged_);
  produces<edm::ValueMap<float> >();
}

ICHttElecIsoCheck::~ICHttElecIsoCheck() {}

void ICHttElecIsoCheck::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<float> > product(new edm::ValueMap<float>());
  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);
  edm::Handle<edm::View<reco::PFCandidate> > pf_handle;
  event.getByLabel(input_pf_charged_, pf_handle);

  std::vector<float> values(elecs_handle->size(), 0.);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    values[i] = 0.;
    for (unsigned pf = 0; pf < pf_handle->size(); ++pf) {
      reco::PFCandidate const& pfcand = pf_handle->at(pf);
      bool barrel = fabs(src.superCluster()->eta()) < 1.479;
      double dr_veto = barrel ? 0.01 : 0.015;
      if (reco::deltaR(src, pfcand) < dr_veto ||
          reco::deltaR(src, pfcand) > 0.4)
        continue;
      if (pfcand.trackRef().isNonnull()) {  // PF candidate has a track
        if (pfcand.trackRef().index() == src.closestCtfTrackRef().index()) {
          values[i] = pfcand.pt();
        }
      }
    }
  }

  edm::ValueMap<float>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICHttElecIsoCheck::beginJob() {}

void ICHttElecIsoCheck::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICHttElecIsoCheck);
