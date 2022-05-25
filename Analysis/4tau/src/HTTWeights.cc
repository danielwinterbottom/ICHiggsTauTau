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
    trg_applied_in_mc_          = false;
    do_tracking_eff_            = false;
    jets_label_                 = "pfJetsPFlow";
    ditau_label_                = "emtauCandidates";
    do_tau_id_sf_               = false;
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
  std::cout << boost::format(param_fmt()) % "do_tau_id_sf"        % do_tau_id_sf_;
  std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
  std::cout << boost::format(param_fmt()) % "ditau_label"         % ditau_label_;
  std::cout << boost::format(param_fmt()) % "scalefactor_file"    % scalefactor_file_;


  if(scalefactor_file_!="") {
    // putting new definitions of all function here for full run2 analyses to avoid the mess/confusion 
    // for now just for 2018 but will update for 2017 and 2016 legacy soon
    TFile f(scalefactor_file_.c_str());
    w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
    f.Close();


    // tracking corrections for electrons and muons
    fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));
    fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));

 
    // triggers for tau legs in et, mt and tt channels
    fns_["t_trg_35_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_pog_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));

    // IC trigger SFs
    fns_["t_trg_35_ic_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_ditau_ratio")->functor(w_->argSet("t_pt,t_dm")));
    fns_["t_trg_35_ic_mvadm_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("t_trg_ic_deeptau_medium_mvadm_ditau_ratio")->functor(w_->argSet("t_pt,t_mvadm")));

    // electron id/iso
    fns_["e_idiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("e_idiso_binned_ic_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso"))); 

    // muon id/iso
     fns_["m_idiso_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_idiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_id_ic_ratio")->functor(w_->argSet("m_pt,m_eta")));
    fns_["m_looseiso_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_looseiso_binned_ic_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
    fns_["m_sel_trg_ratio"] = std::shared_ptr<RooFunctor>(
        w_->function("m_sel_trg_ic_ratio")->functor(w_->argSet("gt1_pt,gt1_eta,gt2_pt,gt2_eta")));

    // tau id
    fns_["t_id_pt_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_pt_tight")->functor(w_->argSet("t_pt")));
    fns_["t_id_dm_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_dm_tight")->functor(w_->argSet("t_dm")));
    fns_["t_id_dm_vloose"] = std::shared_ptr<RooFunctor>(
      w_->function("t_id_dm_vloose")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_pt_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_medium")->functor(w_->argSet("t_pt")));

    fns_["t_deeptauid_pt_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_pt_vvvloose")->functor(w_->argSet("t_pt")));
    fns_["t_deeptauid_dm_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_medium")->functor(w_->argSet("t_dm")));
    fns_["t_deeptauid_dm_vvvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_dm_vvvloose")->functor(w_->argSet("t_dm")));


    // tau id in mva-dm bins
    fns_["t_deeptauid_mvadm_medium"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium")->functor(w_->argSet("t_pt,t_mvadm")));
    fns_["t_deeptauid_mvadm_medium_tightvsele"] = std::shared_ptr<RooFunctor>(
        w_->function("t_deeptauid_mvadm_medium_tightvsele")->functor(w_->argSet("t_pt,t_mvadm")));

    fns_["t_id_vs_e_eta_vvloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_e_eta_vvloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_vloose"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_vloose")->functor(w_->argSet("t_eta")));
    fns_["t_id_vs_mu_eta_tight"] = std::shared_ptr<RooFunctor>(
        w_->function("t_id_vs_mu_eta_tight")->functor(w_->argSet("t_eta")));

  }
      
  return 0;
}

