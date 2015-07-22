#include "HiggsTauTau/interface/TheoryWeights.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "Objects/interface/Muon.hh"
#include "Objects/interface/CompositeCandidate.hh"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTPairStudy.h"

namespace ic {

TheoryWeights::TheoryWeights(std::string const& name) : ModuleBase(name) {
  fs_ = nullptr;
}

TheoryWeights::~TheoryWeights() { ; }

int TheoryWeights::PreAnalysis() {
  TH1::SetDefaultSumw2(true);
  using ROOT::Math::Pi;
  if (!fs_) return 0;
  TFileDirectory dir = fs_->mkdir(this->ModuleName());
  h_m_ll    = dir.make<TH1F>("m_ll",  "m_ll",   20,   60,   120);
  hpdf_m_ll = PDFSetHist(h_m_ll, &dir, 2001, 2100, "NNPDF3");
  hscale_m_ll = PDFSetHist(h_m_ll, &dir, 1002, 1009, "SCALE");
  h_pt_ll   = dir.make<TH1F>("pt_ll", "pt_ll", 20,    0,   200);
  hpdf_pt_ll = PDFSetHist(h_pt_ll, &dir, 2001, 2100, "NNPDF3");
  hscale_pt_ll = PDFSetHist(h_pt_ll, &dir, 1002, 1009, "SCALE");
  h_n_jets  = dir.make<TH1F>("n_jets", "n_jets", 5, -0.5,   4.5);
  hpdf_n_jets = PDFSetHist(h_n_jets, &dir, 2001, 2100, "NNPDF3");
  hscale_n_jets = PDFSetHist(h_n_jets, &dir, 1002, 1009, "SCALE");
  h_pt_1    = dir.make<TH1F>("pt_1",  "pt_1",   50,    0,   100);
  h_pt_2    = dir.make<TH1F>("pt_2",  "pt_2",   50,    0,   100);
  h_eta_1   = dir.make<TH1F>("eta_1", "eta_1",  30,   -3,     3);
  h_eta_2   = dir.make<TH1F>("eta_2", "eta_2",  30,   -3,     3);
  return 0;
}

int TheoryWeights::Execute(TreeEvent* event) {
  // Get muons
  auto muons = event->GetPtrVec<Muon>("muons");
  
  // Do kinematics, IP, ID and Iso
  erase_if_not(muons, [](Muon const* mu) {
    return MinPtMaxEta(mu, 20., 2.5) && MuonMedium(mu) &&
           std::fabs(mu->dxy_vertex()) < 0.045 &&
           std::fabs(mu->dz_vertex()) < 0.2 && PF03IsolationVal(mu, 0.5, false);
  });

  // Build all possible pairs
  auto pairs = CompositeProducer(event, muons, muons, "muonPairs");

  // Require OS and 60 < mll < 120 GeV
  erase_if_not(pairs, [](CompositeCandidate* pair) {
    return pair->charge() == 0 && pair->M() > 60. && pair->M() < 120.;
  });

  // Sort pairs by pT of cand1
  std::sort(pairs.begin(), pairs.end(),
            [](CompositeCandidate* p1, CompositeCandidate* p2) {
              return (p1->At(0)->pt() > p2->At(0)->pt());
            });

  // Can return if we have no remaining pairs
  if (pairs.size() == 0) return 0;

  // Take the first pair
  auto pair = pairs[0];

  // Get the jets
  auto jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
  erase_if_not(jets, [&](PFJet const* jet) {
    return MinPtMaxEta(jet, 30., 4.7) &&
           MinDRToCollection(jet, pair->AsVector(), 0.5);
  });

  auto info = event->GetPtr<EventInfo>("eventInfo");
  double wt = info->total_weight();

  if (fs_) {
    h_m_ll->Fill(pair->M(), wt);
    hpdf_m_ll.Fill(pair->M(), wt, info);
    hscale_m_ll.Fill(pair->M(), wt, info);
    h_pt_ll->Fill(pair->pt(), wt);
    hpdf_pt_ll.Fill(pair->pt(), wt, info);
    hscale_pt_ll.Fill(pair->pt(), wt, info);
    h_n_jets->Fill(jets.size(), wt);
    hpdf_n_jets.Fill(jets.size(), wt, info);
    hscale_n_jets.Fill(jets.size(), wt, info);
    h_pt_1->Fill(pair->At(0)->pt(), wt);
    h_pt_2->Fill(pair->At(1)->pt(), wt);
    h_eta_1->Fill(pair->At(0)->eta(), wt);
    h_eta_2->Fill(pair->At(1)->eta(), wt);
  }
  return 0;
}

int TheoryWeights::PostAnalysis() {
  hpdf_m_ll.PostProcess();
  hpdf_pt_ll.PostProcess();
  hpdf_n_jets.PostProcess();
  hscale_m_ll.PostProcessEnvelope();
  hscale_pt_ll.PostProcessEnvelope();
  hscale_n_jets.PostProcessEnvelope();
  return 0;
}

void TheoryWeights::PrintInfo() { ; }
}
