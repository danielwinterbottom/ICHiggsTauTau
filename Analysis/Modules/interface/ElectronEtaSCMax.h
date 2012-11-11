#ifndef ICHiggsTauTau_Module_ElectronEtaSCMax_h
#define ICHiggsTauTau_Module_ElectronEtaSCMax_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {


class ElectronEtaSCMax : public ModuleBase {
 private:
  std::string input_name_;
  double max_;

 public:

  ElectronEtaSCMax(std::string const& name, std::string input_name, double const& max);
  virtual ~ElectronEtaSCMax();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
