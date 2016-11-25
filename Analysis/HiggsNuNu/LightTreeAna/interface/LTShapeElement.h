#ifndef ICHiggsTauTau_HiggsNuNu_LTShapeElement_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_LTShapeElement_h

#include <string>
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"


namespace ic {

  class LTShapeElement{
  public:
    LTShapeElement();
    ~LTShapeElement();
  private:
    CLASS_MEMBER(LTShapeElement,std::string,name)
    CLASS_MEMBER(LTShapeElement,std::string,histtitle)
    CLASS_MEMBER(LTShapeElement,bool,dology)
    CLASS_MEMBER(LTShapeElement,double,axisrangemultiplier)
    CLASS_MEMBER(LTShapeElement,double,legleft)
    CLASS_MEMBER(LTShapeElement,double,legright)
  };



}

#endif
