#include "UserCode/ICHiggsTauTau/plugins/ICRun2JetProducer.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

// First define a couple of selectors that don't exisit in the
// CommonTools/UtilAlgos package
typedef SingleObjectSelector<reco::PFJetCollection,
                             StringCutObjectSelector<reco::PFJet>,
                             reco::PFJetRefVector> PFJetRefSelector;

DEFINE_FWK_MODULE(PFJetRefSelector);


/// Produce an ic::PFJet collection from a reco::PFJet collection
typedef ICRun2JetProducer<reco::PFJet> ICRun2PFJetProducer;

/// Produce an ic::PFJet collection from a pat::Jet collection
typedef ICRun2JetProducer<pat::Jet> ICRun2PFJetFromPatProducer;


DEFINE_FWK_MODULE(ICRun2PFJetProducer);
DEFINE_FWK_MODULE(ICRun2PFJetFromPatProducer);
