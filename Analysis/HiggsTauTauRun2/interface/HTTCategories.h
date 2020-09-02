#ifndef ICHiggsTauTau_HiggsTauTauRun2_HTTCategories_h
#define ICHiggsTauTau_HiggsTauTauRun2_HTTCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "TRandom3.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IpCorrection.h"
#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"

#include <string>

namespace ic {

class HTTCategories : public ModuleBase {

 private:
  CLASS_MEMBER(HTTCategories, std::string, ditau_label)
  CLASS_MEMBER(HTTCategories, std::string, met_label)
  CLASS_MEMBER(HTTCategories, std::string, jets_label)
  CLASS_MEMBER(HTTCategories, ic::channel, channel)
  CLASS_MEMBER(HTTCategories, ic::era, era)
  CLASS_MEMBER(HTTCategories, ic::strategy, strategy)
  CLASS_MEMBER(HTTCategories, bool, write_tree)
  CLASS_MEMBER(HTTCategories, bool, make_sync_ntuple)
  CLASS_MEMBER(HTTCategories, bool, make_mva_ntuple)
  CLASS_MEMBER(HTTCategories, bool, is_embedded)
  CLASS_MEMBER(HTTCategories, bool, is_data)
  CLASS_MEMBER(HTTCategories, bool, systematic_shift)
  CLASS_MEMBER(HTTCategories, std::string, sync_output_name)
  CLASS_MEMBER(HTTCategories, std::string, mva_output_name)
  CLASS_MEMBER(HTTCategories, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTCategories, bool, do_ff_weights)
  CLASS_MEMBER(HTTCategories, bool, do_ff_systematics)
  CLASS_MEMBER(HTTCategories, bool, do_qcd_scale_wts)
  CLASS_MEMBER(HTTCategories, bool, do_mssm_higgspt)
  CLASS_MEMBER(HTTCategories, bool, do_sm_ps_wts)
  CLASS_MEMBER(HTTCategories, bool, do_faketaus)
  CLASS_MEMBER(HTTCategories, bool, trg_applied_in_mc)

  TTree *outtree_;
  TTree *synctree_;
  TTree *mvatree_;
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
  branch_var wt_;
  int run_;
  unsigned long long event_;
  int lumi_;
  float rho_;
  float pu_weight_;
  double wt_btag_;
  double wt_btag_up_;
  double wt_btag_down_;
  double wt_btag_real_up_;
  double wt_btag_real_down_;
  double wt_btag_fake_up_;
  double wt_btag_fake_down_;
  double wt_tau_fake_up_;
  double wt_tau_fake_down_;
  double wt_tquark_up_;
  double wt_tquark_down_;
  double wt_zpt_up_;
  double wt_zpt_down_;
  double wt_efake_rate_up_;
  double wt_efake_rate_down_;
  double wt_mfake_rate_up_;
  double wt_mfake_rate_down_;

  double wt_em_qcd_;
  double wt_em_qcd_extrapdown_;
  double wt_em_qcd_extrapup_;
  double wt_em_qcd_njets0_unc1_up_;
  double wt_em_qcd_njets0_unc1_down_;
  double wt_em_qcd_njets0_unc2_up_;
  double wt_em_qcd_njets0_unc2_down_;
  double wt_em_qcd_njets1_unc1_up_;
  double wt_em_qcd_njets1_unc1_down_;
  double wt_em_qcd_njets1_unc2_up_;
  double wt_em_qcd_njets1_unc2_down_;
  double wt_em_qcd_njets2_unc1_up_;
  double wt_em_qcd_njets2_unc1_down_;
  double wt_em_qcd_njets2_unc2_up_;
  double wt_em_qcd_njets2_unc2_down_;

