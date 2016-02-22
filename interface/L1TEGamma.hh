#ifndef ICHiggsTauTau_L1TEGamma_hh
#define ICHiggsTauTau_L1TEGamma_hh

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"

#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Rtypes.h"

namespace ic {
  
  class L1TEGamma : public ic::Candidate, public ic::L1TObject {
  public:
    L1TEGamma();
    virtual ~L1TEGamma();
    virtual void Print() const;
    
    #ifndef SKIP_CINT_DICT
  public:
    ClassDef(L1TEGamma, 1);
    #endif
  };
  
  typedef std::vector<ic::L1TEGamma> L1TEGammaCollection;
}

#endif
