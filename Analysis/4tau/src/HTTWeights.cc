#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTWeights.h"
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
    do_tracking_eff_            = false;
    jets_label_                 = "pfJetsPFlow";
    ditau_label_                = "emtauCandidates";
    do_tau_id_sf_               = false;
    do_ditau_trg_               = false;
    do_etau_fakerate_           = false;
    do_etaucross_trg_           = false;
    do_singlee_trg_             = false;
    do_mtau_fakerate_           = false;
    do_mtaucross_trg_           = false;
    do_singlem_trg_             = false;
    do_emucross_trg_           = false;
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
  std::cout << boost::format(param_fmt()) % "strategy"            % Strategy2String(strategy_);
  std::cout << boost::format(param_fmt()) % "do_trg_weights"      % do_trg_weights_;
  std::cout << boost::format(param_fmt()) % "do_tau_id_sf"        % do_tau_id_sf_;
  std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
  std::cout << boost::format(param_fmt()) % "ditau_label"         % ditau_label_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file"    % scalefactor_file_;


  if(scalefactor_file_!="") {

    TFile f(scalefactor_file_.c_str());
    w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
    f.Close();


    // tracking corrections for electrons and muons
    fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));

 
    // triggers for tau legs in ditau trigger
    fns_["t_trg_35_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));

    // electron id/iso
    fns_["e_idiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_binned_ic_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso"))); 

    // muon id/iso
     fns_["m_idiso_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_idiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));

    // tau id
    fns_["t_deeptauid_dm_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_vvvloose")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_vvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_vvloose")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_vloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_vloose")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_loose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_loose")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_tight")->functor(w_->argSet("t_dm")));

    // lepton fake rates
    fns_["t_id_vs_mu_eta_vloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_vloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_loose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_loose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_medium")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_tight")->functor(w_->argSet("t_eta")));

    fns_["t_id_vs_e_eta_vvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vvloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_vloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_loose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_loose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_medium")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_tight")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_e_eta_vtight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vtight")->functor(w_->argSet("t_eta")));

    // Trigger functions
    fns_["t_doubletau_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_data")->functor(w_->argSet("t_pt,t_dm"))); // double tau leg data
    fns_["t_doubletau_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_mc")->functor(w_->argSet("t_pt,t_dm"))); // double tau leg mc

    fns_["t_mutaucross_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_data")->functor(w_->argSet("t_pt,t_dm"))); // tau leg for mutaucross data
    fns_["t_mutaucross_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_mutau_mc")->functor(w_->argSet("t_pt,t_dm"))); // tau leg for mutaucross mc
    fns_["m_mutaucross_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_23_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // muon leg for mutaucross data
    fns_["m_mutaucross_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_23_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // muon leg for mutaucross mc

    fns_["t_etaucross_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_data")->functor(w_->argSet("t_pt,t_dm")));  // tau leg for etaucross data
    fns_["t_etaucross_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_etau_mc")->functor(w_->argSet("t_pt,t_dm"))); // tau leg for etaucross mc

    if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {
      fns_["e_etaucross_trg_data"] = std::shared_ptr<RooFunctor>(
          w_->function("e_trg_24_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));  // electron leg for etaucross data
      fns_["e_etaucross_trg_mc"] = std::shared_ptr<RooFunctor>(
          w_->function("e_trg_24_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // electron leg for etaucross mc
    }

    fns_["m_singlem_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // single muon mc
    fns_["m_singlem_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trg_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // single muon data
    fns_["e_singlee_trg_mc"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // single electron mc
    fns_["e_singlee_trg_data"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trg_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // single electron data


    fns_["e_high_emucross_trg_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_23_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // high electron leg for emucross mc
    fns_["e_high_emucross_trg_data"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_23_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // high electron leg for emucross data
    fns_["e_low_emucross_trg_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_12_binned_ic_mc")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // low electron leg for emucross mc
    fns_["e_low_emucross_trg_data"] = std::shared_ptr<RooFunctor>(
       w_->function("e_trg_12_binned_ic_data")->functor(w_->argSet("e_pt,e_eta,e_iso"))); // low electron leg for emucross data
    fns_["m_high_emucross_trg_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_23_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // high muon leg for emucross mc
    fns_["m_high_emucross_trg_data"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_23_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // high muon leg for emucross data
    fns_["m_low_emucross_trg_mc"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_8_binned_ic_mc")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // low muon leg for emucross mc
    fns_["m_low_emucross_trg_data"] = std::shared_ptr<RooFunctor>(
       w_->function("m_trg_8_binned_ic_data")->functor(w_->argSet("m_pt,m_eta,m_iso"))); // low muon leg for emucross data

  }
      
  return 0;
}

int HTTWeights::Execute(TreeEvent *event) {

  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);
  EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

  std::string po = Channel2String(channel_);
  std::string e_string = "e";
  std::string m_string = "m";
  std::string t_string = "t";



  /// IDISO SCALE FACTORS
  if (do_tau_id_sf_) {
    double total_id_sf = 1.0;
    double total_etau_fakerate = 1.0;
    double total_mtau_fakerate = 1.0;
    for (int i = 0; i < 4; i = i + 1) {
      double id_sf = 1.0;
      double id_sf_vvvloose = 1.0;
      double id_sf_vvloose = 1.0;
      double id_sf_vloose = 1.0;
      double id_sf_loose = 1.0;
      double id_sf_medium = 1.0;
      double id_sf_tight = 1.0;

      double etau_fakerate = 1.0;
      double etau_fakerate_vvloose = 1.0;
      double etau_fakerate_vloose = 1.0;
      double etau_fakerate_loose = 1.0;
      double etau_fakerate_medium = 1.0;
      double etau_fakerate_tight = 1.0;
      double etau_fakerate_vtight = 1.0;

      double mtau_fakerate = 1.0;
      double mtau_fakerate_vloose = 1.0;
      double mtau_fakerate_loose = 1.0;
      double mtau_fakerate_medium = 1.0;
      double mtau_fakerate_tight = 1.0;

      int pn = i + 1; // define to use to get particle information

      if ( e_string == po[i] ) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_id_sf = std::vector<double>{elec->pt(),elec->sc_eta(),PF03EAIsolationVal(elec, eventInfo->jet_rho())};
        id_sf = fns_["e_idiso_ratio"]->eval(args_id_sf.data());
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
        id_sf = (gen_match==5) ? fns_["t_deeptauid_dm_vvvloose"]->eval(args_id_sf.data()) : 1.0;
        id_sf_vvvloose = (gen_match==5) ? fns_["t_deeptauid_dm_vvvloose"]->eval(args_id_sf.data()) : 1.0;
        id_sf_vvloose = (gen_match==5) ? fns_["t_deeptauid_dm_vvloose"]->eval(args_id_sf.data()) : 1.0;
        id_sf_vloose = (gen_match==5) ? fns_["t_deeptauid_dm_vloose"]->eval(args_id_sf.data()) : 1.0;
        id_sf_loose = (gen_match==5) ? fns_["t_deeptauid_dm_loose"]->eval(args_id_sf.data()) : 1.0;
        id_sf_medium = (gen_match==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_id_sf.data()) : 1.0;
        id_sf_tight = (gen_match==5) ? fns_["t_deeptauid_dm_tight"]->eval(args_id_sf.data()) : 1.0;

        if(do_etau_fakerate_) {
          auto args_etau_fakerate = std::vector<double>{fabs(tau->eta())};
          etau_fakerate = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vvloose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vvloose = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vvloose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vloose = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vloose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_loose = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_loose"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_medium = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_medium"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_tight = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_tight"]->eval(args_etau_fakerate.data()) : 1.0;
          etau_fakerate_vtight = (gen_match==1 || gen_match==3) ? fns_["t_id_vs_e_eta_vtight"]->eval(args_etau_fakerate.data()) : 1.0;
        }
        if(do_mtau_fakerate_) {
          auto args_mtau_fakerate = std::vector<double>{fabs(tau->eta())};
          mtau_fakerate = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_vloose"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_vloose = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_vloose"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_loose = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_loose"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_medium = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_medium"]->eval(args_mtau_fakerate.data()) : 1.0;
          mtau_fakerate_tight = (gen_match==2 || gen_match==4) ? fns_["t_id_vs_mu_eta_tight"]->eval(args_mtau_fakerate.data()) : 1.0;
        }
      }
      event->Add("idisoweight_"+std::to_string(pn), id_sf);
      event->Add("idisoweight_vvvloose_"+std::to_string(pn), id_sf_vvvloose);
      event->Add("idisoweight_vvloose_"+std::to_string(pn), id_sf_vvloose);
      event->Add("idisoweight_vloose_"+std::to_string(pn), id_sf_vloose);
      event->Add("idisoweight_loose_"+std::to_string(pn), id_sf_loose);
      event->Add("idisoweight_medium_"+std::to_string(pn), id_sf_medium);
      event->Add("idisoweight_tight_"+std::to_string(pn), id_sf_tight);

      event->Add("etau_fakerate_"+std::to_string(pn), etau_fakerate);
      event->Add("etau_fakerate_vvloose_"+std::to_string(pn), etau_fakerate_vvloose);
      event->Add("etau_fakerate_vloose_"+std::to_string(pn), etau_fakerate_vloose);
      event->Add("etau_fakerate_loose_"+std::to_string(pn), etau_fakerate_loose);
      event->Add("etau_fakerate_medium_"+std::to_string(pn), etau_fakerate_medium);
      event->Add("etau_fakerate_tight_"+std::to_string(pn), etau_fakerate_tight);
      event->Add("etau_fakerate_vtight_"+std::to_string(pn), etau_fakerate_vtight);

      event->Add("mtau_fakerate_"+std::to_string(pn), mtau_fakerate);
      event->Add("mtau_fakerate_vloose_"+std::to_string(pn), mtau_fakerate_vloose);
      event->Add("mtau_fakerate_loose_"+std::to_string(pn), mtau_fakerate_loose);
      event->Add("mtau_fakerate_medium_"+std::to_string(pn), mtau_fakerate_medium);
      event->Add("mtau_fakerate_tight_"+std::to_string(pn), mtau_fakerate_tight);

      total_id_sf *= id_sf;
      total_etau_fakerate *= etau_fakerate;
      total_mtau_fakerate *= mtau_fakerate;
      if (channel_ == channel::ttt && pn == 3) break;
    }
    eventInfo->set_weight("wt_tau_id_sf",total_id_sf);
    eventInfo->set_weight("wt_etau_fakerate",total_etau_fakerate); 
    eventInfo->set_weight("wt_mtau_fakerate",total_mtau_fakerate);
  }


  /// TRACKING EFFICIENCIES
  if (do_tracking_eff_) {
    double total_trk_sf = 1.0;
    for (int i = 0; i < 4; i = i + 1) {
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
      if (channel_ == channel::ttt && pn  == 3) break;
    }
    eventInfo->set_weight("wt_tracking_sf",total_trk_sf);
  }

  /// TRIGGER SCALE FACTORS
  if (do_trg_weights_) {
    for (int i = 0; i < 4; i = i + 1) {
      int pn = i + 1; // define to use to get particle information
      if ( e_string == po[i] ) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_e = std::vector<double>{elec->pt(),elec->eta(),PF03EAIsolationVal(elec, eventInfo->jet_rho())};
        if (do_singlee_trg_){
          event->Add("trigeff_e_singlee_data_"+std::to_string(pn), fns_["e_singlee_trg_data"]->eval(args_e.data()));
          event->Add("trigeff_e_singlee_mc_"+std::to_string(pn), fns_["e_singlee_trg_mc"]->eval(args_e.data()));
        }
        if (do_etaucross_trg_){
          if(mc_ == mc::mc2017 || mc_ == mc::mc2018) {
            event->Add("trigeff_e_etaucross_data_"+std::to_string(pn), fns_["e_etaucross_trg_data"]->eval(args_e.data()));
            event->Add("trigeff_e_etaucross_mc_"+std::to_string(pn), fns_["e_etaucross_trg_mc"]->eval(args_e.data()));
          }
        }
        if (do_emucross_trg_){
          event->Add("trigeff_e_high_emucross_data_"+std::to_string(pn), fns_["e_high_emucross_trg_data"]->eval(args_e.data()));
          event->Add("trigeff_e_high_emucross_mc_"+std::to_string(pn), fns_["e_high_emucross_trg_mc"]->eval(args_e.data()));
          event->Add("trigeff_e_low_emucross_data_"+std::to_string(pn), fns_["e_low_emucross_trg_data"]->eval(args_e.data()));
          event->Add("trigeff_e_low_emucross_mc_"+std::to_string(pn), fns_["e_low_emucross_trg_mc"]->eval(args_e.data()));
        }
      }
      if ( m_string == po[i] ) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_m = std::vector<double>{muon->pt(),muon->eta(),PF03IsolationVal(muon,0.5,0)};
        if (do_singlem_trg_){
          event->Add("trigeff_m_singlem_data_"+std::to_string(pn), fns_["m_singlem_trg_data"]->eval(args_m.data()));
          event->Add("trigeff_m_singlem_mc_"+std::to_string(pn), fns_["m_singlem_trg_mc"]->eval(args_m.data()));
        }
        if (do_mtaucross_trg_){
          event->Add("trigeff_m_mutaucross_data_"+std::to_string(pn), fns_["m_mutaucross_trg_data"]->eval(args_m.data()));
          event->Add("trigeff_m_mutaucross_mc_"+std::to_string(pn), fns_["m_mutaucross_trg_mc"]->eval(args_m.data()));
        }
        if (do_emucross_trg_){
          event->Add("trigeff_m_high_emucross_data_"+std::to_string(pn), fns_["m_high_emucross_trg_data"]->eval(args_m.data()));
          event->Add("trigeff_m_high_emucross_mc_"+std::to_string(pn), fns_["m_high_emucross_trg_mc"]->eval(args_m.data()));
          event->Add("trigeff_m_low_emucross_data_"+std::to_string(pn), fns_["m_low_emucross_trg_data"]->eval(args_m.data()));
          event->Add("trigeff_m_low_emucross_mc_"+std::to_string(pn), fns_["m_low_emucross_trg_mc"]->eval(args_m.data()));
        }
      }
      if ( t_string == po[i] ) {
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_t = std::vector<double>{tau->pt(),static_cast<double>(tau->decay_mode())};
        if (do_ditau_trg_){
          event->Add("trigeff_t_doubletau_data_"+std::to_string(pn), fns_["t_doubletau_trg_data"]->eval(args_t.data()));
          event->Add("trigeff_t_doubletau_mc_"+std::to_string(pn), fns_["t_doubletau_trg_mc"]->eval(args_t.data()));
        }
        if (do_mtaucross_trg_){
          event->Add("trigeff_t_mutaucross_data_"+std::to_string(pn), fns_["t_mutaucross_trg_data"]->eval(args_t.data()));
          event->Add("trigeff_t_mutaucross_mc_"+std::to_string(pn), fns_["t_mutaucross_trg_mc"]->eval(args_t.data()));
        }
        if (do_etaucross_trg_){
          event->Add("trigeff_t_etaucross_data_"+std::to_string(pn), fns_["t_etaucross_trg_data"]->eval(args_t.data()));
          event->Add("trigeff_t_etaucross_mc_"+std::to_string(pn), fns_["t_etaucross_trg_mc"]->eval(args_t.data()));
        }
      }
      if (channel_ == channel::ttt && pn ==3) break;
    }

  }
  double total_trg = 0.;
  double d1 = 0.; 
  double d2 = 0.; 
  double d3 = 0.; 
  double d4 = 0.; 
  double m1 = 0.; 
  double m2 = 0.; 
  double m3 = 0.; 
  double m4 = 0.; 

  if ( e_string == po[0] ) {
    d1 = event->Exists("trigeff_e_singlee_data_1") ? event->Get<double>("trigeff_e_singlee_data_1") : 1.0;
    m1 = event->Exists("trigeff_e_singlee_mc_1") ? event->Get<double>("trigeff_e_singlee_mc_1") : 1.0;
  } else if ( m_string == po[0] ) {
    d1 = event->Exists("trigeff_m_singlem_data_1") ? event->Get<double>("trigeff_m_singlem_data_1") : 1.0;
    m1 = event->Exists("trigeff_m_singlem_mc_1") ? event->Get<double>("trigeff_m_singlem_mc_1") : 1.0;
  } else if ( t_string == po[0] ) {
    d1 = event->Exists("trigeff_t_doubletau_data_1") ? event->Get<double>("trigeff_t_doubletau_data_1") : 1.0;
    m1 = event->Exists("trigeff_t_doubletau_mc_1") ? event->Get<double>("trigeff_t_doubletau_mc_1") : 1.0;
  }

  if ( e_string == po[1] ) {
    d2 = event->Exists("trigeff_e_singlee_data_2") ? event->Get<double>("trigeff_e_singlee_data_2") : 1.0;
    m2 = event->Exists("trigeff_e_singlee_mc_2") ? event->Get<double>("trigeff_e_singlee_mc_2") : 1.0;
  } else if ( m_string == po[1] ) {
    d2 = event->Exists("trigeff_m_singlem_data_2") ? event->Get<double>("trigeff_m_singlem_data_2") : 1.0;
    m2 = event->Exists("trigeff_m_singlem_mc_2") ? event->Get<double>("trigeff_m_singlem_mc_2") : 1.0;
  } else if ( t_string == po[1] ) {
    d2 = event->Exists("trigeff_t_doubletau_data_2") ? event->Get<double>("trigeff_t_doubletau_data_2") : 1.0;
    m2 = event->Exists("trigeff_t_doubletau_mc_2") ? event->Get<double>("trigeff_t_doubletau_mc_2") : 1.0;
  }

  if ( e_string == po[2] ) {
    d3 = event->Exists("trigeff_e_singlee_data_3") ? event->Get<double>("trigeff_e_singlee_data_3") : 1.0;
    m3 = event->Exists("trigeff_e_singlee_mc_3") ? event->Get<double>("trigeff_e_singlee_mc_3") : 1.0;
  } else if ( m_string == po[2] ) {
    d3 = event->Exists("trigeff_m_singlem_data_3") ? event->Get<double>("trigeff_m_singlem_data_3") : 1.0;
    m3 = event->Exists("trigeff_m_singlem_mc_3") ? event->Get<double>("trigeff_m_singlem_mc_3") : 1.0;
  } else if ( t_string == po[2] ) {
    d3 = event->Exists("trigeff_t_doubletau_data_3") ? event->Get<double>("trigeff_t_doubletau_data_3") : 1.0;
    m3 = event->Exists("trigeff_t_doubletau_mc_3") ? event->Get<double>("trigeff_t_doubletau_mc_3") : 1.0;
  }

  if (channel_ != channel::ttt) {
    if ( e_string == po[3] ) {
      d4 = event->Exists("trigeff_e_singlee_data_4") ? event->Get<double>("trigeff_e_singlee_data_4") : 1.0;
      m4 = event->Exists("trigeff_e_singlee_mc_4") ? event->Get<double>("trigeff_e_singlee_mc_4") : 1.0;
    } else if ( m_string == po[3] ) {
      d4 = event->Exists("trigeff_m_singlem_data_4") ? event->Get<double>("trigeff_m_singlem_data_4") : 1.0;
      m4 = event->Exists("trigeff_m_singlem_mc_4") ? event->Get<double>("trigeff_m_singlem_mc_4") : 1.0;
    } else if ( t_string == po[3] ) {
      d4 = event->Exists("trigeff_t_doubletau_data_4") ? event->Get<double>("trigeff_t_doubletau_data_4") : 1.0;
      m4 = event->Exists("trigeff_t_doubletau_mc_4") ? event->Get<double>("trigeff_t_doubletau_mc_4") : 1.0;
    }
  }

  if (channel_ == channel::ttt) {
    total_trg = (((d1*d2) + (d1*d3) + (d2*d3) - ((d1*d2)*(d1*d3)) - ((d1*d2)*(d2*d3)) - ((d1*d3)*(d2*d3)) + ((d1*d2)*(d1*d3)*(d2*d3))) / ((m1*m2) + (m1*m3) + (m2*m3) - ((m1*m2)*(m1*m3)) - ((m1*m2)*(m2*m3)) - ((m1*m3)*(m2*m3)) + ((m1*m2)*(m1*m3)*(m2*m3))));
  } else if (channel_ == channel::tttt) {
    total_trg = (((d1*d2) + (d1*d3) + (d1*d4) + (d2*d3) + (d2*d4) + (d3*d4) - ((d1*d2)*(d1*d3)) - ((d1*d2)*(d1*d4)) - ((d1*d2)*(d2*d3)) - ((d1*d2)*(d2*d4)) - ((d1*d2)*(d3*d4)) - ((d1*d3)*(d1*d4)) - ((d1*d3)*(d2*d3)) - ((d1*d3)*(d2*d4)) - ((d1*d3)*(d3*d4)) - ((d1*d4)*(d2*d3)) - ((d1*d4)*(d2*d4)) - ((d1*d4)*(d3*d4)) - ((d2*d3)*(d2*d4)) - ((d2*d3)*(d3*d4)) - ((d2*d4)*(d3*d4)) + ((d1*d2)*(d1*d3)*(d1*d4)) + ((d1*d2)*(d1*d3)*(d2*d3)) + ((d1*d2)*(d1*d3)*(d2*d4)) + ((d1*d2)*(d1*d3)*(d3*d4)) + ((d1*d2)*(d1*d4)*(d2*d3)) + ((d1*d2)*(d1*d4)*(d2*d4)) + ((d1*d2)*(d1*d4)*(d3*d4)) + ((d1*d2)*(d2*d3)*(d2*d4)) + ((d1*d2)*(d2*d3)*(d3*d4)) + ((d1*d2)*(d2*d4)*(d3*d4)) + ((d1*d3)*(d1*d4)*(d2*d3)) + ((d1*d3)*(d1*d4)*(d2*d4)) + ((d1*d3)*(d1*d4)*(d3*d4)) + ((d1*d3)*(d2*d3)*(d2*d4)) + ((d1*d3)*(d2*d3)*(d3*d4)) + ((d1*d3)*(d2*d4)*(d3*d4)) + ((d1*d4)*(d2*d3)*(d2*d4)) + ((d1*d4)*(d2*d3)*(d3*d4)) + ((d1*d4)*(d2*d4)*(d3*d4)) + ((d2*d3)*(d2*d4)*(d3*d4)) - ((d1*d2)*(d1*d3)*(d1*d4)*(d2*d3)) - ((d1*d2)*(d1*d3)*(d1*d4)*(d2*d4)) - ((d1*d2)*(d1*d3)*(d1*d4)*(d3*d4)) - ((d1*d2)*(d1*d3)*(d2*d3)*(d2*d4)) - ((d1*d2)*(d1*d3)*(d2*d3)*(d3*d4)) - ((d1*d2)*(d1*d3)*(d2*d4)*(d3*d4)) - ((d1*d2)*(d1*d4)*(d2*d3)*(d2*d4)) - ((d1*d2)*(d1*d4)*(d2*d3)*(d3*d4)) - ((d1*d2)*(d1*d4)*(d2*d4)*(d3*d4)) - ((d1*d2)*(d2*d3)*(d2*d4)*(d3*d4)) - ((d1*d3)*(d1*d4)*(d2*d3)*(d2*d4)) - ((d1*d3)*(d1*d4)*(d2*d3)*(d3*d4)) - ((d1*d3)*(d1*d4)*(d2*d4)*(d3*d4)) - ((d1*d3)*(d2*d3)*(d2*d4)*(d3*d4)) - ((d1*d4)*(d2*d3)*(d2*d4)*(d3*d4)) + ((d1*d2)*(d1*d3)*(d1*d4)*(d2*d3)*(d2*d4)) + ((d1*d2)*(d1*d3)*(d1*d4)*(d2*d3)*(d3*d4)) + ((d1*d2)*(d1*d3)*(d1*d4)*(d2*d4)*(d3*d4)) + ((d1*d2)*(d1*d3)*(d2*d3)*(d2*d4)*(d3*d4)) + ((d1*d2)*(d1*d4)*(d2*d3)*(d2*d4)*(d3*d4)) + ((d1*d3)*(d1*d4)*(d2*d3)*(d2*d4)*(d3*d4)) - ((d1*d2)*(d1*d3)*(d1*d4)*(d2*d3)*(d2*d4)*(d3*d4))) / ((m1*m2) + (m1*m3) + (m1*m4) + (m2*m3) + (m2*m4) + (m3*m4) - ((m1*m2)*(m1*m3)) - ((m1*m2)*(m1*m4)) - ((m1*m2)*(m2*m3)) - ((m1*m2)*(m2*m4)) - ((m1*m2)*(m3*m4)) - ((m1*m3)*(m1*m4)) - ((m1*m3)*(m2*m3)) - ((m1*m3)*(m2*m4)) - ((m1*m3)*(m3*m4)) - ((m1*m4)*(m2*m3)) - ((m1*m4)*(m2*m4)) - ((m1*m4)*(m3*m4)) - ((m2*m3)*(m2*m4)) - ((m2*m3)*(m3*m4)) - ((m2*m4)*(m3*m4)) + ((m1*m2)*(m1*m3)*(m1*m4)) + ((m1*m2)*(m1*m3)*(m2*m3)) + ((m1*m2)*(m1*m3)*(m2*m4)) + ((m1*m2)*(m1*m3)*(m3*m4)) + ((m1*m2)*(m1*m4)*(m2*m3)) + ((m1*m2)*(m1*m4)*(m2*m4)) + ((m1*m2)*(m1*m4)*(m3*m4)) + ((m1*m2)*(m2*m3)*(m2*m4)) + ((m1*m2)*(m2*m3)*(m3*m4)) + ((m1*m2)*(m2*m4)*(m3*m4)) + ((m1*m3)*(m1*m4)*(m2*m3)) + ((m1*m3)*(m1*m4)*(m2*m4)) + ((m1*m3)*(m1*m4)*(m3*m4)) + ((m1*m3)*(m2*m3)*(m2*m4)) + ((m1*m3)*(m2*m3)*(m3*m4)) + ((m1*m3)*(m2*m4)*(m3*m4)) + ((m1*m4)*(m2*m3)*(m2*m4)) + ((m1*m4)*(m2*m3)*(m3*m4)) + ((m1*m4)*(m2*m4)*(m3*m4)) + ((m2*m3)*(m2*m4)*(m3*m4)) - ((m1*m2)*(m1*m3)*(m1*m4)*(m2*m3)) - ((m1*m2)*(m1*m3)*(m1*m4)*(m2*m4)) - ((m1*m2)*(m1*m3)*(m1*m4)*(m3*m4)) - ((m1*m2)*(m1*m3)*(m2*m3)*(m2*m4)) - ((m1*m2)*(m1*m3)*(m2*m3)*(m3*m4)) - ((m1*m2)*(m1*m3)*(m2*m4)*(m3*m4)) - ((m1*m2)*(m1*m4)*(m2*m3)*(m2*m4)) - ((m1*m2)*(m1*m4)*(m2*m3)*(m3*m4)) - ((m1*m2)*(m1*m4)*(m2*m4)*(m3*m4)) - ((m1*m2)*(m2*m3)*(m2*m4)*(m3*m4)) - ((m1*m3)*(m1*m4)*(m2*m3)*(m2*m4)) - ((m1*m3)*(m1*m4)*(m2*m3)*(m3*m4)) - ((m1*m3)*(m1*m4)*(m2*m4)*(m3*m4)) - ((m1*m3)*(m2*m3)*(m2*m4)*(m3*m4)) - ((m1*m4)*(m2*m3)*(m2*m4)*(m3*m4)) + ((m1*m2)*(m1*m3)*(m1*m4)*(m2*m3)*(m2*m4)) + ((m1*m2)*(m1*m3)*(m1*m4)*(m2*m3)*(m3*m4)) + ((m1*m2)*(m1*m3)*(m1*m4)*(m2*m4)*(m3*m4)) + ((m1*m2)*(m1*m3)*(m2*m3)*(m2*m4)*(m3*m4)) + ((m1*m2)*(m1*m4)*(m2*m3)*(m2*m4)*(m3*m4)) + ((m1*m3)*(m1*m4)*(m2*m3)*(m2*m4)*(m3*m4)) - ((m1*m2)*(m1*m3)*(m1*m4)*(m2*m3)*(m2*m4)*(m3*m4))));
  } else if (channel_ == channel::ettt || channel_ == channel::mttt) {
    total_trg = ((d1 + (d2*d3) + (d2*d4) + (d3*d4) - (d1*(d2*d3)) - (d1*(d2*d4)) - (d1*(d3*d4)) - ((d2*d3)*(d2*d4)) - ((d2*d3)*(d3*d4)) - ((d2*d4)*(d3*d4)) + (d1*(d2*d3)*(d2*d4)) + (d1*(d2*d3)*(d3*d4)) + (d1*(d2*d4)*(d3*d4)) + ((d2*d3)*(d2*d4)*(d3*d4)) - (d1*(d2*d3)*(d2*d4)*(d3*d4))) / (m1 + (m2*m3) + (m2*m4) + (m3*m4) - (m1*(m2*m3)) - (m1*(m2*m4)) - (m1*(m3*m4)) - ((m2*m3)*(m2*m4)) - ((m2*m3)*(m3*m4)) - ((m2*m4)*(m3*m4)) + (m1*(m2*m3)*(m2*m4)) + (m1*(m2*m3)*(m3*m4)) + (m1*(m2*m4)*(m3*m4)) + ((m2*m3)*(m2*m4)*(m3*m4)) - (m1*(m2*m3)*(m2*m4)*(m3*m4))));
  } else if (channel_ == channel::emtt || channel_ == channel::eett || channel_ == channel::mmtt) {
    total_trg = ((d1 + d2 + (d3*d4) - (d1*d2) - (d1*(d3*d4)) - (d2*(d3*d4)) + (d1*d2*(d3*d4))) / (m1 + m2 + (m3*m4) - (m1*m2) - (m1*(m3*m4)) - (m2*(m3*m4)) + (m1*m2*(m3*m4))));
  } else if (channel_ == channel::mmmm) {
    total_trg = ((d1 + d2 + d3 + d4 - (d1*d2) - (d1*d3) - (d1*d4) - (d2*d3) - (d2*d4) - (d3*d4) + (d1*d2*d3) + (d1*d2*d4) + (d1*d3*d4) + (d2*d3*d4) - (d1*d2*d3*d4)) / (m1 + m2 + m3 + m4 - (m1*m2) - (m1*m3) - (m1*m4) - (m2*m3) - (m2*m4) - (m3*m4) + (m1*m2*m3) + (m1*m2*m4) + (m1*m3*m4) + (m2*m3*m4) - (m1*m2*m3*m4)));
  }

  event->Add("wt_total_trg", total_trg);
  eventInfo->set_weight("wt_total_trg",total_trg);

  return 0;
}

  int HTTWeights::PostAnalysis() {
    return 0;
  }

  void HTTWeights::PrintInfo() {
    ;
  }

}
