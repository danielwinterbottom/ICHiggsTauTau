#ifndef ICHiggsTauTau_HiggsNuNu_APlot_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_APlot_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>

namespace ic {

  class APlot : public LTModule{

  protected:								
    std::vector<std::vector<std::string> > sets_;
  public:								
    virtual APlot & set_sets(std::vector<std::vector<std::string> > const& sets) {sets_ = sets; return *this; } 
    virtual std::vector<std::vector<std::string> > & set() {return sets_; }
    CLASS_MEMBER(APlot,std::vector<int>,setscolours)
    CLASS_MEMBER(APlot,std::vector<std::string>,setsnames)
    CLASS_MEMBER(APlot,std::vector<std::string>,setsscales)
    CLASS_MEMBER(APlot,std::string,cat)
    CLASS_MEMBER(APlot,std::string,basesel)
    CLASS_MEMBER(APlot,std::vector<std::string>,shapes)
    CLASS_MEMBER(APlot,std::vector<std::string>,axisnames)
    CLASS_MEMBER(APlot,std::vector<int>,axiscolours)
    CLASS_MEMBER(APlot,std::vector<std::string>,setstitle)
    CLASS_MEMBER(APlot,std::vector<std::string>,drawopts)   

  public:
    APlot(std::string);
    virtual ~APlot();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
