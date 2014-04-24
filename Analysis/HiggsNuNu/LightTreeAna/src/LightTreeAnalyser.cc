#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"

namespace ic{

  LTAnalyser::LTAnalyser(){
  };
  
  LTAnalyser::LTAnalyser(int verbosity){
    verbosity_=verbosity;
  };
  
  LTAnalyser LTAnalyser::AddModule(LTModule module){
    modulelist_.push_back(module);
    return *this;
  };

}
