#ifndef ICHiggsTauTau_HiggsTauTau_HTTCategories_h
#define ICHiggsTauTau_HiggsTauTau_HTTCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "TRandom3.h"

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
  CLASS_MEMBER(HTTCategories, bool, is_data)
  CLASS_MEMBER(HTTCategories, bool, systematic_shift)
  CLASS_MEMBER(HTTCategories, bool, add_Hhh_variables)
  CLASS_MEMBER(HTTCategories, bool, do_HLT_Studies)
  CLASS_MEMBER(HTTCategories, bool, add_nlo_weights)
  CLASS_MEMBER(HTTCategories, std::string, sync_output_name)
  CLASS_MEMBER(HTTCategories, bool, tau_id_study)
  CLASS_MEMBER(HTTCategories, bool, optimisation_study)
  CLASS_MEMBER(HTTCategories, bool, qcd_study)
  CLASS_MEMBER(HTTCategories, bool, jetfake_study)
  CLASS_MEMBER(HTTCategories, int, kinfit_mode )
  CLASS_MEMBER(HTTCategories, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTCategories, bool, do_ff_weights)
  CLASS_MEMBER(HTTCategories, bool, do_ff_systematics)
  CLASS_MEMBER(HTTCategories, std::string, ff_categories)
  CLASS_MEMBER(HTTCategories, bool, do_qcd_scale_wts)
  CLASS_MEMBER(HTTCategories, bool, do_mssm_higgspt)
  CLASS_MEMBER(HTTCategories, bool, do_sm_scale_wts)
  CLASS_MEMBER(HTTCategories, bool, do_sm_ps_wts)
  CLASS_MEMBER(HTTCategories, bool, do_z_weights)
  CLASS_MEMBER(HTTCategories, bool, do_faketaus)
  CLASS_MEMBER(HTTCategories, bool, official_ggH)
  CLASS_MEMBER(HTTCategories, bool, trg_applied_in_mc)

  TTree *outtree_;
  TTree *synctree_;
  TFile *lOFile;

  TRandom3  *rand;

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
  float mc_weight_;
  float pu_weight_;
  double wt_btag_;
  double wt_btag_up_;
  double wt_btag_down_;
  double wt_ggh_pt_up_;
  double wt_ggh_pt_down_;
  double wt_tau_fake_up_;
  double wt_tau_fake_down_;
  double wt_tquark_up_;
  double wt_tquark_down_;
  double wt_zpt_up_;
  double wt_zpt_down_;
  double wt_tau_id_up_;
  double wt_tau_id_down_;
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

  double wt_embed_mc_yield_;
  double wt_quarkmass_;
  double wt_quarkmass_up_;
  double wt_quarkmass_down_;
  double wt_fullquarkmass_;
  // temp gen stuff
  int npNLO_;
  unsigned partons_;
  double parton_pt_;
  double parton_pt_2_;
  double parton_pt_3_;
  double parton_mjj_;
  //end of temp gen stuff
  double wt_ps_up_;
  double wt_ps_down_;
  double wt_ue_up_;
  double wt_ue_down_;
  double wt_nlo_pt_;
  double wt_tau_id_sf_;
  double nlo_pt_;
  float trigweight_1_;
  float trigweight_2_;
  double xtrg_sf_;
  double single_l_sf_;
  double xtrg_notrig_;
  double OR_notrig_;
  double wt_trig_up_1_;
  double wt_trig_up_2_;
  double wt_trig_down_1_;
  double wt_trig_down_2_;
  double wt_trig_up_;
  double wt_trig_down_;
  float idisoweight_1_;
  float idisoweight_2_;
  double idisoweight_up_1_;
  double idisoweight_down_1_;
  double idisoweight_up_2_;
  double idisoweight_down_2_;
  float trackingweight_1_;
  float trackingweight_2_;
  float isoweight_1_;
  float isoweight_2_;
  float effweight_;
  float fakeweight_;
  float embeddedweight_;
  float signalweight_;
  bool os_;
  bool dilepton_veto_;
  bool extraelec_veto_;
  bool extramuon_veto_;
  bool minimal_extraelec_veto_;
  bool minimal_extramuon_veto_;
  bool lepton_veto_;
  unsigned n_vtx_;
  bool good_vtx_;
