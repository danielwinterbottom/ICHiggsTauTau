#ifndef ICHiggsTauTau_L1TSum_hh
#define ICHiggsTauTau_L1TSum_hh

#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"

#include <vector>

namespace ic {
  
  class L1TSum : public ic::L1TObject {
  public:
    
    enum SumType {
      kTotalEt,
      kTotalHt,
      kMissingEt,
      kMissingHt,
      kTotalEtx,
      kTotalEty,
      kTotalHtx,
      kTotalHty,
    };
    
    L1TSum();
    virtual ~L1TSum();
    virtual void Print() const;

  #ifndef SKIP_CINT_DICT
  public:
    ClassDef(L1TSum, 1);
  #endif
    
  public:
    short int sumType;
    
  };
  
  typedef std::vector<ic::L1TSum> L1TSumCollection;
}

#endif