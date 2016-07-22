#include <algorithm>
#include <map>
#include "HiggsTauTau/interface/HTT2016Studies.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/SecondaryVertex.hh"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

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
    return 0;
  }

  int ZmmTreeProducer::Execute(TreeEvent *event) {
    auto pairs = event->GetPtrVec<CompositeCandidate>("dimuon");
    auto const* info = event->GetPtr<EventInfo>("eventInfo");

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

    wt = info->total_weight();
    wt_pu = info->weight_defined("pileup") ? info->weight("pileup") : 1.0;

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
    if (event->ExistsInTree("triggerPaths")) {
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


    outtree_->Fill();
    return 0;
  }

  int ZmmTreeProducer::PostAnalysis() {
    return 0;
  }

  void ZmmTreeProducer::PrintInfo() { }
}
