#ifndef ICHiggsTauTau_HiggsNuNu_EventList_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_EventList_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class EventList : public LTModule{
    CLASS_MEMBER(EventList,std::string,set)
    CLASS_MEMBER(EventList,std::string,cat)
    CLASS_MEMBER(EventList,std::string,basesel)
    CLASS_MEMBER(EventList,std::string,weight)
  public:
    EventList(std::string);
    virtual ~EventList();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
