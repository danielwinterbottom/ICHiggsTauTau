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
      outtree_->Branch("wt_trk",      &wt_trk);
      outtree_->Branch("wt_id",       &wt_id);
      outtree_->Branch("wt_iso",      &wt_iso);
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
      outtree_->Branch("trg_IsoTkMu22",     &trg_IsoTkMu22);
    }

    TFile f(sf_workspace_.c_str());
    ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_trk_ratio")->functor(ws_->argSet("m_eta")));
    fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_id_ratio")->functor(ws_->argSet("m_pt,m_eta")));
    fns_["m_iso_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_iso_ratio")->functor(ws_->argSet("m_pt,m_eta")));
    fns_["m_trg_data"] = std::shared_ptr<RooFunctor>(
      ws_->function("m_trg_data")->functor(ws_->argSet("m_pt,m_eta")));
    return 0;
  }

  int ZmmTreeProducer::Execute(TreeEvent *event) {
    typedef std::vector<double> v_double;
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
    trg_IsoTkMu22 = false;

    if (info->is_data()) {
      bool path_fired_IsoMu22 = HLTPathCheck(event, "triggerPaths", "HLT_IsoMu22_v");
      bool path_fired_IsoTkMu22 = HLTPathCheck(event, "triggerPaths", "HLT_IsoTkMu22_v");

      std::string filter_IsoMu22 = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
      auto const& trg_objs_IsoMu22 = event->GetPtrVec<TriggerObject>("triggerObjects_IsoMu22");
      trg_IsoMu22 = path_fired_IsoMu22 &&
        (IsFilterMatched(lep_1, trg_objs_IsoMu22, filter_IsoMu22, 0.5) ||
         IsFilterMatched(lep_2, trg_objs_IsoMu22, filter_IsoMu22, 0.5));

      std::string filter_IsoTkMu22 = "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
      auto const& trg_objs_IsoTkMu22 = event->GetPtrVec<TriggerObject>("triggerObjects_IsoTkMu22");
      trg_IsoTkMu22 = path_fired_IsoTkMu22 &&
        (IsFilterMatched(lep_1, trg_objs_IsoTkMu22, filter_IsoTkMu22, 0.5) ||
         IsFilterMatched(lep_2, trg_objs_IsoTkMu22, filter_IsoTkMu22, 0.5));
    } else {
      trg_IsoMu22 = true;
      trg_IsoTkMu22 = true;
    }

    wt_trk = 1.0;
    wt_id = 1.0;
    wt_iso = 1.0;
    wt_trg = 1.0;
    auto args_1 = std::vector<double>{lep_1->pt(), lep_1->eta()};
    auto args_2 = std::vector<double>{lep_2->pt(), lep_2->eta()};
    if (!info->is_data()) {
      float eff_1 = fns_["m_trg_data"]->eval(args_1.data());
      float eff_2 = fns_["m_trg_data"]->eval(args_2.data());
      wt_trg = eff_1 + eff_2 - eff_1 * eff_2;
      wt_trk = fns_["m_trk_ratio"]->eval(v_double{lep_1->eta()}.data()) *
              fns_["m_trk_ratio"]->eval(v_double{lep_2->eta()}.data());
      wt_id = fns_["m_id_ratio"]->eval(args_1.data()) *
              fns_["m_id_ratio"]->eval(args_2.data());
      wt_iso = fns_["m_iso_ratio"]->eval(args_1.data()) *
               fns_["m_iso_ratio"]->eval(args_2.data());
    }
    info->set_weight("trk", wt_trk);
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


  ZeeTreeProducer::ZeeTreeProducer(std::string const& name)
      : ModuleBase(name) {
      fs_ = nullptr;
  }

  ZeeTreeProducer::~ZeeTreeProducer() {
    ;
  }

  int ZeeTreeProducer::PreAnalysis() {
    if (fs_) {
      outtree_ = fs_->make<TTree>("Zee","Zee");

      outtree_->Branch("wt",          &wt);
      outtree_->Branch("wt_pu",       &wt_pu);
      outtree_->Branch("wt_trk",      &wt_trk);
      outtree_->Branch("wt_id",       &wt_id);
      outtree_->Branch("wt_iso",      &wt_iso);
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
      outtree_->Branch("trg_Ele25eta2p1WPTight",       &trg_Ele25eta2p1WPTight);
    }

    TFile f(sf_workspace_.c_str());
    ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    fns_["e_trk_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("e_trk_ratio")->functor(ws_->argSet("e_pt,e_eta")));
    fns_["e_id_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("e_id_ratio")->functor(ws_->argSet("e_pt,e_eta")));
    fns_["e_iso_ratio"] = std::shared_ptr<RooFunctor>(
      ws_->function("e_iso_ratio")->functor(ws_->argSet("e_pt,e_eta")));
    fns_["e_trg_data"] = std::shared_ptr<RooFunctor>(
      ws_->function("e_trg_data")->functor(ws_->argSet("e_pt,e_eta")));
    f.Close();
    return 0;
  }

  int ZeeTreeProducer::Execute(TreeEvent *event) {
    auto pairs = event->GetPtrVec<CompositeCandidate>("dielec");
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

    Electron const* lep_1 = dynamic_cast<Electron const*>(leptons[0]);
    Electron const* lep_2 = dynamic_cast<Electron const*>(leptons[1]);

    n_vtx = info->good_vertices();

    // Fill tree info
    os = (pair->charge() == 0);
    pt_1 = lep_1->pt();
    eta_1 = lep_1->eta();
    phi_1 = lep_1->phi();
    iso_1 = PF03IsolationVal(lep_1, 0.5, 0);

    pt_2 = lep_2->pt();
    eta_2 = lep_2->eta();
    phi_2 = lep_2->phi();
    iso_2 = PF03IsolationVal(lep_2, 0.5, 0);

    m_ll = pair->M();
    pt_ll = pair->pt();
    dr_ll = ROOT::Math::VectorUtil::DeltaR(lep_1->vector(), lep_2->vector());

    trg_Ele25eta2p1WPTight = false;

    if (info->is_data()) {
      bool path_fired_Ele25eta2p1WPTight = HLTPathCheck(event, "triggerPaths", "HLT_Ele25_eta2p1_WPTight_Gsf_v");

      std::string filter_Ele25eta2p1WPTight = "hltEle25erWPTightGsfTrackIsoFilter";
      auto const& trg_objs_Ele25eta2p1WPTight = event->GetPtrVec<TriggerObject>("triggerObjects_Ele25_eta2p1_WPTight_Gsf");
      trg_Ele25eta2p1WPTight = path_fired_Ele25eta2p1WPTight &&
        (IsFilterMatched(lep_1, trg_objs_Ele25eta2p1WPTight, filter_Ele25eta2p1WPTight, 0.5) ||
         IsFilterMatched(lep_2, trg_objs_Ele25eta2p1WPTight, filter_Ele25eta2p1WPTight, 0.5));
    } else {
      trg_Ele25eta2p1WPTight = true;
    }

    wt_trk = 1.0;
    wt_id = 1.0;
    wt_iso = 1.0;
    wt_trg = 1.0;
    auto args_1 = std::vector<double>{lep_1->pt(), lep_1->sc_eta()};
    auto args_2 = std::vector<double>{lep_2->pt(), lep_2->sc_eta()};
    if (!info->is_data()) {
      float eff_1 = fns_["e_trg_data"]->eval(args_1.data());
      float eff_2 = fns_["e_trg_data"]->eval(args_2.data());
      wt_trg = eff_1 + eff_2 - eff_1 * eff_2;
      wt_trk = fns_["e_trk_ratio"]->eval(args_1.data()) *
               fns_["e_trk_ratio"]->eval(args_2.data());
      wt_id = fns_["e_id_ratio"]->eval(args_1.data()) *
              fns_["e_id_ratio"]->eval(args_2.data());
      wt_iso = fns_["e_iso_ratio"]->eval(args_1.data()) *
               fns_["e_iso_ratio"]->eval(args_2.data());
    }
    info->set_weight("trk", wt_trk);
    info->set_weight("trg", wt_trg);
    info->set_weight("id", wt_id);
    info->set_weight("iso", wt_iso);
    wt = info->total_weight();
    wt_pu = info->weight_defined("pileup") ? info->weight("pileup") : 1.0;

    outtree_->Fill();
    return 0;
  }

  int ZeeTreeProducer::PostAnalysis() {
    return 0;
  }

  void ZeeTreeProducer::PrintInfo() { }



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
      outtree_->Branch("run",         &run);
      outtree_->Branch("pt_t",        &pt_t);
      outtree_->Branch("eta_t",       &eta_t);
      outtree_->Branch("phi_t",       &phi_t);
      outtree_->Branch("id_t",        &id_t);
      outtree_->Branch("iso_t",       &iso_t);
      outtree_->Branch("muon_p",      &muon_p);
      outtree_->Branch("pt_p",        &pt_p);
      outtree_->Branch("eta_p",       &eta_p);
      outtree_->Branch("id_p",        &id_p);
      outtree_->Branch("iso_p",       &iso_p);
      outtree_->Branch("m_ll",        &m_ll);
      outtree_->Branch("trg_t_IsoMu22",       &trg_t_IsoMu22);
      outtree_->Branch("trg_t_IsoMu19Tau",    &trg_t_IsoMu19Tau);
      outtree_->Branch("trg_p_IsoMu22",       &trg_p_IsoMu22);
      outtree_->Branch("trg_p_IsoTkMu22",     &trg_p_IsoTkMu22);
      outtree_->Branch("trg_p_PFTau120",      &trg_p_PFTau120);
      outtree_->Branch("trg_p_IsoMu19TauL1",  &trg_p_IsoMu19TauL1);
      outtree_->Branch("trg_p_IsoMu19Tau",    &trg_p_IsoMu19Tau);
    }

    TFile f(sf_workspace_.c_str());
    ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    return 0;
  }

  int ZmmTPTreeProducer::Execute(TreeEvent *event) {
    auto pairs = event->GetPtrVec<CompositeCandidate>("dimuon");
    auto info = event->GetPtr<EventInfo>("eventInfo");
    run = info->run();

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
      trg_t_IsoMu19Tau = false;
      trg_p_IsoMu22 = false;
      trg_p_IsoTkMu22 = false;
      trg_p_PFTau120 = false;
      trg_p_IsoMu19TauL1 = false;
      trg_p_IsoMu19Tau = false;
      if (info->is_data()) {
        bool path_fired_IsoMu22 = HLTPathCheck(event, "triggerPaths", "HLT_IsoMu22_v");
        bool path_fired_IsoTkMu22 = HLTPathCheck(event, "triggerPaths", "HLT_IsoTkMu22_v");
        bool path_fired_PFTau120 = HLTPathCheck(event, "triggerPaths", "HLT_VLooseIsoPFTau120_Trk50_eta2p1_v");

        std::string filter_IsoMu22 = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
        auto const& trg_objs_IsoMu22 = event->GetPtrVec<TriggerObject>("triggerObjects_IsoMu22");

        trg_t_IsoMu22 = path_fired_IsoMu22 && IsFilterMatched(mu_t, trg_objs_IsoMu22, filter_IsoMu22, 0.5);
        trg_p_IsoMu22 = path_fired_IsoMu22 && IsFilterMatched(mu_p, trg_objs_IsoMu22, filter_IsoMu22, 0.5);

        std::string filter_IsoTkMu22 = "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
        auto const& trg_objs_IsoTkMu22 = event->GetPtrVec<TriggerObject>("triggerObjects_IsoTkMu22");
        trg_p_IsoTkMu22 = path_fired_IsoTkMu22 && IsFilterMatched(mu_p, trg_objs_IsoTkMu22, filter_IsoTkMu22, 0.5);

        std::string filter_PFTau120  = "hltPFTau120TrackPt50LooseAbsOrRelVLooseIso";
        auto const& trg_objs_PFTau120  = event->GetPtrVec<TriggerObject>("triggerObjects_VLooseIsoPFTau120_Trk50_eta2p1");
        trg_p_PFTau120 = path_fired_PFTau120 && IsFilterMatched(mu_p, trg_objs_PFTau120, filter_PFTau120, 0.5);

        std::string filter_IsoMu19TauL1  = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
        auto const& trg_objs_IsoMu19TauL1  = event->GetPtrVec<TriggerObject>("triggerObjects_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1");
        trg_p_IsoMu19TauL1 = IsFilterMatched(mu_p, trg_objs_IsoMu19TauL1, filter_IsoMu19TauL1, 0.5);

        // For the mu-tau L1 cross seed we have to take a bit more care. Need to check that something other than the probe muon
        std::string filter_IsoMu19Tau  = "hltL3crIsoL1sMu18erTauJet20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
        auto const& trg_objs_IsoMu19Tau  = event->GetPtrVec<TriggerObject>("triggerObjects_IsoMu19_eta2p1_LooseIsoPFTau20");
        trg_t_IsoMu19Tau = false;
        for (auto obj : trg_objs_IsoMu19Tau) {
          // First skip objects that don't come from the L1
          auto const& filters = obj->filters();
          if (std::find(filters.begin(), filters.end(),
                        CityHash64("hltL1sMu18erTau20er")) == filters.end()) {
            continue;
          }
          // Then skip L1 objects that aren't a tau
          auto types = GetTriggerTypes(obj);
          if (types.count(-100) == 0) continue;

          // Finally check if the object is separated from the probe muon
          if (DR(obj, mu_p) > 0.5) {
            trg_t_IsoMu19Tau = true;
            break;
          }
        }
        trg_p_IsoMu19Tau = trg_t_IsoMu19Tau && IsFilterMatched(mu_p, trg_objs_IsoMu19Tau, filter_IsoMu19Tau, 0.5);
      } else {
        trg_t_IsoMu22 = true;
        trg_t_IsoMu19Tau = true;
        trg_p_IsoMu22 = true;
        trg_p_IsoTkMu22 = true;
        trg_p_PFTau120 = true;
      }
      muon_p = true;
      outtree_->Fill();
    }

    auto tpairs = event->GetPtrVec<CompositeCandidate>("mutrk");
    auto probe_muons = event->GetPtrVec<Muon>("probe_muons");

    for (auto pair : tpairs) {
      Muon const* mu_t = dynamic_cast<Muon const*>(pair->At(0));
      Candidate const* mu_p = dynamic_cast<Candidate const*>(pair->At(1));
      pt_t = mu_t->pt();
      eta_t = mu_t->eta();
      phi_t = mu_t->phi();
      id_t = MuonMediumHIPsafe(mu_t) && fabs(mu_t->dxy_vertex()) < 0.045 &&
             fabs(mu_t->dz_vertex()) < 0.2;
      iso_t = PF04IsolationVal(mu_t, 0.5, 0);
      pt_p = mu_p->pt();
      eta_p = mu_p->eta();
      phi_p = mu_p->phi();
      auto mu_matches = MatchByDR(std::vector<ic::Candidate const*>{mu_p},
                                  probe_muons, 0.3, true, true);
      id_p = false;
      if (mu_matches.size() > 0) {
        auto match_mu = dynamic_cast<ic::Muon *>(mu_matches[0].second);
        if (MuonMediumHIPsafe(match_mu) &&
            fabs(match_mu->dxy_vertex()) < 0.045 &&
            fabs(match_mu->dz_vertex()) < 0.2) {
          id_p = true;
        }
      }
      iso_p = false;
      m_ll = pair->M();

      trg_t_IsoMu22 = false;
      trg_t_IsoMu19Tau = false;
      trg_p_IsoMu22 = false;
      trg_p_IsoTkMu22 = false;
      trg_p_PFTau120 = false;
      trg_p_IsoMu19TauL1 = false;
      trg_p_IsoMu19Tau = false;
      muon_p = false;
      outtree_->Fill();
    }
    return 0;
  }

  int ZmmTPTreeProducer::PostAnalysis() {
    return 0;
  }

  void ZmmTPTreeProducer::PrintInfo() { }


  ZeeTPTreeProducer::ZeeTPTreeProducer(std::string const& name)
      : ModuleBase(name) {
    fs_ = nullptr;
  }

  ZeeTPTreeProducer::~ZeeTPTreeProducer() {
    ;
  }

  int ZeeTPTreeProducer::PreAnalysis() {
    if (fs_) {
      outtree_ = fs_->make<TTree>("ZeeTP","ZeeTP");

      outtree_->Branch("wt",          &wt);
      outtree_->Branch("n_vtx",       &n_vtx);
      outtree_->Branch("pt_t",        &pt_t);
      outtree_->Branch("eta_t",       &eta_t);
      outtree_->Branch("phi_t",       &phi_t);
      outtree_->Branch("id_t",        &id_t);
      outtree_->Branch("iso_t",       &iso_t);
      outtree_->Branch("pt_p",        &pt_p);
      outtree_->Branch("eta_p",       &eta_p);
      outtree_->Branch("sc_eta_p",    &sc_eta_p);
      outtree_->Branch("phi_p",       &phi_p);
      outtree_->Branch("id_p",        &id_p);
      outtree_->Branch("iso_p",       &iso_p);
      outtree_->Branch("m_ll",        &m_ll);
      outtree_->Branch("trg_t_Ele25eta2p1WPTight",       &trg_t_Ele25eta2p1WPTight);
      outtree_->Branch("trg_p_Ele25eta2p1WPTight",       &trg_p_Ele25eta2p1WPTight);
    }

    TFile f(sf_workspace_.c_str());
    ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    return 0;
  }

  int ZeeTPTreeProducer::Execute(TreeEvent *event) {
    auto pairs = event->GetPtrVec<CompositeCandidate>("dielec");
    auto info = event->GetPtr<EventInfo>("eventInfo");

    n_vtx = info->good_vertices();
    wt = info->total_weight();

    for (auto pair : pairs) {
      Electron const* el_t = dynamic_cast<Electron const*>(pair->At(0));
      Electron const* el_p = dynamic_cast<Electron const*>(pair->At(1));
      pt_t = el_t->pt();
      eta_t = el_t->eta();
      phi_t = el_t->phi();
      id_t = ElectronHTTIdSpring15(el_t, false) && fabs(el_t->dxy_vertex()) < 0.045 && fabs(el_t->dz_vertex()) < 0.2;
      iso_t = PF03IsolationVal(el_t, 0.5, 0);
      pt_p = el_p->pt();
      eta_p = el_p->eta();
      sc_eta_p = el_p->sc_eta();
      phi_p = el_p->phi();
      id_p = ElectronHTTIdSpring15(el_p, false) && fabs(el_p->dxy_vertex()) < 0.045 && fabs(el_p->dz_vertex()) < 0.2;
      iso_p = PF03IsolationVal(el_p, 0.5, 0);
      m_ll = pair->M();

      trg_t_Ele25eta2p1WPTight = false;
      trg_p_Ele25eta2p1WPTight = false;
      if (info->is_data()) {
        bool path_fired_Ele25eta2p1WPTight = HLTPathCheck(event, "triggerPaths", "HLT_Ele25_eta2p1_WPTight_Gsf_v");

        std::string filter_Ele25eta2p1WPTight = "hltEle25erWPTightGsfTrackIsoFilter";
        auto const& trg_objs_Ele25eta2p1WPTight = event->GetPtrVec<TriggerObject>("triggerObjects_Ele25_eta2p1_WPTight_Gsf");

        trg_t_Ele25eta2p1WPTight = path_fired_Ele25eta2p1WPTight && IsFilterMatched(el_t, trg_objs_Ele25eta2p1WPTight, filter_Ele25eta2p1WPTight, 0.5);
        trg_p_Ele25eta2p1WPTight = path_fired_Ele25eta2p1WPTight && IsFilterMatched(el_p, trg_objs_Ele25eta2p1WPTight, filter_Ele25eta2p1WPTight, 0.5);
      } else {
        trg_t_Ele25eta2p1WPTight = true;
        trg_p_Ele25eta2p1WPTight = true;
      }
      outtree_->Fill();
    }

    return 0;
  }

  int ZeeTPTreeProducer::PostAnalysis() {
    return 0;
  }

  void ZeeTPTreeProducer::PrintInfo() { }

  bool HLTPathCheck(ic::TreeEvent* event, std::string const& label,
                    std::string const& path) {
    bool path_fired = false;
    auto const& trg_paths = event->GetPtrVec<TriggerPath>(label);
    for (auto trg : trg_paths) {
      if (trg->id() == CityHash64(path) && trg->accept()) {
        path_fired = true;
        break;
      }
    }
    return path_fired;
  }

  std::set<int16_t> GetTriggerTypes(ic::TriggerObject* obj) {
    std::set<int16_t> types;
    ui64 packed_type;
    packed_type.one = obj->id();

    for (unsigned i = 0; i < 4; ++i) {
      int16_t type = packed_type.four[i];
      if (type != 0) types.insert(type);
    }
    return types;
  }
}
