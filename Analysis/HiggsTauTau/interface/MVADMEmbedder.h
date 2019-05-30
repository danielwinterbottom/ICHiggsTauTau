#ifndef ICHiggsTauTau_HiggsTauTau_MVADMEmbedder_h
#define ICHiggsTauTau_HiggsTauTau_MVADMEmbedder_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <string>
#include "Utilities/interface/FnRootTools.h"
#include "TMVA/Reader.h"

namespace ic {
  
class MVADMEmbedder : public ModuleBase {
 private:
  CLASS_MEMBER(MVADMEmbedder, fwlite::TFileService*, fs)
  CLASS_MEMBER(MVADMEmbedder, ic::channel, channel)
  
  TMVA::Reader *reader_even_;
  TMVA::Reader *reader_odd_;

 public:
  MVADMEmbedder(std::string const& name);
  virtual ~MVADMEmbedder();
  virtual std::vector<float> read_mva_scores(unsigned isEven);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  unsigned isEven_;
  float event_;

  float Egamma1_tau_; 
  float Egamma2_tau_;
  float Egamma3_tau_;
  float Epi_tau_; 
  float rho_dEta_tau_;
  float rho_dphi_tau_;
  float gammas_dEta_tau_;
  float gammas_dR_tau_;
  float DeltaR2WRTtau_tau_;
  float eta_;
  float pt_;
  float Epi0_;
  float Epi_;
  float rho_dEta_;
  float rho_dphi_;
  float gammas_dEta_;
  float tau_decay_mode_;
  float Mrho_;
  float Mpi0_;
  float DeltaR2WRTtau_;
  float Mpi0_TwoHighGammas_;
  float Mpi0_ThreeHighGammas_;
  float Mpi0_FourHighGammas_;
  float Mrho_OneHighGammas_;
  float Mrho_TwoHighGammas_;
  float Mrho_ThreeHighGammas_;
  float Mrho_subleadingGamma_;


};

}

#endif
