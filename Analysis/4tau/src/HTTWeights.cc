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
    do_mtau_fakerate_           = false;
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
  }
      
  return 0;
}

int HTTWeights::Execute(TreeEvent *event) {

  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);
  EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

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

      std::string po = Channel2String(channel_);
      std::string e_string = "e";
      std::string m_string = "m";
      std::string t_string = "t";

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
      if ( strcmp(&Channel2String(channel_)[i],"e") == 0 ) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_trk_sf = std::vector<double>{elec->pt(),elec->sc_eta()};
        trk_sf *= fns_["e_trk_ratio"]->eval(args_trk_sf.data());
      }
      if ( strcmp(&Channel2String(channel_)[i],"m") == 0 ) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton"+std::to_string(pn)));
        auto args_trk_sf = std::vector<double>{muon->eta()};
        trk_sf *= fns_["m_trk_ratio"]->eval(args_trk_sf.data());
      }
      event->Add("trackingweight_"+std::to_string(pn),trk_sf);
      total_trk_sf *= trk_sf;
    }
    eventInfo->set_weight("wt_tracking_sf",total_trk_sf);
  }

  /// TRIGGER SCALE FACTORS - SETTING UP JUST FOR DOUBLE TAU TO START WITH HIGHEST PT TAUS IN CHANNELS WITH >2 HADRONIC TAUS
  if (do_trg_weights_) {
    if (do_ditau_trg_){
      Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
      Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      if (channel_== channel::tttt) {
        tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
        tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      }
      if (channel_== channel::ettt || channel_== channel::mttt) {
        tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton3"));
      }
      if (channel_== channel::eett || channel_== channel::mmtt || channel_== channel::emtt) {
        tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton3"));
        tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton4"));
      }

      auto args_tau_1 = std::vector<double>{tau1->pt(),static_cast<double>(tau1->decay_mode())};
      auto args_tau_2 = std::vector<double>{tau2->pt(),static_cast<double>(tau2->decay_mode())};

      double tau1_trg = fns_["t_trg_35_ratio"]->eval(args_tau_1.data());
      double tau2_trg = fns_["t_trg_35_ratio"]->eval(args_tau_2.data());

      double trg_tot = (tau1_trg*tau2_trg);

      event->Add("trigweight_tau_1", tau1_trg);
      event->Add("trigweight_tau_2", tau2_trg);
      eventInfo->set_weight("wt_trigger_sf",trg_tot);
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
