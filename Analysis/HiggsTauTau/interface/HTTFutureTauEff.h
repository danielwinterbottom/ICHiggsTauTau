#ifndef ICHiggsTauTau_HiggsTauTau_HTTFutureTauEff_h
#define ICHiggsTauTau_HiggsTauTau_HTTFutureTauEff_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

namespace ic {



class HTTFutureTauEff : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFutureTauEff, std::string, genparticle_label)
  CLASS_MEMBER(HTTFutureTauEff, std::string, taus_label)
  CLASS_MEMBER(HTTFutureTauEff, fwlite::TFileService*, fs)

 TTree* outtree_;
 double tau_pt=0;  
 double tau_eta=-999.;  
 bool tau_dm_finding=false;
 int tau_dm=-1;  
 double tau_mvaiso_raw=-999;
 double tau_combiso=-1;

 public:
  explicit HTTFutureTauEff(std::string const& name);
  virtual ~HTTFutureTauEff();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);

  /*TauEff_Tau BuildTauInfo(GenParticle *tau,
                            std::vector<GenParticle *> const &parts,
                            bool is_pythia8);*/
  virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
