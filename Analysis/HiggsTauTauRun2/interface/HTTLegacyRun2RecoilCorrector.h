#ifndef ICHiggsTauTau_Module_HTTLegacyRun2RecoilCorrector_h
#define ICHiggsTauTau_Module_HTTLegacyRun2RecoilCorrector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/RecoilCorrectorLegacyRun2.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/MEtSys.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include <string>

namespace ic {

class HTTLegacyRun2RecoilCorrector : public ModuleBase {
 private:
 // std::string jets_label_;
  bool disable_recoil_corrs;
  bool disable_met_sys;
  bool is_wjets;
  RecoilCorrectorLegacyRun2 * corrector_;
  MEtSys * metSys_;
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, std::string, met_label)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, std::string, jets_label)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, std::string, sample)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, ic::channel, channel)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, ic::strategy, strategy)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, ic::mc, mc)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, ic::era, era)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, bool, use_puppimet)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, bool, use_quantile_map)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, bool, store_boson_pt)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, int, met_scale_mode)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, int, met_res_mode)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, unsigned, njets_mode)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, bool, do_recoil)
  CLASS_MEMBER(HTTLegacyRun2RecoilCorrector, bool, embed_recoil)



 public:
  HTTLegacyRun2RecoilCorrector(std::string const& name);
  virtual ~HTTLegacyRun2RecoilCorrector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
