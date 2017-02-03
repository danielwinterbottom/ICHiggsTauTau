#include "UserCode/ICHiggsTauTau/plugins/ICVertexProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICVertexProducer::ICVertexProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      first_only_(config.getParameter<bool>("firstVertexOnly")),
      track_pt_threshold_(config.getParameter<double>("trackPtThreshold")),
      request_trks_(config.getParameter<bool>("requestTracks")) {
  consumes<edm::View<reco::Vertex>>(input_);
  vertices_ = new std::vector<ic::Vertex>();
  if (request_trks_) {
    produces<reco::TrackRefVector>("requestedTracks");
  }
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, first_only_, "firstVertexOnly");
  PrintOptional(1, request_trks_, "requestTracks");
}

ICVertexProducer::~ICVertexProducer() { delete vertices_; }

void ICVertexProducer::produce(edm::Event& event,
                               const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Vertex> > vtxs_handle;
  event.getByLabel(input_, vtxs_handle);

  std::unique_ptr<reco::TrackRefVector> trk_requests(new reco::TrackRefVector());

  vertices_->clear();
  if (!first_only_) {
    vertices_->resize(vtxs_handle->size());
  } else {
    vertices_->resize(vtxs_handle->size() > 0 ? 1 : 0);
  }

  for (unsigned i = 0; i < vtxs_handle->size(); ++i) {
    reco::Vertex const& src = vtxs_handle->at(i);
    ic::Vertex & dest = vertices_->at(i);
    dest.set_id(vertex_hasher_(&src));
    dest.set_vx(src.x());
    dest.set_vy(src.y());
    dest.set_vz(src.z());
    dest.set_chi2(src.chi2());
    dest.set_ndof(src.ndof());
    if (request_trks_) {
      for (reco::Vertex::trackRef_iterator trk_iter = src.tracks_begin();
           trk_iter != src.tracks_end(); ++trk_iter) {
        float weight = src.trackWeight(*trk_iter);
        reco::TrackRef trk_ref = trk_iter->castTo<reco::TrackRef>();
        if (trk_ref->pt() < track_pt_threshold_) continue;
        dest.AddTrack(track_hasher_(&(*trk_ref)), weight);
        trk_requests->push_back(trk_ref);
      }
    }
    if (first_only_) break;
  }
  if (request_trks_) event.put(std::move(trk_requests), "requestedTracks");
}

void ICVertexProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &vertices_);
}

void ICVertexProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICVertexProducer);
