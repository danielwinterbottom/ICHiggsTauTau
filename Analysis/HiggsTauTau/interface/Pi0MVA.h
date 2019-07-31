#ifndef ICHiggsTauTau_HiggsTauTau_Pi0MVA_h
#define ICHiggsTauTau_HiggsTauTau_Pi0MVA_h

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
  
class Pi0MVA : public ModuleBase {
 private:
  CLASS_MEMBER(Pi0MVA, fwlite::TFileService*, fs)
  CLASS_MEMBER(Pi0MVA, ic::channel, channel)
  CLASS_MEMBER(Pi0MVA, bool, maketrees)
  CLASS_MEMBER(Pi0MVA, ic::strategy, strategy)
  
  TTree *outtree_;
  double wt_;
  TMVA::Reader *reader_;

 public:
  Pi0MVA(std::string const& name);
  virtual ~Pi0MVA();
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  unsigned gen_match_1_;

  unsigned event_;
  double wt_cp_sm_, wt_cp_ps_, wt_cp_mm_;
  int n_pu_;

  float pi0_2gammas_E_, pi0_3gammas_E_, pi0_4gammas_E_;
  float pi0_2gammas_eta_, pi0_3gammas_eta_, pi0_4gammas_eta_;
  float pi0_2gammas_phi_, pi0_3gammas_phi_, pi0_4gammas_phi_;

  float pi0_2gammas_M_, pi0_3gammas_M_, pi0_4gammas_M_, pi0_1gammas_M_;
  float Egammas_4ormore_;

  double tauFlag1_;
  double tauFlag2_;
  double gen_match_2_;
  double tau_decay_mode_1_;
  double tau_decay_mode_2_;
  unsigned Ngammas_1_, NgammaspT1_1_;
  float n_vtx_, rho_;
  float Egentau_1_, Phigentau_1_, Etagentau_1_, Mgentau_1_, Pxgentau_1_, Pygentau_1_, Pzgentau_1_;
  float Egenpi0_1_, Phigenpi0_1_, Etagenpi0_1_, Mgenpi0_1_, Pxgenpi0_1_, Pygenpi0_1_, Pzgenpi0_1_;
  float Epi_1_, Phipi_1_, Etapi_1_, Mpi_1_, Pxpi_1_, Pypi_1_, Pzpi_1_;
  float Epi0_1_, Phipi0_1_, Etapi0_1_, Mpi0_1_, Pxpi0_1_, Pypi0_1_, Pzpi0_1_;
  float Etau_1_, Phitau_1_, Etatau_1_, Mtau_1_, Pxtau_1_, Pytau_1_, Pztau_1_;
  float Egammas_1_, Phigammas_1_, Etagammas_1_, Mgammas_1_, Pxgammas_1_, Pygammas_1_, Pzgammas_1_;
  float Egammas_2_, Phigammas_2_, Etagammas_2_, Mgammas_2_, Pxgammas_2_, Pygammas_2_, Pzgammas_2_;
  float Egammas_3_, Phigammas_3_, Etagammas_3_, Mgammas_3_, Pxgammas_3_, Pygammas_3_, Pzgammas_3_;
  float Egammas_4_, Phigammas_4_, Etagammas_4_, Mgammas_4_, Pxgammas_4_, Pygammas_4_, Pzgammas_4_;
  float Egammas_5_, Phigammas_5_, Etagammas_5_, Mgammas_5_, Pxgammas_5_, Pygammas_5_, Pzgammas_5_;
  float Egammas_6_, Phigammas_6_, Etagammas_6_, Mgammas_6_, Pxgammas_6_, Pygammas_6_, Pzgammas_6_;
  float Egammas_7_, Phigammas_7_, Etagammas_7_, Mgammas_7_, Pxgammas_7_, Pygammas_7_, Pzgammas_7_;

  float eta_, pt_;
  float new_pi0_E_1_ = 0., new_pi0_E_2_ = 0., new_pi0_E_3_ =0., new_pi0_E_4_=0.;
  float new_M_1_, new_M_2_, new_M_3_, new_M_4_;
  float new_eta_1_, new_eta_2_, new_eta_3_, new_eta_4_;
  float new_phi_1_, new_phi_2_, new_phi_3_, new_phi_4_;
  float mod_M_;
  unsigned Nseeds_;

  float Egenpi0_1_1_, Egenpi0_1_2_;
  float Egengamma_1_, Egengamma_2_;
  float max_gengamma_dR_;
  float gen_tau_pt_; 
  float a1_rho_mass_1_, a1_rho_mass_2_;

};

}

#endif
