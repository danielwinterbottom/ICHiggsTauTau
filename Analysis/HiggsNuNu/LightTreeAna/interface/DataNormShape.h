#ifndef ICHiggsTauTau_HiggsNuNu_DataNormShape_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataNormShape_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataNormShape : public LTModule{
    CLASS_MEMBER(DataNormShape,std::vector<std::string>,shape)
    CLASS_MEMBER(DataNormShape,std::vector<std::string>,shapename)
    CLASS_MEMBER(DataNormShape,std::string,sigmcset)
    CLASS_MEMBER(DataNormShape,std::string,contmcset)
    CLASS_MEMBER(DataNormShape,std::vector<std::string>,contbkgset)
    CLASS_MEMBER(DataNormShape,std::vector<std::string>,contbkgextrafactordir)
    CLASS_MEMBER(DataNormShape,std::vector<int>,contbkgisz)
    CLASS_MEMBER(DataNormShape,std::string,contdataset)
    CLASS_MEMBER(DataNormShape,std::string,sigcat)
    CLASS_MEMBER(DataNormShape,std::string,zcontcat)
    CLASS_MEMBER(DataNormShape,std::string,contcat)
    CLASS_MEMBER(DataNormShape,std::string,contdataextrasel)
    CLASS_MEMBER(DataNormShape,std::string,contmcextrasel)
    CLASS_MEMBER(DataNormShape,std::string,contbkgextrasel)
    CLASS_MEMBER(DataNormShape,std::string,basesel)
    CLASS_MEMBER(DataNormShape,std::string,sigmcweight)
    CLASS_MEMBER(DataNormShape,std::string,contmcweight)
    CLASS_MEMBER(DataNormShape,std::string,contdataweight)
    CLASS_MEMBER(DataNormShape,double,sigcontextrafactor)
    CLASS_MEMBER(DataNormShape,std::string,dirname)
  public:
    virtual DataNormShape & set_shape(std::string const& shape) {
      std::vector<std::string> shapes;
      shapes.push_back(shape);
      shape_ = shapes;
      return *this;
    }
    DataNormShape(std::string);
    virtual ~DataNormShape();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
