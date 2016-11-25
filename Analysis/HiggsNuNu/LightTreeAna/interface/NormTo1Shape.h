#ifndef ICHiggsTauTau_HiggsNuNu_NormTo1Shape_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_NormTo1Shape_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class NormTo1Shape : public LTModule{
    CLASS_MEMBER(NormTo1Shape,std::vector<std::string>,shape)
    CLASS_MEMBER(NormTo1Shape,std::vector<std::string>,shapename)
    CLASS_MEMBER(NormTo1Shape,std::vector<std::string>,dataset)
    CLASS_MEMBER(NormTo1Shape,std::string,cat)
    CLASS_MEMBER(NormTo1Shape,std::string,basesel)
    CLASS_MEMBER(NormTo1Shape,std::string,dataweight)
    CLASS_MEMBER(NormTo1Shape,std::string,dirname)
  public:
    virtual NormTo1Shape & set_shape(std::string const& shape) {
      std::vector<std::string> shapes;
      shapes.push_back(shape);
      shape_ = shapes;
      return *this;
    }
    virtual NormTo1Shape & set_dataset(std::string const& dataset) {
      std::vector<std::string> datasets;
      datasets.push_back(dataset);
      dataset_ = datasets;
      return *this;
    }
    NormTo1Shape(std::string);
    virtual ~NormTo1Shape();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
