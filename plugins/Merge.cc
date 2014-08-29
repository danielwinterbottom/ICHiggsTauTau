#include "UserCode/ICHiggsTauTau/plugins/Merge.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"

typedef ic::Merge<reco::Muon> ICMuonMerger;
typedef ic::Merge<reco::Track> ICTrackMerger;

// define these as plug-ins
DEFINE_FWK_MODULE(ICMuonMerger);
DEFINE_FWK_MODULE(ICTrackMerger);
