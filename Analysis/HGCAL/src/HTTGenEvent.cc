#include "HGCAL/interface/HTTGenEvent.h"

#include <string>
#include <set>
#include <vector>
// #include "boost/format.hpp"
#include "Objects/interface/PFJet.hh"
#include "Objects/interface/GenParticle.hh"
#include "Objects/interface/GenJet.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"

namespace ic {

HTTGenEvent::HTTGenEvent(std::string const &name) : ModuleBase(name) {
  is_pythia8_ = false;
}

HTTGenEvent::~HTTGenEvent() { ; }

int HTTGenEvent::PreAnalysis() {
  return 0;
}

int HTTGenEvent::PostAnalysis() {
  return 0;
}


int HTTGenEvent::Execute(TreeEvent *event) {
  auto const& parts = event->GetPtrVec<GenParticle>(genparticle_label_);


  // Consider either an SM (pdgid = 25) or MSSM (25, 35, 36) Higgs boson
  std::set<int> higgs_pdg = {25 /*h*/, 35 /*H*/, 36 /*A*/};

  // Copy the list of particles, then filter it, keeping only status 3 particles
  // with a Higgs pdgid
  auto higgs_list = parts;
  ic::erase_if(higgs_list, [&](GenParticle * p) {
    return !(
      // Can't use status == 3 anymore - pythia 8 will have different status codes
      // p->status() == 3 &&
      higgs_pdg.count(std::abs(p->pdgid())));
  });

  GenEvent_XToTauTau gen_event;

  // If we found exactly one Higgs boson we will look for tau daughters and fill
  // the full set of tau properties from these.
  // ==> Also can't expect just one here, so loop through and look for any H->tautau decay
  unsigned htt_decays = 0;
  for (unsigned i = 0; i < higgs_list.size(); ++i) {
    gen_event.boson = higgs_list[i];
    auto daughters = ic::ExtractDaughters(gen_event.boson, parts);
    // Sanity test - should only find two daughter particles
    if (daughters.size() == 2) {
      // Both daughters should be taus
      if (std::abs(daughters[0]->pdgid()) == 15 &&
          std::abs(daughters[1]->pdgid()) == 15) {
        ++htt_decays;
        gen_event.tau_0 = BuildTauInfo(daughters[0], parts, is_pythia8_);
        gen_event.tau_1 = BuildTauInfo(daughters[1], parts, is_pythia8_);
      }
    }
  }

  if (htt_decays > 1) {
    std::cout << "Found more than one htt decay!\n";
    return 1;
  }

  if (htt_decays == 0) {
    // If we didn't find a single Higgs boson, we will just check if
    // there are exactly two status 3 taus in the event, and use these
    // instead
    auto list = parts;
    ic::erase_if(list, [&](GenParticle * p) {
      return !(
        p->status() == 3 && std::abs(p->pdgid()) == 15);
    });
    if (list.size() == 2) {
      gen_event.tau_0 = BuildTauInfo(list[0], parts, is_pythia8_);
      gen_event.tau_1 = BuildTauInfo(list[1], parts, is_pythia8_);
    }
  }

  std::vector<GenJet> tau_h_jets;
  std::vector<TauInfo *> gtaus = {&gen_event.tau_0, &gen_event.tau_1};

  for (auto gtau : gtaus) {
    tau_h_jets.push_back(GenJet());
    ROOT::Math::PtEtaPhiEVector vec;
    std::vector<std::size_t> id_vec;
    for (unsigned k = 0; k < gtau->all_vis.size(); ++k) {
      if (abs(gtau->all_vis[k]->pdgid()) == 12 ||
          abs(gtau->all_vis[k]->pdgid()) == 14 ||
          abs(gtau->all_vis[k]->pdgid()) == 16)
        continue;
      vec += gtau->all_vis[k]->vector();
      tau_h_jets.back().set_charge(tau_h_jets.back().charge() +
                                   gtau->all_vis[k]->charge());
      id_vec.push_back(gtau->all_vis[k]->id());
    }
    tau_h_jets.back().set_vector(vec);
    tau_h_jets.back().set_constituents(id_vec);
  }

  event->Add("genEvent_XToTauTau_TauJets", tau_h_jets);
  auto &tau_h_jet_ptrs =
      event->Get<std::vector<GenJet>>("genEvent_XToTauTau_TauJets");
  gen_event.tau_0.vis_jet = &(tau_h_jet_ptrs[0]);
  gen_event.tau_1.vis_jet = &(tau_h_jet_ptrs[1]);

  event->Add("genEvent_XToTauTau", gen_event);
  return 0;
}

TauInfo HTTGenEvent::BuildTauInfo(
    GenParticle *tau, std::vector<GenParticle *> const &parts, bool is_pythia8) {
  TauInfo info;
  // We could have been given a status 3 or status 2 tau
  if (tau->status() == 3) {
    // If status 3, we'll record a pointer to this tau in the info object, then
    // check that there is exactly one daughter dau with status 2 (this seems to
    // be just where the tau is copied from the matrix element to tauola for
    // doing the decay)
    info.tau_st3 = tau;
    auto daughters = ExtractDaughters(tau, parts);
    if (daughters.size() != 1) {
      throw std::runtime_error(
          "[HTTGenEvent::BuildTauInfo] Status 3 tau does not have exactly one "
          "daughter");
    }
    if (daughters[0]->status() == 2 && std::abs(daughters[0]->pdgid()) == 15) {
      // Call this status 2 tau "pre_fsr": there may be some photon emissions to
      // collect before the tau actually decays
      info.tau_st2_pre_fsr = daughters[0];
    } else {
      throw std::runtime_error(
          "[HTTGenEvent::BuildTauInfo] Daughter of status 3 tau is not a "
          "status 2 tau");
    }
  } else {
    // We were given a status 2 instead - we'll just assume this is pre_fsr
    info.tau_st2_pre_fsr = tau;
  }

  // Now we do the FSR extraction:
  //  1) Add all photons in daughters to the FSR collection
  //  2) If there is another tau daughter, get its daughters and repeat
  //  3) Stop when there is no tau daughter
  //  4) By the end, fsr_scan should contain the actual tau decay

  // Start by assuming the pre_fsr tau is also the post_fsr tau
  GenParticle *tau_st2_post_fsr = info.tau_st2_pre_fsr;
  // But flag that we haven't actually extracted any fsr photons yet
  bool extracted_fsr = false;
  // Skip the FSR extraction if we're in pythia8 - we seem to decay
  // directly to pions
  // if (is_pythia8) extracted_fsr = true;
  // Start by getting a list of daughters
  auto fsr_scan = ExtractDaughters(info.tau_st2_pre_fsr, parts);
  if (!is_pythia8) {
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
  } else {
    while (!extracted_fsr) {
      bool has_fsr = false;
      // does this list of daughters have another tau in?
      for (auto const& t : fsr_scan) {
        if (std::abs(t->pdgid()) == 15) {
          has_fsr = true;
          tau_st2_post_fsr = t;
        }
      }
      if (has_fsr) {
        for (auto const& t : fsr_scan) {
          if (t->pdgid() == 22) {
            info.fsr.push_back(t);
          }
        }
        fsr_scan = ExtractDaughters(tau_st2_post_fsr, parts);
      } else {
        extracted_fsr = true;
      }
    }
  }
  info.tau_st2_post_fsr = tau_st2_post_fsr;

  info.all_vis = ExtractStableDaughters(tau_st2_post_fsr, parts);

  // Now get the daughters of the actual tau decay
  auto post_fsr_dts = ExtractDaughters(tau_st2_post_fsr, parts);
  for (auto const& t : post_fsr_dts) {
    // Should always find a neutrino
    if (std::abs(t->pdgid()) == 16) info.tau_nu = t;
    // e/mu imply a leptonic decay
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
      if (!is_pythia8) {
        std::cout << "Hadronic tau - odd number of particles:\n";
        for (auto const& t : post_fsr_dts) t->Print();
      } else {
        // No hadron to add, just put the tau back in
        info.had = tau_st2_post_fsr;
      }
    } else {
      info.had = post_fsr_dts[0];
      int pdg = std::abs(info.had->pdgid());
      if (pdg == 211 || pdg == 321) {
        info.pi_charged.push_back(info.had);
      }
      if (!(pdg == 211 || pdg == 321 || pdg == 213 || pdg == 20213 ||
            pdg == 323 || pdg == 24)) {
        std::cout << "Hadronic tau - odd decay:\n";
        for (auto const& t : post_fsr_dts) {
          t->Print();
        }
      }
    }
  }


