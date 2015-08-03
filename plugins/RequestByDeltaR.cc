#include "UserCode/ICHiggsTauTau/plugins/RequestByDeltaR.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

typedef RequestByDeltaR<reco::Track> RequestTracksByDeltaR;
typedef RequestByDeltaR<reco::PFCandidate> RequestPFCandidatesByDeltaR;

DEFINE_FWK_MODULE(RequestTracksByDeltaR);
DEFINE_FWK_MODULE(RequestPFCandidatesByDeltaR);
