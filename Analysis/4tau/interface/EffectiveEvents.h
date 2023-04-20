#ifndef ICHiggsTauTau_4tau_EffectiveEvents_h
#define ICHiggsTauTau_4tau_EffectiveEvents_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

namespace ic {

class EffectiveEvents : public ModuleBase {


 private:

  CLASS_MEMBER(EffectiveEvents, fwlite::TFileService*, fs)
  CLASS_MEMBER(EffectiveEvents, bool, do_qcd_scale_wts)
  TTree *outtree_;
  
  int mcsign_;
  double gen_ht_;

  double wt_cp_sm_;
  double wt_cp_ps_;
  double wt_cp_mm_;

  double wt_cp_prod_sm_;
  double wt_cp_prod_ps_;
  double wt_cp_prod_mm_; 

  int run_;
  unsigned long long event_;
  int lumi_;

  int prev_run_;
  unsigned long long prev_event_;
  int prev_lumi_;
 
  double scale1_;
  double scale2_;
  double scale3_;
  double scale4_;
  double scale5_;
  double scale6_;
  double scale7_;
  double scale8_;
  double scale9_;

  float wt_pdf_;
  float wt_alphas_up_;
  float wt_alphas_down_;

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
