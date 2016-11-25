#ifndef ICHiggsTauTau_HiggsNuNu_DataQCDEst_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataQCDEst_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataQCDEst : public LTModule{
    CLASS_MEMBER(DataQCDEst,std::string,Aset)
    CLASS_MEMBER(DataQCDEst,std::string,Bset)
    CLASS_MEMBER(DataQCDEst,std::string,Cset)
    CLASS_MEMBER(DataQCDEst,std::vector<std::string>,Abkgset)
    CLASS_MEMBER(DataQCDEst,std::vector<std::string>,Bbkgset)
    CLASS_MEMBER(DataQCDEst,std::vector<std::string>,Cbkgset)
    CLASS_MEMBER(DataQCDEst,std::string,Acat)
    CLASS_MEMBER(DataQCDEst,std::string,Bcat)
    CLASS_MEMBER(DataQCDEst,std::string,Ccat)
    CLASS_MEMBER(DataQCDEst,std::string,basesel)
  public:
    DataQCDEst(std::string);
    virtual ~DataQCDEst();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
