#ifndef CombineTools_HttSystematics_h
#define CombineTools_HttSystematics_h
#include "CombineTools/interface/CombineHarvester.h"

namespace ch {
  void AddDefaultSystematics(CombineHarvester *cb);
  void AddMSSMSystematics(CombineHarvester *cb);
}

#endif
