#ifndef ICHiggsTauTau_Module_WJetsWeights_h
#define ICHiggsTauTau_Module_WJetsWeights_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "TTree.h"
#include "TFile.h"



#include <string>

namespace ic {

class WJetsWeights : public ModuleBase {
 private:
  CLASS_MEMBER(WJetsWeights, fwlite::TFileService*, fs)
  DynamicHistoSet* hists_;
  std::vector<double> counts_;
 public:
  WJetsWeights(std::string const& name);
  virtual ~WJetsWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
