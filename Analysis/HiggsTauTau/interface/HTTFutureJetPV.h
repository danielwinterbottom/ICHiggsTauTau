#ifndef ICHiggsTauTau_HiggsTauTau_HTTFutureJetPV_h
#define ICHiggsTauTau_HiggsTauTau_HTTFutureJetPV_h

#include <vector>
#include <string>
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"

namespace ic {
bool SortByJetPt(std::pair<ic::PFJet*,ic::GenJet*> const c1,
                                       std::pair<ic::PFJet*,ic::GenJet*> const c2); 

class HTTFutureJetPV : public ModuleBase {

 private:
  CLASS_MEMBER(HTTFutureJetPV, std::string, genjet_label)
  CLASS_MEMBER(HTTFutureJetPV, std::string, genparticle_label)
  CLASS_MEMBER(HTTFutureJetPV, std::string, jets_label)
  CLASS_MEMBER(HTTFutureJetPV, fwlite::TFileService*, fs)

 TTree* outtree_;
 double jet_pt=0;  
 double jet_eta=0;  
 double genjet_pt=0;  
 double genjet_eta=0;  
 double jet_beta=0; 
 double jet_beta_puppi=0; 
 int jet_flav=0;
  TTree* outtree_second_;
 double jet_pt1=0;  
 double jet_eta1=0;  
 double genjet_pt1=0;  
 double genjet_eta1=0;  
 double jet_beta1=0; 
 int jet_flav1=0;
 double jet_pt2=0;  
 double jet_eta2=0;  
 double genjet_pt2=0;  
 double genjet_eta2=0;  
 double jet_beta2=0; 
 int jet_flav2=0;
 double vbf_dphi=-9999;
 double vtxdz=0;
 double vtxdx=0;
 double vtxdy=0;

 


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
