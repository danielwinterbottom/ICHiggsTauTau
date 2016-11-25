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
  CLASS_MEMBER(PileupWeight, TH1*, data)
  CLASS_MEMBER(PileupWeight, TH1*, mc)
  CLASS_MEMBER(PileupWeight, bool, print_weights)
  CLASS_MEMBER(PileupWeight, bool, use_sampled_interactions)
  CLASS_MEMBER(PileupWeight, bool, weight_is_active)
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
