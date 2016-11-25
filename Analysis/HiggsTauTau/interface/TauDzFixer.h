#ifndef ICHiggsTauTau_HiggsTauTau_TauDzFixer_h
#define ICHiggsTauTau_HiggsTauTau_TauDzFixer_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class TauDzFixer : public ModuleBase {
 private:
 
  CLASS_MEMBER(TauDzFixer, std::string, tau_label)
  CLASS_MEMBER(TauDzFixer, std::string, vertex_label)

 public:
  TauDzFixer(std::string const& name);
  virtual ~TauDzFixer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
