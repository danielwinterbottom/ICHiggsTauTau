#ifndef ICHiggsTauTau_HiggsTauTau_HTTFutureGenEvent_h
#define ICHiggsTauTau_HiggsTauTau_HTTFutureGenEvent_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

namespace ic {



/*struct GenEvent_Tau {
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
  GenEvent_Tau()
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
};

struct GenEvent_XToTauTau {
  GenParticle *boson;
  GenEvent_Tau tau_0;
  GenEvent_Tau tau_1;
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
// };*/

class HTTFutureGenEvent : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFutureGenEvent, std::string, genparticle_label)
  CLASS_MEMBER(HTTFutureGenEvent, std::string, genjet_label)
  CLASS_MEMBER(HTTFutureGenEvent, std::string, jets_label)
  CLASS_MEMBER(HTTFutureGenEvent, fwlite::TFileService*, fs)

 TTree* outtree_;
 double tau1_pt=0;  
 double tau2_pt=0;  
 double tau1_eta=0; 
 double tau2_eta=0;  
 int tau1_type=0; 
 int tau2_type=0;  
 double jet1_pt=0;
 double jet2_pt=0;
 double jet1_eta=0;
 double jet2_eta=0;
 int jet1_flav=0;
 int jet2_flav=0;
 double vtxdz=-1;
 double vtxdy=-1;
 double vtxdx=-1;

 public:
  explicit HTTFutureGenEvent(std::string const& name);
  virtual ~HTTFutureGenEvent();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);

  /*GenEvent_Tau BuildTauInfo(GenParticle *tau,
                            std::vector<GenParticle *> const &parts,
                            bool is_pythia8);*/
  virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
