#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/HHKinFit/include/HHKinFitMaster.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/HHKinFit/include/HHDiJetKinFitMaster.h"

#include "TMVA/Reader.h"
#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include "TLorentzVector.h"

namespace ic {

  HTTCategories::HTTCategories(std::string const& name) : ModuleBase(name), 
      channel_(channel::et), 
      era_(era::data_2012_rereco),
      strategy_(strategy::paper2013) {
      ditau_label_ = "emtauCandidates";
      jets_label_ = "pfJetsPFlow";
      met_label_ = "pfMVAMetNoLeptons";
      mass_shift_ = 1.0;
      fs_ = NULL;
      write_tree_ = true;
      bjet_regression_ = false;
      make_sync_ntuple_ = false;
      sync_output_name_ = "SYNC.root";
      iso_study_=false;
      tau_id_study_=false;
      is_embedded_=false;
      is_data_=false;
      kinfit_mode_ = 0; //0 = don't run, 1 = run simple 125,125 default fit, 2 = run extra masses default fit, 3 = run m_bb only fit
      systematic_shift_ = false;
      add_Hhh_variables_ = false; //set to include custom variables for the H->hh analysis
  }

  HTTCategories::~HTTCategories() {
    ;
  }

  int HTTCategories::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTCategories" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
      std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
      std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
      std::cout << boost::format(param_fmt()) % "era"             % Era2String(era_);
      std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
      std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
      std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;
      std::cout << boost::format(param_fmt()) % "mass_shift"      % mass_shift_;
      std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;
      std::cout << boost::format(param_fmt()) % "kinfit_mode"     % kinfit_mode_;
      std::cout << boost::format(param_fmt()) % "make_sync_ntuple" % make_sync_ntuple_;
      std::cout << boost::format(param_fmt()) % "bjet_regression" % bjet_regression_;

    if (fs_ && write_tree_) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      outtree_->Branch("event",             &event_);
      outtree_->Branch("wt",                &wt_.var_double);
      outtree_->Branch("os",                &os_);
      outtree_->Branch("m_sv",              &m_sv_.var_double);
      outtree_->Branch("mt_sv",              &mt_sv_.var_double);
      outtree_->Branch("m_vis",             &m_vis_.var_double);
      outtree_->Branch("pt_h",              &pt_h_.var_double);
      outtree_->Branch("pt_tt",             &pt_tt_.var_double);
      outtree_->Branch("mt_tot",            &mt_tot_.var_double);
      outtree_->Branch("mt_lep",            &mt_lep_.var_double);
      outtree_->Branch("mt_2",              &mt_2_.var_double);
      outtree_->Branch("mt_1",              &mt_1_.var_double);
      outtree_->Branch("pfmt_1",            &pfmt_1_.var_double);
      outtree_->Branch("puppimt_1",         &puppimt_1_.var_double);
      outtree_->Branch("pzeta",             &pzeta_.var_double);
      outtree_->Branch("pfpzeta",           &pfpzeta_.var_double);
      outtree_->Branch("puppipzeta",        &puppipzeta_.var_double);
      outtree_->Branch("iso_1",             &iso_1_.var_double);
      outtree_->Branch("iso_2",             &iso_2_.var_double);
      outtree_->Branch("iso_pho_sum_pt_2",  &lPhotonPtSum_2.var_double);
      outtree_->Branch("iso_pho_sum_pt_1",  &lPhotonPtSum_1.var_double);
      outtree_->Branch("antiele_1",         &antiele_1_);
      outtree_->Branch("antimu_1",          &antimu_1_);
      outtree_->Branch("antiele_2",         &antiele_2_);
      outtree_->Branch("antimu_2",          &antimu_2_);
      outtree_->Branch("leptonveto",        &lepton_veto_);
      outtree_->Branch("dilepton_veto",     &dilepton_veto_);
      outtree_->Branch("extraelec_veto",    &extraelec_veto_);
      outtree_->Branch("extramuon_veto",    &extramuon_veto_);
      outtree_->Branch("minimal_extraelec_veto",    &minimal_extraelec_veto_);
      outtree_->Branch("minimal_extramuon_veto",    &minimal_extramuon_veto_);
      outtree_->Branch("met",               &mvamet_.var_double);
      outtree_->Branch("pfmet",             &pfmet_.var_double);
      outtree_->Branch("n_jets",            &n_jets_);
      outtree_->Branch("n_bjets",           &n_bjets_);
      outtree_->Branch("mjj",               &mjj_.var_double);
      outtree_->Branch("n_jetsingap",       &n_jetsingap_);
      outtree_->Branch("jdeta",             &jdeta_.var_double);
      outtree_->Branch("n_lowpt_jets",      &n_lowpt_jets_);
      outtree_->Branch("n_jetsingap_lowpt", &n_jetsingap_lowpt_);
      outtree_->Branch("pt_2",              &pt_2_.var_double);
      outtree_->Branch("mjj_lowpt",         &mjj_lowpt_);
      outtree_->Branch("gen_match_1", &gen_match_1_);
      outtree_->Branch("gen_match_2", &gen_match_2_);
      outtree_->Branch("db_loose_1",&lbyLooseCombinedIsolation_1);
      outtree_->Branch("db_loose_2",&lbyLooseCombinedIsolation_2);
      outtree_->Branch("db_medium_1",&lbyMediumCombinedIsolation_1);
      outtree_->Branch("db_medium_2",&lbyMediumCombinedIsolation_2);
      outtree_->Branch("db_tight_1",&lbyTightCombinedIsolation_1);
      outtree_->Branch("db_tight_2",&lbyTightCombinedIsolation_2);

/*      outtree_->Branch("leading_lepton_match_pt", &leading_lepton_match_pt_);
      outtree_->Branch("subleading_lepton_match_pt",&subleading_lepton_match_pt_);
      outtree_->Branch("leading_lepton_match_DR", &leading_lepton_match_DR_);
      outtree_->Branch("subleading_lepton_match_DR",&subleading_lepton_match_DR_);*/

      outtree_->Branch("jdeta_lowpt",       &jdeta_lowpt_);
      if (channel_ == channel::em) {
        outtree_->Branch("em_gf_mva",         &em_gf_mva_);
        // outtree_->Branch("em_vbf_mva",        &em_vbf_mva_);
      }
      if(add_Hhh_variables_) { 
        outtree_->Branch("jet_csv_mjj",               &jet_csv_mjj_);
        outtree_->Branch("m_H_hh",     &m_H_hh_);
        outtree_->Branch("convergence_hh", &convergence_hh_);
        outtree_->Branch("mjj_tt",            &mjj_tt_);
        outtree_->Branch("n_jets_csv",        &n_jets_csv_);
        outtree_->Branch("n_bjets_csv",       &n_bjets_csv_);
        outtree_->Branch("jet_csvbcsv_1",     &jet_csvbcsv_1_);
        outtree_->Branch("jet_csvbcsv_2",     &jet_csvbcsv_2_);
      }
      if(iso_study_){
        //Add different isolation variables for if studying isolation
        outtree_->Branch("iso_1_db03", &iso_1_db03_);
        outtree_->Branch("iso_1_db03allch", &iso_1_db03allch_);
        outtree_->Branch("iso_1_db04allch", &iso_1_db04allch_);
        outtree_->Branch("iso_1_db04", &iso_1_db04_);
        outtree_->Branch("iso_1_ea03", &iso_1_ea03_);
        outtree_->Branch("iso_1_trk03", &iso_1_trk03_);
        outtree_->Branch("iso_2_db03", &iso_2_db03_);
        outtree_->Branch("iso_2_db03allch", &iso_2_db03allch_);
        outtree_->Branch("iso_2_db04allch", &iso_2_db04allch_);
        outtree_->Branch("iso_2_db04", &iso_2_db04_);
        outtree_->Branch("iso_2_ea03", &iso_2_ea03_);
        outtree_->Branch("iso_2_trk03", &iso_2_trk03_);
      }
 
      if(tau_id_study_){
       outtree_->Branch("mvanew_vloose_1",&lbyVLooseIsolationMVA3newDMwLT_1);
       outtree_->Branch("mvanew_vloose_2",&lbyVLooseIsolationMVA3newDMwLT_2);
       outtree_->Branch("mvanew_loose_1",&lbyLooseIsolationMVA3newDMwLT_1);
       outtree_->Branch("mvanew_loose_2",&lbyLooseIsolationMVA3newDMwLT_2);
       outtree_->Branch("mvanew_medium_1",&lbyMediumIsolationMVA3newDMwLT_1);
       outtree_->Branch("mvanew_medium_2",&lbyMediumIsolationMVA3newDMwLT_2);
       outtree_->Branch("mvanew_tight_1",&lbyTightIsolationMVA3newDMwLT_1);
       outtree_->Branch("mvanew_tight_2",&lbyTightIsolationMVA3newDMwLT_2);
       outtree_->Branch("mvanew_vtight_1",&lbyVTightIsolationMVA3newDMwLT_1);
       outtree_->Branch("mvanew_vtight_2",&lbyVTightIsolationMVA3newDMwLT_2);
       outtree_->Branch("puw_loose_1",&lbyLoosePileupWeightedIsolation_1);
       outtree_->Branch("puw_loose_2",&lbyLoosePileupWeightedIsolation_2);
       outtree_->Branch("puw_medium_1",&lbyMediumPileupWeightedIsolation_1);
       outtree_->Branch("puw_medium_2",&lbyMediumPileupWeightedIsolation_2);
       outtree_->Branch("puw_tight_1",&lbyTightPileupWeightedIsolation_1);
       outtree_->Branch("puw_tight_2",&lbyTightPileupWeightedIsolation_2);
       outtree_->Branch("antie_vloose_1",&lagainstElectronVLooseMVA5_1);
       outtree_->Branch("antie_loose_1",&lagainstElectronLooseMVA5_1);
       outtree_->Branch("antie_medium_1",&lagainstElectronMediumMVA5_1); 
       outtree_->Branch("antie_tight_1",&lagainstElectronTightMVA5_1);
       outtree_->Branch("antie_vtight_1",&lagainstElectronVTightMVA5_1);
       outtree_->Branch("antimu_loose_1",&lagainstMuonLoose3_1);
       outtree_->Branch("antimu_tight_1",&lagainstMuonTight3_1);
       outtree_->Branch("antie_vloose_2",&lagainstElectronVLooseMVA5_2);
       outtree_->Branch("antie_loose_2",&lagainstElectronLooseMVA5_2);
       outtree_->Branch("antie_medium_2",&lagainstElectronMediumMVA5_2); 
       outtree_->Branch("antie_tight_2",&lagainstElectronTightMVA5_2);
       outtree_->Branch("antie_vtight_2",&lagainstElectronVTightMVA5_2);
       outtree_->Branch("antimu_loose_2",&lagainstMuonLoose3_2);
       outtree_->Branch("antimu_tight_2",&lagainstMuonTight3_2);
       outtree_->Branch("isoPhoSumPt_2",&lPhotonPtSum_2.var_float);
       outtree_->Branch("isoPhoSumPt_1",&lPhotonPtSum_1.var_float);
       outtree_->Branch("iso_mva_new_1",&lbyIsolationMVA3newDMwLTraw_1);
       outtree_->Branch("iso_mva_old_1",&lbyIsolationMVA3oldDMwLTraw_1);
       outtree_->Branch("iso_mva_new_2",&lbyIsolationMVA3newDMwLTraw_2);
       outtree_->Branch("iso_mva_old_2",&lbyIsolationMVA3oldDMwLTraw_2);
       outtree_->Branch("olddm_1",&ldecayModeFindingOldDMs_1);
       outtree_->Branch("olddm_2",&ldecayModeFindingOldDMs_2);
      }

