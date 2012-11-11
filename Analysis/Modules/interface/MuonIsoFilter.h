#ifndef ICHiggsTauTau_Module_MuonIsoFilter_h
#define ICHiggsTauTau_Module_MuonIsoFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class MuonIsoFilter : public ModuleBase {
 private:
  std::string input_name_;

 public:

  MuonIsoFilter(std::string const& name, std::string input_name);
  virtual ~MuonIsoFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