  double wt_quarkmass_;
  double wt_fullquarkmass_;
  double wt_ue_up_;
  double wt_ue_down_;
  float trigweight_1_;
  float trigweight_2_;
  double xtrg_sf_;
  double single_l_sf_;
  double et_trg_;
  double et_trg_or_;
  double et_trg_cross_;
  double mvadm_idiso_et_;
  double xtrg_notrig_;
  double OR_notrig_;
  float idisoweight_1_;
  float idisoweight_2_;
  float trackingweight_1_;
  float trackingweight_2_;
  float wt_embedding_yield_;
  float isoweight_1_;
  float isoweight_2_;
  float effweight_;
  bool os_;
  bool dilepton_veto_;
  bool extraelec_veto_;
  bool extramuon_veto_;
  bool lepton_veto_;
  unsigned n_vtx_;
  float n_pu_;
  branch_var m_sv_;
  branch_var mt_sv_;
  branch_var m_vis_;
  branch_var pt_h_;
  float eta_h_;
  float phi_h_;
  branch_var pt_tt_;
  branch_var pt_vis_;
  branch_var mt_1_;
  branch_var mt_lep_;
  branch_var mt_tot_;
  branch_var mt_2_;
  double mt_ll_;
  branch_var pzeta_;
  branch_var pzetavis_;
  branch_var pzetamiss_;
  double dphi_;

  double dphi_jtt_;
  double shifted_dphi_jtt_;
  //double dphi_jtt_smear_;
  //double smear_jet_delta_;
  //double shifted_dphi_jtt_smear_;
  //float sjdphi_smear_;
  //double residual_pt_;
  //double residual_phi_;
  //double dphi_residual_tt_;

  double dR_;
  double met_dphi_1_;
  double met_dphi_2_;

  double newmet_dphi_1_;
  double newmet_dphi_2_;
  double newmet_;
  double qcd_frac_score_, w_frac_score_;
  double w_frac_, qcd_frac_, ttbar_frac_;

  branch_var d0_1_;
  branch_var d0_2_;
  branch_var dz_1_;
  branch_var dz_2_;
  branch_var pt_1_;
  branch_var pt_2_;
  branch_var eta_1_;
  branch_var eta_2_;
  branch_var phi_1_;
  branch_var phi_2_;
  double raw_pt_1_;
  double E_1_;
  double E_2_;
  int q_1_;
  int q_2_;
  int jet_flav_1_;
  int jet_flav_2_;
  int jet_flav_3_;
  double jet_pt_1_;
  double jet_pt_2_;
  branch_var iso_1_;
  branch_var iso_2_;
  float m_1_;
  float m_2_;
  branch_var met_;
  branch_var met_phi_;
  branch_var uncorrmet_;
  branch_var uncorrmet_phi_;
  float metCov00_;
  float metCov01_;
  float metCov10_;
  float metCov11_;
  float gen_px_;
  float gen_py_;
  float vis_px_;
  float vis_py_;

  int tau_decay_mode_1_;
  int tau_decay_mode_2_;

  int tau_mva_decay_mode_1_;
  int tau_mva_decay_mode_2_;

  unsigned n_jets_;
  unsigned n_lowpt_jets_;
  unsigned n_bjets_;
  unsigned n_loose_bjets_;
  unsigned n_btag_;
  unsigned n_loose_btag_;
  branch_var jpt_1_;     // Defined if n_jets >= 1
  branch_var jpt_2_;     // Defined if n_jets >= 2
  branch_var jeta_1_;    // Defined if n_jets >= 1
  branch_var jeta_2_;    // Defined if n_jets >= 2
  float jphi_1_;    // Defined if n_jets >= 1
  float jphi_2_;    // Defined if n_jets >= 2
  float dijetpt_;
  float jmva_1_; 
  float jmva_2_; 

  branch_var bpt_1_;     // Defined if n_bjets >= 1
  branch_var beta_1_;    // Defined if n_bjets >= 1
  float bphi_1_;    // Defined if n_bjets >= 1
  float bmva_1_;
  branch_var bcsv_1_; 
  branch_var bpt_2_;     // Defined if n_bjets >= 2
  branch_var beta_2_;    // Defined if n_bjets >= 2
  float bphi_2_;    // Defined if n_bjets >= 2
  float bmva_2_;
  float bcsv_2_; 