  // KL0 130 (stable)
  // KS0 310 (stable)
  // eta 221 (unstable)
  // photon 22 (stable)
  if (info.had) {
    auto had_decay = ExtractDaughters(info.had, parts);
    if (is_pythia8) {
      ic::erase_if(had_decay, [](GenParticle *p) {
        return std::abs(p->pdgid()) == 16;
      });
    }
    for (auto const& t : had_decay) {
      if (std::abs(t->pdgid()) == 211 || std::abs(t->pdgid()) == 321) {
        // Charged pion or kaon
        info.pi_charged.push_back(t);
      } else if (std::abs(t->pdgid()) == 111) {
        // neutral pion
        info.pi_neutral.push_back(t);
      } else if (t->pdgid() == 130 || t->pdgid() == 310) {
        // Ks/Kl
        info.other_neutral.push_back(t);
      } else if (t->pdgid() == 22) {
        // Found a direct photon in the decay, but should we really add it to
        // the fsr?
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
      } else if (std::abs(t->pdgid()) == 311) {  // K0
        auto t_daughters = ExtractDaughters(t, parts);
        for (auto const& t_d : t_daughters) {
          if (t_d->pdgid() == 130 || t_d->pdgid() == 310) {
            info.other_neutral.push_back(t_d);
          } else {
            std::cout << "Odd K0 decay:\n";
            t_d->Print();
          }
        }
      } else if (t->pdgid() == 223) {
        auto t_daughters = ExtractDaughters(t, parts);
        for (auto const& t_d : t_daughters) {
          if (std::abs(t_d->pdgid()) == 211) {
            info.pi_charged.push_back(t_d);
          } else if (t_d->pdgid() == 111) {
            info.pi_neutral.push_back(t_d);
          } else {
            std::cout << "Odd omega decay:\n";
            t_d->Print();
          }
        }
      }else {
        std::cout << "Odd had decay:\n";
        t->Print();
        auto t_daughters = ExtractDaughters(t, parts);
        for (auto const& t_d : t_daughters) {
            t_d->Print();
        }
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

void TauInfo::Print() {
  std::cout << "** Tau Status 3:" << (tau_st3 ? "\n" : " <none>\n");
  if (tau_st3) tau_st3->Print();
  std::cout << "** Tau Status 2 (pre-fsr):" << (tau_st2_pre_fsr ? "\n" : " <none>\n");
  if (tau_st2_pre_fsr) tau_st2_pre_fsr->Print();
  std::cout << "** Tau Status 3 (post-fsr):" << (tau_st2_post_fsr ? "\n" : " <none>\n");
  if (tau_st2_post_fsr) tau_st2_post_fsr->Print();
  std::cout << "** Tau nu:" << (tau_nu ? "\n" : " <none>\n");
  if (tau_nu) tau_nu->Print();
  if (lep) {
    std::cout << "** Leptonic decay:" << (lep ? "\n" : " <none>\n");
    if (lep) lep->Print();
    std::cout << "** Lepton nu:" << (lep_nu ? "\n" : " <none>\n");
    if (lep_nu) lep_nu->Print();
    std::cout << "** FSR:\n";
    for (auto const& part : fsr) part->Print();
  } else {
    std::cout << "** Hadronic decay mode: " << hadronic_mode << "\n";
    std::cout << "** Resonance:" << (had ? "\n" : " <none>\n");
    if (had) had->Print();
    std::cout << "** Charged hadrons:\n";
    for (auto const& part : pi_charged) part->Print();
    std::cout << "** Neutral pions:\n";
    for (auto const& part : pi_neutral) part->Print();
    std::cout << "** Other neutral:\n";
    for (auto const& part : other_neutral) part->Print();
    std::cout << "** All visible:\n";
    for (auto const& part : all_vis) part->Print();
    std::cout << "** FSR:\n";
    for (auto const& part : fsr) part->Print();
  }
}
}
