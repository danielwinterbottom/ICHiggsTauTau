#include "UserCode/ICHiggsTauTau/plugins/ICJetProducer.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

// First define a couple of selectors that don't exisit in the
// CommonTools/UtilAlgos package
typedef SingleObjectSelector<reco::PFJetCollection,
                             StringCutObjectSelector<reco::PFJet>,
                             reco::PFJetRefVector> PFJetRefSelector;

typedef SingleObjectSelector<reco::JPTJetCollection,
                             StringCutObjectSelector<reco::JPTJet>,
                             reco::JPTJetRefVector> JPTJetRefSelector;

DEFINE_FWK_MODULE(PFJetRefSelector);
DEFINE_FWK_MODULE(JPTJetRefSelector);

/**@{
\anchor JetProducers
\name   Available jet producers
*/
/// Produce an ic::CaloJet collection from a reco::CaloJet collection
typedef ICJetProducer<ic::CaloJet, reco::CaloJet> ICCaloJetProducer;
/// Produce an ic::JPTJet collection from a reco::JPTJet collection
typedef ICJetProducer<ic::JPTJet, reco::JPTJet> ICJPTJetProducer;
/// Produce an ic::PFJet collection from a reco::PFJet collection
typedef ICJetProducer<ic::PFJet, reco::PFJet> ICPFJetProducer;
/// Produce an ic::Jet collection from a reco::Jet collection
typedef ICJetProducer<ic::Jet, reco::Jet> ICBasicJetProducer;
/// Produce an ic::CaloJet collection from a pat::Jet collection
typedef ICJetProducer<ic::CaloJet, pat::Jet> ICCaloJetFromPatProducer;
/// Produce an ic::JPTJet collection from a pat::Jet collection
typedef ICJetProducer<ic::JPTJet, pat::Jet> ICJPTJetFromPatProducer;
/// Produce an ic::PFJet collection from a pat::Jet collection
typedef ICJetProducer<ic::PFJet, pat::Jet> ICPFJetFromPatProducer;
/// Produce an ic::Jet collection from a pat::Jet collection
typedef ICJetProducer<ic::Jet, pat::Jet> ICJetFromPatProducer;
/**@}*/

DEFINE_FWK_MODULE(ICCaloJetProducer);
DEFINE_FWK_MODULE(ICPFJetProducer);
DEFINE_FWK_MODULE(ICJPTJetProducer);
DEFINE_FWK_MODULE(ICBasicJetProducer);
DEFINE_FWK_MODULE(ICCaloJetFromPatProducer);
DEFINE_FWK_MODULE(ICPFJetFromPatProducer);
DEFINE_FWK_MODULE(ICJPTJetFromPatProducer);
DEFINE_FWK_MODULE(ICJetFromPatProducer);
