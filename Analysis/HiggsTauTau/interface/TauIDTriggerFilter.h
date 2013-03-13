#ifndef ICHiggsTauTau_Module_TauIDTriggerFilter_h
#define ICHiggsTauTau_Module_TauIDTriggerFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>
#include <fstream>


namespace ic {

class TauIDTriggerFilter : public ModuleBase {
 private:
  CLASS_MEMBER(TauIDTriggerFilter, ic::channel, channel)
  CLASS_MEMBER(TauIDTriggerFilter, ic::mc, mc)
  CLASS_MEMBER(TauIDTriggerFilter, std::string, pair_label)
  CLASS_MEMBER(TauIDTriggerFilter, bool, is_data)


 public:
  TauIDTriggerFilter(std::string const& name);
  virtual ~TauIDTriggerFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
