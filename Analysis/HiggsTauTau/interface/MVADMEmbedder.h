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
  CLASS_MEMBER(MVADMEmbedder, ic::strategy, strategy) 
 
  TMVA::Reader *reader_even_;
  TMVA::Reader *reader_odd_;
  TMVA::Reader *reader_dm10_even_;
  TMVA::Reader *reader_dm10_odd_;

 public:
  MVADMEmbedder(std::string const& name);
  virtual ~MVADMEmbedder();
  virtual std::vector<float> read_mva_scores(unsigned isEven, std::vector<float> vars, int tau_decay_mode_1_);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  unsigned isEven_;
  float event_;
  unsigned long long evt_;
  double gammas_pt_cut_ = 0.5;


  float Egamma1_tau_1_; 
  float Egamma2_tau_1_;
  float Epi_tau_1_; 
  float rho_dEta_tau_1_;
  float rho_dphi_tau_1_;
  float gammas_dEta_tau_1_;
  float gammas_dR_tau_1_;
  float DeltaR2WRTtau_tau_1_;
  float eta_1_;
  float pt_1_;
  float Epi0_1_;
  float Epi_1_;
  float rho_dEta_1_;
  float rho_dphi_1_;
  float gammas_dEta_1_;
  float tau_decay_mode_1_;
  float Mrho_1_;
  float Mpi0_1_;
  float DeltaR2WRTtau_1_;
  float Mpi0_TwoHighGammas_1_;
  float Mrho_OneHighGammas_1_;
  float Mrho_TwoHighGammas_1_;
  float Mrho_subleadingGamma_1_;
  float strip_pt_1_;

  float Egamma1_tau_2_;
  float Egamma2_tau_2_;
  float Epi_tau_2_; 
  float rho_dEta_tau_2_;
  float rho_dphi_tau_2_;
  float gammas_dEta_tau_2_;
  float gammas_dR_tau_2_;
  float DeltaR2WRTtau_tau_2_;
  float eta_2_;
  float pt_2_;
  float Epi0_2_;
  float Epi_2_;
  float rho_dEta_2_;
  float rho_dphi_2_;
  float gammas_dEta_2_;
  float tau_decay_mode_2_;
  float Mrho_2_;
  float Mpi0_2_;
  float DeltaR2WRTtau_2_;
  float Mpi0_TwoHighGammas_2_;
  float Mrho_OneHighGammas_2_;
  float Mrho_TwoHighGammas_2_;
  float Mrho_subleadingGamma_2_;
  float strip_pt_2_;

  float E_1_;
  float E1_1_;
  float E1_overEa1_1_;
  float E1_overEtau_1_;
  float E2_1_;
  float E2_overEa1_1_;
  float E2_overEtau_1_;
  float E3_1_;
  float E3_overEa1_1_;
  float E3_overEtau_1_;
  float a1_pi0_dEta_1_;
  float a1_pi0_dEta_timesEtau_1_;
  float a1_pi0_dphi_1_;
  float a1_pi0_dphi_timesEtau_1_;
  float h1_h2_dEta_1_;
  float h1_h2_dEta_timesE12_1_;
  float h1_h2_dphi_1_;
  float h1_h2_dphi_timesE12_1_;
  float h1_h3_dEta_1_;
  float h1_h3_dEta_timesE13_1_;
  float h1_h3_dphi_1_;
  float h1_h3_dphi_timesE13_1_;
  float h2_h3_dEta_1_;
  float h2_h3_dEta_timesE23_1_;
  float h2_h3_dphi_1_;
  float h2_h3_dphi_timesE23_1_;
  float mass0_1_;
  float mass1_1_;
  float mass2_1_;
  float strip_E_1_;

  float E_2_;
  float E1_2_;
  float E1_overEa1_2_;
  float E1_overEtau_2_;
  float E2_2_;
  float E2_overEa1_2_;
  float E2_overEtau_2_;
  float E3_2_;
  float E3_overEa1_2_;
  float E3_overEtau_2_;
  float a1_pi0_dEta_2_;
  float a1_pi0_dEta_timesEtau_2_;
  float a1_pi0_dphi_2_;
  float a1_pi0_dphi_timesEtau_2_;
  float h1_h2_dEta_2_;
  float h1_h2_dEta_timesE12_2_;
  float h1_h2_dphi_2_;
  float h1_h2_dphi_timesE12_2_;
  float h1_h3_dEta_2_;
  float h1_h3_dEta_timesE13_2_;
  float h1_h3_dphi_2_;
  float h1_h3_dphi_timesE13_2_;
  float h2_h3_dEta_2_;
  float h2_h3_dEta_timesE23_2_;
  float h2_h3_dphi_2_;
  float h2_h3_dphi_timesE23_2_;
  float mass0_2_;
  float mass1_2_;
  float mass2_2_;
  float strip_E_2_;


  float var0, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15, var16, var17, var18, var19, var20, var21, var22, var23, var24, var25, var26, var27, var28, var29, var30, var31, var32, var33, var34, var35, var36, var37, var38, var39;

};

}

#endif
