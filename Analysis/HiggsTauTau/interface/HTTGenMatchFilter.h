#ifndef ICHiggsTauTau_Module_HTTGenMatchFilter_h
#define ICHiggsTauTau_Module_HTTGenMatchFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {
  
class HTTGenMatchFilter : public ModuleBase {
 private:
  CLASS_MEMBER(HTTGenMatchFilter, std::string, input_vec_label)
  CLASS_MEMBER(HTTGenMatchFilter, std::string, output_vec_label)
  CLASS_MEMBER(HTTGenMatchFilter, ic::mcorigin, gen_match)
 public:
  HTTGenMatchFilter(std::string const& name);
  virtual ~HTTGenMatchFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
