#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/WMuNuCategories.h"
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

  WMuNuCategories::WMuNuCategories(std::string const& name) : ModuleBase(name), 
      channel_(channel::et), 
      era_(era::data_2012_rereco),
      strategy_(strategy::paper2013) {
      muon_label_ = "sel_muons";
      jets_label_ = "pfJetsPFlow";
      met_label_ = "pfMVAMetNoLeptons";
      fs_ = NULL;
      write_tree_ = true;
      is_data_=false;
  }

  WMuNuCategories::~WMuNuCategories() {
    ;
  }

  int WMuNuCategories::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "WMuNuCategories" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
      std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
      std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
      std::cout << boost::format(param_fmt()) % "era"             % Era2String(era_);
      std::cout << boost::format(param_fmt()) % "muon_label"      % muon_label_;
      std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
      std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;
      std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;

    if (fs_ && write_tree_) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      outtree_->Branch("wt",                &wt_.var_double);
      outtree_->Branch("mt_1",              &mt_1_.var_double);
      outtree_->Branch("iso_1",             &iso_1_.var_double);
      outtree_->Branch("met",               &mvamet_.var_double);
      outtree_->Branch("n_jets",            &n_jets_);
      outtree_->Branch("n_bjets",           &n_bjets_);
      outtree_->Branch("mjj",               &mjj_.var_double);
      outtree_->Branch("jdeta",             &jdeta_.var_double);
      //Variables needed for control plots need only be generated for central systematics
      outtree_->Branch("n_vtx",             &n_vtx_);
      outtree_->Branch("pt_1",              &pt_1_.var_double);
      outtree_->Branch("eta_1",             &eta_1_.var_double);
      outtree_->Branch("met_phi",           &mvamet_phi_.var_double);
      outtree_->Branch("n_loose_bjets",     &n_loose_bjets_);
      outtree_->Branch("jpt_1",             &jpt_1_.var_double);
      outtree_->Branch("j1_dm",             &j1_dm_);
      outtree_->Branch("jpt_2",             &jpt_2_.var_double);
      outtree_->Branch("jeta_1",            &jeta_1_.var_double);
      outtree_->Branch("jeta_2",            &jeta_2_.var_double);
      outtree_->Branch("bpt_1",             &bpt_1_.var_double);
      outtree_->Branch("beta_1",            &beta_1_.var_double);
      outtree_->Branch("bcsv_1",            &bcsv_1_.var_double);
    }

    return 0;
  }

  int WMuNuCategories::Execute(TreeEvent *event) {

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

    wt_ = {eventInfo->total_weight(), static_cast<float>(eventInfo->total_weight())};
    run_ = eventInfo->run();
    event_ = (unsigned long long) eventInfo->event();
    lumi_ = eventInfo->lumi_block();
    std::vector<PileupInfo *> puInfo;

    rho_ = eventInfo->jet_rho();
    
    std::vector<Muon *> & muons_vec = event->GetPtrVec<Muon>(muon_label_);
    std::sort(muons_vec.begin(), muons_vec.end(), bind(&Muon::pt, _1) > bind(&Muon::pt, _2));
    Muon const* lep1 = muons_vec.at(0);
//    std::vector <Met const* mets = event->GetPtrVec<Met>(met_label_);
   // std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);
    std::vector<Met*> met_vec = event->GetPtrVec<Met>("pfMet");
    Met const* mets = met_vec.at(0);  

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::vector<PFJet*> corrected_jets;
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
    if(strategy_ == strategy::spring15) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::spring15) btag_wp = 0.814 ;
    ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < 0.244);
    //Extra set of jets which are CSV ordered is required for the H->hh analysis
    std::vector<PFJet*> jets_csv = prebjets;
    std::vector<PFJet*> bjets_csv = prebjets;
    std::sort(jets_csv.begin(), jets_csv.end(), bind(&PFJet::GetBDiscriminator, _1, btag_label) > bind(&PFJet::GetBDiscriminator, _2, btag_label));

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
    
    // Define event properties
    // IMPORTANT: Make sure each property is re-set
    // for each new event
    
    n_vtx_ = eventInfo->good_vertices();


    Met const* pfmet = NULL;
    //slightly different pfMET format for new ntuples
    if(strategy_ == strategy::paper2013) pfmet = event->GetPtr<Met>("pfMet");
    if(strategy_ == strategy::phys14 || strategy_ == strategy::spring15) {
      std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMet");
      pfmet = pfMet_vec.at(0);  
    }



    // This is the HCP hack for the em channel
    // to better align the data with the embedded
    // mass.  

    mt_1_ = MT(lep1, pfmet);

    pt_1_ = lep1->pt();
    eta_1_ = lep1->eta();
    phi_1_ = lep1->phi();
    m_1_ = lep1->M();
    q_1_ = lep1->charge();
    mvamet_ = mets->pt();
    mvamet_phi_ = mets->phi();

    mvametCov00_ = mets->xx_sig();
    mvametCov10_ = mets->yx_sig();
    mvametCov01_ = mets->xy_sig();
    mvametCov11_ = mets->yy_sig();
    
    pfmet_ = pfmet->pt();
    pfmet_phi_ = pfmet->phi();
  
    pfmetCov00_ = pfmet->xx_sig();
    pfmetCov01_ = pfmet->xy_sig();
    pfmetCov10_ = pfmet->yx_sig();
    pfmetCov11_ = pfmet->yy_sig();
    

    if (channel_ == channel::wmnu) {
     // Muon const* muon = dynamic_cast<Muon const*>(lep1);
      d0_1_ = lep1->dxy_vertex();
      if(strategy_ == strategy::phys14 || strategy_ == strategy::spring15) {
        iso_1_ = PF03IsolationVal(lep1, 0.5, 0);
        mva_1_ = 0.0;
      }
    }

    n_jets_ = jets.size();
    n_bjets_ = bjets.size();
    n_loose_bjets_ = loose_bjets.size();


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


    if (n_bjets_ >= 1) {
      bcsv_1_ = bjets[0]->GetBDiscriminator(btag_label);
    } else {
      bcsv_1_ = -9999;
    }
    if (n_bjets_ >= 2) {
      bcsv_2_ = bjets[1]->GetBDiscriminator(btag_label);
    } else {
      bcsv_2_ = -9999;
    }

    
    if (write_tree_ && fs_) outtree_->Fill();


    return 0;
  }

  int WMuNuCategories::PostAnalysis() {
    return 0;
  }

  void WMuNuCategories::PrintInfo() {
    ;
  }
}
