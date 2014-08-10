#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/adaptor/uniqued.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>
#include "TFile.h"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"

int main() {
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  /*
  AnalysisBase analysis(json);

  TH1F jet_pt("jet_pt", "jet_pt", 100, 0, 100);

  auto module = QuickModule("QuickModule")
      .SetMethod([&](Event *event) {
    auto const& jets = event->GetPtrVec<PFJet>(json["jets"]);
    if (jets.size() > 0) jet_pt.Fill(jets[0]->pt());
    return 0;
  });

  std::vector<ic::Module *> sequence1;
  sequence1.push_back(&module1);
  ...
  std::vector<ic::Module *> sequence2;
  sequence2.push_back(&module1);
  ...

  analysis.AddModuleSequence("mt", sequence_mt);
  analysis.AddModuleSequence("mt_tscale_hi", sequence_mt_tscale_hi);
  analysis.AddModuleSequence("mt_tscale_lo", sequence_mt_tscale_lo);

  analysis.RunAnalysis();

  How to fill weighted yield table?

  Just optional?

  class EventCounterBase(unsigned n_modules) {
    virtual beginEvent(ic::Event const* evt) {
      info_ = evt->GetPtr<EventInfo>("eventInfo");
    }
    virtual double Increment(unsigned return_code) {
      if (return_code == 0) return info_->total_weight();
    }
  }
  */

  ic::TreeEvent t;
  TFile f("/Volumes/HDD/June6/MC_53X/VBF_HToTauTau_M-125/EventTree_1.root");
  TTree *tree = dynamic_cast<TTree*>(f.Get("icEventProducer/EventTree"));
  t.SetTree(tree);
  tree->SetCacheSize(100000000);
  tree->SetCacheLearnEntries(100);
  for (unsigned i = 0; i < tree->GetEntries(); ++i) {
    tree->LoadTree(i);
    if (i % 10000 == 0) std::cout << i << "\n";
    t.SetEvent(i);
    auto const& muons = t.GetPtrVec<ic::Muon>("muonsPFlow");
    if (muons.size() == 0) continue;
    muons[0]->Candidate::Print();
    muons[0]->set_pt(5.0);
    muons[0]->Candidate::Print();
    auto const& muons_id = t.GetIDMap<ic::Muon>("muons_id_map", "muonsPFlow");
    muons[0]->Candidate::Print();
    t.SetEvent(i);
    auto const& muons2 = t.GetPtrVec<ic::Muon>("muonsPFlow");
    muons[0]->Candidate::Print();
    // auto const& jets = t.GetPtrVec<ic::PFJet>("pfJetsPFlow");
    // auto const& electrons = t.GetPtrVec<ic::Electron>("electrons");
    // t.GetIDMap<ic::Electron>("electrons_id_map", "electrons");
    // t.DeepCopyPtrVec<ic::Electron>("electrons", "new_electrons",
    // "electrons");
    // t.List();
    // std::cout << t.ExistsInTree("electrons") << "\n";
    // std::cout << t.ExistsInEvent("electrons") << "\n";
    // std::cout << t.ExistsInEvent("electron_id_map") << "\n";
  }
  tree->PrintCacheStats();
  return 0;
}
