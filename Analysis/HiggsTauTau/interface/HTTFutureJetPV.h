#ifndef ICHiggsTauTau_HiggsTauTau_HTTFutureJetPV_h
#define ICHiggsTauTau_HiggsTauTau_HTTFutureJetPV_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

namespace ic {

class HTTFutureJetPV : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFutureJetPV, std::string, genjet_label)
  CLASS_MEMBER(HTTFutureJetPV, std::string, jets_label)
  CLASS_MEMBER(HTTFutureJetPV, fwlite::TFileService*, fs)

 TTree* outtree_;
 double jet_pt=0;  
 double jet_eta=0;  
 double genjet_pt=0;  
 double genjet_eta=0;  
 double jet_beta=0; 
 int jet_flav=0;

 public:
  explicit HTTFutureJetPV(std::string const& name);
  virtual ~HTTFutureJetPV();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);

  virtual int PostAnalysis();
  // virtual void PrintInfo();
};
}

#endif