      if(channel_ == channel::tpzmm || channel_ == channel::tpzee){
        //Extra variables needed for tag and probe
        outtree_->Branch("id_1", &mva_1_.var_double);
        outtree_->Branch("id_2", &mva_2_.var_double);
        outtree_->Branch("q_1", &q_1_);
        outtree_->Branch("q_2", &q_2_);
        outtree_->Branch("dxy_1", &d0_1_.var_double);
        outtree_->Branch("dxy_2", &d0_2_.var_double);
        outtree_->Branch("dz_1", &dz_1_.var_double);
        outtree_->Branch("dz_2", &dz_2_.var_double);
        outtree_->Branch("trigger_match_1", &trigger_match_1_);
        outtree_->Branch("trigger_match_2", &trigger_match_2_);
      }
      //Variables needed for control plots need only be generated for central systematics
      if(!systematic_shift_) {
        //outtree_->Branch("wt_ggh_pt_up",      &wt_ggh_pt_up_);
        //outtree_->Branch("wt_ggh_pt_down",    &wt_ggh_pt_down_);
        //outtree_->Branch("wt_tau_fake_up",    &wt_tau_fake_up_);
        //outtree_->Branch("wt_tau_fake_down",  &wt_tau_fake_down_);
        //outtree_->Branch("wt_tquark_up",      &wt_tquark_up_);
        //outtree_->Branch("wt_tquark_down",    &wt_tquark_down_);
        //outtree_->Branch("wt_tau_id_up",      &wt_tau_id_up_);
        //outtree_->Branch("wt_tau_id_down",    &wt_tau_id_down_);
        outtree_->Branch("n_vtx",             &n_vtx_);
        outtree_->Branch("good_vtx",          &good_vtx_);
        outtree_->Branch("pt_1",              &pt_1_.var_double);
        outtree_->Branch("eta_1",             &eta_1_.var_double);
        outtree_->Branch("eta_2",             &eta_2_.var_double);
        outtree_->Branch("phi_1",             &phi_1_.var_double);
        outtree_->Branch("phi_2",             &phi_2_.var_double);
        outtree_->Branch("E_1",               &E_1_);
        outtree_->Branch("E_2",               &E_2_);
        outtree_->Branch("z_2",               &z_2_);
        outtree_->Branch("m_2",               &m_2_.var_double);
        outtree_->Branch("met_phi",           &mvamet_phi_.var_double);
        outtree_->Branch("tau_decay_mode_2",    &tau_decay_mode_2_);
        outtree_->Branch("tau_decay_mode_1",    &tau_decay_mode_1_);
        outtree_->Branch("n_prebjets",        &n_prebjets_);
        outtree_->Branch("n_loose_bjets",     &n_loose_bjets_);
        outtree_->Branch("jpt_1",             &jpt_1_.var_double);
        outtree_->Branch("j1_dm",             &j1_dm_);
        outtree_->Branch("jpt_2",             &jpt_2_.var_double);
        outtree_->Branch("jeta_1",            &jeta_1_.var_double);
        outtree_->Branch("jeta_2",            &jeta_2_.var_double);
        outtree_->Branch("bpt_1",             &bpt_1_.var_double);
        outtree_->Branch("beta_1",            &beta_1_.var_double);
        outtree_->Branch("bcsv_1",            &bcsv_1_.var_double);
/*        outtree_->Branch("trigger_object_pt_1",&trigger_object_pt_1.var_double);
        outtree_->Branch("trigger_object_eta_1",&trigger_object_eta_1.var_double);
        outtree_->Branch("trigger_object_pt_2",&trigger_object_pt_2.var_double);
        outtree_->Branch("trigger_object_eta_2",&trigger_object_eta_2.var_double);
*/
        if (channel_ == channel::em) {
          outtree_->Branch("pzetavis",          &pzetavis_.var_double);
          outtree_->Branch("pzetamiss",         &pzetamiss_.var_double);
          outtree_->Branch("mt_ll",             &mt_ll_);
          outtree_->Branch("emu_dphi",          &emu_dphi_);
          outtree_->Branch("emu_csv",           &emu_csv_);
          outtree_->Branch("emu_dxy_1",         &emu_dxy_1_);
          outtree_->Branch("emu_dxy_2",         &emu_dxy_2_);
        }
        if(add_Hhh_variables_) {
          outtree_->Branch("jet_csvpt_1",       &jet_csvpt_1_);
          outtree_->Branch("jet_csveta_1",      &jet_csveta_1_);
          outtree_->Branch("jet_csvpt_2",       &jet_csvpt_2_);
          outtree_->Branch("jet_csveta_2",      &jet_csveta_2_);
          outtree_->Branch("mjj_h",             &mjj_h_);
          outtree_->Branch("mbb_h",             &mbb_h_);
          if(kinfit_mode_ > 1) {
            outtree_->Branch("m_H_best",               &m_H_best_);
            outtree_->Branch("m_H_chi2_best",               &m_H_chi2_best_);
            outtree_->Branch("pull_balance_H_best", &pull_balance_H_best_);
            outtree_->Branch("convergence_H_best", &convergence_H_best_); 
            outtree_->Branch("m_H_hZ",          &m_H_hZ_);
            outtree_->Branch("m_H_hZ_chi2",     &m_H_hZ_chi2_);
            outtree_->Branch("pull_balance_hZ", &pull_balance_hZ_);
            outtree_->Branch("convergence_hZ", &convergence_hZ_);
            outtree_->Branch("m_H_Zh",          &m_H_Zh_);
            outtree_->Branch("m_H_Zh_chi2",     &m_H_Zh_chi2_);
            outtree_->Branch("pull_balance_Zh",  &pull_balance_Zh_);
            outtree_->Branch("convergence_Zh",  &convergence_Zh_);
            outtree_->Branch("m_H_hh_all",     &m_H_hh_all_);
            outtree_->Branch("m_H_hh_chi2",     &m_H_hh_chi2_);
            outtree_->Branch("pull_balance_hh", &pull_balance_hh_);
            outtree_->Branch("m_bb",     &m_bb_);
            outtree_->Branch("m_bb_chi2",     &m_bb_chi2_);
            outtree_->Branch("pull_balance_bb", &pull_balance_bb_);
            outtree_->Branch("convergence_bb", &convergence_bb_);
          }
        }
      }
    }
    if(make_sync_ntuple_) {
      //Due to the possibility of other groups requesting different branch names/branch contents
      //we have to make an alternative (albeit very similar) TTree for the sync ntuple. 
      lOFile = new TFile(sync_output_name_.c_str(), "RECREATE");
      lOFile->cd();
      // Tree should be named "TauCheck" to aid scripts which
      // make comparisons between sync trees
      synctree_ = new TTree("TauCheck", "TauCheck");

      // The sync tree is filled for all events passing the di-lepton
      // selections in each channel. This includes vertex selection,
      // trigger, ID, isolation, di-lepton and extra lepton vetoes.
      // Topological (e.g. mT) and opposite-charge requirements are
      // not applied.

      // Note: not all of the following variables were in the original
      // list of sync tree variables, and not all are necessary/used in
      // the legacy htt anaylsis

      // Lepton properties are numbered as follows for each channel:
      // electron     (1)  + tau        (2)
      // muon         (1)  + tau        (2)
      // electron     (1)  + muon       (2)
      // high pT tau  (1)  + low pT tau (2)

      // Run
      synctree_->Branch("run", &run_, "run/I");
      // Lumi
      synctree_->Branch("lumi", &lumi_, "lumi/I");
      // Event
      synctree_->Branch("evt", &event_, "event/l");

      // Number of primary vertices passing good vertex selection
      synctree_->Branch("npv", &n_vtx_, "n_vtx/I");
      // Number of in-time pileup interactions (used for pileup reweighting)
      synctree_->Branch("npu", &n_pu_, "n_pu/I");
      // The rho used for jet energy corrections
      synctree_->Branch("rho", &rho_, "rho/F");

      // The lumi scaling factor for mc * additional weights
      // (not filled in IC trees!)
      /*synctree_->Branch("mcweight", &mc_weight_, "mc_weight/F");*/
      // Pileup weight
      synctree_->Branch("puweight", &pu_weight_, "pu_weight/F");

      // Tag-and-probe weights for leptons
      // Total trigger weight for lepton 1
      /*synctree_->Branch("trigweight_1", &trigweight_1_, "trigweight_1/F");
      // Total trigger weight for lepton 2
      synctree_->Branch("trigweight_2", &trigweight_2_, "trigweight_2/F");
      // Total ID weight for lepton 1
      synctree_->Branch("idweight_1", &idweight_1_, "idweight_1/F");
      // Total ID weight for lepton 2
      synctree_->Branch("idweight_2", &idweight_2_, "idweight_2/F");
      // Total iso weight for lepton 1
      synctree_->Branch("isoweight_1", &isoweight_1_, "isoweight_1/F");
      // Total iso weight for lepton 2
      synctree_->Branch("isoweight_2", &isoweight_2_, "isoweight_2/F");
     // Product of all trigger, ID and iso weights
      synctree_->Branch("effweight", &effweight_, "effweight/F");
      // Jet->tau fake rate weight (pT-dependent)
      synctree_->Branch("fakeweight", &fakeweight_, "fakeweight/F");
      // Product of all embedded weights, but only for rechit samples
      synctree_->Branch("embeddedWeight", &embeddedweight_, "embeddedweight/F");
      // Higgs pt weights (for ggh samples)
      synctree_->Branch("signalWeight", &signalweight_, "signalweight/F");
      // Total combined event weight (excluding lumi weighting)
      // NB: may contain weights not included in the above
      synctree_->Branch("weight", &wt_.var_float, "wt/F");
*/

      // Visible di-tau mass
      synctree_->Branch("m_vis", &m_vis_.var_float, "m_vis/F");
      // SVFit di-tau mass
      synctree_->Branch("m_sv", &m_sv_.var_float, "m_sv/F");
      // SVFit di-tau pt (only for Markov-Chain SVFit)
      synctree_->Branch("pt_sv", &pt_h_.var_float, "pt_h/F");
      // SVFit di-tau eta (only for Markov-Chain SVFit)
      synctree_->Branch("eta_sv", &eta_h_, "eta_h/F");
      // SVFit di-tau phi (only for Markov-Chain SVFit)
      synctree_->Branch("phi_sv", &phi_h_, "phi_h/F");

      // Lepton 1 properties
      // pt (including effect of any energy scale corrections)
      synctree_->Branch("pt_1", &pt_1_.var_float, "pt_1/F");
      // phi
      synctree_->Branch("phi_1", &phi_1_.var_float, "phi_1/F");
      // eta
      synctree_->Branch("eta_1", &eta_1_.var_float, "eta_1/F");
      // mass
      synctree_->Branch("m_1", &m_1_, "m_1/F");
      // charge
      synctree_->Branch("q_1", &q_1_, "q_1/I");
      // delta-beta corrected isolation (relative or absolute as appropriate)
      // If lepton 1 is a tau, this is the value of byIsolationMVAraw,
      // which is no longer used in the analysis, but retained for legacy
      // reasons
      synctree_->Branch("iso_1", &iso_1_.var_float, "iso_1/F");
      // If an electron, the output of the ID MVA, zero otherwise
      synctree_->Branch("mva_1", &mva_1_.var_float, "mva_1/F");
      // Transverse (x-y) impact parameter w.r.t to the primary vertex
      synctree_->Branch("d0_1", &d0_1_.var_float, "d0_1/F");
      // Longitudinal (z) impact parameter w.r.t to the primary vertex
      synctree_->Branch("dZ_1", &dz_1_.var_float, "dz_1/F");
      // Whether lepton passes ID selection (always true in IC ntuples)
//      synctree_->Branch("passid_1", &lPassId1, "lPassId1/B");
      // Whether lepton passes iso selection (always true in IC ntuples)
//      synctree_->Branch("passiso_1", &lPassIso1, "lPassIso1/B");
      // Transverse mass of lepton 1 and MVA MET
      synctree_->Branch("mt_1", &mt_1_.var_float, "mt_1/F");
      synctree_->Branch("pfmt_1", &pfmt_1_.var_float, "pfmt_1/F");
      synctree_->Branch("puppimt_1", &puppimt_1_.var_float, "puppimt_1/F");
      // Non-triggering electron ID MVA score
      synctree_->Branch("id_e_mva_nt_loose_1", &id_e_mva_nt_loose_1_, "id_e_mva_nt_loose_1/F");
      /*synctree_->Branch("trigger_object_pt_1",&trigger_object_pt_1.var_float,"trigger_object_pt_1/F");
      synctree_->Branch("trigger_object_eta_1",&trigger_object_eta_1.var_float,"trigger_object_eta_1/F");
      synctree_->Branch("trigger_object_pt_2",&trigger_object_pt_2.var_float,"trigger_object_pt_2/F");
      synctree_->Branch("trigger_object_eta_2",&trigger_object_eta_2.var_float,"trigger_object_eta_2/F");
*/


      // Lepton 2 properties
      // pt (including effect of any energy scale corrections)
      synctree_->Branch("pt_2", &pt_2_.var_float, "pt_2/F");
      // phi
      synctree_->Branch("phi_2", &phi_2_.var_float, "phi_2/F");
      // eta
      synctree_->Branch("eta_2", &eta_2_.var_float, "eta_2/F");
      // mass
      synctree_->Branch("m_2", &m_2_.var_float, "lM2/F");
      // charge
      synctree_->Branch("q_2", &q_2_, "lq2/I");
      // delta-beta corrected isolation (relative or absolute as appropriate)
      // If lepton 2 is a tau, this is the value of byIsolationMVAraw,
      // which is no longer used in the analysis, but retained for legacy
      // reasons
      synctree_->Branch("iso_2", &iso_2_.var_float, "iso_2/F");
      // Transverse (x-y) impact parameter w.r.t to the primary vertex
      synctree_->Branch("d0_2", &d0_2_.var_float, "d0_2/F");
      // Longitudinal (z) impact parameter w.r.t to the primary vertex
      synctree_->Branch("dZ_2", &dz_2_.var_float, "dz_2/F");
      // If an electron, the output of the ID MVA, zero otherwise
      synctree_->Branch("mva_2", &mva_2_.var_float, "mva_2/F");
      // Whether lepton passes ID selection (always true in IC ntuples)
//      synctree_->Branch("passid_2", &lPassId2, "lPassId2/B");
      // Whether lepton passes iso selection (always true in IC ntuples)
//      synctree_->Branch("passiso_2", &lPassIso2, "lPassIso2/B");
      // Transverse mass of lepton 2 and MVA MET
      synctree_->Branch("mt_2", &mt_2_.var_float, "mt_2/F");

      // Whether event is os or ss
      synctree_->Branch("os", &os_, "os/O");
      // Whether event passes lepton vetos. TRUE = event is vetoed.  
      synctree_->Branch("dilepton_veto", &dilepton_veto_, "dilepton_veto/O");
      synctree_->Branch("extraelec_veto", &extraelec_veto_, "extraelec_veto/O");
      synctree_->Branch("extramuon_veto", &extramuon_veto_, "extramuon_veto/O");
      synctree_->Branch("gen_match_1", &gen_match_1_, "gen_match_1/i");
      synctree_->Branch("gen_match_2", &gen_match_2_,"gen_match_2/i");

      // Variables defined when lepton 2 is a tau
      // raw value of the 3hits delta-beta isolation
      
      if(strategy_ == strategy::paper2013) {
          synctree_->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_2", &l3Hits_2,
                         "byCombinedIsolationDeltaBetaCorrRaw3Hits_2/F");
          // raw value of the anti-electron MVA3 output
          synctree_->Branch("againstElectronMVA3raw_2", &lagainstElectronMVA3raw_2,
                         "againstElectronMVA3raw_2/F");
          // raw value of the MVA2 isolation
          synctree_->Branch("byIsolationMVA2raw_2", &lbyIsolationMVA2raw_2,
                         "byIsolationMVA2raw_2/F");
          // output of againstMuonLoose2
          synctree_->Branch("againstMuonLoose2_2", &lagainstMuonLoose2_2,
                         "againstMuonLoose2_2/F");
          // output of againstMuonMedium2
          synctree_->Branch("againstMuonMedium2_2", &lagainstMuonMedium2_2,
                         "againstMuonMedium2_2/F");
          // output of againstMuonTight2
          synctree_->Branch("againstMuonTight2_2", &lagainstMuonTight2_2,
                     "againstMuonTight2_2/F");
      }
      if(strategy_ == strategy::phys14 || strategy_==strategy::spring15) {
          synctree_->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_1", &l3Hits_1,
                         "byCombinedIsolationDeltaBetaCorrRaw3Hits_1/F");
          synctree_->Branch("byIsolationMVA3newDMwoLTraw_1", &lbyIsolationMVA3newDMwoLTraw_1,"byIsolationMVA3newDMwoLTraw_1/F");
          synctree_->Branch("byIsolationMVA3oldDMwoLTraw_1", &lbyIsolationMVA3oldDMwoLTraw_1,"byIsolationMVA3oldDMwoLTraw_1/F");
          synctree_->Branch("byIsolationMVA3newDMwLTraw_1", &lbyIsolationMVA3newDMwLTraw_1,"byIsolationMVA3newDMwLTraw_1/F");
          synctree_->Branch("byIsolationMVA3oldDMwLTraw_1", &lbyIsolationMVA3oldDMwLTraw_1,"byIsolationMVA3oldDMwLTraw_1/F");
          synctree_->Branch("againstElectronLooseMVA5_1", &lagainstElectronLooseMVA5_1, "againstElectronLooseMVA5_1/F");
          synctree_->Branch("againstElectronMediumMVA5_1", &lagainstElectronMediumMVA5_1, "againstElectronMediumMVA5_1/F");
          synctree_->Branch("againstElectronTightMVA5_1", &lagainstElectronTightMVA5_1, "againstElectronTightMVA5_1/F");
          synctree_->Branch("againstElectronVLooseMVA5_1", &lagainstElectronVLooseMVA5_1, "againstElectronVLooseMVA5_1/F");
          synctree_->Branch("againstElectronVTightMVA5_1", &lagainstElectronVTightMVA5_1, "againstElectronVTightMVA5_1/F");
          synctree_->Branch("againstMuonLoose3_1", &lagainstMuonLoose3_1, "againstMuonLoose3_1/F");
          synctree_->Branch("againstMuonTight3_1", &lagainstMuonTight3_1, "againstMuonTight3_1/F");
          synctree_->Branch("chargedIsoPtSum_1", &lchargedIsoPtSum_1,"chargedIsoPtSum_1/F");
          synctree_->Branch("neutralIsoPtSum_1", &lneutralIsoPtSum_1,"neutralIsoPtSum_1/F");
          synctree_->Branch("puCorrPtSum_1", &lpuCorrPtSum_1,"puCorrPtSum_1/F");
          synctree_->Branch("decayModeFindingOldDMs_1",&ldecayModeFindingOldDMs_1,"decayModeFindingOldDMs_1/O");
          synctree_->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_2", &l3Hits_2,
                         "byCombinedIsolationDeltaBetaCorrRaw3Hits_2/F");
          synctree_->Branch("byIsolationMVA3newDMwoLTraw_2", &lbyIsolationMVA3newDMwoLTraw_2,"byIsolationMVA3newDMwoLTraw_2/F");
          synctree_->Branch("byIsolationMVA3oldDMwoLTraw_2", &lbyIsolationMVA3oldDMwoLTraw_2,"byIsolationMVA3oldDMwoLTraw_2/F");
          synctree_->Branch("byIsolationMVA3newDMwLTraw_2", &lbyIsolationMVA3newDMwLTraw_2,"byIsolationMVA3newDMwLTraw_2/F");
          synctree_->Branch("byIsolationMVA3oldDMwLTraw_2", &lbyIsolationMVA3oldDMwLTraw_2,"byIsolationMVA3oldDMwLTraw_2/F");
          synctree_->Branch("againstElectronLooseMVA5_1", &lagainstElectronLooseMVA5_1, "againstElectronLooseMVA5_1/F");
          synctree_->Branch("againstElectronLooseMVA5_2", &lagainstElectronLooseMVA5_2, "againstElectronLooseMVA5_2/F");
          synctree_->Branch("againstElectronMediumMVA5_2", &lagainstElectronMediumMVA5_2, "againstElectronMediumMVA5_2/F");
          synctree_->Branch("againstElectronTightMVA5_2", &lagainstElectronTightMVA5_2, "againstElectronTightMVA5_2/F");
          synctree_->Branch("againstElectronVLooseMVA5_2", &lagainstElectronVLooseMVA5_2, "againstElectronVLooseMVA5_2/F");
          synctree_->Branch("againstElectronVTightMVA5_2", &lagainstElectronVTightMVA5_2, "againstElectronVTightMVA5_2/F");
          synctree_->Branch("againstMuonLoose3_2", &lagainstMuonLoose3_2, "againstMuonLoose3_2/F");
          synctree_->Branch("againstMuonTight3_2", &lagainstMuonTight3_2, "againstMuonTight3_2/F");
          synctree_->Branch("chargedIsoPtSum_2", &lchargedIsoPtSum_2,"chargedIsoPtSum_2/F");
          synctree_->Branch("neutralIsoPtSum_2", &lneutralIsoPtSum_2,"neutralIsoPtSum_2/F");
          synctree_->Branch("puCorrPtSum_2", &lpuCorrPtSum_2,"puCorrPtSum_2/F");
          synctree_->Branch("decayModeFindingOldDMs_2",&ldecayModeFindingOldDMs_2,"decayModeFindingOldDMs_2/O");

      }
      // Uncorrected PF MET (not used in analysis)
      synctree_->Branch("met", &pfmet_.var_float, "pfmet/F");
      // Uncorrected PF MET phi (not used in analysis)
      synctree_->Branch("metphi", &pfmet_phi_, "pfmet_phi/F");
      // Elements of the PF MET covariance matrix (not used in analysis)
      synctree_->Branch("metcov00", &pfmetCov00_, "pfmetCov00/F");
      synctree_->Branch("metcov01", &pfmetCov01_, "pfmetCov01/F");
      synctree_->Branch("metcov10", &pfmetCov10_, "pfmetCov10/F");
      synctree_->Branch("metcov11", &pfmetCov11_, "pfmetCov11/F");
      //Puppi Met
      synctree_->Branch("puppimet",&puppimet_.var_float, "puppimet/F");
      synctree_->Branch("puppimetphi", &puppimet_phi_,"puppimet_phi/F");
      // MVA MET
      synctree_->Branch("mvamet", &mvamet_.var_float, "mvamet/F");
      // MVA MET phi
      synctree_->Branch("mvametphi", &mvamet_phi_.var_float, "mvamet_phi/F");
      // Elements of the MVA MET covariance matrix
      synctree_->Branch("mvacov00", &mvametCov00_, "mvametCov00/F");
      synctree_->Branch("mvacov01", &mvametCov01_, "mvametCov01/F");
      synctree_->Branch("mvacov10", &mvametCov10_, "mvametCov10/F");
      synctree_->Branch("mvacov11", &mvametCov11_, "mvametCov11/F");

      // pt of the di-tau + MET system
      synctree_->Branch("pt_tt", &pt_tt_.var_float, "pt_tt/F");

      // Visible pzeta
      synctree_->Branch("pzetavis", &pzetavis_.var_float, "pzetavis/F");
      // MET pzeta
      synctree_->Branch("pzetamiss", &pzetamiss_.var_float, "pzetamiss/F");
      // PF met pzeta
      synctree_->Branch("pfpzetamiss",&pfpzetamiss_.var_float,"pfpzetamiss/F");
      // Puppi met pzeta
      synctree_->Branch("puppipzetamiss",&puppipzetamiss_.var_float,"puppipzetamiss/F");
      // ttbar-rejection MVA output (emu channel only)
      synctree_->Branch("mva_gf", &em_gf_mva_, "em_gf_mva/F");

      // Jet properties
      // The following properties are for the leading (1) and sub-leading (2) jets
      // with pt > 30, |eta| < 4.7 after jet energy corrections, PF jet ID and
      // pileup jet ID are applied. Jets overlapping with either selected lepton
      // are not counted

      // Number of jets passing above selection
      synctree_->Branch("njets", &n_jets_, "n_jets/I");
      // Number of jets passing above selection but with
      // pt > 20 instead of pt > 30
      synctree_->Branch("njetspt20", &n_lowpt_jets_, "n_lowpt_jets/I");

      // Leading Jet
      // pt
      synctree_->Branch("jpt_1", &jpt_1_.var_float, "jpt_1/F");
      // eta
      synctree_->Branch("jeta_1", &jeta_1_.var_float, "jeta_1/F");
      // phi
      synctree_->Branch("jphi_1", &jphi_1_, "jphi_1/F");
      // raw pt (before JEC)
      synctree_->Branch("jrawf_1", &jrawf_1_, "jrawf_1/F");
      // pt uncertainty relative to corrected pt (not in IC ntuples)
