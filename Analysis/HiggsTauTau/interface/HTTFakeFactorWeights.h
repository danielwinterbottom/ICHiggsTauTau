#ifndef ICHiggsTauTau_Module_HTTFakeFactorWeights_h
#define ICHiggsTauTau_Module_HTTFakeFactorWeights_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FakeFactor.h"
#include <string>

namespace ic {

class HTTFakeFactorWeights : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFakeFactorWeights, std::string, met_label)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string, jets_label)
  CLASS_MEMBER(HTTFakeFactorWeights, ic::channel, channel)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string, ditau_label)
  CLASS_MEMBER(HTTFakeFactorWeights, ic::strategy, strategy)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string, categories)
  CLASS_MEMBER(HTTFakeFactorWeights, bool, do_systematics)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string, ff_file)

 public:
  HTTFakeFactorWeights(std::string const& name);
  virtual ~HTTFakeFactorWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  std::map<std::string, std::shared_ptr<FakeFactor>> fake_factors_;
  std::vector<std::string> category_names_;

};

}


#endif
