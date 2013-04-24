#ifndef ICHiggsTauTau_Module_HTTL1MetCorrector_h
#define ICHiggsTauTau_Module_HTTL1MetCorrector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {

class HTTL1MetCorrector : public ModuleBase {
 private:
  // CLASS_MEMBER(HTTL1MetCorrector, ic::channel, channel)
  CLASS_MEMBER(HTTL1MetCorrector, std::string, l1_met_label)
  CLASS_MEMBER(HTTL1MetCorrector, std::string, uncorrected_calo_met_label)
  CLASS_MEMBER(HTTL1MetCorrector, std::string, corrected_calo_met_label)

 public:
  HTTL1MetCorrector(std::string const& name);
  virtual ~HTTL1MetCorrector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
