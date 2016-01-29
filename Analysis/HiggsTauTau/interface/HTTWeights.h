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
  CLASS_MEMBER(HTTWeights, bool, do_tau_mode_scale)
  CLASS_MEMBER(HTTWeights, bool, do_etau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_mtau_fakerate)
  CLASS_MEMBER(HTTWeights, bool, do_idiso_weights)
  CLASS_MEMBER(HTTWeights, bool, do_id_weights)
  CLASS_MEMBER(HTTWeights, bool, do_emu_e_fakerates)
  CLASS_MEMBER(HTTWeights, bool, do_emu_m_fakerates)
  CLASS_MEMBER(HTTWeights, bool, do_top_factors)
  CLASS_MEMBER(HTTWeights, bool, do_btag_weight)
  CLASS_MEMBER(HTTWeights, bool, do_topquark_weights)
  CLASS_MEMBER(HTTWeights, bool, do_top_jeteta_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_fake_weights)
  CLASS_MEMBER(HTTWeights, int, btag_mode)
  CLASS_MEMBER(HTTWeights, int, bfake_mode)
  CLASS_MEMBER(HTTWeights, std::string, ggh_mass)
  CLASS_MEMBER(HTTWeights, std::string, gen_tau_collection)
  CLASS_MEMBER(HTTWeights, std::string, jets_label)
  CLASS_MEMBER(HTTWeights, std::string, btag_label)
  CLASS_MEMBER(HTTWeights, std::string, ditau_label)
  CLASS_MEMBER(HTTWeights, bool, do_w_soup)
  CLASS_MEMBER(HTTWeights, bool, do_w_soup_htbinned)
  CLASS_MEMBER(HTTWeights, bool, do_dy_soup)
  CLASS_MEMBER(HTTWeights, bool, do_dy_soup_htbinned)
  CLASS_MEMBER(HTTWeights, bool, do_tt_muon_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_id_weights)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_idiso_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_idiso_data)
  CLASS_MEMBER(HTTWeights, TH2D*, et_idiso_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, et_idiso_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_e_idiso_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, em_e_idiso_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_m_idiso_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, em_m_idiso_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_m17_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, em_m17_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_m8_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, em_m8_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_e17_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, em_e17_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_e12_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, em_e12_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, et_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, et_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_trig_data)
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
  TF1 *tau_fake_weights_;

  double f0_,f1_,f2_,f3_,f4_,n_inc_,n1_,n2_,n3_,n4_,w0_,w1_,w2_,w3_,w4_;
  double zf0_,zf1_,zf2_,zf3_,zf4_,zn_inc_,zn1_,zn2_,zn3_,zn4_,zw0_,zw1_,zw2_,zw3_,zw4_;
  double zxs0_,zxs1_,zxs2_,zxs3_,zxs4_,z_lo_nlo_corr_;
  double wxs0_,wxs1_,wxs2_,wxs3_,wxs4_,w_lo_nlo_corr_;
  double wt_lumi_;


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
  void SetDYInputCrossSections(double zxs0, double zxs1, double zxs2, double zxs3, double zxs4);
  void SetWInputCrossSections(double wxs0, double wxs1, double wxs2, double wxs3, double wxs4);
};

}

#undef MEMBER_NP

#endif
