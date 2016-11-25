#include "UserCode/ICHiggsTauTau/plugins/ICPFCandidateProducer.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"


typedef SingleObjectSelector<reco::PFCandidateCollection,
                             StringCutObjectSelector<reco::PFCandidate>,
                             reco::PFCandidateRefVector> PFCandidateRefSelector;

DEFINE_FWK_MODULE(PFCandidateRefSelector);

typedef ICPFCandidateProducer<reco::PFCandidate> ICPFProducer;
DEFINE_FWK_MODULE(ICPFProducer);

#if CMSSW_MAJOR_VERSION >= 7
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

typedef ICPFCandidateProducer<pat::PackedCandidate> ICPFFromPackedProducer;
DEFINE_FWK_MODULE(ICPFFromPackedProducer);
#endif
