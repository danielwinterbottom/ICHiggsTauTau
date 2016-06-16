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

  public:
    ICL1TObject();
    virtual ~ICL1TObject();
    virtual void Print() const;
    
    void setSumType(short int value) { sumType = value; }
    short int getSumType() { return sumType; }
    
    void setCharge(short int value) { charge = value; }
    short int getCharge() { return charge; }
    
    void setIsolation(unsigned short int value) { isolation = value; }
    unsigned short int getIsolation() { return isolation; }
    
    void setQuality(unsigned short int value) { quality = value; }
    unsigned short int getQuality() { return quality; }
    
    #ifndef SKIP_CINT_DICT
  public:
    ClassDef(ICL1TObject, 1);
    #endif
  
  private:
    short int          sumType;
    short int          charge;
    unsigned short int isolation;
    unsigned short int quality;
  };
  
  typedef std::vector<ic::ICL1TObject> ICL1TObjectCollection;
}

#endif