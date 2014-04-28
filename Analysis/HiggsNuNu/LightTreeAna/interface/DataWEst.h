#ifndef ICHiggsTauTau_HiggsNuNu_DataWEst_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataWEst_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataWEst : public LTModule{
    CLASS_MEMBER(DataWEst,std::string,sigset)
    CLASS_MEMBER(DataWEst,std::string,contset)
    CLASS_MEMBER(DataWEst,std::string,sigcat)
    CLASS_MEMBER(DataWEst,std::string,contcat)
    CLASS_MEMBER(DataWEst,std::string,basesel)
  public:
    DataWEst(std::string const& name);
    virtual ~DataWEst();
    virtual int Init();
    virtual int Run(LTFiles*);
  };

}
#endif
