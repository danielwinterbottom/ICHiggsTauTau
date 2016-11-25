#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

#if CMSSW_MAJOR_VERSION < 6
edm::ConsumesCollector consumesCollector(){
 edm::ConsumesCollector cons;
 return cons;
}

#endif