  branch_var mjj_;       // Defined if n_jets >= 2
  branch_var jdeta_;     // Defined if n_jets >= 2
  float jdphi_;
  float sjdphi_;
  unsigned gen_match_1_;
  unsigned gen_match_2_;
  double gen_match_1_pt_;
  double gen_match_2_pt_;
  double gen_m_;
  double gen_pt_;
  int tauFlag_1_;
  int tauFlag_2_;


  double aco_sign_rand_;
  double aco_angle_rand_;
  double aco_sign_;
  double aco_angle_1_;
  double aco_angle_2_;
  double aco_angle_3_;
  double aco_angle_4_;
  double aco_angle_5_;
  double aco_angle_6_;
  double aco_angle_7_;
  double lead_pt_1_, lead_pt_2_;
  double mass0_=-1, mass1_=-1, mass2_=-1;
  double alpha1_1_,alpha1_2_, alpha2_1_, alpha2_2_;

  double rand_;
  double y_1_1_;
  double y_1_2_;
  double y_2_2_;
  double y_3_2_;
  double y_4_2_;

  double wt_cp_sm_;
  double wt_cp_ps_;
  double wt_cp_mm_;
  double wt_cp_sm_alt_;
  double wt_cp_ps_alt_;
  double wt_cp_mm_alt_;
  double wt_cp_prod_sm_;
  double wt_cp_prod_ps_;
  double wt_cp_prod_mm_;

  double gen_phitt_;
  double gen_phitt_2_;

  float IC_BDT_max_score_;
  int IC_BDT_max_index_;

  double wt_ff_dmbins_1_;
  double wt_ff_1_;
  double wt_ff_qcd_l_pt_down_1_;
  double wt_ff_qcd_l_pt_up_1_;
  double wt_ff_qcd_met_down_1_;
  double wt_ff_qcd_met_up_1_;
  double wt_ff_ttbar_met_up_1_;
  double wt_ff_ttbar_met_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm10_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm10_up_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm11_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm11_up_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm1_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm1_up_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm2_down_1_;
  double wt_ff_qcd_stat_unc1_njet0_mvadm2_up_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm10_down_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm10_up_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm11_down_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm11_up_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm1_down_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm1_up_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm2_down_1_;
  double wt_ff_qcd_stat_unc1_njet1_mvadm2_up_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm10_down_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm10_up_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm11_down_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm11_up_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm1_down_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm1_up_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm2_down_1_;
  double wt_ff_qcd_stat_unc1_njet2_mvadm2_up_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm10_down_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm10_up_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm11_down_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm11_up_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm1_down_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm1_up_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm2_down_1_;
  double wt_ff_qcd_stat_unc2_njet0_mvadm2_up_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm10_down_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm10_up_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm11_down_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm11_up_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm1_down_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm1_up_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm2_down_1_;
  double wt_ff_qcd_stat_unc2_njet1_mvadm2_up_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm10_down_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm10_up_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm11_down_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm11_up_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm1_down_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm1_up_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm2_down_1_;
  double wt_ff_qcd_stat_unc2_njet2_mvadm2_up_1_;

