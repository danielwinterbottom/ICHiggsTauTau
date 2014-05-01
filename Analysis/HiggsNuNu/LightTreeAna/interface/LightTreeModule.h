#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeModule_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeModule_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

namespace ic{

  class LTModule {
  protected:
    std::string module_name_;
    fwlite::TFileService* fs_;
  public:
    LTModule(std::string);
    std::string module_name();
    virtual int Init(fwlite::TFileService*) =0;
    virtual int Run(LTFiles*)=0;
  };

}

#endif
