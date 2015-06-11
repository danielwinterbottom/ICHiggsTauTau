#ifndef HiggsTauTau_ToyTauStudy_h
#define HiggsTauTau_ToyTauStudy_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "TH1F.h"
#include "TH2F.h"

#include <string>

namespace ic {


class ToyTauStudy : public ModuleBase {
 private:
  struct EfficiencyPlot1D {
    TH1F* all;
    TH1F* pass;

    EfficiencyPlot1D() {
      all = nullptr;
      pass = nullptr;
    }

    void Fill(double val, bool passes) {
      all->Fill(val);
      if (passes) pass->Fill(val);
    }

    EfficiencyPlot1D(TFileDirectory dir, std::string const &name, unsigned nbins,
                     double lo, double hi) {
      all = dir.make<TH1F>((name + "_all").c_str(), "", nbins, lo, hi);
      pass = dir.make<TH1F>((name + "_pass").c_str(), "", nbins, lo, hi);
    }
  };

  struct MainPlots {
    MainPlots() = default;
    MainPlots(TFileDirectory dir);

    EfficiencyPlot1D rec_eff_vs_pt;
    EfficiencyPlot1D rec_dm_eff_vs_pt;
    EfficiencyPlot1D rec_dm_chiso0p8_eff_vs_pt;
    EfficiencyPlot1D rec_dm_chiso1p0_eff_vs_pt;
    EfficiencyPlot1D rec_dm_chiso2p0_eff_vs_pt;
    EfficiencyPlot1D rec_dm_cmbiso0p8_eff_vs_pt;
    EfficiencyPlot1D rec_dm_cmbiso1p0_eff_vs_pt;
    EfficiencyPlot1D rec_dm_cmbiso2p0_eff_vs_pt;

    EfficiencyPlot1D rec_dm_cmbiso2p0_nh2p0_eff_vs_pt;
    EfficiencyPlot1D rec_dm_cmbiso2p0_nh5p0_eff_vs_pt;
    EfficiencyPlot1D rec_dm_cmbiso2p0_nh10p0_eff_vs_pt;
    EfficiencyPlot1D rec_dm_cmbiso2p0_nh10p0_scaled_eff_vs_pt;

    EfficiencyPlot1D rec_dm_cmbiso0p8_eff_vs_otpu;
    EfficiencyPlot1D rec_dm_cmbiso1p0_eff_vs_otpu;
    EfficiencyPlot1D rec_dm_cmbiso2p0_eff_vs_otpu;

    EfficiencyPlot1D rec_chiso0p8_gvn_dm_eff_vs_pt;
    EfficiencyPlot1D rec_chiso1p0_gvn_dm_eff_vs_pt;
    EfficiencyPlot1D rec_chiso2p0_gvn_dm_eff_vs_pt;
    EfficiencyPlot1D rec_cmbiso0p8_gvn_dm_eff_vs_pt;
    EfficiencyPlot1D rec_cmbiso1p0_gvn_dm_eff_vs_pt;
    EfficiencyPlot1D rec_cmbiso2p0_gvn_dm_eff_vs_pt;

    TH1F *ch_iso;
    TH1F *cmb_iso;

    TH1F *cmb_iso_gvn_nh_iso_2p0;
    TH1F *cmb_iso_gvn_nh_iso_5p0;
    TH1F *cmb_iso_gvn_nh_iso_10p0;
    TH1F *cmb_iso_gvn_nh_iso_10p0_scaled;

    TH1F *nh_iso_profile;
    TH1F *nh_iso_profile_den;

    TH1F *pt_resp;
    TH1F *gen_pt;
    TH1F *gen_eta;
    TH1F *rec_pt;
    TH1F *rec_eta;
  };

  CLASS_MEMBER(ToyTauStudy, bool, do_real_th_studies)
  CLASS_MEMBER(ToyTauStudy, bool, do_fake_th_studies)
  CLASS_MEMBER(ToyTauStudy, std::string, taus_label)
  CLASS_MEMBER(ToyTauStudy, std::string, pfcands_label)

 private:
  void DoRealThStudies(TreeEvent *event);
  void DoFakeThStudies(TreeEvent *event);

  void Fill(MainPlots & plots, Candidate const* gen_th_vis, Tau const* rec_th);
  void FillIso(MainPlots & plots, Tau const *rec_th);

  double th_pt_acc_;
  double th_eta_acc_;

  TFileDirectory * dir_;

  MainPlots mp_all_dm;
  MainPlots mp_dm0;
  MainPlots mp_dm1;
  MainPlots mp_dm10;

  std::map<Tau const*, std::vector<ic::PFCandidate *>> * iso_deps_;

  CLASS_MEMBER(ToyTauStudy, fwlite::TFileService*, fs)

 public:
  ToyTauStudy(std::string const& name);
  virtual ~ToyTauStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  bool QualityTrack(Track const* trk, Vertex const* vtx);
};

}

#endif
