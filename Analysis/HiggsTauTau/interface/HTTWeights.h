#ifndef ICHiggsTauTau_Module_HTTWeights_h
#define ICHiggsTauTau_Module_HTTWeights_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include <string>


namespace ic {

class HTTWeights : public ModuleBase {
 private:
  CLASS_MEMBER(HTTWeights, ic::channel, channel)
  CLASS_MEMBER(HTTWeights, ic::mc, mc)
  CLASS_MEMBER(HTTWeights, ic::era, era)
  CLASS_MEMBER(HTTWeights, bool, do_trg_weights)
  CLASS_MEMBER(HTTWeights, bool, trg_applied_in_mc)
  CLASS_MEMBER(HTTWeights, bool, do_singlemu_trg_weights)
  CLASS_MEMBER(HTTWeights, bool, do_etau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_mtau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_idiso_weights)
  CLASS_MEMBER(HTTWeights, bool, do_id_weights)
  CLASS_MEMBER(HTTWeights, bool, do_emu_e_fakerates)
  CLASS_MEMBER(HTTWeights, bool, do_emu_m_fakerates)
  CLASS_MEMBER(HTTWeights, bool, do_top_factors)
  CLASS_MEMBER(HTTWeights, bool, do_btag_weight)
  CLASS_MEMBER(HTTWeights, int, btag_mode)
  CLASS_MEMBER(HTTWeights, int, bfake_mode)
  CLASS_MEMBER(HTTWeights, std::string, ggh_mass)
  CLASS_MEMBER(HTTWeights, bool, do_w_soup)
  CLASS_MEMBER(HTTWeights, bool, do_dy_soup)

  TFile *muTauSF2011;
  TH2D *hist_muTauSF2011;
  TH2D *hist_muTauSF2011PFTau10;
  TH2D *hist_muTauSF2011PFTau15;
  TH2D *hist_muTauSF2011PFTau15MC;
  TH2D *hist_muTauSF2011PFTau20;
  TFile *ElectronFRFile;
  TFile *MuonFRFile;
  TFile *ggh_weights_;
  TH1F *ggh_hist_;
  TH1F *ggh_hist_up_;
  TH1F *ggh_hist_down_;
  mithep::TH2DAsymErr* MuonFakeRateHist_PtEta;
  mithep::TH2DAsymErr* ElectronFakeRateHist_PtEta;
  BTagWeight btag_weight;

  double f0_,f1_,f2_,f3_,f4_,n_inc_,n1_,n2_,n3_,n4_,w0_,w1_,w2_,w3_,w4_;
  double zf0_,zf1_,zf2_,zf3_,zf4_,zn_inc_,zn1_,zn2_,zn3_,zn4_,zw0_,zw1_,zw2_,zw3_,zw4_;


 public:
  HTTWeights(std::string const& name);
  virtual ~HTTWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  double Efficiency(double m, double m0, double sigma, double alpha, double n, double norm);
  void SetWTargetFractions(double f0, double f1, double f2, double f3, double f4);
  void SetWInputYields(double n_inc, double n1, double n2, double n3, double n4);
  void SetDYTargetFractions(double zf0, double zf1, double zf2, double zf3, double zf4);
  void SetDYInputYields(double zn_inc, double zn1, double zn2, double zn3, double zn4);

};

}

#undef MEMBER_NP

#endif
