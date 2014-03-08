#ifndef ICHiggsTauTau_Zbb_ZbbPairSelector_h
#define ICHiggsTauTau_Zbb_ZbbPairSelector_h
// C++ Standard Library
#include <string>
// CMSSW
#include "PhysicsTools/FWLite/interface/TFileService.h"
// Analysis Framework
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"

namespace ic {

class ZbbPairSelector : public ModuleBase {
 private:
  CLASS_MEMBER(ZbbPairSelector, std::string, elec_pairs)
  CLASS_MEMBER(ZbbPairSelector, std::string, muon_pairs)
  CLASS_MEMBER(ZbbPairSelector, std::string, output_label)

  unsigned n_ee;
  unsigned n_mm;

 public:
  ZbbPairSelector(std::string const& name);
  virtual ~ZbbPairSelector();
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
}

#endif
