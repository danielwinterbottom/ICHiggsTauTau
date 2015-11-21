#include "UserCode/ICHiggsTauTau/plugins/ICLightTrackProducer.hh"
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

ICLightTrackProducer::ICLightTrackProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::Track>>(input_);
  tracks_ = new std::vector<ic::LightTrack>();
  PrintHeaderWithProduces(config, input_, branch_);
}

ICLightTrackProducer::~ICLightTrackProducer() { delete tracks_; }

void ICLightTrackProducer::produce(edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Track> > track_handle;
  event.getByLabel(input_, track_handle);

  // Prepare output collection
  tracks_->clear();
  tracks_->resize(track_handle->size(), ic::LightTrack());

  for (unsigned i = 0; i < track_handle->size(); ++i) {
    reco::Track const& src = track_handle->at(i);
    ic::LightTrack& dest = tracks_->at(i);
    dest.set_id(track_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_vz(src.vz());
  }
}

void ICLightTrackProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &tracks_);
}

void ICLightTrackProducer::endJob() {}

DEFINE_FWK_MODULE(ICLightTrackProducer);

