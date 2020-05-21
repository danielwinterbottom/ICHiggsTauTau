#ifndef ICHiggsTauTau_HiggsTauTauRun2_HTTSmearScale_h
#define ICHiggsTauTau_HiggsTauTauRun2_HTTSmearScale_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include <string>

namespace ic {

class HTTSmearScale : public ModuleBase {
 private:
 
  CLASS_MEMBER(HTTSmearScale, ic::channel, channel)
  CLASS_MEMBER(HTTSmearScale, ic::strategy, strategy)
  CLASS_MEMBER(HTTSmearScale, std::string, input_label)
  CLASS_MEMBER(HTTSmearScale, unsigned, e_unc_mode)

 public:
  HTTSmearScale(std::string const& name);
  virtual ~HTTSmearScale();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void WriteRunScript();
};

}

#endif
