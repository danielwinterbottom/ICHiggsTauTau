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

int main() {
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  ic::TreeEvent t;
  TFile f("/Volumes/HDD/June6/MC_53X/VBF_HToTauTau_M-125/EventTree_1.root");
  TTree *tree = dynamic_cast<TTree*>(f.Get("icEventProducer/EventTree"));
  t.SetTree(tree);
  for (unsigned i = 0; i < tree->GetEntries(); ++i) {
    if (i % 10000 == 0) std::cout << i << "\n";
    t.SetEvent(i);
    auto const& electrons =
        t.GetPtrVec<ic::Electron>("electrons");
        t.GetIDMap<ic::Electron>("electrons_id_map", "electrons");
        // t.DeepCopyPtrVec<ic::Electron>("electrons", "new_electrons", "electrons");
    // t.List();
    // std::cout << t.ExistsInTree("electrons") << "\n";
    // std::cout << t.ExistsInEvent("electrons") << "\n";
    // std::cout << t.ExistsInEvent("electron_id_map") << "\n";
  }
  return 0;
}
