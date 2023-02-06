#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"

#include "TMVA/Reader.h"
#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include "TLorentzVector.h"

#include "UserCode/ICHiggsTauTau/Analysis/4tau/test/pola_module.hpp"

namespace ic {

  HTTCategories::HTTCategories(std::string const& name) : ModuleBase(name), 
      channel_(channel::tttt), 
      era_(era::data_2018),
      strategy_(strategy::cpdecays18) {
      fourtau_label_ = "4tau";
      fs_ = NULL;
      write_tree_ = true;
      met_label_ = "pfMVAMetNoLeptons";
      jets_label_ = "pfJetsPFlow";
      filename_ = "Tau";
      is_data_ = false;
  }

  HTTCategories::~HTTCategories() {
    ;
  }

  int HTTCategories::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTCategories Four Tau" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
    std::cout << boost::format(param_fmt()) % "era"             % Era2String(era_);
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % fourtau_label_;
    std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;


    
    if (fs_ && write_tree_) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
     
      // Event information      
      outtree_->Branch("event",             &event_);
      outtree_->Branch("run",               &run_);
      outtree_->Branch("lumi",               &lumi_);
      outtree_->Branch("rho",               &rho_, "rho/F");

      // weights
      outtree_->Branch("wt",                &wt_.var_double);
      outtree_->Branch("wt_dysoup",         &wt_dysoup_);
      outtree_->Branch("wt_wsoup",         &wt_wsoup_);
      outtree_->Branch("wt_mc_sign",                &wt_mc_sign_);

      // gen matches
      outtree_->Branch("gen_match_1",         &gen_match_1_);
      outtree_->Branch("gen_match_2",         &gen_match_2_);
      outtree_->Branch("gen_match_3",         &gen_match_3_);
      outtree_->Branch("gen_match_4",         &gen_match_4_);

      // mother pdgid
      outtree_->Branch("pdgid_mother_1",         &pdgid_mother_1_);
      outtree_->Branch("pdgid_mother_2",         &pdgid_mother_2_);
      outtree_->Branch("pdgid_mother_3",         &pdgid_mother_3_);
      outtree_->Branch("pdgid_mother_4",         &pdgid_mother_4_);
      outtree_->Branch("all_taus_from_bosons",   &all_taus_from_bosons_);
      outtree_->Branch("pair_number",            &pair_number_);

      // trigger booleans
      outtree_->Branch("trg_doubletau_12",      &trg_doubletau_12_);
      outtree_->Branch("trg_doubletau_13",      &trg_doubletau_13_);
      outtree_->Branch("trg_doubletau_14",      &trg_doubletau_14_);
      outtree_->Branch("trg_doubletau_23",      &trg_doubletau_23_);
      outtree_->Branch("trg_doubletau_24",      &trg_doubletau_24_);
      outtree_->Branch("trg_doubletau_34",      &trg_doubletau_34_);
      outtree_->Branch("trg_doubleelectron_12", &trg_doubleelectron_12_);
      outtree_->Branch("trg_doublemuon_12",     &trg_doublemuon_12_);
      outtree_->Branch("trg_singlemuon_1",      &trg_singlemuon_1_);
      outtree_->Branch("trg_singlemuon_2",      &trg_singlemuon_2_);
      outtree_->Branch("trg_singlemuon_3",      &trg_singlemuon_3_);
      outtree_->Branch("trg_singlemuon_4",      &trg_singlemuon_4_);
      outtree_->Branch("trg_singleelectron_1",  &trg_singleelectron_1_);
      outtree_->Branch("trg_singleelectron_2",  &trg_singleelectron_2_);
      outtree_->Branch("trg_mutaucross_12",     &trg_mutaucross_12_);
      outtree_->Branch("trg_mutaucross_13",     &trg_mutaucross_13_);
      outtree_->Branch("trg_mutaucross_14",     &trg_mutaucross_14_);
      outtree_->Branch("trg_mutaucross_23",     &trg_mutaucross_23_);
      outtree_->Branch("trg_mutaucross_24",     &trg_mutaucross_24_);
      outtree_->Branch("trg_etaucross_12",      &trg_etaucross_12_);
      outtree_->Branch("trg_etaucross_13",      &trg_etaucross_13_);
      outtree_->Branch("trg_etaucross_14",      &trg_etaucross_14_);
      outtree_->Branch("trg_etaucross_23",      &trg_etaucross_23_);
      outtree_->Branch("trg_etaucross_24",      &trg_etaucross_24_);
      outtree_->Branch("trg_emucross_12",       &trg_emucross_12_);

      outtree_->Branch("trg_doubletau_leg_1",      &trg_doubletau_leg_1_);
      outtree_->Branch("trg_doubletau_leg_2",      &trg_doubletau_leg_2_);
      outtree_->Branch("trg_doubletau_leg_3",      &trg_doubletau_leg_3_);
      outtree_->Branch("trg_doubletau_leg_4",      &trg_doubletau_leg_4_);

      // idiso weights
      outtree_->Branch("idisoweight_1", &idisoweight_1_);
      outtree_->Branch("idisoweight_2", &idisoweight_2_);
      outtree_->Branch("idisoweight_3", &idisoweight_3_);
      outtree_->Branch("idisoweight_4", &idisoweight_4_);

      outtree_->Branch("idisoweight_ratio_1_up", &idisoweight_ratio_1_up_);
      outtree_->Branch("idisoweight_ratio_2_up", &idisoweight_ratio_2_up_);
      outtree_->Branch("idisoweight_ratio_3_up", &idisoweight_ratio_3_up_);
      outtree_->Branch("idisoweight_ratio_4_up", &idisoweight_ratio_4_up_);

      outtree_->Branch("idisoweight_ratio_1_down", &idisoweight_ratio_1_down_);
      outtree_->Branch("idisoweight_ratio_2_down", &idisoweight_ratio_2_down_);
      outtree_->Branch("idisoweight_ratio_3_down", &idisoweight_ratio_3_down_);
      outtree_->Branch("idisoweight_ratio_4_down", &idisoweight_ratio_4_down_);

      outtree_->Branch("idisoweight_vvvloose_1", &idisoweight_vvvloose_1_);
      outtree_->Branch("idisoweight_vvvloose_2", &idisoweight_vvvloose_2_);
      outtree_->Branch("idisoweight_vvvloose_3", &idisoweight_vvvloose_3_);
      outtree_->Branch("idisoweight_vvvloose_4", &idisoweight_vvvloose_4_);

      outtree_->Branch("idisoweight_vvloose_1", &idisoweight_vvloose_1_);
      outtree_->Branch("idisoweight_vvloose_2", &idisoweight_vvloose_2_);
      outtree_->Branch("idisoweight_vvloose_3", &idisoweight_vvloose_3_);
      outtree_->Branch("idisoweight_vvloose_4", &idisoweight_vvloose_4_);

      outtree_->Branch("idisoweight_vloose_1", &idisoweight_vloose_1_);
      outtree_->Branch("idisoweight_vloose_2", &idisoweight_vloose_2_);
      outtree_->Branch("idisoweight_vloose_3", &idisoweight_vloose_3_);
      outtree_->Branch("idisoweight_vloose_4", &idisoweight_vloose_4_);

      outtree_->Branch("idisoweight_loose_1", &idisoweight_loose_1_);
      outtree_->Branch("idisoweight_loose_2", &idisoweight_loose_2_);
      outtree_->Branch("idisoweight_loose_3", &idisoweight_loose_3_);
      outtree_->Branch("idisoweight_loose_4", &idisoweight_loose_4_);

      outtree_->Branch("idisoweight_medium_1", &idisoweight_medium_1_);
      outtree_->Branch("idisoweight_medium_2", &idisoweight_medium_2_);
      outtree_->Branch("idisoweight_medium_3", &idisoweight_medium_3_);
      outtree_->Branch("idisoweight_medium_4", &idisoweight_medium_4_);

      outtree_->Branch("idisoweight_tight_1", &idisoweight_tight_1_);
      outtree_->Branch("idisoweight_tight_2", &idisoweight_tight_2_);
      outtree_->Branch("idisoweight_tight_3", &idisoweight_tight_3_);
      outtree_->Branch("idisoweight_tight_4", &idisoweight_tight_4_);

      // etau fake rates
      outtree_->Branch("etau_fakerate_1", &etau_fakerate_1_);
      outtree_->Branch("etau_fakerate_2", &etau_fakerate_2_);
      outtree_->Branch("etau_fakerate_3", &etau_fakerate_3_);
      outtree_->Branch("etau_fakerate_4", &etau_fakerate_4_);

      outtree_->Branch("etau_fakerate_ratio_1_up", &etau_fakerate_ratio_1_up_);
      outtree_->Branch("etau_fakerate_ratio_2_up", &etau_fakerate_ratio_2_up_);
      outtree_->Branch("etau_fakerate_ratio_3_up", &etau_fakerate_ratio_3_up_);
      outtree_->Branch("etau_fakerate_ratio_4_up", &etau_fakerate_ratio_4_up_);

      outtree_->Branch("etau_fakerate_ratio_1_down", &etau_fakerate_ratio_1_down_);
      outtree_->Branch("etau_fakerate_ratio_2_down", &etau_fakerate_ratio_2_down_);
      outtree_->Branch("etau_fakerate_ratio_3_down", &etau_fakerate_ratio_3_down_);
      outtree_->Branch("etau_fakerate_ratio_4_down", &etau_fakerate_ratio_4_down_);

      outtree_->Branch("etau_fakerate_vvloose_1", &etau_fakerate_vvloose_1_);
      outtree_->Branch("etau_fakerate_vvloose_2", &etau_fakerate_vvloose_2_);
      outtree_->Branch("etau_fakerate_vvloose_3", &etau_fakerate_vvloose_3_);
      outtree_->Branch("etau_fakerate_vvloose_4", &etau_fakerate_vvloose_4_);

      outtree_->Branch("etau_fakerate_vloose_1", &etau_fakerate_vloose_1_);
      outtree_->Branch("etau_fakerate_vloose_2", &etau_fakerate_vloose_2_);
      outtree_->Branch("etau_fakerate_vloose_3", &etau_fakerate_vloose_3_);
      outtree_->Branch("etau_fakerate_vloose_4", &etau_fakerate_vloose_4_);

      outtree_->Branch("etau_fakerate_loose_1", &etau_fakerate_loose_1_);
      outtree_->Branch("etau_fakerate_loose_2", &etau_fakerate_loose_2_);
      outtree_->Branch("etau_fakerate_loose_3", &etau_fakerate_loose_3_);
      outtree_->Branch("etau_fakerate_loose_4", &etau_fakerate_loose_4_);

      outtree_->Branch("etau_fakerate_medium_1", &etau_fakerate_medium_1_);
      outtree_->Branch("etau_fakerate_medium_2", &etau_fakerate_medium_2_);
      outtree_->Branch("etau_fakerate_medium_3", &etau_fakerate_medium_3_);
      outtree_->Branch("etau_fakerate_medium_4", &etau_fakerate_medium_4_);

      outtree_->Branch("etau_fakerate_tight_1", &etau_fakerate_tight_1_);
      outtree_->Branch("etau_fakerate_tight_2", &etau_fakerate_tight_2_);
      outtree_->Branch("etau_fakerate_tight_3", &etau_fakerate_tight_3_);
      outtree_->Branch("etau_fakerate_tight_4", &etau_fakerate_tight_4_);

      outtree_->Branch("etau_fakerate_vtight_1", &etau_fakerate_vtight_1_);
      outtree_->Branch("etau_fakerate_vtight_2", &etau_fakerate_vtight_2_);
      outtree_->Branch("etau_fakerate_vtight_3", &etau_fakerate_vtight_3_);
      outtree_->Branch("etau_fakerate_vtight_4", &etau_fakerate_vtight_4_);

      // mtau fake rates
      outtree_->Branch("mtau_fakerate_1", &mtau_fakerate_1_);
      outtree_->Branch("mtau_fakerate_2", &mtau_fakerate_2_);
      outtree_->Branch("mtau_fakerate_3", &mtau_fakerate_3_);
      outtree_->Branch("mtau_fakerate_4", &mtau_fakerate_4_);

      outtree_->Branch("mtau_fakerate_ratio_1_up", &mtau_fakerate_ratio_1_up_);
      outtree_->Branch("mtau_fakerate_ratio_2_up", &mtau_fakerate_ratio_2_up_);
      outtree_->Branch("mtau_fakerate_ratio_3_up", &mtau_fakerate_ratio_3_up_);
      outtree_->Branch("mtau_fakerate_ratio_4_up", &mtau_fakerate_ratio_4_up_);

      outtree_->Branch("mtau_fakerate_ratio_1_down", &mtau_fakerate_ratio_1_down_);
      outtree_->Branch("mtau_fakerate_ratio_2_down", &mtau_fakerate_ratio_2_down_);
      outtree_->Branch("mtau_fakerate_ratio_3_down", &mtau_fakerate_ratio_3_down_);
      outtree_->Branch("mtau_fakerate_ratio_4_down", &mtau_fakerate_ratio_4_down_);

      outtree_->Branch("mtau_fakerate_vloose_1", &mtau_fakerate_vloose_1_);
      outtree_->Branch("mtau_fakerate_vloose_2", &mtau_fakerate_vloose_2_);
      outtree_->Branch("mtau_fakerate_vloose_3", &mtau_fakerate_vloose_3_);
      outtree_->Branch("mtau_fakerate_vloose_4", &mtau_fakerate_vloose_4_);

      outtree_->Branch("mtau_fakerate_loose_1", &mtau_fakerate_loose_1_);
      outtree_->Branch("mtau_fakerate_loose_2", &mtau_fakerate_loose_2_);
      outtree_->Branch("mtau_fakerate_loose_3", &mtau_fakerate_loose_3_);
      outtree_->Branch("mtau_fakerate_loose_4", &mtau_fakerate_loose_4_);

      outtree_->Branch("mtau_fakerate_medium_1", &mtau_fakerate_medium_1_);
      outtree_->Branch("mtau_fakerate_medium_2", &mtau_fakerate_medium_2_);
      outtree_->Branch("mtau_fakerate_medium_3", &mtau_fakerate_medium_3_);
      outtree_->Branch("mtau_fakerate_medium_4", &mtau_fakerate_medium_4_);

      outtree_->Branch("mtau_fakerate_tight_1", &mtau_fakerate_tight_1_);
      outtree_->Branch("mtau_fakerate_tight_2", &mtau_fakerate_tight_2_);
      outtree_->Branch("mtau_fakerate_tight_3", &mtau_fakerate_tight_3_);
      outtree_->Branch("mtau_fakerate_tight_4", &mtau_fakerate_tight_4_);

      // tracking weights
      outtree_->Branch("trackingweight_1", &trackingweight_1_);
      outtree_->Branch("trackingweight_2", &trackingweight_2_);
      outtree_->Branch("trackingweight_3", &trackingweight_3_);
      outtree_->Branch("trackingweight_4", &trackingweight_4_);

