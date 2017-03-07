#ifndef ICHiggsTauTau_Module_PileupWeight_h
#define ICHiggsTauTau_Module_PileupWeight_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include <string>
#include "TH1F.h"

namespace ic {

class PileupWeight : public ModuleBase {
 private:
  bool is_valid_;
  TH1* weights_;
  TH1* weights_up_;
  TH1* weights_down_;
  CLASS_MEMBER(PileupWeight, TH1*, data)
  CLASS_MEMBER(PileupWeight, TH1*, data_up)
  CLASS_MEMBER(PileupWeight, TH1*, data_down)
  CLASS_MEMBER(PileupWeight, TH1*, mc)
  CLASS_MEMBER(PileupWeight, bool, print_weights)
  CLASS_MEMBER(PileupWeight, bool, use_sampled_interactions)
  CLASS_MEMBER(PileupWeight, bool, weight_is_active)
  CLASS_MEMBER(PileupWeight, bool, do_pu_systematic)
  std::string label_;

 public:
  PileupWeight(std::string const& name);
  PileupWeight(std::string const& name, std::string const& label);
  virtual ~PileupWeight();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
