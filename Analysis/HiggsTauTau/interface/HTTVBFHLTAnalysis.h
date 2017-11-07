#ifndef ICHiggsTauTau_HiggsTauTau_HTTVBFHLTAnalysis_h
#define ICHiggsTauTau_HiggsTauTau_HTTVBFHLTAnalysis_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"

#include <string>

namespace ic {
  
class HTTVBFHLTAnalysis : public ModuleBase {
 private:
  CLASS_MEMBER(HTTVBFHLTAnalysis, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTVBFHLTAnalysis, std::string, channel_str)
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, min_jet_pt    )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, max_jet_eta  )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, min_e_pt     )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, min_mu_pt    )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, min_tau1_pt  )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, min_tau2_pt  )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, max_e_eta    )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, max_mu_eta   )
  CLASS_MEMBER(HTTVBFHLTAnalysis, double, max_tau_eta  )
  CLASS_MEMBER(HTTVBFHLTAnalysis, bool, do_theory_uncert)
  
  TTree *outtree_;
    
    unsigned long long event_;
  
    bool passedVBF_2jets;
    bool passedVBF_3jets;
    bool passedMETMHT;
    bool passedIsoMu;
    
    //HLT passthrough
    bool VBFPassed_;
    bool VBFL1Passed_;
    bool METMHTPassed_;
    bool IsoMuPassed_;
    
    //HLT properties
    double vbf_pf_jpt_1_;
    double vbf_pf_jpt_2_;
    double vbf_pf_jpt_3_;
    double vbf_pf_jpt_4_;
    
    double vbf_pf_jpt_2d_plot_;
    
    double vbf_pf_jeta_1_;
    double vbf_pf_jeta_2_;
    double vbf_pf_jeta_3_;
    double vbf_pf_jeta_4_;
    double vbf_pf_mjj_;
    double vbf_pf_mjj30_;
    
    
    double vbf_all_pf_jpt_1_;
    double vbf_all_pf_jpt_2_;
    double vbf_all_pf_jpt_3_;
    double vbf_all_pf_jpt_4_;
    
    double vbf_all_pf_jeta_1_;
    double vbf_all_pf_jeta_2_;
    double vbf_all_pf_jeta_3_;
    double vbf_all_pf_jeta_4_;
    double vbf_all_pf_mjj_;
    double vbf_all_pf_mjj30_;
    
    double l1_jpt_1_;
    double l1_jpt_2_;
    double l1_jpt_3_;
    double l1_jpt_4_;
    
    double l1_jpt_2d_plot_;
    
    double l1_jeta_1_;
    double l1_jeta_2_;
    double l1_jeta_3_;
    double l1_jeta_4_;
    double l1_mjj_;
    double l1_mjj30_;
    double l1_etajj_;
    double l1_phijj_;
    
    
    double vbf_l1_jpt_1_;
    double vbf_l1_jpt_2_;
    double vbf_l1_jpt_3_;
    double vbf_l1_jpt_4_;
    
    double vbf_l1_jpt_2d_plot_;
    
    double vbf_l1_jeta_1_;
    double vbf_l1_jeta_2_;
    double vbf_l1_jeta_3_;
    double vbf_l1_jeta_4_;
    double vbf_l1_mjj_;
    double vbf_l1_mjj30_;
    double vbf_l1_etajj_;
    double vbf_l1_phijj_;

    
    
    double vbf_calo_met_;
    double vbf_noise_cleaned_calo_met;
    double vbf_pf_met_;
   
    //Offline properties
    double leading_jpt_1_;
    double leading_jpt_2_;
    double leading_jeta_1_;
    double leading_jeta_2_;
    double leading_mjj_;
    
    
    double offline_jpt_1_;
    double offline_jpt_2_;
    double offline_jpt_3_;
    
    double offline_jeta_1_;
    double offline_jeta_2_;
    double offline_jeta_3_;
    
    double offline_mjj_;
    double offline_mjj30_;
    
    double offline_dphijj_;
    double offline_detajj_;
    
    double offline_matched_jpt_1_;
    double offline_matched_jpt_2_;
    double offline_matched_jpt_3_;
    
    double offline_matched_jeta_1_;
    double offline_matched_jeta_2_;
    double offline_matched_jeta_3_;
    
    double offline_matched_mjj_;
    double offline_matched_mjj30_;
    
    double offline_matched_dphijj_;
    double offline_matched_detajj_;
    
    double offline_met_;
 
  
 public:
  HTTVBFHLTAnalysis(std::string const& name);
  virtual ~HTTVBFHLTAnalysis();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
