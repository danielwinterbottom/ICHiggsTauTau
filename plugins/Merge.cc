#include "UserCode/ICHiggsTauTau/plugins/Merge.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

typedef ic::Merge<reco::Muon> ICMuonMerger;
typedef ic::Merge<reco::Track> ICTrackMerger;
typedef ic::Merge<reco::PFCandidate> ICPFCandidateMerger;

// define these as plug-ins
DEFINE_FWK_MODULE(ICMuonMerger);
DEFINE_FWK_MODULE(ICTrackMerger);
DEFINE_FWK_MODULE(ICPFCandidateMerger);
