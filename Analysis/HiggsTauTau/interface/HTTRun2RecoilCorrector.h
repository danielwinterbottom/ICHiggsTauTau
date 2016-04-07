#ifndef ICHiggsTauTau_Module_HTTRun2RecoilCorrector_h
#define ICHiggsTauTau_Module_HTTRun2RecoilCorrector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/RecoilCorrectorRun2.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/MEtSys.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HTTRun2RecoilCorrector : public ModuleBase {
 private:
 // std::string jets_label_;
  bool disable;
  bool is_wjets;
  RecoilCorrectorRun2 * corrector_;
  MEtSys * metSys_;
  CLASS_MEMBER(HTTRun2RecoilCorrector, std::string, met_label)
  CLASS_MEMBER(HTTRun2RecoilCorrector, std::string, jets_label)
  CLASS_MEMBER(HTTRun2RecoilCorrector, std::string, sample)
  CLASS_MEMBER(HTTRun2RecoilCorrector, ic::channel, channel)
  CLASS_MEMBER(HTTRun2RecoilCorrector, ic::strategy, strategy)
  CLASS_MEMBER(HTTRun2RecoilCorrector, ic::mc, mc)
  CLASS_MEMBER(HTTRun2RecoilCorrector, ic::era, era)
  CLASS_MEMBER(HTTRun2RecoilCorrector, bool, use_quantile_map)
  CLASS_MEMBER(HTTRun2RecoilCorrector, bool, store_boson_pt)
  CLASS_MEMBER(HTTRun2RecoilCorrector, int, met_scale_mode)
  CLASS_MEMBER(HTTRun2RecoilCorrector, int, met_res_mode)



 public:
  HTTRun2RecoilCorrector(std::string const& name);
  virtual ~HTTRun2RecoilCorrector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
