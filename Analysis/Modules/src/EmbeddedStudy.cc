#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EmbeddedStudy.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"

namespace ic {

  EmbeddedStudy::EmbeddedStudy(std::string const& name) : ModuleBase(name) {

    fs_ = NULL;
    is_dyjets_ = false;

}

  EmbeddedStudy::~EmbeddedStudy() {
    ;
  }

  int EmbeddedStudy::PreAnalysis() {

    if (fs_) hset = new DynamicHistoSet(fs_->mkdir("kinematics"));
    if (fs_) h2dset = new Dynamic2DHistoSet(fs_->mkdir("kinematics_2d"));
    hset->Create("embed_decay_mu__pt", 100, 0, 100);
    hset->Create("embed_decay_mu__pt__eweight", 100, 0, 100);
    hset->Create("embed_tau1__pt", 100, 0, 100);
    hset->Create("embed_tau2__pt", 100, 0, 100);
    hset->Create("gen_z__mass", 160, 60, 120);
    hset->Create("st1_gen_z__mass", 160, 60, 120);
    hset->Create("embed_z__mass", 160, 60, 120);


    h2dset->Create("gen_vs_emb__pt", 100, 0, 100, 100, 0, 100);
    h2dset->Create("gen_vs_emb__eta", 50, -2.5, 2.5, 50, -2.5, 2.5);
    h2dset->Create("st1_gen_vs_emb__pt", 100, 0, 100, 100, 0, 100);
    h2dset->Create("st1_gen_vs_emb__eta", 50, -2.5, 2.5, 50, -2.5, 2.5);

    h2dset->Create("mass_unfold", 60, 60, 120, 60, 60, 120);
    hset->Create("mass_unfold_gen_test", 60, 60, 120);
    hset->Create("mass_unfold_reco_test", 60, 60, 120);
    hset->Create("mass_unfold_reco_result", 60, 60, 120);
    hset->Create("mass_unfold_gen_result", 60, 60, 120);

    return 0;
  }

