#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauIDCategories.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  TauIDCategories::TauIDCategories(std::string const& name) : ModuleBase(name), 
  channel_(channel::et), 
  era_(era::data_2012_moriond),
  strategy_(strategy::paper2013) {
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    mass_shift_ = 1.0;
    fs_ = NULL;
    
  }

  TauIDCategories::~TauIDCategories() {
    ;
  }

  

  int TauIDCategories::PreAnalysis() {
    std::cout << "** PreAnalysis Info for HTT Categories **" << std::endl;
    if (fs_) {
      std::cout << "Channel: " << Channel2String(channel_) << std::endl;
      std::cout << "Ditau Label: " << ditau_label_ << std::endl;
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "Mass Shift: " << mass_shift_ << std::endl;
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      outtree_->Branch("wt",                &wt_);
      outtree_->Branch("os",                &os_);
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
      outtree_->Branch("tau_iso",           &tau_iso_);
      outtree_->Branch("m_2",               &m_2_);
      outtree_->Branch("met",               &met_);
      outtree_->Branch("met_phi",           &met_phi_);
      outtree_->Branch("tau_decay_mode",    &tau_decay_mode_);
      outtree_->Branch("n_jets",             &n_jets_);
      outtree_->Branch("n_lowpt_jets",      &n_lowpt_jets_);
      outtree_->Branch("n_bjets",           &n_bjets_);
      outtree_->Branch("jpt_1",             &jpt_1_);
      outtree_->Branch("jpt_2",             &jpt_2_);
      outtree_->Branch("jeta_1",            &jeta_1_);
      outtree_->Branch("jeta_2",            &jeta_2_);
      outtree_->Branch("bpt_1",             &bpt_1_);
      outtree_->Branch("beta_1",            &beta_1_);
      outtree_->Branch("mjj",               &mjj_);
      outtree_->Branch("jdeta",             &jdeta_);
      outtree_->Branch("mjj_lowpt",         &mjj_lowpt_);
      outtree_->Branch("jdeta_lowpt",       &jdeta_lowpt_);
      outtree_->Branch("n_jetsingap_lowpt", &n_jetsingap_lowpt_);
    }

    return 0;
  }

  int TauIDCategories::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

    wt_ = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    Met const* met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    std::vector<PFJet*> prebjets = lowpt_jets;
    ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<PFJet*> bjets = prebjets;
    std::vector<PFJet*> loose_bjets = prebjets;
    ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.244);

  // Instead of changing b-tag value in the promote/demote method we look for a map of bools
  // that say whether a jet should pass the WP or not
    if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
      ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
    } else {
      ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
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

  // This is the HCP hack for the em channel
  // to better align the data with the embedded
  // mass.  
    if (channel_ == channel::em) {
      m_sv_ = m_sv_ * mass_shift_;
      m_vis_ = m_vis_ * mass_shift_;
    }
    if (event->Exists("mass_scale")) {
      m_sv_ = m_sv_ * event->Get<double>("mass_scale");
      m_vis_ = m_vis_ * event->Get<double>("mass_scale");
    }

    mt_1_ = MT(lep1, met);
    pzeta_ = PZeta(ditau, met, 0.85);

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();

    m_2_ = lep2->M();
    met_ = met->pt();
    met_phi_ = met->phi();

    Tau const* tau = dynamic_cast<Tau const*>(lep2);
    if (tau) {
      tau_decay_mode_ = tau->decay_mode();
      z_2_ = tau->vz() + (130. / tan(tau->vector().theta()));
      tau_iso_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    } else {
      tau_decay_mode_ = 0;
      z_2_ = 0.;
    }

    n_jets_ = jets.size();
    n_lowpt_jets_ = lowpt_jets.size();
    n_bjets_ = bjets.size();
    n_loose_bjets_ = loose_bjets.size();

    if (n_jets_ >= 1) {
      jpt_1_ = jets[0]->pt();
      jeta_1_ = jets[0]->eta();
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

    if (prebjets.size() >= 1) {
      bcsv_1_ = prebjets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags");
    } else {
      bcsv_1_ = -9999;
    }

    if (outtree_) outtree_->Fill();
    return 0;
  }



  int TauIDCategories::PostAnalysis() {
    return 0;
  }

  void TauIDCategories::PrintInfo() {
    ;
  }
}
