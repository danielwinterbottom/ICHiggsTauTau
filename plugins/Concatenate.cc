#include "UserCode/ICHiggsTauTau/plugins/Concatenate.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MET.h"


typedef ic::Concatenate<reco::PFMET> ICPFMETConcatenate;
typedef ic::Concatenate<pat::MET> ICPATMETConcatenate;
typedef ic::Concatenate<std::size_t> ICIDConcatenate;

// define these as plug-ins
DEFINE_FWK_MODULE(ICPFMETConcatenate);
DEFINE_FWK_MODULE(ICPATMETConcatenate);
DEFINE_FWK_MODULE(ICIDConcatenate);
