#ifndef ICHiggsTauTau_HiggsTauTau_HTTEMuExtras_h
#define ICHiggsTauTau_HiggsTauTau_HTTEMuExtras_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class HTTEMuExtras : public ModuleBase {
 private:
 
  CLASS_MEMBER(HTTEMuExtras, std::string, electron_label)
  CLASS_MEMBER(HTTEMuExtras, bool, apply_muon_dr_cut)
  CLASS_MEMBER(HTTEMuExtras, bool, correct_pf_isolation)

 public:
  HTTEMuExtras(std::string const& name);
  virtual ~HTTEMuExtras();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