//      synctree_->Branch("jptunc_1", &jptunc_1_, "jptunc_1/F");
      // Pileup ID MVA output
      synctree_->Branch("jmva_1", &jmva_1_, "jmva_1/F");
      // Linear radial moment (not used in htt analysis)
 //     synctree_->Branch("jlrm_1", &jlrm_1_, "jlrm_1/F");
      // Charged track multiplicity (not used in htt analysis)
  //    synctree_->Branch("jctm_1", &jctm_1_, "jctm_1/I");

      // Sub-leading Jet
      // pt
      synctree_->Branch("jpt_2", &jpt_2_.var_float, "jpt_2/F");
      // eta
      synctree_->Branch("jeta_2", &jeta_2_.var_float, "jeta_2/F");
      // phi
      synctree_->Branch("jphi_2", &jphi_2_, "jphi_2/F");
      // raw pt (before JEC)
      synctree_->Branch("jrawf_2", &jrawf_2_, "jrawf_2/F");
      // pt uncertainty relative to corrected pt (not in IC ntuples)
//      synctree_->Branch("jptunc_2", &jptunc_2_, "jptunc_2/F");
      // Pileup ID MVA output
      synctree_->Branch("jmva_2", &jmva_2_, "jmva_2/F");
      // Linear radial moment (not used in htt analysis)