      // trigger weights
      outtree_->Branch("trigeff_doubletau_data_1",     &trigeff_doubletau_data_1_);
      outtree_->Branch("trigeff_doubletau_data_2",     &trigeff_doubletau_data_2_);
      outtree_->Branch("trigeff_doubletau_data_3",     &trigeff_doubletau_data_3_);
      outtree_->Branch("trigeff_doubletau_data_4",     &trigeff_doubletau_data_4_);
      outtree_->Branch("trigeff_etaucross_data_1",     &trigeff_etaucross_data_1_);
      outtree_->Branch("trigeff_etaucross_data_2",     &trigeff_etaucross_data_2_);
      outtree_->Branch("trigeff_etaucross_data_3",     &trigeff_etaucross_data_3_);
      outtree_->Branch("trigeff_etaucross_data_4",     &trigeff_etaucross_data_4_);
      outtree_->Branch("trigeff_mutaucross_data_1",    &trigeff_mutaucross_data_1_);
      outtree_->Branch("trigeff_mutaucross_data_2",    &trigeff_mutaucross_data_2_);
      outtree_->Branch("trigeff_mutaucross_data_3",    &trigeff_mutaucross_data_3_);
      outtree_->Branch("trigeff_mutaucross_data_4",    &trigeff_mutaucross_data_4_);
      outtree_->Branch("trigeff_singlee_data_1",       &trigeff_singlee_data_1_);
      outtree_->Branch("trigeff_singlee_data_2",       &trigeff_singlee_data_2_);
      outtree_->Branch("trigeff_singlem_data_1",       &trigeff_singlem_data_1_);
      outtree_->Branch("trigeff_singlem_data_2",       &trigeff_singlem_data_2_);
      outtree_->Branch("trigeff_emucross_high_data_1", &trigeff_emucross_high_data_1_);
      outtree_->Branch("trigeff_emucross_high_data_2", &trigeff_emucross_high_data_2_);
      outtree_->Branch("trigeff_emucross_low_data_1",  &trigeff_emucross_low_data_1_);
      outtree_->Branch("trigeff_emucross_low_data_2",  &trigeff_emucross_low_data_2_);

      outtree_->Branch("trigeff_doubletau_mc_1",     &trigeff_doubletau_mc_1_);
      outtree_->Branch("trigeff_doubletau_mc_2",     &trigeff_doubletau_mc_2_);
      outtree_->Branch("trigeff_doubletau_mc_3",     &trigeff_doubletau_mc_3_);
      outtree_->Branch("trigeff_doubletau_mc_4",     &trigeff_doubletau_mc_4_);
      outtree_->Branch("trigeff_etaucross_mc_1",     &trigeff_etaucross_mc_1_);
      outtree_->Branch("trigeff_etaucross_mc_2",     &trigeff_etaucross_mc_2_);
      outtree_->Branch("trigeff_etaucross_mc_3",     &trigeff_etaucross_mc_3_);
      outtree_->Branch("trigeff_etaucross_mc_4",     &trigeff_etaucross_mc_4_);
      outtree_->Branch("trigeff_mutaucross_mc_1",    &trigeff_mutaucross_mc_1_);
      outtree_->Branch("trigeff_mutaucross_mc_2",    &trigeff_mutaucross_mc_2_);
      outtree_->Branch("trigeff_mutaucross_mc_3",    &trigeff_mutaucross_mc_3_);
      outtree_->Branch("trigeff_mutaucross_mc_4",    &trigeff_mutaucross_mc_4_);
      outtree_->Branch("trigeff_singlee_mc_1",       &trigeff_singlee_mc_1_);
      outtree_->Branch("trigeff_singlee_mc_2",       &trigeff_singlee_mc_2_);
      outtree_->Branch("trigeff_singlem_mc_1",       &trigeff_singlem_mc_1_);
      outtree_->Branch("trigeff_singlem_mc_2",       &trigeff_singlem_mc_2_);
      outtree_->Branch("trigeff_emucross_high_mc_1", &trigeff_emucross_high_mc_1_);
      outtree_->Branch("trigeff_emucross_high_mc_2", &trigeff_emucross_high_mc_2_);
      outtree_->Branch("trigeff_emucross_low_mc_1",  &trigeff_emucross_low_mc_1_);
      outtree_->Branch("trigeff_emucross_low_mc_2",  &trigeff_emucross_low_mc_2_);

      outtree_->Branch("total_trg",  &total_trg_);

      outtree_->Branch("total_trg_ratio_doubletau_up",    &total_trg_ratio_doubletau_up_);
      outtree_->Branch("total_trg_ratio_doubletau_down",  &total_trg_ratio_doubletau_down_);

      // dxy and dz
      outtree_->Branch("d0_1", &d0_1_.var_float, "d0_1/F");
      outtree_->Branch("dZ_1", &dz_1_.var_float, "dz_1/F");
      outtree_->Branch("d0_2", &d0_2_.var_float, "d0_2/F");
      outtree_->Branch("dZ_2", &dz_2_.var_float, "dz_2/F");
      outtree_->Branch("d0_3", &d0_3_.var_float, "d0_3/F");
      outtree_->Branch("dZ_3", &dz_3_.var_float, "dz_3/F");
      outtree_->Branch("d0_4", &d0_4_.var_float, "d0_4/F");
      outtree_->Branch("dZ_4", &dz_4_.var_float, "dz_4/F");

      // lepton isolations
      outtree_->Branch("iso_1",             &iso_1_.var_double);
      outtree_->Branch("iso_2",             &iso_2_.var_double);
      outtree_->Branch("iso_3",             &iso_3_.var_double);
      outtree_->Branch("iso_4",             &iso_4_.var_double);

      // transverse momentums
      outtree_->Branch("pt_1",              &pt_1_.var_double);
      outtree_->Branch("pt_2",              &pt_2_.var_double);
      outtree_->Branch("pt_3",              &pt_3_.var_double);
      outtree_->Branch("pt_4",              &pt_4_.var_double);
      outtree_->Branch("jet_pt_1",          &jet_pt_1_.var_double);
      outtree_->Branch("jet_pt_2",          &jet_pt_2_.var_double);
      outtree_->Branch("jet_pt_3",          &jet_pt_3_.var_double);
      outtree_->Branch("jet_pt_4",          &jet_pt_4_.var_double);


      // masses and energies
      outtree_->Branch("m_1",              &m_1_.var_double);
      outtree_->Branch("m_2",              &m_2_.var_double);
      outtree_->Branch("m_3",              &m_3_.var_double);
      outtree_->Branch("m_4",              &m_4_.var_double);
      outtree_->Branch("E_1",              &E_1_.var_double);
      outtree_->Branch("E_2",              &E_2_.var_double);
      outtree_->Branch("E_3",              &E_3_.var_double);
      outtree_->Branch("E_4",              &E_4_.var_double);

      // angles
      outtree_->Branch("eta_1",              &eta_1_.var_double);
      outtree_->Branch("eta_2",              &eta_2_.var_double);
      outtree_->Branch("eta_3",              &eta_3_.var_double);
      outtree_->Branch("eta_4",              &eta_4_.var_double);
      outtree_->Branch("phi_1",              &phi_1_.var_double);
      outtree_->Branch("phi_2",              &phi_2_.var_double);
      outtree_->Branch("phi_3",              &phi_3_.var_double);
      outtree_->Branch("phi_4",              &phi_4_.var_double);

      // angles/distances between objects
      outtree_->Branch("dphi_12",              &dphi_12_.var_double);
      outtree_->Branch("dphi_13",              &dphi_13_.var_double);
      outtree_->Branch("dphi_14",              &dphi_14_.var_double);
      outtree_->Branch("dphi_23",              &dphi_23_.var_double);
      outtree_->Branch("dphi_24",              &dphi_24_.var_double);
      outtree_->Branch("dphi_34",              &dphi_34_.var_double);

      outtree_->Branch("dR_12",              &dR_12_.var_double);
      outtree_->Branch("dR_13",              &dR_13_.var_double);
      outtree_->Branch("dR_14",              &dR_14_.var_double);
      outtree_->Branch("dR_23",              &dR_23_.var_double);
      outtree_->Branch("dR_24",              &dR_24_.var_double);
      outtree_->Branch("dR_34",              &dR_34_.var_double);
      outtree_->Branch("dR_min_sum_dR_1",    &dR_min_sum_dR_1_.var_double);
      outtree_->Branch("dR_min_sum_dR_2",    &dR_min_sum_dR_2_.var_double);

      // pair properties
      outtree_->Branch("mt_1",              &mt_1_.var_double);
      outtree_->Branch("mt_2",              &mt_2_.var_double);
      outtree_->Branch("mt_3",              &mt_3_.var_double);
      outtree_->Branch("mt_4",              &mt_4_.var_double);
      outtree_->Branch("mt_lep_12",              &mt_lep_12_.var_double);
      outtree_->Branch("mt_lep_13",              &mt_lep_13_.var_double);
      outtree_->Branch("mt_lep_14",              &mt_lep_14_.var_double);
      outtree_->Branch("mt_lep_23",              &mt_lep_23_.var_double);
      outtree_->Branch("mt_lep_24",              &mt_lep_24_.var_double);
      outtree_->Branch("mt_lep_34",              &mt_lep_34_.var_double);
      outtree_->Branch("mt_tot",                 &mt_tot_.var_double);

      outtree_->Branch("mvis_1234",            &mvis_1234_.var_double);
      outtree_->Branch("mvis_12",              &mvis_12_.var_double);
      outtree_->Branch("mvis_13",              &mvis_13_.var_double);
      outtree_->Branch("mvis_14",              &mvis_14_.var_double);
      outtree_->Branch("mvis_23",              &mvis_23_.var_double);
      outtree_->Branch("mvis_24",              &mvis_24_.var_double);
      outtree_->Branch("mvis_34",              &mvis_34_.var_double);
      outtree_->Branch("mvis_min_dphi_1",      &mvis_min_dphi_1_.var_double);
      outtree_->Branch("mvis_min_dphi_2",      &mvis_min_dphi_2_.var_double);
      outtree_->Branch("mvis_min_dR_1",        &mvis_min_dR_1_.var_double);
      outtree_->Branch("mvis_min_dR_2",        &mvis_min_dR_2_.var_double);
      outtree_->Branch("mvis_min_sum_dphi_1",  &mvis_min_sum_dphi_1_.var_double);
      outtree_->Branch("mvis_min_sum_dphi_2",  &mvis_min_sum_dphi_2_.var_double);
      outtree_->Branch("mvis_min_sum_dR_1",    &mvis_min_sum_dR_1_.var_double);
      outtree_->Branch("mvis_min_sum_dR_2",    &mvis_min_sum_dR_2_.var_double);
      outtree_->Branch("mvis_phi",             &mvis_phi_.var_double);
      outtree_->Branch("mvis_A",               &mvis_A_.var_double);
      outtree_->Branch("pt_min_dphi_1",        &pt_min_dphi_1_.var_double);
      outtree_->Branch("pt_min_dphi_2",        &pt_min_dphi_2_.var_double);
      outtree_->Branch("pt_min_dR_1",          &pt_min_dR_1_.var_double);
      outtree_->Branch("pt_min_dR_2",          &pt_min_dR_2_.var_double);
      outtree_->Branch("pt_min_sum_dphi_1",    &pt_min_sum_dphi_1_.var_double);
      outtree_->Branch("pt_min_sum_dphi_2",    &pt_min_sum_dphi_2_.var_double);
      outtree_->Branch("pt_min_sum_dR_1",      &pt_min_sum_dR_1_.var_double);
      outtree_->Branch("pt_min_sum_dR_2",      &pt_min_sum_dR_2_.var_double);
      outtree_->Branch("pt_tt_12",             &pt_tt_12_.var_double);
      outtree_->Branch("pt_tt_13",             &pt_tt_13_.var_double);
      outtree_->Branch("pt_tt_14",             &pt_tt_14_.var_double);
      outtree_->Branch("pt_tt_23",             &pt_tt_23_.var_double);
      outtree_->Branch("pt_tt_24",             &pt_tt_24_.var_double);
      outtree_->Branch("pt_tt_34",             &pt_tt_34_.var_double);
      outtree_->Branch("pt_tttt",             &pt_tttt_.var_double);
      outtree_->Branch("pt_phi",               &pt_phi_.var_double);
      outtree_->Branch("pt_A",                 &pt_A_.var_double);
      outtree_->Branch("st",                   &st_.var_double);
      outtree_->Branch("met",                  &met_.var_double);

      outtree_->Branch("p_min_dphi_1",      &p_min_dphi_1_);
      outtree_->Branch("p_min_dphi_2",      &p_min_dphi_2_);
      outtree_->Branch("p_min_sum_dphi_1",  &p_min_sum_dphi_1_);
      outtree_->Branch("p_min_sum_dphi_2",  &p_min_sum_dphi_2_);
      outtree_->Branch("p_min_dR_1",        &p_min_dR_1_);
      outtree_->Branch("p_min_dR_2",        &p_min_dR_2_);
      outtree_->Branch("p_min_sum_dR_1",    &p_min_sum_dR_1_);
      outtree_->Branch("p_min_sum_dR_2",    &p_min_sum_dR_2_);

      // charges
      outtree_->Branch("q_1",              &q_1_.var_double);
      outtree_->Branch("q_2",              &q_2_.var_double);
      outtree_->Branch("q_3",              &q_3_.var_double);
      outtree_->Branch("q_4",              &q_4_.var_double);
      outtree_->Branch("q_sum",            &q_sum_.var_double);

      // DeepTau scores
      outtree_->Branch("deepTauVsEle_iso_1",       &deepTauVsEle_iso_1_);
      outtree_->Branch("deepTauVsEle_iso_2",       &deepTauVsEle_iso_2_);
      outtree_->Branch("deepTauVsEle_iso_3",       &deepTauVsEle_iso_3_);
      outtree_->Branch("deepTauVsEle_iso_4",       &deepTauVsEle_iso_4_);
      outtree_->Branch("deepTauVsMu_iso_1",        &deepTauVsMu_iso_1_);
      outtree_->Branch("deepTauVsMu_iso_2",        &deepTauVsMu_iso_2_);
      outtree_->Branch("deepTauVsMu_iso_3",        &deepTauVsMu_iso_3_);
      outtree_->Branch("deepTauVsMu_iso_4",        &deepTauVsMu_iso_4_);
      outtree_->Branch("deepTauVsJets_iso_1",       &deepTauVsJets_iso_1_);
      outtree_->Branch("deepTauVsJets_iso_2",       &deepTauVsJets_iso_2_);
      outtree_->Branch("deepTauVsJets_iso_3",       &deepTauVsJets_iso_3_);
      outtree_->Branch("deepTauVsJets_iso_4",       &deepTauVsJets_iso_4_);

      // DeepTau booleans
      outtree_->Branch("deepTauVsJets_vvvloose_1", &deepTauVsJets_vvvloose_1_);
      outtree_->Branch("deepTauVsJets_vvvloose_2", &deepTauVsJets_vvvloose_2_);
      outtree_->Branch("deepTauVsJets_vvvloose_3", &deepTauVsJets_vvvloose_3_);
      outtree_->Branch("deepTauVsJets_vvvloose_4", &deepTauVsJets_vvvloose_4_);

      outtree_->Branch("deepTauVsJets_vvloose_1",  &deepTauVsJets_vvloose_1_);
      outtree_->Branch("deepTauVsJets_vvloose_2",  &deepTauVsJets_vvloose_2_);
      outtree_->Branch("deepTauVsJets_vvloose_3",  &deepTauVsJets_vvloose_3_);
      outtree_->Branch("deepTauVsJets_vvloose_4",  &deepTauVsJets_vvloose_4_);

      outtree_->Branch("deepTauVsJets_vloose_1",   &deepTauVsJets_vloose_1_);
      outtree_->Branch("deepTauVsJets_vloose_2",   &deepTauVsJets_vloose_2_);
      outtree_->Branch("deepTauVsJets_vloose_3",   &deepTauVsJets_vloose_3_);
      outtree_->Branch("deepTauVsJets_vloose_4",   &deepTauVsJets_vloose_4_);

