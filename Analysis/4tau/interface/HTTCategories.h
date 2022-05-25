#ifndef ICHiggsTauTau_4tau_HTTCategories_h
#define ICHiggsTauTau_4tau_HTTCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "TRandom3.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IpCorrection.h"

#include <string>

namespace ic {

class HTTCategories : public ModuleBase {

 private:
  CLASS_MEMBER(HTTCategories, std::string, fourtau_label)
  CLASS_MEMBER(HTTCategories, ic::channel, channel)
  CLASS_MEMBER(HTTCategories, ic::era, era)
  CLASS_MEMBER(HTTCategories, bool, write_tree)
  CLASS_MEMBER(HTTCategories, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTCategories, ic::strategy, strategy)

  TTree *outtree_;
  TFile *lOFile;

  TRandom3  *rand;

  std::map<std::string, TF1*> fns_;

  struct branch_var {
      double var_double;  
      float var_float;
      branch_var& operator=(double doub){
          var_double = doub;
          var_float = static_cast<float>(doub);
          return *this;
      }
      branch_var& operator*(double sf){
          var_double*=sf;
          var_float*=sf;
          return *this;
      }
  };

  // Event Properties
  int run_;
  unsigned long long event_;
  int lumi_;
  float rho_;

  branch_var d0_1_;
  branch_var d0_2_;
  branch_var d0_3_;
  branch_var d0_4_;
  branch_var dz_1_;
  branch_var dz_2_;
  branch_var dz_3_;
  branch_var dz_4_;

  int tau_decay_mode_1_;
  int tau_decay_mode_2_;
  int tau_decay_mode_3_;
  int tau_decay_mode_4_;

  branch_var iso_1_;
  branch_var iso_2_;

  branch_var pt_1_;
  branch_var pt_2_;
  branch_var pt_3_;
  branch_var pt_4_;

  branch_var eta_1_;
  branch_var eta_2_;
  branch_var eta_3_;
  branch_var eta_4_;
  branch_var phi_1_;
  branch_var phi_2_;
  branch_var phi_3_;
  branch_var phi_4_;
  branch_var dphi_12_;
  branch_var dphi_13_;
  branch_var dphi_14_;
  branch_var dphi_23_;
  branch_var dphi_24_;
  branch_var dphi_34_;
  branch_var dR_12_;
  branch_var dR_13_;
  branch_var dR_14_;
  branch_var dR_23_;
  branch_var dR_24_;
  branch_var dR_34_;
  branch_var E_1_;
  branch_var E_2_;
  branch_var E_3_;
  branch_var E_4_;
  branch_var m_1_;
  branch_var m_2_;
  branch_var m_3_;
  branch_var m_4_;
  branch_var q_1_;
  branch_var q_2_;
  branch_var q_3_;
  branch_var q_4_;


  double deepTauVsEle_iso_1_;
  double deepTauVsEle_iso_2_;
  double deepTauVsEle_iso_3_;
  double deepTauVsEle_iso_4_;
  double deepTauVsMu_iso_1_;
  double deepTauVsMu_iso_2_;
  double deepTauVsMu_iso_3_;
  double deepTauVsMu_iso_4_;
  double deepTauVsJets_iso_1_;
  double deepTauVsJets_iso_2_;
  double deepTauVsJets_iso_3_;
  double deepTauVsJets_iso_4_;

  bool deepTauVsJets_vvvloose_1_;
  bool deepTauVsJets_vvvloose_2_;
  bool deepTauVsJets_vvvloose_3_;
  bool deepTauVsJets_vvvloose_4_;

  bool deepTauVsJets_vvloose_1_;
  bool deepTauVsJets_vvloose_2_;
  bool deepTauVsJets_vvloose_3_;
  bool deepTauVsJets_vvloose_4_;

  bool deepTauVsJets_vloose_1_;
  bool deepTauVsJets_vloose_2_;
  bool deepTauVsJets_vloose_3_;
  bool deepTauVsJets_vloose_4_;

  bool deepTauVsJets_loose_1_;
  bool deepTauVsJets_loose_2_;
  bool deepTauVsJets_loose_3_;
  bool deepTauVsJets_loose_4_;

