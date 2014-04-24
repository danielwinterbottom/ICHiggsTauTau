#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"

namespace ic{

  class LTAnalyser {
  CLASS_MEMBER(LTAnalyser,int,verbosity)
    
  protected:
    LTFiles filemanager_;
    std::vector<ic::LTModule *> modulelist_;
    
    bool print_module_list_;
  public:
    LTAnalyser();
    LTAnalyser(int);

    LTAnalyser AddModule(ic::LTModule*);

    bool PostModule(int);
    int RunAnalysis();
  };

}

#endif
