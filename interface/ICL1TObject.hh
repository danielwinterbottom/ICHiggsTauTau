#ifndef ICHiggsTauTau_ICL1TObject_hh
#define ICHiggsTauTau_ICL1TObject_hh

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Rtypes.h"

namespace ic {
  
  class ICL1TObject : public ic::Candidate {
  public:
    ICL1TObject();
    virtual ~ICL1TObject();
    virtual void Print() const;

    inline void set_bx(int value){
      bx_ = value;
    };

    inline int bx() const{
      return bx_;
    };

    inline void setSumType(int value) { sumType = value; }
    inline int getSumType() const { return sumType; }
    
    inline void setCharge(short int value) { charge = value; }
    inline short int getCharge() const { return charge; }
    
    inline void setIsolation(unsigned short int value) { isolation = value; }
    inline unsigned short int getIsolation() const { return isolation; }
    
    inline void setQuality(unsigned short int value) { quality = value; }
    inline unsigned short int getQuality() const { return quality; }
    
  private:
    int bx_;
    int          sumType;
    short int          charge;
    unsigned short int isolation;
    unsigned short int quality;

    #ifndef SKIP_CINT_DICT
  public:
    ClassDef(ICL1TObject, 2);
    #endif
  
  };
  
  typedef std::vector<ic::ICL1TObject> ICL1TObjectCollection;
}

#endif