int HTTWeights::Execute(TreeEvent *event) {

  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);
  EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

 /// IDISO SCALE FACTORS
  if (do_tau_id_sf_) {
    double tau_sf_1 = 1.0;
    double tau_sf_2 = 1.0;
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

    double mvadm_1 = tau1->GetTauID("MVADM2017v1");
    double mvadm_2 = tau2->GetTauID("MVADM2017v1");

    auto args_mvadm_1 = std::vector<double>{pt_1,mvadm_1};
    auto args_mvadm_2 = std::vector<double>{pt_2,mvadm_2};

    double tau_sf_mvadm_1=1.;
    double tau_sf_mvadm_2=1.;

    auto args_dm_1 = std::vector<double>{decay_mode_1};
    tau_sf_1 = (gen_match_1==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_dm_1.data()) : 1.0;
    tau_sf_mvadm_1 = (gen_match_1==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm_1.data()) : 1.0;
    auto args_dm_2 = std::vector<double>{decay_mode_2};
    tau_sf_2 = (gen_match_2==5) ? fns_["t_deeptauid_dm_medium"]->eval(args_dm_2.data()) : 1.0;
    tau_sf_mvadm_2 = (gen_match_2==5) ? fns_["t_deeptauid_mvadm_medium"]->eval(args_mvadm_2.data()) : 1.0;
    event->Add("wt_tau_id_mvadm",(tau_sf_mvadm_1*tau_sf_mvadm_2)/(tau_sf_1*tau_sf_2));

    eventInfo->set_weight("wt_tau_id_sf",tau_sf_1*tau_sf_2);
    if(channel_==channel::tt) event->Add("idisoweight_1", tau_sf_1);
    event->Add("idisoweight_2", tau_sf_2);
  }

  /// TRACKING EFFICIENCIES - NEED TO IMPLEMENT ON LEPTONS
  if (do_tracking_eff_) {
      double tracking_wt_1 = 1.0;
      double tracking_wt_2 = 1.0;
    if(channel_ == channel::et){
      Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
      auto args = std::vector<double>{elec->pt(),elec->sc_eta()};
      tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
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
      tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
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
      tracking_wt_1 *= fns_["e_trk_ratio"]->eval(args.data());
      auto args_2 = std::vector<double>{elec_2->pt(),elec_2->sc_eta()};
      tracking_wt_2 *= fns_["e_trk_ratio"]->eval(args_2.data());
     }
     event->Add("trackingweight_1",tracking_wt_1);
     event->Add("trackingweight_2",tracking_wt_2);
     eventInfo->set_weight("wt_tracking_eff",tracking_wt_1*tracking_wt_2);
  }
        

  /// TRIGGER SCALE FACTORS - SETTING UP JUST FOR DOUBLE TAU TO START WITH
  if (do_trg_weights_) {
    if (channel_ == channel::tt){
      Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
      Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      double pt_1 = tau1->pt();
      double pt_2 = tau2->pt();
      double tau1_trg = 1.0;
      double tau1_trg_mc = 1.0;
      double tau2_trg = 1.0;
      double tau2_trg_mc = 1.0;
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

      double mvadm_1 = tau1->GetTauID("MVADM2017v1");
      double mvadm_2 = tau2->GetTauID("MVADM2017v1");

      auto args_mvadm_1 = std::vector<double>{pt_1,mvadm_1};
      auto args_mvadm_2 = std::vector<double>{pt_2,mvadm_2};

      double tau1_trg_ic=1., tau2_trg_ic=1., tau1_trg_ic_mvadm=1., tau2_trg_ic_mvadm=1.;

      tau1_trg = fns_["t_trg_35_ratio"]->eval(args_2_1.data());
      tau1_trg_mc = 1.;
      tau2_trg = fns_["t_trg_35_ratio"]->eval(args_2_2.data());
      tau2_trg_mc = 1.;

      tau1_trg_ic = fns_["t_trg_35_ic_ratio"]->eval(args_2_1.data());
      tau2_trg_ic = fns_["t_trg_35_ic_ratio"]->eval(args_2_2.data());
      tau1_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_ratio"]->eval(args_mvadm_1.data());
      tau2_trg_ic_mvadm = fns_["t_trg_35_ic_mvadm_ratio"]->eval(args_mvadm_2.data());

      double trg_tot = (tau1_trg*tau2_trg)/(tau1_trg_mc*tau2_trg_mc);

      double tau_trg_ic = tau1_trg_ic*tau2_trg_ic;
      double tau_trg_ic_mvadm = tau1_trg_ic_mvadm*tau2_trg_ic_mvadm;

      tau_trg_ic = (trg_tot==0) ? tau_trg_ic : tau_trg_ic/trg_tot;
      tau_trg_ic_mvadm = (trg_tot==0) ? tau_trg_ic_mvadm : tau_trg_ic_mvadm/trg_tot;

      event->Add("wt_tau_trg_ic",tau_trg_ic);
      event->Add("wt_tau_trg_mvadm",tau_trg_ic_mvadm);
         
      if(trg_applied_in_mc_){
        tau1_trg = tau1_trg / tau1_trg_mc;
        tau2_trg = tau2_trg / tau2_trg_mc;
      }
      event->Add("trigweight_1", tau1_trg);
      event->Add("trigweight_2", tau2_trg);
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
