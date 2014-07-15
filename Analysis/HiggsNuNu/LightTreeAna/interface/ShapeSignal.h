#ifndef ICHiggsNuNu_ShapeSignal_h
#define ICHiggsNuNu_ShapeSignal_h

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>

namespace ic{

  class ShapeSignal : public LTModule {
  protected:
    std::vector<std::vector<std::string> > sets_;
  public:
    virtual ShapeSignal & set_sets(std::vector<std::vector<std::string> > const& sets) {sets_ = sets; return *this; }
    virtual std::vector<std::vector<std::string> > & set() {return sets_; }
      CLASS_MEMBER(ShapeSignal,std::string,sigmcset)
      CLASS_MEMBER(ShapeSignal,std::string,contmcset)
      CLASS_MEMBER(ShapeSignal,std::vector<std::string>,contbkgset)
      CLASS_MEMBER(ShapeSignal,std::string,contdataset)
      CLASS_MEMBER(ShapeSignal,std::string,sigcat)
      CLASS_MEMBER(ShapeSignal,std::string,contcat)
      CLASS_MEMBER(ShapeSignal,std::string,basesel)
      CLASS_MEMBER(ShapeSignal,std::string,sigmcweight)
      CLASS_MEMBER(ShapeSignal,std::string,contmcweight)
      CLASS_MEMBER(ShapeSignal,std::string,contdataweight)
      CLASS_MEMBER(ShapeSignal,std::string,SignalShape)
  public:
    ShapeSignal(std::string);
    virtual ~ShapeSignal();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

} //namespace

#endif