  bool deepTauVsJets_medium_1_;
  bool deepTauVsJets_medium_2_;
  bool deepTauVsJets_medium_3_;
  bool deepTauVsJets_medium_4_;

  bool deepTauVsJets_tight_1_;
  bool deepTauVsJets_tight_2_;
  bool deepTauVsJets_tight_3_;
  bool deepTauVsJets_tight_4_;

  bool deepTauVsJets_vtight_1_;
  bool deepTauVsJets_vtight_2_;
  bool deepTauVsJets_vtight_3_;
  bool deepTauVsJets_vtight_4_;

  bool deepTauVsJets_vvtight_1_;
  bool deepTauVsJets_vvtight_2_;
  bool deepTauVsJets_vvtight_3_;
  bool deepTauVsJets_vvtight_4_;

  bool deepTauVsEle_vvvloose_1_;
  bool deepTauVsEle_vvvloose_2_;
  bool deepTauVsEle_vvvloose_3_;
  bool deepTauVsEle_vvvloose_4_;

  bool deepTauVsEle_vvloose_1_;
  bool deepTauVsEle_vvloose_2_;
  bool deepTauVsEle_vvloose_3_;
  bool deepTauVsEle_vvloose_4_;

  bool deepTauVsEle_vloose_1_;
  bool deepTauVsEle_vloose_2_;
  bool deepTauVsEle_vloose_3_;
  bool deepTauVsEle_vloose_4_;

  bool deepTauVsEle_loose_1_;
  bool deepTauVsEle_loose_2_;
  bool deepTauVsEle_loose_3_;
  bool deepTauVsEle_loose_4_;

  bool deepTauVsEle_medium_1_;
  bool deepTauVsEle_medium_2_;
  bool deepTauVsEle_medium_3_;
  bool deepTauVsEle_medium_4_;

  bool deepTauVsEle_tight_1_;
  bool deepTauVsEle_tight_2_;
  bool deepTauVsEle_tight_3_;
  bool deepTauVsEle_tight_4_;

  bool deepTauVsEle_vtight_1_;
  bool deepTauVsEle_vtight_2_;
  bool deepTauVsEle_vtight_3_;
  bool deepTauVsEle_vtight_4_;

  bool deepTauVsEle_vvtight_1_;
  bool deepTauVsEle_vvtight_2_;
  bool deepTauVsEle_vvtight_3_;
  bool deepTauVsEle_vvtight_4_;

  bool deepTauVsMu_vvvloose_1_;
  bool deepTauVsMu_vvvloose_2_;
  bool deepTauVsMu_vvvloose_3_;
  bool deepTauVsMu_vvvloose_4_;

  bool deepTauVsMu_vvloose_1_;
  bool deepTauVsMu_vvloose_2_;
  bool deepTauVsMu_vvloose_3_;
  bool deepTauVsMu_vvloose_4_;

  bool deepTauVsMu_vloose_1_;
  bool deepTauVsMu_vloose_2_;
  bool deepTauVsMu_vloose_3_;
  bool deepTauVsMu_vloose_4_;

  bool deepTauVsMu_loose_1_;
  bool deepTauVsMu_loose_2_;
  bool deepTauVsMu_loose_3_;
  bool deepTauVsMu_loose_4_;

  bool deepTauVsMu_medium_1_;
  bool deepTauVsMu_medium_2_;
  bool deepTauVsMu_medium_3_;
  bool deepTauVsMu_medium_4_;

  bool deepTauVsMu_tight_1_;
  bool deepTauVsMu_tight_2_;
  bool deepTauVsMu_tight_3_;
  bool deepTauVsMu_tight_4_;

  bool deepTauVsMu_vtight_1_;
  bool deepTauVsMu_vtight_2_;
  bool deepTauVsMu_vtight_3_;
  bool deepTauVsMu_vtight_4_;

  bool deepTauVsMu_vvtight_1_;
  bool deepTauVsMu_vvtight_2_;
  bool deepTauVsMu_vvtight_3_;
  bool deepTauVsMu_vvtight_4_;


 public:
  HTTCategories(std::string const& name);
  virtual ~HTTCategories();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
