#ifndef ICHiggsTauTau_HiggsTauTau_HTTFutureTauResolution_h
#define ICHiggsTauTau_HiggsTauTau_HTTFutureTauResolution_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

namespace ic {



class HTTFutureTauResolution : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFutureTauResolution, std::string, genparticle_label)
  CLASS_MEMBER(HTTFutureTauResolution, std::string, taus_label)
  CLASS_MEMBER(HTTFutureTauResolution, fwlite::TFileService*, fs)

 TTree* outtree_;
 double tau_resolution=0;  
 unsigned tau_dm=0;

 public:
  explicit HTTFutureTauResolution(std::string const& name);
  virtual ~HTTFutureTauResolution();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);

  /*TauResolution_Tau BuildTauInfo(GenParticle *tau,
                            std::vector<GenParticle *> const &parts,
                            bool is_pythia8);*/
  virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
