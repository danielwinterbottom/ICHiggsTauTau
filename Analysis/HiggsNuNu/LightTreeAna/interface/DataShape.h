#ifndef ICHiggsTauTau_HiggsNuNu_DataShape_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_DataShape_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class DataShape : public LTModule{
    CLASS_MEMBER(DataShape,std::vector<std::string>,shape)
    CLASS_MEMBER(DataShape,std::vector<std::string>,shapename)
    CLASS_MEMBER(DataShape,std::string,dataset)
    CLASS_MEMBER(DataShape,std::string,cat)
    CLASS_MEMBER(DataShape,std::string,basesel)
    CLASS_MEMBER(DataShape,std::string,dataweight)
    CLASS_MEMBER(DataShape,std::string,dirname)
  public:
    virtual DataShape & set_shape(std::string const& shape) {
      std::vector<std::string> shapes;
      shapes.push_back(shape);
      shape_ = shapes;
      return *this;
    }
    DataShape(std::string);
    virtual ~DataShape();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
