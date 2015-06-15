#ifndef ICHiggsTauTau_HiggsTauTau_HTTCategories_h
#define ICHiggsTauTau_HiggsTauTau_HTTCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>

namespace ic {

class HTTCategories : public ModuleBase {

 private:
  CLASS_MEMBER(HTTCategories, std::string, ditau_label)
  CLASS_MEMBER(HTTCategories, std::string, met_label)
  CLASS_MEMBER(HTTCategories, std::string, jets_label)
  CLASS_MEMBER(HTTCategories, double, mass_shift)
  CLASS_MEMBER(HTTCategories, ic::channel, channel)
  CLASS_MEMBER(HTTCategories, ic::era, era)
  CLASS_MEMBER(HTTCategories, ic::strategy, strategy)
  CLASS_MEMBER(HTTCategories, bool, write_tree)
  CLASS_MEMBER(HTTCategories, bool, bjet_regression)
  CLASS_MEMBER(HTTCategories, bool, make_sync_ntuple)
  CLASS_MEMBER(HTTCategories, bool, is_embedded)
  CLASS_MEMBER(HTTCategories, std::string, sync_output_name)
  CLASS_MEMBER(HTTCategories, int, kinfit_mode )
  CLASS_MEMBER(HTTCategories, fwlite::TFileService*, fs)
 
  TTree *outtree_;
  TTree *synctree_;
  TFile *lOFile;

  // Event Properties
  double wt_;
  int run_;
  int event_;
  int lumi_;
  double rho_;
  double mc_weight_;
  double pu_weight_;
  double wt_ggh_pt_up_;
  double wt_ggh_pt_down_;
  double wt_tau_fake_up_;
  double wt_tau_fake_down_;
  double wt_tquark_up_;
  double wt_tquark_down_;
  double wt_tau_id_up_;
  double wt_tau_id_down_;
  double trigweight_1_;
  double trigweight_2_;
  double idweight_1_;
  double idweight_2_;
  double isoweight_1_;
  double isoweight_2_;
  double effweight_;
  double fakeweight_;
  double embeddedweight_;
  double signalweight_;
  bool os_;
  unsigned n_vtx_;
  unsigned n_pu_;
  double m_sv_;
  double m_vis_;
  double pt_h_;
  double eta_h_;
  double phi_h_;
  double pt_tt_;
  double mt_1_;
  double mt_2_;
  double mt_ll_;
  double pzeta_;
  double pzetavis_;
  double pzetamiss_;
  double emu_dphi_;
  double emu_csv_;
  double emu_dxy_1_;
  double emu_dxy_2_;
  double d0_1_;
  double d0_2_;
  double dz_1_;
  double dz_2_;
  double pt_1_;
  double pt_2_;
  double eta_1_;
  double eta_2_;
  double phi_1_;
  double phi_2_;
  int q_1_;
  int q_2_;
  double iso_1_;
  double iso_2_;
  double z_2_;
  double m_1_;
  double m_2_;
  double mva_1_;
  double mva_2_;
  double met_;
  double met_phi_;
  double pfmet_;
  double pfmet_phi_;
  double metCov00_;
  double metCov01_;
  double metCov10_;
  double metCov11_;
  double pfmetCov00_;
  double pfmetCov01_;
  double pfmetCov10_;
  double pfmetCov11_;

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
  double jphi_1_;    // Defined if n_jets >= 1
  double jphi_2_;    // Defined if n_jets >= 2
  double bpt_1_;     // Defined if n_bjets >= 1
  double beta_1_;    // Defined if n_bjets >= 1
  double bphi_1_;    // Defined if n_bjets >= 1
  double bcsv_1_; 
  double jet_csvpt_1_;     // Defined if n_jets >= 1
  double jet_csvEt_1_;     // Defined if n_jets >= 1
  double jet_csvpt_2_;     // Defined if n_jets >= 2
	double jet_csvpt_bb_;
	double jet_csv_dR_;
  double jet_csveta_1_;    // Defined if n_jets >= 1
  double jet_csveta_2_;    // Defined if n_jets >= 2
  double jet_csvbcsv_1_; 
  double jet_csvbcsv_2_; 

  int j1_dm_;

  double mjj_;       // Defined if n_jets >= 2
  double mjj_h_;       // Defined if n_jets >= 2
  double mbb_h_;       // Defined if n_jets >= 2
  double mjj_tt_;       // Defined if n_jets >= 2
  double m_H_best_;       // Defined if n_jets >= 2
  double m_H_chi2_best_;       // Defined if n_jets >= 2
  double pull_balance_H_best_; //Defined if n_jets >= 2
  int convergence_H_best_; //Defined if n_jets >= 2
  double m_H_hZ_;     //Defined if n_jets >= 2
  double m_H_hZ_chi2_;  //Defined if n_jets >= 2
  double pull_balance_hZ_; //Defined if n_jets >= 2
  int convergence_hZ_; //Defined if n_jets >= 2
  double m_H_Zh_;      //Defined if n_jets >= 2
  double m_H_Zh_chi2_;   //Defined if n_jets >= 2 
  double pull_balance_Zh_; //Defined if n_jets >= 2
  int convergence_Zh_; //Defined if n_jets >= 2
  double m_H_hh_;	     //Defined if n_jets >= 2
  double m_H_hh_all_;	     //Defined if n_jets >= 2
  double m_H_hh_chi2_;   //Defined if n_jets >= 2
  double pull_balance_hh_; //Defined if n_jets >= 2
  int convergence_hh_; //Defined if n_jets >= 2
  double m_bb_;	     //Defined if n_jets >= 2
  double m_bb_chi2_;   //Defined if n_jets >= 2
  double pull_balance_bb_; //Defined if n_jets >= 2
  int convergence_bb_; //Defined if n_jets >= 2
  double jdeta_;     // Defined if n_jets >= 2

  double jet_csv_mjj_;       // Defined if n_jets >= 2
  double jet_csv_deta_;     // Defined if n_jets >= 2
  double jet_csv_dphi_;     // Defined if n_jets >= 2
  double jet_csv_dtheta_;     // Defined if n_jets >= 2
  double mjj_lowpt_;       // Defined if n_lowpt_jets >= 2
  double jdeta_lowpt_;     // Defined if n_lowpt_jets >= 2
  unsigned n_jetsingap_lowpt_; // Defined if n_lowpt_jets >= 2

  unsigned n_prebjets_;
  unsigned n_jets_csv_;
  unsigned n_bjets_csv_;

  double l1_met_;
  double calo_nohf_met_;

  double em_gf_mva_;
  double em_vbf_mva_;

    // Other VBF MVA variables?

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
