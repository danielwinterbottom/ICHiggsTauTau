#ifndef ICHiggsTauTau_HiggsHTohh_JetTauFakeRateExample_h
#define ICHiggsTauTau_HiggsHTohh_JetTauFakeRateExample_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>

namespace ic {

class JetTauFakeRateExample : public ModuleBase {
 private:
  
  CLASS_MEMBER(JetTauFakeRateExample, bool, write_tree)
  CLASS_MEMBER(JetTauFakeRateExample, bool, write_plots)
  CLASS_MEMBER(JetTauFakeRateExample, fwlite::TFileService*, fs)
  
  TTree *outtree_;
  
  double jpt_;     
  double jeta_;     
 
 public:
  JetTauFakeRateExample(std::string const& name);
  virtual ~JetTauFakeRateExample();
  
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
