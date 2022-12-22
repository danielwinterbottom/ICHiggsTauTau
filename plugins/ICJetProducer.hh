#ifndef UserCode_ICHiggsTauTau_ICJetProducer_h
#define UserCode_ICHiggsTauTau_ICJetProducer_h

#include <memory>
#include <typeinfo>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "UserCode/ICHiggsTauTau/plugins/ICJetSrcHelper.hh"
#include "UserCode/ICHiggsTauTau/plugins/ICJetDestHelper.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

/**
 * @brief See documentation [here](\ref objs-jet)
 *
 * @tparam T The ic::Jet type to produce, e.g. ic::Jet, ic::PFJet, ic::CaloJet
 *or ic::JPTJet
 * @tparam U The input jet type, e.g. reco::CaloJet, reco::PFJet or pat::Jet
 */
template <class T, class U>
class ICJetProducer : public edm::one::EDProducer<> {
 public:
  explicit ICJetProducer(const edm::ParameterSet &);
  ~ICJetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<U> > const& jets_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<T> *jets_;
  edm::InputTag input_;
  std::string branch_;

  std::vector<unsigned> passed_;

  JetSrcHelper<U> src_;
  JetDestHelper<T> dest_;
};

//////
std::map<unsigned, unsigned> BuildTrkVtxMap(
    edm::Handle<reco::TrackCollection> const &trk_handle,
    edm::Handle<reco::VertexCollection> const &vtx_handle) {
  std::map<unsigned, unsigned> trk_vtx_map;
  if (trk_handle->size() > 0) {
    reco::Track const *ptr_to_first = &(trk_handle->at(0));
    for (unsigned i = 0; i < vtx_handle->size(); ++i) {
      std::vector<reco::TrackBaseRef>::const_iterator trk_it;
      for (trk_it = vtx_handle->at(i).tracks_begin();
           trk_it != vtx_handle->at(i).tracks_end(); ++trk_it) {
        reco::Track const *ptr_to_trk =
            dynamic_cast<reco::Track const *>(&(**trk_it));
        unsigned trk_idx = (unsigned(ptr_to_trk - ptr_to_first));
        trk_vtx_map[trk_idx] = i;
      }
    }
  }
  return trk_vtx_map;
}

void FillPFTracksAndRequest(ic::PFJet *dest,
                            std::vector<reco::PFCandidatePtr> const &pfcands,
                            reco::TrackRefVector *track_requests,
                            boost::hash<reco::Track const *> *track_hasher) {
  std::vector<std::size_t> track_ids;
  for (unsigned iP = 0; iP < pfcands.size(); ++iP) {
    if (pfcands[iP]->trackRef().isNonnull()) {
      track_requests->push_back(pfcands[iP]->trackRef());
      track_ids.push_back((*track_hasher)(&(*(pfcands[iP]->trackRef()))));
    }
  }
  dest->set_constituent_tracks(track_ids);
}

void FillJPTTrackVars(ic::JPTJet *dest,
                      std::vector<reco::Track const *> const &all_tracks,
                      edm::Handle<reco::TrackCollection> const &trk_handle,
                      edm::Handle<reco::VertexCollection> const &vtx_handle,
                      std::map<unsigned, unsigned> const &trk_vtx_map) {
  std::vector<double> pt_at_vtx_vec(vtx_handle->size(), 0.0);
  reco::Track const *ptr_first = &(trk_handle->at(0));
  double trk_pt_total = 0.0;
  for (unsigned j = 0; j < all_tracks.size(); ++j) {
    unsigned idx = unsigned(all_tracks[j] - ptr_first);
    trk_pt_total += all_tracks[j]->pt();
    // Is track associated to a vertex?
    if (trk_vtx_map.count(idx) > 0) {
      pt_at_vtx_vec[trk_vtx_map.find(idx)->second] += all_tracks[j]->pt();
    } else {  // No, so is it within 0.2 cm wrt the closest vertex in z
      std::vector<double> dz_with_vtx(vtx_handle->size(), 0.0);
      for (unsigned k = 0; k < vtx_handle->size(); ++k) {
        dz_with_vtx[k] = fabs(vtx_handle->at(k).z() - all_tracks[j]->vz());
      }
      std::vector<double>::const_iterator min =
          std::min_element(dz_with_vtx.begin(), dz_with_vtx.end());
      if (min != dz_with_vtx.end()) {
        if (*min < 0.2) {
          pt_at_vtx_vec[unsigned(min - dz_with_vtx.begin())] +=
              all_tracks[j]->pt();
        }
      }
    }
  }
  if (vtx_handle->size() > 0 && trk_pt_total > 0.0) {
    for (unsigned j = 0; j < pt_at_vtx_vec.size(); ++j) {
      pt_at_vtx_vec[j] = pt_at_vtx_vec[j] / trk_pt_total;
    }
    dest->set_beta(pt_at_vtx_vec[0]);
    dest->set_beta_max(
        *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end()));
    dest->set_track_pt_total(trk_pt_total);
  } else {
    dest->set_beta(-1.);
    dest->set_beta_max(-1.);
    dest->set_track_pt_total(0);
  }
}

