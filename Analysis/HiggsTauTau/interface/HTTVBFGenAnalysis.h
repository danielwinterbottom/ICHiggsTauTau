#ifndef ICHiggsTauTau_HiggsTauTau_HTTVBFGenAnalysis_h
#define ICHiggsTauTau_HiggsTauTau_HTTVBFGenAnalysis_h

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
  
class HTTVBFGenAnalysis : public ModuleBase {
 private:
  CLASS_MEMBER(HTTVBFGenAnalysis, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTVBFGenAnalysis, std::string, channel_str)
  CLASS_MEMBER(HTTVBFGenAnalysis, double, min_jet_pt    )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, max_jet_eta  )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, min_e_pt     )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, min_mu_pt    )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, min_tau1_pt  )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, min_tau2_pt  )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, max_e_eta    )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, max_mu_eta   )
  CLASS_MEMBER(HTTVBFGenAnalysis, double, max_tau_eta  )
  CLASS_MEMBER(HTTVBFGenAnalysis, bool, do_theory_uncert)
  
  TTree *outtree_;
  
  unsigned long long event_;
  std::vector<double> scale_variation_wts_;
  std::vector<double> NNPDF_wts_;
  std::vector<double> alpha_s_wts_;
  std::vector<double> CT10_wts_;
  std::vector<double> CT10_alpha_s_wts_;
  std::vector<double> MMHT_wts_;
  
  unsigned count_ee_;
  unsigned count_em_;
  unsigned count_et_;
  unsigned count_mm_;
  unsigned count_mt_;
  unsigned count_tt_;
    
    
    bool HbbPassed_;
    bool HLTPassThroughPassed_;
    bool L1Passed_;
    
  
  bool passed_;
  double pt_1_;
  double pt_2_;
  double eta_1_;
  double eta_2_;
  double phi_1_;
  double phi_2_;
  double met_;
  double calo_met_;
  double hlt_met_;
  double mt_1_;
  double mt_2_;
  double pzeta_;
  double n_bjets_;
  unsigned n_jets_nofilter_;
  unsigned n_jets_;
  unsigned n_jetsingap_;
  double hlt_jpt_1_;
  double hlt_jpt_2_;
  double hlt_jpt_3_;
  double hlt_jpt_4_;
  double calo_jpt_1_;
  double calo_jpt_2_;
  double hlt_jeta_1_;
  double hlt_jeta_2_;
  double hlt_jeta_3_;
  double hlt_jeta_4_;
  double hlt_mjj_;
  double min_dphi_;
  double dphi_;
  double deta_;
  double calo_mjj_;
  double hlt_mjj30_;
  double calo_mjj30_;
  double jpt_1_;  
  double jpt_2_;  
  double jpt_3_;
  double jeta_1_; 
  double jeta_2_; 
  double jphi_1_;
  double jphi_2_;
  double mjj_;
  double mjj30_;
  double jdeta_;
  double m_vis_;
  double pt_tt_;
  double wt_;
  double HiggsPt_;
  double FirstHiggsPt_;
  std::string decayType;
  double wt_ggh_pt_           ;
  double wt_ggh_pt_up_        ;
  double wt_ggh_pt_down_      ;
  double wt_ggh_pt_herwig_    ;
  double wt_ggh_pt_amc_       ;
  double wt_ggh_pt_pythiaup_  ;
  double wt_ggh_pt_pythiadown_;
  double wt_ggh_pt_scalehigh_ ;
  double wt_ggh_pt_scalelow_  ;
  double genpt_1_;
  double genpt_2_;
  double geneta_1_;
  double geneta_2_;
  bool hasFSR_;
  unsigned n_bquarks_;
  double genpT_;
  double genM_;
  float wt_dy_;
  double wtzpt_;
  TH2D z_pt_mass_hist_;
  
  TFile *ggh_weights_;
  TH1F *ggh_hist_;
  TH1F *ggh_hist_up_;
  TH1F *ggh_hist_down_;
  TH1F *ggh_herwig_hist_;
  TH1F *ggh_amcnlo_hist_;
  TH1F *ggh_pythiaup_hist_;
  TH1F *ggh_pythiadown_hist_;
  TH1F *ggh_scalehigh_;
  TH1F *ggh_scalelow_;
  
 public:
  HTTVBFGenAnalysis(std::string const& name);
  virtual ~HTTVBFGenAnalysis();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
