#include "HiggsTauTau/interface/HTTGenEvent.h"

#include <string>
#include <set>
// #include "boost/format.hpp"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

namespace ic {

HTTGenEvent::HTTGenEvent(std::string const &name) : ModuleBase(name) {}

HTTGenEvent::~HTTGenEvent() { ; }

int HTTGenEvent::PreAnalysis() {
  std::cout << "-------------------------------------\n";
  std::cout << "HTTGenEvent" << std::endl;
  std::cout << "-------------------------------------\n";
  return 0;
}

int HTTGenEvent::Execute(TreeEvent *event) {
  auto const& parts = event->GetPtrVec<GenParticle>(genparticle_label_);

  // std::map<int, GenParticle const*> pmap;
  // for (auto const& p : parts) pmap[p->index()] = p;

  // First see if this is a (H->tau tau event)
  //  - exactly one boson (h/H/A) status 3
  //  - two daughter taus

  std::set<int> higgs_pdg = {25 /*h*/, 35 /*H*/, 36 /*A*/};
  auto higgs_list = parts;
  ic::erase_if(higgs_list, [&](GenParticle * p) {
    return !(
      p->status() == 3 &&
      higgs_pdg.count(std::abs(p->pdgid())));
  });

  std::vector<GenParticle *> taus_gen;

  if (higgs_list.size() == 1) {
    ic::GenParticle *higgs = higgs_list[0];
    auto daughters = ic::ExtractDaughters(higgs, parts);
    if (daughters.size() == 2) {
      if (std::abs(daughters[0]->pdgid()) == 15 &&
          std::abs(daughters[1]->pdgid()) == 15) {
        taus_gen.push_back(daughters[0]);
        taus_gen.push_back(daughters[1]);
      }
    } else {
    }
  } else {
  }

  if (taus_gen.size() == 2) {
    auto t0_daughters = ExtractDaughters(ExtractDaughters(taus_gen[0], parts)[0], parts);
    //std::cout << "t0:\n";
    //for (auto t : t0_daughters) t->Print();
    // auto t1_daughters = ExtractStableDaughters(taus_gen[1], parts);
    // std::cout << "T1:\n";
    // for (auto t : t1_daughters) t->Print();
  }
  return 0;
}
}
