#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"

namespace ic{

  LTModule::LTModule(){
  };

  std::string LTModule::module_name(){
    return module_name_;
  };
}
