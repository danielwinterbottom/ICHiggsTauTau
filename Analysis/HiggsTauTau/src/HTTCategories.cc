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
      met_label_ = "pfMVAMet";
      mass_shift_ = 1.0;
      fs_ = NULL;
      write_tree_ = true;
      write_plots_ = false;
      experimental_ = false;
      bjet_regression_ = false;
      kinfit_mode_ = 0; //0 = don't run, 1 = run simple 125,125 default fit, 2 = run extra masses default fit, 3 = run m_bb only fit
  }

  HTTCategories::~HTTCategories() {
    ;
  }

  void HTTCategories::InitSelection(std::string const& selection) {
    selections_[selection] = false;
  }
  
  void HTTCategories::InitCategory(std::string const& category) {
    categories_[category] = false;
    InitMassPlots(category);
  }

  int HTTCategories::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTCategories" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    if (fs_) {
      std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
      std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
      std::cout << boost::format(param_fmt()) % "era"             % Era2String(era_);
      std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
      std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
      std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;
      std::cout << boost::format(param_fmt()) % "mass_shift"      % mass_shift_;
      std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;
      std::cout << boost::format(param_fmt()) % "write_plots"     % write_plots_;
      std::cout << boost::format(param_fmt()) % "experimental"    % experimental_;
      std::cout << boost::format(param_fmt()) % "kinfit_mode"     % kinfit_mode_;
      std::cout << boost::format(param_fmt()) % "bjet_regression" % bjet_regression_;

      if (write_tree_) {
        outtree_ = fs_->make<TTree>("ntuple","ntuple");
        outtree_->Branch("wt",                &wt_);
        outtree_->Branch("wt_ggh_pt_up",      &wt_ggh_pt_up_);
        outtree_->Branch("wt_ggh_pt_down",    &wt_ggh_pt_down_);
        outtree_->Branch("wt_tau_fake_up",    &wt_tau_fake_up_);
        outtree_->Branch("wt_tau_fake_down",  &wt_tau_fake_down_);
        outtree_->Branch("wt_tquark_up",      &wt_tquark_up_);
        outtree_->Branch("wt_tquark_down",    &wt_tquark_down_);
        outtree_->Branch("wt_tau_id_up",      &wt_tau_id_up_);
        outtree_->Branch("wt_tau_id_down",    &wt_tau_id_down_);
        outtree_->Branch("os",                &os_);
        outtree_->Branch("n_vtx",             &n_vtx_);
        outtree_->Branch("m_sv",              &m_sv_);
        outtree_->Branch("m_vis",             &m_vis_);
        outtree_->Branch("pt_h",              &pt_h_);
        outtree_->Branch("pt_tt",             &pt_tt_);
        outtree_->Branch("mt_1",              &mt_1_);
        outtree_->Branch("pzeta",             &pzeta_);
        outtree_->Branch("pt_1",              &pt_1_);
        outtree_->Branch("pt_2",              &pt_2_);
        outtree_->Branch("eta_1",             &eta_1_);
        outtree_->Branch("eta_2",             &eta_2_);
        outtree_->Branch("iso_2",             &iso_2_);
        outtree_->Branch("z_2",               &z_2_);
        outtree_->Branch("m_2",               &m_2_);
        outtree_->Branch("met",               &met_);
        outtree_->Branch("met_phi",           &met_phi_);
        outtree_->Branch("tau_decay_mode",    &tau_decay_mode_);
        outtree_->Branch("n_jets",            &n_jets_);
        outtree_->Branch("n_lowpt_jets",      &n_lowpt_jets_);
        outtree_->Branch("n_bjets",           &n_bjets_);
        outtree_->Branch("n_prebjets",        &n_prebjets_);
        outtree_->Branch("n_jets_csv",        &n_jets_csv_);
        outtree_->Branch("n_bjets_csv",       &n_bjets_csv_);
        outtree_->Branch("n_loose_bjets",     &n_loose_bjets_);
        outtree_->Branch("n_jetsingap",       &n_jetsingap_);
        outtree_->Branch("jpt_1",             &jpt_1_);
        outtree_->Branch("j1_dm",             &j1_dm_);
        outtree_->Branch("jpt_2",             &jpt_2_);
        outtree_->Branch("jeta_1",            &jeta_1_);
        outtree_->Branch("jeta_2",            &jeta_2_);
        outtree_->Branch("bpt_1",             &bpt_1_);
        outtree_->Branch("beta_1",            &beta_1_);
        outtree_->Branch("bcsv_1",            &bcsv_1_);
        outtree_->Branch("jet_csvpt_1",       &jet_csvpt_1_);
        outtree_->Branch("jet_csvEt_1",       &jet_csvEt_1_);
        outtree_->Branch("jet_csveta_1",      &jet_csveta_1_);
        outtree_->Branch("jet_csvbcsv_1",     &jet_csvbcsv_1_);
        outtree_->Branch("jet_csvpt_2",       &jet_csvpt_2_);
        outtree_->Branch("jet_csvpt_bb",	  &jet_csvpt_bb_);
        outtree_->Branch("jet_csv_dR",		  &jet_csv_dR_);
        outtree_->Branch("jet_csveta_2",      &jet_csveta_2_);
        outtree_->Branch("jet_csvbcsv_2",     &jet_csvbcsv_2_);
        outtree_->Branch("mjj",               &mjj_);
        outtree_->Branch("mjj_h",             &mjj_h_);
        outtree_->Branch("mbb_h",             &mbb_h_);
        outtree_->Branch("mjj_tt",            &mjj_tt_);
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
        outtree_->Branch("m_H_hh",     &m_H_hh_);
        outtree_->Branch("m_H_hh_all",     &m_H_hh_all_);
        outtree_->Branch("m_H_hh_chi2",     &m_H_hh_chi2_);
        outtree_->Branch("pull_balance_hh", &pull_balance_hh_);
        outtree_->Branch("convergence_hh", &convergence_hh_);
        outtree_->Branch("m_bb",     &m_bb_);
        outtree_->Branch("m_bb_chi2",     &m_bb_chi2_);
        outtree_->Branch("pull_balance_bb", &pull_balance_bb_);
        outtree_->Branch("convergence_bb", &convergence_bb_);
        outtree_->Branch("jdeta",             &jdeta_);
        outtree_->Branch("jet_csv_mjj",               &jet_csv_mjj_);
        outtree_->Branch("jet_csv_dphi",               &jet_csv_dphi_);
        outtree_->Branch("jet_csv_deta",             &jet_csv_deta_);
        outtree_->Branch("jet_csv_dtheta",             &jet_csv_dtheta_);
        outtree_->Branch("mjj_lowpt",         &mjj_lowpt_);
        outtree_->Branch("jdeta_lowpt",       &jdeta_lowpt_);
        outtree_->Branch("n_jetsingap_lowpt", &n_jetsingap_lowpt_);
        outtree_->Branch("l1_met",            &l1_met_);
        outtree_->Branch("calo_nohf_met",     &calo_nohf_met_);
        if (channel_ == channel::em) {
          outtree_->Branch("em_gf_mva",         &em_gf_mva_);
          // outtree_->Branch("em_vbf_mva",        &em_vbf_mva_);
          outtree_->Branch("pzetavis",          &pzetavis_);
          outtree_->Branch("pzetamiss",         &pzetamiss_);
          outtree_->Branch("mt_ll",             &mt_ll_);
          outtree_->Branch("emu_dphi",          &emu_dphi_);
          outtree_->Branch("emu_csv",           &emu_csv_);
          outtree_->Branch("emu_dxy_1",         &emu_dxy_1_);
          outtree_->Branch("emu_dxy_2",         &emu_dxy_2_);
        }
      }
    }
    TH1F::SetDefaultSumw2();
    misc_plots_ = new DynamicHistoSet(fs_->mkdir("misc_plots"));
    misc_2dplots_ = new Dynamic2DHistoSet(fs_->mkdir("misc_2dplots"));
    misc_2dplots_->Create("jpt_vs_hpt", 50, 0, 200, 50, 0, 200);
    misc_2dplots_->Create("hpt_vs_pt_tt", 50, 0, 200, 50, 0, 200);
    misc_2dplots_->Create("gen_vs_hpt", 50, 0, 200, 50, 0, 200);
    misc_2dplots_->Create("gen_vs_pt_tt", 50, 0, 200, 50, 0, 200);
    misc_2dplots_->Create("m_gen_m_sv_nobtag", 75, 0, 1500, 75, 0, 1500);
    misc_2dplots_->Create("m_gen_m_sv_btag", 75, 0, 1500, 75, 0, 1500);
    misc_2dplots_->Create("m_gen_m_vis", 75, 0, 1500, 75, 0, 1500);
    misc_2dplots_->Create("orig_met_m_sv", 75, 0, 750, 75, 0, 1500);
    misc_2dplots_->Create("orig_met_m_vis", 75, 0, 750, 75, 0, 1500);
    misc_plots_->Create("M",150,0,1500);

    if (write_plots_) {
      InitSelection("os");
      InitSelection("os_sel");
      InitSelection("os_con");
      InitSelection("os_con_mt_60-120");
      InitSelection("ss");
      InitSelection("ss_sel");
      InitSelection("ss_con");
      InitSelection("ss_con_mt_60-120");

      InitCategory("inclusive");
      InitCoreControlPlots("inclusive");

      InitCategory("vbf");
      InitCategory("vbf_no_cjv");
      InitCategory("vbf_loose");
      InitCategory("vbf_loose_jets20");

      InitCategory("twojet");
      InitCoreControlPlots("twojet");

      InitCategory("1jet");
      InitCoreControlPlots("1jet");

      InitCategory("1jet_high");
      InitCategory("1jet_low");
      InitCategory("1jet_low_nometcut");
      InitCategory("0jet_high");
      InitCategory("0jet_low");

      if (experimental_) {
        InitCategory("jpt_30_1jet_high");
        InitCategory("jpt_30_1jet_low");
        InitCategory("jpt_30_0jet_high");
        InitCategory("jpt_30_0jet_low");
        InitCategory("jpt_40_1jet_high");
        InitCategory("jpt_40_1jet_low");
        InitCategory("jpt_40_0jet_high");
        InitCategory("jpt_40_0jet_low");
        InitCategory("jpt_60_1jet_high");
        InitCategory("jpt_60_1jet_low");
        InitCategory("jpt_60_0jet_high");
        InitCategory("jpt_60_0jet_low");
        InitCategory("jpt_80_1jet_high");
        InitCategory("jpt_80_1jet_low");
        InitCategory("jpt_80_0jet_high");
        InitCategory("jpt_80_0jet_low");

        InitCategory("hpt_30_1jet_high");
        InitCategory("hpt_30_1jet_low");
        InitCategory("hpt_30_0jet_high");
        InitCategory("hpt_30_0jet_low");
        InitCategory("hpt_40_1jet_high");
        InitCategory("hpt_40_1jet_low");
        InitCategory("hpt_40_0jet_high");
        InitCategory("hpt_40_0jet_low");
        InitCategory("hpt_60_1jet_high");
        InitCategory("hpt_60_1jet_low");
        InitCategory("hpt_60_0jet_high");
        InitCategory("hpt_60_0jet_low");
        InitCategory("hpt_80_1jet_high");
        InitCategory("hpt_80_1jet_low");
        InitCategory("hpt_80_0jet_high");
        InitCategory("hpt_80_0jet_low");
      }
      InitCategory("prebtag");
      InitCoreControlPlots("prebtag");

      InitCategory("btag");
      InitCoreControlPlots("btag");

      InitCategory("btag_low");
      InitCategory("btag_low_loose");

      InitCategory("btag_high");
      InitCategory("btag_high_loose");

      InitCategory("btag_loose");

      InitCategory("nobtag");
    }
    return 0;
  }

  int HTTCategories::Execute(TreeEvent *event) {

    Reset();

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

    wt_ = eventInfo->total_weight();
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
    
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    Met const* met = event->GetPtr<Met>(met_label_);
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
    } else {
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

    n_vtx_ = eventInfo->good_vertices();

    if (event->Exists("svfitMass")) {
      m_sv_ = event->Get<double>("svfitMass");
    } else {
      m_sv_ = -9999;
    }

    if (event->Exists("svfitHiggs")) {
      pt_h_ = event->Get<Candidate>("svfitHiggs").pt();
    } else {
      pt_h_ = -9999;
    }

    pt_tt_ = (ditau->vector() + met->vector()).pt();
    m_vis_ = ditau->M();
   
    GenParticle const* boson = NULL;
    if (experimental_ && event->Exists("genParticles")) {
      std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
      for (unsigned i = 0; i < parts.size(); ++i) {
        unsigned id = abs(parts[i]->pdgid());
        if (parts[i]->status() == 3) {
          if (id == 25 || id == 35 || id == 36) {
            boson = parts[i];
          }
        }
      }
    }

    // This is the HCP hack for the em channel
    // to better align the data with the embedded
    // mass.  
    if (channel_ == channel::em) {
      m_sv_ = m_sv_ * mass_shift_;
      m_vis_ = m_vis_ * mass_shift_;
      em_gf_mva_ = event->Exists("em_gf_mva") ? event->Get<double>("em_gf_mva") : 0.;
      // em_vbf_mva_ = event->Exists("em_vbf_mva") ? event->Get<double>("em_vbf_mva") : 0.;
    }
    if (event->Exists("mass_scale")) {
      m_sv_ = m_sv_ * event->Get<double>("mass_scale");
      m_vis_ = m_vis_ * event->Get<double>("mass_scale");
    }



    mt_1_ = MT(lep1, met);
    mt_ll_ = MT(ditau, met);
    pzeta_ = PZeta(ditau, met, 0.85);
    pzetavis_ = PZetaVis(ditau);
    pzetamiss_ = PZeta(ditau, met, 0.0);
    emu_dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(), lep2->vector()));

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();

    m_2_ = lep2->M();
    met_ = met->pt();
    met_phi_ = met->phi();

    emu_dxy_1_ = 0.0;
    emu_dxy_2_ = 0.0;

    if (strategy_ == strategy::paper2013 || strategy_ == strategy::phys14) {
      if (channel_ == channel::et) {
        Electron const* elec = dynamic_cast<Electron const*>(lep1);
        iso_1_ = PF04IsolationVal(elec, 0.5);
        Tau const* tau = dynamic_cast<Tau const*>(lep2);
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
      }
      if (channel_ == channel::mt || channel_ == channel::mtmet) {
        Muon const* muon = dynamic_cast<Muon const*>(lep1);
        iso_1_ = PF04IsolationVal(muon, 0.5);
        Tau const* tau = dynamic_cast<Tau const*>(lep2);
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
      }
      if (channel_ == channel::em) {
        Electron const* elec = dynamic_cast<Electron const*>(lep1);
        iso_1_ = PF04IsolationVal(elec, 0.5);
        Muon const* muon = dynamic_cast<Muon const*>(lep2);
        iso_2_ = PF04IsolationVal(muon, 0.5);
        emu_dxy_1_ = -1. * elec->dxy_vertex();
        emu_dxy_2_ = -1. * muon->dxy_vertex();
      }
    } else {
      iso_1_ = 0.0;
      iso_2_ = 0.0;
    }

    l1_met_ = 0.0;
    calo_nohf_met_ = 0.0;

    Tau const* tau = dynamic_cast<Tau const*>(lep2);
    if (tau) {
      tau_decay_mode_ = tau->decay_mode();
      z_2_ = tau->vz() + (130. / tan(tau->vector().theta()));
    } else {
      tau_decay_mode_ = 0;
      z_2_ = 0.;
    }

    n_jets_ = jets.size();
    n_lowpt_jets_ = lowpt_jets.size();
    n_bjets_ = bjets.size();
    n_jets_csv_ = jets_csv.size();
    n_loose_bjets_ = loose_bjets.size();

    if (n_jets_ >= 1) {
      jpt_1_ = jets[0]->pt();
      jeta_1_ = jets[0]->eta();
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
    }

    if (n_jets_ >= 2) {
      jpt_2_ = jets[1]->pt();
      jeta_2_ = jets[1]->eta();
      mjj_ = (jets[0]->vector() + jets[1]->vector()).M();
      jdeta_ = fabs(jets[0]->eta() - jets[1]->eta());
      double eta_high = (jets[0]->eta() > jets[1]->eta()) ? jets[0]->eta() : jets[1]->eta();
      double eta_low = (jets[0]->eta() > jets[1]->eta()) ? jets[1]->eta() : jets[0]->eta();
      n_jetsingap_ = 0;
      if (n_jets_ > 2) {
        for (unsigned i = 2; i < jets.size(); ++i) {
         if (jets[i]->pt() > 30.0 &&  jets[i]->eta() > eta_low && jets[i]->eta() < eta_high) ++n_jetsingap_;
        }
      }
    } else {
      jpt_2_ = -9999;
      jeta_2_ = -9999;
      mjj_ = -9999;
      jdeta_ = -9999;
      n_jetsingap_ = 9999;
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
      beta_1_ = bjets[0]->eta();
    } else {
      bpt_1_ = -9999;
      beta_1_ = -9999;
    }

    if (jets_csv.size() >= 1) {
      bcsv_1_ = jets_csv[0]->GetBDiscriminator(btag_label);
    } else {
      bcsv_1_ = -9999;
    }
    emu_csv_ = (bcsv_1_ > 0.244) ? bcsv_1_ : -1.0;


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
      mjj_tt_= (jets_csv[0]->vector() + jets_csv[1]->vector() + ditau->vector() + met->vector()).M();
      if(bjet_regression_) mjj_tt_= (jet_csv_pairs[0].second->vector() + jet_csv_pairs[1].second->vector() + ditau->vector() + met->vector()).M();
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
        TLorentzVector ptmiss  = TLorentzVector(met->vector().px(),met->vector().py(),0,met->vector().pt());
        TLorentzVector higgs;
        if (event->Exists("svfitHiggs")) {
          higgs = TLorentzVector(event->Get<Candidate>("svfitHiggs").vector().px(),event->Get<Candidate>("svfitHiggs").vector().py(),event->Get<Candidate>("svfitHiggs").vector().pz(),event->Get<Candidate>("svfitHiggs").vector().E());
        }
        TMatrixD metcov(2,2);
        metcov(0,0)=met->xx_sig();
        metcov(1,0)=met->yx_sig();
        metcov(0,1)=met->xy_sig();
        metcov(1,1)=met->yy_sig();
            
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
    


    if ((event->Exists("genParticlesEmbedded") || event->Exists("genParticles")) && os_ && mt_1_ <30.) {
      // recalculate met
      ROOT::Math::PtEtaPhiEVector met_vec = met->vector();
      ROOT::Math::PtEtaPhiEVector ditau_vec = ditau->vector();
      ROOT::Math::PtEtaPhiEVector dimu_vec;
      std::string gen_label = event->Exists("genParticlesEmbedded") ? "genParticlesEmbedded" : "genParticles";
      std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_label);
      for (unsigned i = 0; i < particles.size(); ++i) {
        if (particles[i]->pdgid() == 23) {
          misc_plots_->Fill("M",particles[i]->vector().M(),wt_);
          if (n_jets_ <= 1 && n_bjets_ > 0) {
            misc_2dplots_->Fill("m_gen_m_sv_btag",particles[i]->vector().M(),m_sv_,wt_);
          }
          if (n_bjets_ == 0) {
            misc_2dplots_->Fill("m_gen_m_sv_nobtag",particles[i]->vector().M(),m_sv_,wt_);
          }
          misc_2dplots_->Fill("m_gen_m_vis",particles[i]->vector().M(),m_vis_,wt_);
          dimu_vec = particles[i]->vector();
          met_vec += ditau_vec;
          met_vec -= dimu_vec;
          misc_2dplots_->Fill("orig_met_m_sv",met_vec.pt(),m_sv_,wt_);
          misc_2dplots_->Fill("orig_met_m_vis",met_vec.pt(),m_vis_,wt_);
        }
      }
    }

    if (write_tree_) outtree_->Fill();
    if (!write_plots_) return 0;

    // Define which selections this event passes
    if (channel_ == channel::et || channel_ == channel::etmet || channel_ == channel::mt || channel_ == channel::mtmet) {
      if (os_ && mt_1_ < 30.0) {
        SetPassSelection("os_sel");
        if (experimental_) {
          misc_2dplots_->Fill("jpt_vs_hpt", (jpt_1_ > 0.) ? jpt_1_ : 0., pt_h_, wt_);
          misc_2dplots_->Fill("hpt_vs_pt_tt", pt_h_, pt_tt_, wt_);
          if (boson) {
            misc_2dplots_->Fill("gen_vs_hpt", boson->pt(), pt_h_, wt_);
            misc_2dplots_->Fill("gen_vs_pt_tt", boson->pt(), pt_tt_, wt_);
          }
        }
      }
      if (os_) SetPassSelection("os");
      if (!os_) SetPassSelection("ss");
      if (os_ && mt_1_ > 70.0) SetPassSelection("os_con");
      if (os_ && mt_1_ > 60.0 && mt_1_ < 120.) SetPassSelection("os_con_mt_60-120");
      if (!os_ && mt_1_ < 30.0) SetPassSelection("ss_sel");
      if (!os_ && mt_1_ > 70.0) SetPassSelection("ss_con");
      if (!os_ && mt_1_ > 60.0 && mt_1_ < 120.) SetPassSelection("ss_con_mt_60-120");
    }

    if (channel_ == channel::em) {
      if (os_ && pzeta_ > -20) SetPassSelection("os_sel");
      if (os_) SetPassSelection("os");
      if (!os_) SetPassSelection("ss");
      if (!os_ && pzeta_ > -20) SetPassSelection("ss_sel");
    }

    // Define the 1- and 0-jet split based on pt_2
    double pt2_split = 40.0; // Tau pT for et,mt and mtmet
    if (channel_ == channel::em) pt2_split = 35.0;  // Mu pT for em


    // Inclusive Category
    SetPassCategory("inclusive");
    FillCoreControlPlots("inclusive");

    // VBF Selection
    // In the em channel, additionally apply b-jet veto
    if (n_jets_ >= 2 && n_jetsingap_ == 0 && mjj_ > 500. && jdeta_ > 3.5) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) {
        SetPassCategory("vbf");
        FillCoreControlPlots("vbf");
      }
    }

    // VBF Selection
    // In the em channel, additionally apply b-jet veto
    // if (n_jets_ >= 2 && n_jetsingap_ == 0 && mjj_ > 800. && jdeta_ > 4.0) {
    //   if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) {
    //     SetPassCategory("vbf_tight");
    //   }
    // }

    // Tau-tau-like VBF Selection
    // In the em channel, additionally apply b-jet veto
    // if (n_jets_ >= 2 && n_jetsingap_ == 0 && mjj_ > 250. && jdeta_ > 2.5) {
    //   bool lepton_pt = pt_1_ > 30.0 && pt_2_ > 45.0;
    //   bool pt_50_jet = jets[0]->pt() > 50.0 && fabs(jets[0]->eta()) < 3.0;
    //   bool hpt = ((ditau->vector() + met->vector()).pt()) > 110.0;
    //   if (lepton_pt && pt_50_jet && hpt) SetPassCategory("tautau_vbf");
    // }

    // VBF Selection with no CJV, used for VBF category Fakes estimate in em
    if (n_jets_ >= 2 && mjj_ > 500. && jdeta_ > 3.5) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf_no_cjv");
    }

    // Loose VBF Selection
    // Used for background shape estimation in VBF category
    if (n_jets_ >= 2 && n_jetsingap_ == 0 && mjj_ > 200. && jdeta_ > 2.0) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf_loose");
    }
    
    // Loose VBF Selection with low pT Jet Requirement
    // Used for background shape of QCD in VBF Category
    if (n_lowpt_jets_ >= 2 && n_jetsingap_lowpt_ == 0 && mjj_lowpt_ > 200. && jdeta_lowpt_ > 2.0) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf_loose_jets20");
    }

    // Twojet Selection
    // In the em channel, additionally apply b-jet veto
    if (n_jets_ >= 2) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) {
        SetPassCategory("twojet");
        FillCoreControlPlots("twojet");
      }
    }

    // 1-jet (note no vbf veto: this is really for "inclusive" control plots)
    if (n_jets_ >= 1) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) {
        SetPassCategory("1jet");
        FillCoreControlPlots("1jet");
      }
    }

    // 1-jet High Category
    // In the et channel, apply a MET > 30 cut
    if (!PassesCategory("vbf") && n_jets_ >= 1 && pt_2_ > pt2_split && n_bjets_ == 0) {
      if ( (channel_ == channel::et || channel_ == channel::etmet) ? (met_ > 30.) : true) {
        SetPassCategory("1jet_high");
        FillCoreControlPlots("1jet_high");
      }
    }

    // 1-jet Low Category
    // In the et channel, apply a MET > 30 cut
    if (!PassesCategory("vbf") && n_jets_ >= 1 && pt_2_ <= pt2_split && n_bjets_ == 0) {
      if ( (channel_ == channel::et || channel_ == channel::etmet) ? (met_ > 30.) : true) {
        SetPassCategory("1jet_low");
        FillCoreControlPlots("1jet_low");
      }
    }
    

    if (experimental_) {
      if (!PassesCategory("vbf")  && pt_2_ > pt2_split && n_bjets_ == 0) {
        if ( (channel_ == channel::et || channel_ == channel::etmet) ? (met_ > 30.) : true) {
          if (jpt_1_ > 30.) SetPassCategory("jpt_30_1jet_high");
          if (jpt_1_ > 40.) SetPassCategory("jpt_40_1jet_high");
          if (jpt_1_ > 60.) SetPassCategory("jpt_60_1jet_high");
          if (jpt_1_ > 80.) SetPassCategory("jpt_80_1jet_high");
          if (pt_h_ > 30.) SetPassCategory("hpt_30_1jet_high");
          if (pt_h_ > 40.) SetPassCategory("hpt_40_1jet_high");
          if (pt_h_ > 60.) SetPassCategory("hpt_60_1jet_high");
          if (pt_h_ > 80.) SetPassCategory("hpt_80_1jet_high");
        }
        if (jpt_1_ <= 30.) SetPassCategory("jpt_30_0jet_high");
        if (jpt_1_ <= 40.) SetPassCategory("jpt_40_0jet_high");
        if (jpt_1_ <= 60.) SetPassCategory("jpt_60_0jet_high");
        if (jpt_1_ <= 80.) SetPassCategory("jpt_80_0jet_high");
        if (pt_h_ <= 30.) SetPassCategory("hpt_30_0jet_high");
        if (pt_h_ <= 40.) SetPassCategory("hpt_40_0jet_high");
        if (pt_h_ <= 60.) SetPassCategory("hpt_60_0jet_high");
        if (pt_h_ <= 80.) SetPassCategory("hpt_80_0jet_high");
      }
      if (!PassesCategory("vbf")  && pt_2_ <= pt2_split && n_bjets_ == 0) {
        if ( (channel_ == channel::et || channel_ == channel::etmet) ? (met_ > 30.) : true) {
          if (jpt_1_ > 30.) SetPassCategory("jpt_30_1jet_low");
          if (jpt_1_ > 40.) SetPassCategory("jpt_40_1jet_low");
          if (jpt_1_ > 60.) SetPassCategory("jpt_60_1jet_low");
          if (jpt_1_ > 80.) SetPassCategory("jpt_80_1jet_low");
          if (pt_h_ > 30.) SetPassCategory("hpt_30_1jet_low");
          if (pt_h_ > 40.) SetPassCategory("hpt_40_1jet_low");
          if (pt_h_ > 60.) SetPassCategory("hpt_60_1jet_low");
          if (pt_h_ > 80.) SetPassCategory("hpt_80_1jet_low");
        }
        if (jpt_1_ <= 30.) SetPassCategory("jpt_30_0jet_low");
        if (jpt_1_ <= 40.) SetPassCategory("jpt_40_0jet_low");
        if (jpt_1_ <= 60.) SetPassCategory("jpt_60_0jet_low");
        if (jpt_1_ <= 80.) SetPassCategory("jpt_80_0jet_low");
        if (pt_h_ <= 30.) SetPassCategory("hpt_30_0jet_low");
        if (pt_h_ <= 40.) SetPassCategory("hpt_40_0jet_low");
        if (pt_h_ <= 60.) SetPassCategory("hpt_60_0jet_low");
        if (pt_h_ <= 80.) SetPassCategory("hpt_80_0jet_low");
      }
    }
    

    // 1-jet Low Category
    // In the et channel, no met cut
    if (!PassesCategory("vbf") && n_jets_ >= 1 && pt_2_ <= pt2_split && n_bjets_ == 0) {
      SetPassCategory("1jet_low_nometcut");
    }

    // Eta veto no longer needed
    // bool em_0jet_high_muon_eta = true;
    // if (channel_ == channel::em && (era_ == era::data_2012_hcp || era_ == era::data_2012_moriond || era_ == era::data_2012_donly) ) {
    //   if (eta_2_ > -0.2 && eta_2_ < 1.0) em_0jet_high_muon_eta = false;
    // }
    // 0-jet High Category
    if (n_jets_ == 0 && pt_2_ > pt2_split && n_bjets_ == 0) {
      SetPassCategory("0jet_high");
      FillCoreControlPlots("0jet_high");
    }

    // 0-jet Low Category
    if (n_jets_ == 0 && pt_2_ <= pt2_split && n_bjets_ == 0) {
      SetPassCategory("0jet_low");
      FillCoreControlPlots("0jet_low");
    }

    // auto lowpt_jets_copy = lowpt_jets;
    // ic::erase_if(lowpt_jets_copy,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    // if (lowpt_jets_copy.size() >= 2 && n_bjets_ >= 1) SetPassCategory("sasha");
    if (n_jets_ <= 1 && prebjets.size() > 0) {
      SetPassCategory("prebtag");
      FillCoreControlPlots("prebtag");
    }

    if (n_jets_ <= 1 && n_bjets_ > 0) {
      SetPassCategory("btag");
      FillCoreControlPlots("btag");
    }
    
    if (n_jets_ <= 1 && n_bjets_ > 0 && pt_2_ <= pt2_split) SetPassCategory("btag_low");
    if (n_jets_ <= 1 && n_bjets_ > 0 && pt_2_ > pt2_split)  SetPassCategory("btag_high");

    if (n_jets_ <= 1 && n_loose_bjets_ > 0)                       SetPassCategory("btag_loose");
    if (n_jets_ <= 1 && n_loose_bjets_ > 0 && pt_2_ <= pt2_split) SetPassCategory("btag_low_loose");
    if (n_jets_ <= 1 && n_loose_bjets_ > 0 && pt_2_ > pt2_split)  SetPassCategory("btag_high_loose");


    if (!PassesCategory("vbf") && n_bjets_ == 0) SetPassCategory("nobtag");

    return 0;
  }

  bool HTTCategories::PassesCategory(std::string const& category) const {
    std::map<std::string, bool>::const_iterator it = categories_.find(category);
    if (it != categories_.end()) {
      return it->second;
    } else {
      std::cerr << "Error in HTTCategories::PassesCategory: No category registered with label " << category << std::endl;
      throw;
      return false;
    }
  }


  void HTTCategories::InitMassPlots(std::string const& category) {
    for (std::map<std::string, bool>::const_iterator it = selections_.begin(); it != selections_.end(); ++it) {
      massplots_[category+"_"+it->first] = new MassPlots(fs_->mkdir(category+"_"+it->first));
    }

  }
  void HTTCategories::InitCoreControlPlots(std::string const& category) {
    for (std::map<std::string, bool>::const_iterator it = selections_.begin(); it != selections_.end(); ++it) {
      controlplots_[category+"_"+it->first] = new CoreControlPlots(fs_->mkdir(category+"_"+it->first));
    }
  }

  void HTTCategories::Reset() {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      it->second = false;
    }
    for (std::map<std::string, bool>::iterator it = categories_.begin(); it != categories_.end(); ++it) {
      it->second = false;
    }
  }

  void HTTCategories::SetPassSelection(std::string const& selection) {
    std::map<std::string, bool>::iterator it = selections_.find(selection);
    if (it != selections_.end()) {
      it->second = true;
    } else {
      std::cerr << "Error in HTTCategories::SetPassSelection: No selection registered with label " << selection << std::endl;
      throw;
    }
  }

  void HTTCategories::SetPassCategory(std::string const& category) {
    std::map<std::string, bool>::iterator it = categories_.find(category);
    if (it != categories_.end()) {
      it->second = true;
      FillMassPlots(category);
      FillYields(category);
    } else {
      std::cerr << "Error in HTTCategories::SetPassCategory: No category registered with label " << category << std::endl;
      throw;
    }
  }

  void HTTCategories::FillMassPlots(std::string const& category) {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      if (it->second) {
        std::map<std::string, MassPlots*>::iterator p_it = massplots_.find(category+"_"+it->first);
        if (p_it != massplots_.end()) {
          MassPlots *plots = p_it->second;
          plots->m_sv->Fill(m_sv_, wt_);
          plots->m_sv_sm->Fill(m_sv_, wt_);
          plots->m_sv_sm_fine->Fill(m_sv_, wt_);
          plots->m_sv_mssm->Fill(m_sv_, wt_);
          plots->m_sv_mssm_fine->Fill(m_sv_, wt_);
          plots->m_vis->Fill(m_vis_, wt_);
          plots->m_vis_sm->Fill(m_vis_, wt_);
          plots->m_vis_sm_fine->Fill(m_vis_, wt_);
          plots->m_vis_mssm->Fill(m_vis_, wt_);
          plots->m_vis_mssm_fine->Fill(m_vis_, wt_);
        }
      } 
    }
  }

  void HTTCategories::FillYields(std::string const& category) {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      if (it->second) {
        yields_[category+"_"+it->first] = yields_[category+"_"+it->first] + wt_;
      } 
    }
  }


  void HTTCategories::FillCoreControlPlots(std::string const& category) {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      if (it->second) {
        std::map<std::string, CoreControlPlots*>::iterator p_it = controlplots_.find(category+"_"+it->first);
        if (p_it != controlplots_.end()) {
          CoreControlPlots *plots = p_it->second;
          plots->n_vtx->Fill(n_vtx_, wt_);
          plots->mt_1->Fill(mt_1_, wt_);
          plots->pzeta->Fill(pzeta_, wt_);
          plots->pt_h->Fill(pt_h_, wt_);
          plots->pt_tt->Fill(pt_tt_, wt_);
          plots->pt_1->Fill(pt_1_, wt_);
          plots->pt_2->Fill(pt_2_, wt_);
          plots->eta_1->Fill(eta_1_, wt_);
          plots->eta_2->Fill(eta_2_, wt_);
          plots->z_2->Fill(z_2_, wt_);
          plots->m_2->Fill(m_2_, wt_);
          plots->met->Fill(met_, wt_);
          plots->l1_met->Fill(l1_met_, wt_);
          plots->calo_nohf_met->Fill(calo_nohf_met_, wt_);
          plots->met_phi->Fill(met_phi_, wt_);
          plots->n_jets->Fill(n_jets_, wt_);
          plots->n_bjets->Fill(n_bjets_, wt_);
          plots->n_jetsingap->Fill(n_jetsingap_, wt_);
          if (tau_decay_mode_ == 0) plots->tau_decay_mode->Fill(0. , wt_);
          if (tau_decay_mode_ == 1 || tau_decay_mode_ == 2) plots->tau_decay_mode->Fill(1., wt_);
          if (tau_decay_mode_ == 10) plots->tau_decay_mode->Fill(2., wt_);
          // Have to be careful here: we fill the jet control plots even
          // though these values may not be defined. This is to be sure
          // when plotting the inclusive category yield is consistent
          // if (n_jets_ >= 1) {
            plots->jpt_1->Fill(jpt_1_, wt_);
            plots->jeta_1->Fill(jeta_1_, wt_);
          // }
          // if (n_jets_ >= 2) {
            plots->jpt_2->Fill(jpt_2_, wt_);
            plots->jeta_2->Fill(jeta_2_, wt_);            
            plots->mjj->Fill(mjj_, wt_);
            plots->jdeta->Fill(jdeta_, wt_);
          // }
          // if (n_bjets_ >= 1) {
            plots->bpt_1->Fill(bpt_1_, wt_);
            plots->beta_1->Fill(beta_1_, wt_);
            plots->bcsv_1->Fill(bcsv_1_, wt_);
          // }
        }
      } 
    }
  }



  int HTTCategories::PostAnalysis() {
    if (write_plots_) {
      std::cout << "-------------------------------------" << std::endl;
      std::cout << "HTTCategories" << std::endl;
      std::cout << "-------------------------------------" << std::endl;      
      std::vector<std::string> print_selections;
      print_selections.push_back("os_sel");
      print_selections.push_back("os_con");
      print_selections.push_back("ss_sel");
      print_selections.push_back("ss_con");
      std::vector<std::string> print_cats;
      print_cats.push_back("inclusive");
      print_cats.push_back("vbf_loose");
      print_cats.push_back("vbf");
      // print_cats.push_back("vbf_tight");
      print_cats.push_back("1jet_high");
      print_cats.push_back("1jet_low");
      print_cats.push_back("0jet_high");
      print_cats.push_back("0jet_low");
      print_cats.push_back("btag");
      print_cats.push_back("nobtag");
      std::cout << boost::format("%-20s") % "Selections:";
      for (unsigned i = 0; i < print_selections.size(); ++i) {
        std::cout << boost::format("%-12s") % print_selections[i];
      }
      std::cout << std::endl;
      for (unsigned i = 0; i < print_cats.size(); ++i) {
        std::cout << boost::format("%-20s") % print_cats[i];
        for (unsigned j = 0; j < print_selections.size(); ++j) {
          std::cout << boost::format("%-12s") % (yields_[print_cats[i]+"_"+print_selections[j]]);
        }
        std::cout << std::endl;
      }
    }
    return 0;
  }

  void HTTCategories::PrintInfo() {
    ;
  }
}
