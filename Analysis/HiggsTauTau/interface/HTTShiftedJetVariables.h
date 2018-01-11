#ifndef ICHiggsTauTau_HiggsTauTau_HTTShiftedJetVariables_h
#define ICHiggsTauTau_HiggsTauTau_HTTShiftedJetVariables_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include <string>

namespace ic {
  
class HTTShiftedJetVariables : public ModuleBase {
 private:
  CLASS_MEMBER(HTTShiftedJetVariables, std::string, jets_label)
  CLASS_MEMBER(HTTShiftedJetVariables, std::string, source)
  CLASS_MEMBER(HTTShiftedJetVariables, ic::strategy, strategy)
  
  
 public:
  HTTShiftedJetVariables(std::string const& name);
  virtual ~HTTShiftedJetVariables();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
