#ifndef ICHiggsTauTau_Module_HttDataTriggerFilter_h
#define ICHiggsTauTau_Module_HttDataTriggerFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>
#include <fstream>

#define MEMBER_NP(type,name)                                                \
    private:                                                                \
      type name##_;                                                         \
    public:                                                                 \
      virtual HttDataTriggerFilter & set_##name(type const& name) {name##_ = name; return *this; }

namespace ic {

class HttDataTriggerFilter : public ModuleBase {
 private:
  CLASS_MEMBER(HttDataTriggerFilter, ic::channel, channel)
  MEMBER_NP(bool, do_obj_match)
  MEMBER_NP(std::string, lep1label)
  MEMBER_NP(std::string, lep2label)

  std::map<unsigned, unsigned> yields_;


 public:
  HttDataTriggerFilter(std::string const& name);
  virtual ~HttDataTriggerFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#undef MEMBER_NP

#endif
