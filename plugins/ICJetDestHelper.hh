#ifndef UserCode_ICHiggsTauTau_ICJetDestHelper_h
#define UserCode_ICHiggsTauTau_ICJetDestHelper_h

#include <memory>
#include "boost/functional/hash.hpp"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
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
  void DoSetup(edm::stream::EDProducer<> * prod) {}
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

  void DoSetup(edm::stream::EDProducer<> * prod) {
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

  void DoSetup(edm::stream::EDProducer<> * prod) {
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

  explicit JetDestHelper(const edm::ParameterSet &pset, edm::ConsumesCollector && collector)
      : do_pu_id(pset.getParameter<bool>("includePileupID")),
        input_pu_id(pset.getParameter<edm::InputTag>("inputPileupID")),
        do_trk_vars(pset.getParameter<bool>("includeTrackBasedVars")),
        input_trks(pset.getParameter<edm::InputTag>("inputTracks")),
        input_vtxs(pset.getParameter<edm::InputTag>("inputVertices")),
        request_trks(pset.getParameter<bool>("requestTracks")) {
          collector.consumes<edm::ValueMap<float>>(input_pu_id);
          collector.consumes<reco::TrackCollection>(input_trks);
          collector.consumes<reco::VertexCollection>(input_vtxs);
        }

  void DoSetup(edm::stream::EDProducer<> * prod) {
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

#endif
