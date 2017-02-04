#ifndef UserCode_ICHiggsTauTau_ICJetDestHelper_h
#define UserCode_ICHiggsTauTau_ICJetDestHelper_h

#include <memory>
#include "boost/functional/hash.hpp"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

template <class U>
struct JetDestHelper {
  explicit JetDestHelper(const edm::ParameterSet &pset, edm::ConsumesCollector && collector) {}
  void DoSetup(edm::EDProducer * prod) {}
  ~JetDestHelper() {}
};

template <>
struct JetDestHelper<ic::CaloJet> {
  bool do_jet_id;
  edm::InputTag input_jet_id;
  JetIDSelectionFunctor *loose_id;
  JetIDSelectionFunctor *tight_id;
  bool do_n_carrying;

  explicit JetDestHelper(const edm::ParameterSet &pset,edm::ConsumesCollector && collector)
      : do_jet_id(pset.getParameter<bool>("includeJetID")),
        input_jet_id(pset.getParameter<edm::InputTag>("inputJetID")),
        loose_id(NULL),
        tight_id(NULL),
        do_n_carrying(pset.getParameter<bool>("includeTowerCounts")) {
    collector.consumes<reco::JetIDValueMap>(input_jet_id);
    loose_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::LOOSE);
    tight_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::TIGHT);
  }

  void DoSetup(edm::EDProducer * prod) {
    std::cout << "CaloJet specific options:\n";
    PrintOptional(1, do_jet_id, "includeJetID");
    PrintOptional(1, do_n_carrying, "includeTowerCounts");
  }

  ~JetDestHelper() {
    delete loose_id;
    delete tight_id;
  }
};

template <>
struct JetDestHelper<ic::JPTJet> {
  bool do_jet_id;
  edm::InputTag input_jet_id;
  JetIDSelectionFunctor *loose_id;
  JetIDSelectionFunctor *tight_id;
  bool request_trks;
  boost::hash<reco::Track const*> track_hasher;
  bool do_trk_vars;
  edm::InputTag input_trks;
  edm::InputTag input_vtxs;
  bool do_n_carrying;

  explicit JetDestHelper(const edm::ParameterSet &pset, edm::ConsumesCollector && collector)
      : do_jet_id(pset.getParameter<bool>("includeJetID")),
        input_jet_id(pset.getParameter<edm::InputTag>("inputJetID")),
        loose_id(NULL),
        tight_id(NULL),
        request_trks(pset.getParameter<bool>("requestTracks")),
        do_trk_vars(pset.getParameter<bool>("includeTrackBasedVars")),
        input_trks(pset.getParameter<edm::InputTag>("inputTracks")),
        input_vtxs(pset.getParameter<edm::InputTag>("inputVertices")),
        do_n_carrying(pset.getParameter<bool>("includeTowerCounts")) {
    collector.consumes<edm::ValueMap<float>>(input_jet_id);
    collector.consumes<reco::TrackCollection>(input_trks);
    collector.consumes<reco::VertexCollection>(input_vtxs);
    loose_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::LOOSE);
    tight_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::TIGHT);
  }

  void DoSetup(edm::EDProducer * prod) {
    if (request_trks) {
      prod->produces<reco::TrackRefVector>("requestedTracks");
    }
    std::cout << "JPTJet specific options:\n";
    PrintOptional(1, do_jet_id, "includeJetID");
    PrintOptional(1, do_n_carrying, "includeTowerCounts");
    PrintOptional(1, request_trks, "requestTracks");
    PrintOptional(1, do_trk_vars, "includeTrackBasedVars");
  }

  std::vector<std::size_t> DoTrackCollection(
      reco::TrackRefVector const &ref_vec, reco::TrackRefVector *target) {
    std::vector<std::size_t> res(ref_vec.size(), 0);
    for (unsigned i = 0; i < ref_vec.size(); ++i) {
      res[i] = track_hasher(&(*(ref_vec[i])));
      target->push_back(ref_vec[i]);
    }
    return res;
  }

  void AddTrackCollection(reco::TrackRefVector const &ref_vec,
                          std::vector<reco::Track const *> *target) {
    for (unsigned i = 0; i < ref_vec.size(); ++i) {
      target->push_back(&(*(ref_vec[i])));
    }
  }

  ~JetDestHelper() {
    delete loose_id;
    delete tight_id;
  }
};

