#ifndef ICHiggsTauTau_Module_HttWeights_h
#define ICHiggsTauTau_Module_HttWeights_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include <string>

#define MEMBER_NP(type,name)                                                \
    private:                                                                \
      type name##_;                                                         \
    public:                                                                 \
      virtual HttWeights & set_##name(type const& name) {name##_ = name; return *this; }

namespace ic {

class HttWeights : public ModuleBase {
 private:
  MEMBER_NP(unsigned, mode)
  MEMBER_NP(bool, is_2012)
  MEMBER_NP(bool, do_trg_weights)
  MEMBER_NP(bool, trg_applied_in_mc)
  MEMBER_NP(bool, do_etau_fakerate)
  MEMBER_NP(bool, do_idiso_weights)
  MEMBER_NP(bool, do_emu_e_fakerates)
  MEMBER_NP(bool, do_emu_m_fakerates)
  MEMBER_NP(bool, do_top_factors)
  MEMBER_NP(bool, do_btag_weight)
  MEMBER_NP(std::string, ggh_mass)
  MEMBER_NP(bool, do_w_soup)
  MEMBER_NP(unsigned, era)

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
  mithep::TH2DAsymErr* MuonFakeRateHist_PtEta;
  mithep::TH2DAsymErr* ElectronFakeRateHist_PtEta;
  BTagWeight btag_weight;

  double f0_,f1_,f2_,f3_,f4_,n_inc_,n1_,n2_,n3_,n4_,w0_,w1_,w2_,w3_,w4_;


 public:
  HttWeights(std::string const& name);
  virtual ~HttWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  double Efficiency(double m, double m0, double sigma, double alpha, double n, double norm);
  void SetWTargetFractions(double f0, double f1, double f2, double f3, double f4);
  void SetWInputYields(double n_inc, double n1, double n2, double n3, double n4);

};

}

#undef MEMBER_NP

#endif
