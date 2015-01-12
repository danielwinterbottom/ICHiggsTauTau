#ifndef ICHiggsTauTau_HiggsTauTau_HhhMetScale_h
#define ICHiggsTauTau_HiggsTauTau_HhhMetScale_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HhhMetScale : public ModuleBase {
 private:
 
  CLASS_MEMBER(HhhMetScale, std::string, met_label)
  CLASS_MEMBER(HhhMetScale, double, scale_shift)
  CLASS_MEMBER(HhhMetScale, int, met_scale_mode)


 public:
  HhhMetScale(std::string const& name);
  virtual ~HhhMetScale();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void WriteRunScript();
};

}

#endif
