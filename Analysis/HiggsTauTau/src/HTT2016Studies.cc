#include <algorithm>
#include <map>
#include "RooRealVar.h"
#include "HiggsTauTau/interface/HTT2016Studies.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/SecondaryVertex.hh"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "Utilities/interface/FnRootTools.h"

namespace ic {

  ZmmTreeProducer::ZmmTreeProducer(std::string const& name)
      : ModuleBase(name) {
      fs_ = nullptr;
  }

  ZmmTreeProducer::~ZmmTreeProducer() {
    ;
  }

  int ZmmTreeProducer::PreAnalysis() {
    if (fs_) {
      outtree_ = fs_->make<TTree>("Zmm","Zmm");

      outtree_->Branch("wt",          &wt);
      outtree_->Branch("wt_pu",       &wt_pu);
      outtree_->Branch("wt_trg",      &wt_trg);
      outtree_->Branch("n_vtx",       &n_vtx);
      outtree_->Branch("os",          &os);
      outtree_->Branch("pt_1",        &pt_1);
      outtree_->Branch("eta_1",       &eta_1);
      outtree_->Branch("phi_1",       &phi_1);
      outtree_->Branch("iso_1",       &iso_1);
      outtree_->Branch("pt_2",        &pt_2);
      outtree_->Branch("eta_2",       &eta_2);
      outtree_->Branch("phi_2",       &phi_2);
      outtree_->Branch("iso_2",       &iso_2);
      outtree_->Branch("m_ll",        &m_ll);
      outtree_->Branch("pt_ll",       &pt_ll);
      outtree_->Branch("dr_ll",       &dr_ll);
      outtree_->Branch("trg_IsoMu22",       &trg_IsoMu22);
    }

    TFile f(sf_workspace_.c_str());
    ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_id_ratio")->functor(ws_->argSet("m_pt,m_eta")));
    fns_["m_iso_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_iso_ratio")->functor(ws_->argSet("m_pt,m_eta")));
    fns_["m_trg_data"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_trg_data")->functor(ws_->argSet("m_pt,m_eta")));
    return 0;
  }

