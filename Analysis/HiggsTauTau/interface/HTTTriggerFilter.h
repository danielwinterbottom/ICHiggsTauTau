#ifndef ICHiggsTauTau_Module_HTTTriggerFilter_h
#define ICHiggsTauTau_Module_HTTTriggerFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>
#include <fstream>


namespace ic {

class HTTTriggerFilter : public ModuleBase {
 private:
  CLASS_MEMBER(HTTTriggerFilter, ic::channel, channel)
  CLASS_MEMBER(HTTTriggerFilter, ic::mc, mc)
  CLASS_MEMBER(HTTTriggerFilter, ic::era, era)
  CLASS_MEMBER(HTTTriggerFilter, ic::strategy, strategy)
  CLASS_MEMBER(HTTTriggerFilter, std::string, pair_label)
  CLASS_MEMBER(HTTTriggerFilter, bool, is_data)
  CLASS_MEMBER(HTTTriggerFilter, bool, is_embedded)
  CLASS_MEMBER(HTTTriggerFilter, bool, do_leptonplustau)
  CLASS_MEMBER(HTTTriggerFilter, bool, do_singlelepton)
  CLASS_MEMBER(HTTTriggerFilter, bool, do_singletau)
  CLASS_MEMBER(HTTTriggerFilter, bool, do_filter)

 public:
  HTTTriggerFilter(std::string const& name);
  virtual ~HTTTriggerFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
