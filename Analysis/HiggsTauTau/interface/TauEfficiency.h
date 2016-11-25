#ifndef ICHiggsTauTau_Module_TauEfficiency_h
#define ICHiggsTauTau_Module_TauEfficiency_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {

class TauEfficiency : public ModuleBase {
 private:
  CLASS_MEMBER(TauEfficiency, ic::channel, channel)
  CLASS_MEMBER(TauEfficiency, std::string, tau_label)
  CLASS_MEMBER(TauEfficiency, std::string, gen_label)
  CLASS_MEMBER(TauEfficiency, bool, is_data)
  CLASS_MEMBER(TauEfficiency, bool, is_embedded)
  CLASS_MEMBER(TauEfficiency, bool, is_fake)
  CLASS_MEMBER(TauEfficiency, bool, gen_match)
  CLASS_MEMBER(TauEfficiency, fwlite::TFileService*, fs)
  Dynamic2DHistoSet * hists_;

 public:
  TauEfficiency(std::string const& name);
  virtual ~TauEfficiency();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
