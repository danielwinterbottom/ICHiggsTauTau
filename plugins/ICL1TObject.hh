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
    ICL1TObject(string);
    virtual ~ICL1TObject();
    virtual void Print() const;
    string getName() { return fName; }
    void setName( string name ) { fName=name; }
    
    #ifndef SKIP_CINT_DICT
  public:
    ClassDef(ICL1TObject, 1);
    #endif
  
  public:
    string             fName;
    short int          sumType;
    short int          charge;
    unsigned short int isolation;
    unsigned short int quality;
  };
  
  typedef std::vector<ic::ICL1TObject> ICL1TObjectCollection;
}

#endif