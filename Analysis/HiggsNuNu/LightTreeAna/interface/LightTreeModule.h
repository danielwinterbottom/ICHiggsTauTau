#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeModule_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeModule_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "TFile.h"

namespace ic{

  class LTModule {
  protected:
    std::string module_name_;
    TFile* fs_;
  public:
    LTModule(std::string);
    std::string module_name();
    virtual int Init(TFile*) =0;
    virtual int Run(LTFiles*)=0;
  };

}

#endif