template <>
struct JetDestHelper<ic::PFJet> {
  bool do_pu_id;
  edm::InputTag input_pu_id;
  bool do_trk_vars;
  edm::InputTag input_trks;
  edm::InputTag input_vtxs;
  bool request_trks;
  boost::hash<reco::Track const*> track_hasher;
//  edm::InputTag input_pfcands;

  explicit JetDestHelper(const edm::ParameterSet &pset, edm::ConsumesCollector && collector)
      : do_pu_id(pset.getParameter<bool>("includePileupID")),
        input_pu_id(pset.getParameter<edm::InputTag>("inputPileupID")),
        do_trk_vars(pset.getParameter<bool>("includeTrackBasedVars")),
        input_trks(pset.getParameter<edm::InputTag>("inputTracks")),
        input_vtxs(pset.getParameter<edm::InputTag>("inputVertices")),
        request_trks(pset.getParameter<bool>("requestTracks")) {
//        input_pfcands(pset.getParameter<edm::InputTag>("pfcands")) {
          collector.consumes<edm::ValueMap<float>>(input_pu_id);
          collector.consumes<reco::TrackCollection>(input_trks);
          collector.consumes<reco::VertexCollection>(input_vtxs);
//          collector.consumes<std::vector<pat::PackedCandidate>>(input_pfcands);
        }

  void DoSetup(edm::EDProducer * prod) {
    if (request_trks) {
      prod->produces<reco::TrackRefVector>("requestedTracks");
    }
    std::cout << "PFJet specific options:\n";
    PrintOptional(1, do_pu_id, "includePileupID");
    PrintOptional(1, request_trks, "requestTracks");
    PrintOptional(1, do_trk_vars, "includeTrackBasedVars");
  }

  ~JetDestHelper() {}
};

template<class U>
void FillCommonPFJet(ic::PFJet *dest, U const& src) {
  dest->set_muon_multiplicity(src.muonMultiplicity());
  dest->set_charged_multiplicity(src.chargedMultiplicity());
  dest->set_charged_had_multiplicity(src.chargedHadronMultiplicity());
  dest->set_neutral_had_multiplicity(src.neutralHadronMultiplicity());
  dest->set_photon_multiplicity(src.photonMultiplicity());
  dest->set_electron_multiplicity(src.electronMultiplicity());
  dest->set_HF_had_multiplicity(src.HFHadronMultiplicity());
  dest->set_HF_em_multiplicity(src.HFEMMultiplicity());
  dest->set_neutral_multiplicity(src.neutralMultiplicity());
  dest->set_charged_em_energy(src.chargedEmEnergy());
  dest->set_neutral_em_energy(src.neutralEmEnergy());
  dest->set_charged_had_energy(src.chargedHadronEnergy());
  dest->set_neutral_had_energy(src.neutralHadronEnergy());
  dest->set_photon_energy(src.photonEnergy());
  dest->set_electron_energy(src.electronEnergy());
  dest->set_muon_energy(src.muonEnergy());
  dest->set_HF_had_energy(src.HFHadronEnergy());
  dest->set_HF_em_energy(src.HFEMEnergy());
  dest->set_charged_mu_energy(src.chargedMuEnergy());
}

template<class U>
void FillCommonCaloJet(ic::CaloJet *dest, U const& src) {
  dest->set_max_em_tower_energy(src.maxEInEmTowers());
  dest->set_max_had_tower_energy(src.maxEInHadTowers());
  dest->set_energy_frac_had(src.energyFractionHadronic());
  dest->set_energy_frac_em(src.emEnergyFraction());
  dest->set_had_energy_HB(src.hadEnergyInHB());
  dest->set_had_energy_HO(src.hadEnergyInHO());
  dest->set_had_energy_HE(src.hadEnergyInHE());
  dest->set_had_energy_HF(src.hadEnergyInHF());
  dest->set_em_energy_EB(src.emEnergyInEB());
  dest->set_em_energy_EE(src.emEnergyInEE());
  dest->set_em_energy_HF(src.emEnergyInHF());
  dest->set_towers_area(src.towersArea());
}

template<class U>
void FillCommonJPTJet(ic::JPTJet *dest, U const& src) {
  dest->set_muon_multiplicity(src.muonMultiplicity());
  dest->set_charged_multiplicity(src.chargedMultiplicity());
  dest->set_charged_em_energy(src.chargedEmEnergy());
  dest->set_neutral_em_energy(src.neutralEmEnergy());
  dest->set_charged_had_energy(src.chargedHadronEnergy());
  dest->set_neutral_had_energy(src.neutralHadronEnergy());
}


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

#endif
