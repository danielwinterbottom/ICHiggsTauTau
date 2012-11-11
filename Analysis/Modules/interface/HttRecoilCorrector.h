#ifndef ICHiggsTauTau_Module_HttRecoilCorrector_h
#define ICHiggsTauTau_Module_HttRecoilCorrector_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/RecoilCorrector.h"

#include <string>

namespace ic {

class HttRecoilCorrector : public ModuleBase {
 private:
  std::string jets_label_;
  bool disable;
  RecoilCorrector * corrector_;
  bool is_wjets_;
  std::vector<unsigned> boson_id_;
  CLASS_MEMBER(HttRecoilCorrector, std::string, dilepton_label)
  CLASS_MEMBER(HttRecoilCorrector, std::string, met_label)
  CLASS_MEMBER(HttRecoilCorrector, std::string, sample)
  CLASS_MEMBER(HttRecoilCorrector, int, mode)
  CLASS_MEMBER(HttRecoilCorrector, int, era)
  CLASS_MEMBER(HttRecoilCorrector, bool, is_2012)



 public:
  HttRecoilCorrector(std::string const& name);
  virtual ~HttRecoilCorrector();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
