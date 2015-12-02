#include "UserCode/ICHiggsTauTau/plugins/ICSecondaryVertexProducer.hh"
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
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/SecondaryVertex.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICSecondaryVertexProducer::ICSecondaryVertexProducer(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      track_pt_threshold_(config.getParameter<double>("trackPtThreshold")),
      request_trks_(config.getParameter<bool>("requestTracks")) {
  consumes<edm::View<reco::SecondaryVertexTagInfo>>(input_);
  vertices_ = new std::vector<ic::SecondaryVertex>();
  if (request_trks_) {
    produces<reco::TrackRefVector>("requestedTracks");
  }
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, request_trks_, "requestTracks");
}

ICSecondaryVertexProducer::~ICSecondaryVertexProducer() { delete vertices_; }

void ICSecondaryVertexProducer::produce(edm::Event& event,
                                        const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::SecondaryVertexTagInfo> > vtxs_handle;
  event.getByLabel(input_, vtxs_handle);

  std::auto_ptr<reco::TrackRefVector> trk_requests(new reco::TrackRefVector());

  vertices_->clear();

  for (unsigned i = 0; i < vtxs_handle->size(); ++i) {
    reco::SecondaryVertexTagInfo const& sv_src = vtxs_handle->at(i);
    for (unsigned j = 0; j < sv_src.nVertices(); ++j) {
      reco::Vertex const& src = sv_src.secondaryVertex(j);
      vertices_->push_back(ic::SecondaryVertex());
      ic::SecondaryVertex & dest = vertices_->back();
      dest.set_id(vertex_hasher_(&src));
      dest.set_vx(src.x());
      dest.set_vy(src.y());
      dest.set_vz(src.z());
      dest.set_chi2(src.chi2());
      dest.set_ndof(src.ndof());
      dest.set_distance_3d(sv_src.flightDistance(j, true).value());
      dest.set_distance_err_3d(sv_src.flightDistance(j, true).error());
      for (reco::Vertex::trackRef_iterator trk_iter = src.tracks_begin();
           trk_iter != src.tracks_end(); ++trk_iter) {
        float weight = src.trackWeight(*trk_iter);
        reco::TrackRef trk_ref = trk_iter->castTo<reco::TrackRef>();
        if (trk_ref->pt() < track_pt_threshold_) continue;
        dest.AddTrack(track_hasher_(&(*trk_ref)), weight);
        trk_requests->push_back(trk_ref);
      }
    }
  }
  event.put(trk_requests, "requestedTracks");
}

void ICSecondaryVertexProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &vertices_);
}

void ICSecondaryVertexProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICSecondaryVertexProducer);
