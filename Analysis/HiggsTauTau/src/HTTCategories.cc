#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/HHKinFit/include/HHKinFitMaster.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/HHKinFit/include/HHDiJetKinFitMaster.h"
#include "Utilities/interface/FnRootTools.h"

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
      optimisation_study_=false;
      tau_id_study_=false;
      is_embedded_=false;
      add_nlo_weights_=false;
      is_data_=false;
      qcd_study_=false;
      jetfake_study_=false;
      kinfit_mode_ = 0; //0 = don't run, 1 = run simple 125,125 default fit, 2 = run extra masses default fit, 3 = run m_bb only fit
      systematic_shift_ = false;
      add_Hhh_variables_ = false; //set to include custom variables for the H->hh analysis
      do_qcd_scale_wts_ = false;
      do_mssm_higgspt_ = false;
      do_sm_scale_wts_ = false;
      do_sm_ps_wts_ = false;
      do_z_weights_ = false;
      do_faketaus_ = false;
      trg_applied_in_mc_ = true;
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


    rand = new TRandom3(0);
    if (fs_ && write_tree_) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      
      outtree_->Branch("event",             &event_);
      outtree_->Branch("run",               &run_);
      outtree_->Branch("lumi",               &lumi_);
      outtree_->Branch("npu",               &n_pu_, "n_pu/F");
      outtree_->Branch("rho",               &rho_, "rho/F");
      outtree_->Branch("puweight",          &pu_weight_, "pu_weight/F");
      outtree_->Branch("wt",                &wt_.var_double);
      outtree_->Branch("wt_dysoup",         &wt_dysoup_);
      outtree_->Branch("wt_btag",           &wt_btag_);
      outtree_->Branch("trigweight_1", &trigweight_1_, "trigweight_1/F");
      outtree_->Branch("trigweight_2", &trigweight_2_, "trigweight_2/F");
      outtree_->Branch("xtrg_sf", &xtrg_sf_);
      outtree_->Branch("single_l_sf", &single_l_sf_);
      outtree_->Branch("OR_notrig", OR_notrig_);
      outtree_->Branch("xtrg_notrig", xtrg_notrig_);
      outtree_->Branch("idisoweight_1", &idisoweight_1_, "idisoweight_1/F");
      outtree_->Branch("idisoweight_2", &idisoweight_2_, "idisoweight_2/F");
      outtree_->Branch("wt_quarkmass", &wt_quarkmass_);
      outtree_->Branch("wt_quarkmass_up", &wt_quarkmass_up_);
      outtree_->Branch("wt_quarkmass_down", &wt_quarkmass_down_);
      outtree_->Branch("wt_fullquarkmass", & wt_fullquarkmass_);
      outtree_->Branch("looseiso_wt", &looseiso_wt_);
      // adding tempoary gen stuff
      outtree_->Branch("partons"     , &partons_);
      outtree_->Branch("parton_pt"     , &parton_pt_);
      outtree_->Branch("parton_pt_2"     , &parton_pt_2_);
      outtree_->Branch("parton_pt_3"     , &parton_pt_3_);
      outtree_->Branch("parton_mjj",    &parton_mjj_);
      outtree_->Branch("npNLO", &npNLO_);
      outtree_->Branch("tauFlag_1", &tauFlag_1_);
      outtree_->Branch("tauFlag_2", &tauFlag_2_);

      outtree_->Branch("ip_mag_1", &ip_mag_1_);
      outtree_->Branch("ip_mag_2", &ip_mag_2_);
      outtree_->Branch("ip_sig_1", &ip_sig_1_);
      outtree_->Branch("ip_sig_2", &ip_sig_2_);

      outtree_->Branch("q_tot_1", &q_tot_1_);
      outtree_->Branch("q_tot_2", &q_tot_2_);
      //end of temp gen stuff
      // add temporary tau trg effs
      outtree_->Branch("tau1_trgeff_mc", &tau1_trgeff_mc_);
      outtree_->Branch("tau2_trgeff_mc", &tau2_trgeff_mc_);
      if(do_sm_ps_wts_ && !systematic_shift_){

        outtree_->Branch("wt_tau_id_dm0_up", &wt_tau_id_dm0_up_);
        outtree_->Branch("wt_tau_id_dm1_up", &wt_tau_id_dm1_up_);
        outtree_->Branch("wt_tau_id_dm10_up", &wt_tau_id_dm10_up_);
        outtree_->Branch("wt_tau_id_dm11_up"  , &wt_tau_id_dm11_up_);  
        outtree_->Branch("wt_tau_id_dm0_down", &wt_tau_id_dm0_down_);
        outtree_->Branch("wt_tau_id_dm1_down", &wt_tau_id_dm1_down_);
        outtree_->Branch("wt_tau_id_dm10_down", &wt_tau_id_dm10_down_);
        outtree_->Branch("wt_tau_id_dm11_down", &wt_tau_id_dm11_down_);
        outtree_->Branch("wt_tau_id_pt_bin1_up", &wt_tau_id_pt_bin1_up_);
        outtree_->Branch("wt_tau_id_pt_bin2_up", &wt_tau_id_pt_bin2_up_);
        outtree_->Branch("wt_tau_id_pt_bin3_up", &wt_tau_id_pt_bin3_up_);
        outtree_->Branch("wt_tau_id_pt_bin4_up", &wt_tau_id_pt_bin4_up_);
        outtree_->Branch("wt_tau_id_pt_bin5_up", &wt_tau_id_pt_bin5_up_);
        outtree_->Branch("wt_tau_id_pt_bin1_down", &wt_tau_id_pt_bin1_down_);
        outtree_->Branch("wt_tau_id_pt_bin2_down", &wt_tau_id_pt_bin2_down_);
        outtree_->Branch("wt_tau_id_pt_bin3_down", &wt_tau_id_pt_bin3_down_);
        outtree_->Branch("wt_tau_id_pt_bin4_down", &wt_tau_id_pt_bin4_down_);
        outtree_->Branch("wt_tau_id_pt_bin5_down", &wt_tau_id_pt_bin5_down_);
        outtree_->Branch("wt_tau_trg_dm0_up", &wt_tau_trg_dm0_up_);
        outtree_->Branch("wt_tau_trg_dm1_up", &wt_tau_trg_dm1_up_);
        outtree_->Branch("wt_tau_trg_dm10_up", &wt_tau_trg_dm10_up_);
        outtree_->Branch("wt_tau_trg_dm11_up", &wt_tau_trg_dm11_up_);
        outtree_->Branch("wt_tau_trg_dm0_down", &wt_tau_trg_dm0_down_);
        outtree_->Branch("wt_tau_trg_dm1_down", &wt_tau_trg_dm1_down_);
        outtree_->Branch("wt_tau_trg_dm10_down", &wt_tau_trg_dm10_down_);
        outtree_->Branch("wt_tau_trg_dm11_down", &wt_tau_trg_dm11_down_);

        outtree_->Branch("wt_ps_up", & wt_ps_up_);
        outtree_->Branch("wt_ps_down", & wt_ps_down_);
        outtree_->Branch("wt_ue_up", & wt_ue_up_);
        outtree_->Branch("wt_ue_down", & wt_ue_down_);
      }
     
      if(strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        outtree_->Branch("wt_prefire", &wt_prefire_);
        outtree_->Branch("wt_prefire_up", &wt_prefire_up_);
        outtree_->Branch("wt_prefire_down", &wt_prefire_down_);
      }
  
      if(do_mssm_higgspt_){
        outtree_->Branch("wt_ggh_t", &wt_ggh_t_);
        outtree_->Branch("wt_ggh_b", &wt_ggh_b_);
        outtree_->Branch("wt_ggh_i", &wt_ggh_i_);
        outtree_->Branch("wt_ggH_t", &wt_ggH_t_);
        outtree_->Branch("wt_ggH_b", &wt_ggH_b_);
        outtree_->Branch("wt_ggH_i", &wt_ggH_i_);
        outtree_->Branch("wt_ggA_t", &wt_ggA_t_);
        outtree_->Branch("wt_ggA_b", &wt_ggA_b_);
        outtree_->Branch("wt_ggA_i", &wt_ggA_i_);    
      }
      if(channel_==channel::em){
        outtree_->Branch("idisoweight_up_1",&idisoweight_up_1_);
        outtree_->Branch("idisoweight_up_2",&idisoweight_up_2_);
        outtree_->Branch("idisoweight_down_1",&idisoweight_down_1_);
        outtree_->Branch("idisoweight_down_2",&idisoweight_down_2_);
      }
      if(add_nlo_weights_) {
        outtree_->Branch("wt_nlo_pt",         &wt_nlo_pt_);
        outtree_->Branch("nlo_pt",            &nlo_pt_);
      }
      
      // fake-factor weights
      if (do_ff_weights_ && (channel_ == channel::et || channel_ == channel::mt || channel_ == channel::tt)){
        if(strategy_ == strategy::mssmsummer16) {  
          if (channel_ == channel::tt ){
            outtree_->Branch("wt_ff_inclusive_1",    &ff_weight_inclusive_);
            outtree_->Branch("wt_ff_inclusive_2",    &ff_weight_inclusive_2_);
            outtree_->Branch("wt_ff_nobtag_1",    &ff_weight_nobtag_);
            outtree_->Branch("wt_ff_nobtag_2",    &ff_weight_nobtag_2_);
            outtree_->Branch("wt_ff_btag_1",    &ff_weight_btag_);
            outtree_->Branch("wt_ff_btag_2",    &ff_weight_btag_2_);
          } else{
            outtree_->Branch("wt_ff_inclusive",    &ff_weight_inclusive_);
            outtree_->Branch("wt_ff_nobtag_tight",    &ff_weight_nobtag_tight_);
            outtree_->Branch("wt_ff_nobtag_loosemt",    &ff_weight_nobtag_loosemt_);
            outtree_->Branch("wt_ff_nobtag_looseiso",    &ff_weight_nobtag_looseiso_);
            outtree_->Branch("wt_ff_btag_tight",    &ff_weight_btag_tight_);
            outtree_->Branch("wt_ff_btag_loosemt",    &ff_weight_btag_loosemt_);
            outtree_->Branch("wt_ff_btag_looseiso",    &ff_weight_btag_looseiso_);
          }
            
          if(do_ff_systematics_){
            if(channel_ == channel::et || channel_ == channel::mt){
              outtree_->Branch("wt_ff_inclusive_qcd_syst_up"            , &wt_ff_inclusive_qcd_syst_up_             );
              outtree_->Branch("wt_ff_inclusive_qcd_syst_down"          , &wt_ff_inclusive_qcd_syst_down_           );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet0_stat_up"  , &wt_ff_inclusive_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet0_stat_down", &wt_ff_inclusive_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet1_stat_up"  , &wt_ff_inclusive_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet1_stat_down", &wt_ff_inclusive_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet0_stat_up"  , &wt_ff_inclusive_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet0_stat_down", &wt_ff_inclusive_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet1_stat_up"  , &wt_ff_inclusive_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet1_stat_down", &wt_ff_inclusive_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_inclusive_w_syst_up"              , &wt_ff_inclusive_w_syst_up_               );
              outtree_->Branch("wt_ff_inclusive_w_syst_down"            , &wt_ff_inclusive_w_syst_down_             );
              outtree_->Branch("wt_ff_inclusive_w_dm0_njet0_stat_up"    , &wt_ff_inclusive_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_inclusive_w_dm0_njet0_stat_down"  , &wt_ff_inclusive_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_inclusive_w_dm0_njet1_stat_up"    , &wt_ff_inclusive_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_inclusive_w_dm0_njet1_stat_down"  , &wt_ff_inclusive_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_inclusive_w_dm1_njet0_stat_up"    , &wt_ff_inclusive_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_inclusive_w_dm1_njet0_stat_down"  , &wt_ff_inclusive_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_inclusive_w_dm1_njet1_stat_up"    , &wt_ff_inclusive_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_inclusive_w_dm1_njet1_stat_down"  , &wt_ff_inclusive_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_inclusive_tt_syst_up"             , &wt_ff_inclusive_tt_syst_up_              );
              outtree_->Branch("wt_ff_inclusive_tt_syst_down"           , &wt_ff_inclusive_tt_syst_down_            );
              outtree_->Branch("wt_ff_inclusive_tt_dm0_njet0_stat_up"   , &wt_ff_inclusive_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_inclusive_tt_dm0_njet0_stat_down" , &wt_ff_inclusive_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_inclusive_tt_dm0_njet1_stat_up"   , &wt_ff_inclusive_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_inclusive_tt_dm0_njet1_stat_down" , &wt_ff_inclusive_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_inclusive_tt_dm1_njet0_stat_up"   , &wt_ff_inclusive_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_inclusive_tt_dm1_njet0_stat_down" , &wt_ff_inclusive_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_inclusive_tt_dm1_njet1_stat_up"   , &wt_ff_inclusive_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_inclusive_tt_dm1_njet1_stat_down" , &wt_ff_inclusive_tt_dm1_njet1_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_qcd_syst_up"            , &wt_ff_nobtag_qcd_syst_up_             );
              outtree_->Branch("wt_ff_nobtag_qcd_syst_down"          , &wt_ff_nobtag_qcd_syst_down_           );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet0_stat_up"  , &wt_ff_nobtag_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet0_stat_down", &wt_ff_nobtag_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet1_stat_up"  , &wt_ff_nobtag_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet1_stat_down", &wt_ff_nobtag_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet0_stat_up"  , &wt_ff_nobtag_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet0_stat_down", &wt_ff_nobtag_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet1_stat_up"  , &wt_ff_nobtag_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet1_stat_down", &wt_ff_nobtag_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_w_syst_up"              , &wt_ff_nobtag_w_syst_up_               );
              outtree_->Branch("wt_ff_nobtag_w_syst_down"            , &wt_ff_nobtag_w_syst_down_             );
              outtree_->Branch("wt_ff_nobtag_w_dm0_njet0_stat_up"    , &wt_ff_nobtag_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_w_dm0_njet0_stat_down"  , &wt_ff_nobtag_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_w_dm0_njet1_stat_up"    , &wt_ff_nobtag_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_w_dm0_njet1_stat_down"  , &wt_ff_nobtag_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_w_dm1_njet0_stat_up"    , &wt_ff_nobtag_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_w_dm1_njet0_stat_down"  , &wt_ff_nobtag_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_w_dm1_njet1_stat_up"    , &wt_ff_nobtag_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_w_dm1_njet1_stat_down"  , &wt_ff_nobtag_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_tt_syst_up"             , &wt_ff_nobtag_tt_syst_up_              );
              outtree_->Branch("wt_ff_nobtag_tt_syst_down"           , &wt_ff_nobtag_tt_syst_down_            );
              outtree_->Branch("wt_ff_nobtag_tt_dm0_njet0_stat_up"   , &wt_ff_nobtag_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tt_dm0_njet0_stat_down" , &wt_ff_nobtag_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_tt_dm0_njet1_stat_up"   , &wt_ff_nobtag_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tt_dm0_njet1_stat_down" , &wt_ff_nobtag_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_tt_dm1_njet0_stat_up"   , &wt_ff_nobtag_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tt_dm1_njet0_stat_down" , &wt_ff_nobtag_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_tt_dm1_njet1_stat_up"   , &wt_ff_nobtag_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tt_dm1_njet1_stat_down" , &wt_ff_nobtag_tt_dm1_njet1_stat_down_  ); 
              outtree_->Branch("wt_ff_btag_qcd_syst_up"            , &wt_ff_btag_qcd_syst_up_             );
              outtree_->Branch("wt_ff_btag_qcd_syst_down"          , &wt_ff_btag_qcd_syst_down_           );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet0_stat_up"  , &wt_ff_btag_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet0_stat_down", &wt_ff_btag_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet1_stat_up"  , &wt_ff_btag_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet1_stat_down", &wt_ff_btag_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet0_stat_up"  , &wt_ff_btag_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet0_stat_down", &wt_ff_btag_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet1_stat_up"  , &wt_ff_btag_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet1_stat_down", &wt_ff_btag_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_w_syst_up"              , &wt_ff_btag_w_syst_up_               );
              outtree_->Branch("wt_ff_btag_w_syst_down"            , &wt_ff_btag_w_syst_down_             );
              outtree_->Branch("wt_ff_btag_w_dm0_njet0_stat_up"    , &wt_ff_btag_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_w_dm0_njet0_stat_down"  , &wt_ff_btag_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_w_dm0_njet1_stat_up"    , &wt_ff_btag_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_w_dm0_njet1_stat_down"  , &wt_ff_btag_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_w_dm1_njet0_stat_up"    , &wt_ff_btag_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_w_dm1_njet0_stat_down"  , &wt_ff_btag_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_w_dm1_njet1_stat_up"    , &wt_ff_btag_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_w_dm1_njet1_stat_down"  , &wt_ff_btag_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_tt_syst_up"             , &wt_ff_btag_tt_syst_up_              );
              outtree_->Branch("wt_ff_btag_tt_syst_down"           , &wt_ff_btag_tt_syst_down_            );
              outtree_->Branch("wt_ff_btag_tt_dm0_njet0_stat_up"   , &wt_ff_btag_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_tt_dm0_njet0_stat_down" , &wt_ff_btag_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_tt_dm0_njet1_stat_up"   , &wt_ff_btag_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_tt_dm0_njet1_stat_down" , &wt_ff_btag_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_btag_tt_dm1_njet0_stat_up"   , &wt_ff_btag_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_tt_dm1_njet0_stat_down" , &wt_ff_btag_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_tt_dm1_njet1_stat_up"   , &wt_ff_btag_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_tt_dm1_njet1_stat_down" , &wt_ff_btag_tt_dm1_njet1_stat_down_  );
              
              outtree_->Branch("wt_ff_nobtag_tight_qcd_syst_up"            , &wt_ff_nobtag_tight_qcd_syst_up_             );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_syst_down"          , &wt_ff_nobtag_tight_qcd_syst_down_           );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm0_njet0_stat_up"  , &wt_ff_nobtag_tight_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm0_njet0_stat_down", &wt_ff_nobtag_tight_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm0_njet1_stat_up"  , &wt_ff_nobtag_tight_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm0_njet1_stat_down", &wt_ff_nobtag_tight_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm1_njet0_stat_up"  , &wt_ff_nobtag_tight_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm1_njet0_stat_down", &wt_ff_nobtag_tight_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm1_njet1_stat_up"  , &wt_ff_nobtag_tight_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_tight_qcd_dm1_njet1_stat_down", &wt_ff_nobtag_tight_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_tight_w_syst_up"              , &wt_ff_nobtag_tight_w_syst_up_               );
              outtree_->Branch("wt_ff_nobtag_tight_w_syst_down"            , &wt_ff_nobtag_tight_w_syst_down_             );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm0_njet0_stat_up"    , &wt_ff_nobtag_tight_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm0_njet0_stat_down"  , &wt_ff_nobtag_tight_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm0_njet1_stat_up"    , &wt_ff_nobtag_tight_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm0_njet1_stat_down"  , &wt_ff_nobtag_tight_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm1_njet0_stat_up"    , &wt_ff_nobtag_tight_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm1_njet0_stat_down"  , &wt_ff_nobtag_tight_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm1_njet1_stat_up"    , &wt_ff_nobtag_tight_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_tight_w_dm1_njet1_stat_down"  , &wt_ff_nobtag_tight_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_tight_tt_syst_up"             , &wt_ff_nobtag_tight_tt_syst_up_              );
              outtree_->Branch("wt_ff_nobtag_tight_tt_syst_down"           , &wt_ff_nobtag_tight_tt_syst_down_            );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm0_njet0_stat_up"   , &wt_ff_nobtag_tight_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm0_njet0_stat_down" , &wt_ff_nobtag_tight_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm0_njet1_stat_up"   , &wt_ff_nobtag_tight_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm0_njet1_stat_down" , &wt_ff_nobtag_tight_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm1_njet0_stat_up"   , &wt_ff_nobtag_tight_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm1_njet0_stat_down" , &wt_ff_nobtag_tight_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm1_njet1_stat_up"   , &wt_ff_nobtag_tight_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_tight_tt_dm1_njet1_stat_down" , &wt_ff_nobtag_tight_tt_dm1_njet1_stat_down_  ); 
              outtree_->Branch("wt_ff_btag_tight_qcd_syst_up"            , &wt_ff_btag_tight_qcd_syst_up_             );
              outtree_->Branch("wt_ff_btag_tight_qcd_syst_down"          , &wt_ff_btag_tight_qcd_syst_down_           );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm0_njet0_stat_up"  , &wt_ff_btag_tight_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm0_njet0_stat_down", &wt_ff_btag_tight_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm0_njet1_stat_up"  , &wt_ff_btag_tight_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm0_njet1_stat_down", &wt_ff_btag_tight_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm1_njet0_stat_up"  , &wt_ff_btag_tight_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm1_njet0_stat_down", &wt_ff_btag_tight_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm1_njet1_stat_up"  , &wt_ff_btag_tight_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_tight_qcd_dm1_njet1_stat_down", &wt_ff_btag_tight_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_tight_w_syst_up"              , &wt_ff_btag_tight_w_syst_up_               );
              outtree_->Branch("wt_ff_btag_tight_w_syst_down"            , &wt_ff_btag_tight_w_syst_down_             );
              outtree_->Branch("wt_ff_btag_tight_w_dm0_njet0_stat_up"    , &wt_ff_btag_tight_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_tight_w_dm0_njet0_stat_down"  , &wt_ff_btag_tight_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_tight_w_dm0_njet1_stat_up"    , &wt_ff_btag_tight_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_tight_w_dm0_njet1_stat_down"  , &wt_ff_btag_tight_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_tight_w_dm1_njet0_stat_up"    , &wt_ff_btag_tight_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_tight_w_dm1_njet0_stat_down"  , &wt_ff_btag_tight_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_tight_w_dm1_njet1_stat_up"    , &wt_ff_btag_tight_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_tight_w_dm1_njet1_stat_down"  , &wt_ff_btag_tight_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_tight_tt_syst_up"             , &wt_ff_btag_tight_tt_syst_up_              );
              outtree_->Branch("wt_ff_btag_tight_tt_syst_down"           , &wt_ff_btag_tight_tt_syst_down_            );
              outtree_->Branch("wt_ff_btag_tight_tt_dm0_njet0_stat_up"   , &wt_ff_btag_tight_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_tight_tt_dm0_njet0_stat_down" , &wt_ff_btag_tight_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_tight_tt_dm0_njet1_stat_up"   , &wt_ff_btag_tight_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_tight_tt_dm0_njet1_stat_down" , &wt_ff_btag_tight_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_btag_tight_tt_dm1_njet0_stat_up"   , &wt_ff_btag_tight_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_tight_tt_dm1_njet0_stat_down" , &wt_ff_btag_tight_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_tight_tt_dm1_njet1_stat_up"   , &wt_ff_btag_tight_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_tight_tt_dm1_njet1_stat_down" , &wt_ff_btag_tight_tt_dm1_njet1_stat_down_  ); 
              
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_syst_up"            , &wt_ff_nobtag_loosemt_qcd_syst_up_             );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_syst_down"          , &wt_ff_nobtag_loosemt_qcd_syst_down_           );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm0_njet0_stat_up"  , &wt_ff_nobtag_loosemt_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm0_njet0_stat_down", &wt_ff_nobtag_loosemt_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm0_njet1_stat_up"  , &wt_ff_nobtag_loosemt_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm0_njet1_stat_down", &wt_ff_nobtag_loosemt_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm1_njet0_stat_up"  , &wt_ff_nobtag_loosemt_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm1_njet0_stat_down", &wt_ff_nobtag_loosemt_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm1_njet1_stat_up"  , &wt_ff_nobtag_loosemt_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_qcd_dm1_njet1_stat_down", &wt_ff_nobtag_loosemt_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_syst_up"              , &wt_ff_nobtag_loosemt_w_syst_up_               );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_syst_down"            , &wt_ff_nobtag_loosemt_w_syst_down_             );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm0_njet0_stat_up"    , &wt_ff_nobtag_loosemt_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm0_njet0_stat_down"  , &wt_ff_nobtag_loosemt_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm0_njet1_stat_up"    , &wt_ff_nobtag_loosemt_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm0_njet1_stat_down"  , &wt_ff_nobtag_loosemt_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm1_njet0_stat_up"    , &wt_ff_nobtag_loosemt_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm1_njet0_stat_down"  , &wt_ff_nobtag_loosemt_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm1_njet1_stat_up"    , &wt_ff_nobtag_loosemt_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_loosemt_w_dm1_njet1_stat_down"  , &wt_ff_nobtag_loosemt_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_syst_up"             , &wt_ff_nobtag_loosemt_tt_syst_up_              );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_syst_down"           , &wt_ff_nobtag_loosemt_tt_syst_down_            );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm0_njet0_stat_up"   , &wt_ff_nobtag_loosemt_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm0_njet0_stat_down" , &wt_ff_nobtag_loosemt_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm0_njet1_stat_up"   , &wt_ff_nobtag_loosemt_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm0_njet1_stat_down" , &wt_ff_nobtag_loosemt_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm1_njet0_stat_up"   , &wt_ff_nobtag_loosemt_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm1_njet0_stat_down" , &wt_ff_nobtag_loosemt_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm1_njet1_stat_up"   , &wt_ff_nobtag_loosemt_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_loosemt_tt_dm1_njet1_stat_down" , &wt_ff_nobtag_loosemt_tt_dm1_njet1_stat_down_  ); 
              outtree_->Branch("wt_ff_btag_loosemt_qcd_syst_up"            , &wt_ff_btag_loosemt_qcd_syst_up_             );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_syst_down"          , &wt_ff_btag_loosemt_qcd_syst_down_           );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm0_njet0_stat_up"  , &wt_ff_btag_loosemt_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm0_njet0_stat_down", &wt_ff_btag_loosemt_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm0_njet1_stat_up"  , &wt_ff_btag_loosemt_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm0_njet1_stat_down", &wt_ff_btag_loosemt_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm1_njet0_stat_up"  , &wt_ff_btag_loosemt_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm1_njet0_stat_down", &wt_ff_btag_loosemt_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm1_njet1_stat_up"  , &wt_ff_btag_loosemt_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_loosemt_qcd_dm1_njet1_stat_down", &wt_ff_btag_loosemt_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_loosemt_w_syst_up"              , &wt_ff_btag_loosemt_w_syst_up_               );
              outtree_->Branch("wt_ff_btag_loosemt_w_syst_down"            , &wt_ff_btag_loosemt_w_syst_down_             );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm0_njet0_stat_up"    , &wt_ff_btag_loosemt_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm0_njet0_stat_down"  , &wt_ff_btag_loosemt_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm0_njet1_stat_up"    , &wt_ff_btag_loosemt_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm0_njet1_stat_down"  , &wt_ff_btag_loosemt_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm1_njet0_stat_up"    , &wt_ff_btag_loosemt_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm1_njet0_stat_down"  , &wt_ff_btag_loosemt_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm1_njet1_stat_up"    , &wt_ff_btag_loosemt_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_loosemt_w_dm1_njet1_stat_down"  , &wt_ff_btag_loosemt_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_loosemt_tt_syst_up"             , &wt_ff_btag_loosemt_tt_syst_up_              );
              outtree_->Branch("wt_ff_btag_loosemt_tt_syst_down"           , &wt_ff_btag_loosemt_tt_syst_down_            );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm0_njet0_stat_up"   , &wt_ff_btag_loosemt_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm0_njet0_stat_down" , &wt_ff_btag_loosemt_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm0_njet1_stat_up"   , &wt_ff_btag_loosemt_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm0_njet1_stat_down" , &wt_ff_btag_loosemt_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm1_njet0_stat_up"   , &wt_ff_btag_loosemt_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm1_njet0_stat_down" , &wt_ff_btag_loosemt_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm1_njet1_stat_up"   , &wt_ff_btag_loosemt_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_loosemt_tt_dm1_njet1_stat_down" , &wt_ff_btag_loosemt_tt_dm1_njet1_stat_down_  ); 
              
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_syst_up"            , &wt_ff_nobtag_looseiso_qcd_syst_up_             );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_syst_down"          , &wt_ff_nobtag_looseiso_qcd_syst_down_           );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm0_njet0_stat_up"  , &wt_ff_nobtag_looseiso_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm0_njet0_stat_down", &wt_ff_nobtag_looseiso_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm0_njet1_stat_up"  , &wt_ff_nobtag_looseiso_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm0_njet1_stat_down", &wt_ff_nobtag_looseiso_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm1_njet0_stat_up"  , &wt_ff_nobtag_looseiso_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm1_njet0_stat_down", &wt_ff_nobtag_looseiso_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm1_njet1_stat_up"  , &wt_ff_nobtag_looseiso_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_qcd_dm1_njet1_stat_down", &wt_ff_nobtag_looseiso_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_syst_up"              , &wt_ff_nobtag_looseiso_w_syst_up_               );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_syst_down"            , &wt_ff_nobtag_looseiso_w_syst_down_             );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm0_njet0_stat_up"    , &wt_ff_nobtag_looseiso_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm0_njet0_stat_down"  , &wt_ff_nobtag_looseiso_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm0_njet1_stat_up"    , &wt_ff_nobtag_looseiso_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm0_njet1_stat_down"  , &wt_ff_nobtag_looseiso_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm1_njet0_stat_up"    , &wt_ff_nobtag_looseiso_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm1_njet0_stat_down"  , &wt_ff_nobtag_looseiso_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm1_njet1_stat_up"    , &wt_ff_nobtag_looseiso_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_nobtag_looseiso_w_dm1_njet1_stat_down"  , &wt_ff_nobtag_looseiso_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_syst_up"             , &wt_ff_nobtag_looseiso_tt_syst_up_              );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_syst_down"           , &wt_ff_nobtag_looseiso_tt_syst_down_            );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm0_njet0_stat_up"   , &wt_ff_nobtag_looseiso_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm0_njet0_stat_down" , &wt_ff_nobtag_looseiso_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm0_njet1_stat_up"   , &wt_ff_nobtag_looseiso_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm0_njet1_stat_down" , &wt_ff_nobtag_looseiso_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm1_njet0_stat_up"   , &wt_ff_nobtag_looseiso_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm1_njet0_stat_down" , &wt_ff_nobtag_looseiso_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm1_njet1_stat_up"   , &wt_ff_nobtag_looseiso_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_nobtag_looseiso_tt_dm1_njet1_stat_down" , &wt_ff_nobtag_looseiso_tt_dm1_njet1_stat_down_  ); 
              outtree_->Branch("wt_ff_btag_looseiso_qcd_syst_up"            , &wt_ff_btag_looseiso_qcd_syst_up_             );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_syst_down"          , &wt_ff_btag_looseiso_qcd_syst_down_           );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm0_njet0_stat_up"  , &wt_ff_btag_looseiso_qcd_dm0_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm0_njet0_stat_down", &wt_ff_btag_looseiso_qcd_dm0_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm0_njet1_stat_up"  , &wt_ff_btag_looseiso_qcd_dm0_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm0_njet1_stat_down", &wt_ff_btag_looseiso_qcd_dm0_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm1_njet0_stat_up"  , &wt_ff_btag_looseiso_qcd_dm1_njet0_stat_up_   );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm1_njet0_stat_down", &wt_ff_btag_looseiso_qcd_dm1_njet0_stat_down_ );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm1_njet1_stat_up"  , &wt_ff_btag_looseiso_qcd_dm1_njet1_stat_up_   );
              outtree_->Branch("wt_ff_btag_looseiso_qcd_dm1_njet1_stat_down", &wt_ff_btag_looseiso_qcd_dm1_njet1_stat_down_ );
              outtree_->Branch("wt_ff_btag_looseiso_w_syst_up"              , &wt_ff_btag_looseiso_w_syst_up_               );
              outtree_->Branch("wt_ff_btag_looseiso_w_syst_down"            , &wt_ff_btag_looseiso_w_syst_down_             );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm0_njet0_stat_up"    , &wt_ff_btag_looseiso_w_dm0_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm0_njet0_stat_down"  , &wt_ff_btag_looseiso_w_dm0_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm0_njet1_stat_up"    , &wt_ff_btag_looseiso_w_dm0_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm0_njet1_stat_down"  , &wt_ff_btag_looseiso_w_dm0_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm1_njet0_stat_up"    , &wt_ff_btag_looseiso_w_dm1_njet0_stat_up_     );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm1_njet0_stat_down"  , &wt_ff_btag_looseiso_w_dm1_njet0_stat_down_   );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm1_njet1_stat_up"    , &wt_ff_btag_looseiso_w_dm1_njet1_stat_up_     );
              outtree_->Branch("wt_ff_btag_looseiso_w_dm1_njet1_stat_down"  , &wt_ff_btag_looseiso_w_dm1_njet1_stat_down_   );
              outtree_->Branch("wt_ff_btag_looseiso_tt_syst_up"             , &wt_ff_btag_looseiso_tt_syst_up_              );
              outtree_->Branch("wt_ff_btag_looseiso_tt_syst_down"           , &wt_ff_btag_looseiso_tt_syst_down_            );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm0_njet0_stat_up"   , &wt_ff_btag_looseiso_tt_dm0_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm0_njet0_stat_down" , &wt_ff_btag_looseiso_tt_dm0_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm0_njet1_stat_up"   , &wt_ff_btag_looseiso_tt_dm0_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm0_njet1_stat_down" , &wt_ff_btag_looseiso_tt_dm0_njet1_stat_down_  );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm1_njet0_stat_up"   , &wt_ff_btag_looseiso_tt_dm1_njet0_stat_up_    );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm1_njet0_stat_down" , &wt_ff_btag_looseiso_tt_dm1_njet0_stat_down_  );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm1_njet1_stat_up"   , &wt_ff_btag_looseiso_tt_dm1_njet1_stat_up_    );
              outtree_->Branch("wt_ff_btag_looseiso_tt_dm1_njet1_stat_down" , &wt_ff_btag_looseiso_tt_dm1_njet1_stat_down_  ); 
            } else if (channel_ == channel::tt){
              
              outtree_->Branch("wt_ff_inclusive_qcd_syst_up_1"  , &wt_inclusive_ff_qcd_syst_up_1               );
              outtree_->Branch("wt_ff_inclusive_qcd_syst_down_1"  , &wt_inclusive_ff_qcd_syst_down_1             );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet0_stat_up_1"  , &wt_inclusive_ff_qcd_dm0_njet0_stat_up_1     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet0_stat_down_1"  , &wt_inclusive_ff_qcd_dm0_njet0_stat_down_1   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet1_stat_up_1"  , &wt_inclusive_ff_qcd_dm0_njet1_stat_up_1     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet1_stat_down_1"  , &wt_inclusive_ff_qcd_dm0_njet1_stat_down_1   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet0_stat_up_1"  , &wt_inclusive_ff_qcd_dm1_njet0_stat_up_1     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet0_stat_down_1"  , &wt_inclusive_ff_qcd_dm1_njet0_stat_down_1   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet1_stat_up_1"  , &wt_inclusive_ff_qcd_dm1_njet1_stat_up_1     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet1_stat_down_1"  , &wt_inclusive_ff_qcd_dm1_njet1_stat_down_1   );
              outtree_->Branch("wt_ff_inclusive_w_syst_up_1"  , &wt_inclusive_ff_w_syst_up_1                 );
              outtree_->Branch("wt_ff_inclusive_w_syst_down_1"  , &wt_inclusive_ff_w_syst_down_1               );
              outtree_->Branch("wt_ff_inclusive_tt_syst_up_1"  , &wt_inclusive_ff_tt_syst_up_1                );
              outtree_->Branch("wt_ff_inclusive_tt_syst_down_1"  , &wt_inclusive_ff_tt_syst_down_1              );
              outtree_->Branch("wt_ff_inclusive_w_frac_syst_up_1"  , &wt_inclusive_ff_w_frac_syst_up_1            );
              outtree_->Branch("wt_ff_inclusive_w_frac_syst_down_1"  , &wt_inclusive_ff_w_frac_syst_down_1          );
              outtree_->Branch("wt_ff_inclusive_tt_frac_syst_up_1"  , &wt_inclusive_ff_tt_frac_syst_up_1           );
              outtree_->Branch("wt_ff_inclusive_tt_frac_syst_down_1"  , &wt_inclusive_ff_tt_frac_syst_down_1         );
              outtree_->Branch("wt_ff_inclusive_dy_frac_syst_up_1"  , &wt_inclusive_ff_dy_frac_syst_up_1           );
              outtree_->Branch("wt_ff_inclusive_dy_frac_syst_down_1"  , &wt_inclusive_ff_dy_frac_syst_down_1         );
              outtree_->Branch("wt_ff_nobtag_qcd_syst_up_1"  , &wt_nobtag_ff_qcd_syst_up_1                  );
              outtree_->Branch("wt_ff_nobtag_qcd_syst_down_1"  , &wt_nobtag_ff_qcd_syst_down_1                );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet0_stat_up_1"  , &wt_nobtag_ff_qcd_dm0_njet0_stat_up_1        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet0_stat_down_1"  , &wt_nobtag_ff_qcd_dm0_njet0_stat_down_1      );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet1_stat_up_1"  , &wt_nobtag_ff_qcd_dm0_njet1_stat_up_1        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet1_stat_down_1"  , &wt_nobtag_ff_qcd_dm0_njet1_stat_down_1      );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet0_stat_up_1"  , &wt_nobtag_ff_qcd_dm1_njet0_stat_up_1        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet0_stat_down_1"  , &wt_nobtag_ff_qcd_dm1_njet0_stat_down_1      );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet1_stat_up_1"  , &wt_nobtag_ff_qcd_dm1_njet1_stat_up_1        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet1_stat_down_1"  , &wt_nobtag_ff_qcd_dm1_njet1_stat_down_1      );
              outtree_->Branch("wt_ff_nobtag_w_syst_up_1"  , &wt_nobtag_ff_w_syst_up_1                    );
              outtree_->Branch("wt_ff_nobtag_w_syst_down_1"  , &wt_nobtag_ff_w_syst_down_1                  );
              outtree_->Branch("wt_ff_nobtag_tt_syst_up_1"  , &wt_nobtag_ff_tt_syst_up_1                   );
              outtree_->Branch("wt_ff_nobtag_tt_syst_down_1"  , &wt_nobtag_ff_tt_syst_down_1                 );
              outtree_->Branch("wt_ff_nobtag_w_frac_syst_up_1"  , &wt_nobtag_ff_w_frac_syst_up_1               );
              outtree_->Branch("wt_ff_nobtag_w_frac_syst_down_1"  , &wt_nobtag_ff_w_frac_syst_down_1             );
              outtree_->Branch("wt_ff_nobtag_tt_frac_syst_up_1"  , &wt_nobtag_ff_tt_frac_syst_up_1              );
              outtree_->Branch("wt_ff_nobtag_tt_frac_syst_down_1"  , &wt_nobtag_ff_tt_frac_syst_down_1            );
              outtree_->Branch("wt_ff_nobtag_dy_frac_syst_up_1"  , &wt_nobtag_ff_dy_frac_syst_up_1              );
              outtree_->Branch("wt_ff_nobtag_dy_frac_syst_down_1"  , &wt_nobtag_ff_dy_frac_syst_down_1            );
              outtree_->Branch("wt_ff_btag_qcd_syst_up_1"  , &wt_btag_ff_qcd_syst_up_1                    );
              outtree_->Branch("wt_ff_btag_qcd_syst_down_1"  , &wt_btag_ff_qcd_syst_down_1                  );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet0_stat_up_1"  , &wt_btag_ff_qcd_dm0_njet0_stat_up_1          );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet0_stat_down_1"  , &wt_btag_ff_qcd_dm0_njet0_stat_down_1        );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet1_stat_up_1"  , &wt_btag_ff_qcd_dm0_njet1_stat_up_1          );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet1_stat_down_1"  , &wt_btag_ff_qcd_dm0_njet1_stat_down_1        );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet0_stat_up_1"  , &wt_btag_ff_qcd_dm1_njet0_stat_up_1          );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet0_stat_down_1"  , &wt_btag_ff_qcd_dm1_njet0_stat_down_1        );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet1_stat_up_1"  , &wt_btag_ff_qcd_dm1_njet1_stat_up_1          );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet1_stat_down_1"  , &wt_btag_ff_qcd_dm1_njet1_stat_down_1        );
              outtree_->Branch("wt_ff_btag_w_syst_up_1"  , &wt_btag_ff_w_syst_up_1                      );
              outtree_->Branch("wt_ff_btag_w_syst_down_1"  , &wt_btag_ff_w_syst_down_1                    );
              outtree_->Branch("wt_ff_btag_tt_syst_up_1"  , &wt_btag_ff_tt_syst_up_1                     );
              outtree_->Branch("wt_ff_btag_tt_syst_down_1"  , &wt_btag_ff_tt_syst_down_1                   );
              outtree_->Branch("wt_ff_btag_w_frac_syst_up_1"  , &wt_btag_ff_w_frac_syst_up_1                 );
              outtree_->Branch("wt_ff_btag_w_frac_syst_down_1"  , &wt_btag_ff_w_frac_syst_down_1               );
              outtree_->Branch("wt_ff_btag_tt_frac_syst_up_1"  , &wt_btag_ff_tt_frac_syst_up_1                );
              outtree_->Branch("wt_ff_btag_tt_frac_syst_down_1"  , &wt_btag_ff_tt_frac_syst_down_1              );
              outtree_->Branch("wt_ff_btag_dy_frac_syst_up_1"  , &wt_btag_ff_dy_frac_syst_up_1                );
              outtree_->Branch("wt_ff_btag_dy_frac_syst_down_1"  , &wt_btag_ff_dy_frac_syst_down_1              );
              outtree_->Branch("wt_ff_inclusive_qcd_syst_up_2"  , &wt_inclusive_ff_qcd_syst_up_2               );
              outtree_->Branch("wt_ff_inclusive_qcd_syst_down_2"  , &wt_inclusive_ff_qcd_syst_down_2             );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet0_stat_up_2"  , &wt_inclusive_ff_qcd_dm0_njet0_stat_up_2     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet0_stat_down_2"  , &wt_inclusive_ff_qcd_dm0_njet0_stat_down_2   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet1_stat_up_2"  , &wt_inclusive_ff_qcd_dm0_njet1_stat_up_2     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm0_njet1_stat_down_2"  , &wt_inclusive_ff_qcd_dm0_njet1_stat_down_2   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet0_stat_up_2"  , &wt_inclusive_ff_qcd_dm1_njet0_stat_up_2     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet0_stat_down_2"  , &wt_inclusive_ff_qcd_dm1_njet0_stat_down_2   );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet1_stat_up_2"  , &wt_inclusive_ff_qcd_dm1_njet1_stat_up_2     );
              outtree_->Branch("wt_ff_inclusive_qcd_dm1_njet1_stat_down_2"  , &wt_inclusive_ff_qcd_dm1_njet1_stat_down_2   );
              outtree_->Branch("wt_ff_inclusive_w_syst_up_2"  , &wt_inclusive_ff_w_syst_up_2                 );
              outtree_->Branch("wt_ff_inclusive_w_syst_down_2"  , &wt_inclusive_ff_w_syst_down_2               );
              outtree_->Branch("wt_ff_inclusive_tt_syst_up_2"  , &wt_inclusive_ff_tt_syst_up_2                );
              outtree_->Branch("wt_ff_inclusive_tt_syst_down_2"  , &wt_inclusive_ff_tt_syst_down_2              );
              outtree_->Branch("wt_ff_inclusive_w_frac_syst_up_2"  , &wt_inclusive_ff_w_frac_syst_up_2            );
              outtree_->Branch("wt_ff_inclusive_w_frac_syst_down_2"  , &wt_inclusive_ff_w_frac_syst_down_2          );
              outtree_->Branch("wt_ff_inclusive_tt_frac_syst_up_2"  , &wt_inclusive_ff_tt_frac_syst_up_2           );
              outtree_->Branch("wt_ff_inclusive_tt_frac_syst_down_2"  , &wt_inclusive_ff_tt_frac_syst_down_2         );
              outtree_->Branch("wt_ff_inclusive_dy_frac_syst_up_2"  , &wt_inclusive_ff_dy_frac_syst_up_2           );
              outtree_->Branch("wt_ff_inclusive_dy_frac_syst_down_2"  , &wt_inclusive_ff_dy_frac_syst_down_2         );
              outtree_->Branch("wt_ff_nobtag_qcd_syst_up_2"  , &wt_nobtag_ff_qcd_syst_up_2                  );
              outtree_->Branch("wt_ff_nobtag_qcd_syst_down_2"  , &wt_nobtag_ff_qcd_syst_down_2                );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet0_stat_up_2"  , &wt_nobtag_ff_qcd_dm0_njet0_stat_up_2        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet0_stat_down_2"  , &wt_nobtag_ff_qcd_dm0_njet0_stat_down_2      );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet1_stat_up_2"  , &wt_nobtag_ff_qcd_dm0_njet1_stat_up_2        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm0_njet1_stat_down_2"  , &wt_nobtag_ff_qcd_dm0_njet1_stat_down_2      );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet0_stat_up_2"  , &wt_nobtag_ff_qcd_dm1_njet0_stat_up_2        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet0_stat_down_2"  , &wt_nobtag_ff_qcd_dm1_njet0_stat_down_2      );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet1_stat_up_2"  , &wt_nobtag_ff_qcd_dm1_njet1_stat_up_2        );
              outtree_->Branch("wt_ff_nobtag_qcd_dm1_njet1_stat_down_2"  , &wt_nobtag_ff_qcd_dm1_njet1_stat_down_2      );
              outtree_->Branch("wt_ff_nobtag_w_syst_up_2"  , &wt_nobtag_ff_w_syst_up_2                    );
              outtree_->Branch("wt_ff_nobtag_w_syst_down_2"  , &wt_nobtag_ff_w_syst_down_2                  );
              outtree_->Branch("wt_ff_nobtag_tt_syst_up_2"  , &wt_nobtag_ff_tt_syst_up_2                   );
              outtree_->Branch("wt_ff_nobtag_tt_syst_down_2"  , &wt_nobtag_ff_tt_syst_down_2                 );
              outtree_->Branch("wt_ff_nobtag_w_frac_syst_up_2"  , &wt_nobtag_ff_w_frac_syst_up_2               );
              outtree_->Branch("wt_ff_nobtag_w_frac_syst_down_2"  , &wt_nobtag_ff_w_frac_syst_down_2             );
              outtree_->Branch("wt_ff_nobtag_tt_frac_syst_up_2"  , &wt_nobtag_ff_tt_frac_syst_up_2              );
              outtree_->Branch("wt_ff_nobtag_tt_frac_syst_down_2"  , &wt_nobtag_ff_tt_frac_syst_down_2            );
              outtree_->Branch("wt_ff_nobtag_dy_frac_syst_up_2"  , &wt_nobtag_ff_dy_frac_syst_up_2              );
              outtree_->Branch("wt_ff_nobtag_dy_frac_syst_down_2"  , &wt_nobtag_ff_dy_frac_syst_down_2            );
              outtree_->Branch("wt_ff_btag_qcd_syst_up_2"  , &wt_btag_ff_qcd_syst_up_2                    );
              outtree_->Branch("wt_ff_btag_qcd_syst_down_2"  , &wt_btag_ff_qcd_syst_down_2                  );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet0_stat_up_2"  , &wt_btag_ff_qcd_dm0_njet0_stat_up_2          );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet0_stat_down_2"  , &wt_btag_ff_qcd_dm0_njet0_stat_down_2        );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet1_stat_up_2"  , &wt_btag_ff_qcd_dm0_njet1_stat_up_2          );
              outtree_->Branch("wt_ff_btag_qcd_dm0_njet1_stat_down_2"  , &wt_btag_ff_qcd_dm0_njet1_stat_down_2        );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet0_stat_up_2"  , &wt_btag_ff_qcd_dm1_njet0_stat_up_2          );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet0_stat_down_2"  , &wt_btag_ff_qcd_dm1_njet0_stat_down_2        );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet1_stat_up_2"  , &wt_btag_ff_qcd_dm1_njet1_stat_up_2          );
              outtree_->Branch("wt_ff_btag_qcd_dm1_njet1_stat_down_2"  , &wt_btag_ff_qcd_dm1_njet1_stat_down_2        );
              outtree_->Branch("wt_ff_btag_w_syst_up_2"  , &wt_btag_ff_w_syst_up_2                      );
              outtree_->Branch("wt_ff_btag_w_syst_down_2"  , &wt_btag_ff_w_syst_down_2                    );
              outtree_->Branch("wt_ff_btag_tt_syst_up_2"  , &wt_btag_ff_tt_syst_up_2                     );
              outtree_->Branch("wt_ff_btag_tt_syst_down_2"  , &wt_btag_ff_tt_syst_down_2                   );
              outtree_->Branch("wt_ff_btag_w_frac_syst_up_2"  , &wt_btag_ff_w_frac_syst_up_2                 );
              outtree_->Branch("wt_ff_btag_w_frac_syst_down_2"  , &wt_btag_ff_w_frac_syst_down_2               );
              outtree_->Branch("wt_ff_btag_tt_frac_syst_up_2"  , &wt_btag_ff_tt_frac_syst_up_2                );
              outtree_->Branch("wt_ff_btag_tt_frac_syst_down_2"  , &wt_btag_ff_tt_frac_syst_down_2              );
              outtree_->Branch("wt_ff_btag_dy_frac_syst_up_2"  , &wt_btag_ff_dy_frac_syst_up_2                );
              outtree_->Branch("wt_ff_btag_dy_frac_syst_down_2"  , &wt_btag_ff_dy_frac_syst_down_2              );
          
            }
          } 
        } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16  || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
          outtree_->Branch("wt_ff_1"  , &wt_ff_1_); 
          outtree_->Branch("wt_ff_2"  , &wt_ff_2_);  
          outtree_->Branch("wt_ff_dmbins_1"  , &wt_ff_dmbins_1_);
          outtree_->Branch("wt_ff_dmbins_2"  , &wt_ff_dmbins_2_); 
          if(channel_ == channel::tt && strategy_ != strategy::cpdecays18){
            //outtree_->Branch("wt_ff_2"  , &wt_ff_2_); 
            //outtree_->Branch("wt_ff_1"  , &wt_ff_1_);
            outtree_->Branch("wt_ff_qcd_1"  , &wt_ff_qcd_1_);
            outtree_->Branch("wt_ff_qcd_2"  , &wt_ff_qcd_2_); 
          }
            if(do_ff_systematics_){
                outtree_->Branch("wt_ff_qcd_syst_up_1"            , &wt_ff_qcd_syst_up_1_             );
                outtree_->Branch("wt_ff_qcd_syst_down_1"          , &wt_ff_qcd_syst_down_1_           );
                outtree_->Branch("wt_ff_w_syst_up_1"              , &wt_ff_w_syst_up_1_               );
                outtree_->Branch("wt_ff_w_syst_down_1"            , &wt_ff_w_syst_down_1_             );
                outtree_->Branch("wt_ff_tt_syst_up_1"             , &wt_ff_tt_syst_up_1_              );
                outtree_->Branch("wt_ff_tt_syst_down_1"           , &wt_ff_tt_syst_down_1_            );

                if(strategy_ != strategy::cpdecays18) {
                  outtree_->Branch("wt_ff_qcd_dm0_njet0_stat_up_1"  , &wt_ff_qcd_dm0_njet0_stat_up_1_   );
                  outtree_->Branch("wt_ff_qcd_dm0_njet0_stat_down_1", &wt_ff_qcd_dm0_njet0_stat_down_1_ );
                  outtree_->Branch("wt_ff_qcd_dm0_njet1_stat_up_1"  , &wt_ff_qcd_dm0_njet1_stat_up_1_   );
                  outtree_->Branch("wt_ff_qcd_dm0_njet1_stat_down_1", &wt_ff_qcd_dm0_njet1_stat_down_1_ );
                  outtree_->Branch("wt_ff_qcd_dm1_njet0_stat_up_1"  , &wt_ff_qcd_dm1_njet0_stat_up_1_   );
                  outtree_->Branch("wt_ff_qcd_dm1_njet0_stat_down_1", &wt_ff_qcd_dm1_njet0_stat_down_1_ );
                  outtree_->Branch("wt_ff_qcd_dm1_njet1_stat_up_1"  , &wt_ff_qcd_dm1_njet1_stat_up_1_   );
                  outtree_->Branch("wt_ff_qcd_dm1_njet1_stat_down_1", &wt_ff_qcd_dm1_njet1_stat_down_1_ );
                  outtree_->Branch("wt_ff_realtau_up_1"              , &wt_ff_realtau_up_1_               );
                  outtree_->Branch("wt_ff_realtau_down_1"              , &wt_ff_realtau_down_1_               );
                } else {
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_1"  , &wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_1"  , &wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_1"  , &wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_1"  , &wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm1_up_1"  , &wt_ff_qcd_stat_njet0_mvadm1_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm1_down_1"  , &wt_ff_qcd_stat_njet0_mvadm1_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm2_up_1"  , &wt_ff_qcd_stat_njet0_mvadm2_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm2_down_1"  , &wt_ff_qcd_stat_njet0_mvadm2_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm10_up_1"  , &wt_ff_qcd_stat_njet0_mvadm10_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm10_down_1"  , &wt_ff_qcd_stat_njet0_mvadm10_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm11_up_1"  , &wt_ff_qcd_stat_njet0_mvadm11_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet0_mvadm11_down_1"  , &wt_ff_qcd_stat_njet0_mvadm11_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_1"  , &wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_1"  , &wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_1"  , &wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_1"  , &wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm1_up_1"  , &wt_ff_qcd_stat_njet1_mvadm1_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm1_down_1"  , &wt_ff_qcd_stat_njet1_mvadm1_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm2_up_1"  , &wt_ff_qcd_stat_njet1_mvadm2_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm2_down_1"  , &wt_ff_qcd_stat_njet1_mvadm2_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm10_up_1"  , &wt_ff_qcd_stat_njet1_mvadm10_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm10_down_1"  , &wt_ff_qcd_stat_njet1_mvadm10_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm11_up_1"  , &wt_ff_qcd_stat_njet1_mvadm11_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet1_mvadm11_down_1"  , &wt_ff_qcd_stat_njet1_mvadm11_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_1"  , &wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_1"  , &wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_1"  , &wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_1"  , &wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm1_up_1"  , &wt_ff_qcd_stat_njet2_mvadm1_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm1_down_1"  , &wt_ff_qcd_stat_njet2_mvadm1_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm2_up_1"  , &wt_ff_qcd_stat_njet2_mvadm2_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm2_down_1"  , &wt_ff_qcd_stat_njet2_mvadm2_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm10_up_1"  , &wt_ff_qcd_stat_njet2_mvadm10_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm10_down_1"  , &wt_ff_qcd_stat_njet2_mvadm10_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm11_up_1"  , &wt_ff_qcd_stat_njet2_mvadm11_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_njet2_mvadm11_down_1"  , &wt_ff_qcd_stat_njet2_mvadm11_down_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_pt2_up_1"  , &wt_ff_qcd_stat_pt2_up_1_   );
                  outtree_->Branch("wt_ff_qcd_stat_pt2_down_2"  , &wt_ff_qcd_stat_pt2_down_1_   );
                }
                
                if(channel_ == channel::et || channel_ == channel::mt){
                  outtree_->Branch("wt_ff_w_dm0_njet0_stat_up_1"    , &wt_ff_w_dm0_njet0_stat_up_1_     );
                  outtree_->Branch("wt_ff_w_dm0_njet0_stat_down_1"  , &wt_ff_w_dm0_njet0_stat_down_1_   );
                  outtree_->Branch("wt_ff_w_dm0_njet1_stat_up_1"    , &wt_ff_w_dm0_njet1_stat_up_1_     );
                  outtree_->Branch("wt_ff_w_dm0_njet1_stat_down_1"  , &wt_ff_w_dm0_njet1_stat_down_1_   );
                  outtree_->Branch("wt_ff_w_dm1_njet0_stat_up_1"    , &wt_ff_w_dm1_njet0_stat_up_1_     );
                  outtree_->Branch("wt_ff_w_dm1_njet0_stat_down_1"  , &wt_ff_w_dm1_njet0_stat_down_1_   );
                  outtree_->Branch("wt_ff_w_dm1_njet1_stat_up_1"    , &wt_ff_w_dm1_njet1_stat_up_1_     );
                  outtree_->Branch("wt_ff_w_dm1_njet1_stat_down_1"  , &wt_ff_w_dm1_njet1_stat_down_1_   );
                  outtree_->Branch("wt_ff_tt_dm0_njet0_stat_up_1"   , &wt_ff_tt_dm0_njet0_stat_up_1_    );
                  outtree_->Branch("wt_ff_tt_dm0_njet0_stat_down_1" , &wt_ff_tt_dm0_njet0_stat_down_1_  );
                  outtree_->Branch("wt_ff_tt_dm0_njet1_stat_up_1"   , &wt_ff_tt_dm0_njet1_stat_up_1_    );
                  outtree_->Branch("wt_ff_tt_dm0_njet1_stat_down_1" , &wt_ff_tt_dm0_njet1_stat_down_1_  );
                  outtree_->Branch("wt_ff_tt_dm1_njet0_stat_up_1"   , &wt_ff_tt_dm1_njet0_stat_up_1_    );
                  outtree_->Branch("wt_ff_tt_dm1_njet0_stat_down_1" , &wt_ff_tt_dm1_njet0_stat_down_1_  );
                  outtree_->Branch("wt_ff_tt_dm1_njet1_stat_up_1"   , &wt_ff_tt_dm1_njet1_stat_up_1_    );
                  outtree_->Branch("wt_ff_tt_dm1_njet1_stat_down_1" , &wt_ff_tt_dm1_njet1_stat_down_1_  );
                }
                
                if(channel_ == channel::tt && strategy_ != strategy::cpdecays18){
                  outtree_->Branch("wt_ff_qcd_syst_up_2"            , &wt_ff_qcd_syst_up_2_             );
                  outtree_->Branch("wt_ff_qcd_syst_down_2"          , &wt_ff_qcd_syst_down_2_           );
                  outtree_->Branch("wt_ff_qcd_dm0_njet0_stat_up_2"  , &wt_ff_qcd_dm0_njet0_stat_up_2_   );
                  outtree_->Branch("wt_ff_qcd_dm0_njet0_stat_down_2", &wt_ff_qcd_dm0_njet0_stat_down_2_ );
                  outtree_->Branch("wt_ff_qcd_dm0_njet1_stat_up_2"  , &wt_ff_qcd_dm0_njet1_stat_up_2_   );
                  outtree_->Branch("wt_ff_qcd_dm0_njet1_stat_down_2", &wt_ff_qcd_dm0_njet1_stat_down_2_ );
                  outtree_->Branch("wt_ff_qcd_dm1_njet0_stat_up_2"  , &wt_ff_qcd_dm1_njet0_stat_up_2_   );
                  outtree_->Branch("wt_ff_qcd_dm1_njet0_stat_down_2", &wt_ff_qcd_dm1_njet0_stat_down_2_ );
                  outtree_->Branch("wt_ff_qcd_dm1_njet1_stat_up_2"  , &wt_ff_qcd_dm1_njet1_stat_up_2_   );
                  outtree_->Branch("wt_ff_qcd_dm1_njet1_stat_down_2", &wt_ff_qcd_dm1_njet1_stat_down_2_ );
                  outtree_->Branch("wt_ff_realtau_up_2"              , &wt_ff_realtau_up_2_               );
                  outtree_->Branch("wt_ff_realtau_down_2"              , &wt_ff_realtau_down_2_               );
                  outtree_->Branch("wt_ff_w_syst_up_2"              , &wt_ff_w_syst_up_2_               );
                  outtree_->Branch("wt_ff_w_syst_down_2"            , &wt_ff_w_syst_down_2_             );
                  outtree_->Branch("wt_ff_tt_syst_up_2"             , &wt_ff_tt_syst_up_2_              );
                  outtree_->Branch("wt_ff_tt_syst_down_2"           , &wt_ff_tt_syst_down_2_            );
                  outtree_->Branch("wt_ff_w_frac_syst_up_1"     , &wt_ff_w_frac_syst_up_1_    ); 
                  outtree_->Branch("wt_ff_w_frac_syst_down_1"   , &wt_ff_w_frac_syst_down_1_  );
                  outtree_->Branch("wt_ff_tt_frac_syst_up_1"    , &wt_ff_tt_frac_syst_up_1_   );
                  outtree_->Branch("wt_ff_tt_frac_syst_down_1"  , &wt_ff_tt_frac_syst_down_1_ );
                  outtree_->Branch("wt_ff_dy_frac_syst_up_1"    , &wt_ff_dy_frac_syst_up_1_   );
                  outtree_->Branch("wt_ff_dy_frac_syst_down_1"  , &wt_ff_dy_frac_syst_down_1_ );
                  outtree_->Branch("wt_ff_w_frac_syst_up_2"     , &wt_ff_w_frac_syst_up_2_    );
                  outtree_->Branch("wt_ff_w_frac_syst_down_2"   , &wt_ff_w_frac_syst_down_2_  );
                  outtree_->Branch("wt_ff_tt_frac_syst_up_2"    , &wt_ff_tt_frac_syst_up_2_   );
                  outtree_->Branch("wt_ff_tt_frac_syst_down_2"  , &wt_ff_tt_frac_syst_down_2_ );
                  outtree_->Branch("wt_ff_dy_frac_syst_up_2"    , &wt_ff_dy_frac_syst_up_2_   );
                  outtree_->Branch("wt_ff_dy_frac_syst_down_2"  , &wt_ff_dy_frac_syst_down_2_ );
                }
            }
         }
      }
      
     if(do_qcd_scale_wts_){
       outtree_->Branch("wt_mur1_muf1",    &scale1_);
       outtree_->Branch("wt_mur1_muf2",    &scale2_);
       outtree_->Branch("wt_mur1_muf0p5",  &scale3_);
       outtree_->Branch("wt_mur2_muf1",    &scale4_);
       outtree_->Branch("wt_mur2_muf2",    &scale5_);
       outtree_->Branch("wt_mur2_muf0p5",  &scale6_);
       outtree_->Branch("wt_mur0p5_muf1",  &scale7_);
       outtree_->Branch("wt_mur0p5_muf2",  &scale8_);
       outtree_->Branch("wt_mur0p5_muf0p5",&scale9_);
     }
      
      outtree_->Branch("os",                &os_);
      outtree_->Branch("m_sv",              &m_sv_.var_double);
      outtree_->Branch("mt_sv",             &mt_sv_.var_double);
      outtree_->Branch("m_vis",             &m_vis_.var_double);
      outtree_->Branch("pt_vis",            &pt_vis_.var_double);
      outtree_->Branch("pt_h",              &pt_h_.var_double);
      outtree_->Branch("eta_h",             &eta_h_);
      outtree_->Branch("pt_tt",             &pt_tt_.var_double);
      outtree_->Branch("pfpt_tt",          &pfpt_tt_.var_double);
      outtree_->Branch("mt_tot",            &mt_tot_.var_double);
      outtree_->Branch("pfmt_tot",          &pfmt_tot_.var_double);
      outtree_->Branch("mt_lep",            &mt_lep_.var_double);
      outtree_->Branch("mt_2",              &mt_2_.var_double);
      outtree_->Branch("mt_1",              &mt_1_.var_double);
      outtree_->Branch("pfmt_1",            &pfmt_1_.var_double);
      outtree_->Branch("pfmt_2",            &pfmt_2_.var_double);
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
      outtree_->Branch("antimu_loose_2",    &antimu_loose_2_);
      outtree_->Branch("antiele_tight_2",    &antiele_tight_2_);
      outtree_->Branch("leptonveto",        &lepton_veto_);
      outtree_->Branch("dilepton_veto",     &dilepton_veto_);
      outtree_->Branch("extraelec_veto",    &extraelec_veto_);
      outtree_->Branch("extramuon_veto",    &extramuon_veto_);
      outtree_->Branch("minimal_extraelec_veto",    &minimal_extraelec_veto_);
      outtree_->Branch("minimal_extramuon_veto",    &minimal_extramuon_veto_);
      outtree_->Branch("met",               &met_.var_double);
      outtree_->Branch("pfmet",             &pfmet_.var_double);
      outtree_->Branch("n_jets",            &n_jets_);
      outtree_->Branch("n_bjets",           &n_bjets_);
      outtree_->Branch("n_loose_bjets",     &n_loose_bjets_);
      outtree_->Branch("mjj",               &mjj_.var_double);
      outtree_->Branch("n_jetsingap",       &n_jetsingap_);
      outtree_->Branch("jdeta",             &jdeta_.var_double);
      outtree_->Branch("jdphi",             &jdphi_);
      outtree_->Branch("dphi_jtt",          &dphi_jtt_);
      outtree_->Branch("dijetpt",           &dijetpt_);
      outtree_->Branch("centrality",        &centrality_);
      outtree_->Branch("mt_1_nomu"    , &mt_1_nomu_     );
      outtree_->Branch("mu_pt", &mu_pt_);
      if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16  || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
        outtree_->Branch("sjdphi",             &sjdphi_);
        outtree_->Branch("D0", &D0_);
        outtree_->Branch("DCP", &DCP_);
        outtree_->Branch("spjdphi", &spjdphi_     );
        outtree_->Branch("min_hj_deta", &min_hj_deta_ );
        outtree_->Branch("pjdeta", &pjdeta_      );
        outtree_->Branch("pjahdeta", &pjahdeta_    );
        outtree_->Branch("pjbhdeta", &pjbhdeta_    );
        outtree_->Branch("prob_region", &prob_region_ );
        outtree_->Branch("n_pjets", &n_pjets_ );
        outtree_->Branch("opp_sides",             &opp_sides_);
      }
      outtree_->Branch("n_lowpt_jets",      &n_lowpt_jets_);
      outtree_->Branch("n_jetsingap_lowpt", &n_jetsingap_lowpt_);
      outtree_->Branch("pt_2",              &pt_2_.var_double);
      outtree_->Branch("pt_1",              &pt_1_.var_double);
      outtree_->Branch("eta_1",             &eta_1_.var_double);
      outtree_->Branch("eta_2",             &eta_2_.var_double);
      outtree_->Branch("mjj_lowpt",         &mjj_lowpt_);
      outtree_->Branch("gen_match_1", &gen_match_1_);
      outtree_->Branch("gen_match_2", &gen_match_2_);
      outtree_->Branch("gen_match_1_pt", &gen_match_1_pt_);
      outtree_->Branch("gen_match_2_pt", &gen_match_2_pt_);
      outtree_->Branch("gen_met", &gen_met_);
      outtree_->Branch("fake_met", &fake_met_);
      outtree_->Branch("gen_sjdphi", &gen_sjdphi_);
      outtree_->Branch("gen_mjj", &gen_mjj_);
      outtree_->Branch("ngenjets" , &ngenjets_);
      outtree_->Branch("genM", &gen_m_);
      outtree_->Branch("genpT", &gen_pt_);
      outtree_->Branch("m_1", &m_1_, "m_1/F");
      outtree_->Branch("m_2", &m_2_, "m_2/F");

      outtree_->Branch("aco_angle", &aco_angle_);
      outtree_->Branch("aco_angle_mod", &aco_angle_mod_);
      outtree_->Branch("cp_sign", &cp_sign_);
      outtree_->Branch("wt_cp_sm", &wt_cp_sm_);
      outtree_->Branch("wt_cp_ps", &wt_cp_ps_);
      outtree_->Branch("wt_cp_mm", &wt_cp_mm_);

      outtree_->Branch("mvadm_pi_1", &mvadm_pi_1_);
      outtree_->Branch("mvadm_rho_1", &mvadm_rho_1_);
      outtree_->Branch("mvadm_a1_1", &mvadm_a1_1_);
      outtree_->Branch("mvadm_3pi_1", &mvadm_3pi_1_);
      outtree_->Branch("mvadm_3pipi0_1", &mvadm_3pipi0_1_);

      outtree_->Branch("mvadm_pi_2", &mvadm_pi_2_); 
      outtree_->Branch("mvadm_rho_2", &mvadm_rho_2_);
      outtree_->Branch("mvadm_a1_2", &mvadm_a1_2_); 
      outtree_->Branch("mvadm_3pi_2", &mvadm_3pi_2_);
      outtree_->Branch("mvadm_3pipi0_2", &mvadm_3pipi0_2_);

      outtree_->Branch("mva_dm_1", &tau_mva_decay_mode_1_);
      outtree_->Branch("mva_dm_2", &tau_mva_decay_mode_2_);

      outtree_->Branch("aco_angle_1", &aco_angle_1_);
      outtree_->Branch("aco_angle_2", &aco_angle_2_);
      outtree_->Branch("aco_angle_3", &aco_angle_3_);
      outtree_->Branch("aco_angle_4", &aco_angle_4_);
      outtree_->Branch("aco_angle_5", &aco_angle_5_);
      outtree_->Branch("aco_angle_6", &aco_angle_6_);
      outtree_->Branch("lead_pt_1", &lead_pt_1_);
      outtree_->Branch("lead_pt_2", &lead_pt_2_);

      outtree_->Branch("aco_sign_1", &aco_sign_1_);
      outtree_->Branch("aco_sign_2", &aco_sign_2_);
      outtree_->Branch("aco_sign_3", &aco_sign_3_);
      outtree_->Branch("aco_sign_4", &aco_sign_4_);
      outtree_->Branch("aco_sign_5", &aco_sign_5_);
      outtree_->Branch("rho_dphi", &rho_dphi_);
      outtree_->Branch("rho_deta", &rho_deta_);
      outtree_->Branch("mass0",         &mass0_);
      outtree_->Branch("mass1",         &mass1_);
      outtree_->Branch("mass2",         &mass2_);
      outtree_->Branch("cp_channel" , &cp_channel_);
      outtree_->Branch("y_1_1", &y_1_1_);
      outtree_->Branch("y_1_2", &y_1_2_);
      outtree_->Branch("y_2_2", &y_2_2_);
      outtree_->Branch("y_3_2", &y_3_2_);
      outtree_->Branch("y_4_2", &y_4_2_);
      outtree_->Branch("rand", &rand_);
      outtree_->Branch("cp_sign_1",     &cp_sign_1_);
      outtree_->Branch("cp_sign_2",     &cp_sign_2_);
      outtree_->Branch("cp_sign_3",     &cp_sign_3_);
      outtree_->Branch("cp_sign_4",     &cp_sign_4_);

      outtree_->Branch("IC_BDT_max_score", &IC_BDT_max_score_);
      outtree_->Branch("IC_BDT_max_index", &IC_BDT_max_index_);

      // moved these here from !(systematics_shifts) because want to use
      // them in BDT
      outtree_->Branch("phi_1",             &phi_1_.var_double);
      outtree_->Branch("phi_2",             &phi_2_.var_double);
      outtree_->Branch("dphi",              &dphi_);
      outtree_->Branch("dR",              &dR_);
      outtree_->Branch("jphi_1",            &jphi_1_, "jphi_1/F");
      outtree_->Branch("jphi_2",            &jphi_2_, "jphi_2/F");
      outtree_->Branch("bpt_1",             &bpt_1_.var_float);
      outtree_->Branch("bpt_2",             &bpt_2_.var_float);
      outtree_->Branch("beta_1",            &beta_1_.var_float);
      outtree_->Branch("beta_2",            &beta_2_.var_float);
      outtree_->Branch("bcsv_1",            &bcsv_1_.var_float);
      outtree_->Branch("bcsv_2",            &bcsv_2_);
      outtree_->Branch("met_dphi_1",             &met_dphi_1_);
      outtree_->Branch("met_dphi_2",             &met_dphi_2_);


      outtree_->Branch("db_loose_1",&lbyLooseCombinedIsolation_1);
      outtree_->Branch("db_loose_2",&lbyLooseCombinedIsolation_2);
      outtree_->Branch("db_medium_1",&lbyMediumCombinedIsolation_1);
      outtree_->Branch("db_medium_2",&lbyMediumCombinedIsolation_2);
      outtree_->Branch("db_tight_1",&lbyTightCombinedIsolation_1);
      outtree_->Branch("db_tight_2",&lbyTightCombinedIsolation_2);

      if(strategy_==strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        outtree_->Branch("mva_olddm_vloose_1",&tau_id_olddm_vloose_1_);
        outtree_->Branch("mva_olddm_vloose_2",&tau_id_olddm_vloose_2_);
        outtree_->Branch("mva_olddm_loose_1",&tau_id_olddm_loose_1_);
        outtree_->Branch("mva_olddm_loose_2",&tau_id_olddm_loose_2_);
        outtree_->Branch("mva_olddm_medium_1",&tau_id_olddm_medium_1_);
        outtree_->Branch("mva_olddm_medium_2",&tau_id_olddm_medium_2_);
        outtree_->Branch("mva_olddm_tight_1",&tau_id_olddm_tight_1_);
        outtree_->Branch("mva_olddm_tight_2",&tau_id_olddm_tight_2_);
        outtree_->Branch("mva_olddm_vtight_1",&tau_id_olddm_vtight_1_);
        outtree_->Branch("mva_olddm_vtight_2",&tau_id_olddm_vtight_2_);
        outtree_->Branch("mva_newdm_vloose_1",&tau_id_newdm_vloose_1_);
        outtree_->Branch("mva_newdm_vloose_2",&tau_id_newdm_vloose_2_);
        outtree_->Branch("mva_newdm_loose_1",&tau_id_newdm_loose_1_);
        outtree_->Branch("mva_newdm_loose_2",&tau_id_newdm_loose_2_);
        outtree_->Branch("mva_newdm_medium_1",&tau_id_newdm_medium_1_);
        outtree_->Branch("mva_newdm_medium_2",&tau_id_newdm_medium_2_);
        outtree_->Branch("mva_newdm_tight_1",&tau_id_newdm_tight_1_);
        outtree_->Branch("mva_newdm_tight_2",&tau_id_newdm_tight_2_);  
      } else {    
        outtree_->Branch("mva_olddm_vloose_1",&lbyVLooseIsolationMVArun2DBoldDMwLT_1);
        outtree_->Branch("mva_olddm_vloose_2",&lbyVLooseIsolationMVArun2DBoldDMwLT_2);
        outtree_->Branch("mva_olddm_loose_1",&lbyLooseIsolationMVArun2DBoldDMwLT_1);
        outtree_->Branch("mva_olddm_loose_2",&lbyLooseIsolationMVArun2DBoldDMwLT_2);
        outtree_->Branch("mva_olddm_medium_1",&lbyMediumIsolationMVArun2DBoldDMwLT_1);
        outtree_->Branch("mva_olddm_medium_2",&lbyMediumIsolationMVArun2DBoldDMwLT_2);
        outtree_->Branch("mva_olddm_tight_1",&lbyTightIsolationMVArun2DBoldDMwLT_1);
        outtree_->Branch("mva_olddm_tight_2",&lbyTightIsolationMVArun2DBoldDMwLT_2);
        outtree_->Branch("mva_olddm_vtight_1",&lbyVTightIsolationMVArun2DBoldDMwLT_1);
        outtree_->Branch("mva_olddm_vtight_2",&lbyVTightIsolationMVArun2DBoldDMwLT_2);
        outtree_->Branch("mva_newdm_vloose_1",&lbyVLooseIsolationMVArun2DBnewDMwLT_1);
        outtree_->Branch("mva_newdm_vloose_2",&lbyVLooseIsolationMVArun2DBnewDMwLT_2);
        outtree_->Branch("mva_newdm_loose_1",&lbyLooseIsolationMVArun2DBnewDMwLT_1);
        outtree_->Branch("mva_newdm_loose_2",&lbyLooseIsolationMVArun2DBnewDMwLT_2);
        outtree_->Branch("mva_newdm_medium_1",&lbyMediumIsolationMVArun2DBnewDMwLT_1);
        outtree_->Branch("mva_newdm_medium_2",&lbyMediumIsolationMVArun2DBnewDMwLT_2);
        outtree_->Branch("mva_newdm_tight_1",&lbyTightIsolationMVArun2DBnewDMwLT_1);
        outtree_->Branch("mva_newdm_tight_2",&lbyTightIsolationMVArun2DBnewDMwLT_2);
      }
      // NN Tau IDs
      outtree_->Branch("deepTauVsJets_iso_1",      &deepTauVsJets_iso_1_);
      outtree_->Branch("deepTauVsJets_iso_2",      &deepTauVsJets_iso_2_);
      outtree_->Branch("deepTauVsEle_iso_1",       &deepTauVsEle_iso_1_);
      outtree_->Branch("deepTauVsEle_iso_2",       &deepTauVsEle_iso_2_);
      outtree_->Branch("deepTauVsMu_iso_1",        &deepTauVsMu_iso_1_);
      outtree_->Branch("deepTauVsMu_iso_2",        &deepTauVsMu_iso_2_);

      outtree_->Branch("deepTauVsJets_vvvloose_1", &deepTauVsJets_vvvloose_1_);
      outtree_->Branch("deepTauVsJets_vvvloose_2", &deepTauVsJets_vvvloose_2_);
      outtree_->Branch("deepTauVsJets_vvloose_1",  &deepTauVsJets_vvloose_1_);
      outtree_->Branch("deepTauVsJets_vvloose_2",  &deepTauVsJets_vvloose_2_);
      outtree_->Branch("deepTauVsJets_vloose_1",   &deepTauVsJets_vloose_1_);
      outtree_->Branch("deepTauVsJets_vloose_2",   &deepTauVsJets_vloose_2_);
      outtree_->Branch("deepTauVsJets_loose_1",    &deepTauVsJets_loose_1_);
      outtree_->Branch("deepTauVsJets_loose_2",    &deepTauVsJets_loose_2_);
      outtree_->Branch("deepTauVsJets_medium_1",   &deepTauVsJets_medium_1_);
      outtree_->Branch("deepTauVsJets_medium_2",   &deepTauVsJets_medium_2_);
      outtree_->Branch("deepTauVsJets_tight_1",    &deepTauVsJets_tight_1_);
      outtree_->Branch("deepTauVsJets_tight_2",    &deepTauVsJets_tight_2_);
      outtree_->Branch("deepTauVsJets_vtight_1",   &deepTauVsJets_vtight_1_);
      outtree_->Branch("deepTauVsJets_vtight_2",   &deepTauVsJets_vtight_2_);
      outtree_->Branch("deepTauVsJets_vvtight_1",  &deepTauVsJets_vvtight_1_);
      outtree_->Branch("deepTauVsJets_vvtight_2",  &deepTauVsJets_vvtight_2_);

      outtree_->Branch("deepTauVsEle_vvvloose_1",  &deepTauVsEle_vvvloose_1_);
      outtree_->Branch("deepTauVsEle_vvvloose_2",  &deepTauVsEle_vvvloose_2_);
      outtree_->Branch("deepTauVsEle_vvloose_1",   &deepTauVsEle_vvloose_1_);
      outtree_->Branch("deepTauVsEle_vvloose_2",   &deepTauVsEle_vvloose_2_);
      outtree_->Branch("deepTauVsEle_vloose_1",    &deepTauVsEle_vloose_1_);
      outtree_->Branch("deepTauVsEle_vloose_2",    &deepTauVsEle_vloose_2_);
      outtree_->Branch("deepTauVsEle_loose_1",     &deepTauVsEle_loose_1_);
      outtree_->Branch("deepTauVsEle_loose_2",     &deepTauVsEle_loose_2_);
      outtree_->Branch("deepTauVsEle_medium_1",    &deepTauVsEle_medium_1_);
      outtree_->Branch("deepTauVsEle_medium_2",    &deepTauVsEle_medium_2_);
      outtree_->Branch("deepTauVsEle_tight_1",     &deepTauVsEle_tight_1_);
      outtree_->Branch("deepTauVsEle_tight_2",     &deepTauVsEle_tight_2_);
      outtree_->Branch("deepTauVsEle_vtight_1",    &deepTauVsEle_vtight_1_);
      outtree_->Branch("deepTauVsEle_vtight_2",    &deepTauVsEle_vtight_2_);
      outtree_->Branch("deepTauVsEle_vvtight_1",   &deepTauVsEle_vvtight_1_);
      outtree_->Branch("deepTauVsEle_vvtight_2",   &deepTauVsEle_vvtight_2_);
      outtree_->Branch("deepTauVsMu_vvvloose_1",  &deepTauVsMu_vvvloose_1_);
      outtree_->Branch("deepTauVsMu_vvvloose_2",  &deepTauVsMu_vvvloose_2_);
      outtree_->Branch("deepTauVsMu_vvloose_1",   &deepTauVsMu_vvloose_1_);
      outtree_->Branch("deepTauVsMu_vvloose_2",   &deepTauVsMu_vvloose_2_);
      outtree_->Branch("deepTauVsMu_vloose_1",    &deepTauVsMu_vloose_1_);
      outtree_->Branch("deepTauVsMu_vloose_2",    &deepTauVsMu_vloose_2_);
      outtree_->Branch("deepTauVsMu_loose_1",     &deepTauVsMu_loose_1_);
      outtree_->Branch("deepTauVsMu_loose_2",     &deepTauVsMu_loose_2_);
      outtree_->Branch("deepTauVsMu_medium_1",    &deepTauVsMu_medium_1_);
      outtree_->Branch("deepTauVsMu_medium_2",    &deepTauVsMu_medium_2_);
      outtree_->Branch("deepTauVsMu_tight_1",     &deepTauVsMu_tight_1_);
      outtree_->Branch("deepTauVsMu_tight_2",     &deepTauVsMu_tight_2_);
      outtree_->Branch("deepTauVsMu_vtight_1",    &deepTauVsMu_vtight_1_);
      outtree_->Branch("deepTauVsMu_vtight_2",    &deepTauVsMu_vtight_2_);
      outtree_->Branch("deepTauVsMu_vvtight_1",   &deepTauVsMu_vvtight_1_);
      outtree_->Branch("deepTauVsMu_vvtight_2",   &deepTauVsMu_vvtight_2_);

      outtree_->Branch("tau_decay_mode_2",    &tau_decay_mode_2_);
      outtree_->Branch("tau_decay_mode_1",    &tau_decay_mode_1_);
      outtree_->Branch("trg_singleelectron",    &trg_singleelectron_);
      outtree_->Branch("trg_singlemuon",    &trg_singlemuon_);
      outtree_->Branch("trg_doubletau",    &trg_doubletau_);
      outtree_->Branch("trg_vbfdoubletau",    &trg_vbfdoubletau_);
      outtree_->Branch("trg_muonelectron",    &trg_muonelectron_);
      outtree_->Branch("trg_muonelectron_1",    &trg_muonelectron_1_);
      outtree_->Branch("trg_muonelectron_2",    &trg_muonelectron_2_);
      outtree_->Branch("trg_muonelectron_3",    &trg_muonelectron_3_);
      outtree_->Branch("trg_singletau_1",    &trg_singletau_1_);
      outtree_->Branch("trg_singletau_2",    &trg_singletau_2_);
      outtree_->Branch("trg_mutaucross",    &trg_mutaucross_);
      outtree_->Branch("trg_etaucross",    &trg_etaucross_);
      outtree_->Branch("jpt_1",             &jpt_1_.var_double);
      outtree_->Branch("jpt_2",             &jpt_2_.var_double);
      outtree_->Branch("jeta_1",            &jeta_1_.var_double);
      outtree_->Branch("jeta_2",            &jeta_2_.var_double);
      outtree_->Branch("jmva_1",             &jmva_1_);
      outtree_->Branch("jmva_2",             &jmva_2_);
      outtree_->Branch("jchm_1"  , &jchm_1_);
      outtree_->Branch("jnm_1"   , &jnm_1_);
      outtree_->Branch("jpm_1"   , &jpm_1_);
      outtree_->Branch("jchemf_1", &jchemf_1_);
      outtree_->Branch("jnemf_1" , &jnemf_1_);
      outtree_->Branch("jchhf_1", &jchhf_1_);
      outtree_->Branch("jnhf_1" , &jnhf_1_);
      outtree_->Branch("jchm_2"  , &jchm_2_);
      outtree_->Branch("jnm_2"   , &jnm_2_);
      outtree_->Branch("jpm_2"   , &jpm_2_);
      outtree_->Branch("jchemf_2", &jchemf_2_);
      outtree_->Branch("jnemf_2" , &jnemf_2_);
      outtree_->Branch("jchhf_2", &jchhf_2_);
      outtree_->Branch("jnhf_2" , &jnhf_2_);

      outtree_->Branch("jlrm_1",             &jlrm_1_);
      outtree_->Branch("jlrm_2",             &jlrm_2_);

      //outtree_->Branch("HLT_paths",    &HLT_paths_);

/*      outtree_->Branch("leading_lepton_match_pt", &leading_lepton_match_pt_);
      outtree_->Branch("subleading_lepton_match_pt",&subleading_lepton_match_pt_);
      outtree_->Branch("leading_lepton_match_DR", &leading_lepton_match_DR_);
      outtree_->Branch("subleading_lepton_match_DR",&subleading_lepton_match_DR_);*/

      outtree_->Branch("jdeta_lowpt",       &jdeta_lowpt_);
      if (channel_ == channel::em) {
        outtree_->Branch("em_gf_mva",         &em_gf_mva_);
        outtree_->Branch("wt_em_qcd",         &wt_em_qcd_);
        outtree_->Branch("wt_em_qcd_extrapup",      &wt_em_qcd_extrapup_);
        outtree_->Branch("wt_em_qcd_extrapdown",    &wt_em_qcd_extrapdown_);
        outtree_->Branch("wt_em_qcd_njets0_unc1_up",    &wt_em_qcd_njets0_unc1_up_);
        outtree_->Branch("wt_em_qcd_njets0_unc1_down",  &wt_em_qcd_njets0_unc1_down_);
        outtree_->Branch("wt_em_qcd_njets0_unc2_up",    &wt_em_qcd_njets0_unc2_up_);
        outtree_->Branch("wt_em_qcd_njets0_unc2_down",  &wt_em_qcd_njets0_unc2_down_);
        outtree_->Branch("wt_em_qcd_njets1_unc1_up",    &wt_em_qcd_njets1_unc1_up_);
        outtree_->Branch("wt_em_qcd_njets1_unc1_down",  &wt_em_qcd_njets1_unc1_down_);
        outtree_->Branch("wt_em_qcd_njets1_unc2_up",    &wt_em_qcd_njets1_unc2_up_);
        outtree_->Branch("wt_em_qcd_njets1_unc2_down",  &wt_em_qcd_njets1_unc2_down_);
        outtree_->Branch("wt_em_qcd_njets2_unc1_up",    &wt_em_qcd_njets2_unc1_up_);
        outtree_->Branch("wt_em_qcd_njets2_unc1_down",  &wt_em_qcd_njets2_unc1_down_);
        outtree_->Branch("wt_em_qcd_njets2_unc2_up",    &wt_em_qcd_njets2_unc2_up_);
        outtree_->Branch("wt_em_qcd_njets2_unc2_down",  &wt_em_qcd_njets2_unc2_down_);
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
 
      if(tau_id_study_){
       outtree_->Branch("mvadbnew_vloose_1",&lbyVLooseIsolationMVArun2DBnewDMwLT_1);
       outtree_->Branch("mvadbnew_vloose_2",&lbyVLooseIsolationMVArun2DBnewDMwLT_2);
       outtree_->Branch("mvadbnew_loose_1",&lbyLooseIsolationMVArun2DBnewDMwLT_1);
       outtree_->Branch("mvadbnew_loose_2",&lbyLooseIsolationMVArun2DBnewDMwLT_2);
       outtree_->Branch("mvadbnew_medium_1",&lbyMediumIsolationMVArun2DBnewDMwLT_1);
       outtree_->Branch("mvadbnew_medium_2",&lbyMediumIsolationMVArun2DBnewDMwLT_2);
       outtree_->Branch("mvadbnew_tight_1",&lbyTightIsolationMVArun2DBnewDMwLT_1);
       outtree_->Branch("mvadbnew_tight_2",&lbyTightIsolationMVArun2DBnewDMwLT_2);
       outtree_->Branch("mvadbnew_vtight_1",&lbyVTightIsolationMVArun2DBnewDMwLT_1);
       outtree_->Branch("mvadbnew_vtight_2",&lbyVTightIsolationMVArun2DBnewDMwLT_2);
       outtree_->Branch("mvadbnew_vvtight_1",&lbyVVTightIsolationMVArun2DBnewDMwLT_1);
       outtree_->Branch("mvadbnew_vvtight_2",&lbyVVTightIsolationMVArun2DBnewDMwLT_2);
       outtree_->Branch("mvadbold_vloose_1",&lbyVLooseIsolationMVArun2DBoldDMwLT_1);
       outtree_->Branch("mvadbold_vloose_2",&lbyVLooseIsolationMVArun2DBoldDMwLT_2);
       outtree_->Branch("mvadbold_loose_1",&lbyLooseIsolationMVArun2DBoldDMwLT_1);
       outtree_->Branch("mvadbold_loose_2",&lbyLooseIsolationMVArun2DBoldDMwLT_2);
       outtree_->Branch("mvadbold_medium_1",&lbyMediumIsolationMVArun2DBoldDMwLT_1);
       outtree_->Branch("mvadbold_medium_2",&lbyMediumIsolationMVArun2DBoldDMwLT_2);
       outtree_->Branch("mvadbold_tight_1",&lbyTightIsolationMVArun2DBoldDMwLT_1);
       outtree_->Branch("mvadbold_tight_2",&lbyTightIsolationMVArun2DBoldDMwLT_2);
       outtree_->Branch("mvadbold_vtight_1",&lbyVTightIsolationMVArun2DBoldDMwLT_1);
       outtree_->Branch("mvadbold_vtight_2",&lbyVTightIsolationMVArun2DBoldDMwLT_2);
       outtree_->Branch("mvadbold_vvtight_1",&lbyVVTightIsolationMVArun2DBoldDMwLT_1);
       outtree_->Branch("mvadbold_vvtight_2",&lbyVVTightIsolationMVArun2DBoldDMwLT_2);
       outtree_->Branch("mvapwnew_vloose_1",&lbyVLooseIsolationMVArun2PWnewDMwLT_1);
       outtree_->Branch("mvapwnew_vloose_2",&lbyVLooseIsolationMVArun2PWnewDMwLT_2);
       outtree_->Branch("mvapwnew_loose_1",&lbyLooseIsolationMVArun2PWnewDMwLT_1);
       outtree_->Branch("mvapwnew_loose_2",&lbyLooseIsolationMVArun2PWnewDMwLT_2);
       outtree_->Branch("mvapwnew_medium_1",&lbyMediumIsolationMVArun2PWnewDMwLT_1);
       outtree_->Branch("mvapwnew_medium_2",&lbyMediumIsolationMVArun2PWnewDMwLT_2);
       outtree_->Branch("mvapwnew_tight_1",&lbyTightIsolationMVArun2PWnewDMwLT_1);
       outtree_->Branch("mvapwnew_tight_2",&lbyTightIsolationMVArun2PWnewDMwLT_2);
       outtree_->Branch("mvapwnew_vtight_1",&lbyVTightIsolationMVArun2PWnewDMwLT_1);
       outtree_->Branch("mvapwnew_vtight_2",&lbyVTightIsolationMVArun2PWnewDMwLT_2);
       outtree_->Branch("mvapwnew_vvtight_1",&lbyVVTightIsolationMVArun2PWnewDMwLT_1);
       outtree_->Branch("mvapwnew_vvtight_2",&lbyVVTightIsolationMVArun2PWnewDMwLT_2);
       outtree_->Branch("mvapwold_vloose_1",&lbyVLooseIsolationMVArun2PWoldDMwLT_1);
       outtree_->Branch("mvapwold_vloose_2",&lbyVLooseIsolationMVArun2PWoldDMwLT_2);
       outtree_->Branch("mvapwold_loose_1",&lbyLooseIsolationMVArun2PWoldDMwLT_1);
       outtree_->Branch("mvapwold_loose_2",&lbyLooseIsolationMVArun2PWoldDMwLT_2);
       outtree_->Branch("mvapwold_medium_1",&lbyMediumIsolationMVArun2PWoldDMwLT_1);
       outtree_->Branch("mvapwold_medium_2",&lbyMediumIsolationMVArun2PWoldDMwLT_2);
       outtree_->Branch("mvapwold_tight_1",&lbyTightIsolationMVArun2PWoldDMwLT_1);
       outtree_->Branch("mvapwold_tight_2",&lbyTightIsolationMVArun2PWoldDMwLT_2);
       outtree_->Branch("mvapwold_vtight_1",&lbyVTightIsolationMVArun2PWoldDMwLT_1);
       outtree_->Branch("mvapwold_vtight_2",&lbyVTightIsolationMVArun2PWoldDMwLT_2);
       outtree_->Branch("mvapwold_vvtight_1",&lbyVVTightIsolationMVArun2PWoldDMwLT_1);
       outtree_->Branch("mvapwold_vvtight_2",&lbyVVTightIsolationMVArun2PWoldDMwLT_2);
       outtree_->Branch("puw_loose_1",&lbyLoosePileupWeightedIsolation_1);
       outtree_->Branch("puw_loose_2",&lbyLoosePileupWeightedIsolation_2);
       outtree_->Branch("puw_medium_1",&lbyMediumPileupWeightedIsolation_1);
       outtree_->Branch("puw_medium_2",&lbyMediumPileupWeightedIsolation_2);
       outtree_->Branch("puw_tight_1",&lbyTightPileupWeightedIsolation_1);
       outtree_->Branch("puw_tight_2",&lbyTightPileupWeightedIsolation_2);
       outtree_->Branch("antie_vloose_1",&lagainstElectronVLooseMVA_1);
       outtree_->Branch("antie_loose_1",&lagainstElectronLooseMVA_1);
       outtree_->Branch("antie_medium_1",&lagainstElectronMediumMVA_1); 
       outtree_->Branch("antie_tight_1",&lagainstElectronTightMVA_1);
       outtree_->Branch("antie_vtight_1",&lagainstElectronVTightMVA_1);
       outtree_->Branch("antimu_loose_1",&lagainstMuonLoose3_1);
       outtree_->Branch("antimu_tight_1",&lagainstMuonTight3_1);
       outtree_->Branch("antie_vloose_2",&lagainstElectronVLooseMVA_2);
       outtree_->Branch("antie_loose_2",&lagainstElectronLooseMVA_2);
       outtree_->Branch("antie_medium_2",&lagainstElectronMediumMVA_2); 
       outtree_->Branch("antie_tight_2",&lagainstElectronTightMVA_2);
       outtree_->Branch("antie_vtight_2",&lagainstElectronVTightMVA_2);
       outtree_->Branch("antimu_loose_2",&lagainstMuonLoose3_2);
       outtree_->Branch("antimu_tight_2",&lagainstMuonTight3_2);
       outtree_->Branch("isoPhoSumPt_2",&lPhotonPtSum_2.var_float);
       outtree_->Branch("isoPhoSumPt_1",&lPhotonPtSum_1.var_float);
       outtree_->Branch("iso_mvadb_new_1",&lbyIsolationMVArun2DBnewDMwLTraw_1.var_double);
       outtree_->Branch("iso_mvadb_old_1",&lbyIsolationMVArun2DBoldDMwLTraw_1.var_double);
       outtree_->Branch("iso_mvadb_new_2",&lbyIsolationMVArun2DBnewDMwLTraw_2.var_double);
       outtree_->Branch("iso_mvadb_old_2",&lbyIsolationMVArun2DBoldDMwLTraw_2.var_double);
       outtree_->Branch("iso_mvapw_new_1",&lbyIsolationMVArun2PWnewDMwLTraw_1.var_double);
       outtree_->Branch("iso_mvapw_old_1",&lbyIsolationMVArun2PWoldDMwLTraw_1.var_double);
       outtree_->Branch("iso_mvapw_new_2",&lbyIsolationMVArun2PWnewDMwLTraw_2.var_double);
       outtree_->Branch("iso_mvapw_old_2",&lbyIsolationMVArun2PWoldDMwLTraw_2.var_double);
       outtree_->Branch("olddm_1",&ldecayModeFindingOldDMs_1);
       outtree_->Branch("olddm_2",&ldecayModeFindingOldDMs_2);
       outtree_->Branch("chargedIsoPtSum_1", &lchargedIsoPtSum_1);
       outtree_->Branch("neutralIsoPtSum_1", &lneutralIsoPtSum_1);
       outtree_->Branch("chargedIsoPtSum_2", &lchargedIsoPtSum_2);
       outtree_->Branch("neutralIsoPtSum_2", &lneutralIsoPtSum_2);
       outtree_->Branch("chargedRelIsoPtSum_1", &lchargedRelIsoPtSum_1);
       outtree_->Branch("chargedRelIsoPtSum_2", &lchargedRelIsoPtSum_2);
       outtree_->Branch("db_iso_1", &l3Hits_1);
       outtree_->Branch("db_iso_2", &l3Hits_2);
      }
      if(qcd_study_){
        outtree_->Branch("jet_flav_1", &jet_flav_1_);
        outtree_->Branch("jet_flav_2", &jet_flav_2_);
        outtree_->Branch("jet_flav_3", &jet_flav_3_);
      }
      if(do_faketaus_){
        outtree_->Branch("tau_dm_1", &tau_dm_1_);
        outtree_->Branch("tau_dm_2", &tau_dm_2_);  
        outtree_->Branch("tau_pt_1", &tau_pt_1_);
        outtree_->Branch("tau_pt_2", &tau_pt_2_);      
        outtree_->Branch("tau_id_1", &tau_id_1_);
        outtree_->Branch("tau_id_2", &tau_id_2_);
        outtree_->Branch("tau_vloose_1", &tau_vloose_1_);
        outtree_->Branch("tau_vloose_2", &tau_vloose_2_);
      }

      outtree_->Branch("q_1", &q_1_);
      outtree_->Branch("q_2", &q_2_);
      if(channel_ == channel::tpzmm || channel_ == channel::tpzee){
        //Extra variables needed for tag and probe
        outtree_->Branch("id_1", &mva_1_.var_double);
        outtree_->Branch("id_2", &mva_2_.var_double);
        outtree_->Branch("dxy_1", &d0_1_.var_double);
        outtree_->Branch("dxy_2", &d0_2_.var_double);
        outtree_->Branch("dz_1", &dz_1_.var_double);
        outtree_->Branch("dz_2", &dz_2_.var_double);
        outtree_->Branch("tag_trigger_match_1", &tag_trigger_match_1_);
        outtree_->Branch("tag_trigger_match_2", &tag_trigger_match_2_);
        outtree_->Branch("probe_trigger_match_1", &probe_trigger_match_1_);
        outtree_->Branch("probe_trigger_match_2", &probe_trigger_match_2_);
      }
      outtree_->Branch("wt_zpt_down",       &wt_zpt_down_);
      outtree_->Branch("wt_embed_mc_yield", &wt_embed_mc_yield_);
      if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16){
        outtree_->Branch("wt_zpt_stat_m400pt0_up"    , &wt_zpt_stat_m400pt0_up     );
        outtree_->Branch("wt_zpt_stat_m400pt40_up"   , &wt_zpt_stat_m400pt40_up    );
        outtree_->Branch("wt_zpt_stat_m400pt80_up"   , &wt_zpt_stat_m400pt80_up    );
        outtree_->Branch("wt_zpt_stat_m400pt0_down"  , &wt_zpt_stat_m400pt0_down   );
        outtree_->Branch("wt_zpt_stat_m400pt40_down" , &wt_zpt_stat_m400pt40_down  );
        outtree_->Branch("wt_zpt_stat_m400pt80_down" , &wt_zpt_stat_m400pt80_down  );
        outtree_->Branch("wt_zpt_esup"               , &wt_zpt_esup                );
        outtree_->Branch("wt_zpt_esdown"             , &wt_zpt_esdown              );
        outtree_->Branch("wt_zpt_ttup"               , &wt_zpt_ttup                );
        outtree_->Branch("wt_zpt_ttdown"             , &wt_zpt_ttdown              );
      }
        
                                                                
      //Variables needed for control plots need only be generated for central systematics
      if(!systematic_shift_) {
        //outtree_->Branch("wt_ggh_pt_up",      &wt_ggh_pt_up_);
        //outtree_->Branch("wt_ggh_pt_down",    &wt_ggh_pt_down_);
        outtree_->Branch("wt_tau_fake_up",    &wt_tau_fake_up_);
        outtree_->Branch("wt_tau_fake_down",  &wt_tau_fake_down_);
        outtree_->Branch("wt_tquark_up",      &wt_tquark_up_);
        outtree_->Branch("wt_tquark_down",    &wt_tquark_down_);
        outtree_->Branch("wt_zpt_up",         &wt_zpt_up_);
        outtree_->Branch("wt_tau_id_up",      &wt_tau_id_up_);
        outtree_->Branch("wt_tau_id_down",    &wt_tau_id_down_);
        outtree_->Branch("wt_trig_up_1",    &wt_trig_up_1_);
        outtree_->Branch("wt_trig_up_2",    &wt_trig_up_2_);
        outtree_->Branch("wt_trig_down_1",    &wt_trig_down_1_);
        outtree_->Branch("wt_trig_down_2",    &wt_trig_down_2_);
        outtree_->Branch("wt_trig_up",    &wt_trig_up_);
        outtree_->Branch("wt_trig_down",    &wt_trig_down_);
        outtree_->Branch("n_vtx",             &n_vtx_);
        outtree_->Branch("good_vtx",          &good_vtx_);
        outtree_->Branch("z_2",               &z_2_);
        outtree_->Branch("met_phi",           &met_phi_.var_double);
        outtree_->Branch("n_prebjets",        &n_prebjets_);
        outtree_->Branch("nearjpt_1",             &nearjpt_1_);
        outtree_->Branch("j1_dm",             &j1_dm_);
        if((strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) && do_sm_scale_wts_ ){
          outtree_->Branch("wt_scale_et_0jet"   , &wt_scale_et_0jet_    );
          outtree_->Branch("wt_scale_et_boosted" , &wt_scale_et_boosted_ );
          outtree_->Branch("wt_scale_et_vbf"    , &wt_scale_et_vbf_     );
          outtree_->Branch("wt_scale_mt_0jet"   , &wt_scale_mt_0jet_    );
          outtree_->Branch("wt_scale_mt_boosted" , &wt_scale_mt_boosted_ );
          outtree_->Branch("wt_scale_mt_vbf"    , &wt_scale_mt_vbf_     );
          outtree_->Branch("wt_scale_em_0jet"   , &wt_scale_em_0jet_    );
          outtree_->Branch("wt_scale_em_boosted" , &wt_scale_em_boosted_ );
          outtree_->Branch("wt_scale_em_vbf"    , &wt_scale_em_vbf_     );
          outtree_->Branch("wt_scale_tt_0jet"   , &wt_scale_tt_0jet_    );
          outtree_->Branch("wt_scale_tt_boosted" , &wt_scale_tt_boosted_ );
          outtree_->Branch("wt_scale_tt_vbf"    , &wt_scale_tt_vbf_     );
        }
        if((strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) && do_sm_scale_wts_ ){
          outtree_->Branch("wt_qcdscale_up"   , &wt_qcdscale_up_    );
          outtree_->Branch("wt_qcdscale_down" , &wt_qcdscale_down_ );
        }
        if(do_z_weights_){
          outtree_->Branch("wt_z_mjj",      &wt_z_mjj_);   
          outtree_->Branch("wt_z_mjj_up",      &wt_z_mjj_up_);    
          outtree_->Branch("wt_z_mjj_down",    &wt_z_mjj_down_);    
        }

        if (channel_ == channel::em) {
          outtree_->Branch("pzetavis",          &pzetavis_.var_double);
          outtree_->Branch("pzetamiss",         &pzetamiss_.var_double);
          outtree_->Branch("mt_ll",             &mt_ll_);
          outtree_->Branch("emu_dphi",          &dphi_);
          outtree_->Branch("emu_csv",           &emu_csv_);
          outtree_->Branch("emu_dxy_1",         &emu_dxy_1_);
          outtree_->Branch("emu_dxy_2",         &emu_dxy_2_);
          outtree_->Branch("dz_1",              &dz_1_.var_double);
          outtree_->Branch("dz_2",              &dz_2_.var_double);
        } 
        if (optimisation_study_){
          outtree_->Branch("pzetavis",          &pzetavis_.var_double);
          outtree_->Branch("pzetamiss",         &pzetamiss_.var_double);
          outtree_->Branch("mt_ll",             &mt_ll_);
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
      synctree_->Branch("npu", &n_pu_, "n_pu/F");
      // The rho used for jet energy corrections
      synctree_->Branch("rho", &rho_, "rho/F");

      // The lumi scaling factor for mc * additional weights
      // (not filled in IC trees!)
      /*synctree_->Branch("mcweight", &mc_weight_, "mc_weight/F");*/
      // Pileup weight
      synctree_->Branch("puweight", &pu_weight_, "pu_weight/F");

      // Tag-and-probe weights for leptons
      // Total trigger weight for lepton 1
     synctree_->Branch("trigweight_1", &trigweight_1_, "trigweight_1/F");
      // Total trigger weight for lepton 2
      synctree_->Branch("trigweight_2", &trigweight_2_, "trigweight_2/F");
      // Total ID weight for lepton 1
      synctree_->Branch("idisoweight_1", &idisoweight_1_, "idisoweight_1/F");
      // Total ID weight for lepton 2
      synctree_->Branch("idisoweight_2", &idisoweight_2_, "idisoweight_2/F");
      // Tracking efficiency weightfor lepton 1
      synctree_->Branch("trackingweight_1", &trackingweight_1_, "trackingweight_1/F");
      // Tracking efficiency weight for lepton 2
      synctree_->Branch("trackingweight_2", &trackingweight_2_, "trackingweight_2/F");
      // Total iso weight for lepton 1
//      synctree_->Branch("isoweight_1", &isoweight_1_, "isoweight_1/F");
      // Total iso weight for lepton 2
 //     synctree_->Branch("isoweight_2", &isoweight_2_, "isoweight_2/F");
     // Product of all trigger, ID and iso weights
      synctree_->Branch("effweight", &effweight_, "effweight/F");
      // Jet->tau fake rate weight (pT-dependent)
  //    synctree_->Branch("fakeweight", &fakeweight_, "fakeweight/F");
      // Product of all embedded weights, but only for rechit samples
 //     synctree_->Branch("embeddedWeight", &embeddedweight_, "embeddedweight/F");
      // Higgs pt weights (for ggh samples)
//      synctree_->Branch("signalWeight", &signalweight_, "signalweight/F");
      // Total combined event weight (excluding lumi weighting)
      // NB: may contain weights not included in the above
      synctree_->Branch("weight", &wt_.var_float, "wt/F");


      // Visible di-tau mass
      synctree_->Branch("m_vis", &m_vis_.var_float, "m_vis/F");
      // SVFit di-tau transverse mass
      synctree_->Branch("mt_sv", &mt_sv_.var_float, "mt_sv/F");
      synctree_->Branch("mt_fast", &m_sv_.var_float, "mt_fast/F");
      // SVFit di-tau mass
      synctree_->Branch("m_sv",   &m_sv_.var_float, "m_sv/F");
      synctree_->Branch("m_fast", &m_sv_.var_float, "m_fast/F");
      // SVFit di-tau pt (only for Markov-Chain SVFit)
      synctree_->Branch("pt_sv", &pt_h_.var_float, "pt_h/F");
      // SVFit di-tau eta (only for Markov-Chain SVFit)
      synctree_->Branch("eta_sv", &eta_h_, "eta_h/F");
      // SVFit di-tau phi (only for Markov-Chain SVFit)
      synctree_->Branch("phi_sv", &phi_h_, "phi_h/F");

      // Lepton 1 properties
      if ((strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) && (channel_ == channel::et || channel_ == channel::em)) { 
        synctree_->Branch("eCorr_1",           &E_1_);
      }
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

      // repeated twice in case want different branch names for sync
      synctree_->Branch("tau_decay_mode_1", & tau_decay_mode_1_, "tau_decay_mode_1/I");
      synctree_->Branch("tau_decay_mode_2", & tau_decay_mode_2_, "tau_decay_mode_2/I");
      synctree_->Branch("dm_1", & tau_decay_mode_1_, "dm_1/I");
      synctree_->Branch("dm_2", & tau_decay_mode_2_, "dm_2/I");
      // MVA tau decay modes (for decays analysis specifically)
      synctree_->Branch("mva_dm_1",         & tau_mva_decay_mode_1_, "mva_dm_1/I");
      synctree_->Branch("mva_dm_2",         & tau_mva_decay_mode_2_, "mva_dm_2/I");
      synctree_->Branch("dmMVA_1",         & tau_mva_decay_mode_1_, "dmMVA_1/I");
      synctree_->Branch("dmMVA_2",         & tau_mva_decay_mode_2_, "dmMVA_2/I");

      synctree_->Branch("mva_olddm_medium_1",&lbyMediumIsolationMVArun2DBoldDMwLT_1,"mva_olddm_medium_1/O");
      synctree_->Branch("mva_olddm_medium_2",&lbyMediumIsolationMVArun2DBoldDMwLT_2,"mva_olddm_medium_2/O");
      synctree_->Branch("mva_olddm_tight_1",&lbyTightIsolationMVArun2DBoldDMwLT_1,"mva_olddm_tight_1/O");
      synctree_->Branch("mva_olddm_tight_2",&lbyTightIsolationMVArun2DBoldDMwLT_2,"mva_olddm_tight_2/O");
      synctree_->Branch("mva_olddm_vtight_1",&lbyVTightIsolationMVArun2DBoldDMwLT_1,"mva_olddm_tight_1/O");
      synctree_->Branch("mva_olddm_vtight_2",&lbyVTightIsolationMVArun2DBoldDMwLT_2,"mva_olddm_tight_2/O");

      // deeptau variables
      synctree_->Branch("deepTauVsJetsRaw_1", & deepTauVsJets_iso_1_, "deepTauVsJetsRaw_1/D");
      synctree_->Branch("deepTauVsJetsRaw_2", & deepTauVsJets_iso_2_, "deepTauVsJetsRaw_2/D");
      synctree_->Branch("deepTauVsEleRaw_1",  & deepTauVsEle_iso_1_, "deepTauVsEleRaw_1/D");
      synctree_->Branch("deepTauVsEleRaw_2",  & deepTauVsEle_iso_2_, "deepTauVsEleRaw_2/D");
      synctree_->Branch("deepTauVsMuRaw_1",   & deepTauVsMu_iso_1_, "deepTauVsMuRaw_1/D");
      synctree_->Branch("deepTauVsMuRaw_2",   & deepTauVsMu_iso_2_, "deepTauVsMuRaw_2/D");

      // tauspinner weights
      synctree_->Branch("tauspinnerH",      & wt_cp_sm_, "tauspinnerH/D");
      synctree_->Branch("tauspinnerA",      & wt_cp_ps_, "tauspinnerA/D");
      synctree_->Branch("tauspinnerMaxMix", & wt_cp_mm_, "tauspinnerMaxMix/D");


      // Lepton 2 properties
      // pt (including effect of any energy scale corrections)
      synctree_->Branch("pt_2", &pt_2_.var_float, "pt_2/F");
      // phi
      synctree_->Branch("phi_2", &phi_2_.var_float, "phi_2/F");
      // eta
      synctree_->Branch("eta_2", &eta_2_.var_float, "eta_2/F");
      // mass
      synctree_->Branch("m_2", &m_2_, "lM2/F");
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
          synctree_->Branch("againstElectronLooseMVA5_1", &lagainstElectronLooseMVA_1, "againstElectronLooseMVA5_1/F");
          synctree_->Branch("againstElectronMediumMVA5_1", &lagainstElectronMediumMVA_1, "againstElectronMediumMVA5_1/F");
          synctree_->Branch("againstElectronTightMVA5_1", &lagainstElectronTightMVA_1, "againstElectronTightMVA5_1/F");
          synctree_->Branch("againstElectronVLooseMVA5_1", &lagainstElectronVLooseMVA_1, "againstElectronVLooseMVA5_1/F");
          synctree_->Branch("againstElectronVTightMVA5_1", &lagainstElectronVTightMVA_1, "againstElectronVTightMVA5_1/F");
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
          synctree_->Branch("againstElectronLooseMVA5_1", &lagainstElectronLooseMVA_1, "againstElectronLooseMVA5_1/F");
          synctree_->Branch("againstElectronLooseMVA5_2", &lagainstElectronLooseMVA_2, "againstElectronLooseMVA5_2/F");
          synctree_->Branch("againstElectronMediumMVA5_2", &lagainstElectronMediumMVA_2, "againstElectronMediumMVA5_2/F");
          synctree_->Branch("againstElectronTightMVA5_2", &lagainstElectronTightMVA_2, "againstElectronTightMVA5_2/F");
          synctree_->Branch("againstElectronVLooseMVA5_2", &lagainstElectronVLooseMVA_2, "againstElectronVLooseMVA5_2/F");
          synctree_->Branch("againstElectronVTightMVA5_2", &lagainstElectronVTightMVA_2, "againstElectronVTightMVA5_2/F");
          synctree_->Branch("againstMuonLoose3_2", &lagainstMuonLoose3_2, "againstMuonLoose3_2/F");
          synctree_->Branch("againstMuonTight3_2", &lagainstMuonTight3_2, "againstMuonTight3_2/F");
          synctree_->Branch("chargedIsoPtSum_2", &lchargedIsoPtSum_2,"chargedIsoPtSum_2/F");
          synctree_->Branch("neutralIsoPtSum_2", &lneutralIsoPtSum_2,"neutralIsoPtSum_2/F");
          synctree_->Branch("puCorrPtSum_2", &lpuCorrPtSum_2,"puCorrPtSum_2/F");
          synctree_->Branch("decayModeFindingOldDMs_2",&ldecayModeFindingOldDMs_2,"decayModeFindingOldDMs_2/O");

      }
      if(strategy_ == strategy::fall15||strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16  || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
          synctree_->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_1", &l3Hits_1,
                         "byCombinedIsolationDeltaBetaCorrRaw3Hits_1/F");
          synctree_->Branch("byIsolationMVA3newDMwoLTraw_1", &lbyIsolationMVA3newDMwoLTraw_1,"byIsolationMVA3newDMwoLTraw_1/F");
          synctree_->Branch("byIsolationMVA3oldDMwoLTraw_1", &lbyIsolationMVA3oldDMwoLTraw_1,"byIsolationMVA3oldDMwoLTraw_1/F");
          synctree_->Branch("byIsolationMVA3newDMwLTraw_1", &lbyIsolationMVA3newDMwLTraw_1,"byIsolationMVA3newDMwLTraw_1/F");
          synctree_->Branch("againstElectronLooseMVA6_1", &lagainstElectronLooseMVA_1, "againstElectronLooseMVA6_1/F");
          synctree_->Branch("againstElectronMediumMVA6_1", &lagainstElectronMediumMVA_1, "againstElectronMediumMVA6_1/F");
          synctree_->Branch("againstElectronTightMVA6_1", &lagainstElectronTightMVA_1, "againstElectronTightMVA6_1/F");
          synctree_->Branch("againstElectronVLooseMVA6_1", &lagainstElectronVLooseMVA_1, "againstElectronVLooseMVA6_1/F");
          synctree_->Branch("againstElectronVTightMVA6_1", &lagainstElectronVTightMVA_1, "againstElectronVTightMVA6_1/F");
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
          
          if(strategy_==strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
            if(channel_==channel::tt) synctree_->Branch("byIsolationMVA3oldDMwLTraw_1", &iso_1_);  
            synctree_->Branch("byIsolationMVA3oldDMwLTraw_2", &iso_2_);
          } else {
            synctree_->Branch("byIsolationMVA3oldDMwLTraw_1", &lbyIsolationMVA3oldDMwLTraw_1,"byIsolationMVA3oldDMwLTraw_1/F");
            synctree_->Branch("byIsolationMVA3oldDMwLTraw_2", &lbyIsolationMVA3oldDMwLTraw_2,"byIsolationMVA3oldDMwLTraw_2/F");
          }
          synctree_->Branch("byIsolationMVA3newDMwLTraw_2", &lbyIsolationMVA3newDMwLTraw_2,"byIsolationMVA3newDMwLTraw_2/F");
          synctree_->Branch("againstElectronLooseMVA6_1", &lagainstElectronLooseMVA_1, "againstElectronLooseMVA6_1/F");
          synctree_->Branch("againstElectronLooseMVA6_2", &lagainstElectronLooseMVA_2, "againstElectronLooseMVA6_2/F");
          synctree_->Branch("againstElectronMediumMVA6_2", &lagainstElectronMediumMVA_2, "againstElectronMediumMVA6_2/F");
          synctree_->Branch("againstElectronTightMVA6_2", &lagainstElectronTightMVA_2, "againstElectronTightMVA6_2/F");
          synctree_->Branch("againstElectronVLooseMVA6_2", &lagainstElectronVLooseMVA_2, "againstElectronVLooseMVA6_2/F");
          synctree_->Branch("againstElectronVTightMVA6_2", &lagainstElectronVTightMVA_2, "againstElectronVTightMVA6_2/F");
          synctree_->Branch("againstMuonLoose3_2", &lagainstMuonLoose3_2, "againstMuonLoose3_2/F");
          synctree_->Branch("againstMuonTight3_2", &lagainstMuonTight3_2, "againstMuonTight3_2/F");
          synctree_->Branch("chargedIsoPtSum_2", &lchargedIsoPtSum_2,"chargedIsoPtSum_2/F");
          synctree_->Branch("neutralIsoPtSum_2", &lneutralIsoPtSum_2,"neutralIsoPtSum_2/F");
          synctree_->Branch("puCorrPtSum_2", &lpuCorrPtSum_2,"puCorrPtSum_2/F");
          synctree_->Branch("decayModeFindingOldDMs_2",&ldecayModeFindingOldDMs_2,"decayModeFindingOldDMs_2/O");

      }
      synctree_->Branch("met", &met_.var_float, "met/F");
      synctree_->Branch("metphi", &met_phi_.var_float, "met_phi/F");
      synctree_->Branch("uncorrmet", &uncorrmet_.var_float, "met/F");
      synctree_->Branch("uncorrmetphi", &uncorrmet_phi_.var_float, "met_phi/F");
      // Elements of the PF MET covariance matrix (not used in analysis)
      synctree_->Branch("metcov00", &metCov00_, "metCov00/F");
      synctree_->Branch("metcov01", &metCov01_, "metCov01/F");
      synctree_->Branch("metcov10", &metCov10_, "metCov10/F");
      synctree_->Branch("metcov11", &metCov11_, "metCov11/F");
      //Puppi Met
      synctree_->Branch("puppimet",&puppimet_.var_float, "puppimet/F");
      synctree_->Branch("puppimetphi", &puppimet_phi_,"puppimet_phi/F");
      //Gen/vis px/py for recoil corrections:
      synctree_->Branch("genpX", &gen_px_, "genpX/F");
      synctree_->Branch("genpY", &gen_py_, "genpY/F");
      synctree_->Branch("vispX", &vis_px_, "vispX/F");
      synctree_->Branch("vispY", &vis_py_, "vispY/F");

      // pt of the di-tau + MET system
      synctree_->Branch("pt_tt", &pt_tt_.var_float, "pt_tt/F");
      //mt total
      synctree_->Branch("mt_tot", &mt_tot_.var_float, "mt_tot/F");

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
      // are not counted NOTE in fully hadronic: CSV loose b-tagged jets!

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
      
      synctree_->Branch("trg_singleelectron",    &trg_singleelectron_);
      synctree_->Branch("trg_singlemuon",    &trg_singlemuon_);
      synctree_->Branch("trg_doubletau",    &trg_doubletau_);
      synctree_->Branch("trg_muonelectron",    &trg_muonelectron_);
      synctree_->Branch("trg_singletau_1",    &trg_singletau_1_);
      synctree_->Branch("trg_singletau_2",    &trg_singletau_2_);
      synctree_->Branch("trg_mutaucross", &trg_mutaucross_);
      synctree_->Branch("trg_etaucross", &trg_etaucross_);
      
      synctree_->Branch("flagMETFilter", &flagMETFilter_);

    }
    return 0;
  }

  int HTTCategories::Execute(TreeEvent *event) {
    
    if (event->Exists("trg_singleelectron")) trg_singleelectron_ = event->Get<bool>("trg_singleelectron");
    if (event->Exists("trg_singlemuon"))     trg_singlemuon_     = event->Get<bool>("trg_singlemuon");
    if (event->Exists("trg_doubletau"))      trg_doubletau_      = event->Get<bool>("trg_doubletau");
    if (event->Exists("trg_vbfdoubletau"))   trg_vbfdoubletau_   = event->Get<bool>("trg_vbfdoubletau");
    if (event->Exists("trg_muonelectron"))   trg_muonelectron_   = event->Get<bool>("trg_muonelectron");
    if (event->Exists("trg_muonelectron_1"))   trg_muonelectron_1_   = event->Get<bool>("trg_muonelectron_1");
    if (event->Exists("trg_muonelectron_2"))   trg_muonelectron_2_   = event->Get<bool>("trg_muonelectron_2");
    if (event->Exists("trg_muonelectron_3"))   trg_muonelectron_3_   = event->Get<bool>("trg_muonelectron_3");
    if (event->Exists("trg_singletau_1"))    trg_singletau_1_      = event->Get<bool>("trg_singletau_1");
    if (event->Exists("trg_singletau_2"))    trg_singletau_2_      = event->Get<bool>("trg_singletau_2");
    if (event->Exists("trg_mutaucross"))     trg_mutaucross_ = event->Get<bool>("trg_mutaucross");
    if (event->Exists("trg_etaucross"))     trg_etaucross_ = event->Get<bool>("trg_etaucross");
   
    if(!trg_applied_in_mc_) {
      trg_singleelectron_ = true;
      trg_singlemuon_     = true;
      trg_doubletau_      = true;
      trg_vbfdoubletau_   = true;
      trg_muonelectron_   = true;
      trg_singletau_1_      = true;
      trg_singletau_2_      = true; 
      trg_mutaucross_ = true;
      trg_etaucross_ = true;
    } 

    if (event->Exists("flagMETFilter")) flagMETFilter_ = event->Get<bool>("flagMETFilter"); 
    else flagMETFilter_ = false;

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    
    wt_ = {eventInfo->total_weight(), static_cast<float>(eventInfo->total_weight())};
    wt_dysoup_ = eventInfo->weight_defined("dysoup") ? eventInfo->weight("dysoup") : 1.0;
    wt_dysoup_ = eventInfo->weight_defined("ggHsoup") ? eventInfo->weight("ggHsoup") : wt_dysoup_;

    // adding some gen stuff tempoarily can be deleted later
    std::vector<double> parton_pt_vec;
    npNLO_ = eventInfo->npNLO();
    parton_mjj_=-9999;
    parton_pt_=-9999;
    parton_pt_2_=-9999;
    parton_pt_3_=-9999;
    partons_=0;
    bool lhe_exists = event->ExistsInTree("lheParticles");
    if(lhe_exists){
      std::vector<GenParticle*> const& lhe_parts = event->GetPtrVec<GenParticle>("lheParticles");
      parton_pt_=-9999;
      std::vector<GenParticle*> outparts;
      for(unsigned i = 0; i< lhe_parts.size(); ++i){
           if(lhe_parts[i]->status() != 1) continue;
           unsigned id = abs(lhe_parts[i]->pdgid());
           if ((id >= 1 && id <=6) || id == 21){
             outparts.push_back(lhe_parts[i]);
             partons_++;
             parton_pt_vec.push_back(lhe_parts[i]->pt());
        }
      }
      //std::sort(outparts.begin(),outparts.end(),PtComparatorGenPart());
      if(outparts.size()>1) parton_mjj_ = (outparts[0]->vector()+outparts[1]->vector()).M();
      else parton_mjj_ = -9999;
      if(outparts.size()>2){
        double parton_mjj_2 = (outparts[0]->vector()+outparts[2]->vector()).M();
        double parton_mjj_3 = (outparts[1]->vector()+outparts[2]->vector()).M();
        if(parton_mjj_ < std::max(parton_mjj_2, parton_mjj_3)) parton_mjj_ = std::max(parton_mjj_2, parton_mjj_3);

      }
    }
    std::sort(parton_pt_vec.begin(),parton_pt_vec.end());
    std::reverse(parton_pt_vec.begin(),parton_pt_vec.end());
    if (parton_pt_vec.size()>0) parton_pt_ = parton_pt_vec[0];
    else parton_pt_ = -9999;
    if (parton_pt_vec.size()>1) parton_pt_2_ = parton_pt_vec[1];
    else parton_pt_2_ = -9999;
    if (parton_pt_vec.size()>2) parton_pt_3_ = parton_pt_vec[2];
    else parton_pt_3_ = -9999;
    // end of added gen stuff

    //std::cout << (unsigned long long) eventInfo->event() << std::endl; 
    //eventInfo->print_all_weights();
   
    wt_tau_id_dm0_up_ =  (event->Exists("wt_tau_id_dm0_up")) ? event->Get<double>("wt_tau_id_dm0_up") : 1.;
    wt_tau_id_dm1_up_ =  (event->Exists("wt_tau_id_dm1_up")) ? event->Get<double>("wt_tau_id_dm1_up") : 1.;
    wt_tau_id_dm10_up_ =  (event->Exists("wt_tau_id_dm10_up")) ? event->Get<double>("wt_tau_id_dm10_up") : 1.;
    wt_tau_id_dm11_up_ =  (event->Exists("wt_tau_id_dm11_up")) ? event->Get<double>("wt_tau_id_dm11_up") : 1.;
    wt_tau_id_dm0_down_ =  (event->Exists("wt_tau_id_dm0_down")) ? event->Get<double>("wt_tau_id_dm0_down") : 1.;
    wt_tau_id_dm1_down_ =  (event->Exists("wt_tau_id_dm1_down")) ? event->Get<double>("wt_tau_id_dm1_down") : 1.;
    wt_tau_id_dm10_down_ =  (event->Exists("wt_tau_id_dm10_down")) ? event->Get<double>("wt_tau_id_dm10_down") : 1.;
    wt_tau_id_dm11_down_ =  (event->Exists("wt_tau_id_dm11_down")) ? event->Get<double>("wt_tau_id_dm11_down") : 1.;

    wt_tau_id_pt_bin1_up_ =  (event->Exists("wt_tau_id_pt_bin1_up")) ? event->Get<double>("wt_tau_id_pt_bin1_up") : 1.;
    wt_tau_id_pt_bin2_up_ =  (event->Exists("wt_tau_id_pt_bin2_up")) ? event->Get<double>("wt_tau_id_pt_bin2_up") : 1.;
    wt_tau_id_pt_bin3_up_ =  (event->Exists("wt_tau_id_pt_bin3_up")) ? event->Get<double>("wt_tau_id_pt_bin3_up") : 1.;
    wt_tau_id_pt_bin4_up_ =  (event->Exists("wt_tau_id_pt_bin4_up")) ? event->Get<double>("wt_tau_id_pt_bin4_up") : 1.;
    wt_tau_id_pt_bin5_up_ =  (event->Exists("wt_tau_id_pt_bin5_up")) ? event->Get<double>("wt_tau_id_pt_bin5_up") : 1.;
    wt_tau_id_pt_bin1_down_ =  (event->Exists("wt_tau_id_pt_bin1_down")) ? event->Get<double>("wt_tau_id_pt_bin1_down") : 1.;
    wt_tau_id_pt_bin2_down_ =  (event->Exists("wt_tau_id_pt_bin2_down")) ? event->Get<double>("wt_tau_id_pt_bin2_down") : 1.;
    wt_tau_id_pt_bin3_down_ =  (event->Exists("wt_tau_id_pt_bin3_down")) ? event->Get<double>("wt_tau_id_pt_bin3_down") : 1.;
    wt_tau_id_pt_bin4_down_ =  (event->Exists("wt_tau_id_pt_bin4_down")) ? event->Get<double>("wt_tau_id_pt_bin4_down") : 1.;
    wt_tau_id_pt_bin5_down_ =  (event->Exists("wt_tau_id_pt_bin5_down")) ? event->Get<double>("wt_tau_id_pt_bin5_down") : 1.;

    wt_tau_trg_dm0_up_ =  (event->Exists("wt_tau_trg_dm0_up")) ? event->Get<double>("wt_tau_trg_dm0_up") : 1.;
    wt_tau_trg_dm1_up_ =  (event->Exists("wt_tau_trg_dm1_up")) ? event->Get<double>("wt_tau_trg_dm1_up") : 1.;
    wt_tau_trg_dm10_up_ =  (event->Exists("wt_tau_trg_dm10_up")) ? event->Get<double>("wt_tau_trg_dm10_up") : 1.;
    wt_tau_trg_dm11_up_ =  (event->Exists("wt_tau_trg_dm11_up")) ? event->Get<double>("wt_tau_trg_dm11_up") : 1.;
    wt_tau_trg_dm0_down_ =  (event->Exists("wt_tau_trg_dm0_down")) ? event->Get<double>("wt_tau_trg_dm0_down") : 1.;
    wt_tau_trg_dm1_down_ =  (event->Exists("wt_tau_trg_dm1_down")) ? event->Get<double>("wt_tau_trg_dm1_down") : 1.;
    wt_tau_trg_dm10_down_ =  (event->Exists("wt_tau_trg_dm10_down")) ? event->Get<double>("wt_tau_trg_dm10_down") : 1.;
    wt_tau_trg_dm11_down_ =  (event->Exists("wt_tau_trg_dm11_down")) ? event->Get<double>("wt_tau_trg_dm11_down") : 1.;

    if(do_mssm_higgspt_){
      wt_ggh_t_ = event->Exists("wt_ggh_t") ? event->Get<double>("wt_ggh_t") : 1.0;
      wt_ggh_b_ = event->Exists("wt_ggh_b") ? event->Get<double>("wt_ggh_b") : 1.0;
      wt_ggh_i_ = event->Exists("wt_ggh_i") ? event->Get<double>("wt_ggh_i") : 1.0;
      wt_ggH_t_ = event->Exists("wt_ggH_t") ? event->Get<double>("wt_ggH_t") : 1.0;
      wt_ggH_b_ = event->Exists("wt_ggH_b") ? event->Get<double>("wt_ggH_b") : 1.0;
      wt_ggH_i_ = event->Exists("wt_ggH_i") ? event->Get<double>("wt_ggH_i") : 1.0;
      wt_ggA_t_ = event->Exists("wt_ggA_t") ? event->Get<double>("wt_ggA_t") : 1.0;
      wt_ggA_b_ = event->Exists("wt_ggA_b") ? event->Get<double>("wt_ggA_b") : 1.0;
      wt_ggA_i_ = event->Exists("wt_ggA_i") ? event->Get<double>("wt_ggA_i") : 1.0;   
    }
    wt_quarkmass_ = 1.0;
    wt_quarkmass_up_ = 1.0;
    wt_quarkmass_down_ = 1.0;
    wt_fullquarkmass_ =1.0;
    if(event->Exists("wt_quarkmass")) wt_quarkmass_ = event->Get<double>("wt_quarkmass");
    if(event->Exists("wt_quarkmass_up")) wt_quarkmass_up_ = event->Get<double>("wt_quarkmass_up");
    if(event->Exists("wt_quarkmass_down")) wt_quarkmass_down_ = event->Get<double>("wt_quarkmass_down");
    if(event->Exists("wt_fullquarkmass")) wt_fullquarkmass_ = event->Get<double>("wt_fullquarkmass");

    if(do_sm_ps_wts_ && !systematic_shift_){
        wt_ps_up_    = event->Exists("wt_ps_up") ? event->Get<double>("wt_ps_up") : 1.0;
        wt_ps_down_  = event->Exists("wt_ps_down") ? event->Get<double>("wt_ps_down") : 1.0;
        wt_ue_up_    = event->Exists("wt_ue_up") ? event->Get<double>("wt_ue_up") : 1.0;
        wt_ue_down_  = event->Exists("wt_ue_down") ? event->Get<double>("wt_ue_down") : 1.0;
    }

    if(strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
      wt_prefire_ = event->Exists("wt_prefire") ? event->Get<double>("wt_prefire") : 1.0;
      wt_prefire_up_ = event->Exists("wt_prefire_up") ? event->Get<double>("wt_prefire_up") : 1.0;
      wt_prefire_down_ = event->Exists("wt_prefire_down") ? event->Get<double>("wt_prefire_down") : 1.0;
    }
   
    looseiso_wt_ = event->Exists("looseiso_wt") ? event->Get<double>("looseiso_wt") : 1.0;
 
    
    run_ = eventInfo->run();
    event_ = (unsigned long long) eventInfo->event();
    lumi_ = eventInfo->lumi_block();
    rand->SetSeed(event_);
    rand_ = rand->Uniform();
  
      // fake-factor weights        
      if (do_ff_weights_ && (channel_ == channel::et || channel_ == channel::mt || channel_ == channel::tt)){
        if(strategy_ == strategy::mssmsummer16){  
          if(channel_ == channel::et || channel_ == channel::mt){
            if(event->Exists("wt_ff_inclusive")) ff_weight_inclusive_ = event->Get<double>("wt_ff_inclusive");
            if(event->Exists("wt_ff_nobtag_tight"   )) ff_weight_nobtag_tight_    = event->Get<double>("wt_ff_nobtag_tight"   );
            if(event->Exists("wt_ff_nobtag_loosemt" )) ff_weight_nobtag_loosemt_  = event->Get<double>("wt_ff_nobtag_loosemt" );
            if(event->Exists("wt_ff_nobtag_looseiso")) ff_weight_nobtag_looseiso_ = event->Get<double>("wt_ff_nobtag_looseiso");
            if(event->Exists("wt_ff_btag_tight"     )) ff_weight_btag_tight_      = event->Get<double>("wt_ff_btag_tight"     );
            if(event->Exists("wt_ff_btag_loosemt"   )) ff_weight_btag_loosemt_    = event->Get<double>("wt_ff_btag_loosemt"   );
            if(event->Exists("wt_ff_btag_looseiso"  )) ff_weight_btag_looseiso_   = event->Get<double>("wt_ff_btag_looseiso"  );
          } else if(channel_ == channel::tt){
            if(event->Exists("wt_ff_inclusive"  )) ff_weight_inclusive_   = event->Get<double>("wt_ff_inclusive"   );
            if(event->Exists("wt_ff_inclusive_2")) ff_weight_inclusive_2_ = event->Get<double>("wt_ff_inclusive_2");
            if(event->Exists("wt_ff_nobtag"     )) ff_weight_nobtag_      = event->Get<double>("wt_ff_nobtag"     );
            if(event->Exists("wt_ff_nobtag_2"   )) ff_weight_nobtag_2_    = event->Get<double>("wt_ff_nobtag_2"   );
            if(event->Exists("wt_ff_btag"       )) ff_weight_btag_        = event->Get<double>("wt_ff_btag"       );
            if(event->Exists("wt_ff_btag_2"     )) ff_weight_btag_2_      = event->Get<double>("wt_ff_btag_2"     );
          }
        
        
        if(do_ff_systematics_){
          if(channel_ == channel::et || channel_ == channel::mt){
            if(event->Exists("wt_inclusive_ff_qcd_syst_up"            )) wt_ff_inclusive_qcd_syst_up_             = event->Get<double>("wt_inclusive_ff_qcd_syst_up"            );
            if(event->Exists("wt_inclusive_ff_qcd_syst_down"          )) wt_ff_inclusive_qcd_syst_down_           = event->Get<double>("wt_inclusive_ff_qcd_syst_down"          );
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet0_stat_up"  )) wt_ff_inclusive_qcd_dm0_njet0_stat_up_   = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet0_stat_up"  );
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet0_stat_down")) wt_ff_inclusive_qcd_dm0_njet0_stat_down_ = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet0_stat_down");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet1_stat_up"  )) wt_ff_inclusive_qcd_dm0_njet1_stat_up_   = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet1_stat_up"  );
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet1_stat_down")) wt_ff_inclusive_qcd_dm0_njet1_stat_down_ = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet1_stat_down");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet0_stat_up"  )) wt_ff_inclusive_qcd_dm1_njet0_stat_up_   = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet0_stat_up"  );
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet0_stat_down")) wt_ff_inclusive_qcd_dm1_njet0_stat_down_ = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet0_stat_down");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet1_stat_up"  )) wt_ff_inclusive_qcd_dm1_njet1_stat_up_   = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet1_stat_up"  );
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet1_stat_down")) wt_ff_inclusive_qcd_dm1_njet1_stat_down_ = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet1_stat_down");
            if(event->Exists("wt_inclusive_ff_w_syst_up"              )) wt_ff_inclusive_w_syst_up_               = event->Get<double>("wt_inclusive_ff_w_syst_up"              );
            if(event->Exists("wt_inclusive_ff_w_syst_down"            )) wt_ff_inclusive_w_syst_down_             = event->Get<double>("wt_inclusive_ff_w_syst_down"            );
            if(event->Exists("wt_inclusive_ff_w_dm0_njet0_stat_up"    )) wt_ff_inclusive_w_dm0_njet0_stat_up_     = event->Get<double>("wt_inclusive_ff_w_dm0_njet0_stat_up"    );
            if(event->Exists("wt_inclusive_ff_w_dm0_njet0_stat_down"  )) wt_ff_inclusive_w_dm0_njet0_stat_down_   = event->Get<double>("wt_inclusive_ff_w_dm0_njet0_stat_down"  );
            if(event->Exists("wt_inclusive_ff_w_dm0_njet1_stat_up"    )) wt_ff_inclusive_w_dm0_njet1_stat_up_     = event->Get<double>("wt_inclusive_ff_w_dm0_njet1_stat_up"    );
            if(event->Exists("wt_inclusive_ff_w_dm0_njet1_stat_down"  )) wt_ff_inclusive_w_dm0_njet1_stat_down_   = event->Get<double>("wt_inclusive_ff_w_dm0_njet1_stat_down"  );
            if(event->Exists("wt_inclusive_ff_w_dm1_njet0_stat_up"    )) wt_ff_inclusive_w_dm1_njet0_stat_up_     = event->Get<double>("wt_inclusive_ff_w_dm1_njet0_stat_up"    );
            if(event->Exists("wt_inclusive_ff_w_dm1_njet0_stat_down"  )) wt_ff_inclusive_w_dm1_njet0_stat_down_   = event->Get<double>("wt_inclusive_ff_w_dm1_njet0_stat_down"  );
            if(event->Exists("wt_inclusive_ff_w_dm1_njet1_stat_up"    )) wt_ff_inclusive_w_dm1_njet1_stat_up_     = event->Get<double>("wt_inclusive_ff_w_dm1_njet1_stat_up"    );
            if(event->Exists("wt_inclusive_ff_w_dm1_njet1_stat_down"  )) wt_ff_inclusive_w_dm1_njet1_stat_down_   = event->Get<double>("wt_inclusive_ff_w_dm1_njet1_stat_down"  );
            if(event->Exists("wt_inclusive_ff_tt_syst_up"             )) wt_ff_inclusive_tt_syst_up_              = event->Get<double>("wt_inclusive_ff_tt_syst_up"             );
            if(event->Exists("wt_inclusive_ff_tt_syst_down"           )) wt_ff_inclusive_tt_syst_down_            = event->Get<double>("wt_inclusive_ff_tt_syst_down"           );
            if(event->Exists("wt_inclusive_ff_tt_dm0_njet0_stat_up"   )) wt_ff_inclusive_tt_dm0_njet0_stat_up_    = event->Get<double>("wt_inclusive_ff_tt_dm0_njet0_stat_up"   );
            if(event->Exists("wt_inclusive_ff_tt_dm0_njet0_stat_down" )) wt_ff_inclusive_tt_dm0_njet0_stat_down_  = event->Get<double>("wt_inclusive_ff_tt_dm0_njet0_stat_down" );
            if(event->Exists("wt_inclusive_ff_tt_dm0_njet1_stat_up"   )) wt_ff_inclusive_tt_dm0_njet1_stat_up_    = event->Get<double>("wt_inclusive_ff_tt_dm0_njet1_stat_up"   );
            if(event->Exists("wt_inclusive_ff_tt_dm0_njet1_stat_down" )) wt_ff_inclusive_tt_dm0_njet1_stat_down_  = event->Get<double>("wt_inclusive_ff_tt_dm0_njet1_stat_down" );
            if(event->Exists("wt_inclusive_ff_tt_dm1_njet0_stat_up"   )) wt_ff_inclusive_tt_dm1_njet0_stat_up_    = event->Get<double>("wt_inclusive_ff_tt_dm1_njet0_stat_up"   );
            if(event->Exists("wt_inclusive_ff_tt_dm1_njet0_stat_down" )) wt_ff_inclusive_tt_dm1_njet0_stat_down_  = event->Get<double>("wt_inclusive_ff_tt_dm1_njet0_stat_down" );
            if(event->Exists("wt_inclusive_ff_tt_dm1_njet1_stat_up"   )) wt_ff_inclusive_tt_dm1_njet1_stat_up_    = event->Get<double>("wt_inclusive_ff_tt_dm1_njet1_stat_up"   );
            if(event->Exists("wt_inclusive_ff_tt_dm1_njet1_stat_down" )) wt_ff_inclusive_tt_dm1_njet1_stat_down_  = event->Get<double>("wt_inclusive_ff_tt_dm1_njet1_stat_down" );
            
            if(event->Exists("wt_nobtag_ff_qcd_syst_up"               )) wt_ff_nobtag_qcd_syst_up_                = event->Get<double>("wt_nobtag_ff_qcd_syst_up"               );
            if(event->Exists("wt_nobtag_ff_qcd_syst_down"             )) wt_ff_nobtag_qcd_syst_down_              = event->Get<double>("wt_nobtag_ff_qcd_syst_down"             );
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet0_stat_up"     )) wt_ff_nobtag_qcd_dm0_njet0_stat_up_      = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet0_stat_down"   )) wt_ff_nobtag_qcd_dm0_njet0_stat_down_    = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet1_stat_up"     )) wt_ff_nobtag_qcd_dm0_njet1_stat_up_      = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet1_stat_down"   )) wt_ff_nobtag_qcd_dm0_njet1_stat_down_    = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet0_stat_up"     )) wt_ff_nobtag_qcd_dm1_njet0_stat_up_      = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet0_stat_down"   )) wt_ff_nobtag_qcd_dm1_njet0_stat_down_    = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet1_stat_up"     )) wt_ff_nobtag_qcd_dm1_njet1_stat_up_      = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet1_stat_down"   )) wt_ff_nobtag_qcd_dm1_njet1_stat_down_    = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_ff_w_syst_up"                 )) wt_ff_nobtag_w_syst_up_                  = event->Get<double>("wt_nobtag_ff_w_syst_up"                 );
            if(event->Exists("wt_nobtag_ff_w_syst_down"               )) wt_ff_nobtag_w_syst_down_                = event->Get<double>("wt_nobtag_ff_w_syst_down"               );
            if(event->Exists("wt_nobtag_ff_w_dm0_njet0_stat_up"       )) wt_ff_nobtag_w_dm0_njet0_stat_up_        = event->Get<double>("wt_nobtag_ff_w_dm0_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_ff_w_dm0_njet0_stat_down"     )) wt_ff_nobtag_w_dm0_njet0_stat_down_      = event->Get<double>("wt_nobtag_ff_w_dm0_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_ff_w_dm0_njet1_stat_up"       )) wt_ff_nobtag_w_dm0_njet1_stat_up_        = event->Get<double>("wt_nobtag_ff_w_dm0_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_ff_w_dm0_njet1_stat_down"     )) wt_ff_nobtag_w_dm0_njet1_stat_down_      = event->Get<double>("wt_nobtag_ff_w_dm0_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_ff_w_dm1_njet0_stat_up"       )) wt_ff_nobtag_w_dm1_njet0_stat_up_        = event->Get<double>("wt_nobtag_ff_w_dm1_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_ff_w_dm1_njet0_stat_down"     )) wt_ff_nobtag_w_dm1_njet0_stat_down_      = event->Get<double>("wt_nobtag_ff_w_dm1_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_ff_w_dm1_njet1_stat_up"       )) wt_ff_nobtag_w_dm1_njet1_stat_up_        = event->Get<double>("wt_nobtag_ff_w_dm1_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_ff_w_dm1_njet1_stat_down"     )) wt_ff_nobtag_w_dm1_njet1_stat_down_      = event->Get<double>("wt_nobtag_ff_w_dm1_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_ff_tt_syst_up"                )) wt_ff_nobtag_tt_syst_up_                 = event->Get<double>("wt_nobtag_ff_tt_syst_up"                );
            if(event->Exists("wt_nobtag_ff_tt_syst_down"              )) wt_ff_nobtag_tt_syst_down_               = event->Get<double>("wt_nobtag_ff_tt_syst_down"              );
            if(event->Exists("wt_nobtag_ff_tt_dm0_njet0_stat_up"      )) wt_ff_nobtag_tt_dm0_njet0_stat_up_       = event->Get<double>("wt_nobtag_ff_tt_dm0_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_ff_tt_dm0_njet0_stat_down"    )) wt_ff_nobtag_tt_dm0_njet0_stat_down_     = event->Get<double>("wt_nobtag_ff_tt_dm0_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_ff_tt_dm0_njet1_stat_up"      )) wt_ff_nobtag_tt_dm0_njet1_stat_up_       = event->Get<double>("wt_nobtag_ff_tt_dm0_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_ff_tt_dm0_njet1_stat_down"    )) wt_ff_nobtag_tt_dm0_njet1_stat_down_     = event->Get<double>("wt_nobtag_ff_tt_dm0_njet1_stat_down"    );
            if(event->Exists("wt_nobtag_ff_tt_dm1_njet0_stat_up"      )) wt_ff_nobtag_tt_dm1_njet0_stat_up_       = event->Get<double>("wt_nobtag_ff_tt_dm1_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_ff_tt_dm1_njet0_stat_down"    )) wt_ff_nobtag_tt_dm1_njet0_stat_down_     = event->Get<double>("wt_nobtag_ff_tt_dm1_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_ff_tt_dm1_njet1_stat_up"      )) wt_ff_nobtag_tt_dm1_njet1_stat_up_       = event->Get<double>("wt_nobtag_ff_tt_dm1_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_ff_tt_dm1_njet1_stat_down"    )) wt_ff_nobtag_tt_dm1_njet1_stat_down_     = event->Get<double>("wt_nobtag_ff_tt_dm1_njet1_stat_down"    );
            if(event->Exists("wt_btag_ff_qcd_syst_up"                 )) wt_ff_btag_qcd_syst_up_                  = event->Get<double>("wt_btag_ff_qcd_syst_up"                 );
            if(event->Exists("wt_btag_ff_qcd_syst_down"               )) wt_ff_btag_qcd_syst_down_                = event->Get<double>("wt_btag_ff_qcd_syst_down"               );
            if(event->Exists("wt_btag_ff_qcd_dm0_njet0_stat_up"       )) wt_ff_btag_qcd_dm0_njet0_stat_up_        = event->Get<double>("wt_btag_ff_qcd_dm0_njet0_stat_up"       );
            if(event->Exists("wt_btag_ff_qcd_dm0_njet0_stat_down"     )) wt_ff_btag_qcd_dm0_njet0_stat_down_      = event->Get<double>("wt_btag_ff_qcd_dm0_njet0_stat_down"     );
            if(event->Exists("wt_btag_ff_qcd_dm0_njet1_stat_up"       )) wt_ff_btag_qcd_dm0_njet1_stat_up_        = event->Get<double>("wt_btag_ff_qcd_dm0_njet1_stat_up"       );
            if(event->Exists("wt_btag_ff_qcd_dm0_njet1_stat_down"     )) wt_ff_btag_qcd_dm0_njet1_stat_down_      = event->Get<double>("wt_btag_ff_qcd_dm0_njet1_stat_down"     );
            if(event->Exists("wt_btag_ff_qcd_dm1_njet0_stat_up"       )) wt_ff_btag_qcd_dm1_njet0_stat_up_        = event->Get<double>("wt_btag_ff_qcd_dm1_njet0_stat_up"       );
            if(event->Exists("wt_btag_ff_qcd_dm1_njet0_stat_down"     )) wt_ff_btag_qcd_dm1_njet0_stat_down_      = event->Get<double>("wt_btag_ff_qcd_dm1_njet0_stat_down"     );
            if(event->Exists("wt_btag_ff_qcd_dm1_njet1_stat_up"       )) wt_ff_btag_qcd_dm1_njet1_stat_up_        = event->Get<double>("wt_btag_ff_qcd_dm1_njet1_stat_up"       );
            if(event->Exists("wt_btag_ff_qcd_dm1_njet1_stat_down"     )) wt_ff_btag_qcd_dm1_njet1_stat_down_      = event->Get<double>("wt_btag_ff_qcd_dm1_njet1_stat_down"     );
            if(event->Exists("wt_btag_ff_w_syst_up"                   )) wt_ff_btag_w_syst_up_                    = event->Get<double>("wt_btag_ff_w_syst_up"                   );
            if(event->Exists("wt_btag_ff_w_syst_down"                 )) wt_ff_btag_w_syst_down_                  = event->Get<double>("wt_btag_ff_w_syst_down"                 );
            if(event->Exists("wt_btag_ff_w_dm0_njet0_stat_up"         )) wt_ff_btag_w_dm0_njet0_stat_up_          = event->Get<double>("wt_btag_ff_w_dm0_njet0_stat_up"         );
            if(event->Exists("wt_btag_ff_w_dm0_njet0_stat_down"       )) wt_ff_btag_w_dm0_njet0_stat_down_        = event->Get<double>("wt_btag_ff_w_dm0_njet0_stat_down"       );
            if(event->Exists("wt_btag_ff_w_dm0_njet1_stat_up"         )) wt_ff_btag_w_dm0_njet1_stat_up_          = event->Get<double>("wt_btag_ff_w_dm0_njet1_stat_up"         );
            if(event->Exists("wt_btag_ff_w_dm0_njet1_stat_down"       )) wt_ff_btag_w_dm0_njet1_stat_down_        = event->Get<double>("wt_btag_ff_w_dm0_njet1_stat_down"       );
            if(event->Exists("wt_btag_ff_w_dm1_njet0_stat_up"         )) wt_ff_btag_w_dm1_njet0_stat_up_          = event->Get<double>("wt_btag_ff_w_dm1_njet0_stat_up"         );
            if(event->Exists("wt_btag_ff_w_dm1_njet0_stat_down"       )) wt_ff_btag_w_dm1_njet0_stat_down_        = event->Get<double>("wt_btag_ff_w_dm1_njet0_stat_down"       );
            if(event->Exists("wt_btag_ff_w_dm1_njet1_stat_up"         )) wt_ff_btag_w_dm1_njet1_stat_up_          = event->Get<double>("wt_btag_ff_w_dm1_njet1_stat_up"         );
            if(event->Exists("wt_btag_ff_w_dm1_njet1_stat_down"       )) wt_ff_btag_w_dm1_njet1_stat_down_        = event->Get<double>("wt_btag_ff_w_dm1_njet1_stat_down"       );
            if(event->Exists("wt_btag_ff_tt_syst_up"                  )) wt_ff_btag_tt_syst_up_                   = event->Get<double>("wt_btag_ff_tt_syst_up"                  );
            if(event->Exists("wt_btag_ff_tt_syst_down"                )) wt_ff_btag_tt_syst_down_                 = event->Get<double>("wt_btag_ff_tt_syst_down"                );
            if(event->Exists("wt_btag_ff_tt_dm0_njet0_stat_up"        )) wt_ff_btag_tt_dm0_njet0_stat_up_         = event->Get<double>("wt_btag_ff_tt_dm0_njet0_stat_up"        );
            if(event->Exists("wt_btag_ff_tt_dm0_njet0_stat_down"      )) wt_ff_btag_tt_dm0_njet0_stat_down_       = event->Get<double>("wt_btag_ff_tt_dm0_njet0_stat_down"      );
            if(event->Exists("wt_btag_ff_tt_dm0_njet1_stat_up"        )) wt_ff_btag_tt_dm0_njet1_stat_up_         = event->Get<double>("wt_btag_ff_tt_dm0_njet1_stat_up"        );
            if(event->Exists("wt_btag_ff_tt_dm0_njet1_stat_down"      )) wt_ff_btag_tt_dm0_njet1_stat_down_       = event->Get<double>("wt_btag_ff_tt_dm0_njet1_stat_down"      );
            if(event->Exists("wt_btag_ff_tt_dm1_njet0_stat_up"        )) wt_ff_btag_tt_dm1_njet0_stat_up_         = event->Get<double>("wt_btag_ff_tt_dm1_njet0_stat_up"        );
            if(event->Exists("wt_btag_ff_tt_dm1_njet0_stat_down"      )) wt_ff_btag_tt_dm1_njet0_stat_down_       = event->Get<double>("wt_btag_ff_tt_dm1_njet0_stat_down"      );
            if(event->Exists("wt_btag_ff_tt_dm1_njet1_stat_up"        )) wt_ff_btag_tt_dm1_njet1_stat_up_         = event->Get<double>("wt_btag_ff_tt_dm1_njet1_stat_up"        );
            if(event->Exists("wt_btag_ff_tt_dm1_njet1_stat_down"      )) wt_ff_btag_tt_dm1_njet1_stat_down_       = event->Get<double>("wt_btag_ff_tt_dm1_njet1_stat_down"      );
            
            if(event->Exists("wt_nobtag_tight_ff_qcd_syst_up"               )) wt_ff_nobtag_tight_qcd_syst_up_                = event->Get<double>("wt_nobtag_tight_ff_qcd_syst_up"               );
            if(event->Exists("wt_nobtag_tight_ff_qcd_syst_down"             )) wt_ff_nobtag_tight_qcd_syst_down_              = event->Get<double>("wt_nobtag_tight_ff_qcd_syst_down"             );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm0_njet0_stat_up"     )) wt_ff_nobtag_tight_qcd_dm0_njet0_stat_up_      = event->Get<double>("wt_nobtag_tight_ff_qcd_dm0_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm0_njet0_stat_down"   )) wt_ff_nobtag_tight_qcd_dm0_njet0_stat_down_    = event->Get<double>("wt_nobtag_tight_ff_qcd_dm0_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm0_njet1_stat_up"     )) wt_ff_nobtag_tight_qcd_dm0_njet1_stat_up_      = event->Get<double>("wt_nobtag_tight_ff_qcd_dm0_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm0_njet1_stat_down"   )) wt_ff_nobtag_tight_qcd_dm0_njet1_stat_down_    = event->Get<double>("wt_nobtag_tight_ff_qcd_dm0_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm1_njet0_stat_up"     )) wt_ff_nobtag_tight_qcd_dm1_njet0_stat_up_      = event->Get<double>("wt_nobtag_tight_ff_qcd_dm1_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm1_njet0_stat_down"   )) wt_ff_nobtag_tight_qcd_dm1_njet0_stat_down_    = event->Get<double>("wt_nobtag_tight_ff_qcd_dm1_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm1_njet1_stat_up"     )) wt_ff_nobtag_tight_qcd_dm1_njet1_stat_up_      = event->Get<double>("wt_nobtag_tight_ff_qcd_dm1_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_tight_ff_qcd_dm1_njet1_stat_down"   )) wt_ff_nobtag_tight_qcd_dm1_njet1_stat_down_    = event->Get<double>("wt_nobtag_tight_ff_qcd_dm1_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_tight_ff_w_syst_up"                 )) wt_ff_nobtag_tight_w_syst_up_                  = event->Get<double>("wt_nobtag_tight_ff_w_syst_up"                 );
            if(event->Exists("wt_nobtag_tight_ff_w_syst_down"               )) wt_ff_nobtag_tight_w_syst_down_                = event->Get<double>("wt_nobtag_tight_ff_w_syst_down"               );
            if(event->Exists("wt_nobtag_tight_ff_w_dm0_njet0_stat_up"       )) wt_ff_nobtag_tight_w_dm0_njet0_stat_up_        = event->Get<double>("wt_nobtag_tight_ff_w_dm0_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_tight_ff_w_dm0_njet0_stat_down"     )) wt_ff_nobtag_tight_w_dm0_njet0_stat_down_      = event->Get<double>("wt_nobtag_tight_ff_w_dm0_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_tight_ff_w_dm0_njet1_stat_up"       )) wt_ff_nobtag_tight_w_dm0_njet1_stat_up_        = event->Get<double>("wt_nobtag_tight_ff_w_dm0_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_tight_ff_w_dm0_njet1_stat_down"     )) wt_ff_nobtag_tight_w_dm0_njet1_stat_down_      = event->Get<double>("wt_nobtag_tight_ff_w_dm0_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_tight_ff_w_dm1_njet0_stat_up"       )) wt_ff_nobtag_tight_w_dm1_njet0_stat_up_        = event->Get<double>("wt_nobtag_tight_ff_w_dm1_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_tight_ff_w_dm1_njet0_stat_down"     )) wt_ff_nobtag_tight_w_dm1_njet0_stat_down_      = event->Get<double>("wt_nobtag_tight_ff_w_dm1_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_tight_ff_w_dm1_njet1_stat_up"       )) wt_ff_nobtag_tight_w_dm1_njet1_stat_up_        = event->Get<double>("wt_nobtag_tight_ff_w_dm1_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_tight_ff_w_dm1_njet1_stat_down"     )) wt_ff_nobtag_tight_w_dm1_njet1_stat_down_      = event->Get<double>("wt_nobtag_tight_ff_w_dm1_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_tight_ff_tt_syst_up"                )) wt_ff_nobtag_tight_tt_syst_up_                 = event->Get<double>("wt_nobtag_tight_ff_tt_syst_up"                );
            if(event->Exists("wt_nobtag_tight_ff_tt_syst_down"              )) wt_ff_nobtag_tight_tt_syst_down_               = event->Get<double>("wt_nobtag_tight_ff_tt_syst_down"              );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm0_njet0_stat_up"      )) wt_ff_nobtag_tight_tt_dm0_njet0_stat_up_       = event->Get<double>("wt_nobtag_tight_ff_tt_dm0_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm0_njet0_stat_down"    )) wt_ff_nobtag_tight_tt_dm0_njet0_stat_down_     = event->Get<double>("wt_nobtag_tight_ff_tt_dm0_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm0_njet1_stat_up"      )) wt_ff_nobtag_tight_tt_dm0_njet1_stat_up_       = event->Get<double>("wt_nobtag_tight_ff_tt_dm0_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm0_njet1_stat_down"    )) wt_ff_nobtag_tight_tt_dm0_njet1_stat_down_     = event->Get<double>("wt_nobtag_tight_ff_tt_dm0_njet1_stat_down"    );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm1_njet0_stat_up"      )) wt_ff_nobtag_tight_tt_dm1_njet0_stat_up_       = event->Get<double>("wt_nobtag_tight_ff_tt_dm1_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm1_njet0_stat_down"    )) wt_ff_nobtag_tight_tt_dm1_njet0_stat_down_     = event->Get<double>("wt_nobtag_tight_ff_tt_dm1_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm1_njet1_stat_up"      )) wt_ff_nobtag_tight_tt_dm1_njet1_stat_up_       = event->Get<double>("wt_nobtag_tight_ff_tt_dm1_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_tight_ff_tt_dm1_njet1_stat_down"    )) wt_ff_nobtag_tight_tt_dm1_njet1_stat_down_     = event->Get<double>("wt_nobtag_tight_ff_tt_dm1_njet1_stat_down"    );
            if(event->Exists("wt_btag_tight_ff_qcd_syst_up"                 )) wt_ff_btag_tight_qcd_syst_up_                  = event->Get<double>("wt_btag_tight_ff_qcd_syst_up"                 );
            if(event->Exists("wt_btag_tight_ff_qcd_syst_down"               )) wt_ff_btag_tight_qcd_syst_down_                = event->Get<double>("wt_btag_tight_ff_qcd_syst_down"               );
            if(event->Exists("wt_btag_tight_ff_qcd_dm0_njet0_stat_up"       )) wt_ff_btag_tight_qcd_dm0_njet0_stat_up_        = event->Get<double>("wt_btag_tight_ff_qcd_dm0_njet0_stat_up"       );
            if(event->Exists("wt_btag_tight_ff_qcd_dm0_njet0_stat_down"     )) wt_ff_btag_tight_qcd_dm0_njet0_stat_down_      = event->Get<double>("wt_btag_tight_ff_qcd_dm0_njet0_stat_down"     );
            if(event->Exists("wt_btag_tight_ff_qcd_dm0_njet1_stat_up"       )) wt_ff_btag_tight_qcd_dm0_njet1_stat_up_        = event->Get<double>("wt_btag_tight_ff_qcd_dm0_njet1_stat_up"       );
            if(event->Exists("wt_btag_tight_ff_qcd_dm0_njet1_stat_down"     )) wt_ff_btag_tight_qcd_dm0_njet1_stat_down_      = event->Get<double>("wt_btag_tight_ff_qcd_dm0_njet1_stat_down"     );
            if(event->Exists("wt_btag_tight_ff_qcd_dm1_njet0_stat_up"       )) wt_ff_btag_tight_qcd_dm1_njet0_stat_up_        = event->Get<double>("wt_btag_tight_ff_qcd_dm1_njet0_stat_up"       );
            if(event->Exists("wt_btag_tight_ff_qcd_dm1_njet0_stat_down"     )) wt_ff_btag_tight_qcd_dm1_njet0_stat_down_      = event->Get<double>("wt_btag_tight_ff_qcd_dm1_njet0_stat_down"     );
            if(event->Exists("wt_btag_tight_ff_qcd_dm1_njet1_stat_up"       )) wt_ff_btag_tight_qcd_dm1_njet1_stat_up_        = event->Get<double>("wt_btag_tight_ff_qcd_dm1_njet1_stat_up"       );
            if(event->Exists("wt_btag_tight_ff_qcd_dm1_njet1_stat_down"     )) wt_ff_btag_tight_qcd_dm1_njet1_stat_down_      = event->Get<double>("wt_btag_tight_ff_qcd_dm1_njet1_stat_down"     );
            if(event->Exists("wt_btag_tight_ff_w_syst_up"                   )) wt_ff_btag_tight_w_syst_up_                    = event->Get<double>("wt_btag_tight_ff_w_syst_up"                   );
            if(event->Exists("wt_btag_tight_ff_w_syst_down"                 )) wt_ff_btag_tight_w_syst_down_                  = event->Get<double>("wt_btag_tight_ff_w_syst_down"                 );
            if(event->Exists("wt_btag_tight_ff_w_dm0_njet0_stat_up"         )) wt_ff_btag_tight_w_dm0_njet0_stat_up_          = event->Get<double>("wt_btag_tight_ff_w_dm0_njet0_stat_up"         );
            if(event->Exists("wt_btag_tight_ff_w_dm0_njet0_stat_down"       )) wt_ff_btag_tight_w_dm0_njet0_stat_down_        = event->Get<double>("wt_btag_tight_ff_w_dm0_njet0_stat_down"       );
            if(event->Exists("wt_btag_tight_ff_w_dm0_njet1_stat_up"         )) wt_ff_btag_tight_w_dm0_njet1_stat_up_          = event->Get<double>("wt_btag_tight_ff_w_dm0_njet1_stat_up"         );
            if(event->Exists("wt_btag_tight_ff_w_dm0_njet1_stat_down"       )) wt_ff_btag_tight_w_dm0_njet1_stat_down_        = event->Get<double>("wt_btag_tight_ff_w_dm0_njet1_stat_down"       );
            if(event->Exists("wt_btag_tight_ff_w_dm1_njet0_stat_up"         )) wt_ff_btag_tight_w_dm1_njet0_stat_up_          = event->Get<double>("wt_btag_tight_ff_w_dm1_njet0_stat_up"         );
            if(event->Exists("wt_btag_tight_ff_w_dm1_njet0_stat_down"       )) wt_ff_btag_tight_w_dm1_njet0_stat_down_        = event->Get<double>("wt_btag_tight_ff_w_dm1_njet0_stat_down"       );
            if(event->Exists("wt_btag_tight_ff_w_dm1_njet1_stat_up"         )) wt_ff_btag_tight_w_dm1_njet1_stat_up_          = event->Get<double>("wt_btag_tight_ff_w_dm1_njet1_stat_up"         );
            if(event->Exists("wt_btag_tight_ff_w_dm1_njet1_stat_down"       )) wt_ff_btag_tight_w_dm1_njet1_stat_down_        = event->Get<double>("wt_btag_tight_ff_w_dm1_njet1_stat_down"       );
            if(event->Exists("wt_btag_tight_ff_tt_syst_up"                  )) wt_ff_btag_tight_tt_syst_up_                   = event->Get<double>("wt_btag_tight_ff_tt_syst_up"                  );
            if(event->Exists("wt_btag_tight_ff_tt_syst_down"                )) wt_ff_btag_tight_tt_syst_down_                 = event->Get<double>("wt_btag_tight_ff_tt_syst_down"                );
            if(event->Exists("wt_btag_tight_ff_tt_dm0_njet0_stat_up"        )) wt_ff_btag_tight_tt_dm0_njet0_stat_up_         = event->Get<double>("wt_btag_tight_ff_tt_dm0_njet0_stat_up"        );
            if(event->Exists("wt_btag_tight_ff_tt_dm0_njet0_stat_down"      )) wt_ff_btag_tight_tt_dm0_njet0_stat_down_       = event->Get<double>("wt_btag_tight_ff_tt_dm0_njet0_stat_down"      );
            if(event->Exists("wt_btag_tight_ff_tt_dm0_njet1_stat_up"        )) wt_ff_btag_tight_tt_dm0_njet1_stat_up_         = event->Get<double>("wt_btag_tight_ff_tt_dm0_njet1_stat_up"        );
            if(event->Exists("wt_btag_tight_ff_tt_dm0_njet1_stat_down"      )) wt_ff_btag_tight_tt_dm0_njet1_stat_down_       = event->Get<double>("wt_btag_tight_ff_tt_dm0_njet1_stat_down"      );
            if(event->Exists("wt_btag_tight_ff_tt_dm1_njet0_stat_up"        )) wt_ff_btag_tight_tt_dm1_njet0_stat_up_         = event->Get<double>("wt_btag_tight_ff_tt_dm1_njet0_stat_up"        );
            if(event->Exists("wt_btag_tight_ff_tt_dm1_njet0_stat_down"      )) wt_ff_btag_tight_tt_dm1_njet0_stat_down_       = event->Get<double>("wt_btag_tight_ff_tt_dm1_njet0_stat_down"      );
            if(event->Exists("wt_btag_tight_ff_tt_dm1_njet1_stat_up"        )) wt_ff_btag_tight_tt_dm1_njet1_stat_up_         = event->Get<double>("wt_btag_tight_ff_tt_dm1_njet1_stat_up"        );
            if(event->Exists("wt_btag_tight_ff_tt_dm1_njet1_stat_down"      )) wt_ff_btag_tight_tt_dm1_njet1_stat_down_       = event->Get<double>("wt_btag_tight_ff_tt_dm1_njet1_stat_down"      );
            
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_syst_up"               )) wt_ff_nobtag_loosemt_qcd_syst_up_                = event->Get<double>("wt_nobtag_loosemt_ff_qcd_syst_up"               );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_syst_down"             )) wt_ff_nobtag_loosemt_qcd_syst_down_              = event->Get<double>("wt_nobtag_loosemt_ff_qcd_syst_down"             );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm0_njet0_stat_up"     )) wt_ff_nobtag_loosemt_qcd_dm0_njet0_stat_up_      = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm0_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm0_njet0_stat_down"   )) wt_ff_nobtag_loosemt_qcd_dm0_njet0_stat_down_    = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm0_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm0_njet1_stat_up"     )) wt_ff_nobtag_loosemt_qcd_dm0_njet1_stat_up_      = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm0_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm0_njet1_stat_down"   )) wt_ff_nobtag_loosemt_qcd_dm0_njet1_stat_down_    = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm0_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm1_njet0_stat_up"     )) wt_ff_nobtag_loosemt_qcd_dm1_njet0_stat_up_      = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm1_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm1_njet0_stat_down"   )) wt_ff_nobtag_loosemt_qcd_dm1_njet0_stat_down_    = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm1_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm1_njet1_stat_up"     )) wt_ff_nobtag_loosemt_qcd_dm1_njet1_stat_up_      = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm1_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_loosemt_ff_qcd_dm1_njet1_stat_down"   )) wt_ff_nobtag_loosemt_qcd_dm1_njet1_stat_down_    = event->Get<double>("wt_nobtag_loosemt_ff_qcd_dm1_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_loosemt_ff_w_syst_up"                 )) wt_ff_nobtag_loosemt_w_syst_up_                  = event->Get<double>("wt_nobtag_loosemt_ff_w_syst_up"                 );
            if(event->Exists("wt_nobtag_loosemt_ff_w_syst_down"               )) wt_ff_nobtag_loosemt_w_syst_down_                = event->Get<double>("wt_nobtag_loosemt_ff_w_syst_down"               );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm0_njet0_stat_up"       )) wt_ff_nobtag_loosemt_w_dm0_njet0_stat_up_        = event->Get<double>("wt_nobtag_loosemt_ff_w_dm0_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm0_njet0_stat_down"     )) wt_ff_nobtag_loosemt_w_dm0_njet0_stat_down_      = event->Get<double>("wt_nobtag_loosemt_ff_w_dm0_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm0_njet1_stat_up"       )) wt_ff_nobtag_loosemt_w_dm0_njet1_stat_up_        = event->Get<double>("wt_nobtag_loosemt_ff_w_dm0_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm0_njet1_stat_down"     )) wt_ff_nobtag_loosemt_w_dm0_njet1_stat_down_      = event->Get<double>("wt_nobtag_loosemt_ff_w_dm0_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm1_njet0_stat_up"       )) wt_ff_nobtag_loosemt_w_dm1_njet0_stat_up_        = event->Get<double>("wt_nobtag_loosemt_ff_w_dm1_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm1_njet0_stat_down"     )) wt_ff_nobtag_loosemt_w_dm1_njet0_stat_down_      = event->Get<double>("wt_nobtag_loosemt_ff_w_dm1_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm1_njet1_stat_up"       )) wt_ff_nobtag_loosemt_w_dm1_njet1_stat_up_        = event->Get<double>("wt_nobtag_loosemt_ff_w_dm1_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_loosemt_ff_w_dm1_njet1_stat_down"     )) wt_ff_nobtag_loosemt_w_dm1_njet1_stat_down_      = event->Get<double>("wt_nobtag_loosemt_ff_w_dm1_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_syst_up"                )) wt_ff_nobtag_loosemt_tt_syst_up_                 = event->Get<double>("wt_nobtag_loosemt_ff_tt_syst_up"                );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_syst_down"              )) wt_ff_nobtag_loosemt_tt_syst_down_               = event->Get<double>("wt_nobtag_loosemt_ff_tt_syst_down"              );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm0_njet0_stat_up"      )) wt_ff_nobtag_loosemt_tt_dm0_njet0_stat_up_       = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm0_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm0_njet0_stat_down"    )) wt_ff_nobtag_loosemt_tt_dm0_njet0_stat_down_     = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm0_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm0_njet1_stat_up"      )) wt_ff_nobtag_loosemt_tt_dm0_njet1_stat_up_       = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm0_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm0_njet1_stat_down"    )) wt_ff_nobtag_loosemt_tt_dm0_njet1_stat_down_     = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm0_njet1_stat_down"    );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm1_njet0_stat_up"      )) wt_ff_nobtag_loosemt_tt_dm1_njet0_stat_up_       = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm1_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm1_njet0_stat_down"    )) wt_ff_nobtag_loosemt_tt_dm1_njet0_stat_down_     = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm1_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm1_njet1_stat_up"      )) wt_ff_nobtag_loosemt_tt_dm1_njet1_stat_up_       = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm1_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_loosemt_ff_tt_dm1_njet1_stat_down"    )) wt_ff_nobtag_loosemt_tt_dm1_njet1_stat_down_     = event->Get<double>("wt_nobtag_loosemt_ff_tt_dm1_njet1_stat_down"    );
            if(event->Exists("wt_btag_loosemt_ff_qcd_syst_up"                 )) wt_ff_btag_loosemt_qcd_syst_up_                  = event->Get<double>("wt_btag_loosemt_ff_qcd_syst_up"                 );
            if(event->Exists("wt_btag_loosemt_ff_qcd_syst_down"               )) wt_ff_btag_loosemt_qcd_syst_down_                = event->Get<double>("wt_btag_loosemt_ff_qcd_syst_down"               );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm0_njet0_stat_up"       )) wt_ff_btag_loosemt_qcd_dm0_njet0_stat_up_        = event->Get<double>("wt_btag_loosemt_ff_qcd_dm0_njet0_stat_up"       );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm0_njet0_stat_down"     )) wt_ff_btag_loosemt_qcd_dm0_njet0_stat_down_      = event->Get<double>("wt_btag_loosemt_ff_qcd_dm0_njet0_stat_down"     );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm0_njet1_stat_up"       )) wt_ff_btag_loosemt_qcd_dm0_njet1_stat_up_        = event->Get<double>("wt_btag_loosemt_ff_qcd_dm0_njet1_stat_up"       );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm0_njet1_stat_down"     )) wt_ff_btag_loosemt_qcd_dm0_njet1_stat_down_      = event->Get<double>("wt_btag_loosemt_ff_qcd_dm0_njet1_stat_down"     );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm1_njet0_stat_up"       )) wt_ff_btag_loosemt_qcd_dm1_njet0_stat_up_        = event->Get<double>("wt_btag_loosemt_ff_qcd_dm1_njet0_stat_up"       );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm1_njet0_stat_down"     )) wt_ff_btag_loosemt_qcd_dm1_njet0_stat_down_      = event->Get<double>("wt_btag_loosemt_ff_qcd_dm1_njet0_stat_down"     );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm1_njet1_stat_up"       )) wt_ff_btag_loosemt_qcd_dm1_njet1_stat_up_        = event->Get<double>("wt_btag_loosemt_ff_qcd_dm1_njet1_stat_up"       );
            if(event->Exists("wt_btag_loosemt_ff_qcd_dm1_njet1_stat_down"     )) wt_ff_btag_loosemt_qcd_dm1_njet1_stat_down_      = event->Get<double>("wt_btag_loosemt_ff_qcd_dm1_njet1_stat_down"     );
            if(event->Exists("wt_btag_loosemt_ff_w_syst_up"                   )) wt_ff_btag_loosemt_w_syst_up_                    = event->Get<double>("wt_btag_loosemt_ff_w_syst_up"                   );
            if(event->Exists("wt_btag_loosemt_ff_w_syst_down"                 )) wt_ff_btag_loosemt_w_syst_down_                  = event->Get<double>("wt_btag_loosemt_ff_w_syst_down"                 );
            if(event->Exists("wt_btag_loosemt_ff_w_dm0_njet0_stat_up"         )) wt_ff_btag_loosemt_w_dm0_njet0_stat_up_          = event->Get<double>("wt_btag_loosemt_ff_w_dm0_njet0_stat_up"         );
            if(event->Exists("wt_btag_loosemt_ff_w_dm0_njet0_stat_down"       )) wt_ff_btag_loosemt_w_dm0_njet0_stat_down_        = event->Get<double>("wt_btag_loosemt_ff_w_dm0_njet0_stat_down"       );
            if(event->Exists("wt_btag_loosemt_ff_w_dm0_njet1_stat_up"         )) wt_ff_btag_loosemt_w_dm0_njet1_stat_up_          = event->Get<double>("wt_btag_loosemt_ff_w_dm0_njet1_stat_up"         );
            if(event->Exists("wt_btag_loosemt_ff_w_dm0_njet1_stat_down"       )) wt_ff_btag_loosemt_w_dm0_njet1_stat_down_        = event->Get<double>("wt_btag_loosemt_ff_w_dm0_njet1_stat_down"       );
            if(event->Exists("wt_btag_loosemt_ff_w_dm1_njet0_stat_up"         )) wt_ff_btag_loosemt_w_dm1_njet0_stat_up_          = event->Get<double>("wt_btag_loosemt_ff_w_dm1_njet0_stat_up"         );
            if(event->Exists("wt_btag_loosemt_ff_w_dm1_njet0_stat_down"       )) wt_ff_btag_loosemt_w_dm1_njet0_stat_down_        = event->Get<double>("wt_btag_loosemt_ff_w_dm1_njet0_stat_down"       );
            if(event->Exists("wt_btag_loosemt_ff_w_dm1_njet1_stat_up"         )) wt_ff_btag_loosemt_w_dm1_njet1_stat_up_          = event->Get<double>("wt_btag_loosemt_ff_w_dm1_njet1_stat_up"         );
            if(event->Exists("wt_btag_loosemt_ff_w_dm1_njet1_stat_down"       )) wt_ff_btag_loosemt_w_dm1_njet1_stat_down_        = event->Get<double>("wt_btag_loosemt_ff_w_dm1_njet1_stat_down"       );
            if(event->Exists("wt_btag_loosemt_ff_tt_syst_up"                  )) wt_ff_btag_loosemt_tt_syst_up_                   = event->Get<double>("wt_btag_loosemt_ff_tt_syst_up"                  );
            if(event->Exists("wt_btag_loosemt_ff_tt_syst_down"                )) wt_ff_btag_loosemt_tt_syst_down_                 = event->Get<double>("wt_btag_loosemt_ff_tt_syst_down"                );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm0_njet0_stat_up"        )) wt_ff_btag_loosemt_tt_dm0_njet0_stat_up_         = event->Get<double>("wt_btag_loosemt_ff_tt_dm0_njet0_stat_up"        );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm0_njet0_stat_down"      )) wt_ff_btag_loosemt_tt_dm0_njet0_stat_down_       = event->Get<double>("wt_btag_loosemt_ff_tt_dm0_njet0_stat_down"      );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm0_njet1_stat_up"        )) wt_ff_btag_loosemt_tt_dm0_njet1_stat_up_         = event->Get<double>("wt_btag_loosemt_ff_tt_dm0_njet1_stat_up"        );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm0_njet1_stat_down"      )) wt_ff_btag_loosemt_tt_dm0_njet1_stat_down_       = event->Get<double>("wt_btag_loosemt_ff_tt_dm0_njet1_stat_down"      );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm1_njet0_stat_up"        )) wt_ff_btag_loosemt_tt_dm1_njet0_stat_up_         = event->Get<double>("wt_btag_loosemt_ff_tt_dm1_njet0_stat_up"        );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm1_njet0_stat_down"      )) wt_ff_btag_loosemt_tt_dm1_njet0_stat_down_       = event->Get<double>("wt_btag_loosemt_ff_tt_dm1_njet0_stat_down"      );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm1_njet1_stat_up"        )) wt_ff_btag_loosemt_tt_dm1_njet1_stat_up_         = event->Get<double>("wt_btag_loosemt_ff_tt_dm1_njet1_stat_up"        );
            if(event->Exists("wt_btag_loosemt_ff_tt_dm1_njet1_stat_down"      )) wt_ff_btag_loosemt_tt_dm1_njet1_stat_down_       = event->Get<double>("wt_btag_loosemt_ff_tt_dm1_njet1_stat_down"      );
            
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_syst_up"               )) wt_ff_nobtag_looseiso_qcd_syst_up_                = event->Get<double>("wt_nobtag_looseiso_ff_qcd_syst_up"               );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_syst_down"             )) wt_ff_nobtag_looseiso_qcd_syst_down_              = event->Get<double>("wt_nobtag_looseiso_ff_qcd_syst_down"             );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm0_njet0_stat_up"     )) wt_ff_nobtag_looseiso_qcd_dm0_njet0_stat_up_      = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm0_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm0_njet0_stat_down"   )) wt_ff_nobtag_looseiso_qcd_dm0_njet0_stat_down_    = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm0_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm0_njet1_stat_up"     )) wt_ff_nobtag_looseiso_qcd_dm0_njet1_stat_up_      = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm0_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm0_njet1_stat_down"   )) wt_ff_nobtag_looseiso_qcd_dm0_njet1_stat_down_    = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm0_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm1_njet0_stat_up"     )) wt_ff_nobtag_looseiso_qcd_dm1_njet0_stat_up_      = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm1_njet0_stat_up"     );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm1_njet0_stat_down"   )) wt_ff_nobtag_looseiso_qcd_dm1_njet0_stat_down_    = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm1_njet0_stat_down"   );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm1_njet1_stat_up"     )) wt_ff_nobtag_looseiso_qcd_dm1_njet1_stat_up_      = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm1_njet1_stat_up"     );
            if(event->Exists("wt_nobtag_looseiso_ff_qcd_dm1_njet1_stat_down"   )) wt_ff_nobtag_looseiso_qcd_dm1_njet1_stat_down_    = event->Get<double>("wt_nobtag_looseiso_ff_qcd_dm1_njet1_stat_down"   );
            if(event->Exists("wt_nobtag_looseiso_ff_w_syst_up"                 )) wt_ff_nobtag_looseiso_w_syst_up_                  = event->Get<double>("wt_nobtag_looseiso_ff_w_syst_up"                 );
            if(event->Exists("wt_nobtag_looseiso_ff_w_syst_down"               )) wt_ff_nobtag_looseiso_w_syst_down_                = event->Get<double>("wt_nobtag_looseiso_ff_w_syst_down"               );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm0_njet0_stat_up"       )) wt_ff_nobtag_looseiso_w_dm0_njet0_stat_up_        = event->Get<double>("wt_nobtag_looseiso_ff_w_dm0_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm0_njet0_stat_down"     )) wt_ff_nobtag_looseiso_w_dm0_njet0_stat_down_      = event->Get<double>("wt_nobtag_looseiso_ff_w_dm0_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm0_njet1_stat_up"       )) wt_ff_nobtag_looseiso_w_dm0_njet1_stat_up_        = event->Get<double>("wt_nobtag_looseiso_ff_w_dm0_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm0_njet1_stat_down"     )) wt_ff_nobtag_looseiso_w_dm0_njet1_stat_down_      = event->Get<double>("wt_nobtag_looseiso_ff_w_dm0_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm1_njet0_stat_up"       )) wt_ff_nobtag_looseiso_w_dm1_njet0_stat_up_        = event->Get<double>("wt_nobtag_looseiso_ff_w_dm1_njet0_stat_up"       );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm1_njet0_stat_down"     )) wt_ff_nobtag_looseiso_w_dm1_njet0_stat_down_      = event->Get<double>("wt_nobtag_looseiso_ff_w_dm1_njet0_stat_down"     );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm1_njet1_stat_up"       )) wt_ff_nobtag_looseiso_w_dm1_njet1_stat_up_        = event->Get<double>("wt_nobtag_looseiso_ff_w_dm1_njet1_stat_up"       );
            if(event->Exists("wt_nobtag_looseiso_ff_w_dm1_njet1_stat_down"     )) wt_ff_nobtag_looseiso_w_dm1_njet1_stat_down_      = event->Get<double>("wt_nobtag_looseiso_ff_w_dm1_njet1_stat_down"     );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_syst_up"                )) wt_ff_nobtag_looseiso_tt_syst_up_                 = event->Get<double>("wt_nobtag_looseiso_ff_tt_syst_up"                );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_syst_down"              )) wt_ff_nobtag_looseiso_tt_syst_down_               = event->Get<double>("wt_nobtag_looseiso_ff_tt_syst_down"              );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm0_njet0_stat_up"      )) wt_ff_nobtag_looseiso_tt_dm0_njet0_stat_up_       = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm0_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm0_njet0_stat_down"    )) wt_ff_nobtag_looseiso_tt_dm0_njet0_stat_down_     = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm0_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm0_njet1_stat_up"      )) wt_ff_nobtag_looseiso_tt_dm0_njet1_stat_up_       = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm0_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm0_njet1_stat_down"    )) wt_ff_nobtag_looseiso_tt_dm0_njet1_stat_down_     = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm0_njet1_stat_down"    );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm1_njet0_stat_up"      )) wt_ff_nobtag_looseiso_tt_dm1_njet0_stat_up_       = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm1_njet0_stat_up"      );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm1_njet0_stat_down"    )) wt_ff_nobtag_looseiso_tt_dm1_njet0_stat_down_     = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm1_njet0_stat_down"    );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm1_njet1_stat_up"      )) wt_ff_nobtag_looseiso_tt_dm1_njet1_stat_up_       = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm1_njet1_stat_up"      );
            if(event->Exists("wt_nobtag_looseiso_ff_tt_dm1_njet1_stat_down"    )) wt_ff_nobtag_looseiso_tt_dm1_njet1_stat_down_     = event->Get<double>("wt_nobtag_looseiso_ff_tt_dm1_njet1_stat_down"    );
            if(event->Exists("wt_btag_looseiso_ff_qcd_syst_up"                 )) wt_ff_btag_looseiso_qcd_syst_up_                  = event->Get<double>("wt_btag_looseiso_ff_qcd_syst_up"                 );
            if(event->Exists("wt_btag_looseiso_ff_qcd_syst_down"               )) wt_ff_btag_looseiso_qcd_syst_down_                = event->Get<double>("wt_btag_looseiso_ff_qcd_syst_down"               );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm0_njet0_stat_up"       )) wt_ff_btag_looseiso_qcd_dm0_njet0_stat_up_        = event->Get<double>("wt_btag_looseiso_ff_qcd_dm0_njet0_stat_up"       );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm0_njet0_stat_down"     )) wt_ff_btag_looseiso_qcd_dm0_njet0_stat_down_      = event->Get<double>("wt_btag_looseiso_ff_qcd_dm0_njet0_stat_down"     );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm0_njet1_stat_up"       )) wt_ff_btag_looseiso_qcd_dm0_njet1_stat_up_        = event->Get<double>("wt_btag_looseiso_ff_qcd_dm0_njet1_stat_up"       );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm0_njet1_stat_down"     )) wt_ff_btag_looseiso_qcd_dm0_njet1_stat_down_      = event->Get<double>("wt_btag_looseiso_ff_qcd_dm0_njet1_stat_down"     );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm1_njet0_stat_up"       )) wt_ff_btag_looseiso_qcd_dm1_njet0_stat_up_        = event->Get<double>("wt_btag_looseiso_ff_qcd_dm1_njet0_stat_up"       );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm1_njet0_stat_down"     )) wt_ff_btag_looseiso_qcd_dm1_njet0_stat_down_      = event->Get<double>("wt_btag_looseiso_ff_qcd_dm1_njet0_stat_down"     );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm1_njet1_stat_up"       )) wt_ff_btag_looseiso_qcd_dm1_njet1_stat_up_        = event->Get<double>("wt_btag_looseiso_ff_qcd_dm1_njet1_stat_up"       );
            if(event->Exists("wt_btag_looseiso_ff_qcd_dm1_njet1_stat_down"     )) wt_ff_btag_looseiso_qcd_dm1_njet1_stat_down_      = event->Get<double>("wt_btag_looseiso_ff_qcd_dm1_njet1_stat_down"     );
            if(event->Exists("wt_btag_looseiso_ff_w_syst_up"                   )) wt_ff_btag_looseiso_w_syst_up_                    = event->Get<double>("wt_btag_looseiso_ff_w_syst_up"                   );
            if(event->Exists("wt_btag_looseiso_ff_w_syst_down"                 )) wt_ff_btag_looseiso_w_syst_down_                  = event->Get<double>("wt_btag_looseiso_ff_w_syst_down"                 );
            if(event->Exists("wt_btag_looseiso_ff_w_dm0_njet0_stat_up"         )) wt_ff_btag_looseiso_w_dm0_njet0_stat_up_          = event->Get<double>("wt_btag_looseiso_ff_w_dm0_njet0_stat_up"         );
            if(event->Exists("wt_btag_looseiso_ff_w_dm0_njet0_stat_down"       )) wt_ff_btag_looseiso_w_dm0_njet0_stat_down_        = event->Get<double>("wt_btag_looseiso_ff_w_dm0_njet0_stat_down"       );
            if(event->Exists("wt_btag_looseiso_ff_w_dm0_njet1_stat_up"         )) wt_ff_btag_looseiso_w_dm0_njet1_stat_up_          = event->Get<double>("wt_btag_looseiso_ff_w_dm0_njet1_stat_up"         );
            if(event->Exists("wt_btag_looseiso_ff_w_dm0_njet1_stat_down"       )) wt_ff_btag_looseiso_w_dm0_njet1_stat_down_        = event->Get<double>("wt_btag_looseiso_ff_w_dm0_njet1_stat_down"       );
            if(event->Exists("wt_btag_looseiso_ff_w_dm1_njet0_stat_up"         )) wt_ff_btag_looseiso_w_dm1_njet0_stat_up_          = event->Get<double>("wt_btag_looseiso_ff_w_dm1_njet0_stat_up"         );
            if(event->Exists("wt_btag_looseiso_ff_w_dm1_njet0_stat_down"       )) wt_ff_btag_looseiso_w_dm1_njet0_stat_down_        = event->Get<double>("wt_btag_looseiso_ff_w_dm1_njet0_stat_down"       );
            if(event->Exists("wt_btag_looseiso_ff_w_dm1_njet1_stat_up"         )) wt_ff_btag_looseiso_w_dm1_njet1_stat_up_          = event->Get<double>("wt_btag_looseiso_ff_w_dm1_njet1_stat_up"         );
            if(event->Exists("wt_btag_looseiso_ff_w_dm1_njet1_stat_down"       )) wt_ff_btag_looseiso_w_dm1_njet1_stat_down_        = event->Get<double>("wt_btag_looseiso_ff_w_dm1_njet1_stat_down"       );
            if(event->Exists("wt_btag_looseiso_ff_tt_syst_up"                  )) wt_ff_btag_looseiso_tt_syst_up_                   = event->Get<double>("wt_btag_looseiso_ff_tt_syst_up"                  );
            if(event->Exists("wt_btag_looseiso_ff_tt_syst_down"                )) wt_ff_btag_looseiso_tt_syst_down_                 = event->Get<double>("wt_btag_looseiso_ff_tt_syst_down"                );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm0_njet0_stat_up"        )) wt_ff_btag_looseiso_tt_dm0_njet0_stat_up_         = event->Get<double>("wt_btag_looseiso_ff_tt_dm0_njet0_stat_up"        );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm0_njet0_stat_down"      )) wt_ff_btag_looseiso_tt_dm0_njet0_stat_down_       = event->Get<double>("wt_btag_looseiso_ff_tt_dm0_njet0_stat_down"      );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm0_njet1_stat_up"        )) wt_ff_btag_looseiso_tt_dm0_njet1_stat_up_         = event->Get<double>("wt_btag_looseiso_ff_tt_dm0_njet1_stat_up"        );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm0_njet1_stat_down"      )) wt_ff_btag_looseiso_tt_dm0_njet1_stat_down_       = event->Get<double>("wt_btag_looseiso_ff_tt_dm0_njet1_stat_down"      );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm1_njet0_stat_up"        )) wt_ff_btag_looseiso_tt_dm1_njet0_stat_up_         = event->Get<double>("wt_btag_looseiso_ff_tt_dm1_njet0_stat_up"        );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm1_njet0_stat_down"      )) wt_ff_btag_looseiso_tt_dm1_njet0_stat_down_       = event->Get<double>("wt_btag_looseiso_ff_tt_dm1_njet0_stat_down"      );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm1_njet1_stat_up"        )) wt_ff_btag_looseiso_tt_dm1_njet1_stat_up_         = event->Get<double>("wt_btag_looseiso_ff_tt_dm1_njet1_stat_up"        );
            if(event->Exists("wt_btag_looseiso_ff_tt_dm1_njet1_stat_down"      )) wt_ff_btag_looseiso_tt_dm1_njet1_stat_down_       = event->Get<double>("wt_btag_looseiso_ff_tt_dm1_njet1_stat_down"      );
          } else if (channel_ == channel::tt){
            if(event->Exists("wt_inclusive_ff_qcd_syst_up_1"                )) wt_inclusive_ff_qcd_syst_up_1                = event->Get<double>("wt_inclusive_ff_qcd_syst_up_1");
            if(event->Exists("wt_inclusive_ff_qcd_syst_down_1"              )) wt_inclusive_ff_qcd_syst_down_1              = event->Get<double>("wt_inclusive_ff_qcd_syst_down_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet0_stat_up_1"      )) wt_inclusive_ff_qcd_dm0_njet0_stat_up_1      = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet0_stat_up_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet0_stat_down_1"    )) wt_inclusive_ff_qcd_dm0_njet0_stat_down_1    = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet0_stat_down_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet1_stat_up_1"      )) wt_inclusive_ff_qcd_dm0_njet1_stat_up_1      = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet1_stat_up_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet1_stat_down_1"    )) wt_inclusive_ff_qcd_dm0_njet1_stat_down_1    = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet1_stat_down_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet0_stat_up_1"      )) wt_inclusive_ff_qcd_dm1_njet0_stat_up_1      = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet0_stat_up_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet0_stat_down_1"    )) wt_inclusive_ff_qcd_dm1_njet0_stat_down_1    = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet0_stat_down_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet1_stat_up_1"      )) wt_inclusive_ff_qcd_dm1_njet1_stat_up_1      = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet1_stat_up_1");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet1_stat_down_1"    )) wt_inclusive_ff_qcd_dm1_njet1_stat_down_1    = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet1_stat_down_1");
            if(event->Exists("wt_inclusive_ff_w_syst_up_1"                  )) wt_inclusive_ff_w_syst_up_1                  = event->Get<double>("wt_inclusive_ff_w_syst_up_1");
            if(event->Exists("wt_inclusive_ff_w_syst_down_1"                )) wt_inclusive_ff_w_syst_down_1                = event->Get<double>("wt_inclusive_ff_w_syst_down_1");
            if(event->Exists("wt_inclusive_ff_tt_syst_up_1"                 )) wt_inclusive_ff_tt_syst_up_1                 = event->Get<double>("wt_inclusive_ff_tt_syst_up_1");
            if(event->Exists("wt_inclusive_ff_tt_syst_down_1"               )) wt_inclusive_ff_tt_syst_down_1               = event->Get<double>("wt_inclusive_ff_tt_syst_down_1");
            if(event->Exists("wt_inclusive_ff_w_frac_syst_up_1"             )) wt_inclusive_ff_w_frac_syst_up_1             = event->Get<double>("wt_inclusive_ff_w_frac_syst_up_1");
            if(event->Exists("wt_inclusive_ff_w_frac_syst_down_1"           )) wt_inclusive_ff_w_frac_syst_down_1           = event->Get<double>("wt_inclusive_ff_w_frac_syst_down_1");
            if(event->Exists("wt_inclusive_ff_tt_frac_syst_up_1"            )) wt_inclusive_ff_tt_frac_syst_up_1            = event->Get<double>("wt_inclusive_ff_tt_frac_syst_up_1");
            if(event->Exists("wt_inclusive_ff_tt_frac_syst_down_1"          )) wt_inclusive_ff_tt_frac_syst_down_1          = event->Get<double>("wt_inclusive_ff_tt_frac_syst_down_1");
            if(event->Exists("wt_inclusive_ff_dy_frac_syst_up_1"            )) wt_inclusive_ff_dy_frac_syst_up_1            = event->Get<double>("wt_inclusive_ff_dy_frac_syst_up_1");
            if(event->Exists("wt_inclusive_ff_dy_frac_syst_down_1"          )) wt_inclusive_ff_dy_frac_syst_down_1          = event->Get<double>("wt_inclusive_ff_dy_frac_syst_down_1");
            if(event->Exists("wt_nobtag_ff_qcd_syst_up_1"                   )) wt_nobtag_ff_qcd_syst_up_1                   = event->Get<double>("wt_nobtag_ff_qcd_syst_up_1");
            if(event->Exists("wt_nobtag_ff_qcd_syst_down_1"                 )) wt_nobtag_ff_qcd_syst_down_1                 = event->Get<double>("wt_nobtag_ff_qcd_syst_down_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet0_stat_up_1"         )) wt_nobtag_ff_qcd_dm0_njet0_stat_up_1         = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet0_stat_up_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet0_stat_down_1"       )) wt_nobtag_ff_qcd_dm0_njet0_stat_down_1       = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet0_stat_down_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet1_stat_up_1"         )) wt_nobtag_ff_qcd_dm0_njet1_stat_up_1         = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet1_stat_up_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet1_stat_down_1"       )) wt_nobtag_ff_qcd_dm0_njet1_stat_down_1       = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet1_stat_down_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet0_stat_up_1"         )) wt_nobtag_ff_qcd_dm1_njet0_stat_up_1         = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet0_stat_up_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet0_stat_down_1"       )) wt_nobtag_ff_qcd_dm1_njet0_stat_down_1       = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet0_stat_down_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet1_stat_up_1"         )) wt_nobtag_ff_qcd_dm1_njet1_stat_up_1         = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet1_stat_up_1");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet1_stat_down_1"       )) wt_nobtag_ff_qcd_dm1_njet1_stat_down_1       = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet1_stat_down_1");
            if(event->Exists("wt_nobtag_ff_w_syst_up_1"                     )) wt_nobtag_ff_w_syst_up_1                     = event->Get<double>("wt_nobtag_ff_w_syst_up_1");
            if(event->Exists("wt_nobtag_ff_w_syst_down_1"                   )) wt_nobtag_ff_w_syst_down_1                   = event->Get<double>("wt_nobtag_ff_w_syst_down_1");
            if(event->Exists("wt_nobtag_ff_tt_syst_up_1"                    )) wt_nobtag_ff_tt_syst_up_1                    = event->Get<double>("wt_nobtag_ff_tt_syst_up_1");
            if(event->Exists("wt_nobtag_ff_tt_syst_down_1"                  )) wt_nobtag_ff_tt_syst_down_1                  = event->Get<double>("wt_nobtag_ff_tt_syst_down_1");
            if(event->Exists("wt_nobtag_ff_w_frac_syst_up_1"                )) wt_nobtag_ff_w_frac_syst_up_1                = event->Get<double>("wt_nobtag_ff_w_frac_syst_up_1");
            if(event->Exists("wt_nobtag_ff_w_frac_syst_down_1"              )) wt_nobtag_ff_w_frac_syst_down_1              = event->Get<double>("wt_nobtag_ff_w_frac_syst_down_1");
            if(event->Exists("wt_nobtag_ff_tt_frac_syst_up_1"               )) wt_nobtag_ff_tt_frac_syst_up_1               = event->Get<double>("wt_nobtag_ff_tt_frac_syst_up_1");
            if(event->Exists("wt_nobtag_ff_tt_frac_syst_down_1"             )) wt_nobtag_ff_tt_frac_syst_down_1             = event->Get<double>("wt_nobtag_ff_tt_frac_syst_down_1");
            if(event->Exists("wt_nobtag_ff_dy_frac_syst_up_1"               )) wt_nobtag_ff_dy_frac_syst_up_1               = event->Get<double>("wt_nobtag_ff_dy_frac_syst_up_1");
            if(event->Exists("wt_nobtag_ff_dy_frac_syst_down_1"             )) wt_nobtag_ff_dy_frac_syst_down_1             = event->Get<double>("wt_nobtag_ff_dy_frac_syst_down_1");
            if(event->Exists("wt_btag_ff_qcd_syst_up_1"                     )) wt_btag_ff_qcd_syst_up_1                     = event->Get<double>("wt_btag_ff_qcd_syst_up_1");
            if(event->Exists("wt_btag_ff_qcd_syst_down_1"                   )) wt_btag_ff_qcd_syst_down_1                   = event->Get<double>("wt_btag_ff_qcd_syst_down_1");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet0_stat_up_1"           )) wt_btag_ff_qcd_dm0_njet0_stat_up_1           = event->Get<double>("wt_btag_ff_qcd_dm0_njet0_stat_up_1");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet0_stat_down_1"         )) wt_btag_ff_qcd_dm0_njet0_stat_down_1         = event->Get<double>("wt_btag_ff_qcd_dm0_njet0_stat_down_1");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet1_stat_up_1"           )) wt_btag_ff_qcd_dm0_njet1_stat_up_1           = event->Get<double>("wt_btag_ff_qcd_dm0_njet1_stat_up_1");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet1_stat_down_1"         )) wt_btag_ff_qcd_dm0_njet1_stat_down_1         = event->Get<double>("wt_btag_ff_qcd_dm0_njet1_stat_down_1");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet0_stat_up_1"           )) wt_btag_ff_qcd_dm1_njet0_stat_up_1           = event->Get<double>("wt_btag_ff_qcd_dm1_njet0_stat_up_1");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet0_stat_down_1"         )) wt_btag_ff_qcd_dm1_njet0_stat_down_1         = event->Get<double>("wt_btag_ff_qcd_dm1_njet0_stat_down_1");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet1_stat_up_1"           )) wt_btag_ff_qcd_dm1_njet1_stat_up_1           = event->Get<double>("wt_btag_ff_qcd_dm1_njet1_stat_up_1");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet1_stat_down_1"         )) wt_btag_ff_qcd_dm1_njet1_stat_down_1         = event->Get<double>("wt_btag_ff_qcd_dm1_njet1_stat_down_1");
            if(event->Exists("wt_btag_ff_w_syst_up_1"                       )) wt_btag_ff_w_syst_up_1                       = event->Get<double>("wt_btag_ff_w_syst_up_1");
            if(event->Exists("wt_btag_ff_w_syst_down_1"                     )) wt_btag_ff_w_syst_down_1                     = event->Get<double>("wt_btag_ff_w_syst_down_1");
            if(event->Exists("wt_btag_ff_tt_syst_up_1"                      )) wt_btag_ff_tt_syst_up_1                      = event->Get<double>("wt_btag_ff_tt_syst_up_1");
            if(event->Exists("wt_btag_ff_tt_syst_down_1"                    )) wt_btag_ff_tt_syst_down_1                    = event->Get<double>("wt_btag_ff_tt_syst_down_1");
            if(event->Exists("wt_btag_ff_w_frac_syst_up_1"                  )) wt_btag_ff_w_frac_syst_up_1                  = event->Get<double>("wt_btag_ff_w_frac_syst_up_1");
            if(event->Exists("wt_btag_ff_w_frac_syst_down_1"                )) wt_btag_ff_w_frac_syst_down_1                = event->Get<double>("wt_btag_ff_w_frac_syst_down_1");
            if(event->Exists("wt_btag_ff_tt_frac_syst_up_1"                 )) wt_btag_ff_tt_frac_syst_up_1                 = event->Get<double>("wt_btag_ff_tt_frac_syst_up_1");
            if(event->Exists("wt_btag_ff_tt_frac_syst_down_1"               )) wt_btag_ff_tt_frac_syst_down_1               = event->Get<double>("wt_btag_ff_tt_frac_syst_down_1");
            if(event->Exists("wt_btag_ff_dy_frac_syst_up_1"                 )) wt_btag_ff_dy_frac_syst_up_1                 = event->Get<double>("wt_btag_ff_dy_frac_syst_up_1");
            if(event->Exists("wt_btag_ff_dy_frac_syst_down_1"               )) wt_btag_ff_dy_frac_syst_down_1               = event->Get<double>("wt_btag_ff_dy_frac_syst_down_1");
            if(event->Exists("wt_inclusive_ff_qcd_syst_up_2"                )) wt_inclusive_ff_qcd_syst_up_2                = event->Get<double>("wt_inclusive_ff_qcd_syst_up_2");
            if(event->Exists("wt_inclusive_ff_qcd_syst_down_2"              )) wt_inclusive_ff_qcd_syst_down_2              = event->Get<double>("wt_inclusive_ff_qcd_syst_down_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet0_stat_up_2"      )) wt_inclusive_ff_qcd_dm0_njet0_stat_up_2      = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet0_stat_up_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet0_stat_down_2"    )) wt_inclusive_ff_qcd_dm0_njet0_stat_down_2    = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet0_stat_down_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet1_stat_up_2"      )) wt_inclusive_ff_qcd_dm0_njet1_stat_up_2      = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet1_stat_up_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm0_njet1_stat_down_2"    )) wt_inclusive_ff_qcd_dm0_njet1_stat_down_2    = event->Get<double>("wt_inclusive_ff_qcd_dm0_njet1_stat_down_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet0_stat_up_2"      )) wt_inclusive_ff_qcd_dm1_njet0_stat_up_2      = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet0_stat_up_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet0_stat_down_2"    )) wt_inclusive_ff_qcd_dm1_njet0_stat_down_2    = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet0_stat_down_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet1_stat_up_2"      )) wt_inclusive_ff_qcd_dm1_njet1_stat_up_2      = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet1_stat_up_2");
            if(event->Exists("wt_inclusive_ff_qcd_dm1_njet1_stat_down_2"    )) wt_inclusive_ff_qcd_dm1_njet1_stat_down_2    = event->Get<double>("wt_inclusive_ff_qcd_dm1_njet1_stat_down_2");
            if(event->Exists("wt_inclusive_ff_w_syst_up_2"                  )) wt_inclusive_ff_w_syst_up_2                  = event->Get<double>("wt_inclusive_ff_w_syst_up_2");
            if(event->Exists("wt_inclusive_ff_w_syst_down_2"                )) wt_inclusive_ff_w_syst_down_2                = event->Get<double>("wt_inclusive_ff_w_syst_down_2");
            if(event->Exists("wt_inclusive_ff_tt_syst_up_2"                 )) wt_inclusive_ff_tt_syst_up_2                 = event->Get<double>("wt_inclusive_ff_tt_syst_up_2");
            if(event->Exists("wt_inclusive_ff_tt_syst_down_2"               )) wt_inclusive_ff_tt_syst_down_2               = event->Get<double>("wt_inclusive_ff_tt_syst_down_2");
            if(event->Exists("wt_inclusive_ff_w_frac_syst_up_2"             )) wt_inclusive_ff_w_frac_syst_up_2             = event->Get<double>("wt_inclusive_ff_w_frac_syst_up_2");
            if(event->Exists("wt_inclusive_ff_w_frac_syst_down_2"           )) wt_inclusive_ff_w_frac_syst_down_2           = event->Get<double>("wt_inclusive_ff_w_frac_syst_down_2");
            if(event->Exists("wt_inclusive_ff_tt_frac_syst_up_2"            )) wt_inclusive_ff_tt_frac_syst_up_2            = event->Get<double>("wt_inclusive_ff_tt_frac_syst_up_2");
            if(event->Exists("wt_inclusive_ff_tt_frac_syst_down_2"          )) wt_inclusive_ff_tt_frac_syst_down_2          = event->Get<double>("wt_inclusive_ff_tt_frac_syst_down_2");
            if(event->Exists("wt_inclusive_ff_dy_frac_syst_up_2"            )) wt_inclusive_ff_dy_frac_syst_up_2            = event->Get<double>("wt_inclusive_ff_dy_frac_syst_up_2");
            if(event->Exists("wt_inclusive_ff_dy_frac_syst_down_2"          )) wt_inclusive_ff_dy_frac_syst_down_2          = event->Get<double>("wt_inclusive_ff_dy_frac_syst_down_2");
            if(event->Exists("wt_nobtag_ff_qcd_syst_up_2"                   )) wt_nobtag_ff_qcd_syst_up_2                   = event->Get<double>("wt_nobtag_ff_qcd_syst_up_2");
            if(event->Exists("wt_nobtag_ff_qcd_syst_down_2"                 )) wt_nobtag_ff_qcd_syst_down_2                 = event->Get<double>("wt_nobtag_ff_qcd_syst_down_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet0_stat_up_2"         )) wt_nobtag_ff_qcd_dm0_njet0_stat_up_2         = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet0_stat_up_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet0_stat_down_2"       )) wt_nobtag_ff_qcd_dm0_njet0_stat_down_2       = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet0_stat_down_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet1_stat_up_2"         )) wt_nobtag_ff_qcd_dm0_njet1_stat_up_2         = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet1_stat_up_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm0_njet1_stat_down_2"       )) wt_nobtag_ff_qcd_dm0_njet1_stat_down_2       = event->Get<double>("wt_nobtag_ff_qcd_dm0_njet1_stat_down_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet0_stat_up_2"         )) wt_nobtag_ff_qcd_dm1_njet0_stat_up_2         = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet0_stat_up_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet0_stat_down_2"       )) wt_nobtag_ff_qcd_dm1_njet0_stat_down_2       = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet0_stat_down_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet1_stat_up_2"         )) wt_nobtag_ff_qcd_dm1_njet1_stat_up_2         = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet1_stat_up_2");
            if(event->Exists("wt_nobtag_ff_qcd_dm1_njet1_stat_down_2"       )) wt_nobtag_ff_qcd_dm1_njet1_stat_down_2       = event->Get<double>("wt_nobtag_ff_qcd_dm1_njet1_stat_down_2");
            if(event->Exists("wt_nobtag_ff_w_syst_up_2"                     )) wt_nobtag_ff_w_syst_up_2                     = event->Get<double>("wt_nobtag_ff_w_syst_up_2");
            if(event->Exists("wt_nobtag_ff_w_syst_down_2"                   )) wt_nobtag_ff_w_syst_down_2                   = event->Get<double>("wt_nobtag_ff_w_syst_down_2");
            if(event->Exists("wt_nobtag_ff_tt_syst_up_2"                    )) wt_nobtag_ff_tt_syst_up_2                    = event->Get<double>("wt_nobtag_ff_tt_syst_up_2");
            if(event->Exists("wt_nobtag_ff_tt_syst_down_2"                  )) wt_nobtag_ff_tt_syst_down_2                  = event->Get<double>("wt_nobtag_ff_tt_syst_down_2");
            if(event->Exists("wt_nobtag_ff_w_frac_syst_up_2"                )) wt_nobtag_ff_w_frac_syst_up_2                = event->Get<double>("wt_nobtag_ff_w_frac_syst_up_2");
            if(event->Exists("wt_nobtag_ff_w_frac_syst_down_2"              )) wt_nobtag_ff_w_frac_syst_down_2              = event->Get<double>("wt_nobtag_ff_w_frac_syst_down_2");
            if(event->Exists("wt_nobtag_ff_tt_frac_syst_up_2"               )) wt_nobtag_ff_tt_frac_syst_up_2               = event->Get<double>("wt_nobtag_ff_tt_frac_syst_up_2");
            if(event->Exists("wt_nobtag_ff_tt_frac_syst_down_2"             )) wt_nobtag_ff_tt_frac_syst_down_2             = event->Get<double>("wt_nobtag_ff_tt_frac_syst_down_2");
            if(event->Exists("wt_nobtag_ff_dy_frac_syst_up_2"               )) wt_nobtag_ff_dy_frac_syst_up_2               = event->Get<double>("wt_nobtag_ff_dy_frac_syst_up_2");
            if(event->Exists("wt_nobtag_ff_dy_frac_syst_down_2"             )) wt_nobtag_ff_dy_frac_syst_down_2             = event->Get<double>("wt_nobtag_ff_dy_frac_syst_down_2");
            if(event->Exists("wt_btag_ff_qcd_syst_up_2"                     )) wt_btag_ff_qcd_syst_up_2                     = event->Get<double>("wt_btag_ff_qcd_syst_up_2");
            if(event->Exists("wt_btag_ff_qcd_syst_down_2"                   )) wt_btag_ff_qcd_syst_down_2                   = event->Get<double>("wt_btag_ff_qcd_syst_down_2");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet0_stat_up_2"           )) wt_btag_ff_qcd_dm0_njet0_stat_up_2           = event->Get<double>("wt_btag_ff_qcd_dm0_njet0_stat_up_2");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet0_stat_down_2"         )) wt_btag_ff_qcd_dm0_njet0_stat_down_2         = event->Get<double>("wt_btag_ff_qcd_dm0_njet0_stat_down_2");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet1_stat_up_2"           )) wt_btag_ff_qcd_dm0_njet1_stat_up_2           = event->Get<double>("wt_btag_ff_qcd_dm0_njet1_stat_up_2");
            if(event->Exists("wt_btag_ff_qcd_dm0_njet1_stat_down_2"         )) wt_btag_ff_qcd_dm0_njet1_stat_down_2         = event->Get<double>("wt_btag_ff_qcd_dm0_njet1_stat_down_2");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet0_stat_up_2"           )) wt_btag_ff_qcd_dm1_njet0_stat_up_2           = event->Get<double>("wt_btag_ff_qcd_dm1_njet0_stat_up_2");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet0_stat_down_2"         )) wt_btag_ff_qcd_dm1_njet0_stat_down_2         = event->Get<double>("wt_btag_ff_qcd_dm1_njet0_stat_down_2");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet1_stat_up_2"           )) wt_btag_ff_qcd_dm1_njet1_stat_up_2           = event->Get<double>("wt_btag_ff_qcd_dm1_njet1_stat_up_2");
            if(event->Exists("wt_btag_ff_qcd_dm1_njet1_stat_down_2"         )) wt_btag_ff_qcd_dm1_njet1_stat_down_2         = event->Get<double>("wt_btag_ff_qcd_dm1_njet1_stat_down_2");
            if(event->Exists("wt_btag_ff_w_syst_up_2"                       )) wt_btag_ff_w_syst_up_2                       = event->Get<double>("wt_btag_ff_w_syst_up_2");
            if(event->Exists("wt_btag_ff_w_syst_down_2"                     )) wt_btag_ff_w_syst_down_2                     = event->Get<double>("wt_btag_ff_w_syst_down_2");
            if(event->Exists("wt_btag_ff_tt_syst_up_2"                      )) wt_btag_ff_tt_syst_up_2                      = event->Get<double>("wt_btag_ff_tt_syst_up_2");
            if(event->Exists("wt_btag_ff_tt_syst_down_2"                    )) wt_btag_ff_tt_syst_down_2                    = event->Get<double>("wt_btag_ff_tt_syst_down_2");
            if(event->Exists("wt_btag_ff_w_frac_syst_up_2"                  )) wt_btag_ff_w_frac_syst_up_2                  = event->Get<double>("wt_btag_ff_w_frac_syst_up_2");
            if(event->Exists("wt_btag_ff_w_frac_syst_down_2"                )) wt_btag_ff_w_frac_syst_down_2                = event->Get<double>("wt_btag_ff_w_frac_syst_down_2");
            if(event->Exists("wt_btag_ff_tt_frac_syst_up_2"                 )) wt_btag_ff_tt_frac_syst_up_2                 = event->Get<double>("wt_btag_ff_tt_frac_syst_up_2");
            if(event->Exists("wt_btag_ff_tt_frac_syst_down_2"               )) wt_btag_ff_tt_frac_syst_down_2               = event->Get<double>("wt_btag_ff_tt_frac_syst_down_2");
            if(event->Exists("wt_btag_ff_dy_frac_syst_up_2"                 )) wt_btag_ff_dy_frac_syst_up_2                 = event->Get<double>("wt_btag_ff_dy_frac_syst_up_2");
            if(event->Exists("wt_btag_ff_dy_frac_syst_down_2"               )) wt_btag_ff_dy_frac_syst_down_2               = event->Get<double>("wt_btag_ff_dy_frac_syst_down_2");
          }
        }
      } else if (strategy_ == strategy::cpdecays18 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::legacy16) {
        if(event->Exists("wt_ff_1")) wt_ff_1_ = event->Get<double>("wt_ff_1");
        if(event->Exists("wt_ff_2")) wt_ff_2_ = event->Get<double>("wt_ff_2");
        if(event->Exists("wt_ff_dmbins_1")) wt_ff_dmbins_1_ = event->Get<double>("wt_ff_dmbins_1");
        if(event->Exists("wt_ff_dmbins_2")) wt_ff_dmbins_2_ = event->Get<double>("wt_ff_dmbins_2");
        if(do_ff_systematics_){
          if(event->Exists("wt_ff_qcd_syst_up_1"            )) wt_ff_qcd_syst_up_1_               = event->Get<double>("wt_ff_qcd_syst_up_1");
          if(event->Exists("wt_ff_qcd_syst_down_1"          )) wt_ff_qcd_syst_down_1_             = event->Get<double>("wt_ff_qcd_syst_down_1");
          if(event->Exists("wt_ff_wjets_syst_up_1"              )) wt_ff_w_syst_up_1_                 = event->Get<double>("wt_ff_wjets_syst_up_1");
          if(event->Exists("wt_ff_wjets_syst_down_1"            )) wt_ff_w_syst_down_1_               = event->Get<double>("wt_ff_wjets_syst_down_1");
          if(event->Exists("wt_ff_ttbar_syst_up_1"             )) wt_ff_tt_syst_up_1_                = event->Get<double>("wt_ff_ttbar_syst_up_1");
          if(event->Exists("wt_ff_ttbar_syst_down_1"           )) wt_ff_tt_syst_down_1_              = event->Get<double>("wt_ff_ttbar_syst_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_1")) wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_1")) wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm0_sig_lt3_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_1")) wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_1")) wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm0_sig_gt3_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm1_up_1")) wt_ff_qcd_stat_njet0_mvadm1_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm1_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm1_down_1")) wt_ff_qcd_stat_njet0_mvadm1_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm1_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm2_up_1")) wt_ff_qcd_stat_njet0_mvadm2_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm2_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm2_down_1")) wt_ff_qcd_stat_njet0_mvadm2_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm2_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm10_up_1")) wt_ff_qcd_stat_njet0_mvadm10_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm10_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm10_down_1")) wt_ff_qcd_stat_njet0_mvadm10_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm10_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm11_up_1")) wt_ff_qcd_stat_njet0_mvadm11_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm11_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet0_mvadm11_down_1")) wt_ff_qcd_stat_njet0_mvadm11_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet0_mvadm11_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_1")) wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_1")) wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm0_sig_lt3_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_1")) wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_1")) wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm0_sig_gt3_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm1_up_1")) wt_ff_qcd_stat_njet1_mvadm1_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm1_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm1_down_1")) wt_ff_qcd_stat_njet1_mvadm1_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm1_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm2_up_1")) wt_ff_qcd_stat_njet1_mvadm2_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm2_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm2_down_1")) wt_ff_qcd_stat_njet1_mvadm2_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm2_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm10_up_1")) wt_ff_qcd_stat_njet1_mvadm10_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm10_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm10_down_1")) wt_ff_qcd_stat_njet1_mvadm10_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm10_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm11_up_1")) wt_ff_qcd_stat_njet1_mvadm11_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm11_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet1_mvadm11_down_1")) wt_ff_qcd_stat_njet1_mvadm11_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet1_mvadm11_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_1")) wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_1")) wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm0_sig_lt3_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_1")) wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_1")) wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm0_sig_gt3_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm1_up_1")) wt_ff_qcd_stat_njet2_mvadm1_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm1_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm1_down_1")) wt_ff_qcd_stat_njet2_mvadm1_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm1_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm2_up_1")) wt_ff_qcd_stat_njet2_mvadm2_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm2_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm2_down_1")) wt_ff_qcd_stat_njet2_mvadm2_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm2_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm10_up_1")) wt_ff_qcd_stat_njet2_mvadm10_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm10_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm10_down_1")) wt_ff_qcd_stat_njet2_mvadm10_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm10_down_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm11_up_1")) wt_ff_qcd_stat_njet2_mvadm11_up_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm11_up_1");
          if(event->Exists("wt_ff_qcd_stat_njet2_mvadm11_down_1")) wt_ff_qcd_stat_njet2_mvadm11_down_1_ = event->Get<double>("wt_ff_qcd_stat_njet2_mvadm11_down_1");
          if(event->Exists("wt_ff_qcd_stat_pt2_up_1")) wt_ff_qcd_stat_pt2_up_1_ = event->Get<double>("wt_ff_qcd_stat_pt2_up_1");
          if(event->Exists("wt_ff_qcd_stat_pt2_down_1")) wt_ff_qcd_stat_pt2_down_1_ = event->Get<double>("wt_ff_qcd_stat_pt2_down_1");
        }

      } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17){
        if(event->Exists("wt_ff_1")) wt_ff_1_ = event->Get<double>("wt_ff_1");    
        if(event->Exists("wt_ff_2")) wt_ff_2_ = event->Get<double>("wt_ff_2");
       
        if(event->Exists("wt_ff_qcd_1")) wt_ff_qcd_1_ = event->Get<double>("wt_ff_qcd_1"); 
        if(event->Exists("wt_ff_qcd_2")) wt_ff_qcd_2_ = event->Get<double>("wt_ff_qcd_2");
 
        if(do_ff_systematics_){
          
          if(event->Exists("wt_ff_qcd_syst_up_1"            )) wt_ff_qcd_syst_up_1_               = event->Get<double>("wt_ff_qcd_syst_up_1"            );
          if(event->Exists("wt_ff_qcd_syst_down_1"          )) wt_ff_qcd_syst_down_1_             = event->Get<double>("wt_ff_qcd_syst_down_1"          );
          if(event->Exists("wt_ff_qcd_dm0_njet0_stat_up_1"  )) wt_ff_qcd_dm0_njet0_stat_up_1_     = event->Get<double>("wt_ff_qcd_dm0_njet0_stat_up_1"  );
          if(event->Exists("wt_ff_qcd_dm0_njet0_stat_down_1")) wt_ff_qcd_dm0_njet0_stat_down_1_   = event->Get<double>("wt_ff_qcd_dm0_njet0_stat_down_1");
          if(event->Exists("wt_ff_qcd_dm0_njet1_stat_up_1"  )) wt_ff_qcd_dm0_njet1_stat_up_1_     = event->Get<double>("wt_ff_qcd_dm0_njet1_stat_up_1"  );
          if(event->Exists("wt_ff_qcd_dm0_njet1_stat_down_1")) wt_ff_qcd_dm0_njet1_stat_down_1_   = event->Get<double>("wt_ff_qcd_dm0_njet1_stat_down_1");
          if(event->Exists("wt_ff_qcd_dm1_njet0_stat_up_1"  )) wt_ff_qcd_dm1_njet0_stat_up_1_     = event->Get<double>("wt_ff_qcd_dm1_njet0_stat_up_1"  );
          if(event->Exists("wt_ff_qcd_dm1_njet0_stat_down_1")) wt_ff_qcd_dm1_njet0_stat_down_1_   = event->Get<double>("wt_ff_qcd_dm1_njet0_stat_down_1");
          if(event->Exists("wt_ff_qcd_dm1_njet1_stat_up_1"  )) wt_ff_qcd_dm1_njet1_stat_up_1_     = event->Get<double>("wt_ff_qcd_dm1_njet1_stat_up_1"  );
          if(event->Exists("wt_ff_qcd_dm1_njet1_stat_down_1")) wt_ff_qcd_dm1_njet1_stat_down_1_   = event->Get<double>("wt_ff_qcd_dm1_njet1_stat_down_1");
          if(event->Exists("wt_ff_realtau_up_1"              )) wt_ff_realtau_up_1_                  = event->Get<double>("wt_ff_realtau_up_1"             );
          if(event->Exists("wt_ff_realtau_down_1"              )) wt_ff_realtau_down_1_                  = event->Get<double>("wt_ff_realtau_down_1"             );

          if(event->Exists("wt_ff_w_syst_up_1"              )) wt_ff_w_syst_up_1_                 = event->Get<double>("wt_ff_w_syst_up_1"              );
          if(event->Exists("wt_ff_w_syst_down_1"            )) wt_ff_w_syst_down_1_               = event->Get<double>("wt_ff_w_syst_down_1"            );
          if(event->Exists("wt_ff_tt_syst_up_1"             )) wt_ff_tt_syst_up_1_                = event->Get<double>("wt_ff_tt_syst_up_1"             );
          if(event->Exists("wt_ff_tt_syst_down_1"           )) wt_ff_tt_syst_down_1_              = event->Get<double>("wt_ff_tt_syst_down_1"           );
          
          if(channel_==channel::et || channel_==channel::mt){
            if(event->Exists("wt_ff_w_dm0_njet0_stat_up_1"   )) wt_ff_w_dm0_njet0_stat_up_1_    = event->Get<double>("wt_ff_w_dm0_njet0_stat_up_1"   );
            if(event->Exists("wt_ff_w_dm0_njet0_stat_down_1" )) wt_ff_w_dm0_njet0_stat_down_1_  = event->Get<double>("wt_ff_w_dm0_njet0_stat_down_1" );  
            if(event->Exists("wt_ff_w_dm0_njet1_stat_up_1"   )) wt_ff_w_dm0_njet1_stat_up_1_    = event->Get<double>("wt_ff_w_dm0_njet1_stat_up_1"   );  
            if(event->Exists("wt_ff_w_dm0_njet1_stat_down_1" )) wt_ff_w_dm0_njet1_stat_down_1_  = event->Get<double>("wt_ff_w_dm0_njet1_stat_down_1" );  
            if(event->Exists("wt_ff_w_dm1_njet0_stat_up_1"   )) wt_ff_w_dm1_njet0_stat_up_1_    = event->Get<double>("wt_ff_w_dm1_njet0_stat_up_1"   );  
            if(event->Exists("wt_ff_w_dm1_njet0_stat_down_1" )) wt_ff_w_dm1_njet0_stat_down_1_  = event->Get<double>("wt_ff_w_dm1_njet0_stat_down_1" );  
            if(event->Exists("wt_ff_w_dm1_njet1_stat_up_1"   )) wt_ff_w_dm1_njet1_stat_up_1_    = event->Get<double>("wt_ff_w_dm1_njet1_stat_up_1"   );  
            if(event->Exists("wt_ff_w_dm1_njet1_stat_down_1" )) wt_ff_w_dm1_njet1_stat_down_1_  = event->Get<double>("wt_ff_w_dm1_njet1_stat_down_1" );  
            if(event->Exists("wt_ff_tt_dm0_njet0_stat_up_1"  )) wt_ff_tt_dm0_njet0_stat_up_1_   = event->Get<double>("wt_ff_tt_dm0_njet0_stat_up_1"  );  
            if(event->Exists("wt_ff_tt_dm0_njet0_stat_down_1")) wt_ff_tt_dm0_njet0_stat_down_1_ = event->Get<double>("wt_ff_tt_dm0_njet0_stat_down_1");  
            if(event->Exists("wt_ff_tt_dm0_njet1_stat_up_1"  )) wt_ff_tt_dm0_njet1_stat_up_1_   = event->Get<double>("wt_ff_tt_dm0_njet1_stat_up_1"  );  
            if(event->Exists("wt_ff_tt_dm0_njet1_stat_down_1")) wt_ff_tt_dm0_njet1_stat_down_1_ = event->Get<double>("wt_ff_tt_dm0_njet1_stat_down_1");  
            if(event->Exists("wt_ff_tt_dm1_njet0_stat_up_1"  )) wt_ff_tt_dm1_njet0_stat_up_1_   = event->Get<double>("wt_ff_tt_dm1_njet0_stat_up_1"  );  
            if(event->Exists("wt_ff_tt_dm1_njet0_stat_down_1")) wt_ff_tt_dm1_njet0_stat_down_1_ = event->Get<double>("wt_ff_tt_dm1_njet0_stat_down_1");  
            if(event->Exists("wt_ff_tt_dm1_njet1_stat_up_1"  )) wt_ff_tt_dm1_njet1_stat_up_1_   = event->Get<double>("wt_ff_tt_dm1_njet1_stat_up_1"  );  
            if(event->Exists("wt_ff_tt_dm1_njet1_stat_down_1")) wt_ff_tt_dm1_njet1_stat_down_1_ = event->Get<double>("wt_ff_tt_dm1_njet1_stat_down_1");  
          }
          if(channel_==channel::tt){
            if(event->Exists("wt_ff_qcd_syst_up_2"            )) wt_ff_qcd_syst_up_2_                = event->Get<double>("wt_ff_qcd_syst_up_2"            );
            if(event->Exists("wt_ff_qcd_syst_down_2"          )) wt_ff_qcd_syst_down_2_              = event->Get<double>("wt_ff_qcd_syst_down_2"          );
            if(event->Exists("wt_ff_qcd_dm0_njet0_stat_up_2"  )) wt_ff_qcd_dm0_njet0_stat_up_2_      = event->Get<double>("wt_ff_qcd_dm0_njet0_stat_up_2"  );
            if(event->Exists("wt_ff_qcd_dm0_njet0_stat_down_2")) wt_ff_qcd_dm0_njet0_stat_down_2_    = event->Get<double>("wt_ff_qcd_dm0_njet0_stat_down_2");
            if(event->Exists("wt_ff_qcd_dm0_njet1_stat_up_2"  )) wt_ff_qcd_dm0_njet1_stat_up_2_      = event->Get<double>("wt_ff_qcd_dm0_njet1_stat_up_2"  );
            if(event->Exists("wt_ff_qcd_dm0_njet1_stat_down_2")) wt_ff_qcd_dm0_njet1_stat_down_2_    = event->Get<double>("wt_ff_qcd_dm0_njet1_stat_down_2");
            if(event->Exists("wt_ff_qcd_dm1_njet0_stat_up_2"  )) wt_ff_qcd_dm1_njet0_stat_up_2_      = event->Get<double>("wt_ff_qcd_dm1_njet0_stat_up_2"  );
            if(event->Exists("wt_ff_qcd_dm1_njet0_stat_down_2")) wt_ff_qcd_dm1_njet0_stat_down_2_    = event->Get<double>("wt_ff_qcd_dm1_njet0_stat_down_2");
            if(event->Exists("wt_ff_qcd_dm1_njet1_stat_up_2"  )) wt_ff_qcd_dm1_njet1_stat_up_2_      = event->Get<double>("wt_ff_qcd_dm1_njet1_stat_up_2"  );
            if(event->Exists("wt_ff_qcd_dm1_njet1_stat_down_2")) wt_ff_qcd_dm1_njet1_stat_down_2_    = event->Get<double>("wt_ff_qcd_dm1_njet1_stat_down_2");
            if(event->Exists("wt_ff_realtau_up_2"              )) wt_ff_realtau_up_2_                  = event->Get<double>("wt_ff_realtau_up_2"             );
            if(event->Exists("wt_ff_realtau_down_2"              )) wt_ff_realtau_down_2_                  = event->Get<double>("wt_ff_realtau_down_2"             );
            if(event->Exists("wt_ff_w_syst_up_2"              )) wt_ff_w_syst_up_2_                  = event->Get<double>("wt_ff_w_syst_up_2"              );
            if(event->Exists("wt_ff_w_syst_down_2"            )) wt_ff_w_syst_down_2_                = event->Get<double>("wt_ff_w_syst_down_2"            );
            if(event->Exists("wt_ff_tt_syst_up_2"             )) wt_ff_tt_syst_up_2_                 = event->Get<double>("wt_ff_tt_syst_up_2"             );
            if(event->Exists("wt_ff_tt_syst_down_2"           )) wt_ff_tt_syst_down_2_               = event->Get<double>("wt_ff_tt_syst_down_2"           );
            if(event->Exists("wt_ff_w_frac_syst_up_1"         )) wt_ff_w_frac_syst_up_1_                       = event->Get<double>("wt_ff_w_frac_syst_up_1"         );
            if(event->Exists("wt_ff_w_frac_syst_down_1"       )) wt_ff_w_frac_syst_down_1_                     = event->Get<double>("wt_ff_w_frac_syst_down_1"       );
            if(event->Exists("wt_ff_tt_frac_syst_up_1"        )) wt_ff_tt_frac_syst_up_1_                      = event->Get<double>("wt_ff_tt_frac_syst_up_1"        );
            if(event->Exists("wt_ff_tt_frac_syst_down_1"      )) wt_ff_tt_frac_syst_down_1_                    = event->Get<double>("wt_ff_tt_frac_syst_down_1"      );
            if(event->Exists("wt_ff_dy_frac_syst_up_1"        )) wt_ff_dy_frac_syst_up_1_                      = event->Get<double>("wt_ff_dy_frac_syst_up_1"        );
            if(event->Exists("wt_ff_dy_frac_syst_down_1"      )) wt_ff_dy_frac_syst_down_1_                    = event->Get<double>("wt_ff_dy_frac_syst_down_1"      );
            if(event->Exists("wt_ff_w_frac_syst_up_2"         )) wt_ff_w_frac_syst_up_2_                       = event->Get<double>("wt_ff_w_frac_syst_up_2"         );
            if(event->Exists("wt_ff_w_frac_syst_down_2"       )) wt_ff_w_frac_syst_down_2_                     = event->Get<double>("wt_ff_w_frac_syst_down_2"       );
            if(event->Exists("wt_ff_tt_frac_syst_up_2"        )) wt_ff_tt_frac_syst_up_2_                      = event->Get<double>("wt_ff_tt_frac_syst_up_2"        );
            if(event->Exists("wt_ff_tt_frac_syst_down_2"      )) wt_ff_tt_frac_syst_down_2_                    = event->Get<double>("wt_ff_tt_frac_syst_down_2"      );
            if(event->Exists("wt_ff_dy_frac_syst_up_2"        )) wt_ff_dy_frac_syst_up_2_                      = event->Get<double>("wt_ff_dy_frac_syst_up_2"        );
            if(event->Exists("wt_ff_dy_frac_syst_down_2"      )) wt_ff_dy_frac_syst_down_2_                    = event->Get<double>("wt_ff_dy_frac_syst_down_2"      );
          }
            
        }

      }
    }
    
    
   if(do_qcd_scale_wts_){
     // note some of these labels may be generator dependent so need to make sure you check before using them
      
     if(eventInfo->weight_defined("1001")) scale1_ = eventInfo->weight("1001"); else scale1_=1.0;
     if(eventInfo->weight_defined("1002")) scale2_ = eventInfo->weight("1002"); else scale2_=1.0;
     if(eventInfo->weight_defined("1003")) scale3_ = eventInfo->weight("1003"); else scale3_=1.0;
     if(eventInfo->weight_defined("1004")) scale4_ = eventInfo->weight("1004"); else scale4_=1.0;
     if(eventInfo->weight_defined("1005")) scale5_ = eventInfo->weight("1005"); else scale5_=1.0;
     if(eventInfo->weight_defined("1006")) scale6_ = eventInfo->weight("1006"); else scale6_=1.0;
     if(eventInfo->weight_defined("1007")) scale7_ = eventInfo->weight("1007"); else scale7_=1.0;
     if(eventInfo->weight_defined("1008")) scale8_ = eventInfo->weight("1008"); else scale8_=1.0;
     if(eventInfo->weight_defined("1009")) scale9_ = eventInfo->weight("1009"); else scale9_=1.0;    

     // W-jets weights are numbered 1-9 - be careful this doesn't overwrite scale weights for other samples with some other weight!
     if(eventInfo->weight_defined("1")) scale1_ = eventInfo->weight("1"); else scale1_=1.0;
     if(eventInfo->weight_defined("2")) scale2_ = eventInfo->weight("2"); else scale2_=1.0;
     if(eventInfo->weight_defined("3")) scale3_ = eventInfo->weight("3"); else scale3_=1.0;
     if(eventInfo->weight_defined("4")) scale4_ = eventInfo->weight("4"); else scale4_=1.0;
     if(eventInfo->weight_defined("5")) scale5_ = eventInfo->weight("5"); else scale5_=1.0;
     if(eventInfo->weight_defined("6")) scale6_ = eventInfo->weight("6"); else scale6_=1.0;
     if(eventInfo->weight_defined("7")) scale7_ = eventInfo->weight("7"); else scale7_=1.0;
     if(eventInfo->weight_defined("8")) scale8_ = eventInfo->weight("8"); else scale8_=1.0;
     if(eventInfo->weight_defined("9")) scale9_ = eventInfo->weight("9"); else scale9_=1.0;
   }
    
    std::vector<PileupInfo *> puInfo;
    float true_int = -1;

    if (event->Exists("pileupInfo") || strategy_ == strategy::phys14 || ((strategy_==strategy::spring15||strategy_==strategy::fall15||strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) && !is_data_ && !is_embedded_) ) {
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
    if(event->Exists("gen_match_1_pt")) gen_match_1_pt_ = event->Get<double>("gen_match_1_pt");
    if(event->Exists("gen_match_2_pt")) gen_match_2_pt_ = event->Get<double>("gen_match_2_pt");
    if(event->Exists("gen_met")) gen_met_ = event->Get<double>("gen_met");
    if(event->Exists("fake_met")) fake_met_ = event->Get<double>("fake_met");
    /*if(event->Exists("leading_lepton_match_pt")) leading_lepton_match_pt_ = event->Get<double>("leading_lepton_match_pt");
    if(event->Exists("subleading_lepton_match_pt")) subleading_lepton_match_pt_ = event->Get<double>("subleading_lepton_match_pt");
    if(event->Exists("leading_lepton_match_DR")) leading_lepton_match_DR_ = event->Get<double>("leading_lepton_match_DR");
    if(event->Exists("subleading_lepton_match_DR")) subleading_lepton_match_DR_ = event->Get<double>("subleading_lepton_match_DR");*/

    if(event->Exists("ngenjets")) ngenjets_ = event->Get<unsigned>("ngenjets");
    if(event->Exists("gen_sjdphi")) gen_sjdphi_ = event->Get<double>("gen_sjdphi");
    if(event->Exists("gen_mjj")) gen_mjj_ = event->Get<double>("gen_mjj");
    if(event->Exists("tauFlag1")) tauFlag_1_ = event->Get<int>("tauFlag1");
    if(event->Exists("tauFlag2")) tauFlag_2_ = event->Get<int>("tauFlag2");
   
    wt_ggh_pt_up_ = 1.0;
    wt_ggh_pt_down_ = 1.0;
    wt_tau_fake_up_ = 1.0;
    wt_tau_fake_down_ = 1.0;
    wt_tquark_up_ = 1.0;
    wt_tquark_down_ = 1.0;
    wt_zpt_up_ = 1.0;
    wt_zpt_down_ = 1.0;
    wt_tau_id_up_ = 1.0;
    wt_tau_id_down_ = 1.0;
    wt_em_qcd_ = 1.0;
    wt_nlo_pt_ = 1.0;
    nlo_pt_ = 9999.;
    wt_embed_mc_yield_ = 1.0;
    if (event->Exists("wt_ggh_pt_up"))      wt_ggh_pt_up_   = event->Get<double>("wt_ggh_pt_up");
    if (event->Exists("wt_ggh_pt_down"))    wt_ggh_pt_down_ = event->Get<double>("wt_ggh_pt_down");
    if (event->Exists("wt_tau_fake_up"))    wt_tau_fake_up_   = event->Get<double>("wt_tau_fake_up");
    if (event->Exists("wt_tau_fake_down"))  wt_tau_fake_down_ = event->Get<double>("wt_tau_fake_down");
    if (event->Exists("wt_tquark_up"))      wt_tquark_up_   = event->Get<double>("wt_tquark_up");
    if (event->Exists("wt_tquark_down"))    wt_tquark_down_ = event->Get<double>("wt_tquark_down");
    if (event->Exists("wt_zpt_up"))         wt_zpt_up_   = event->Get<double>("wt_zpt_up");
    if (event->Exists("wt_zpt_down"))       wt_zpt_down_ = event->Get<double>("wt_zpt_down");
    if (event->Exists("wt_tau_id_up"))      wt_tau_id_up_   = event->Get<double>("wt_tau_id_up");
    if (event->Exists("wt_tau_id_down"))    wt_tau_id_down_ = event->Get<double>("wt_tau_id_down");
    if (event->Exists("wt_em_qcd"))         wt_em_qcd_ = event->Get<double>("wt_em_qcd");
    if (event->Exists("wt_em_qcd_extrapup"))      wt_em_qcd_extrapup_ = event->Get<double>("wt_em_qcd_extrapup");
    if (event->Exists("wt_em_qcd_extrapdown"))    wt_em_qcd_extrapdown_ = event->Get<double>("wt_em_qcd_extrapdown");

    wt_em_qcd_njets0_unc1_up_   = (event->Exists("wt_em_qcd_njets0_unc1_up"  )) ? event->Get<double>("wt_em_qcd_njets0_unc1_up"  ) : 1.0; 
    wt_em_qcd_njets0_unc1_down_ = (event->Exists("wt_em_qcd_njets0_unc1_down")) ? event->Get<double>("wt_em_qcd_njets0_unc1_down") : 1.0; 
    wt_em_qcd_njets0_unc2_up_   = (event->Exists("wt_em_qcd_njets0_unc2_up"  )) ? event->Get<double>("wt_em_qcd_njets0_unc2_up"  ) : 1.0; 
    wt_em_qcd_njets0_unc2_down_ = (event->Exists("wt_em_qcd_njets0_unc2_down")) ? event->Get<double>("wt_em_qcd_njets0_unc2_down") : 1.0; 
    wt_em_qcd_njets1_unc1_up_   = (event->Exists("wt_em_qcd_njets1_unc1_up"  )) ? event->Get<double>("wt_em_qcd_njets1_unc1_up"  ) : 1.0; 
    wt_em_qcd_njets1_unc1_down_ = (event->Exists("wt_em_qcd_njets1_unc1_down")) ? event->Get<double>("wt_em_qcd_njets1_unc1_down") : 1.0; 
    wt_em_qcd_njets1_unc2_up_   = (event->Exists("wt_em_qcd_njets1_unc2_up"  )) ? event->Get<double>("wt_em_qcd_njets1_unc2_up"  ) : 1.0; 
    wt_em_qcd_njets1_unc2_down_ = (event->Exists("wt_em_qcd_njets1_unc2_down")) ? event->Get<double>("wt_em_qcd_njets1_unc2_down") : 1.0; 
    wt_em_qcd_njets2_unc1_up_   = (event->Exists("wt_em_qcd_njets2_unc1_up"  )) ? event->Get<double>("wt_em_qcd_njets2_unc1_up"  ) : 1.0; 
    wt_em_qcd_njets2_unc1_down_ = (event->Exists("wt_em_qcd_njets2_unc1_down")) ? event->Get<double>("wt_em_qcd_njets2_unc1_down") : 1.0; 
    wt_em_qcd_njets2_unc2_up_   = (event->Exists("wt_em_qcd_njets2_unc2_up"  )) ? event->Get<double>("wt_em_qcd_njets2_unc2_up"  ) : 1.0; 
    wt_em_qcd_njets2_unc2_down_ = (event->Exists("wt_em_qcd_njets2_unc2_down")) ? event->Get<double>("wt_em_qcd_njets2_unc2_down") : 1.0;

    if(event->Exists("mssm_nlo_wt"))        wt_nlo_pt_ = event->Get<double>("mssm_nlo_wt");
    if(event->Exists("mssm_nlo_pt"))        nlo_pt_ = event->Get<double>("mssm_nlo_pt");
    wt_embed_mc_yield_ = event->Exists("wt_embed_mc_yield") ? event->Get<double>("wt_embed_mc_yield") : 1.0;

    wt_zpt_stat_m400pt0_up     = 1.0;
    wt_zpt_stat_m400pt40_up    = 1.0;
    wt_zpt_stat_m400pt80_up    = 1.0;
    wt_zpt_stat_m400pt0_down   = 1.0;
    wt_zpt_stat_m400pt40_down  = 1.0;
    wt_zpt_stat_m400pt80_down  = 1.0;
    wt_zpt_esup                = 1.0;
    wt_zpt_esdown              = 1.0;
    wt_zpt_ttup                = 1.0;
    wt_zpt_ttdown              = 1.0;
    
    if (event->Exists("wt_zpt_stat_m400pt0_up"    )) wt_zpt_stat_m400pt0_up     = event->Get<double>("wt_zpt_stat_m400pt0_up"    );
    if (event->Exists("wt_zpt_stat_m400pt40_up"   )) wt_zpt_stat_m400pt40_up    = event->Get<double>("wt_zpt_stat_m400pt40_up"   );
    if (event->Exists("wt_zpt_stat_m400pt80_up"   )) wt_zpt_stat_m400pt80_up    = event->Get<double>("wt_zpt_stat_m400pt80_up"   );
    if (event->Exists("wt_zpt_stat_m400pt0_down"  )) wt_zpt_stat_m400pt0_down   = event->Get<double>("wt_zpt_stat_m400pt0_down"  );
    if (event->Exists("wt_zpt_stat_m400pt40_down" )) wt_zpt_stat_m400pt40_down  = event->Get<double>("wt_zpt_stat_m400pt40_down" );
    if (event->Exists("wt_zpt_stat_m400pt80_down" )) wt_zpt_stat_m400pt80_down  = event->Get<double>("wt_zpt_stat_m400pt80_down" );
    if (event->Exists("wt_zpt_esup"               )) wt_zpt_esup                = event->Get<double>("wt_zpt_esup"               );
    if (event->Exists("wt_zpt_esdown"             )) wt_zpt_esdown              = event->Get<double>("wt_zpt_esdown"             );
    if (event->Exists("wt_zpt_ttup"               )) wt_zpt_ttup                = event->Get<double>("wt_zpt_ttup"               );
    if (event->Exists("wt_zpt_ttdown"             )) wt_zpt_ttdown              = event->Get<double>("wt_zpt_ttdown"             );
    
  mc_weight_ = 0.0;
  if (!is_embedded_ && eventInfo->weight_defined("pileup")) pu_weight_ = eventInfo->weight("pileup"); else pu_weight_ = 0.0;
  if (event->Exists("trigweight_1")) trigweight_1_ = event->Get<double>("trigweight_1"); else trigweight_1_ = 1.0;
  if (event->Exists("trigweight_2")) trigweight_2_ = event->Get<double>("trigweight_2"); else trigweight_2_ = 1.0;
  if (event->Exists("trigweight_up_1")) wt_trig_up_1_ = event->Get<double>("trigweight_up_1"); else wt_trig_up_1_ = 1.0;
  if (event->Exists("trigweight_up_2")) wt_trig_up_2_ = event->Get<double>("trigweight_up_2"); else wt_trig_up_2_ = 1.0;
  if (event->Exists("trigweight_down_1")) wt_trig_down_1_ = event->Get<double>("trigweight_down_1"); else wt_trig_down_1_ = 1.0;
  if (event->Exists("trigweight_down_2")) wt_trig_down_2_ = event->Get<double>("trigweight_down_2"); else wt_trig_down_2_ = 1.0;
  if (event->Exists("trigweight_up")) wt_trig_up_ = event->Get<double>("trigweight_up"); else wt_trig_up_ = 1.0;
  if (event->Exists("trigweight_down")) wt_trig_down_ = event->Get<double>("trigweight_down"); else wt_trig_down_ = 1.0; 
  if (event->Exists("xtrg_sf")) xtrg_sf_ = event->Get<double>("xtrg_sf"); else xtrg_sf_ = 1.0;
  if (event->Exists("single_l_sf")) single_l_sf_ = event->Get<double>("single_l_sf"); else single_l_sf_ = 1.0;
  if (event->Exists("xtrg_notrig")) xtrg_notrig_ = event->Get<double>("xtrg_notrig"); else xtrg_notrig_ = 1.0;
  if (event->Exists("OR_notrig")) OR_notrig_ = event->Get<double>("OR_notrig"); else OR_notrig_ = 1.0; 
  if (event->Exists("idisoweight_1")) idisoweight_1_ = event->Get<double>("idisoweight_1"); else idisoweight_1_ = 0.0;
  if (event->Exists("idisoweight_2")) idisoweight_2_ = event->Get<double>("idisoweight_2"); else idisoweight_2_ = 0.0;
  if(channel_==channel::em){
    if(event->Exists("idisoweight_up_1")) idisoweight_up_1_ = event->Get<double>("idisoweight_up_1"); else idisoweight_up_1_ = 1.0;
    if(event->Exists("idisoweight_up_2")) idisoweight_up_2_ = event->Get<double>("idisoweight_up_2"); else idisoweight_up_2_ = 1.0;
    if(event->Exists("idisoweight_down_1")) idisoweight_down_1_ = event->Get<double>("idisoweight_down_1"); else  idisoweight_down_1_ = 1.0;
    if(event->Exists("idisoweight_down_2")) idisoweight_down_2_ = event->Get<double>("idisoweight_down_2"); else idisoweight_down_2_=1.0;
  }
  if (event->Exists("trackingweight_1")) trackingweight_1_ = event->Get<double>("trackingweight_1"); else trackingweight_1_ = 0.0;
  if (event->Exists("trackingweight_2")) trackingweight_2_ = event->Get<double>("trackingweight_2"); else trackingweight_2_ = 0.0;
//  if (event->Exists("isoweight_1")) isoweight_1_ = event->Get<double>("isoweight_1"); else isoweight_1_ = 0.0;
//  if (event->Exists("isoweight_2")) isoweight_2_ = event->Get<double>("isoweight_2"); else isoweight_2_ = 0.0;
  if (eventInfo->weight_defined("lepton")) effweight_ = eventInfo->weight("lepton"); else effweight_ = 0.0;
  if (eventInfo->weight_defined("tau_fake_weight")) fakeweight_ = eventInfo->weight("tau_fake_weight"); else fakeweight_ = 0.0;
  if (eventInfo->weight_defined("tau_mode_scale")) effweight_ *= eventInfo->weight("tau_mode_scale") ;

  // temporary tau trg effs
  if (event->Exists("tau1_trgeff_mc")) tau1_trgeff_mc_ = event->Get<double>("tau1_trgeff_mc"); else tau1_trgeff_mc_ = 1.0;
  if (event->Exists("tau2_trgeff_mc")) tau2_trgeff_mc_ = event->Get<double>("tau2_trgeff_mc"); else tau2_trgeff_mc_ = 1.0;
   
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
    mets = event->GetPtr<Met>(met_label_);

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::vector<PFJet*> uncleaned_jets = event->GetPtrVec<PFJet>(jets_label_+"UnFiltered");
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
    std::string btag_label_extra ="combinedSecondaryVertexBJetTags";
    double btag_wp =  0.679;
    double loose_btag_wp = 0.244;
    if(strategy_ == strategy::phys14) btag_label = "combinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::phys14) btag_wp = 0.814 ;
    if(strategy_ == strategy::spring15) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::spring15) btag_wp = 0.89 ;
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||
      strategy_ == strategy::smspring16  || strategy_ == strategy::mssmsummer16 || 
      strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16|| strategy_ == strategy::cpdecays16) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16) btag_wp = 0.8;
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16) btag_wp = 0.8484;
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 || 
      strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || 
      strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) loose_btag_wp = 0.46;
    if (era_ == era::data_2017) {
      btag_wp = 0.4941;
      loose_btag_wp = 0.1522;
      btag_label = "pfDeepCSVJetTags:probb";
      btag_label_extra = "pfDeepCSVJetTags:probbb";
    }
    if (era_ == era::data_2018) {
      btag_wp = 0.4184;
      loose_btag_wp = 0.1241;
      btag_label = "pfDeepCSVJetTags:probb";
      btag_label_extra = "pfDeepCSVJetTags:probbb";
    }
    if (era_ == era::data_2016) {
      btag_wp = 0.6321;
      loose_btag_wp = 0.2217;
      btag_label = "pfDeepCSVJetTags:probb";
      btag_label_extra = "pfDeepCSVJetTags:probbb";
    }


    auto sortRuleBTagSum = [btag_label, btag_label_extra] (PFJet* s1, PFJet* s2) -> bool {
      return s1->GetBDiscriminator(btag_label) + s1->GetBDiscriminator(btag_label_extra) > 
        s2->GetBDiscriminator(btag_label) + s2->GetBDiscriminator(btag_label_extra);
    };
    auto filterBTagSumTight = [btag_label, btag_label_extra, btag_wp] (PFJet* s1) -> bool {
      return s1->GetBDiscriminator(btag_label) + s1->GetBDiscriminator(btag_label_extra) > btag_wp;
    };
    auto filterBTagSumLoose = [btag_label, btag_label_extra, loose_btag_wp] (PFJet* s1) -> bool {
      return s1->GetBDiscriminator(btag_label) + s1->GetBDiscriminator(btag_label_extra) > loose_btag_wp;
    };

   //Extra set of jets which are CSV ordered is required for the H->hh analysis
    std::vector<PFJet*> jets_csv = prebjets;
    std::vector<PFJet*> bjets_csv = prebjets;
    if (era_ != era::data_2018)
      std::sort(jets_csv.begin(), jets_csv.end(), bind(&PFJet::GetBDiscriminator, _1, btag_label) > bind(&PFJet::GetBDiscriminator, _2, btag_label));
    else {
        std::sort(jets_csv.begin(), jets_csv.end(), sortRuleBTagSum);
    }

    std::vector<std::pair<PFJet*,PFJet*> > jet_csv_pairs;
    if(bjet_regression_) jet_csv_pairs = MatchByDR(jets_csv, corrected_jets, 0.5, true, true);

    //Sort out the loose (em,mt,et) or medium (tt) b-jets
    if(era_ != era::data_2016 && era_ != era::data_2017 && era_ != era::data_2018){
      if(channel_!= channel::tt){
        ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < loose_btag_wp);
      } else {
        ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) <btag_wp);
        ic::erase_if(bjets_csv, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) <btag_wp);
      }
      // Instead of changing b-tag value in the promote/demote method we look for a map of bools
      // that say whether a jet should pass the WP or not
      if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
         if(channel_ != channel::tt ){
            ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
            ic::erase_if(bjets_csv, !boost::bind(IsReBTagged, _1, retag_result));
         } else {
            ic::erase_if(loose_bjets, !boost::bind(IsReBTagged, _1, retag_result));
        }
      } else{ 
        if(channel_ != channel::tt){
          ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
          ic::erase_if(bjets_csv, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
        } else {
          ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < loose_btag_wp);
        }
      } 
    } 
    else if (era_ == era::data_2017 || era_ == era::data_2018 || era_ == era::data_2016) {
      ic::erase_if_not(loose_bjets, filterBTagSumLoose);
      // Instead of changing b-tag value in the promote/demote method we look for a map of bools
      // that say whether a jet should pass the WP or not
      if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
        ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
        ic::erase_if(bjets_csv, !boost::bind(IsReBTagged, _1, retag_result));
      } else{ 
        ic::erase_if_not(bjets, filterBTagSumTight);
        ic::erase_if_not(bjets_csv, filterBTagSumTight);
      } 
    }

    
    //Compare with btag shape reweighting:
    if(event->Exists("btag_evt_weight")){
     wt_btag_ = event->Get<double>("btag_evt_weight");
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
    bool dilep_veto_=false;
    if(channel_ == channel::et) { 
        if(event->Exists("dielec_veto")) dilepton_veto_ = event->Get<bool>("dielec_veto");
        if(event->Exists("dimuon_veto")) dilep_veto_ = event->Get<bool>("dimuon_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");
    }
    if(channel_ == channel::mt) { 
        if(event->Exists("dimuon_veto")) dilepton_veto_ = event->Get<bool>("dimuon_veto");
        if(event->Exists("dielec_veto")) dilep_veto_ = event->Get<bool>("dielec_veto");
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
        if(strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
          if(event->Exists("dimuon_veto")) dilep_veto_ = event->Get<bool>("dimuon_veto");
          if(event->Exists("dielec_veto")) dilep_veto_ = dilep_veto_ || event->Get<bool>("dielec_veto");
        }
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");

    }
    lepton_veto_ = dilepton_veto_ || extraelec_veto_ || extramuon_veto_;
    
    if((strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) && !make_sync_ntuple_) dilepton_veto_ = dilep_veto_ || dilepton_veto_;

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
    if(strategy_ != strategy::paper2013) {
      std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
      pfmet = pfMet_vec.at(0);  
      if(event->ExistsInTree("puppiMet")){
        std::vector<Met*> puppiMet_vec = event->GetPtrVec<Met>("puppiMet");
        puppimet = puppiMet_vec.at(0);
      }
    }
    if(strategy_ == strategy::smspring16 || strategy_ == strategy::mssmspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) pfmet = event->GetPtr<Met>("pfMET");

    pfpt_tt_ = (ditau->vector() + pfmet->vector()).pt();
    //mvapt_tt_ = (ditau->vector() + mets->vector()).pt();
    pt_tt_ = (ditau->vector() + mets->vector()).pt();

    if(channel_ == channel::zmm || channel_ == channel::zee) pt_tt_ = (ditau->vector()).pt(); 
    m_vis_ = ditau->M();
    pt_vis_ = ditau->pt();

    

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
    mt_ll_ = MT(ditau, mets);
    pzeta_ = PZeta(ditau, mets, 0.85);
    pzetamiss_ = PZeta(ditau, mets, 0.0);
    pfpzeta_ = PZeta(ditau, pfmet, 0.85);
    pfpzetamiss_ = PZeta(ditau, pfmet, 0.0);
    pzetavis_ = PZetaVis(ditau);

    met_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(mets->vector(),lep1->vector()));
    met_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(mets->vector(),lep2->vector()));
    //save some pfmet and puppi met versions as well for now
    pfmt_1_ = MT(lep1, pfmet);
    pfmt_2_ = MT(lep2, pfmet);
    pfmt_tot_ = sqrt(pow(mt_lep_.var_double,2)+pow(pfmt_2_.var_double,2)+pow(pfmt_1_.var_double,2));
    mt_1_ = MT(lep1, mets);
    mt_2_ = MT(lep2, mets);
    mt_tot_ = sqrt(pow(mt_lep_.var_double,2)+pow(mt_2_.var_double,2)+pow(mt_1_.var_double,2));

    if(puppimet != NULL){
      puppimt_1_ = MT(lep1, puppimet);
      puppipzeta_ = PZeta(ditau, puppimet, 0.85);
      puppipzetamiss_ = PZeta(ditau, puppimet,0.0);
    }


    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    eta_1_ = lep1->eta();
    //std::cout << pt_1_.var_double << "    " << eta_1_.var_double << std::endl;
    eta_2_ = lep2->eta();
    phi_1_ = lep1->phi();
    phi_2_ = lep2->phi();
    dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector()));
    dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep2->vector()));
    E_1_ = lep1->energy();
    E_2_ = lep2->energy();
    m_1_ = lep1->M();
    m_2_ = lep2->M();
    q_1_ = lep1->charge();
    q_2_ = lep2->charge();


    if(make_sync_ntuple_){
      event->Exists("genpX") ? gen_px_ = event->Get<double>("genpX") : 0.;
      event->Exists("genpY") ? gen_py_ = event->Get<double>("genpY") : 0.;
      event->Exists("vispX") ? vis_px_ = event->Get<double>("vispX") : 0.;
      event->Exists("vispY") ? vis_py_ = event->Get<double>("vispY") : 0.;
    }
    met_ = mets->vector().pt();
    met_phi_ = mets->vector().phi();

    event->Exists("genM") ? gen_m_ = event->Get<double>("genM") : 0.;
    event->Exists("genpT") ? gen_pt_ = event->Get<double>("genpT") : 0.;

    uncorrmet_ = met_;
    if (event->Exists("met_norecoil")) uncorrmet_ = event->Get<double>("met_norecoil");
    uncorrmet_phi_ = met_phi_;
    if (event->Exists("met_phi_norecoil")) uncorrmet_phi_ = event->Get<double>("met_phi_norecoil");

    metCov00_ = mets->xx_sig();
    metCov10_ = mets->yx_sig();
    metCov01_ = mets->xy_sig();
    metCov11_ = mets->yy_sig();
    
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

        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        antiele_2_ = lagainstElectronTightMVA_2;
        antimu_2_ = lagainstMuonLoose3_2;
      }
      if(strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA5raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
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
        antiele_2_ = lagainstElectronTightMVA_2;
        antimu_2_ = lagainstMuonLoose3_2;
      }
      if(strategy_ == strategy::fall15) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        mva_2_ = tau->GetTauID("againstElectronMVA6raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA6") ? tau->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA6") ? tau->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA6") ? tau->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA6") ? tau->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA6") ? tau->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lchargedRelIsoPtSum_2 = lchargedIsoPtSum_2/tau->pt();
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lPhotonPtSum_2 = tau->HasTauID("photonPtSumOutsideSignalCone") ? tau->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVArun2DBnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        antiele_2_ = lagainstElectronTightMVA_2;
        antimu_2_ = lagainstMuonLoose3_2;
      }
      if(strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        if (strategy_ == strategy::legacy16)
            iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
        else
            iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        mva_1_ = elec->GetIdIso("generalPurposeMVASpring16");
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA6") ? tau->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA6") ? tau->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA6") ? tau->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA6") ? tau->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA6") ? tau->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lchargedRelIsoPtSum_2 = lchargedIsoPtSum_2/tau->pt();
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lPhotonPtSum_2 = tau->HasTauID("photonPtSumOutsideSignalCone") ? tau->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVArun2DBnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        antiele_2_ = lagainstElectronTightMVA_2;
        antimu_2_ = lagainstMuonLoose3_2;
      } 
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //should be lepton_rho but its the same
        iso_2_ = tau->GetTauID("byIsolationMVArun2017v2DBoldDMwLTraw2017");
        tau_id_olddm_vloose_2_  = tau->HasTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vloose_2_  = tau->HasTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_loose_2_   = tau->HasTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_loose_2_   = tau->HasTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_medium_2_  = tau->HasTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_medium_2_  = tau->HasTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_tight_2_   = tau->HasTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_tight_2_   = tau->HasTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vtight_2_  = tau->HasTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vtight_2_  = tau->HasTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vvtight_2_ = tau->HasTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vvtight_2_ = tau->HasTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
      }
      // add deepTau ID v2
      // Raw DNN scores
      deepTauVsJets_iso_2_      = tau->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_2_       = tau->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_2_        = tau->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing workpoints
      deepTauVsJets_vvvloose_2_ = tau->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_2_  = tau->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_2_   = tau->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_    = tau->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_     = tau->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_      = tau->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_     = tau->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_      = tau->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_     = tau->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_    = tau->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

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
        mva_1_ = 0.0;
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        mva_2_ = tau->GetTauID("againstElectronMVA5raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
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
        antiele_2_ = lagainstElectronVLooseMVA_2;
        antimu_2_ = lagainstMuonTight3_2;
      }
      if(strategy_ == strategy::fall15) {
        iso_1_ = PF03IsolationVal(muon, 0.5, 0);
        mva_1_ = 0.0;
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        mva_2_ = tau->GetTauID("againstElectronMVA6raw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA6") ? tau->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA6") ? tau->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA6") ? tau->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA6") ? tau->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA6") ? tau->GetTauID("againstElectronVLooseMVA6") :0. ;
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
        lbyIsolationMVArun2DBnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        antiele_2_ = lagainstElectronVLooseMVA_2;
        antimu_2_ = lagainstMuonTight3_2;
       } 
       if (strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
        iso_1_ = PF04IsolationVal(muon, 0.5, 0);
        mva_1_ = 0.0;
        lPhotonPtSum_1 = 0.;
        iso_2_ = tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        l3Hits_2 = tau->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau->HasTauID("againstElectronLooseMVA6") ? tau->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_2 = tau->HasTauID("againstElectronMediumMVA6") ? tau->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_2 = tau->HasTauID("againstElectronTightMVA6") ? tau->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_2 = tau->HasTauID("againstElectronVTightMVA6") ? tau->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_2 = tau->HasTauID("againstElectronVLooseMVA6") ? tau->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lchargedRelIsoPtSum_2 = lchargedIsoPtSum_2/tau->pt();
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwLTraw") ? tau->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;
        lPhotonPtSum_2 = tau->HasTauID("photonPtSumOutsideSignalCone") ? tau->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lbyIsolationMVArun2DBnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_2 = tau->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_2 = tau->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        antiele_2_ = lagainstElectronVLooseMVA_2;
        antiele_tight_2_ = lagainstElectronTightMVA_2;
        antimu_2_ = lagainstMuonTight3_2;
        antimu_loose_2_ = lagainstMuonLoose3_2;
      }
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        iso_2_ = tau->GetTauID("byIsolationMVArun2017v2DBoldDMwLTraw2017");
        tau_id_olddm_vloose_2_  = tau->HasTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vloose_2_  = tau->HasTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_loose_2_   = tau->HasTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_loose_2_   = tau->HasTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_medium_2_  = tau->HasTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_medium_2_  = tau->HasTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_tight_2_   = tau->HasTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_tight_2_   = tau->HasTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vtight_2_  = tau->HasTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vtight_2_  = tau->HasTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vvtight_2_ = tau->HasTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau->GetTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vvtight_2_ = tau->HasTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau->GetTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
      }
      // add deepTau ID v2
      // Raw DNN scores
      deepTauVsJets_iso_2_      = tau->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_2_       = tau->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_2_        = tau->HasTauID("byDeepTau2017v2p1VSmuraw")       ? tau->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing workpoints
      deepTauVsJets_vvvloose_2_ = tau->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_2_  = tau->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_2_   = tau->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")  ? tau->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_    = tau->HasTauID("byVVLooseDeepTau2017v2p1VSmu")   ? tau->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_     = tau->HasTauID("byVLooseDeepTau2017v2p1VSmu")    ? tau->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_      = tau->HasTauID("byLooseDeepTau2017v2p1VSmu")     ? tau->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_     = tau->HasTauID("byMediumDeepTau2017v2p1VSmu")    ? tau->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_      = tau->HasTauID("byTightDeepTau2017v2p1VSmu")     ? tau->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_     = tau->HasTauID("byVTightDeepTau2017v2p1VSmu")    ? tau->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_    = tau->HasTauID("byVVTightDeepTau2017v2p1VSmu")   ? tau->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;
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
      if(strategy_ == strategy::spring15 || strategy_ == strategy::fall15) {
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        iso_2_ = PF03IsolationVal(muon, 0.5, 0);
        mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
      }
      if(strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
        if (strategy_ == strategy::legacy16)
            iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
        else
            iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        iso_2_ = PF04IsolationVal(muon, 0.5, 0);
        mva_1_ = elec->GetIdIso("generalPurposeMVASpring16");
      }
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho
        iso_2_ = PF04IsolationVal(muon, 0.5, 0);
        mva_1_ = elec->GetIdIso("generalPurposeMVASpring16");
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
        lagainstElectronLooseMVA_1 = tau1->HasTauID("againstElectronLooseMVA5") ? tau1->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA_1 = tau1->HasTauID("againstElectronMediumMVA5") ? tau1->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA_1 = tau1->HasTauID("againstElectronTightMVA5") ? tau1->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA_1= tau1->HasTauID("againstElectronVTightMVA5") ? tau1->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA_1 = tau1->HasTauID("againstElectronVLooseMVA5") ? tau1->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_1 = tau1->HasTauID("againstMuonLoose3") ? tau1->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_1 = tau1->HasTauID("againstMuonTight3") ? tau1->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_1 = tau1->HasTauID("photonPtSumOutsideSignalCone") ? tau1->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lbyLooseCombinedIsolation_1 = tau1->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_1 = tau1->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_1 = tau1->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        l3Hits_2 = tau2->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau2->HasTauID("againstElectronLooseMVA5") ? tau2->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA_2 = tau2->HasTauID("againstElectronMediumMVA5") ? tau2->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA_2 = tau2->HasTauID("againstElectronTightMVA5") ? tau2->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA_2 = tau2->HasTauID("againstElectronVTightMVA5") ? tau2->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA_2 = tau2->HasTauID("againstElectronVLooseMVA5") ? tau2->GetTauID("againstElectronVLooseMVA5") :0. ;
        lagainstMuonLoose3_2 = tau2->HasTauID("againstMuonLoose3") ? tau2->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau2->HasTauID("againstMuonTight3") ? tau2->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_2 = tau2->HasTauID("photonPtSumOutsideSignalCone") ? tau2->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        lbyLooseCombinedIsolation_2 = tau2->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau2->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau2->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        antiele_1_ = lagainstElectronVLooseMVA_1;
        antimu_1_ = lagainstMuonLoose3_1;
        antiele_2_ = lagainstElectronVLooseMVA_2;
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
      if(strategy_ == strategy::fall15) {
        iso_1_ = tau1->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        mva_1_ = tau1->GetTauID("againstElectronMVA6raw");
        iso_2_ = tau2->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        mva_2_ = tau2->GetTauID("againstElectronMVA6raw");
        l3Hits_1 = tau1->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_1 = tau1->HasTauID("againstElectronLooseMVA6") ? tau1->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_1 = tau1->HasTauID("againstElectronMediumMVA6") ? tau1->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_1 = tau1->HasTauID("againstElectronTightMVA6") ? tau1->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_1= tau1->HasTauID("againstElectronVTightMVA6") ? tau1->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_1 = tau1->HasTauID("againstElectronVLooseMVA6") ? tau1->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_1 = tau1->HasTauID("againstMuonLoose3") ? tau1->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_1 = tau1->HasTauID("againstMuonTight3") ? tau1->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_1 = tau1->HasTauID("photonPtSumOutsideSignalCone") ? tau1->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        l3Hits_2 = tau2->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau2->HasTauID("againstElectronLooseMVA6") ? tau2->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_2 = tau2->HasTauID("againstElectronMediumMVA6") ? tau2->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_2 = tau2->HasTauID("againstElectronTightMVA6") ? tau2->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_2 = tau2->HasTauID("againstElectronVTightMVA6") ? tau2->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_2 = tau2->HasTauID("againstElectronVLooseMVA6") ? tau2->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_2 = tau2->HasTauID("againstMuonLoose3") ? tau2->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau2->HasTauID("againstMuonTight3") ? tau2->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_2 = tau2->HasTauID("photonPtSumOutsideSignalCone") ? tau2->GetTauID("photonPtSumOutsideSignalCone") : 0.;

        antiele_1_ = lagainstElectronVLooseMVA_1;
        antimu_1_ = lagainstMuonLoose3_1;
        antiele_2_ = lagainstElectronVLooseMVA_2;
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

        lbyIsolationMVArun2DBnewDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau2->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau2->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau2->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;


        lbyIsolationMVArun2DBnewDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_1 = tau1->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_1 = tau1->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_1 = tau1->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;

      }
      if(strategy_ == strategy::mssmspring16 || strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        iso_1_ = tau1->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        iso_2_ = tau2->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw");
        l3Hits_1 = tau1->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_1 = tau1->HasTauID("againstElectronLooseMVA6") ? tau1->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_1 = tau1->HasTauID("againstElectronMediumMVA6") ? tau1->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_1 = tau1->HasTauID("againstElectronTightMVA6") ? tau1->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_1= tau1->HasTauID("againstElectronVTightMVA6") ? tau1->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_1 = tau1->HasTauID("againstElectronVLooseMVA6") ? tau1->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_1 = tau1->HasTauID("againstMuonLoose3") ? tau1->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_1 = tau1->HasTauID("againstMuonTight3") ? tau1->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_1 = tau1->HasTauID("photonPtSumOutsideSignalCone") ? tau1->GetTauID("photonPtSumOutsideSignalCone") : 0.;
        l3Hits_2 = tau2->HasTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0. ;
        lagainstElectronLooseMVA_2 = tau2->HasTauID("againstElectronLooseMVA6") ? tau2->GetTauID("againstElectronLooseMVA6") : 0.;
        lagainstElectronMediumMVA_2 = tau2->HasTauID("againstElectronMediumMVA6") ? tau2->GetTauID("againstElectronMediumMVA6") : 0.;
        lagainstElectronTightMVA_2 = tau2->HasTauID("againstElectronTightMVA6") ? tau2->GetTauID("againstElectronTightMVA6") : 0.;
        lagainstElectronVTightMVA_2 = tau2->HasTauID("againstElectronVTightMVA6") ? tau2->GetTauID("againstElectronVTightMVA6") : 0.;
        lagainstElectronVLooseMVA_2 = tau2->HasTauID("againstElectronVLooseMVA6") ? tau2->GetTauID("againstElectronVLooseMVA6") :0. ;
        lagainstMuonLoose3_2 = tau2->HasTauID("againstMuonLoose3") ? tau2->GetTauID("againstMuonLoose3") : 0.;
        lagainstMuonTight3_2 = tau2->HasTauID("againstMuonTight3") ? tau2->GetTauID("againstMuonTight3") : 0.;
        lPhotonPtSum_2 = tau2->HasTauID("photonPtSumOutsideSignalCone") ? tau2->GetTauID("photonPtSumOutsideSignalCone") : 0.;

        antiele_1_ = lagainstElectronVLooseMVA_1;
        antimu_1_ = lagainstMuonLoose3_1;
        antiele_2_ = lagainstElectronVLooseMVA_2;
        antimu_2_ = lagainstMuonLoose3_2;
        lchargedIsoPtSum_1 = tau1->HasTauID("chargedIsoPtSum") ? tau1->GetTauID("chargedIsoPtSum") : 0.;
        lchargedRelIsoPtSum_1 = lchargedIsoPtSum_1/tau1->pt();
        lneutralIsoPtSum_1 = tau1->HasTauID("neutralIsoPtSum") ? tau1->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_1 = tau1->HasTauID("puCorrPtSum") ? tau1->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_1 = tau1->HasTauID("decayModeFinding") ? tau1->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_1 = tau1->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau1->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_1 = tau1->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau1->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_1 = tau1->HasTauID("byIsolationMVA3newDMwLTraw") ? tau1->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_1 = tau1->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau1->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;

        lchargedIsoPtSum_2 = tau2->HasTauID("chargedIsoPtSum") ? tau2->GetTauID("chargedIsoPtSum") : 0.;
        lchargedRelIsoPtSum_2 = lchargedIsoPtSum_2/tau2->pt();
        lneutralIsoPtSum_2 = tau2->HasTauID("neutralIsoPtSum") ? tau2->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_2 = tau2->HasTauID("puCorrPtSum") ? tau2->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau2->HasTauID("decayModeFinding") ? tau2->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau2->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau2->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau2->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau2->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau2->HasTauID("byIsolationMVA3newDMwLTraw") ? tau2->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau2->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau2->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;

        lbyIsolationMVArun2DBnewDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_2 = tau2->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau2->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_2 = tau2->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_2 = tau2->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_2 = tau2->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau2->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_2 = tau2->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau2->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;


        lbyIsolationMVArun2DBnewDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1DBnewDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1DBnewDMwLTraw") : 0.;
        lbyIsolationMVArun2DBoldDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1DBoldDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1DBoldDMwLTraw") : 0.;
        lbyIsolationMVArun2PWnewDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1PWnewDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1PWnewDMwLTraw") : 0.;
        lbyIsolationMVArun2PWoldDMwLTraw_1 = tau1->HasTauID("byIsolationMVArun2v1PWoldDMwLTraw") ? tau1->GetTauID("byIsolationMVArun2v1PWoldDMwLTraw") : 0.;
        lbyLooseCombinedIsolation_1 = tau1->HasTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyMediumCombinedIsolation_1 = tau1->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") : 0.; 
        lbyTightCombinedIsolation_1 = tau1->HasTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") ? tau1->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits") : 0.;
        lbyVLooseIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyTightIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyTightIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBoldDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2DBnewDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1DBnewDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVLooseIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byVLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyLooseIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byLooseIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byLooseIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyMediumIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byMediumIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byMediumIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyTightIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyTightIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byTightIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVTightIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byVTightIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byVTightIsolationMVArun2v1PWnewDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWoldDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1PWoldDMwLT") : 0.;
        lbyVVTightIsolationMVArun2PWnewDMwLT_1 = tau1->HasTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") ? tau1->GetTauID("byVVTightIsolationMVArun2v1PWnewDMwLT") : 0.;


      }
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        iso_1_ = tau1->GetTauID("byIsolationMVArun2017v2DBoldDMwLTraw2017");
        tau_id_olddm_vloose_1_  = tau1->HasTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017")  ? tau1->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vloose_1_  = tau1->HasTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017")  ? tau1->GetTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_loose_1_   = tau1->HasTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017")   ? tau1->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_loose_1_   = tau1->HasTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017")   ? tau1->GetTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_medium_1_  = tau1->HasTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017")  ? tau1->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_medium_1_  = tau1->HasTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017")  ? tau1->GetTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_tight_1_   = tau1->HasTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017")   ? tau1->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_tight_1_   = tau1->HasTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017")   ? tau1->GetTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vtight_1_  = tau1->HasTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017")  ? tau1->GetTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vtight_1_  = tau1->HasTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017")  ? tau1->GetTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vvtight_1_ = tau1->HasTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau1->GetTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vvtight_1_ = tau1->HasTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau1->GetTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        iso_2_ = tau2->GetTauID("byIsolationMVArun2017v2DBoldDMwLTraw2017");
        tau_id_olddm_vloose_2_  = tau2->HasTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017")  ? tau2->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vloose_2_  = tau2->HasTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017")  ? tau2->GetTauID("byVLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_loose_2_   = tau2->HasTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017")   ? tau2->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_loose_2_   = tau2->HasTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017")   ? tau2->GetTauID("byLooseIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_medium_2_  = tau2->HasTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017")  ? tau2->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_medium_2_  = tau2->HasTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017")  ? tau2->GetTauID("byMediumIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_tight_2_   = tau2->HasTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017")   ? tau2->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_tight_2_   = tau2->HasTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017")   ? tau2->GetTauID("byTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vtight_2_  = tau2->HasTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017")  ? tau2->GetTauID("byVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vtight_2_  = tau2->HasTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017")  ? tau2->GetTauID("byVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
        tau_id_olddm_vvtight_2_ = tau2->HasTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") ? tau2->GetTauID("byVVTightIsolationMVArun2017v2DBoldDMwLT2017") : 0.;
        tau_id_newdm_vvtight_2_ = tau2->HasTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") ? tau2->GetTauID("byVVTightIsolationMVArun2017v2DBnewDMwLT2017") : 0.;
      }
      // add deepTau ID v2
      // Raw DNN scores
      deepTauVsJets_iso_1_      = tau1->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau1->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsJets_iso_2_      = tau2->HasTauID("byDeepTau2017v2p1VSjetraw")      ? tau2->GetTauID("byDeepTau2017v2p1VSjetraw"):      0.;
      deepTauVsEle_iso_1_       = tau1->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsEle_iso_2_       = tau2->HasTauID("byDeepTau2017v2p1VSeraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSeraw"):        0.;
      deepTauVsMu_iso_1_       = tau1->HasTauID("byDeepTau2017v2p1VSmuraw")        ? tau1->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;
      deepTauVsMu_iso_2_       = tau2->HasTauID("byDeepTau2017v2p1VSmuraw")        ? tau2->GetTauID("byDeepTau2017v2p1VSmuraw"):        0.;

      // Existing workpoints
      deepTauVsJets_vvvloose_1_ = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_1_  = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_1_   = tau1->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_1_    = tau1->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_1_   = tau1->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_1_    = tau1->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau1->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_1_   = tau1->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau1->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_1_  = tau1->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsJets_vvvloose_2_ = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"): 0.;
      deepTauVsJets_vvloose_2_  = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSjet"):  0.;
      deepTauVsJets_vloose_2_   = tau2->HasTauID("byVLooseDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_loose_2_    = tau2->HasTauID("byLooseDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byLooseDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_medium_2_   = tau2->HasTauID("byMediumDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byMediumDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_tight_2_    = tau2->HasTauID("byTightDeepTau2017v2p1VSjet")    ? tau2->GetTauID("byTightDeepTau2017v2p1VSjet"):    0.;
      deepTauVsJets_vtight_2_   = tau2->HasTauID("byVTightDeepTau2017v2p1VSjet")   ? tau2->GetTauID("byVTightDeepTau2017v2p1VSjet"):   0.;
      deepTauVsJets_vvtight_2_  = tau2->HasTauID("byVVTightDeepTau2017v2p1VSjet")  ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSjet"):  0.;

      deepTauVsEle_vvvloose_1_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_1_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_1_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_1_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_1_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_1_     = tau1->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_1_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_1_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsEle_vvvloose_2_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
      deepTauVsEle_vvloose_2_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSe"):    0.;
      deepTauVsEle_vloose_2_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_loose_2_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSe")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_medium_2_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSe")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_tight_2_     = tau2->HasTauID("byTightDeepTau2017v2p1VSe")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSe"):      0.;
      deepTauVsEle_vtight_2_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSe")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSe"):     0.;
      deepTauVsEle_vvtight_2_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSe")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSe"):    0.;

      deepTauVsMu_vvvloose_1_  = tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_1_   = tau1->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_1_    = tau1->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_1_     = tau1->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_1_    = tau1->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_1_     = tau1->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau1->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_1_    = tau1->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau1->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_1_   = tau1->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau1->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

      deepTauVsMu_vvvloose_2_  = tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSmu")   ? tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSmu"):   0.;
      deepTauVsMu_vvloose_2_   = tau2->HasTauID("byVVLooseDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVLooseDeepTau2017v2p1VSmu"):    0.;
      deepTauVsMu_vloose_2_    = tau2->HasTauID("byVLooseDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_loose_2_     = tau2->HasTauID("byLooseDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byLooseDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_medium_2_    = tau2->HasTauID("byMediumDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byMediumDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_tight_2_     = tau2->HasTauID("byTightDeepTau2017v2p1VSmu")      ? tau2->GetTauID("byTightDeepTau2017v2p1VSmu"):      0.;
      deepTauVsMu_vtight_2_    = tau2->HasTauID("byVTightDeepTau2017v2p1VSmu")     ? tau2->GetTauID("byVTightDeepTau2017v2p1VSmu"):     0.;
      deepTauVsMu_vvtight_2_   = tau2->HasTauID("byVVTightDeepTau2017v2p1VSmu")    ? tau2->GetTauID("byVVTightDeepTau2017v2p1VSmu"):    0.;

    }
    if (channel_ == channel::zee || channel_ == channel::tpzee) {
      Electron const* elec1 = dynamic_cast<Electron const*>(lep1);
      Electron const* elec2 = dynamic_cast<Electron const*>(lep2);
      if(strategy_ == strategy::spring15 || strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 || strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) {
        if (strategy_ == strategy::legacy16){
            iso_1_ = PF03EAIsolationVal(elec1, eventInfo->jet_rho());
            iso_2_ = PF03EAIsolationVal(elec2, eventInfo->jet_rho());
        }
        else{
            iso_1_ = PF03IsolationVal(elec1, 0.5, 0);
            iso_2_ = PF03IsolationVal(elec2, 0.5, 0);
        }
        if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
          iso_1_ = PF03EAIsolationVal(elec1, eventInfo->jet_rho()); //should be lepton_rho
          iso_2_ = PF03EAIsolationVal(elec2, eventInfo->jet_rho()); 
        }
        if(strategy_ == strategy::spring15 || strategy_ == strategy::fall15) {
          mva_1_ = ElectronHTTIdSpring15(elec1, false);
          mva_2_ = ElectronHTTIdSpring15(elec2, false);
        }
      }
      d0_1_ = elec1->dxy_vertex();
      dz_1_ = elec1->dz_vertex();
      d0_2_ = elec2->dxy_vertex();
      dz_2_ = elec2->dz_vertex();
    }
    if (channel_ == channel::zmm || channel_ == channel::tpzmm) {
      Muon const* muon1 = dynamic_cast<Muon const*>(lep1);
      Muon const* muon2 = dynamic_cast<Muon const*>(lep2);
      if(strategy_ == strategy::spring15 || strategy_ == strategy::fall15) {
        iso_1_ = PF03IsolationVal(muon1, 0.5, 0);
        iso_2_ = PF03IsolationVal(muon2, 0.5, 0);
        mva_1_ = MuonMedium(muon1);
        mva_2_ = MuonMedium(muon2);
      }
      if(strategy_ == strategy::mssmspring16 || strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
        iso_1_ = PF04IsolationVal(muon1, 0.5, 0);
        iso_2_ = PF04IsolationVal(muon2, 0.5, 0);
        mva_1_ = MuonMediumHIPsafe(muon1);
        mva_2_ = MuonMediumHIPsafe(muon2);
      }
      d0_1_ = muon1->dxy_vertex();
      dz_1_ = muon1->dz_vertex();
      d0_2_ = muon2->dxy_vertex();
      dz_2_ = muon2->dz_vertex();
    }
    

    if (channel_ == channel::tpzmm || channel_ == channel::tpzee){
      tag_trigger_match_1_ = event->Exists("tp_tag_leg1_match") ? event->Get<bool>("tp_tag_leg1_match") : 0;
      tag_trigger_match_2_ = event->Exists("tp_tag_leg2_match") ? event->Get<bool>("tp_tag_leg2_match") : 0;
      probe_trigger_match_1_ = event->Exists("tp_probe_leg1_match") ? event->Get<bool>("tp_probe_leg1_match") : 0;
      probe_trigger_match_2_ = event->Exists("tp_probe_leg2_match") ? event->Get<bool>("tp_probe_leg2_match") : 0;
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

    if(uncleaned_jets.size() > 0 && (channel_ == channel::mt || channel_ == channel::et) && jetfake_study_) {
      std::vector<Candidate *> subleading_lepton;
      subleading_lepton.push_back(ditau->GetCandidate("lepton2")); 
      std::vector<std::pair<ic::PFJet *, ic::Candidate *>> matches = MatchByDR(uncleaned_jets, subleading_lepton, 0.5, true, true);
      if(matches.size() > 0) {
        nearjpt_1_ = (matches.at(0)).first->pt();
      } else {
        nearjpt_1_ = -9999;
      }
    }
    

    if(qcd_study_ && (channel_ == channel::mt || channel_ == channel::et)){
      std::vector<Candidate *> leading_lepton;
      std::vector<Candidate *> subleading_lepton;
      leading_lepton.push_back(ditau->GetCandidate("lepton1"));
      subleading_lepton.push_back(ditau->GetCandidate("lepton2")); 
      std::vector<std::pair<ic::PFJet *, ic::Candidate *>> mu_matches = MatchByDR(uncleaned_jets, leading_lepton, 0.5, true, true);
      std::vector<std::pair<ic::PFJet *, ic::Candidate *>> tau_matches = MatchByDR(uncleaned_jets, subleading_lepton, 0.5, true, true);
      if(mu_matches.size() > 0) {
          jet_flav_1_ = (mu_matches.at(0)).first->parton_flavour();
      } else jet_flav_1_ = -9999;
      if(tau_matches.size() > 0) {
          jet_flav_2_ = (tau_matches.at(0)).first->parton_flavour();
      } else jet_flav_2_ = -9999;
      if(jets.size() > 0) {
          jet_flav_3_ = jets[0]->parton_flavour();
      } else jet_flav_3_ = -9999;
    }
    
    if (n_lowpt_jets_ >= 1) {
      jpt_1_ = lowpt_jets[0]->pt();
      jeta_1_ = lowpt_jets[0]->eta();
      jphi_1_ = lowpt_jets[0]->phi();
      jrawf_1_ = lowpt_jets[0]->uncorrected_energy()/lowpt_jets[0]->energy();//* (jets[0]->pt() / jets[0]->energy());
      jptunc_1_ = 0.0;
      jmva_1_ = lowpt_jets[0]->pu_id_mva_value();
      // vars for noise jet study
      jchm_1_   = lowpt_jets[0]->charged_multiplicity();
      jnm_1_    = lowpt_jets[0]->neutral_multiplicity();
      jpm_1_    = lowpt_jets[0]->photon_multiplicity();
      jchemf_1_ = lowpt_jets[0]->charged_em_energy_frac();
      jnemf_1_  = lowpt_jets[0]->neutral_em_energy_frac();
      jchhf_1_  = lowpt_jets[0]->charged_had_energy_frac();
      jnhf_1_   = lowpt_jets[0]->neutral_had_energy_frac();
      //
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
      dphi_jtt_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), ditau->vector()));
    } else {
      jpt_1_ = -9999;
      jeta_1_ = -9999;
      jphi_1_ = -9999;
      jrawf_1_ = -9999;
      jptunc_1_ = -9999;
      jmva_1_ = -9999;
      jlrm_1_ = -9999;
      jctm_1_ = -9999;
      dphi_jtt_ = -9999.;
      jchm_1_   = -9999.;
      jnm_1_    = -9999.;
      jpm_1_    = -9999.;
      jchemf_1_ = -9999.;
      jnemf_1_  = -9999.;
      jchhf_1_ = -9999.;
      jnhf_1_  = -9999.;
    }

    if (n_lowpt_jets_ >= 2) {
      jpt_2_ = lowpt_jets[1]->pt();
      jeta_2_ = lowpt_jets[1]->eta();
      jphi_2_ = lowpt_jets[1]->phi();
      jrawf_2_ = lowpt_jets[1]->uncorrected_energy()/lowpt_jets[1]->energy();// * (jets[1]->pt() / jets[1]->energy());
      jptunc_2_ = 0.0;
      jmva_2_ = lowpt_jets[1]->pu_id_mva_value();
      // vars for noise jet study
      jchm_2_   = lowpt_jets[1]->charged_multiplicity();
      jnm_2_    = lowpt_jets[1]->neutral_multiplicity();
      jpm_2_    = lowpt_jets[1]->photon_multiplicity();
      jchemf_2_ = lowpt_jets[1]->charged_em_energy_frac();
      jnemf_2_  = lowpt_jets[1]->neutral_em_energy_frac();
      jchhf_2_  = lowpt_jets[1]->charged_had_energy_frac();
      jnhf_2_   = lowpt_jets[1]->neutral_had_energy_frac();
      //
      jlrm_2_ = lowpt_jets[1]->linear_radial_moment();
      jctm_2_ = lowpt_jets[1]->charged_multiplicity_nopu();
      mjj_ = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
      jdeta_ = fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta());
      jdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), lowpt_jets[1]->vector());
      dijetpt_ =  (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).pt();
      // add centrality variable to test - could be useful for VBF vs ggH
      //zfeld = float(getattr(tree,"eta_h")) - (float(getattr(tree,"jeta_1"))+float(getattr(tree,"jeta_2")))/
      float zfeld = eta_h_ - (lowpt_jets[0]->eta()+lowpt_jets[1]->eta())/2;
      centrality_ = std::exp(-4*std::pow((zfeld/std::fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta())),2));

      if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
        if (event->Exists("D0")) D0_ = event->Get<float>("D0");
        else D0_ = -9999;
        if (event->Exists("DCP")) DCP_ = event->Get<float>("DCP");
        else DCP_ = -9999;

        if(lowpt_jets[0]->eta() > lowpt_jets[1]->eta()){
          sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), lowpt_jets[1]->vector());
        }
        else{
          sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[1]->vector(), lowpt_jets[0]->vector());
        }
        opp_sides_ = lowpt_jets[0]->eta()*lowpt_jets[1]->eta() < 0 ? 1 : 0;
        
        n_pjets_=0;
        if (jets.size()==1) n_pjets_=1;
        if(jets.size()>=2){
          double higgs_eta = 0;
          if (event->Exists("svfitHiggs")) {
            Candidate const& higgs = event->Get<Candidate>("svfitHiggs");
            higgs_eta = higgs.eta();
          }
          else higgs_eta = (lep1->vector()+lep2->vector()).Rapidity();
          // sort jets higher and lower than higgs eta_1
          std::vector<PFJet*> jets_high;
          std::vector<PFJet*> jets_low;
          for (unsigned i=0; i<jets.size(); ++i){
            if (jets[i]->eta() > higgs_eta) jets_high.push_back(jets[i]);    
            else jets_low.push_back(jets[i]);
          }
          if(jets_low.size()>0) n_pjets_++;
          if(jets_high.size()>0) n_pjets_++;
          Candidate pseudo_jet_a;
          Candidate pseudo_jet_b;
          for (auto j : jets_low) pseudo_jet_a.set_vector(pseudo_jet_a.vector()+j->vector());
          for (auto j : jets_high) pseudo_jet_b.set_vector(pseudo_jet_b.vector()+j->vector());
          spjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(pseudo_jet_a.vector(),pseudo_jet_b.vector());
          for (unsigned i=0; i<jets.size(); ++i){
            double dEta = std::fabs(higgs_eta - jets[i]->eta());  
            if(i==0 || dEta<min_hj_deta_) min_hj_deta_ = dEta;    
          }
          pjdeta_ = std::fabs(pseudo_jet_a.vector().Rapidity() - pseudo_jet_b.vector().Rapidity());
          pjahdeta_ = std::fabs(pseudo_jet_a.vector().Rapidity() - higgs_eta);
          pjbhdeta_ = std::fabs(pseudo_jet_a.vector().Rapidity() - higgs_eta);
          if((jets[0]->eta()>higgs_eta&&jets[1]->eta()>higgs_eta)||(jets[0]->eta()<higgs_eta&&jets[1]->eta()<higgs_eta)) prob_region_ = 1;
          else prob_region_ = 0;
        }
        else {
          spjdphi_ = -9999;
          min_hj_deta_ = -9999;
          pjdeta_ =-9999;
          pjahdeta_ = -9999;
          pjbhdeta_ = -9999;
          prob_region_ = -9999;
        }
      }
      
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
      dijetpt_ = -9999;
      centrality_ = -9999;
      jrawf_2_ = -9999;
      jptunc_2_ = -9999;
      jmva_2_ = -9999;
      jlrm_2_ = -9999;
      jctm_2_ = -9999;
      n_jetsingap_ = 9999;
      n_jetsingap20_ = 9999;
      jchm_2_   = -9999.;
      jnm_2_    = -9999.;
      jpm_2_    = -9999.;
      jchemf_2_ = -9999.;
      jnemf_2_  = -9999.;
      jchhf_2_  = -9999.;
      jnhf_2_   = -9999.;
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
    
    if((strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 ||  strategy_ == strategy::cpdecays16) && do_sm_scale_wts_ && !systematic_shift_){
      // weights needed for SM scale uncertainties are computer here rather than in HTTWeights - since these are parametarized as a function of the offline mjj and pt_tt which are computer in HTTCategories anyway
      wt_scale_et_0jet_  = 0.973 + 0.0003405 * pt_2_.var_double;
      wt_scale_et_boosted_ = 0.986 - 0.0000278 *pt_tt_.var_double;
      wt_scale_et_vbf_  = 0.971 + 0.0000327 * mjj_.var_double;
      wt_scale_mt_0jet_ = 0.929 + 0.0001702 * pt_2_.var_double;
      wt_scale_mt_boosted_ = 0.919 + 0.0010055 * pt_tt_.var_double;
      wt_scale_mt_vbf_ = 1.026 +0.000066 * mjj_.var_double;
      wt_scale_em_0jet_ = 0.942 - 0.0000170 * pt_2_.var_double;
      wt_scale_em_boosted_ = 0.936 + 0.0008871 * pt_tt_.var_double;
      wt_scale_em_vbf_ = 1.032 + 0.000102 * mjj_.var_double;
      wt_scale_tt_0jet_ = 0.814 + 0.0027094 * pt_1_.var_double;
      wt_scale_tt_boosted_ = 0.973 + 0.0008596 * pt_tt_.var_double;
      wt_scale_tt_vbf_ = 1.094 + 0.0000545 * mjj_.var_double;     
    }
    if((strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) && !systematic_shift_ && do_sm_scale_wts_){
      if (official_ggH_){
        wt_qcdscale_up_ = eventInfo->weight_defined("1005") ? eventInfo->weight("1005")*1.18 : 1.0;
        wt_qcdscale_down_ = eventInfo->weight_defined("1009") ? eventInfo->weight("1009")*0.84 : 1.0;
      } else {
        wt_qcdscale_up_ = eventInfo->weight_defined("1005") ? eventInfo->weight("1005")*2.27 : 1.0;
        wt_qcdscale_down_ = eventInfo->weight_defined("1009") ? eventInfo->weight("1009")*1.77 : 1.0;
        // MM 2.26750 1.76825
        // PS 2.26810 1.76739
        // SM 2.26824 1.76799
        // private numbers
        //wt_qcdscale_up_ = eventInfo->weight_defined("1005") ? eventInfo->weight("1005")*2.33 : 1.0; 
        //wt_qcdscale_down_ = eventInfo->weight_defined("1009") ? eventInfo->weight("1009")*1.72 : 1.0;
      }
      //SM = 1.18179 0.844494, MM = 2.32794 1.71996 PS = 2.32857 1.72129  -> for 2017!
    }
    if(do_z_weights_ && !systematic_shift_){
      wt_z_mjj_   = event->Exists("wt_z_mjj" ) ? event->Get<double>("wt_z_mjj"  ) : 1.0;  
      wt_z_mjj_up_   = event->Exists("wt_z_mjj_up" ) ? event->Get<double>("wt_z_mjj_up"  ) : 1.0;
      wt_z_mjj_down_ = event->Exists("wt_z_mjj_down") ? event->Get<double>("wt_z_mjj_down") : 1.0;   
    }


    if (channel_ == channel::tt && strategy_ == strategy::fall15){
      if (n_loose_bjets_ >= 1) {
        bpt_1_ = loose_bjets[0]->pt();
        brawf_1_ = loose_bjets[0]->uncorrected_energy()/loose_bjets[0]->energy();//* (jets[0]->pt() / jets[0]->energy());
        beta_1_ = loose_bjets[0]->eta();
        bphi_1_ = loose_bjets[0]->phi();
        bmva_1_ = loose_bjets[0]->pu_id_mva_value();
      
      } else {
        bpt_1_ = -9999;
        brawf_1_ = -9999;
        beta_1_ = -9999;
        bphi_1_ = -9999;
        bmva_1_ = -9999;
      }

      if (n_loose_bjets_ >= 2) {
        bpt_2_ = loose_bjets[1]->pt();
        brawf_2_ = loose_bjets[1]->uncorrected_energy()/loose_bjets[1]->energy();//* (jets[0]->pt() / jets[0]->energy());
        beta_2_ = loose_bjets[1]->eta();
        bphi_2_ = loose_bjets[1]->phi();
        bmva_2_ = loose_bjets[1]->pu_id_mva_value();
      
      } else {
        bpt_2_ = -9999;
        brawf_2_ = -9999;
        beta_2_ = -9999;
        bphi_2_ = -9999;
        bmva_2_ = -9999;
      }

    } else {
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
    
    if(do_faketaus_&&(channel_==channel::zmm||channel_==channel::em)){
      std::vector<Tau *> taus = event->GetPtrVec<Tau>("taus");
      std::sort(taus.begin(), taus.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
      tau_pt_1_=-9999;
      tau_pt_2_=-9999;
      tau_id_1_=0;
      tau_id_2_=0;
      tau_vloose_1_=0;
      tau_vloose_2_=0;
      tau_dm_1_=-9999;
      tau_dm_2_=-9999;

      if(channel_==channel::zmm){
        std::vector<const ic::Candidate*> muons;
        muons.push_back(lep1);
        muons.push_back(lep2);

        Met *newmet = event->GetPtr<Met>("pfMET");
        double WZ_ratio = 80.4/91.2;
        TLorentzVector muon1_vec;
        TLorentzVector muon2_vec;

        muon1_vec.SetXYZM(muons[0]->vector().Px(),muons[0]->vector().Py(),muons[0]->vector().Pz(),0);
        muon2_vec.SetXYZM(muons[1]->vector().Px(),muons[1]->vector().Py(),muons[1]->vector().Pz(),0);
        TVector3 Zboost = (muon1_vec+muon2_vec).BoostVector();
        TLorentzVector Z_vec = muon1_vec+muon2_vec;
        double scale = sqrt( pow(Z_vec.Px(),2) + pow(Z_vec.Py(),2) + pow(Z_vec.Pz(),2) + pow(Z_vec.M()*WZ_ratio,2));
        TVector3 BoostBack(Z_vec.Px()/scale, Z_vec.Py()/scale, Z_vec.Pz()/scale); 
        muon1_vec.Boost(-Zboost);
        muon2_vec.Boost(-Zboost);
        muon1_vec.SetXYZM(muon1_vec.Px()*WZ_ratio,muon1_vec.Py()*WZ_ratio,muon1_vec.Pz()*WZ_ratio ,0);
        muon2_vec.SetXYZM(muon2_vec.Px()*WZ_ratio,muon2_vec.Py()*WZ_ratio,muon2_vec.Pz()*WZ_ratio ,0);
        muon1_vec.Boost(BoostBack);
        muon2_vec.Boost(BoostBack);
        ic::Candidate* muon1 = new ic::Candidate();
        ic::Candidate* muon2 = new ic::Candidate();
        muon1->set_pt(muon1_vec.Pt());
        muon1->set_eta(muon1_vec.Eta());
        muon1->set_phi(muon1_vec.Phi());
        muon1->set_energy(muon1_vec.E());
        muon2->set_pt(muon2_vec.Pt());
        muon2->set_eta(muon2_vec.Eta());
        muon2->set_phi(muon2_vec.Phi());
        muon2->set_energy(muon2_vec.E());
        ic::Candidate *not_removed_mu;
        if (eventInfo->event() % 2 == 0){
          newmet->set_vector(muon1->vector()+newmet->vector());
          not_removed_mu = muon2;
        } else {
          newmet->set_vector(muon2->vector()+newmet->vector());
          not_removed_mu = muon1;
        }
        mt_1_nomu_ = MT(not_removed_mu, newmet); 
        mu_pt_ = not_removed_mu->pt();
      }

      if(taus.size()>0) { 
        deepTauVsEle_vvvloose_1_   = taus[0]->HasTauID("byVVVLooseDeepTau2017v2p1VSe")   ? taus[0]->GetTauID("byVVVLooseDeepTau2017v2p1VSe"):   0.;
        deepTauVsMu_vloose_1_      =    taus[0]->HasTauID("byVLooseDeepTau2017v2p1VSmu")   ? taus[0]->GetTauID("byVLooseDeepTau2017v2p1VSmu"):   0.;
        deepTauVsJets_vvvloose_1_  = taus[0]->HasTauID("byVVVLooseDeepTau2017v2p1VSjet")   ? taus[0]->GetTauID("byVVVLooseDeepTau2017v2p1VSjet"):   0.;
        tau_mva_decay_mode_1_ = taus[0]->HasTauID("MVADM2017v1") ? taus[0]->GetTauID("MVADM2017v1") : 0.0;
      }
      else {
        deepTauVsEle_vvvloose_1_ = 0.;
        deepTauVsMu_vloose_1_ = 0.;
        deepTauVsJets_vvvloose_1_ = 0.;
        tau_mva_decay_mode_1_ = -1;
      } 

      if(taus.size()>0){
        tau_pt_1_ = taus[0]->pt();
        tau_dm_1_ = taus[0]->decay_mode();
      }

    }

    // cp stuff
    wt_cp_sm_=1; wt_cp_ps_=1; wt_cp_mm_=1; 
    if(event->ExistsInTree("tauspinner")){
      EventInfo const* tauspinner = event->GetPtr<EventInfo>("tauspinner");
      wt_cp_sm_ = tauspinner->weight("wt_cp_0");
      wt_cp_ps_ = tauspinner->weight("wt_cp_0p5");
      wt_cp_mm_ = tauspinner->weight("wt_cp_0p25");
    }

    mvadm_rho_1_ = event->Exists("mvadm_rho_1") ? event->Get<float>("mvadm_rho_1") : 0.0;
    mvadm_a1_1_ = event->Exists("mvadm_a1_1") ? event->Get<float>("mvadm_a1_1") : 0.0;
    mvadm_other_1_ = event->Exists("mvadm_other_1") ? event->Get<float>("mvadm_other_1") : 0.0;
    mvadm_pi_1_ = event->Exists("mvadm_pi_1") ? event->Get<float>("mvadm_pi_1") : 0.0;
    mvadm_3pipi0_1_ = event->Exists("mvadm_3pipi0_1") ? event->Get<float>("mvadm_3pipi0_1") : 0.0;

    mvadm_rho_2_ = event->Exists("mvadm_rho_2") ? event->Get<float>("mvadm_rho_2") : 0.0;
    mvadm_a1_2_ = event->Exists("mvadm_a1_2") ? event->Get<float>("mvadm_a1_2") : 0.0;
    mvadm_other_2_ = event->Exists("mvadm_other_2") ? event->Get<float>("mvadm_other_2") : 0.0;
    mvadm_pi_2_ = event->Exists("mvadm_pi_2") ? event->Get<float>("mvadm_pi_2") : 0.0;
    mvadm_3pipi0_2_ = event->Exists("mvadm_3pipi0_2") ? event->Get<float>("mvadm_3pipi0_2") : 0.0;

    if (channel_ == channel::tt && event->ExistsInTree("pfCandidates")) {
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);

      lead_pt_1_ =  tau1->lead_pt();
      lead_pt_2_ =  tau2->lead_pt();

      mvadm_pi_1_ = tau1->HasTauID("MVADM2016v1DM0raw") ? tau1->GetTauID("MVADM2016v1DM0raw") : 0.0;
      mvadm_rho_1_ = tau1->HasTauID("MVADM2016v1DM1raw") ? tau1->GetTauID("MVADM2016v1DM1raw") : 0.0;
      mvadm_a1_1_ = tau1->HasTauID("MVADM2016v1DM2raw") ? tau1->GetTauID("MVADM2016v1DM2raw") : 0.0;
      mvadm_3pi_1_ = tau1->HasTauID("MVADM2016v1DM10raw") ? tau1->GetTauID("MVADM2016v1DM10raw") : 0.0;
      mvadm_3pipi0_1_ = tau1->HasTauID("MVADM2016v1DM11raw") ? tau1->GetTauID("MVADM2016v1DM11raw") : 0.0;

      mvadm_pi_2_ = tau2->HasTauID("MVADM2017v1DM0raw") ? tau2->GetTauID("MVADM2017v1DM0raw") : 0.0;
      mvadm_rho_2_ = tau2->HasTauID("MVADM2017v1DM1raw") ? tau2->GetTauID("MVADM2017v1DM1raw") : 0.0;
      mvadm_a1_2_ = tau2->HasTauID("MVADM2017v1DM2raw") ? tau2->GetTauID("MVADM2017v1DM2raw") : 0.0; 
      mvadm_3pi_2_ = tau2->HasTauID("MVADM2017v1DM10raw") ? tau2->GetTauID("MVADM2017v1DM10raw") : 0.0;
      mvadm_3pipi0_2_ = tau2->HasTauID("MVADM2017v1DM11raw") ? tau2->GetTauID("MVADM2017v1DM11raw") : 0.0;
     
      tau_mva_decay_mode_1_ = tau1->HasTauID("MVADM2017v1") ? tau1->GetTauID("MVADM2017v1") : 0.0;
      tau_mva_decay_mode_2_ = tau2->HasTauID("MVADM2017v1") ? tau2->GetTauID("MVADM2017v1") : 0.0;


      std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
      std::pair<ic::Candidate*, ic::Candidate*> rho1 = GetRho(tau1, pfcands);
      ic::Candidate *pi_tau1 = rho1.first;
      ic::Candidate *pi0_tau1 = rho1.second;


      std::pair<ic::Candidate*, ic::Candidate*> rho2 = GetRho(tau2, pfcands);
      ic::Candidate *pi_tau2 = rho2.first;
      ic::Candidate *pi0_tau2 = rho2.second;
 
      TLorentzVector lvec1;
      TLorentzVector lvec2;
      TLorentzVector lvec3;
      TLorentzVector lvec4;
      TLorentzVector pvtosv;
      std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices");
      std::vector<ic::Vertex*> & refit_vertex_vec = event->GetPtrVec<ic::Vertex>("refittedVertices");
      //std::vector<ic::Vertex*> & refit_vertex_bs_vec = event->GetPtrVec<ic::Vertex>("refittedVerticesBS");
      ic::Vertex* refit_vertex = vertex_vec[0];
      for(auto v : refit_vertex_vec) {
        //if(v->id() == tau1->id()+tau2->id()){ refit_vertex = v; std::cout << "found match!" << std::endl; std::cout << v->vx() << "    " << vertex_vec[0]->vx() << "    " << tau1->HasTauID("byVVVLooseDeepTau2017v2p1VSjet")*tau2->HasTauID("byVVVLooseDeepTau2017v2p1VSjet") << std::endl; }
        if(v->id() == tau1->id()+tau2->id()) refit_vertex = v; 
      }

      std::pair<TVector3,double> ipandsig_1 = IPAndSignificance(tau1, refit_vertex, pfcands);
      std::pair<TVector3,double> ipandsig_2 = IPAndSignificance(tau2, refit_vertex, pfcands);
      ip_mag_1_ = ipandsig_1.first.Mag();
      ip_sig_1_ = ipandsig_1.second;
      ip_mag_2_ = ipandsig_2.first.Mag();
      ip_sig_2_ = ipandsig_2.second;

      std::vector<ic::PFCandidate*> charged_cands_1 = GetTauHads(tau1, pfcands);
      std::vector<ic::PFCandidate*> charged_cands_2 = GetTauHads(tau2, pfcands);
      q_tot_1_=0., q_tot_2_=0.;

      for(auto c : charged_cands_1) q_tot_1_+=c->charge();
      for(auto c : charged_cands_2) q_tot_2_+=c->charge();

      auto primary_vtx = refit_vertex;

      if(tau_decay_mode_1_==0&&tau_decay_mode_2_==0) {
        cp_channel_=5;

        TLorentzVector pvtosv1(
                tau1->svx() - primary_vtx->vx(),
                tau1->svy() - primary_vtx->vy(),
                tau1->svz() - primary_vtx->vz(),
                0.);
        TLorentzVector pvtosv2(
                tau2->svx() - primary_vtx->vx(),
                tau2->svy() - primary_vtx->vy(),
                tau2->svz() - primary_vtx->vz(),
                0.);

        lvec3 = ConvertToLorentz(tau1->vector()); 
        lvec4 = ConvertToLorentz(tau2->vector()); 

        TVector3 ip1 = (pvtosv1.Vect() - pvtosv1.Vect().Dot(lvec3.Vect().Unit())*lvec3.Vect().Unit()).Unit();
        lvec1 = TLorentzVector(ip1, 0.);
        TVector3 ip2 = (pvtosv2.Vect() - pvtosv2.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
        lvec2 = TLorentzVector(ip2, 0.);

        aco_angle_6_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
      }

      if((tau_decay_mode_1_==1&&tau_decay_mode_2_==0) || (tau_decay_mode_1_==0&&tau_decay_mode_2_==1)) {
               
        cp_channel_=4;
        ic::Candidate *pi;
        ic::Candidate *pi0;
        ic::Tau const *tau;
        ic::Tau const *tau_1;

        if(tau_decay_mode_1_==1&&tau_decay_mode_2_==0) {
          pi = pi_tau1;
          pi0 = pi0_tau1;
          tau = tau2;
          tau_1 = tau1;
        } else {
          pi = pi_tau2;
          pi0 = pi0_tau2;
          tau = tau1;
          tau_1 = tau2;
        }
        cp_sign_ = YRho(std::vector<Candidate*>({pi, pi0}),TVector3());
        TLorentzVector pvtosv1(
                tau->svx() - primary_vtx->vx(),
                tau->svy() - primary_vtx->vy(),
                tau->svz() - primary_vtx->vz(),
                0.);
        lvec1 = ConvertToLorentz(pi0->vector()); //pi zero from rho
        lvec3 = ConvertToLorentz(pi->vector()); //pi charge from rho
        lvec4 = ConvertToLorentz(tau->vector()); //pi charge from tau

        TVector3 ip = (pvtosv1.Vect() - pvtosv1.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
        lvec2 = TLorentzVector(ip, 0.);

        TLorentzVector lvec3_1 = ConvertToLorentz(pi->vector());

        TLorentzVector pvtosv2(
                tau_1->svx() - primary_vtx->vx(),
                tau_1->svy() - primary_vtx->vy(),
                tau_1->svz() - primary_vtx->vz(),
                0.);

        TVector3 ip1 = (pvtosv2.Vect() - pvtosv2.Vect().Dot(lvec3_1.Vect().Unit())*lvec3_1.Vect().Unit()).Unit();
        TLorentzVector lvec1_1 = TLorentzVector(ip1, 0.);


        aco_angle_5_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
        if (cp_sign_<0) {
          if (aco_angle_5_<M_PI)  aco_angle_5_ = aco_angle_5_+M_PI;
          else                    aco_angle_5_ = aco_angle_5_-M_PI;
        }
        aco_angle_6_ = IPAcoAngle(lvec1_1, lvec2, lvec3_1, lvec4,false);
        if (cp_sign_<0) {
          if (aco_angle_6_<M_PI)  aco_angle_6_ = aco_angle_6_+M_PI;
          else                    aco_angle_6_ = aco_angle_6_-M_PI;
        }
      }

      if(tau_decay_mode_1_==1&&tau_decay_mode_2_==1){
        rho_dphi_  = ROOT::Math::VectorUtil::DeltaPhi(pi0_tau1->vector(),pi_tau1->vector());
        rho_deta_ = pi0_tau1->eta()-pi_tau1->eta();
        cp_channel_=1;
        lvec1 = ConvertToLorentz(pi0_tau1->vector());
        lvec2 = ConvertToLorentz(pi0_tau2->vector());
        lvec3 = ConvertToLorentz(pi_tau1->vector());
        lvec4 = ConvertToLorentz(pi_tau2->vector());
        cp_sign_ = YRho(std::vector<Candidate*>({pi0_tau1, pi_tau1}),TVector3())
            *YRho(std::vector<Candidate*>({pi0_tau2, pi_tau2}),TVector3());
        y_1_1_ = YRho(std::vector<Candidate*>({pi0_tau1, pi_tau1}),TVector3());
        y_1_2_ = YRho(std::vector<Candidate*>({pi0_tau2, pi_tau2}),TVector3());

        aco_angle_1_ = AcoplanarityAngle(std::vector<Candidate*> ({pi_tau1,pi0_tau1}), std::vector<Candidate*> ({pi_tau2,pi0_tau2}));
        aco_sign_1_ = AcoplanarityAngleWithSign(std::vector<Candidate*> ({rho1.first,rho1.second}), std::vector<Candidate*> ({rho2.first,rho2.second})).second;

        if (cp_sign_<0) {
          if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
          else                   aco_angle_1_ -= M_PI;
        }  
      }

      else if((tau_decay_mode_1_==0&&tau_decay_mode_2_>=10) || (tau_decay_mode_1_>=10&&tau_decay_mode_2_==0)){

        cp_channel_=2;

        ic::Tau const *tau_1;
        std::vector<ic::PFCandidate*> a1_daughters;
        if(tau_decay_mode_1_>=10) {
          a1_daughters  = GetA1(tau1, pfcands).first;
          tau_1 = tau2;
        } else {
          a1_daughters  = GetA1(tau2, pfcands).first;
          tau_1 = tau1;
        }

        TLorentzVector pvtosv1(
                tau_1->svx() - primary_vtx->vx(),
                tau_1->svy() - primary_vtx->vy(),
                tau_1->svz() - primary_vtx->vz(),
                0.);
        lvec3 = ConvertToLorentz(tau_1->vector()); //pi charge from tau

        TVector3 ip = (pvtosv1.Vect() - pvtosv1.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
        lvec1 = TLorentzVector(ip, 0.);

        if (a1_daughters.size()>2){

            lvec2 = ConvertToLorentz(a1_daughters[0]->vector()); //pi zero from rho
            lvec4 = ConvertToLorentz(a1_daughters[1]->vector()); //pi charge from rho
            aco_angle_5_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);

            cp_sign_ = YRho(std::vector<Candidate*>({a1_daughters[0], a1_daughters[1]}),TVector3());

            if (cp_sign_<0) {
              if (aco_angle_5_<M_PI)  aco_angle_5_ = aco_angle_5_+M_PI;
              else                    aco_angle_5_ = aco_angle_5_-M_PI;
            }

          }
      }


      else if((tau_decay_mode_1_==1&&tau_decay_mode_2_>=10) || (tau_decay_mode_1_>=10&&tau_decay_mode_2_==1)){
      
        cp_channel_=2;

        std::vector<ic::PFCandidate*> a1_daughters;
        std::pair<ic::Candidate*, ic::Candidate*> rho_daughters;
        if(tau_decay_mode_1_>9){
          a1_daughters  = GetA1(tau1, pfcands).first;
          rho_daughters = GetRho(tau2, pfcands);
        }
        if(tau_decay_mode_2_>9){
          a1_daughters  = GetA1(tau2, pfcands).first;
          rho_daughters = GetRho(tau1, pfcands);
        }

        if (a1_daughters.size()>2){
            mass0_ = (a1_daughters[0]->vector() + a1_daughters[1]->vector() + a1_daughters[2]->vector()).M();
            mass1_ = (a1_daughters[0]->vector() + a1_daughters[1]->vector()).M();
            mass2_ = (a1_daughters[0]->vector() + a1_daughters[2]->vector()).M();

            aco_angle_1_ = AcoplanarityAngle(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({a1_daughters[0],a1_daughters[1]}));
            //aco_angle_3_ = AcoplanarityAngle(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({a1_daughters[0],a1_daughters[2]})); 

            Candidate* rho_1  = new Candidate();
            Candidate* rho_2  = new Candidate();
            rho_1->set_vector(a1_daughters[0]->vector()+a1_daughters[1]->vector());
            rho_2->set_vector(a1_daughters[0]->vector()+a1_daughters[2]->vector());
            aco_angle_2_ = AcoplanarityAngle(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({rho_1,a1_daughters[2]}));
            //aco_angle_4_ = AcoplanarityAngle(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({rho_2,a1_daughters[1]}));

            y_1_1_ = YRho(std::vector<Candidate*>({rho_daughters.first, rho_daughters.second}),TVector3());
            y_1_2_ = YRho(std::vector<Candidate*>({a1_daughters[0], a1_daughters[1]}),TVector3());
            y_3_2_ = YRho(std::vector<Candidate*>({a1_daughters[0], a1_daughters[2]}),TVector3());
            y_2_2_ = YA1(std::vector<Candidate*>({rho_1, a1_daughters[2]}),TVector3());
            y_4_2_ = YA1(std::vector<Candidate*>({rho_2, a1_daughters[1]}),TVector3());

            aco_sign_1_ = AcoplanarityAngleWithSign(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({a1_daughters[0],a1_daughters[1]})).second;
            aco_sign_3_ = AcoplanarityAngleWithSign(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({a1_daughters[0],a1_daughters[2]})).second;
            aco_sign_2_ = AcoplanarityAngleWithSign(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({rho_1,a1_daughters[2]})).second;
            aco_sign_4_ = AcoplanarityAngleWithSign(std::vector<Candidate*> ({rho_daughters.first,rho_daughters.second}), std::vector<Candidate*> ({rho_2,a1_daughters[1]})).second;

            cp_sign_1_ = y_1_1_*y_1_2_;
            cp_sign_2_ = y_1_1_*y_2_2_;
            cp_sign_3_ = y_1_1_*y_3_2_;
            cp_sign_4_ = y_1_1_*y_4_2_;
        }   

        if (cp_sign_1_<0) {
          if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
          else                   aco_angle_1_ -= M_PI;
        }
        if (cp_sign_2_<0) {
          if (aco_angle_2_<M_PI) aco_angle_2_ += M_PI;
          else                   aco_angle_2_ -= M_PI;
        }
        //if (cp_sign_3_<0) {
        //  if (aco_angle_3_<M_PI) aco_angle_3_ += M_PI;
        //  else                   aco_angle_3_ -= M_PI;
        //}
        //if (cp_sign_4_<0) {
        //  if (aco_angle_4_<M_PI) aco_angle_4_ += M_PI;
        //  else                   aco_angle_4_ -= M_PI;
        //}

      } else if(tau_decay_mode_1_>=10&&tau_decay_mode_2_>=10){
        cp_channel_=3;
        std::vector<ic::PFCandidate*> a1_daughters_1 = GetA1(tau1, pfcands).first;
        std::vector<ic::PFCandidate*> a1_daughters_2 = GetA1(tau2, pfcands).first;

        if(a1_daughters_1.size()>2 && a1_daughters_2.size()>2) {
          Candidate* rho_1  = new Candidate();
          Candidate* rho_2  = new Candidate();
          rho_1->set_vector(a1_daughters_1[0]->vector()+a1_daughters_1[1]->vector());
          rho_2->set_vector(a1_daughters_2[0]->vector()+a1_daughters_2[1]->vector());

          aco_angle_1_ = AcoplanarityAngle(std::vector<Candidate*> ({a1_daughters_1[0],a1_daughters_1[1]}), std::vector<Candidate*> ({a1_daughters_2[0],a1_daughters_2[1]}));

          aco_angle_2_ = AcoplanarityAngle(std::vector<Candidate*> ({a1_daughters_1[0],a1_daughters_1[1]}), std::vector<Candidate*> ({rho_2,a1_daughters_2[2]}));

          aco_angle_3_ = AcoplanarityAngle(std::vector<Candidate*> ({a1_daughters_2[0],a1_daughters_2[1]}), std::vector<Candidate*> ({rho_1,a1_daughters_1[2]}));

          aco_angle_4_ = AcoplanarityAngle(std::vector<Candidate*> ({rho_1,a1_daughters_1[2]}), std::vector<Candidate*> ({rho_2,a1_daughters_2[2]})); 

          double yrho_1_ = YRho(std::vector<Candidate*>({a1_daughters_1[0],a1_daughters_1[1]}),TVector3());
          double yrho_2_ = YRho(std::vector<Candidate*>({a1_daughters_2[0],a1_daughters_2[1]}),TVector3());

          double ya1_1_ = YA1(std::vector<Candidate*>({rho_1, a1_daughters_1[2]}),TVector3());
          double ya1_2_ = YA1(std::vector<Candidate*>({rho_2, a1_daughters_2[2]}),TVector3());

          cp_sign_1_=yrho_1_*yrho_2_;
          cp_sign_2_=yrho_1_*ya1_2_;
          cp_sign_3_=yrho_2_*ya1_1_;
          cp_sign_4_=ya1_1_*ya1_2_;

          if (cp_sign_1_<0) {
            if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
            else                   aco_angle_1_ -= M_PI;
          }
          if (cp_sign_2_<0) {
            if (aco_angle_2_<M_PI) aco_angle_2_ += M_PI;
            else                   aco_angle_2_ -= M_PI;
          }
          if (cp_sign_3_<0) {
            if (aco_angle_3_<M_PI) aco_angle_3_ += M_PI;
            else                   aco_angle_3_ -= M_PI;
          }
          if (cp_sign_4_<0) {
            if (aco_angle_4_<M_PI) aco_angle_4_ += M_PI;
            else                   aco_angle_4_ -= M_PI;
          }
        }
      }
      else {
        cp_channel_ =-1;
        cp_sign_ = -9999;
      }

      if(cp_channel_!=-1){
        aco_angle_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
      }
      if(cp_channel_==3 || cp_channel_==2) {
        if (cp_sign_<0) {
          if (aco_angle_<M_PI) aco_angle_mod_ = aco_angle_+M_PI;
          else                  aco_angle_mod_ = aco_angle_-M_PI;
      } else {
        aco_angle_mod_ = aco_angle_;
        }
        //aco_angle_1_ = aco_angle_mod_;
      }

    }
    else if (channel_ == channel::mt && event->ExistsInTree("pfCandidates")) {
      Muon const* muon1 = dynamic_cast<Muon const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);

      lead_pt_2_ =  tau2->lead_pt();

      mvadm_pi_2_ = tau2->HasTauID("MVADM2017v1DM0raw") ? tau2->GetTauID("MVADM2017v1DM0raw") : 0.0;
      mvadm_rho_2_ = tau2->HasTauID("MVADM2017v1DM1raw") ? tau2->GetTauID("MVADM2017v1DM1raw") : 0.0;
      mvadm_a1_2_ = tau2->HasTauID("MVADM2017v1DM2raw") ? tau2->GetTauID("MVADM2017v1DM2raw") : 0.0;
      mvadm_3pi_2_ = tau2->HasTauID("MVADM2017v1DM10raw") ? tau2->GetTauID("MVADM2017v1DM10raw") : 0.0;
      mvadm_3pipi0_2_ = tau2->HasTauID("MVADM2017v1DM11raw") ? tau2->GetTauID("MVADM2017v1DM11raw") : 0.0;
      tau_mva_decay_mode_2_ = tau2->HasTauID("MVADM2017v1") ? tau2->GetTauID("MVADM2017v1") : 0.0;


      std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
      std::pair<ic::Candidate*, ic::Candidate*> rho = GetRho(tau2, pfcands);
      ic::Candidate *pi_tau2 = rho.first;
      ic::Candidate *pi0_tau2 = rho.second;
      
      TLorentzVector lvec1;
      TLorentzVector lvec2;
      TLorentzVector lvec3;
      TLorentzVector lvec4;
      TLorentzVector pvtosv1;


      std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices");
      std::vector<ic::Vertex*> & refit_vertex_vec = event->GetPtrVec<ic::Vertex>("refittedVertices");
      //std::vector<ic::Vertex*> & refit_vertex_bs_vec = event->GetPtrVec<ic::Vertex>("refittedVerticesBS");
      ic::Vertex* refit_vertex = new ic::Vertex();
      if(vertex_vec.size()>0) refit_vertex = vertex_vec[0];
      for(auto v : refit_vertex_vec) {
        if(v->id() == muon1->id()+tau2->id())refit_vertex = v;
      }

      auto primary_vtx = refit_vertex;

      pvtosv1.SetXYZT(
              muon1->vx() - refit_vertex->vx(),
              muon1->vy() - refit_vertex->vy(),
              muon1->vz() - refit_vertex->vz(),
              0.);

      TVector3 ip1 = (pvtosv1.Vect() - pvtosv1.Vect().Dot(lvec3.Vect().Unit())*lvec3.Vect().Unit()).Unit();
      lvec1 = TLorentzVector(ip1, 0.);
      lvec3 = ConvertToLorentz(muon1->vector());


      if(tau_decay_mode_2_==0) {
        cp_channel_=5;

        TLorentzVector pvtosv2(
                tau2->svx() - primary_vtx->vx(),
                tau2->svy() - primary_vtx->vy(),
                tau2->svz() - primary_vtx->vz(),
                0.);
        lvec4 = ConvertToLorentz(tau2->vector()); 

        TVector3 ip2 = (pvtosv2.Vect() - pvtosv2.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
        lvec2 = TLorentzVector(ip2, 0.);

        aco_angle_6_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
      }

      if(tau_decay_mode_2_==1) {
               
        cp_channel_=4;
        ic::Candidate *pi;
        ic::Candidate *pi0;

        pi = pi_tau2;
        pi0 = pi0_tau2;

        lvec2 = ConvertToLorentz(pi0->vector()); //pi zero from rho
        lvec4 = ConvertToLorentz(pi->vector()); //pi charge from rho

        cp_sign_ = YRho(std::vector<Candidate*>({pi, pi0}),TVector3());
        TLorentzVector pvtosv2(
                tau2->svx() - primary_vtx->vx(),
                tau2->svy() - primary_vtx->vy(),
                tau2->svz() - primary_vtx->vz(),
                0.);
        
        TLorentzVector lvec4_2 = ConvertToLorentz(tau2->vector());

        TVector3 ip2 = (pvtosv2.Vect() - pvtosv2.Vect().Dot(lvec4_2.Vect().Unit())*lvec4_2.Vect().Unit()).Unit();
        TLorentzVector lvec2_2 = TLorentzVector(ip2, 0.);

        aco_angle_5_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
        if (cp_sign_<0) {
          if (aco_angle_5_<M_PI)  aco_angle_5_ = aco_angle_5_+M_PI;
          else                    aco_angle_5_ = aco_angle_5_-M_PI;
        }
        aco_angle_6_ = IPAcoAngle(lvec1, lvec2_2, lvec3, lvec4_2,false);
        if (cp_sign_<0) {
          if (aco_angle_6_<M_PI)  aco_angle_6_ = aco_angle_6_+M_PI;
          else                    aco_angle_6_ = aco_angle_6_-M_PI;
        }
      }

      else if(tau_decay_mode_2_>=10){

        cp_channel_=2;

        std::vector<ic::PFCandidate*> a1_daughters;
        std::pair<ic::Candidate*, ic::Candidate*> rho_daughters;
        a1_daughters  = GetA1(tau2, pfcands).first;

        if (a1_daughters.size()>2){

            lvec2 = ConvertToLorentz(a1_daughters[0]->vector()); //pi zero from rho
            lvec4 = ConvertToLorentz(a1_daughters[1]->vector()); //pi charge from rho
            aco_angle_5_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);

            cp_sign_ = YRho(std::vector<Candidate*>({a1_daughters[0], a1_daughters[1]}),TVector3());

            if (cp_sign_<0) {
              if (aco_angle_5_<M_PI)  aco_angle_5_ = aco_angle_5_+M_PI;
              else                    aco_angle_5_ = aco_angle_5_-M_PI;
            }

          }
      }

      std::pair<TVector3,double> ipandsig_2 = IPAndSignificance(tau2, refit_vertex,pfcands);
      ip_mag_2_ = ipandsig_2.first.Mag();
      ip_sig_2_ = ipandsig_2.second;
      
    }
    else if (channel_ == channel::et && event->ExistsInTree("pfCandidates")) {
      Electron const* ele1 = dynamic_cast<Electron const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);

      lead_pt_2_ =  tau2->lead_pt();
    
      mvadm_pi_2_ = tau2->HasTauID("MVADM2017v1DM0raw") ? tau2->GetTauID("MVADM2017v1DM0raw") : 0.0;
      mvadm_rho_2_ = tau2->HasTauID("MVADM2017v1DM1raw") ? tau2->GetTauID("MVADM2017v1DM1raw") : 0.0;
      mvadm_a1_2_ = tau2->HasTauID("MVADM2017v1DM2raw") ? tau2->GetTauID("MVADM2017v1DM2raw") : 0.0;
      mvadm_3pi_2_ = tau2->HasTauID("MVADM2017v1DM10raw") ? tau2->GetTauID("MVADM2017v1DM10raw") : 0.0;
      mvadm_3pipi0_2_ = tau2->HasTauID("MVADM2017v1DM11raw") ? tau2->GetTauID("MVADM2017v1DM11raw") : 0.0;

      tau_mva_decay_mode_2_ = tau2->HasTauID("MVADM2017v1") ? tau2->GetTauID("MVADM2017v1") : 0.0;


      mva_dm_2_=-1;
      if(event->Exists("mvadm_max_index_2")) {
        int max_index = event->Get<int>("mvadm_max_index_2");
        if(tau_decay_mode_2_<2){
          if(max_index==1) mva_dm_2_= 1;
          if(max_index==2) mva_dm_2_= 0;
          if(max_index==3) mva_dm_2_= 2;
        } else {
          if(max_index==1) mva_dm_2_= 10;
          if(max_index==2) mva_dm_2_= 11;
        }
      }

 
      std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
      std::pair<ic::Candidate*, ic::Candidate*> rho = GetRho(tau2, pfcands);
      ic::Candidate *pi_tau2 = rho.first;
      ic::Candidate *pi0_tau2 = rho.second;
 
      TLorentzVector lvec1;
      TLorentzVector lvec2;
      TLorentzVector lvec3;
      TLorentzVector lvec4;
      TLorentzVector pvtosv;

      std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices");
      std::vector<ic::Vertex*> & refit_vertex_vec = event->GetPtrVec<ic::Vertex>("refittedVertices");
      //std::vector<ic::Vertex*> & refit_vertex_bs_vec = event->GetPtrVec<ic::Vertex>("refittedVerticesBS");
      ic::Vertex* refit_vertex = vertex_vec[0];
      for(auto v : refit_vertex_vec) {
        if(v->id() == ele1->id()+tau2->id()) refit_vertex = v; 
      }

      std::pair<TVector3,double> ipandsig_2 = IPAndSignificance(tau2, refit_vertex, pfcands);
      ip_mag_2_ = ipandsig_2.first.Mag();
      ip_sig_2_ = ipandsig_2.second;

      if(tau_decay_mode_2_==1){


        cp_channel_=2;
        lvec1 = ConvertToLorentz(pi0_tau2->vector());
        pvtosv.SetXYZT(
                ele1->vx() - vertex_vec[0]->vx(),
                ele1->vy() - vertex_vec[0]->vy(),
                ele1->vz() - vertex_vec[0]->vz(),
                0.);
        lvec3 = ConvertToLorentz(pi_tau2->vector());
        lvec4 = ConvertToLorentz(ele1->vector());


        cp_sign_ = YRho(std::vector<Candidate*>({pi_tau2, pi0_tau2}),TVector3());
      }
      else {
        cp_channel_ =-1;
        cp_sign_ = -9999;
      }

      if(cp_channel_!=-1){
        if (cp_channel_ == 2)
          aco_angle_ = IPAcoAngle(lvec1, pvtosv, lvec3, lvec4,false);
        else
          aco_angle_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
      }
      if(cp_channel_==3 || cp_channel_==2) {
        if (cp_sign_<0) {
          if (aco_angle_<M_PI) aco_angle_mod_ = aco_angle_+M_PI;
          else                  aco_angle_mod_ = aco_angle_-M_PI;
      } else {
        aco_angle_mod_ = aco_angle_;
        }
      }

    }
    else { 
      aco_angle_     = -9999;
      aco_angle_mod_ = -9999;
    }
      
    // signal background event classification
    IC_BDT_max_score_ = event->Exists("IC_BDT_max_score") ? event->Get<float>("IC_BDT_max_score") : -999.0;
    IC_BDT_max_index_ = event->Exists("IC_BDT_max_index") ? event->Get<float>("IC_BDT_max_index") : -999.0;
    
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
