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
