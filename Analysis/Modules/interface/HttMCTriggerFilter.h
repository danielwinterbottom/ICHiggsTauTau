#ifndef ICHiggsTauTau_Module_HttMCTriggerFilter_h
#define ICHiggsTauTau_Module_HttMCTriggerFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>

namespace ic {

class HttMCTriggerFilter : public ModuleBase {
 private:
  CLASS_MEMBER(HttMCTriggerFilter, ic::channel, channel)
  CLASS_MEMBER(HttMCTriggerFilter, ic::mc, mc)
  CLASS_MEMBER(HttMCTriggerFilter, bool, do_obj_match)
  CLASS_MEMBER(HttMCTriggerFilter, std::string, lep1label)
  CLASS_MEMBER(HttMCTriggerFilter, std::string, lep2label)

  std::string trig_obj_label_;
  std::string elmu_filter_;
  std::string tau_filter_;
  std::string mulow_label_;
  std::string muhigh_label_;
  std::string mulow_mu_;
  std::string mulow_el_;
  std::string muhigh_mu_;
  std::string muhigh_el_;


 public:
  HttMCTriggerFilter(std::string const& name);
  virtual ~HttMCTriggerFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