  int ZmmTreeProducer::Execute(TreeEvent *event) {
    auto pairs = event->GetPtrVec<CompositeCandidate>("dimuon");
    auto info = event->GetPtr<EventInfo>("eventInfo");

    // Take the pair closest to the Z mass
    std::sort(pairs.begin(), pairs.end(), [=](CompositeCandidate *c1, CompositeCandidate *c2) {
      return std::fabs(c1->M() - 91.1876) < std::fabs(c2->M() - 91.1876);
    });
    auto const* pair = pairs.at(0);

    // Sort the pair by descending pT
    std::vector<Candidate *> leptons = pair->AsVector();
    std::sort(leptons.begin(), leptons.end(),
        bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    Muon const* lep_1 = dynamic_cast<Muon const*>(leptons[0]);
    Muon const* lep_2 = dynamic_cast<Muon const*>(leptons[1]);

    n_vtx = info->good_vertices();

    // Fill tree info
    os = (pair->charge() == 0);
    pt_1 = lep_1->pt();
    eta_1 = lep_1->eta();
    phi_1 = lep_1->phi();
    iso_1 = PF04IsolationVal(lep_1, 0.5, 0);

    pt_2 = lep_2->pt();
    eta_2 = lep_2->eta();
    phi_2 = lep_2->phi();
    iso_2 = PF04IsolationVal(lep_2, 0.5, 0);

    m_ll = pair->M();
    pt_ll = pair->pt();
    dr_ll = ROOT::Math::VectorUtil::DeltaR(lep_1->vector(), lep_2->vector());

    trg_IsoMu22 = false;
    if (info->is_data()) {
      bool path_fired = false;
      auto const& trg_paths = event->GetPtrVec<TriggerPath>("triggerPaths");
      for (auto trg : trg_paths) {
        if (trg->id() == CityHash64("HLT_IsoMu22_v") && trg->accept()) {
          path_fired = true;
          break;
        }
      }
      bool obj_matched = false;
      if (path_fired) {
        std::string filter = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
        auto const& trg_objs = event->GetPtrVec<TriggerObject>("triggerObjects_IsoMu22");
        obj_matched = IsFilterMatched(lep_1, trg_objs, filter, 0.5) ||
                      IsFilterMatched(lep_2, trg_objs, filter, 0.5);
      }
      trg_IsoMu22 = path_fired && obj_matched;
    } else {
      trg_IsoMu22 = true;
    }
    wt_id = 1.0;
    wt_iso = 1.0;
    wt_trg = 1.0;
    auto args_1 = std::vector<double>{lep_1->pt(), lep_1->eta()};
    auto args_2 = std::vector<double>{lep_2->pt(), lep_2->eta()};
    if (!info->is_data()) {
      float eff_1 = fns_["m_trg_data"]->eval(args_1.data());
      float eff_2 = fns_["m_trg_data"]->eval(args_2.data());
      wt_trg = eff_1 + eff_2 - eff_1 * eff_2;
      wt_id = fns_["m_id_ratio"]->eval(args_1.data()) *
              fns_["m_id_ratio"]->eval(args_2.data());
      wt_iso = fns_["m_iso_ratio"]->eval(args_1.data()) *
               fns_["m_iso_ratio"]->eval(args_2.data());
    }
    info->set_weight("trg", wt_trg);
    info->set_weight("id", wt_id);
    info->set_weight("iso", wt_iso);
    wt = info->total_weight();
    wt_pu = info->weight_defined("pileup") ? info->weight("pileup") : 1.0;

    outtree_->Fill();
    return 0;
  }

  int ZmmTreeProducer::PostAnalysis() {
    return 0;
  }

  void ZmmTreeProducer::PrintInfo() { }



  ZmmTPTreeProducer::ZmmTPTreeProducer(std::string const& name)
      : ModuleBase(name) {
      fs_ = nullptr;
  }

  ZmmTPTreeProducer::~ZmmTPTreeProducer() {
    ;
  }

  int ZmmTPTreeProducer::PreAnalysis() {
    if (fs_) {
      outtree_ = fs_->make<TTree>("ZmmTP","ZmmTP");

      outtree_->Branch("wt",          &wt);
      outtree_->Branch("n_vtx",       &n_vtx);
      outtree_->Branch("pt_t",        &pt_t);
      outtree_->Branch("eta_t",       &eta_t);
      outtree_->Branch("phi_t",       &phi_t);
      outtree_->Branch("id_t",        &id_t);
      outtree_->Branch("iso_t",       &iso_t);
      outtree_->Branch("pt_p",        &pt_p);
      outtree_->Branch("eta_p",       &eta_p);
      outtree_->Branch("phi_p",       &phi_p);
      outtree_->Branch("id_p",        &id_p);
      outtree_->Branch("iso_p",       &iso_p);
      outtree_->Branch("m_ll",        &m_ll);
      outtree_->Branch("trg_t_IsoMu22",       &trg_t_IsoMu22);
      outtree_->Branch("trg_p_IsoMu22",       &trg_p_IsoMu22);
    }

    TFile f(sf_workspace_.c_str());
    ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    return 0;
  }

  int ZmmTPTreeProducer::Execute(TreeEvent *event) {
    auto pairs = event->GetPtrVec<CompositeCandidate>("dimuon");
    auto info = event->GetPtr<EventInfo>("eventInfo");

    n_vtx = info->good_vertices();
    wt = info->total_weight();

    for (auto pair : pairs) {
      Muon const* mu_t = dynamic_cast<Muon const*>(pair->At(0));
      Muon const* mu_p = dynamic_cast<Muon const*>(pair->At(1));
      pt_t = mu_t->pt();
      eta_t = mu_t->eta();
      phi_t = mu_t->phi();
      id_t = MuonMediumHIPsafe(mu_t) && fabs(mu_t->dxy_vertex()) < 0.045 && fabs(mu_t->dz_vertex()) < 0.2;
      iso_t = PF04IsolationVal(mu_t, 0.5, 0);
      pt_p = mu_p->pt();
      eta_p = mu_p->eta();
      phi_p = mu_p->phi();
      id_p = MuonMediumHIPsafe(mu_p) && fabs(mu_p->dxy_vertex()) < 0.045 && fabs(mu_p->dz_vertex()) < 0.2;
      iso_p = PF04IsolationVal(mu_p, 0.5, 0);
      m_ll = pair->M();

      trg_t_IsoMu22 = false;
      trg_p_IsoMu22 = false;
      if (info->is_data()) {
        bool path_fired = false;
        auto const& trg_paths = event->GetPtrVec<TriggerPath>("triggerPaths");
        for (auto trg : trg_paths) {
          if (trg->id() == CityHash64("HLT_IsoMu22_v") && trg->accept()) {
            path_fired = true;
            break;
          }
        }
        bool obj_t_matched = false;
        bool obj_p_matched = false;
        if (path_fired) {
          std::string filter = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
          auto const& trg_objs = event->GetPtrVec<TriggerObject>("triggerObjects_IsoMu22");
          obj_t_matched = IsFilterMatched(mu_t, trg_objs, filter, 0.5);
          obj_p_matched = IsFilterMatched(mu_p, trg_objs, filter, 0.5);
        }
        trg_t_IsoMu22 = path_fired && obj_t_matched;
        trg_p_IsoMu22 = path_fired && obj_p_matched;
      } else {
        trg_t_IsoMu22 = true;
        trg_p_IsoMu22 = true;
      }
      outtree_->Fill();
    }


    return 0;
  }

  int ZmmTPTreeProducer::PostAnalysis() {
    return 0;
  }

  void ZmmTPTreeProducer::PrintInfo() { }
}
