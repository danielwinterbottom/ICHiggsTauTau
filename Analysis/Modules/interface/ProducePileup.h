#ifndef ICHiggsTauTau_Module_ProducePileup_h
#define ICHiggsTauTau_Module_ProducePileup_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "TFile.h"
#include "TH1D.h"



#include <string>

namespace ic {

class ProducePileup : public ModuleBase {
 private:
  std::set<unsigned> events_;
  TH1D* hist_;
  TFile* file_;

  CLASS_MEMBER(ProducePileup, unsigned, bins);
  CLASS_MEMBER(ProducePileup, double, max);

 public:
  ProducePileup(std::string const& name);
  virtual ~ProducePileup();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();



};

}

#endif
