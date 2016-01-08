#ifndef UserCode_ICHiggsTauTau_ICTauProducer_h
#define UserCode_ICHiggsTauTau_ICTauProducer_h

#include <memory>
#include <vector>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#if CMSSW_MAJOR_VERSION>=7
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#endif
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

/**
 * @brief See documentation [here](\ref objs-tau)
 */
template <class T>
class ICTauProducer : public edm::EDProducer {
 public:
  explicit ICTauProducer(const edm::ParameterSet &);
  ~ICTauProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<T> > const& taus_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<ic::Tau> *taus_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<T const*> tau_hasher_;
  boost::hash<reco::Track const*> track_hasher_;
  edm::InputTag track_input_;
  std::vector<std::pair<std::string, edm::InputTag> > tau_ids_;
  edm::InputTag input_vertices_;
  bool do_vertex_ip_;
  bool request_trks_;
  std::map<std::string, std::size_t> observed_id_;
};

// =============================
// Template class implementation
// =============================
template <class T>
ICTauProducer<T>::ICTauProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_vertices_(config.getParameter<edm::InputTag>("inputVertices")),
      do_vertex_ip_(config.getParameter<bool>("includeVertexIP")),
      request_trks_(config.getParameter<bool>("requestTracks")) {
  consumes<edm::View<T>>(input_);
  consumes<edm::View<reco::Vertex>>(input_vertices_);
  taus_ = new std::vector<ic::Tau>();

  edm::ParameterSet pset = config.getParameter<edm::ParameterSet>("tauIDs");
  std::vector<std::string> vec = pset.getParameterNamesForType<edm::InputTag>();
  for (unsigned i = 0; i < vec.size(); ++i) {
    tau_ids_.push_back(
        std::make_pair(vec[i], pset.getParameter<edm::InputTag>(vec[i])));
        consumes<reco::PFTauDiscriminator>(tau_ids_[i].second);
  }

  if (request_trks_) {
    produces<reco::TrackRefVector>("requestedTracks");
  }

  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_vertex_ip_, "includeVertexIP");
  PrintOptional(1, request_trks_, "requestTracks");
}

template <class T>
ICTauProducer<T>::~ICTauProducer() { delete taus_; }

// =============
// Main producer
// =============
template <class T>
void ICTauProducer<T>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  edm::Handle<edm::View<T> > taus_handle;
  event.getByLabel(input_, taus_handle);

  edm::Handle<edm::View<reco::Vertex> > vertices_handle;
  if (do_vertex_ip_) event.getByLabel(input_vertices_, vertices_handle);

  std::auto_ptr<reco::TrackRefVector> trk_requests(new reco::TrackRefVector());

  taus_->clear();
  taus_->resize(taus_handle->size(), ic::Tau());
  for (unsigned i = 0; i < taus_handle->size(); ++i) {
    T const& src = taus_handle->at(i);
    ic::Tau& dest = taus_->at(i);

    dest.set_id(tau_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_decay_mode(src.decayMode());

    if (src.leadPFChargedHadrCand().isNonnull()) {
      dest.set_lead_ecal_energy(src.leadPFChargedHadrCand()->ecalEnergy());
      dest.set_lead_hcal_energy(src.leadPFChargedHadrCand()->hcalEnergy());
      dest.set_lead_p(src.leadPFChargedHadrCand()->p());
      reco::TrackRef const& trk = src.leadPFChargedHadrCand()->trackRef();
      reco::GsfTrackRef const& gsf = src.leadPFChargedHadrCand()->gsfTrackRef();
      if (do_vertex_ip_ && vertices_handle->size() > 0) {
        reco::Vertex const& vtx = vertices_handle->at(0);
        if (trk.isNonnull()) {
          dest.set_lead_dxy_vertex(trk->dxy(vtx.position()));
          dest.set_lead_dz_vertex(trk->dz(vtx.position()));
        } else if (gsf.isNonnull()) {
          dest.set_lead_dxy_vertex(gsf->dxy(vtx.position()));
          dest.set_lead_dz_vertex(gsf->dz(vtx.position()));
        }
      }
    }


    if (request_trks_) {
      std::vector<std::size_t> trk_ids;
      for (unsigned j = 0; j < src.signalPFChargedHadrCands().size(); ++j) {
        reco::TrackRef const& trk =
            src.signalPFChargedHadrCands().at(j)->trackRef();
        if (trk.isNonnull()) {
          trk_requests->push_back(trk);
          trk_ids.push_back(track_hasher_(&(*trk)));
        }
      }
      dest.set_constituent_tracks(trk_ids);
    }

    dest.set_vx(src.vx());
    dest.set_vy(src.vy());
    dest.set_vz(src.vz());
  }
  constructSpecific(taus_handle, event, setup);
  if (request_trks_) event.put(trk_requests, "requestedTracks");
}