/*  branch_var trigger_object_pt_1;
  branch_var trigger_object_eta_1;
  branch_var trigger_object_pt_2;
  branch_var trigger_object_eta_2;
*/
 float n_pu_;
  branch_var m_sv_;
  branch_var mt_sv_;
  branch_var m_vis_;
  branch_var pt_h_;
  float eta_h_;
  float phi_h_;
  branch_var pt_tt_;
  branch_var pfpt_tt_;
  branch_var pt_vis_;
  branch_var mt_1_;
  branch_var mt_lep_;
  branch_var mt_tot_;
  branch_var pfmt_tot_;
  branch_var pfmt_1_;
  branch_var puppimt_1_;
  branch_var mt_2_;
  branch_var pfmt_2_;
  double mt_ll_;
  double pfmt_ll_;
  branch_var pzeta_;
  branch_var pfpzeta_;
  branch_var puppipzeta_;
  branch_var pzetavis_;
  branch_var pzetamiss_;
  branch_var pfpzetamiss_;
  branch_var puppipzetamiss_;
  double dphi_;

  double dphi_jtt_;

  double dR_;
  double met_dphi_1_;
  double met_dphi_2_;
  double emu_csv_;
  double emu_dxy_1_;
  double emu_dxy_2_;
  double eCorr_1_;
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
  double E_1_;
  double E_2_;
  int q_1_;
  int q_2_;
  int jet_flav_1_;
  int jet_flav_2_;
  int jet_flav_3_;
  bool tag_trigger_match_1_;
  bool tag_trigger_match_2_;
  bool probe_trigger_match_1_;
  bool probe_trigger_match_2_;
  branch_var iso_1_;
  branch_var iso_2_;
  bool antiele_1_;
  bool antimu_1_;
  bool antiele_2_;
  bool antimu_2_;
  bool antimu_loose_2_;
  bool antiele_tight_2_;
  double z_1_;
  double z_2_;
  float m_1_;
  float m_2_;
  branch_var mva_1_;
  branch_var mva_2_;
  branch_var pfmet_;
  branch_var met_;
  float pfmet_phi_;
  branch_var met_phi_;
  branch_var uncorrmet_;
  branch_var uncorrmet_phi_;
  branch_var puppimet_;
  float puppimet_phi_; 
  float pfmetCov00_;
  float pfmetCov01_;
  float pfmetCov10_;
  float pfmetCov11_;
  float metCov00_;
  float metCov01_;
  float metCov10_;
  float metCov11_;
  float gen_px_;
  float gen_py_;
  float vis_px_;
  float vis_py_;

  // Tau ID variables
  float l3Hits_2 = 0;
  float lagainstElectronMVA3raw_2 = 0;
  float lbyIsolationMVA2raw_2 = 0;
  float lagainstMuonLoose2_2 = 0;
  float lagainstMuonMedium2_2 = 0;
  float lagainstMuonTight2_2 = 0;
  float lbyIsolationMVA3newDMwoLTraw_2 = 0;
  float lbyIsolationMVA3oldDMwoLTraw_2 = 0;
  float lbyIsolationMVA3newDMwLTraw_2 = 0;
  float lbyIsolationMVA3oldDMwLTraw_2 = 0;
  float lchargedIsoPtSum_2 = 0;
  float lchargedRelIsoPtSum_2 = 0;
  float lneutralIsoPtSum_2 = 0;
  float lpuCorrPtSum_2 = 0;
  branch_var lPhotonPtSum_2;
  bool ldecayModeFindingOldDMs_2 = 0;

  float l3Hits_1 = 0;
  float lagainstElectronLooseMVA_1 = 0;
  float lagainstElectronMediumMVA_1 = 0;
  float lagainstElectronTightMVA_1 = 0;
  float lagainstElectronVTightMVA_1 = 0;
  float lagainstElectronVLooseMVA_1 = 0;
  float lagainstMuonLoose3_1 = 0;
  float lagainstMuonTight3_1 = 0;
  float lbyIsolationMVA3newDMwoLTraw_1 = 0;
  float lbyIsolationMVA3oldDMwoLTraw_1 = 0;
  float lbyIsolationMVA3newDMwLTraw_1 = 0;
  float lbyIsolationMVA3oldDMwLTraw_1 = 0;
  float lchargedIsoPtSum_1 = 0;
  float lchargedRelIsoPtSum_1 = 0;
  float lneutralIsoPtSum_1 = 0;
  float lpuCorrPtSum_1 = 0;
  branch_var lPhotonPtSum_1;
  bool ldecayModeFindingOldDMs_1 = 0;
  bool lbyVLooseIsolationMVA3newDMwLT_1=0;
  bool lbyVLooseIsolationMVA3newDMwLT_2=0;
  bool lbyLooseIsolationMVA3newDMwLT_1=0;
  bool lbyLooseIsolationMVA3newDMwLT_2=0;
  bool lbyMediumIsolationMVA3newDMwLT_1=0;
  bool lbyMediumIsolationMVA3newDMwLT_2=0;
  bool lbyVTightIsolationMVA3newDMwLT_1=0;
  bool lbyVTightIsolationMVA3newDMwLT_2=0;
  bool lbyTightIsolationMVA3newDMwLT_1=0;
  bool lbyTightIsolationMVA3newDMwLT_2=0;
  bool lbyLooseCombinedIsolation_1=0;
  bool lbyLooseCombinedIsolation_2=0;
  bool lbyMediumCombinedIsolation_1=0;
  bool lbyMediumCombinedIsolation_2=0;
  bool lbyTightCombinedIsolation_1=0;
  bool lbyTightCombinedIsolation_2=0;
  bool lbyLoosePileupWeightedIsolation_1=0;
  bool lbyLoosePileupWeightedIsolation_2=0;
  bool lbyMediumPileupWeightedIsolation_1=0;
  bool lbyMediumPileupWeightedIsolation_2=0;
  bool lbyTightPileupWeightedIsolation_1=0;
  bool lbyTightPileupWeightedIsolation_2=0;
  branch_var lbyIsolationMVArun2DBnewDMwLTraw_2;
  branch_var lbyIsolationMVArun2DBoldDMwLTraw_2;
  branch_var lbyIsolationMVArun2PWnewDMwLTraw_2;
  branch_var lbyIsolationMVArun2PWoldDMwLTraw_2;
  bool lbyVVTightIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyVVTightIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyVVTightIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyVVTightIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyVTightIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyVTightIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyVTightIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyVTightIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyTightIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyTightIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyTightIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyTightIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyMediumIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyMediumIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyMediumIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyMediumIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyLooseIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyLooseIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyLooseIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyLooseIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2DBnewDMwLT_2=0;
  branch_var lbyIsolationMVArun2DBnewDMwLTraw_1;
  branch_var lbyIsolationMVArun2DBoldDMwLTraw_1;
  branch_var lbyIsolationMVArun2PWnewDMwLTraw_1;
  branch_var lbyIsolationMVArun2PWoldDMwLTraw_1;
  bool lbyVVTightIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyVVTightIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyVVTightIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyVVTightIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyVTightIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyVTightIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyVTightIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyVTightIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyTightIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyTightIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyTightIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyTightIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyMediumIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyMediumIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyMediumIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyMediumIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyLooseIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyLooseIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyLooseIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyLooseIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2DBnewDMwLT_1=0;
  float lagainstElectronLooseMVA_2 = 0;
  float lagainstElectronMediumMVA_2 = 0;
  float lagainstElectronTightMVA_2 = 0;
  float lagainstElectronVTightMVA_2 = 0;
  float lagainstElectronVLooseMVA_2 = 0;
  float lagainstMuonLoose3_2 = 0;
  float lagainstMuonTight3_2 = 0;
    
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
  unsigned n_jetsingap_; // Defined if n_jets >= 2
  unsigned n_jetsingap20_; // Defined if n_jets >= 2
  branch_var jpt_1_;     // Defined if n_jets >= 1
  branch_var jpt_2_;     // Defined if n_jets >= 2
  branch_var jeta_1_;    // Defined if n_jets >= 1
  branch_var jeta_2_;    // Defined if n_jets >= 2
  float jphi_1_;    // Defined if n_jets >= 1
  float jphi_2_;    // Defined if n_jets >= 2
  float dijetpt_;
  float centrality_;
  float jptraw_1_; 
  float jrawf_1_;
  float jptraw_2_; 
  float jrawf_2_;
  float jptunc_1_; 
  float jptunc_2_; 
  float jmva_1_; 
  float jmva_2_; 
  float jchm_1_  ;
  float jnm_1_   ;
  float jpm_1_   ;
  float jchemf_1_;
  float jnemf_1_ ;
  float jchhf_1_;
  float jnhf_1_ ;
  float jchm_2_  ;
  float jnm_2_   ;
  float jpm_2_   ;
  float jchemf_2_;
  float jnemf_2_ ;
  float jchhf_2_;
  float jnhf_2_ ;
  float jlrm_1_; 
  float jlrm_2_; 
  float jctm_1_; 
  float jctm_2_;
  float nearjpt_1_;
  branch_var bpt_1_;     // Defined if n_bjets >= 1
  branch_var beta_1_;    // Defined if n_bjets >= 1
  float bphi_1_;    // Defined if n_bjets >= 1
  float bmva_1_;
  float brawf_1_;
  branch_var bcsv_1_; 
  branch_var bpt_2_;     // Defined if n_bjets >= 2
  branch_var beta_2_;    // Defined if n_bjets >= 2
  float bphi_2_;    // Defined if n_bjets >= 2
  float bmva_2_;
  float bcsv_2_; 
  float brawf_2_;
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

  branch_var mjj_;       // Defined if n_jets >= 2
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
  branch_var jdeta_;     // Defined if n_jets >= 2
  float jdphi_;
  float sjdphi_;
  float D0_;
  float DCP_;
  unsigned opp_sides_;
  unsigned gen_match_1_;
  unsigned gen_match_2_;
  double gen_match_1_pt_;
  double gen_match_2_pt_;
  double gen_sjdphi_;
  double gen_mjj_;
  double gen_m_;
  double gen_pt_;
  int tauFlag_1_;
  int tauFlag_2_;
  /*double leading_lepton_match_pt_;
  double subleading_lepton_match_pt_;
  double leading_lepton_match_DR_;
  double subleading_lepton_match_DR_;*/

  double aco_angle_;
  double aco_angle_mod_;
  int cp_channel_;
  double cp_sign_;

  double aco_angle_1_;
  double aco_angle_2_;
  double aco_angle_3_;
  double aco_angle_4_;
  double aco_angle_5_;
  double aco_angle_6_;
  double aco_sign_1_;
  double aco_sign_2_;
  double aco_sign_3_;
  double aco_sign_4_;
  double aco_sign_5_;
  double lead_pt_1_, lead_pt_2_;
  double mass0_=-1, mass1_=-1, mass2_=-1;


  double rand_;
  double y_1_1_;
  double y_1_2_;
  double y_2_2_;
  double y_3_2_;
  double y_4_2_;
  double cp_sign_1_;
  double cp_sign_2_;
  double cp_sign_3_;
  double cp_sign_4_;

  double wt_cp_sm_;
  double wt_cp_ps_;
  double wt_cp_mm_;
 
  float mvadm_rho_1_;
  float mvadm_a1_1_;
  float mvadm_pi_1_;
  float mvadm_other_1_;
  float mvadm_3pipi0_1_;
  float mvadm_rho_2_;
  float mvadm_a1_2_;
  float mvadm_pi_2_;
  float mvadm_other_2_;
  float mvadm_3pipi0_2_;
  float mvadm_3pi_1_;
  float mvadm_3pi_2_;
  int mva_dm_1_, mva_dm_2_;

  float IC_BDT_max_score_;
  float IC_BDT_max_index_;

  double jet_csv_mjj_;       // Defined if n_jets >= 2
  double jet_csv_deta_;     // Defined if n_jets >= 2
  double jet_csv_dphi_;     // Defined if n_jets >= 2
  double jet_csv_dtheta_;     // Defined if n_jets >= 2
  double mjj_lowpt_;       // Defined if n_lowpt_jets >= 2
  double jdeta_lowpt_;     // Defined if n_lowpt_jets >= 2
  unsigned n_jetsingap_lowpt_; // Defined if n_lowpt_jets >= 2
  float lbyMediumIsolationMVArun2DBoldDMwLT_1_;
  float lbyMediumIsolationMVArun2DBnewDMwLT_1_;
  float lbyTightIsolationMVArun2DBoldDMwLT_1_;
  float lbyTightIsolationMVArun2DBnewDMwLT_1_;
  float lbyMediumIsolationMVArun2DBoldDMwLT_2_;
  float lbyMediumIsolationMVArun2DBnewDMwLT_2_;
  float lbyTightIsolationMVArun2DBoldDMwLT_2_;
  float lbyTightIsolationMVArun2DBnewDMwLT_2_;

  unsigned n_prebjets_;
  unsigned n_jets_csv_;
  unsigned n_bjets_csv_;

  double em_gf_mva_;
  double em_vbf_mva_;

  double wt_ff_dmbins_1_;
  double wt_ff_dmbins_2_; 
 
  double wt_ff_1_;
  double wt_ff_2_;

  double wt_ff_dmbins_qcd_stat_l_pt_down_1_;
  double wt_ff_dmbins_qcd_stat_l_pt_up_1_;
  double wt_ff_dmbins_qcd_stat_met_down_1_;
  double wt_ff_dmbins_qcd_stat_met_up_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm0_down_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm0_up_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm10_down_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm10_up_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm11_down_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm11_up_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm1_down_2_;
  double wt_ff_dmbins_qcd_stat_njet0_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_njet0_dm1_up_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm0_down_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm0_up_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm10_down_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm10_up_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm11_down_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm11_up_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm1_down_2_;
  double wt_ff_dmbins_qcd_stat_njet1_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_njet1_dm1_up_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm0_down_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm0_down_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm0_up_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm0_up_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm10_down_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm10_down_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm10_up_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm10_up_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm11_down_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm11_down_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm11_up_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm11_up_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm1_down_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm1_down_2_;
  double wt_ff_dmbins_qcd_stat_njet2_dm1_up_1_;
  double wt_ff_dmbins_qcd_stat_njet2_dm1_up_2_;
  double wt_ff_dmbins_qcd_syst_down_1_;
  double wt_ff_dmbins_qcd_syst_up_1_;
  double wt_ff_dmbins_wjets_stat_l_pt_down_1_;
  double wt_ff_dmbins_wjets_stat_l_pt_up_1_;
  double wt_ff_dmbins_wjets_stat_met_down_1_;
  double wt_ff_dmbins_wjets_stat_met_up_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_njet0_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_njet1_dm1_up_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm0_down_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm0_up_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm10_down_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm10_up_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm11_down_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm11_up_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm1_down_1_;
  double wt_ff_dmbins_wjets_stat_njet2_dm1_up_1_;
  double wt_ff_dmbins_wjets_syst_down_1_;
  double wt_ff_dmbins_wjets_syst_up_1_;
  double wt_ff_qcd_stat_l_pt_down_1_;
  double wt_ff_qcd_stat_l_pt_up_1_;
  double wt_ff_qcd_stat_met_down_1_;
  double wt_ff_qcd_stat_met_down_2_;
  double wt_ff_qcd_stat_met_up_1_;
  double wt_ff_qcd_stat_met_up_2_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_2_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_2_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_2_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_2_;
  double wt_ff_qcd_stat_njet0_mvadm10_down_1_;
  double wt_ff_qcd_stat_njet0_mvadm10_down_2_;
  double wt_ff_qcd_stat_njet0_mvadm10_up_1_;
  double wt_ff_qcd_stat_njet0_mvadm10_up_2_;
  double wt_ff_qcd_stat_njet0_mvadm11_down_1_;
  double wt_ff_qcd_stat_njet0_mvadm11_down_2_;
  double wt_ff_qcd_stat_njet0_mvadm11_up_1_;
  double wt_ff_qcd_stat_njet0_mvadm11_up_2_;
  double wt_ff_qcd_stat_njet0_mvadm1_down_1_;
  double wt_ff_qcd_stat_njet0_mvadm1_down_2_;
  double wt_ff_qcd_stat_njet0_mvadm1_up_1_;
  double wt_ff_qcd_stat_njet0_mvadm1_up_2_;
  double wt_ff_qcd_stat_njet0_mvadm2_down_1_;
  double wt_ff_qcd_stat_njet0_mvadm2_down_2_;
  double wt_ff_qcd_stat_njet0_mvadm2_up_1_;
  double wt_ff_qcd_stat_njet0_mvadm2_up_2_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_2_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_2_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_2_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_2_;
  double wt_ff_qcd_stat_njet1_mvadm10_down_1_;
  double wt_ff_qcd_stat_njet1_mvadm10_down_2_;
  double wt_ff_qcd_stat_njet1_mvadm10_up_1_;
  double wt_ff_qcd_stat_njet1_mvadm10_up_2_;
  double wt_ff_qcd_stat_njet1_mvadm11_down_1_;
  double wt_ff_qcd_stat_njet1_mvadm11_down_2_;
  double wt_ff_qcd_stat_njet1_mvadm11_up_1_;
  double wt_ff_qcd_stat_njet1_mvadm11_up_2_;
  double wt_ff_qcd_stat_njet1_mvadm1_down_1_;
  double wt_ff_qcd_stat_njet1_mvadm1_down_2_;
  double wt_ff_qcd_stat_njet1_mvadm1_up_1_;
  double wt_ff_qcd_stat_njet1_mvadm1_up_2_;
  double wt_ff_qcd_stat_njet1_mvadm2_down_1_;
  double wt_ff_qcd_stat_njet1_mvadm2_down_2_;
  double wt_ff_qcd_stat_njet1_mvadm2_up_1_;
  double wt_ff_qcd_stat_njet1_mvadm2_up_2_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_1_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_2_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_1_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_2_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_1_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_2_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_1_;
  double wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_2_;
  double wt_ff_qcd_stat_njet2_mvadm10_down_1_;
  double wt_ff_qcd_stat_njet2_mvadm10_down_2_;
  double wt_ff_qcd_stat_njet2_mvadm10_up_1_;
  double wt_ff_qcd_stat_njet2_mvadm10_up_2_;
  double wt_ff_qcd_stat_njet2_mvadm11_down_1_;
  double wt_ff_qcd_stat_njet2_mvadm11_down_2_;
  double wt_ff_qcd_stat_njet2_mvadm11_up_1_;
  double wt_ff_qcd_stat_njet2_mvadm11_up_2_;
  double wt_ff_qcd_stat_njet2_mvadm1_down_1_;
  double wt_ff_qcd_stat_njet2_mvadm1_down_2_;
  double wt_ff_qcd_stat_njet2_mvadm1_up_1_;
  double wt_ff_qcd_stat_njet2_mvadm1_up_2_;
  double wt_ff_qcd_stat_njet2_mvadm2_down_1_;
  double wt_ff_qcd_stat_njet2_mvadm2_down_2_;
  double wt_ff_qcd_stat_njet2_mvadm2_up_1_;
  double wt_ff_qcd_stat_njet2_mvadm2_up_2_;
  double wt_ff_qcd_syst_down_1_;
  double wt_ff_qcd_syst_down_2_;
  double wt_ff_qcd_syst_up_1_;
  double wt_ff_qcd_syst_up_2_;
  double wt_ff_wjets_stat_l_pt_down_1_;
  double wt_ff_wjets_stat_l_pt_up_1_;
  double wt_ff_wjets_stat_met_down_1_;
  double wt_ff_wjets_stat_met_up_1_;
  double wt_ff_wjets_stat_njet0_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_njet0_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_njet0_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_njet0_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_njet0_mvadm10_down_1_;
  double wt_ff_wjets_stat_njet0_mvadm10_up_1_;
  double wt_ff_wjets_stat_njet0_mvadm11_down_1_;
  double wt_ff_wjets_stat_njet0_mvadm11_up_1_;
  double wt_ff_wjets_stat_njet0_mvadm1_down_1_;
  double wt_ff_wjets_stat_njet0_mvadm1_up_1_;
  double wt_ff_wjets_stat_njet0_mvadm2_down_1_;
  double wt_ff_wjets_stat_njet0_mvadm2_up_1_;
  double wt_ff_wjets_stat_njet1_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_njet1_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_njet1_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_njet1_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_njet1_mvadm10_down_1_;
  double wt_ff_wjets_stat_njet1_mvadm10_up_1_;
  double wt_ff_wjets_stat_njet1_mvadm11_down_1_;
  double wt_ff_wjets_stat_njet1_mvadm11_up_1_;
  double wt_ff_wjets_stat_njet1_mvadm1_down_1_;
  double wt_ff_wjets_stat_njet1_mvadm1_up_1_;
  double wt_ff_wjets_stat_njet1_mvadm2_down_1_;
  double wt_ff_wjets_stat_njet1_mvadm2_up_1_;
  double wt_ff_wjets_stat_njet2_mvadm0_sig_gt3_down_1_;
  double wt_ff_wjets_stat_njet2_mvadm0_sig_gt3_up_1_;
  double wt_ff_wjets_stat_njet2_mvadm0_sig_lt3_down_1_;
  double wt_ff_wjets_stat_njet2_mvadm0_sig_lt3_up_1_;
  double wt_ff_wjets_stat_njet2_mvadm10_down_1_;
  double wt_ff_wjets_stat_njet2_mvadm10_up_1_;
  double wt_ff_wjets_stat_njet2_mvadm11_down_1_;
  double wt_ff_wjets_stat_njet2_mvadm11_up_1_;
  double wt_ff_wjets_stat_njet2_mvadm1_down_1_;
  double wt_ff_wjets_stat_njet2_mvadm1_up_1_;
  double wt_ff_wjets_stat_njet2_mvadm2_down_1_;
  double wt_ff_wjets_stat_njet2_mvadm2_up_1_;
  double wt_ff_wjets_syst_down_1_;
  double wt_ff_wjets_syst_down_2_;
  double wt_ff_wjets_syst_up_1_;
  double wt_ff_wjets_syst_up_2_; 
  double wt_ff_qcd_1_;
  double wt_ff_wjets_1_;
  double wt_ff_dmbins_qcd_1_;
  double wt_ff_dmbins_wjets_1_;
  double wt_ff_dmbins_ttbar_syst_up_1_;
  double wt_ff_dmbins_ttbar_syst_down_1_;
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
  double wt_ff_us_wjets_syst_closure_up_1_;
  double wt_ff_us_wjets_syst_closure_down_1_;
  double wt_ff_us_ttbar_syst_closure_up_1_;
  double wt_ff_us_ttbar_syst_closure_down_1_;
  double wt_ff_us_qcd_stat_unc1_up_1_;
  double wt_ff_us_qcd_stat_unc1_down_1_;
  double wt_ff_us_qcd_stat_unc2_up_1_;
  double wt_ff_us_qcd_stat_unc2_down_1_;
  double wt_ff_us_wjets_stat_unc1_up_1_;
  double wt_ff_us_wjets_stat_unc1_down_1_;
  double wt_ff_us_wjets_stat_unc2_up_1_;
  double wt_ff_us_wjets_stat_unc2_down_1_;
  double wt_ff_us_ttbar_stat_unc1_up_1_;
  double wt_ff_us_ttbar_stat_unc1_down_1_;
  double wt_ff_us_ttbar_stat_unc2_up_1_;
  double wt_ff_us_ttbar_stat_unc2_down_1_;
 
  bool trg_singleelectron_;
  bool trg_singlemuon_;
  bool trg_muonelectron_;
  bool trg_muonelectron_1_;
  bool trg_muonelectron_2_;
  bool trg_muonelectron_3_;
  bool trg_doubletau_;
  bool trg_vbfdoubletau_;
  bool trg_singletau_1_;
  bool trg_singletau_2_;
  bool trg_mutaucross_;
  bool trg_etaucross_;
  
  bool flagMETFilter_;
  
  double wt_zpt_stat_m400pt0_up; 
  double wt_zpt_stat_m400pt40_up; 
  double wt_zpt_stat_m400pt80_up; 
  double wt_zpt_stat_m400pt0_down;   
  double wt_zpt_stat_m400pt40_down;  
  double wt_zpt_stat_m400pt80_down;  
  double wt_zpt_esup;
  double wt_zpt_esdown;
  double wt_zpt_ttup;
  double wt_zpt_ttdown;
  
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
  
  double wt_scale_et_0jet_;  
  double wt_scale_et_boosted_;
  double wt_scale_et_vbf_;   
  double wt_scale_mt_0jet_;  
  double wt_scale_mt_boosted_;
  double wt_scale_mt_vbf_;   
  double wt_scale_em_0jet_;  
  double wt_scale_em_boosted_;
  double wt_scale_em_vbf_;   
  double wt_scale_tt_0jet_;  
  double wt_scale_tt_boosted_;
  double wt_scale_tt_vbf_;
  double wt_qcdscale_up_;
  double wt_qcdscale_down_; 

  double wt_z_mjj_;
  double wt_z_mjj_down_;
  double wt_z_mjj_up_;
  
  double spjdphi_;
  double min_hj_deta_;
  double pjdeta_;
  double pjahdeta_;
  double pjbhdeta_;
  int prob_region_;
  unsigned n_pjets_;
  double tau_pt_1_;
  double tau_pt_2_;
  unsigned tau_id_1_;
  unsigned tau_id_2_;
  int tau_dm_1_;
  int tau_dm_2_;
  unsigned tau_vloose_1_;
  unsigned tau_vloose_2_;
  double wt_dysoup_;

  double tau1_trgeff_mc_;
  double tau2_trgeff_mc_;

  bool tau_id_olddm_vloose_1_ ; 
  bool tau_id_newdm_vloose_1_ ;
  bool tau_id_olddm_loose_1_  ;
  bool tau_id_newdm_loose_1_  ;
  bool tau_id_olddm_medium_1_ ;
  bool tau_id_newdm_medium_1_ ;
  bool tau_id_olddm_tight_1_  ;
  bool tau_id_newdm_tight_1_  ;
  bool tau_id_olddm_vtight_1_ ;
  bool tau_id_newdm_vtight_1_ ;
  bool tau_id_olddm_vvtight_1_;
  bool tau_id_newdm_vvtight_1_;
  bool tau_id_olddm_vloose_2_ ;
  bool tau_id_newdm_vloose_2_ ;
  bool tau_id_olddm_loose_2_  ;
  bool tau_id_newdm_loose_2_  ;
  bool tau_id_olddm_medium_2_ ;
  bool tau_id_newdm_medium_2_ ;
  bool tau_id_olddm_tight_2_  ;
  bool tau_id_newdm_tight_2_  ;
  bool tau_id_olddm_vtight_2_ ;
  bool tau_id_newdm_vtight_2_ ;
  bool tau_id_olddm_vvtight_2_;
  bool tau_id_newdm_vvtight_2_;

  double wt_prefire_;
  double wt_prefire_up_;
  double wt_prefire_down_;

  // deepTau ID
  double deepTauVsJets_iso_1_;
  double deepTauVsJets_iso_2_;
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
  //

  double mt_1_nomu_, mu_pt_;
  double rho_dphi_, rho_deta_;
  double gen_met_, fake_met_;

  unsigned ngenjets_;
  double ip_mag_1_, ip_mag_2_, ip_sig_1_, ip_sig_2_;
  double q_tot_1_, q_tot_2_;

  double looseiso_wt_;

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

  double wt_mg_nnlops_;
  double wt_ph_nnlops_;

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
