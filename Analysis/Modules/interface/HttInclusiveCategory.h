#ifndef ICHiggsTauTau_Module_HttZeroJetCategory_h
#define ICHiggsTauTau_Module_HttZeroJetCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ModuleBase.h"
#include <string>

namespace ic {

class HttZeroJetCategory : public ModuleBase {
 private:

 public:
  HttZeroJetCategory(std::string const& name);
  virtual ~HttZeroJetCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
