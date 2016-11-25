#include "UserCode/ICHiggsTauTau/plugins/RequestByDeltaR.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/TrackReco/interface/Track.h"

typedef RequestByDeltaR<reco::Track> RequestTracksByDeltaR;

DEFINE_FWK_MODULE(RequestTracksByDeltaR);
