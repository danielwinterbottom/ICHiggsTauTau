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
    mc_(mc::summer12_53X),
    era_(era::data_2012_rereco) {
    do_trg_weights_           = false;
    trg_applied_in_mc_        = false;
    do_singlemu_trg_weights_  = false;
    do_etau_fakerate_         = false;
    do_mtau_fakerate_         = false;
    do_idiso_weights_         = false;
    do_id_weights_            = false;
    do_emu_e_fakerates_       = false;
    do_emu_m_fakerates_       = false;
    do_top_factors_           = false;
    do_btag_weight_           = false;
    btag_mode_                = 0;
    bfake_mode_               = 0;
    do_w_soup_                = false;
    do_dy_soup_               = false;
    do_dy_soup_htbinned_      = false;
    do_w_soup_htbinned_      = false;
    ggh_mass_                 = "";
    ggh_hist_                 = nullptr;
    ggh_hist_up_              = nullptr;
    ggh_hist_down_            = nullptr;
    do_tau_mode_scale_        = false;
    do_topquark_weights_      = false;
    do_top_jeteta_weights_    = false;
    do_tau_fake_weights_      = false;
    do_tt_muon_weights_       = false;
    gen_tau_collection_       = "genParticlesTaus";
    jets_label_               = "pfJetsPFlow";
    btag_label_ 	      = "combinedSecondaryVertexBJetTags";
    ditau_label_              = "emtauCandidates";
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
    mt_trig_mc_               = nullptr;
    mt_trig_data_             = nullptr;
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
    std::cout << boost::format(param_fmt()) % "btag_mode"           % btag_mode_;
    std::cout << boost::format(param_fmt()) % "bfake_mode"          % bfake_mode_;
    std::cout << boost::format(param_fmt()) % "do_topquark_weights" % do_topquark_weights_;
    std::cout << boost::format(param_fmt()) % "do_top_jeteta_weights" % do_top_jeteta_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_fake_weights" % do_tau_fake_weights_;
    std::cout << boost::format(param_fmt()) % "do_tau_id_weights"   % do_tau_id_weights_;
    std::cout << boost::format(param_fmt()) % "jets_label"          % jets_label_;
    std::cout << boost::format(param_fmt()) % "btag_label"          % btag_label_;
    std::cout << boost::format(param_fmt()) % "ditau_label"          % ditau_label_;

    if (do_tau_fake_weights_) {
     tau_fake_weights_ = new TF1("tau_fake_weights","(1.15743)-(0.00736136*x)+(4.3699e-05*x*x)-(1.188e-07*x*x*x)",0,200); 
    }

    if (ggh_mass_ != "" && mc_ == mc::fall11_42X) {
      // if (ggh_mass_ == "90" || ggh_mass_ == "95" ||
      //     ggh_mass_ == "100" || ggh_mass_ == "105") ggh_mass_ = "110";
      // if (ggh_mass_ == "150" || ggh_mass_ == "155" || ggh_mass_ == "160") ggh_mass_ = "145";
      // std::string file = "input/ggh_weights/weight_ptH_"+ggh_mass_+".root";
      std::string file = "input/ggh_weights/HRes_weight_pTH_mH125_7TeV.root";
      std::cout << boost::format(param_fmt()) % "higgs_pt_weights" % file;
      ggh_weights_ = new TFile(file.c_str());
      ggh_weights_->cd();
      ggh_hist_ = (TH1F*)gDirectory->Get("Nominal");
      ggh_hist_up_ = (TH1F*)gDirectory->Get("Up");
      ggh_hist_down_ = (TH1F*)gDirectory->Get("Down");
      // gDirectory->cd("powheg_weight");
      // ggh_hist_ = (TH1F*)gDirectory->Get(("weight_hqt_fehipro_fit_"+ggh_mass_).c_str());
    }
    if (ggh_mass_ != "" && mc_ == mc::summer12_53X) {
      if (  ggh_mass_ ==  "90"  || ggh_mass_ ==  "95" || ggh_mass_ == "100"
         || ggh_mass_ == "105"  || ggh_mass_ == "110" ) ggh_mass_ = "100";
      if (  ggh_mass_ == "115"  || ggh_mass_ ==  "120" || ggh_mass_ == "125"
         || ggh_mass_ == "130"  || ggh_mass_ == "135" ) ggh_mass_ = "125";
      if (  ggh_mass_ == "140"  || ggh_mass_ == "145" || ggh_mass_ == "150"
         || ggh_mass_ == "155"  || ggh_mass_ == "160" ) ggh_mass_ = "150";
      std::string file = "input/ggh_weights/HRes_weight_pTH_mH"+ggh_mass_+"_8TeV.root";
      std::cout << boost::format(param_fmt()) % "higgs_pt_weights" % file;
      ggh_weights_ = new TFile(file.c_str());
      ggh_weights_->cd();
      ggh_hist_ = (TH1F*)gDirectory->Get("Nominal");
      ggh_hist_up_ = (TH1F*)gDirectory->Get("Up");
      ggh_hist_down_ = (TH1F*)gDirectory->Get("Down");
    }

    if (do_emu_e_fakerates_ || do_emu_m_fakerates_) {
      std::string electron_fr_file, muon_fr_file;
      if (era_ == era::data_2012_rereco) {
        electron_fr_file  = "input/emu_fakerate/ElectronFakeRate_2012_19ifb_rereco.root";
        muon_fr_file      = "input/emu_fakerate/MuonFakeRate_2012_19ifb_rereco.root";
      } else {
        electron_fr_file  = "input/emu_fakerate/ElectronFakeRate_2011.root";
        muon_fr_file      = "input/emu_fakerate/MuonFakeRate_2011.root";
      }    
      std::cout << boost::format(param_fmt()) % "electron_fr_file"  % electron_fr_file;
      std::cout << boost::format(param_fmt()) % "muon_fr_file"      % muon_fr_file;
      ElectronFRFile = new TFile(electron_fr_file.c_str());
      MuonFRFile = new TFile(muon_fr_file.c_str());
      ElectronFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(ElectronFRFile->Get("ElectronFakeRateDenominatorV4_Ele8CaloIdLCaloIsoVLCombinedSample_ptThreshold35_PtEta"));
      if (mc_ == mc::fall11_42X) {
        MuonFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(MuonFRFile->Get("MuonFakeRateDenominatorV6_Mu8PtCombinedSample_ptThreshold25_PtEta"));        
      } else {
        MuonFakeRateHist_PtEta = (mithep::TH2DAsymErr*)(MuonFRFile->Get("MuonFakeRateDenominatorV6_Mu8PtCombinedSample_ptThreshold25_PtEta"));        
      }
      ElectronFakeRateHist_PtEta->SetDirectory(0);
      MuonFakeRateHist_PtEta->SetDirectory(0);
    }

    if (do_w_soup_) {
      std::cout << boost::format(param_fmt()) % "make_w_soup"      % true;
      std::cout << "nInc = " << n_inc_ << std::endl;
      w1_ = (n_inc_*f1_) / ( (n_inc_*f1_) + n1_ );
      w2_ = (n_inc_*f2_) / ( (n_inc_*f2_) + n2_ );
      w3_ = (n_inc_*f3_) / ( (n_inc_*f3_) + n3_ );
      w4_ = (n_inc_*f4_) / ( (n_inc_*f4_) + n4_ );
      std::cout << boost::format("f1=%-9.2f  n1=%-9i  w1=%-9.2f \n") % f1_ % n1_ % w1_;
      std::cout << boost::format("f2=%-9.2f  n2=%-9i  w2=%-9.2f \n") % f2_ % n2_ % w2_;
      std::cout << boost::format("f3=%-9.2f  n3=%-9i  w3=%-9.2f \n") % f3_ % n3_ % w3_;
      std::cout << boost::format("f4=%-9.2f  n4=%-9i  w4=%-9.2f \n") % f4_ % n4_ % w4_;
    }
    if (do_dy_soup_ ) {
      std::cout << boost::format(param_fmt()) % "make_dy_soup"      % true;
      std::cout << "nInc = " << zn_inc_ << std::endl;
      zw1_ = (zn_inc_*zf1_) / ( (zn_inc_*zf1_) + zn1_ );
      zw2_ = (zn_inc_*zf2_) / ( (zn_inc_*zf2_) + zn2_ );
      zw3_ = (zn_inc_*zf3_) / ( (zn_inc_*zf3_) + zn3_ );
      zw4_ = (zn_inc_*zf4_) / ( (zn_inc_*zf4_) + zn4_ );
      std::cout << boost::format("f1=%-9.2f  n1=%-9i  w1=%-9.2f \n") % zf1_ % zn1_ % zw1_;
      std::cout << boost::format("f2=%-9.2f  n2=%-9i  w2=%-9.2f \n") % zf2_ % zn2_ % zw2_;
      std::cout << boost::format("f3=%-9.2f  n3=%-9i  w3=%-9.2f \n") % zf3_ % zn3_ % zw3_;
      std::cout << boost::format("f4=%-9.2f  n4=%-9i  w4=%-9.2f \n") % zf4_ % zn4_ % zw4_;
    }
    if (do_w_soup_htbinned_ ) {
      std::cout << boost::format(param_fmt()) % "make_w_soup"      % true;
      f1_ = wxs1_/wxs0_;
      f2_ = wxs2_/wxs0_;
      f3_ = wxs3_/wxs0_;
      f4_ = wxs4_/wxs0_;
      w1_ = (n_inc_*f1_) / ((n_inc_*f1_) + n1_);
      w2_ = (n_inc_*f2_) / ((n_inc_*f2_) + n2_);
      w3_ = (n_inc_*f3_) / ((n_inc_*f3_) + n3_);
      w4_ = (n_inc_*f4_) / ((n_inc_*f4_) + n4_);
      std::cout << boost::format("f 100<ht<200=%-9.5f  n 100<ht<200=%-9i  w 100<ht<200=%-9.5f \n") % f1_ % n1_ % w1_;
      std::cout << boost::format("f 200<ht<400=%-9.5f  n 200<ht<400=%-9i  w 200<ht<400=%-9.5f \n") % f2_ % n2_ % w2_;
      std::cout << boost::format("f 400<ht<600=%-9.5f  n 400<ht<600=%-9i  w 400<ht<600=%-9.5f \n") % f3_ % n3_ % w3_;
      std::cout << boost::format("f ht>600=%-9.5f  n ht>600=%-9i  w ht>600=%-9.5f \n") % f4_ % n4_ % w4_;
    }

    if (do_dy_soup_htbinned_ ) {
      std::cout << boost::format(param_fmt()) % "make_dy_soup"      % true;
      zf1_ = zxs1_/zxs0_;
      zf2_ = zxs2_/zxs0_;
      zf3_ = zxs3_/zxs0_;
      zf4_ = zxs4_/zxs0_;
      zw1_ = (zn_inc_*zf1_) / ( (zn_inc_*zf1_) + zn1_ );
      zw2_ = (zn_inc_*zf2_) / ( (zn_inc_*zf2_) + zn2_ );
      zw3_ = (zn_inc_*zf3_) / ( (zn_inc_*zf3_) + zn3_ );
      zw4_ = (zn_inc_*zf4_) / ( (zn_inc_*zf4_) + zn4_ );
      std::cout << boost::format("f 100<ht<200=%-9.5f  n 100<ht<200=%-9i  w 100<ht<200=%-9.5f \n") % zf1_ % zn1_ % zw1_;
      std::cout << boost::format("f 200<ht<400=%-9.5f  n 200<ht<400=%-9i  w 200<ht<400=%-9.5f \n") % zf2_ % zn2_ % zw2_;
      std::cout << boost::format("f 400<ht<600=%-9.5f  n 400<ht<600=%-9i  w 400<ht<600=%-9.5f \n") % zf3_ % zn3_ % zw3_;
      std::cout << boost::format("f ht>600=%-9.5f  n ht>600=%-9i  w ht>600=%-9.5f \n") % zf4_ % zn4_ % zw4_;
    }

    return 0;
  }

  int HTTWeights::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(ditau_label_);

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
      if (mc_ == mc::summer12_53X || mc_ == mc::fall11_42X) {
        double weight_up   = ggh_hist_up_->GetBinContent(fbin)   / pt_weight;
        double weight_down = ggh_hist_down_->GetBinContent(fbin) / pt_weight;
        event->Add("wt_ggh_pt_up", weight_up);
        event->Add("wt_ggh_pt_down", weight_down);
      }
    }

    if (do_topquark_weights_) {
      double top_wt = 1.0;
      double top_wt_up = 1.0;
      double top_wt_down = 1.0;
      std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      for (unsigned i = 0; i < parts.size(); ++i) {
        if (parts[i]->status() == 3 && abs(parts[i]->pdgid()) == 6) {
          double pt = parts[i]->pt();
          pt = std::min(pt, 400.);
          if (mc_ == mc::summer12_53X) top_wt *= std::exp(0.156-0.00137*pt);
          if (mc_ == mc::fall11_42X)   top_wt *= std::exp(0.199-0.00166*pt);
        }
      }
      top_wt = std::sqrt(top_wt);
      top_wt_up = top_wt * top_wt;
      top_wt_down = 1.0;
      event->Add("wt_tquark_up", top_wt_up / top_wt);
      event->Add("wt_tquark_down", top_wt_down / top_wt);
      eventInfo->set_weight("topquark_weight", top_wt);
    }
    
    if (do_tau_fake_weights_) {
      Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      double fake_pt = tau->pt() < 200. ? tau->pt() : 200.;
      double fake_weight = tau_fake_weights_->Eval(fake_pt);
      eventInfo->set_weight("tau_fake_weight",fake_weight);
      double weight_up   = (fake_weight + 0.5*(1.0-fake_weight)) / fake_weight;
      double weight_down = (fake_weight - 0.5*(1.0-fake_weight)) / fake_weight;
      event->Add("wt_tau_fake_up", weight_up);
      event->Add("wt_tau_fake_down", weight_down);
    }

    if (do_tau_id_weights_) {
      std::vector<Candidate *> tau = { (dilepton[0]->GetCandidate("lepton2")) };
      std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_tau_collection_);
      std::vector<GenJet> gen_taus = BuildTauJets(particles, false,false);
      std::vector<GenJet *> gen_taus_ptr;
      for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
      std::vector<std::pair<Candidate*, GenJet*> > matches = MatchByDR(tau, gen_taus_ptr, 0.5, true, true);
      double weight_up    = 1.0;
      double weight_down  = 1.0;
      if (matches.size() == 1) {
        double weight = (0.20*matches[0].second->pt())/1000.;
        weight_up   = 1.0 +weight;
        weight_down = std::max(0.0, 1.0 - weight);
        event->Add("wt_tau_id_up", weight_up);
        event->Add("wt_tau_id_down", weight_down);
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
    
    
    if (do_btag_weight_) {
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
      //double no_btag_weight = btag_weight.GetWeight(jets, "CSVM", 0, 0, is_2012_);
      //double inclusive_btag_weight = btag_weight.GetWeight(jets, "CSVM", 1, 99, is_2012_);
      double no_btag_weight = 1.0;
      double inclusive_btag_weight = 1.0;
      BTagWeight::payload set = BTagWeight::payload::ALL2011;
      if (mc_ == mc::summer12_53X) set = BTagWeight::payload::EPS13;
      std::map<std::size_t, bool> retag_result = btag_weight.ReTag(jets, set, BTagWeight::tagger::CSVM, btag_mode_, bfake_mode_);
      event->Add("no_btag_weight", no_btag_weight);
      event->Add("inclusive_btag_weight", inclusive_btag_weight);
      event->Add("retag_result", retag_result);
    }

    if (do_trg_weights_) {
      if (channel_ == channel::et) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double e_pt = elec->pt();
        double e_eta = fabs(elec->sc_eta());
        double t_pt = tau->pt();
        double t_eta = fabs(tau->eta());
        double ele_trg = 1.0;
        double tau_trg = 1.0;
        double ele_trg_mc = 1.0;
        double tau_trg_mc = 1.0;
        if (mc_ == mc::summer12_53X) {
          double emc22ABCD = 1.0;
          double edataRERECO = 1.0;
          if (e_eta < 1.479) {
            emc22ABCD  = Efficiency(e_pt, 21.7243, 0.619015, 0.739301, 1.34903, 1.02594); // MC Reweighted to ABCD
            edataRERECO = Efficiency(e_pt, 22.9704, 1.0258, 1.26889, 1.31024, 1.06409);
          } else {
            emc22ABCD  = Efficiency(e_pt, 22.1217, 1.34054, 1.8885, 1.01855, 4.7241); // MC Reweighted to ABCD
            edataRERECO = Efficiency(e_pt, 21.9816, 1.40993, 0.978597, 2.33144, 0.937552);
          }
          if (era_ == era::data_2012_rereco) {
            ele_trg = edataRERECO;
            ele_trg_mc = emc22ABCD;
          }

          double tdataRERECO = 1.0;
          double tmcRERECO   = 1.0;

          if (t_eta < 1.5) {
            tdataRERECO = Efficiency(t_pt, 18.538229, 0.651562, 0.324869, 13.099048, 0.902365);
            tmcRERECO   = Efficiency(t_pt, 18.605055, 0.264062, 0.139561, 4.792849, 0.915035);
          } else {
            tdataRERECO = Efficiency(t_pt, 18.756548, 0.230732, 0.142859, 3.358497, 0.851919);
            tmcRERECO   = Efficiency(t_pt, 18.557810, 0.280908, 0.119282, 17.749043, 0.865756);

          }
          if (era_ == era::data_2012_rereco) {
            tau_trg = tdataRERECO;
            tau_trg_mc = tmcRERECO;
          }
        } else if (mc_ == mc::fall11_42X) {
          if (fabs(e_eta) < 1.479) {
            double ele15_eb = Efficiency(e_pt, 14.8474, 0.634431, 0.547211, 2.11286, 0.978835);
            double ele18_eb = Efficiency(e_pt, 18.318, 0.811329, 0.938142, 1.85834, 0.927328);
            double ele20_eb = Efficiency(e_pt, 20.244, 0.783044, 0.672118, 2.35819, 0.972123);
            // double ele15_eb = Efficiency(e_pt, 14.8772, 0.311255, 0.221021, 1.87734, 0.986665);
            // double ele18_eb = Efficiency(e_pt, 18.3193, 0.443703, 0.385554, 1.86523, 0.986514);
            // double ele20_eb = Efficiency(e_pt, 20.554, 0.683776, 0.855573, 1.45917, 1.03957);
            ele_trg = (0.41 * ele15_eb) + (0.39 * ele18_eb) + (0.2 * ele20_eb);
            ele_trg_mc = Efficiency(e_pt, 16.9487, 0.162306, 0.0506008, 2.37608, 0.973829);
            // ele_trg_mc = Efficiency(e_pt, 15.1804, 2.43126, 3.85048, 1.72284, 0.998507);
          } else {
            double ele15_ee = Efficiency(e_pt, 16.47, 1.20691, 1.54605, 1.68873, 0.985916);
            double ele18_ee = Efficiency(e_pt, 16.9487, 0.162306, 0.0506008, 2.37608, 0.973829);
            double ele20_ee = Efficiency(e_pt, 22.4559, 1.70818, 0.917021, 3.86463, 0.964978);
            // double ele15_ee = Efficiency(e_pt, 15.6629, 0.759192, 0.47756, 2.02154, 0.998816);
            // double ele18_ee = Efficiency(e_pt, 19.6586, 0.682633, 0.279486, 2.66423, 0.973455);
            // double ele20_ee = Efficiency(e_pt, 23.6386, 1.60775, 1.72093, 1.4131, 1.13962);
            ele_trg = (0.41 * ele15_ee) + (0.39 * ele18_ee) + (0.2 * ele20_ee);
            ele_trg_mc = Efficiency(e_pt, 18.05, 1.66609, 2.3836, 1.49316, 1.01229);
            // ele_trg_mc = Efficiency(e_pt, 16.993, 0.0693958, 0.00695096, 1.9566, 1.00632);
          }
          if (fabs(t_eta) < 1.5) {
            double tau20l_eb = Efficiency(t_pt, 19.3916,  0.996964,  1.70131,   1.38002,   0.903245);
            double tau20m_eb = Efficiency(t_pt, 19.5667,  1.15203 ,  1.68126,   1.40025,   0.848033);
            double tau20t_eb = Efficiency(t_pt, 19.6013,  0.987317,  1.08015,   1.88592,   0.776894);
            tau_trg = (0.25 * tau20l_eb) + (0.59 * tau20m_eb) + (0.16 * tau20t_eb);
            tau_trg_mc = Efficiency(t_pt, 19.468, 0.0615381, 0.0349325, 1.59349, 0.860096);
          } else {
            double tau20l_ee = Efficiency(t_pt, 18.8166,  0.526632,  0.20666,   6.80392,   0.903245);
            double tau20m_ee = Efficiency(t_pt, 18.8476,  0.528963,  0.16717,   3.65814,   0.749759);
            double tau20t_ee = Efficiency(t_pt, 18.8859,  0.271301,  0.128008,  1.50993,   0.825122);
            tau_trg = (0.25 * tau20l_ee) + (0.59 * tau20m_ee) + (0.16 * tau20t_ee);
            tau_trg_mc = Efficiency(t_pt, 19.3862, 0.247148, 0.123187, 2.87108, 0.790894);
          }
        } else if (mc_ == mc::spring15_74X){
          if(e_pt<100){
            ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),et_trig_data_->GetYaxis()->FindBin(e_pt));
            ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),et_trig_mc_->GetYaxis()->FindBin(e_pt));
          } else {
            ele_trg = et_trig_data_->GetBinContent(et_trig_data_->GetXaxis()->FindBin(e_eta),(et_trig_data_->GetYaxis()->FindBin(e_pt)-1));
            ele_trg_mc = et_trig_mc_->GetBinContent(et_trig_mc_->GetXaxis()->FindBin(e_eta),(et_trig_mc_->GetYaxis()->FindBin(e_pt)-1));
          }         
          tau_trg=1;
          tau_trg_mc=1;
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
        double m_a_eta = muon->eta();
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double t_pt = tau->pt();
        double t_eta = fabs(tau->eta());
        double mu_trg = 1.0;
        double tau_trg = 1.0;
        double mu_trg_mc = 1.0;
        double tau_trg_mc = 1.0;
        if (mc_ == mc::summer12_53X) {
          double muRERECO = 1.0;
          double mcRERECO = 1.0;
          if (m_a_eta >= 0 && m_a_eta < 0.8) {
            muRERECO  = Efficiency(pt, 17.313, 0.662731, 1.3412, 1.05778, 1.26624);
            mcRERECO  = Efficiency(pt, 15.9289, 0.0271317, 0.00448573, 1.92101, 0.978625);
          } else if (m_a_eta >= 0.8 && m_a_eta < 1.2) {
            muRERECO  = Efficiency(pt, 16.9966, 0.550532, 0.807863, 1.55402, 0.885134);
            mcRERECO  = Efficiency(pt, 16.5678, 0.328333, 0.354533, 1.67085, 0.916992);
          } else if (m_a_eta >= 1.2) {
            muRERECO  = Efficiency(pt, 15.9962, 0.000106195, 4.95058e-08, 1.9991, 0.851294);
            mcRERECO  = Efficiency(pt, 15.997, 7.90069e-05, 4.40036e-08, 1.66272, 0.884502);
          } else if (m_a_eta >= -0.8 && m_a_eta < 0) {
            muRERECO  = Efficiency(pt, 16.4307, 0.226312, 0.265553, 1.55756, 0.974462);
            mcRERECO  = Efficiency(pt, 15.9556, 0.0236127, 0.00589832, 1.75409, 0.981338);
          } else if (m_a_eta >= -1.2 && m_a_eta < -0.8) {
            muRERECO  = Efficiency(pt, 17.3974, 0.804001, 1.47145, 1.24295, 0.928198);
            mcRERECO  = Efficiency(pt, 17.3135, 0.747636, 1.21803, 1.40611, 0.934983);
          } else if (m_a_eta < -1.2) {
            muRERECO  = Efficiency(pt, 15.9977, 7.64004e-05, 6.4951e-08, 1.57403, 0.865325);
            mcRERECO  = Efficiency(pt, 16.0051, 2.45144e-05, 4.3335e-09, 1.66134, 0.87045);
          }
          if (era_ == era::data_2012_rereco) {
            mu_trg = muRERECO;
            mu_trg_mc = mcRERECO;
          }

          double tdataRERECO   = 1.0;
          double tmcRERECO     = 1.0;

          if (fabs(t_eta) < 1.5) {
            tdataRERECO  = Efficiency(t_pt, 18.604910, 0.276042, 0.137039, 2.698437, 0.940721);
            tmcRERECO    = Efficiency(t_pt, 18.532997, 1.027880, 2.262950, 1.003322, 5.297292);
          } else {
            tdataRERECO  = Efficiency(t_pt, 18.701715, 0.216523, 0.148111, 2.245081, 0.895320);
            tmcRERECO    = Efficiency(t_pt, 18.212782, 0.338119, 0.122828, 12.577926, 0.893975);

          }
          if (era_ == era::data_2012_rereco) {
            tau_trg = tdataRERECO;
            tau_trg_mc = tmcRERECO;
          }
        } else if (mc_ == mc::fall11_42X) {
          if (m_eta < 0.8) {
            double mu12 = 0.920;
            double mu15 = 0.917;
            double mu15_2p1 = Efficiency(pt, 15.9877, 2.90938e-07, 2.63922e-11, 5.81194, 0.906943);
            mu_trg = (0.034 * mu12) + (0.368 * mu15) + (0.598 * mu15_2p1);
            mu_trg_mc = 0.923;
          } else if (m_eta >= 0.8 && m_eta < 1.2) {
            double mu12 = 0.868;
            double mu15 = 0.871;
            double mu15_2p1 = Efficiency(pt, 15.9995, 1.35931e-07, 7.88264e-11, 4.60253, 0.855461);
            mu_trg = (0.034 * mu12) + (0.368 * mu15) + (0.598 * mu15_2p1);
            mu_trg_mc = 0.879;
          } else {
            double mu12 = 0.845;
            double mu15 = 0.864;
            double mu15_2p1 = Efficiency(pt, 15.9084, 2.27242e-12, 8.77174e-14, 1.00241, 12.9909);
            mu_trg = (0.034 * mu12) + (0.368 * mu15) + (0.598 * mu15_2p1);
            mu_trg_mc = 0.839;
          }
          // if (fabs(m_eta) < 1.4) {
          //   double mu1215_eb = 0.901;
          //   double mu15_2p1_eb = Efficiency(pt, 15.06,  0.55278,   1.34236,   1.003,   3.36767);
          //   mu_trg = (0.402 * mu1215_eb) + (0.598 * mu15_2p1_eb);
          //   mu_trg_mc = 0.917;
          // } else {
          //   double mu1215_ee = 0.863;
          //   double mu15_2p1_ee = Efficiency(pt, 15.03175,   0.866114,  1.25009,   1.63711,   0.844906);
          //   mu_trg = (0.402 * mu1215_ee) + (0.598 * mu15_2p1_ee);
          //   mu_trg_mc = 0.836;
          // }

          if (fabs(t_eta) < 1.5) {
            double tau10l_eb = Efficiency(t_pt, 13.6046,   1.66291,   1.71551,   141.929,   0.910686);
            double tau15l_eb = Efficiency(t_pt, 13.9694,   0.084835,  0.057743,  1.50674,   0.984976);
            double tau20l_eb = Efficiency(t_pt, 19.2102,   1.26519,   2.48994,   1.04699,  1.3492);
            tau_trg = (0.043 * tau10l_eb) + (0.359 * tau15l_eb) + (0.598 * tau20l_eb);
            tau_trg_mc = Efficiency(t_pt, 14.4601, 0.0485272, 0.03849, 1.48324, 0.965257);
          } else {
            double tau10l_ee = Efficiency(t_pt, -0.392211,   7.90467,   5.48228,   134.599,   0.925858);
            double tau15l_ee = Efficiency(t_pt, 14.435,  1.34952,   2.43996,   1.03631,   1.79081);
            double tau20l_ee = Efficiency(t_pt, 19.2438,   1.37298,   1.76448,   1.73935,   0.901291);
            tau_trg = (0.043 * tau10l_ee) + (0.359 * tau15l_ee) + (0.598 * tau20l_ee);
            tau_trg_mc = Efficiency(t_pt, 14.4451, 0.0790573, 0.0732472, 1.47046, 0.942028);
          }
        } else if (mc_ == mc::spring15_74X){
          if(pt<100){
            mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),mt_trig_data_->GetYaxis()->FindBin(pt));
            mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),mt_trig_mc_->GetYaxis()->FindBin(pt));
          } else {
            mu_trg = mt_trig_data_->GetBinContent(mt_trig_data_->GetXaxis()->FindBin(m_eta),(mt_trig_data_->GetYaxis()->FindBin(pt)-1));
            mu_trg_mc = mt_trig_mc_->GetBinContent(mt_trig_mc_->GetXaxis()->FindBin(m_eta),(mt_trig_mc_->GetYaxis()->FindBin(pt)-1));
          }         
          tau_trg=1;
          tau_trg_mc=1;
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
        double m_pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_trg = 1.0;
        double m_trg_mc = 1.0;
        double e_trg = 1.0;
        double e_trg_mc = 1.0;
        double m_trg_17 = 1.0;
        double m_trg_17_mc = 1.0;
        double m_trg_8 = 1.0;
        double m_trg_8_mc = 1.0;
        double e_trg_17 = 1.0;
        double e_trg_17_mc = 1.0;
        double e_trg_12 = 1.0;
        double e_trg_12_mc = 1.0;
        if (mc_ == mc::summer12_53X) {
          if (era_ == era::data_2012_rereco) {
            if (m_eta < 0.8) {
              if (m_pt <= 15.0)                { m_trg_mc = 0.9870; m_trg = 0.9701; }
              if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9868; m_trg = 0.9720; }
              if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9812; m_trg = 0.9764; }
              if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9853; m_trg = 0.9725; }
              if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9826; m_trg = 0.9785; }
              if (m_pt > 35.0)                 { m_trg_mc = 0.9692; m_trg = 0.9679; }
            } else if (m_eta >= 0.8 && m_eta < 1.2) {
              if (m_pt <= 15.0)                { m_trg_mc = 0.9666; m_trg = 0.9419; }
              if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9445; m_trg = 0.9305; }
              if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9822; m_trg = 0.9439; }
              if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9618; m_trg = 0.9405; }
              if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9454; m_trg = 0.9342; }
              if (m_pt > 35.0)                 { m_trg_mc = 0.9759; m_trg = 0.9310; }
            } else if (m_eta >= 1.2 && m_eta < 1.6) {
              if (m_pt <= 15.0)                { m_trg_mc = 0.9357; m_trg = 0.9303; }
              if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9512; m_trg = 0.9267; }
              if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9640; m_trg = 0.9366; }
              if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9538; m_trg = 0.9218; }
              if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9247; m_trg = 0.9184; }
              if (m_pt > 35.0)                 { m_trg_mc = 0.9522; m_trg = 0.9092; }
            } else {
              if (m_pt <= 15.0)                { m_trg_mc = 0.9415; m_trg = 0.8623; }
              if (m_pt > 15.0 && m_pt <= 20.0) { m_trg_mc = 0.9638; m_trg = 0.8995; }
              if (m_pt > 20.0 && m_pt <= 25.0) { m_trg_mc = 0.9657; m_trg = 0.9134; }
              if (m_pt > 25.0 && m_pt <= 30.0) { m_trg_mc = 0.9287; m_trg = 0.8824; }
              if (m_pt > 30.0 && m_pt <= 35.0) { m_trg_mc = 0.9573; m_trg = 0.8990; }
              if (m_pt > 35.0)                 { m_trg_mc = 0.9606; m_trg = 0.9016; }
            }
            if (e_eta < 0.8) {
              if (e_pt <= 15.0)                { e_trg_mc = 0.7615; e_trg = 0.7270; }
              if (e_pt > 15.0 && e_pt <= 20.0) { e_trg_mc = 0.8903; e_trg = 0.8752; }
              if (e_pt > 20.0 && e_pt <= 25.0) { e_trg_mc = 0.9419; e_trg = 0.9142; }
              if (e_pt > 25.0 && e_pt <= 30.0) { e_trg_mc = 0.9591; e_trg = 0.9368; }
              if (e_pt > 30.0 && e_pt <= 35.0) { e_trg_mc = 0.9454; e_trg = 0.9499; }
              if (e_pt > 35.0)                 { e_trg_mc = 0.9629; e_trg = 0.9689; }
            } else if (e_eta >= 0.8 && e_eta < 1.479) {
              if (e_pt <= 15.0)                { e_trg_mc = 0.8186; e_trg = 0.7380; }
              if (e_pt > 15.0 && e_pt <= 20.0) { e_trg_mc = 0.9367; e_trg = 0.9059; }
              if (e_pt > 20.0 && e_pt <= 25.0) { e_trg_mc = 0.9746; e_trg = 0.9484; }
              if (e_pt > 25.0 && e_pt <= 30.0) { e_trg_mc = 0.9757; e_trg = 0.9630; }
              if (e_pt > 30.0 && e_pt <= 35.0) { e_trg_mc = 0.9748; e_trg = 0.9642; }
              if (e_pt > 35.0)                 { e_trg_mc = 0.9763; e_trg = 0.9809; }
            } else {
              if (e_pt <= 15.0)                { e_trg_mc = 0.7652; e_trg = 0.6899; }
              if (e_pt > 15.0 && e_pt <= 20.0) { e_trg_mc = 0.9126; e_trg = 0.8635; }
              if (e_pt > 20.0 && e_pt <= 25.0) { e_trg_mc = 0.9654; e_trg = 0.9356; }
              if (e_pt > 25.0 && e_pt <= 30.0) { e_trg_mc = 0.9732; e_trg = 0.9466; }
              if (e_pt > 30.0 && e_pt <= 35.0) { e_trg_mc = 0.9875; e_trg = 0.9735; }
              if (e_pt > 35.0)                 { e_trg_mc = 0.9787; e_trg = 0.9802; }
            }
          }
        } else if (mc_ == mc::fall11_42X) {
          if (m_eta < 0.8) {
            if (m_pt <= 15.0)                 { m_trg_mc = 0.984910277; m_trg = 0.9660; }
            if (m_pt > 15.0 && m_pt <= 20.0)  { m_trg_mc = 0.985725938; m_trg = 0.9668; }
            if (m_pt > 20.0 && m_pt <= 25.0)  { m_trg_mc = 0.984354758; m_trg = 0.9878; }
            if (m_pt > 25.0 && m_pt <= 30.0)  { m_trg_mc = 0.986547085; m_trg = 0.9680; }
            if (m_pt > 30.0)                  { m_trg_mc = 0.988551569; m_trg = 0.9671; }
          } else if (m_eta >= 0.8 && m_eta < 1.2) {
            if (m_pt <= 15.0)                 { m_trg_mc = 0.967889432; m_trg = 0.9314; }
            if (m_pt > 15.0 && m_pt <= 20.0)  { m_trg_mc = 0.975998366; m_trg = 0.9556; }
            if (m_pt > 20.0 && m_pt <= 25.0)  { m_trg_mc = 0.979876161; m_trg = 0.9495; }
            if (m_pt > 25.0 && m_pt <= 30.0)  { m_trg_mc = 0.976183175; m_trg = 0.9550; }
            if (m_pt > 30.0)                  { m_trg_mc = 0.982728307; m_trg = 0.9502; }
          } else {
            if (m_pt <= 15.0)                 { m_trg_mc = 0.958862737; m_trg = 0.9207; }
            if (m_pt > 15.0 && m_pt <= 20.0)  { m_trg_mc = 0.966324889; m_trg = 0.9613; }
            if (m_pt > 20.0 && m_pt <= 25.0)  { m_trg_mc = 0.967505674; m_trg = 0.9379; }
            if (m_pt > 25.0 && m_pt <= 30.0)  { m_trg_mc = 0.970721649; m_trg = 0.9416; }
            if (m_pt > 30.0)                  { m_trg_mc = 0.969919372; m_trg = 0.9383; }
          }
          if (e_eta < 0.8) {
            if (e_pt <= 15.0)                 { e_trg_mc = 0.786907216; e_trg = 0.7633; }
            if (e_pt > 15.0 && e_pt <= 20.0)  { e_trg_mc = 0.806727273; e_trg = 0.8874; }
            if (e_pt > 20.0 && e_pt <= 25.0)  { e_trg_mc = 0.938775510; e_trg = 0.9200; }
            if (e_pt > 25.0 && e_pt <= 30.0)  { e_trg_mc = 0.958571429; e_trg = 0.9394; }
            if (e_pt > 30.0)                  { e_trg_mc = 0.974040404; e_trg = 0.9643; }
          } else if (e_eta >= 0.8 && e_eta < 1.479) {
            if (e_pt <= 15.0)                 { e_trg_mc = 0.750612245; e_trg = 0.7356; }
            if (e_pt > 15.0 && e_pt <= 20.0)  { e_trg_mc = 0.849722222; e_trg = 0.9177; }
            if (e_pt > 20.0 && e_pt <= 25.0)  { e_trg_mc = 0.961111111; e_trg = 0.9515; }
            if (e_pt > 25.0 && e_pt <= 30.0)  { e_trg_mc = 0.977171717; e_trg = 0.9674; }
            if (e_pt > 30.0)                  { e_trg_mc = 0.977800000; e_trg = 0.9778; }
          } else {
            if (e_pt <= 15.0)                 { e_trg_mc = 0.865432099; e_trg = 0.7010; }
            if (e_pt > 15.0 && e_pt <= 20.0)  { e_trg_mc = 0.809523810; e_trg = 0.8500; }
            if (e_pt > 20.0 && e_pt <= 25.0)  { e_trg_mc = 0.971145833; e_trg = 0.9323; }
            if (e_pt > 25.0 && e_pt <= 30.0)  { e_trg_mc = 0.967291667; e_trg = 0.9286; }
            if (e_pt > 30.0)                  { e_trg_mc = 0.983535354; e_trg = 0.9737; }
          }
        } else if (mc_ == mc::spring15_74X){
          if(e_pt<100){
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
          if(m_pt<100){
            m_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),em_e17_trig_data_->GetYaxis()->FindBin(m_pt));
            m_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),em_e17_trig_mc_->GetYaxis()->FindBin(m_pt));
            m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(e_eta),em_m8_trig_data_->GetYaxis()->FindBin(m_pt));
            m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(e_eta),em_m8_trig_mc_->GetYaxis()->FindBin(m_pt));
          } else {
            m_trg_17 = em_e17_trig_data_->GetBinContent(em_e17_trig_data_->GetXaxis()->FindBin(e_eta),(em_e17_trig_data_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_17_mc = em_e17_trig_mc_->GetBinContent(em_e17_trig_mc_->GetXaxis()->FindBin(e_eta),(em_e17_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_8 = em_m8_trig_data_->GetBinContent(em_m8_trig_data_->GetXaxis()->FindBin(e_eta),(em_m8_trig_data_->GetYaxis()->FindBin(m_pt)-1));
            m_trg_8_mc = em_m8_trig_mc_->GetBinContent(em_m8_trig_mc_->GetXaxis()->FindBin(e_eta),(em_m8_trig_mc_->GetYaxis()->FindBin(m_pt)-1));
          }         
       }
       if(mc_ !=mc::spring15_74X){
        if (trg_applied_in_mc_) {
          m_trg = m_trg / m_trg_mc;
          e_trg = e_trg / e_trg_mc;
        }
        weight *= (e_trg * m_trg);
        event->Add("trigweight_1", e_trg);
        event->Add("trigweight_2", m_trg);
       } else {
        if (trg_applied_in_mc_){
          e_trg = (m_trg_17*e_trg_12 + m_trg_8*e_trg_17 - m_trg_17*e_trg_17)/(m_trg_17_mc*e_trg_12_mc + m_trg_8_mc*e_trg_17_mc - m_trg_17_mc*e_trg_17_mc);
        } else e_trg = (m_trg_17*e_trg_12 + m_trg_8*e_trg_17 - m_trg_17*e_trg_17);

        weight *= (e_trg);
        //trigweight_1 is actually the full trigger weight because of the way the efficiencies are combined
        event->Add("trigweight_1", e_trg);
        event->Add("trigweight_2", double(1.0));
       }
      } else if (channel_ == channel::mtmet) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        double pt = muon->pt();
        double m_eta = fabs(muon->eta());
        Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
        double t_pt = tau->pt();
        double t_eta = fabs(tau->eta());
        double mu_trg = 1.0;
        double tau_trg = 1.0;
        double mu_trg_mc = 1.0;
        double tau_trg_mc = 1.0;
        if (mc_ == mc::summer12_53X) {
          double muD = 1.0;
          double mcD = 1.0;
          if (m_eta >= 0 && m_eta < 0.8) {
            muD   = Efficiency(pt, 6.9755, 0.000173944, 4.65439e-07, 1.20934, 1.35392);
          } else if (m_eta >= 0.8 && m_eta < 1.2) {
            muD   = Efficiency(pt, 7.005, 1.39453e-06, 1.77824e-11, 1.30978, 1.08927);
          } else {
            muD   = Efficiency(pt, 7.00501, 2.69916e-06, 2.54492e-11, 1.8705, 0.864745);
          } 
          mu_trg = muD;
          mu_trg_mc = mcD;
          double tdataD   = 1.0;
          double tmcD    = 1.0;
          if (fabs(t_eta) < 1.5) {
            tdataD  = Efficiency(t_pt, 18.434069, 0.530757, 0.201776, 7.190775, 0.932454);
            tmcD    = Efficiency(t_pt, 18.539244, 1.326609, 2.909963, 1.001269, 12.692931);
          } else {
            tdataD  = Efficiency(t_pt, 18.671316, 1.572636, 3.074206, 1.000632, 41.692646);
            tmcD    = Efficiency(t_pt, 18.326162, 1.849068, 3.737118, 140.684357, 0.889961);
          }
          tau_trg = tdataD;
          tau_trg_mc = tmcD;
        } 
        if (trg_applied_in_mc_) {
          mu_trg = mu_trg / mu_trg_mc;
          tau_trg = tau_trg / tau_trg_mc;
        }
        weight *= (mu_trg * tau_trg);
        event->Add("trigweight_1", mu_trg);
        event->Add("trigweight_2", tau_trg);
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
        double ele_id = 1.0;
        double ele_iso = 1.0;
        double ele_idiso_mc =1.0;
        double ele_idiso_data =1.0;
        double ele_idiso=1.0;
        if (mc_ == mc::summer12_53X) {
          if (era_ == era::data_2012_rereco) {
            if (pt > 24.0 && pt <= 30.0 && sc_eta < 1.479)  { ele_id = 0.8999; ele_iso = 0.9417; }
            if (pt > 24.0 && pt <= 30.0 && sc_eta >= 1.479) { ele_id = 0.7945; ele_iso = 0.9471; }
            if (pt > 30.0 && sc_eta < 1.479)                { ele_id = 0.9486; ele_iso = 0.9804; }
            if (pt > 30.0 && sc_eta >= 1.479)               { ele_id = 0.8866; ele_iso = 0.9900; }  
          }
        } else if (mc_ == mc::fall11_42X) {    
          if (pt > 20.0 && pt <= 30.0 && sc_eta < 1.479)  { ele_id = 0.9590; ele_iso = 0.9907; }
          if (pt > 20.0 && pt <= 30.0 && sc_eta >= 1.479) { ele_id = 0.9462; ele_iso = 0.9875; }
          if (pt > 30.0 && sc_eta < 1.479)                { ele_id = 0.9826; ele_iso = 0.9845; }
          if (pt > 30.0 && sc_eta >= 1.479)               { ele_id = 0.9689; ele_iso = 0.9971; }
        } else if (mc_ == mc::spring15_74X){
          if(pt<100){
            ele_idiso_data = et_idiso_data_->GetBinContent(et_idiso_data_->GetXaxis()->FindBin(sc_eta),et_idiso_data_->GetYaxis()->FindBin(pt));
            ele_idiso_mc = et_idiso_mc_->GetBinContent(et_idiso_mc_->GetXaxis()->FindBin(sc_eta),et_idiso_mc_->GetYaxis()->FindBin(pt));
          } else {
            ele_idiso_data = et_idiso_data_->GetBinContent(et_idiso_data_->GetXaxis()->FindBin(sc_eta),(et_idiso_data_->GetYaxis()->FindBin(pt)-1));
            ele_idiso_mc = et_idiso_mc_->GetBinContent(et_idiso_mc_->GetXaxis()->FindBin(sc_eta),(et_idiso_mc_->GetYaxis()->FindBin(pt)-1));
          }         
            ele_idiso = ele_idiso_data/ele_idiso_mc;
        }
        if(mc_ != mc::spring15_74X){
          if (do_id_weights_) ele_iso = 1.0;
          weight *= (ele_id * ele_iso);
          event->Add("idweight_1", ele_id);
          event->Add("idweight_2", double(1.0));
          event->Add("isoweight_1", ele_iso);
          event->Add("isoweight_2", double(1.0));
        } else {
          weight *= ele_idiso;
          event->Add("idisoweight_1",ele_idiso);
          event->Add("idisoweight_2",double(1.0));
        }
      } else if (channel_ == channel::mt) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        double pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double mu_id = 1.0;
        double mu_iso = 1.0;
        double mu_idiso_mc = 1.0;
        double mu_idiso_data = 1.0;
        double mu_idiso=1.0;
        if (mc_ == mc::summer12_53X) {
          if (era_ == era::data_2012_rereco) {
            if (pt > 20.0 && pt <= 30.0 && m_eta < 0.8)                   { mu_id = 0.9818; mu_iso = 0.9494; }
            if (pt > 20.0 && pt <= 30.0 && m_eta >= 0.8 && m_eta < 1.2)   { mu_id = 0.9829; mu_iso = 0.9835; }
            if (pt > 20.0 && pt <= 30.0 && m_eta >= 1.2)                  { mu_id = 0.9869; mu_iso = 0.9923; }
            if (pt > 30.0 && m_eta < 0.8)                                 { mu_id = 0.9852; mu_iso = 0.9883; }
            if (pt > 30.0 && m_eta >= 0.8 && m_eta < 1.2)                 { mu_id = 0.9850; mu_iso = 0.9937; }
            if (pt > 30.0 && m_eta >= 1.2)                                { mu_id = 0.9884; mu_iso = 0.9996; }
          }
        } else if (mc_ == mc::fall11_42X) {
          if (pt > 17.0 && pt <= 20.0 && m_eta < 0.8)                   { mu_id = 0.9963; mu_iso = 0.9910; }
          if (pt > 17.0 && pt <= 20.0 && m_eta >= 0.8 && m_eta < 1.2)   { mu_id = 0.9846; mu_iso = 0.9643; }
          if (pt > 17.0 && pt <= 20.0 && m_eta >= 1.2)                  { mu_id = 0.9830; mu_iso = 0.9504; }
          if (pt > 20.0 && pt <= 30.0 && m_eta < 0.8)                   { mu_id = 0.9962; mu_iso = 1.0011; }
          if (pt > 20.0 && pt <= 30.0 && m_eta >= 0.8 && m_eta < 1.2)   { mu_id = 0.9904; mu_iso = 0.9834; }
          if (pt > 20.0 && pt <= 30.0 && m_eta >= 1.2)                  { mu_id = 0.9828; mu_iso = 0.9975; }
          if (pt > 30.0 && m_eta < 0.8)                                 { mu_id = 0.9977; mu_iso = 0.9895; }
          if (pt > 30.0 && m_eta >= 0.8 && m_eta < 1.2)                 { mu_id = 0.9893; mu_iso = 0.9936; }
          if (pt > 30.0 && m_eta >= 1.2)                                { mu_id = 0.9829; mu_iso = 0.9960; }
        } else if (mc_ == mc::spring15_74X){
          if(pt<100){
            mu_idiso_data = mt_idiso_data_->GetBinContent(mt_idiso_data_->GetXaxis()->FindBin(m_eta),mt_idiso_data_->GetYaxis()->FindBin(pt));
            mu_idiso_mc = mt_idiso_mc_->GetBinContent(mt_idiso_mc_->GetXaxis()->FindBin(m_eta),mt_idiso_mc_->GetYaxis()->FindBin(pt));
          } else {
            mu_idiso_data = mt_idiso_data_->GetBinContent(mt_idiso_data_->GetXaxis()->FindBin(m_eta),(mt_idiso_data_->GetYaxis()->FindBin(pt)-1));
            mu_idiso_mc = mt_idiso_mc_->GetBinContent(mt_idiso_mc_->GetXaxis()->FindBin(m_eta),(mt_idiso_mc_->GetYaxis()->FindBin(pt)-1));
          }         
            mu_idiso = mu_idiso_data/mu_idiso_mc;
        }
        if(mc_ != mc::spring15_74X){ 
          if (do_id_weights_) mu_iso = 1.0;
          weight *= (mu_id * mu_iso);
          event->Add("idweight_1", mu_id);
          event->Add("idweight_2", double(1.0));
          event->Add("isoweight_1", mu_iso);
          event->Add("isoweight_2", double(1.0));
        } else {
          weight *= mu_idiso;
          event->Add("idisoweight_1", mu_idiso);
          event->Add("idisoweight_2", double(1.0));
        }
      } else if (channel_ == channel::em) {
        Electron const* elec = dynamic_cast<Electron const*>(dilepton[0]->GetCandidate("lepton1"));
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton2"));
        double e_pt = elec->pt();
        double e_eta = fabs(elec->sc_eta());
        double m_pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double m_idiso = 1.0;
        double e_idiso = 1.0;
        double m_idiso_mc = 1.0;
        double m_idiso_data = 1.0;
        double e_idiso_mc = 1.0;
        double e_idiso_data = 1.0;
        if (mc_ == mc::summer12_53X) {
          if (era_ == era::data_2012_rereco) {
            if (m_eta < 0.8) {
              if (m_pt <= 15.0)                 m_idiso = 0.9771;
              if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9548;
              if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9648;
              if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9676;
              if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9730;
              if (m_pt > 35.0)                  m_idiso = 0.9826;
            } else if (m_eta >= 0.8 && m_eta < 1.2) {
              if (m_pt <= 15.0)                 m_idiso = 0.9746;
              if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9701;
              if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9836;
              if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9817;
              if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9833;
              if (m_pt > 35.0)                  m_idiso = 0.9841;
            } else if (m_eta >= 1.2 && m_eta < 1.6) {
              if (m_pt <= 15.0)                 m_idiso = 0.9644;
              if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9766;
              if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9820;
              if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9886;
              if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9910;
              if (m_pt > 35.0)                  m_idiso = 0.9900;
            } else {
              if (m_pt <= 15.0)                 m_idiso = 0.9891;
              if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 0.9892;
              if (m_pt > 20.0 && m_pt <= 25.0)  m_idiso = 0.9909;
              if (m_pt > 25.0 && m_pt <= 30.0)  m_idiso = 0.9883;
              if (m_pt > 30.0 && m_pt <= 35.0)  m_idiso = 0.9900;
              if (m_pt > 35.0)                  m_idiso = 0.9886;
            }
            if (e_eta < 0.8) {
              if (e_pt <= 15.0)                 e_idiso = 0.7654;
              if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.8394;
              if (e_pt > 20.0 && e_pt <= 25.0)  e_idiso = 0.8772;
              if (e_pt > 25.0 && e_pt <= 30.0)  e_idiso = 0.9006;
              if (e_pt > 30.0 && e_pt <= 35.0)  e_idiso = 0.9261;
              if (e_pt > 35.0)                  e_idiso = 0.9514;
            } else if (e_eta >= 0.8 && e_eta < 1.479) {
              if (e_pt <= 15.0)                 e_idiso = 0.7693;
              if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.8457;
              if (e_pt > 20.0 && e_pt <= 25.0)  e_idiso = 0.8530;
              if (e_pt > 25.0 && e_pt <= 30.0)  e_idiso = 0.8874;
              if (e_pt > 30.0 && e_pt <= 35.0)  e_idiso = 0.9199;
              if (e_pt > 35.0)                  e_idiso = 0.9445;
            } else {
              if (e_pt <= 15.0)                 e_idiso = 0.5719;
              if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.7024;
              if (e_pt > 20.0 && e_pt <= 25.0)  e_idiso = 0.7631;
              if (e_pt > 25.0 && e_pt <= 30.0)  e_idiso = 0.8092;
              if (e_pt > 30.0 && e_pt <= 35.0)  e_idiso = 0.8469;
              if (e_pt > 35.0)                  e_idiso = 0.9078;
            }
          }
        } else if (mc_ == mc::fall11_42X) {
          if (m_eta < 0.8) {
            if (m_pt <= 15.0)                 m_idiso = 0.9303;
            if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 1.0176;
            if (m_pt > 20.0)                  m_idiso = 0.9998;
          } else if (m_eta >= 0.8 && m_eta < 1.2) {
            if (m_pt <= 15.0)                 m_idiso = 1.0125;
            if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 1.0040;
            if (m_pt > 20.0)                  m_idiso = 1.0006;
          } else {
            if (m_pt <= 15.0)                 m_idiso = 0.9994;
            if (m_pt > 15.0 && m_pt <= 20.0)  m_idiso = 1.0063;
            if (m_pt > 20.0)                  m_idiso = 1.0045;
          }
          if (e_eta < 0.8) {
            if (e_pt <= 15.0)                 e_idiso = 1.0078;
            if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.9612;
            if (e_pt > 20.0)                  e_idiso = 0.9862;
          } else if (e_eta >= 0.8 && e_eta < 1.479) {
            if (e_pt <= 15.0)                 e_idiso = 1.1236;
            if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 0.9773;
            if (e_pt > 20.0)                  e_idiso = 0.9786;
          } else {
            if (e_pt <= 15.0)                 e_idiso = 0.9336;
            if (e_pt > 15.0 && e_pt <= 20.0)  e_idiso = 1.0600;
            if (e_pt > 20.0)                  e_idiso = 1.0136;  
          }
        } else if (mc_ == mc::spring15_74X){
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
        }
         
        // if (do_id_weights_) mu_iso = 1.0;
        weight *= (e_idiso * m_idiso);
        event->Add("idweight_1", e_idiso);
        event->Add("idweight_2", m_idiso);
        event->Add("isoweight_1", double(1.0));
        event->Add("isoweight_2", double(1.0));
      } else if (channel_ == channel::mtmet) {
        Muon const* muon = dynamic_cast<Muon const*>(dilepton[0]->GetCandidate("lepton1"));
        double pt = muon->pt();
        double m_eta = fabs(muon->eta());
        double mu_id = 1.0;
        double mu_iso = 1.0;
        if (pt > 8.0 && pt <= 15.0 && m_eta < 0.8)                    { mu_id = 0.9790; mu_iso = 0.9963; }
        if (pt > 8.0 && pt <= 15.0 && m_eta >= 0.8 && m_eta < 1.2)    { mu_id = 0.9809; mu_iso = 0.9769; }
        if (pt > 8.0 && pt <= 15.0 && m_eta >= 1.2)                   { mu_id = 0.9967; mu_iso = 0.9870; }
        if (pt > 15.0 && m_eta < 0.8)                                 { mu_id = 0.9746; mu_iso = 0.9842; }
        if (pt > 15.0 && m_eta >= 0.8 && m_eta < 1.2)                 { mu_id = 0.9796; mu_iso = 0.9664; }
        if (pt > 15.0 && m_eta >= 1.2)                                { mu_id = 0.9864; mu_iso = 0.9795; }
        if (do_id_weights_) mu_iso = 1.0;
        weight *= (mu_id * mu_iso);
        event->Add("idweight_1", mu_id);
        event->Add("idweight_2", double(1.0));
        event->Add("isoweight_1", mu_iso);
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
      if (era_ == era::data_2012_rereco) eleEta = fabs(eleEta);
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
      if (era_ == era::data_2012_rereco) muEta = fabs(muEta);
      int muptbin = MuonFakeRateHist_PtEta->GetXaxis()->FindFixBin(mufopt);
      int muetabin = MuonFakeRateHist_PtEta->GetYaxis()->FindFixBin(muEta);    
      double muprob = MuonFakeRateHist_PtEta->GetBinContent(muptbin,muetabin);
      double mufakerate = muprob/(1.0 - muprob);
      //double mufakerate_errlow = MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrLow)/pow((1-MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrLow)),2);
      //double mufakerate_errhigh = MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrHigh)/pow((1-MuonFakeRateHist_PtEta->GetError(mufopt,fabs(muon->eta()),mithep::TH2DAsymErr::kStatErrHigh)),2);
      eventInfo->set_weight("emu_m_fakerate", mufakerate);
    }

    if (do_etau_fakerate_) {
      std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
      ic::erase_if(parts, !(boost::bind(&GenParticle::status, _1) == 3));
      ic::erase_if(parts, ! ((boost::bind(&GenParticle::pdgid, _1) == 11)||(boost::bind(&GenParticle::pdgid, _1) == -11)) );
      ic::erase_if(parts, ! (boost::bind(MinPtMaxEta, _1, 8.0, 2.6)));
      std::vector<Candidate *> tau_cand;
      tau_cand.push_back(dilepton[0]->GetCandidate("lepton2"));
      Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau_cand, parts, 0.5, true, true);
      // Mode 0 Barrel: 0.87 +/- 0.04
      // Mode 0 Endcap: 0.40 +/- 0.20
      // Mode 1 Barrel: 1.19 +/- 0.06
      // Mode 1 Endcap: 0.72 +/ 0.17
      if (matches.size() > 0) {
        if (mc_ == mc::fall11_42X) {
          if (fabs(tau_cand[0]->eta()) < 1.5) {
            if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 1.142);
            if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 1.617);
          } else {
            if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 0.859);
            if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 0.610);
          }
        } else {
          if (era_ == era::data_2012_rereco) {
            if (fabs(tau_cand[0]->eta()) < 1.5) {
              if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 1.37);
              if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 2.18);
            } else {
              if (tau->decay_mode() == 0) eventInfo->set_weight("etau_fakerate", 1.11);
              if (tau->decay_mode() == 1) eventInfo->set_weight("etau_fakerate", 0.47);
            }
          }
        }
      }
    }

    if (do_mtau_fakerate_) {
      std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
      ic::erase_if(parts, !(boost::bind(&GenParticle::status, _1) == 3));
      ic::erase_if(parts, ! ((boost::bind(&GenParticle::pdgid, _1) == 13)||(boost::bind(&GenParticle::pdgid, _1) == -13)) );
      ic::erase_if(parts, ! (boost::bind(MinPtMaxEta, _1, 8.0, 2.6)));
      std::vector<Candidate *> tau_cand;
      tau_cand.push_back(dilepton[0]->GetCandidate("lepton2"));
      std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau_cand, parts, 0.5, true, true);
      //We didnt use this for any of 2011 or 2012 in the end, just leaving the code here with a weight of 1 for now.
      if (matches.size() > 0) {
       eventInfo->set_weight("mtau_fakerate", 1.00);
      }
    }

    if (do_tau_mode_scale_) {
      Tau const* tau = dynamic_cast<Tau const*>(dilepton[0]->GetCandidate("lepton2"));
      if (tau->decay_mode() == 0 && era_ == era::data_2012_rereco) {
        eventInfo->set_weight("tau_mode_scale", 0.88);
      }
    }

    if (do_w_soup_) {
      std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      bool count_jets = false;
      unsigned partons = 0;
      for (unsigned i = 0; i < parts.size(); ++i) {
        if (parts[i]->status() != 3) continue;
        unsigned id = abs(parts[i]->pdgid());
        if (count_jets) { 
          if (id == 1 || id == 2 || id == 3 || id == 4 || id == 5 || id == 6 || id == 21) partons++;
        }
        if (id == 24) count_jets = true; 
      }
      if (partons > 4) {
        std::cerr << "Error making soup, event has " << partons << " partons!" << std::endl;
        throw;
      }
      if (partons == 1) eventInfo->set_weight("wsoup", w1_);
      if (partons == 2) eventInfo->set_weight("wsoup", w2_);
      if (partons == 3) eventInfo->set_weight("wsoup", w3_);
      if (partons == 4) eventInfo->set_weight("wsoup", w4_);
    }

    if (do_dy_soup_) {
      std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      bool count_jets = false;
      unsigned partons = 0;
      for (unsigned i = 0; i < parts.size(); ++i) {
        // std::cout << i << "\t" << parts[i]->status() << "\t" << parts[i]->pdgid() << "\t" << parts[i]->vector() << std::endl;
        if (parts[i]->status() != 3) continue;
        unsigned id = abs(parts[i]->pdgid());
        if (count_jets) { 
          if (id == 1 || id == 2 || id == 3 || id == 4 || id == 5 || id == 6 || id == 21) partons++;
        }
        if (id == 23) count_jets = true; 
      }
      if (partons > 4) {
        std::cerr << "Error making soup, event has " << partons << " partons!" << std::endl;
        throw;
      }
      if (partons == 1) eventInfo->set_weight("dysoup", zw1_);
      if (partons == 2) eventInfo->set_weight("dysoup", zw2_);
      if (partons == 3) eventInfo->set_weight("dysoup", zw3_);
      if (partons == 4) eventInfo->set_weight("dysoup", zw4_);
    }

   if (do_w_soup_htbinned_){
     double gen_ht = eventInfo->gen_ht() ;
     if (100 <= gen_ht&&gen_ht <200) eventInfo->set_weight("wsoup", w1_);
     if (200 <= gen_ht&&gen_ht <400) eventInfo->set_weight("wsoup", w2_);
     if (400 <= gen_ht &&gen_ht<600) eventInfo->set_weight("wsoup", w3_);
     if (gen_ht >= 600) eventInfo->set_weight("wsoup", w4_);
   }


   if (do_dy_soup_htbinned_){
     double gen_ht = eventInfo->gen_ht() ;
     if (100 <= gen_ht&&gen_ht <200) eventInfo->set_weight("dysoup", zw1_);
     if (200 <= gen_ht&&gen_ht <400) eventInfo->set_weight("dysoup", zw2_);
     if (400 <= gen_ht &&gen_ht<600) eventInfo->set_weight("dysoup", zw3_);
     if (gen_ht >= 600) eventInfo->set_weight("dysoup", zw4_);
   }

    return 0;
  }

  int HTTWeights::PostAnalysis() {
    return 0;
  }

  void HTTWeights::PrintInfo() {
    ;
  }

  void HTTWeights::SetWTargetFractions(double f0, double f1, double f2, double f3, double f4) {
    f0_ = f0;
    f1_ = f1;
    f2_ = f2;
    f3_ = f3;
    f4_ = f4;

  }
  void HTTWeights::SetWInputYields(double n_inc, double n1, double n2, double n3, double n4) {
    n_inc_ = n_inc;
    n1_ = n1;
    n2_ = n2;
    n3_ = n3;
    n4_ = n4;
  }

  void HTTWeights::SetDYTargetFractions(double zf0, double zf1, double zf2, double zf3, double zf4) {
    zf0_ = zf0;
    zf1_ = zf1;
    zf2_ = zf2;
    zf3_ = zf3;
    zf4_ = zf4;

  }
  void HTTWeights::SetDYInputYields(double zn_inc, double zn1, double zn2, double zn3, double zn4) {
    zn_inc_ = zn_inc;
    zn1_ = zn1;
    zn2_ = zn2;
    zn3_ = zn3;
    zn4_ = zn4;
  }

  void HTTWeights::SetDYInputCrossSections(double zxs0, double zxs1, double zxs2, double zxs3, double zxs4) {
    zxs0_ = zxs0;
    zxs1_ = zxs1;
    zxs2_ = zxs2;
    zxs3_ = zxs3;
    zxs4_ = zxs4;
  }

  void HTTWeights::SetWInputCrossSections(double wxs0, double wxs1, double wxs2, double wxs3, double wxs4) {
    wxs0_ = wxs0;
    wxs1_ = wxs1;
    wxs2_ = wxs2;
    wxs3_ = wxs3;
    wxs4_ = wxs4;
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
