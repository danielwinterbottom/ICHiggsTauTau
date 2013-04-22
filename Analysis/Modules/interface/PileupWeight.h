#ifndef ICHiggsTauTau_Module_PileupWeight_h
#define ICHiggsTauTau_Module_PileupWeight_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>
#include "TH1F.h"

#define MEMBER_NP(type,name)                                                \
    private:                                                                \
      type name##_;                                                         \
    public:                                                                 \
      virtual PileupWeight & set_##name(type const& name) {name##_ = name; return *this; }

namespace ic {

class PileupWeight : public ModuleBase {
 private:
  bool is_valid_;
  //std::vector<double> weights_;
  TH1* weights_;
  MEMBER_NP(TH1*, data)
  MEMBER_NP(TH1*, mc)
  MEMBER_NP(bool, print_weights)

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

#undef MEMBER_NP

}

#endif