  int EmbeddedStudy::Execute(TreeEvent *event) {

    EventInfo *info = event->GetPtr<EventInfo>("eventInfo");
    
    // First identify the two gen muons
    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> gen_muons;
    std::vector<GenParticle *> st1_gen_muons;
    for (unsigned i = 0; i < parts.size(); ++i) {
      if (abs(parts[i]->pdgid()) == 13 && parts[i]->status() == 3) gen_muons.push_back(parts[i]);
      if (abs(parts[i]->pdgid()) == 13 && parts[i]->status() == 1 && parts[i]->pt() > 8.0) st1_gen_muons.push_back(parts[i]);
    }

    // Only requiring a Z->mumu decay(status 3)
    if (gen_muons.size() != 2) return 1;

    // If this is DYJetsToLL, apply embedding phase space cuts first:
    if (is_dyjets_) {
      std::sort(gen_muons.begin(), gen_muons.end(), bind(&GenParticle::pt, _1) > bind(&GenParticle::pt, _2));
      if (!(
            gen_muons[0]->pt() > 20. && fabs(gen_muons[0]->eta()) < 2.5 
        &&  gen_muons[1]->pt() > 10. && fabs(gen_muons[1]->eta()) < 2.5) ) return 1;
    }


    hset->Fill("gen_z__mass", (gen_muons[0]->vector() + gen_muons[1]->vector()).M());
    hset->Fill("mass_unfold_gen_test", (gen_muons[0]->vector() + gen_muons[1]->vector()).M());

    if (st1_gen_muons.size() != 2) return 1;
    hset->Fill("st1_gen_z__mass", (st1_gen_muons[0]->vector() + st1_gen_muons[1]->vector()).M());
    

    // Then get the two embedded (sim) taus

    if (!is_dyjets_) {
      std::vector<GenParticle *> embed_taus;
      std::vector<GenParticle *> embedded_parts = event->GetPtrVec<GenParticle>("genParticlesEmbedded");

      GenParticle * embed_decay_mu = NULL;
      for (unsigned i = 0; i < embedded_parts.size(); ++i) {
        //std::cout << i << "\t" << embedded_parts[i]->status() << "\t" << embedded_parts[i]->pdgid() << "\t" << embedded_parts[i]->vector() << std::endl;
        if (abs(embedded_parts[i]->pdgid()) == 15 && embedded_parts[i]->status() == 2) embed_taus.push_back(embedded_parts[i]);
        if (abs(embedded_parts[i]->pdgid()) == 13 && embedded_parts[i]->status() == 1) embed_decay_mu = embedded_parts[i];
      }
      if (embed_taus.size() != 2) return 1;
      hset->Fill("embed_z__mass", (embed_taus[0]->vector() + embed_taus[1]->vector()).M());
      hset->Fill("mass_unfold_reco_test", (embed_taus[0]->vector() + embed_taus[1]->vector()).M());


      std::sort(embed_taus.begin(), embed_taus.end(), bind(&GenParticle::pt, _1) > bind(&GenParticle::pt, _2));
      hset->Fill("embed_tau1__pt", embed_taus[0]->pt());
      hset->Fill("embed_tau2__pt", embed_taus[1]->pt());

      std::vector<std::pair<GenParticle*, GenParticle*> > pairs = MatchByDR(gen_muons, embed_taus, 0.5, true, true);
      if (pairs.size() != 2) return 1;

      std::vector<std::pair<GenParticle*, GenParticle*> > st1_pairs = MatchByDR(st1_gen_muons, embed_taus, 0.5, true, true);
      if (st1_pairs.size() != 2) return 1;

      h2dset->Fill("mass_unfold", (gen_muons[0]->vector() + gen_muons[1]->vector()).M(), (embed_taus[0]->vector() + embed_taus[1]->vector()).M());

      h2dset->Fill("gen_vs_emb__pt", pairs[0].first->pt(), pairs[0].second->pt());
      h2dset->Fill("gen_vs_emb__eta", pairs[0].first->eta(), pairs[0].second->eta());
      h2dset->Fill("gen_vs_emb__pt", pairs[1].first->pt(), pairs[1].second->pt());
      h2dset->Fill("gen_vs_emb__eta", pairs[1].first->eta(), pairs[1].second->eta());

      h2dset->Fill("st1_gen_vs_emb__pt", st1_pairs[0].first->pt(), st1_pairs[0].second->pt());
      h2dset->Fill("st1_gen_vs_emb__eta", st1_pairs[0].first->eta(), st1_pairs[0].second->eta());
      h2dset->Fill("st1_gen_vs_emb__pt", st1_pairs[1].first->pt(), st1_pairs[1].second->pt());
      h2dset->Fill("st1_gen_vs_emb__eta", st1_pairs[1].first->eta(), st1_pairs[1].second->eta());
      if (embed_decay_mu) {
        hset->Fill("embed_decay_mu__pt", embed_decay_mu->pt());
        hset->Fill("embed_decay_mu__pt__eweight", embed_decay_mu->pt(), info->weight("embed_weight"));
      }
    }

    return 0;
  }
  int EmbeddedStudy::PostAnalysis() {
    TH2F *h2d = h2dset->Get_Histo("mass_unfold");
    unsigned nx = h2d->GetNbinsX();
    unsigned ny = h2d->GetNbinsY();

    for (unsigned i = 1; i <= nx; ++i) {
      double col_tot = 0.0;
      for (unsigned j = 1; j <= ny; ++j) col_tot += h2d->GetBinContent(i, j);
      for (unsigned j = 1; j <= ny; ++j) h2d->SetBinContent(i, j, h2d->GetBinContent(i, j) / col_tot);
    }

    typedef ROOT::Math::SMatrix<double, 60>             SMatrix60;
    typedef ROOT::Math::SVector<double, 60>             SVector60;

    SMatrix60 matrix;

    for (unsigned i = 1; i <= 60; ++i) {
      for (unsigned j = 1; j <= 60; ++j) {
        matrix(j-1, i-1) = h2d->GetBinContent(i, j);
      }
    }

    TH1F *gen_test = hset->Get_Histo("mass_unfold_gen_test");
    TH1F *reco_result = hset->Get_Histo("mass_unfold_reco_result");

    SVector60 gen_test_vec;
    for (unsigned x = 1; x <= 60; ++x) gen_test_vec(x-1) = gen_test->GetBinContent(x);
    SVector60 reco_result_vec = matrix * gen_test_vec;
    for (unsigned x = 1; x <= 60; ++x) reco_result->SetBinContent(x, reco_result_vec(x-1));

    matrix.Invert();

    TH1F *reco_test = hset->Get_Histo("mass_unfold_reco_test");
    TH1F *gen_result = hset->Get_Histo("mass_unfold_gen_result");
    SVector60 reco_test_vec;
    for (unsigned x = 1; x <= 60; ++x) reco_test_vec(x-1) = reco_test->GetBinContent(x);
    SVector60 gen_result_vec = matrix * reco_test_vec;
    for (unsigned x = 1; x <= 60; ++x) gen_result->SetBinContent(x, gen_result_vec(x-1));

    return 0;
  }

  void EmbeddedStudy::PrintInfo() {
    ;
  }
}
