#ifndef ICHiggsTauTau_Module_ZJetsControlPlots_h
#define ICHiggsTauTau_Module_ZJetsControlPlots_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>


namespace ic {

class ZJetsControlPlots : public ModuleBase {
 private:
  DynamicHistoSet *hset;
  Dynamic2DHistoSet *h2dset;

  CLASS_MEMBER(ZJetsControlPlots, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZJetsControlPlots, ic::channel, channel)
  CLASS_MEMBER(ZJetsControlPlots, std::string, met_label)



 public:
  ZJetsControlPlots(std::string const& name);
  virtual ~ZJetsControlPlots();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}


#endif
