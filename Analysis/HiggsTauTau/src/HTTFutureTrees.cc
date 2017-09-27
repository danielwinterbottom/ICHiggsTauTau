#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTFutureTrees.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include "TLorentzVector.h"

namespace ic {

  HTTFutureTrees::HTTFutureTrees(std::string const& name) : ModuleBase(name), 
      channel_(channel::et), 
      strategy_(strategy::paper2013) {
      ditau_label_ = "emtauCandidates";
      jets_label_ = "pfJetsPFlow";
      met_label_ = "pfMVAMetNoLeptons";
      fs_ = NULL;
      write_tree_ = true;
  }

  HTTFutureTrees::~HTTFutureTrees() {
    ;
  }

  int HTTFutureTrees::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTFutureTrees" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
      std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
      std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
      std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
      std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
      std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;
      std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;

    if (fs_ && write_tree_) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      outtree_->Branch("event",             &event_);
      outtree_->Branch("wt",                &wt_.var_double);
      outtree_->Branch("wt_btag",           &wt_btag_);
      outtree_->Branch("os",                &os_);
      outtree_->Branch("m_sv",              &m_sv_.var_double);
      outtree_->Branch("mt_sv",             &mt_sv_.var_double);
      outtree_->Branch("m_vis",             &m_vis_.var_double);
      outtree_->Branch("pt_h",              &pt_h_.var_double);
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
      outtree_->Branch("leptonveto",        &lepton_veto_);
      outtree_->Branch("dilepton_veto",     &dilepton_veto_);
      outtree_->Branch("extraelec_veto",    &extraelec_veto_);
      outtree_->Branch("extramuon_veto",    &extramuon_veto_);
      outtree_->Branch("pfmet",             &pfmet_.var_double);
      outtree_->Branch("pfmet_perp_res",    &met_perp_res_);
      outtree_->Branch("pfmet_par_res",    &met_par_res_);
      outtree_->Branch("n_jets",            &n_jets_);
      outtree_->Branch("n_bjets",           &n_bjets_);
      outtree_->Branch("n_loose_bjets",     &n_loose_bjets_);
      outtree_->Branch("mjj",               &mjj_.var_double);
      outtree_->Branch("n_jetsingap",       &n_jetsingap_);
      outtree_->Branch("jdeta",             &jdeta_.var_double);
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
      outtree_->Branch("tau_decay_mode_2",    &tau_decay_mode_2_);
      outtree_->Branch("tau_decay_mode_1",    &tau_decay_mode_1_);
      outtree_->Branch("jet_flav_1", &jet_flav_1_);
      outtree_->Branch("jet_flav_2", &jet_flav_2_);
      outtree_->Branch("n_vtx",             &n_vtx_);
      outtree_->Branch("good_vtx",          &good_vtx_);
      outtree_->Branch("phi_1",             &phi_1_.var_double);
      outtree_->Branch("phi_2",             &phi_2_.var_double);
      outtree_->Branch("dphi",              &dphi_);
      outtree_->Branch("E_1",               &E_1_);
      outtree_->Branch("E_2",               &E_2_);
      outtree_->Branch("z_2",               &z_2_);
      outtree_->Branch("met_phi",           &mvamet_phi_.var_double);
      outtree_->Branch("n_prebjets",        &n_prebjets_);
      outtree_->Branch("jpt_1",             &jpt_1_.var_double);
      outtree_->Branch("nearjpt_1",             &nearjpt_1_);
      outtree_->Branch("j1_dm",             &j1_dm_);
      outtree_->Branch("jpt_2",             &jpt_2_.var_double);
      outtree_->Branch("jeta_1",            &jeta_1_.var_double);
      outtree_->Branch("jeta_2",            &jeta_2_.var_double);
      outtree_->Branch("bpt_1",             &bpt_1_.var_double);
      outtree_->Branch("beta_1",            &beta_1_.var_double);
      outtree_->Branch("bcsv_1",            &bcsv_1_.var_double);
      outtree_->Branch("pzetavis",          &pzetavis_.var_double);
      outtree_->Branch("pzetamiss",         &pzetamiss_.var_double);
      outtree_->Branch("mt_ll",             &mt_ll_);
    }

   return 0;
  }

  int HTTFutureTrees::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
   
    run_ = eventInfo->run();
    event_ = (unsigned long long) eventInfo->event();
    lumi_ = eventInfo->lumi_block();
    rho_ = eventInfo->jet_rho();
    if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    if(event->Exists("gen_match_2")) gen_match_2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
    if(event->Exists("gen_match_1_pt")) gen_match_1_pt_ = event->Get<double>("gen_match_1_pt");
    if(event->Exists("gen_match_2_pt")) gen_match_2_pt_ = event->Get<double>("gen_match_2_pt");

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
//    std::vector<PFJet*> uncleaned_jets = event->GetPtrVec<PFJet>(jets_label_+"UnFiltered");

    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    std::vector<PFJet*> prebjets = lowpt_jets;
    ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<PFJet*> bjets = prebjets;
    std::vector<PFJet*> loose_bjets = prebjets;
    std::string btag_label="pfCombinedInclusiveSecondaryVertexV2BJetTags";
    double btag_wp =  0.8;
    ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) <btag_wp);

    if (PairOppSign(ditau)) {
      os_ = true;
    } else {
      os_ = false;
    }
    dilepton_veto_ = false;
    extraelec_veto_ = false;
    extramuon_veto_ = false;
    if(channel_ == channel::et) { 
        if(event->Exists("dielec_veto")) dilepton_veto_ = event->Get<bool>("dielec_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
    }
    if(channel_ == channel::mt) { 
        if(event->Exists("dimuon_veto")) dilepton_veto_ = event->Get<bool>("dimuon_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
    }
    if(channel_ == channel::em) { 
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
    }
    if(channel_ == channel::tt) {
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
    }
    lepton_veto_ = dilepton_veto_ || extraelec_veto_ || extramuon_veto_;
    n_vtx_ = eventInfo->good_vertices();
    if (event->Exists("svfitMass")) {
      m_sv_ = event->Get<double>("svfitMass");
    } else {
      m_sv_ = -9999;
    }

    Met const* pfmet = NULL;
    pfmet = event->GetPtr<Met>("pfMET");
    pfpt_tt_ = (ditau->vector() + pfmet->vector()).pt();
    pt_tt_ = pfpt_tt_;

    Met const* genmet=NULL;
    std::vector<Met*> genmet_vec = event->GetPtrVec<Met>("GenMetFromSlimmed");
    genmet = genmet_vec.at(0); 
    double met_genmetdphi= fabs(ROOT::Math::VectorUtil::DeltaPhi(pfmet->vector(),genmet->vector()));
    met_perp_res_ = TMath::Sin(met_genmetdphi)*pfmet->pt();
    met_par_res_ = TMath::Cos(met_genmetdphi)*pfmet->pt()-genmet->pt();
    
  
    m_vis_ = ditau->M();
    mt_lep_ = MT(lep1,lep2);
    mt_ll_ = MT(ditau, pfmet);
    pfpzeta_ = PZeta(ditau, pfmet, 0.85);
    pfpzetamiss_ = PZeta(ditau, pfmet, 0.0);
    pzetavis_ = PZetaVis(ditau);
    pzeta_ = pfpzeta_;
    pzetamiss_ = pfpzetamiss_;
    met_dphi_1_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pfmet->vector(),lep1->vector()));
    met_dphi_2_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(pfmet->vector(),lep2->vector()));
    //save some pfmet and puppi met versions as well for now
    pfmt_1_ = MT(lep1, pfmet);
    pfmt_2_ = MT(lep2, pfmet);
    pfmt_tot_ = sqrt(pow(mt_lep_.var_double,2)+pow(pfmt_2_.var_double,2)+pow(pfmt_1_.var_double,2));
    mt_1_ = pfmt_1_;
    mt_2_ = pfmt_2_;
    mt_tot_ = pfmt_tot_;
    if(channel_ == channel::em || channel_ == channel::et){
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      id_e_mva_nt_loose_1_ = elec->GetIdIso("mvaNonTrigSpring15");
    }
    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();
    phi_1_ = lep1->phi();
    phi_2_ = lep2->phi();
    dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector()));
    E_1_ = lep1->energy();
    E_2_ = lep2->energy();
    m_1_ = lep1->M();
    m_2_ = lep2->M();
    q_1_ = lep1->charge();
    q_2_ = lep2->charge();
    pfmet_ = pfmet->pt();
    pfmet_phi_ = pfmet->phi();
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
      iso_1_ = PF03IsolationVal(elec, 0.5, 0);
      mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
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
   } else if (channel_ == channel::mt){
      Muon const* muon = dynamic_cast<Muon const*>(lep1);
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      d0_1_ = muon->dxy_vertex();
      dz_1_ = muon->dz_vertex();
      d0_2_ = tau->lead_dxy_vertex();
      dz_2_ = tau->lead_dz_vertex();
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
      antimu_2_ = lagainstMuonTight3_2;
   }else if (channel_ == channel::em) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      Muon const* muon = dynamic_cast<Muon const*>(lep2);
      iso_1_ = PF03IsolationVal(elec, 0.5, 0);
      iso_2_ = PF04IsolationVal(muon, 0.5, 0);
      mva_1_ = elec->GetIdIso("mvaNonTrigSpring15");
      lPhotonPtSum_1 = 0.;
      lPhotonPtSum_2 = 0.;
      mva_2_ = 0.0;
      d0_1_ = elec->dxy_vertex();
      d0_2_ = muon->dxy_vertex();
      dz_1_ = elec->dz_vertex();
      dz_2_ = muon->dz_vertex();
  } else if (channel_ == channel::tt){
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      d0_1_ = tau1->lead_dxy_vertex();
      dz_1_ = tau1->lead_dz_vertex();
      d0_2_ = tau2->lead_dxy_vertex();
      dz_2_ = tau2->lead_dz_vertex();
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

    n_jets_ = jets.size();
    n_lowpt_jets_ = lowpt_jets.size();
    n_bjets_ = bjets.size();
    n_prebjets_ = prebjets.size();
    n_loose_bjets_ = loose_bjets.size();

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
      jdphi_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), lowpt_jets[1]->vector()));
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
    if (write_tree_ && fs_) outtree_->Fill();

    return 0;
  }

  int HTTFutureTrees::PostAnalysis() {
    return 0;
  }

  void HTTFutureTrees::PrintInfo() {
    ;
  }

}