// ==================
// Specific producers
// ==================
template <class T>
void ICTauProducer<T>::constructSpecific(
    edm::Handle<edm::View<T> > const& taus_handle, edm::Event& event,
    const edm::EventSetup& setup) {}

template <>
void ICTauProducer<reco::PFTau>::constructSpecific(
    edm::Handle<edm::View<reco::PFTau> > const& taus_handle, edm::Event& event,
    const edm::EventSetup& setup) {

  std::vector<edm::Handle<reco::PFTauDiscriminator> > id_handles(
      tau_ids_.size());
  for (unsigned i = 0; i < id_handles.size(); ++i) {
    event.getByLabel(tau_ids_[i].second, id_handles[i]);
  }

  for (unsigned i = 0; i < taus_handle->size(); ++i) {
     //reco::PFTau const& src = taus_handle->at(i);
    ic::Tau& dest = taus_->at(i);
    reco::PFTauRef const& ref = taus_handle->refAt(i).castTo<reco::PFTauRef>();
    for (unsigned j = 0; j < tau_ids_.size(); ++j) {
      dest.SetTauID(tau_ids_[j].first, (*(id_handles[j]))[ref]);
      observed_id_[tau_ids_[j].first] = CityHash64(tau_ids_[j].first);
    }
  }
}

template <>
void ICTauProducer<pat::Tau>::constructSpecific(
    edm::Handle<edm::View<pat::Tau> > const& taus_handle, edm::Event& event,
    const edm::EventSetup& setup) {
  for (unsigned i = 0; i < taus_handle->size(); ++i) {
    pat::Tau const& src = taus_handle->at(i);
    ic::Tau& dest = taus_->at(i);
    std::vector<std::pair<std::string, float> > tau_ids = src.tauIDs();
    for (unsigned j = 0; j < tau_ids.size(); ++j) {
      dest.SetTauID(tau_ids[j].first, tau_ids[j].second);
      observed_id_[tau_ids[j].first] = CityHash64(tau_ids[j].first);
    }
     //dest.set_lead_ecal_energy(src.leadChargedHadrCand()->ecalEnergy());
     //dest.set_lead_hcal_energy(src.leadChargedHadrCand()->hcalEnergy());
#if CMSSW_MAJOR_VERSION >= 7
    if(src.leadChargedHadrCand().isNonnull()){
      pat::PackedCandidate const* packedCand = dynamic_cast<pat::PackedCandidate const*>(src.leadChargedHadrCand().get());
      if(packedCand){
        dest.set_lead_dz_vertex(packedCand->dz());
        dest.set_lead_dxy_vertex(packedCand->dxy());
        dest.set_lead_p(packedCand->p());
      }
    }
#endif
  }
}

template <class T>
void ICTauProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &taus_);
}

template <class T>
void ICTauProducer<T>::endJob() {
  std::cout << std::string(78, '-') << "\n";
  std::cout << boost::format("%-56s  %20s\n")
      % std::string("Tau Discriminators") % std::string("Hash Summmary");
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_id_.begin(); iter != observed_id_.end(); ++iter) {
    std::cout << boost::format("%-56s| %020i\n") % iter->first % iter->second;
  }
}

#endif
