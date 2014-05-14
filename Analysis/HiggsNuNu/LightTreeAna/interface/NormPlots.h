#ifndef ICHiggsTauTau_HiggsNuNu_NormPlots_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_NormPlots_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class NormPlots : public LTModule{
    CLASS_MEMBER(NormPlots,std::string,qcdset)
    CLASS_MEMBER(NormPlots,std::string,sigset)
    CLASS_MEMBER(NormPlots,std::vector<std::string>,ewkset)
    CLASS_MEMBER(NormPlots,std::string,cat)
    CLASS_MEMBER(NormPlots,std::string,basesel)
    CLASS_MEMBER(NormPlots,std::vector<std::string>,shapes)
  public:
    NormPlots(std::string);
    virtual ~NormPlots();
    virtual int Init(fwlite::TFileService*);
    virtual int Run(LTFiles*);
  };

}
#endif