  double wt_ff_qcd_syst_down_1_;
  double wt_ff_qcd_syst_up_1_;
  double wt_ff_wjets_l_pt_down_1_;
  double wt_ff_wjets_l_pt_up_1_;
  double wt_ff_wjets_met_down_1_;
  double wt_ff_wjets_met_up_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm10_down_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm10_up_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm11_down_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm11_up_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm1_down_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm1_up_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm2_down_1_;
  double wt_ff_wjets_stat_unc1_njet0_mvadm2_up_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm10_down_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm10_up_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm11_down_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm11_up_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm1_down_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm1_up_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm2_down_1_;
  double wt_ff_wjets_stat_unc1_njet1_mvadm2_up_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm10_down_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm10_up_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm11_down_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm11_up_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm1_down_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm1_up_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm2_down_1_;
  double wt_ff_wjets_stat_unc1_njet2_mvadm2_up_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm10_down_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm10_up_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm11_down_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm11_up_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm1_down_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm1_up_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm2_down_1_;
  double wt_ff_wjets_stat_unc2_njet0_mvadm2_up_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm10_down_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm10_up_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm11_down_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm11_up_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm1_down_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm1_up_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm2_down_1_;
  double wt_ff_wjets_stat_unc2_njet1_mvadm2_up_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm10_down_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm10_up_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm11_down_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm11_up_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm1_down_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm1_up_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm2_down_1_;
  double wt_ff_wjets_stat_unc2_njet2_mvadm2_up_1_;

  double wt_ff_wjets_syst_down_1_;
  double wt_ff_wjets_syst_up_1_;
  double wt_ff_qcd_1_;
  double wt_ff_wjets_1_;
  double wt_ff_ttbar_1_;
  double wt_ff_ttbar_syst_up_1_;
  double wt_ff_ttbar_syst_down_1_;

  double wt_ff_us_1_;
  double wt_ff_us_qcd_syst_osss_up_1_;
  double wt_ff_us_qcd_syst_osss_down_1_;
  double wt_ff_us_wjets_syst_mt_unc1_up_1_;
  double wt_ff_us_wjets_syst_mt_unc1_down_1_;
  double wt_ff_us_wjets_syst_mt_unc2_up_1_;
  double wt_ff_us_wjets_syst_mt_unc2_down_1_;
  double wt_ff_us_qcd_syst_closure_up_1_;
  double wt_ff_us_qcd_syst_closure_down_1_;
  double wt_ff_us_qcd_syst_closure_low_up_1_;
  double wt_ff_us_qcd_syst_closure_low_down_1_;
  double wt_ff_us_wjets_syst_closure_low_up_1_;
  double wt_ff_us_wjets_syst_closure_low_down_1_;
  double wt_ff_us_ttbar_syst_closure_low_up_1_;
  double wt_ff_us_ttbar_syst_closure_low_down_1_;
  double wt_ff_us_qcd_syst_closure_high_up_1_;
  double wt_ff_us_qcd_syst_closure_high_down_1_;
  double wt_ff_us_wjets_syst_closure_high_up_1_;
  double wt_ff_us_wjets_syst_closure_high_down_1_;
  double wt_ff_us_ttbar_syst_closure_high_up_1_;
  double wt_ff_us_ttbar_syst_closure_high_down_1_;
  double wt_ff_us_qcd_stat_njets0_unc1_up_1_;
  double wt_ff_us_qcd_stat_njets0_unc1_down_1_;
  double wt_ff_us_qcd_stat_njets0_unc2_up_1_;
  double wt_ff_us_qcd_stat_njets0_unc2_down_1_;
  double wt_ff_us_qcd_stat_njets1_unc1_up_1_;
  double wt_ff_us_qcd_stat_njets1_unc1_down_1_;
  double wt_ff_us_qcd_stat_njets1_unc2_up_1_;
  double wt_ff_us_qcd_stat_njets1_unc2_down_1_;
  double wt_ff_us_qcd_stat_njets2_unc1_up_1_;
  double wt_ff_us_qcd_stat_njets2_unc1_down_1_;
  double wt_ff_us_qcd_stat_njets2_unc2_up_1_;
  double wt_ff_us_qcd_stat_njets2_unc2_down_1_;
  double wt_ff_us_wjets_stat_njets0_unc1_up_1_;
  double wt_ff_us_wjets_stat_njets0_unc1_down_1_;
  double wt_ff_us_wjets_stat_njets0_unc2_up_1_;
  double wt_ff_us_wjets_stat_njets0_unc2_down_1_;
  double wt_ff_us_wjets_stat_njets1_unc1_up_1_;
  double wt_ff_us_wjets_stat_njets1_unc1_down_1_;
  double wt_ff_us_wjets_stat_njets1_unc2_up_1_;
  double wt_ff_us_wjets_stat_njets1_unc2_down_1_;
  double wt_ff_us_wjets_stat_njets2_unc1_up_1_;
  double wt_ff_us_wjets_stat_njets2_unc1_down_1_;
  double wt_ff_us_wjets_stat_njets2_unc2_up_1_;
  double wt_ff_us_wjets_stat_njets2_unc2_down_1_;
  double wt_ff_us_ttbar_stat_unc1_up_1_;
  double wt_ff_us_ttbar_stat_unc1_down_1_;
  double wt_ff_us_ttbar_stat_unc2_up_1_;
  double wt_ff_us_ttbar_stat_unc2_down_1_;

