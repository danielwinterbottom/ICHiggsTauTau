#ifndef ICHiggsTauTau_HiggsTauTau_PreselectionFilter_h
#define ICHiggsTauTau_HiggsTauTau_PreselectionFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

#include <string>

namespace ic {

class PreselectionFilter : public ModuleBase {
 private:

  CLASS_MEMBER(PreselectionFilter, ic::channel, channel)
  CLASS_MEMBER(PreselectionFilter, bool, do_preselection)
  CLASS_MEMBER(PreselectionFilter, std::string, dilepton_label)

 public:
  PreselectionFilter(std::string const& name);
  virtual ~PreselectionFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
