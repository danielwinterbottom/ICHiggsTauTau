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
    do_tracking_eff_            = false;
    do_topquark_weights_        = false;
    do_tau_fake_weights_        = false;
    do_em_qcd_weights_          = false;
    gen_tau_collection_         = "genParticlesTaus";
    jets_label_                 = "pfJetsPFlow";
    ditau_label_                = "emtauCandidates";
    scalefactor_file_           = "";
    scalefactor_file_ggh_       = "";
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
  std::cout << boost::format(param_fmt()) % "do_topquark_weights" % do_topquark_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_fake_weights" % do_tau_fake_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_id_weights"   % do_tau_id_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_id_sf"        % do_tau_id_sf_;
  std::cout << boost::format(param_fmt()) % "do_em_qcd_weights"   % do_em_qcd_weights_;
  std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
  std::cout << boost::format(param_fmt()) % "ditau_label"         % ditau_label_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file"    % scalefactor_file_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file_ggh" % scalefactor_file_ggh_;

  std::string MVADM2017 = " ";

  if(era_ == era::data_2016 || era_ == era::data_2017 || era_ == era::data_2018) std::string MVADM2017 = "MVADM2017v1";

  if(era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP || era_ == era::data_2017UL || era_ == era::data_2018UL) std::string MVADM2017 = "MVADM2017v2";

  if (do_nnlops_weights_){
    // Retrieve file with TGraphs of weights
    std::string file = "input/ggh_weights/NNLOPS_reweight.root";
    ggh_weights_ = new TFile(file.c_str());
    ggh_weights_->cd();
    ggh_mg_0jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_0jet");
    ggh_mg_1jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_1jet");
    ggh_mg_2jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_2jet");
    ggh_mg_3jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_mcatnlo_3jet");

    ggh_ph_0jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_0jet");
    ggh_ph_1jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_1jet");
    ggh_ph_2jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_2jet");
    ggh_ph_3jet_ = (TGraph*)gDirectory->Get("gr_NNLOPSratio_pt_powheg_3jet");

    ggh_weights_->Close();
 }

  if(scalefactor_file_!="") {
    // putting new definitions of all function here for full run2 analyses to avoid the mess/confusion 
    // for now just for 2018 but will update for 2017 and 2016 legacy soon
    TFile f(scalefactor_file_.c_str());
    w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
    f.Close();

    if (scalefactor_file_ggh_ != "") {
      TFile f_ggh(scalefactor_file_ggh_.c_str());
      w_ggh_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
      f_ggh.Close();
    }

    // tracking corrections for electrons and muons
    fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["e_trk_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trk_embed_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));


    // triggers for muon legs in mt and zmm channels
    
    fns_["m_trg_binned_mc"] = std::shared_ptr<RooFunctor>(  w_->function("m_trg_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_trg_binned_embed"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    if(mc_ == mc::mcleg2016) {
      fns_["m_crosstrg_data"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_19_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_crosstrg_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_19_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_crosstrg_embed"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_19_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    }
    if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {
      fns_["m_crosstrg_data"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_20_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_crosstrg_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_20_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_crosstrg_embed"] = std::shared_ptr<RooFunctor>(
         w_->function("m_trg_20_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    }

    fns_["e_trg_binned_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["e_trg_binned_embed"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {
      fns_["e_crosstrg_mc"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_24_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_crosstrg_data"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_24_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
      fns_["e_crosstrg_embed"] = std::shared_ptr<RooFunctor>(
         w_->function("e_trg_24_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    }

    // triggers for electron and muon legs in em channel 
    fns_["e_trg_binned_12_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_12_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["e_trg_binned_12_data"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_12_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
     fns_["e_trg_binned_12_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_12_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["e_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_23_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["e_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_23_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
     fns_["e_trg_binned_23_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_23_binned_ic_embed")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["m_trg_binned_8_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_8_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_trg_binned_8_data"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_8_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
     fns_["m_trg_binned_8_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_8_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_trg_binned_23_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_23_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_trg_binned_23_data"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_23_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
     fns_["m_trg_binned_23_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_23_binned_ic_embed")->functor(w_->argSet("m_pt,m_eta,m_iso")));

    fns_["t_trg_30_embed_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
 
    // triggers for tau legs in et, mt and tt channels
    if(era_ != era::data_2018 || era_ != era::data_2018UL) {
      fns_["t_trg_30_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_data")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_mc"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_mc")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    } else { 
      fns_["t_trg_30_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_data")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_mc"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_mc")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    }


    fns_["t_trg_27_embed_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_data")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_data")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_mc")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));


    fns_["t_trg_35_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

    fns_["t_trg_27_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

    fns_["t_trg_27_embed_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm0_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm1_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm10_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm11_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm0_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm1_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm10_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_27_embed_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_mutau_embed_ratio_dm11_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

    fns_["t_trg_30_embed_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm0_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm1_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm10_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm11_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm0_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm1_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm10_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_30_embed_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_etau_embed_ratio_dm11_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

    if(era_ != era::data_2018 || era_ != era::data_2018UL) {

      fns_["t_trg_30_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_pog_deeptau_medium_etau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

    } else {

      fns_["t_trg_30_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));
    }

    fns_["t_trg_35_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm0_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm1_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm10_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm11_up")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm0_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm1_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm10_down")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio_dm11_down")->functor(w_->argSet("t_pt,t_dm")));

    fns_["t_trg_35_embed_ratio_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm0_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm1_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm10_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm11_up")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm0_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm1_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm10_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));
    fns_["t_trg_35_embed_ratio_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_mediumDeepTau_ditau_embed_ratio_dm11_down")->functor(w_->argSet("t_pt,t_eta,t_phi,t_dm")));

    // trigger SF for MSSM 

    // OR trigger functions for lt channels
    std::vector<std::string> systs = {"","_crosslep_up","_crosslep_down","_singlelep_up","_singlelep_down","_singletau_up","_singletau_down",
    "_dm0_up","_dm0_down","_dm1_up","_dm1_down","_dm10_up","_dm10_down","_dm11_up","_dm11_down"
    };

    for (auto s: systs) {
      fns_["mt_trg_ratio"+s] = std::shared_ptr<RooFunctor>(w_->function(("mt_trg_ratio"+s).c_str())->functor(w_->argSet("m_pt,m_eta,m_iso,t_pt,t_dm,t_eta")));
      fns_["mt_trg_embed_ratio"+s] = std::shared_ptr<RooFunctor>(w_->function(("mt_trg_embed_ratio"+s).c_str())->functor(w_->argSet("m_pt,m_eta,m_iso,t_pt,t_dm,t_eta,t_phi")));
      
      fns_["et_trg_ratio"+s] = std::shared_ptr<RooFunctor>(w_->function(("et_trg_ratio"+s).c_str())->functor(w_->argSet("e_pt,e_eta,e_iso,t_pt,t_dm,t_eta")));
      fns_["et_trg_embed_ratio"+s] = std::shared_ptr<RooFunctor>(w_->function(("et_trg_embed_ratio"+s).c_str())->functor(w_->argSet("e_pt,e_eta,e_iso,t_pt,t_dm,t_eta,t_phi")));
    }

    systs = {"","_singletau_up","_singletau_down","_lowpt_dm0_up","_lowpt_dm0_down","_lowpt_dm1_up","_lowpt_dm1_down","_lowpt_dm10_up","_lowpt_dm10_down","_lowpt_dm11_up","_lowpt_dm11_down",
    "_highpt_dm0_up", "_highpt_dm0_down","_highpt_dm1_up","_highpt_dm1_down","_highpt_dm10_up","_highpt_dm10_down","_highpt_dm11_up","_highpt_dm11_down"
    };
    for (auto s : systs){
      fns_["t_trg_2d_ratio"+s] = std::shared_ptr<RooFunctor>(
          w_->function(("t_trg_2d_ratio"+s).c_str())->functor(w_->argSet("t_pt,t_dm,t_pt_2,t_dm_2")));
      fns_["t_trg_2d_embed_ratio"+s] = std::shared_ptr<RooFunctor>(
          w_->function(("t_trg_2d_embed_ratio"+s).c_str())->functor(w_->argSet("t_pt,t_dm,t_pt_2,t_dm_2")));
    }

    fns_["t_trg_2d_doubleonly_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_2d_doubleonly_ratio")->functor(w_->argSet("t_pt,t_dm,t_pt_2,t_dm_2")));
    fns_["t_trg_2d_doubleonly_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_2d_doubleonly_embed_ratio")->functor(w_->argSet("t_pt,t_dm,t_pt_2,t_dm_2")));

    fns_["t_trg_single_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_single_data")->functor(w_->argSet("t_pt")));
    fns_["t_trg_single_data_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_single_data_up")->functor(w_->argSet("t_pt")));
    fns_["t_trg_single_data_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_single_data_down")->functor(w_->argSet("t_pt")));
    fns_["t_trg_single_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_single_mc")->functor(w_->argSet("t_pt")));
    fns_["t_trg_single_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_single_embed")->functor(w_->argSet("t_pt")));
    ////////

    // Mohammads trigger SFs
    //t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm1_down

    fns_["t_trg_27_ic_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mutau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_27_ic_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mutau_embed_ratio")->functor(w_->argSet("t_pt,t_dm")));

    fns_["t_trg_27_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_27_ic_mvadm_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_27_ic_mvadm_embed_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_mutau_embed_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

    if(mc_ != mc::mcleg2016) {
      fns_["t_trg_30_ic_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_ratio")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ic_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_embed_ratio")->functor(w_->argSet("t_pt,t_dm")));

      fns_["t_trg_30_ic_embed_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_etau_embed_data")->functor(w_->argSet("t_pt,t_dm")));
      fns_["t_trg_30_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_data"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_data")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_mc"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_mc")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_30_ic_mvadm_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
      fns_["t_trg_30_ic_mvadm_embed_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
          w_->function("t_trg_ic_deeptau_medium_mvadm_etau_embed_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
    }

    fns_["t_trg_35_ic_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ic_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_ditau_embed_ratio")->functor(w_->argSet("t_pt,t_dm")));

    fns_["t_trg_35_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_35_ic_mvadm_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_35_ic_mvadm_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_35_ic_mvadm_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_35_ic_mvadm_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_trg_35_ic_mvadm_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_trg_35_ic_mvadm_embed_ratio_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_embed_ratio_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

    // electron id/iso
    fns_["e_idiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_binned_ic_embed_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
    fns_["e_idiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_binned_ic_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso"))); 

    // muon id/iso
    fns_["m_idiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_idiso_binned_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
     fns_["m_idiso_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_idiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_id_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_looseiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_looseiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_looseiso_embed_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_looseiso_binned_ic_embed_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_sel_idEmb_ratio"] = std::shared_ptr<RooFunctor>(
         w_->function("m_sel_id_ic_ratio")->functor(w_->argSet("gt_eta,gt_pt")));
    fns_["m_sel_trg_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_sel_trg_ic_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));
    fns_["m_sel_idEmb_kit_ratio"] = std::shared_ptr<RooFunctor>(
         w_->function("m_sel_idEmb_ratio")->functor(w_->argSet("gt_eta,gt_pt")));
    fns_["m_sel_trg_kit_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_sel_trg_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));

    // tau id
    fns_["t_id_pt_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_pt_tight")->functor(w_->argSet("t_pt")));
    fns_["t_id_dm_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_dm_tight")->functor(w_->argSet("t_dm")));
    fns_["t_id_dm_vloose"] = std::shared_ptr<RooFunctor>(
      w_->function("t_id_dm_vloose")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_pt_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium")->functor(w_->argSet("t_pt")));

    // high pT tau ID efficiency for MSSM analysis
    fns_["t_deeptauid_highpt"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_bin5_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_bin5_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_bin5_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_bin5_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_bin6_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_bin6_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_bin6_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_bin6_down")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_highpt_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_embed")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_embed_bin5_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_embed_bin5_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_embed_bin5_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_embed_bin5_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_embed_bin6_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_embed_bin6_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_embed_bin6_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_embed_bin6_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_tightvse_embed"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_tightvse_embed")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_tightvse_embed_bin5_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_tightvse_embed_bin5_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_tightvse_embed_bin5_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_tightvse_embed_bin5_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_tightvse_embed_bin6_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_tightvse_embed_bin6_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_highpt_tightvse_embed_bin6_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_highpt_tightvse_embed_bin6_down")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_pt_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_vvvloose")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_dm_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_vvvloose")->functor(w_->argSet("t_dm")));


    fns_["t_deeptauid_pt_medium_bin1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin1_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin2_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin3_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin3_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin4_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin4_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin5_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin5_up")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_pt_medium_bin1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin1_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin2_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin3_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin3_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin4_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin4_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_medium_bin5_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium_bin5_down")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_dm_medium_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm0_up")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm1_up")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm10_up")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm11_up")->functor(w_->argSet("t_dm")));

    fns_["t_deeptauid_dm_medium_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm0_down")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm1_down")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm10_down")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium_dm11_down")->functor(w_->argSet("t_dm")));

    fns_["t_deeptauid_pt_embed_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_vvvloose")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_dm_embed_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_vvvloose")->functor(w_->argSet("t_dm")));

    fns_["t_deeptauid_pt_tightvse_embed_medium_bin1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin1_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin2_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin3_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin3_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin4_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin4_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin5_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin5_up")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_pt_tightvse_embed_medium_bin1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin1_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin2_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin3_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin3_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin4_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin4_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_tightvse_embed_medium_bin5_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_tightvse_embed_medium_bin5_down")->functor(w_->argSet("t_pt")));


    fns_["t_deeptauid_pt_embed_medium_bin1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin1_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin2_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin3_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin3_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin4_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin4_up")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin5_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin5_up")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_pt_embed_medium_bin1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin1_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin2_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin3_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin3_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin4_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin4_down")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_pt_embed_medium_bin5_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_embed_medium_bin5_down")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_dm_embed_medium_dm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm0_up")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_medium_dm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm1_up")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_medium_dm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm10_up")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_medium_dm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm11_up")->functor(w_->argSet("t_dm")));

    fns_["t_deeptauid_dm_embed_medium_dm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm0_down")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_medium_dm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm1_down")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_medium_dm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm10_down")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_embed_medium_dm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_embed_medium_dm11_down")->functor(w_->argSet("t_dm")));

    // tau id in mva-dm bins
    fns_["t_deeptauid_mvadm_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_deeptauid_mvadm_medium_tightvsele"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_tightvsele")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_deeptauid_mvadm_embed_medium_tightvsele"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_tightvsele")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_lowpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_highpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down")->functor(w_->argSet("t_pt,t_mvadm")));

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

    // zpt reweighting
    fns_["zpt_weight_nom"] = std::shared_ptr<RooFunctor>(
        w_->function("zptmass_weight_nom")->functor(w_->argSet("z_gen_pt,z_gen_mass")));

    // em osss same names for all years
    fns_["em_qcd_osss"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_extrap_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_extrap_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_extrap_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_extrap_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

    fns_["em_qcd_osss_stat_0jet_unc1_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_0jet_unc1_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_0jet_unc1_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_0jet_unc1_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_0jet_unc2_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_0jet_unc2_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_0jet_unc2_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_0jet_unc2_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

    fns_["em_qcd_osss_stat_1jet_unc1_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_1jet_unc1_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_1jet_unc1_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_1jet_unc1_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_1jet_unc2_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_1jet_unc2_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_1jet_unc2_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_1jet_unc2_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

    fns_["em_qcd_osss_stat_2jet_unc1_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_2jet_unc1_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_2jet_unc1_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_2jet_unc1_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_2jet_unc2_up"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_2jet_unc2_up")->functor(w_->argSet("dR,njets,e_pt,m_pt")));
    fns_["em_qcd_osss_stat_2jet_unc2_down"] = std::shared_ptr<RooFunctor>(
       w_->function("em_qcd_osss_stat_2jet_unc2_down")->functor(w_->argSet("dR,njets,e_pt,m_pt")));

    // MG ggH specific weights
    if(do_quarkmass_higgspt_){
      fns_["ggH_quarkmass_corr"] = std::shared_ptr<RooFunctor>(
      	  w_ggh_->function("ggH_quarkmass_corr")->functor(w_ggh_->argSet("HpT")));
      fns_["ggH_quarkmass_corr_up"] = std::shared_ptr<RooFunctor>(
      	  w_ggh_->function("ggH_quarkmass_corr_up")->functor(w_ggh_->argSet("HpT")));
      fns_["ggH_quarkmass_corr_down"] = std::shared_ptr<RooFunctor>(
      	  w_ggh_->function("ggH_quarkmass_corr_down")->functor(w_ggh_->argSet("HpT")));
      fns_["ggH_fullquarkmass_corr"] = std::shared_ptr<RooFunctor>(
      	  w_ggh_->function("ggH_fullquarkmass_corr")->functor(w_ggh_->argSet("HpT")));
    }
    if(do_ps_weights_){
      fns_["ggH_mg_ue_up"] = std::shared_ptr<RooFunctor>(
      	  w_ggh_->function("ggH_mg_ue_up")->functor(w_ggh_->argSet("ngenjets")));
      fns_["ggH_mg_ue_down"] = std::shared_ptr<RooFunctor>(
      	  w_ggh_->function("ggH_mg_ue_down")->functor(w_ggh_->argSet("ngenjets")));
    }
  }
      
  if(mssm_higgspt_file_!="" && do_mssm_higgspt_){
    TFile f(mssm_higgspt_file_.c_str());
    mssm_w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    //std::string mass_str = mssm_mass_;
    std::vector<std::string> uncerts = {"", "_scale_up", "_scale_down", "_hdamp_up", "_hdamp_down"};
    for (auto u : uncerts) {
      fns_["h_t_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_t_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));     
      fns_["h_b_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_b_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["h_i_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_i_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["H_t_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_t_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["H_b_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_b_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["H_i_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_i_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["A_t_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_t_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["A_b_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_b_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
      fns_["A_i_ratio"+u] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_i_ratio"+u).c_str())->functor(mssm_w_->argSet("h_pt,h_mass")));
    }

  }

  return 0;
}

int HTTWeights::Execute(TreeEvent *event) {

  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);

  double weight = 1.0;
  EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

  std::string MVADM2017 = " ";

  if(era_ == era::data_2016 || era_ == era::data_2017 || era_ == era::data_2018) std::string MVADM2017 = "MVADM2017v1";

  if(era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP || era_ == era::data_2017UL || era_ == era::data_2018UL) std::string MVADM2017 = "MVADM2017v2";

  if(is_embedded_){
    double gen_match_undecayed_1_pt = event->Get<double>("gen_match_undecayed_1_pt");
    double gen_match_undecayed_2_pt = event->Get<double>("gen_match_undecayed_2_pt");
    double gen_match_undecayed_1_eta = event->Get<double>("gen_match_undecayed_1_eta");
    double gen_match_undecayed_2_eta = event->Get<double>("gen_match_undecayed_2_eta");
    auto args_1 = std::vector<double>{gen_match_undecayed_1_eta,gen_match_undecayed_1_pt};
    auto args_2 = std::vector<double>{gen_match_undecayed_2_eta,gen_match_undecayed_2_pt};
    auto args_4 = std::vector<double>{gen_match_undecayed_1_pt,gen_match_undecayed_1_eta,gen_match_undecayed_2_pt,gen_match_undecayed_2_eta};
    double wt_embedding_yield = fns_["m_sel_idEmb_ratio"]->eval(args_1.data())*fns_["m_sel_idEmb_ratio"]->eval(args_2.data())*fns_["m_sel_trg_ratio"]->eval(args_4.data());
    double wt_embedding_yield_kit = fns_["m_sel_idEmb_kit_ratio"]->eval(args_1.data())*fns_["m_sel_idEmb_kit_ratio"]->eval(args_2.data())*fns_["m_sel_trg_kit_ratio"]->eval(args_4.data());
    eventInfo->set_weight("muonEffIDWeight_1", fns_["m_sel_idEmb_ratio"]->eval(args_1.data()), false);
    eventInfo->set_weight("muonEffIDWeight_2", fns_["m_sel_idEmb_ratio"]->eval(args_2.data()), false);
    eventInfo->set_weight("muonEffTrgWeight", fns_["m_sel_trg_ratio"]->eval(args_4.data()), false);

    if (eventInfo->weight("wt_embedding") > 1) wt_embedding_yield = 0.; // values > 1 are un-physical so set yield to 0
    if (eventInfo->weight("wt_embedding") > 1) wt_embedding_yield_kit = 0.; // values > 1 are un-physical so set yield to 0
    eventInfo->set_weight("wt_embedding_yield", wt_embedding_yield);
    event->Add("wt_embedding_yield", wt_embedding_yield);
    wt_embedding_yield_kit = wt_embedding_yield > 0 ? wt_embedding_yield_kit/wt_embedding_yield : 0.;
    double wt_embedding_yield_trg_kit = wt_embedding_yield > 0 ? fns_["m_sel_trg_kit_ratio"]->eval(args_4.data())/fns_["m_sel_trg_ratio"]->eval(args_4.data()) : 0.;
    event->Add("wt_emb_sel_kit", wt_embedding_yield_kit);
    event->Add("wt_emb_sel_trg_kit", wt_embedding_yield_trg_kit);
  }

  if (do_topquark_weights_) {
    double top_wt = 1.0;
    double top_wt_alt = 1.0;
    double top_wt_up = 1.0;
    double top_wt_down = 1.0;
    std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
    for (unsigned i = 0; i < parts.size(); ++i){
      std::vector<bool> status_flags = parts[i]->statusFlags();
      unsigned id = abs(parts[i]->pdgid());
      if(id == 6 && status_flags[FromHardProcess] && status_flags[IsLastCopy]){
        double pt = parts[i]->pt();
        pt = std::min(pt, 472.);
        double pt_max2000 = std::min(parts[i]->pt(), 2000.);
        double a = 0.088, b = -0.00087, c = 9.2e-07;
        top_wt *= std::exp(a + b * pt + c * pt*pt);
        top_wt_alt *= 0.103*std::exp(-0.0118*pt_max2000) - 0.000134*pt_max2000 + 0.973; 
      }
    }
        
    top_wt = std::sqrt(top_wt);
    top_wt_up = top_wt * top_wt;
    top_wt_down = 1.0;
    event->Add("wt_tquark_up", top_wt_up / top_wt);
    event->Add("wt_tquark_down", top_wt_down / top_wt);
    event->Add("wt_tquark_alt", top_wt_alt / top_wt);
    eventInfo->set_weight("topquark_weight", top_wt);
  }
  
  if (do_tau_fake_weights_){
    if(channel_ != channel::em){
      unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
      double weight_up = 1.0;
      double weight_down = 1.0;
      double tau_1_wt=0;
      double tau_2_wt=0;
      if(gen_match_2 == 6){
        Candidate const* tau2 = dilepton[0]->GetCandidate("lepton2");
        double pt_2 = tau2->pt();
        tau_2_wt = (0.20*pt_2)/100.;
      } 
      weight_down = 1.0 + tau_2_wt;
      weight_up = std::max(0.0, 1.0 - tau_2_wt);
      if (channel_ == channel::tt){
       unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
       if(gen_match_1 == 6){
         Candidate const* tau1 = dilepton[0]->GetCandidate("lepton1");
         double pt_1 = tau1->pt();
         tau_1_wt = (0.20*pt_1/100.);
       }
       weight_down = (1.0+tau_2_wt)*(1.0+tau_1_wt);
       weight_up = std::max(0.0, (1.0-tau_2_wt)*(1.0-tau_1_wt));
      }
     event->Add("wt_tau_fake_up", weight_up);
     event->Add("wt_tau_fake_down", weight_down);
    }
  }

  if (do_tau_id_weights_) {
     if(channel_ != channel::em){
       double gen_match_2_pt = event->Get<double>("gen_match_2_pt");
       unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
       double weight_up    = 1.0;
       double weight_down  = 1.0;
       double tau2_wt_up = 0;
       double tau2_wt_down = 0;
       if(gen_match_2 == 5){
         tau2_wt_up = (0.05*gen_match_2_pt)/1000.;
         tau2_wt_down = (0.35*gen_match_2_pt)/1000.;
       }
       weight_up   = 1.0 +tau2_wt_up;
       weight_down = std::max(0.0, 1.0 - tau2_wt_down);
       if(channel_ == channel::tt){
        double tau1_wt_up = 0;
        double tau1_wt_down = 0;
        double gen_match_1_pt = event->Get<double>("gen_match_1_pt");
        unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
        if(gen_match_1 == 5){
          tau1_wt_up = (0.05*gen_match_1_pt)/1000.;
          tau1_wt_down = (0.35*gen_match_1_pt)/1000.;
        }
        weight_up = (1.0+tau2_wt_up)*(1.0+tau1_wt_up);
        weight_down = std::max(0.0,(1.0-tau2_wt_down)*(1.0-tau1_wt_down));
       }
       event->Add("wt_tau_id_up", weight_up);
       event->Add("wt_tau_id_down", weight_down);
      }   
  }
  if (
      do_tau_id_sf_ && channel_!= channel::em  && channel_!= channel::tpzee  && 
      channel_!= channel::tpzmm && channel_ != channel::zmm && channel_ != channel::zee
    ) {
    double tau_sf_1 = 1.0;
    double tau_sf_2 = 1.0;
    double tau_sf_highpt_1 = 1.0;
    double tau_sf_highpt_2 = 1.0;
    if (channel_ != channel::tt){
      unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
      Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      double decay_mode_2 = tau2->decay_mode();
      double pt_2 = tau2->pt();
      double eta_2 = tau2->eta();
      auto args_2 = std::vector<double>{pt_2,eta_2,decay_mode_2};

      double mvadm_2 = tau2->GetTauID(MVADM2017);
      auto args_mvadm = std::vector<double>{pt_2,mvadm_2};

      if (!is_embedded_){
        auto args_pt = std::vector<double>{pt_2};
        tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium"]->eval(args_pt.data()) : 1.0;
        tau_sf_highpt_2 = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt"]->eval(args_pt.data()) : 1.0;

        eventInfo->set_weight("tauIDScaleFactorWeight_medium_DeepTau2017v2p1VSjet_2", tau_sf_2 , false);
        eventInfo->set_weight("tauIDScaleFactorWeight_highpt_deeptauid_2",tau_sf_highpt_2 , false);

        std::vector<std::string> systs = {"bin5_up","bin5_down","bin6_up","bin6_down"};
        for (auto s: systs) {
          double tau_sf_highpt_2_syst=1.;
          tau_sf_highpt_2_syst = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_"+s]->eval(args_pt.data()) : 1.0;
          event->Add("wt_tau_id_mssm_"+s,pt_2>100 ? tau_sf_highpt_2_syst/tau_sf_highpt_2 : 1.);
        }

        double tau_sf_2_bin1_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin1_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin2_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin2_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin3_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin3_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin4_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin4_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin5_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin5_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin1_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin1_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin2_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin2_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin3_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin3_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin4_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin4_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin5_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_medium_bin5_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;

        event->Add("wt_tau_id_pt_bin1_up",tau_sf_2_bin1_up);
        event->Add("wt_tau_id_pt_bin2_up",tau_sf_2_bin2_up);
        event->Add("wt_tau_id_pt_bin3_up",tau_sf_2_bin3_up);
        event->Add("wt_tau_id_pt_bin4_up",tau_sf_2_bin4_up);
        event->Add("wt_tau_id_pt_bin5_up",tau_sf_2_bin5_up);
        event->Add("wt_tau_id_pt_bin1_down",tau_sf_2_bin1_down);
        event->Add("wt_tau_id_pt_bin2_down",tau_sf_2_bin2_down);
        event->Add("wt_tau_id_pt_bin3_down",tau_sf_2_bin3_down);
        event->Add("wt_tau_id_pt_bin4_down",tau_sf_2_bin4_down);
        event->Add("wt_tau_id_pt_bin5_down",tau_sf_2_bin5_down);

        double tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm.data()) : 1.0;

        if(channel_== channel::et) {
          tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_tightvsele"]->eval(args_mvadm.data()) : 1.0;
        }

        double tau_sf_mvadm_2_lowpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;

        event->Add("wt_tau_id_mvadm",tau_sf_mvadm_2/tau_sf_2);
        event->Add("wt_tau_id_mvadm_sync",tau_sf_mvadm_2);

        event->Add("wt_tau_id_mssm",pt_2>100 ? tau_sf_highpt_2/tau_sf_2 : 1.);

        event->Add("wt_tau_id_lowpt_mvadm0_up",tau_sf_mvadm_2_lowpt_mvadm0_up); 
        event->Add("wt_tau_id_lowpt_mvadm1_up",tau_sf_mvadm_2_lowpt_mvadm1_up); 
        event->Add("wt_tau_id_lowpt_mvadm2_up",tau_sf_mvadm_2_lowpt_mvadm2_up); 
        event->Add("wt_tau_id_lowpt_mvadm10_up",tau_sf_mvadm_2_lowpt_mvadm10_up); 
        event->Add("wt_tau_id_lowpt_mvadm11_up",tau_sf_mvadm_2_lowpt_mvadm11_up); 
        event->Add("wt_tau_id_highpt_mvadm0_up",tau_sf_mvadm_2_highpt_mvadm0_up);
        event->Add("wt_tau_id_highpt_mvadm1_up",tau_sf_mvadm_2_highpt_mvadm1_up);
        event->Add("wt_tau_id_highpt_mvadm2_up",tau_sf_mvadm_2_highpt_mvadm2_up);
        event->Add("wt_tau_id_highpt_mvadm10_up",tau_sf_mvadm_2_highpt_mvadm10_up);
        event->Add("wt_tau_id_highpt_mvadm11_up",tau_sf_mvadm_2_highpt_mvadm11_up);
        event->Add("wt_tau_id_lowpt_mvadm0_down",tau_sf_mvadm_2_lowpt_mvadm0_down);
        event->Add("wt_tau_id_lowpt_mvadm1_down",tau_sf_mvadm_2_lowpt_mvadm1_down);
        event->Add("wt_tau_id_lowpt_mvadm2_down",tau_sf_mvadm_2_lowpt_mvadm2_down);
        event->Add("wt_tau_id_lowpt_mvadm10_down",tau_sf_mvadm_2_lowpt_mvadm10_down);
        event->Add("wt_tau_id_lowpt_mvadm11_down",tau_sf_mvadm_2_lowpt_mvadm11_down);
        event->Add("wt_tau_id_highpt_mvadm0_down",tau_sf_mvadm_2_highpt_mvadm0_down);
        event->Add("wt_tau_id_highpt_mvadm1_down",tau_sf_mvadm_2_highpt_mvadm1_down);
        event->Add("wt_tau_id_highpt_mvadm2_down",tau_sf_mvadm_2_highpt_mvadm2_down);
        event->Add("wt_tau_id_highpt_mvadm10_down",tau_sf_mvadm_2_highpt_mvadm10_down);
        event->Add("wt_tau_id_highpt_mvadm11_down",tau_sf_mvadm_2_highpt_mvadm11_down);
      }
      else if (is_embedded_) {

        auto args_pt = std::vector<double>{pt_2};
        tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium"]->eval(args_pt.data()) : 1.0;
        tau_sf_highpt_2 = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_embed"]->eval(args_pt.data()) : 1.0;

        if(channel_== channel::et) {
          tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium"]->eval(args_pt.data()) : 1.0;
          tau_sf_highpt_2 = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_tightvse_embed"]->eval(args_pt.data()) : 1.0;
          double et_tau_extra = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium"]->eval(args_pt.data()) : 1.0;
          et_tau_extra/=tau_sf_2;
          event->Add("wt_tau_id_extra",et_tau_extra);
        }

        eventInfo->set_weight("tauIDScaleFactorWeight_medium_DeepTau2017v2p1VSjet_2", tau_sf_2 , false);
        eventInfo->set_weight("tauIDScaleFactorWeight_highpt_deeptauid_2",tau_sf_highpt_2 , false);

        std::vector<std::string> systs = {"bin5_up","bin5_down","bin6_up","bin6_down"};
        for (auto s: systs) {
          double tau_sf_highpt_2_syst=1.;
          if(channel_== channel::et) tau_sf_highpt_2_syst = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_tightvse_embed_"+s]->eval(args_pt.data()) : 1.0;
          else tau_sf_highpt_2_syst = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_embed_"+s]->eval(args_pt.data()) : 1.0;
          event->Add("wt_tau_id_mssm_"+s,pt_2>100 ? tau_sf_highpt_2_syst/tau_sf_highpt_2 : 1.);
        }

        double tau_sf_2_bin1_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin1_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin2_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin2_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin3_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin3_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin4_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin4_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin5_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin5_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin1_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin1_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin2_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin2_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin3_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin3_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin4_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin4_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        double tau_sf_2_bin5_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_embed_medium_bin5_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;

        if(channel_== channel::et) {
          tau_sf_2_bin1_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin1_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin2_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin2_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin3_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin3_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin4_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin4_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin5_up = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin5_up"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin1_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin1_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin2_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin2_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin3_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin3_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin4_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin4_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
          tau_sf_2_bin5_down = (gen_match_2==5) ? fns_["t_deeptauid_pt_tightvse_embed_medium_bin5_down"]->eval(args_pt.data())/tau_sf_2 : 1.0;
        }


        if(gen_match_2==5) {
          if (decay_mode_2==0) tau_sf_2*=0.975;
          else if (decay_mode_2==1) tau_sf_2*=0.975*1.051;
          else if (decay_mode_2==5||decay_mode_2==6) tau_sf_2*=pow(0.975,2);
          else if (decay_mode_2==10) tau_sf_2*=pow(0.975,3);
          else if (decay_mode_2==11) tau_sf_2*=pow(0.975,3)*1.051;
        }

        event->Add("wt_tau_id_pt_bin1_up",tau_sf_2_bin1_up);
        event->Add("wt_tau_id_pt_bin2_up",tau_sf_2_bin2_up);
        event->Add("wt_tau_id_pt_bin3_up",tau_sf_2_bin3_up);
        event->Add("wt_tau_id_pt_bin4_up",tau_sf_2_bin4_up);
        event->Add("wt_tau_id_pt_bin5_up",tau_sf_2_bin5_up);
        event->Add("wt_tau_id_pt_bin1_down",tau_sf_2_bin1_down);
        event->Add("wt_tau_id_pt_bin2_down",tau_sf_2_bin2_down);
        event->Add("wt_tau_id_pt_bin3_down",tau_sf_2_bin3_down);
        event->Add("wt_tau_id_pt_bin4_down",tau_sf_2_bin4_down);
        event->Add("wt_tau_id_pt_bin5_down",tau_sf_2_bin5_down);

        double tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium"]->eval(args_mvadm.data()) : 1.0;
        if(channel_== channel::et) {
          tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_tightvsele"]->eval(args_mvadm.data()) : 1.0;
        }

        double tau_sf_mvadm_2_lowpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_lowpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm0_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm1_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm2_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm10_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm11_up = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm0_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm1_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm2_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm10_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;
        double tau_sf_mvadm_2_highpt_mvadm11_down = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"]->eval(args_mvadm.data())/tau_sf_mvadm_2 : 1.0;

        event->Add("wt_tau_id_mvadm",tau_sf_mvadm_2/tau_sf_2);
        event->Add("wt_tau_id_mvadm_sync",tau_sf_mvadm_2);

        event->Add("wt_tau_id_mssm",pt_2>100 ? tau_sf_highpt_2/tau_sf_2 : 1.);

        event->Add("wt_tau_id_lowpt_mvadm0_up",tau_sf_mvadm_2_lowpt_mvadm0_up);
        event->Add("wt_tau_id_lowpt_mvadm1_up",tau_sf_mvadm_2_lowpt_mvadm1_up);
        event->Add("wt_tau_id_lowpt_mvadm2_up",tau_sf_mvadm_2_lowpt_mvadm2_up);
        event->Add("wt_tau_id_lowpt_mvadm10_up",tau_sf_mvadm_2_lowpt_mvadm10_up);
        event->Add("wt_tau_id_lowpt_mvadm11_up",tau_sf_mvadm_2_lowpt_mvadm11_up);
        event->Add("wt_tau_id_highpt_mvadm0_up",tau_sf_mvadm_2_highpt_mvadm0_up);
        event->Add("wt_tau_id_highpt_mvadm1_up",tau_sf_mvadm_2_highpt_mvadm1_up);
        event->Add("wt_tau_id_highpt_mvadm2_up",tau_sf_mvadm_2_highpt_mvadm2_up);
        event->Add("wt_tau_id_highpt_mvadm10_up",tau_sf_mvadm_2_highpt_mvadm10_up);
        event->Add("wt_tau_id_highpt_mvadm11_up",tau_sf_mvadm_2_highpt_mvadm11_up);
        event->Add("wt_tau_id_lowpt_mvadm0_down",tau_sf_mvadm_2_lowpt_mvadm0_down);
        event->Add("wt_tau_id_lowpt_mvadm1_down",tau_sf_mvadm_2_lowpt_mvadm1_down);
        event->Add("wt_tau_id_lowpt_mvadm2_down",tau_sf_mvadm_2_lowpt_mvadm2_down);
        event->Add("wt_tau_id_lowpt_mvadm10_down",tau_sf_mvadm_2_lowpt_mvadm10_down);
        event->Add("wt_tau_id_lowpt_mvadm11_down",tau_sf_mvadm_2_lowpt_mvadm11_down);
        event->Add("wt_tau_id_highpt_mvadm0_down",tau_sf_mvadm_2_highpt_mvadm0_down);
        event->Add("wt_tau_id_highpt_mvadm1_down",tau_sf_mvadm_2_highpt_mvadm1_down);
        event->Add("wt_tau_id_highpt_mvadm2_down",tau_sf_mvadm_2_highpt_mvadm2_down);
        event->Add("wt_tau_id_highpt_mvadm10_down",tau_sf_mvadm_2_highpt_mvadm10_down);
        event->Add("wt_tau_id_highpt_mvadm11_down",tau_sf_mvadm_2_highpt_mvadm11_down);

      }
      else tau_sf_2 =  (gen_match_2 == 5) ? fns_["t_iso_mva_m_pt30_sf"]->eval(args_2.data()) : 1.0;
    } else {
      unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
      unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
      Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
      double decay_mode_1 = tau1->decay_mode();
      double pt_1 = tau1->pt();
      double eta_1 = tau1->eta();
      Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      double decay_mode_2 = tau2->decay_mode();
      double pt_2 = tau2->pt();
      double eta_2 = tau2->eta();
      auto args_1 = std::vector<double>{pt_1,eta_1,decay_mode_1};
      auto args_2 = std::vector<double>{pt_2,eta_2,decay_mode_2};
      auto args_pt_1 = std::vector<double>{pt_1};
      auto args_pt_2 = std::vector<double>{pt_2};

      double mvadm_1 = tau1->GetTauID(MVADM2017);
      double mvadm_2 = tau2->GetTauID(MVADM2017);

      auto args_mvadm_1 = std::vector<double>{pt_1,mvadm_1};
      auto args_mvadm_2 = std::vector<double>{pt_2,mvadm_2};

      double tau_sf_dm0_up=1.;
      double tau_sf_dm1_up=1.;
      double tau_sf_dm10_up=1.;
      double tau_sf_dm11_up=1.;
      double tau_sf_dm0_down=1.;
      double tau_sf_dm1_down=1.;
      double tau_sf_dm10_down=1.;
      double tau_sf_dm11_down=1.;

      double tau_sf_mvadm_1=1.;
      double tau_sf_mvadm_2=1.;

      double tau_sf_mvadm_lowpt_mvadm0_up=1.;
      double tau_sf_mvadm_lowpt_mvadm1_up=1.;
      double tau_sf_mvadm_lowpt_mvadm2_up=1.;
      double tau_sf_mvadm_lowpt_mvadm10_up=1.;
      double tau_sf_mvadm_lowpt_mvadm11_up=1.;
      double tau_sf_mvadm_lowpt_mvadm0_down=1.;
      double tau_sf_mvadm_lowpt_mvadm1_down=1.;
      double tau_sf_mvadm_lowpt_mvadm2_down=1.;
      double tau_sf_mvadm_lowpt_mvadm10_down=1.;
      double tau_sf_mvadm_lowpt_mvadm11_down=1.;
      double tau_sf_mvadm_highpt_mvadm0_up=1.;
      double tau_sf_mvadm_highpt_mvadm1_up=1.;
      double tau_sf_mvadm_highpt_mvadm2_up=1.;
      double tau_sf_mvadm_highpt_mvadm10_up=1.;
      double tau_sf_mvadm_highpt_mvadm11_up=1.;
      double tau_sf_mvadm_highpt_mvadm0_down=1.;
      double tau_sf_mvadm_highpt_mvadm1_down=1.;
      double tau_sf_mvadm_highpt_mvadm2_down=1.;
      double tau_sf_mvadm_highpt_mvadm10_down=1.;
      double tau_sf_mvadm_highpt_mvadm11_down=1.;

      if (!is_embedded_){
        auto args_dm_1 = std::vector<double>{decay_mode_1};
        tau_sf_1 = (gen_match_1==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_dm_1.data()) : 1.0;
        tau_sf_highpt_1 = (gen_match_2==5&&pt_1>100) ? fns_["t_deeptauid_highpt"]->eval(args_pt_1.data()) : 1.0;
        tau_sf_dm0_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm0_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm1_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm1_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm10_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm10_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm11_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm11_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm0_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm0_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm1_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm1_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm10_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm10_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm11_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_medium_dm11_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;

        tau_sf_mvadm_1 = (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm_1.data()) : 1.0;

        tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;

      }
      else if (is_embedded_) {
        auto args_dm_1 = std::vector<double>{decay_mode_1};
        tau_sf_1 = (gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium"]->eval(args_dm_1.data()) : 1.0;
        tau_sf_highpt_1 = (gen_match_1==5&&pt_1>100) ? fns_["t_deeptauid_highpt_embed"]->eval(args_pt_1.data()) : 1.0;
        tau_sf_dm0_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm1_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm10_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm11_up*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_up"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm0_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm1_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm10_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;
        tau_sf_dm11_down*=((gen_match_1==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_down"]->eval(args_dm_1.data()) : 1.0)/tau_sf_1;

        if(gen_match_1==5) {
          if (decay_mode_1==0)       tau_sf_1*=0.975;
          else if (decay_mode_1==1)  tau_sf_1*=0.975*1.051;
          else if (decay_mode_1==5||decay_mode_1==6) tau_sf_1*=pow(0.975,2);
          else if (decay_mode_1==10) tau_sf_1*=pow(0.975,3);
          else if (decay_mode_1==11) tau_sf_1*=pow(0.975,3)*1.051;
        }

        tau_sf_mvadm_1 = (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium"]->eval(args_mvadm_1.data()) : 1.0;

        tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_1==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"]->eval(args_mvadm_1.data())/tau_sf_mvadm_1 : 1.0;

      }
      else tau_sf_1 = (gen_match_1==5) ? fns_["t_iso_mva_t_pt40_eta2p1_sf"]->eval(args_1.data()) : 1.0;
      if (!is_embedded_){
        auto args_dm_2 = std::vector<double>{decay_mode_2};
        tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_dm_2.data()) : 1.0;
        tau_sf_highpt_2 = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt"]->eval(args_pt_2.data()) : 1.0;
        tau_sf_dm0_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm0_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm1_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm1_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm10_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm10_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm11_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm11_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm0_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm0_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm1_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm1_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm10_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm10_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm11_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_medium_dm11_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;

        tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm_2.data()) : 1.0;

        tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_lowpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium_highpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;

      }
      else if (is_embedded_) {
        auto args_dm_2 = std::vector<double>{decay_mode_2};
        tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium"]->eval(args_dm_2.data()) : 1.0;
        tau_sf_highpt_2 = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_embed"]->eval(args_pt_2.data()) : 1.0;
        tau_sf_dm0_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm1_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm10_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm11_up*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_up"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm0_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm0_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm1_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm1_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm10_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm10_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;
        tau_sf_dm11_down*=((gen_match_2==5) ? fns_["t_deeptauid_dm_embed_medium_dm11_down"]->eval(args_dm_2.data()) : 1.0)/tau_sf_2;

        if(gen_match_2==5) {
          if (decay_mode_2==0)       tau_sf_2*=0.975;
          else if (decay_mode_2==1)  tau_sf_2*=0.975*1.051;
          else if (decay_mode_2==5||decay_mode_2==6) tau_sf_2*=pow(0.975,2);
          else if (decay_mode_2==10) tau_sf_2*=pow(0.975,3);
          else if (decay_mode_2==11) tau_sf_2*=pow(0.975,3)*1.051;
        }

        tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium"]->eval(args_mvadm_2.data()) : 1.0;

        tau_sf_mvadm_lowpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_lowpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_lowpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_up *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_up"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm0_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm0_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm1_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm1_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm2_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm2_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm10_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm10_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;
        tau_sf_mvadm_highpt_mvadm11_down *= (gen_match_2==5) ? fns_["t_deeptauid_mvadm_embed_medium_highpt_mvadm11_down"]->eval(args_mvadm_2.data())/tau_sf_mvadm_2 : 1.0;

      }
      else tau_sf_2 = (gen_match_2==5) ? fns_["t_iso_mva_t_pt40_eta2p1_sf"]->eval(args_2.data()) : 1.0;

      event->Add("wt_tau_id_dm0_up",tau_sf_dm0_up);
      event->Add("wt_tau_id_dm1_up",tau_sf_dm1_up);
      event->Add("wt_tau_id_dm10_up",tau_sf_dm10_up);
      event->Add("wt_tau_id_dm11_up",tau_sf_dm11_up);
      event->Add("wt_tau_id_dm0_down",tau_sf_dm0_down);
      event->Add("wt_tau_id_dm1_down",tau_sf_dm1_down);
      event->Add("wt_tau_id_dm10_down",tau_sf_dm10_down);
      event->Add("wt_tau_id_dm11_down",tau_sf_dm11_down);      

      event->Add("wt_tau_id_mvadm",(tau_sf_mvadm_1*tau_sf_mvadm_2)/(tau_sf_1*tau_sf_2));
      
      event->Add("wt_tau_id_mssm",(pt_1>100 ? tau_sf_highpt_1/tau_sf_1 : 1.)*(pt_2>100 ? tau_sf_highpt_2/tau_sf_2 : 1.));

      std::vector<std::string> systs = {"bin5_up","bin5_down","bin6_up","bin6_down"};
      for (auto s: systs) {
        double tau_sf_highpt_2_syst=1., tau_sf_highpt_1_syst=1.;
        if(is_embedded_) {
          tau_sf_highpt_2_syst = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_embed_"+s]->eval(args_pt_2.data()) : 1.0;
          tau_sf_highpt_1_syst = (gen_match_1==5&&pt_1>100) ? fns_["t_deeptauid_highpt_embed_"+s]->eval(args_pt_1.data()) : 1.0;

        } else {
          tau_sf_highpt_2_syst = (gen_match_2==5&&pt_2>100) ? fns_["t_deeptauid_highpt_"+s]->eval(args_pt_2.data()) : 1.0;
          tau_sf_highpt_1_syst = (gen_match_1==5&&pt_1>100) ? fns_["t_deeptauid_highpt_"+s]->eval(args_pt_1.data()) : 1.0;
        }
        event->Add("wt_tau_id_mssm_"+s,(tau_sf_highpt_1_syst*tau_sf_highpt_2_syst)/(tau_sf_highpt_1*tau_sf_highpt_2));
      }


      event->Add("wt_tau_id_lowpt_mvadm0_up",tau_sf_mvadm_lowpt_mvadm0_up);
      event->Add("wt_tau_id_lowpt_mvadm1_up",tau_sf_mvadm_lowpt_mvadm1_up);
      event->Add("wt_tau_id_lowpt_mvadm2_up",tau_sf_mvadm_lowpt_mvadm2_up);
      event->Add("wt_tau_id_lowpt_mvadm10_up",tau_sf_mvadm_lowpt_mvadm10_up);
      event->Add("wt_tau_id_lowpt_mvadm11_up",tau_sf_mvadm_lowpt_mvadm11_up);
      event->Add("wt_tau_id_highpt_mvadm0_up",tau_sf_mvadm_highpt_mvadm0_up);
      event->Add("wt_tau_id_highpt_mvadm1_up",tau_sf_mvadm_highpt_mvadm1_up);
      event->Add("wt_tau_id_highpt_mvadm2_up",tau_sf_mvadm_highpt_mvadm2_up);
      event->Add("wt_tau_id_highpt_mvadm10_up",tau_sf_mvadm_highpt_mvadm10_up);
      event->Add("wt_tau_id_highpt_mvadm11_up",tau_sf_mvadm_highpt_mvadm11_up);
      event->Add("wt_tau_id_lowpt_mvadm0_down",tau_sf_mvadm_lowpt_mvadm0_down);
      event->Add("wt_tau_id_lowpt_mvadm1_down",tau_sf_mvadm_lowpt_mvadm1_down);
      event->Add("wt_tau_id_lowpt_mvadm2_down",tau_sf_mvadm_lowpt_mvadm2_down);
      event->Add("wt_tau_id_lowpt_mvadm10_down",tau_sf_mvadm_lowpt_mvadm10_down);
      event->Add("wt_tau_id_lowpt_mvadm11_down",tau_sf_mvadm_lowpt_mvadm11_down);
      event->Add("wt_tau_id_highpt_mvadm0_down",tau_sf_mvadm_highpt_mvadm0_down);
      event->Add("wt_tau_id_highpt_mvadm1_down",tau_sf_mvadm_highpt_mvadm1_down);
      event->Add("wt_tau_id_highpt_mvadm2_down",tau_sf_mvadm_highpt_mvadm2_down);
      event->Add("wt_tau_id_highpt_mvadm10_down",tau_sf_mvadm_highpt_mvadm10_down);
      event->Add("wt_tau_id_highpt_mvadm11_down",tau_sf_mvadm_highpt_mvadm11_down);

    }
    eventInfo->set_weight("wt_tau_id_sf",tau_sf_1*tau_sf_2);
    if(channel_==channel::tt) event->Add("idisoweight_1", tau_sf_1);
    event->Add("idisoweight_2", tau_sf_2);
  }
  if (do_em_qcd_weights_){
    if(channel_ == channel::em){
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1")); 
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2")); 
     double qcd_weight=1.0;
     std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
     ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
     double n_jets = (double)jets.size();
     double dR = fabs(ROOT::Math::VectorUtil::DeltaR(elec->vector(),muon->vector()));
     auto args = std::vector<double>{dR,n_jets,elec->pt(),muon->pt()};
     qcd_weight = fns_["em_qcd_osss"]->eval(args.data()); 
     double qcd_extrap_up = fns_["em_qcd_osss_extrap_up"]->eval(args.data())/qcd_weight;
     double qcd_extrap_down = fns_["em_qcd_osss_extrap_down"]->eval(args.data())/qcd_weight;
     event->Add("wt_em_qcd_extrapdown",qcd_extrap_down);
     event->Add("wt_em_qcd_extrapup",qcd_extrap_up);

     double qcd_njets0_unc1_up =   fns_["em_qcd_osss_stat_0jet_unc1_up"]->eval(args.data())/qcd_weight;
     double qcd_njets0_unc1_down =   fns_["em_qcd_osss_stat_0jet_unc1_down"]->eval(args.data())/qcd_weight;
     double qcd_njets0_unc2_up =   fns_["em_qcd_osss_stat_0jet_unc2_up"]->eval(args.data())/qcd_weight;
     double qcd_njets0_unc2_down =   fns_["em_qcd_osss_stat_0jet_unc2_down"]->eval(args.data())/qcd_weight;

     double qcd_njets1_unc1_up =   fns_["em_qcd_osss_stat_1jet_unc1_up"]->eval(args.data())/qcd_weight;
     double qcd_njets1_unc1_down =   fns_["em_qcd_osss_stat_1jet_unc1_down"]->eval(args.data())/qcd_weight;
     double qcd_njets1_unc2_up =   fns_["em_qcd_osss_stat_1jet_unc2_up"]->eval(args.data())/qcd_weight;
     double qcd_njets1_unc2_down =   fns_["em_qcd_osss_stat_1jet_unc2_down"]->eval(args.data())/qcd_weight;

     double qcd_njets2_unc1_up =   fns_["em_qcd_osss_stat_2jet_unc1_up"]->eval(args.data())/qcd_weight;
     double qcd_njets2_unc1_down =   fns_["em_qcd_osss_stat_2jet_unc1_down"]->eval(args.data())/qcd_weight;
     double qcd_njets2_unc2_up =   fns_["em_qcd_osss_stat_2jet_unc2_up"]->eval(args.data())/qcd_weight;
     double qcd_njets2_unc2_down =   fns_["em_qcd_osss_stat_2jet_unc2_down"]->eval(args.data())/qcd_weight;
     
     event->Add("wt_em_qcd_njets0_unc1_up",qcd_njets0_unc1_up);
     event->Add("wt_em_qcd_njets0_unc1_down",qcd_njets0_unc1_down);
     event->Add("wt_em_qcd_njets0_unc2_up",qcd_njets0_unc2_up);
     event->Add("wt_em_qcd_njets0_unc2_down",qcd_njets0_unc2_down);

     event->Add("wt_em_qcd_njets1_unc1_up",qcd_njets1_unc1_up);
     event->Add("wt_em_qcd_njets1_unc1_down",qcd_njets1_unc1_down);
     event->Add("wt_em_qcd_njets1_unc2_up",qcd_njets1_unc2_up);
     event->Add("wt_em_qcd_njets1_unc2_down",qcd_njets1_unc2_down);

     event->Add("wt_em_qcd_njets2_unc1_up",qcd_njets2_unc1_up);
     event->Add("wt_em_qcd_njets2_unc1_down",qcd_njets2_unc1_down);
     event->Add("wt_em_qcd_njets2_unc2_up",qcd_njets2_unc2_up);
     event->Add("wt_em_qcd_njets2_unc2_down",qcd_njets2_unc2_down);

     event->Add("wt_em_qcd",qcd_weight);
   }
 }

 if(is_embedded_) {
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

   double wtzpt_embed_ic = embed_pt_weights_ic_hist_->GetBinContent(embed_pt_weights_ic_hist_->FindBin(zmass,zpt));
   event->Add("wt_zpt_embed_ic",wtzpt_embed_ic);
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
      event->Add("wt_zpt_up",wtzpt_up/wtzpt);
      event->Add("wt_zpt_down",wtzpt_down/wtzpt);
  }
  if(mssm_higgspt_file_!="" && do_mssm_higgspt_){

    double pT = -9999;  
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    for(unsigned i=0; i<gen_particles.size(); ++i){
      unsigned genID = std::fabs(gen_particles[i]->pdgid());  
      if((genID==36 || genID==25 || genID==35) && gen_particles[i]->statusFlags()[IsLastCopy]) pT = gen_particles[i]->vector().Pt();
    }

    double mass = std::stod(mssm_mass_);

    auto args = std::vector<double>{pT,mass}; 

    
    std::vector<std::string> uncerts = {"", "_scale_up", "_scale_down", "_hdamp_up", "_hdamp_down"};
    for (auto u : uncerts){
      double wt_ggh_t_ = fns_["h_t_ratio"+u]->eval(args.data());        
      double wt_ggh_b_ = fns_["h_b_ratio"+u]->eval(args.data());
      double wt_ggh_i_ = fns_["h_i_ratio"+u]->eval(args.data());
      double wt_ggH_t_ = fns_["H_t_ratio"+u]->eval(args.data());
      double wt_ggH_b_ = fns_["H_b_ratio"+u]->eval(args.data());
      double wt_ggH_i_ = fns_["H_i_ratio"+u]->eval(args.data());
      double wt_ggA_t_ = fns_["A_t_ratio"+u]->eval(args.data());
      double wt_ggA_b_ = fns_["A_b_ratio"+u]->eval(args.data());
      double wt_ggA_i_ = fns_["A_i_ratio"+u]->eval(args.data());
    
      event->Add("wt_ggh_t"+u ,wt_ggh_t_);
      event->Add("wt_ggh_b"+u ,wt_ggh_b_);
      event->Add("wt_ggh_i"+u ,wt_ggh_i_);
      event->Add("wt_ggH_t"+u ,wt_ggH_t_);
      event->Add("wt_ggH_b"+u ,wt_ggH_b_);
      event->Add("wt_ggH_i"+u ,wt_ggH_i_);
      event->Add("wt_ggA_t"+u ,wt_ggA_t_);
      event->Add("wt_ggA_b"+u ,wt_ggA_b_);
      event->Add("wt_ggA_i"+u ,wt_ggA_i_);
    }


  }
  if(do_quarkmass_higgspt_ || do_ps_weights_){
    double HpT=-9999;
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    for(unsigned i=0; i<gen_particles.size(); ++i){
      unsigned genID = std::fabs(gen_particles[i]->pdgid());
      if(genID==25 && gen_particles[i]->statusFlags()[IsLastCopy]) HpT = gen_particles[i]->vector().Pt();
    }
    if(do_quarkmass_higgspt_){
      auto args = std::vector<double>{HpT};
      double wt_quarkmass = fns_["ggH_quarkmass_corr"]->eval(args.data());
      eventInfo->set_weight("wt_quarkmass",wt_quarkmass);
      event->Add("wt_quarkmass" ,1./wt_quarkmass);
      double wt_fullquarkmass = fns_["ggH_fullquarkmass_corr"]->eval(args.data());
      event->Add("wt_fullquarkmass" , wt_fullquarkmass/wt_quarkmass);
    }
    if(do_ps_weights_) {
      unsigned ngenjets = event->Get<unsigned>("ngenjets");
      auto args_1 = std::vector<double>{(double)ngenjets};
      double wt_ue_up = fns_["ggH_mg_ue_up"]->eval(args_1.data());
      double wt_ue_down = fns_["ggH_mg_ue_down"]->eval(args_1.data());
      event->Add("wt_ue_up", wt_ue_up);
      event->Add("wt_ue_down", wt_ue_down);
    }
  }
  if (do_nnlops_weights_){
    double wt_mg_nnlops = 1.;
    double wt_ph_nnlops = 1.;

    // Get n_jets30 and higgs pt from eventinfo (produced with rivet)
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsWG/SignalModelingTools
    unsigned n_jets30 = eventInfo->n_jets30();
    double pt_h = eventInfo->pt_h();
    if(pt_h==0 && n_jets30==0){
      n_jets30 = event->Exists("ngenjets") ? event->Get<unsigned>("ngenjets") : -1;
      std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
      for(unsigned i=0; i<gen_particles.size(); ++i){
        unsigned genID = std::fabs(gen_particles[i]->pdgid());
        if(genID==25 && gen_particles[i]->statusFlags()[IsLastCopy]) pt_h = gen_particles[i]->vector().Pt();
      }
    }

    if (n_jets30      == 0) wt_mg_nnlops = ggh_mg_0jet_->Eval(std::min(pt_h, 125.));
    else if (n_jets30 == 1) wt_mg_nnlops = ggh_mg_1jet_->Eval(std::min(pt_h, 625.));
    else if (n_jets30 == 2) wt_mg_nnlops = ggh_mg_2jet_->Eval(std::min(pt_h, 800.));
    else if (n_jets30 >= 3) wt_mg_nnlops = ggh_mg_3jet_->Eval(std::min(pt_h, 925.));
    else wt_mg_nnlops = 1.;

    if (n_jets30      == 0) wt_ph_nnlops = ggh_ph_0jet_->Eval(std::min(pt_h, 125.));
    else if (n_jets30 == 1) wt_ph_nnlops = ggh_ph_1jet_->Eval(std::min(pt_h, 625.));
    else if (n_jets30 == 2) wt_ph_nnlops = ggh_ph_2jet_->Eval(std::min(pt_h, 800.));
    else if (n_jets30 >= 3) wt_ph_nnlops = ggh_ph_3jet_->Eval(std::min(pt_h, 925.));
    else wt_ph_nnlops = 1.;

    event->Add("wt_mg_nnlops", wt_mg_nnlops);
    event->Add("wt_ph_nnlops", wt_ph_nnlops);
  }

 if (do_tracking_eff_){
     double tracking_wt_1 = 1.0;
     double tracking_wt_2 = 1.0;
   if(channel_ == channel::et){
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     auto args = std::vector<double>{elec->pt(),elec->sc_eta()};
     if(!is_embedded_) tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
     if(is_embedded_) tracking_wt_1*=fns_["e_trk_embed_ratio"]->eval(args.data());
     tracking_wt_2 = 1.0;
   }
   if(channel_ == channel::mt){
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
     auto args = std::vector<double>{muon->eta()};
     tracking_wt_1 *= fns_["m_trk_ratio"]->eval(args.data());
     tracking_wt_2 = 1.0;
   } 
   if(channel_ == channel::em){
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
     auto args = std::vector<double>{elec->pt(),elec->sc_eta()};
     if(!is_embedded_) tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
     if(is_embedded_) tracking_wt_1*=fns_["e_trk_embed_ratio"]->eval(args.data());
     auto args_2 = std::vector<double>{muon->eta()};
     tracking_wt_2 *= fns_["m_trk_ratio"]->eval(args_2.data());
    }
    if(channel_ == channel::zmm){
     Muon const* muon_1 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
     Muon const* muon_2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
     auto args = std::vector<double>{muon_1->eta()};
     tracking_wt_1 *= fns_["m_trk_ratio"]->eval(args.data());
     auto args_2 = std::vector<double>{muon_2->eta()};
     tracking_wt_2 *= fns_["m_trk_ratio"]->eval(args_2.data());
    }
    if(channel_ == channel::zee){
     Electron const* elec_1 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     Electron const* elec_2 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton2"));
     auto args = std::vector<double>{elec_1->pt(),elec_1->sc_eta()};
     if(!is_embedded_) tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
     if(is_embedded_) tracking_wt_1*=fns_["e_trk_embed_ratio"]->eval(args.data());
     auto args_2 = std::vector<double>{elec_2->pt(),elec_2->sc_eta()};
     if(!is_embedded_) tracking_wt_2 *= fns_["e_trk_ratio"]->eval(args_2.data());
     if(is_embedded_) tracking_wt_2*=fns_["e_trk_embed_ratio"]->eval(args_2.data());
    }
    event->Add("trackingweight_1",tracking_wt_1);
    event->Add("trackingweight_2",tracking_wt_2);
    eventInfo->set_weight("wt_tracking_eff",tracking_wt_1*tracking_wt_2);
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
     if (trg_applied_in_mc_) {
       ele1_trg = ele1_trg / ele1_trg_mc;
       ele2_trg = ele2_trg / ele2_trg_mc;
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

 if (do_idiso_weights_ || do_id_weights_) {
   if (channel_ == channel::et) {
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     double pt = elec->pt();
     double e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho()); 
     double e_sceta = elec->sc_eta();
     auto args_1 = std::vector<double>{pt,e_sceta,e_iso};
     double ele_idiso=1.;  
     if(is_embedded_){
       ele_idiso = fns_["e_idiso_embed_ratio"]->eval(args_1.data());
     } else {
       ele_idiso = fns_["e_idiso_ratio"]->eval(args_1.data());
     }
     eventInfo->set_weight("idiso", ele_idiso, false);
     event->Add("idisoweight_1",ele_idiso);
     weight *= (ele_idiso); 
   } else if (channel_ == channel::mt) {
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
     double pt = muon->pt();
     double m_signed_eta = muon->eta();
     double m_iso = PF04IsolationVal(muon, 0.5, 0);
     double mu_idiso=1.0;
     auto args_1 = std::vector<double>{pt,m_signed_eta,m_iso};  
     if(is_embedded_){
       mu_idiso = fns_["m_idiso_embed_ratio"]->eval(args_1.data());
     } else{
       mu_idiso = fns_["m_idiso_ratio"]->eval(args_1.data());
     }

     weight *= mu_idiso;
     event->Add("idisoweight_1", mu_idiso);
     eventInfo->set_weight("idiso", mu_idiso, false);
   } else if (channel_ == channel::em) {
     Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
     Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));

     double e_pt = elec->pt();
     double m_pt = muon->pt();
     double m_signed_eta = muon->eta();
     double m_idiso = 1.0;
     double e_idiso = 1.0;
     double m_iso = PF04IsolationVal(muon, 0.5, 0); 
     double e_iso = PF03EAIsolationVal(elec, eventInfo->jet_rho()); //lepton_rho
     double e_sceta = elec->sc_eta(); 
     auto args_1 = std::vector<double>{e_pt,e_sceta,e_iso};  
     auto args_2_1 = std::vector<double>{m_pt,m_signed_eta};
     auto args_2_2 = std::vector<double>{m_pt,m_signed_eta,m_iso};  
     if(!is_embedded_){
       e_idiso= fns_["e_idiso_ratio"]->eval(args_1.data());
     }
     if(is_embedded_){
       e_idiso = fns_["e_idiso_embed_ratio"]->eval(args_1.data());
     } 
     if(!is_embedded_) m_idiso = fns_["m_looseiso_ratio"]->eval(args_2_2.data())*fns_["m_id_ratio"]->eval(args_2_1.data());;
     if(is_embedded_) m_idiso = fns_["m_looseiso_embed_ratio"]->eval(args_2_2.data())*fns_["m_id_embed_ratio"]->eval(args_2_1.data());

     weight *= (e_idiso * m_idiso); 
     event->Add("idisoweight_1", e_idiso);
     event->Add("idisoweight_2", m_idiso);  
     event->Add("isoweight_1", double(1.0));
     event->Add("isoweight_2", double(1.0));
   } else if (channel_ == channel::zmm){
     Muon const* muon_1 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
     Muon const* muon_2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
     double m_1_pt = muon_1->pt();
     double m_1_signed_eta = muon_1->eta();
     double m_2_pt = muon_2->pt();
     double m_2_signed_eta = muon_2->eta();
     double m_1_iso = PF04IsolationVal(muon_1,0.5,0);
     double m_2_iso = PF04IsolationVal(muon_2,0.5,0);
     double m_1_idiso = 1.0;
     double m_2_idiso = 1.0;
     auto args1_2 = std::vector<double>{m_1_pt,m_1_signed_eta,m_1_iso};
     auto args2_2 = std::vector<double>{m_2_pt,m_2_signed_eta,m_2_iso};
     if(is_embedded_){
       m_1_idiso = fns_["m_idiso_embed_ratio"]->eval(args1_2.data());
       m_2_idiso = fns_["m_idiso_embed_ratio"]->eval(args2_2.data());
     } else {
       m_1_idiso = fns_["m_idiso_ratio"]->eval(args1_2.data());
       m_2_idiso = fns_["m_idiso_ratio"]->eval(args2_2.data());
     }

     weight *= (m_1_idiso * m_2_idiso);
     event->Add("idisoweight_1", m_1_idiso);
     event->Add("idisoweight_2", m_2_idiso);
     event->Add("isoweight_1", double(1.0));
     event->Add("isoweight_2", double(1.0));
   } else if (channel_ == channel::zee){
       Electron const* ele_1 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
       Electron const* ele_2 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton2"));
       double e_1_pt = ele_1->pt();
       double e_2_pt = ele_2->pt();
       double e_1_idiso = 1.0;
       double e_2_idiso = 1.0;
       double e_1_iso = PF03EAIsolationVal(ele_1, eventInfo->jet_rho());
       double e_2_iso = PF03EAIsolationVal(ele_2, eventInfo->jet_rho());
       double e_1_sceta = ele_1->sc_eta();   
       double e_2_sceta = ele_2->sc_eta();
       auto args1_1 = std::vector<double>{e_1_pt,e_1_sceta,e_1_iso};
       auto args2_1 = std::vector<double>{e_2_pt,e_2_sceta,e_2_iso};

       if(is_embedded_){
         e_1_idiso = fns_["e_idiso_embed_ratio"]->eval(args1_1.data()); 
         e_2_idiso = fns_["e_idiso_embed_ratio"]->eval(args2_1.data()); 
       } else {
         e_1_idiso = fns_["e_idiso_ratio"]->eval(args1_1.data()); 
         e_2_idiso = fns_["e_idiso_ratio"]->eval(args2_1.data());
       }

       weight *= (e_1_idiso * e_2_idiso);
       event->Add("idisoweight_1", e_1_idiso);
       event->Add("idisoweight_2", e_2_idiso);
       event->Add("isoweight_1", double(1.0));
       event->Add("isoweight_2", double(1.0));
      }
 }
 eventInfo->set_weight("lepton", weight);

 if(do_etau_fakerate_) {
     double etau_fakerate_1=1.0;
     double etau_fakerate_2=1.0;
     double etau_fakerate_1_up=1.0;
     double etau_fakerate_1_down=1.0;
     double etau_fakerate_2_up=1.0;
     double etau_fakerate_2_down=1.0;

     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double t_eta = fabs(tau->eta());
     auto t_args = std::vector<double>{t_eta};
     unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
     unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
     if(gm2_==1||gm2_==3){
       if(channel_ == channel::et) {
         etau_fakerate_2 = fns_["t_id_vs_e_eta_tight"]->eval(t_args.data());
         etau_fakerate_2_up = fns_["t_id_vs_e_eta_tight_up"]->eval(t_args.data());
         etau_fakerate_2_down = fns_["t_id_vs_e_eta_tight_down"]->eval(t_args.data());
       }
       else {
         etau_fakerate_2 = fns_["t_id_vs_e_eta_vvloose"]->eval(t_args.data());
         etau_fakerate_2_up = fns_["t_id_vs_e_eta_vvloose_up"]->eval(t_args.data());
         etau_fakerate_2_down = fns_["t_id_vs_e_eta_vvloose_down"]->eval(t_args.data());
       }
     }
     if(channel_ == channel::tt && (gm1_==1||gm1_==3)) {
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
       double t_1_eta = fabs(tau1->eta());
       auto t_1_args = std::vector<double>{t_1_eta};
       etau_fakerate_1 = fns_["t_id_vs_e_eta_vvloose"]->eval(t_1_args.data());
       etau_fakerate_1_up = fns_["t_id_vs_e_eta_vvloose_up"]->eval(t_1_args.data());
       etau_fakerate_1_down = fns_["t_id_vs_e_eta_vvloose_down"]->eval(t_1_args.data());
     }

     eventInfo->set_weight("etau_fakerate",etau_fakerate_1*etau_fakerate_2);
     event->Add("wt_efake_rate_up",etau_fakerate_1_up*etau_fakerate_2_up/(etau_fakerate_1*etau_fakerate_2));
     event->Add("wt_efake_rate_down",etau_fakerate_1_down*etau_fakerate_2_down/(etau_fakerate_1*etau_fakerate_2));

   }
  
   if(do_mtau_fakerate_) {
     double mtau_fakerate_1=1.0;
     double mtau_fakerate_2=1.0;
     double mtau_fakerate_1_up=1.0;
     double mtau_fakerate_1_down=1.0;
     double mtau_fakerate_2_up=1.0;
     double mtau_fakerate_2_down=1.0;

     Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
     double t_eta = fabs(tau->eta());
     auto t_args = std::vector<double>{t_eta};
     unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
     unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
     if (gm2_==2||gm2_==4){
       if(channel_ == channel::mt) {
         mtau_fakerate_2 = fns_["t_id_vs_mu_eta_tight"]->eval(t_args.data());
         mtau_fakerate_2_up = fns_["t_id_vs_mu_eta_tight_up"]->eval(t_args.data());
         mtau_fakerate_2_down = fns_["t_id_vs_mu_eta_tight_down"]->eval(t_args.data());
       }
       else {                       
         mtau_fakerate_2 = fns_["t_id_vs_mu_eta_vloose"]->eval(t_args.data()); 
         mtau_fakerate_2_up = fns_["t_id_vs_mu_eta_vloose_up"]->eval(t_args.data());
         mtau_fakerate_2_down = fns_["t_id_vs_mu_eta_vloose_down"]->eval(t_args.data());
       }
     }
     if(channel_ == channel::tt && (gm1_==2||gm1_==4)) {
       Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
       double t_1_eta = fabs(tau1->eta());
       auto t_1_args = std::vector<double>{t_1_eta};
       mtau_fakerate_1 = fns_["t_id_vs_mu_eta_vloose"]->eval(t_1_args.data()); 
       mtau_fakerate_1_up = fns_["t_id_vs_mu_eta_vloose_up"]->eval(t_1_args.data());
       mtau_fakerate_1_down = fns_["t_id_vs_mu_eta_vloose_down"]->eval(t_1_args.data());
     }

     eventInfo->set_weight("mtau_fakerate",mtau_fakerate_1*mtau_fakerate_2);
     event->Add("wt_mfake_rate_up",mtau_fakerate_1_up*mtau_fakerate_2_up/(mtau_fakerate_1*mtau_fakerate_2));
     event->Add("wt_mfake_rate_down",mtau_fakerate_1_down*mtau_fakerate_2_down/(mtau_fakerate_1*mtau_fakerate_2));

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
