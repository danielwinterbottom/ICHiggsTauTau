#ifndef ICHiggsTauTau_HiggsTauTau_HTTSequence_h
#define ICHiggsTauTau_HiggsTauTau_HTTSequence_h

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/json.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

namespace ic {

class HTTSequence {
 private:
  std::shared_ptr<fwlite::TFileService> fs;
  std::vector<std::shared_ptr<ic::ModuleBase>> seq;
  std::string channel_str, var_str, strategy_str, era_str, mc_str;
  Json::Value js;
  double tau_pt, tau_eta, tau_iso, tau_dz;
  double elec_pt, elec_eta, elec_dxy, elec_dz;
  double muon_pt, muon_eta, muon_dxy, muon_dz;
  std::string tau_anti_elec_discr, tau_anti_muon_discr, tau_iso_discr;
  std::string output_folder, output_name, addit_output_folder, lumimask_output_name;
  double veto_elec_pt, veto_elec_eta, veto_elec_dxy, veto_elec_dz;
  double veto_muon_pt, veto_muon_eta, veto_muon_dxy, veto_muon_dz;
  double veto_dielec_pt, veto_dielec_eta, veto_dielec_dxy, veto_dielec_dz;
  double veto_dimuon_pt, veto_dimuon_eta, veto_dimuon_dxy, veto_dimuon_dz;
  unsigned min_taus, new_svfit_mode, kinfit_mode, mva_met_mode, faked_tau_selector, hadronic_tau_selector;
  unsigned pu_id_training, special_mode, btag_mode, bfake_mode, jes_mode, ztautau_mode, vh_filter_mode;
  std::string met_label, jets_label, svfit_folder, svfit_override, allowed_tau_modes;
  bool bjet_regr_correction, tau_scale_mode, make_sync_ntuple, moriond_tau_scale;
  bool is_data, is_embedded, real_tau_sample;
  double pair_dr, tau_shift, mass_shift;

 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  HTTSequence(std::string & chan, std::string postf, Json::Value const& js);
  HTTSequence() = default;
  ~HTTSequence();
  ModuleSequence* getSequence(){return &seq;}
  void BuildSequence();
  void BuildETPairs();
  void BuildMTPairs();
  void BuildEMPairs();
  void BuildTTPairs();
  void BuildZEEPairs();
  void BuildZMMPairs();
  void BuildTPZEEPairs();
  void BuildTPZMMPairs();
  void BuildWMuNu();

  void BuildTauSelection();

  void BuildDiElecVeto();
  void BuildDiMuonVeto();

  void BuildExtraMuonVeto();
  void BuildExtraElecVeto();


  template<class T>
  void BuildModule(T const& mod) {
     seq.push_back(std::shared_ptr<ModuleBase>(new T(mod)));
  }
};
}

#endif
