#include "UserCode/ICHiggsTauTau/plugins/ICJetProducer.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"

typedef ICJetProducer<ic::CaloJet, reco::CaloJet> ICNewCaloJetProducer;
typedef ICJetProducer<ic::Jet, reco::CaloJet> ICNewJetFromCaloProducer;

DEFINE_FWK_MODULE(ICNewCaloJetProducer);
DEFINE_FWK_MODULE(ICNewJetFromCaloProducer);
