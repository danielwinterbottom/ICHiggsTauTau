#ifndef ICHiggsTauTau_HiggsNuNu_EventPicker_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_EventPicker_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>

namespace ic {

  class EventPicker : public LTModule{
    CLASS_MEMBER(EventPicker,std::vector<std::string>,sets)
    CLASS_MEMBER(EventPicker,std::vector<std::string>,variables)
    CLASS_MEMBER(EventPicker,std::vector<unsigned>,run)
    CLASS_MEMBER(EventPicker,std::vector<unsigned>,lumi)
    CLASS_MEMBER(EventPicker,std::vector<unsigned>,event)
    CLASS_MEMBER(EventPicker,bool,setorfile)
  public:
    EventPicker(std::string);
    virtual ~EventPicker();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
