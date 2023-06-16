#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTWeights.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "TMath.h"
#include "TSystem.h"
#include "TFile.h"
#include "boost/format.hpp"

namespace ic {

  HTTWeights::HTTWeights(std::string const& name) : ModuleBase(name),
    channel_(channel::et),
    mc_(mc::summer12_53X),
    era_(era::data_2012_rereco),
    strategy_(strategy::paper2013){
    do_trg_weights_             = false;
    trg_applied_in_mc_          = false;
    do_etau_fakerate_           = false;
    do_mtau_fakerate_           = false;
    do_idiso_weights_           = false;
    do_id_weights_              = false;
    do_zpt_weight_              = false;
    do_zpt_weight_NLO_        = false;
    do_tracking_eff_            = false;
    do_topquark_weights_        = false;
    do_tau_fake_weights_        = false;
    do_em_qcd_weights_          = false;
    gen_tau_collection_         = "genParticlesTaus";
    jets_label_                 = "pfJetsPFlow";
    ditau_label_                = "emtauCandidates";
    scalefactor_file_           = "";
    scalefactor_file_ggh_       = "";
    scalefactor_file_UL_        = "";
    do_tau_id_sf_               = false;
    mssm_higgspt_file_          = "";
    do_mssm_higgspt_            = false;
    is_embedded_                = false;
    do_quarkmass_higgspt_       = false;
    do_ps_weights_              = false;
    do_nnlops_weights_          = false;
    embed_pt_weights_ic_hist_             = nullptr;
    }
HTTWeights::~HTTWeights() {
  ;
}

int HTTWeights::PreAnalysis() {
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HTTWeights" << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << boost::format(param_fmt()) % "channel"             % Channel2String(channel_);
  std::cout << boost::format(param_fmt()) % "era"                 % Era2String(era_);
  std::cout << boost::format(param_fmt()) % "mc"                  % MC2String(mc_);
  std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
  std::cout << boost::format(param_fmt()) % "do_trg_weights"      % do_trg_weights_;
  if (do_trg_weights_) {
    std::cout << boost::format(param_fmt()) % "trg_applied_in_mc" % trg_applied_in_mc_;
  }
  std::cout << boost::format(param_fmt()) % "do_idiso_weights"    % do_idiso_weights_;
  std::cout << boost::format(param_fmt()) % "do_id_weights"       % do_id_weights_;
  std::cout << boost::format(param_fmt()) % "do_etau_fakerate"    % do_etau_fakerate_;
  std::cout << boost::format(param_fmt()) % "do_mtau_fakerate"    % do_mtau_fakerate_;
  std::cout << boost::format(param_fmt()) % "do_zpt_weight"       % do_zpt_weight_;
  std::cout << boost::format(param_fmt()) % "do_zpt_weight_NLO"       % do_zpt_weight_NLO_;
  std::cout << boost::format(param_fmt()) % "do_topquark_weights" % do_topquark_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_fake_weights" % do_tau_fake_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_id_weights"   % do_tau_id_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_id_sf"        % do_tau_id_sf_;
  std::cout << boost::format(param_fmt()) % "do_em_qcd_weights"   % do_em_qcd_weights_;
  std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
  std::cout << boost::format(param_fmt()) % "ditau_label"         % ditau_label_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file"    % scalefactor_file_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file_ggh" % scalefactor_file_ggh_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file_UL" % scalefactor_file_UL_;

  std::string MVADM2017;
  if(era_ == era::data_2016 || era_ == era::data_2017 || era_ == era::data_2018){ 
       MVADM2017 = "MVADM2017v1";

  } else if(era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP || era_ == era::data_2017UL || era_ == era::data_2018UL){ 
        MVADM2017 = "MVADM2017v2";
  } else {
        MVADM2017 = " ";
  }


  if(scalefactor_file_!="") {
    // putting new definitions of all function here for full run2 analyses to avoid the mess/confusion 
    // for now just for 2018 but will update for 2017 and 2016 legacy soon
    TFile f(scalefactor_file_.c_str());
    w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
    f.Close();

    // zpt reweighting - LO
    fns_["zpt_weight_nom"] = std::shared_ptr<RooFunctor>(
    	w_->function("zptmass_weight_nom")->functor(w_->argSet("z_gen_pt,z_gen_mass")));            

    // zpt reweighting
    fns_["zpt_weight_nom_NLO"] = std::shared_ptr<RooFunctor>(
        w_->function("zptmass_weight_nom_NLO")->functor(w_->argSet("z_gen_pt,z_gen_mass")));
    
    // muon trk efficiency from MuonPOG
    fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trk_ratio")->functor(w_->argSet("m_pt,m_eta")));
    // electron reco efficiency from EGammaPOG
    fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));

    // tau ID scale factors & systematics (+stat)
    std::string year = "";
    if (era_ == era::data_2016UL_preVFP){
       year = "2016_preVFP";
    } else if (era_ == era::data_2016UL_postVFP){
       year = "2016_postVFP";
    } else if (era_ == era::data_2017UL){
       year = "2017";
    } else if (era_ == era::data_2018UL){
       year = "2018";
    }
    fns_["t_deeptauid_dm_pt_loose"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));

    fns_["t_deeptauid_dm_pt_loose_uncert0_up"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_fit_uncert0_up").c_str())->functor(w_->argSet("t_dm,t_pt")));
    fns_["t_deeptauid_dm_pt_loose_uncert0_down"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_fit_uncert0_down").c_str())->functor(w_->argSet("t_dm,t_pt")));

    fns_["t_deeptauid_dm_pt_loose_uncert1_up"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_fit_uncert1_up").c_str())->functor(w_->argSet("t_dm,t_pt")));
    fns_["t_deeptauid_dm_pt_loose_uncert1_down"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_fit_uncert1_down").c_str())->functor(w_->argSet("t_dm,t_pt")));

    fns_["t_deeptauid_dm_pt_loose_syst_alleras_up"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_syst_alleras_up_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));
    fns_["t_deeptauid_dm_pt_loose_syst_alleras_down"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_syst_alleras_down_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));

    fns_["t_deeptauid_dm_pt_loose_syst_year_up"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_syst_"+year+"_up_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));
    fns_["t_deeptauid_dm_pt_loose_syst_year_down"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_syst_"+year+"_down_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));

    fns_["t_deeptauid_dm_pt_loose_syst_dm_year_up"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_syst_dm_"+year+"_up_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));
    fns_["t_deeptauid_dm_pt_loose_syst_dm_year_down"] = std::shared_ptr<RooFunctor>(
        w_->function(("tauID_VsJetsLoose_VsEleVVLoose_DM_"+year+"_syst_dm_"+year+"_down_fit").c_str())->functor(w_->argSet("t_dm,t_pt")));

    // l->tau fake SFs
    fns_["t_id_vs_e_eta_vvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vvloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_vvloose_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vvloose_up")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_vvloose_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vvloose_down")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_tight")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_tight_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_tight_up")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_tight_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_tight_down")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_vloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_vloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_vloose_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_vloose_up")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_vloose_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_vloose_down")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_tight")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_tight_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_tight_up")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_tight_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_tight_down")->functor(w_->argSet("t_eta")));
      

    // deepTau Trigger SFs from TauPOG
    // triggers for tau legs in et
    fns_["trg_tauleg_etau_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_data")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_mc")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio")->functor(w_->argSet("t_pt,t_dm")));

    fns_["trg_tauleg_etau_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_etau_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

    // triggers for tau legs in mt
    fns_["trg_tauleg_mutau_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_data")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_mc")->functor(w_->argSet("t_pt,t_dm")));  
    fns_["trg_tauleg_mutau_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio")->functor(w_->argSet("t_pt,t_dm")));

    fns_["trg_tauleg_mutau_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_mutau_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

    // triggers for tau legs in tt
    fns_["trg_tauleg_ditau_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_data")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_mc")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));

    fns_["trg_tauleg_ditau_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["trg_tauleg_ditau_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

    // electron ID-ISO ScaleFactors + Uncertainties
    // Here we have the ID and ISO efficiencies & corrections split in the workspace so, can be added if needed

    fns_["e_idiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_ic_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_idiso_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_ic_ratio_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_idiso_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_ic_ratio_down")->functor(w_->argSet("e_pt,e_eta")));

    fns_["e_idiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_ic_embed_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_idiso_embed_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_ic_embed_ratio_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_idiso_embed_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_ic_embed_ratio_down")->functor(w_->argSet("e_pt,e_eta")));

    // muon ID-ISO ScaleFactors + Uncertainties
    // Here we have the ID and ISO efficiencies & corrections split in the workspace so, can be added if needed
    fns_["m_idiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_ic_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_idiso_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_ic_ratio_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_idiso_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_ic_ratio_down")->functor(w_->argSet("m_pt,m_eta")));

    fns_["m_idiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_idiso_embed_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_ic_embed_ratio_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_idiso_embed_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_ic_embed_ratio_down")->functor(w_->argSet("m_pt,m_eta")));

    fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_id_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_ratio_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_id_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_ratio_down")->functor(w_->argSet("m_pt,m_eta")));

    fns_["m_id_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_id_embed_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_embed_ratio_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_id_embed_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_embed_ratio_down")->functor(w_->argSet("m_pt,m_eta")));

    // SingleMuon Trigger efficiencies / corrections & uncertainties
    fns_["m_single_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_mc")->functor(w_->argSet("m_pt,m_eta"))); 
    fns_["m_single_trg_mc_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_mc_up")->functor(w_->argSet("m_pt,m_eta"))); 
    fns_["m_single_trg_mc_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_mc_down")->functor(w_->argSet("m_pt,m_eta"))); 

    fns_["m_single_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_data")->functor(w_->argSet("m_pt,m_eta"))); 
    fns_["m_single_trg_data_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_data_up")->functor(w_->argSet("m_pt,m_eta"))); 
    fns_["m_single_trg_data_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_data_down")->functor(w_->argSet("m_pt,m_eta"))); 

    fns_["m_single_trg_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_embed")->functor(w_->argSet("m_pt,m_eta"))); 
    fns_["m_single_trg_embed_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_embed_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_single_trg_embed_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_embed_down")->functor(w_->argSet("m_pt,m_eta")));

    fns_["m_single_trg_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_ratio")->functor(w_->argSet("m_pt,m_eta"))); 
    fns_["m_single_trg_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_ratio_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_single_trg_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_ratio_down")->functor(w_->argSet("m_pt,m_eta")));

    fns_["m_single_trg_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_single_trg_embed_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_embed_ratio_up")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_single_trg_embed_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_ic_embed_ratio_down")->functor(w_->argSet("m_pt,m_eta")));

    // SingleElectron Trigger efficiencies / corrections & uncertainties
    fns_["e_single_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_mc")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_mc_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_mc_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_mc_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_mc_down")->functor(w_->argSet("e_pt,e_eta")));

    fns_["e_single_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_data")->functor(w_->argSet("e_pt,e_eta"))); 
    fns_["e_single_trg_data_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_data_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_data_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_data_down")->functor(w_->argSet("e_pt,e_eta")));

    fns_["e_single_trg_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_embed")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_embed_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_embed_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_embed_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_embed_down")->functor(w_->argSet("e_pt,e_eta")));

    fns_["e_single_trg_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_ratio_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_ratio_down")->functor(w_->argSet("e_pt,e_eta")));

    fns_["e_single_trg_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_embed_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_embed_ratio_up"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_embed_ratio_up")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_single_trg_embed_ratio_down"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_ic_embed_ratio_down")->functor(w_->argSet("e_pt,e_eta")));

  //  These have not been calculated yet
  /*  fns_["m_looseiso_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_looseiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
  
      fns_["m_sel_idEmb_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_sel_id_ic_ratio")->functor(w_->argSet("gt_eta,gt_pt")));
  
      fns_["m_sel_trg_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_sel_trg_ic_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));


      // triggers for electron and muon legs in em channel
      fns_["e_trg_binned_12_mc"] = std::shared_ptr<RooFunctor>(w_->function("e_trg_12_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_12_data"] = std::shared_ptr<RooFunctor>(w_->function("e_trg_12_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));

      fns_["e_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(w_->function("e_trg_23_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(w_->function("e_trg_23_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));

      fns_["m_trg_binned_8_mc"] = std::shared_ptr<RooFunctor>(w_->function("m_trg_8_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_8_data"] = std::shared_ptr<RooFunctor>(w_->function("m_trg_8_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));

      fns_["m_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(w_->function("m_trg_23_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(w_->function("m_trg_23_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
  */

  }
  return 0;
}

