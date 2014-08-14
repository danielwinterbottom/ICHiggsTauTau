#ifndef ICHiggsTauTau_HiggsNuNu_DataZEst_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataZEst_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataZEst : public LTModule{
    CLASS_MEMBER(DataZEst,std::string,sigmcewkset)
    CLASS_MEMBER(DataZEst,std::string,sigmcqcdset)
    CLASS_MEMBER(DataZEst,std::string,contmcewkset)
    CLASS_MEMBER(DataZEst,std::string,contmcqcdset)
    CLASS_MEMBER(DataZEst,std::vector<std::string>,contbkgset)
    CLASS_MEMBER(DataZEst,std::string,contdataset)
    CLASS_MEMBER(DataZEst,std::string,sigcat)
    CLASS_MEMBER(DataZEst,std::string,contcat)
    CLASS_MEMBER(DataZEst,std::string,basesel)
    CLASS_MEMBER(DataZEst,double,sigmainccontewk)
    CLASS_MEMBER(DataZEst,double,sigmainccontqcd)
    CLASS_MEMBER(DataZEst,double,sigmaincsigewk)
    CLASS_MEMBER(DataZEst,double,sigmaincsigqcd)
    CLASS_MEMBER(DataZEst,double,ngenincewk)
    CLASS_MEMBER(DataZEst,double,ngenincqcd)
    CLASS_MEMBER(DataZEst,double,ngenmassfilteredewk)
    CLASS_MEMBER(DataZEst,double,ngenmassfilteredqcd)
  public:
    DataZEst(std::string);
    virtual ~DataZEst();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