  double wt_ff_us_qcd_stat_njets0_up_1_;
  double wt_ff_us_qcd_stat_njets0_down_1_;
  double wt_ff_us_qcd_stat_njets1_up_1_;
  double wt_ff_us_qcd_stat_njets1_down_1_;
  double wt_ff_us_qcd_stat_njets2_up_1_;
  double wt_ff_us_qcd_stat_njets2_down_1_;
  double wt_ff_us_qcd_syst_closure_njets0_up_1_;
  double wt_ff_us_qcd_syst_closure_njets0_down_1_;
  double wt_ff_us_qcd_syst_closure_njets1_up_1_;
  double wt_ff_us_qcd_syst_closure_njets1_down_1_;
  double wt_ff_us_qcd_syst_closure_njets2_up_1_;
  double wt_ff_us_qcd_syst_closure_njets2_down_1_;

  // HPS DM FF uncerts

  double wt_ff_dmbins_qcd_l_pt_down_1_;
  double wt_ff_dmbins_qcd_l_pt_up_1_;
  double wt_ff_dmbins_qcd_met_down_1_;
  double wt_ff_dmbins_qcd_met_up_1_;
  double wt_ff_dmbins_ttbar_met_up_1_;
  double wt_ff_dmbins_ttbar_met_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet0_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet1_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_unc1_njet2_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet0_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet1_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_unc2_njet2_dm1_up_1_;

  double wt_ff_dmbins_qcd_syst_down_1_;
  double wt_ff_dmbins_qcd_syst_up_1_;
  double wt_ff_dmbins_wjets_l_pt_down_1_;
  double wt_ff_dmbins_wjets_l_pt_up_1_;
  double wt_ff_dmbins_wjets_met_down_1_;
  double wt_ff_dmbins_wjets_met_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet0_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet1_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_unc1_njet2_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet0_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet1_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_unc2_njet2_dm1_up_1_;

  double wt_ff_dmbins_wjets_syst_down_1_;
  double wt_ff_dmbins_wjets_syst_up_1_;
  double wt_ff_dmbins_qcd_1_;
  double wt_ff_dmbins_wjets_1_;
  double wt_ff_dmbins_ttbar_1_;
  double wt_ff_dmbins_ttbar_syst_up_1_;
  double wt_ff_dmbins_ttbar_syst_down_1_;
 
  bool trg_singleelectron_;
  bool trg_singlemuon_;
  bool trg_muonelectron_;
  bool trg_doubletau_;
  bool trg_vbfdoubletau_;
  bool trg_singletau_1_;
  bool trg_singletau_2_;
  bool trg_mutaucross_;
  bool trg_etaucross_;
  bool trg_etau_matched_;
  
  double scale1_;
  double scale2_;
  double scale3_;
  double scale4_;
  double scale5_;
  double scale6_;
  double scale7_;
  double scale8_;
  double scale9_;
  
  double wt_ggh_t_;
  double wt_ggh_b_;
  double wt_ggh_i_;
  double wt_ggH_t_;
  double wt_ggH_b_;
  double wt_ggH_i_;
  double wt_ggA_t_;
  double wt_ggA_b_;
  double wt_ggA_i_;
  
