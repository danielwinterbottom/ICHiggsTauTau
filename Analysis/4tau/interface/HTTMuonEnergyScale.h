#ifndef ICHiggsTauTau_4tau_HTTMuonEnergyScale_h
#define ICHiggsTauTau_4tau_HTTMuonEnergyScale_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HTTMuonEnergyScale : public ModuleBase {
 private:
 
  CLASS_MEMBER(HTTMuonEnergyScale, ic::channel, channel)
  CLASS_MEMBER(HTTMuonEnergyScale, ic::strategy, strategy)
  CLASS_MEMBER(HTTMuonEnergyScale, std::string, input_label)
  CLASS_MEMBER(HTTMuonEnergyScale, double, far_endcap)
  CLASS_MEMBER(HTTMuonEnergyScale, double, near_endcap)
  CLASS_MEMBER(HTTMuonEnergyScale, double, barrel)


 public:
  HTTMuonEnergyScale(std::string const& name);
  virtual ~HTTMuonEnergyScale();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void WriteRunScript();
};

}

#endif
