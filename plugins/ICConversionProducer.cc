#include "UserCode/ICHiggsTauTau/plugins/ICConversionProducer.hh"
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
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICConversionProducer::ICConversionProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      tracks_branch_(config.getParameter<std::string>("tracksBranch")),
      request_trks_(config.getParameter<bool>("saveTracks")) {
  consumes<edm::View<reco::Conversion>>(input_);
  vertices_ = new std::vector<ic::Vertex>();
  tracks_ = new std::vector<ic::Track>();
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, request_trks_, "saveTracks");
}

ICConversionProducer::~ICConversionProducer() { delete vertices_; }

void ICConversionProducer::produce(edm::Event& event,
                               const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Conversion> > convs_handle;
  event.getByLabel(input_, convs_handle);


  vertices_->clear();
  vertices_->resize(convs_handle->size());
  tracks_->clear();

  for (unsigned i = 0; i < convs_handle->size(); ++i) {
    reco::Conversion const& src = convs_handle->at(i);
    ic::Vertex & dest = vertices_->at(i);
    dest.set_id(conversion_hasher_(&src));
    dest.set_vx(src.conversionVertex().x());
    dest.set_vy(src.conversionVertex().y());
    dest.set_vz(src.conversionVertex().z());
    dest.set_chi2(src.conversionVertex().chi2());
    dest.set_ndof(src.conversionVertex().ndof());
    if (request_trks_) {
      for (reco::Vertex::trackRef_iterator trk_iter = src.tracks().begin();
           trk_iter != src.tracks().end(); ++trk_iter) {
        float weight = 0.;
        reco::TrackBase const* trk = trk_iter->get();
        dest.AddTrack(track_hasher_(trk), weight);
        tracks_->push_back(ic::Track());
        ic::Track & dtrk = tracks_->back();
        dtrk.set_id(track_hasher_(trk));
        dtrk.set_pt(trk->pt());
        dtrk.set_eta(trk->eta());
        dtrk.set_phi(trk->phi());
        dtrk.set_charge(trk->charge());
        dtrk.set_vx(trk->vx());
        dtrk.set_vy(trk->vy());
        dtrk.set_vz(trk->vz());
        dtrk.set_normalized_chi2(trk->normalizedChi2());
        dtrk.set_hits(trk->hitPattern().numberOfValidHits());
        dtrk.set_pixel_hits(trk->hitPattern().numberOfValidPixelHits());
        dtrk.set_algorithm(trk->algo());
        dtrk.set_pt_err(trk->ptError());
        dtrk.set_quality(trk->qualityMask());
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 2)
        dtrk.set_hits_miss_inner(
            trk->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
#else
        dtrk.set_hits_miss_inner(trk->trackerExpectedHitsInner().numberOfHits());
#endif
      }
    }
  }
}

void ICConversionProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &vertices_);
  if (request_trks_) ic::StaticTree::tree_->Branch(tracks_branch_.c_str(), &tracks_);
}

void ICConversionProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICConversionProducer);
