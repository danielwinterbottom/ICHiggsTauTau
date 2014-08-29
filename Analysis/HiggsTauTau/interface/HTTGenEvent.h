#ifndef ICHiggsTauTau_HiggsTauTau_HTTGenEvent_h
#define ICHiggsTauTau_HiggsTauTau_HTTGenEvent_h

#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/RecoilCorrector.h"
#include "HiggsTauTau/interface/HTTConfig.h"

namespace ic {

class HTTGenEvent : public ModuleBase {
 private:
  CLASS_MEMBER(HTTGenEvent, std::string, genparticle_label)
  CLASS_MEMBER(HTTGenEvent, std::string, genjet_label)

 public:
  explicit HTTGenEvent(std::string const& name);
  virtual ~HTTGenEvent();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  // virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