      outtree_->Branch("deepTauVsJets_loose_1",    &deepTauVsJets_loose_1_);
      outtree_->Branch("deepTauVsJets_loose_2",    &deepTauVsJets_loose_2_);
      outtree_->Branch("deepTauVsJets_loose_3",    &deepTauVsJets_loose_3_);
      outtree_->Branch("deepTauVsJets_loose_4",    &deepTauVsJets_loose_4_);

      outtree_->Branch("deepTauVsJets_medium_1",   &deepTauVsJets_medium_1_);
      outtree_->Branch("deepTauVsJets_medium_2",   &deepTauVsJets_medium_2_);
      outtree_->Branch("deepTauVsJets_medium_3",   &deepTauVsJets_medium_3_);
      outtree_->Branch("deepTauVsJets_medium_4",   &deepTauVsJets_medium_4_);

      outtree_->Branch("deepTauVsJets_tight_1",    &deepTauVsJets_tight_1_);
      outtree_->Branch("deepTauVsJets_tight_2",    &deepTauVsJets_tight_2_);
      outtree_->Branch("deepTauVsJets_tight_3",    &deepTauVsJets_tight_3_);
      outtree_->Branch("deepTauVsJets_tight_4",    &deepTauVsJets_tight_4_);

      outtree_->Branch("deepTauVsJets_vtight_1",   &deepTauVsJets_vtight_1_);
      outtree_->Branch("deepTauVsJets_vtight_2",   &deepTauVsJets_vtight_2_);
      outtree_->Branch("deepTauVsJets_vtight_3",   &deepTauVsJets_vtight_3_);
      outtree_->Branch("deepTauVsJets_vtight_4",   &deepTauVsJets_vtight_4_);

      outtree_->Branch("deepTauVsJets_vvtight_1",  &deepTauVsJets_vvtight_1_);
      outtree_->Branch("deepTauVsJets_vvtight_2",  &deepTauVsJets_vvtight_2_);
      outtree_->Branch("deepTauVsJets_vvtight_3",  &deepTauVsJets_vvtight_3_);
      outtree_->Branch("deepTauVsJets_vvtight_4",  &deepTauVsJets_vvtight_4_);

      outtree_->Branch("deepTauVsEle_vvvloose_1",  &deepTauVsEle_vvvloose_1_);
      outtree_->Branch("deepTauVsEle_vvvloose_2",  &deepTauVsEle_vvvloose_2_);
      outtree_->Branch("deepTauVsEle_vvvloose_3",  &deepTauVsEle_vvvloose_3_);
      outtree_->Branch("deepTauVsEle_vvvloose_4",  &deepTauVsEle_vvvloose_4_);

      outtree_->Branch("deepTauVsEle_vvloose_1",   &deepTauVsEle_vvloose_1_);
      outtree_->Branch("deepTauVsEle_vvloose_2",   &deepTauVsEle_vvloose_2_);
      outtree_->Branch("deepTauVsEle_vvloose_3",   &deepTauVsEle_vvloose_3_);
      outtree_->Branch("deepTauVsEle_vvloose_4",   &deepTauVsEle_vvloose_4_);

      outtree_->Branch("deepTauVsEle_vloose_1",    &deepTauVsEle_vloose_1_);
      outtree_->Branch("deepTauVsEle_vloose_2",    &deepTauVsEle_vloose_2_);
      outtree_->Branch("deepTauVsEle_vloose_3",    &deepTauVsEle_vloose_3_);
      outtree_->Branch("deepTauVsEle_vloose_4",    &deepTauVsEle_vloose_4_);

      outtree_->Branch("deepTauVsEle_loose_1",     &deepTauVsEle_loose_1_);
      outtree_->Branch("deepTauVsEle_loose_2",     &deepTauVsEle_loose_2_);
      outtree_->Branch("deepTauVsEle_loose_3",     &deepTauVsEle_loose_3_);
      outtree_->Branch("deepTauVsEle_loose_4",     &deepTauVsEle_loose_4_);

      outtree_->Branch("deepTauVsEle_medium_1",    &deepTauVsEle_medium_1_);
      outtree_->Branch("deepTauVsEle_medium_2",    &deepTauVsEle_medium_2_);
      outtree_->Branch("deepTauVsEle_medium_3",    &deepTauVsEle_medium_3_);
      outtree_->Branch("deepTauVsEle_medium_4",    &deepTauVsEle_medium_4_);

      outtree_->Branch("deepTauVsEle_tight_1",     &deepTauVsEle_tight_1_);
      outtree_->Branch("deepTauVsEle_tight_2",     &deepTauVsEle_tight_2_);
      outtree_->Branch("deepTauVsEle_tight_3",     &deepTauVsEle_tight_3_);
      outtree_->Branch("deepTauVsEle_tight_4",     &deepTauVsEle_tight_4_);

      outtree_->Branch("deepTauVsEle_vtight_1",    &deepTauVsEle_vtight_1_);
      outtree_->Branch("deepTauVsEle_vtight_2",    &deepTauVsEle_vtight_2_);
      outtree_->Branch("deepTauVsEle_vtight_3",    &deepTauVsEle_vtight_3_);
      outtree_->Branch("deepTauVsEle_vtight_4",    &deepTauVsEle_vtight_4_);

      outtree_->Branch("deepTauVsEle_vvtight_1",   &deepTauVsEle_vvtight_1_);
      outtree_->Branch("deepTauVsEle_vvtight_2",   &deepTauVsEle_vvtight_2_);
      outtree_->Branch("deepTauVsEle_vvtight_3",   &deepTauVsEle_vvtight_3_);
      outtree_->Branch("deepTauVsEle_vvtight_4",   &deepTauVsEle_vvtight_4_);

      outtree_->Branch("deepTauVsMu_vvvloose_1",  &deepTauVsMu_vvvloose_1_);
      outtree_->Branch("deepTauVsMu_vvvloose_2",  &deepTauVsMu_vvvloose_2_);
      outtree_->Branch("deepTauVsMu_vvvloose_3",  &deepTauVsMu_vvvloose_3_);
      outtree_->Branch("deepTauVsMu_vvvloose_4",  &deepTauVsMu_vvvloose_4_);

      outtree_->Branch("deepTauVsMu_vvloose_1",   &deepTauVsMu_vvloose_1_);
      outtree_->Branch("deepTauVsMu_vvloose_2",   &deepTauVsMu_vvloose_2_);
      outtree_->Branch("deepTauVsMu_vvloose_3",   &deepTauVsMu_vvloose_3_);
      outtree_->Branch("deepTauVsMu_vvloose_4",   &deepTauVsMu_vvloose_4_);

      outtree_->Branch("deepTauVsMu_vloose_1",    &deepTauVsMu_vloose_1_);
      outtree_->Branch("deepTauVsMu_vloose_2",    &deepTauVsMu_vloose_2_);
      outtree_->Branch("deepTauVsMu_vloose_3",    &deepTauVsMu_vloose_3_);
      outtree_->Branch("deepTauVsMu_vloose_4",    &deepTauVsMu_vloose_4_);

      outtree_->Branch("deepTauVsMu_loose_1",     &deepTauVsMu_loose_1_);
      outtree_->Branch("deepTauVsMu_loose_2",     &deepTauVsMu_loose_2_);
      outtree_->Branch("deepTauVsMu_loose_3",     &deepTauVsMu_loose_3_);
      outtree_->Branch("deepTauVsMu_loose_4",     &deepTauVsMu_loose_4_);

      outtree_->Branch("deepTauVsMu_medium_1",    &deepTauVsMu_medium_1_);
      outtree_->Branch("deepTauVsMu_medium_2",    &deepTauVsMu_medium_2_);
      outtree_->Branch("deepTauVsMu_medium_3",    &deepTauVsMu_medium_3_);
      outtree_->Branch("deepTauVsMu_medium_4",    &deepTauVsMu_medium_4_);

      outtree_->Branch("deepTauVsMu_tight_1",     &deepTauVsMu_tight_1_);
      outtree_->Branch("deepTauVsMu_tight_2",     &deepTauVsMu_tight_2_);
      outtree_->Branch("deepTauVsMu_tight_3",     &deepTauVsMu_tight_3_);
      outtree_->Branch("deepTauVsMu_tight_4",     &deepTauVsMu_tight_4_);

      outtree_->Branch("deepTauVsMu_vtight_1",    &deepTauVsMu_vtight_1_);
      outtree_->Branch("deepTauVsMu_vtight_2",    &deepTauVsMu_vtight_2_);
      outtree_->Branch("deepTauVsMu_vtight_3",    &deepTauVsMu_vtight_3_);
      outtree_->Branch("deepTauVsMu_vtight_4",    &deepTauVsMu_vtight_4_);

      outtree_->Branch("deepTauVsMu_vvtight_1",   &deepTauVsMu_vvtight_1_);
      outtree_->Branch("deepTauVsMu_vvtight_2",   &deepTauVsMu_vvtight_2_);
      outtree_->Branch("deepTauVsMu_vvtight_3",   &deepTauVsMu_vvtight_3_);
      outtree_->Branch("deepTauVsMu_vvtight_4",   &deepTauVsMu_vvtight_4_);

      // tau decay modes
      outtree_->Branch("tau_decay_mode_1",    &tau_decay_mode_1_);
      outtree_->Branch("tau_decay_mode_2",    &tau_decay_mode_2_);
      outtree_->Branch("tau_decay_mode_3",    &tau_decay_mode_3_);
      outtree_->Branch("tau_decay_mode_4",    &tau_decay_mode_4_);

      // jets properties
      outtree_->Branch("n_jets",            &n_jets_);
      outtree_->Branch("n_bjets",           &n_deepbjets_);

      // data filenames
      outtree_->Branch("isTau",              &isTau_);
      outtree_->Branch("isSingleElectron",   &isSingleElectron_);
      outtree_->Branch("isSingleMuon",       &isSingleMuon_);
      outtree_->Branch("isMuonEG",           &isMuonEG_);

      // other 
      outtree_->Branch("n_jetfakes",         &n_jetfakes_);

      // deepjet scores on jet matched to tau
      outtree_->Branch("jet_probb_1",          &jet_probb_1_);
      outtree_->Branch("jet_probb_2",          &jet_probb_2_);
      outtree_->Branch("jet_probb_3",          &jet_probb_3_);
      outtree_->Branch("jet_probb_4",          &jet_probb_4_);
      outtree_->Branch("jet_probbb_1",         &jet_probbb_1_);
      outtree_->Branch("jet_probbb_2",         &jet_probbb_2_);
      outtree_->Branch("jet_probbb_3",         &jet_probbb_3_);
      outtree_->Branch("jet_probbb_4",         &jet_probbb_4_);
      outtree_->Branch("jet_problepb_1",       &jet_problepb_1_);
      outtree_->Branch("jet_problepb_2",       &jet_problepb_2_);
      outtree_->Branch("jet_problepb_3",       &jet_problepb_3_);
      outtree_->Branch("jet_problepb_4",       &jet_problepb_4_);
      outtree_->Branch("jet_probc_1",          &jet_probc_1_);
      outtree_->Branch("jet_probc_2",          &jet_probc_2_);
      outtree_->Branch("jet_probc_3",          &jet_probc_3_);
      outtree_->Branch("jet_probc_4",          &jet_probc_4_);
      outtree_->Branch("jet_probuds_1",        &jet_probuds_1_);
      outtree_->Branch("jet_probuds_2",        &jet_probuds_2_);
      outtree_->Branch("jet_probuds_3",        &jet_probuds_3_);
      outtree_->Branch("jet_probuds_4",        &jet_probuds_4_);
      outtree_->Branch("jet_probg_1",          &jet_probg_1_);
      outtree_->Branch("jet_probg_2",          &jet_probg_2_);
      outtree_->Branch("jet_probg_3",          &jet_probg_3_);
      outtree_->Branch("jet_probg_4",          &jet_probg_4_);

      outtree_->Branch("wt_ggZZ_k_factor",         &wt_ggZZ_);
      outtree_->Branch("wt_qqZZ_k_factor",         &wt_qqZZ_);
      outtree_->Branch("wt_ggZZ_PDFScaleUp",       &wt_ggZZ_PDFScaleUp_);
      outtree_->Branch("wt_ggZZ_PDFScaleDn",       &wt_ggZZ_PDFScaleDn_);
      outtree_->Branch("wt_ggZZ_QCDScaleUp",       &wt_ggZZ_QCDScaleUp_);
      outtree_->Branch("wt_ggZZ_QCDScaleDn",       &wt_ggZZ_QCDScaleDn_);
      outtree_->Branch("wt_ggZZ_AsUp",             &wt_ggZZ_AsUp_);
      outtree_->Branch("wt_ggZZ_AsDn",             &wt_ggZZ_AsDn_);
      outtree_->Branch("wt_ggZZ_PDFReplicaUp",&wt_ggZZ_PDFReplicaUp_);
      outtree_->Branch("wt_ggZZ_PDFReplicaDn",&wt_ggZZ_PDFReplicaDn_);

      outtree_->Branch("wt_mc_1001",&scale_1_);
      outtree_->Branch("wt_mc_1002",&scale_2_);
      outtree_->Branch("wt_mc_1003",&scale_3_);
      outtree_->Branch("wt_mc_1004",&scale_4_);
      outtree_->Branch("wt_mc_1005",&scale_5_);
      outtree_->Branch("wt_mc_1006",&scale_6_);
      outtree_->Branch("wt_mc_1007",&scale_7_);
      outtree_->Branch("wt_mc_1008",&scale_8_);
      outtree_->Branch("wt_mc_1009",&scale_9_);

