#ifndef ICHiggsTauTau_Module_TriggerInfo_h
#define ICHiggsTauTau_Module_TriggerInfo_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/JsonTools.h"
#include <string>

namespace ic {

struct TriggerPathInfo {
  int first_run;
  int last_run;
  int lowest_prescale;
  int highest_prescale;
  std::map <unsigned, unsigned> yield_per_run;
  std::map <unsigned, bool> unprescaled_per_run;


  TriggerPathInfo()
      : first_run(-1),
        last_run(-1),
        lowest_prescale(-1),
        highest_prescale(-1) {}
};

/**
 * Prints summary information about triggers in data
 *
 */
class TriggerInfo : public ModuleBase {
 private:
  CLASS_MEMBER(TriggerInfo, std::string, triggerpaths_label)
  CLASS_MEMBER(TriggerInfo, std::string, eventinfo_label)
  CLASS_MEMBER(TriggerInfo, std::string, output_file)
  Json::Value output_;
  std::map<std::string, std::map<int, TriggerPathInfo>> info_;

 public:
  TriggerInfo(std::string const& name);
  virtual ~TriggerInfo();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent* event);
  virtual int PostAnalysis();
};
}

#endif
