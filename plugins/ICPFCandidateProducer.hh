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
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

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
};

// =============================
// Template class implementation
// =============================
template <class T>
ICPFCandidateProducer<T>::ICPFCandidateProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      request_trks_(config.getParameter<bool>("requestTracks")),
      request_gsf_trks_(config.getParameter<bool>("requestGsfTracks")),
      track_input_(config.getParameter<edm::InputTag>("inputUnpackedTracks")) {
  consumes<edm::View<T>>(input_);
  consumes<edm::View<reco::Track>>(track_input_);
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

  std::unique_ptr<reco::TrackRefVector> trk_requests(new reco::TrackRefVector());
  std::unique_ptr<reco::GsfTrackRefVector> gsf_trk_requests(new reco::GsfTrackRefVector());

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
  if (request_trks_) event.put(std::move(trk_requests), "requestedTracks");
  if (request_gsf_trks_) event.put(std::move(gsf_trk_requests), "requestedGsfTracks");
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
    reco::GsfTrackRefVector* /*gsf_trk_requests*/, edm::Event& event,
    const edm::EventSetup& setup) {
  // To get tracks here we'd actually have to produce a new track collection
  // first using pseudoTrack(), get it into the event, then produce the refs.
  // Not trivial I think.

  edm::Handle<edm::View<reco::Track> > tracks_handle;
  typedef std::set<reco::Track const*> TrackSet;
  typedef std::map<reco::Track const*, unsigned> TrackMap;
  TrackSet trk_set;
  TrackMap trk_map;

  if(request_trks_){
    event.getByLabel(track_input_, tracks_handle);
    for (unsigned i = 0; i < tracks_handle->size(); ++i) {
      trk_set.insert(&(tracks_handle->at(i)));
      trk_map[&(tracks_handle->at(i))] = i;
    }
  }

  for (unsigned i = 0; i < cands_handle->size(); ++i) {
    pat::PackedCandidate const& src = cands_handle->at(i);
    ic::PFCandidate& dest = cands_->at(i);
    // PackedCandidates don't store the same set of flags as PFCandidates, but
    // they do store their own set of flags that we can get via the status()
    // method, so let's just save this here. For details see:
    // twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
    dest.set_flags(src.status());
    std::vector<std::size_t> trk_ids;
    if (request_trks_ && src.charge() != 0) {
      reco::Track pseudo = src.pseudoTrack();
      for (TrackSet::iterator it = trk_set.begin(); it != trk_set.end(); ++it) {
        // Ok, comparing floats normally not a good idea, but here they really
        // should be identical
        if (pseudo.momentum().x() != (*it)->momentum().x() ||
            pseudo.momentum().y() != (*it)->momentum().y() ||
            pseudo.momentum().z() != (*it)->momentum().z()) continue;
        trk_set.erase(it);
        trk_requests->push_back(tracks_handle->refAt(trk_map.at(*it))
                                    .template castTo<reco::TrackRef>());
        trk_ids.push_back(track_hasher_(*it));
        break;
      }
      // PhysicsTools/PatAlgos/plugins/TrackAndVertexUnpacker.cc doesn't create
      // a Track for every charged PackedCandidate, just those that meet the
      // conditions below.

      // UNCOMMENT TO CHECK IF WE'RE MISSING ANY TRACKS THAT SHOULD BE THERE
      // if (src.charge() != 0 && src.numberOfHits() > 0 && trk_ids.size() == 0) {
      //   std::cout << "Have an unmatched track!\n";
      // }
      dest.set_constituent_tracks(trk_ids);
    }

    // NB: can't do anything for gsf_trk_requests!
  }
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
