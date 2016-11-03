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
    : ModuleBase(name),
      channel_(ic::channel::et),
      do_loose_(true),
      do_gen_evt_(true),
      fs_(nullptr),
      trig_filter_("HTTTriggerFilter") {
  loose_pairs = 0;
  selected_pairs = 0;
  selected_matched_pairs = 0;
  selected_anymatched_pairs = 0;
  trig_filter_
    .set_mc(ic::mc::spring16_80X)
    .set_is_data(false)
    .set_is_embedded(false);
}

  HTTPairStudy::~HTTPairStudy() {
    ;
  }

  int HTTPairStudy::PreAnalysis() {
    if (fs_) {
      TFileDirectory subdir = fs_->mkdir(this->ModuleName());
      h_mtt_vis = subdir.make<TH1F>("mtt_vis", "", 70, 0, 350);
    }
    return 0;
  }

  int HTTPairStudy::Execute(TreeEvent *event) {
    ic::channel true_channel = ic::channel::et;

    std::vector<GenJet *> gen_0;
    std::vector<GenJet *> gen_1;
    std::vector<GenJet *> gen_both;
    if (do_gen_evt_) {
      auto const& gevt = event->Get<GenEvent_XToTauTau>("genEvent_XToTauTau");
      gen_both = {gevt.tau_0.vis_jet, gevt.tau_1.vis_jet};

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
      if (gevt.tau_0.hadronic_mode >= 0 && gevt.tau_1.hadronic_mode >= 0) {
        true_channel = ic::channel::tt;
        gen_0 = {gevt.tau_0.vis_jet, gevt.tau_1.vis_jet};
      }
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
        auto & et_pairs =
            CompositeProducer(event, ele_loose, tau_loose, "ditau_loose_et");
        ic::keep_if(et_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        trig_filter_
          .set_pair_label("ditau_loose_et")
          .set_channel(ic::channel::et)
          .Execute(event);
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
        auto & et_pairs =
            CompositeProducer(event, ele_loose, tau_loose, "ditau_tight_et");
        ic::keep_if(et_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        trig_filter_
          .set_pair_label("ditau_tight_et")
          .set_channel(ic::channel::et)
          .Execute(event);
        std::sort(et_pairs.begin(), et_pairs.end(), SortByOSThenSumPt);
        if (et_pairs.size() == 0) return 0;
        ditau = et_pairs[0];
        sel_ele = {static_cast<Electron const*>(ditau->At(0))};
        sel_tau = {static_cast<Tau const*>(ditau->At(1))};
      }
      // Di-electron veto
      if (event->GetPtrVec<CompositeCandidate>("eleVetoPairs").size() > 0)
        return 0;
      // Extra-electron veto
      if (event->GetPtrVec<Electron>("extraElectrons").size() > 1) return 0;
      // Extra-muon veto
      if (event->GetPtrVec<Muon>("extraMuons").size() > 0) return 0;
      ++selected_pairs;
      if (!do_gen_evt_) return 0;
      h_mtt_vis->Fill(ditau->M());
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
        auto & mt_pairs =
            CompositeProducer(event, muo_loose, tau_loose, "ditau_loose_mt");
        ic::keep_if(mt_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        trig_filter_
          .set_pair_label("ditau_loose_mt")
          .set_channel(ic::channel::mt)
          .Execute(event);
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
        auto & mt_pairs =
            CompositeProducer(event, muo_loose, tau_loose, "ditau_tight_mt");
        ic::keep_if(mt_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        trig_filter_
          .set_pair_label("ditau_tight_mt")
          .set_channel(ic::channel::mt)
          .Execute(event);
        std::sort(mt_pairs.begin(), mt_pairs.end(), SortByOSThenSumPt);
        if (mt_pairs.size() == 0) return 0;
        ditau = mt_pairs[0];
        sel_muo = {static_cast<Muon const*>(ditau->At(0))};
        sel_tau = {static_cast<Tau const*>(ditau->At(1))};
      }
      // Di-muon veto
      if (event->GetPtrVec<CompositeCandidate>("muoVetoPairs").size() > 0)
        return 0;
      // Extra-electron veto
      if (event->GetPtrVec<Electron>("extraElectrons").size() > 0) return 0;
      // Extra-muon veto
      if (event->GetPtrVec<Muon>("extraMuons").size() > 1) return 0;
      ++selected_pairs;
      if (!do_gen_evt_) return 0;
      h_mtt_vis->Fill(ditau->M());
      auto m_matches = MatchByDR(gen_0, sel_muo, 0.5, true, true);
      auto t_matches = MatchByDR(gen_1, sel_tau, 0.5, true, true);
      auto any_matches = MatchByDR(gen_both, ditau->AsVector(), 0.5, true, true);
      if (any_matches.size() >= 2) ++selected_anymatched_pairs;
      if (m_matches.size() > 0 && t_matches.size() > 0 &&
          true_channel == channel::mt)
        ++selected_matched_pairs;
    }

    if (channel_ == channel::em) {
      std::vector<Electron const*> sel_ele;
      std::vector<Muon const*> sel_muo;
      ic::CompositeCandidate* ditau = nullptr;
      auto elecs = event->GetPtrVec<Electron>("electrons");
      std::vector<Electron> new_elecs(elecs.size());
      for (unsigned i = 0; i < elecs.size(); ++i) {
        new_elecs[i] = *(elecs[i]);
        elecs[i] = &(new_elecs[i]);
        Electron * ele = elecs[i];
        if (ele->HasIdIso("trackInIsoSum")) {
          float current_iso = ele->dr04_pfiso_charged_all();
          float sub_iso = ele->GetIdIso("trackInIsoSum");
          if (current_iso >= sub_iso) {
            ele->set_dr04_pfiso_charged_all(current_iso - sub_iso);
          }
        }
      }
      auto muons = event->GetPtrVec<Muon>("muonsPFlow");
      // Some extra "legacy" BS for e-mu
      ic::keep_if(elecs, [&](Electron const* e) {
        return ic::MinDRToCollection(e, muons, 0.3) &&
                       e->HasIdIso("matchedRecoMuon")
                   ? e->GetIdIso("matchedRecoMuon") < 0.5
                   : true;
      });
      if (do_loose_) {
        auto ele_loose = copy_keep_if(elecs, EMLooseEle);
        auto muo_loose = copy_keep_if(muons, EMLooseMuo);
        auto & em_pairs =
            CompositeProducer(event, ele_loose, muo_loose, "ditau_loose_em");
        ic::keep_if(em_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.3 && PairOneWithPt(c, 20.);
        });
        trig_filter_
          .set_pair_label("ditau_loose_em")
          .set_channel(ic::channel::em)
          .Execute(event);
        std::sort(em_pairs.begin(), em_pairs.end(), SuperSortEM);
        if (em_pairs.size() == 0) return 0;
        ditau = em_pairs[0];
        sel_ele = {static_cast<Electron const*>(ditau->At(0))};
        sel_muo = {static_cast<Muon const*>(ditau->At(1))};
        ++loose_pairs;
        ic::keep_if(sel_ele, EMTightEle);
        ic::keep_if(sel_muo, EMTightMuo);
        if (sel_ele.size() == 0 || sel_muo.size() == 0) return 0;
      } else {
        auto ele_loose = copy_keep_if(elecs, EMTightEle);
        auto muo_loose = copy_keep_if(muons, EMTightMuo);
        auto & em_pairs =
            CompositeProducer(event, ele_loose, muo_loose, "ditau_tight_em");
        ic::keep_if(em_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.3 && PairOneWithPt(c, 20.);
        });
        trig_filter_
          .set_pair_label("ditau_tight_em")
          .set_channel(ic::channel::em)
          .Execute(event);
        std::sort(em_pairs.begin(), em_pairs.end(), SortByOSThenSumPt);
        if (em_pairs.size() == 0) return 0;
        ditau = em_pairs[0];
        sel_ele = {static_cast<Electron const*>(ditau->At(0))};
        sel_muo = {static_cast<Muon const*>(ditau->At(1))};
      }
      // Extra-electron veto
      if (event->GetPtrVec<Electron>("extraElectrons").size() > 1) return 0;
      // Extra-muon veto
      if (event->GetPtrVec<Muon>("extraMuons").size() > 1) return 0;
      ++selected_pairs;
      if (!do_gen_evt_) return 0;
      h_mtt_vis->Fill(ditau->M());
      auto e_matches = MatchByDR(gen_0, sel_ele, 0.5, true, true);
      auto m_matches = MatchByDR(gen_1, sel_muo, 0.5, true, true);
      auto any_matches = MatchByDR(gen_both, ditau->AsVector(), 0.5, true, true);
      if (any_matches.size() >= 2) ++selected_anymatched_pairs;
      if (e_matches.size() > 0 && m_matches.size() > 0 &&
          true_channel == channel::em)
        ++selected_matched_pairs;
    }

    if (channel_ == channel::tt) {
      std::vector<Tau const*> sel_tau0;
      std::vector<Tau const*> sel_tau1;
      ic::CompositeCandidate* ditau = nullptr;
      auto taus = event->GetPtrVec<Tau>("taus");
      if (do_loose_) {
        auto tau_loose = copy_keep_if(taus, TTLooseTau);
        auto & tt_pairs =
            CompositeProducer(event, tau_loose, tau_loose, "ditau_loose_tt");
        ic::keep_if(tt_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        // trig_filter_
        //   .set_pair_label("ditau_loose_tt")
        //   .set_channel(ic::channel::tt)
        //   .Execute(event);
        std::sort(tt_pairs.begin(), tt_pairs.end(), SuperSortTT);
        if (tt_pairs.size() == 0) return 0;
        ditau = tt_pairs[0];
        sel_tau0 = {static_cast<Tau const*>(ditau->At(0))};
        sel_tau1 = {static_cast<Tau const*>(ditau->At(1))};
        ++loose_pairs;
        ic::keep_if(sel_tau0, TTTightTau);
        ic::keep_if(sel_tau1, TTTightTau);
        if (sel_tau0.size() == 0 || sel_tau1.size() == 0) return 0;
      } else {
        auto tau_loose = copy_keep_if(taus, TTTightTau);
        auto & tt_pairs =
            CompositeProducer(event, tau_loose, tau_loose, "ditau_tight_tt");
        ic::keep_if(tt_pairs, [&](CompositeCandidate const* c) {
          return DR(c->At(0), c->At(1)) > 0.5;
        });
        // trig_filter_
        //   .set_pair_label("ditau_tight_tt")
        //   .set_channel(ic::channel::tt)
        //   .Execute(event);
        std::sort(tt_pairs.begin(), tt_pairs.end(), SuperSortTT);
        if (tt_pairs.size() == 0) return 0;
        ditau = tt_pairs[0];
        sel_tau0 = {static_cast<Tau const*>(ditau->At(0))};
        sel_tau1 = {static_cast<Tau const*>(ditau->At(1))};
      }
      // Extra-electron veto
      if (event->GetPtrVec<Electron>("extraElectrons").size() > 0) return 0;
      // Extra-muon veto
      if (event->GetPtrVec<Muon>("extraMuons").size() > 0) return 0;
      ++selected_pairs;
      if (!do_gen_evt_) return 0;
      h_mtt_vis->Fill(ditau->M());
      // Slightly different treatment here. For the tt channel we have put both
      // gen taus in the "gen_0" vector, so we can just do the matching in one
      // step
      auto t_matches = MatchByDR(gen_0, ditau->AsVector(), 0.5, true, true);
      auto any_matches = MatchByDR(gen_both, ditau->AsVector(), 0.5, true, true);
      if (any_matches.size() >= 2) ++selected_anymatched_pairs;
      if (t_matches.size() >= 2 &&
          true_channel == channel::tt)
        ++selected_matched_pairs;
    }

    /*
TauTau: againstMuonLoose on both legs. againstElectronLoose (highest pt tau) + againstElectronLoose+againstElectronLooseMVA3 (subleading pt tau)

    Loose Filter: pT 45, eta 2.1, decayModeFinding
    Tight Filter: loose + againstElectronLoose + againstMuonLoose
    Produce Pairs, filter DR > 0.5
    By construction all pairs at this stage should appear twice
    sort logic(iso1, pt1, iso2, pt2)


    */

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
           TauEoverP(t, 0.2) &&
           t->GetTauID("againstMuonTight") > 0.5;
  }

  // EM selection
  // ----------------------------------------------------------------
  bool EMLooseEle(Electron const* e) {
    return e->pt() > 10. && std::fabs(e->eta()) < 2.3 &&
           std::fabs(e->dxy_vertex()) < 0.02 &&
           std::fabs(e->dz_vertex()) < 0.1 && ElectronHTTId(e, true);
  }

  bool EMTightEle(Electron const* e) {
    return EMLooseEle(e) && PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
  }

  bool EMLooseMuo(Muon const* m) {
    return m->pt() > 10. && std::fabs(m->eta()) < 2.1 &&
           std::fabs(m->dxy_vertex()) < 0.02 &&
           std::fabs(m->dz_vertex()) < 0.1 && MuonTight(m);
  }

  bool EMTightMuo(Muon const* m) {
    return EMLooseMuo(m) && PF04IsolationEB(m, 0.5, 0.15, 0.1);
  }

  // TT selection
  // ----------------------------------------------------------------
  bool TTLooseTau(Tau const* t) {
    return t->pt() > 45. && std::fabs(t->eta()) < 2.1 &&
          std::fabs(t->lead_dz_vertex()) < 0.2 &&
          t->GetTauID("decayModeFinding") > 0.5;
  }

  bool TTTightTau(Tau const* t) {
    return TTLooseTau(t) &&
      t->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") > 0.5 &&
      t->GetTauID("againstMuonLoose") > 0.5 &&
      t->GetTauID("againstElectronLoose") > 0.5;
  }



  // Sorting
  // ----------------------------------------------------------------
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

  bool SuperSortEM(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the muons
    Muon const* m1 = static_cast<Muon const*>(c1->At(1));
    Muon const* m2 = static_cast<Muon const*>(c2->At(1));
    double m_iso1 = PF04IsolationVal(m1, 0.5);
    double m_iso2 = PF04IsolationVal(m2, 0.5);
    // If the iso is different we just use this
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    // If not try the pT
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();
    // If both of these are the same then try the electrons
    Electron const* e1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2 = static_cast<Electron const*>(c2->At(0));
    double e_iso1 = PF04IsolationVal(e1, 0.5);
    double e_iso2 = PF04IsolationVal(e2, 0.5);
    if (e_iso1 != e_iso2) return e_iso1 < e_iso2;
    return e1->pt() > e2->pt();
  }

  bool SuperSortTT(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the electrons
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(0));
    Tau const* t1_2 = static_cast<Tau const*>(c2->At(0));
    double t1_iso1 = t1_1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t1_iso2 = t1_2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    // If the iso is different we just use this
    if (t1_iso1 != t1_iso2) return t1_iso1 < t1_iso2;
    // If not try the pT
    if (t1_1->pt() != t1_2->pt()) return t1_1->pt() > t1_2->pt();
    // If both of these are the same then try the taus
    Tau const* t2_1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(1));
    double t2_iso1 = t2_1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t2_iso2 = t2_2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    // If the iso is different we just use this
    if (t2_iso1 != t2_iso2) return t2_iso1 < t2_iso2;
    // If not try the pT
    return t2_1->pt() > t2_2->pt();
  }

  }
