#ifndef ICHiggsTauTau_HiggsNuNu_MVATrain_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_MVATrain_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class MVATrain : public LTModule{
    CLASS_MEMBER(MVATrain,std::vector<std::string>,sigsets)
    CLASS_MEMBER(MVATrain,std::vector<std::string>,bkgsets)
    CLASS_MEMBER(MVATrain,std::vector<std::string>,variables)
    CLASS_MEMBER(MVATrain,std::vector<std::string>,specvariables)
    CLASS_MEMBER(MVATrain,std::string,basesel)
    CLASS_MEMBER(MVATrain,std::string,sigcat)
    CLASS_MEMBER(MVATrain,std::string,bkgcat)
  public:
    MVATrain(std::string);
    virtual ~MVATrain();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
