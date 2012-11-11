#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "TTree.h"

namespace ic {
  
  TTree *ic::StaticTree::tree_ = NULL;
}