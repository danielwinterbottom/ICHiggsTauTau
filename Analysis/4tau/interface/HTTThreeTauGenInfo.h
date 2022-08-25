#ifndef ICHiggsTauTau_Module_HTTThreeTauGenInfo_h
#define ICHiggsTauTau_Module_HTTThreeTauGenInfo_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"


#include <string>

namespace ic {
  
class HTTThreeTauGenInfo : public ModuleBase {
 private:
  CLASS_MEMBER(HTTThreeTauGenInfo, std::string, ditau_label)
  CLASS_MEMBER(HTTThreeTauGenInfo, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTThreeTauGenInfo, bool, write_plots)
  CLASS_MEMBER(HTTThreeTauGenInfo, bool, ngenjets)
  CLASS_MEMBER(HTTThreeTauGenInfo, ic::channel, channel)
  std::vector<Dynamic2DHistoSet *> hists_;

 public:
  HTTThreeTauGenInfo(std::string const& name);
  virtual ~HTTThreeTauGenInfo();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
