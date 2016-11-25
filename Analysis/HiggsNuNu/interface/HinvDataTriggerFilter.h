#ifndef ICHiggsTauTau_Module_HinvDataTriggerFilter_h
#define ICHiggsTauTau_Module_HinvDataTriggerFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>
#include <fstream>

#define MEMBER_NP(type,name)                                                \
    private:                                                                \
      type name##_;                                                         \
    public:                                                                 \
      virtual HinvDataTriggerFilter & set_##name(type const& name) {name##_ = name; return *this; }

namespace ic {

class HinvDataTriggerFilter : public ModuleBase {
 private:
  MEMBER_NP(bool, is_data)
  MEMBER_NP(std::string, trigger_path)
  MEMBER_NP(std::string, trig_obj_label)

  unsigned counter1_;
  unsigned counter2_;


 public:
  HinvDataTriggerFilter(std::string const& name);
  virtual ~HinvDataTriggerFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#undef MEMBER_NP

#endif
