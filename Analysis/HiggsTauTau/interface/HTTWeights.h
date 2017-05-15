#ifndef ICHiggsTauTau_Module_HTTWeights_h
#define ICHiggsTauTau_Module_HTTWeights_h

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include "Utilities/interface/FnRootTools.h"

#include <string>


namespace ic {

class HTTWeights : public ModuleBase {
 private:
  CLASS_MEMBER(HTTWeights, ic::channel, channel)
  CLASS_MEMBER(HTTWeights, ic::mc, mc)
  CLASS_MEMBER(HTTWeights, ic::era, era)
  CLASS_MEMBER(HTTWeights, bool, do_trg_weights)
  CLASS_MEMBER(HTTWeights, bool, trg_applied_in_mc)
  CLASS_MEMBER(HTTWeights, bool, do_single_lepton_trg)
  CLASS_MEMBER(HTTWeights, bool, do_cross_trg)
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
  CLASS_MEMBER(HTTWeights, bool, do_zpt_weight)
  CLASS_MEMBER(HTTWeights, bool, do_tracking_eff)
  CLASS_MEMBER(HTTWeights, bool, do_topquark_weights)
  CLASS_MEMBER(HTTWeights, bool, do_top_jeteta_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_fake_weights)
  CLASS_MEMBER(HTTWeights, int, btag_mode)
  CLASS_MEMBER(HTTWeights, int, bfake_mode)
  CLASS_MEMBER(HTTWeights, int, tt_trg_iso_mode)
  CLASS_MEMBER(HTTWeights, std::string, ggh_mass)
  CLASS_MEMBER(HTTWeights, std::string, gen_tau_collection)
  CLASS_MEMBER(HTTWeights, std::string, jets_label)
  CLASS_MEMBER(HTTWeights, std::string, btag_label)
  CLASS_MEMBER(HTTWeights, std::string, ditau_label)
  CLASS_MEMBER(HTTWeights, bool, do_tt_muon_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_id_weights)
  CLASS_MEMBER(HTTWeights, bool, do_tau_id_sf)
  CLASS_MEMBER(HTTWeights, bool, do_em_qcd_weights)
  CLASS_MEMBER(HTTWeights, TH2D*, z_pt_mass_hist)
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
  CLASS_MEMBER(HTTWeights, TH2D*, et_antiiso1_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, et_antiiso2_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, et_xtrig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, et_xtrig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, et_conditional_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, et_conditional_data)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_trig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_antiiso1_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_antiiso2_trig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_xtrig_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_xtrig_data)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_conditional_mc)
  CLASS_MEMBER(HTTWeights, TH2D*, mt_conditional_data)
  CLASS_MEMBER(HTTWeights, TH2D*, em_qcd_cr1_lt2)
  CLASS_MEMBER(HTTWeights, TH2D*, em_qcd_cr1_2to4)
  CLASS_MEMBER(HTTWeights, TH2D*, em_qcd_cr1_gt4)
  CLASS_MEMBER(HTTWeights, TH2D*, em_qcd_cr2_lt2)
  CLASS_MEMBER(HTTWeights, TH2D*, em_qcd_cr2_2to4)
  CLASS_MEMBER(HTTWeights, TH2D*, em_qcd_cr2_gt4)
  CLASS_MEMBER(HTTWeights, TH2F*, ele_tracking_sf)
  CLASS_MEMBER(HTTWeights, TH1D*, muon_tracking_sf)
  CLASS_MEMBER(HTTWeights, std::string, scalefactor_file)
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
  std::shared_ptr<RooWorkspace> w_;
  mithep::TH2DAsymErr* MuonFakeRateHist_PtEta;
  mithep::TH2DAsymErr* ElectronFakeRateHist_PtEta;
  BTagWeight btag_weight;
  TF1 *tau_fake_weights_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;


 public:
  HTTWeights(std::string const& name);
  virtual ~HTTWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  double Efficiency(double m, double m0, double sigma, double alpha, double n, double norm);
};

}

#undef MEMBER_NP

#endif
