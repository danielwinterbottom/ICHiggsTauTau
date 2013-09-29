#ifndef ICHiggsTauTau_Module_BTagCheck_h
#define ICHiggsTauTau_Module_BTagCheck_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"


#include <string>

namespace ic {

class BTagCheck : public ModuleBase {
 private:
  CLASS_MEMBER(BTagCheck, fwlite::TFileService*, fs)
  Dynamic2DHistoSet * hists_;

 public:
  BTagCheck(std::string const& name);
  virtual ~BTagCheck();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