void FillPFTrackVars(ic::PFJet *dest,
                     std::vector<reco::PFCandidatePtr> const &pfcands,
                     edm::Handle<reco::TrackCollection> const &trk_handle,
                     edm::Handle<reco::VertexCollection> const &vtx_handle,
                     std::map<unsigned, unsigned> const &trk_vtx_map) {
  int charged_multiplicity_nopu = 0;
  float linear_radial_moment_nopu = 0.0;
  double trk_pt_total = 0.0;
  std::vector<double> pt_at_vtx_vec(vtx_handle->size(), 0.0);
  reco::Track const* ptr_first = &(trk_handle->at(0));
  for (unsigned iP = 0; iP < pfcands.size(); ++iP) {
    if (pfcands[iP]->trackRef().isNonnull()) {
      unsigned idx = unsigned(&(*(pfcands[iP]->trackRef())) - ptr_first);
      trk_pt_total += pfcands[iP]->trackRef()->pt();
      // Is track associated to a vertex?
      if (trk_vtx_map.count(idx) > 0) {
        // If this track is mapped to the first PV, increase number of nopu
        // charged multiplicity
        if (trk_vtx_map.find(idx)->second == 0) {
          ++charged_multiplicity_nopu;
          linear_radial_moment_nopu +=
              pfcands[iP]->pt() * deltaR(dest->vector().Rapidity(), dest->phi(),
                                         pfcands[iP]->rapidity(),
                                         pfcands[iP]->phi());
        }
        pt_at_vtx_vec[trk_vtx_map.find(idx)->second] +=
            pfcands[iP]->trackRef()->pt();
      } else {  // No, so is it within 0.2 cm wrt the closest vertex in z
        std::vector<double> dz_with_vtx(vtx_handle->size(), 0.0);
        for (unsigned iV = 0; iV < vtx_handle->size(); ++iV) {
          dz_with_vtx[iV] =
              fabs(vtx_handle->at(iV).z() - pfcands[iP]->trackRef()->vz());
        }
        std::vector<double>::const_iterator min =
            std::min_element(dz_with_vtx.begin(), dz_with_vtx.end());
        if (min != dz_with_vtx.end()) {
          if (*min < 0.2) {
            // As above, if this track is not associated to any vertex, but
            // is within 0.2 cm of first PV, then increase multiplicity
            // count
            if (unsigned(min-dz_with_vtx.begin()) == 0) {
              ++charged_multiplicity_nopu;
              linear_radial_moment_nopu +=
                  pfcands[iP]->pt() * deltaR(dest->vector().Rapidity(), dest->phi(),
                                             pfcands[iP]->rapidity(),
                                             pfcands[iP]->phi());
            }
            pt_at_vtx_vec[unsigned(min - dz_with_vtx.begin())] +=
                pfcands[iP]->trackRef()->pt();
          }
        }
      }
    } else {  // No track found, assume PF candidate is not PU
      linear_radial_moment_nopu +=
          pfcands[iP]->pt() * deltaR(dest->vector().Rapidity(), dest->phi(),
                                     pfcands[iP]->rapidity(),
                                     pfcands[iP]->phi());
    }
  }
  // IMPORTANT: taking the dest pt() here not the src()!
  linear_radial_moment_nopu = linear_radial_moment_nopu / dest->pt();
  dest->set_linear_radial_moment(linear_radial_moment_nopu);
  dest->set_charged_multiplicity_nopu(charged_multiplicity_nopu);
  // jet.set_constituent_tracks(trks);
  if (vtx_handle->size() > 0 && trk_pt_total > 0.0) {
    for (unsigned iV = 0; iV < pt_at_vtx_vec.size(); ++iV) {
      pt_at_vtx_vec[iV] = pt_at_vtx_vec[iV] / trk_pt_total;
    }
    dest->set_beta(pt_at_vtx_vec[0]);
    dest->set_beta_max(
        *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end()));
  } else {
    dest->set_beta(-1.0);
    dest->set_beta_max(-1.0);
  }
}
////