      outtree_->Branch("wt_prefire",      &wt_prefire_);
      outtree_->Branch("wt_prefire_up",   &wt_prefire_up_);
      outtree_->Branch("wt_prefire_down", &wt_prefire_down_);

    }

    return 0;
  }

  int HTTCategories::Execute(TreeEvent *event) {

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    wt_mc_sign_ = eventInfo->weight_defined("wt_mc_sign") ? eventInfo->weight("wt_mc_sign") : 1.0;

    wt_prefire_ = eventInfo->weight_defined("wt_prefire") ? eventInfo->weight("wt_prefire") : 1.0;
    wt_prefire_up_ = eventInfo->weight_defined("wt_prefire_up") ? eventInfo->weight("wt_prefire_up") : 1.0;
    wt_prefire_down_ = eventInfo->weight_defined("wt_prefire_down") ? eventInfo->weight("wt_prefire_down") : 1.0;

    std::vector<CompositeCandidate *> const& fourtau_vec = event->GetPtrVec<CompositeCandidate>(fourtau_label_);
    CompositeCandidate const* fourtau = fourtau_vec.at(0);

    Candidate const* lep1 = fourtau->GetCandidate("lepton1");
    Candidate const* lep2 = fourtau->GetCandidate("lepton2");
    Candidate const* lep3 = fourtau->GetCandidate("lepton3");
    Candidate const* lep4 = 0;
    if (channel_ != channel::ttt) lep4 = fourtau->GetCandidate("lepton4");
    

    run_ = eventInfo->run();
    event_ = (unsigned long long) eventInfo->event();
    lumi_ = eventInfo->lumi_block();
    rho_ = eventInfo->jet_rho();

    wt_ = {eventInfo->total_weight(), static_cast<float>(eventInfo->total_weight())};
    wt_wsoup_ = eventInfo->weight_defined("wsoup") ? eventInfo->weight("wsoup") : 1.0;
    wt_dysoup_ = eventInfo->weight_defined("dysoup") ? eventInfo->weight("dysoup") : 1.0;

    gen_match_1_ = event->Exists("gen_match_1") ? MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1")) : 0.0;
    gen_match_2_ = event->Exists("gen_match_2") ? MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2")) : 0.0;
    gen_match_3_ = event->Exists("gen_match_3") ? MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_3")) : 0.0;
    gen_match_4_ = event->Exists("gen_match_4") ? MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_4")) : 0.0;

    if (channel_ == channel::tttt) {
      n_jetfakes_ = (gen_match_1_==6) + (gen_match_2_==6) + (gen_match_3_==6) + (gen_match_4_==6);
    }
    if (channel_ == channel::mttt || channel_ == channel::ettt) {
      n_jetfakes_ = (gen_match_2_==6) + (gen_match_3_==6) + (gen_match_4_==6);
    }
    if (channel_ == channel::mmtt || channel_ == channel::eett || channel_ == channel::emtt) {
      n_jetfakes_ = (gen_match_3_==6) + (gen_match_4_==6);
    }
    if (channel_ == channel::ttt) {
      n_jetfakes_ = (gen_match_1_==6) + (gen_match_2_==6) + (gen_match_3_==6);
    }
    if (channel_ == channel::mmmm){
      n_jetfakes_ = 0;
    }

    pdgid_mother_1_ = event->Exists("pdgid_mother_1") ? event->Get<int>("pdgid_mother_1") : 0;
    pdgid_mother_2_ = event->Exists("pdgid_mother_2") ? event->Get<int>("pdgid_mother_2") : 0;
    pdgid_mother_3_ = event->Exists("pdgid_mother_3") ? event->Get<int>("pdgid_mother_3") : 0; 
    pdgid_mother_4_ = event->Exists("pdgid_mother_4") ? event->Get<int>("pdgid_mother_4") : 0; 

    all_taus_from_bosons_ = (((((pdgid_mother_1_ == 25) + (pdgid_mother_2_ == 25) + (pdgid_mother_3_ == 25) + (pdgid_mother_4_ == 25)) == 2) + (((pdgid_mother_1_ == 35) + (pdgid_mother_2_ == 35) + (pdgid_mother_3_ == 35) + (pdgid_mother_4_ == 35)) == 2) + (((pdgid_mother_1_ == 36) + (pdgid_mother_2_ == 36) + (pdgid_mother_3_ == 36) + (pdgid_mother_4_ == 36)) == 2)) == 2);
    pair_number_ = -9999;
    if(channel_ != channel::ttt){
       if ((pdgid_mother_1_ == pdgid_mother_2_) && (pdgid_mother_3_ == pdgid_mother_4_)) {
          pair_number_ = 0;
       } else if ((pdgid_mother_1_ == pdgid_mother_3_) && (pdgid_mother_2_ == pdgid_mother_4_)) {
         pair_number_ = 1;
       } else if ((pdgid_mother_1_ == pdgid_mother_4_) && (pdgid_mother_2_ == pdgid_mother_3_)) {
         pair_number_ = 2;
       }
    } else if (channel_ == channel::ttt){
      if (pdgid_mother_1_ == pdgid_mother_2_){
         pair_number_ = 0;
      } else if (pdgid_mother_1_ == pdgid_mother_3_){
        pair_number_ = 1;
      } else if (pdgid_mother_2_ == pdgid_mother_3_){
        pair_number_ = 2;
      }
    }

    trg_doubletau_12_ = event->Exists("trg_doubletau_12") ? event->Get<bool>("trg_doubletau_12") : false;
    trg_doubletau_13_ = event->Exists("trg_doubletau_13") ? event->Get<bool>("trg_doubletau_13") : false;
    trg_doubletau_14_ = event->Exists("trg_doubletau_14") ? event->Get<bool>("trg_doubletau_14") : false;
    trg_doubletau_23_ = event->Exists("trg_doubletau_23") ? event->Get<bool>("trg_doubletau_23") : false;
    trg_doubletau_24_ = event->Exists("trg_doubletau_24") ? event->Get<bool>("trg_doubletau_24") : false;
    trg_doubletau_34_ = event->Exists("trg_doubletau_34") ? event->Get<bool>("trg_doubletau_34") : false;
    trg_doubleelectron_12_ = event->Exists("trg_doubleelectron_12") ? event->Get<bool>("trg_doubleelectron_12") : false;
    trg_doublemuon_12_ = event->Exists("trg_doublemuon_12") ? event->Get<bool>("trg_doublemuon_12") : false;
    trg_singlemuon_1_ = event->Exists("trg_singlemuon_1") ? event->Get<bool>("trg_singlemuon_1") : false;
    trg_singlemuon_2_ = event->Exists("trg_singlemuon_2") ? event->Get<bool>("trg_singlemuon_2") : false;
    trg_singlemuon_3_ = event->Exists("trg_singlemuon_3") ? event->Get<bool>("trg_singlemuon_3") : false;
    trg_singlemuon_4_ = event->Exists("trg_singlemuon_4") ? event->Get<bool>("trg_singlemuon_4") : false;
    trg_singleelectron_1_ = event->Exists("trg_singleelectron_1") ? event->Get<bool>("trg_singleelectron_1") : false;
    trg_singleelectron_2_ = event->Exists("trg_singleelectron_2") ? event->Get<bool>("trg_singleelectron_2") : false;
    trg_mutaucross_12_ = event->Exists("trg_mutaucross_12") ? event->Get<bool>("trg_mutaucross_12") : false;
    trg_mutaucross_13_ = event->Exists("trg_mutaucross_13") ? event->Get<bool>("trg_mutaucross_13") : false;
    trg_mutaucross_14_ = event->Exists("trg_mutaucross_14") ? event->Get<bool>("trg_mutaucross_14") : false;
    trg_mutaucross_23_ = event->Exists("trg_mutaucross_23") ? event->Get<bool>("trg_mutaucross_23") : false;
    trg_mutaucross_24_ = event->Exists("trg_mutaucross_24") ? event->Get<bool>("trg_mutaucross_24") : false;
    trg_etaucross_12_ = event->Exists("trg_etaucross_12") ? event->Get<bool>("trg_etaucross_12") : false;
    trg_etaucross_13_ = event->Exists("trg_etaucross_13") ? event->Get<bool>("trg_etaucross_13") : false;
    trg_etaucross_14_ = event->Exists("trg_etaucross_14") ? event->Get<bool>("trg_etaucross_14") : false;
    trg_etaucross_23_ = event->Exists("trg_etaucross_23") ? event->Get<bool>("trg_etaucross_23") : false;
    trg_etaucross_24_ = event->Exists("trg_etaucross_24") ? event->Get<bool>("trg_etaucross_24") : false;
    trg_emucross_12_ = event->Exists("trg_emucross_12") ? event->Get<bool>("trg_emucross_12") : false;

    trg_doubletau_leg_1_ = event->Exists("trg_doubletau_leg_1") ? event->Get<bool>("trg_doubletau_leg_1") : false;
    trg_doubletau_leg_2_ = event->Exists("trg_doubletau_leg_2") ? event->Get<bool>("trg_doubletau_leg_2") : false;
    trg_doubletau_leg_3_ = event->Exists("trg_doubletau_leg_3") ? event->Get<bool>("trg_doubletau_leg_3") : false;
    trg_doubletau_leg_4_ = event->Exists("trg_doubletau_leg_4") ? event->Get<bool>("trg_doubletau_leg_4") : false;

    idisoweight_1_ = event->Exists("idisoweight_1") ? event->Get<double>("idisoweight_1") : 1.0;
    idisoweight_2_ = event->Exists("idisoweight_2") ? event->Get<double>("idisoweight_2") : 1.0;
    idisoweight_3_ = event->Exists("idisoweight_3") ? event->Get<double>("idisoweight_3") : 1.0;
    idisoweight_4_ = event->Exists("idisoweight_4") ? event->Get<double>("idisoweight_4") : 1.0;

    idisoweight_ratio_1_up_ = event->Exists("idisoweight_ratio_1_up") ? event->Get<double>("idisoweight_ratio_1_up") : 1.0;
    idisoweight_ratio_2_up_ = event->Exists("idisoweight_ratio_2_up") ? event->Get<double>("idisoweight_ratio_2_up") : 1.0;
    idisoweight_ratio_3_up_ = event->Exists("idisoweight_ratio_3_up") ? event->Get<double>("idisoweight_ratio_3_up") : 1.0;
    idisoweight_ratio_4_up_ = event->Exists("idisoweight_ratio_4_up") ? event->Get<double>("idisoweight_ratio_4_up") : 1.0;

    idisoweight_ratio_1_down_ = event->Exists("idisoweight_ratio_1_down") ? event->Get<double>("idisoweight_ratio_1_down") : 1.0;
    idisoweight_ratio_2_down_ = event->Exists("idisoweight_ratio_2_down") ? event->Get<double>("idisoweight_ratio_2_down") : 1.0;
    idisoweight_ratio_3_down_ = event->Exists("idisoweight_ratio_3_down") ? event->Get<double>("idisoweight_ratio_3_down") : 1.0;
    idisoweight_ratio_4_down_ = event->Exists("idisoweight_ratio_4_down") ? event->Get<double>("idisoweight_ratio_4_down") : 1.0;

    idisoweight_vvvloose_1_ = event->Exists("idisoweight_vvvloose_1") ? event->Get<double>("idisoweight_vvvloose_1") : 1.0;
    idisoweight_vvvloose_2_ = event->Exists("idisoweight_vvvloose_2") ? event->Get<double>("idisoweight_vvvloose_2") : 1.0;
    idisoweight_vvvloose_3_ = event->Exists("idisoweight_vvvloose_3") ? event->Get<double>("idisoweight_vvvloose_3") : 1.0;
    idisoweight_vvvloose_4_ = event->Exists("idisoweight_vvvloose_4") ? event->Get<double>("idisoweight_vvvloose_4") : 1.0;

    idisoweight_vvloose_1_ = event->Exists("idisoweight_vvloose_1") ? event->Get<double>("idisoweight_vvloose_1") : 1.0;
    idisoweight_vvloose_2_ = event->Exists("idisoweight_vvloose_2") ? event->Get<double>("idisoweight_vvloose_2") : 1.0;
    idisoweight_vvloose_3_ = event->Exists("idisoweight_vvloose_3") ? event->Get<double>("idisoweight_vvloose_3") : 1.0;
    idisoweight_vvloose_4_ = event->Exists("idisoweight_vvloose_4") ? event->Get<double>("idisoweight_vvloose_4") : 1.0;

    idisoweight_vloose_1_ = event->Exists("idisoweight_vloose_1") ? event->Get<double>("idisoweight_vloose_1") : 1.0;
    idisoweight_vloose_2_ = event->Exists("idisoweight_vloose_2") ? event->Get<double>("idisoweight_vloose_2") : 1.0;
    idisoweight_vloose_3_ = event->Exists("idisoweight_vloose_3") ? event->Get<double>("idisoweight_vloose_3") : 1.0;
    idisoweight_vloose_4_ = event->Exists("idisoweight_vloose_4") ? event->Get<double>("idisoweight_vloose_4") : 1.0;

    idisoweight_loose_1_ = event->Exists("idisoweight_loose_1") ? event->Get<double>("idisoweight_loose_1") : 1.0;
    idisoweight_loose_2_ = event->Exists("idisoweight_loose_2") ? event->Get<double>("idisoweight_loose_2") : 1.0;
    idisoweight_loose_3_ = event->Exists("idisoweight_loose_3") ? event->Get<double>("idisoweight_loose_3") : 1.0;
    idisoweight_loose_4_ = event->Exists("idisoweight_loose_4") ? event->Get<double>("idisoweight_loose_4") : 1.0;

    idisoweight_medium_1_ = event->Exists("idisoweight_medium_1") ? event->Get<double>("idisoweight_medium_1") : 1.0;
    idisoweight_medium_2_ = event->Exists("idisoweight_medium_2") ? event->Get<double>("idisoweight_medium_2") : 1.0;
    idisoweight_medium_3_ = event->Exists("idisoweight_medium_3") ? event->Get<double>("idisoweight_medium_3") : 1.0;
    idisoweight_medium_4_ = event->Exists("idisoweight_medium_4") ? event->Get<double>("idisoweight_medium_4") : 1.0;

    idisoweight_tight_1_ = event->Exists("idisoweight_tight_1") ? event->Get<double>("idisoweight_tight_1") : 1.0;
    idisoweight_tight_2_ = event->Exists("idisoweight_tight_2") ? event->Get<double>("idisoweight_tight_2") : 1.0;
    idisoweight_tight_3_ = event->Exists("idisoweight_tight_3") ? event->Get<double>("idisoweight_tight_3") : 1.0;
    idisoweight_tight_4_ = event->Exists("idisoweight_tight_4") ? event->Get<double>("idisoweight_tight_4") : 1.0;

    etau_fakerate_1_ = event->Exists("etau_fakerate_1") ? event->Get<double>("etau_fakerate_1") : 1.0;
    etau_fakerate_2_ = event->Exists("etau_fakerate_2") ? event->Get<double>("etau_fakerate_2") : 1.0;
    etau_fakerate_3_ = event->Exists("etau_fakerate_3") ? event->Get<double>("etau_fakerate_3") : 1.0;
    etau_fakerate_4_ = event->Exists("etau_fakerate_4") ? event->Get<double>("etau_fakerate_4") : 1.0;

    etau_fakerate_ratio_1_up_ = event->Exists("etau_fakerate_ratio_1_up") ? event->Get<double>("etau_fakerate_ratio_1_up") : 1.0;
    etau_fakerate_ratio_2_up_ = event->Exists("etau_fakerate_ratio_2_up") ? event->Get<double>("etau_fakerate_ratio_2_up") : 1.0;
    etau_fakerate_ratio_3_up_ = event->Exists("etau_fakerate_ratio_3_up") ? event->Get<double>("etau_fakerate_ratio_3_up") : 1.0;
    etau_fakerate_ratio_4_up_ = event->Exists("etau_fakerate_ratio_4_up") ? event->Get<double>("etau_fakerate_ratio_4_up") : 1.0;

    etau_fakerate_ratio_1_down_ = event->Exists("etau_fakerate_ratio_1_down") ? event->Get<double>("etau_fakerate_ratio_1_down") : 1.0;
    etau_fakerate_ratio_2_down_ = event->Exists("etau_fakerate_ratio_2_down") ? event->Get<double>("etau_fakerate_ratio_2_down") : 1.0;
    etau_fakerate_ratio_3_down_ = event->Exists("etau_fakerate_ratio_3_down") ? event->Get<double>("etau_fakerate_ratio_3_down") : 1.0;
    etau_fakerate_ratio_4_down_ = event->Exists("etau_fakerate_ratio_4_down") ? event->Get<double>("etau_fakerate_ratio_4_down") : 1.0;

    etau_fakerate_vvloose_1_ = event->Exists("etau_fakerate_vvloose_1") ? event->Get<double>("etau_fakerate_vvloose_1") : 1.0;
    etau_fakerate_vvloose_2_ = event->Exists("etau_fakerate_vvloose_2") ? event->Get<double>("etau_fakerate_vvloose_2") : 1.0;
    etau_fakerate_vvloose_3_ = event->Exists("etau_fakerate_vvloose_3") ? event->Get<double>("etau_fakerate_vvloose_3") : 1.0;
    etau_fakerate_vvloose_4_ = event->Exists("etau_fakerate_vvloose_4") ? event->Get<double>("etau_fakerate_vvloose_4") : 1.0;

    etau_fakerate_vloose_1_ = event->Exists("etau_fakerate_vloose_1") ? event->Get<double>("etau_fakerate_vloose_1") : 1.0;
    etau_fakerate_vloose_2_ = event->Exists("etau_fakerate_vloose_2") ? event->Get<double>("etau_fakerate_vloose_2") : 1.0;
    etau_fakerate_vloose_3_ = event->Exists("etau_fakerate_vloose_3") ? event->Get<double>("etau_fakerate_vloose_3") : 1.0;
    etau_fakerate_vloose_4_ = event->Exists("etau_fakerate_vloose_4") ? event->Get<double>("etau_fakerate_vloose_4") : 1.0;

    etau_fakerate_loose_1_ = event->Exists("etau_fakerate_loose_1") ? event->Get<double>("etau_fakerate_loose_1") : 1.0;
    etau_fakerate_loose_2_ = event->Exists("etau_fakerate_loose_2") ? event->Get<double>("etau_fakerate_loose_2") : 1.0;
    etau_fakerate_loose_3_ = event->Exists("etau_fakerate_loose_3") ? event->Get<double>("etau_fakerate_loose_3") : 1.0;
    etau_fakerate_loose_4_ = event->Exists("etau_fakerate_loose_4") ? event->Get<double>("etau_fakerate_loose_4") : 1.0;

    etau_fakerate_medium_1_ = event->Exists("etau_fakerate_medium_1") ? event->Get<double>("etau_fakerate_medium_1") : 1.0;
    etau_fakerate_medium_2_ = event->Exists("etau_fakerate_medium_2") ? event->Get<double>("etau_fakerate_medium_2") : 1.0;
    etau_fakerate_medium_3_ = event->Exists("etau_fakerate_medium_3") ? event->Get<double>("etau_fakerate_medium_3") : 1.0;
    etau_fakerate_medium_4_ = event->Exists("etau_fakerate_medium_4") ? event->Get<double>("etau_fakerate_medium_4") : 1.0;

    etau_fakerate_tight_1_ = event->Exists("etau_fakerate_tight_1") ? event->Get<double>("etau_fakerate_tight_1") : 1.0;
    etau_fakerate_tight_2_ = event->Exists("etau_fakerate_tight_2") ? event->Get<double>("etau_fakerate_tight_2") : 1.0;
    etau_fakerate_tight_3_ = event->Exists("etau_fakerate_tight_3") ? event->Get<double>("etau_fakerate_tight_3") : 1.0;
    etau_fakerate_tight_4_ = event->Exists("etau_fakerate_tight_4") ? event->Get<double>("etau_fakerate_tight_4") : 1.0;

    etau_fakerate_vtight_1_ = event->Exists("etau_fakerate_vtight_1") ? event->Get<double>("etau_fakerate_vtight_1") : 1.0;
    etau_fakerate_vtight_2_ = event->Exists("etau_fakerate_vtight_2") ? event->Get<double>("etau_fakerate_vtight_2") : 1.0;
    etau_fakerate_vtight_3_ = event->Exists("etau_fakerate_vtight_3") ? event->Get<double>("etau_fakerate_vtight_3") : 1.0;
    etau_fakerate_vtight_4_ = event->Exists("etau_fakerate_vtight_4") ? event->Get<double>("etau_fakerate_vtight_4") : 1.0;

    mtau_fakerate_1_ = event->Exists("mtau_fakerate_1") ? event->Get<double>("mtau_fakerate_1") : 1.0;
    mtau_fakerate_2_ = event->Exists("mtau_fakerate_2") ? event->Get<double>("mtau_fakerate_2") : 1.0;
    mtau_fakerate_3_ = event->Exists("mtau_fakerate_3") ? event->Get<double>("mtau_fakerate_3") : 1.0;
    mtau_fakerate_4_ = event->Exists("mtau_fakerate_4") ? event->Get<double>("mtau_fakerate_4") : 1.0;
   
    mtau_fakerate_ratio_1_up_ = event->Exists("mtau_fakerate_ratio_1_up") ? event->Get<double>("mtau_fakerate_ratio_1_up") : 1.0;
    mtau_fakerate_ratio_2_up_ = event->Exists("mtau_fakerate_ratio_2_up") ? event->Get<double>("mtau_fakerate_ratio_2_up") : 1.0;
    mtau_fakerate_ratio_3_up_ = event->Exists("mtau_fakerate_ratio_3_up") ? event->Get<double>("mtau_fakerate_ratio_3_up") : 1.0;
    mtau_fakerate_ratio_4_up_ = event->Exists("mtau_fakerate_ratio_4_up") ? event->Get<double>("mtau_fakerate_ratio_4_up") : 1.0;

    mtau_fakerate_ratio_1_down_ = event->Exists("mtau_fakerate_ratio_1_down") ? event->Get<double>("mtau_fakerate_ratio_1_down") : 1.0;
    mtau_fakerate_ratio_2_down_ = event->Exists("mtau_fakerate_ratio_2_down") ? event->Get<double>("mtau_fakerate_ratio_2_down") : 1.0;
    mtau_fakerate_ratio_3_down_ = event->Exists("mtau_fakerate_ratio_3_down") ? event->Get<double>("mtau_fakerate_ratio_3_down") : 1.0;
    mtau_fakerate_ratio_4_down_ = event->Exists("mtau_fakerate_ratio_4_down") ? event->Get<double>("mtau_fakerate_ratio_4_down") : 1.0;
 
    mtau_fakerate_vloose_1_ = event->Exists("mtau_fakerate_vloose_1") ? event->Get<double>("mtau_fakerate_vloose_1") : 1.0;
    mtau_fakerate_vloose_2_ = event->Exists("mtau_fakerate_vloose_2") ? event->Get<double>("mtau_fakerate_vloose_2") : 1.0;
    mtau_fakerate_vloose_3_ = event->Exists("mtau_fakerate_vloose_3") ? event->Get<double>("mtau_fakerate_vloose_3") : 1.0;
    mtau_fakerate_vloose_4_ = event->Exists("mtau_fakerate_vloose_4") ? event->Get<double>("mtau_fakerate_vloose_4") : 1.0;

    mtau_fakerate_loose_1_ = event->Exists("mtau_fakerate_loose_1") ? event->Get<double>("mtau_fakerate_loose_1") : 1.0;
    mtau_fakerate_loose_2_ = event->Exists("mtau_fakerate_loose_2") ? event->Get<double>("mtau_fakerate_loose_2") : 1.0;
    mtau_fakerate_loose_3_ = event->Exists("mtau_fakerate_loose_3") ? event->Get<double>("mtau_fakerate_loose_3") : 1.0;
    mtau_fakerate_loose_4_ = event->Exists("mtau_fakerate_loose_4") ? event->Get<double>("mtau_fakerate_loose_4") : 1.0;

    mtau_fakerate_medium_1_ = event->Exists("mtau_fakerate_medium_1") ? event->Get<double>("mtau_fakerate_medium_1") : 1.0;
    mtau_fakerate_medium_2_ = event->Exists("mtau_fakerate_medium_2") ? event->Get<double>("mtau_fakerate_medium_2") : 1.0;
    mtau_fakerate_medium_3_ = event->Exists("mtau_fakerate_medium_3") ? event->Get<double>("mtau_fakerate_medium_3") : 1.0;
    mtau_fakerate_medium_4_ = event->Exists("mtau_fakerate_medium_4") ? event->Get<double>("mtau_fakerate_medium_4") : 1.0;

    mtau_fakerate_tight_1_ = event->Exists("mtau_fakerate_tight_1") ? event->Get<double>("mtau_fakerate_tight_1") : 1.0;
    mtau_fakerate_tight_2_ = event->Exists("mtau_fakerate_tight_2") ? event->Get<double>("mtau_fakerate_tight_2") : 1.0;
    mtau_fakerate_tight_3_ = event->Exists("mtau_fakerate_tight_3") ? event->Get<double>("mtau_fakerate_tight_3") : 1.0;
    mtau_fakerate_tight_4_ = event->Exists("mtau_fakerate_tight_4") ? event->Get<double>("mtau_fakerate_tight_4") : 1.0;

    trackingweight_1_ = event->Exists("trackingweight_1") ? event->Get<double>("trackingweight_1") : 1.0;
    trackingweight_2_ = event->Exists("trackingweight_2") ? event->Get<double>("trackingweight_2") : 1.0;
    trackingweight_3_ = event->Exists("trackingweight_3") ? event->Get<double>("trackingweight_3") : 1.0;
    trackingweight_4_ = event->Exists("trackingweight_4") ? event->Get<double>("trackingweight_4") : 1.0;

    trigeff_doubletau_data_1_ = event->Exists("trigeff_t_doubletau_data_1") ? event->Get<double>("trigeff_t_doubletau_data_1") : 1.0;
    trigeff_doubletau_data_2_ = event->Exists("trigeff_t_doubletau_data_2") ? event->Get<double>("trigeff_t_doubletau_data_2") : 1.0;
    trigeff_doubletau_data_3_ = event->Exists("trigeff_t_doubletau_data_3") ? event->Get<double>("trigeff_t_doubletau_data_3") : 1.0;
    trigeff_doubletau_data_4_ = event->Exists("trigeff_t_doubletau_data_4") ? event->Get<double>("trigeff_t_doubletau_data_4") : 1.0;

    trigeff_etaucross_data_1_ = event->Exists("trigeff_e_etaucross_data_1") ? event->Get<double>("trigeff_e_etaucross_data_1") : 1.0;
    if(channel_ == channel::ettt) {
      trigeff_etaucross_data_2_ = event->Exists("trigeff_t_etaucross_data_2") ? event->Get<double>("trigeff_t_etaucross_data_2") : 1.0;
    } else if(channel_ == channel::eett) {
      trigeff_etaucross_data_2_ = event->Exists("trigeff_e_etaucross_data_2") ? event->Get<double>("trigeff_e_etaucross_data_2") : 1.0;
    } else {
      trigeff_etaucross_data_2_ = 1.0;
    }
    trigeff_etaucross_data_3_ = event->Exists("trigeff_t_etaucross_data_3") ? event->Get<double>("trigeff_t_etaucross_data_3") : 1.0;
    trigeff_etaucross_data_4_ = event->Exists("trigeff_t_etaucross_data_4") ? event->Get<double>("trigeff_t_etaucross_data_4") : 1.0;

    trigeff_mutaucross_data_1_ = event->Exists("trigeff_m_mutaucross_data_1") ? event->Get<double>("trigeff_m_mutaucross_data_1") : 1.0;
    if(channel_ == channel::mttt) {
      trigeff_mutaucross_data_2_ =  event->Exists("trigeff_t_mutaucross_data_2") ? event->Get<double>("trigeff_t_mutaucross_data_2") : 1.0;
    } else if(channel_ == channel::mmtt || channel_ == channel::emtt) {
      trigeff_mutaucross_data_2_ =  event->Exists("trigeff_m_mutaucross_data_2") ? event->Get<double>("trigeff_m_mutaucross_data_2") : 1.0;
    } else {
      trigeff_mutaucross_data_2_ = 1.0;
    }
    trigeff_mutaucross_data_3_ = event->Exists("trigeff_t_mutaucross_data_3") ? event->Get<double>("trigeff_t_mutaucross_data_3") : 1.0;
    trigeff_mutaucross_data_4_ = event->Exists("trigeff_t_mutaucross_data_4") ? event->Get<double>("trigeff_t_mutaucross_data_4") : 1.0;

    trigeff_singlee_data_1_ = event->Exists("trigeff_e_singlee_data_1") ? event->Get<double>("trigeff_e_singlee_data_1") : 1.0;
    trigeff_singlee_data_2_ = event->Exists("trigeff_e_singlee_data_2") ? event->Get<double>("trigeff_e_singlee_data_2") : 1.0;
    trigeff_singlem_data_1_ = event->Exists("trigeff_m_singlem_data_1") ? event->Get<double>("trigeff_m_singlem_data_1") : 1.0;
    trigeff_singlem_data_2_ = event->Exists("trigeff_m_singlem_data_2") ? event->Get<double>("trigeff_m_singlem_data_2") : 1.0;

    trigeff_emucross_high_data_1_ = event->Exists("trigeff_e_emucross_high_data_1") ? event->Get<double>("trigeff_e_emucross_high_data_1") : 1.0;
    trigeff_emucross_high_data_2_ = event->Exists("trigeff_m_emucross_high_data_2") ? event->Get<double>("trigeff_m_emucross_high_data_2") : 1.0;
    trigeff_emucross_low_data_1_ = event->Exists("trigeff_e_emucross_low_data_1") ? event->Get<double>("trigeff_e_emucross_low_data_1") : 1.0;
    trigeff_emucross_low_data_2_ = event->Exists("trigeff_e_emucross_low_data_2") ? event->Get<double>("trigeff_e_emucross_low_data_2") : 1.0;

    trigeff_doubletau_mc_1_ = event->Exists("trigeff_t_doubletau_mc_1") ? event->Get<double>("trigeff_t_doubletau_mc_1") : 1.0;
    trigeff_doubletau_mc_2_ = event->Exists("trigeff_t_doubletau_mc_2") ? event->Get<double>("trigeff_t_doubletau_mc_2") : 1.0;
    trigeff_doubletau_mc_3_ = event->Exists("trigeff_t_doubletau_mc_3") ? event->Get<double>("trigeff_t_doubletau_mc_3") : 1.0;
    trigeff_doubletau_mc_4_ = event->Exists("trigeff_t_doubletau_mc_4") ? event->Get<double>("trigeff_t_doubletau_mc_4") : 1.0;

    trigeff_etaucross_mc_1_ = event->Exists("trigeff_e_etaucross_mc_1") ? event->Get<double>("trigeff_e_etaucross_mc_1") : 1.0;
    if(channel_ == channel::ettt) {
      trigeff_etaucross_mc_2_ = event->Exists("trigeff_t_etaucross_mc_2") ? event->Get<double>("trigeff_t_etaucross_mc_2") : 1.0;
    } else if(channel_ == channel::eett) {
      trigeff_etaucross_mc_2_ = event->Exists("trigeff_e_etaucross_mc_2") ? event->Get<double>("trigeff_e_etaucross_mc_2") : 1.0;
    } else {
      trigeff_etaucross_mc_2_ = 1.0;
    }
    trigeff_etaucross_mc_3_ = event->Exists("trigeff_t_etaucross_mc_3") ? event->Get<double>("trigeff_t_etaucross_mc_3") : 1.0;
    trigeff_etaucross_mc_4_ = event->Exists("trigeff_t_etaucross_mc_4") ? event->Get<double>("trigeff_t_etaucross_mc_4") : 1.0;

    trigeff_mutaucross_mc_1_ = event->Exists("trigeff_m_mutaucross_mc_1") ? event->Get<double>("trigeff_m_mutaucross_mc_1") : 1.0;
    if(channel_ == channel::mttt) {
      trigeff_mutaucross_mc_2_ =  event->Exists("trigeff_t_mutaucross_mc_2") ? event->Get<double>("trigeff_t_mutaucross_mc_2") : 1.0;
    } else if(channel_ == channel::mmtt || channel_ == channel::emtt) {
      trigeff_mutaucross_mc_2_ =  event->Exists("trigeff_m_mutaucross_mc_2") ? event->Get<double>("trigeff_m_mutaucross_mc_2") : 1.0;
    } else {
      trigeff_mutaucross_mc_2_ = 1.0;
    }
    trigeff_mutaucross_mc_3_ = event->Exists("trigeff_t_mutaucross_mc_3") ? event->Get<double>("trigeff_t_mutaucross_mc_3") : 1.0;
    trigeff_mutaucross_mc_4_ = event->Exists("trigeff_t_mutaucross_mc_4") ? event->Get<double>("trigeff_t_mutaucross_mc_4") : 1.0;

    trigeff_singlee_mc_1_ = event->Exists("trigeff_e_singlee_mc_1") ? event->Get<double>("trigeff_e_singlee_mc_1") : 1.0;
    trigeff_singlee_mc_2_ = event->Exists("trigeff_e_singlee_mc_2") ? event->Get<double>("trigeff_e_singlee_mc_2") : 1.0;
    trigeff_singlem_mc_1_ = event->Exists("trigeff_m_singlem_mc_1") ? event->Get<double>("trigeff_m_singlem_mc_1") : 1.0;
    trigeff_singlem_mc_2_ = event->Exists("trigeff_m_singlem_mc_2") ? event->Get<double>("trigeff_m_singlem_mc_2") : 1.0;

    trigeff_emucross_high_mc_1_ = event->Exists("trigeff_e_emucross_high_mc_1") ? event->Get<double>("trigeff_e_emucross_high_mc_1") : 1.0;
    trigeff_emucross_high_mc_2_ = event->Exists("trigeff_m_emucross_high_mc_2") ? event->Get<double>("trigeff_m_emucross_high_mc_2") : 1.0;
    trigeff_emucross_low_mc_1_ = event->Exists("trigeff_e_emucross_low_mc_1") ? event->Get<double>("trigeff_e_emucross_low_mc_1") : 1.0;
    trigeff_emucross_low_mc_2_ = event->Exists("trigeff_e_emucross_low_mc_2") ? event->Get<double>("trigeff_e_emucross_low_mc_2") : 1.0;

    total_trg_ = event->Exists("wt_total_trg") ? event->Get<double>("wt_total_trg") : 1.0;

    total_trg_ratio_doubletau_up_ = event->Exists("wt_tau_trg_ratio_up") ? event->Get<double>("wt_tau_trg_ratio_up") : 1.0;
    total_trg_ratio_doubletau_down_ = event->Exists("wt_tau_trg_ratio_down") ? event->Get<double>("wt_tau_trg_ratio_down") : 1.0;

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    pt_3_ = lep3->pt();
	
    jet_pt_1_ = 0;
    jet_pt_2_ = 0;
    jet_pt_3_ = 0;
	
    jet_probb_1_ = 0;
    jet_probb_2_ = 0;
    jet_probb_3_ = 0;
	
    jet_probbb_1_ = 0;    
    jet_probbb_2_ = 0;
    jet_probbb_3_ = 0;
	
    jet_problepb_1_ = 0;    
    jet_problepb_2_ = 0;
    jet_problepb_3_ = 0;
	
    jet_probc_1_ = 0;    
    jet_probc_2_ = 0;
    jet_probc_3_ = 0;
	
    jet_probuds_1_ = 0;    
    jet_probuds_2_ = 0;
    jet_probuds_3_ = 0;
	
    jet_probg_1_ = 0;
    jet_probg_2_ = 0;
    jet_probg_3_ = 0;    
	
    std::vector<PFJet*> uncleaned_jets = event->GetPtrVec<PFJet>("ak4PFJetsCHSUnFiltered");
    std::vector<Candidate *> lepvec1;
    std::vector<Candidate *> lepvec2;
    std::vector<Candidate *> lepvec3;    
	
    lepvec1.push_back(fourtau->GetCandidate("lepton1"));
    lepvec2.push_back(fourtau->GetCandidate("lepton2"));
    lepvec3.push_back(fourtau->GetCandidate("lepton3"));    
	
    std::vector<std::pair<ic::PFJet *, ic::Candidate *>> tau_matches_1 = MatchByDR(uncleaned_jets, lepvec1, 0.5, true, true);
    std::vector<std::pair<ic::PFJet *, ic::Candidate *>> tau_matches_2 = MatchByDR(uncleaned_jets, lepvec2, 0.5, true, true);
    std::vector<std::pair<ic::PFJet *, ic::Candidate *>> tau_matches_3 = MatchByDR(uncleaned_jets, lepvec3, 0.5, true, true);

    if(channel_ == channel::tttt || channel_ == channel::ttt) {
      if(tau_matches_1.size() > 0) {
        jet_pt_1_ = (tau_matches_1.at(0)).first->pt();
        jet_probb_1_ = (tau_matches_1.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probb");
        jet_probbb_1_ = (tau_matches_1.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probbb");
        jet_problepb_1_ = (tau_matches_1.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:problepb");
        jet_probc_1_ = (tau_matches_1.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probc");
        jet_probuds_1_ = (tau_matches_1.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probuds");
        jet_probg_1_ = (tau_matches_1.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probg");
      }
    }
    if(channel_ == channel::tttt || channel_ == channel::ttt ||  channel_ == channel::ettt || channel_ == channel::mttt) {
      if(tau_matches_2.size() > 0) {
        jet_pt_2_ = (tau_matches_2.at(0)).first->pt();
        jet_probb_2_ = (tau_matches_2.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probb");
        jet_probbb_2_ = (tau_matches_2.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probbb");
        jet_problepb_2_ = (tau_matches_2.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:problepb");
        jet_probc_2_ = (tau_matches_2.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probc");
        jet_probuds_2_ = (tau_matches_2.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probuds");
        jet_probg_2_ = (tau_matches_2.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probg");
      }
    }
    if(tau_matches_3.size() > 0) {
      jet_pt_3_ = (tau_matches_3.at(0)).first->pt();
      jet_probb_3_ = (tau_matches_3.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probb");
      jet_probbb_3_ = (tau_matches_3.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probbb");
      jet_problepb_3_ = (tau_matches_3.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:problepb");
      jet_probc_3_ = (tau_matches_3.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probc");
      jet_probuds_3_ = (tau_matches_3.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probuds");
      jet_probg_3_ = (tau_matches_3.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probg");
    }

    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();
    eta_3_ = lep3->eta();

    phi_1_ = lep1->phi();
    phi_2_ = lep2->phi();
    phi_3_ = lep3->phi();

    dphi_12_ = ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector());
    dphi_13_ = ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep3->vector());   
    dphi_23_ = ROOT::Math::VectorUtil::DeltaPhi(lep2->vector(),lep3->vector()); 
   

    dR_12_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep2->vector()));
    dR_13_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep3->vector()));
    dR_23_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep2->vector(),lep3->vector()));
   
    E_1_ = lep1->energy();
    E_2_ = lep2->energy();
    E_3_ = lep3->energy();
   
    m_1_ = lep1->M();
    m_2_ = lep2->M();
    m_3_ = lep3->M();

    Met const* mets = NULL;
    mets = event->GetPtr<Met>(met_label_);
    met_ = mets->vector().pt();
    mt_1_ = MT(lep1, mets);
    mt_2_ = MT(lep2, mets);
    mt_3_ = MT(lep3, mets);

    mt_lep_12_ = MT(lep1, lep2);
    mt_lep_13_ = MT(lep1, lep3);
   
    mt_lep_23_ = MT(lep2, lep3);

    q_1_ = lep1->charge();
    q_2_ = lep2->charge();
    q_3_ = lep3->charge();
   
    q_sum_ = lep1->charge()+lep2->charge()+lep3->charge();
    st_ = lep1->pt()+lep2->pt()+lep3->pt()+mets->vector().pt();
	
    CompositeCandidate *total_product = new CompositeCandidate();
    total_product->AddCandidate("lepton1",fourtau->GetCandidate("lepton1"));
    total_product->AddCandidate("lepton2",fourtau->GetCandidate("lepton2"));
    total_product->AddCandidate("lepton3",fourtau->GetCandidate("lepton3"));
    if (channel_ != channel::ttt) {
      total_product->AddCandidate("lepton4",fourtau->GetCandidate("lepton4"));
    }
   
    mvis_1234_ = total_product->M();
    pt_tttt_ = total_product->pt();
    if (channel_ != channel::ttt) { 
      mt_tot_ = sqrt(pow(mt_lep_12_.var_double,2)+pow(mt_lep_13_.var_double,2)+pow(mt_lep_14_.var_double,2)+pow(mt_lep_23_.var_double,2)+pow(mt_lep_24_.var_double,2)+pow(mt_lep_34_.var_double,2)+pow(mt_1_.var_double,2)+pow(mt_2_.var_double,2)+pow(mt_3_.var_double,2)+pow(mt_4_.var_double,2));
    } else {
      mt_tot_ = sqrt(pow(mt_lep_12_.var_double,2)+pow(mt_lep_13_.var_double,2)+pow(mt_lep_23_.var_double,2)+pow(mt_1_.var_double,2)+pow(mt_2_.var_double,2)+pow(mt_3_.var_double,2));
    }

    // get ditau mass of all combinations:
    CompositeCandidate *pair12 = new CompositeCandidate();
    CompositeCandidate *pair13 = new CompositeCandidate();
    CompositeCandidate *pair23 = new CompositeCandidate();

    pair12->AddCandidate("lepton1",fourtau->GetCandidate("lepton1"));
    pair12->AddCandidate("lepton2",fourtau->GetCandidate("lepton2"));

    pair13->AddCandidate("lepton1",fourtau->GetCandidate("lepton1"));
    pair13->AddCandidate("lepton2",fourtau->GetCandidate("lepton3"));

    pair23->AddCandidate("lepton1",fourtau->GetCandidate("lepton2"));
    pair23->AddCandidate("lepton2",fourtau->GetCandidate("lepton3"));

    //std::cout << "pair12 :" << " px " << pair12->px() << " py " << pair12->py() << " pz " << pair12->pz() << "energy : " << pair12->energy()  << std::endl;
    //std::cout << "pair13 :" << " px " << pair13->px() << " py " << pair13->py() << " pz " << pair13->pz() << "energy : " << pair13->energy()  << std::endl;
    //std::cout << "pair23 :" << " px " << pair23->px() << " py " << pair23->py() << " pz " << pair23->pz() << "energy : " << pair23->energy()  << std::endl;

    mvis_12_ = pair12->M();
    mvis_13_ = pair13->M();
    mvis_23_ = pair23->M();

    pt_tt_12_ = pair12->pt();
    pt_tt_13_ = pair13->pt();
    pt_tt_23_ = pair23->pt();

//    if (channel_ == channel::ttt) {
//      if (fourtau->GetCandidate("lepton1")->charge() == -fourtau->GetCandidate("lepton2")->charge()) {
//        mvis_os_ = pair12->M();
//        pt_tt_os_ = pair12->pt();
//      } else if (fourtau->GetCandidate("lepton1")->charge() == -fourtau->GetCandidate("lepton3")->charge()) {
//        mvis_os_ = pair13->M();
//        pt_tt_os_ = pair13->pt();
//      } else if (fourtau->GetCandidate("lepton2")->charge() == -fourtau->GetCandidate("lepton3")->charge()) {
//        mvis_os_ = pair23->M();
//        pt_tt_os_ = pair23->pt();
//      }
//    }

    double min_dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton1")->vector(),fourtau->GetCandidate("lepton2")->vector()));
    p_min_dR_1_ = 1;
    p_min_dR_2_ = 2;
    if (channel_ == channel::ttt) {
      for(int i = 1; i < 4; i++){
        for(int j = 1; j < 4; j++){
          if (i < j) {  
            if (fourtau->GetCandidate("lepton"+std::to_string(i))->charge() == -fourtau->GetCandidate("lepton"+std::to_string(j))->charge()) {
              if (std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector())) < min_dR_) {
                min_dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()));
                p_min_dR_1_ = i;
                p_min_dR_2_ = j;
              }
            } 
          }
        }
      }
      CompositeCandidate *pair_min_dR = new CompositeCandidate();
      pair_min_dR->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(p_min_dR_1_)));
      pair_min_dR->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(p_min_dR_2_)));
      mvis_min_sum_dR_1_ = pair_min_dR->M();
      pt_min_sum_dR_1_ = pair_min_dR->pt();
      dR_min_sum_dR_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(p_min_dR_1_))->vector(),fourtau->GetCandidate("lepton"+std::to_string(p_min_dR_2_))->vector()));
    }

    mvis_phi_ = 0.0;
    mvis_A_ = 0.0;
    pt_phi_ = 0.0;
    pt_A_ = 0.0;

    if (channel_ != channel::ttt) {
      pt_4_= lep4->pt();
	  
      jet_pt_4_ = 0;
      jet_probb_4_ = 0;
      jet_probbb_4_ = 0;    
      jet_problepb_4_ = 0;
      jet_probc_4_ = 0;
      jet_probuds_4_ = 0;
      jet_probg_4_ = 0;
      std::vector<Candidate *> lepvec4;
      lepvec4.push_back(fourtau->GetCandidate("lepton4"));
      std::vector<std::pair<ic::PFJet *, ic::Candidate *>> tau_matches_4 = MatchByDR(uncleaned_jets, lepvec4, 0.5, true, true);
	    
	    if(tau_matches_4.size() > 0) {
		  jet_pt_4_ = (tau_matches_4.at(0)).first->pt();
		  jet_probb_4_ = (tau_matches_4.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probb");
		  jet_probbb_4_ = (tau_matches_4.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probbb");
		  jet_problepb_4_ = (tau_matches_4.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:problepb");
		  jet_probc_4_ = (tau_matches_4.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probc");
		  jet_probuds_4_ = (tau_matches_4.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probuds");
		  jet_probg_4_ = (tau_matches_4.at(0)).first->GetBDiscriminator("pfDeepFlavourJetTags:probg");
	    }
	  
      eta_4_ = lep4->eta();
      phi_4_ = lep4->phi();
      dphi_14_ = ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep4->vector());
      dphi_34_ = ROOT::Math::VectorUtil::DeltaPhi(lep3->vector(),lep4->vector());
      dphi_24_ = ROOT::Math::VectorUtil::DeltaPhi(lep2->vector(),lep4->vector());
      dR_14_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep4->vector()));
      dR_24_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep2->vector(),lep4->vector()));
      dR_34_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep3->vector(),lep4->vector()));
      E_4_ = lep4->energy();
      m_4_ = lep4->M();
      mt_4_ = MT(lep4, mets);
      mt_lep_14_ = MT(lep1, lep4);
      mt_lep_24_ = MT(lep2, lep4);
      mt_lep_34_ = MT(lep3, lep4);
      q_4_ = lep4->charge();
      q_sum_ = lep1->charge()+lep2->charge()+lep3->charge()+lep4->charge();
      st_ = lep1->pt()+lep2->pt()+lep3->pt()+lep4->pt()+mets->vector().pt();
      CompositeCandidate *pair14 = new CompositeCandidate();
      CompositeCandidate *pair24 = new CompositeCandidate();
      CompositeCandidate *pair34 = new CompositeCandidate();
      pair14->AddCandidate("lepton1",fourtau->GetCandidate("lepton1"));
      pair14->AddCandidate("lepton2",fourtau->GetCandidate("lepton4"));
      pair24->AddCandidate("lepton1",fourtau->GetCandidate("lepton2"));
      pair24->AddCandidate("lepton2",fourtau->GetCandidate("lepton4"));
      pair34->AddCandidate("lepton1",fourtau->GetCandidate("lepton3"));
      pair34->AddCandidate("lepton2",fourtau->GetCandidate("lepton4"));
      mvis_14_ = pair14->M();
      mvis_24_ = pair24->M();
      mvis_34_ = pair34->M();
      pt_tt_14_ = pair14->pt();
      pt_tt_24_ = pair24->pt();
      pt_tt_34_ = pair34->pt();
      if (((pdgid_mother_1_ == 25) && (pdgid_mother_2_ == 25)) || ((pdgid_mother_1_ == 35) && (pdgid_mother_2_ == 35))) {
        mvis_phi_ = pair12->M();
        pt_phi_ = pair12->pt();
      } else if (((pdgid_mother_1_ == 25) && (pdgid_mother_3_ == 25)) || ((pdgid_mother_1_ == 35) && (pdgid_mother_3_ == 35))) {
        mvis_phi_ = pair13->M();
        pt_phi_ = pair13->pt();
      } else if (((pdgid_mother_1_ == 25) && (pdgid_mother_4_ == 25)) || ((pdgid_mother_1_ == 35) && (pdgid_mother_4_ == 35))) {
        mvis_phi_ = pair14->M();
        pt_phi_ = pair14->pt();
      } else if (((pdgid_mother_2_ == 25) && (pdgid_mother_3_ == 25)) || ((pdgid_mother_2_ == 35) && (pdgid_mother_3_ == 35))) {
        mvis_phi_ = pair23->M();
        pt_phi_ = pair23->pt();
      } else if (((pdgid_mother_2_ == 25) && (pdgid_mother_4_ == 25)) || ((pdgid_mother_2_ == 35) && (pdgid_mother_4_ == 35))) {
        mvis_phi_ = pair24->M();
        pt_phi_ = pair24->pt();
      } else if (((pdgid_mother_3_ == 25) && (pdgid_mother_4_ == 25)) || ((pdgid_mother_3_ == 35) && (pdgid_mother_4_ == 35))) {
        mvis_phi_ = pair34->M();
        pt_phi_ = pair34->pt();
      }

      if ((pdgid_mother_1_ == 36) && (pdgid_mother_2_ == 36)) {
        mvis_A_ = pair12->M();
        pt_A_ = pair12->pt();
      } else if ((pdgid_mother_1_ == 36) && (pdgid_mother_3_ == 36)) {
        mvis_A_ = pair13->M();
        pt_A_ = pair13->pt();
      } else if ((pdgid_mother_1_ == 36) && (pdgid_mother_4_ == 36)) {
        mvis_A_ = pair14->M();
        pt_A_ = pair14->pt();
      } else if ((pdgid_mother_2_ == 36) && (pdgid_mother_3_ == 36)) {
        mvis_A_ = pair23->M();
        pt_A_ = pair23->pt();
      } else if ((pdgid_mother_2_ == 36) && (pdgid_mother_4_ == 36)) {
        mvis_A_ = pair24->M();
        pt_A_ = pair24->pt();
      } else if ((pdgid_mother_3_ == 36) && (pdgid_mother_4_ == 36)) {
        mvis_A_ = pair34->M();
        pt_A_ = pair34->pt();
      }

        // select closest tau pairs
       p_min_dphi_1_ = 1;
       p_min_dR_1_ = 1;
       p_min_sum_dphi_1_ = 1;
       p_min_sum_dR_1_ = 1;
       p_min_dphi_2_ = 2;
       p_min_dR_2_ = 2;
       p_min_sum_dphi_2_ = 2;
       p_min_sum_dR_2_ = 2;

       double min_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton1")->vector(),fourtau->GetCandidate("lepton2")->vector()));
       double min_sum_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton1")->vector(),fourtau->GetCandidate("lepton2")->vector())) +std::fabs( ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton3")->vector(),fourtau->GetCandidate("lepton4")->vector()));
       double min_sum_dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton1")->vector(),fourtau->GetCandidate("lepton2")->vector())) +std::fabs( ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton3")->vector(),fourtau->GetCandidate("lepton4")->vector()));

       for(int i = 1; i < 5; i++){
         for(int j = 1; j < 5; j++){
           if (i < j) { 
             vector<int> v {1,2,3,4};
             v.erase(std::remove(v.begin(), v.end(), i), v.end());
             v.erase(std::remove(v.begin(), v.end(), j), v.end());
             if (fourtau->GetCandidate("lepton"+std::to_string(i))->charge() == -fourtau->GetCandidate("lepton"+std::to_string(j))->charge()) {
               //min_dphi
               if (std::fabs(ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector())) < min_dphi_) {
                 min_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()));
                 p_min_dphi_1_ = i;
                 p_min_dphi_2_ = j;
               }
               // min dR
               if (std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector())) < min_dR_) {
                 min_dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()));
                 p_min_dR_1_ = i;
                 p_min_dR_2_ = j;
              }
  
               // min sum dphi
               if ((std::fabs(ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()))+std::fabs(ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton"+std::to_string(v[0]))->vector(),fourtau->GetCandidate("lepton"+std::to_string(v[1]))->vector()))) < min_sum_dphi_) {
                 min_sum_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()))+std::fabs(ROOT::Math::VectorUtil::DeltaPhi(fourtau->GetCandidate("lepton"+std::to_string(v[0]))->vector(),fourtau->GetCandidate("lepton"+std::to_string(v[1]))->vector()));
                 p_min_sum_dphi_1_ = i;
                 p_min_sum_dphi_2_ = j;
               }
  
               // min sum dR
               if ((std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()))+std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(v[0]))->vector(),fourtau->GetCandidate("lepton"+std::to_string(v[1]))->vector()))) < min_sum_dR_) {
                 min_sum_dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(i))->vector(),fourtau->GetCandidate("lepton"+std::to_string(j))->vector()))+std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(v[0]))->vector(),fourtau->GetCandidate("lepton"+std::to_string(v[1]))->vector()));
                 p_min_sum_dR_1_ = i;
                 p_min_sum_dR_2_ = j;
               }
             }
           }
         }
       }

       CompositeCandidate *pair_min_dphi_1 = new CompositeCandidate();
       CompositeCandidate *pair_min_dphi_2 = new CompositeCandidate();
       CompositeCandidate *pair_min_dR_1 = new CompositeCandidate();
       CompositeCandidate *pair_min_dR_2 = new CompositeCandidate();
       CompositeCandidate *pair_min_sum_dphi_1 = new CompositeCandidate();
       CompositeCandidate *pair_min_sum_dphi_2 = new CompositeCandidate();
       CompositeCandidate *pair_min_sum_dR_1 = new CompositeCandidate();
       CompositeCandidate *pair_min_sum_dR_2 = new CompositeCandidate();

       pair_min_dphi_1->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(p_min_dphi_1_)));
       pair_min_dphi_1->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(p_min_dphi_2_)));
       vector<int> v1 {1,2,3,4};
       v1.erase(std::remove(v1.begin(), v1.end(), p_min_dphi_1_), v1.end());
       v1.erase(std::remove(v1.begin(), v1.end(), p_min_dphi_2_), v1.end());
       pair_min_dphi_2->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(v1[0])));
       pair_min_dphi_2->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(v1[1])));

       pair_min_dR_1->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(p_min_dR_1_)));
       pair_min_dR_1->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(p_min_dR_2_)));
       vector<int> v2 {1,2,3,4};
       v2.erase(std::remove(v2.begin(), v2.end(), p_min_dR_1_), v2.end());
       v2.erase(std::remove(v2.begin(), v2.end(), p_min_dR_2_), v2.end());
       pair_min_dR_2->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(v2[0])));
       pair_min_dR_2->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(v2[1])));

       pair_min_sum_dphi_1->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(p_min_sum_dphi_1_)));
       pair_min_sum_dphi_1->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(p_min_sum_dphi_2_)));
       vector<int> v3 {1,2,3,4};
       v3.erase(std::remove(v3.begin(), v3.end(), p_min_sum_dphi_1_), v3.end());
       v3.erase(std::remove(v3.begin(), v3.end(), p_min_sum_dphi_2_), v3.end());
       pair_min_sum_dphi_2->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(v3[0])));
       pair_min_sum_dphi_2->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(v3[1])));

       pair_min_sum_dR_1->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(p_min_sum_dR_1_)));
       pair_min_sum_dR_1->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(p_min_sum_dR_2_)));
       dR_min_sum_dR_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(p_min_dphi_1_))->vector(),fourtau->GetCandidate("lepton"+std::to_string(p_min_dphi_2_))->vector()));
       vector<int> v4 {1,2,3,4};
       v4.erase(std::remove(v4.begin(), v4.end(), p_min_sum_dR_1_), v4.end());
       v4.erase(std::remove(v4.begin(), v4.end(), p_min_sum_dR_2_), v4.end());
       pair_min_sum_dR_2->AddCandidate("lepton1",fourtau->GetCandidate("lepton"+std::to_string(v4[0])));
       pair_min_sum_dR_2->AddCandidate("lepton2",fourtau->GetCandidate("lepton"+std::to_string(v4[1])));
       dR_min_sum_dR_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(fourtau->GetCandidate("lepton"+std::to_string(v4[0]))->vector(),fourtau->GetCandidate("lepton"+std::to_string(v4[1]))->vector()));

       if (pair_min_dphi_1->M()>pair_min_dphi_2->M()) {
         mvis_min_dphi_1_ = pair_min_dphi_1->M();
         mvis_min_dphi_2_ = pair_min_dphi_2->M();
         pt_min_dphi_1_ = pair_min_dphi_1->pt();
         pt_min_dphi_2_ = pair_min_dphi_2->pt();
       } else {
         mvis_min_dphi_1_ = pair_min_dphi_2->M();
         mvis_min_dphi_2_ = pair_min_dphi_1->M();
         pt_min_dphi_1_ = pair_min_dphi_2->pt();
         pt_min_dphi_2_ = pair_min_dphi_1->pt();
       }

       if (pair_min_sum_dphi_1->M()>pair_min_sum_dphi_2->M()) {
         mvis_min_sum_dphi_1_ = pair_min_sum_dphi_1->M();
         mvis_min_sum_dphi_2_ = pair_min_sum_dphi_2->M();
         pt_min_sum_dphi_1_ = pair_min_sum_dphi_1->pt();
         pt_min_sum_dphi_2_ = pair_min_sum_dphi_2->pt();
       } else {
         mvis_min_sum_dphi_1_ = pair_min_sum_dphi_2->M();
         mvis_min_sum_dphi_2_ = pair_min_sum_dphi_1->M();
         pt_min_sum_dphi_1_ = pair_min_sum_dphi_2->pt();
         pt_min_sum_dphi_2_ = pair_min_sum_dphi_1->pt();
       }

       if (pair_min_dR_1->M()>pair_min_dR_2->M()) {
         mvis_min_dR_1_ = pair_min_dR_1->M();
         mvis_min_dR_2_ = pair_min_dR_2->M();
         pt_min_dR_1_ = pair_min_dR_1->pt();
         pt_min_dR_2_ = pair_min_dR_2->pt();
       } else {
         mvis_min_dR_1_ = pair_min_dR_2->M();
         mvis_min_dR_2_ = pair_min_dR_1->M();
         pt_min_dR_1_ = pair_min_dR_2->pt();
         pt_min_dR_2_ = pair_min_dR_1->pt();
       }

       if (pair_min_sum_dR_1->M()>pair_min_sum_dR_2->M()) {
         mvis_min_sum_dR_1_ = pair_min_sum_dR_1->M();
         mvis_min_sum_dR_2_ = pair_min_sum_dR_2->M();
         pt_min_sum_dR_1_ = pair_min_sum_dR_1->pt();
         pt_min_sum_dR_2_ = pair_min_sum_dR_2->pt();
       } else {
         mvis_min_sum_dR_1_ = pair_min_sum_dR_2->M();
         mvis_min_sum_dR_2_ = pair_min_sum_dR_1->M();
         pt_min_sum_dR_1_ = pair_min_sum_dR_2->pt();
         pt_min_sum_dR_2_ = pair_min_sum_dR_1->pt();
       }
   }

    if (channel_ == channel::ettt) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      Tau const* tau1 = dynamic_cast<Tau const*>(lep2);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep3);
      Tau const* tau3 = dynamic_cast<Tau const*>(lep4);
      d0_1_ = elec->dxy_vertex();
      dz_1_ = elec->dz_vertex();
      d0_2_ = tau1->lead_dxy_vertex();
      dz_2_ = tau1->lead_dz_vertex();
      d0_3_ = tau2->lead_dxy_vertex();
      dz_3_ = tau2->lead_dz_vertex();
      d0_4_ = tau3->lead_dxy_vertex();
      dz_4_ = tau3->lead_dz_vertex();
      iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
      tau_decay_mode_2_ = tau1->decay_mode();
      tau_decay_mode_3_ = tau2->decay_mode();
      tau_decay_mode_4_ = tau3->decay_mode();

      // Raw DNN scores
      deepTauVsJets_iso_2_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_2_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_2_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_3_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_4_      = tau3->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau3->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_4_       = tau3->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau3->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_4_        = tau3->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau3->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing workpoints
      deepTauVsJets_vvvloose_2_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_2_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_2_   = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_    = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_     = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_      = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_     = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_      = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_     = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_    = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_3_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;
      
      deepTauVsEle_vvvloose_3_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_   = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_    = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_     = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_      = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_     = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_      = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_     = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_    = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_4_ = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_4_  = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_4_   = tau3->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_4_    = tau3->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_4_   = tau3->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_4_    = tau3->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_4_   = tau3->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_4_  = tau3->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;
      
      deepTauVsEle_vvvloose_4_  = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_4_   = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_4_    = tau3->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_4_     = tau3->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau3->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_4_    = tau3->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau3->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_4_     = tau3->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau3->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_4_    = tau3->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_4_   = tau3->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_4_   = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_4_    = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_4_     = tau3->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_4_      = tau3->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_4_     = tau3->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_4_      = tau3->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_4_     = tau3->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_4_    = tau3->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

    }
    if (channel_ == channel::mttt) {
      Muon const* muon = dynamic_cast<Muon const*>(lep1);
      Tau const* tau1 = dynamic_cast<Tau const*>(lep2);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep3);
      Tau const* tau3 = dynamic_cast<Tau const*>(lep4);
      d0_1_ = muon->dxy_vertex();
      dz_1_ = muon->dz_vertex();
      d0_2_ = tau1->lead_dxy_vertex();
      dz_2_ = tau1->lead_dz_vertex();
      d0_3_ = tau2->lead_dxy_vertex();
      dz_3_ = tau2->lead_dz_vertex();
      d0_4_ = tau3->lead_dxy_vertex();
      dz_4_ = tau3->lead_dz_vertex();
      iso_1_ = PF04IsolationVal(muon, 0.5, 0);
      tau_decay_mode_2_ = tau1->decay_mode();
      tau_decay_mode_3_ = tau2->decay_mode();
      tau_decay_mode_4_ = tau3->decay_mode();


      // Raw DNN scores
      deepTauVsJets_iso_2_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_2_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_2_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_3_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_4_      = tau3->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau3->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_4_       = tau3->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau3->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_4_        = tau3->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau3->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing working points
      deepTauVsJets_vvvloose_2_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_2_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_2_   = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_    = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_     = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_      = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_     = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_      = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_     = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_    = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_3_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_3_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_   = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_    = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_     = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_      = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_     = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_      = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_     = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_    = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_4_ = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_4_  = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_4_   = tau3->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_4_    = tau3->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_4_   = tau3->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_4_    = tau3->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_4_   = tau3->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_4_  = tau3->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_4_  = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_4_   = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_4_    = tau3->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_4_     = tau3->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau3->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_4_    = tau3->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau3->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_4_     = tau3->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau3->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_4_    = tau3->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_4_   = tau3->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_4_   = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_4_    = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_4_     = tau3->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_4_      = tau3->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_4_     = tau3->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_4_      = tau3->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_4_     = tau3->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_4_    = tau3->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;


    }
    if (channel_ == channel::emtt) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      Muon const* muon = dynamic_cast<Muon const*>(lep2);
      Tau const* tau1 = dynamic_cast<Tau const*>(lep3);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep4);
      iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho
      iso_2_ = PF04IsolationVal(muon, 0.5, 0);
      d0_1_ = elec->dxy_vertex();
      dz_1_ = elec->dz_vertex();
      d0_2_ = muon->dxy_vertex();
      dz_2_ = muon->dz_vertex();
      d0_3_ = tau1->lead_dxy_vertex();
      dz_3_ = tau1->lead_dz_vertex();
      d0_4_ = tau2->lead_dxy_vertex();
      dz_4_ = tau2->lead_dz_vertex();
      tau_decay_mode_3_ = tau1->decay_mode();
      tau_decay_mode_4_ = tau2->decay_mode();


      // Raw DNN Scores
      deepTauVsJets_iso_3_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_4_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_4_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_4_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing working points
      deepTauVsJets_vvvloose_3_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_3_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_   = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_    = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_     = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_      = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_     = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_      = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_     = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_    = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_4_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_4_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_4_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_4_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_4_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_4_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_4_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_4_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_4_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_4_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_4_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_4_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_4_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_4_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_4_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_4_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_4_   = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_4_    = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_4_     = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_4_      = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_4_     = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_4_      = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_4_     = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_4_    = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

    }
    if (channel_ == channel::ttt) {
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      Tau const* tau3 = dynamic_cast<Tau const*>(lep3);
      d0_1_ = tau1->lead_dxy_vertex();
      dz_1_ = tau1->lead_dz_vertex();
      d0_2_ = tau2->lead_dxy_vertex();
      dz_2_ = tau2->lead_dz_vertex();
      d0_3_ = tau3->lead_dxy_vertex();
      dz_3_ = tau3->lead_dz_vertex();
      tau_decay_mode_1_ = tau1->decay_mode();
      tau_decay_mode_2_ = tau2->decay_mode();
      tau_decay_mode_3_ = tau3->decay_mode();
      // Raw DNN scores
      deepTauVsJets_iso_1_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_1_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_1_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_2_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_2_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_2_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_3_      = tau3->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau3->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau3->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau3->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau3->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau3->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing workpoints 
      deepTauVsJets_vvvloose_1_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_1_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_1_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_1_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_1_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_1_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_1_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_1_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_1_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_1_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_1_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_1_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_1_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_1_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_1_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_1_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_1_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_1_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_1_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_1_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_1_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_1_     = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_1_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_1_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_2_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_2_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_2_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_     = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_3_ = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau3->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau3->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau3->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau3->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau3->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau3->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_3_  = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau3->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau3->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau3->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau3->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau3->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau3->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau3->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau3->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau3->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_  = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_   = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_    = tau3->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_     = tau3->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau3->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_    = tau3->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_     = tau3->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau3->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_    = tau3->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_   = tau3->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;
     
    }


    if (channel_ == channel::tttt) {
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      Tau const* tau3 = dynamic_cast<Tau const*>(lep3);
      Tau const* tau4 = dynamic_cast<Tau const*>(lep4);
      d0_1_ = tau1->lead_dxy_vertex();
      dz_1_ = tau1->lead_dz_vertex();
      d0_2_ = tau2->lead_dxy_vertex();
      dz_2_ = tau2->lead_dz_vertex();
      d0_3_ = tau3->lead_dxy_vertex();
      dz_3_ = tau3->lead_dz_vertex();
      d0_4_ = tau4->lead_dxy_vertex();
      dz_4_ = tau4->lead_dz_vertex();
      tau_decay_mode_1_ = tau1->decay_mode();
      tau_decay_mode_2_ = tau2->decay_mode();
      tau_decay_mode_3_ = tau3->decay_mode();
      tau_decay_mode_4_ = tau4->decay_mode();

      // Raw DNN scores
      deepTauVsJets_iso_1_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_1_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_1_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_2_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_2_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_2_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_3_      = tau3->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau3->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau3->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau3->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau3->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau3->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_4_      = tau4->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau4->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_4_       = tau4->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau4->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_4_        = tau4->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau4->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing workpoints
      deepTauVsJets_vvvloose_1_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_1_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_1_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_1_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_1_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_1_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_1_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_1_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_1_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_1_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_1_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_1_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_1_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_1_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_1_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_1_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_1_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_1_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_1_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_1_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_1_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_1_     = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_1_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_1_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_2_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_2_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_2_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_     = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_3_ = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau3->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau3->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau3->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau3->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau3->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau3->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau3->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau3->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_3_  = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau3->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau3->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau3->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau3->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau3->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau3->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau3->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau3->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau3->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau3->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_  = tau3->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau3->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_   = tau3->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_    = tau3->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_     = tau3->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau3->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_    = tau3->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_     = tau3->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau3->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_    = tau3->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau3->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_   = tau3->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau3->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_4_ = tau4->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau4->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_4_  = tau4->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau4->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_4_   = tau4->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau4->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_4_    = tau4->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau4->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_4_   = tau4->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau4->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_4_    = tau4->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau4->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_4_   = tau4->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau4->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_4_  = tau4->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau4->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_4_  = tau4->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau4->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_4_   = tau4->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau4->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_4_    = tau4->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau4->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_4_     = tau4->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau4->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_4_    = tau4->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau4->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_4_     = tau4->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau4->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_4_    = tau4->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau4->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_4_   = tau4->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau4->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_4_  = tau4->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau4->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_4_   = tau4->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau4->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_4_    = tau4->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau4->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_4_     = tau4->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau4->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_4_    = tau4->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau4->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_4_     = tau4->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau4->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_4_    = tau4->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau4->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_4_   = tau4->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau4->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

    }
    if (channel_ == channel::eett) {
      Electron const* elec1 = dynamic_cast<Electron const*>(lep1);
      Electron const* elec2 = dynamic_cast<Electron const*>(lep2);
      Tau const* tau1 = dynamic_cast<Tau const*>(lep3);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep4);
      d0_1_ = elec1->dxy_vertex();
      dz_1_ = elec1->dz_vertex();
      d0_2_ = elec2->dxy_vertex();
      dz_2_ = elec2->dz_vertex();
      d0_3_ = tau1->lead_dxy_vertex();
      dz_3_ = tau1->lead_dz_vertex();
      d0_4_ = tau2->lead_dxy_vertex();
      dz_4_ = tau2->lead_dz_vertex();
      iso_1_ = PF03EAIsolationVal(elec1, eventInfo->jet_rho());
      iso_2_ = PF03EAIsolationVal(elec2, eventInfo->jet_rho());
      tau_decay_mode_3_ = tau1->decay_mode();
      tau_decay_mode_4_ = tau2->decay_mode();


      // Raw DNN Scores
      deepTauVsJets_iso_3_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_4_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_4_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_4_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing working points
      deepTauVsJets_vvvloose_3_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_3_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_     = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_4_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_4_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_4_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_4_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_4_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_4_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_4_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_4_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_4_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_4_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_4_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_4_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_4_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_4_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_4_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_4_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_4_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_4_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_4_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_4_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_4_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_4_     = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_4_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_4_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

    }
    if (channel_ == channel::mmtt) {
      Muon const* muon1 = dynamic_cast<Muon const*>(lep1);
      Muon const* muon2 = dynamic_cast<Muon const*>(lep2);
      Tau const* tau1 = dynamic_cast<Tau const*>(lep3);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep4);
      d0_1_ = muon1->dxy_vertex();
      dz_1_ = muon1->dz_vertex();
      d0_2_ = muon2->dxy_vertex();
      dz_2_ = muon2->dz_vertex();
      d0_3_ = tau1->lead_dxy_vertex();
      dz_3_ = tau1->lead_dz_vertex();
      d0_4_ = tau2->lead_dxy_vertex();
      dz_4_ = tau2->lead_dz_vertex();
      iso_1_ = PF04IsolationVal(muon1, 0.5, 0);
      iso_2_ = PF04IsolationVal(muon2, 0.5, 0);
      tau_decay_mode_3_ = tau1->decay_mode();
      tau_decay_mode_4_ = tau2->decay_mode();


      deepTauVsJets_iso_3_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_3_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_3_        = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_iso_4_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_4_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_4_        = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      deepTauVsJets_vvvloose_3_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_3_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_3_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_3_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_3_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_3_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_3_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_3_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_3_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_3_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_3_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_3_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_3_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_3_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_3_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_3_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_3_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_3_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_3_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_3_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_3_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_3_     = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_3_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_3_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsJets_vvvloose_4_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_4_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_4_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_4_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_4_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_4_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_4_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_4_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_4_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_4_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_4_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_4_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_4_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_4_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_4_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_4_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_4_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_4_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_4_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_4_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_4_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_4_     = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_4_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_4_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

    }

    if (channel_ == channel::mmmm) {
      Muon const* muon1 = dynamic_cast<Muon const*>(lep1);
      Muon const* muon2 = dynamic_cast<Muon const*>(lep2);
      Muon const* muon3 = dynamic_cast<Muon const*>(lep3);
      Muon const* muon4 = dynamic_cast<Muon const*>(lep4);
      d0_1_ = muon1->dxy_vertex();
      dz_1_ = muon1->dz_vertex();
      d0_2_ = muon2->dxy_vertex();
      dz_2_ = muon2->dz_vertex();
      d0_3_ = muon3->dxy_vertex();
      dz_3_ = muon3->dz_vertex();
      d0_4_ = muon4->dxy_vertex();
      dz_4_ = muon4->dz_vertex();
      iso_1_ = PF04IsolationVal(muon1, 0.5, 0);
      iso_2_ = PF04IsolationVal(muon2, 0.5, 0);
      iso_3_ = PF04IsolationVal(muon3, 0.5, 0);
      iso_4_ = PF04IsolationVal(muon4, 0.5, 0);
    }



    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    std::vector<PFJet*> prebjets = lowpt_jets;
    double eta_cut = 2.4;
    if(era_ == era::data_2017 || era_ == era::data_2017UL || era_ == era::data_2018 || era_ == era::data_2018UL) eta_cut = 2.5;
    ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, eta_cut));
    std::vector<PFJet*> deepbjets = prebjets;

    double deepjet_wp;
    std::string btag_label="s";
    std::string btag_label_extra ="";

    if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      deepjet_wp = 0.3033;
    }
    if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      deepjet_wp = 0.2770;
    }
    if (era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP) {
      btag_label_extra = "pfDeepCSVJetTags:probbb";
      deepjet_wp = 0.3093;
    }

    std::string deepjet_label_1 = "pfDeepFlavourJetTags:probb";
    std::string deepjet_label_2 = "pfDeepFlavourJetTags:probbb";
    std::string deepjet_label_3 = "pfDeepFlavourJetTags:problepb";

    auto filterDeepJetBTag = [deepjet_label_1, deepjet_label_2, deepjet_label_3, deepjet_wp] (PFJet* s1) -> bool {
      return s1->GetBDiscriminator(deepjet_label_1) + s1->GetBDiscriminator(deepjet_label_2) + s1->GetBDiscriminator(deepjet_label_3) > deepjet_wp;
    };

    // promte-demote method only
    if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result");
      ic::erase_if(deepbjets, !boost::bind(IsReBTagged, _1, retag_result));
    } else{
      ic::erase_if_not(deepbjets, filterDeepJetBTag);
    }

    n_deepbjets_ = deepbjets.size();
    n_jets_ = jets.size();

    isTau_ = 0;
    isSingleElectron_ = 0;
    isSingleMuon_ = 0;
    isMuonEG_ = 0;
    if (is_data_) {
      if (filename_.find("Tau") != filename_.npos) {
        isTau_ = true;
      }
      if (filename_.find("SingleElectron") != filename_.npos || filename_.find("EGamma") != filename_.npos) {
        isSingleElectron_ = true;
      }
      if (filename_.find("SingleMuon") != filename_.npos) {
        isSingleMuon_ = true;
      }
      if (filename_.find("MuonEG") != filename_.npos) {
        isMuonEG_ = true;
      }
    }

    wt_ggZZ_ = event->Exists("wt_ggZZ") ? event->Get<double>("wt_ggZZ") : 1.0;
    wt_qqZZ_ = event->Exists("wt_qqZZ") ? event->Get<float>("wt_qqZZ") : 1.0;
    wt_ggZZ_PDFScaleUp_ = event->Exists("wt_ggZZ_PDFScaleUp") ? event->Get<double>("wt_ggZZ_PDFScaleUp") : 1.0;
    wt_ggZZ_PDFScaleDn_ = event->Exists("wt_ggZZ_PDFScaleDn") ? event->Get<double>("wt_ggZZ_PDFScaleDn") : 1.0;
    wt_ggZZ_QCDScaleUp_ = event->Exists("wt_ggZZ_QCDScaleUp") ? event->Get<double>("wt_ggZZ_QCDScaleUp") : 1.0;
    wt_ggZZ_QCDScaleDn_ = event->Exists("wt_ggZZ_QCDScaleDn") ? event->Get<double>("wt_ggZZ_QCDScaleDn") : 1.0;
    wt_ggZZ_AsUp_ = event->Exists("wt_ggZZ_AsUp") ? event->Get<double>("wt_ggZZ_AsUp") : 1.0;
    wt_ggZZ_AsDn_ = event->Exists("wt_ggZZ_AsDn") ? event->Get<double>("wt_ggZZ_AsDn") : 1.0;
    wt_ggZZ_PDFReplicaUp_ = event->Exists("wt_ggZZ_PDFReplicaUp") ? event->Get<double>("wt_ggZZ_PDFReplicaUp") : 1.0;
    wt_ggZZ_PDFReplicaDn_ = event->Exists("wt_ggZZ_PDFReplicaUp") ? event->Get<double>("wt_ggZZ_PDFReplicaUp") : 1.0;

   if(eventInfo->weight_defined("1001")) scale_1_ = eventInfo->weight("1001"); else scale_1_=1.0;
   if(eventInfo->weight_defined("1002")) scale_2_ = eventInfo->weight("1002"); else scale_2_=1.0;
   if(eventInfo->weight_defined("1003")) scale_3_ = eventInfo->weight("1003"); else scale_3_=1.0;
   if(eventInfo->weight_defined("1004")) scale_4_ = eventInfo->weight("1004"); else scale_4_=1.0;
   if(eventInfo->weight_defined("1005")) scale_5_ = eventInfo->weight("1005"); else scale_5_=1.0;
   if(eventInfo->weight_defined("1006")) scale_6_ = eventInfo->weight("1006"); else scale_6_=1.0;
   if(eventInfo->weight_defined("1007")) scale_7_ = eventInfo->weight("1007"); else scale_7_=1.0;
   if(eventInfo->weight_defined("1008")) scale_8_ = eventInfo->weight("1008"); else scale_8_=1.0;
   if(eventInfo->weight_defined("1009")) scale_9_ = eventInfo->weight("1009"); else scale_9_=1.0;

    if (write_tree_ && fs_) outtree_->Fill();
    return 0;
  }

  int HTTCategories::PostAnalysis() {
    return 0;
  }

  void HTTCategories::PrintInfo() {
    ;
  }
}
