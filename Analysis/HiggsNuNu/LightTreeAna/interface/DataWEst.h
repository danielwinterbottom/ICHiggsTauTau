#ifndef ICHiggsTauTau_HiggsNuNu_DataWEst_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataWEst_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataWEst : public LTModule{
    CLASS_MEMBER(DataWEst,std::string,sigmcset)
    CLASS_MEMBER(DataWEst,std::string,contmcset)
    CLASS_MEMBER(DataWEst,std::vector<std::string>,contbkgset)
    CLASS_MEMBER(DataWEst,std::string,contdataset)
    CLASS_MEMBER(DataWEst,std::string,sigcat)
    CLASS_MEMBER(DataWEst,std::string,contcat)
    CLASS_MEMBER(DataWEst,std::string,basesel)
    CLASS_MEMBER(DataWEst,std::string,sigmcweight)
    CLASS_MEMBER(DataWEst,std::string,contmcweight)
    CLASS_MEMBER(DataWEst,std::string,contdataweight)
  public:
    DataWEst(std::string);
    virtual ~DataWEst();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
