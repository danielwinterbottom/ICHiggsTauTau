#ifndef UserCode_ICHiggsTauTau_ICJetProducerConfig_h
#define UserCode_ICHiggsTauTau_ICJetProducerConfig_h

#include <memory>
#include "boost/functional/hash.hpp"
// #include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
// #include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
// #include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"

template <class U>
struct SpecificConfig {
  SpecificConfig() {}
  ~SpecificConfig() {}

  void Set(const edm::ParameterSet & /*pset*/) {}
};

template <>
struct SpecificConfig<reco::CaloJet> {
  edm::InputTag input_jet_id;
  bool do_jet_id;
  JetIDSelectionFunctor *loose_id;
  JetIDSelectionFunctor *tight_id;

  SpecificConfig() : do_jet_id(false), loose_id(NULL), tight_id(NULL) {
    loose_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::LOOSE);
    tight_id = new JetIDSelectionFunctor(JetIDSelectionFunctor::PURE09,
                                         JetIDSelectionFunctor::TIGHT);
  }
  ~SpecificConfig() {
    delete loose_id;
    delete tight_id;
  }

  void Set(const edm::ParameterSet &pset) {
    if (pset.exists("includeJetID")) {
      input_jet_id = pset.getParameter<edm::InputTag>("includeJetID");
      do_jet_id = true;
    }
  }
};

#endif