  double wt_qcdscale_up_;
  double wt_qcdscale_down_;
  double wt_ps_isr_up_  ;
  double wt_ps_isr_down_;
  double wt_ps_fsr_up_;
  double wt_ps_fsr_down_;
  
  double tau_pt_1_;
  int tau_dm_1_;
  double wt_dysoup_;
  double wt_wsoup_;

  double wt_prefire_;
  double wt_prefire_up_;
  double wt_prefire_down_;

  // deepTau ID
  double deepTauVsEle_iso_1_;
  double deepTauVsEle_iso_2_;
  double deepTauVsMu_iso_1_;
  double deepTauVsMu_iso_2_;

  bool deepTauVsJets_vvvloose_1_;
  bool deepTauVsJets_vvvloose_2_;
  bool deepTauVsJets_vvloose_1_;
  bool deepTauVsJets_vvloose_2_;
  bool deepTauVsJets_vloose_1_;
  bool deepTauVsJets_vloose_2_;
  bool deepTauVsJets_loose_1_;
  bool deepTauVsJets_loose_2_;
  bool deepTauVsJets_medium_1_;
  bool deepTauVsJets_medium_2_;
  bool deepTauVsJets_tight_1_;
  bool deepTauVsJets_tight_2_;
  bool deepTauVsJets_vtight_1_;
  bool deepTauVsJets_vtight_2_;
  bool deepTauVsJets_vvtight_1_;
  bool deepTauVsJets_vvtight_2_;

  bool deepTauVsEle_vvvloose_1_;
  bool deepTauVsEle_vvvloose_2_;
  bool deepTauVsEle_vvloose_1_;
  bool deepTauVsEle_vvloose_2_;
  bool deepTauVsEle_vloose_1_;
  bool deepTauVsEle_vloose_2_;
  bool deepTauVsEle_loose_1_;
  bool deepTauVsEle_loose_2_;
  bool deepTauVsEle_medium_1_;
  bool deepTauVsEle_medium_2_;
  bool deepTauVsEle_tight_1_;
  bool deepTauVsEle_tight_2_;
  bool deepTauVsEle_vtight_1_;
  bool deepTauVsEle_vtight_2_;
  bool deepTauVsEle_vvtight_1_;
  bool deepTauVsEle_vvtight_2_;

  bool deepTauVsMu_vvvloose_1_;
  bool deepTauVsMu_vvvloose_2_;
  bool deepTauVsMu_vvloose_1_;
  bool deepTauVsMu_vvloose_2_;
  bool deepTauVsMu_vloose_1_;
  bool deepTauVsMu_vloose_2_;
  bool deepTauVsMu_loose_1_;
  bool deepTauVsMu_loose_2_;
  bool deepTauVsMu_medium_1_;
  bool deepTauVsMu_medium_2_;
  bool deepTauVsMu_tight_1_;
  bool deepTauVsMu_tight_2_;
  bool deepTauVsMu_vtight_1_;
  bool deepTauVsMu_vtight_2_;
  bool deepTauVsMu_vvtight_1_;
  bool deepTauVsMu_vvtight_2_;

  double mt_1_nomu_, mu_pt_;

  unsigned ngenjets_, gen_sjdphi_;
  double ip_mag_1_, ip_mag_2_, ip_sig_1_, ip_sig_2_;
  double ipx_1_;
  double ipy_1_;
  double ipz_1_;
  double ipx_2_;
  double ipy_2_;
  double ipz_2_;

  double ipx_uncorr_1_;
  double ipy_uncorr_1_;
  double ipz_uncorr_1_;
  double ipx_uncorr_2_;
  double ipy_uncorr_2_;
  double ipz_uncorr_2_;
  double ip_sig_uncorr_1_;
  double ip_sig_uncorr_2_;

  double gen_ipx_1_;
  double gen_ipy_1_;
  double gen_ipz_1_;
  double gen_ipx_2_;
  double gen_ipy_2_;
  double gen_ipz_2_;
  bool use_refitted_vertex_;
  double pvx_;
  double pvy_;
  double pvz_;
  double q_tot_1_, q_tot_2_;

