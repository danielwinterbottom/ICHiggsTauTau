#ifndef ICHiggsTauTau_HiggsTauTau_HTTEnergyScale_h
#define ICHiggsTauTau_HiggsTauTau_HTTEnergyScale_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HTTEnergyScale : public ModuleBase {
 private:
 
  CLASS_MEMBER(HTTEnergyScale, ic::channel, channel)
  CLASS_MEMBER(HTTEnergyScale, ic::strategy, strategy)
  CLASS_MEMBER(HTTEnergyScale, bool , moriond_corrections)
  CLASS_MEMBER(HTTEnergyScale, std::string, input_label)
  CLASS_MEMBER(HTTEnergyScale, double, shift)
  CLASS_MEMBER(HTTEnergyScale, double, shift_endcap)


 public:
  HTTEnergyScale(std::string const& name);
  virtual ~HTTEnergyScale();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void WriteRunScript();
};

}

#endif
