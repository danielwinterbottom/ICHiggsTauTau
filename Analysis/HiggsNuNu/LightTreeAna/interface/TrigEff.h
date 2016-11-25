#ifndef ICHiggsTauTau_HiggsNuNu_TrigEff_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_TrigEff_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class TrigEff : public LTModule{
    CLASS_MEMBER(TrigEff,std::vector<std::string>,trigger)
    CLASS_MEMBER(TrigEff,std::vector<std::string>,triggername)
    CLASS_MEMBER(TrigEff,std::string,shape)
    CLASS_MEMBER(TrigEff,std::string,shapename)
    CLASS_MEMBER(TrigEff,std::vector<std::string>,dataset)
    CLASS_MEMBER(TrigEff,std::vector<double>,binning)
    CLASS_MEMBER(TrigEff,std::string,cat)
    CLASS_MEMBER(TrigEff,std::string,basesel)
    CLASS_MEMBER(TrigEff,std::string,numweight)
    CLASS_MEMBER(TrigEff,std::string,denweight)
    CLASS_MEMBER(TrigEff,std::string,dirname)
    CLASS_MEMBER(TrigEff,std::string,histtitle)
    CLASS_MEMBER(TrigEff,std::string,namestr)
    CLASS_MEMBER(TrigEff,bool,do_fit)
  public:
    virtual TrigEff & set_dataset(std::string const& dataset) {
      std::vector<std::string> datasets;
      datasets.push_back(dataset);
      dataset_ = datasets;
      return *this;
    }
    virtual TrigEff & set_trigger(std::string const& trigger) {
      std::vector<std::string> triggers;
      triggers.push_back(trigger);
      trigger_ = triggers;
      return *this;
    }
    TrigEff(std::string);
    virtual ~TrigEff();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
