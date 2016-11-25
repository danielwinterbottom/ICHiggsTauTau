#ifndef ICHiggsTauTau_Zbb_ZbbTriggerFilter_h
#define ICHiggsTauTau_Zbb_ZbbTriggerFilter_h
// C++ Standard Library
#include <string>
#include <fstream>
// Analysis Framework
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"

namespace ic {

class ZbbTriggerFilter : public ModuleBase {
 private:
  CLASS_MEMBER(ZbbTriggerFilter, std::string, elec_pairs)
  CLASS_MEMBER(ZbbTriggerFilter, std::string, muon_pairs)

 public:
  ZbbTriggerFilter(std::string const& name);
  virtual ~ZbbTriggerFilter();
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
}

#endif