int HTTWeights::Execute(TreeEvent *event) {

  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);
  EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

  eventInfo->set_weight("wt_prefire",eventInfo->weight_defined("wt_prefire") ? eventInfo->weight("wt_prefire") : 1.0);

  std::string MVADM2017;
  MVADM2017 = "MVADM2017v2";

  std::string po = Channel2String(channel_);
  std::string e_string = "e";
  std::string m_string = "m";
  std::string t_string = "t";

  if (do_tau_id_sf_ && channel_!= channel::em  && channel_!= channel::tpzee  && channel_!= channel::tpzmm && channel_ != channel::zmm && channel_ != channel::zee) {
    double total_id_sf = 1.0;
    double total_etau_fakerate = 1.0;
    double total_mtau_fakerate = 1.0;
    for (int = 0; i < 2; i = i + 1) {
      double id_sf = 1.0;
      double id_sf_vvvloose = 1.0;
      double id_sf_vvloose = 1.0;
      double id_sf_vloose = 1.0;
      double id_sf_loose = 1.0;
      double id_sf_medium = 1.0;
      double id_sf_tight = 1.0;

      double id_sf_loose_up = 1.0;
      double id_sf_loose_down = 1.0;

      double  id_sf_loose_uncert0_up = 1.0;
      double  id_sf_loose_uncert0_down = 1.0;
      double  id_sf_loose_uncert1_up = 1.0;
      double  id_sf_loose_uncert1_down =  1.0;
      double  id_sf_loose_syst_all_eras_up = 1.0;
      double  id_sf_loose_syst_all_eras_down = 1.0;
      double  id_sf_loose_syst_year_up = 1.0;
      double  id_sf_loose_syst_year_down = 1.0;
      double  id_sf_loose_syst_dm_year_up = 1.0;
      double  id_sf_loose_syst_dm_year_down = 1.0;
      double  id_sf_loose_highpt_up = 1.0;
      double  id_sf_loose_highpt_down = 1.0;

      double etau_fakerate = 1.0;
      double etau_fakerate_vvloose = 1.0;
      double etau_fakerate_vloose = 1.0;
      double etau_fakerate_loose = 1.0;
      double etau_fakerate_medium = 1.0;
      double etau_fakerate_tight = 1.0;
      double etau_fakerate_vtight = 1.0;

      double etau_fakerate_vvloose_up = 1.0;
      double etau_fakerate_vvloose_down = 1.0;

      double mtau_fakerate = 1.0;
      double mtau_fakerate_vloose = 1.0;
      double mtau_fakerate_loose = 1.0;
      double mtau_fakerate_medium = 1.0;
      double mtau_fakerate_tight = 1.0;

      double mtau_fakerate_vloose_up = 1.0;
      double mtau_fakerate_vloose_down = 1.0;
      int pn = i + 1; // define to use to get particle information

      if ( e_string == po[i] ) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_id_sf = std::vector<double>{elec->sc_eta(),elec->pt()};
        auto args_iso_sf = std::vector<double>{elec->pt(),elec->sc_eta(),PF03EAIsolationVal(elec, eventInfo->jet_rho())};
        id_sf = fns_["e_id_ratio"]->eval(args_id_sf.data());
        id_sf *= fns_["e_iso_ratio"]->eval(args_iso_sf.data());
      }
      if ( m_string == po[i] ) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_id_sf = std::vector<double>{muon->pt(),muon->eta(),PF04IsolationVal(muon, 0.5, 0)};  
        id_sf = fns_["m_idiso_ratio"]->eval(args_id_sf.data());
      }
      if ( t_string == po[i] ) {
        unsigned gen_match = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_"+std::to_string(pn)));
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_id_sf = std::vector<double>{static_cast<double>(tau->decay_mode())};
        auto args_id_sf_dm_pt = std::vector<double>{static_cast<double>(tau->decay_mode()),tau->pt()};
        id_sf = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose"]->eval(args_id_sf_dm_pt.data()) : 1.0;

        id_sf_loose_uncert0_up = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_uncert0_up"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_uncert0_down = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_uncert0_down"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_uncert1_up = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_uncert1_up"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_uncert1_down = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_uncert1_down"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_syst_all_eras_up = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_syst_alleras_up"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_syst_all_eras_down = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_syst_alleras_down"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_syst_year_up = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_syst_year_up"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_syst_year_down = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_syst_year_down"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_syst_dm_year_up = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_syst_dm_year_up"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_syst_dm_year_down = (gen_match==5) ?fns_["t_deeptauid_dm_pt_loose_syst_dm_year_down"]->eval(args_id_sf_dm_pt.data()) : 1.0;
        id_sf_loose_highpt_up = (gen_match==5) ? (1. + (std::min(tau->pt(),500.) -40.)*(tau->pt()>40)*0.00018) : 1.0;
        id_sf_loose_highpt_down = (gen_match==5) ? (1. - (std::min(tau->pt(),500.) -40.)*(tau->pt()>40)*0.00037) : 1.0;
        if(do_etau_fakerate_) {
          auto args_etau_fakerate = std::vector<double>{fabs(tau->eta())};
          etau_fakerate = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vvloose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vvloose = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vvloose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vloose = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vloose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_loose = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_loose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_medium = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_medium"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_tight = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_tight"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vtight = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vtight"]->eval(args_etau_fakerate.data()) : 1.0;

          etau_fakerate_vvloose_up = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vvloose_up"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vvloose_down = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vvloose_down"]->eval(args_etau_fakerate.data()) : 1.0;

        }
        if(do_mtau_fakerate_) {
          auto args_mtau_fakerate = std::vector<double>{fabs(tau->eta())};
          mtau_fakerate = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_vloose"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_vloose = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_vloose"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_loose = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_loose"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_medium = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_medium"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_tight = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_tight"]->eval(args_mtau_fakerate.data()) : 1.0;

          mtau_fakerate_vloose_up = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_vloose_up"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_vloose_down = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_vloose_down"]->eval(args_mtau_fakerate.data()) : 1.0;
        }
      }
      event->Add("idisoweight_"+std::to_string(pn), id_sf);
      event->Add("idisoweight_vvvloose_"+std::to_string(pn), id_sf_vvvloose);
      event->Add("idisoweight_vvloose_"+std::to_string(pn), id_sf_vvloose);
      event->Add("idisoweight_vloose_"+std::to_string(pn), id_sf_vloose);
      event->Add("idisoweight_loose_"+std::to_string(pn), id_sf_loose);
      event->Add("idisoweight_medium_"+std::to_string(pn), id_sf_medium);
      event->Add("idisoweight_tight_"+std::to_string(pn), id_sf_tight);

      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_up", id_sf_loose_up/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_down", id_sf_loose_down/id_sf_loose);

      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_uncert0_up",id_sf_loose_uncert0_up/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_uncert0_down",id_sf_loose_uncert0_down/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_uncert1_up",id_sf_loose_uncert1_up/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_uncert1_down",id_sf_loose_uncert1_down/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_syst_all_eras_up",id_sf_loose_syst_all_eras_up/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_syst_all_eras_down",id_sf_loose_syst_all_eras_down/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_syst_year_up",id_sf_loose_syst_year_up/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_syst_year_down",id_sf_loose_syst_year_down/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_syst_dm_year_up",id_sf_loose_syst_dm_year_up/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_syst_dm_year_down",id_sf_loose_syst_dm_year_down/id_sf_loose);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_highpt_up",id_sf_loose_highpt_up);
      event->Add("idisoweight_ratio_"+std::to_string(pn)+"_highpt_down",id_sf_loose_highpt_down);

 
      event->Add("etau_fakerate_"+std::to_string(pn), etau_fakerate);
      event->Add("etau_fakerate_vvloose_"+std::to_string(pn), etau_fakerate_vvloose);
      event->Add("etau_fakerate_vloose_"+std::to_string(pn), etau_fakerate_vloose);
      event->Add("etau_fakerate_loose_"+std::to_string(pn), etau_fakerate_loose);
      event->Add("etau_fakerate_medium_"+std::to_string(pn), etau_fakerate_medium);
      event->Add("etau_fakerate_tight_"+std::to_string(pn), etau_fakerate_tight);
      event->Add("etau_fakerate_vtight_"+std::to_string(pn), etau_fakerate_vtight);

      event->Add("etau_fakerate_ratio_"+std::to_string(pn)+"_up", etau_fakerate_vvloose_up/etau_fakerate);
      event->Add("etau_fakerate_ratio_"+std::to_string(pn)+"_down", etau_fakerate_vvloose_down/etau_fakerate);

      event->Add("mtau_fakerate_"+std::to_string(pn), mtau_fakerate);
      event->Add("mtau_fakerate_vloose_"+std::to_string(pn), mtau_fakerate_vloose);
      event->Add("mtau_fakerate_loose_"+std::to_string(pn), mtau_fakerate_loose);
      event->Add("mtau_fakerate_medium_"+std::to_string(pn), mtau_fakerate_medium);
      event->Add("mtau_fakerate_tight_"+std::to_string(pn), mtau_fakerate_tight);

      event->Add("mtau_fakerate_ratio_"+std::to_string(pn)+"_up", mtau_fakerate_vloose_up/etau_fakerate);
      event->Add("mtau_fakerate_ratio_"+std::to_string(pn)+"_down", mtau_fakerate_vloose_down/etau_fakerate);

      total_id_sf *= id_sf;
      total_etau_fakerate *= etau_fakerate;
      total_mtau_fakerate *= mtau_fakerate;
    }
    eventInfo->set_weight("wt_tau_id_sf",total_id_sf);
    eventInfo->set_weight("wt_etau_fakerate",total_etau_fakerate); 
    eventInfo->set_weight("wt_mtau_fakerate",total_mtau_fakerate);
  } 
  
  if ((do_idiso_weights_ || do_id_weights) && (channel_ == channel::em  || channel_ == channel::tpzee || channel_ == channel::tpzmm || channel_ == channel::zmm || channel_ == channel::zee)){
     double total_lepton_id_sf = 1.0;
     for (int i = 0; i < 2; i = i + 1) {
        double lepton_id_sf = 1.0;
        int pn = i + 1; // define to use to get particle information

        if ( e_string == po[i] ) {
          Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
          auto args_id_sf = std::vector<double>{elec->sc_eta(),elec->pt()};
          auto args_iso_sf = std::vector<double>{elec->pt(),elec->sc_eta(),PF03EAIsolationVal(elec, eventInfo->jet_rho())};
          lepton_id_sf = fns_["e_id_ratio"]->eval(args_id_sf.data());
          lepton_id_sf *= fns_["e_iso_ratio"]->eval(args_iso_sf.data());
        }
        if ( m_string == po[i] ) {
          Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
          auto args_id_sf = std::vector<double>{muon->pt(),muon->eta(),PF04IsolationVal(muon, 0.5, 0)};  
          id_sf = fns_["m_idiso_ratio"]->eval(args_id_sf.data());
        }
        total_lepton_id_sf *= lepton_id_sf;
     }
     eventInfo->set_weight("wt_lepton_id_sf",total_lepton_id_sf);
  }

  if (do_zpt_weight_){

      double zpt = event->Exists("genpT") ? event->Get<double>("genpT") : 0;
      double zmass = event->Exists("genM") ? event->Get<double>("genM") : 0;

      if(!event->Exists("genpT") || !event->Exists("genM")) {

        std::vector<GenParticle *> sel_gen_parts;
        std::vector<GenParticle *> parts;
        if(event->ExistsInTree("genParticles")) parts = event->GetPtrVec<GenParticle>("genParticles");

        for(unsigned i = 0; i < parts.size(); ++i){
          std::vector<bool> status_flags = parts[i]->statusFlags();
          unsigned id = abs(parts[i]->pdgid());
          unsigned status = abs(parts[i]->status());
          if ( (id >= 11 && id <= 16 && status_flags[FromHardProcess] && status==1) || status_flags[IsDirectHardProcessTauDecayProduct]) sel_gen_parts.push_back(parts[i]);
        }

        ROOT::Math::PtEtaPhiEVector gen_boson;
        for( unsigned i = 0; i < sel_gen_parts.size() ; ++i){
          gen_boson += sel_gen_parts[i]->vector();
        }
        zpt = gen_boson.pt();
        zmass = gen_boson.M();

      }

      auto args = std::vector<double>{zpt,zmass};  
      double wtzpt = fns_["zpt_weight_nom"]->eval(args.data());
      double wtzpt_down=1.0;
      double wtzpt_up = wtzpt*wtzpt;
      eventInfo->set_weight("wt_zpt",wtzpt);
      event->Add("zpt_sf",wtzpt);
      event->Add("wt_zpt_up",wtzpt_up/wtzpt);
      event->Add("wt_zpt_down",wtzpt_down/wtzpt);
   }
  if (do_zpt_weight_NLO_){

      double zpt = event->Exists("genpT") ? event->Get<double>("genpT") : 0;
      double zmass = event->Exists("genM") ? event->Get<double>("genM") : 0;

      if(!event->Exists("genpT") || !event->Exists("genM")) {

        std::vector<GenParticle *> sel_gen_parts;
        std::vector<GenParticle *> parts;
        if(event->ExistsInTree("genParticles")) parts = event->GetPtrVec<GenParticle>("genParticles");

        for(unsigned i = 0; i < parts.size(); ++i){
          std::vector<bool> status_flags = parts[i]->statusFlags();
          unsigned id = abs(parts[i]->pdgid());
          unsigned status = abs(parts[i]->status());
          if ( (id >= 11 && id <= 16 && status_flags[FromHardProcess] && status==1) || status_flags[IsDirectHardProcessTauDecayProduct]) sel_gen_parts.push_back(parts[i]);
        }

        ROOT::Math::PtEtaPhiEVector gen_boson;
        for( unsigned i = 0; i < sel_gen_parts.size() ; ++i){
          gen_boson += sel_gen_parts[i]->vector();
        }
        zpt = gen_boson.pt();
        zmass = gen_boson.M();

      }

      auto args = std::vector<double>{zpt,zmass};
      double wtzpt_NLO = fns_["zpt_weight_nom_NLO"]->eval(args.data());
      double wtzpt_down_NLO=1.0;
      double wtzpt_up_NLO = wtzpt_NLO*wtzpt_NLO;
      eventInfo->set_weight("wt_zpt_NLO",wtzpt_NLO);
      event->Add("zpt_sf_NLO",wtzpt_NLO);
      event->Add("wt_zpt_up_NLO",wtzpt_up_NLO/wtzpt_NLO);
      event->Add("wt_zpt_down_NLO",wtzpt_down_NLO/wtzpt_NLO);
   }

  if (do_tracking_eff_) {
    double total_trk_sf = 1.0;
    for (int i = 0; i < 2; i = i + 1) {
      double trk_sf = 1.0;
      int pn = i + 1; // define to use to get particle information
      if ( e_string == po[i] ) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_trk_sf = std::vector<double>{elec->pt(),elec->sc_eta()};
        trk_sf *= fns_["e_trk_ratio"]->eval(args_trk_sf.data());
      }
      if ( m_string == po[i] ) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_trk_sf = std::vector<double>{muon->eta()};
        trk_sf *= fns_["m_trk_ratio"]->eval(args_trk_sf.data());
      }
      event->Add("trackingweight_"+std::to_string(pn),trk_sf);
      total_trk_sf *= trk_sf;
    }
    eventInfo->set_weight("wt_tracking_sf",total_trk_sf);
 }
       
 if (do_trg_weights_) {
   if (channel_ == channel::et) {
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double e_pt = elec->pt();
     double e_iso = PF03IsolationVal(elec,0.5,0);
     double e_eta = fabs(elec->eta());
     double t_pt = tau->pt();
     double t_signed_eta = tau->eta();
     double t_phi = tau->phi();
     double ele_trg = 1.0;
     double tau_trg = 1.0;
     double tau_trg_mc=1.;
     double ele_trg_mc = 1.0;
      // add here for e tau cross trg
      e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho());
      auto args_1 = std::vector<double>{e_pt,e_eta,e_iso};
      ele_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
      if(!is_embedded_) ele_trg_mc = fns_["e_trg_binned_mc"]->eval(args_1.data());
      else ele_trg_mc = fns_["e_trg_binned_embed"]->eval(args_1.data());

      double single_e_sf = ele_trg_mc>0 ? ele_trg / ele_trg_mc : 0.;
      double t_dm = tau->decay_mode();
      auto args_2 = std::vector<double>{e_pt,e_eta}; 
      auto args_3 = std::vector<double>{t_pt,t_signed_eta,t_phi,t_dm}; 
      auto args_4 = std::vector<double>{t_pt,t_dm};

      double t_mvadm = tau->GetTauID(MVADM2017);
      auto args_mvadm = std::vector<double>{t_pt,t_mvadm};

      double ele_xtrg = 1.;
      double ele_xtrg_mc=1.;
      double tau_trg_ic=1., tau_trg_mvadm=1., tau_trg_ic_data=1., tau_trg_mvadm_data=1., tau_trg_data=1., tau_trg_mvadm_mc=1.;

      double xtrg_et_sf_mvadm=1.;

      if(era_ == era::data_2017 || era_ == era::data_2017UL || era_ == era::data_2018 || era_ == era::data_2018UL) {

        ele_xtrg = fns_["e_crosstrg_data"]->eval(args_1.data());
        if(is_embedded_) ele_xtrg_mc = fns_["e_crosstrg_embed"]->eval(args_1.data());
        else ele_xtrg_mc = fns_["e_crosstrg_mc"]->eval(args_1.data());
        
        if(is_embedded_) {
          tau_trg_mc = 1.;
          tau_trg = fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
          tau_trg_data = fns_["t_trg_30_embed_data"]->eval(args_3.data());
          tau_trg_ic = fns_["t_trg_30_ic_embed_ratio"]->eval(args_4.data());
          tau_trg_mvadm = fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
          tau_trg_ic_data = fns_["t_trg_30_ic_embed_data"]->eval(args_4.data());
          tau_trg_mvadm_data = fns_["t_trg_30_ic_mvadm_embed_data"]->eval(args_mvadm.data());
          tau_trg_mvadm_mc = fns_["t_trg_30_ic_mvadm_embed"]->eval(args_mvadm.data());
        }
        else {
          tau_trg_mc = 1.;
          tau_trg = fns_["t_trg_30_ratio"]->eval(args_4.data());
          tau_trg_ic = fns_["t_trg_30_ic_ratio"]->eval(args_4.data());
          tau_trg_mvadm = fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
          tau_trg_mvadm_mc = fns_["t_trg_30_ic_mvadm_mc"]->eval(args_mvadm.data());

        }
        xtrg_et_sf_mvadm = tau_trg_mvadm;

      }

      double et_trg_or=1.;
      //double cpdecay_sf = 1.;
      if(era_ != era::data_2016 || era_ != era::data_2016UL_preVFP || era_ != era::data_2016UL_postVFP){
        et_trg_or = (ele_trg + (ele_xtrg - ele_trg)*tau_trg_mvadm_data)/(ele_trg_mc + (ele_xtrg_mc - ele_trg_mc)*tau_trg_mvadm_mc);   
        //eff(single-e) + (eff(e-leg, e-tau)-eff(single-e))*eff(tau-leg, e-tau) 
      }

      double e_high_pt_cut=28.;
      if(era_ == era::data_2018 || era_ == era::data_2018UL) e_high_pt_cut=33;

      double xtrg_et_sf = (ele_xtrg_mc*tau_trg_mc) > 0 ? (ele_xtrg*tau_trg)/(ele_xtrg_mc*tau_trg_mc) : 0.0;

      double xtrg_OR_sf = (e_pt >= e_high_pt_cut) ? single_e_sf : xtrg_et_sf;

      // if trigger is not applied in the MC then set the SF to the efficiency
      if(!trg_applied_in_mc_) {
        xtrg_et_sf = ele_xtrg*tau_trg;
        xtrg_OR_sf = ele_trg*(1-tau_trg) + ele_xtrg*tau_trg;
        single_e_sf = ele_trg;
      }

      xtrg_et_sf_mvadm = ele_xtrg / ele_xtrg_mc *tau_trg_mvadm;

      if((era_ == era::data_2017 || era_ == era::data_2017UL) && is_embedded_ && e_pt<40 && fabs(e_eta)>1.479){
        // electron triggers in this eta/pT region don't work properly for the embedding in 2017 so set the SF to the data efficiency and have all events pass the trigger in HTTTriggerFilter - this works for the 2018 but needs to be checked for the 2016 legacy!
        single_e_sf = ele_trg;
        xtrg_et_sf = ele_xtrg*tau_trg_data;
        xtrg_OR_sf = (e_pt >= e_high_pt_cut) ? single_e_sf : xtrg_et_sf;
        tau_trg_ic = tau_trg_ic_data; 
        tau_trg_mvadm = tau_trg_mvadm_data;
        xtrg_et_sf_mvadm = tau_trg_mvadm_data*ele_xtrg; 
        et_trg_or = ele_trg + (ele_xtrg - ele_trg)*tau_trg_mvadm_data;   
      }

      bool pass_single_offline = (e_pt>e_high_pt_cut);
      bool pass_cross_offline = (t_pt>35);

      if (!pass_cross_offline) et_trg_or = single_e_sf;
      else if(!pass_single_offline) et_trg_or = xtrg_et_sf_mvadm;

      event->Add("et_trg_or",et_trg_or); 
      event->Add("et_trg_cross",xtrg_et_sf_mvadm); 
      event->Add("et_trg_single",single_e_sf); 
 
      // for 2016 we only use the single electron trigger
      if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP) {
        xtrg_et_sf=0.;
        xtrg_OR_sf=single_e_sf;
      }

      if(era_ != era::data_2016 || era_ != era::data_2016UL_preVFP || era_ != era::data_2016UL_postVFP) {
        tau_trg_ic = (tau_trg==0) ? tau_trg_ic : tau_trg_ic/tau_trg;
        tau_trg_mvadm = (tau_trg==0) ? tau_trg_mvadm : tau_trg_mvadm/tau_trg;
        tau_trg_ic = (e_pt>=e_high_pt_cut) ? 1. : tau_trg_ic;
        tau_trg_mvadm = (e_pt>=e_high_pt_cut) ? 1. : tau_trg_mvadm;
        event->Add("wt_tau_trg_ic",tau_trg_ic);
        event->Add("wt_tau_trg_mvadm",tau_trg_mvadm);
      } else {
        event->Add("wt_tau_trg_ic", 1.);
        event->Add("wt_tau_trg_mvadm", 1.);
      }

      eventInfo->set_weight("trigger", xtrg_OR_sf, false);

      std::vector<double> args_etau = std::vector<double>{e_pt,e_eta,e_iso,t_pt,t_dm,t_signed_eta};
      std::string extra = "";
      if(is_embedded_){
        args_etau = std::vector<double>{e_pt,e_eta,e_iso,t_pt,t_dm,t_signed_eta,t_phi};
        extra="_embed";
      }

      if(era_ == era::data_2017 || era_ == era::data_2017UL){
        // in 2017 these is an additional factor recommended by the EGammas POG to correct electron triggers
        xtrg_OR_sf*=0.991;
        single_e_sf*=0.991;
        xtrg_et_sf*=0.991;
      }

      std::vector<std::string> systs = {"_crosslep_up","_crosslep_down","_singlelep_up","_singlelep_down","_singletau_up","_singletau_down","_dm0_up","_dm0_down","_dm1_up","_dm1_down","_dm10_up","_dm10_down","_dm11_up","_dm11_down"};

      double wt_tau_trg_mssm = fns_["et_trg"+extra+"_ratio"]->eval(args_etau.data());
      eventInfo->set_weight("et_triggerweight_ic", wt_tau_trg_mssm, false);
      event->Add("wt_tau_trg_mssm", xtrg_OR_sf > 0. ? wt_tau_trg_mssm/xtrg_OR_sf : 0.);
      for (auto s: systs) {
        double wt_tau_trg_mssm_syst = fns_["et_trg"+extra+"_ratio"+s]->eval(args_etau.data());
        event->Add("wt_tau_trg_mssm"+s, wt_tau_trg_mssm > 0. ? wt_tau_trg_mssm_syst/wt_tau_trg_mssm : 0.);
      }
      event->Add("wt_tau_trg_mssm_doubleonly", 1.);

      double trigweight_up=1., trigweight_down=1.;
      double tau_trg_dm0_up=1.;
      double tau_trg_dm1_up=1.;
      double tau_trg_dm10_up=1.;
      double tau_trg_dm11_up=1.;
      double tau_trg_dm0_down=1.;
      double tau_trg_dm1_down=1.;
      double tau_trg_dm10_down=1.;
      double tau_trg_dm11_down=1.;

      double tau_trg_mvadm0_up=1;
      double tau_trg_mvadm1_up=1;
      double tau_trg_mvadm2_up=1;
      double tau_trg_mvadm10_up=1;
      double tau_trg_mvadm11_up=1;
      double tau_trg_mvadm0_down=1;
      double tau_trg_mvadm1_down=1;
      double tau_trg_mvadm2_down=1;
      double tau_trg_mvadm10_down=1;
      double tau_trg_mvadm11_down=1;


      if(era_ == era::data_2017 || era_ == era::data_2017UL || era_ == era::data_2018 || era_ == era::data_2018UL) {

        if(e_pt<e_high_pt_cut) { // if this isn't true then we are using the single lepton trigger and therefore the weight = 1
          if(!is_embedded_) {
            if(fns_["t_trg_30_ratio"]->eval(args_4.data())>0){
              tau_trg_dm0_up=fns_["t_trg_30_ratio_dm0_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm1_up=fns_["t_trg_30_ratio_dm1_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm10_up=fns_["t_trg_30_ratio_dm10_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm11_up=fns_["t_trg_30_ratio_dm11_up"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm0_down=fns_["t_trg_30_ratio_dm0_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm1_down=fns_["t_trg_30_ratio_dm1_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm10_down=fns_["t_trg_30_ratio_dm10_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
              tau_trg_dm11_down=fns_["t_trg_30_ratio_dm11_down"]->eval(args_4.data())/fns_["t_trg_30_ratio"]->eval(args_4.data());
            }
            if(fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data())>0){
              tau_trg_mvadm0_up   =fns_["t_trg_30_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm1_up   =fns_["t_trg_30_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm2_up   =fns_["t_trg_30_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm10_up  =fns_["t_trg_30_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm11_up  =fns_["t_trg_30_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm0_down =fns_["t_trg_30_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm1_down =fns_["t_trg_30_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm2_down =fns_["t_trg_30_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm10_down=fns_["t_trg_30_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm11_down=fns_["t_trg_30_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_ratio"]->eval(args_mvadm.data());
            }

          } else {
            if(fns_["t_trg_30_embed_ratio"]->eval(args_3.data())>0) {
              tau_trg_dm0_up=fns_["t_trg_30_embed_ratio_dm0_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm1_up=fns_["t_trg_30_embed_ratio_dm1_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm10_up=fns_["t_trg_30_embed_ratio_dm10_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm11_up=fns_["t_trg_30_embed_ratio_dm11_up"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm0_down=fns_["t_trg_30_embed_ratio_dm0_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm1_down=fns_["t_trg_30_embed_ratio_dm1_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm10_down=fns_["t_trg_30_embed_ratio_dm10_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
              tau_trg_dm11_down=fns_["t_trg_30_embed_ratio_dm11_down"]->eval(args_3.data())/fns_["t_trg_30_embed_ratio"]->eval(args_3.data());
            }
            if(fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data())>0){
              tau_trg_mvadm0_up   =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm1_up   =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm2_up   =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm10_up  =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm11_up  =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm0_down =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm1_down =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm2_down =fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm10_down=fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
              tau_trg_mvadm11_down=fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_30_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            }  
          }
        }

        tau_trg_dm0_up = std::isnan(tau_trg_dm0_up) ? 0 : tau_trg_dm0_up;
        tau_trg_dm0_down = std::isnan(tau_trg_dm0_down) ? 0 : tau_trg_dm0_down;
        tau_trg_dm1_up = std::isnan(tau_trg_dm1_up) ? 0 : tau_trg_dm1_up;
        tau_trg_dm1_down = std::isnan(tau_trg_dm1_down) ? 0 : tau_trg_dm1_down;
        tau_trg_dm10_up = std::isnan(tau_trg_dm10_up) ? 0 : tau_trg_dm10_up;
        tau_trg_dm10_down = std::isnan(tau_trg_dm10_down) ? 0 : tau_trg_dm10_down;
        tau_trg_dm11_up = std::isnan(tau_trg_dm11_up) ? 0 : tau_trg_dm11_up;
        tau_trg_dm11_down = std::isnan(tau_trg_dm11_down) ? 0 : tau_trg_dm11_down;

        tau_trg_mvadm0_up    = std::isnan(tau_trg_mvadm0_up)    ? 0 : tau_trg_mvadm0_up;
        tau_trg_mvadm0_down  = std::isnan(tau_trg_mvadm0_down)  ? 0 : tau_trg_mvadm0_down;
        tau_trg_mvadm1_up    = std::isnan(tau_trg_mvadm1_up)    ? 0 : tau_trg_mvadm1_up;
        tau_trg_mvadm1_down  = std::isnan(tau_trg_mvadm1_down)  ? 0 : tau_trg_mvadm1_down;
        tau_trg_mvadm2_up    = std::isnan(tau_trg_mvadm2_up)    ? 0 : tau_trg_mvadm2_up;
        tau_trg_mvadm2_down  = std::isnan(tau_trg_mvadm2_down)  ? 0 : tau_trg_mvadm2_down;
        tau_trg_mvadm10_up   = std::isnan(tau_trg_mvadm10_up)   ? 0 : tau_trg_mvadm10_up;
        tau_trg_mvadm10_down = std::isnan(tau_trg_mvadm10_down) ? 0 : tau_trg_mvadm10_down;
        tau_trg_mvadm11_up   = std::isnan(tau_trg_mvadm11_up)   ? 0 : tau_trg_mvadm11_up;
        tau_trg_mvadm11_down = std::isnan(tau_trg_mvadm11_down) ? 0 : tau_trg_mvadm11_down;

        event->Add("wt_tau_trg_dm0_up",tau_trg_dm0_up);
        event->Add("wt_tau_trg_dm1_up",tau_trg_dm1_up);
        event->Add("wt_tau_trg_dm10_up",tau_trg_dm10_up);
        event->Add("wt_tau_trg_dm11_up",tau_trg_dm11_up);
        event->Add("wt_tau_trg_dm0_down",tau_trg_dm0_down);
        event->Add("wt_tau_trg_dm1_down",tau_trg_dm1_down);
        event->Add("wt_tau_trg_dm10_down",tau_trg_dm10_down);
        event->Add("wt_tau_trg_dm11_down",tau_trg_dm11_down);

        event->Add("wt_tau_trg_mvadm0_up"   ,tau_trg_mvadm0_up);
        event->Add("wt_tau_trg_mvadm1_up"   ,tau_trg_mvadm1_up);
        event->Add("wt_tau_trg_mvadm2_up"   ,tau_trg_mvadm2_up);
        event->Add("wt_tau_trg_mvadm10_up"  ,tau_trg_mvadm10_up);
        event->Add("wt_tau_trg_mvadm11_up"  ,tau_trg_mvadm11_up);
        event->Add("wt_tau_trg_mvadm0_down" ,tau_trg_mvadm0_down);
        event->Add("wt_tau_trg_mvadm1_down" ,tau_trg_mvadm1_down);
        event->Add("wt_tau_trg_mvadm2_down" ,tau_trg_mvadm2_down);
        event->Add("wt_tau_trg_mvadm10_down",tau_trg_mvadm10_down);
        event->Add("wt_tau_trg_mvadm11_down",tau_trg_mvadm11_down);

      }

      event->Add("trigweight_up", trigweight_up);
      event->Add("trigweight_down", trigweight_down);

      // have xtrg OR as default but save others to check 
      event->Add("single_l_sf", xtrg_OR_sf==0 ? single_e_sf : single_e_sf/xtrg_OR_sf );
      event->Add("xtrg_sf", xtrg_OR_sf==0 ? xtrg_et_sf : xtrg_et_sf/xtrg_OR_sf);

      ele_trg = xtrg_OR_sf;
      ele_trg_mc = 1.0;

      tau_trg = 1.0;
      tau_trg_mc = 1.0; 

      xtrg_et_sf_mvadm = xtrg_OR_sf==0 ? xtrg_et_sf_mvadm : xtrg_et_sf_mvadm/xtrg_OR_sf;

     if (trg_applied_in_mc_) {
       ele_trg = ele_trg / ele_trg_mc;
       tau_trg = tau_trg / tau_trg_mc;
     }
     if(ele_trg>2) ele_trg=2;
     weight *= (ele_trg * tau_trg);
     event->Add("trigweight_1", ele_trg);
     event->Add("trigweight_2", xtrg_et_sf_mvadm);
   } else if (channel_ == channel::mt) {
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
     double pt = muon->pt();
     double m_signed_eta = muon->eta();
     double m_iso = PF04IsolationVal(muon, 0.5, 0);
     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double t_pt = tau->pt();
     double t_signed_eta = tau->eta();
     double t_phi = tau->phi();
     double mu_trg = 1.0;
     double tau_trg = 1.0;
     double mu_trg_mc = 1.0;
     double tau_trg_mc = 1.0;
      auto args_1 = std::vector<double>{pt,m_signed_eta,m_iso};  
      mu_trg = fns_["m_trg_binned_data"]->eval(args_1.data());
      if(!is_embedded_) mu_trg_mc = fns_["m_trg_binned_mc"]->eval(args_1.data());
      else mu_trg_mc = fns_["m_trg_binned_embed"]->eval(args_1.data());

      double single_m_sf = mu_trg / mu_trg_mc;
      double t_dm = tau->decay_mode();
      auto args_2 = std::vector<double>{pt,m_signed_eta};  
      auto args_3 = std::vector<double>{t_pt,t_signed_eta,t_phi,t_dm};  
      auto args_4 = std::vector<double>{t_pt,t_dm};
      double t_mvadm = tau->GetTauID(MVADM2017);
      auto args_mvadm = std::vector<double>{t_pt,t_mvadm};

      double m_high_pt_cut = 25;
      if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP) m_high_pt_cut = 23;

      double mu_xtrg = fns_["m_crosstrg_data"]->eval(args_1.data());
      double mu_xtrg_mc;
      if(is_embedded_) mu_xtrg_mc = fns_["m_crosstrg_embed"]->eval(args_2.data());
      else mu_xtrg_mc = fns_["m_crosstrg_mc"]->eval(args_2.data());

      double tau_trg_ic=1., tau_trg_mvadm=1.;
      
      if(is_embedded_) {
        tau_trg_mc = 1.;
        tau_trg = fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
        tau_trg_ic = fns_["t_trg_27_ic_embed_ratio"]->eval(args_4.data());
        tau_trg_mvadm = fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());

      }
      else {
        tau_trg_mc = 1.;
        tau_trg = fns_["t_trg_27_ratio"]->eval(args_4.data());
        tau_trg_ic = fns_["t_trg_27_ic_ratio"]->eval(args_4.data());
        tau_trg_mvadm = fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
      }

      double xtrg_mt_sf = (mu_xtrg_mc*tau_trg_mc) > 0 ? (mu_xtrg*tau_trg)/(mu_xtrg_mc*tau_trg_mc) : 0.0;

      double xtrg_OR_sf = (pt>=m_high_pt_cut) ? single_m_sf : xtrg_mt_sf ; 

      // if trigger is not applied in the MC then set the SF to the efficiency
      if(!trg_applied_in_mc_) {
        xtrg_mt_sf = mu_xtrg*tau_trg;
        xtrg_OR_sf = mu_trg*(1-tau_trg) + mu_xtrg*tau_trg;
        single_m_sf = mu_trg;
      }
      // Tau Leg Scale Factor
      double tau_trg_tt_leg;
      tau_trg_tt_leg = fns_["t_trg_35_ratio"]->eval(args_4.data());
      event->Add("tau_leg_SF", tau_trg_tt_leg);
      std::vector<double> args_mutau = std::vector<double>{pt,m_signed_eta,m_iso,t_pt,t_dm,t_signed_eta};
      std::string extra = ""; 
      if(is_embedded_){
        args_mutau = std::vector<double>{pt,m_signed_eta,m_iso,t_pt,t_dm,t_signed_eta,t_phi}; 
        extra="_embed";
      }

      std::vector<std::string> systs = {"_crosslep_up","_crosslep_down","_singlelep_up","_singlelep_down","_singletau_up","_singletau_down","_dm0_up","_dm0_down","_dm1_up","_dm1_down","_dm10_up","_dm10_down","_dm11_up","_dm11_down"};

      double wt_tau_trg_mssm = fns_["mt_trg"+extra+"_ratio"]->eval(args_mutau.data());
      eventInfo->set_weight("mt_triggerweight_ic", wt_tau_trg_mssm, false);
      event->Add("wt_tau_trg_mssm", xtrg_OR_sf > 0. ? wt_tau_trg_mssm/xtrg_OR_sf : 0.);
      for (auto s: systs) {
        double wt_tau_trg_mssm_syst = fns_["mt_trg"+extra+"_ratio"+s]->eval(args_mutau.data());
        event->Add("wt_tau_trg_mssm"+s, wt_tau_trg_mssm > 0. ? wt_tau_trg_mssm_syst/wt_tau_trg_mssm : 0.);
      }
      event->Add("wt_tau_trg_mssm_doubleonly", 1.);

      tau_trg_ic = (tau_trg==0) ? tau_trg_ic : tau_trg_ic/tau_trg;
      tau_trg_mvadm = (tau_trg==0) ? tau_trg_mvadm : tau_trg_mvadm/tau_trg;
      tau_trg_ic = (pt>=m_high_pt_cut) ? 1. : tau_trg_ic; 
      tau_trg_mvadm = (pt>=m_high_pt_cut) ? 1. : tau_trg_mvadm; 

      event->Add("wt_tau_trg_ic",tau_trg_ic);
      event->Add("wt_tau_trg_mvadm",tau_trg_mvadm);

      double tau_trg_dm0_up=1.;
      double tau_trg_dm1_up=1.;
      double tau_trg_dm10_up=1.;
      double tau_trg_dm11_up=1.;
      double tau_trg_dm0_down=1.;
      double tau_trg_dm1_down=1.;
      double tau_trg_dm10_down=1.;
      double tau_trg_dm11_down=1.;

      double tau_trg_mvadm0_up=1.;
      double tau_trg_mvadm1_up=1.;
      double tau_trg_mvadm2_up=1.;
      double tau_trg_mvadm10_up=1.;
      double tau_trg_mvadm11_up=1.;
      double tau_trg_mvadm0_down=1.;
      double tau_trg_mvadm1_down=1.;
      double tau_trg_mvadm2_down=1.;
      double tau_trg_mvadm10_down=1.;
      double tau_trg_mvadm11_down=1.;

      if(pt<m_high_pt_cut) { // if this isn't true then we are using the single lepton trigger and therefore the weight = 1
        if(!is_embedded_) {
          if(fns_["t_trg_27_ratio"]->eval(args_4.data())>0){
            tau_trg_dm0_up=fns_["t_trg_27_ratio_dm0_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm1_up=fns_["t_trg_27_ratio_dm1_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm10_up=fns_["t_trg_27_ratio_dm10_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm11_up=fns_["t_trg_27_ratio_dm11_up"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm0_down=fns_["t_trg_27_ratio_dm0_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm1_down=fns_["t_trg_27_ratio_dm1_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm10_down=fns_["t_trg_27_ratio_dm10_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
            tau_trg_dm11_down=fns_["t_trg_27_ratio_dm11_down"]->eval(args_4.data())/fns_["t_trg_27_ratio"]->eval(args_4.data());
          }
          if(fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data())>0){
            tau_trg_mvadm0_up   =fns_["t_trg_27_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm1_up   =fns_["t_trg_27_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm2_up   =fns_["t_trg_27_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm10_up  =fns_["t_trg_27_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm11_up  =fns_["t_trg_27_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm0_down =fns_["t_trg_27_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm1_down =fns_["t_trg_27_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm2_down =fns_["t_trg_27_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm10_down=fns_["t_trg_27_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm11_down=fns_["t_trg_27_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_ratio"]->eval(args_mvadm.data());
          }

        } else {
          if(fns_["t_trg_27_embed_ratio"]->eval(args_3.data())>0) {
            tau_trg_dm0_up=fns_["t_trg_27_embed_ratio_dm0_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm1_up=fns_["t_trg_27_embed_ratio_dm1_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm10_up=fns_["t_trg_27_embed_ratio_dm10_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm11_up=fns_["t_trg_27_embed_ratio_dm11_up"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm0_down=fns_["t_trg_27_embed_ratio_dm0_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm1_down=fns_["t_trg_27_embed_ratio_dm1_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm10_down=fns_["t_trg_27_embed_ratio_dm10_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
            tau_trg_dm11_down=fns_["t_trg_27_embed_ratio_dm11_down"]->eval(args_3.data())/fns_["t_trg_27_embed_ratio"]->eval(args_3.data());
          }
          if(fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data())>0){
            tau_trg_mvadm0_up   =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm1_up   =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm2_up   =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm.data())   /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm10_up  =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm11_up  =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm.data())  /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm0_down =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm1_down =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm2_down =fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm.data()) /fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm10_down=fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
            tau_trg_mvadm11_down=fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm.data())/fns_["t_trg_27_ic_mvadm_embed_ratio"]->eval(args_mvadm.data());
          }

        }
      }

      tau_trg_dm0_up = std::isnan(tau_trg_dm0_up) ? 0 : tau_trg_dm0_up;
      tau_trg_dm0_down = std::isnan(tau_trg_dm0_down) ? 0 : tau_trg_dm0_down;
      tau_trg_dm1_up = std::isnan(tau_trg_dm1_up) ? 0 : tau_trg_dm1_up;
      tau_trg_dm1_down = std::isnan(tau_trg_dm1_down) ? 0 : tau_trg_dm1_down;
      tau_trg_dm10_up = std::isnan(tau_trg_dm10_up) ? 0 : tau_trg_dm10_up;
      tau_trg_dm10_down = std::isnan(tau_trg_dm10_down) ? 0 : tau_trg_dm10_down;
      tau_trg_dm11_up = std::isnan(tau_trg_dm11_up) ? 0 : tau_trg_dm11_up;
      tau_trg_dm11_down = std::isnan(tau_trg_dm11_down) ? 0 : tau_trg_dm11_down;

      tau_trg_mvadm0_up    = std::isnan(tau_trg_mvadm0_up)    ? 0 : tau_trg_mvadm0_up;
      tau_trg_mvadm0_down  = std::isnan(tau_trg_mvadm0_down)  ? 0 : tau_trg_mvadm0_down;
      tau_trg_mvadm1_up    = std::isnan(tau_trg_mvadm1_up)    ? 0 : tau_trg_mvadm1_up;
      tau_trg_mvadm1_down  = std::isnan(tau_trg_mvadm1_down)  ? 0 : tau_trg_mvadm1_down;
      tau_trg_mvadm2_up    = std::isnan(tau_trg_mvadm2_up)    ? 0 : tau_trg_mvadm2_up;
      tau_trg_mvadm2_down  = std::isnan(tau_trg_mvadm2_down)  ? 0 : tau_trg_mvadm2_down;
      tau_trg_mvadm10_up   = std::isnan(tau_trg_mvadm10_up)   ? 0 : tau_trg_mvadm10_up;
      tau_trg_mvadm10_down = std::isnan(tau_trg_mvadm10_down) ? 0 : tau_trg_mvadm10_down;
      tau_trg_mvadm11_up   = std::isnan(tau_trg_mvadm11_up)   ? 0 : tau_trg_mvadm11_up;
      tau_trg_mvadm11_down = std::isnan(tau_trg_mvadm11_down) ? 0 : tau_trg_mvadm11_down;

      event->Add("wt_tau_trg_dm0_up",tau_trg_dm0_up);
      event->Add("wt_tau_trg_dm1_up",tau_trg_dm1_up);
      event->Add("wt_tau_trg_dm10_up",tau_trg_dm10_up);
      event->Add("wt_tau_trg_dm11_up",tau_trg_dm11_up);
      event->Add("wt_tau_trg_dm0_down",tau_trg_dm0_down);
      event->Add("wt_tau_trg_dm1_down",tau_trg_dm1_down);
      event->Add("wt_tau_trg_dm10_down",tau_trg_dm10_down);
      event->Add("wt_tau_trg_dm11_down",tau_trg_dm11_down);

      event->Add("wt_tau_trg_mvadm0_up"   ,tau_trg_mvadm0_up);
      event->Add("wt_tau_trg_mvadm1_up"   ,tau_trg_mvadm1_up);
      event->Add("wt_tau_trg_mvadm2_up"   ,tau_trg_mvadm2_up);
      event->Add("wt_tau_trg_mvadm10_up"  ,tau_trg_mvadm10_up);
      event->Add("wt_tau_trg_mvadm11_up"  ,tau_trg_mvadm11_up);
      event->Add("wt_tau_trg_mvadm0_down" ,tau_trg_mvadm0_down);
      event->Add("wt_tau_trg_mvadm1_down" ,tau_trg_mvadm1_down);
      event->Add("wt_tau_trg_mvadm2_down" ,tau_trg_mvadm2_down);
      event->Add("wt_tau_trg_mvadm10_down",tau_trg_mvadm10_down);
      event->Add("wt_tau_trg_mvadm11_down",tau_trg_mvadm11_down);


      // have xtrg OR as default but save others to check 
      event->Add("single_l_sf", xtrg_OR_sf==0 ? single_m_sf : single_m_sf/xtrg_OR_sf);
      event->Add("xtrg_sf", xtrg_OR_sf==0 ? xtrg_mt_sf : xtrg_mt_sf/xtrg_OR_sf);

      mu_trg = xtrg_OR_sf;
      mu_trg_mc = 1.0;

      tau_trg = 1.0;
      tau_trg_mc = 1.0; 

     if (trg_applied_in_mc_) {
       mu_trg = mu_trg / mu_trg_mc;
       tau_trg = tau_trg / tau_trg_mc;
     }
     if(mu_trg>2) mu_trg=2;
     weight *= (mu_trg * tau_trg);
     event->Add("trigweight_1", mu_trg);
     event->Add("trigweight_2", tau_trg);
   } else if (channel_ == channel::em) {
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
     double e_pt = elec->pt();
     double e_eta = fabs(elec->eta());
     double m_pt = muon->pt();
     double m_eta = fabs(muon->eta());
     double m_iso = PF04IsolationVal(muon, 0.5, 0);
     double m_trg_23 = 1.0;
     double m_trg_23_mc = 1.0;
     double m_trg_8 = 1.0;
     double m_trg_8_mc = 1.0;
     double e_trg_23 = 1.0;
     double e_trg_23_mc = 1.0;
     double e_trg_12 = 1.0;
     double e_trg_12_mc = 1.0;
     double e_trg=1.;
     double e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho

     auto args_1_2 = std::vector<double>{e_pt,e_eta,e_iso};
     auto args_2_2 = std::vector<double>{m_pt,m_eta,m_iso};

     m_trg_23 = fns_["m_trg_binned_23_data"]->eval(args_2_2.data());
     m_trg_8  = fns_["m_trg_binned_8_data"]->eval(args_2_2.data());
     e_trg_23 = fns_["e_trg_binned_23_data"]->eval(args_1_2.data());
     e_trg_12 = fns_["e_trg_binned_12_data"]->eval(args_1_2.data());

     if(is_embedded_){
       m_trg_23_mc = fns_["m_trg_binned_23_embed"]->eval(args_2_2.data());
       m_trg_8_mc  = fns_["m_trg_binned_8_embed"]->eval(args_2_2.data());
       e_trg_23_mc = fns_["e_trg_binned_23_embed"]->eval(args_1_2.data());
       e_trg_12_mc = fns_["e_trg_binned_12_embed"]->eval(args_1_2.data());
     } else {
       m_trg_23_mc = fns_["m_trg_binned_23_mc"]->eval(args_2_2.data());
       m_trg_8_mc  = fns_["m_trg_binned_8_mc"]->eval(args_2_2.data());
       e_trg_23_mc = fns_["e_trg_binned_23_mc"]->eval(args_1_2.data());
       e_trg_12_mc = fns_["e_trg_binned_12_mc"]->eval(args_1_2.data());
     }
     if (trg_applied_in_mc_){
       e_trg = (m_trg_23_mc*e_trg_12_mc + m_trg_8_mc*e_trg_23_mc - m_trg_23_mc*e_trg_23_mc) > 0. ? (m_trg_23*e_trg_12 + m_trg_8*e_trg_23 - m_trg_23*e_trg_23)/(m_trg_23_mc*e_trg_12_mc + m_trg_8_mc*e_trg_23_mc - m_trg_23_mc*e_trg_23_mc) : 0.;
       // if one of the leptons has pT < 24 then we aren't using the OR of the 2 triggrs anymore for the SF logic is different
       if(e_pt<24.) e_trg = m_trg_23_mc*e_trg_12_mc > 0. ? (m_trg_23*e_trg_12)/(m_trg_23_mc*e_trg_12_mc) :  0.;
       if(m_pt<24.) e_trg = m_trg_8_mc*e_trg_23_mc > 0. ? (m_trg_8*e_trg_23)/(m_trg_8_mc*e_trg_23_mc) : 0.;
     } else {
       e_trg = (m_trg_23*e_trg_12 + m_trg_8*e_trg_23 - m_trg_23*e_trg_23);
       if(e_pt<24.) e_trg = m_trg_23*e_trg_12;
       if(m_pt<24.) e_trg = m_trg_8*e_trg_23;
     }

     if(era_ == era::data_2017 || era_ == era::data_2017UL) e_trg*=0.991;
     if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP)  eventInfo->set_weight("dz_filter_eff",double(0.979));
     if(e_trg>2.) e_trg=2.;
     weight *= (e_trg);
     event->Add("trigweight_1", e_trg);
     event->Add("trigweight_2", double(1.0));
   } else if (channel_ == channel::tt){
     Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
     Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double pt_1 = tau1->pt();
     double pt_2 = tau2->pt();
     double tau1_trg = 1.0;
     double tau1_trg_mc = 1.0;
     double tau1_trg_up = 1.0;
     double tau1_trg_down = 1.0;
     double tau2_trg = 1.0;
     double tau2_trg_mc = 1.0;
     double tau2_trg_up = 1.0;
     double tau2_trg_down = 1.0;
      double eta_1 = tau1->eta();
      double eta_2 = tau2->eta();  
      double phi_1 = tau1->phi();
      double phi_2 = tau2->phi();
      double dm_1 = tau1->decay_mode();
      double dm_2 = tau2->decay_mode();
      // fix for using DM10 SFs for all new DM modes too
      auto args_1 = std::vector<double>{pt_1,eta_1,phi_1,dm_1};  
      auto args_2 = std::vector<double>{pt_2,eta_2,phi_2,dm_2};

      auto args_2_1 = std::vector<double>{pt_1,dm_1};
      auto args_2_2 = std::vector<double>{pt_2,dm_2};

      double mvadm_1 = tau1->GetTauID(MVADM2017);
      double mvadm_2 = tau2->GetTauID(MVADM2017);

      auto args_mvadm_1 = std::vector<double>{pt_1,mvadm_1};
      auto args_mvadm_2 = std::vector<double>{pt_2,mvadm_2};

      double tau1_trg_ic=1., tau2_trg_ic=1., tau1_trg_ic_mvadm=1., tau2_trg_ic_mvadm=1.;

      if(is_embedded_){
        tau1_trg_mc = 1.;
        tau2_trg_mc = 1.;
        tau1_trg = fns_["t_trg_35_embed_ratio"]->eval(args_1.data());
        tau2_trg = fns_["t_trg_35_embed_ratio"]->eval(args_2.data());

        tau1_trg_ic = fns_["t_trg_35_ic_embed_ratio"]->eval(args_2_1.data());
        tau2_trg_ic = fns_["t_trg_35_ic_embed_ratio"]->eval(args_2_2.data());
        tau1_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_embed_ratio"]->eval(args_mvadm_1.data());
        tau2_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_embed_ratio"]->eval(args_mvadm_2.data());
      } else{
        tau1_trg = fns_["t_trg_35_ratio"]->eval(args_2_1.data());
        tau1_trg_mc = 1.;
        tau2_trg = fns_["t_trg_35_ratio"]->eval(args_2_2.data());
        tau2_trg_mc = 1.;

        tau1_trg_ic = fns_["t_trg_35_ic_ratio"]->eval(args_2_1.data());
        tau2_trg_ic = fns_["t_trg_35_ic_ratio"]->eval(args_2_2.data());
        tau1_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_ratio"]->eval(args_mvadm_1.data());
        tau2_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_ratio"]->eval(args_mvadm_2.data());
      }

      double tau_trg_dm0_up=1;
      double tau_trg_dm1_up=1;
      double tau_trg_dm10_up=1;
      double tau_trg_dm11_up=1;
      double tau_trg_dm0_down=1;
      double tau_trg_dm1_down=1;
      double tau_trg_dm10_down=1;
      double tau_trg_dm11_down=1;

      double tau_trg_mvadm0_up=1;
      double tau_trg_mvadm1_up=1;
      double tau_trg_mvadm2_up=1;
      double tau_trg_mvadm10_up=1;
      double tau_trg_mvadm11_up=1;
      double tau_trg_mvadm0_down=1;
      double tau_trg_mvadm1_down=1;
      double tau_trg_mvadm2_down=1;
      double tau_trg_mvadm10_down=1;
      double tau_trg_mvadm11_down=1;

      double trg_tot = (tau1_trg*tau2_trg)/(tau1_trg_mc*tau2_trg_mc);


      double tau_trg_ic = tau1_trg_ic*tau2_trg_ic;
      double tau_trg_ic_mvadm = tau1_trg_ic_mvadm*tau2_trg_ic_mvadm;

      bool do_wts = !(tau1_trg==0 || tau2_trg==0 || tau1_trg_mc==0 || tau2_trg_mc==0);

      if(is_embedded_ && do_wts){
        tau_trg_dm0_up = fns_["t_trg_35_embed_ratio_dm0_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm0_up"]->eval(args_2.data())/trg_tot;
        tau_trg_dm1_up = fns_["t_trg_35_embed_ratio_dm1_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm1_up"]->eval(args_2.data())/trg_tot;
        tau_trg_dm10_up = fns_["t_trg_35_embed_ratio_dm10_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm10_up"]->eval(args_2.data())/trg_tot;
        tau_trg_dm11_up = fns_["t_trg_35_embed_ratio_dm11_up"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm11_up"]->eval(args_2.data())/trg_tot;
        tau_trg_dm0_down = fns_["t_trg_35_embed_ratio_dm0_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm0_down"]->eval(args_2.data())/trg_tot;
        tau_trg_dm1_down = fns_["t_trg_35_embed_ratio_dm1_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm1_down"]->eval(args_2.data())/trg_tot;
        tau_trg_dm10_down = fns_["t_trg_35_embed_ratio_dm10_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm10_down"]->eval(args_2.data())/trg_tot;
        tau_trg_dm11_down = fns_["t_trg_35_embed_ratio_dm11_down"]->eval(args_1.data())*fns_["t_trg_35_embed_ratio_dm11_down"]->eval(args_2.data())/trg_tot;

        tau_trg_mvadm0_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm1_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm2_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm10_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm11_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;

        tau_trg_mvadm0_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm1_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm2_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm10_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm11_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
      } else if(do_wts) {
        tau_trg_dm0_up = fns_["t_trg_35_ratio_dm0_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm0_up"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm1_up = fns_["t_trg_35_ratio_dm1_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm1_up"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm10_up = fns_["t_trg_35_ratio_dm10_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm10_up"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm11_up = fns_["t_trg_35_ratio_dm11_up"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm11_up"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm0_down = fns_["t_trg_35_ratio_dm0_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm0_down"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm1_down = fns_["t_trg_35_ratio_dm1_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm1_down"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm10_down = fns_["t_trg_35_ratio_dm10_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm10_down"]->eval(args_2_2.data())/trg_tot;
        tau_trg_dm11_down = fns_["t_trg_35_ratio_dm11_down"]->eval(args_2_1.data())*fns_["t_trg_35_ratio_dm11_down"]->eval(args_2_2.data())/trg_tot;

        tau_trg_mvadm0_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm0_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm1_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm1_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm2_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm2_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm10_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm10_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm11_up = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm11_up"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;

        tau_trg_mvadm0_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm0_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm1_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm1_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm2_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm2_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm10_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm10_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;
        tau_trg_mvadm11_down = (tau_trg_ic_mvadm>0) ? fns_["t_trg_35_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm_1.data())*fns_["t_trg_35_ic_mvadm_ratio_mvadm11_down"]->eval(args_mvadm_2.data())/tau_trg_ic_mvadm : 0.;

      }

      tau_trg_dm0_up = std::isnan(tau_trg_dm0_up) ? 0 : tau_trg_dm0_up;
      tau_trg_dm0_down = std::isnan(tau_trg_dm0_down) ? 0: tau_trg_dm0_down;
      tau_trg_dm1_up = std::isnan(tau_trg_dm1_up) ? 0 : tau_trg_dm1_up;
      tau_trg_dm1_down = std::isnan(tau_trg_dm1_down) ? 0: tau_trg_dm1_down;
      tau_trg_dm10_up = std::isnan(tau_trg_dm10_up) ? 0 : tau_trg_dm10_up;
      tau_trg_dm10_down = std::isnan(tau_trg_dm10_down) ? 0: tau_trg_dm10_down;
      tau_trg_dm11_up = std::isnan(tau_trg_dm11_up) ? 0 : tau_trg_dm11_up;
      tau_trg_dm11_down = std::isnan(tau_trg_dm11_down) ? 0 : tau_trg_dm11_down;

      event->Add("wt_tau_trg_dm0_up",tau_trg_dm0_up);
      event->Add("wt_tau_trg_dm1_up",tau_trg_dm1_up);
      event->Add("wt_tau_trg_dm10_up",tau_trg_dm10_up);
      event->Add("wt_tau_trg_dm11_up",tau_trg_dm11_up);
      event->Add("wt_tau_trg_dm0_down",tau_trg_dm0_down);
      event->Add("wt_tau_trg_dm1_down",tau_trg_dm1_down);
      event->Add("wt_tau_trg_dm10_down",tau_trg_dm10_down);
      event->Add("wt_tau_trg_dm11_down",tau_trg_dm11_down);

      event->Add("wt_tau_trg_mvadm0_up",   tau_trg_mvadm0_up);
      event->Add("wt_tau_trg_mvadm1_up",   tau_trg_mvadm1_up);
      event->Add("wt_tau_trg_mvadm2_up",   tau_trg_mvadm2_up);
      event->Add("wt_tau_trg_mvadm10_up",  tau_trg_mvadm10_up);
      event->Add("wt_tau_trg_mvadm11_up",  tau_trg_mvadm11_up);
      event->Add("wt_tau_trg_mvadm0_down", tau_trg_mvadm0_down);
      event->Add("wt_tau_trg_mvadm1_down", tau_trg_mvadm1_down);
      event->Add("wt_tau_trg_mvadm2_down", tau_trg_mvadm2_down);
      event->Add("wt_tau_trg_mvadm10_down",tau_trg_mvadm10_down);
      event->Add("wt_tau_trg_mvadm11_down",tau_trg_mvadm11_down);

      tau_trg_ic = (trg_tot==0) ? tau_trg_ic : tau_trg_ic/trg_tot;
      tau_trg_ic_mvadm = (trg_tot==0) ? tau_trg_ic_mvadm : tau_trg_ic_mvadm/trg_tot;

      event->Add("wt_tau_trg_ic",tau_trg_ic);
      event->Add("wt_tau_trg_mvadm",tau_trg_ic_mvadm);

      auto args_2d = std::vector<double>{pt_1,dm_1,pt_2,dm_2};

      double tau_trg_mssm_double=1., tau_trg_mssm=1.;
      if(is_embedded_) {
        tau_trg_mssm_double = fns_["t_trg_2d_doubleonly_embed_ratio"]->eval(args_2d.data()); 
        tau_trg_mssm        = fns_["t_trg_2d_embed_ratio"]->eval(args_2d.data()); 
      } else {
        tau_trg_mssm_double = fns_["t_trg_2d_doubleonly_ratio"]->eval(args_2d.data()); 
        tau_trg_mssm        = fns_["t_trg_2d_ratio"]->eval(args_2d.data()); 
      }

      tau_trg_mssm_double = (trg_tot==0) ? tau_trg_mssm_double : tau_trg_mssm_double/trg_tot;

      std::vector<std::string> systs = {"_singletau_up","_singletau_down","_lowpt_dm0_up","_lowpt_dm0_down","_lowpt_dm1_up","_lowpt_dm1_down","_lowpt_dm10_up","_lowpt_dm10_down","_lowpt_dm11_up","_lowpt_dm11_down",
    "_highpt_dm0_up", "_highpt_dm0_down","_highpt_dm1_up","_highpt_dm1_down","_highpt_dm10_up","_highpt_dm10_down","_highpt_dm11_up","_highpt_dm11_down"
      };

      std::string extra = "";
      if(is_embedded_){
        extra="_embed";
      }

      for (auto s: systs) {
        double wt_tau_trg_mssm_syst = fns_["t_trg_2d"+extra+"_ratio"+s]->eval(args_2d.data());
        event->Add("wt_tau_trg_mssm"+s, tau_trg_mssm > 0. ? wt_tau_trg_mssm_syst/tau_trg_mssm : 0.);
      }

      event->Add("wt_tau_trg_mssm_doubleonly",tau_trg_mssm_double);
      event->Add("wt_tau_trg_mssm", (trg_tot==0) ? tau_trg_mssm : tau_trg_mssm/trg_tot);
 
     if(trg_applied_in_mc_){
       tau1_trg = tau1_trg / tau1_trg_mc;
       tau2_trg = tau2_trg / tau2_trg_mc;
     }
     weight *= (tau1_trg*tau2_trg);
     event->Add("trigweight_1", tau1_trg);
     event->Add("trigweight_2", tau2_trg);
     event->Add("trigweight_up_1", tau1_trg_up);
     event->Add("trigweight_up_2", tau2_trg_up);
     event->Add("trigweight_down_1", tau1_trg_down);
     event->Add("trigweight_down_2", tau2_trg_down);
   } else if (channel_ == channel::zee) {
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     double e1_pt = elec->pt();
     double e1_eta = fabs(elec->sc_eta());
     double e_iso_1 = PF03EAIsolationVal(elec, eventInfo->jet_rho());
     double ele1_trg = 1.0;
     double ele2_trg = 1.0;
     double ele1_trg_mc = 1.0;
     double ele2_trg_mc = 1.0;
     auto args_1 = std::vector<double>{e1_pt,e1_eta,e_iso_1};
     ele1_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
     if(is_embedded_) ele1_trg_mc = fns_["e_trg_binned_embed"]->eval(args_1.data());
     else ele1_trg_mc = fns_["e_trg_binned_mc"]->eval(args_1.data());
     ele2_trg = 1.0;
     ele2_trg_mc = 1.0;
     
    // cout << "ele1_trg: " << ele1_trg << endl;
    // cout << "ele1_trg_mc: " << ele1_trg_mc << endl;
    // cout << "e1_pt: " << e1_pt << endl;
    // cout << "e1_eta: " << e1_eta << endl;
    // cout << "e_iso_1: " << e_iso_1 << endl;

     if (trg_applied_in_mc_) {
		ele1_trg = ele1_trg / ele1_trg_mc;
		ele2_trg = ele2_trg / ele2_trg_mc;
		if(ele1_trg_mc == 0){
			 ele1_trg = 0;
		 }
		if(ele2_trg_mc ==0){
			 ele2_trg_mc =0;
		 }
			
     }
     weight *= (ele1_trg * ele2_trg);
     event->Add("trigweight_1", ele1_trg);
     event->Add("trigweight_2", ele2_trg);
   } else if (channel_ == channel::zmm) {
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
     double pt1 = muon->pt();
     double m1_signed_eta = muon->eta();
     double m_iso_1 = PF04IsolationVal(muon, 0.5, 0);
     double mu1_trg = 1.0;
     double mu2_trg = 1.0;
     double mu1_trg_mc = 1.0;
     double mu2_trg_mc = 1.0;
     auto args_1 = std::vector<double>{pt1,m1_signed_eta,m_iso_1};  
     auto args_2 = std::vector<double>{pt1,m1_signed_eta};
     mu1_trg = fns_["m_trg_binned_data"]->eval(args_1.data());
     if (is_embedded_) {
       mu1_trg_mc = fns_["m_trg_binned_embed"]->eval(args_1.data());
     } else {
       mu1_trg_mc = fns_["m_trg_binned_mc"]->eval(args_1.data());
     }
     mu2_trg = 1.0;
     mu2_trg_mc = 1.0;

     if (trg_applied_in_mc_) {
       mu1_trg = mu1_trg / mu1_trg_mc;
       mu2_trg = mu2_trg / mu2_trg_mc;
     }
     weight *= (mu1_trg * mu2_trg);
     event->Add("trigweight_1", mu1_trg);
     event->Add("trigweight_2", mu2_trg);
   }
 }
 return 0;
}

  int HTTWeights::PostAnalysis() {
    return 0;
  }

  void HTTWeights::PrintInfo() {
    ;
  }

}
