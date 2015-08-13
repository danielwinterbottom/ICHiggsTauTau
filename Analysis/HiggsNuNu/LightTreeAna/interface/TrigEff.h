#ifndef ICHiggsTauTau_HiggsNuNu_TrigEff_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_TrigEff_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class TrigEff : public LTModule{
    CLASS_MEMBER(TrigEff,std::string,trigger)
    CLASS_MEMBER(TrigEff,std::string,shape)
    CLASS_MEMBER(TrigEff,std::string,shapename)
    CLASS_MEMBER(TrigEff,std::vector<std::string>,dataset)
    CLASS_MEMBER(TrigEff,std::vector<double>,binning)
    CLASS_MEMBER(TrigEff,std::string,cat)
    CLASS_MEMBER(TrigEff,std::string,basesel)
    CLASS_MEMBER(TrigEff,std::string,dataweight)
    CLASS_MEMBER(TrigEff,std::string,dirname)
    CLASS_MEMBER(TrigEff,std::string,histtitle)
  public:
    virtual TrigEff & set_dataset(std::string const& dataset) {
      std::vector<std::string> datasets;
      datasets.push_back(dataset);
      dataset_ = datasets;
      return *this;
    }
    TrigEff(std::string);
    virtual ~TrigEff();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
