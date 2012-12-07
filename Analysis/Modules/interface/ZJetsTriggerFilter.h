#ifndef ICHiggsTauTau_Module_ZJetsTriggerFilter_h
#define ICHiggsTauTau_Module_ZJetsTriggerFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>
#include <fstream>


namespace ic {

class ZJetsTriggerFilter : public ModuleBase {
 private:
  CLASS_MEMBER(ZJetsTriggerFilter, ic::channel, channel)
  CLASS_MEMBER(ZJetsTriggerFilter, std::string, pair_label)
  CLASS_MEMBER(ZJetsTriggerFilter, bool, is_data)


 public:
  ZJetsTriggerFilter(std::string const& name);
  virtual ~ZJetsTriggerFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#undef MEMBER_NP

#endif
