#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauEfficiency.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  TauEfficiency::TauEfficiency(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    tau_label_ = "taus";
    gen_label_ = "genParticlesTaus"; // genParticlesEmbedded
    is_data_ = false;
    is_embedded_ = false;
    is_fake_ = false;
    gen_match_ = false;
    fs_ = NULL;
    hists_ = NULL;
  }

  TauEfficiency::~TauEfficiency() {
    ;
  }

  int TauEfficiency::PreAnalysis() {
    std::string param_fmt = "%-25s %-40s\n";

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for TauEfficiency" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << boost::format(param_fmt) % "tau_label" % tau_label_;
    std::cout << boost::format(param_fmt) % "gen_label" % gen_label_;
    std::cout << boost::format(param_fmt) % "is_data" % is_data_;
    std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded_;
    std::cout << boost::format(param_fmt) % "is_fake" % is_fake_;
    std::cout << boost::format(param_fmt) % "gen_match" % gen_match_;
    if (fs_) {
      hists_ = new Dynamic2DHistoSet(fs_->mkdir("TauEfficiency"));
      hists_->Create("barrel_mva1_num", 100, 0, 500, 100, -1, 1);
      hists_->Create("barrel_mva1_den", 100, 0, 500, 100, -1, 1);
      hists_->Create("barrel_mva2_num", 100, 0, 500, 100, -1, 1);
      hists_->Create("barrel_mva2_den", 100, 0, 500, 100, -1, 1);
      hists_->Create("barrel_3hit_num", 100, 0, 500, 100, 0, 10);
      hists_->Create("barrel_3hit_den", 100, 0, 500, 100, 0, 10);
      hists_->Create("endcap_mva1_num", 100, 0, 500, 100, -1, 1);
      hists_->Create("endcap_mva1_den", 100, 0, 500, 100, -1, 1);
      hists_->Create("endcap_mva2_num", 100, 0, 500, 100, -1, 1);
      hists_->Create("endcap_mva2_den", 100, 0, 500, 100, -1, 1);
      hists_->Create("endcap_3hit_num", 100, 0, 500, 100, 0, 10);
      hists_->Create("endcap_3hit_den", 100, 0, 500, 100, 0, 10);
    }

    return 0;
  }

  int TauEfficiency::Execute(TreeEvent *event) {
    // At this point taus have been selected on pt, eta, decayModeFinding and dz
    auto taus = event->GetPtrVec<Tau>(tau_label_);
    if (event->Exists("selMuons")) {
        std::vector<Muon *> const& muons = event->GetPtrVec<Muon>("selMuons");
        ic::erase_if(taus, !boost::bind(MinDRToCollection<Muon*>, _1, muons, 0.5));
    }
    if (event->Exists("selElectrons")) {
        std::vector<Electron *> const& elecs = event->GetPtrVec<Electron>("selElectrons");
        ic::erase_if(taus, !boost::bind(MinDRToCollection<Electron*>, _1, elecs, 0.5));
    }
    // Get the tau gen particles 
    std::vector<GenParticle *> parts;
    std::vector<GenJet *> tau_jets;
    if (!is_data_) {
      parts = event->GetPtrVec<GenParticle>(gen_label_);
      for (auto part1 : parts) {
        if (abs(part1->pdgid()) == 15) {
          std::vector<GenParticle *> daughters;
          std::vector<int> d_ids = part1->daughters();
          bool skip = false;
          for (auto part2 : parts) {
            if (std::find(d_ids.begin(), d_ids.end(), part2->index()) != d_ids.end()) {
              if (abs(part2->pdgid()) == 15 || abs(part2->pdgid()) == 11 || abs(part2->pdgid()) == 13) {
                skip = true;
                break;
              }
              if (abs(part2->pdgid()) != 16) daughters.push_back(part2);
            } 
          }
          if (skip) continue;
          ROOT::Math::PtEtaPhiEVector vis_vec;
          int charge = 0;
          for (auto dau : daughters) {
            vis_vec += dau->vector();
            charge += dau->charge();
          }
          tau_jets.push_back(new GenJet());
          tau_jets.back()->set_vector(vis_vec);
          tau_jets.back()->set_charge(charge);
        }
      }
    }

    if (gen_match_ && !is_fake_ && !is_data_) {
      auto pairs = MatchByDR(taus, tau_jets, 0.5, true, true);
      taus = ExtractFirst(pairs);
    }

    EventInfo *info = event->GetPtr<EventInfo>("eventInfo");
    double wt = info->total_weight();

    for (auto tau : taus) {
      double mva1 = tau->GetTauID("byIsolationMVAraw");
      double mva2 = tau->GetTauID("byIsolationMVA2raw");
      double hits = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
      std::string region = (fabs(tau->eta()) < 1.5) ? "barrel" : "endcap";
      TH2F *h_mva1_num = hists_->Get_Histo(region+"_mva1_num");
      TH2F *h_mva1_den = hists_->Get_Histo(region+"_mva1_den");
      for (unsigned i = 1; i <= h_mva1_num->GetNbinsY(); ++i) {
        if (mva1 > h_mva1_num->GetYaxis()->GetBinLowEdge(i)) {
          h_mva1_num->Fill(tau->pt(), h_mva1_num->GetYaxis()->GetBinCenter(i), wt);
        }
      }
      for (unsigned i = 1; i <= h_mva1_den->GetNbinsY(); ++i) {
        h_mva1_den->Fill(tau->pt(), h_mva1_den->GetYaxis()->GetBinCenter(i), wt);
      }
      TH2F *h_mva2_num = hists_->Get_Histo(region+"_mva2_num");
      TH2F *h_mva2_den = hists_->Get_Histo(region+"_mva2_den");
      for (unsigned i = 1; i <= h_mva2_num->GetNbinsY(); ++i) {
        if (mva2 > h_mva2_num->GetYaxis()->GetBinLowEdge(i)) {
          h_mva2_num->Fill(tau->pt(), h_mva2_num->GetYaxis()->GetBinCenter(i), wt);
        }
      }
      for (unsigned i = 1; i <= h_mva2_den->GetNbinsY(); ++i) {
        h_mva2_den->Fill(tau->pt(), h_mva2_den->GetYaxis()->GetBinCenter(i), wt);
      }
      TH2F *h_hits_num = hists_->Get_Histo(region+"_3hit_num");
      TH2F *h_hits_den = hists_->Get_Histo(region+"_3hit_den");
      for (unsigned i = 1; i <= h_hits_num->GetNbinsY(); ++i) {
        if (hits < (h_hits_num->GetYaxis()->GetBinLowEdge(i)+h_hits_num->GetYaxis()->GetBinWidth(i))) {
          h_hits_num->Fill(tau->pt(), h_hits_num->GetYaxis()->GetBinCenter(i), wt);
        }
      }
      for (unsigned i = 1; i <= h_hits_den->GetNbinsY(); ++i) {
        h_hits_den->Fill(tau->pt(), h_hits_den->GetYaxis()->GetBinCenter(i), wt);
      }
    }


    std::for_each(tau_jets.begin(), tau_jets.end(), [] (GenJet * jet) {delete jet;});
    return 0;
  }
  int TauEfficiency::PostAnalysis() {
    return 0;
  }

  void TauEfficiency::PrintInfo() {
    ;
  }
}
