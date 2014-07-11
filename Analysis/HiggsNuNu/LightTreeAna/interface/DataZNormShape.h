#ifndef ICHiggsTauTau_HiggsNuNu_DataZNormShape_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataZNormShape_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataZNormShape : public LTModule{
    CLASS_MEMBER(DataZNormShape,std::vector<std::string>,shape)
    CLASS_MEMBER(DataZNormShape,std::vector<std::string>,shapename)
    CLASS_MEMBER(DataZNormShape,std::string,sigmcewkset)
    CLASS_MEMBER(DataZNormShape,std::string,sigmcqcdset)
    CLASS_MEMBER(DataZNormShape,std::string,contmcewkset)
    CLASS_MEMBER(DataZNormShape,std::string,contmcqcdset)
    CLASS_MEMBER(DataZNormShape,std::vector<std::string>,contbkgset)
    CLASS_MEMBER(DataZNormShape,std::string,contdataset)
    CLASS_MEMBER(DataZNormShape,std::string,sigcat)
    CLASS_MEMBER(DataZNormShape,std::string,contcat)
    CLASS_MEMBER(DataZNormShape,std::string,contdataextrasel)
    CLASS_MEMBER(DataZNormShape,std::string,basesel)
    CLASS_MEMBER(DataZNormShape,double,sigmainccontewk)
    CLASS_MEMBER(DataZNormShape,double,sigmainccontqcd)
    CLASS_MEMBER(DataZNormShape,double,sigmaincsigewk)
    CLASS_MEMBER(DataZNormShape,double,sigmaincsigqcd)
    CLASS_MEMBER(DataZNormShape,double,ngenincewk)
    CLASS_MEMBER(DataZNormShape,double,ngenincqcd)
    CLASS_MEMBER(DataZNormShape,double,ngenmassfilteredewk)
    CLASS_MEMBER(DataZNormShape,double,ngenmassfilteredqcd)
    CLASS_MEMBER(DataZNormShape,std::string,sigmcweight)
    CLASS_MEMBER(DataZNormShape,std::string,contmcweight)
    CLASS_MEMBER(DataZNormShape,std::string,contdataweight)
    CLASS_MEMBER(DataZNormShape,double,sigcontextrafactor)
    CLASS_MEMBER(DataZNormShape,std::string,dirname)
  public:
    virtual DataZNormShape & set_shape(std::string const& shape) {
      std::vector<std::string> shapes;
      shapes.push_back(shape);
      shape_ = shapes;
      return *this;
    }
    DataZNormShape(std::string);
    virtual ~DataZNormShape();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