  double wt_tau_id_extra_;

  double wt_tau_id_dm0_up_;
  double wt_tau_id_dm1_up_;
  double wt_tau_id_dm10_up_;
  double wt_tau_id_dm11_up_;  
  double wt_tau_id_dm0_down_;
  double wt_tau_id_dm1_down_;
  double wt_tau_id_dm10_down_;
  double wt_tau_id_dm11_down_;
  double wt_tau_id_pt_bin1_up_;
  double wt_tau_id_pt_bin2_up_;
  double wt_tau_id_pt_bin3_up_;
  double wt_tau_id_pt_bin4_up_;
  double wt_tau_id_pt_bin5_up_;
  double wt_tau_id_pt_bin1_down_;
  double wt_tau_id_pt_bin2_down_;
  double wt_tau_id_pt_bin3_down_;
  double wt_tau_id_pt_bin4_down_;
  double wt_tau_id_pt_bin5_down_;
  double wt_tau_trg_dm0_up_;
  double wt_tau_trg_dm1_up_;
  double wt_tau_trg_dm10_up_;
  double wt_tau_trg_dm11_up_;
  double wt_tau_trg_dm0_down_;
  double wt_tau_trg_dm1_down_;
  double wt_tau_trg_dm10_down_;
  double wt_tau_trg_dm11_down_;
  double wt_tau_trg_ic_;
  double wt_tau_trg_mvadm_;
  double wt_tau_trg_mvadm0_up_;
  double wt_tau_trg_mvadm1_up_;
  double wt_tau_trg_mvadm2_up_;
  double wt_tau_trg_mvadm10_up_;
  double wt_tau_trg_mvadm11_up_;
  double wt_tau_trg_mvadm0_down_;
  double wt_tau_trg_mvadm1_down_;
  double wt_tau_trg_mvadm2_down_;
  double wt_tau_trg_mvadm10_down_;
  double wt_tau_trg_mvadm11_down_;

  double wt_tau_id_mvadm_;
  double wt_tau_id_lowpt_mvadm0_up_;
  double wt_tau_id_lowpt_mvadm1_up_;
  double wt_tau_id_lowpt_mvadm2_up_;
  double wt_tau_id_lowpt_mvadm10_up_;
  double wt_tau_id_lowpt_mvadm11_up_;
  double wt_tau_id_highpt_mvadm0_up_;
  double wt_tau_id_highpt_mvadm1_up_;
  double wt_tau_id_highpt_mvadm2_up_;
  double wt_tau_id_highpt_mvadm10_up_;
  double wt_tau_id_highpt_mvadm11_up_;
  double wt_tau_id_lowpt_mvadm0_down_;
  double wt_tau_id_lowpt_mvadm1_down_;
  double wt_tau_id_lowpt_mvadm2_down_;
  double wt_tau_id_lowpt_mvadm10_down_;
  double wt_tau_id_lowpt_mvadm11_down_;
  double wt_tau_id_highpt_mvadm0_down_;
  double wt_tau_id_highpt_mvadm1_down_;
  double wt_tau_id_highpt_mvadm2_down_;
  double wt_tau_id_highpt_mvadm10_down_;
  double wt_tau_id_highpt_mvadm11_down_;

  double wt_mg_nnlops_;
  double wt_ph_nnlops_;

  double ip_sig_1_raw_;
  double ip_sig_2_raw_;
  double ip_sig_1_up_;
  double ip_sig_2_up_;
  double ip_sig_1_down_;
  double ip_sig_2_down_;

  bool a1_flag_;
  bool a1_flag_2_;

 public:
  HTTCategories(std::string const& name);
  virtual ~HTTCategories();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  IpCorrection ipCorrector = IpCorrection();
  IpCorrection ipCorrectorEle = IpCorrection();

};

}


#endif
