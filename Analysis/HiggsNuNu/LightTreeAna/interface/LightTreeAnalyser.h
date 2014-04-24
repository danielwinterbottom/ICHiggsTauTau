#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"

namespace ic{

  class LTAnalyser {
  CLASS_MEMBER(LTAnalyser,int,verbosity)
    
  protected:
    Files filemanager_;
    std::vector<LTModule> modulelist_;
      
  public:
    LTAnalyser();
    LTAnalyser(int);

    LTAnalyser AddModule(LTModule);
  };

}

#endif
