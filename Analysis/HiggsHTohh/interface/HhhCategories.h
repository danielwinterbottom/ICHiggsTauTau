#ifndef ICHiggsTauTau_HiggsHTohh_HhhCategories_h
#define ICHiggsTauTau_HiggsHTohh_HhhCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/HHKinFit/include/HHKinFitMaster.h"

#include <string>

namespace ic {

class HhhCategories : public ModuleBase {

 private:
  std::string jets_label_;
  CLASS_MEMBER(HhhCategories, std::string, ditau_label)
  CLASS_MEMBER(HhhCategories, std::string, met_label)
  CLASS_MEMBER(HhhCategories, double, mass_shift)
  CLASS_MEMBER(HhhCategories, ic::channel, channel)
  CLASS_MEMBER(HhhCategories, ic::era, era)
  CLASS_MEMBER(HhhCategories, ic::strategy, strategy)
  CLASS_MEMBER(HhhCategories, bool, write_tree)
  CLASS_MEMBER(HhhCategories, bool, write_plots)
  CLASS_MEMBER(HhhCategories, bool, experimental)
  CLASS_MEMBER(HhhCategories, bool, kinfit_mode )
  CLASS_MEMBER(HhhCategories, fwlite::TFileService*, fs)

  std::map<std::string, bool> categories_;
  std::map<std::string, bool> selections_;
  std::map<std::string, MassPlots*> massplots_;
  std::map<std::string, double> yields_;
  std::map<std::string, CoreControlPlots*> controlplots_;
  DynamicHistoSet * misc_plots_;
  Dynamic2DHistoSet * misc_2dplots_;
  
  TTree *outtree_;

  // Event Properties
  double wt_;
  double wt_ggh_pt_up_;
  double wt_ggh_pt_down_;
  double wt_tau_fake_up_;
  double wt_tau_fake_down_;
  double wt_tquark_up_;
  double wt_tquark_down_;
  double wt_tau_id_up_;
  double wt_tau_id_down_;
  bool os_;
  unsigned n_vtx_;
  double m_sv_;
  double m_vis_;
  double pt_h_;
  double pt_tt_;
  double mt_1_;
  double mt_ll_;
  double pzeta_;
  double pzetavis_;
  double pzetamiss_;
  double emu_dphi_;
  double emu_csv_;
  double emu_dxy_1_;
  double emu_dxy_2_;
  double pt_1_;
  double pt_2_;
  //double bdt_;
  double eta_1_;
  double eta_2_;
  double iso_1_;
  double iso_2_;
  double z_2_;
  double m_2_;
  double met_;
  double met_sig_;
  double met_phi_;

  int    tau_decay_mode_;

  unsigned n_jets_;
  unsigned n_lowpt_jets_;
  unsigned n_bjets_;
  unsigned n_loose_bjets_;
  unsigned n_jetsingap_; // Defined if n_jets >= 2
  double jpt_1_;     // Defined if n_jets >= 1
  double jpt_2_;     // Defined if n_jets >= 2
  double jeta_1_;    // Defined if n_jets >= 1
  double jeta_2_;    // Defined if n_jets >= 2
  double bpt_1_;     // Defined if n_bjets >= 1
  double beta_1_;    // Defined if n_bjets >= 1
  double bcsv_1_; 
  double prebjetpt_1_;     // Defined if n_jets >= 1
  double prebjetEt_1_;     // Defined if n_jets >= 1
  double prebjetpt_2_;     // Defined if n_jets >= 2
  double prebjeteta_1_;    // Defined if n_jets >= 1
  double prebjeteta_2_;    // Defined if n_jets >= 2
  double prebjetbcsv_1_; 
  double prebjetbcsv_2_; 

  int j1_dm_;
  int prebjet1_dm_;

  double mjj_;       // Defined if n_jets >= 2
  double mjj_h_;       // Defined if n_jets >= 2
  double mjj_tt_;       // Defined if n_jets >= 2
	double m_H_;       // Defined if n_jets >= 2
  double m_H_chi2_;       // Defined if n_jets >= 2
	double pull_balance_H_; //Defined if n_jets >= 2
	int convergence_H_; //Defined if n_jets >= 2
	double m_H_hZ_;     //Defined if n_jets >= 2
	double m_H_hZ_chi2_;  //Defined if n_jets >= 2
	double pull_balance_hZ_; //Defined if n_jets >= 2
	int convergence_hZ_; //Defined if n_jets >= 2
	double m_H_Zh_;      //Defined if n_jets >= 2
	double m_H_Zh_chi2_;   //Defined if n_jets >= 2
	double pull_balance_Zh_; //Defined if n_jets >= 2
	int convergence_Zh_; //Defined if n_jets >= 2
	double m_H_hh_;	     //Defined if n_jets >= 2
	double m_H_hh_chi2_;   //Defined if n_jets >= 2
	double pull_balance_hh_; //Defined if n_jets >= 2
	int convergence_hh_; //Defined if n_jets >= 2

  double jdeta_;     // Defined if n_jets >= 2
  
  double prebjet_mjj_;       // Defined if n_jets >= 2
  double prebjet_deta_;     // Defined if n_jets >= 2
  double prebjet_dphi_;     // Defined if n_jets >= 2
  double prebjet_dtheta_;     // Defined if n_jets >= 2
  double prebjet_1_met_dphi_;     // Defined if n_jets >= 2
  double prebjet_1_met_dtheta_;     // Defined if n_jets >= 2
  double prebjet_1_lep1_dphi_;     // Defined if n_jets >= 2
  double prebjet_1_lep1_dtheta_;     // Defined if n_jets >= 2
  double prebjet_1_lep1_m_;     // Defined if n_jets >= 2
  double jet_1_met_m_;     // Defined if n_jets >= 2
  double jet_1_met_dphi_;     // Defined if n_jets >= 2
  double jet_1_met_dtheta_;     // Defined if n_jets >= 2
  double E_1_;     // Defined if n_jets >= 1

  double mjj_lowpt_;       // Defined if n_lowpt_jets >= 2
  double jdeta_lowpt_;     // Defined if n_lowpt_jets >= 2
  unsigned n_jetsingap_lowpt_; // Defined if n_lowpt_jets >= 2

  unsigned n_prebjets_;

  double l1_met_;
  double calo_nohf_met_;

  double em_gf_mva_;
  //double em_vbf_mva_;
	double em_gf_mva_bdtg_;
	double em_gf_mva_bdt_;
	double mt_bdt_2jet1tag_;
	double mt_bdt_2jet2tag_;

    // Other VBF MVA variables?

 public:
  HhhCategories(std::string const& name);
  virtual ~HhhCategories();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  bool PassesCategory(std::string const& category) const;

  void InitSelection(std::string const& selection);
  void InitCategory(std::string const& category);
  void InitMassPlots(std::string const& category);
  void FillMassPlots(std::string const& category);
  void FillYields(std::string const& category);
  void InitCoreControlPlots(std::string const& category);
  void FillCoreControlPlots(std::string const& category);

  void SetPassCategory(std::string const& category);
  void SetPassSelection(std::string const& selection);

  void Reset();


};

}


#endif
