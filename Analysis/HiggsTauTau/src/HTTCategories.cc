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
      do_pdf_wts_ = false;
      do_mssm_higgspt_ = false;
      do_sm_scale_wts_ = false;
      do_sm_ps_wts_ = false;
      do_jes_vars_ = false;
      do_z_weights_ = false;
      do_faketaus_ = false;
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
      outtree_->Branch("npu",               &n_pu_, "n_pu/F");
      outtree_->Branch("rho",               &rho_, "rho/F");
      outtree_->Branch("puweight",          &pu_weight_, "pu_weight/F");
      outtree_->Branch("wt",                &wt_.var_double);
      outtree_->Branch("wt_dysoup",         &wt_dysoup_);
      outtree_->Branch("wt_btag",           &wt_btag_);
      outtree_->Branch("wt_tau_id_loose", &wt_tau_id_loose_);
      outtree_->Branch("wt_tau_id_medium", &wt_tau_id_medium_);
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
      // adding tempoary gen stuff
      outtree_->Branch("partons"     , &partons_);
      outtree_->Branch("parton_pt"     , &parton_pt_);
      outtree_->Branch("parton_pt_2"     , &parton_pt_2_);
      outtree_->Branch("parton_pt_3"     , &parton_pt_3_);
      outtree_->Branch("parton_mjj",    &parton_mjj_);
      outtree_->Branch("npNLO", &npNLO_);
      //end of temp gen stuff
      if(do_sm_ps_wts_ && !systematic_shift_){
        outtree_->Branch("wt_ps_up", & wt_ps_up_);
        outtree_->Branch("wt_ps_down", & wt_ps_down_);
        outtree_->Branch("wt_ue_up", & wt_ue_up_);
        outtree_->Branch("wt_ue_down", & wt_ue_down_);
      }
      
      if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) outtree_->Branch("wt_lfake_rate"    ,    &wt_lfake_rate_); 
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
      if(channel_==channel::tt){
        outtree_->Branch("wt_tau_id_vtight", &wt_tau_id_vtight_); 
        outtree_->Branch("wt_tau_id_tight", &wt_tau_id_tight_);
        outtree_->Branch("wt_tau1_id_loose", &wt_tau1_id_loose_);
        outtree_->Branch("wt_tau1_id_medium", &wt_tau1_id_medium_);
        outtree_->Branch("wt_tau1_id_tight", &wt_tau1_id_tight_);
        outtree_->Branch("wt_tau1_id_vtight", &wt_tau1_id_vtight_);
        outtree_->Branch("wt_tau2_id_loose", &wt_tau2_id_loose_);
        outtree_->Branch("wt_tau2_id_medium", &wt_tau2_id_medium_);
        outtree_->Branch("wt_tau2_id_tight", &wt_tau2_id_tight_);
        outtree_->Branch("wt_tau2_id_vtight", &wt_tau2_id_vtight_);
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
        } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) {
          outtree_->Branch("wt_ff_1"  , &wt_ff_1_);    
          if(channel_ == channel::tt){
            outtree_->Branch("wt_ff_2"  , &wt_ff_2_); 
          }
            if(do_ff_systematics_){
                outtree_->Branch("wt_ff_qcd_syst_up_1"            , &wt_ff_qcd_syst_up_1_             );
                outtree_->Branch("wt_ff_qcd_syst_down_1"          , &wt_ff_qcd_syst_down_1_           );
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
                outtree_->Branch("wt_ff_w_syst_up_1"              , &wt_ff_w_syst_up_1_               );
                outtree_->Branch("wt_ff_w_syst_down_1"            , &wt_ff_w_syst_down_1_             );
                outtree_->Branch("wt_ff_tt_syst_up_1"             , &wt_ff_tt_syst_up_1_              );
                outtree_->Branch("wt_ff_tt_syst_down_1"           , &wt_ff_tt_syst_down_1_            );
                
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
                
                if(channel_ == channel::tt){
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
     if(do_pdf_wts_){  
       outtree_->Branch("wt_pdf_1",&wt_pdf_1_);
       outtree_->Branch("wt_pdf_2",&wt_pdf_2_);
       outtree_->Branch("wt_pdf_3",&wt_pdf_3_);
       outtree_->Branch("wt_pdf_4",&wt_pdf_4_);
       outtree_->Branch("wt_pdf_5",&wt_pdf_5_);
       outtree_->Branch("wt_pdf_6",&wt_pdf_6_);
       outtree_->Branch("wt_pdf_7",&wt_pdf_7_);
       outtree_->Branch("wt_pdf_8",&wt_pdf_8_);
       outtree_->Branch("wt_pdf_9",&wt_pdf_9_);
       outtree_->Branch("wt_pdf_10",&wt_pdf_10_);
       outtree_->Branch("wt_pdf_11",&wt_pdf_11_);
       outtree_->Branch("wt_pdf_12",&wt_pdf_12_);
       outtree_->Branch("wt_pdf_13",&wt_pdf_13_);
       outtree_->Branch("wt_pdf_14",&wt_pdf_14_);
       outtree_->Branch("wt_pdf_15",&wt_pdf_15_);
       outtree_->Branch("wt_pdf_16",&wt_pdf_16_);
       outtree_->Branch("wt_pdf_17",&wt_pdf_17_);
       outtree_->Branch("wt_pdf_18",&wt_pdf_18_);
       outtree_->Branch("wt_pdf_19",&wt_pdf_19_);
       outtree_->Branch("wt_pdf_20",&wt_pdf_20_);
       outtree_->Branch("wt_pdf_21",&wt_pdf_21_);
       outtree_->Branch("wt_pdf_22",&wt_pdf_22_);
       outtree_->Branch("wt_pdf_23",&wt_pdf_23_);
       outtree_->Branch("wt_pdf_24",&wt_pdf_24_);
       outtree_->Branch("wt_pdf_25",&wt_pdf_25_);
       outtree_->Branch("wt_pdf_26",&wt_pdf_26_);
       outtree_->Branch("wt_pdf_27",&wt_pdf_27_);
       outtree_->Branch("wt_pdf_28",&wt_pdf_28_);
       outtree_->Branch("wt_pdf_29",&wt_pdf_29_);
       outtree_->Branch("wt_pdf_30",&wt_pdf_30_);
       outtree_->Branch("wt_pdf_31",&wt_pdf_31_);
       outtree_->Branch("wt_pdf_32",&wt_pdf_32_);
       outtree_->Branch("wt_pdf_33",&wt_pdf_33_);
       outtree_->Branch("wt_pdf_34",&wt_pdf_34_);
       outtree_->Branch("wt_pdf_35",&wt_pdf_35_);
       outtree_->Branch("wt_pdf_36",&wt_pdf_36_);
       outtree_->Branch("wt_pdf_37",&wt_pdf_37_);
       outtree_->Branch("wt_pdf_38",&wt_pdf_38_);
       outtree_->Branch("wt_pdf_39",&wt_pdf_39_);
       outtree_->Branch("wt_pdf_40",&wt_pdf_40_);
       outtree_->Branch("wt_pdf_41",&wt_pdf_41_);
       outtree_->Branch("wt_pdf_42",&wt_pdf_42_);
       outtree_->Branch("wt_pdf_43",&wt_pdf_43_);
       outtree_->Branch("wt_pdf_44",&wt_pdf_44_);
       outtree_->Branch("wt_pdf_45",&wt_pdf_45_);
       outtree_->Branch("wt_pdf_46",&wt_pdf_46_);
       outtree_->Branch("wt_pdf_47",&wt_pdf_47_);
       outtree_->Branch("wt_pdf_48",&wt_pdf_48_);
       outtree_->Branch("wt_pdf_49",&wt_pdf_49_);
       outtree_->Branch("wt_pdf_50",&wt_pdf_50_);
       outtree_->Branch("wt_pdf_51",&wt_pdf_51_);
       outtree_->Branch("wt_pdf_52",&wt_pdf_52_);
       outtree_->Branch("wt_pdf_53",&wt_pdf_53_);
       outtree_->Branch("wt_pdf_54",&wt_pdf_54_);
       outtree_->Branch("wt_pdf_55",&wt_pdf_55_);
       outtree_->Branch("wt_pdf_56",&wt_pdf_56_);
       outtree_->Branch("wt_pdf_57",&wt_pdf_57_);
       outtree_->Branch("wt_pdf_58",&wt_pdf_58_);
       outtree_->Branch("wt_pdf_59",&wt_pdf_59_);
       outtree_->Branch("wt_pdf_60",&wt_pdf_60_);
       outtree_->Branch("wt_pdf_61",&wt_pdf_61_);
       outtree_->Branch("wt_pdf_62",&wt_pdf_62_);
       outtree_->Branch("wt_pdf_63",&wt_pdf_63_);
       outtree_->Branch("wt_pdf_64",&wt_pdf_64_);
       outtree_->Branch("wt_pdf_65",&wt_pdf_65_);
       outtree_->Branch("wt_pdf_66",&wt_pdf_66_);
       outtree_->Branch("wt_pdf_67",&wt_pdf_67_);
       outtree_->Branch("wt_pdf_68",&wt_pdf_68_);
       outtree_->Branch("wt_pdf_69",&wt_pdf_69_);
       outtree_->Branch("wt_pdf_70",&wt_pdf_70_);
       outtree_->Branch("wt_pdf_71",&wt_pdf_71_);
       outtree_->Branch("wt_pdf_72",&wt_pdf_72_);
       outtree_->Branch("wt_pdf_73",&wt_pdf_73_);
       outtree_->Branch("wt_pdf_74",&wt_pdf_74_);
       outtree_->Branch("wt_pdf_75",&wt_pdf_75_);
       outtree_->Branch("wt_pdf_76",&wt_pdf_76_);
       outtree_->Branch("wt_pdf_77",&wt_pdf_77_);
       outtree_->Branch("wt_pdf_78",&wt_pdf_78_);
       outtree_->Branch("wt_pdf_79",&wt_pdf_79_);
       outtree_->Branch("wt_pdf_80",&wt_pdf_80_);
       outtree_->Branch("wt_pdf_81",&wt_pdf_81_);
       outtree_->Branch("wt_pdf_82",&wt_pdf_82_);
       outtree_->Branch("wt_pdf_83",&wt_pdf_83_);
       outtree_->Branch("wt_pdf_84",&wt_pdf_84_);
       outtree_->Branch("wt_pdf_85",&wt_pdf_85_);
       outtree_->Branch("wt_pdf_86",&wt_pdf_86_);
       outtree_->Branch("wt_pdf_87",&wt_pdf_87_);
       outtree_->Branch("wt_pdf_88",&wt_pdf_88_);
       outtree_->Branch("wt_pdf_89",&wt_pdf_89_);
       outtree_->Branch("wt_pdf_90",&wt_pdf_90_);
       outtree_->Branch("wt_pdf_91",&wt_pdf_91_);
       outtree_->Branch("wt_pdf_92",&wt_pdf_92_);
       outtree_->Branch("wt_pdf_93",&wt_pdf_93_);
       outtree_->Branch("wt_pdf_94",&wt_pdf_94_);
       outtree_->Branch("wt_pdf_95",&wt_pdf_95_);
       outtree_->Branch("wt_pdf_96",&wt_pdf_96_);
       outtree_->Branch("wt_pdf_97",&wt_pdf_97_);
       outtree_->Branch("wt_pdf_98",&wt_pdf_98_);
       outtree_->Branch("wt_pdf_99",&wt_pdf_99_);
       outtree_->Branch("wt_pdf_100",&wt_pdf_100_);
       
       outtree_->Branch("wt_alphasdown",&wt_alphasdown_);
       outtree_->Branch("wt_alphasup",&wt_alphasup_);
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
      outtree_->Branch("m_2",               &m_2_.var_double);
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
      outtree_->Branch("dijetpt",           &dijetpt_);
      if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17){
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
      outtree_->Branch("db_loose_1",&lbyLooseCombinedIsolation_1);
      outtree_->Branch("db_loose_2",&lbyLooseCombinedIsolation_2);
      outtree_->Branch("db_medium_1",&lbyMediumCombinedIsolation_1);
      outtree_->Branch("db_medium_2",&lbyMediumCombinedIsolation_2);
      outtree_->Branch("db_tight_1",&lbyTightCombinedIsolation_1);
      outtree_->Branch("db_tight_2",&lbyTightCombinedIsolation_2);

      if(strategy_==strategy::cpsummer17) {
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
        outtree_->Branch("mva_newdm_vloose_1",&tau_id_newdm_tight_1_);
        outtree_->Branch("mva_newdm_vloose_2",&tau_id_newdm_tight_1_);
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

      outtree_->Branch("tau_decay_mode_2",    &tau_decay_mode_2_);
      outtree_->Branch("tau_decay_mode_1",    &tau_decay_mode_1_);
      outtree_->Branch("trg_singleelectron",    &trg_singleelectron_);
      outtree_->Branch("trg_singlemuon",    &trg_singlemuon_);
      outtree_->Branch("trg_doubletau",    &trg_doubletau_);
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
      /* outtree_->Branch("jmva_1",             &jmva_1_); */
      /* outtree_->Branch("jmva_2",             &jmva_2_); */

      //outtree_->Branch("HLT_paths",    &HLT_paths_);

/*      outtree_->Branch("leading_lepton_match_pt", &leading_lepton_match_pt_);
      outtree_->Branch("subleading_lepton_match_pt",&subleading_lepton_match_pt_);
      outtree_->Branch("leading_lepton_match_DR", &leading_lepton_match_DR_);
      outtree_->Branch("subleading_lepton_match_DR",&subleading_lepton_match_DR_);*/

      outtree_->Branch("jdeta_lowpt",       &jdeta_lowpt_);
      if (channel_ == channel::em) {
        outtree_->Branch("em_gf_mva",         &em_gf_mva_);
        outtree_->Branch("wt_em_qcd",         &wt_em_qcd_);
        outtree_->Branch("wt_em_qcd_bothaiso",         &wt_em_qcd_bothaiso_);
        outtree_->Branch("wt_em_qcd_up",      &wt_em_qcd_up_);
        outtree_->Branch("wt_em_qcd_down",    &wt_em_qcd_down_);
        outtree_->Branch("wt_em_qcd_shapeup",      &wt_em_qcd_shapeup_);
        outtree_->Branch("wt_em_qcd_shapedown",    &wt_em_qcd_shapedown_);
        outtree_->Branch("wt_em_qcd_extrapup",      &wt_em_qcd_extrapup_);
        outtree_->Branch("wt_em_qcd_extrapdown",    &wt_em_qcd_extrapdown_);
        outtree_->Branch("wt_em_qcd_bjetsdown",    &wt_em_qcd_bjetsdown_);
        outtree_->Branch("wt_em_qcd_bjetsup",    &wt_em_qcd_bjetsup_);
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
        outtree_->Branch("iso_1_puw03", &iso_1_puw03_);
        outtree_->Branch("iso_1_puw04", &iso_1_puw04_);
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
        outtree_->Branch("iso_2_puw03", &iso_2_puw03_);
        outtree_->Branch("iso_2_puw04", &iso_2_puw04_);
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
        
      if(do_jes_vars_){
        outtree_->Branch("n_jets_1",    &n_jets_1_   );
        outtree_->Branch("n_jets_2",    &n_jets_2_   );
        outtree_->Branch("n_jets_3",    &n_jets_3_   );
        outtree_->Branch("n_jets_4",    &n_jets_4_   );
        outtree_->Branch("n_jets_5",    &n_jets_5_   );
        outtree_->Branch("n_jets_6",    &n_jets_6_   );
        outtree_->Branch("n_jets_7",    &n_jets_7_   );
        outtree_->Branch("n_jets_8",    &n_jets_8_   );
        outtree_->Branch("n_jets_9",    &n_jets_9_   );
        outtree_->Branch("n_jets_10",   &n_jets_10_  );
        outtree_->Branch("n_jets_11",   &n_jets_11_  );
        outtree_->Branch("n_jets_12",   &n_jets_12_  );
        outtree_->Branch("n_jets_13",   &n_jets_13_  );
        outtree_->Branch("n_jets_14",   &n_jets_14_  );
        outtree_->Branch("n_jets_15",   &n_jets_15_  );
        outtree_->Branch("n_jets_16",   &n_jets_16_  );
        outtree_->Branch("n_jets_17",   &n_jets_17_  );
        outtree_->Branch("n_jets_18",   &n_jets_18_  );
        outtree_->Branch("n_jets_19",   &n_jets_19_  );
        outtree_->Branch("n_jets_20",   &n_jets_20_  );
        outtree_->Branch("n_jets_21",   &n_jets_21_  );
        outtree_->Branch("n_jets_22",   &n_jets_22_  );
        outtree_->Branch("n_jets_23",   &n_jets_23_  );
        outtree_->Branch("n_jets_24",   &n_jets_24_  );
        outtree_->Branch("n_jets_25",   &n_jets_25_  );
        outtree_->Branch("n_jets_26",   &n_jets_26_  );
        outtree_->Branch("n_jets_27",   &n_jets_27_  );
        outtree_->Branch("n_jets_28",   &n_jets_28_  );
        outtree_->Branch("n_bjets_1",   &n_bjets_1_  );
        outtree_->Branch("n_bjets_2",   &n_bjets_2_  );
        outtree_->Branch("n_bjets_3",   &n_bjets_3_  );
        outtree_->Branch("n_bjets_4",   &n_bjets_4_  );
        outtree_->Branch("n_bjets_5",   &n_bjets_5_  );
        outtree_->Branch("n_bjets_6",   &n_bjets_6_  );
        outtree_->Branch("n_bjets_7",   &n_bjets_7_  );
        outtree_->Branch("n_bjets_8",   &n_bjets_8_  );
        outtree_->Branch("n_bjets_9",   &n_bjets_9_  );
        outtree_->Branch("n_bjets_10",  &n_bjets_10_ );
        outtree_->Branch("n_bjets_11",  &n_bjets_11_ );
        outtree_->Branch("n_bjets_12",  &n_bjets_12_ );
        outtree_->Branch("n_bjets_13",  &n_bjets_13_ );
        outtree_->Branch("n_bjets_14",  &n_bjets_14_ );
        outtree_->Branch("n_bjets_15",  &n_bjets_15_ );
        outtree_->Branch("n_bjets_16",  &n_bjets_16_ );
        outtree_->Branch("n_bjets_17",  &n_bjets_17_ );
        outtree_->Branch("n_bjets_18",  &n_bjets_18_ );
        outtree_->Branch("n_bjets_19",  &n_bjets_19_ );
        outtree_->Branch("n_bjets_20",  &n_bjets_20_ );
        outtree_->Branch("n_bjets_21",  &n_bjets_21_ );
        outtree_->Branch("n_bjets_22",  &n_bjets_22_ );
        outtree_->Branch("n_bjets_23",  &n_bjets_23_ );
        outtree_->Branch("n_bjets_24",  &n_bjets_24_ );
        outtree_->Branch("n_bjets_25",  &n_bjets_25_ );
        outtree_->Branch("n_bjets_26",  &n_bjets_26_ );
        outtree_->Branch("n_bjets_27",  &n_bjets_27_ );
        outtree_->Branch("n_bjets_28",  &n_bjets_28_ );
        outtree_->Branch("mjj_1",       &mjj_1_      );
        outtree_->Branch("mjj_2",       &mjj_2_      );
        outtree_->Branch("mjj_3",       &mjj_3_      );
        outtree_->Branch("mjj_4",       &mjj_4_      );
        outtree_->Branch("mjj_5",       &mjj_5_      );
        outtree_->Branch("mjj_6",       &mjj_6_      );
        outtree_->Branch("mjj_7",       &mjj_7_      );
        outtree_->Branch("mjj_8",       &mjj_8_      );
        outtree_->Branch("mjj_9",       &mjj_9_      );
        outtree_->Branch("mjj_10",      &mjj_10_     );
        outtree_->Branch("mjj_11",      &mjj_11_     );
        outtree_->Branch("mjj_12",      &mjj_12_     );
        outtree_->Branch("mjj_13",      &mjj_13_     );
        outtree_->Branch("mjj_14",      &mjj_14_     );
        outtree_->Branch("mjj_15",      &mjj_15_     );
        outtree_->Branch("mjj_16",      &mjj_16_     );
        outtree_->Branch("mjj_17",      &mjj_17_     );
        outtree_->Branch("mjj_18",      &mjj_18_     );
        outtree_->Branch("mjj_19",      &mjj_19_     );
        outtree_->Branch("mjj_20",      &mjj_20_     );
        outtree_->Branch("mjj_21",      &mjj_21_     );
        outtree_->Branch("mjj_22",      &mjj_22_     );
        outtree_->Branch("mjj_23",      &mjj_23_     );
        outtree_->Branch("mjj_24",      &mjj_24_     );
        outtree_->Branch("mjj_25",      &mjj_25_     );
        outtree_->Branch("mjj_26",      &mjj_26_     );
        outtree_->Branch("mjj_27",      &mjj_27_     );
        outtree_->Branch("mjj_28",      &mjj_28_     );
        if( strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16){
          outtree_->Branch("sjdphi_1",     &sjdphi_1_    );
          outtree_->Branch("sjdphi_2",     &sjdphi_2_    );
          outtree_->Branch("sjdphi_3",     &sjdphi_3_    );
          outtree_->Branch("sjdphi_4",     &sjdphi_4_    );
          outtree_->Branch("sjdphi_5",     &sjdphi_5_    );
          outtree_->Branch("sjdphi_6",     &sjdphi_6_    );
          outtree_->Branch("sjdphi_7",     &sjdphi_7_    );
          outtree_->Branch("sjdphi_8",     &sjdphi_8_    );
          outtree_->Branch("sjdphi_9",     &sjdphi_9_    );
          outtree_->Branch("sjdphi_10",    &sjdphi_10_   );
          outtree_->Branch("sjdphi_11",    &sjdphi_11_   );
          outtree_->Branch("sjdphi_12",    &sjdphi_12_   );
          outtree_->Branch("sjdphi_13",    &sjdphi_13_   );
          outtree_->Branch("sjdphi_14",    &sjdphi_14_   );
          outtree_->Branch("sjdphi_15",    &sjdphi_15_   );
          outtree_->Branch("sjdphi_16",    &sjdphi_16_   );
          outtree_->Branch("sjdphi_17",    &sjdphi_17_   );
          outtree_->Branch("sjdphi_18",    &sjdphi_18_   );
          outtree_->Branch("sjdphi_19",    &sjdphi_19_   );
          outtree_->Branch("sjdphi_20",    &sjdphi_20_   );
          outtree_->Branch("sjdphi_21",    &sjdphi_21_   );
          outtree_->Branch("sjdphi_22",    &sjdphi_22_   );
          outtree_->Branch("sjdphi_23",    &sjdphi_23_   );
          outtree_->Branch("sjdphi_24",    &sjdphi_24_   );
          outtree_->Branch("sjdphi_25",    &sjdphi_25_   );
          outtree_->Branch("sjdphi_26",    &sjdphi_26_   );
          outtree_->Branch("sjdphi_27",    &sjdphi_27_   );
          outtree_->Branch("sjdphi_28",    &sjdphi_28_   );
          outtree_->Branch("D0_1"   , &D0_1_  );
          outtree_->Branch("D0_2"   , &D0_2_  );
          outtree_->Branch("D0_3"   , &D0_3_  );
          outtree_->Branch("D0_4"   , &D0_4_  );
          outtree_->Branch("D0_5"   , &D0_5_  );
          outtree_->Branch("D0_6"   , &D0_6_  );
          outtree_->Branch("D0_7"   , &D0_7_  );
          outtree_->Branch("D0_8"   , &D0_8_  );
          outtree_->Branch("D0_9"   , &D0_9_  );
          outtree_->Branch("D0_10"  , &D0_10_ );
          outtree_->Branch("D0_11"  , &D0_11_ );
          outtree_->Branch("D0_12"  , &D0_12_ );
          outtree_->Branch("D0_13"  , &D0_13_ );
          outtree_->Branch("D0_14"  , &D0_14_ );
          outtree_->Branch("D0_15"  , &D0_15_ );
          outtree_->Branch("D0_16"  , &D0_16_ );
          outtree_->Branch("D0_17"  , &D0_17_ );
          outtree_->Branch("D0_18"  , &D0_18_ );
          outtree_->Branch("D0_19"  , &D0_19_ );
          outtree_->Branch("D0_20"  , &D0_20_ );
          outtree_->Branch("D0_21"  , &D0_21_ );
          outtree_->Branch("D0_22"  , &D0_22_ );
          outtree_->Branch("D0_23"  , &D0_23_ );
          outtree_->Branch("D0_24"  , &D0_24_ );
          outtree_->Branch("D0_25"  , &D0_25_ );
          outtree_->Branch("D0_26"  , &D0_26_ );
          outtree_->Branch("D0_27"  , &D0_27_ );
          outtree_->Branch("D0_28"  , &D0_28_ );
          
          outtree_->Branch("DCP_1"   , &DCP_1_  );
          outtree_->Branch("DCP_2"   , &DCP_2_  );
          outtree_->Branch("DCP_3"   , &DCP_3_  );
          outtree_->Branch("DCP_4"   , &DCP_4_  );
          outtree_->Branch("DCP_5"   , &DCP_5_  );
          outtree_->Branch("DCP_6"   , &DCP_6_  );
          outtree_->Branch("DCP_7"   , &DCP_7_  );
          outtree_->Branch("DCP_8"   , &DCP_8_  );
          outtree_->Branch("DCP_9"   , &DCP_9_  );
          outtree_->Branch("DCP_10"  , &DCP_10_ );
          outtree_->Branch("DCP_11"  , &DCP_11_ );
          outtree_->Branch("DCP_12"  , &DCP_12_ );
          outtree_->Branch("DCP_13"  , &DCP_13_ );
          outtree_->Branch("DCP_14"  , &DCP_14_ );
          outtree_->Branch("DCP_15"  , &DCP_15_ );
          outtree_->Branch("DCP_16"  , &DCP_16_ );
          outtree_->Branch("DCP_17"  , &DCP_17_ );
          outtree_->Branch("DCP_18"  , &DCP_18_ );
          outtree_->Branch("DCP_19"  , &DCP_19_ );
          outtree_->Branch("DCP_20"  , &DCP_20_ );
          outtree_->Branch("DCP_21"  , &DCP_21_ );
          outtree_->Branch("DCP_22"  , &DCP_22_ );
          outtree_->Branch("DCP_23"  , &DCP_23_ );
          outtree_->Branch("DCP_24"  , &DCP_24_ );
          outtree_->Branch("DCP_25"  , &DCP_25_ );
          outtree_->Branch("DCP_26"  , &DCP_26_ );
          outtree_->Branch("DCP_27"  , &DCP_27_ );
          outtree_->Branch("DCP_28"  , &DCP_28_ );
        }
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
        outtree_->Branch("phi_1",             &phi_1_.var_double);
        outtree_->Branch("phi_2",             &phi_2_.var_double);
        outtree_->Branch("dphi",              &dphi_);
        outtree_->Branch("dR",              &dR_);
        outtree_->Branch("E_1",               &E_1_);
        outtree_->Branch("E_2",               &E_2_);
        outtree_->Branch("z_2",               &z_2_);
        outtree_->Branch("met_phi",           &met_phi_.var_double);
        outtree_->Branch("n_prebjets",        &n_prebjets_);
        outtree_->Branch("nearjpt_1",             &nearjpt_1_);
        outtree_->Branch("j1_dm",             &j1_dm_);
        outtree_->Branch("jphi_1",            &jphi_1_, "jphi_1/F");
        outtree_->Branch("jphi_2",            &jphi_2_, "jphi_1/F");
        outtree_->Branch("bpt_1",             &bpt_1_.var_double);
        outtree_->Branch("bpt_2",             &bpt_2_.var_double);
        outtree_->Branch("beta_1",            &beta_1_.var_double);
        outtree_->Branch("bcsv_1",            &bcsv_1_.var_double);
        outtree_->Branch("met_dphi_1",             &met_dphi_1_);
        outtree_->Branch("met_dphi_2",             &met_dphi_2_);
/*        outtree_->Branch("trigger_object_pt_1",&trigger_object_pt_1.var_double);
        outtree_->Branch("trigger_object_eta_1",&trigger_object_eta_1.var_double);
        outtree_->Branch("trigger_object_pt_2",&trigger_object_pt_2.var_double);
        outtree_->Branch("trigger_object_eta_2",&trigger_object_eta_2.var_double);
*/        
        if((strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) && do_sm_scale_wts_){
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
        if((strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) && do_sm_scale_wts_){
          outtree_->Branch("wt_qcdscale_up"   , &wt_qcdscale_up_    );
          outtree_->Branch("wt_qcdscale_down" , &wt_qcdscale_down_ );
        }
        if(do_z_weights_){
          outtree_->Branch("wt_z_mjj",      &wt_z_mjj_);   
          outtree_->Branch("wt_z_mjj_up",      &wt_z_mjj_up_);    
          outtree_->Branch("wt_z_mjj_down",    &wt_z_mjj_down_);    
        }
        if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16){
          outtree_->Branch("wt_tau_id_dm0_up"   ,    &wt_tau_id_dm0_up_);   
          outtree_->Branch("wt_tau_id_dm0_down" ,    &wt_tau_id_dm0_down_); 
          outtree_->Branch("wt_tau_id_dm1_up"   ,    &wt_tau_id_dm1_up_);   
          outtree_->Branch("wt_tau_id_dm1_down" ,    &wt_tau_id_dm1_down_); 
          outtree_->Branch("wt_tau_id_dm10_up"  ,    &wt_tau_id_dm10_up_);  
          outtree_->Branch("wt_tau_id_dm10_down",    &wt_tau_id_dm10_down_); 
          outtree_->Branch("wt_lfake_dm0_up"    ,    &wt_lfake_dm0_up_);    
          outtree_->Branch("wt_lfake_dm0_down"  ,    &wt_lfake_dm0_down_);  
          outtree_->Branch("wt_lfake_dm1_up"    ,    &wt_lfake_dm1_up_);    
          outtree_->Branch("wt_lfake_dm1_down"  ,    &wt_lfake_dm1_down_);     
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
      // SVFit di-tau mass
      synctree_->Branch("m_sv", &m_sv_.var_float, "m_sv/F");
      // SVFit di-tau pt (only for Markov-Chain SVFit)
      synctree_->Branch("pt_sv", &pt_h_.var_float, "pt_h/F");
      // SVFit di-tau eta (only for Markov-Chain SVFit)
      synctree_->Branch("eta_sv", &eta_h_, "eta_h/F");
      // SVFit di-tau phi (only for Markov-Chain SVFit)
      synctree_->Branch("phi_sv", &phi_h_, "phi_h/F");

      // Lepton 1 properties
      if (strategy_ == strategy::cpsummer17 && (channel_ == channel::et || channel_ == channel::em)) { 
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

      synctree_->Branch("tau_decay_mode_2",    &tau_decay_mode_2_, "tau_decay_mode_2/I");
      synctree_->Branch("tau_decay_mode_1",    &tau_decay_mode_1_,"tau_decay_mode_1/I");

      synctree_->Branch("mva_olddm_medium_1",&lbyMediumIsolationMVArun2DBoldDMwLT_1,"mva_olddm_medium_1/O");
      synctree_->Branch("mva_olddm_medium_2",&lbyMediumIsolationMVArun2DBoldDMwLT_2,"mva_olddm_medium_2/O");
      synctree_->Branch("mva_olddm_tight_1",&lbyTightIsolationMVArun2DBoldDMwLT_1,"mva_olddm_tight_1/O");
      synctree_->Branch("mva_olddm_tight_2",&lbyTightIsolationMVArun2DBoldDMwLT_2,"mva_olddm_tight_2/O");
      synctree_->Branch("mva_olddm_vtight_1",&lbyVTightIsolationMVArun2DBoldDMwLT_1,"mva_olddm_tight_1/O");
      synctree_->Branch("mva_olddm_vtight_2",&lbyVTightIsolationMVArun2DBoldDMwLT_2,"mva_olddm_tight_2/O");

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
      if(strategy_ == strategy::fall15||strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) {
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
          
          if(strategy_==strategy::cpsummer17){
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
    if (event->Exists("trg_muonelectron"))   trg_muonelectron_   = event->Get<bool>("trg_muonelectron");
    if (event->Exists("trg_muonelectron_1"))   trg_muonelectron_1_   = event->Get<bool>("trg_muonelectron_1");
    if (event->Exists("trg_muonelectron_2"))   trg_muonelectron_2_   = event->Get<bool>("trg_muonelectron_2");
    if (event->Exists("trg_muonelectron_3"))   trg_muonelectron_3_   = event->Get<bool>("trg_muonelectron_3");
    if (event->Exists("trg_singletau_1"))    trg_singletau_1_      = event->Get<bool>("trg_singletau_1");
    if (event->Exists("trg_singletau_2"))    trg_singletau_2_      = event->Get<bool>("trg_singletau_2");
    if (event->Exists("trg_mutaucross"))     trg_mutaucross_ = event->Get<bool>("trg_mutaucross");
    if (event->Exists("trg_etaucross"))     trg_etaucross_ = event->Get<bool>("trg_etaucross");
    
    if (event->Exists("flagMETFilter")) flagMETFilter_ = event->Get<bool>("flagMETFilter"); 
    else flagMETFilter_ = false;

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    
    wt_ = {eventInfo->total_weight(), static_cast<float>(eventInfo->total_weight())};
    wt_dysoup_ = eventInfo->weight_defined("dysoup") ? eventInfo->weight("dysoup") : 1.0;
    /* wt_dysoup_ = eventInfo->weight_defined("ggHsoup") ? eventInfo->weight("ggHsoup") : 1.0; */

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
    //eventInfo->print_weights();
    wt_tau_id_tight_ = 1.0;
    if (event->Exists("wt_tau_id_tight")) wt_tau_id_tight_  = event->Get<double>("wt_tau_id_tight");
    wt_tau_id_loose_ = 1.0;
    if (event->Exists("wt_tau_id_loose")) wt_tau_id_loose_  = event->Get<double>("wt_tau_id_loose");
    wt_tau_id_medium_ = 1.0;
    if (event->Exists("wt_tau_id_medium")) wt_tau_id_medium_  = event->Get<double>("wt_tau_id_medium");
    wt_tau_id_vtight_ = 1.0;
    if (event->Exists("wt_tau_id_vtight")) wt_tau_id_vtight_  = event->Get<double>("wt_tau_id_vtight");
    wt_tau1_id_loose_ = 1.0;
    if (event->Exists("wt_tau1_id_loose")) wt_tau1_id_loose_  = event->Get<double>("wt_tau1_id_loose");
    wt_tau1_id_medium_ = 1.0;
    if (event->Exists("wt_tau1_id_medium")) wt_tau1_id_medium_  = event->Get<double>("wt_tau1_id_medium");
    wt_tau1_id_tight_ = 1.0;
    if (event->Exists("wt_tau1_id_tight")) wt_tau1_id_tight_  = event->Get<double>("wt_tau1_id_tight");
    wt_tau1_id_vtight_ = 1.0;
    if (event->Exists("wt_tau1_id_vtight")) wt_tau1_id_vtight_  = event->Get<double>("wt_tau1_id_vtight");
    wt_tau2_id_loose_ = 1.0;
    if (event->Exists("wt_tau2_id_loose")) wt_tau2_id_loose_  = event->Get<double>("wt_tau2_id_loose");
    wt_tau2_id_medium_ = 1.0;
    if (event->Exists("wt_tau2_id_medium")) wt_tau2_id_medium_  = event->Get<double>("wt_tau2_id_medium");
    wt_tau2_id_tight_ = 1.0;
    if (event->Exists("wt_tau2_id_tight")) wt_tau2_id_tight_  = event->Get<double>("wt_tau2_id_tight");
    wt_tau2_id_vtight_ = 1.0;
    if (event->Exists("wt_tau2_id_vtight")) wt_tau2_id_vtight_  = event->Get<double>("wt_tau2_id_vtight");
    
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

    
    run_ = eventInfo->run();
    event_ = (unsigned long long) eventInfo->event();
    lumi_ = eventInfo->lumi_block();
    
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
      } else if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17){
        if(event->Exists("wt_ff_1")) wt_ff_1_ = event->Get<double>("wt_ff_1");    
        if(event->Exists("wt_ff_2")) wt_ff_2_ = event->Get<double>("wt_ff_2");
        
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
   if(do_pdf_wts_){ 
     //pdf variation weights
     if(eventInfo->weight_defined("2001")) wt_pdf_1_ = eventInfo->weight("2001"); else wt_pdf_1_=1.0;
     if(eventInfo->weight_defined("2002")) wt_pdf_2_ = eventInfo->weight("2002"); else wt_pdf_2_=1.0;
     if(eventInfo->weight_defined("2003")) wt_pdf_3_ = eventInfo->weight("2003"); else wt_pdf_3_=1.0;
     if(eventInfo->weight_defined("2004")) wt_pdf_4_ = eventInfo->weight("2004"); else wt_pdf_4_=1.0;
     if(eventInfo->weight_defined("2005")) wt_pdf_5_ = eventInfo->weight("2005"); else wt_pdf_5_=1.0;
     if(eventInfo->weight_defined("2006")) wt_pdf_6_ = eventInfo->weight("2006"); else wt_pdf_6_=1.0;
     if(eventInfo->weight_defined("2007")) wt_pdf_7_ = eventInfo->weight("2007"); else wt_pdf_7_=1.0;
     if(eventInfo->weight_defined("2008")) wt_pdf_8_ = eventInfo->weight("2008"); else wt_pdf_8_=1.0;
     if(eventInfo->weight_defined("2009")) wt_pdf_9_ = eventInfo->weight("2009"); else wt_pdf_9_=1.0;
     if(eventInfo->weight_defined("2010")) wt_pdf_10_ = eventInfo->weight("2010"); else wt_pdf_10_=1.0;
     if(eventInfo->weight_defined("2011")) wt_pdf_11_ = eventInfo->weight("2011"); else wt_pdf_11_=1.0;
     if(eventInfo->weight_defined("2012")) wt_pdf_12_ = eventInfo->weight("2012"); else wt_pdf_12_=1.0;
     if(eventInfo->weight_defined("2013")) wt_pdf_13_ = eventInfo->weight("2013"); else wt_pdf_13_=1.0;
     if(eventInfo->weight_defined("2014")) wt_pdf_14_ = eventInfo->weight("2014"); else wt_pdf_14_=1.0;
     if(eventInfo->weight_defined("2015")) wt_pdf_15_ = eventInfo->weight("2015"); else wt_pdf_15_=1.0;
     if(eventInfo->weight_defined("2016")) wt_pdf_16_ = eventInfo->weight("2016"); else wt_pdf_16_=1.0;
     if(eventInfo->weight_defined("2017")) wt_pdf_17_ = eventInfo->weight("2017"); else wt_pdf_17_=1.0;
     if(eventInfo->weight_defined("2018")) wt_pdf_18_ = eventInfo->weight("2018"); else wt_pdf_18_=1.0;
     if(eventInfo->weight_defined("2019")) wt_pdf_19_ = eventInfo->weight("2019"); else wt_pdf_19_=1.0;
     if(eventInfo->weight_defined("2020")) wt_pdf_20_ = eventInfo->weight("2020"); else wt_pdf_20_=1.0;
     if(eventInfo->weight_defined("2021")) wt_pdf_21_ = eventInfo->weight("2021"); else wt_pdf_21_=1.0;
     if(eventInfo->weight_defined("2022")) wt_pdf_22_ = eventInfo->weight("2022"); else wt_pdf_22_=1.0;
     if(eventInfo->weight_defined("2023")) wt_pdf_23_ = eventInfo->weight("2023"); else wt_pdf_23_=1.0;
     if(eventInfo->weight_defined("2024")) wt_pdf_24_ = eventInfo->weight("2024"); else wt_pdf_24_=1.0;
     if(eventInfo->weight_defined("2025")) wt_pdf_25_ = eventInfo->weight("2025"); else wt_pdf_25_=1.0;
     if(eventInfo->weight_defined("2026")) wt_pdf_26_ = eventInfo->weight("2026"); else wt_pdf_26_=1.0;
     if(eventInfo->weight_defined("2027")) wt_pdf_27_ = eventInfo->weight("2027"); else wt_pdf_27_=1.0;
     if(eventInfo->weight_defined("2028")) wt_pdf_28_ = eventInfo->weight("2028"); else wt_pdf_28_=1.0;
     if(eventInfo->weight_defined("2029")) wt_pdf_29_ = eventInfo->weight("2029"); else wt_pdf_29_=1.0;
     if(eventInfo->weight_defined("2030")) wt_pdf_30_ = eventInfo->weight("2030"); else wt_pdf_30_=1.0;
     if(eventInfo->weight_defined("2031")) wt_pdf_31_ = eventInfo->weight("2031"); else wt_pdf_31_=1.0;
     if(eventInfo->weight_defined("2032")) wt_pdf_32_ = eventInfo->weight("2032"); else wt_pdf_32_=1.0;
     if(eventInfo->weight_defined("2033")) wt_pdf_33_ = eventInfo->weight("2033"); else wt_pdf_33_=1.0;
     if(eventInfo->weight_defined("2034")) wt_pdf_34_ = eventInfo->weight("2034"); else wt_pdf_34_=1.0;
     if(eventInfo->weight_defined("2035")) wt_pdf_35_ = eventInfo->weight("2035"); else wt_pdf_35_=1.0;
     if(eventInfo->weight_defined("2036")) wt_pdf_36_ = eventInfo->weight("2036"); else wt_pdf_36_=1.0;
     if(eventInfo->weight_defined("2037")) wt_pdf_37_ = eventInfo->weight("2037"); else wt_pdf_37_=1.0;
     if(eventInfo->weight_defined("2038")) wt_pdf_38_ = eventInfo->weight("2038"); else wt_pdf_38_=1.0;
     if(eventInfo->weight_defined("2039")) wt_pdf_39_ = eventInfo->weight("2039"); else wt_pdf_39_=1.0;
     if(eventInfo->weight_defined("2040")) wt_pdf_40_ = eventInfo->weight("2040"); else wt_pdf_40_=1.0;
     if(eventInfo->weight_defined("2041")) wt_pdf_41_ = eventInfo->weight("2041"); else wt_pdf_41_=1.0;
     if(eventInfo->weight_defined("2042")) wt_pdf_42_ = eventInfo->weight("2042"); else wt_pdf_42_=1.0;
     if(eventInfo->weight_defined("2043")) wt_pdf_43_ = eventInfo->weight("2043"); else wt_pdf_43_=1.0;
     if(eventInfo->weight_defined("2044")) wt_pdf_44_ = eventInfo->weight("2044"); else wt_pdf_44_=1.0;
     if(eventInfo->weight_defined("2045")) wt_pdf_45_ = eventInfo->weight("2045"); else wt_pdf_45_=1.0;
     if(eventInfo->weight_defined("2046")) wt_pdf_46_ = eventInfo->weight("2046"); else wt_pdf_46_=1.0;
     if(eventInfo->weight_defined("2047")) wt_pdf_47_ = eventInfo->weight("2047"); else wt_pdf_47_=1.0;
     if(eventInfo->weight_defined("2048")) wt_pdf_48_ = eventInfo->weight("2048"); else wt_pdf_48_=1.0;
     if(eventInfo->weight_defined("2049")) wt_pdf_49_ = eventInfo->weight("2049"); else wt_pdf_49_=1.0;
     if(eventInfo->weight_defined("2050")) wt_pdf_50_ = eventInfo->weight("2050"); else wt_pdf_50_=1.0;
     if(eventInfo->weight_defined("2051")) wt_pdf_51_ = eventInfo->weight("2051"); else wt_pdf_51_=1.0;
     if(eventInfo->weight_defined("2052")) wt_pdf_52_ = eventInfo->weight("2052"); else wt_pdf_52_=1.0;
     if(eventInfo->weight_defined("2053")) wt_pdf_53_ = eventInfo->weight("2053"); else wt_pdf_53_=1.0;
     if(eventInfo->weight_defined("2054")) wt_pdf_54_ = eventInfo->weight("2054"); else wt_pdf_54_=1.0;
     if(eventInfo->weight_defined("2055")) wt_pdf_55_ = eventInfo->weight("2055"); else wt_pdf_55_=1.0;
     if(eventInfo->weight_defined("2056")) wt_pdf_56_ = eventInfo->weight("2056"); else wt_pdf_56_=1.0;
     if(eventInfo->weight_defined("2057")) wt_pdf_57_ = eventInfo->weight("2057"); else wt_pdf_57_=1.0;
     if(eventInfo->weight_defined("2058")) wt_pdf_58_ = eventInfo->weight("2058"); else wt_pdf_58_=1.0;
     if(eventInfo->weight_defined("2059")) wt_pdf_59_ = eventInfo->weight("2059"); else wt_pdf_59_=1.0;
     if(eventInfo->weight_defined("2060")) wt_pdf_60_ = eventInfo->weight("2060"); else wt_pdf_60_=1.0;
     if(eventInfo->weight_defined("2061")) wt_pdf_61_ = eventInfo->weight("2061"); else wt_pdf_61_=1.0;
     if(eventInfo->weight_defined("2062")) wt_pdf_62_ = eventInfo->weight("2062"); else wt_pdf_62_=1.0;
     if(eventInfo->weight_defined("2063")) wt_pdf_63_ = eventInfo->weight("2063"); else wt_pdf_63_=1.0;
     if(eventInfo->weight_defined("2064")) wt_pdf_64_ = eventInfo->weight("2064"); else wt_pdf_64_=1.0;
     if(eventInfo->weight_defined("2065")) wt_pdf_65_ = eventInfo->weight("2065"); else wt_pdf_65_=1.0;
     if(eventInfo->weight_defined("2066")) wt_pdf_66_ = eventInfo->weight("2066"); else wt_pdf_66_=1.0;
     if(eventInfo->weight_defined("2067")) wt_pdf_67_ = eventInfo->weight("2067"); else wt_pdf_67_=1.0;
     if(eventInfo->weight_defined("2068")) wt_pdf_68_ = eventInfo->weight("2068"); else wt_pdf_68_=1.0;
     if(eventInfo->weight_defined("2069")) wt_pdf_69_ = eventInfo->weight("2069"); else wt_pdf_69_=1.0;
     if(eventInfo->weight_defined("2070")) wt_pdf_70_ = eventInfo->weight("2070"); else wt_pdf_70_=1.0;
     if(eventInfo->weight_defined("2071")) wt_pdf_71_ = eventInfo->weight("2071"); else wt_pdf_71_=1.0;
     if(eventInfo->weight_defined("2072")) wt_pdf_72_ = eventInfo->weight("2072"); else wt_pdf_72_=1.0;
     if(eventInfo->weight_defined("2073")) wt_pdf_73_ = eventInfo->weight("2073"); else wt_pdf_73_=1.0;
     if(eventInfo->weight_defined("2074")) wt_pdf_74_ = eventInfo->weight("2074"); else wt_pdf_74_=1.0;
     if(eventInfo->weight_defined("2075")) wt_pdf_75_ = eventInfo->weight("2075"); else wt_pdf_75_=1.0;
     if(eventInfo->weight_defined("2076")) wt_pdf_76_ = eventInfo->weight("2076"); else wt_pdf_76_=1.0;
     if(eventInfo->weight_defined("2077")) wt_pdf_77_ = eventInfo->weight("2077"); else wt_pdf_77_=1.0;
     if(eventInfo->weight_defined("2078")) wt_pdf_78_ = eventInfo->weight("2078"); else wt_pdf_78_=1.0;
     if(eventInfo->weight_defined("2079")) wt_pdf_79_ = eventInfo->weight("2079"); else wt_pdf_79_=1.0;
     if(eventInfo->weight_defined("2080")) wt_pdf_80_ = eventInfo->weight("2080"); else wt_pdf_80_=1.0;
     if(eventInfo->weight_defined("2081")) wt_pdf_81_ = eventInfo->weight("2081"); else wt_pdf_81_=1.0;
     if(eventInfo->weight_defined("2082")) wt_pdf_82_ = eventInfo->weight("2082"); else wt_pdf_82_=1.0;
     if(eventInfo->weight_defined("2083")) wt_pdf_83_ = eventInfo->weight("2083"); else wt_pdf_83_=1.0;
     if(eventInfo->weight_defined("2084")) wt_pdf_84_ = eventInfo->weight("2084"); else wt_pdf_84_=1.0;
     if(eventInfo->weight_defined("2085")) wt_pdf_85_ = eventInfo->weight("2085"); else wt_pdf_85_=1.0;
     if(eventInfo->weight_defined("2086")) wt_pdf_86_ = eventInfo->weight("2086"); else wt_pdf_86_=1.0;
     if(eventInfo->weight_defined("2087")) wt_pdf_87_ = eventInfo->weight("2087"); else wt_pdf_87_=1.0;
     if(eventInfo->weight_defined("2088")) wt_pdf_88_ = eventInfo->weight("2088"); else wt_pdf_88_=1.0;
     if(eventInfo->weight_defined("2089")) wt_pdf_89_ = eventInfo->weight("2089"); else wt_pdf_89_=1.0;
     if(eventInfo->weight_defined("2090")) wt_pdf_90_ = eventInfo->weight("2090"); else wt_pdf_90_=1.0;
     if(eventInfo->weight_defined("2091")) wt_pdf_91_ = eventInfo->weight("2091"); else wt_pdf_91_=1.0;
     if(eventInfo->weight_defined("2092")) wt_pdf_92_ = eventInfo->weight("2092"); else wt_pdf_92_=1.0;
     if(eventInfo->weight_defined("2093")) wt_pdf_93_ = eventInfo->weight("2093"); else wt_pdf_93_=1.0;
     if(eventInfo->weight_defined("2094")) wt_pdf_94_ = eventInfo->weight("2094"); else wt_pdf_94_=1.0;
     if(eventInfo->weight_defined("2095")) wt_pdf_95_ = eventInfo->weight("2095"); else wt_pdf_95_=1.0;
     if(eventInfo->weight_defined("2096")) wt_pdf_96_ = eventInfo->weight("2096"); else wt_pdf_96_=1.0;
     if(eventInfo->weight_defined("2097")) wt_pdf_97_ = eventInfo->weight("2097"); else wt_pdf_97_=1.0;
     if(eventInfo->weight_defined("2098")) wt_pdf_98_ = eventInfo->weight("2098"); else wt_pdf_98_=1.0;
     if(eventInfo->weight_defined("2099")) wt_pdf_99_ = eventInfo->weight("2099"); else wt_pdf_99_=1.0;
     if(eventInfo->weight_defined("2100")) wt_pdf_100_ = eventInfo->weight("2100"); else wt_pdf_100_=1.0;

     //alpha_s variation weights
     if(eventInfo->weight_defined("2101")) wt_alphasdown_ = eventInfo->weight("2101"); else wt_alphasdown_=1.0; 
     if(eventInfo->weight_defined("2102")) wt_alphasup_ = eventInfo->weight("2102"); else wt_alphasup_=1.0; 
   }
    
    std::vector<PileupInfo *> puInfo;
    float true_int = -1;

    if (event->Exists("pileupInfo") || strategy_ == strategy::phys14 || ((strategy_==strategy::spring15||strategy_==strategy::fall15||strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) && !is_data_ && !is_embedded_) ) {
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
    wt_zpt_up_ = 1.0;
    wt_zpt_down_ = 1.0;
    wt_tau_id_up_ = 1.0;
    wt_tau_id_down_ = 1.0;
    wt_em_qcd_ = 1.0;
    wt_em_qcd_bothaiso_ = 1.0;
    wt_em_qcd_down_ = 1.0;
    wt_em_qcd_up_ = 1.0;
    wt_em_qcd_shapedown_ = 1.0;
    wt_em_qcd_shapeup_ = 1.0;
    wt_em_qcd_bjetsdown_ = 1.0;
    wt_em_qcd_bjetsup_ = 1.0;
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
    if (event->Exists("wt_em_qcd_bothaiso"))         wt_em_qcd_bothaiso_ = event->Get<double>("wt_em_qcd_bothaiso");
    if (event->Exists("wt_em_qcd_up"))      wt_em_qcd_up_ = event->Get<double>("wt_em_qcd_up");
    if (event->Exists("wt_em_qcd_down"))    wt_em_qcd_down_ = event->Get<double>("wt_em_qcd_down");
    if (event->Exists("wt_em_qcd_shapeup"))      wt_em_qcd_shapeup_ = event->Get<double>("wt_em_qcd_shapeup");
    if (event->Exists("wt_em_qcd_shapedown"))    wt_em_qcd_shapedown_ = event->Get<double>("wt_em_qcd_shapedown");
    if (event->Exists("wt_em_qcd_extrapup"))      wt_em_qcd_extrapup_ = event->Get<double>("wt_em_qcd_extrapup");
    if (event->Exists("wt_em_qcd_extrapdown"))    wt_em_qcd_extrapdown_ = event->Get<double>("wt_em_qcd_extrapdown");
    if (event->Exists("wt_em_qcd_bjetsdown"))    wt_em_qcd_bjetsdown_ = event->Get<double>("wt_em_qcd_bjetsdown");
    if (event->Exists("wt_em_qcd_bjetsup"))    wt_em_qcd_bjetsup_ = event->Get<double>("wt_em_qcd_bjetsup");
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
    double btag_wp =  0.679;
    double loose_btag_wp = 0.244;
    if(strategy_ == strategy::phys14) btag_label = "combinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::phys14) btag_wp = 0.814 ;
    if(strategy_ == strategy::spring15) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::spring15) btag_wp = 0.89 ;
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||
      strategy_ == strategy::smspring16  || strategy_ == strategy::mssmsummer16 || 
      strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || 
      strategy_ == strategy::cpsummer17) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16) btag_wp = 0.8;
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) btag_wp = 0.8484;
    if(strategy_ == strategy::cpsummer17) btag_wp = 0.8838;
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 || 
      strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || 
      strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) loose_btag_wp = 0.46;
    if(strategy_ == strategy::cpsummer17) loose_btag_wp = 0.5803;

   //Extra set of jets which are CSV ordered is required for the H->hh analysis
    std::vector<PFJet*> jets_csv = prebjets;
    std::vector<PFJet*> bjets_csv = prebjets;
    std::sort(jets_csv.begin(), jets_csv.end(), bind(&PFJet::GetBDiscriminator, _1, btag_label) > bind(&PFJet::GetBDiscriminator, _2, btag_label));
    std::vector<std::pair<PFJet*,PFJet*> > jet_csv_pairs;
    if(bjet_regression_) jet_csv_pairs = MatchByDR(jets_csv, corrected_jets, 0.5, true, true);

    //Sort out the loose (em,mt,et) or medium (tt) b-jets
    if(era_ != era::data_2016 && era_ != era::data_2017){
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
    } else {
      ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < loose_btag_wp);
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
        if(strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17){
          if(event->Exists("dimuon_veto")) dilep_veto_ = event->Get<bool>("dimuon_veto");
          if(event->Exists("dielec_veto")) dilep_veto_ = dilep_veto_ || event->Get<bool>("dielec_veto");
        }
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        if(event->Exists("minimal_extra_elec_veto")) minimal_extraelec_veto_ = event->Get<bool>("minimal_extra_elec_veto");
        if(event->Exists("minimal_extra_muon_veto")) minimal_extramuon_veto_ = event->Get<bool>("minimal_extra_muon_veto");

    }
    lepton_veto_ = dilepton_veto_ || extraelec_veto_ || extramuon_veto_;
    
    if((strategy_==strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) && !make_sync_ntuple_) dilepton_veto_ = dilep_veto_ || dilepton_veto_;

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
    if(strategy_ == strategy::smspring16 || strategy_ == strategy::mssmspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) pfmet = event->GetPtr<Met>("pfMET");

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

    // printing event for MET
    if (event_ == 139707196 && run_ == 297425 && lumi_ == 87) {
        std::cout << "MET (pt, phi, eta) " << mets->vector().pt() << mets->vector().phi() << mets->vector().eta() << std::endl;
    }
    
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
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(elec, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(elec, eventInfo->lepton_rho());
          iso_1_db03allch_ = PF03IsolationVal(elec, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(elec, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(elec, 0.5, 0);
          iso_1_puw03_ = 0;
          iso_1_puw04_ = 0;
          iso_2_puw03_ = 0;
          iso_2_puw04_ = 0;
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
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(elec, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(elec, eventInfo->lepton_rho());
          iso_1_db03allch_ = PF03IsolationVal(elec, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(elec, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(elec, 0.5, 0);
          iso_1_puw03_ = 0;
          iso_1_puw04_ = 0;
          iso_2_puw03_ = 0;
          iso_2_puw04_ = 0;
          iso_1_trk03_ = 0; 
          iso_2_db03_ = 0;
          iso_2_ea03_ = 0;
          iso_2_db03allch_ = 0;
          iso_2_db04allch_ = 0;
        }
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
      if(strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) {
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
      if(strategy_ == strategy::cpsummer17) {
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
          iso_1_ea03_ = PF03EAIsolationVal(muon, eventInfo->lepton_rho());
          iso_1_db03allch_ = PF03IsolationVal(muon, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(muon, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(muon, 0.5, 0);
          iso_1_trk03_ = MuonTkIsoVal(muon);
          iso_1_puw03_ = PUW03IsolationVal(muon);
          iso_1_puw04_ = PUW04IsolationVal(muon);
          iso_2_puw03_ = 0;
          iso_2_puw04_ = 0;
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
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(muon, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(muon, eventInfo->lepton_rho());
          iso_1_db03allch_ = PF03IsolationVal(muon, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(muon, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(muon, 0.5, 0);
          iso_1_trk03_ = MuonTkIsoVal(muon);
          iso_1_puw03_ = PUW03IsolationVal(muon);
          iso_1_puw04_ = PUW04IsolationVal(muon);
          iso_2_puw03_ = 0;
          iso_2_puw04_ = 0;
          iso_2_db03_ = 0;
          iso_2_ea03_ = 0;
          iso_2_trk03_ = 0;
          iso_2_db04_ = 0;
          iso_2_db03allch_ = 0;
          iso_2_db04allch_ = 0;
        }
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
       if (strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17){
        iso_1_ = PF04IsolationVal(muon, 0.5, 0);
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(muon, 0.5, 0);
          if (strategy_ == strategy::cpsummer17){
            iso_1_ea03_ = PF03EAIsolationVal(muon, eventInfo->jet_rho()); //lepton_rho
          } else {
              iso_1_ea03_ = PF03EAIsolationVal(muon, eventInfo->lepton_rho());
          }
          iso_1_db03allch_ = PF03IsolationVal(muon, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(muon, 0.5, 1);
          iso_1_db04_ = PF04IsolationVal(muon, 0.5, 0);
          iso_1_trk03_ = MuonTkIsoVal(muon);
          iso_1_puw03_ = PUW03IsolationVal(muon);
          iso_1_puw04_ = PUW04IsolationVal(muon);
          iso_2_puw03_ = 0;
          iso_2_puw04_ = 0;
          iso_2_db03_ = 0;
          iso_2_ea03_ = 0;
          iso_2_trk03_ = 0;
          iso_2_db04_ = 0;
          iso_2_db03allch_ = 0;
          iso_2_db04allch_ = 0;
        }
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
      if(strategy_ == strategy::cpsummer17) {
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
        if(iso_study_){
          iso_1_db03_ = PF03IsolationVal(elec, 0.5, 0);
          iso_1_db04_ = PF04IsolationVal(elec, 0.5, 0);
          iso_1_ea03_ = PF03EAIsolationVal(elec, eventInfo->lepton_rho());
          iso_1_db03allch_ = PF03IsolationVal(elec, 0.5, 1);
          iso_1_db04allch_ = PF04IsolationVal(elec, 0.5, 1);
          iso_1_trk03_=0;
          iso_2_puw03_ = PUW03IsolationVal(muon);
          iso_2_puw04_ = PUW04IsolationVal(muon);
          iso_1_puw03_ = 0;
          iso_1_puw04_ = 0;
          iso_2_db03_ = PF03IsolationVal(muon, 0.5, 0);
          iso_2_db04_ = PF04IsolationVal(muon, 0.5, 0);
          iso_2_ea03_ = PF03EAIsolationVal(muon, eventInfo->lepton_rho());
          iso_2_trk03_ = MuonTkIsoVal(muon);
          iso_2_db03allch_ = PF03IsolationVal(muon, 0.5, 1);
          iso_2_db04allch_ = PF04IsolationVal(muon, 0.5, 1);
        }
        mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
      }
      if(strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16){
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        iso_2_ = PF04IsolationVal(muon, 0.5, 0);
        mva_1_ = elec->GetIdIso("generalPurposeMVASpring16");
      }
      if(strategy_ == strategy::cpsummer17) {
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
      if(strategy_ == strategy::mssmspring16 || strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) {
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
      if(strategy_ == strategy::cpsummer17) {
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
    }
    if (channel_ == channel::zee || channel_ == channel::tpzee) {
      Electron const* elec1 = dynamic_cast<Electron const*>(lep1);
      Electron const* elec2 = dynamic_cast<Electron const*>(lep2);
      if(strategy_ == strategy::spring15 || strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) {
        iso_1_ = PF03IsolationVal(elec1, 0.5, 0);
        iso_2_ = PF03IsolationVal(elec2, 0.5, 0);
        if(strategy_ == strategy::cpsummer17){
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
      if(strategy_ == strategy::mssmspring16 || strategy_ == strategy::smspring16 || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17){
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
      jrawf_2_ = lowpt_jets[1]->uncorrected_energy()/lowpt_jets[1]->energy();// * (jets[1]->pt() / jets[1]->energy());
      jptunc_2_ = 0.0;
      jmva_2_ = lowpt_jets[1]->pu_id_mva_value();
      jlrm_2_ = lowpt_jets[1]->linear_radial_moment();
      jctm_2_ = lowpt_jets[1]->charged_multiplicity_nopu();
      mjj_ = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
      jdeta_ = fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta());
      jdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), lowpt_jets[1]->vector());
      dijetpt_ =  (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).pt();

      /* if (!(lowpt_jets[0]->pt()>50 || fabs(lowpt_jets[0]->eta())>3.139 || fabs(lowpt_jets[0]->eta())<2.65) || 
              !(lowpt_jets[1]->pt()>50 || fabs(lowpt_jets[1]->eta())>3.139 || fabs(lowpt_jets[1]->eta())<2.65)) {
        std::cout << "(lowpt_jets[0]->pt(), lowpt_jets[0]->eta()): " << lowpt_jets[0]->pt() << ","<< fabs(lowpt_jets[0]->eta()) << std::endl;
        std::cout << "(lowpt_jets[1]->pt(), lowpt_jets[1]->eta()): " << lowpt_jets[1]->pt() << ","<< fabs(lowpt_jets[1]->eta()) << std::endl;
      } */
      
      if (strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17){
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
    
    if((strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) && do_sm_scale_wts_ && !systematic_shift_){
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
    if((strategy_ == strategy::cpsummer16 || strategy_ == strategy::cpsummer17) && do_sm_scale_wts_ && !systematic_shift_){
      if (official_ggH_){
        wt_qcdscale_up_ = eventInfo->weight_defined("1005") ? eventInfo->weight("1005")*1.18 : 1.0;
        wt_qcdscale_down_ = eventInfo->weight_defined("1009") ? eventInfo->weight("1009")*0.84 : 1.0;
      } else {
        wt_qcdscale_up_ = eventInfo->weight_defined("1005") ? eventInfo->weight("1005")*2.33 : 1.0; 
        wt_qcdscale_down_ = eventInfo->weight_defined("1009") ? eventInfo->weight("1009")*1.72 : 1.0;
      }
      //SM = 1.18179 0.844494, MM = 2.32794 1.71996 PS = 2.32857 1.72129  -> for 2017!
    }
    if(do_z_weights_ && !systematic_shift_){
      wt_z_mjj_   = event->Exists("wt_z_mjj" ) ? event->Get<double>("wt_z_mjj"  ) : 1.0;  
      wt_z_mjj_up_   = event->Exists("wt_z_mjj_up" ) ? event->Get<double>("wt_z_mjj_up"  ) : 1.0;
      wt_z_mjj_down_ = event->Exists("wt_z_mjj_down") ? event->Get<double>("wt_z_mjj_down") : 1.0;   
    }
    if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16){
      wt_tau_id_dm0_up_     = event->Exists("wt_tau_id_dm_up") && tau_decay_mode_2_==0 ? event->Get<double>("wt_tau_id_dm_up") : 1.0;
      wt_tau_id_dm0_down_   = event->Exists("wt_tau_id_dm_down") && tau_decay_mode_2_==0 ? event->Get<double>("wt_tau_id_dm_down") : 1.0;
      wt_tau_id_dm1_up_     = event->Exists("wt_tau_id_dm_up") && tau_decay_mode_2_==1 ? event->Get<double>("wt_tau_id_dm_up") : 1.0;
      wt_tau_id_dm1_down_   = event->Exists("wt_tau_id_dm_down") && tau_decay_mode_2_==1 ? event->Get<double>("wt_tau_id_dm_down") : 1.0;
      wt_tau_id_dm10_up_    = event->Exists("wt_tau_id_dm_up") && tau_decay_mode_2_==10 ? event->Get<double>("wt_tau_id_dm_up") : 1.0;
      wt_tau_id_dm10_down_  = event->Exists("wt_tau_id_dm_down") && tau_decay_mode_2_==10 ? event->Get<double>("wt_tau_id_dm_down") : 1.0;
      wt_lfake_rate_        = event->Exists("wt_lfake_rate") ? event->Get<double>("wt_lfake_rate") : 1.0;
      wt_lfake_dm0_up_      = event->Exists("wt_lfake_rate_up") && tau_decay_mode_2_==0 ? event->Get<double>("wt_lfake_rate_down") : 1.0;
      wt_lfake_dm0_down_    = event->Exists("wt_lfake_rate_down") && tau_decay_mode_2_==0 ? event->Get<double>("wt_lfake_rate_up") : 1.0;
      wt_lfake_dm1_up_      = event->Exists("wt_lfake_rate_up") && tau_decay_mode_2_==1 ? event->Get<double>("wt_lfake_rate_up") : 1.0;
      wt_lfake_dm1_down_    = event->Exists("wt_lfake_rate_down") && tau_decay_mode_2_==1 ? event->Get<double>("wt_lfake_rate_down") : 1.0;     
    }

    if(do_jes_vars_){
      n_jets_1_    = event->Exists("n_jets_1")    ? event->Get<double>("n_jets_1")   : -9999;                
      n_jets_2_    = event->Exists("n_jets_2")    ? event->Get<double>("n_jets_2")   : -9999;
      n_jets_3_    = event->Exists("n_jets_3")    ? event->Get<double>("n_jets_3")   : -9999;
      n_jets_4_    = event->Exists("n_jets_4")    ? event->Get<double>("n_jets_4")   : -9999;
      n_jets_5_    = event->Exists("n_jets_5")    ? event->Get<double>("n_jets_5")   : -9999;
      n_jets_6_    = event->Exists("n_jets_6")    ? event->Get<double>("n_jets_6")   : -9999;
      n_jets_7_    = event->Exists("n_jets_7")    ? event->Get<double>("n_jets_7")   : -9999;
      n_jets_8_    = event->Exists("n_jets_8")    ? event->Get<double>("n_jets_8")   : -9999;
      n_jets_9_    = event->Exists("n_jets_9")    ? event->Get<double>("n_jets_9")   : -9999;
      n_jets_10_   = event->Exists("n_jets_10")   ? event->Get<double>("n_jets_10")  : -9999;
      n_jets_11_   = event->Exists("n_jets_11")   ? event->Get<double>("n_jets_11")  : -9999;
      n_jets_12_   = event->Exists("n_jets_12")   ? event->Get<double>("n_jets_12")  : -9999;
      n_jets_13_   = event->Exists("n_jets_13")   ? event->Get<double>("n_jets_13")  : -9999;
      n_jets_14_   = event->Exists("n_jets_14")   ? event->Get<double>("n_jets_14")  : -9999;
      n_jets_15_   = event->Exists("n_jets_15")   ? event->Get<double>("n_jets_15")  : -9999;
      n_jets_16_   = event->Exists("n_jets_16")   ? event->Get<double>("n_jets_16")  : -9999;
      n_jets_17_   = event->Exists("n_jets_17")   ? event->Get<double>("n_jets_17")  : -9999;
      n_jets_18_   = event->Exists("n_jets_18")   ? event->Get<double>("n_jets_18")  : -9999;
      n_jets_19_   = event->Exists("n_jets_19")   ? event->Get<double>("n_jets_19")  : -9999;
      n_jets_20_   = event->Exists("n_jets_20")   ? event->Get<double>("n_jets_20")  : -9999;
      n_jets_21_   = event->Exists("n_jets_21")   ? event->Get<double>("n_jets_21")  : -9999;
      n_jets_22_   = event->Exists("n_jets_22")   ? event->Get<double>("n_jets_22")  : -9999;
      n_jets_23_   = event->Exists("n_jets_23")   ? event->Get<double>("n_jets_23")  : -9999;
      n_jets_24_   = event->Exists("n_jets_24")   ? event->Get<double>("n_jets_24")  : -9999;
      n_jets_25_   = event->Exists("n_jets_25")   ? event->Get<double>("n_jets_25")  : -9999;
      n_jets_26_   = event->Exists("n_jets_26")   ? event->Get<double>("n_jets_26")  : -9999;
      n_jets_27_   = event->Exists("n_jets_27")   ? event->Get<double>("n_jets_27")  : -9999;
      n_jets_28_   = event->Exists("n_jets_28")   ? event->Get<double>("n_jets_28")  : -9999;
      n_bjets_1_   = event->Exists("n_bjets_1")   ? event->Get<double>("n_bjets_1")  : -9999;
      n_bjets_2_   = event->Exists("n_bjets_2")   ? event->Get<double>("n_bjets_2")  : -9999;
      n_bjets_3_   = event->Exists("n_bjets_3")   ? event->Get<double>("n_bjets_3")  : -9999;
      n_bjets_4_   = event->Exists("n_bjets_4")   ? event->Get<double>("n_bjets_4")  : -9999;
      n_bjets_5_   = event->Exists("n_bjets_5")   ? event->Get<double>("n_bjets_5")  : -9999;
      n_bjets_6_   = event->Exists("n_bjets_6")   ? event->Get<double>("n_bjets_6")  : -9999;
      n_bjets_7_   = event->Exists("n_bjets_7")   ? event->Get<double>("n_bjets_7")  : -9999;
      n_bjets_8_   = event->Exists("n_bjets_8")   ? event->Get<double>("n_bjets_8")  : -9999;
      n_bjets_9_   = event->Exists("n_bjets_9")   ? event->Get<double>("n_bjets_9")  : -9999;
      n_bjets_10_  = event->Exists("n_bjets_10")  ? event->Get<double>("n_bjets_10") : -9999;
      n_bjets_11_  = event->Exists("n_bjets_11")  ? event->Get<double>("n_bjets_11") : -9999;
      n_bjets_12_  = event->Exists("n_bjets_12")  ? event->Get<double>("n_bjets_12") : -9999;
      n_bjets_13_  = event->Exists("n_bjets_13")  ? event->Get<double>("n_bjets_13") : -9999;
      n_bjets_14_  = event->Exists("n_bjets_14")  ? event->Get<double>("n_bjets_14") : -9999;
      n_bjets_15_  = event->Exists("n_bjets_15")  ? event->Get<double>("n_bjets_15") : -9999;
      n_bjets_16_  = event->Exists("n_bjets_16")  ? event->Get<double>("n_bjets_16") : -9999;
      n_bjets_17_  = event->Exists("n_bjets_17")  ? event->Get<double>("n_bjets_17") : -9999;
      n_bjets_18_  = event->Exists("n_bjets_18")  ? event->Get<double>("n_bjets_18") : -9999;
      n_bjets_19_  = event->Exists("n_bjets_19")  ? event->Get<double>("n_bjets_19") : -9999;
      n_bjets_20_  = event->Exists("n_bjets_20")  ? event->Get<double>("n_bjets_20") : -9999;
      n_bjets_21_  = event->Exists("n_bjets_21")  ? event->Get<double>("n_bjets_21") : -9999;
      n_bjets_22_  = event->Exists("n_bjets_22")  ? event->Get<double>("n_bjets_22") : -9999;
      n_bjets_23_  = event->Exists("n_bjets_23")  ? event->Get<double>("n_bjets_23") : -9999;
      n_bjets_24_  = event->Exists("n_bjets_24")  ? event->Get<double>("n_bjets_24") : -9999;
      n_bjets_25_  = event->Exists("n_bjets_25")  ? event->Get<double>("n_bjets_25") : -9999;
      n_bjets_26_  = event->Exists("n_bjets_26")  ? event->Get<double>("n_bjets_26") : -9999;
      n_bjets_27_  = event->Exists("n_bjets_27")  ? event->Get<double>("n_bjets_27") : -9999;
      n_bjets_28_  = event->Exists("n_bjets_28")  ? event->Get<double>("n_bjets_28") : -9999;
      mjj_1_       = event->Exists("mjj_1")       ? event->Get<double>("mjj_1")      : -9999;
      mjj_2_       = event->Exists("mjj_2")       ? event->Get<double>("mjj_2")      : -9999;
      mjj_3_       = event->Exists("mjj_3")       ? event->Get<double>("mjj_3")      : -9999;
      mjj_4_       = event->Exists("mjj_4")       ? event->Get<double>("mjj_4")      : -9999;
      mjj_5_       = event->Exists("mjj_5")       ? event->Get<double>("mjj_5")      : -9999;
      mjj_6_       = event->Exists("mjj_6")       ? event->Get<double>("mjj_6")      : -9999;
      mjj_7_       = event->Exists("mjj_7")       ? event->Get<double>("mjj_7")      : -9999;
      mjj_8_       = event->Exists("mjj_8")       ? event->Get<double>("mjj_8")      : -9999;
      mjj_9_       = event->Exists("mjj_9")       ? event->Get<double>("mjj_9")      : -9999;
      mjj_10_      = event->Exists("mjj_10")      ? event->Get<double>("mjj_10")     : -9999;
      mjj_11_      = event->Exists("mjj_11")      ? event->Get<double>("mjj_11")     : -9999;
      mjj_12_      = event->Exists("mjj_12")      ? event->Get<double>("mjj_12")     : -9999;
      mjj_13_      = event->Exists("mjj_13")      ? event->Get<double>("mjj_13")     : -9999;
      mjj_14_      = event->Exists("mjj_14")      ? event->Get<double>("mjj_14")     : -9999;
      mjj_15_      = event->Exists("mjj_15")      ? event->Get<double>("mjj_15")     : -9999;
      mjj_16_      = event->Exists("mjj_16")      ? event->Get<double>("mjj_16")     : -9999;
      mjj_17_      = event->Exists("mjj_17")      ? event->Get<double>("mjj_17")     : -9999;
      mjj_18_      = event->Exists("mjj_18")      ? event->Get<double>("mjj_18")     : -9999;
      mjj_19_      = event->Exists("mjj_19")      ? event->Get<double>("mjj_19")     : -9999;
      mjj_20_      = event->Exists("mjj_20")      ? event->Get<double>("mjj_20")     : -9999;
      mjj_21_      = event->Exists("mjj_21")      ? event->Get<double>("mjj_21")     : -9999;
      mjj_22_      = event->Exists("mjj_22")      ? event->Get<double>("mjj_22")     : -9999;
      mjj_23_      = event->Exists("mjj_23")      ? event->Get<double>("mjj_23")     : -9999;
      mjj_24_      = event->Exists("mjj_24")      ? event->Get<double>("mjj_24")     : -9999;
      mjj_25_      = event->Exists("mjj_25")      ? event->Get<double>("mjj_25")     : -9999;
      mjj_26_      = event->Exists("mjj_26")      ? event->Get<double>("mjj_26")     : -9999;
      mjj_27_      = event->Exists("mjj_27")      ? event->Get<double>("mjj_27")     : -9999;
      mjj_28_      = event->Exists("mjj_28")      ? event->Get<double>("mjj_28")     : -9999;
      sjdphi_1_     = event->Exists("sjdphi_1")     ? event->Get<double>("sjdphi_1")    : -9999;
      sjdphi_2_     = event->Exists("sjdphi_2")     ? event->Get<double>("sjdphi_2")    : -9999;
      sjdphi_3_     = event->Exists("sjdphi_3")     ? event->Get<double>("sjdphi_3")    : -9999;
      sjdphi_4_     = event->Exists("sjdphi_4")     ? event->Get<double>("sjdphi_4")    : -9999;
      sjdphi_5_     = event->Exists("sjdphi_5")     ? event->Get<double>("sjdphi_5")    : -9999;
      sjdphi_6_     = event->Exists("sjdphi_6")     ? event->Get<double>("sjdphi_6")    : -9999;
      sjdphi_7_     = event->Exists("sjdphi_7")     ? event->Get<double>("sjdphi_7")    : -9999;
      sjdphi_8_     = event->Exists("sjdphi_8")     ? event->Get<double>("sjdphi_8")    : -9999;
      sjdphi_9_     = event->Exists("sjdphi_9")     ? event->Get<double>("sjdphi_9")    : -9999;
      sjdphi_10_    = event->Exists("sjdphi_10")    ? event->Get<double>("sjdphi_10")   : -9999;
      sjdphi_11_    = event->Exists("sjdphi_11")    ? event->Get<double>("sjdphi_11")   : -9999;
      sjdphi_12_    = event->Exists("sjdphi_12")    ? event->Get<double>("sjdphi_12")   : -9999;
      sjdphi_13_    = event->Exists("sjdphi_13")    ? event->Get<double>("sjdphi_13")   : -9999;
      sjdphi_14_    = event->Exists("sjdphi_14")    ? event->Get<double>("sjdphi_14")   : -9999;
      sjdphi_15_    = event->Exists("sjdphi_15")    ? event->Get<double>("sjdphi_15")   : -9999;
      sjdphi_16_    = event->Exists("sjdphi_16")    ? event->Get<double>("sjdphi_16")   : -9999;
      sjdphi_17_    = event->Exists("sjdphi_17")    ? event->Get<double>("sjdphi_17")   : -9999;
      sjdphi_18_    = event->Exists("sjdphi_18")    ? event->Get<double>("sjdphi_18")   : -9999;
      sjdphi_19_    = event->Exists("sjdphi_19")    ? event->Get<double>("sjdphi_19")   : -9999;
      sjdphi_20_    = event->Exists("sjdphi_20")    ? event->Get<double>("sjdphi_20")   : -9999;
      sjdphi_21_    = event->Exists("sjdphi_21")    ? event->Get<double>("sjdphi_21")   : -9999;
      sjdphi_22_    = event->Exists("sjdphi_22")    ? event->Get<double>("sjdphi_22")   : -9999;
      sjdphi_23_    = event->Exists("sjdphi_23")    ? event->Get<double>("sjdphi_23")   : -9999;
      sjdphi_24_    = event->Exists("sjdphi_24")    ? event->Get<double>("sjdphi_24")   : -9999;
      sjdphi_25_    = event->Exists("sjdphi_25")    ? event->Get<double>("sjdphi_25")   : -9999;
      sjdphi_26_    = event->Exists("sjdphi_26")    ? event->Get<double>("sjdphi_26")   : -9999;
      sjdphi_27_    = event->Exists("sjdphi_27")    ? event->Get<double>("sjdphi_27")   : -9999;
      sjdphi_28_    = event->Exists("sjdphi_28")    ? event->Get<double>("sjdphi_28")   : -9999;
      D0_1_    = event->Exists("D0_1")    ? event->Get<int>("D0_1")   : -9999;
      D0_2_    = event->Exists("D0_2")    ? event->Get<int>("D0_2")   : -9999;
      D0_3_    = event->Exists("D0_3")    ? event->Get<int>("D0_3")   : -9999;
      D0_4_    = event->Exists("D0_4")    ? event->Get<int>("D0_4")   : -9999;
      D0_5_    = event->Exists("D0_5")    ? event->Get<int>("D0_5")   : -9999;
      D0_6_    = event->Exists("D0_6")    ? event->Get<int>("D0_6")   : -9999;
      D0_7_    = event->Exists("D0_7")    ? event->Get<int>("D0_7")   : -9999;
      D0_8_    = event->Exists("D0_8")    ? event->Get<int>("D0_8")   : -9999;
      D0_9_    = event->Exists("D0_9")    ? event->Get<int>("D0_9")   : -9999;
      D0_10_    = event->Exists("D0_10")    ? event->Get<int>("D0_10")   : -9999;
      D0_11_    = event->Exists("D0_11")    ? event->Get<int>("D0_11")   : -9999;
      D0_12_    = event->Exists("D0_12")    ? event->Get<int>("D0_12")   : -9999;
      D0_13_    = event->Exists("D0_13")    ? event->Get<int>("D0_13")   : -9999;
      D0_14_    = event->Exists("D0_14")    ? event->Get<int>("D0_14")   : -9999;
      D0_15_    = event->Exists("D0_15")    ? event->Get<int>("D0_15")   : -9999;
      D0_16_    = event->Exists("D0_16")    ? event->Get<int>("D0_16")   : -9999;
      D0_17_    = event->Exists("D0_17")    ? event->Get<int>("D0_17")   : -9999;
      D0_18_    = event->Exists("D0_18")    ? event->Get<int>("D0_18")   : -9999;
      D0_19_    = event->Exists("D0_19")    ? event->Get<int>("D0_19")   : -9999;
      D0_20_    = event->Exists("D0_20")    ? event->Get<int>("D0_20")   : -9999;
      D0_21_    = event->Exists("D0_21")    ? event->Get<int>("D0_21")   : -9999;
      D0_22_    = event->Exists("D0_22")    ? event->Get<int>("D0_22")   : -9999;
      D0_23_    = event->Exists("D0_23")    ? event->Get<int>("D0_23")   : -9999;
      D0_24_    = event->Exists("D0_24")    ? event->Get<int>("D0_24")   : -9999;
      D0_25_    = event->Exists("D0_25")    ? event->Get<int>("D0_25")   : -9999;
      D0_26_    = event->Exists("D0_26")    ? event->Get<int>("D0_26")   : -9999;
      D0_27_    = event->Exists("D0_27")    ? event->Get<int>("D0_27")   : -9999;
      D0_28_    = event->Exists("D0_28")    ? event->Get<int>("D0_28")   : -9999;
      DCP_1_    = event->Exists("DCP_1")    ? event->Get<int>("DCP_1")   : -9999;
      DCP_2_    = event->Exists("DCP_2")    ? event->Get<int>("DCP_2")   : -9999;
      DCP_3_    = event->Exists("DCP_3")    ? event->Get<int>("DCP_3")   : -9999;
      DCP_4_    = event->Exists("DCP_4")    ? event->Get<int>("DCP_4")   : -9999;
      DCP_5_    = event->Exists("DCP_5")    ? event->Get<int>("DCP_5")   : -9999;
      DCP_6_    = event->Exists("DCP_6")    ? event->Get<int>("DCP_6")   : -9999;
      DCP_7_    = event->Exists("DCP_7")    ? event->Get<int>("DCP_7")   : -9999;
      DCP_8_    = event->Exists("DCP_8")    ? event->Get<int>("DCP_8")   : -9999;
      DCP_9_    = event->Exists("DCP_9")    ? event->Get<int>("DCP_9")   : -9999;
      DCP_10_    = event->Exists("DCP_10")    ? event->Get<int>("DCP_10")   : -9999;
      DCP_11_    = event->Exists("DCP_11")    ? event->Get<int>("DCP_11")   : -9999;
      DCP_12_    = event->Exists("DCP_12")    ? event->Get<int>("DCP_12")   : -9999;
      DCP_13_    = event->Exists("DCP_13")    ? event->Get<int>("DCP_13")   : -9999;
      DCP_14_    = event->Exists("DCP_14")    ? event->Get<int>("DCP_14")   : -9999;
      DCP_15_    = event->Exists("DCP_15")    ? event->Get<int>("DCP_15")   : -9999;
      DCP_16_    = event->Exists("DCP_16")    ? event->Get<int>("DCP_16")   : -9999;
      DCP_17_    = event->Exists("DCP_17")    ? event->Get<int>("DCP_17")   : -9999;
      DCP_18_    = event->Exists("DCP_18")    ? event->Get<int>("DCP_18")   : -9999;
      DCP_19_    = event->Exists("DCP_19")    ? event->Get<int>("DCP_19")   : -9999;
      DCP_20_    = event->Exists("DCP_20")    ? event->Get<int>("DCP_20")   : -9999;
      DCP_21_    = event->Exists("DCP_21")    ? event->Get<int>("DCP_21")   : -9999;
      DCP_22_    = event->Exists("DCP_22")    ? event->Get<int>("DCP_22")   : -9999;
      DCP_23_    = event->Exists("DCP_23")    ? event->Get<int>("DCP_23")   : -9999;
      DCP_24_    = event->Exists("DCP_24")    ? event->Get<int>("DCP_24")   : -9999;
      DCP_25_    = event->Exists("DCP_25")    ? event->Get<int>("DCP_25")   : -9999;
      DCP_26_    = event->Exists("DCP_26")    ? event->Get<int>("DCP_26")   : -9999;
      DCP_27_    = event->Exists("DCP_27")    ? event->Get<int>("DCP_27")   : -9999;
      DCP_28_    = event->Exists("DCP_28")    ? event->Get<int>("DCP_28")   : -9999;
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
      if(taus.size()>0){
        tau_pt_1_ = taus[0]->pt();
        tau_dm_1_ = taus[0]->decay_mode();
        if(strategy_==strategy::cpsummer17){
          if(taus[0]->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017")){ tau_id_1_ = 1; tau_vloose_1_=1;}
          if(taus[0]->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017")) tau_id_1_ = 2;
          if(taus[0]->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017")) tau_id_1_ = 3;
          if(taus[0]->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017")) tau_id_1_ = 4; 
          bool pass_antilep = taus[0]->GetTauID("againstMuonLoose3") && taus[0]->GetTauID("againstElectronVLooseMVA6");
          if(!pass_antilep) tau_vloose_1_ = 0;
        } else {
          if(taus[0]->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT")){ tau_id_1_ = 1; tau_vloose_1_=1;}
          if(taus[0]->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT")) tau_id_1_ = 2;
          if(taus[0]->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT")) tau_id_1_ = 3;
          if(taus[0]->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT")) tau_id_1_ = 4;
        }

      }
      if(taus.size()>1){
        tau_pt_2_ = taus[1]->pt();
        tau_dm_2_ = taus[1]->decay_mode();
        if(strategy_==strategy::cpsummer17){
          if(taus[1]->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017")){ tau_id_2_ = 1; tau_vloose_2_=1;}
          if(taus[1]->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017")) tau_id_2_ = 2;
          if(taus[1]->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017")) tau_id_2_ = 3;
          if(taus[1]->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017")) tau_id_2_ = 4;
          bool pass_antilep = taus[1]->GetTauID("againstMuonLoose3") && taus[1]->GetTauID("againstElectronVLooseMVA6");
          if(!pass_antilep) tau_vloose_2_ = 0;
        } else {
          if(taus[1]->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT")) { tau_id_1_ = 1; tau_vloose_2_=1;}
          if(taus[1]->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT")) tau_id_1_ = 2;
          if(taus[1]->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT")) tau_id_1_ = 3;
          if(taus[1]->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT")) tau_id_1_ = 4;
        }
      }
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
