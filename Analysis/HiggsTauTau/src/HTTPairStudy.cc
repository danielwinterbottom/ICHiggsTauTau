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

HTTPairStudy::HTTPairStudy(std::string const& name)
    : ModuleBase(name), channel_(ic::channel::et), do_loose_(true) {
      loose_pairs = 0;
      selected_pairs = 0;
      selected_matched_pairs = 0;
      selected_anymatched_pairs = 0;
    }

  HTTPairStudy::~HTTPairStudy() {
    ;
  }

  int HTTPairStudy::PreAnalysis() {
    return 0;
  }

  int HTTPairStudy::Execute(TreeEvent *event) {
    ic::channel true_channel = ic::channel::et;

    std::vector<GenJet *> gen_0;
    std::vector<GenJet *> gen_1;
    auto const& gevt = event->Get<GenEvent_XToTauTau>("genEvent_XToTauTau");
    std::vector<GenJet *> gen_both = {gevt.tau_0.vis_jet, gevt.tau_1.vis_jet};

    if (gevt.tau_0.leptonic_mode == 11 && gevt.tau_1.hadronic_mode >= 0) {
      true_channel = ic::channel::et;
      gen_0 = {gevt.tau_0.vis_jet};
      gen_1 = {gevt.tau_1.vis_jet};
    }
    if (gevt.tau_1.leptonic_mode == 11 && gevt.tau_0.hadronic_mode >= 0) {
      true_channel = ic::channel::et;
      gen_0 = {gevt.tau_1.vis_jet};
      gen_1 = {gevt.tau_0.vis_jet};
    }
    if (gevt.tau_0.leptonic_mode == 13 && gevt.tau_1.hadronic_mode >= 0) {
      true_channel = ic::channel::mt;
      gen_0 = {gevt.tau_0.vis_jet};
      gen_1 = {gevt.tau_1.vis_jet};
    }
    if (gevt.tau_1.leptonic_mode == 13 && gevt.tau_0.hadronic_mode >= 0) {
      true_channel = ic::channel::mt;
      gen_0 = {gevt.tau_1.vis_jet};
      gen_1 = {gevt.tau_0.vis_jet};
    }
    if (gevt.tau_0.leptonic_mode == 11 && gevt.tau_1.leptonic_mode == 13) {
      true_channel = ic::channel::em;
      gen_0 = {gevt.tau_0.vis_jet};
      gen_1 = {gevt.tau_1.vis_jet};
    }
    if (gevt.tau_1.leptonic_mode == 11 && gevt.tau_0.leptonic_mode == 13) {
      true_channel = ic::channel::em;
      gen_0 = {gevt.tau_1.vis_jet};
      gen_1 = {gevt.tau_0.vis_jet};
    }

    if (channel_ == channel::et) {
      std::vector<Electron const*> sel_ele;
      std::vector<Tau const*> sel_tau;
      ic::CompositeCandidate* ditau = nullptr;
      auto elecs = event->GetPtrVec<Electron>("electrons");
      auto taus = event->GetPtrVec<Tau>("taus");
      if (do_loose_) {
        auto ele_loose = copy_keep_if(elecs, ETLooseEle);
        auto tau_loose = copy_keep_if(taus, ETLooseTau);
        auto et_pairs =
            CompositeProducer(event, ele_loose, tau_loose, "ditau_loose_et");
        ic::keep_if(et_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        std::sort(et_pairs.begin(), et_pairs.end(), SuperSortET);
        if (et_pairs.size() == 0) return 0;
        ditau = et_pairs[0];
        sel_ele = {static_cast<Electron const*>(ditau->At(0))};
        sel_tau = {static_cast<Tau const*>(ditau->At(1))};
        ++loose_pairs;
        ic::keep_if(sel_ele, ETTightEle);
        ic::keep_if(sel_tau, ETTightTau);
        if (sel_ele.size() == 0 || sel_tau.size() == 0) return 0;
      } else {
        auto ele_loose = copy_keep_if(elecs, ETTightEle);
        auto tau_loose = copy_keep_if(taus, ETTightTau);
        auto et_pairs =
            CompositeProducer(event, ele_loose, tau_loose, "ditau_tight_et");
        ic::keep_if(et_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        std::sort(et_pairs.begin(), et_pairs.end(), SortByOSThenSumPt);
        if (et_pairs.size() == 0) return 0;
        ditau = et_pairs[0];
        sel_ele = {static_cast<Electron const*>(ditau->At(0))};
        sel_tau = {static_cast<Tau const*>(ditau->At(1))};
      }
      ++selected_pairs;
      auto e_matches = MatchByDR(gen_0, sel_ele, 0.5, true, true);
      auto t_matches = MatchByDR(gen_1, sel_tau, 0.5, true, true);
      auto any_matches = MatchByDR(gen_both, ditau->AsVector(), 0.5, true, true);
      if (any_matches.size() >= 2) ++selected_anymatched_pairs;
      if (e_matches.size() > 0 && t_matches.size() > 0 &&
          true_channel == channel::et)
        ++selected_matched_pairs;
    }

    if (channel_ == channel::mt) {
      std::vector<Muon const*> sel_muo;
      std::vector<Tau const*> sel_tau;
      ic::CompositeCandidate* ditau = nullptr;
      auto muons = event->GetPtrVec<Muon>("muonsPFlow");
      auto taus = event->GetPtrVec<Tau>("taus");
      if (do_loose_) {
        auto muo_loose = copy_keep_if(muons, MTLooseMuo);
        auto tau_loose = copy_keep_if(taus, MTLooseTau);
        auto mt_pairs =
            CompositeProducer(event, muo_loose, tau_loose, "ditau_loose_mt");
        ic::keep_if(mt_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        std::sort(mt_pairs.begin(), mt_pairs.end(), SuperSortET);
        if (mt_pairs.size() == 0) return 0;
        ditau = mt_pairs[0];
        sel_muo = {static_cast<Muon const*>(ditau->At(0))};
        sel_tau = {static_cast<Tau const*>(ditau->At(1))};
        ++loose_pairs;
        ic::keep_if(sel_muo, MTTightMuo);
        ic::keep_if(sel_tau, MTTightTau);
        if (sel_muo.size() == 0 || sel_tau.size() == 0) return 0;
      } else {
        auto muo_loose = copy_keep_if(muons, MTTightMuo);
        auto tau_loose = copy_keep_if(taus, MTTightTau);
        auto mt_pairs =
            CompositeProducer(event, muo_loose, tau_loose, "ditau_tight_mt");
        ic::keep_if(mt_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        std::sort(mt_pairs.begin(), mt_pairs.end(), SortByOSThenSumPt);
        if (mt_pairs.size() == 0) return 0;
        ditau = mt_pairs[0];
        sel_muo = {static_cast<Muon const*>(ditau->At(0))};
        sel_tau = {static_cast<Tau const*>(ditau->At(1))};
      }
      ++selected_pairs;
      auto m_matches = MatchByDR(gen_0, sel_muo, 0.5, true, true);
      auto t_matches = MatchByDR(gen_1, sel_tau, 0.5, true, true);
      auto any_matches = MatchByDR(gen_both, ditau->AsVector(), 0.5, true, true);
      if (any_matches.size() >= 2) ++selected_anymatched_pairs;
      if (m_matches.size() > 0 && t_matches.size() > 0 &&
          true_channel == channel::mt)
        ++selected_matched_pairs;
    }

    return 0;
  }
  int HTTPairStudy::PostAnalysis() {
    std::cout << "*** " << this->ModuleName() << "\n";
    std::cout << "loose:               " << loose_pairs << "\n";
    std::cout << "selected:            " << selected_pairs << "\n";
    std::cout << "selected_matched:    " << selected_matched_pairs << "\n";
    std::cout << "selected_anymatched: " << selected_anymatched_pairs << "\n";
    return 0;
  }

  void HTTPairStudy::PrintInfo() {}

  // ET selection
  // ----------------------------------------------------------------
  bool ETLooseEle(Electron const* e) {
    return e->pt() > 24. && std::fabs(e->eta()) < 2.1 &&
           std::fabs(e->dxy_vertex()) < 0.045 &&
           std::fabs(e->dz_vertex()) < 0.2 && ElectronHTTId(e, false);
  }

  bool ETTightEle(Electron const* e) {
    return ETLooseEle(e) && PF04IsolationVal(e, 0.5) < 0.1;
  }

  bool ETLooseTau(Tau const* t) {
    return t->pt() > 20. && std::fabs(t->eta()) < 2.3 &&
           std::fabs(t->lead_dz_vertex()) < 0.2 &&
           t->GetTauID("decayModeFinding") > 0.5;
  }

  bool ETTightTau(Tau const* t) {
    return ETLooseTau(t) &&
           t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5 &&
           passAntiEMVA(t, 0) && t->GetTauID("againstMuonLoose") > 0.5;
  }

  // MT selection
  // ----------------------------------------------------------------
  bool MTLooseMuo(Muon const* m) {
    return m->pt() > 20. && std::fabs(m->eta()) < 2.1 &&
           std::fabs(m->dxy_vertex()) < 0.045 &&
           std::fabs(m->dz_vertex()) < 0.2 && MuonTight(m);
  }

  bool MTTightMuo(Muon const* m) {
    return MTLooseMuo(m) && PF04IsolationVal(m, 0.5) < 0.1;
  }

  bool MTLooseTau(Tau const* t) {
    return t->pt() > 20. && std::fabs(t->eta()) < 2.3 &&
           std::fabs(t->lead_dz_vertex()) < 0.2 &&
           t->GetTauID("decayModeFinding") > 0.5;
  }

  bool MTTightTau(Tau const* t) {
    return MTLooseTau(t) &&
           t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5 &&
           t->GetTauID("againstElectronLoose") &&
           t->GetTauID("againstMuonTight") > 0.5;
  }

  // Sorting
  // ----------------------------------------------------------------
  bool SortByIsoThenPtE(Electron const* e1, Electron const* e2) {
    double iso1 = PF04IsolationVal(e1, 0.5);
    double iso2 = PF04IsolationVal(e2, 0.5);
    if (iso1 == iso2) return e1->pt() > e2->pt();
    return iso1 < iso2;
  }

  bool SortByIsoThenPtM(Muon const* m1, Muon const* m2) {
    double iso1 = PF04IsolationVal(m1, 0.5);
    double iso2 = PF04IsolationVal(m2, 0.5);
    if (iso1 == iso2) return m1->pt() > m2->pt();
    return iso1 < iso2;
  }
  bool SortByIsoThenPtT(Tau const* t1, Tau const* t2) {
    double iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    if (iso1 == iso2) return t1->pt() > t2->pt();
    return iso1 < iso2;
  }

  bool SortByOSThenSumPt(CompositeCandidate const* c1,
                                       CompositeCandidate const* c2) {
    if (c1->charge() == 0 && c2->charge() != 0) return true;
    if (c1->charge() != 0 && c2->charge() == 0) return false;
    return ScalarPtSum(c1->AsVector()) > ScalarPtSum(c2->AsVector());
  }

  bool SuperSortET(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the electrons
    Electron const* e1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2 = static_cast<Electron const*>(c2->At(0));
    double e_iso1 = PF04IsolationVal(e1, 0.5);
    double e_iso2 = PF04IsolationVal(e2, 0.5);
    // If the iso is different we just use this
    if (e_iso1 != e_iso2) return e_iso1 < e_iso2;
    // If not try the pT
    if (e1->pt() != e2->pt()) return e1->pt() > e2->pt();
    // If both of these are the same then try the taus
    Tau const* t1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2 = static_cast<Tau const*>(c2->At(1));
    double t_iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t_iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    if (t_iso1 != t_iso2) return t_iso1 < t_iso2;
    return (t1->pt() > t2->pt());
  }

  bool SuperSortMT(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the electrons
    Muon const* m1 = static_cast<Muon const*>(c1->At(0));
    Muon const* m2 = static_cast<Muon const*>(c2->At(0));
    double m_iso1 = PF04IsolationVal(m1, 0.5);
    double m_iso2 = PF04IsolationVal(m2, 0.5);
    // If the iso is different we just use this
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    // If not try the pT
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();
    // If both of these are the same then try the taus
    Tau const* t1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2 = static_cast<Tau const*>(c2->At(1));
    double t_iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t_iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    if (t_iso1 != t_iso2) return t_iso1 < t_iso2;
    return (t1->pt() > t2->pt());
  }

  }
