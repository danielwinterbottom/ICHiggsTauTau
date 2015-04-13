#ifndef ICHiggsTauTau_HiggsNuNu_BkgSubDataNormShape_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_BkgSubDataNormShape_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class BkgSubDataNormShape : public LTModule{
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<std::string>,shape)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<std::string>,shapename)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapebaseset)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<std::string>,contbkgset)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<std::string>,shapebkgset)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<std::string>,contbkgextrafactordir)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<std::string>,shapebkgextrafactordir)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<int>,contbkgisz)
    CLASS_MEMBER(BkgSubDataNormShape,std::vector<int>,shapebkgisz)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contdataset)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapecat)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,zextrashapecat)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contcat)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,zextracontcat)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contdataextrasel)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contbkgextrasel)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapebkgextrasel)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapebaseextrasel)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,basesel)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapebaseweight)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contmcweight)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapemcweight)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contmczweight)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,shapemczweight)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,contdataweight)
    CLASS_MEMBER(BkgSubDataNormShape,double,shapecontextrafactor)
    CLASS_MEMBER(BkgSubDataNormShape,bool,do_latex)
    CLASS_MEMBER(BkgSubDataNormShape,std::string,dirname)
    CLASS_MEMBER(BkgSubDataNormShape,bool,do_ddnorm)
  public:
    virtual BkgSubDataNormShape & set_shape(std::string const& shape) {
      std::vector<std::string> shapes;
      shapes.push_back(shape);
      shape_ = shapes;
      return *this;
    }
    BkgSubDataNormShape(std::string);
    virtual ~BkgSubDataNormShape();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
