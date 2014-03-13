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
      outtree_->Branch("wt_1b_inc_d", &wt_1b_inc_d_);
      outtree_->Branch("wt_1b_exc_d", &wt_1b_exc_d_);
      outtree_->Branch("wt_2b_inc_d", &wt_2b_inc_d_);
      outtree_->Branch("wt_1b_inc_u", &wt_1b_inc_u_);
      outtree_->Branch("wt_1b_exc_u", &wt_1b_exc_u_);
      outtree_->Branch("wt_2b_inc_u", &wt_2b_inc_u_);
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

    if (!info->is_data()) {
      if (is_ee_) {
        wt_ *= ZbbAnalyser::ElectronWeight(lep1, lep2);
      } else {
        wt_ *= ZbbAnalyser::MuonWeight(lep1, lep2);
      }
    }

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
    wt_1b_inc_d_ = 1.0;
    wt_1b_exc_d_ = 1.0;
    wt_2b_inc_d_ = 1.0;
    wt_1b_inc_u_ = 1.0;
    wt_1b_exc_u_ = 1.0;
    wt_2b_inc_u_ = 1.0;
    if (!info->is_data()) {
      if (jets.size() >= 1) {
        wt_1b_inc_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 100);
        wt_1b_exc_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 1);
        wt_1b_inc_d_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 100, 1);
        wt_1b_exc_d_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 1, 1);
        wt_1b_inc_u_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 100, 2);
        wt_1b_exc_u_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 1, 1, 2);
      }
      if (jets.size() >= 2) {
        wt_2b_inc_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 2, 100);
        wt_2b_inc_d_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 2, 100, 1);
        wt_2b_inc_u_ = btag_weight_.
          GetLouvainWeight(jets, BTagWeight::tagger::SSVHEM, 2, 100, 2);
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

  double ZbbAnalyser::ElectronWeight(Candidate const* elec1, Candidate const* elec2) {
    static std::vector<double> ele_id = {
      1.0014,
      1.0040,
      1.0015,
      0.9847,
      1.0094,
      0.9927,
      1.0078,
      0.9442,
    };
    static std::vector<double> ele_iso = {
      1.0037,
      0.9886,
      1.0437,
      1.0130  
    };
    static std::vector<double> ele_trg = {
      0.9906,
      0.98925,
      0.9893,
      0.9893
    };
    double wt = 1.0;
    for (Candidate const* c : {elec1, elec2}) {
      double pt = c->pt();
      double a_eta = std::fabs(c->eta());
      unsigned pt_bin = (pt < 50) ? 0 : 1;
      unsigned eta_bin1 = 
        (a_eta < 0.8)   ? 0 : 
        ((a_eta < 1.44) ? 1 :
        ((a_eta<2.0)    ? 2 : 3));
      unsigned eta_bin2 = (a_eta < 1.6) ? 0 : 1;
      unsigned eta_bin3 = 
        (a_eta < 0.8) ? 0 : 
        ((a_eta < 1.6) ? 1 : 2 );
      
      unsigned bin1 = 2*eta_bin1+pt_bin;
      unsigned bin2 = 2*eta_bin2+pt_bin;
      unsigned bin3 = eta_bin3;

      double id = 1.0;
      if (bin1 < ele_id.size()) id = ele_id[bin1];
      double iso_trg = 1.0;
      if (bin2 < ele_iso.size()) iso_trg = ele_iso[bin2] * ele_trg[bin3];
      wt *= (id*iso_trg);
    }
    return wt;
  }

  double ZbbAnalyser::MuonWeight(Candidate const* muon1, Candidate const* muon2) {
    static std::vector<double> mu_id = {
      0.995,
      0.987,
      0.993,
      0.996
    };
    static std::vector<double> mu_iso = {
      1.022,
      1.001,
      1.017,
      1.002
    };
    static std::vector<double> mu_7_trg = {
      0.971, 0.957, 0.954
    };
    static std::vector<double> mu_8_trg = {
      0.973, 0.964, 0.952
    };
    static std::vector<double> mu_13_trg = {
      0.973, 0.962, 0.946
    };
    double wt = 1.0;
    for (Candidate const* c : {muon1, muon2}) {
      double pt = c->pt();
      double a_eta = std::fabs(c->eta());
      unsigned pt_bin = (pt < 50) ? 0 : 1;
      unsigned eta_bin = (a_eta < 1.2) ? 0 : 1;
      unsigned bin = 2*eta_bin+pt_bin;
      if (bin < mu_id.size()) wt *= (mu_id[bin] * mu_iso[bin]);
    }
    unsigned eta_bin1 = (fabs(muon1->eta()) < 0.9) ? 0 : ((fabs(muon1->eta()) < 1.5) ? 1 : 2);
    unsigned eta_bin2 = (fabs(muon2->eta()) < 0.9) ? 0 : ((fabs(muon2->eta()) < 1.5) ? 1 : 2);
    double mu_7 = mu_7_trg[eta_bin1] * mu_7_trg[eta_bin2];
    double mu_13_mu_8 = mu_13_trg[eta_bin1] * mu_8_trg[eta_bin2]
        + mu_8_trg[eta_bin1] * mu_13_trg[eta_bin2]
        - mu_13_trg[eta_bin1] * mu_13_trg[eta_bin2];
    wt *= 0.10*mu_7 + 0.90*mu_13_mu_8;
    return wt;
  }

  int ZbbAnalyser::PostAnalysis() {
    return 0;
  }

  void ZbbAnalyser::PrintInfo() { }
}
