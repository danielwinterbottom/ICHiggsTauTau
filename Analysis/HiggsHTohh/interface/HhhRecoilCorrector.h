#ifndef ICHiggsTauTau_Module_HhhRecoilCorrector_h
#define ICHiggsTauTau_Module_HhhRecoilCorrector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/RecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HhhRecoilCorrector : public ModuleBase {
 private:
  std::string jets_label_;
  bool disable;
  RecoilCorrector * corrector_;
  bool is_wjets_;
  bool is_ttbar_;
  std::vector<unsigned> boson_id_;
  CLASS_MEMBER(HhhRecoilCorrector, std::string, dilepton_label)
  CLASS_MEMBER(HhhRecoilCorrector, std::string, met_label)
  CLASS_MEMBER(HhhRecoilCorrector, std::string, sample)
  CLASS_MEMBER(HhhRecoilCorrector, ic::channel, channel)
  CLASS_MEMBER(HhhRecoilCorrector, ic::strategy, strategy)
  CLASS_MEMBER(HhhRecoilCorrector, ic::mc, mc)
  CLASS_MEMBER(HhhRecoilCorrector, ic::era, era)
  CLASS_MEMBER(HhhRecoilCorrector, bool, w_hack)
  CLASS_MEMBER(HhhRecoilCorrector, int, met_scale_mode)



 public:
  HhhRecoilCorrector(std::string const& name);
  virtual ~HhhRecoilCorrector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
