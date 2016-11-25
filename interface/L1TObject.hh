#ifndef ICHiggsTauTau_L1TObject_hh
#define ICHiggsTauTau_L1TObject_hh

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Rtypes.h"

namespace ic {
  
  class L1TObject : public ic::Candidate {
  public:
    L1TObject();
    virtual ~L1TObject();
    virtual void Print() const;

    inline void set_bx(int value){ bx_ = value; };
    inline int bx() const{ return bx_; };

    inline void set_sumType(int value) { sumType_ = value; }
    inline int sumType() const { return sumType_; }
    
    inline void set_charge(short int value) { charge_ = value; }
    inline short int charge() const { return charge_; }
    
    inline void set_isolation(unsigned short int value) { isolation_ = value; }
    inline unsigned short int isolation() const { return isolation_; }
    
    inline void set_quality(unsigned short int value) { quality_ = value; }
    inline unsigned short int quality() const { return quality_; }
    
  private:
    int                bx_;
    int                sumType_;
    short int          charge_;
    unsigned short int isolation_;
    unsigned short int quality_;

    #ifndef SKIP_CINT_DICT
  public:
    ClassDef(L1TObject, 2);
    #endif
  
  };
  
  typedef std::vector<ic::L1TObject> L1TObjectCollection;
}

#endif
