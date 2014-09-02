#ifndef ICHiggsTauTau_HiggsTauTau_HTTGenEvent_h
#define ICHiggsTauTau_HiggsTauTau_HTTGenEvent_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

namespace ic {



struct GenEvent_Tau {
  GenParticle *tau_st3            = nullptr;
  GenParticle *tau_st2_pre_fsr    = nullptr;
  GenParticle *tau_st2_post_fsr   = nullptr;
  GenParticle *tau_nu             = nullptr;
  GenParticle *lep                = nullptr;
  GenParticle *lep_nu             = nullptr;
  GenJet *vis_jet                 = nullptr;
  GenParticle *had                = nullptr;
  int hadronic_mode               = -1;
  int leptonic_mode               = -1;
  std::vector<GenParticle *> fsr;
  std::vector<GenParticle *> pi_charged;
  std::vector<GenParticle *> pi_neutral;
  std::vector<GenParticle *> other_neutral;
};

struct GenEvent_XToTauTau {
  GenParticle *boson              = nullptr;
  GenEvent_Tau tau_0;
  GenEvent_Tau tau_1;
};

// struct GenEvent_XToFF {
//   GenParticle *boson              = nullptr;
//   GenParticle *f_0                = nullptr;
//   GenParticle *f_1                = nullptr;
// };

// struct GenEvent_VH_VToFF_HToTauTau {
//   GenEvent_XToFF x_to_ff;
//   GenEvent_XToTauTau x_to_tautau;
// };

class HTTGenEvent : public ModuleBase {
 private:
  CLASS_MEMBER(HTTGenEvent, std::string, genparticle_label)
  CLASS_MEMBER(HTTGenEvent, std::string, genjet_label)

 public:
  explicit HTTGenEvent(std::string const& name);
  virtual ~HTTGenEvent();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);

  GenEvent_Tau BuildTauInfo(GenParticle *tau,
                            std::vector<GenParticle *> const &parts);
  virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
