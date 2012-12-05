#ifndef ICHiggsTauTau_Module_HttOneBJetCategory_h
#define ICHiggsTauTau_Module_HttOneBJetCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>
#include "TEfficiency.h"

namespace ic {

class HttOneBJetCategory : public ModuleBase {
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

  CLASS_MEMBER(HttOneBJetCategory, bool, high_pt)
  CLASS_MEMBER(HttOneBJetCategory, fwlite::TFileService*, fs)
  CLASS_MEMBER(HttOneBJetCategory, ic::channel, channel)
  CLASS_MEMBER(HttOneBJetCategory, bool, apply_special_mode)
  CLASS_MEMBER(HttOneBJetCategory, unsigned, special_min)
  CLASS_MEMBER(HttOneBJetCategory, unsigned, special_max)
  CLASS_MEMBER(HttOneBJetCategory, double, special_pt)
  CLASS_MEMBER(HttOneBJetCategory, double, special_eta)
  CLASS_MEMBER(HttOneBJetCategory, std::string, met_label)
  CLASS_MEMBER(HttOneBJetCategory, bool, do_btag_weight)



  std::vector<HttPlots *> onebjet_plots_;
  std::vector<double> onebjet_yields_;
  std::vector<HttPlots *> onebjet_loose_plots_;
  std::vector<double> onebjet_loose_yields_;

 public:
  HttOneBJetCategory(std::string const& name, std::string const& jets_label);
  virtual ~HttOneBJetCategory();

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
