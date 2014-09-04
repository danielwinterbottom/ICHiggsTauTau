#include "HiggsTauTau/interface/HTTGenEvent.h"

#include <string>
#include <set>
#include <vector>
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

int HTTGenEvent::PostAnalysis() {
  return 0;
}


int HTTGenEvent::Execute(TreeEvent *event) {
  auto const& parts = event->GetPtrVec<GenParticle>(genparticle_label_);


  std::set<int> higgs_pdg = {25 /*h*/, 35 /*H*/, 36 /*A*/};
  auto higgs_list = parts;
  ic::erase_if(higgs_list, [&](GenParticle * p) {
    return !(
      p->status() == 3 &&
      higgs_pdg.count(std::abs(p->pdgid())));
  });

  GenEvent_XToTauTau gen_event;

  if (higgs_list.size() == 1) {
    gen_event.boson = higgs_list[0];
    auto daughters = ic::ExtractDaughters(gen_event.boson, parts);
    if (daughters.size() == 2) {
      if (std::abs(daughters[0]->pdgid()) == 15 &&
          std::abs(daughters[1]->pdgid()) == 15) {
        gen_event.tau_0 = BuildTauInfo(daughters[0], parts);
        gen_event.tau_1 = BuildTauInfo(daughters[1], parts);
      }
    }
  } else {
    auto list = parts;
    ic::erase_if(list, [&](GenParticle * p) {
      return !(
        p->status() == 3 && std::abs(p->pdgid()) == 15);
    });
    if (list.size() == 2) {
      gen_event.tau_0 = BuildTauInfo(list[0], parts);
      gen_event.tau_1 = BuildTauInfo(list[1], parts);
    }
  }

  event->Add("genEvent_XToTauTau", gen_event);
  return 0;
}

GenEvent_Tau HTTGenEvent::BuildTauInfo(
    GenParticle *tau, std::vector<GenParticle *> const &parts) {
  GenEvent_Tau info;
  if (tau->status() == 3) {
    info.tau_st3 = tau;
    auto daughters = ExtractDaughters(tau, parts);
    if (daughters.size() != 1) {
      throw std::runtime_error(
          "[HTTGenEvent::BuildTauInfo] Status 3 tau does not have exactly one "
          "daughter");
    }
    if (daughters[0]->status() == 2 && std::abs(daughters[0]->pdgid()) == 15) {
      info.tau_st2_pre_fsr = daughters[0];
    } else {
      throw std::runtime_error(
          "[HTTGenEvent::BuildTauInfo] Daughter of status 3 tau is not a "
          "status 2 tau");
    }
  } else if (tau->status() == 2) {
    info.tau_st2_pre_fsr = tau;
  }

  // Now we do the FSR extraction:
  //  1) Add all photons in daughters to the FSR collection
  //  2) If there is another tau daughter, get its daughters and repeat
  //  3) Stop when there is no tau daughter
  //  4) By the end, fsr_scan should contain the actual tau decay
  GenParticle *tau_st2_post_fsr = info.tau_st2_pre_fsr;
  bool extracted_fsr = false;
  auto fsr_scan = ExtractDaughters(info.tau_st2_pre_fsr, parts);
  while (!extracted_fsr) {
    bool has_fsr = false;
    // does this list of daughters have another tau in?
    for (auto const& t : fsr_scan) {
      if (std::abs(t->pdgid()) == 15) {
        has_fsr = true;
        tau_st2_post_fsr = t;
      }
      if (t->pdgid() == 22) {
        info.fsr.push_back(t);
      }
    }
    if (has_fsr) {
      fsr_scan = ExtractDaughters(tau_st2_post_fsr, parts);
    } else {
      extracted_fsr = true;
    }
  }
  info.tau_st2_post_fsr = tau_st2_post_fsr;

  auto post_fsr_dts = ExtractDaughters(tau_st2_post_fsr, parts);
  for (auto const& t : post_fsr_dts) {
    if (std::abs(t->pdgid()) == 16) info.tau_nu = t;
    if (std::abs(t->pdgid()) == 11 || std::abs(t->pdgid()) == 13) {
      info.lep = t;
      info.leptonic_mode = std::abs(t->pdgid());
    }
    if (std::abs(t->pdgid()) == 12 || std::abs(t->pdgid()) == 14)
      info.lep_nu = t;
  }
  ic::erase_if(post_fsr_dts, [](GenParticle *p) {
    return std::abs(p->pdgid()) == 16 || p->pdgid() == 22;
  });
  if (!info.lep) {  // this is a hadronic decay
    // 211 = pi+
    // 213 = rho+
    // 20213 = a1+
    // 321 = K+
    // 323 = K*(892)+
    if (post_fsr_dts.size() == 0 || post_fsr_dts.size() > 1) {
      std::cout << "Hadronic tau - odd number of particles:\n";
      for (auto const& t : post_fsr_dts) t->Print();
    } else {
      info.had = post_fsr_dts[0];
      int pdg = std::abs(info.had->pdgid());
      if (pdg == 211 || pdg == 321) {
        info.pi_charged.push_back(info.had);
      }
      if (!(pdg == 211 || pdg == 321 || pdg == 213 || pdg == 20213 ||
            pdg == 323 || pdg == 24)) {
        std::cout << "Hadronic tau - odd decay:\n";
        for (auto const& t : post_fsr_dts) t->Print();
      }
    }
  }

  // KL0 130 (stable)
  // KS0 310 (stable)
  // eta 221 (unstable)
  // photon 22 (stable)
  if (info.had) {
    auto had_decay = ExtractDaughters(info.had, parts);
    for (auto const& t : had_decay) {
      if (std::abs(t->pdgid()) == 211 || std::abs(t->pdgid()) == 321) {
        info.pi_charged.push_back(t);
      } else if (std::abs(t->pdgid()) == 111) {
        info.pi_neutral.push_back(t);
      } else if (t->pdgid() == 130 || t->pdgid() == 310) {
        info.other_neutral.push_back(t);
      } else if (t->pdgid() == 22) {
        info.fsr.push_back(t);
      } else if (t->pdgid() == 221) {
        auto eta_decay = ExtractDaughters(t, parts);
        if (eta_decay.size() == 2 &&
            eta_decay[0]->pdgid() == 22 &&
            eta_decay[1]->pdgid() == 22) {
          info.pi_neutral.push_back(t);
        } else {
          info.other_neutral.push_back(t);
        }
      } else {
        std::cout << "Odd had decay:\n";
        t->Print();
      }
    }
    if (info.pi_charged.size() == 1) {
      if (info.other_neutral.size() == 0 && info.pi_neutral.size() <= 3) {
        info.hadronic_mode = info.pi_neutral.size();
      } else {
        info.hadronic_mode = 4;
      }
    } else if (info.pi_charged.size() == 2) {
      if (info.other_neutral.size() == 0 && info.pi_neutral.size() <= 3) {
        info.hadronic_mode = 5 + info.pi_neutral.size();
      } else {
        info.hadronic_mode = 9;
      }
    } else if (info.pi_charged.size() == 3) {
      if (info.other_neutral.size() == 0 && info.pi_neutral.size() <= 3) {
        info.hadronic_mode = 10 + info.pi_neutral.size();
      } else {
        info.hadronic_mode = 14;
      }
    } else {
      info.hadronic_mode = 15;
    }
  }

  return info;
}
}