// =============================
// Template class implementation
// =============================
template <class T, class U>
ICJetProducer<T, U>::ICJetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      src_(config.getParameterSet("srcConfig"),consumesCollector(), producesCollector()),
      dest_(config.getParameterSet("destConfig"),consumesCollector(), producesCollector()) {
  consumes<edm::View<U>>(input_);
  jets_ = new std::vector<T>();


  PrintHeaderWithProduces(config, input_, branch_);

  src_.DoSetup();
  dest_.DoSetup();
}


template <class T, class U>
ICJetProducer<T, U>::~ICJetProducer() { delete jets_; }


// =============
// Main producer
// =============
template <class T, class U>
void ICJetProducer<T, U>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  
  edm::Handle<edm::View<U> > jets_handle;
  event.getByLabel(input_, jets_handle);

  jets_->clear();
  passed_.clear();
  jets_->reserve(jets_handle->size());
  passed_.reserve(jets_handle->size());

  src_.Produce(jets_handle, jets_, &passed_, event, setup);

  constructSpecific(jets_handle, event, setup);
}


// =============================
// Specific producers
// =============================
template <class T, class U>
void ICJetProducer<T, U>::constructSpecific(
    edm::Handle<edm::View<U> > const& jets_handle, edm::Event& event,
    const edm::EventSetup& setup) {}


