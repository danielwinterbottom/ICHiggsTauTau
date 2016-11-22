#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTWeights.h"
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
    mc_(mc::spring16_80X),
    era_(era::data_2016) {
    do_trg_weights_           = false;
    trg_applied_in_mc_        = false;
    do_single_lepton_trg_     = false;
    do_cross_trg_             = false;
    do_singlemu_trg_weights_  = false;
    do_etau_fakerate_         = false;
    do_mtau_fakerate_         = false;
    do_idiso_weights_         = false;
    do_id_weights_            = false;
    do_emu_e_fakerates_       = false;
    do_emu_m_fakerates_       = false;
    do_top_factors_           = false;
    do_btag_weight_           = false;
    do_zpt_weight_            = false;
    do_tracking_eff_          = false;
    btag_mode_                = 0;
    bfake_mode_               = 0;
    tt_trg_iso_mode_          = 0;
    ggh_mass_                 = "";
    ggh_hist_                 = nullptr;
    ggh_hist_up_              = nullptr;
    ggh_hist_down_            = nullptr;
    do_tau_mode_scale_        = false;
    do_topquark_weights_      = false;
    do_top_jeteta_weights_    = false;
    do_tau_fake_weights_      = false;
    do_tt_muon_weights_       = false;
    do_em_qcd_weights_        = false;
    gen_tau_collection_       = "genParticlesTaus";
    jets_label_               = "pfJetsPFlow";
    btag_label_         = "combinedSecondaryVertexBJetTags";
    ditau_label_              = "emtauCandidates";
    z_pt_mass_hist_            = nullptr;
    mt_idiso_mc_              = nullptr;     
    mt_idiso_data_            = nullptr;     
    et_idiso_mc_              = nullptr;     
    et_idiso_data_            = nullptr;     
    em_m_idiso_mc_            = nullptr;     
    em_m_idiso_data_          = nullptr;     
    em_e_idiso_mc_            = nullptr;     
    em_e_idiso_data_          = nullptr;     
    em_m17_trig_mc_           = nullptr;
    em_m17_trig_data_         = nullptr;
    em_m8_trig_mc_            = nullptr;
    em_m8_trig_data_          = nullptr;
    em_e17_trig_mc_           = nullptr;
    em_e17_trig_data_         = nullptr;
    em_e12_trig_mc_           = nullptr;
    em_e12_trig_data_         = nullptr;
    et_trig_mc_               = nullptr;
    et_trig_data_             = nullptr;
    et_antiiso1_trig_data_    = nullptr;
    et_antiiso2_trig_data_    = nullptr;
    et_xtrig_mc_              = nullptr;
    et_xtrig_data_            = nullptr;
    et_conditional_data_      = nullptr;
    et_conditional_mc_        = nullptr;
    mt_trig_mc_               = nullptr;
    mt_trig_data_             = nullptr;
    mt_antiiso1_trig_data_    = nullptr;
    mt_antiiso2_trig_data_    = nullptr;
    mt_xtrig_mc_              = nullptr;
    mt_xtrig_data_            = nullptr;
    mt_conditional_data_      = nullptr;
    mt_conditional_mc_      = nullptr;
    em_qcd_cr1_lt2_           = nullptr;
    em_qcd_cr1_2to4_          = nullptr;
    em_qcd_cr1_gt4_           = nullptr;
    em_qcd_cr2_lt2_           = nullptr;
    em_qcd_cr2_2to4_          = nullptr;
    em_qcd_cr2_gt4_           = nullptr;
    ele_tracking_sf_          = nullptr;
    muon_tracking_sf_         = nullptr;
    scalefactor_file_         = "";
    do_tau_id_sf_             = false;
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
    std::cout << boost::format(param_fmt()) % "do_trg_weights"      % do_trg_weights_;
    if (do_trg_weights_) {
      std::cout << boost::format(param_fmt()) % "trg_applied_in_mc" % trg_applied_in_mc_;
    }
    std::cout << boost::format(param_fmt()) % "do_idiso_weights"    % do_idiso_weights_;
    std::cout << boost::format(param_fmt()) % "do_id_weights"       % do_id_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_mode_scale"   % do_tau_mode_scale_;
    std::cout << boost::format(param_fmt()) % "do_etau_fakerate"    % do_etau_fakerate_;
    std::cout << boost::format(param_fmt()) % "do_mtau_fakerate"    % do_mtau_fakerate_;
    std::cout << boost::format(param_fmt()) % "do_emu_e_fakerates"  % do_emu_e_fakerates_;
    std::cout << boost::format(param_fmt()) % "do_emu_m_fakerates"  % do_emu_m_fakerates_;
    std::cout << boost::format(param_fmt()) % "do_top_factors"      % do_top_factors_;
    std::cout << boost::format(param_fmt()) % "do_btag_weight"      % do_btag_weight_;
    std::cout << boost::format(param_fmt()) % "do_zpt_weight"       % do_zpt_weight_;
    std::cout << boost::format(param_fmt()) % "btag_mode"           % btag_mode_;
    std::cout << boost::format(param_fmt()) % "bfake_mode"          % bfake_mode_;
    std::cout << boost::format(param_fmt()) % "do_topquark_weights" % do_topquark_weights_;
    std::cout << boost::format(param_fmt()) % "do_top_jeteta_weights" % do_top_jeteta_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_fake_weights" % do_tau_fake_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_id_weights"   % do_tau_id_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_id_sf"        % do_tau_id_sf_;
    std::cout << boost::format(param_fmt()) % "do_em_qcd_weights"   % do_em_qcd_weights_;
    std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
    std::cout << boost::format(param_fmt()) % "btag_label"          % btag_label_;
    std::cout << boost::format(param_fmt()) % "ditau_label"         % ditau_label_;

    if (do_tau_fake_weights_) {
     tau_fake_weights_ = new TF1("tau_fake_weights","(1.15743)-(0.00736136*x)+(4.3699e-05*x*x)-(1.188e-07*x*x*x)",0,200); 
    }


    if (do_emu_e_fakerates_ || do_emu_m_fakerates_) {
      std::string electron_fr_file, muon_fr_file;
      electron_fr_file  = "input/emu_fakerate/ElectronFakeRate_2011.root";
      muon_fr_file      = "input/emu_fakerate/MuonFakeRate_2011.root";
      std::cout << boost::format(param_fmt()) % "electron_fr_file"  % electron_fr_file;
      std::cout << boost::format(param_fmt()) % "muon_fr_file"      % muon_fr_file;
      ElectronFRFile = new TFile(electron_fr_file.c_str());
      MuonFRFile = new TFile(muon_fr_file.c_str());
      ElectronFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(ElectronFRFile->Get("ElectronFakeRateDenominatorV4_Ele8CaloIdLCaloIsoVLCombinedSample_ptThreshold35_PtEta"));
      MuonFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(MuonFRFile->Get("MuonFakeRateDenominatorV6_Mu8PtCombinedSample_ptThreshold25_PtEta"));        
      ElectronFakeRateHist_PtEta->SetDirectory(0);
      MuonFakeRateHist_PtEta->SetDirectory(0);
    }
    if(scalefactor_file_!="") {
        TFile f(scalefactor_file_.c_str());
        w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
        f.Close();
        if(do_trg_weights_ || do_idiso_weights_) {
          fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_id_ratio")->functor(w_->argSet("m_pt,m_eta")));
          fns_["m_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_iso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
          fns_["m_trg_binned_data"] = std::shared_ptr<RooFunctor>(
             w_->function("m_trg_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
          fns_["m_trgOR_binned_data"] = std::shared_ptr<RooFunctor>(
             w_->function("m_trgOR_binned_data")->functor(w_->argSet("m_pt,m_eta,m_iso")));
          fns_["m_idiso0p15_desy_ratio"] = std::shared_ptr<RooFunctor>(
             w_->function("m_idiso0p15_desy_ratio")->functor(w_->argSet("m_pt,m_eta")));
          fns_["m_idiso0p20_desy_ratio"] = std::shared_ptr<RooFunctor>(
             w_->function("m_idiso0p20_desy_ratio")->functor(w_->argSet("m_pt,m_eta")));
          fns_["m_trgIsoMu22orTkIsoMu22_desy_data"] = std::shared_ptr<RooFunctor>(
             w_->function("m_trgIsoMu22orTkIsoMu22_desy_data")->functor(w_->argSet("m_pt,m_eta")));
          fns_["m_trgMu8leg_desy_data"] = std::shared_ptr<RooFunctor>(
             w_->function("m_trgMu8leg_desy_data")->functor(w_->argSet("m_pt,m_eta")));
          fns_["m_trgMu23leg_desy_data"] = std::shared_ptr<RooFunctor>(
             w_->function("m_trgMu23leg_desy_data")->functor(w_->argSet("m_pt,m_eta")));
          fns_["e_id_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_id_ratio")->functor(w_->argSet("e_pt,e_eta")));
          fns_["e_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_iso_binned_ratio")->functor(w_->argSet("e_pt,e_eta,e_iso")));
          fns_["e_trg_binned_data"] = std::shared_ptr<RooFunctor>(
             w_->function("e_trg_binned_data")->functor(w_->argSet("e_pt,e_eta,e_iso")));
          fns_["e_idiso0p15_desy_ratio"] = std::shared_ptr<RooFunctor>(
             w_->function("e_idiso0p15_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
          fns_["e_idiso0p10_desy_ratio"] = std::shared_ptr<RooFunctor>(
             w_->function("e_idiso0p10_desy_ratio")->functor(w_->argSet("e_pt,e_eta")));
          fns_["e_trgEle25eta2p1WPTight_desy_data"] = std::shared_ptr<RooFunctor>(
             w_->function("e_trgEle25eta2p1WPTight_desy_data")->functor(w_->argSet("e_pt,e_eta")));
          fns_["e_trgEle12leg_desy_data"] = std::shared_ptr<RooFunctor>(
             w_->function("e_trgEle12leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
          fns_["e_trgEle23leg_desy_data"] = std::shared_ptr<RooFunctor>(
             w_->function("e_trgEle23leg_desy_data")->functor(w_->argSet("e_pt,e_eta")));
          fns_["t_trgLooseIso_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgLooseIso_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgMediumIso_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgMediumIso_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgTightIso_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgTightIso_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgVTightIso_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgVTightIso_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgLooseIsoSS_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgLooseIsoSS_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgMediumIsoSS_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgMediumIsoSS_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgTightIsoSS_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgTightIsoSS_data")->functor(w_->argSet("t_pt")));
          fns_["t_trgVTightIsoSS_data"] = std::shared_ptr<RooFunctor>(
             w_->function("t_trgVTightIsoSS_data")->functor(w_->argSet("t_pt")));
        }
        if(do_tau_id_sf_){
          fns_["t_iso_mva_m_pt30_sf"] = std::shared_ptr<RooFunctor>(
             w_->function("t_iso_mva_m_pt30_sf")->functor(w_->argSet("t_pt,t_eta,t_dm")));
          fns_["t_iso_mva_t_pt40_eta2p1_sf"] = std::shared_ptr<RooFunctor>(
             w_->function("t_iso_mva_t_pt40_eta2p1_sf")->functor(w_->argSet("t_pt,t_eta,t_dm")));
         }
        if(do_tracking_eff_) {
          fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));
          fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
              w_->function("e_trk_ratio")->functor(w_->argSet("e_pt,e_eta")));
        }
    }

    return 0;
  }

  int HTTWeights::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    //std::vector<CompositeCandidate *> dilepton;

    double weight = 1.0;
    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

    if (ggh_mass_ != "") {
      std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      GenParticle const* higgs = NULL;
      for (unsigned i = 0; i < parts.size(); ++i) {
        if (parts[i]->status() == 3 && parts[i]->pdgid() == 25) {
          higgs = parts[i];
          break;
        }
      }
      if (!higgs) {
        std::cout << "Higgs not found!" << std::endl;
        throw;
      }
      double h_pt = higgs->pt();
      double pt_weight = 1.0;
      int fbin = ggh_hist_->FindBin(h_pt);
      if (fbin > 0 && fbin <= ggh_hist_->GetNbinsX()) {
        pt_weight =  ggh_hist_->GetBinContent(fbin);
        //std::cout << "pt: " << h_pt << "\tweight: " <<  pt_weight << std::endl;
      }
      eventInfo->set_weight("ggh", pt_weight);
    }

    if (do_topquark_weights_) {
      double top_wt = 1.0;
      double top_wt_up = 1.0;
      double top_wt_down = 1.0;
      std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
        for (unsigned i = 0; i < parts.size(); ++i){
          std::vector<bool> status_flags = parts[i]->statusFlags();
          unsigned id = abs(parts[i]->pdgid());
          if(id == 6 && status_flags[FromHardProcess] && status_flags[IsLastCopy]){
          double pt = parts[i]->pt();
          pt = std::min(pt, 400.);
          if (mc_ == mc::fall15_76X || mc_ == mc::spring16_80X) top_wt *= std::exp(0.156-0.00137*pt);
          }
        }
          
      top_wt = std::sqrt(top_wt);
      top_wt_up = top_wt * top_wt;
      top_wt_down = 1.0;
      event->Add("wt_tquark_up", top_wt_up / top_wt);
      event->Add("wt_tquark_down", top_wt_down / top_wt);
      eventInfo->set_weight("topquark_weight", top_wt);
    }
    
    if (do_tau_fake_weights_){
      if(era_ != era::data_2016){
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double fake_pt = tau->pt() < 200. ? tau->pt() : 200.;
        double fake_weight = tau_fake_weights_->Eval(fake_pt);
        eventInfo->set_weight("tau_fake_weight",fake_weight);
        double weight_up   = (fake_weight + 0.5*(1.0-fake_weight)) / fake_weight;
        double weight_down = (fake_weight - 0.5*(1.0-fake_weight)) / fake_weight;
        event->Add("wt_tau_fake_up", weight_up);
        event->Add("wt_tau_fake_down", weight_down);
      } else {
        //For 2016, use UP weight = down 20% per 100 GeV and vice-versa
        if(channel_ != channel::em){
          unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
          double weight_up = 1.0;
          double weight_down = 1.0;
          double tau_1_wt=0;
          double tau_2_wt=0;
          if(gen_match_2 == 6){
            Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
            double pt_2 = tau2->pt();
            tau_2_wt = (0.20*pt_2)/100.;
          } 
          weight_down = 1.0 + tau_2_wt;
          weight_up = std::max(0.0, 1.0 - tau_2_wt);
          if (channel_ == channel::tt){
           unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
           if(gen_match_1 == 6){
             Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
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
    }

    if (do_tau_id_weights_) {
     if(channel_ != channel::em){
       double gen_match_2_pt = event->Get<double>("gen_match_2_pt");
       unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
       double weight_up    = 1.0;
       double weight_down  = 1.0;
       double tau2_wt = 0;
       if(gen_match_2 == 5){
         tau2_wt = (0.20*gen_match_2_pt)/1000.;
       }
        weight_up   = 1.0 +tau2_wt;
        weight_down = std::max(0.0, 1.0 - tau2_wt);
        if(channel_ == channel::tt){
         double tau1_wt = 0;
         double gen_match_1_pt = event->Get<double>("gen_match_1_pt");
         unsigned gen_match_1 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
         if(gen_match_1 == 5){
           tau1_wt = (0.20*gen_match_1_pt)/1000.;
         }
         weight_up = (1.0+tau2_wt)*(1.0+tau1_wt);
         weight_down = std::max(0.0,(1.0-tau2_wt)*(1.0-tau1_wt));
       }
        event->Add("wt_tau_id_up", weight_up);
        event->Add("wt_tau_id_down", weight_down);
      }   
    }
    if (do_tau_id_sf_ && channel_!= channel::em) {
      double tau_sf_1 = 1.0;
      double tau_sf_2 = 1.0;
      if (channel_ != channel::tt){
        unsigned gen_match_2 = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
        Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double decay_mode_2 = tau2->decay_mode();
        double pt_2 = tau2->pt();
        double eta_2 = tau2->eta();
        auto args_2 = std::vector<double>{pt_2,eta_2,decay_mode_2};
        tau_sf_2 = (gen_match_2 == 5) ? fns_["t_iso_mva_m_pt30_sf"]->eval(args_2.data()) : 1.0;
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
        tau_sf_1 = (gen_match_1==5) ? fns_["t_iso_mva_t_pt40_eta2p1_sf"]->eval(args_1.data()) : 1.0;
        tau_sf_2 = (gen_match_2==5) ? fns_["t_iso_mva_t_pt40_eta2p1_sf"]->eval(args_2.data()) : 1.0;
      }
     eventInfo->set_weight("wt_tau_id_sf",tau_sf_1*tau_sf_2);
    }
    if (do_em_qcd_weights_){
      if(channel_ == channel::em){
       Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1")); 
       Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2")); 
       double qcd_weight=1.0;
       double qcd_weight_up=1.0;
       double qcd_weight_down=1.0;
       double elec_pt = elec->pt(); 
       double muon_pt = muon->pt(); 
       double deltaR = DR(elec,muon);
       if(elec_pt > 100) elec_pt = 99.9;
       if(elec_pt < 10) elec_pt = 10.1;
       if(muon_pt > 100) muon_pt = 99.9;
       if(muon_pt < 10) muon_pt = 10.1;
       double lead_pt = std::max(elec_pt,muon_pt); 
       double trail_pt = std::min(elec_pt,muon_pt); 
       if(era_==era::data_2015){
         if(deltaR < 2){
           qcd_weight = em_qcd_cr1_lt2_->Interpolate(trail_pt,lead_pt);
           qcd_weight_up = em_qcd_cr2_lt2_->Interpolate(trail_pt,lead_pt);
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         } else if (deltaR <=4){
           qcd_weight = em_qcd_cr1_2to4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_up = em_qcd_cr2_2to4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         } else {
           qcd_weight = em_qcd_cr1_gt4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_up = em_qcd_cr2_gt4_->Interpolate(trail_pt,lead_pt);
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         }
       } else if (era_==era::data_2016){
         if(deltaR < 2){
           qcd_weight = em_qcd_cr1_lt2_->GetBinContent(em_qcd_cr1_lt2_->FindBin(trail_pt,lead_pt));
           qcd_weight_up = em_qcd_cr2_lt2_->GetBinContent(em_qcd_cr2_lt2_->FindBin(trail_pt,lead_pt));
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         } else if (deltaR <=4){
           qcd_weight = em_qcd_cr1_2to4_->GetBinContent(em_qcd_cr1_2to4_->FindBin(trail_pt,lead_pt));
           qcd_weight_up = em_qcd_cr2_2to4_->GetBinContent(em_qcd_cr2_2to4_->FindBin(trail_pt,lead_pt));
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         } else {
           qcd_weight = em_qcd_cr1_gt4_->GetBinContent(em_qcd_cr1_gt4_->FindBin(trail_pt,lead_pt));
           qcd_weight_up = em_qcd_cr2_gt4_->GetBinContent(em_qcd_cr2_gt4_->FindBin(trail_pt,lead_pt));
           qcd_weight_down = qcd_weight*qcd_weight/qcd_weight_up;
         }
       }   
       event->Add("wt_em_qcd",qcd_weight);
       event->Add("wt_em_qcd_down",qcd_weight_down);
       event->Add("wt_em_qcd_up",qcd_weight_up);
     }
   }
    
    //A derived correction based on a input/MC discrepancy in the subleading b-jet eta in the emu ttbar control region. Used for a cross-check in H->hh analysis.
    if (do_top_jeteta_weights_) {
      std::vector<PFJet*> prebjets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
      std::sort(prebjets.begin(), prebjets.end(), bind(&PFJet::GetBDiscriminator, _1, btag_label_) > bind(&PFJet::GetBDiscriminator, _2, btag_label_));
      std::vector<PFJet*> prebjets_SF = prebjets;
      // Instead of changing b-tag value in the promote/demote method we look for a map of bools
      // that say whether a jet should pass the WP or not
      if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result");
        ic::erase_if(prebjets_SF, !boost::bind(IsReBTagged, _1, retag_result));
      } else {
        ic::erase_if(prebjets_SF, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label_) < 0.679);
      }
      int n_prebjets_SF=prebjets_SF.size();
      double wt=1.00;
      if(n_prebjets_SF>=1) {
        double eta = prebjets[1]->eta();
        if (eta <= -2.0)                           { wt = 1.1897861575; }
        if (eta >= -2.0 && eta < -1.6)             { wt = 1.0639250201; }
        if (eta >= -1.6 && eta < -1.2)             { wt = 1.06317801149; }
        if (eta >= -1.2 && eta < -0.8)             { wt = 0.985871842192; }
        if (eta >= -0.8 && eta < -0.4)             { wt = 0.976539619511; }
        if (eta >= -0.4 && eta < 0.0)              { wt = 0.922588119141; }
        if (eta >= 0.0 && eta < 0.4)               { wt = 1.02200809147; }
        if (eta >= 0.4 && eta < 0.8)               { wt = 0.935068388647; }
        if (eta >= 0.8 && eta < 1.2)               { wt = 0.973272817984; }
        if (eta >= 1.2 && eta < 1.6)               { wt = 1.03197671439; }
        if (eta >= 1.6 && eta < 2.0)               { wt = 1.07689347695; }
        if (eta >= 2.0)                            { wt = 1.13656881923; }
      }
      eventInfo->set_weight("jeteta_weight", wt);
    }
    
    if (do_top_factors_) {
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    }
    
    
   
    if(event->Exists("btag_evt_weight")){
       double wtbtag = event->Get<double>("btag_evt_weight");
       weight *= wtbtag;
     } 

    if (do_zpt_weight_){
      double zpt = event->Exists("genpT") ? event->Get<double>("genpT") : 0;
      double zmass = event->Exists("genM") ? event->Get<double>("genM") : 0;
      double wtzpt = z_pt_mass_hist_->GetBinContent(z_pt_mass_hist_->GetXaxis()->FindBin(zmass),z_pt_mass_hist_->GetYaxis()->FindBin(zpt));
      double wtzpt_down=1.0;
      double wtzpt_up = wtzpt*wtzpt; 
      eventInfo->set_weight("wt_zpt",wtzpt);
      event->Add("wt_zpt_up",wtzpt_up/wtzpt);
      event->Add("wt_zpt_down",wtzpt_down/wtzpt);
    }

   if (do_tracking_eff_){
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
        double e_eta = fabs(elec->sc_eta());
        double e_signed_eta = elec->sc_eta();
        if(era_ == era::data_2015 || era_==era::data_2016) e_eta = fabs(elec->eta());
        double t_pt = tau->pt();
        double ele_trg = 1.0;
        double tau_trg = 1.0;
        double ele_trg_mc = 1.0;
        double tau_trg_mc = 1.0;
       if (mc_ == mc::fall15_76X ){
          if(e_pt<150){
            ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
            ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
          } else {
            ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
            ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
          }         
          tau_trg=1;
          tau_trg_mc=1;

        } else if (mc_ == mc::spring16_80X){
           if(scalefactor_file_==""){ 
              if(do_single_lepton_trg_ && !do_cross_trg_){
                if(e_iso < 0.1){
                  if(e_pt<1000){
                    ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
                    //ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
                  } else {
                    ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                    //ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
                  }         
                  tau_trg=1;
                  tau_trg_mc=1;
                }else if(e_iso <0.2){
                  if(e_pt<1000){
                    ele_trg = et_antiiso1_trig_data_->GetBinContent(et_antiiso1_trig_data_->GetXaxis()->FindBin(e_eta),et_antiiso1_trig_data_->GetYaxis()->FindBin(e_pt));
                    //ele_trg_mc = et_trig_antiiso1_mc_->GetBinContent(et_trig_antiiso1_mc_->GetXaxis()->FindBin(e_eta),et_trig_antiiso1_mc_->GetYaxis()->FindBin(e_pt));
                  } else {
                    ele_trg = et_antiiso1_trig_data_->GetBinContent(et_antiiso1_trig_data_->GetXaxis()->FindBin(e_eta),(et_antiiso1_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                    //ele_trg_mc = et_trig_antiiso1_mc_->GetBinContent(et_trig_antiiso1_mc_->GetXaxis()->FindBin(e_eta),(et_trig_antiiso1_mc_->GetYaxis()->FindBin(e_pt)-1));
                  }         
                  tau_trg=1;
                  tau_trg_mc=1;
               } else {//efficiencies only derived for iso<0.5!
                  if(e_pt<1000){
                    ele_trg = et_antiiso2_trig_data_->GetBinContent(et_antiiso2_trig_data_->GetXaxis()->FindBin(e_eta),et_antiiso2_trig_data_->GetYaxis()->FindBin(e_pt));
                    //ele_trg_mc = et_trig_antiiso2_mc_->GetBinContent(et_trig_antiiso2_mc_->GetXaxis()->FindBin(e_eta),et_trig_antiiso2_mc_->GetYaxis()->FindBin(e_pt));
                  } else {
                    ele_trg = et_antiiso2_trig_data_->GetBinContent(et_antiiso2_trig_data_->GetXaxis()->FindBin(e_eta),(et_antiiso2_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                    //ele_trg_mc = et_trig_antiiso2_mc_->GetBinContent(et_trig_antiiso2_mc_->GetXaxis()->FindBin(e_eta),(et_trig_antiiso2_mc_->GetYaxis()->FindBin(e_pt)-1));
                  }         
                  tau_trg=1;
                  tau_trg_mc=1;
                }
              } else if(do_cross_trg_ && !do_single_lepton_trg_){
                if(e_pt<1000){
                  ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),et_xtrig_data_->GetYaxis()->FindBin(e_pt));
                  ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),et_xtrig_mc_->GetYaxis()->FindBin(e_pt));
                } else {
                  ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),(et_xtrig_data_->GetYaxis()->FindBin(e_pt)-1));
                  ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),(et_xtrig_mc_->GetYaxis()->FindBin(e_pt)-1));
                }         
                tau_trg_mc=1;
                unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
                if(gm2_ == 5){ //Using tight iso:
                  tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
                } else {
                  tau_trg       = Efficiency(t_pt, 20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
                } 
              } else if(do_cross_trg_ && do_single_lepton_trg_){
                double ele_sgl_trg=1.0;
                double ele_sgl_trg_mc=1.0;
                double ele_cond_trg=1.0;
                double ele_cond_trg_mc=1.0;
                if(e_pt<1000){
                  ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),et_xtrig_data_->GetYaxis()->FindBin(e_pt));
                  ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),et_xtrig_mc_->GetYaxis()->FindBin(e_pt));
                  ele_sgl_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
                  ele_sgl_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
                  ele_cond_trg = et_conditional_data_->GetBinContent(et_conditional_data_->GetXaxis()->FindBin(e_eta),et_conditional_data_->GetYaxis()->FindBin(e_pt));
                  ele_cond_trg_mc = et_conditional_mc_->GetBinContent(et_conditional_mc_->GetXaxis()->FindBin(e_eta),et_conditional_mc_->GetYaxis()->FindBin(e_pt));
                } else {
                  ele_trg = et_xtrig_data_->GetBinContent(et_xtrig_data_->GetXaxis()->FindBin(e_eta),(et_xtrig_data_->GetYaxis()->FindBin(e_pt)-1));
                  ele_trg_mc = et_xtrig_mc_->GetBinContent(et_xtrig_mc_->GetXaxis()->FindBin(e_eta),(et_xtrig_mc_->GetYaxis()->FindBin(e_pt)-1));
                  ele_sgl_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
                  ele_sgl_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
                  ele_cond_trg = et_conditional_data_->GetBinContent(et_conditional_data_->GetXaxis()->FindBin(e_eta),(et_conditional_data_->GetYaxis()->FindBin(e_pt)-1));
                  ele_cond_trg_mc = et_conditional_mc_->GetBinContent(et_conditional_mc_->GetXaxis()->FindBin(e_eta),(et_conditional_mc_->GetYaxis()->FindBin(e_pt)-1));
                }         
                tau_trg_mc=1;
                unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
                if(gm2_ == 5){ //Using tight iso:
                  tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
                } else {
                  tau_trg       = Efficiency(t_pt, 20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
                } 
                ele_trg = ele_sgl_trg+tau_trg*ele_trg-tau_trg*ele_cond_trg*ele_trg; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
                ele_trg_mc = ele_sgl_trg_mc+tau_trg_mc*ele_trg_mc-tau_trg_mc*ele_cond_trg_mc*ele_trg_mc; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
                tau_trg = 1.0; //In this case full weight is stored as ele_trg weight 
              } 

            } else {
              if(!do_cross_trg_ && do_single_lepton_trg_){
                  tau_trg = 1;
                  tau_trg_mc=1;
                  auto args_1 = std::vector<double>{e_pt,e_signed_eta,e_iso};
                  auto args_desy = std::vector<double>{e_pt,e_signed_eta};
                  if(e_iso<0.1){
                    ele_trg = fns_["e_trgEle25eta2p1WPTight_desy_data"]->eval(args_desy.data());
                  } else ele_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
                  ele_trg_mc=1;
              } else {
                  std::cout << "Cross trigger not currently supported! Setting trigger efficiencies to 1" << std::endl;
                  tau_trg=1;
                  tau_trg_mc=1;
                  ele_trg=1;
                  ele_trg_mc=1;
              }
           }
        }
        if (trg_applied_in_mc_) {
          ele_trg = ele_trg / ele_trg_mc;
          tau_trg = tau_trg / tau_trg_mc;
        }
        weight *= (ele_trg * tau_trg);
        event->Add("trigweight_1", ele_trg);
        event->Add("trigweight_2", tau_trg);
      } else if (channel_ == channel::mt) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        double pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_signed_eta = muon->eta();
        double m_iso = PF04IsolationVal(muon, 0.5, 0);
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double t_pt = tau->pt();
        double mu_trg = 1.0;
        double tau_trg = 1.0;
        double mu_trg_mc = 1.0;
        double tau_trg_mc = 1.0;
     if (mc_ == mc::fall15_76X ){
          if(pt<150){
            mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
            mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
          } else {
            mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),(mt_trig_data_->GetYaxis()->FindBin(pt)-1));
            mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt)-1));
          }         
          tau_trg=1;
          tau_trg_mc=1;

         } else if(mc_ == mc::spring16_80X){
          if(scalefactor_file_=="") {
              if(do_single_lepton_trg_ && !do_cross_trg_){
                if(m_iso<0.15){
                  if(pt<1000){
                    mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
                   // mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
                  } else {
                    mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),(mt_trig_data_->GetYaxis()->FindBin(pt)-1));
                    //mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt)-1));
                  }         
                  tau_trg=1;
                  tau_trg_mc=1;
                } else if (m_iso<0.25){
                  if(pt<1000){
                    mu_trg = mt_antiiso1_trig_data_->GetBinContent(mt_antiiso1_trig_data_->GetXaxis()->FindBin(m_eta),mt_antiiso1_trig_data_->GetYaxis()->FindBin(pt));
    //                mu_trg_mc = mt_antiiso1_trig_mc_->GetBinContent(mt_antiiso1_trig_mc_->GetXaxis()->FindBin(m_eta),mt_antiiso1_trig_mc_->GetYaxis()->FindBin(pt));
                  } else {
                    mu_trg = mt_antiiso1_trig_data_->GetBinContent(mt_antiiso1_trig_data_->GetXaxis()->FindBin(m_eta),(mt_antiiso1_trig_data_->GetYaxis()->FindBin(pt)-1));
                    //mu_trg_mc = mt_antiiso1_trig_mc_->GetBinContent(mt_antiiso1_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_antiiso1_trig_mc_->GetYaxis()->FindBin(pt)-1));
                  }         
                  tau_trg=1;
                  tau_trg_mc=1;
                } else { //scale factors only derived for iso<0.5!
                  if(pt<1000){
                    mu_trg = mt_antiiso2_trig_data_->GetBinContent(mt_antiiso2_trig_data_->GetXaxis()->FindBin(m_eta),mt_antiiso2_trig_data_->GetYaxis()->FindBin(pt));
                    //mu_trg_mc = mt_antiiso2_trig_mc_->GetBinContent(mt_antiiso2_trig_mc_->GetXaxis()->FindBin(m_eta),mt_antiiso2_trig_mc_->GetYaxis()->FindBin(pt));
                  } else {
                    mu_trg = mt_antiiso2_trig_data_->GetBinContent(mt_antiiso2_trig_data_->GetXaxis()->FindBin(m_eta),(mt_antiiso2_trig_data_->GetYaxis()->FindBin(pt)-1));
                    //mu_trg_mc = mt_antiiso2_trig_mc_->GetBinContent(mt_antiiso2_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_antiiso2_trig_mc_->GetYaxis()->FindBin(pt)-1));
                  }         
                  tau_trg=1;
                  tau_trg_mc=1;
                }
              } else if(do_cross_trg_ &&!do_single_lepton_trg_){
                 if(pt<1000){
                  mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),mt_xtrig_data_->GetYaxis()->FindBin(pt));
                  mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),mt_xtrig_mc_->GetYaxis()->FindBin(pt));
                } else {
                  mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),(mt_xtrig_data_->GetYaxis()->FindBin(pt)-1));
                  mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),(mt_xtrig_mc_->GetYaxis()->FindBin(pt)-1));
                }         
                tau_trg_mc=1;
                unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
                if(gm2_ == 5){ //Using tight iso:
                  tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
                } else {
                  tau_trg       = Efficiency(t_pt,  20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
                } 
              } else if (do_single_lepton_trg_ && do_cross_trg_){
                double mu_sgl_trg = 1.0;
                double mu_sgl_trg_mc = 1.0;
                double mu_cond_trg = 1.0;
                double mu_cond_trg_mc = 1.0;
                 if(pt<1000){
                  mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),mt_xtrig_data_->GetYaxis()->FindBin(pt));
                  mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),mt_xtrig_mc_->GetYaxis()->FindBin(pt));
                  mu_sgl_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
                  mu_sgl_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
                  mu_cond_trg = mt_conditional_data_->GetBinContent(mt_conditional_data_->GetXaxis()->FindBin(m_eta),mt_conditional_data_->GetYaxis()->FindBin(pt));
                  mu_cond_trg_mc = mt_conditional_mc_->GetBinContent(mt_conditional_mc_->GetXaxis()->FindBin(m_eta),mt_conditional_mc_->GetYaxis()->FindBin(pt));
                } else {
                  mu_trg = mt_xtrig_data_->GetBinContent(mt_xtrig_data_->GetXaxis()->FindBin(m_eta),(mt_xtrig_data_->GetYaxis()->FindBin(pt)-1));
                  mu_trg_mc = mt_xtrig_mc_->GetBinContent(mt_xtrig_mc_->GetXaxis()->FindBin(m_eta),(mt_xtrig_mc_->GetYaxis()->FindBin(pt)-1));
                  mu_sgl_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt)-1);
                  mu_sgl_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt)-1);
                  mu_cond_trg = mt_conditional_data_->GetBinContent(mt_conditional_data_->GetXaxis()->FindBin(m_eta),mt_conditional_data_->GetYaxis()->FindBin(pt)-1);
                  mu_cond_trg_mc = mt_conditional_mc_->GetBinContent(mt_conditional_mc_->GetXaxis()->FindBin(m_eta),mt_conditional_mc_->GetYaxis()->FindBin(pt)-1);
                }         
                tau_trg_mc=1;
                unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
                if(gm2_ == 5){ //Using tight iso:
                  tau_trg       = Efficiency(t_pt, 21.1744, 0.773395, 0.705463, 1.65358, 1.0000);
                } else {
                  tau_trg       = Efficiency(t_pt,  20.7216, 1.86861, 1.79281, 1.56784, 9.86642e-01);
                } 
                mu_trg = mu_sgl_trg+tau_trg*mu_trg-tau_trg*mu_cond_trg*mu_trg; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
                mu_trg_mc = mu_sgl_trg_mc+tau_trg_mc*mu_trg_mc-tau_trg_mc*mu_cond_trg_mc*mu_trg_mc; //Conditional probability: P(single)+P(crosstrig)-P(crosstrig)*P(singlelept|muleg crosstrig)
                tau_trg = 1.0; //In this case full weight is stored as mu_trg weight 
              }
            } else {
                if(do_single_lepton_trg_ && !do_cross_trg_){
                    tau_trg = 1;
                    tau_trg_mc=1;
                    auto args_1 = std::vector<double>{pt,m_signed_eta,m_iso};
                    auto args_desy = std::vector<double>{pt,m_signed_eta};
                    if(m_iso<0.15){
                      mu_trg = fns_["m_trgIsoMu22orTkIsoMu22_desy_data"]->eval(args_desy.data());
                    } else  mu_trg = fns_["m_trgOR_binned_data"]->eval(args_1.data());
                  //  mu_trg_mc = fns_["m_trg_mc"]->eval(args_1.data());
                   // mu_trg = 1;
                    mu_trg_mc=1;
                } else {
                    std::cout << "Cross trigger not currently supported! Setting trigger efficiencies to 1" << std::endl;
                    tau_trg=1;
                    tau_trg_mc=1;
                    mu_trg=1;
                    mu_trg_mc=1;
                }
            }
         }

        if (trg_applied_in_mc_) {
          mu_trg = mu_trg / mu_trg_mc;
          tau_trg = tau_trg / tau_trg_mc;
        }
        weight *= (mu_trg * tau_trg);
        event->Add("trigweight_1", mu_trg);
        event->Add("trigweight_2", tau_trg);
      } else if (channel_ == channel::em) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
        double e_pt = elec->pt();
        double e_eta = fabs(elec->sc_eta());
        if(era_ == era::data_2015 || era_ == era::data_2016) e_eta = fabs(elec->sc_eta());
        double m_pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_trg_17 = 1.0;
        double m_trg_17_mc = 1.0;
        double m_trg_8 = 1.0;
        double m_trg_8_mc = 1.0;
        double e_trg_17 = 1.0;
        double e_trg_17_mc = 1.0;
        double e_trg_12 = 1.0;
        double e_trg_12_mc = 1.0;
        double e_trg = 1.0;
        if (mc_ == mc::fall15_76X){
          if(e_pt<150){
            e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),em_e17_trig_data_->GetYaxis()->FindBin(e_pt));
            e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),em_e17_trig_mc_->GetYaxis()->FindBin(e_pt));
            e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),em_e12_trig_data_->GetYaxis()->FindBin(e_pt));
            e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),em_e12_trig_mc_->GetYaxis()->FindBin(e_pt));
          } else {
            e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),(em_e17_trig_data_->GetYaxis()->FindBin(e_pt)-1));
            e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e17_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
            e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),(em_e12_trig_data_->GetYaxis()->FindBin(e_pt)-1));
            e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e12_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
          }         
          if(m_pt<150){
            m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),em_m17_trig_data_->GetYaxis()->FindBin(m_pt));
            m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),em_m17_trig_mc_->GetYaxis()->FindBin(m_pt));
            m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),em_m8_trig_data_->GetYaxis()->FindBin(m_pt));
            m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),em_m8_trig_mc_->GetYaxis()->FindBin(m_pt));
          } else {
            m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),(em_m17_trig_data_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m17_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),(em_m8_trig_data_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m8_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
          }         

       } else if (mc_ == mc::spring16_80X){
      /*    if(e_pt<1000){

            e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),em_e17_trig_data_->GetYaxis()->FindBin(e_pt));
            e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),em_e17_trig_mc_->GetYaxis()->FindBin(e_pt));
            e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),em_e12_trig_data_->GetYaxis()->FindBin(e_pt));
            e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),em_e12_trig_mc_->GetYaxis()->FindBin(e_pt));
          } else {
            e_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),(em_e17_trig_data_->GetYaxis()->FindBin(e_pt)-1));
            e_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e17_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
            e_trg_12 = em_e12_trig_data_->GetBinContent(em_e12_trig_data_->GetXaxis()->FindBin(e_eta),(em_e12_trig_data_->GetYaxis()->FindBin(e_pt)-1));
            e_trg_12_mc = em_e12_trig_mc_->GetBinContent(em_e12_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e12_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
          }         
          if(m_pt<1000){
            m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),em_m17_trig_data_->GetYaxis()->FindBin(m_pt));
            m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),em_m17_trig_mc_->GetYaxis()->FindBin(m_pt));
            m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),em_m8_trig_data_->GetYaxis()->FindBin(m_pt));
            m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),em_m8_trig_mc_->GetYaxis()->FindBin(m_pt));
          } else {
            m_trg_17 = em_m17_trig_data_->GetBinContent(em_m17_trig_data_->GetXaxis()->FindBin(m_eta),(em_m17_trig_data_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_17_mc = em_m17_trig_mc_->GetBinContent(em_m17_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m17_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(m_eta),(em_m8_trig_data_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(m_eta),(em_m8_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
          }*/         
          auto args_1 = std::vector<double>{e_pt,e_eta};
          auto args_2 = std::vector<double>{m_pt,m_eta};
          //17 is actually 23 but just so I don't have to change the bit below where the efficiency is calculated. Probably one to fix at some point
          m_trg_17 = fns_["m_trgMu23leg_desy_data"]->eval(args_2.data());
          m_trg_8  = fns_["m_trgMu8leg_desy_data"]->eval(args_2.data());
          e_trg_17 = fns_["e_trgEle23leg_desy_data"]->eval(args_1.data());
          e_trg_12  = fns_["e_trgEle12leg_desy_data"]->eval(args_1.data());
        }
        if (trg_applied_in_mc_){
          e_trg = (m_trg_17*e_trg_12 + m_trg_8*e_trg_17 - m_trg_17*e_trg_17)/(m_trg_17_mc*e_trg_12_mc + m_trg_8_mc*e_trg_17_mc - m_trg_17_mc*e_trg_17_mc);
        } else e_trg = (m_trg_17*e_trg_12 + m_trg_8*e_trg_17 - m_trg_17*e_trg_17);

        weight *= (e_trg);
        //trigweight_1 is actually the full trigger weight because of the way the efficiencies are combined
        event->Add("trigweight_1", e_trg);
        event->Add("trigweight_2", double(1.0));
      } else if (channel_ == channel::tt){
        Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
        Tau const* tau2 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double pt_1 = tau1->pt();
        double pt_2 = tau2->pt();
        auto args_1 = std::vector<double>{pt_1};
        auto args_2 = std::vector<double>{pt_2};
        double tau1_trg = 1.0;
        double tau1_trg_mc = 1.0;
        double tau1_trg_up = 1.0;
        double tau1_trg_mc_up = 1.0;
        double tau1_trg_down = 1.0;
        double tau1_trg_mc_down = 1.0;
        double tau2_trg = 1.0;
        double tau2_trg_mc = 1.0;
        double tau2_trg_up = 1.0;
        double tau2_trg_mc_up = 1.0;
        double tau2_trg_down = 1.0;
        double tau2_trg_mc_down = 1.0;
        if (mc_ == mc::fall15_76X){
          tau1_trg      = Efficiency(pt_1, 34.5412, 5.63353, 2.49242, 3.35896, 1.0);
          tau1_trg_mc   = Efficiency(pt_1, 36.0274, 5.89434, 5.82870, 1.83737, 9.58000e-01);
          tau2_trg      = Efficiency(pt_2, 34.5412, 5.63353, 2.49242, 3.35896, 1.0);
          tau2_trg_mc   = Efficiency(pt_2, 36.0274, 5.89434, 5.82870, 1.83737, 9.58000e-01);
          tau1_trg_up        = Efficiency(pt_1, 33.1713, 5.66551, 1.87175, 8.07790, 1.0);
          tau1_trg_mc_up     = Efficiency(pt_1, 35.6012, 5.98209, 6.09604, 1.68740, 9.87653e-01);
          tau2_trg_up        = Efficiency(pt_2, 33.1713, 5.66551, 1.87175, 8.07790, 1.0);
          tau2_trg_mc_up     = Efficiency(pt_2, 35.6012, 5.98209, 6.09604, 1.68740, 9.87653e-01);
          tau1_trg_down      = Efficiency(pt_1, 35.6264, 5.30711, 2.81591, 2.40649, 9.99958e-01);
          tau1_trg_mc_down   = Efficiency(pt_1, 36.2436, 5.58461, 5.12924, 2.05921, 9.32305e-01);
          tau2_trg_down      = Efficiency(pt_2, 35.6264, 5.30711, 2.81591, 2.40649, 9.99958e-01);
          tau2_trg_mc_down   = Efficiency(pt_2, 36.2436, 5.58461, 5.12924, 2.05921, 9.32305e-01);
         } else if(mc_ == mc::spring16_80X){
          unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
          unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
          if(tt_trg_iso_mode_==4) {
              //Hack the binned efficiencies in here
              bool vloose_2 = tau2->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
              bool loose_2 =  tau2->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ;
              bool medium_2 = tau2->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT");
              bool tight_2 =  tau2->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
              bool vtight_2 =  tau2->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT");
              bool vvtight_2 =  tau2->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT");
              bool vloose_1 = tau1->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
              bool loose_1 =  tau1->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT") ;
              bool medium_1 = tau1->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT");
              bool tight_1 =  tau1->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
              bool vtight_1 =  tau1->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT");
              bool vvtight_1 =  tau1->GetTauID("byVVTightIsolationMVArun2v1DBoldDMwLT");
              if(gm1_ == 5){ 
               if(vloose_1 > 0.5){
                 if(loose_1 < 0.5){
                   tau1_trg       = Efficiency(pt_1, 38.0333, 7.91088 , 15.20129999, 1.22660615, 0.999998);
                 } else if(medium_1 < 0.5){
                   tau1_trg       = Efficiency(pt_1, 40.336709, 6.7403024, 32.5694946, 1.044272995, 0.8514444586);
                 } else if(tight_1 < 0.5){
                   tau1_trg       = Efficiency(pt_1, 39.056473, 5.8055095, 4.865577978754, 2.575900, 0.999999);
                 } else if(vtight_1 < 0.5){
                   tau1_trg       = Efficiency(pt_1, 38.5853083, 5.61112878,7.3018728623, 3.0092333917, 0.8600521839);
                 } else if(vvtight_1 < 0.5){
                   tau1_trg       = Efficiency(pt_1, 38.618169, 5.312144619, 6.46196978, 25.920636705722, 0.8695357558);
                 } else {
                   tau1_trg       = Efficiency(pt_1, 38.0330158839, 5.200365778, 6.09497263, 1.691237814,0.99999999);
                 }
               }
              } else {
                if(vloose_1 > 0.5){
                 if(loose_1 < 0.5){
                   tau1_trg       = Efficiency(pt_1, 42.4105036, 8.201898577, 18.314907696, 1.0362475723, 0.999996);
                 } else if (medium_1<0.5){
                   tau1_trg       = Efficiency(pt_1, 40.8106337, 7.818987,10.5121936,10.9851908, 0.7664256354);
                 } else if (tight_1<0.5){
                   tau1_trg       = Efficiency(pt_1, 40.17626812, 7.814412357, 10.5986531, 9.4682986699, 0.790908712);
                 } else if (vtight_1<0.5){
                   tau1_trg       = Efficiency(pt_1, 40.294717, 7.869141154, 14.05910804, 1.1801018155, 0.99996473);
                 } else if (vvtight_1<0.5){
                   tau1_trg      = Efficiency(pt_1, 38.639538, 7.3893785184, 11.190558468, 1.6373712, 0.818539785);
                 } else {
                   tau1_trg      = Efficiency(pt_1, 39.2202518, 7.90325645, 11.32316371,1.3296217, 0.992292297);
                 }
               }
              } 
              if(gm2_ == 5){ 
               if(vloose_2 > 0.5){
                 if(loose_2 < 0.5){
                   tau2_trg       = Efficiency(pt_2, 38.0333, 7.91088 , 15.20129999, 1.22660615, 0.999998);
                 } else if(medium_2 < 0.5){
                   tau2_trg       = Efficiency(pt_2, 40.336709, 6.7403024, 32.5694946, 1.044272995, 0.8514444586);
                 } else if(tight_2 < 0.5){
                   tau2_trg       = Efficiency(pt_2, 39.056473, 5.8055095, 4.865577978754, 2.575900, 0.999999);
                 } else if(vtight_2 < 0.5){
                   tau2_trg       = Efficiency(pt_2, 38.5853083, 5.61112878,7.3018728623, 3.0092333917, 0.8600521839);
                 } else if(vvtight_2 < 0.5){
                   tau2_trg       = Efficiency(pt_2, 38.618169, 5.312144619, 6.46196978, 25.920636705722, 0.8695357558);
                 } else {
                   tau2_trg       = Efficiency(pt_2, 38.0330158839, 5.200365778, 6.09497263, 1.691237814,0.99999999);
                 }
               }
              } else {
                if(vloose_2 > 0.5){
                 if(loose_2 < 0.5){
                   tau2_trg       = Efficiency(pt_2, 42.4105036, 8.201898577, 18.314907696, 1.0362475723, 0.999996);
                 } else if (medium_2<0.5){
                   tau2_trg       = Efficiency(pt_2, 40.8106337, 7.818987,10.5121936,10.9851908, 0.7664256354);
                 } else if (tight_2<0.5){
                   tau2_trg       = Efficiency(pt_2, 40.17626812, 7.814412357, 10.5986531, 9.4682986699, 0.790908712);
                 } else if (vtight_2<0.5){
                   tau2_trg       = Efficiency(pt_2, 40.294717, 7.869141154, 14.05910804, 1.1801018155, 0.99996473);
                 } else if (vvtight_2<0.5){
                   tau2_trg      = Efficiency(pt_2, 38.639538, 7.3893785184, 11.190558468, 1.6373712, 0.818539785);
                 } else {
                   tau2_trg      = Efficiency(pt_2, 39.2202518, 7.90325645, 11.32316371,1.3296217, 0.992292297);
                 }
               }
              } 
              /*if(gm2_ == 5){ //Using tight iso:
                tau2_trg       = Efficiency(pt_2, 38.1919, 5.38746, 4.44730, 7.39646, 9.33402e-01);
              } else {
                tau2_trg       = Efficiency(pt_2, 39.9131, 7.77317, 39.9403, 140.999, 7.84025E-01);
              }*/ 
            }else if (tt_trg_iso_mode_==0){//Using tight iso
              if(gm1_ == 5){ 
                tau1_trg = fns_["t_trgTightIso_data"]->eval(args_1.data());
              } else {
                tau1_trg = fns_["t_trgTightIsoSS_data"]->eval(args_1.data());
              } 
              if(gm2_ == 5){ 
                tau2_trg = fns_["t_trgTightIso_data"]->eval(args_2.data());
              } else {
                tau2_trg = fns_["t_trgTightIsoSS_data"]->eval(args_2.data());
              } 
            } else if (tt_trg_iso_mode_==1) {
              if(gm1_ == 5){ //Using medium iso
                tau1_trg       = Efficiency(pt_1, 38.2845, 5.3257, 6.450034, 1.6489256, 0.999998);
              } else {
                tau1_trg       = Efficiency(pt_1, 39.70776, 7.80895, 12.00733, 1.71461644, 0.85339127);
              } 
              if(gm2_ == 5){ //Using medium iso:
                tau2_trg       = Efficiency(pt_2, 38.2845, 5.3257, 6.450034, 1.6489256, 0.999998);
              } else {
                tau2_trg       = Efficiency(pt_2, 39.70776, 7.80895, 12.00733, 1.71461644, 0.85339127);
              } 
            } else if (tt_trg_iso_mode_==2) {
              if(gm1_ == 5){ //Using loose iso
                tau1_trg       = Efficiency(pt_1, 38.44557, 5.43466, 6.976905, 1.560941, 0.999999);
              } else {
                tau1_trg       = Efficiency(pt_1, 39.9946, 7.808277, 11.6038, 2.3037, 0.814011);
              } 
              if(gm2_ == 5){ //Using loose iso:
                tau2_trg       = Efficiency(pt_2, 38.44557, 5.43466, 6.976905, 1.560941, 0.999999);
              } else {
                tau2_trg       = Efficiency(pt_2, 39.9946, 7.808277, 11.6038, 2.3037, 0.814011);
              } 
            } else if (tt_trg_iso_mode_==3) {
              if(gm1_ == 5){ //Using vloose iso
                tau1_trg       = Efficiency(pt_1, 38.55513, 5.52419, 6.905413, 1.601455779, 0.9999999);
              } else {
                tau1_trg       = Efficiency(pt_1, 40.65517, 8.00283, 15.2985, 1.09653, 0.99999);
              } 
              if(gm2_ == 5){ //Using vloose iso:
                tau2_trg       = Efficiency(pt_2, 38.55513, 5.52419, 6.905413, 1.601455779, 0.9999999);
              } else {
                tau2_trg       = Efficiency(pt_2, 40.65517, 8.00283, 15.2985, 1.09653, 0.99999);
              }
            }
         }
        if(trg_applied_in_mc_){
          tau1_trg = tau1_trg / tau1_trg_mc;
          tau2_trg = tau2_trg / tau2_trg_mc;
          //Conservative up/down weights as 
          tau1_trg_up = tau1_trg_up / tau1_trg_mc_down;
          tau2_trg_up = tau2_trg_up / tau2_trg_mc_down;
          tau1_trg_down = tau1_trg_down / tau1_trg_mc_up;
          tau2_trg_down = tau2_trg_down / tau2_trg_mc_up;
          //Want to apply this on top of the default weight:
          tau1_trg_up = tau1_trg_up / tau1_trg;
          tau2_trg_up = tau2_trg_up / tau2_trg;
          tau1_trg_down = tau1_trg_down / tau1_trg;
          tau2_trg_down = tau2_trg_down / tau2_trg;
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
        Electron const* elec2 = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton2"));
        double e1_pt = elec->pt();
        double e1_eta = fabs(elec->sc_eta());
        double e2_pt = elec2->pt();
        double e2_eta = fabs(elec2->sc_eta());
        double e1_signed_eta = elec->eta();
        double e2_signed_eta = elec2->eta();
        double e_iso_1 = PF03IsolationVal(elec, 0.5, 0);
        double e_iso_2 = PF03IsolationVal(elec2, 0.5, 0);
        double ele1_trg = 1.0;
        double ele2_trg = 1.0;
        double ele1_trg_mc = 1.0;
        double ele2_trg_mc = 1.0;
        if(era_ == era::data_2015 || era_==era::data_2016) e1_eta = fabs(elec->eta());
        if (mc_ == mc::fall15_76X){
          if(e1_pt<150){
            ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),et_trig_data_->GetYaxis()->FindBin(e1_pt));
            ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),et_trig_mc_->GetYaxis()->FindBin(e1_pt));
          } else {
            ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),(et_trig_data_->GetYaxis()->FindBin(e1_pt)-1));
            ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),(et_trig_mc_->GetYaxis()->FindBin(e1_pt)-1));
          }         

          if (e2_pt<150){
            ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),et_trig_data_->GetYaxis()->FindBin(e2_pt));
            ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),et_trig_mc_->GetYaxis()->FindBin(e2_pt));
          } else {
            ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),(et_trig_data_->GetYaxis()->FindBin(e2_pt)-1));
            ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),(et_trig_mc_->GetYaxis()->FindBin(e2_pt)-1));
          }
        } else if (mc_ == mc::spring16_80X){
          if(scalefactor_file_==""){
              if(e1_pt<1000){
                ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),et_trig_data_->GetYaxis()->FindBin(e1_pt));
                ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),et_trig_mc_->GetYaxis()->FindBin(e1_pt));
              } else {
                ele1_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e1_eta),(et_trig_data_->GetYaxis()->FindBin(e1_pt)-1));
                ele1_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e1_eta),(et_trig_mc_->GetYaxis()->FindBin(e1_pt)-1));
              }         
              if(e2_pt<1000){
                ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),et_trig_data_->GetYaxis()->FindBin(e2_pt));
                ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),et_trig_mc_->GetYaxis()->FindBin(e2_pt));
              } else {
                ele2_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e2_eta),(et_trig_data_->GetYaxis()->FindBin(e2_pt)-1));
                ele2_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e2_eta),(et_trig_mc_->GetYaxis()->FindBin(e2_pt)-1));
              }         
          } else {
             ele1_trg = 1;
             ele1_trg_mc=1;
             ele2_trg = 1;
             ele2_trg_mc=1;
             auto args_1 = std::vector<double>{e1_pt,e1_signed_eta,e_iso_1};
             auto args_2 = std::vector<double>{e1_pt,e2_signed_eta,e_iso_2};
             ele1_trg = fns_["e_trg_binned_data"]->eval(args_1.data());
             ele2_trg = fns_["e_trg_binned_data"]->eval(args_2.data());
             //  ele_trg_mc = fns_["m_trg_mc"]->eval(args_1.data());
             // ele_trg = 1;
             ele1_trg_mc=1;
             ele2_trg_mc=1;
          }
        }
        ele1_trg = 1-((1-ele1_trg)*(1-ele2_trg));
        ele1_trg_mc = 1-((1-ele1_trg_mc)*(1-ele2_trg_mc));
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
        Muon const* muon2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
        double pt1 = muon->pt();
        double pt2 = muon2->pt();
        double m1_eta = fabs(muon->eta());
        double m2_eta = fabs(muon2->eta());
        double m1_signed_eta = muon->eta();
        double m2_signed_eta = muon2->eta();
        double m_iso_1 = PF04IsolationVal(muon, 0.5, 0);
        double m_iso_2 = PF04IsolationVal(muon2, 0.5, 0);
        double mu1_trg = 1.0;
        double mu2_trg = 1.0;
        double mu1_trg_mc = 1.0;
        double mu2_trg_mc = 1.0;
        if (mc_ == mc::fall15_76X){
          if(pt1<150){
            mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),mt_trig_data_->GetYaxis()->FindBin(pt1));
            mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),mt_trig_mc_->GetYaxis()->FindBin(pt1));
          } else {
            mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),(mt_trig_data_->GetYaxis()->FindBin(pt1)-1));
            mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt1)-1));
          }         

          if(pt2<150){
            mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),mt_trig_data_->GetYaxis()->FindBin(pt2));
            mu2_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m2_eta),mt_trig_mc_->GetYaxis()->FindBin(pt2));
          } else {
            mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),(mt_trig_data_->GetYaxis()->FindBin(pt2)-1));
            mu2_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m2_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt2)-1));
          }
         } else if(mc_ == mc::spring16_80X){
          if(scalefactor_file_==""){
           if(pt1<1000){
              mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),mt_trig_data_->GetYaxis()->FindBin(pt1));
              mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),mt_trig_mc_->GetYaxis()->FindBin(pt1));
            } else {
              mu1_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m1_eta),(mt_trig_data_->GetYaxis()->FindBin(pt1)-1));
              mu1_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m1_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt1)-1));
            }         
            if(pt2<1000){
              mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),mt_trig_data_->GetYaxis()->FindBin(pt2));
              mu2_trg_mc = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),mt_trig_data_->GetYaxis()->FindBin(pt2));
            } else {
              mu2_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m2_eta),(mt_trig_data_->GetYaxis()->FindBin(pt2)-1));
              mu2_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m2_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt2)-1));
            }

          } else {
             mu1_trg = 1;
             mu1_trg_mc=1;
             mu2_trg = 1;
             mu2_trg_mc=1;
             auto args_1 = std::vector<double>{pt1,m1_signed_eta,m_iso_1};
             auto args_2 = std::vector<double>{pt2,m2_signed_eta,m_iso_2};
             mu1_trg = fns_["m_trgOR_binned_data"]->eval(args_1.data());
             mu2_trg = fns_["m_trgOR_binned_data"]->eval(args_2.data());
             //  mu_trg_mc = fns_["m_trg_mc"]->eval(args_1.data());
             // mu_trg = 1;
             mu1_trg_mc=1;
             mu2_trg_mc=1;
           }
        }
        mu1_trg = 1-((1-mu1_trg)*(1-mu2_trg));
        mu1_trg_mc = 1-((1-mu1_trg_mc)*(1-mu2_trg_mc));
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
    if (do_singlemu_trg_weights_) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        double m_pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_trg = 1.0;
        double m_trg_mc = 1.0;
        if (m_eta < 0.9) {
          if (m_pt > 25.0)                { m_trg_mc = 0.93125; m_trg = 0.91403; }
        } else if (m_eta >= 0.9 && m_eta < 1.2) {
          if (m_pt > 25.0)                { m_trg_mc = 0.85067; m_trg = 0.81797; }
        } else {
          if (m_pt > 25.0)                { m_trg_mc = 0.80169; m_trg = 0.79945; }
        }
        if (trg_applied_in_mc_) m_trg = m_trg / m_trg_mc;
        weight *= (m_trg);
        event->Add("trigweight_1", m_trg);
    }

    if (do_idiso_weights_ || do_id_weights_) {
      if (channel_ == channel::et) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
        double pt = elec->pt();
        double sc_eta = fabs(elec->sc_eta());
        double e_signed_eta = elec->sc_eta();
        double e_iso = PF03IsolationVal(elec, 0.5, 0);
        if(era_ == era::data_2015||era_ == era::data_2016) sc_eta = fabs(elec->eta());
        double ele_idiso_mc =1.0;
        double ele_idiso_data =1.0;
        double ele_idiso=1.0;
        if ( mc_ ==mc::fall15_76X){
          if(pt<100){
            ele_idiso_data = et_idiso_data_->GetBinContent(et_idiso_data_->GetXaxis()->FindBin(sc_eta),et_idiso_data_->GetYaxis()->FindBin(pt));
            ele_idiso_mc = et_idiso_mc_->GetBinContent(et_idiso_mc_->GetXaxis()->FindBin(sc_eta),et_idiso_mc_->GetYaxis()->FindBin(pt));
          } else {
            ele_idiso_data = et_idiso_data_->GetBinContent(et_idiso_data_->GetXaxis()->FindBin(sc_eta),(et_idiso_data_->GetYaxis()->FindBin(pt)-1));
            ele_idiso_mc = et_idiso_mc_->GetBinContent(et_idiso_mc_->GetXaxis()->FindBin(sc_eta),(et_idiso_mc_->GetYaxis()->FindBin(pt)-1));
          }         
            ele_idiso = ele_idiso_data/ele_idiso_mc;

        } else if (mc_ == mc::spring16_80X){
           auto args_1 = std::vector<double>{pt,e_signed_eta};
           auto args_2 = std::vector<double>{pt,e_signed_eta,e_iso};
           if(e_iso < 0.1){
             ele_idiso = fns_["e_idiso0p10_desy_ratio"]->eval(args_1.data());
           } else ele_idiso = fns_["e_id_ratio"]->eval(args_1.data()) * fns_["e_iso_binned_ratio"]->eval(args_2.data()) ;
        }
        weight *= ele_idiso;
        event->Add("idisoweight_1",ele_idiso);
        event->Add("idisoweight_2",double(1.0));
      } else if (channel_ == channel::mt) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        double pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_signed_eta = muon->eta();
        double m_iso = PF04IsolationVal(muon, 0.5, 0);
        double mu_idiso_mc = 1.0;
        double mu_idiso_data = 1.0;
        double mu_idiso=1.0;
        if (mc_ == mc::fall15_76X){
          if(pt<100){
            mu_idiso_data = mt_idiso_data_->GetBinContent(mt_idiso_data_->GetXaxis()->FindBin(m_eta),mt_idiso_data_->GetYaxis()->FindBin(pt));
            mu_idiso_mc = mt_idiso_mc_->GetBinContent(mt_idiso_mc_->GetXaxis()->FindBin(m_eta),mt_idiso_mc_->GetYaxis()->FindBin(pt));
          } else {
            mu_idiso_data = mt_idiso_data_->GetBinContent(mt_idiso_data_->GetXaxis()->FindBin(m_eta),(mt_idiso_data_->GetYaxis()->FindBin(pt)-1));
            mu_idiso_mc = mt_idiso_mc_->GetBinContent(mt_idiso_mc_->GetXaxis()->FindBin(m_eta),(mt_idiso_mc_->GetYaxis()->FindBin(pt)-1));
          }         
            mu_idiso = mu_idiso_data/mu_idiso_mc;

        } else if(mc_ == mc::spring16_80X){
           auto args_1 = std::vector<double>{pt,m_signed_eta};
           auto args_2 = std::vector<double>{pt,m_signed_eta,m_iso};
           if(m_iso<0.15){
             mu_idiso = fns_["m_idiso0p15_desy_ratio"]->eval(args_1.data());
           } else mu_idiso = fns_["m_id_ratio"]->eval(args_1.data()) * fns_["m_iso_binned_ratio"]->eval(args_2.data()) ;
        }
          weight *= mu_idiso;
          event->Add("idisoweight_1", mu_idiso);
          event->Add("idisoweight_2", double(1.0));
      } else if (channel_ == channel::em) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
        double e_pt = elec->pt();
        double e_eta = fabs(elec->sc_eta());
        double e_signed_eta = elec->eta();
        if (era_ == era::data_2015 || era_ == era::data_2016) e_eta = fabs(elec->eta());
        double m_pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_signed_eta = muon->eta();
        double m_idiso = 1.0;
        double e_idiso = 1.0;
        double m_idiso_mc = 1.0;
        double m_idiso_data = 1.0;
        double e_idiso_mc = 1.0;
        double e_idiso_data = 1.0;
         if (mc_ ==mc::fall15_76X){
          if(m_pt<100){
            m_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_pt));
            m_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_pt));
          } else {
            m_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_pt)-1));
            m_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_pt)-1));
          }         
          if(e_pt<100){
            e_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_eta),em_e_idiso_data_->GetYaxis()->FindBin(e_pt));
            e_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_eta),em_e_idiso_mc_->GetYaxis()->FindBin(e_pt));
          } else {
            e_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_eta),(em_e_idiso_data_->GetYaxis()->FindBin(e_pt)-1));
            e_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_eta),(em_e_idiso_mc_->GetYaxis()->FindBin(e_pt)-1));
          }         

            m_idiso = m_idiso_data/m_idiso_mc;
            e_idiso = e_idiso_data/e_idiso_mc;

        } else if (mc_ == mc::spring16_80X){
           auto args_1 = std::vector<double>{m_pt,m_signed_eta};
           m_idiso = fns_["m_idiso0p20_desy_ratio"]->eval(args_1.data()) ;
           
           auto args_2 = std::vector<double>{e_pt,e_signed_eta};
           e_idiso = fns_["e_idiso0p15_desy_ratio"]->eval(args_2.data()) ;

         }       
        // if (do_id_weights_) mu_iso = 1.0;
        weight *= (e_idiso * m_idiso);
        event->Add("idisoweight_1", e_idiso);
        event->Add("idisoweight_2", m_idiso);
        event->Add("isoweight_1", double(1.0));
        event->Add("isoweight_2", double(1.0));
      }  else if (channel_ == channel::zmm){
        Muon const* muon_1 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        Muon const* muon_2 = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
        double m_1_pt = muon_1->pt();
        double m_1_eta = fabs(muon_1->eta());
        double m_1_signed_eta = muon_1->eta();
        double m_2_pt = muon_2->pt();
        double m_2_eta = fabs(muon_2->eta());
        double m_2_signed_eta = muon_2->eta();
        double m_1_iso = PF04IsolationVal(muon_1,0.5,0);
        double m_2_iso = PF04IsolationVal(muon_2,0.5,0);
        double m_1_idiso = 1.0;
        double m_2_idiso = 1.0;
        double m_1_idiso_mc = 1.0;
        double m_1_idiso_data = 1.0;
        double m_2_idiso_mc = 1.0;
        double m_2_idiso_data = 1.0;
        if ( mc_==mc::fall15_76X){
          if(m_1_pt<100){
            m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt));
            m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt));
          } else {
            m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt)-1));
            m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt)-1));
          }         
          if(m_2_pt<100){
            m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt));
            m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt));
          } else {
            m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt)-1));
            m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt)-1));
          }         

            m_1_idiso = m_1_idiso_data/m_1_idiso_mc;
            m_2_idiso = m_2_idiso_data/m_2_idiso_mc;
        } else if (mc_ == mc::spring16_80X){
           auto args1_1 = std::vector<double>{m_1_pt,m_1_signed_eta};
           auto args1_2 = std::vector<double>{m_1_pt,m_1_signed_eta,m_1_iso};
           auto args2_1 = std::vector<double>{m_2_pt,m_2_signed_eta};
           auto args2_2 = std::vector<double>{m_2_pt,m_2_signed_eta,m_2_iso};
           m_1_idiso = fns_["m_id_ratio"]->eval(args1_1.data()) * fns_["m_iso_binned_ratio"]->eval(args1_2.data()) ;
           m_2_idiso = fns_["m_id_ratio"]->eval(args2_1.data()) * fns_["m_iso_binned_ratio"]->eval(args2_2.data()) ;
          /*if(m_1_pt<1000){
            m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt));
            m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt));
          } else {
            m_1_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_1_pt)-1));
            m_1_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_1_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_1_pt)-1));
          }         
          if(m_2_pt<1000){
            m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt));
            m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt));
          } else {
            m_2_idiso_data = em_m_idiso_data_->GetBinContent(em_m_idiso_data_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_data_->GetYaxis()->FindBin(m_2_pt)-1));
            m_2_idiso_mc = em_m_idiso_mc_->GetBinContent(em_m_idiso_mc_->GetXaxis()->FindBin(m_2_eta),(em_m_idiso_mc_->GetYaxis()->FindBin(m_2_pt)-1));
          }         

            m_1_idiso = m_1_idiso_data/m_1_idiso_mc;
            m_2_idiso = m_2_idiso_data/m_2_idiso_mc;*/
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
        double e_1_eta = fabs(ele_1->eta());
        double e_2_pt = ele_2->pt();
        double e_2_eta = fabs(ele_2->eta());
        double e_1_idiso = 1.0;
        double e_2_idiso = 1.0;
        double e_1_idiso_mc = 1.0;
        double e_1_idiso_data = 1.0;
        double e_2_idiso_mc = 1.0;
        double e_2_idiso_data = 1.0;
        double e_2_signed_eta = ele_2->eta();
        double e_1_signed_eta = ele_1->eta();
        double e_1_iso = PF03IsolationVal(ele_1,0.5,0);
        double e_2_iso = PF03IsolationVal(ele_2,0.5,0);
        if (mc_==mc::fall15_76X){

          if(e_1_pt<100){
            e_1_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_1_eta),em_e_idiso_data_->GetYaxis()->FindBin(e_1_pt));
            e_1_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_1_eta),em_e_idiso_mc_->GetYaxis()->FindBin(e_1_pt));
          } else {
            e_1_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_1_eta),(em_e_idiso_data_->GetYaxis()->FindBin(e_1_pt)-1));
            e_1_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_1_eta),(em_e_idiso_mc_->GetYaxis()->FindBin(e_1_pt)-1));
          }         
          if(e_2_pt<100){
            e_2_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_2_eta),em_e_idiso_data_->GetYaxis()->FindBin(e_2_pt));
            e_2_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_2_eta),em_e_idiso_mc_->GetYaxis()->FindBin(e_2_pt));
          } else {
            e_2_idiso_data = em_e_idiso_data_->GetBinContent(em_e_idiso_data_->GetXaxis()->FindBin(e_2_eta),(em_e_idiso_data_->GetYaxis()->FindBin(e_2_pt)-1));
            e_2_idiso_mc = em_e_idiso_mc_->GetBinContent(em_e_idiso_mc_->GetXaxis()->FindBin(e_2_eta),(em_e_idiso_mc_->GetYaxis()->FindBin(e_2_pt)-1));
          }         

            e_1_idiso = e_1_idiso_data/e_1_idiso_mc;
            e_2_idiso = e_2_idiso_data/e_2_idiso_mc;
        } else if (mc_ == mc::spring16_80X){
           auto args1_1 = std::vector<double>{e_1_pt,e_1_signed_eta};
           auto args1_2 = std::vector<double>{e_1_pt,e_1_signed_eta,e_1_iso};
           auto args2_1 = std::vector<double>{e_2_pt,e_2_signed_eta};
           auto args2_2 = std::vector<double>{e_2_pt,e_2_signed_eta,e_2_iso};
           e_1_idiso = fns_["e_id_ratio"]->eval(args1_1.data()) * fns_["e_iso_binned_ratio"]->eval(args1_2.data()) ;
           e_2_idiso = fns_["e_id_ratio"]->eval(args2_1.data()) * fns_["e_iso_binned_ratio"]->eval(args2_2.data()) ;
        }

        weight *= (e_1_idiso * e_2_idiso);
        event->Add("idisoweight_1", e_1_idiso);
        event->Add("idisoweight_2", e_2_idiso);
        event->Add("isoweight_1", double(1.0));
        event->Add("isoweight_2", double(1.0));
       }
    }
    eventInfo->set_weight("lepton", weight);


    if (do_tt_muon_weights_) {
      Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
      double m_pt = muon->pt();
      double m_eta = fabs(muon->eta());
      double m_wt = 1.0;
      /*
      if (m_eta < 0.4) {
        if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 1.005;
        if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 0.982;
        if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 0.933;
        if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 0.893;
        if (m_pt > 100.0)                 m_wt = 0.939;
      } else if (m_eta >= 0.4 && m_eta < 1.2) {
        if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 1.009;
        if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 1.061;
        if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 1.054;
        if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 1.144;
        if (m_pt > 100.0)                 m_wt = 1.206;
      } else if (m_eta >= 1.2 && m_eta < 1.6) {
        if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 0.987;
        if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 1.074;
        if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 1.073;
        if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 1.149;
        if (m_pt > 100.0)                 m_wt = 1.443;
      } else {
        if (m_pt > 20.0 && m_pt <= 40.0)  m_wt = 1.075;
        if (m_pt > 40.0 && m_pt <= 60.0)  m_wt = 1.090;
        if (m_pt > 60.0 && m_pt <= 80.0)  m_wt = 1.172;
        if (m_pt > 80.0 && m_pt <= 100.0) m_wt = 1.297;
        if (m_pt > 100.0)                 m_wt = 1.603;
      }
      */
      if (m_eta < 0.4) {
        if (m_pt > 20.0 && m_pt <= 30.0)  m_wt = 1.027;
        if (m_pt > 30.0 && m_pt <= 50.0)  m_wt = 0.932;
        if (m_pt > 50.0 && m_pt <= 75.0)  m_wt = 0.889;
        if (m_pt > 75.0 && m_pt <= 100.0) m_wt = 0.913;
        if (m_pt > 100.0)                 m_wt = 0.913;
      } else if (m_eta >= 0.4 && m_eta < 0.8) {
        if (m_pt > 20.0 && m_pt <= 30.0)  m_wt = 1.068;
        if (m_pt > 30.0 && m_pt <= 50.0)  m_wt = 0.950;
        if (m_pt > 50.0 && m_pt <= 75.0)  m_wt = 1.060;
        if (m_pt > 75.0 && m_pt <= 100.0) m_wt = 1.009;
        if (m_pt > 100.0)                 m_wt = 1.009;
      } else if (m_eta >= 0.8) {
        if (m_pt > 20.0 && m_pt <= 30.0)  m_wt = 0.954;
        if (m_pt > 30.0 && m_pt <= 50.0)  m_wt = 1.068;
        if (m_pt > 50.0 && m_pt <= 75.0)  m_wt = 1.060;
        if (m_pt > 75.0 && m_pt <= 100.0) m_wt = 1.056;
        if (m_pt > 100.0)                 m_wt = 1.056;
      }
      eventInfo->set_weight("tt_muon_weight", m_wt);
    }


    if (do_emu_e_fakerates_) {
      Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
      double elefopt = (elec->pt() < 35) ? elec->pt() : 34.99;
      double eleEta = elec->eta();
      int eleptbin = ElectronFakeRateHist_PtEta->GetXaxis()->FindFixBin(elefopt);
      int eleetabin = ElectronFakeRateHist_PtEta->GetYaxis()->FindFixBin(eleEta);    
      double eleprob = ElectronFakeRateHist_PtEta->GetBinContent(eleptbin,eleetabin);
      double elefakerate = eleprob/(1.0 - eleprob);
      //double elefakerate_errlow = ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrLow)/pow((1-ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrLow)),2);
      //double elefakerate_errhigh = ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrHigh)/pow((1-ElectronFakeRateHist_PtEta->GetError(elefopt,fabs(elec->eta()),mithep::TH2DAsymErr::kStatErrHigh)),2);
      eventInfo->set_weight("emu_e_fakerate", elefakerate);
    }

    if (do_emu_m_fakerates_) {
      Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
      Double_t mufopt = (muon->pt() < 35) ? muon->pt() : 34.99;
      double muEta = muon->eta();
      int muptbin = MuonFakeRateHist_PtEta->GetXaxis()->FindFixBin(mufopt);
      int muetabin = MuonFakeRateHist_PtEta->GetYaxis()->FindFixBin(muEta);    
      double muprob = MuonFakeRateHist_PtEta->GetBinContent(muptbin,muetabin);
      double mufakerate = muprob/(1.0 - muprob);
      //double mufakerate_errlow = MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrLow)/pow((1-MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrLow)),2);
      //double mufakerate_errhigh = MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrHigh)/pow((1-MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrHigh)),2);
      eventInfo->set_weight("emu_m_fakerate", mufakerate);
    }

    if (do_etau_fakerate_ && (era_==era::data_2015||era_==era::data_2016)) {
      unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
      Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      double etau_fakerate_1=1.0;
      double etau_fakerate_2=1.0;
      if(channel_ == channel::et){
        if(gm2_==1||gm2_==3){
          if(fabs(tau->eta()) < 1.5){
             etau_fakerate_2 = 1.80;
          } else etau_fakerate_2=1.30;
        }
      } else {
        if(gm2_==1||gm2_==3){
          if(fabs(tau->eta()) < 1.5){
             etau_fakerate_2=1.02;
          } else etau_fakerate_2=1.11;
        }
      }
      if(channel_ == channel::tt){
        unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
        Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
        if(gm1_==1||gm1_==3){
          if(fabs(tau1->eta()) < 1.5){
             etau_fakerate_1=1.02;
          } else etau_fakerate_1=1.11;
        }
      }
     eventInfo->set_weight("etau_fakerate",etau_fakerate_1*etau_fakerate_2);
    }

    if (do_mtau_fakerate_ && era_==era::data_2016) {
      unsigned gm2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
      Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      double mtau_fakerate_1=1.0;
      double mtau_fakerate_2=1.0;
      if(channel_ == channel::mt){
        if(gm2_==2||gm2_==4){
          if(fabs(tau->eta()) < 0.4){
            mtau_fakerate_2 = 1.5;
          } else if(fabs(tau->eta()) < 0.8){
            mtau_fakerate_2 = 1.4;
          } else if(fabs(tau->eta()) < 1.2){
            mtau_fakerate_2 = 1.21;
          } else if(fabs(tau->eta()) < 1.7){
            mtau_fakerate_2=2.6;
          } else if(fabs(tau->eta()) < 2.3){
            mtau_fakerate_2=2.1;
          }
        }
      } else {
        if(gm2_==2||gm2_==4){
          if(fabs(tau->eta()) < 0.4){
            mtau_fakerate_2=1.15;
          } else if(fabs(tau->eta()) < 0.8){
            mtau_fakerate_2=1.15;
          } else if(fabs(tau->eta()) < 1.2){
            mtau_fakerate_2=1.18;
          } else if(fabs(tau->eta()) < 1.7){
            mtau_fakerate_2=1.2;
          } else if(fabs(tau->eta()) < 2.3){
            mtau_fakerate_2=1.3;
          }
        }
      }
      if(channel_ == channel::tt){
      unsigned gm1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
      Tau const* tau1 = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton1"));
        if(gm1_==2||gm1_==4){
          if(fabs(tau1->eta()) < 0.4){
            mtau_fakerate_1=1.15;
          } else if(fabs(tau1->eta()) < 0.8){
            mtau_fakerate_1=1.15;
          } else if(fabs(tau1->eta()) < 1.2){
            mtau_fakerate_1=1.18;
          } else if(fabs(tau1->eta()) < 1.7){
            mtau_fakerate_1=1.2;
          } else if(fabs(tau1->eta()) < 2.3){
            mtau_fakerate_1=1.3;
          }
        }
      }
     eventInfo->set_weight("mtau_fakerate",mtau_fakerate_1*mtau_fakerate_2);
    }



    return 0;
  }

  int HTTWeights::PostAnalysis() {
    return 0;
  }

  void HTTWeights::PrintInfo() {
    ;
  }


  double HTTWeights::Efficiency(double m, double m0, double sigma, double alpha,
    double n, double norm)
  {
    const double sqrtPiOver2 = 1.2533141373;
    const double sqrt2 = 1.4142135624;
    double sig = fabs((double) sigma);
    double t = (m - m0)/sig;
    if(alpha < 0)
      t = -t;
    double absAlpha = fabs(alpha/sig);
    double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
    double b = absAlpha - n/absAlpha;
    double ApproxErf;
    double arg = absAlpha / sqrt2;
    if (arg > 5.) ApproxErf = 1;
    else if (arg < -5.) ApproxErf = -1;
    else ApproxErf = TMath::Erf(arg);
    double leftArea = (1 + ApproxErf) * sqrtPiOver2;
    double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
    double area = leftArea + rightArea;
    if( t <= absAlpha ){
      arg = t / sqrt2;
      if(arg > 5.) ApproxErf = 1;
      else if (arg < -5.) ApproxErf = -1;
      else ApproxErf = TMath::Erf(arg);
      return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
    }
    else{
      return norm * (leftArea + a * (1/TMath::Power(t-b,n-1) -
        1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area;
    }
  }
}
