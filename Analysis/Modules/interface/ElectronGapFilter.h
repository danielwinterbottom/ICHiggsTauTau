#ifndef ICHiggsTauTau_Module_ElectronGapFilter_h
#define ICHiggsTauTau_Module_ElectronGapFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {


class ElectronGapFilter : public ModuleBase {
 private:
  std::string input_name_;

 public:

  ElectronGapFilter(std::string const& name, std::string input_name);
  virtual ~ElectronGapFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
