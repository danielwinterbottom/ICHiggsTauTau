#include <iostream>
#include "Zbb/interface/ZbbPairSelector.h"
#include "boost/bind.hpp"
#include "boost/range/algorithm_ext/push_back.hpp"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "Utilities/interface/FnPredicates.h"

namespace ic {
  ZbbPairSelector::ZbbPairSelector(std::string const& name) :
      ModuleBase(name) {
    elec_pairs_ = "electron_pairs";
    muon_pairs_ = "muon_pairs";
    output_label_ = "lepton_pair";
    n_ee = 0;
    n_mm = 0;
  }

  ZbbPairSelector::~ZbbPairSelector() { }

  int ZbbPairSelector::PreAnalysis() {
    return 0;
  }

  int ZbbPairSelector::Execute(TreeEvent *event) {
    // Get electron and muon pair collections
    auto const& elec_pair_vec =
      event->GetPtrVec<CompositeCandidate>(elec_pairs_);
    auto const& muon_pair_vec =
      event->GetPtrVec<CompositeCandidate>(muon_pairs_);
    // Combine them into a single vector
    std::vector<CompositeCandidate *> lepton_pair;
    boost::range::push_back(lepton_pair, elec_pair_vec);
    boost::range::push_back(lepton_pair, muon_pair_vec);
    // If this vector is empty there is no lepton pair
    // in this event, so we can stop processing
    if (lepton_pair.size() == 0) return 1;
    // Sort by smallest mass difference to the Z mass
    std::sort(lepton_pair.begin(), lepton_pair.end(),
        boost::bind(MassDiffCompare, _1, _2, 91.19));
    // Can now remove all other lepton pairs
    lepton_pair.erase(lepton_pair.begin()+1, lepton_pair.end());
    // Figure out if the selected pair is ee or mm,
    // and write both the vector and channel into the event
    std::string channel = "ee";
    if (std::find(muon_pair_vec.begin(), muon_pair_vec.end(), lepton_pair[0])
      != muon_pair_vec.end()) {
      channel = "mm";
      ++n_mm;
    } else {
      ++n_ee;
    }
    event->Add(output_label_, lepton_pair);
    event->Add("channel", channel);
    return 0;
  }

  int ZbbPairSelector::PostAnalysis() {
    std::cout << "ee pairs: " << n_ee << std::endl;
    std::cout << "mm pairs: " << n_mm << std::endl;
    return 0;
  }

  void ZbbPairSelector::PrintInfo() { }
}
