#ifndef UserCode_ICHiggsTauTau_ICPFCandidateProducer_h
#define UserCode_ICHiggsTauTau_ICPFCandidateProducer_h

#include <memory>
#include <vector>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#if CMSSW_MAJOR_VERSION >= 7
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#endif
// #include "DataFormats/VertexReco/interface/Vertex.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

/**
 * @brief See documentation [here](\ref objs-pf-candidate)
 */
template <class T>
class ICPFCandidateProducer : public edm::EDProducer {
 public:
  explicit ICPFCandidateProducer(const edm::ParameterSet &);
  ~ICPFCandidateProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<T> > const& cands_handle,
                         reco::TrackRefVector* trk_requests,
                         reco::GsfTrackRefVector* gsf_trk_requests,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<ic::PFCandidate> *cands_;
  edm::InputTag input_;
  std::string branch_;
  bool request_trks_;
  bool request_gsf_trks_;
  boost::hash<T const*> cand_hasher_;
  boost::hash<reco::Track const*> track_hasher_;
  edm::InputTag track_input_;
  edm::InputTag gsf_track_input_;
};

// =============================
// Template class implementation
// =============================
template <class T>
ICPFCandidateProducer<T>::ICPFCandidateProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      request_trks_(config.getParameter<bool>("requestTracks")),
      request_gsf_trks_(config.getParameter<bool>("requestGsfTracks")) {
  cands_ = new std::vector<ic::PFCandidate>();

  if (request_trks_) {
    produces<reco::TrackRefVector>("requestedTracks");
  }

  if (request_gsf_trks_) {
    produces<reco::GsfTrackRefVector>("requestedGsfTracks");
  }

  PrintHeaderWithProduces(config, input_, branch_);
  // PrintOptional(1, do_vertex_ip_, "includeVertexIP");
  PrintOptional(1, request_trks_, "requestTracks");
  PrintOptional(1, request_trks_, "requestGsfTracks");
}

template <class T>
ICPFCandidateProducer<T>::~ICPFCandidateProducer() { delete cands_; }

// =============
// Main producer
// =============
template <class T>
void ICPFCandidateProducer<T>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  edm::Handle<edm::View<T> > cands_handle;
  event.getByLabel(input_, cands_handle);

  // edm::Handle<edm::View<reco::Vertex> > vertices_handle;
  // if (do_vertex_ip_) event.getByLabel(input_vertices_, vertices_handle);

  std::auto_ptr<reco::TrackRefVector> trk_requests(new reco::TrackRefVector());
  std::auto_ptr<reco::GsfTrackRefVector> gsf_trk_requests(new reco::GsfTrackRefVector());

  cands_->clear();
  cands_->resize(cands_handle->size(), ic::PFCandidate());
  for (unsigned i = 0; i < cands_handle->size(); ++i) {
    T const& src = cands_handle->at(i);
    ic::PFCandidate& dest = cands_->at(i);

    dest.set_id(cand_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_pdgid(src.pdgId());

    // dest.set_vx(src.vx());
    // dest.set_vy(src.vy());
    // dest.set_vz(src.vz());
  }
  constructSpecific(cands_handle, trk_requests.get(), gsf_trk_requests.get(),
                    event, setup);
  if (request_trks_) event.put(trk_requests, "requestedTracks");
  if (request_gsf_trks_) event.put(gsf_trk_requests, "requestedGsfTracks");
}

// ==================
// Specific producers
// ==================
template <class T>
void ICPFCandidateProducer<T>::constructSpecific(
    edm::Handle<edm::View<T> > const& cands_handle,
    reco::TrackRefVector* trk_requests,
    reco::GsfTrackRefVector* gsf_trk_requests, edm::Event& event,
    const edm::EventSetup& setup) {}

template <>
void ICPFCandidateProducer<reco::PFCandidate>::constructSpecific(
    edm::Handle<edm::View<reco::PFCandidate> > const& cands_handle,
    reco::TrackRefVector* trk_requests,
    reco::GsfTrackRefVector* gsf_trk_requests, edm::Event& event,
    const edm::EventSetup& setup) {
  for (unsigned i = 0; i < cands_handle->size(); ++i) {
    reco::PFCandidate const& src = cands_handle->at(i);
    ic::PFCandidate& dest = cands_->at(i);
    unsigned flag_val = 0;
    for (int i = ic::PFFlag::NORMAL; i <= ic::PFFlag::GAMMA_TO_GAMMACONV; ++i) {
      if (src.flag(reco::PFCandidate::Flags(i))) flag_val = flag_val | (1 << i);
    }
    dest.set_flags(flag_val);
    std::vector<std::size_t> trk_ids;
    if (request_trks_) {
      if (src.trackRef().isNonnull()) {
        reco::TrackRef const& trk = src.trackRef();
        trk_requests->push_back(trk);
        trk_ids.push_back(track_hasher_(&(*trk)));
      }

      reco::VertexCompositeCandidateRef v0Ref = src.v0Ref();
      if (v0Ref.isNonnull()) {
        for (unsigned ndx = 0; ndx < v0Ref->numberOfDaughters(); ndx++) {
          reco::TrackRef trk = (dynamic_cast<const reco::RecoChargedCandidate*>(
                                 v0Ref->daughter(ndx)))->track();
          // std::cout << "Found V0 track!\n";
          trk_requests->push_back(trk);
          trk_ids.push_back(track_hasher_(&(*trk)));
        }
      }
      reco::ConversionRef convRef = src.conversionRef();
      if (convRef.isNonnull()) {
        std::vector<edm::RefToBase<reco::Track> > const & trks = convRef->tracks();
        for (unsigned ndx = 0; ndx < trks.size(); ++ndx) {
          reco::TrackRef trk(trks[ndx].castTo<reco::TrackRef>());
          // std::cout << "Found conversion track!\n";
          trk_requests->push_back(trk);
          trk_ids.push_back(track_hasher_(&(*trk)));
        }
      }

      dest.set_constituent_tracks(trk_ids);
    }

    if (request_gsf_trks_) {
      std::vector<std::size_t> gsf_trk_ids;
      if (src.gsfTrackRef().isNonnull()) {
        reco::GsfTrackRef const& trk = src.gsfTrackRef();
        gsf_trk_requests->push_back(trk);
        gsf_trk_ids.push_back(track_hasher_(&(*trk)));
      }
      dest.set_constituent_gsf_tracks(gsf_trk_ids);
    }


  }
}

#if CMSSW_MAJOR_VERSION >= 7
template <>
void ICPFCandidateProducer<pat::PackedCandidate>::constructSpecific(
    edm::Handle<edm::View<pat::PackedCandidate> > const& cands_handle,
    reco::TrackRefVector* trk_requests,
    reco::GsfTrackRefVector* gsf_trk_requests, edm::Event& event,
    const edm::EventSetup& setup) {
  // To get tracks here we'd actually have to produce a new track collection
  // first using pseudoTrack(), get it into the event, then produce the refs.
  // Not trivial I think.
}
#endif

template <class T>
void ICPFCandidateProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &cands_);
}

template <class T>
void ICPFCandidateProducer<T>::endJob() {
}

#endif
