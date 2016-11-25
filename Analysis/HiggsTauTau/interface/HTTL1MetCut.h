#ifndef ICHiggsTauTau_Module_HTTL1MetCut_h
#define ICHiggsTauTau_Module_HTTL1MetCut_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include "TRandom3.h"
#include <string>

namespace ic {

class HTTL1MetCut : public ModuleBase {
 private:
  CLASS_MEMBER(HTTL1MetCut, std::string, l1_met_label)

  TRandom3 *rand;

 public:
  HTTL1MetCut(std::string const& name);
  virtual ~HTTL1MetCut();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
