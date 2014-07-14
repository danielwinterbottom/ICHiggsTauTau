#include "UserCode/ICHiggsTauTau/plugins/ICJetProducer.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

typedef SingleObjectSelector<reco::PFJetCollection,
                             StringCutObjectSelector<reco::PFJet>,
                             reco::PFJetRefVector> PFJetRefSelector;

DEFINE_FWK_MODULE(PFJetRefSelector);


typedef ICJetProducer<ic::CaloJet, reco::CaloJet> ICNewCaloJetProducer;
typedef ICJetProducer<ic::Jet, reco::CaloJet> ICNewJetFromCaloProducer;
typedef ICJetProducer<ic::PFJet, reco::PFJet> ICNewPFJetProducer;
typedef ICJetProducer<ic::Jet, reco::PFJet> ICNewJetFromPFProducer;

DEFINE_FWK_MODULE(ICNewCaloJetProducer);
DEFINE_FWK_MODULE(ICNewJetFromCaloProducer);
DEFINE_FWK_MODULE(ICNewPFJetProducer);
DEFINE_FWK_MODULE(ICNewJetFromPFProducer);
