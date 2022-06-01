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

      // gen matches
      outtree_->Branch("gen_match_1",         &gen_match_1_);
      outtree_->Branch("gen_match_2",         &gen_match_2_);
      outtree_->Branch("gen_match_3",         &gen_match_3_);
      outtree_->Branch("gen_match_4",         &gen_match_4_);

      // idiso weights
      outtree_->Branch("idisoweight_1", &idisoweight_1_);
      outtree_->Branch("idisoweight_2", &idisoweight_2_);
      outtree_->Branch("idisoweight_3", &idisoweight_3_);
      outtree_->Branch("idisoweight_4", &idisoweight_4_);

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
      outtree_->Branch("trigweight_tau_1", &trigweight_tau_1_);
      outtree_->Branch("trigweight_tau_2", &trigweight_tau_2_);

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

      // transverse momentums
      outtree_->Branch("pt_1",              &pt_1_.var_double);
      outtree_->Branch("pt_2",              &pt_2_.var_double);
      outtree_->Branch("pt_3",              &pt_3_.var_double);
      outtree_->Branch("pt_4",              &pt_4_.var_double);

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

      // charges
      outtree_->Branch("q_1",              &q_1_.var_double);
      outtree_->Branch("q_2",              &q_2_.var_double);
      outtree_->Branch("q_3",              &q_3_.var_double);
      outtree_->Branch("q_4",              &q_4_.var_double);

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


    }

    return 0;
  }

  int HTTCategories::Execute(TreeEvent *event) {
    
    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<CompositeCandidate *> const& fourtau_vec = event->GetPtrVec<CompositeCandidate>(fourtau_label_);
    CompositeCandidate const* fourtau = fourtau_vec.at(0);
    Candidate const* lep1 = fourtau->GetCandidate("lepton1");
    Candidate const* lep2 = fourtau->GetCandidate("lepton2");
    Candidate const* lep3 = fourtau->GetCandidate("lepton3");
    Candidate const* lep4 = fourtau->GetCandidate("lepton4");

 
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

    idisoweight_1_ = event->Exists("idisoweight_1") ? event->Get<double>("idisoweight_1") : 1.0;
    idisoweight_2_ = event->Exists("idisoweight_2") ? event->Get<double>("idisoweight_2") : 1.0;
    idisoweight_3_ = event->Exists("idisoweight_3") ? event->Get<double>("idisoweight_3") : 1.0;
    idisoweight_4_ = event->Exists("idisoweight_4") ? event->Get<double>("idisoweight_4") : 1.0;

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

    trigweight_tau_1_ = event->Exists("trigweight_tau_1") ? event->Get<double>("trigweight_tau_1") : 1.0;
    trigweight_tau_2_ = event->Exists("trigweight_tau_2") ? event->Get<double>("trigweight_tau_2") : 1.0;

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    pt_3_ = lep3->pt();
    pt_4_ = lep4->pt();
    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();
    eta_3_ = lep1->eta();
    eta_4_ = lep2->eta();
    phi_1_ = lep1->phi();
    phi_2_ = lep2->phi();
    phi_3_ = lep1->phi();
    phi_4_ = lep2->phi();
    dphi_12_ = ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector());
    dphi_13_ = ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector());
    dphi_14_ = ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector());
    dphi_23_ = ROOT::Math::VectorUtil::DeltaPhi(lep2->vector(),lep3->vector()); 
    dphi_24_ = ROOT::Math::VectorUtil::DeltaPhi(lep2->vector(),lep4->vector());
    dphi_34_ = ROOT::Math::VectorUtil::DeltaPhi(lep2->vector(),lep4->vector());
    dR_12_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep2->vector()));
    dR_13_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep3->vector()));
    dR_14_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep4->vector()));
    dR_23_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep2->vector(),lep3->vector()));
    dR_24_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep2->vector(),lep4->vector()));
    dR_34_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep3->vector(),lep4->vector()));
    E_1_ = lep1->energy();
    E_2_ = lep2->energy();
    E_3_ = lep3->energy();
    E_4_ = lep4->energy();
    m_1_ = lep1->M();
    m_2_ = lep2->M();
    m_3_ = lep3->M();
    m_4_ = lep4->M();
    q_1_ = lep1->charge();
    q_2_ = lep2->charge();
    q_3_ = lep3->charge();
    q_4_ = lep4->charge();

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
