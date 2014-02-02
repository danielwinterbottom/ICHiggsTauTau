#ifndef ICHiggsTauTau_CombineTools_HelperFunctions_h
#define ICHiggsTauTau_CombineTools_HelperFunctions_h
// #include <memory>
// #include <string>
// #include "TH1.h"
// #include "CombineTools/interface/Process.h"

namespace ch {

template<class T, class U>
bool MatchingProcess(T first, U second) {
  if (first.bin()         == second.bin()         &&
      first.process()     == second.process()     && 
      first.process_id()  == second.process_id()  && 
      first.analysis()    == second.analysis()    &&
      first.era()         == second.era()         &&
      first.channel()     == second.channel()     &&
      first.bin_id()      == second.bin_id()      &&
      first.mass()        == second.mass()) {
    return true;
  } else {
    return false;
  }
}
}

#endif
