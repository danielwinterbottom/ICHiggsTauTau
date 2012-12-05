#ifndef ICHiggsTauTau_Module_HttBTagCategory_h
#define ICHiggsTauTau_Module_HttBTagCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>
#include "TEfficiency.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


namespace ic {

class HttBTagCategory : public ModuleBase {
 private:
  double jet_pt_;
  double jet_eta_;
  double btag_jet_pt_;
  double btag_jet_eta_;

  double total_btagged_;
  double pu_id_btagged_;
  double total_btagged_real_;
  double pu_id_btagged_real_;;
  std::string jets_label_;

  CLASS_MEMBER(HttBTagCategory, fwlite::TFileService*, fs)
  CLASS_MEMBER(HttBTagCategory, ic::channel, channel)
  CLASS_MEMBER(HttBTagCategory, bool, apply_special_mode)
  CLASS_MEMBER(HttBTagCategory, unsigned, special_min)
  CLASS_MEMBER(HttBTagCategory, unsigned, special_max)
  CLASS_MEMBER(HttBTagCategory, double, special_pt)
  CLASS_MEMBER(HttBTagCategory, double, special_eta)
  CLASS_MEMBER(HttBTagCategory, std::string, met_label)
  CLASS_MEMBER(HttBTagCategory, bool, do_btag_weight)



  std::vector<HttPlots *> btag_plots_;
  std::vector<double> btag_yields_;
  std::vector<HttPlots *> btag_loose_plots_;
  std::vector<double> btag_loose_yields_;

 public:
  HttBTagCategory(std::string const& name, std::string const& jets_label);
  virtual ~HttBTagCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  void PrintEff(std::string const& name, double const& num, double const& den) {
      TEfficiency eff;
      std::cout << name << ":\t\t" << num << "/" << den << " = " << (num/den) << "\t+/- " << (eff.ClopperPearson(den+0.5,num+0.5,0.683 ,1)-(num/den)) << std::endl;
  }
};

}

#endif
