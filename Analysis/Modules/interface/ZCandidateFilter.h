#ifndef ICHiggsTauTau_Module_ZCandidateFilter_h
#define ICHiggsTauTau_Module_ZCandidateFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class ZCandidateFilter : public ModuleBase {
 private:
  std::string input_name_;
  double mass_low_;
  double mass_high_;
  bool do_charge_;

 public:

  ZCandidateFilter(std::string const& name, std::string input_name, 
                   double mass_low, double mass_high, bool do_charge);
  virtual ~ZCandidateFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
