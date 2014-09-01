#include "UserCode/ICHiggsTauTau/plugins/Concatenate.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/METReco/interface/PFMET.h"


typedef ic::Concatenate<reco::PFMET> ICPFMETConcatenate;
typedef ic::Concatenate<std::size_t> ICIDConcatenate;

// define these as plug-ins
DEFINE_FWK_MODULE(ICPFMETConcatenate);
DEFINE_FWK_MODULE(ICIDConcatenate);
