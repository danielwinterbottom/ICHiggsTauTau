#ifndef ICHiggsTauTau_Module_HTTMetStudy_h
#define ICHiggsTauTau_Module_HTTMetStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "TEfficiency.h"

#include <string>

namespace ic {

class HTTMetStudy : public ModuleBase {
 private:
  std::string jets_label_;
  CLASS_MEMBER(HTTMetStudy, std::string, dilepton_label)
  CLASS_MEMBER(HTTMetStudy, std::string, met_label)
  CLASS_MEMBER(HTTMetStudy, int, mode)
  CLASS_MEMBER(HTTMetStudy, fwlite::TFileService*, fs)

  std::vector<HttPlots *> metstudy_plots_;
  std::vector<DynamicHistoSet *> hists_;


  std::vector<double> total_yields_;
  std::vector<double> justpair_yields_;

  void PrintEff(std::string const& name, double const& num, double const& den) {
    TEfficiency eff;
     std::cout << name << ":\t\t" << num << "/" << den << " = " << (num/den) << "\t+/- " << (eff.ClopperPearson(den+0.5,num+0.5,0.683 ,1)-(num/den)) << std::endl;
  };


 public:
  HTTMetStudy(std::string const& name);
  virtual ~HTTMetStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