//      synctree_->Branch("jlrm_2", &jlrm_2_, "jlrm_2/F");
      // Charged track multiplicity (not used in htt analysis)
 //     synctree_->Branch("jctm_2", &jctm_2_, "jctm_2/I");

      // Di-jet properties
      // Calculated with leading and sub-leading jets when njets >= 2
      // di-jet mass
      synctree_->Branch("mjj", &mjj_.var_float, "mjj/F");
      // absolute difference in eta
      synctree_->Branch("jdeta", &jdeta_.var_float, "jdeta/F");
      // Delta phi
      synctree_->Branch("jdphi", &jdphi_, "jdphi/F");
      // number of jets (pt>30 and passing pf jet id), in pseudorapidity gap
      // between jets
      synctree_->Branch("njetingap", &n_jetsingap_, "n_jetsingap/I");
      // number of jets (pt>20 and passing pf jet id) in pseudorapidity gap between jets
      synctree_->Branch("njetingap20", &n_jetsingap20_, "n_jetsingap20/I");

      // B-Tagged Jet properties
      // The following properties are for the leading (in pt) CSV medium b-tagged
      // jet with pt > 20, |eta| < 2.4 after jet energy corrections, PF jet ID and
      // pileup jet ID are applied. Jets overlapping with either selected lepton
      // are not counted

      // Number of b-tagging jets passing above selections
      synctree_->Branch("nbtag", &n_bjets_, "n_bjets/I");
      // pt
      synctree_->Branch("bpt_1", &bpt_1_.var_float, "bpt_1/F");
      // eta
      synctree_->Branch("beta_1", &beta_1_.var_float, "beta_1/F");
      // phi
      synctree_->Branch("bphi_1", &bphi_1_, "bphi_1/F");
      //pu id mva
      synctree_->Branch("bmva_1", &bmva_1_, "bmva_1/F");
      //bcsv value
      synctree_->Branch("bcsv_1", &bcsv_1_.var_float, "bcsv_1/F");

      synctree_->Branch("brawf_1", &brawf_1_, "brawf_1/F");
      //pt b-jet 2
      synctree_->Branch("bpt_2", &bpt_2_.var_float, "bpt_2/F");
      // eta b-jet 2
      synctree_->Branch("beta_2", &beta_2_.var_float, "beta_2/F");
      // phi b-jet 2
      synctree_->Branch("bphi_2", &bphi_2_, "bphi_2/F");
      //pu id mva
      synctree_->Branch("bmva_2", &bmva_2_, "bmva_2/F");
      //bcsv value
      synctree_->Branch("bcsv_2", &bcsv_2_, "bcsv_2/F");

      synctree_->Branch("brawf_2", &brawf_2_, "brawf_2/F");

    }
    return 0;
  }

  int HTTCategories::Execute(TreeEvent *event) {

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    
    wt_ = {eventInfo->total_weight(), static_cast<float>(eventInfo->total_weight())};
    run_ = eventInfo->run();
    event_ = (unsigned long long) eventInfo->event();
    lumi_ = eventInfo->lumi_block();
    std::vector<PileupInfo *> puInfo;
    float true_int = -1;

    if (event->Exists("pileupInfo") || strategy_ == strategy::phys14 || (strategy_==strategy::spring15 && !is_data_) ) {
     puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
      for (unsigned i = 0; i < puInfo.size(); ++i) {
        if (puInfo[i]->bunch_crossing() == 0)
          true_int = puInfo[i]->true_num_interactions();
      }
    }
    n_pu_ = true_int;
    rho_ = eventInfo->jet_rho();
    if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    if(event->Exists("gen_match_2")) gen_match_2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
    /*if(event->Exists("leading_lepton_match_pt")) leading_lepton_match_pt_ = event->Get<double>("leading_lepton_match_pt");
    if(event->Exists("subleading_lepton_match_pt")) subleading_lepton_match_pt_ = event->Get<double>("subleading_lepton_match_pt");
    if(event->Exists("leading_lepton_match_DR")) leading_lepton_match_DR_ = event->Get<double>("leading_lepton_match_DR");
    if(event->Exists("subleading_lepton_match_DR")) subleading_lepton_match_DR_ = event->Get<double>("subleading_lepton_match_DR");*/
   
    wt_ggh_pt_up_ = 1.0;
    wt_ggh_pt_down_ = 1.0;
    wt_tau_fake_up_ = 1.0;
    wt_tau_fake_down_ = 1.0;
    wt_tquark_up_ = 1.0;
    wt_tquark_down_ = 1.0;
    wt_tau_id_up_ = 1.0;
    wt_tau_id_down_ = 1.0;
    if (event->Exists("wt_ggh_pt_up"))      wt_ggh_pt_up_   = event->Get<double>("wt_ggh_pt_up");
    if (event->Exists("wt_ggh_pt_down"))    wt_ggh_pt_down_ = event->Get<double>("wt_ggh_pt_down");
    if (event->Exists("wt_tau_fake_up"))    wt_tau_fake_up_   = event->Get<double>("wt_tau_fake_up");
    if (event->Exists("wt_tau_fake_down"))  wt_tau_fake_down_ = event->Get<double>("wt_tau_fake_down");
    if (event->Exists("wt_tquark_up"))      wt_tquark_up_   = event->Get<double>("wt_tquark_up");
    if (event->Exists("wt_tquark_down"))    wt_tquark_down_ = event->Get<double>("wt_tquark_down");
    if (event->Exists("wt_tau_id_up"))      wt_tau_id_up_   = event->Get<double>("wt_tau_id_up");
    if (event->Exists("wt_tau_id_down"))    wt_tau_id_down_ = event->Get<double>("wt_tau_id_down");
    if (event->Exists("mc_weight_sign"))    std::cout<<event->Get<double>("mc_weight_sign");
  
  mc_weight_ = 0.0;
  if (!is_embedded_ && event->Exists("pileupInfo")) pu_weight_ = eventInfo->weight("pileup"); else pu_weight_ = 0.0;
  if (event->Exists("trigweight_1")) trigweight_1_ = event->Get<double>("trigweight_1"); else trigweight_1_ = 0.0;
  if (event->Exists("trigweight_2")) trigweight_2_ = event->Get<double>("trigweight_2"); else trigweight_2_ = 0.0;
  if (event->Exists("idweight_1")) idweight_1_ = event->Get<double>("idweight_1"); else idweight_1_ = 0.0;
  if (event->Exists("idweight_2")) idweight_2_ = event->Get<double>("idweight_2"); else idweight_2_ = 0.0;
  if (event->Exists("isoweight_1")) isoweight_1_ = event->Get<double>("isoweight_1"); else isoweight_1_ = 0.0;
  if (event->Exists("isoweight_2")) isoweight_2_ = event->Get<double>("isoweight_2"); else isoweight_2_ = 0.0;
  if (eventInfo->weight_defined("lepton")) effweight_ = eventInfo->weight("lepton"); else effweight_ = 0.0;
  if (eventInfo->weight_defined("tau_fake_weight")) fakeweight_ = eventInfo->weight("tau_fake_weight"); else fakeweight_ = 0.0;
  if (eventInfo->weight_defined("tau_mode_scale")) effweight_ *= eventInfo->weight("tau_mode_scale") ;
   
  if (eventInfo->weight_defined("tauspinner")) {
    embeddedweight_ = eventInfo->weight("tauspinner") *
      eventInfo->weight("zmm_eff") *
      //eventInfo->weight("muon_rad") *
      eventInfo->weight("kin_weight1") *
      eventInfo->weight("kin_weight2") *
      eventInfo->weight("kin_weight3") *
      eventInfo->weight("embed_weight");
   } else {
    embeddedweight_ = 0.;
   }
  if (eventInfo->weight_defined("ggh")) {
    signalweight_ = eventInfo->weight("ggh");
   } else {
    signalweight_ = 0.;
   }

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    
    Met const* mets = NULL;
    //MVA met doesnt exist for Z->ee and Z->mumu
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mets = event->GetPtr<Met>(met_label_);

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::vector<PFJet*> corrected_jets;
    if(bjet_regression_) corrected_jets = event->GetPtrVec<PFJet>(jets_label_+"Corrected");
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    std::vector<PFJet*> prebjets = lowpt_jets;
    ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<PFJet*> bjets = prebjets;
    std::vector<PFJet*> loose_bjets = prebjets;
    std::string btag_label="combinedSecondaryVertexBJetTags";
    double btag_wp =  0.679;
    if(strategy_ == strategy::phys14) btag_label = "combinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::phys14) btag_wp = 0.814 ;
    if(strategy_ == strategy::spring15) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::spring15) btag_wp = 0.89 ;

    ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < 0.244);
    //Extra set of jets which are CSV ordered is required for the H->hh analysis
    std::vector<PFJet*> jets_csv = prebjets;
    std::vector<PFJet*> bjets_csv = prebjets;
    std::sort(jets_csv.begin(), jets_csv.end(), bind(&PFJet::GetBDiscriminator, _1, btag_label) > bind(&PFJet::GetBDiscriminator, _2, btag_label));
    std::vector<std::pair<PFJet*,PFJet*> > jet_csv_pairs;
    if(bjet_regression_) jet_csv_pairs = MatchByDR(jets_csv, corrected_jets, 0.5, true, true);

    // Instead of changing b-tag value in the promote/demote method we look for a map of bools
    // that say whether a jet should pass the WP or not
    if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
      ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
      ic::erase_if(bjets_csv, !boost::bind(IsReBTagged, _1, retag_result));
    } else{ 
      ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
      ic::erase_if(bjets_csv, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
    } 
    
    // Define event properties
    // IMPORTANT: Make sure each property is re-set
    // for each new event
    if (PairOppSign(ditau)) {
      os_ = true;
    } else {
      os_ = false;
    }
    //Fill extra lepton veto bools
    dilepton_veto_ = false;
    extraelec_veto_ = false;
    extramuon_veto_ = false;
    minimal_extraelec_veto_ = false;
    minimal_extramuon_veto_ = false;
    if(channel_ == channel::et) { 
        if(event->Exists("dielec_veto")) dilepton_veto_ = event->Get<bool>("dielec_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");
    }
    if(channel_ == channel::mt) { 
        if(event->Exists("dimuon_veto")) dilepton_veto_ = event->Get<bool>("dimuon_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");

    }
    if(channel_ == channel::em) { 
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");

    }
    if(channel_ == channel::tt) {
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");

    }
    lepton_veto_ = dilepton_veto_ || extraelec_veto_ || extramuon_veto_;


    n_vtx_ = eventInfo->good_vertices();
    /*trigger_object_pt_1 = 0;
    trigger_object_pt_2 = 0;
    trigger_object_eta_1 = 0;
    trigger_object_eta_2 = 0;
*/
    if(event->Exists("good_first_vertex")) good_vtx_ = event->Get<bool>("good_first_vertex");
 /*   if(event->Exists("leg1_trigger_obj_pt")) trigger_object_pt_1 = event->Get<double>("leg1_trigger_obj_pt");
    if(event->Exists("leg1_trigger_obj_eta")) trigger_object_eta_1 = event->Get<double>("leg1_trigger_obj_eta");
    if(event->Exists("leg2_trigger_obj_pt")) trigger_object_pt_2 = event->Get<double>("leg2_trigger_obj_pt");
    if(event->Exists("leg2_trigger_obj_eta")) trigger_object_eta_2 = event->Get<double>("leg2_trigger_obj_eta");
*/

    if (event->Exists("svfitMass")) {
      m_sv_ = event->Get<double>("svfitMass");
    } else {
      m_sv_ = -9999;
    }

   if (event->Exists("svfitMT")){
     mt_sv_ = event->Get<double>("svfitMT");
   } else {
     mt_sv_ = -9999;
   }

    if (event->Exists("svfitHiggs")) {
      Candidate const& higgs = event->Get<Candidate>("svfitHiggs");
      pt_h_ = higgs.pt();
      eta_h_ = higgs.eta();
      phi_h_ = higgs.phi();
    } else {
      pt_h_ = -9999;
      eta_h_ = -9999;
      phi_h_ = -9999;
    }

    Met const* pfmet = NULL;
    Met const* puppimet = NULL;
    //slightly different met format for new ntuples
    if(strategy_ == strategy::paper2013) pfmet = event->GetPtr<Met>("pfMet");
    if(strategy_ == strategy::phys14 || strategy_ == strategy::spring15) {
      std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMet");
      pfmet = pfMet_vec.at(0);  
      if(event->ExistsInTree("puppiMet")){
        std::vector<Met*> puppiMet_vec = event->GetPtrVec<Met>("puppiMet");
        puppimet = puppiMet_vec.at(0);
      }
    }


    //MVA met doesnt exist for Z->ee and Z->mumu
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) pt_tt_ = (ditau->vector() + mets->vector()).pt();
    m_vis_ = ditau->M();
   

    // This is the HCP hack for the em channel
    // to better align the data with the embedded
    // mass.  
    if (channel_ == channel::em) {
      m_sv_ = m_sv_ * mass_shift_;
      m_vis_ = m_vis_* mass_shift_;
      em_gf_mva_ = event->Exists("em_gf_mva") ? event->Get<double>("em_gf_mva") : 0.;
      // em_vbf_mva_ = event->Exists("em_vbf_mva") ? event->Get<double>("em_vbf_mva") : 0.;
    }
    if (event->Exists("mass_scale")) {
      m_sv_ = m_sv_ * event->Get<double>("mass_scale");
      m_vis_ = m_vis_* event->Get<double>("mass_scale");
    }

    mt_lep_ = MT(lep1,lep2);
    //MVA met doesnt exist for Z->ee and Z->mumu
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mt_1_ = MT(lep1, mets);
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mt_2_ = MT(lep2, mets);
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mt_ll_ = MT(ditau, mets);
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mt_tot_ = sqrt(pow(mt_lep_.var_double,2)+pow(mt_2_.var_double,2)+pow(mt_1_.var_double,2));
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) pzeta_ = PZeta(ditau, mets, 0.85);
    pzetavis_ = PZetaVis(ditau);
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) pzetamiss_ = PZeta(ditau, mets, 0.0);
    //save some pfmet and puppi met versions as well for now
    pfmt_1_ = MT(lep1, pfmet);
    pfpzeta_ = PZeta(ditau, pfmet, 0.85);
    pfpzetamiss_ = PZeta(ditau, pfmet, 0.0);
    if(puppimet != NULL){
      puppimt_1_ = MT(lep1, puppimet);
      puppipzeta_ = PZeta(ditau, puppimet, 0.85);
      puppipzetamiss_ = PZeta(ditau, puppimet,0.0);
    }

    if(channel_ == channel::em || channel_ == channel::et){
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      id_e_mva_nt_loose_1_ = elec->GetIdIso("mvaNonTrigSpring15");
    }

    emu_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(), lep2->vector()));

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();
    phi_1_ = lep1->phi();
    phi_2_ = lep2->phi();
    E_1_ = lep1->energy();
    E_2_ = lep2->energy();
    m_1_ = lep1->M();
    m_2_ = lep2->M();
    q_1_ = lep1->charge();
    q_2_ = lep2->charge();
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mvamet_ = mets->pt();
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mvamet_phi_ = mets->phi();

    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mvametCov00_ = mets->xx_sig();
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mvametCov10_ = mets->yx_sig();
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mvametCov01_ = mets->xy_sig();
    if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mvametCov11_ = mets->yy_sig();
    
    pfmet_ = pfmet->pt();
    pfmet_phi_ = pfmet->phi();
    if(puppimet != NULL){
      puppimet_ = puppimet->pt();
      puppimet_phi_ = puppimet->phi();
    }
  
    pfmetCov00_ = pfmet->xx_sig();
    pfmetCov01_ = pfmet->xy_sig();
    pfmetCov10_ = pfmet->yx_sig();
    pfmetCov11_ = pfmet->yy_sig();
    
    emu_dxy_1_ = 0.0;
    emu_dxy_2_ = 0.0;
    
    antiele_1_ = true;
    antimu_1_ = true;
    antiele_2_ = true;
    antimu_2_ = true;
    if (channel_ == channel::et) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      d0_1_ = elec->dxy_vertex();
      dz_1_ = elec->dz_vertex();
      d0_2_ = tau->lead_dxy_vertex();
      dz_2_ = tau->lead_dz_vertex();
     
      if(strategy_ == strategy::paper2013) {
        iso_1_ = PF04IsolationVal(elec, 0.5);
        mva_1_ = elec->GetIdIso("mvaNonTrigV0");
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronMVA3raw_2 = tau->HasTauID("againstElectronMVA3raw") ? tau->GetTauID("againstElectronMVA3raw") : 0. ;
        lbyIsolationMVA2raw_2 = tau->HasTauID("byIsolationMVA2raw") ? tau->GetTauID("byIsolationMVA2raw") : 0. ;
        lagainstMuonLoose2_2 = tau->HasTauID("againstMuonLoose2") ? tau->GetTauID("againstMuonLoose2") : 0. ;
        lagainstMuonMedium2_2 = tau->HasTauID("againstMuonMedium2") ? tau->GetTauID("againstMuonMedium2") : 0. ;
        lagainstMuonTight2_2 = tau->HasTauID("againstMuonTight2") ? tau->GetTauID("againstMuonTight2") : 0. ;
      }
      if(strategy_ == strategy::phys14) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        mva_1_ = elec->GetIdIso("mvaNonTrigV025nsPHYS14");
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA5raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;

        lagainstElectronLooseMVA5_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA5_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA5_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA5_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        antiele_2_ = lagainstElectronTightMVA5_2;
        antimu_2_ = lagainstMuonLoose3_2;
      }
      if(strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(elec, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(elec, eventInfo);
          iso_1_db03allch_ = PF03IsolationVal(elec, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(elec, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(elec, 0.5, 0);
          iso_1_trk03_ = 0; 
          iso_2_db03_ = 0;
          iso_2_ea03_ = 0;
          iso_2_db03allch_ = 0;
          iso_2_db04allch_ = 0;
        }
        mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA5raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA5_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA5_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA5_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA5_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lPhotonPtSum_2 = tau->HasTauID("photonPtSumOutsideSignalCone") ? tau->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwLTraw") ? tau->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        antiele_2_ = lagainstElectronTightMVA5_2;
        antimu_2_ = lagainstMuonLoose3_2;
      }

    }
    if (channel_ == channel::mt || channel_ == channel::mtmet) {
      Muon const* muon = dynamic_cast<Muon const*>(lep1);
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      d0_1_ = muon->dxy_vertex();
      dz_1_ = muon->dz_vertex();
      d0_2_ = tau->lead_dxy_vertex();
      dz_2_ = tau->lead_dz_vertex();
      if(strategy_ == strategy::paper2013) {
        iso_1_ = PF04IsolationVal(muon, 0.5);
        mva_1_ = 0.0;
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronMVA3raw_2 = tau->HasTauID("againstElectronMVA3raw") ? tau->GetTauID("againstElectronMVA3raw") : 0. ;
        lbyIsolationMVA2raw_2 = tau->HasTauID("byIsolationMVA2raw") ? tau->GetTauID("byIsolationMVA2raw") : 0. ;
        lagainstMuonLoose2_2 = tau->HasTauID("againstMuonLoose2") ? tau->GetTauID("againstMuonLoose2") : 0. ;
        lagainstMuonMedium2_2 = tau->HasTauID("againstMuonMedium2") ? tau->GetTauID("againstMuonMedium2") : 0. ;
        lagainstMuonTight2_2 = tau->HasTauID("againstMuonTight2") ? tau->GetTauID("againstMuonTight2") : 0. ;
      }
      if(strategy_ == strategy::phys14 || strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(muon, 0.5, 0);
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(muon, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(muon, eventInfo);
          iso_1_db03allch_ = PF03IsolationVal(muon, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(muon, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(muon, 0.5, 0);
          iso_1_trk03_ = MuonTkIsoVal(muon);
          iso_2_db03_ = 0;
          iso_2_ea03_ = 0;
          iso_2_trk03_ = 0;
          iso_2_db04_ = 0;
          iso_2_db03allch_ = 0;
          iso_2_db04allch_ = 0;
        }
        mva_1_ = 0.0;
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA5raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA5_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA5_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA5_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA5_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwLTraw") ? tau->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;
        lPhotonPtSum_2 = tau->HasTauID("photonPtSumOutsideSignalCone") ? tau->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lbyLooseCombinedIsolation_2 = tau->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        antiele_2_ = lagainstElectronVLooseMVA5_2;
        antimu_2_ = lagainstMuonTight3_2;
      }
    }
    if (channel_ == channel::em) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      Muon const* muon = dynamic_cast<Muon const*>(lep2);
      if(strategy_ == strategy::paper2013) {
        iso_1_ = PF04IsolationVal(elec, 0.5);
        iso_2_ = PF04IsolationVal(muon, 0.5);
      }
      if(strategy_ == strategy::phys14) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        iso_2_ = PF03IsolationVal(muon, 0.5, 0);
        mva_1_ = elec->GetIdIso("mvaNonTrigV025nsPHYS14");
      }
      if(strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        iso_2_ = PF03IsolationVal(muon, 0.5, 0);
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(elec, 0.5, 0);
          iso_1_db04_ = PF04IsolationVal(elec, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(elec, eventInfo);
          iso_1_db03allch_ = PF03IsolationVal(elec, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(elec, 0.5, 1);
          iso_1_trk03_=0;
          iso_2_db03_ = PF03IsolationVal(muon, 0.5, 0);
          iso_2_db04_ = PF04IsolationVal(muon, 0.5, 0);
          iso_2_ea03_ = PF03EAIsolationVal(muon, eventInfo);
          iso_2_trk03_ = MuonTkIsoVal(muon);
          iso_2_db03allch_ = PF03IsolationVal(muon, 0.5, 1);
          iso_2_db04allch_ = PF04IsolationVal(muon, 0.5, 1);
        }
        mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
      }
      lPhotonPtSum_1 = 0.;
      lPhotonPtSum_2 = 0.;

      mva_2_ = 0.0;
      if(strategy_ == strategy::paper2013){
        emu_dxy_1_ = -1. * elec->dxy_vertex();
        emu_dxy_2_ = -1. * muon->dxy_vertex();
      } else {
        emu_dxy_1_ = elec->dxy_vertex();
        emu_dxy_2_ = muon->dxy_vertex();
      }
      d0_1_ = static_cast<float>(emu_dxy_1_);
      dz_1_ = elec->dz_vertex();
      emu_dxy_2_ = muon->dxy_vertex();
      d0_2_ = static_cast<float>(emu_dxy_2_);
      dz_2_ = muon->dz_vertex();
    }
    if (channel_ == channel::tt) {
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      d0_1_ = tau1->lead_dxy_vertex();
      dz_1_ = tau1->lead_dz_vertex();
      d0_2_ = tau2->lead_dxy_vertex();
      dz_2_ = tau2->lead_dz_vertex();
      if(strategy_ == strategy::phys14 || strategy_ == strategy::spring15) {
        iso_1_ = tau1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_1_ = tau1->GetTauID("againstElectronMVA5raw");
        iso_2_ = tau2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau2->GetTauID("againstElectronMVA5raw");
        l3Hits_1 = tau1->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA5_1 = tau1->HasTauID("againstElectronLooseMVA5") ? tau1->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_1 = tau1->HasTauID("againstElectronMediumMVA5") ? tau1->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA5_1 = tau1->HasTauID("againstElectronTightMVA5") ? tau1->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA5_1= tau1->HasTauID("againstElectronVTightMVA5") ? tau1->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA5_1 = tau1->HasTauID("againstElectronVLooseMVA5") ? tau1->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_1 = tau1->HasTauID("againstMuonLoose3") ? tau1->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_1 = tau1->HasTauID("againstMuonTight3") ? tau1->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_1 = tau1->HasTauID("photonPtSumOutsideSignalCone") ? tau1->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lbyLooseCombinedIsolation_1 = tau1->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_1 = tau1->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_1 = tau1->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        l3Hits_2 = tau2->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA5_2 = tau2->HasTauID("againstElectronLooseMVA5") ? tau2->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_2 = tau2->HasTauID("againstElectronMediumMVA5") ? tau2->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA5_2 = tau2->HasTauID("againstElectronTightMVA5") ? tau2->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA5_2 = tau2->HasTauID("againstElectronVTightMVA5") ? tau2->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA5_2 = tau2->HasTauID("againstElectronVLooseMVA5") ? tau2->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_2 = tau2->HasTauID("againstMuonLoose3") ? tau2->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau2->HasTauID("againstMuonTight3") ? tau2->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_2 = tau2->HasTauID("photonPtSumOutsideSignalCone") ? tau2->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lbyLooseCombinedIsolation_2 = tau2->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau2->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau2->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        antiele_1_ = lagainstElectronVLooseMVA5_1;
        antimu_1_ = lagainstMuonLoose3_1;
        antiele_2_ = lagainstElectronVLooseMVA5_2;
        antimu_2_ = lagainstMuonLoose3_2;
        lchargedIsoPtSum_1 = tau1->HasTauID("chargedIsoPtSum") ? tau1->GetTauID("chargedIsoPtSum") : 0.;
        lneutralIsoPtSum_1 = tau1->HasTauID("neutralIsoPtSum") ? tau1->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_1 = tau1->HasTauID("puCorrPtSum") ? tau1->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_1 = tau1->HasTauID("decayModeFinding") ? tau1->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_1 = tau1->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau1->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_1 = tau1->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau1->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_1 = tau1->HasTauID("byIsolationMVA3newDMwLTraw") ? tau1->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_1 = tau1->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau1->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;

        lchargedIsoPtSum_2 = tau2->HasTauID("chargedIsoPtSum") ? tau2->GetTauID("chargedIsoPtSum") : 0.;
        lneutralIsoPtSum_2 = tau2->HasTauID("neutralIsoPtSum") ? tau2->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_2 = tau2->HasTauID("puCorrPtSum") ? tau2->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau2->HasTauID("decayModeFinding") ? tau2->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau2->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau2->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau2->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau2->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau2->HasTauID("byIsolationMVA3newDMwLTraw") ? tau2->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau2->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau2->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;

      }
    }
    if (channel_ == channel::zee || channel_ == channel::tpzee) {
      Electron const* elec1 = dynamic_cast<Electron const*>(lep1);
      Electron const* elec2 = dynamic_cast<Electron const*>(lep2);
      if(strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(elec1, 0.5, 0);
        iso_2_ = PF03IsolationVal(elec2, 0.5, 0);
        mva_1_ = ElectronHTTIdSpring15(elec1, false);
        mva_2_ = ElectronHTTIdSpring15(elec2, false);
      }
      d0_1_ = elec1->dxy_vertex();
      dz_1_ = elec1->dz_vertex();
      d0_2_ = elec2->dxy_vertex();
      dz_2_ = elec2->dz_vertex();
    }
    if (channel_ == channel::zmm || channel_ == channel::tpzmm) {
      Muon const* muon1 = dynamic_cast<Muon const*>(lep1);
      Muon const* muon2 = dynamic_cast<Muon const*>(lep2);
      if(strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(muon1, 0.5, 0);
        iso_2_ = PF03IsolationVal(muon2, 0.5, 0);
        mva_1_ = MuonMedium(muon1);
        mva_2_ = MuonMedium(muon2);
      }
      d0_1_ = muon1->dxy_vertex();
      dz_1_ = muon1->dz_vertex();
      d0_2_ = muon2->dxy_vertex();
      dz_2_ = muon2->dz_vertex();
    }

    if (channel_ == channel::tpzmm || channel_ == channel::tpzee){
      trigger_match_1_ = event->Exists("tp_leg1_match") ? event->Get<bool>("tp_leg1_match") : 0;
      trigger_match_2_ = event->Exists("tp_leg2_match") ? event->Get<bool>("tp_leg2_match") : 0;
    }

    Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
    Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
    if (tau1) {
      tau_decay_mode_1_ = tau1->decay_mode();
      z_1_ = tau1->vz() + (130. / tan(tau1->vector().theta()));
    } else {
      tau_decay_mode_1_ = 0;
      z_1_ = 0.;
    }

    if (tau2) {
      tau_decay_mode_2_ = tau2->decay_mode();
      z_2_ = tau2->vz() + (130. / tan(tau2->vector().theta()));
    } else {
      tau_decay_mode_2_ = 0;
      z_2_ = 0.;
    }

    n_jets_ = jets.size();
    n_lowpt_jets_ = lowpt_jets.size();
    n_bjets_ = bjets.size();
    n_prebjets_ = prebjets.size();
    n_jets_csv_ = jets_csv.size();
    n_loose_bjets_ = loose_bjets.size();

    if (n_lowpt_jets_ >= 1) {
      jpt_1_ = lowpt_jets[0]->pt();
      jeta_1_ = lowpt_jets[0]->eta();
      jphi_1_ = lowpt_jets[0]->phi();
      jrawf_1_ = lowpt_jets[0]->uncorrected_energy()/jets[0]->energy();//* (jets[0]->pt() / jets[0]->energy());
      jptunc_1_ = 0.0;
      jmva_1_ = lowpt_jets[0]->pu_id_mva_value();
      jlrm_1_ = lowpt_jets[0]->linear_radial_moment();
      jctm_1_ = lowpt_jets[0]->charged_multiplicity_nopu();
      std::vector<ic::Tau *> taus = event->GetPtrVec<Tau>("taus");
      std::vector<ic::Jet *> leadjet = { jets[0] };
      std::vector<std::pair<ic::Jet *, ic::Tau *>> matches = MatchByDR(leadjet, taus, 0.5, true, true);
      if (matches.size() == 1) {
        j1_dm_ = matches[0].second->decay_mode();
      } else {
        j1_dm_ = -1;
      }
    } else {
      jpt_1_ = -9999;
      jeta_1_ = -9999;
      jphi_1_ = -9999;
      jrawf_1_ = -9999;
      jptunc_1_ = -9999;
      jmva_1_ = -9999;
      jlrm_1_ = -9999;
      jctm_1_ = -9999;
    }

    if (n_lowpt_jets_ >= 2) {
      jpt_2_ = lowpt_jets[1]->pt();
      jeta_2_ = lowpt_jets[1]->eta();
      jphi_2_ = lowpt_jets[1]->phi();
      jrawf_2_ = lowpt_jets[1]->uncorrected_energy()/jets[1]->energy();// * (jets[1]->pt() / jets[1]->energy());
      jptunc_2_ = 0.0;
      jmva_2_ = lowpt_jets[1]->pu_id_mva_value();
      jlrm_2_ = lowpt_jets[1]->linear_radial_moment();
      jctm_2_ = lowpt_jets[1]->charged_multiplicity_nopu();
      mjj_ = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
      jdeta_ = fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta());
      jdphi_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), lowpt_jets[1]->vector()));
      double eta_high = (lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) ? lowpt_jets[0]->eta() : lowpt_jets[1]->eta();
      double eta_low = (lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) ? lowpt_jets[1]->eta() : lowpt_jets[0]->eta();
      n_jetsingap_ = 0;
      n_jetsingap20_ = 0;
      if (n_lowpt_jets_ > 2) {
        for (unsigned i = 2; i < lowpt_jets.size(); ++i) {
         if (lowpt_jets[i]->pt() > 30.0 &&  lowpt_jets[i]->eta() > eta_low && lowpt_jets[i]->eta() < eta_high) ++n_jetsingap_;
         if (lowpt_jets[i]->pt() > 20.0 &&  lowpt_jets[i]->eta() > eta_low && lowpt_jets[i]->eta() < eta_high) ++n_jetsingap20_;
        }
      }
    } else {
      jpt_2_ = -9999;
      jeta_2_ = -9999;
      jphi_2_ = -9999;
      mjj_ = -9999;
      jdeta_ = -9999;
      jdphi_ = -9999;
      jrawf_2_ = -9999;
      jptunc_2_ = -9999;
      jmva_2_ = -9999;
      jlrm_2_ = -9999;
      jctm_2_ = -9999;
      n_jetsingap_ = 9999;
      n_jetsingap20_ = 9999;
    }

    if (n_lowpt_jets_ >= 2) {
      mjj_lowpt_ = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
      jdeta_lowpt_ = fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta());
      double eta_high = (lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) ? lowpt_jets[0]->eta() : lowpt_jets[1]->eta();
      double eta_low = (lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) ? lowpt_jets[1]->eta() : lowpt_jets[0]->eta();
      n_jetsingap_lowpt_ = 0;
      if (n_lowpt_jets_ > 2) {
        for (unsigned i = 2; i < lowpt_jets.size(); ++i) {
         if (lowpt_jets[i]->pt() > 30.0 &&  lowpt_jets[i]->eta() > eta_low && lowpt_jets[i]->eta() < eta_high) ++n_jetsingap_lowpt_;
        }
      }
    } else {
      mjj_lowpt_ = -9999;
      jdeta_lowpt_ = -9999;
      n_jetsingap_lowpt_ = 9999;
    }

    if (n_bjets_ >= 1) {
      bpt_1_ = bjets[0]->pt();
      brawf_1_ = bjets[0]->uncorrected_energy()/bjets[0]->energy();//* (jets[0]->pt() / jets[0]->energy());
      beta_1_ = bjets[0]->eta();
      bphi_1_ = bjets[0]->phi();
      bmva_1_ = bjets[0]->pu_id_mva_value();
      
    } else {
      bpt_1_ = -9999;
      brawf_1_ = -9999;
      beta_1_ = -9999;
      bphi_1_ = -9999;
      bmva_1_ = -9999;
    }

    if (n_bjets_ >= 2) {
      bpt_2_ = bjets[1]->pt();
      brawf_2_ = bjets[1]->uncorrected_energy()/bjets[1]->energy();//* (jets[0]->pt() / jets[0]->energy());
      beta_2_ = bjets[1]->eta();
      bphi_2_ = bjets[1]->phi();
      bmva_2_ = bjets[1]->pu_id_mva_value();
      
    } else {
      bpt_2_ = -9999;
      brawf_2_ = -9999;
      beta_2_ = -9999;
      bphi_2_ = -9999;
      bmva_2_ = -9999;
    }


    if (n_prebjets_ >= 1) {
      bcsv_1_ = prebjets[0]->GetBDiscriminator(btag_label);
    } else {
      bcsv_1_ = -9999;
    }
    if (n_prebjets_ >= 2) {
      bcsv_2_ = prebjets[1]->GetBDiscriminator(btag_label);
    } else {
      bcsv_2_ = -9999;
    }

    emu_csv_ = (bcsv_1_.var_double > 0.244) ? bcsv_1_.var_double : -1.0;


    n_jets_csv_ = jets_csv.size();
    n_bjets_csv_ = bjets_csv.size();

    if (n_jets_csv_ >= 1) {
      jet_csvpt_1_ = jets_csv[0]->pt();
      if(bjet_regression_) jet_csvpt_1_ = jet_csv_pairs[0].second->pt();
      jet_csvEt_1_ = std::sqrt(jets_csv[0]->pt()*jets_csv[0]->pt() + jets_csv[0]->M()*jets_csv[0]->M());
      if(bjet_regression_) jet_csvEt_1_ = std::sqrt(jet_csvpt_1_*jet_csvpt_1_ + jet_csv_pairs[0].second->M()*jet_csv_pairs[0].second->M());
      jet_csveta_1_ = jets_csv[0]->eta();
      jet_csvbcsv_1_ = jets_csv[0]->GetBDiscriminator(btag_label);
      std::vector<ic::Tau *> taus = event->GetPtrVec<Tau>("taus");
      std::vector<ic::Jet *> leadjet = { jets_csv[0] };
      std::vector<std::pair<ic::Jet *, ic::Tau *>> matches = MatchByDR(leadjet, taus, 0.5, true, true);
    } else {
      jet_csvpt_1_ = -9999;
      jet_csvEt_1_ = -9999;
      jet_csveta_1_ = -9999;
      jet_csvbcsv_1_ = -9999;
    }
    
    if (n_jets_csv_ >= 2) {
      jet_csvpt_2_ = jets_csv[1]->pt();
      if(bjet_regression_) jet_csvpt_2_ = jet_csv_pairs[1].second->pt();
      jet_csvpt_bb_ = (jets_csv[0]->vector()+jets_csv[1]->vector()).pt();
      jet_csv_dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(jets_csv[0]->vector(),jets_csv[1]->vector()));
      jet_csveta_2_ = jets_csv[1]->eta();
      jet_csvbcsv_2_ = jets_csv[1]->GetBDiscriminator(btag_label);
      jet_csv_mjj_ = (jets_csv[0]->vector() + jets_csv[1]->vector()).M();
      if(bjet_regression_) jet_csv_mjj_ = (jet_csv_pairs[0].second->vector() + jet_csv_pairs[1].second->vector()).M();
      jet_csv_deta_ = fabs(jets_csv[0]->eta() - jets_csv[1]->eta());
      jet_csv_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(jets_csv[0]->vector(), jets_csv[1]->vector()));
      jet_csv_dtheta_ = std::fabs((jets_csv[0]->vector().theta() -  jets_csv[1]->vector().theta()));
      if(channel_ != channel::tpzee && channel_ != channel::tpzmm && channel_ != channel::zee && channel_ != channel::zmm) mjj_tt_= (jets_csv[0]->vector() + jets_csv[1]->vector() + ditau->vector() + mets->vector()).M();
      if(bjet_regression_) mjj_tt_= (jet_csv_pairs[0].second->vector() + jet_csv_pairs[1].second->vector() + ditau->vector() + mets->vector()).M();
      if (event->Exists("svfitHiggs")) {
        mjj_h_= (jets_csv[0]->vector() + jets_csv[1]->vector() + event->Get<Candidate>("svfitHiggs").vector() ).M();
        if(bjet_regression_) mjj_h_= (jet_csv_pairs[0].second->vector() + jet_csv_pairs[1].second->vector() + event->Get<Candidate>("svfitHiggs").vector() ).M();
      } else {
        mjj_h_ = -9999;
      }
      if(kinfit_mode_>0) {
        std::vector<Int_t> hypo_mh1;
        hypo_mh1.push_back(125);
        //Option to additionally run results from Zh and hZ hypotheses
        if(kinfit_mode_ == 2) hypo_mh1.push_back(90);
        std::vector<Int_t> hypo_mh2;
        hypo_mh2.push_back(125);
        if(kinfit_mode_ == 2) hypo_mh2.push_back(90);

        TLorentzVector b1      = TLorentzVector(jets_csv[0]->vector().px(),jets_csv[0]->vector().py(),jets_csv[0]->vector().pz(), jets_csv[0]->vector().E());
        if(bjet_regression_) b1 = TLorentzVector(jet_csv_pairs[0].second->vector().px(),jet_csv_pairs[0].second->vector().py(),jet_csv_pairs[0].second->vector().pz(),jet_csv_pairs[0].second->vector().E());
        TLorentzVector b2      = TLorentzVector(jets_csv[1]->vector().px(),jets_csv[1]->vector().py(),jets_csv[1]->vector().pz(), jets_csv[1]->vector().E());
        if(bjet_regression_) b2 = TLorentzVector(jet_csv_pairs[1].second->vector().px(),jet_csv_pairs[1].second->vector().py(),jet_csv_pairs[1].second->vector().pz(),jet_csv_pairs[1].second->vector().E());
        TLorentzVector tau1vis      = TLorentzVector(lep1->vector().px(),lep1->vector().py(),lep1->vector().pz(), lep1->vector().E());
        TLorentzVector tau2vis      = TLorentzVector(lep2->vector().px(),lep2->vector().py(),lep2->vector().pz(), lep2->vector().E());
        TLorentzVector ptmiss  = TLorentzVector(mets->vector().px(),mets->vector().py(),0,mets->vector().pt());
        TLorentzVector higgs;
        if (event->Exists("svfitHiggs")) {
          higgs = TLorentzVector(event->Get<Candidate>("svfitHiggs").vector().px(),event->Get<Candidate>("svfitHiggs").vector().py(),event->Get<Candidate>("svfitHiggs").vector().pz(),event->Get<Candidate>("svfitHiggs").vector().E());
        }
        TMatrixD metcov(2,2);
        metcov(0,0)=mets->xx_sig();
        metcov(1,0)=mets->yx_sig();
        metcov(0,1)=mets->xy_sig();
        metcov(1,1)=mets->yy_sig();
            
        //Default version of fitting using visible products plus met
        HHKinFitMaster kinFits = HHKinFitMaster(&b1,&b2,&tau1vis,&tau2vis);
        kinFits.setAdvancedBalance(&ptmiss,metcov);
        kinFits.addMh1Hypothesis(hypo_mh1);
        kinFits.addMh2Hypothesis(hypo_mh2);
        kinFits.doFullFit();
        //Best hypothesis saved. For kinfit_mode_==1 this is identical to m_H_hh (provided the cuts pull_balance_hh > 0 && convergence_hh>0 are applied)
        //since only that hypothesis is run
        m_H_best_ = kinFits.getBestMHFullFit();
        m_H_chi2_best_ = kinFits.getBestChi2FullFit();
        std::pair<Int_t,Int_t> bestHypo = kinFits.getBestHypoFullFit();
        std::map<std::pair<Int_t,Int_t>,Double_t> fit_results_chi2 = kinFits.getChi2FullFit();
        std::map<std::pair<Int_t,Int_t>,Double_t> fit_results_mH = kinFits.getMHFullFit();
        std::map<std::pair<Int_t,Int_t>,Double_t> fit_results_pull_balance = kinFits.getPullBalanceFullFit();
        std::map<std::pair<Int_t,Int_t>,Int_t> fit_convergence = kinFits.getConvergenceFullFit();
        std::pair<Int_t,Int_t> hypoZh(90,125);
        std::pair<Int_t,Int_t> hypohZ(125,90);
        std::pair<Int_t,Int_t> hypohh(125,125);
        //Save results for 125,125 hypothesis
        m_H_hh_ = fit_results_mH.at(hypohh);
        m_H_hh_chi2_ = fit_results_chi2.at(hypohh);
        pull_balance_hh_ = fit_results_pull_balance.at(hypohh);
        convergence_hh_ = fit_convergence.at(hypohh);
        
        //This variable is filled with mttbb if the event fails convergence
        m_H_hh_all_ = m_H_hh_;
        if(convergence_hh_ == -2) m_H_hh_all_ = mjj_tt_;
        
        if(bestHypo.first>0){
          pull_balance_H_best_ = fit_results_pull_balance.at(bestHypo);
          convergence_H_best_ = fit_convergence.at(bestHypo);
        } else {
          pull_balance_H_best_ = -9999;
          convergence_H_best_ = -9999;
        }

        if(kinfit_mode_==3) {
          HHDiJetKinFitMaster DiJetKinFits = HHDiJetKinFitMaster(&b1,&b2);
          DiJetKinFits.addMhHypothesis(125.0);
          DiJetKinFits.doFullFit();
          m_bb_ = (DiJetKinFits.getFitJet1() + DiJetKinFits.getFitJet2()).M();
          //m_bb_chi2_ = DiJetKinFits.GetChi2();
          //pull_balance_bb_ = fit_results_pull_balance.at(125.0);
          //convergence_bb_ = DiJetKinFits.GetConvergence();
          if (event->Exists("svfitHiggs")) {
            mbb_h_= (DiJetKinFits.getFitJet1() + DiJetKinFits.getFitJet2() + higgs ).M();
          } else {
            mbb_h_ = -9999;  
          }
        } else { 
          m_bb_ = -9999;  
          m_bb_chi2_ = -9999;  
          pull_balance_bb_ = -9999;  
          convergence_bb_ = -9999; 
          mbb_h_ = -9999;
        }
        
        //Option to additionally save results from Zh and hZ hypotheses if they have been run
        if(kinfit_mode_ == 2) {
          m_H_Zh_ = fit_results_mH.at(hypoZh);
          m_H_Zh_chi2_ = fit_results_chi2.at(hypoZh);
          pull_balance_Zh_ = fit_results_pull_balance.at(hypoZh);
          convergence_Zh_ = fit_convergence.at(hypoZh);
          m_H_hZ_ = fit_results_mH.at(hypohZ);
          m_H_hZ_chi2_ = fit_results_chi2.at(hypohZ);
          pull_balance_hZ_ = fit_results_pull_balance.at(hypohZ);
          convergence_hZ_ = fit_convergence.at(hypohZ);
        } else {
          m_H_Zh_ = -9999;
          m_H_Zh_chi2_ = -9999;
          pull_balance_Zh_ = -9999;
          convergence_Zh_ = -9999;
          m_H_hZ_ = -9999;
          m_H_hZ_chi2_ = -9999;
          pull_balance_hZ_ = -9999;
          convergence_hZ_ = -9999;
        }
      } else {
        pull_balance_H_best_=-9999;
        convergence_H_best_=-9999;
        m_H_best_ = -9999;
        m_H_chi2_best_=-9999;
        pull_balance_Zh_=-9999;
        convergence_Zh_=-9999;
        m_H_Zh_ = -9999;
        m_H_Zh_chi2_=-9999;
        pull_balance_hZ_=-9999;
        convergence_hZ_=-9999;
        m_H_hZ_ = -9999;
        m_H_hZ_chi2_ = -9999;
        pull_balance_hh_=-9999;
        convergence_hh_=-9999;
        m_H_hh_ = -9999;
        m_H_hh_all_ = -9999;
        m_H_hh_chi2_ = -9999;
        m_bb_ = -9999;  
        m_bb_chi2_ = -9999;  
        pull_balance_bb_ = -9999;  
        convergence_bb_ = -9999; 
        mbb_h_ = -9999;
      }      
    } else {
      jet_csvpt_2_ = -9999;
      jet_csvpt_bb_ = -9999;
      jet_csv_dR_ = -9999;
      jet_csveta_2_ = -9999;
      jet_csvbcsv_2_ = -9999;
      jet_csv_mjj_ = -9999;
      jet_csv_deta_ = -9999;
      jet_csv_dphi_ = -9999;
      jet_csv_dtheta_ = -9999;
      mjj_h_ = -9999;
      mjj_tt_ = -9999;
      m_H_best_ = -9999;
      m_H_chi2_best_=-9999;
      pull_balance_H_best_ = -9999;
      convergence_H_best_ = -9999;
      m_H_Zh_=-9999;
      m_H_Zh_chi2_=-9999;
      pull_balance_Zh_=-9999;
      convergence_Zh_=-9999;
      m_H_hZ_ = -9999;
      m_H_hZ_chi2_ = -9999;
      pull_balance_hZ_=-9999;
      convergence_hZ_=-9999;
      m_H_hh_ = -9999;
      m_H_hh_all_ = -9999;
      m_H_hh_chi2_ = -9999;
      pull_balance_hh_=-9999;
      convergence_hh_=-9999;
      m_bb_ = -9999;  
      m_bb_chi2_ = -9999;  
      pull_balance_bb_ = -9999;  
      convergence_bb_ = -9999; 
      mbb_h_ = -9999;
    }
    
    if (write_tree_ && fs_) outtree_->Fill();
    if (make_sync_ntuple_) synctree_->Fill();


    return 0;
  }

  int HTTCategories::PostAnalysis() {
    if(make_sync_ntuple_) {   
      lOFile->cd();
      synctree_->Write();
      lOFile->Close();
    }
    return 0;
  }

  void HTTCategories::PrintInfo() {
    ;
  }
}
