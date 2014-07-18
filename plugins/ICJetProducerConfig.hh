#ifndef UserCode_ICHiggsTauTau_ICJetProducerConfig_h
#define UserCode_ICHiggsTauTau_ICJetProducerConfig_h

#include <memory>
#include "boost/functional/hash.hpp"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
// #include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"

template <class U>
struct SpecificConfig {
  explicit SpecificConfig(const edm::ParameterSet &pset) {}
  void DoSetup(edm::EDProducer * prod) {}
  ~SpecificConfig() {}
};

template <>
struct SpecificConfig<reco::CaloJet> {
  bool do_jet_id;
  edm::InputTag input_jet_id;
  JetIDSelectionFunctor *loose_id;
  JetIDSelectionFunctor *tight_id;

  explicit SpecificConfig(const edm::ParameterSet &pset)
      : do_jet_id(pset.getParameter<bool>("includeJetID")),
        input_jet_id(pset.getParameter<edm::InputTag>("inputJetID")),
        loose_id(NULL),
        tight_id(NULL) {
    loose_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::LOOSE);
    tight_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::TIGHT);
  }

  void DoSetup(edm::EDProducer * prod) {}

  ~SpecificConfig() {
    delete loose_id;
    delete tight_id;
  }
};

template <>
struct SpecificConfig<reco::JPTJet> {
  bool do_jet_id;
  edm::InputTag input_jet_id;
  JetIDSelectionFunctor *loose_id;
  JetIDSelectionFunctor *tight_id;
  bool request_trks;
  boost::hash<reco::Track const*> track_hasher;
  bool do_trk_vars;
  edm::InputTag input_trks;
  edm::InputTag input_vtxs;

  explicit SpecificConfig(const edm::ParameterSet &pset)
      : do_jet_id(pset.getParameter<bool>("includeJetID")),
        input_jet_id(pset.getParameter<edm::InputTag>("inputJetID")),
        loose_id(NULL),
        tight_id(NULL),
        request_trks(pset.getParameter<bool>("requestTracks")),
        do_trk_vars(pset.getParameter<bool>("includeTrackBasedVars")),
        input_trks(pset.getParameter<edm::InputTag>("inputTracks")),
        input_vtxs(pset.getParameter<edm::InputTag>("inputVertices")) {
    loose_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::LOOSE);
    tight_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::TIGHT);
  }

  void DoSetup(edm::EDProducer * prod) {
    if (request_trks) {
      prod->produces<reco::TrackRefVector>("requestedTracks");
    }
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

  ~SpecificConfig() {
    delete loose_id;
    delete tight_id;
  }
};

template <>
struct SpecificConfig<reco::PFJet> {
  bool do_pu_id;
  edm::InputTag input_pu_id;
  bool do_trk_vars;
  edm::InputTag input_trks;
  edm::InputTag input_vtxs;
  bool request_trks;
  boost::hash<reco::Track const*> track_hasher;

  explicit SpecificConfig(const edm::ParameterSet &pset)
      : do_pu_id(pset.getParameter<bool>("includePileupID")),
        input_pu_id(pset.getParameter<edm::InputTag>("inputPileupID")),
        do_trk_vars(pset.getParameter<bool>("includeTrackBasedVars")),
        input_trks(pset.getParameter<edm::InputTag>("inputTracks")),
        input_vtxs(pset.getParameter<edm::InputTag>("inputVertices")),
        request_trks(pset.getParameter<bool>("requestTracks")) {}

  void DoSetup(edm::EDProducer * prod) {
    if (request_trks) {
      prod->produces<reco::TrackRefVector>("requestedTracks");
    }
  }

  ~SpecificConfig() {}
};

#endif
