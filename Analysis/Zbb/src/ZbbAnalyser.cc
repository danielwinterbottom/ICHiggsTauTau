#include "Zbb/interface/ZbbAnalyser.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

namespace ic {

  ZbbAnalyser::ZbbAnalyser(std::string const& name) 
      : ModuleBase(name) {
      fs_ = nullptr;
      set_z_flav_ = false;
  }

  ZbbAnalyser::~ZbbAnalyser() {
    ;
  }

  int ZbbAnalyser::PreAnalysis() { 
    if (fs_) {
      outtree_ = fs_->make<TTree>("ntuple","ntuple");
      outtree_->Branch("is_ee",     &is_ee_);
      outtree_->Branch("zflav",     &zflav_);
      outtree_->Branch("wt",        &wt_);
      outtree_->Branch("wt_1b_inc", &wt_1b_inc_);
      outtree_->Branch("wt_1b_exc", &wt_1b_exc_);
      outtree_->Branch("wt_2b_inc", &wt_2b_inc_);
      outtree_->Branch("met",       &met_);
      outtree_->Branch("m_z",       &m_z_);
      outtree_->Branch("pt_1",      &pt_1_);
      outtree_->Branch("eta_1",     &eta_1_);
      outtree_->Branch("pt_2",      &pt_2_);
      outtree_->Branch("eta_2",     &eta_2_);
      outtree_->Branch("n_jets",    &n_jets_);
      outtree_->Branch("jpt_1",     &jpt_1_);
      outtree_->Branch("jeta_1",    &jeta_1_);
      outtree_->Branch("jnsv_1",    &jnsv_1_);
      outtree_->Branch("jssv_1",    &jssv_1_);
      outtree_->Branch("jpt_2",     &jpt_2_);
      outtree_->Branch("jeta_2",    &jeta_2_);
      outtree_->Branch("jnsv_2",    &jnsv_2_);
      outtree_->Branch("jssv_2",    &jssv_2_);
      outtree_->Branch("n_b_jets",  &n_b_jets_);
      outtree_->Branch("bpt_1",     &bpt_1_);
      outtree_->Branch("beta_1",    &beta_1_);
      outtree_->Branch("m_sv_1",    &m_sv_1_);
      outtree_->Branch("bpt_2",     &bpt_2_);
      outtree_->Branch("beta_2",    &beta_2_);
      outtree_->Branch("m_sv_2",    &m_sv_2_);
      outtree_->Branch("dphi_z_bb", &dphi_z_bb_);
      outtree_->Branch("pt_bb",     &pt_bb_);
      outtree_->Branch("pt_z",      &pt_z_);
    }
    TH1F::SetDefaultSumw2();
    return 0;
  }

  int ZbbAnalyser::Execute(TreeEvent *event) {
    // Get the objects we need from the event
    auto const* info    = event->GetPtr<EventInfo>("eventInfo");
    auto const* met     = event->GetPtr<Met>("pfMet");
    auto const& channel = event->Get<std::string>("channel");
    auto const& pairs   = event->GetPtrVec<CompositeCandidate>("lepton_pair");
    auto const* pair    = pairs.at(0);
    std::vector<Candidate *> leptons = pair->AsVector();
    std::sort(leptons.begin(), leptons.end(),
        bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    auto const* lep1    = leptons[0];
    auto const* lep2    = leptons[1];
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("selected_jets");
    std::sort(jets.begin(), jets.end(),
        bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> bjets = jets;
    ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator,
        _1, "simpleSecondaryVertexHighEffBJetTags") <= 1.74);

    zflav_ = 0;
    if (set_z_flav_) {
      auto const& parts = event->GetPtrVec<GenParticle>("genParticles");
      unsigned n_b = std::count_if(parts.begin(), parts.end(),
        boost::bind(&GenParticle::pdgid, _1) == 5 ||
        boost::bind(&GenParticle::pdgid, _1) == -5);
      unsigned n_c = std::count_if(parts.begin(), parts.end(),
        boost::bind(&GenParticle::pdgid, _1) == 4 ||
        boost::bind(&GenParticle::pdgid, _1) == -4);
      if (n_b > 0) {
        zflav_ = 2;
      } else if (n_c > 0) {
        zflav_ = 1;
      }
    }

    is_ee_ = channel == "ee" ? true : false;
    wt_ = info->total_weight();


    // global variables
    met_ = met->pt();
    m_z_ = pair->M();

    // Leading lepton
    pt_1_  = lep1->pt();
    eta_1_ = lep1->eta();

    // Subleading lepton
    pt_2_  = lep2->pt();
    eta_2_ = lep2->eta();

    n_jets_  = jets.size();
    if (jets.size() >= 1) {
      jpt_1_  = jets[0]->pt();
      jeta_1_ = jets[0]->eta();
      jnsv_1_ = 0;
      jssv_1_ = jets[0]->GetBDiscriminator("simpleSecondaryVertexHighEffBJetTags");
    } else {
      jpt_1_  = 0.0;
      jeta_1_ = 0.0;
      jnsv_1_ = 0.0;
      jssv_1_ = 0.0;
    }
    if (jets.size() >= 2) {
      jpt_2_  = jets[1]->pt();
      jeta_2_ = jets[1]->eta();
      jnsv_2_ = 0;
      jssv_2_ = jets[1]->GetBDiscriminator("simpleSecondaryVertexHighEffBJetTags");
    } else {
      jpt_2_  = 0.0;
      jeta_2_ = 0.0;
      jnsv_2_ = 0.0;
      jssv_2_ = 0.0;
    }
    wt_1b_inc_ = 1.0;
    wt_1b_exc_ = 1.0;
    wt_2b_inc_ = 1.0;
    if (!info->is_data()) {
      if (jets.size() >= 1) {
        wt_1b_inc_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 100);
        wt_1b_exc_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 1);
      }
      if (jets.size() >= 2) {
        wt_2b_inc_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 2, 100);
      }
    }

    n_b_jets_ = bjets.size();
    // Highest pT b-tagged jet
    if (n_b_jets_ >= 1) {
      bpt_1_  = bjets[0]->pt();
      beta_1_ = bjets[0]->eta();
      m_sv_1_ = 0;
    } else {
      bpt_1_  = 0.0;
      beta_1_ = 0.0;
      m_sv_1_ = 0.0;
    }
    if (n_b_jets_ >= 2) {
      bpt_2_  = bjets[1]->pt();
      beta_2_ = bjets[1]->eta();
      m_sv_2_ = 0;
      CompositeCandidate bb_pair;
      bb_pair.AddCandidate("jet1", bjets[0]);
      bb_pair.AddCandidate("jet2", bjets[1]);
      dphi_z_bb_ = std::fabs(
        ROOT::Math::VectorUtil::DeltaPhi(pair->vector(), bb_pair.vector()));
      pt_bb_ = bb_pair.pt();
      pt_z_ = pair->pt();
    } else {
      bpt_2_  = 0.0;
      beta_2_ = 0.0;
      m_sv_2_ = 0.0;
      dphi_z_bb_ = 0.0;
      pt_bb_     = 0.0;
      pt_z_      = 0.0;
    }

    outtree_->Fill();
    return 0;
  }

  int ZbbAnalyser::PostAnalysis() {
    return 0;
  }

  void ZbbAnalyser::PrintInfo() { }
}
