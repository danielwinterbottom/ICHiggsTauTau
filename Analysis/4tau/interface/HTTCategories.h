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
  CLASS_MEMBER(HTTCategories, std::string, met_label)
  CLASS_MEMBER(HTTCategories, std::string, jets_label)
  CLASS_MEMBER(HTTCategories, std::string, filename)
  CLASS_MEMBER(HTTCategories, ic::channel, channel)
  CLASS_MEMBER(HTTCategories, ic::era, era)
  CLASS_MEMBER(HTTCategories, bool, is_data)
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

  int prev_run_;
  unsigned long long prev_event_;
  int prev_lumi_;

  float rho_;

  branch_var wt_;

  double wt_wsoup_;
  double wt_dysoup_;
  double wt_mc_sign_; 

  double wt_prefire_;
  double wt_prefire_up_;
  double wt_prefire_down_;

  double gen_match_1_;
  double gen_match_2_;
  double gen_match_3_;
  double gen_match_4_;

  double idisoweight_1_;
  double idisoweight_2_;
  double idisoweight_3_;
  double idisoweight_4_;

  double idisoweight_ratio_1_up_;
  double idisoweight_ratio_2_up_;
  double idisoweight_ratio_3_up_;
  double idisoweight_ratio_4_up_;
  double idisoweight_ratio_1_down_;
  double idisoweight_ratio_2_down_;
  double idisoweight_ratio_3_down_;
  double idisoweight_ratio_4_down_;

  double idisoweight_ratio_1_uncert0_up_;
  double idisoweight_ratio_2_uncert0_up_;
  double idisoweight_ratio_3_uncert0_up_;
  double idisoweight_ratio_4_uncert0_up_;

  double idisoweight_ratio_1_uncert0_down_; 
  double idisoweight_ratio_2_uncert0_down_; 
  double idisoweight_ratio_3_uncert0_down_; 
  double idisoweight_ratio_4_uncert0_down_; 

  double idisoweight_ratio_1_uncert1_up_; 
  double idisoweight_ratio_2_uncert1_up_;
  double idisoweight_ratio_3_uncert1_up_;
  double idisoweight_ratio_4_uncert1_up_;

  double idisoweight_ratio_1_uncert1_down_; 
  double idisoweight_ratio_2_uncert1_down_; 
  double idisoweight_ratio_3_uncert1_down_; 
  double idisoweight_ratio_4_uncert1_down_; 

  double idisoweight_ratio_1_syst_all_eras_up_; 
  double idisoweight_ratio_2_syst_all_eras_up_; 
  double idisoweight_ratio_3_syst_all_eras_up_; 
  double idisoweight_ratio_4_syst_all_eras_up_; 

  double idisoweight_ratio_1_syst_all_eras_down_; 
  double idisoweight_ratio_2_syst_all_eras_down_; 
  double idisoweight_ratio_3_syst_all_eras_down_; 
  double idisoweight_ratio_4_syst_all_eras_down_; 

  double idisoweight_ratio_1_syst_year_up_; 
  double idisoweight_ratio_2_syst_year_up_; 
  double idisoweight_ratio_3_syst_year_up_; 
  double idisoweight_ratio_4_syst_year_up_; 

  double idisoweight_ratio_1_syst_year_down_; 
  double idisoweight_ratio_2_syst_year_down_; 
  double idisoweight_ratio_3_syst_year_down_; 
  double idisoweight_ratio_4_syst_year_down_; 

  double idisoweight_ratio_1_syst_dm_year_up_; 
  double idisoweight_ratio_2_syst_dm_year_up_; 
  double idisoweight_ratio_3_syst_dm_year_up_; 
  double idisoweight_ratio_4_syst_dm_year_up_; 

  double idisoweight_ratio_1_syst_dm_year_down_; 
  double idisoweight_ratio_2_syst_dm_year_down_; 
  double idisoweight_ratio_3_syst_dm_year_down_; 
  double idisoweight_ratio_4_syst_dm_year_down_; 

  double idisoweight_ratio_1_highpt_up_; 
  double idisoweight_ratio_2_highpt_up_; 
  double idisoweight_ratio_3_highpt_up_; 
  double idisoweight_ratio_4_highpt_up_; 

  double idisoweight_ratio_1_highpt_down_; 
  double idisoweight_ratio_2_highpt_down_; 
  double idisoweight_ratio_3_highpt_down_; 
  double idisoweight_ratio_4_highpt_down_; 

  double idisoweight_vvvloose_1_;
  double idisoweight_vvvloose_2_;
  double idisoweight_vvvloose_3_;
  double idisoweight_vvvloose_4_;

  double idisoweight_vvloose_1_;
  double idisoweight_vvloose_2_;
  double idisoweight_vvloose_3_;
  double idisoweight_vvloose_4_;

  double idisoweight_vloose_1_;
  double idisoweight_vloose_2_;
  double idisoweight_vloose_3_;
  double idisoweight_vloose_4_;

  double idisoweight_loose_1_;
  double idisoweight_loose_2_;
  double idisoweight_loose_3_;
  double idisoweight_loose_4_;

  double idisoweight_medium_1_;
  double idisoweight_medium_2_;
  double idisoweight_medium_3_;
  double idisoweight_medium_4_;

  double idisoweight_tight_1_;
  double idisoweight_tight_2_;
  double idisoweight_tight_3_;
  double idisoweight_tight_4_;

  double etau_fakerate_1_;
  double etau_fakerate_2_;
  double etau_fakerate_3_;
  double etau_fakerate_4_;

  double etau_fakerate_ratio_1_up_;
  double etau_fakerate_ratio_2_up_;
  double etau_fakerate_ratio_3_up_;
  double etau_fakerate_ratio_4_up_;
  double etau_fakerate_ratio_1_down_;
  double etau_fakerate_ratio_2_down_;
  double etau_fakerate_ratio_3_down_;
  double etau_fakerate_ratio_4_down_;

  double etau_fakerate_vvloose_1_;
  double etau_fakerate_vvloose_2_;
  double etau_fakerate_vvloose_3_;
  double etau_fakerate_vvloose_4_;

  double etau_fakerate_vloose_1_;
  double etau_fakerate_vloose_2_;
  double etau_fakerate_vloose_3_;
  double etau_fakerate_vloose_4_;

  double etau_fakerate_loose_1_;
  double etau_fakerate_loose_2_;
  double etau_fakerate_loose_3_;
  double etau_fakerate_loose_4_;

  double etau_fakerate_medium_1_;
  double etau_fakerate_medium_2_;
  double etau_fakerate_medium_3_;
  double etau_fakerate_medium_4_;

  double etau_fakerate_tight_1_;
  double etau_fakerate_tight_2_;
  double etau_fakerate_tight_3_;
  double etau_fakerate_tight_4_;

  double etau_fakerate_vtight_1_;
  double etau_fakerate_vtight_2_;
  double etau_fakerate_vtight_3_;
  double etau_fakerate_vtight_4_;

  double mtau_fakerate_1_;
  double mtau_fakerate_2_;
  double mtau_fakerate_3_;
  double mtau_fakerate_4_;

  double mtau_fakerate_ratio_1_up_;
  double mtau_fakerate_ratio_2_up_;
  double mtau_fakerate_ratio_3_up_;
  double mtau_fakerate_ratio_4_up_;
  double mtau_fakerate_ratio_1_down_;
  double mtau_fakerate_ratio_2_down_;
  double mtau_fakerate_ratio_3_down_;
  double mtau_fakerate_ratio_4_down_;

  double mtau_fakerate_vloose_1_;
  double mtau_fakerate_vloose_2_;
  double mtau_fakerate_vloose_3_;
  double mtau_fakerate_vloose_4_;

  double mtau_fakerate_loose_1_;
  double mtau_fakerate_loose_2_;
  double mtau_fakerate_loose_3_;
  double mtau_fakerate_loose_4_;

  double mtau_fakerate_medium_1_;
  double mtau_fakerate_medium_2_;
  double mtau_fakerate_medium_3_;
  double mtau_fakerate_medium_4_;

  double mtau_fakerate_tight_1_;
  double mtau_fakerate_tight_2_;
  double mtau_fakerate_tight_3_;
  double mtau_fakerate_tight_4_;

  double trackingweight_1_;
  double trackingweight_2_;
  double trackingweight_3_;
  double trackingweight_4_;

  double trigeff_doubletau_data_1_;
  double trigeff_doubletau_data_2_;
  double trigeff_doubletau_data_3_;
  double trigeff_doubletau_data_4_;
  double trigeff_etaucross_data_1_;
  double trigeff_etaucross_data_2_;
  double trigeff_etaucross_data_3_;
  double trigeff_etaucross_data_4_;
  double trigeff_mutaucross_data_1_;
  double trigeff_mutaucross_data_2_;
  double trigeff_mutaucross_data_3_;
  double trigeff_mutaucross_data_4_;
  double trigeff_singlee_data_1_;
  double trigeff_singlee_data_2_;
  double trigeff_singlem_data_1_;
  double trigeff_singlem_data_2_;
  double trigeff_emucross_high_data_1_;
  double trigeff_emucross_high_data_2_;
  double trigeff_emucross_low_data_1_;
  double trigeff_emucross_low_data_2_;

  double trigeff_doubletau_mc_1_;
  double trigeff_doubletau_mc_2_;
  double trigeff_doubletau_mc_3_;
  double trigeff_doubletau_mc_4_;
  double trigeff_etaucross_mc_1_;
  double trigeff_etaucross_mc_2_;
  double trigeff_etaucross_mc_3_;
  double trigeff_etaucross_mc_4_;
  double trigeff_mutaucross_mc_1_;
  double trigeff_mutaucross_mc_2_;
  double trigeff_mutaucross_mc_3_;
  double trigeff_mutaucross_mc_4_;
  double trigeff_singlee_mc_1_;
  double trigeff_singlee_mc_2_;
  double trigeff_singlem_mc_1_;
  double trigeff_singlem_mc_2_;
  double trigeff_emucross_high_mc_1_;
  double trigeff_emucross_high_mc_2_;
  double trigeff_emucross_low_mc_1_;
  double trigeff_emucross_low_mc_2_;

  double total_trg_;
  double total_trg_ratio_doubletau_up_;
  double total_trg_ratio_doubletau_down_;
  double total_trg_ratio_singlem_up_;
  double total_trg_ratio_singlem_down_;
  double total_trg_ratio_singlee_up_;
  double total_trg_ratio_singlee_down_;

  double jet_probb_1_;
  double jet_probb_2_;
  double jet_probb_3_;
  double jet_probb_4_;
  double jet_probbb_1_;
  double jet_probbb_2_;
  double jet_probbb_3_;
  double jet_probbb_4_;
  double jet_problepb_1_;
  double jet_problepb_2_;
  double jet_problepb_3_;
  double jet_problepb_4_;
  double jet_probc_1_;
  double jet_probc_2_;
  double jet_probc_3_;
  double jet_probc_4_;
  double jet_probuds_1_;
  double jet_probuds_2_;
  double jet_probuds_3_;
  double jet_probuds_4_;
  double jet_probg_1_;
  double jet_probg_2_;
  double jet_probg_3_;
  double jet_probg_4_;

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
  branch_var iso_3_;
  branch_var iso_4_;

  branch_var pt_1_;
  branch_var pt_2_;
  branch_var pt_3_;
  branch_var pt_4_;
  branch_var jet_pt_1_;
  branch_var jet_pt_2_;
  branch_var jet_pt_3_;
  branch_var jet_pt_4_;
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
  branch_var dR_min_sum_dR_1_;
  branch_var dR_min_sum_dR_2_;
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
  branch_var q_sum_;
  branch_var mt_1_;
  branch_var mt_2_;
  branch_var mt_3_;
  branch_var mt_4_;
  branch_var mt_lep_12_;
  branch_var mt_lep_13_;
  branch_var mt_lep_14_;
  branch_var mt_lep_23_;
  branch_var mt_lep_24_;
  branch_var mt_lep_34_;
  branch_var mt_tot_;

  branch_var mvis_1234_;
  branch_var mvis_12_;
  branch_var mvis_13_;
  branch_var mvis_14_;
  branch_var mvis_23_;
  branch_var mvis_24_;
  branch_var mvis_34_;
  branch_var mvis_min_dphi_1_;
  branch_var mvis_min_dphi_2_;
  branch_var mvis_min_sum_dphi_1_;
  branch_var mvis_min_sum_dphi_2_;
  branch_var mvis_min_dR_1_;
  branch_var mvis_min_dR_2_;
  branch_var mvis_min_sum_dR_1_;
  branch_var mvis_min_sum_dR_2_;
  branch_var mvis_phi_;
  branch_var mvis_A_;
  branch_var pt_tt_12_;
  branch_var pt_tt_13_;
  branch_var pt_tt_14_;
  branch_var pt_tt_23_;
  branch_var pt_tt_24_;
  branch_var pt_tt_34_;
  branch_var pt_min_dphi_1_;
  branch_var pt_min_dphi_2_;
  branch_var pt_min_sum_dphi_1_;
  branch_var pt_min_sum_dphi_2_;
  branch_var pt_min_dR_1_;
  branch_var pt_min_dR_2_;
  branch_var pt_min_sum_dR_1_;
  branch_var pt_min_sum_dR_2_;
  branch_var pt_phi_;
  branch_var pt_A_;
  branch_var pt_tttt_;
  branch_var met_;
  branch_var st_;

  float scale_1_;
  float scale_2_;
  float scale_3_;
  float scale_4_;
  float scale_5_;
  float scale_6_;
  float scale_7_;
  float scale_8_;
  float scale_9_;
  float scale_111_;
  float scale_112_;
  float scale_pdf_;

  int p_min_dphi_1_;
  int p_min_dR_1_;
  int p_min_sum_dphi_1_;
  int p_min_sum_dR_1_;
  int p_min_dphi_2_;
  int p_min_dR_2_;
  int p_min_sum_dphi_2_;
  int p_min_sum_dR_2_;

  int pdgid_mother_1_;
  int pdgid_mother_2_;
  int pdgid_mother_3_;
  int pdgid_mother_4_;  

  int all_taus_from_bosons_;
  int pair_number_;

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

  int n_deepbjets_;
  int n_jets_;
  int n_jetfakes_;

  bool trg_doubletau_12_;
  bool trg_doubletau_13_;
  bool trg_doubletau_14_;
  bool trg_doubletau_23_;
  bool trg_doubletau_24_;
  bool trg_doubletau_34_;
  bool trg_doubleelectron_12_;
  bool trg_doublemuon_12_;
  bool trg_singlemuon_1_;
  bool trg_singlemuon_2_;
  bool trg_singlemuon_3_;
  bool trg_singlemuon_4_;
  bool trg_singleelectron_1_;
  bool trg_singleelectron_2_;
  bool trg_mutaucross_12_;
  bool trg_mutaucross_13_;
  bool trg_mutaucross_14_;
  bool trg_mutaucross_23_;
  bool trg_mutaucross_24_;
  bool trg_etaucross_12_;
  bool trg_etaucross_13_;
  bool trg_etaucross_14_;
  bool trg_etaucross_23_;
  bool trg_etaucross_24_;
  bool trg_emucross_12_;

  bool trg_doubletau_leg_1_;
  bool trg_doubletau_leg_2_;
  bool trg_doubletau_leg_3_;
  bool trg_doubletau_leg_4_;

  bool isTau_;
  bool isSingleElectron_;
  bool isSingleMuon_;
  bool isMuonEG_;

  float wt_ggZZ_;
  float wt_qqZZ_;
  float wt_ggZZ_PDFScaleUp_;
  float wt_ggZZ_PDFScaleDn_;
  float wt_ggZZ_QCDScaleUp_;
  float wt_ggZZ_QCDScaleDn_;
  float wt_ggZZ_AsUp_;
  float wt_ggZZ_AsDn_;
  float wt_ggZZ_PDFReplicaUp_;
  float wt_ggZZ_PDFReplicaDn_;

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
