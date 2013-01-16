#ifndef ICHiggsTauTau_Module_HttPairSelector_h
#define ICHiggsTauTau_Module_HttPairSelector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {

class HttPairSelector : public ModuleBase {
 private:
  CLASS_MEMBER(HttPairSelector, ic::channel, channel)
  CLASS_MEMBER(HttPairSelector, std::string, pair_label)
  CLASS_MEMBER(HttPairSelector, std::string, met_label)
  CLASS_MEMBER(HttPairSelector, bool, mva_met_from_vector)
  CLASS_MEMBER(HttPairSelector, bool, use_most_isolated)
  CLASS_MEMBER(HttPairSelector, unsigned, faked_tau_selector)
  CLASS_MEMBER(HttPairSelector, bool, scale_met_for_tau)
  CLASS_MEMBER(HttPairSelector, double, tau_scale)
  CLASS_MEMBER(HttPairSelector, std::string, allowed_tau_modes)
  CLASS_MEMBER(HttPairSelector, fwlite::TFileService*, fs)
  std::vector<Dynamic2DHistoSet *> hists_;
  std::set<int> tau_mode_set_;

 public:
  HttPairSelector(std::string const& name);
  virtual ~HttPairSelector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
