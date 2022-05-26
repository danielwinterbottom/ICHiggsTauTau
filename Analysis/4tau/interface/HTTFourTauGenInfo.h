#ifndef ICHiggsTauTau_Module_HTTFourTauGenInfo_h
#define ICHiggsTauTau_Module_HTTFourTauGenInfo_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"


#include <string>

namespace ic {
  
class HTTFourTauGenInfo : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFourTauGenInfo, std::string, ditau_label)
  CLASS_MEMBER(HTTFourTauGenInfo, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTFourTauGenInfo, bool, write_plots)
  CLASS_MEMBER(HTTFourTauGenInfo, bool, ngenjets)
  CLASS_MEMBER(HTTFourTauGenInfo, ic::channel, channel)
  std::vector<Dynamic2DHistoSet *> hists_;

 public:
  HTTFourTauGenInfo(std::string const& name);
  virtual ~HTTFourTauGenInfo();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
