#ifndef ICHiggsTauTau_Module_ElectronEnergyCorr_h
#define ICHiggsTauTau_Module_ElectronEnergyCorr_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>
#include "TRandom3.h"


namespace ic {


class ElectronEnergyCorr : public ModuleBase {
 private:
  std::string input_name_;
  TRandom3* rand_;

 public:

  ElectronEnergyCorr(std::string const& name, std::string input_name);
  virtual ~ElectronEnergyCorr();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  double corrVgamma(double en, double absEta, double r9, TRandom3* rr, int run, bool data = true);
};

}

#endif
