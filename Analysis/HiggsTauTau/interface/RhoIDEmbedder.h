#ifndef ICHiggsTauTau_HiggsTauTau_RhoIDEmbedder_h
#define ICHiggsTauTau_HiggsTauTau_RhoIDEmbedder_h

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
  
class RhoIDEmbedder : public ModuleBase {
 private:
  CLASS_MEMBER(RhoIDEmbedder, fwlite::TFileService*, fs)
  CLASS_MEMBER(RhoIDEmbedder, ic::channel, channel)
  CLASS_MEMBER(RhoIDEmbedder, bool, maketrees)
  
  TTree *outtree_;
  int wt_;
  TMVA::Reader *reader_;

 public:
  RhoIDEmbedder(std::string const& name);
  virtual ~RhoIDEmbedder();
  virtual double read_mva_score(std::vector<double> vars);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

  float var0, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13;

  double Etau_1_;
  double Etau_2_;
  double rho_dEta_1_;
  double rho_dphi_1_;
  double rho_dEta_2_;
  double rho_dphi_2_;

  // variables for training trees
  unsigned Ngammas_1_;
  double Egamma1_1_;
  double Egamma2_1_;
  double Egamma3_1_;
  double Egamma4_1_;
  double Epi_1_;    
  double Mpi0_1_;   
  double Mrho_1_;   
  double dphi_1_;   
  double dEta_1_;   
  double gammas_dEta_1_;
  double gammas_dphi_1_;
  double pt_1_;
  double eta_1_; 

  unsigned Ngammas_2_;
  double Egamma1_2_;
  double Egamma2_2_;
  double Egamma3_2_;
  double Egamma4_2_;
  double Epi_2_;
  double Mpi0_2_;
  double Mrho_2_;
  double dphi_2_;
  double dEta_2_;
  double gammas_dEta_2_;
  double gammas_dphi_2_;
  double pt_2_;
  double eta_2_;

  int tauFlag1_;
  int tauFlag2_;

  

};

}

#endif
