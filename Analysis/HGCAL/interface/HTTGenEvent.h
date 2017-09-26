#ifndef ICHiggsTauTau_HiggsTauTau_HTTGenEvent_h
#define ICHiggsTauTau_HiggsTauTau_HTTGenEvent_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/GenParticle.hh"
#include "Objects/interface/GenJet.hh"

namespace ic {



struct TauInfo {
  GenParticle *tau_st3;
  GenParticle *tau_st2_pre_fsr;
  GenParticle *tau_st2_post_fsr;
  GenParticle *tau_nu;
  GenParticle *lep;
  GenParticle *lep_nu;
  GenJet *vis_jet;
  GenParticle *had;
  int hadronic_mode;
  int leptonic_mode;
  TauInfo()
      : tau_st3(nullptr),
        tau_st2_pre_fsr(nullptr),
        tau_st2_post_fsr(nullptr),
        tau_nu(nullptr),
        lep(nullptr),
        lep_nu(nullptr),
        vis_jet(nullptr),
        had(nullptr),
        hadronic_mode(-1),
        leptonic_mode(-1) {}
  std::vector<GenParticle *> fsr;
  std::vector<GenParticle *> pi_charged;
  std::vector<GenParticle *> pi_neutral;
  std::vector<GenParticle *> other_neutral;
  std::vector<GenParticle *> all_vis;

  void Print();
};

struct GenEvent_XToTauTau {
  GenParticle *boson;
  TauInfo tau_0;
  TauInfo tau_1;
  GenEvent_XToTauTau() : boson(nullptr) {}
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
  CLASS_MEMBER(HTTGenEvent, bool, is_pythia8)

 public:
  explicit HTTGenEvent(std::string const& name);
  virtual ~HTTGenEvent();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);

  static TauInfo BuildTauInfo(GenParticle *tau,
                            std::vector<GenParticle *> const &parts,
                            bool is_pythia8);
  virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
