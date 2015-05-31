#include "FWCore/Framework/interface/MakerMacros.h"
#include "UserCode/ICHiggsTauTau/plugins/ICPackedCandidateSelector.hh"

#if CMSSW_MAJOR_VERSION >= 7
using namespace pat;
DEFINE_FWK_MODULE(PATPackedCandidateSelector);
#endif
