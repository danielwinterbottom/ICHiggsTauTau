#include "UserCode/ICHiggsTauTau/plugins/ICTrackProducer.hh"
#include <string>
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICTrackProducer::ICTrackProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::Track>>(input_);
  tracks_ = new std::vector<ic::Track>();
  PrintHeaderWithProduces(config, input_, branch_);
}

ICTrackProducer::~ICTrackProducer() { delete tracks_; }

void ICTrackProducer::produce(edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Track> > track_handle;
  event.getByLabel(input_, track_handle);

  // Prepare output collection
  tracks_->clear();
  tracks_->resize(track_handle->size(), ic::Track());

  for (unsigned i = 0; i < track_handle->size(); ++i) {
    reco::Track const& src = track_handle->at(i);
    ic::Track& dest = tracks_->at(i);
    dest.set_id(track_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_charge(src.charge());
    dest.set_vx(src.vx());
    dest.set_vy(src.vy());
    dest.set_vz(src.vz());
    dest.set_normalized_chi2(src.normalizedChi2());
    dest.set_hits(src.hitPattern().numberOfValidHits());
    dest.set_pixel_hits(src.hitPattern().numberOfValidPixelHits());
    dest.set_algorithm(src.algo());
    dest.set_pt_err(src.ptError());
    dest.set_quality(src.qualityMask());
#if CMSSW_MAJOR_VERSION >= 7 && CMSSW_MINOR_VERSION >= 2
    dest.set_hits_miss_inner(
        src.hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
#else
      dest.set_hits_miss_inner(src.trackerExpectedHitsInner().numberOfHits());
#endif
  }
}

void ICTrackProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &tracks_);
}

void ICTrackProducer::endJob() {}

DEFINE_FWK_MODULE(ICTrackProducer);
