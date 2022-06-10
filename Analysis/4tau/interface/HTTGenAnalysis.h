#ifndef ICHiggsTauTau_4tau_HTTGenAnalysis_h
#define ICHiggsTauTau_4tau_HTTGenAnalysis_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "TRandom3.h"
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include <string>
#include "Utilities/interface/FnRootTools.h"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagCalibrationStandalone.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/BTagWeightRun2.h"

namespace ic {
  
class HTTGenAnalysis : public ModuleBase {
 private:
  CLASS_MEMBER(HTTGenAnalysis, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTGenAnalysis, std::string, channel_str)
  CLASS_MEMBER(HTTGenAnalysis, double, min_e_pt     )
  CLASS_MEMBER(HTTGenAnalysis, double, min_mu_pt    )
  CLASS_MEMBER(HTTGenAnalysis, double, min_tau_pt  )
  CLASS_MEMBER(HTTGenAnalysis, double, max_e_eta    )
  CLASS_MEMBER(HTTGenAnalysis, double, max_mu_eta   )
  CLASS_MEMBER(HTTGenAnalysis, double, max_tau_eta  ) 
 
  TTree *outtree_;
  TRandom3  *rand;
  TTree *mvatree_;
  TFile *lOFile; 

  double wt_;
  double gen_wt_;
  double rand_;
 
  unsigned long long event_;

  unsigned n_rho_;
  unsigned n_tot_;
  
  unsigned count_eett_;
  unsigned count_emtt_;
  unsigned count_ettt_;
  unsigned count_mmtt_;
  unsigned count_mttt_;
  unsigned count_tttt_;
  unsigned count_taus_;
  unsigned count_eemt_;
  unsigned count_emmt_;
  unsigned count_eeet_;
  unsigned count_mmmt_;
  unsigned count_eeem_;
  unsigned count_eemm_;
  unsigned count_emmm_;
  unsigned count_eeee_;
  unsigned count_mmmm_;
  unsigned count_channel_;
  
  bool passed_;
  double pt_1_;
  double pt_2_;
  double pt_3_;
  double pt_4_;
  double eta_1_;
  double eta_2_;
  double eta_3_;
  double eta_4_;
  double phi_1_;
  double phi_2_;
  double phi_3_;
  double phi_4_;
  double met_;
  std::string decayType;
  double genpt_1_;
  double genpt_2_;
  double genpt_3_;
  double genpt_4_;
  double geneta_1_;
  double geneta_2_;
  double geneta_3_;
  double geneta_4_;

  float n_pu_;
 
 public:
  HTTGenAnalysis(std::string const& name);
  virtual ~HTTGenAnalysis();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
