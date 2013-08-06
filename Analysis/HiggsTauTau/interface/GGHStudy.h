#ifndef ICHiggsTauTau_Module_GGHStudy_h
#define ICHiggsTauTau_Module_GGHStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "TEfficiency.h"

#include <string>

namespace ic {

class GGHStudy : public ModuleBase {
 private:
  std::string jets_label_;
  CLASS_MEMBER(GGHStudy, fwlite::TFileService*, fs)
  CLASS_MEMBER(GGHStudy, bool, apply_hres_weight)
  DynamicHistoSet * hists_;
  TFile *ggh_weights_;
  TH1F *ggh_hist_;


  // void PrintEff(std::string const& name, double const& num, double const& den) {
  //   TEfficiency eff;
  //    std::cout << name << ":\t\t" << num << "/" << den << " = " << (num/den) << "\t+/- " << (eff.ClopperPearson(den+0.5,num+0.5,0.683 ,1)-(num/den)) << std::endl;
  // };

 public:
  GGHStudy(std::string const& name);
  virtual ~GGHStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
