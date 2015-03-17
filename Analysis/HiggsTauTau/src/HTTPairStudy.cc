#include "HiggsTauTau/interface/HTTPairStudy.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"
#include "boost/range/algorithm_ext/erase.hpp"

namespace ic {

  HTTPairStudy::HTTPairStudy(std::string const& name) : ModuleBase(name) {
  }

  HTTPairStudy::~HTTPairStudy() {
    ;
  }

  int HTTPairStudy::PreAnalysis() {
    return 0;
  }

  int HTTPairStudy::Execute(TreeEvent *event) {
    auto elecs = event->GetPtrVec<Electron>("electrons");
    auto muons = event->GetPtrVec<Muon>("muonsPFlow");
    auto taus = event->GetPtrVec<Tau>("taus");

    bool do_full = false;

    auto elecs_loose_filtered_et = copy_keep_if(elecs, [&](Electron const* e) {
      return
        e->pt()                     > 24.     &&
        std::fabs(e->eta())         < 2.1     &&
        std::fabs(e->dxy_vertex())  < 0.045   &&
        std::fabs(e->dz_vertex())   < 0.2;
    });

    ic::erase_if(elecs_loose_filtered_et, [&](Electron const* e) {
      return !(
        e->pt() > 24. &&
        std::fabs(e->eta()) < 2.1 &&
        std::fabs(e->dxy_vertex()) < 0.045 &&
        std::fabs(e->dz_vertex()) < 0.2 &&
        ElectronHTTId(e, false) && (do_full ?
        PF04IsolationVal(e, 0.5) < 0.1 : true)
        );
    });

    auto taus_loose_filtered_et = taus;
    ic::erase_if(taus_loose_filtered_et, [&](Tau const* t) {
      return !(
        t->pt() > 20. &&
        std::fabs(t->eta()) < 2.3 &&
        std::fabs(t->lead_dz_vertex()) < 0.2 &&
        t->decay_mode() >= 0 && (do_full ? (t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5
        && passAntiEMVA(t, 0) && t->GetTauID("againstMuonLoose") > 0.5) : true)
        );
    });

    std::sort(elecs_loose_filtered_et.begin(), elecs_loose_filtered_et.end(),
              [](Electron const* e1, Electron const* e2) {
      return PF04IsolationVal(e1, 0.5) <= PF04IsolationVal(e2, 0.5);
    });
    // std::sort(elecs_loose_filtered_et.begin(), elecs_loose_filtered_et.end(),
    //           [](Electron const* e1, Electron const* e2) {
    //   return e1->pt() > e2->pt();
    // });

    std::sort(taus_loose_filtered_et.begin(), taus_loose_filtered_et.end(),
              [](Tau const* t1, Tau const* t2) {
      return t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") <=
             t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    });
    // std::sort(taus_loose_filtered_et.begin(), taus_loose_filtered_et.end(),
    //           [](Tau const* t1, Tau const* t2) {
    //   return t1->pt() > t2->pt();
    // });

    auto const& gevt = event->Get<GenEvent_XToTauTau>("genEvent_XToTauTau");

    bool is_et =
        (gevt.tau_0.leptonic_mode == 11 && gevt.tau_1.hadronic_mode >= 0) ||
        (gevt.tau_1.leptonic_mode == 11 && gevt.tau_0.hadronic_mode >= 0);

    std::vector<GenJet *> tau_jets;
    tau_jets.push_back(gevt.tau_0.vis_jet);
    tau_jets.push_back(gevt.tau_1.vis_jet);

    if (elecs_loose_filtered_et.size() > 0 &&
        taus_loose_filtered_et.size() > 0 &&
        is_et) {
      ++et_loose_pairs;
      // See how many we could get right
      std::vector<GenJet *> gen_e = {gevt.tau_0.leptonic_mode == 11 ? gevt.tau_0.vis_jet : gevt.tau_1.vis_jet};
      std::vector<GenJet *> gen_t = {gevt.tau_0.hadronic_mode >= 0 ? gevt.tau_0.vis_jet : gevt.tau_1.vis_jet};
      auto all_e_matches = MatchByDR(gen_e, elecs_loose_filtered_et, 0.5, true, true);
      auto all_t_matches = MatchByDR(gen_t, taus_loose_filtered_et, 0.5, true, true);
      bool could_match = false;
      if (all_e_matches.size() > 0 && all_t_matches.size() > 0) {
        if (DR(all_e_matches[0].second, all_t_matches[0].second) > 0.5) {
          could_match = true;
        }
      }
      if (could_match) ++et_loose_pairs_could;
      auto sel_e = elecs_loose_filtered_et[0];
      ic::erase_if(taus_loose_filtered_et,
                   [&](Tau const* t) { return !(DR(t, sel_e) > 0.5);});
      if (taus_loose_filtered_et.size() == 0) return 0;
      auto sel_t = taus_loose_filtered_et[0];
      auto one_e_match = MatchByDR(gen_e, std::vector<Candidate*>{sel_e}, 0.5, true, true);
      auto one_t_match = MatchByDR(gen_t, std::vector<Candidate*>{sel_t}, 0.5, true, true);
      bool matched = one_e_match.size() > 0 && one_t_match.size() > 0;
      if (matched) ++ et_loose_matched;

      bool e_would_pass = PF04IsolationVal(sel_e, 0.5) < 0.1;
      bool t_would_pass = sel_t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5
        && passAntiEMVA(sel_t, 0) && sel_t->GetTauID("againstMuonLoose") > 0.5;
      if (e_would_pass && t_would_pass && matched) ++et_loose_passed;
    }



    return 0;
  }
  int HTTPairStudy::PostAnalysis() {
    std::cout << "et_loose_pairs: " << et_loose_pairs << "\n";
    std::cout << "et_loose_pairs_could: " << et_loose_pairs_could << "\n";
    std::cout << "et_loose_matched: " << et_loose_matched << "\n";
    std::cout << "et_loose_passed (+matched): " << et_loose_passed << "\n";
    return 0;
  }

  void HTTPairStudy::PrintInfo() {}
}
