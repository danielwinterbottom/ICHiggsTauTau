#ifndef ICHiggsTauTau_Module_NvtxWeight_h
#define ICHiggsTauTau_Module_NvtxWeight_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include <string>
#include "TH1F.h"

namespace ic {

class NvtxWeight : public ModuleBase {
 private:
  bool is_valid_;
  TH1* weights_;
  CLASS_MEMBER(NvtxWeight, TH1*, vertex_dist)
  CLASS_MEMBER(NvtxWeight, bool, print_weights)
  std::string label_;

 public:
  NvtxWeight(std::string const& name);
  NvtxWeight(std::string const& name, std::string const& label);
  virtual ~NvtxWeight();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
