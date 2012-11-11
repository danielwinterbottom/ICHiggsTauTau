#ifndef ICHiggsTauTau_Module_MuonTightFilter_h
#define ICHiggsTauTau_Module_MuonTightFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class MuonTightFilter : public ModuleBase {
 private:
  std::string input_name_;

 public:

  MuonTightFilter(std::string const& name, std::string input_name);
  virtual ~MuonTightFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
