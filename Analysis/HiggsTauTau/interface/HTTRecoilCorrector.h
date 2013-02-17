#ifndef ICHiggsTauTau_Module_HTTRecoilCorrector_h
#define ICHiggsTauTau_Module_HTTRecoilCorrector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/RecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HTTRecoilCorrector : public ModuleBase {
 private:
  std::string jets_label_;
  bool disable;
  RecoilCorrector * corrector_;
  bool is_wjets_;
  std::vector<unsigned> boson_id_;
  CLASS_MEMBER(HTTRecoilCorrector, std::string, dilepton_label)
  CLASS_MEMBER(HTTRecoilCorrector, std::string, met_label)
  CLASS_MEMBER(HTTRecoilCorrector, std::string, sample)
  CLASS_MEMBER(HTTRecoilCorrector, ic::channel, channel)
  CLASS_MEMBER(HTTRecoilCorrector, ic::strategy, strategy)
  CLASS_MEMBER(HTTRecoilCorrector, ic::mc, mc)
  CLASS_MEMBER(HTTRecoilCorrector, ic::era, era)
  CLASS_MEMBER(HTTRecoilCorrector, bool, w_hack)



 public:
  HTTRecoilCorrector(std::string const& name);
  virtual ~HTTRecoilCorrector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
