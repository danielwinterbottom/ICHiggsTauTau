#include "UserCode/ICHiggsTauTau/plugins/ICTauProducer.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

typedef SingleObjectSelector<reco::PFTauCollection,
                             StringCutObjectSelector<reco::PFTau>,
                             reco::PFTauRefVector> PFTauRefSelector;

DEFINE_FWK_MODULE(PFTauRefSelector);

typedef ICTauProducer<reco::PFTau> ICPFTauProducer;
typedef ICTauProducer<pat::Tau> ICPFTauFromPatProducer;

DEFINE_FWK_MODULE(ICPFTauProducer);
DEFINE_FWK_MODULE(ICPFTauFromPatProducer);
