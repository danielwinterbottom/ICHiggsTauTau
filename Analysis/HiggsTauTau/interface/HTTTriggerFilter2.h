#ifndef ICHiggsTauTau_Module_HTTTriggerFilter2_h
#define ICHiggsTauTau_Module_HTTTriggerFilter2_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>
#include <fstream>
#include <set>


namespace ic {

class HTTTriggerFilter2 : public ModuleBase {
 private:
  CLASS_MEMBER(HTTTriggerFilter2, ic::channel, channel)
  CLASS_MEMBER(HTTTriggerFilter2, ic::mc, mc)
  CLASS_MEMBER(HTTTriggerFilter2, ic::era, era)
  CLASS_MEMBER(HTTTriggerFilter2, std::string, pair_label)
  CLASS_MEMBER(HTTTriggerFilter2, bool, is_data)

 public:
  HTTTriggerFilter2(std::string const& name);
  virtual ~HTTTriggerFilter2();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  unsigned totalEventsPassed;
  unsigned notMatched;

};

}

#endif
