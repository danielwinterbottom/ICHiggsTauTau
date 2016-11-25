#ifndef ICHiggsTauTau_Module_RunFilter_h
#define ICHiggsTauTau_Module_RunFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class RunFilter : public ModuleBase {
 private:
  std::set<int> runs_to_filter;

 public:
  RunFilter(std::string const& name);
  virtual ~RunFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  void FilterRun(int const& run) {
    runs_to_filter.insert(run);
  }
};

}

#endif
