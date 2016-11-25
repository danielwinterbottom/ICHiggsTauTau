#ifndef ICHiggsTauTau_HiggsNuNu_Plotter_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_Plotter_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>

namespace ic {

  class Plotter : public LTModule{

  protected:								
    std::vector<std::vector<std::string> > sets_;
  public:								
    virtual Plotter & set_sets(std::vector<std::vector<std::string> > const& sets) {sets_ = sets; return *this; } 
    virtual std::vector<std::vector<std::string> > & set() {return sets_; }
    CLASS_MEMBER(Plotter,std::vector<int>,setscolours)
    CLASS_MEMBER(Plotter,std::vector<std::string>,setsnames)
    CLASS_MEMBER(Plotter,std::vector<std::string>,setsscales)
    CLASS_MEMBER(Plotter,std::string,cat)
    CLASS_MEMBER(Plotter,std::string,basesel)
    CLASS_MEMBER(Plotter,std::vector<std::string>,shapes)
    CLASS_MEMBER(Plotter,std::vector<std::string>,axisnames)

  public:
    Plotter(std::string);
    virtual ~Plotter();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
