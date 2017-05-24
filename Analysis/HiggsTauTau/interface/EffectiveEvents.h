#ifndef ICHiggsTauTau_HiggsTauTau_EffectiveEvents_h
#define ICHiggsTauTau_HiggsTauTau_EffectiveEvents_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

namespace ic {

class EffectiveEvents : public ModuleBase {


 private:

  CLASS_MEMBER(EffectiveEvents, fwlite::TFileService*, fs)
  CLASS_MEMBER(EffectiveEvents, bool, do_qcd_scale_wts)
  TTree *outtree_;
  int mcsign_;
  double gen_ht_;
  
  double scale1_;
  double scale2_;
  double scale3_;
  double scale4_;
  double scale5_;
  double scale6_;
  double scale7_;
  double scale8_;
  double scale9_;

 public:
  EffectiveEvents(std::string const& name);
  virtual ~EffectiveEvents();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}
#endif