// =============================
// reco::CaloJet --> ic::CaloJet
// =============================
template <>
void ICJetProducer<ic::CaloJet, reco::CaloJet>::constructSpecific(
    edm::Handle<edm::View<reco::CaloJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {
  //}

  edm::Handle<reco::JetIDValueMap> jet_id_handle;
  if (dest_.do_jet_id) event.getByLabel(dest_.input_jet_id, jet_id_handle);
  for (unsigned i = 0; i < passed_.size(); ++i) {
    reco::CaloJet const& src = jets_handle->at(passed_[i]);
    ic::CaloJet & dest = jets_->at(i);
    FillCommonCaloJet(&dest, src);
    if (dest_.do_n_carrying) {
      dest.set_n90(src.n90());
      dest.set_n60(src.n60());
    }
    //  Assume input jet is uncorrected
    dest.set_uncorrected_energy(src.energy());
    if (dest_.do_jet_id) {
      reco::JetID const& jet_id =
          (*jet_id_handle)[jets_handle->refAt(passed_[i])];
      dest.set_fHPD(jet_id.fHPD);
      dest.set_fRBX(jet_id.fRBX);
      dest.set_id_loose((*(dest_.loose_id))(src, jet_id));
      dest.set_id_tight((*(dest_.tight_id))(src, jet_id));
    }
  }
}

// =============================
// pat::Jet --> ic::CaloJet
// =============================
template <>
void ICJetProducer<ic::CaloJet, pat::Jet>::constructSpecific(
    edm::Handle<edm::View<pat::Jet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {

  for (unsigned i = 0; i < passed_.size(); ++i) {
    pat::Jet const& src = jets_handle->at(passed_[i]);
    ic::CaloJet & dest = jets_->at(i);
    FillCommonCaloJet(&dest, src);
    if (dest_.do_n_carrying) {
      dest.set_n90(src.n90());
      dest.set_n60(src.n60());
    }

    dest.set_uncorrected_energy(
        (src.jecSetsAvailable() ? src.jecFactor(0) : 1.) * src.energy());
    if (dest_.do_jet_id) {
      dest.set_fHPD(src.jetID().fHPD);
      dest.set_fRBX(src.jetID().fRBX);
      dest.set_id_loose((*(dest_.loose_id))(src));
      dest.set_id_tight((*(dest_.tight_id))(src));
    }
  }
}

// =============================
// reco::JPTJet --> ic::JPTJet
// =============================
template <>
void ICJetProducer<ic::JPTJet, reco::JPTJet>::constructSpecific(
    edm::Handle<edm::View<reco::JPTJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {

  edm::Handle<reco::JetIDValueMap> jet_id_handle;
  if (dest_.do_jet_id) event.getByLabel(dest_.input_jet_id, jet_id_handle);

  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::unique_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (dest_.do_trk_vars) {
    event.getByLabel(dest_.input_trks, trk_handle);
    event.getByLabel(dest_.input_vtxs, vtx_handle);
    trk_vtx_map = BuildTrkVtxMap(trk_handle, vtx_handle);
  }

  for (unsigned i = 0; i < passed_.size(); ++i) {
    reco::JPTJet const& src = jets_handle->at(passed_[i]);

    reco::CaloJetRef calo(src.getCaloJetRef().castTo<reco::CaloJetRef>());
    ic::JPTJet & dest = jets_->at(i);
    FillCommonCaloJet(&dest, *calo);
    if (dest_.do_n_carrying) {
      dest.set_n90(calo->n90());
      dest.set_n60(calo->n60());
    }
    FillCommonJPTJet(&dest, src);
    //  Assume input jet is uncorrected
    dest.set_uncorrected_energy(src.energy());
    if (dest_.do_jet_id) {
      reco::JetID const& jet_id = (*jet_id_handle)[calo];
      dest.set_id_loose((*(dest_.loose_id))(*calo, jet_id));
      dest.set_id_tight((*(dest_.tight_id))(*calo, jet_id));
    }

    if (dest_.request_trks) {
      dest.set_pions_in_vtx_in_calo(dest_.DoTrackCollection(
          src.getPionsInVertexInCalo(), track_requests.get()));
      dest.set_pions_in_vtx_out_calo(dest_.DoTrackCollection(
          src.getPionsInVertexOutCalo(), track_requests.get()));
      dest.set_pions_out_vtx_in_calo(dest_.DoTrackCollection(
          src.getPionsOutVertexInCalo(), track_requests.get()));
      dest.set_muons_in_vtx_in_calo(dest_.DoTrackCollection(
          src.getMuonsInVertexInCalo(), track_requests.get()));
      dest.set_muons_in_vtx_out_calo(dest_.DoTrackCollection(
          src.getMuonsInVertexOutCalo(), track_requests.get()));
      dest.set_muons_out_vtx_in_calo(dest_.DoTrackCollection(
          src.getMuonsOutVertexInCalo(), track_requests.get()));
      dest.set_elecs_in_vtx_in_calo(dest_.DoTrackCollection(
          src.getElecsInVertexInCalo(), track_requests.get()));
      dest.set_elecs_in_vtx_out_calo(dest_.DoTrackCollection(
          src.getElecsInVertexOutCalo(), track_requests.get()));
      dest.set_elecs_out_vtx_in_calo(dest_.DoTrackCollection(
          src.getElecsOutVertexInCalo(), track_requests.get()));
    }

    if (dest_.do_trk_vars && trk_handle->size() > 0) {
      std::vector<reco::Track const*> all_tracks;
      dest_.AddTrackCollection(src.getPionsInVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getPionsInVertexOutCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getPionsOutVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getMuonsInVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getMuonsInVertexOutCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getMuonsOutVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getElecsInVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getElecsInVertexOutCalo(), &all_tracks);
      dest_.AddTrackCollection(src.getElecsOutVertexInCalo(), &all_tracks);
      FillJPTTrackVars(&dest, all_tracks, trk_handle, vtx_handle, trk_vtx_map);
    }
  }
  if (dest_.request_trks) {
    event.put(std::move(track_requests), "requestedTracks");
  }
}

// =============================
// pat::Jet --> ic::JPTJet
// =============================
template <>
void ICJetProducer<ic::JPTJet, pat::Jet>::constructSpecific(
    edm::Handle<edm::View<pat::Jet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {

  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::unique_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (dest_.do_trk_vars) {
    event.getByLabel(dest_.input_trks, trk_handle);
    event.getByLabel(dest_.input_vtxs, vtx_handle);
    trk_vtx_map = BuildTrkVtxMap(trk_handle, vtx_handle);
  }

  for (unsigned i = 0; i < passed_.size(); ++i) {
    pat::Jet const& src = jets_handle->at(passed_[i]);
    ic::JPTJet & dest = jets_->at(i);
    FillCommonCaloJet(&dest, src);
    if (dest_.do_n_carrying) {
      dest.set_n90(src.n90());
      dest.set_n60(src.n60());
    }
    FillCommonJPTJet(&dest, src);
    dest.set_uncorrected_energy(
        (src.jecSetsAvailable() ? src.jecFactor(0) : 1.) * src.energy());
    if (dest_.do_jet_id) {
      dest.set_fHPD(src.jetID().fHPD);
      dest.set_fRBX(src.jetID().fRBX);
      dest.set_id_loose((*(dest_.loose_id))(src));
      dest.set_id_tight((*(dest_.tight_id))(src));
    }

    if (dest_.request_trks) {
      dest.set_pions_in_vtx_in_calo(dest_.DoTrackCollection(
          src.pionsInVertexInCalo(), track_requests.get()));
      dest.set_pions_in_vtx_out_calo(dest_.DoTrackCollection(
          src.pionsInVertexOutCalo(), track_requests.get()));
      dest.set_pions_out_vtx_in_calo(dest_.DoTrackCollection(
          src.pionsOutVertexInCalo(), track_requests.get()));
      dest.set_muons_in_vtx_in_calo(dest_.DoTrackCollection(
          src.muonsInVertexInCalo(), track_requests.get()));
      dest.set_muons_in_vtx_out_calo(dest_.DoTrackCollection(
          src.muonsInVertexOutCalo(), track_requests.get()));
      dest.set_muons_out_vtx_in_calo(dest_.DoTrackCollection(
          src.muonsOutVertexInCalo(), track_requests.get()));
      dest.set_elecs_in_vtx_in_calo(dest_.DoTrackCollection(
          src.elecsInVertexInCalo(), track_requests.get()));
      dest.set_elecs_in_vtx_out_calo(dest_.DoTrackCollection(
          src.elecsInVertexOutCalo(), track_requests.get()));
      dest.set_elecs_out_vtx_in_calo(dest_.DoTrackCollection(
          src.elecsOutVertexInCalo(), track_requests.get()));
    }

    if (dest_.do_trk_vars && trk_handle->size() > 0) {
      std::vector<reco::Track const*> all_tracks;
      dest_.AddTrackCollection(src.pionsInVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.pionsInVertexOutCalo(), &all_tracks);
      dest_.AddTrackCollection(src.pionsOutVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.muonsInVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.muonsInVertexOutCalo(), &all_tracks);
      dest_.AddTrackCollection(src.muonsOutVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.elecsInVertexInCalo(), &all_tracks);
      dest_.AddTrackCollection(src.elecsInVertexOutCalo(), &all_tracks);
      dest_.AddTrackCollection(src.elecsOutVertexInCalo(), &all_tracks);
      FillJPTTrackVars(&dest, all_tracks, trk_handle, vtx_handle, trk_vtx_map);
    }
  }
  if (dest_.request_trks) {
    event.put(std::move(track_requests), "requestedTracks");
  }
}

// =============================
// reco::PFJet --> ic::PFJet
// =============================
template <>
void ICJetProducer<ic::PFJet, reco::PFJet>::constructSpecific(
    edm::Handle<edm::View<reco::PFJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {

  edm::Handle<edm::ValueMap<float> > pu_id_handle;
  if (dest_.do_pu_id) event.getByLabel(dest_.input_pu_id, pu_id_handle);

  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::unique_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (dest_.do_trk_vars) {
    event.getByLabel(dest_.input_trks, trk_handle);
    event.getByLabel(dest_.input_vtxs, vtx_handle);
    trk_vtx_map = BuildTrkVtxMap(trk_handle, vtx_handle);
  }

  for (unsigned i = 0; i < passed_.size(); ++i) {
    reco::PFJet const& src = jets_handle->at(passed_[i]);
    ic::PFJet & dest = jets_->at(i);
    FillCommonPFJet(&dest, src);
    //  Assume input jet is uncorrected
    dest.set_uncorrected_energy(src.energy());
    if (dest_.do_pu_id) {
      dest.set_pu_id_mva_value((*pu_id_handle)[jets_handle->refAt(passed_[i])]);
    }

    if (dest_.request_trks) {
      FillPFTracksAndRequest(&dest, src.getPFConstituents(),
                             track_requests.get(), &(dest_.track_hasher));
    }

    if (dest_.do_trk_vars && trk_handle->size() > 0) {
      if (dest_.do_trk_vars && trk_handle->size() > 0) {
        FillPFTrackVars(&dest, src.getPFConstituents(), trk_handle, vtx_handle,
                        trk_vtx_map);
      }
    }
  }
  if (dest_.request_trks) {
    event.put(std::move(track_requests), "requestedTracks");
  }
}

// =============================
// pat::Jet --> ic::PFJet
// =============================
template <>
void ICJetProducer<ic::PFJet, pat::Jet>::constructSpecific(
    edm::Handle<edm::View<pat::Jet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {

  edm::Handle<edm::ValueMap<float> > pu_id_handle;
  if (dest_.do_pu_id) event.getByLabel(dest_.input_pu_id, pu_id_handle);

  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::unique_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (dest_.do_trk_vars) {
    event.getByLabel(dest_.input_trks, trk_handle);
    event.getByLabel(dest_.input_vtxs, vtx_handle);
    trk_vtx_map = BuildTrkVtxMap(trk_handle, vtx_handle);
  }

  for (unsigned i = 0; i < passed_.size(); ++i) {
    pat::Jet const& src = jets_handle->at(passed_[i]);
    ic::PFJet & dest = jets_->at(i);
    FillCommonPFJet(&dest, src);
    dest.set_uncorrected_energy(
        (src.jecSetsAvailable() ? src.jecFactor(0) : 1.) * src.energy());
    if (dest_.do_pu_id) {
      if (src_.is_slimmed) {
        dest.set_pu_id_mva_value(src.userFloat(src_.slimmed_puid_label));
	//std::cout << " -- jet " << i << " pt " << dest.pt() << " pumva = " << dest.pu_id_mva_value() << std::endl;
      } else {
        dest.set_pu_id_mva_value(
            (*pu_id_handle)[jets_handle->refAt(passed_[i])->originalObjectRef()]);
      }
   }

    if (dest_.request_trks) {
      FillPFTracksAndRequest(&dest, src.getPFConstituents(),
                             track_requests.get(), &(dest_.track_hasher));
    }

    if (dest_.do_trk_vars && trk_handle->size() > 0) {
      FillPFTrackVars(&dest, src.getPFConstituents(), trk_handle, vtx_handle,
                      trk_vtx_map);
    }
  }
  if (dest_.request_trks) {
    event.put(std::move(track_requests), "requestedTracks");
  }
}

template <class T, class U>
void ICJetProducer<T, U>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &jets_);
}

template <class T, class U>
void ICJetProducer<T, U>::endJob() {
}


#endif
