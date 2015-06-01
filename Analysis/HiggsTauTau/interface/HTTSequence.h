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
  std::string channel_str;
  Json::Value js;
  double tau_pt, tau_eta, tau_iso, tau_dz;
  double elec_pt, elec_eta, elec_dxy, elec_dz;
  double muon_pt, muon_eta, muon_dxy, muon_dz;
  std::string tau_anti_elec, tau_anti_muon;
  double veto_elec_pt, veto_elec_eta, veto_elec_dxy, veto_elec_dz;
  double veto_muon_pt, veto_muon_eta, veto_muon_dxy, veto_muon_dz;
  double veto_dielec_pt, veto_dielec_eta, veto_dielec_dxy, veto_dielec_dz;
  double veto_dimuon_pt, veto_dimuon_eta, veto_dimuon_dxy, veto_dimuon_dz;
  unsigned min_taus;
  double pair_dr;

 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  HTTSequence(std::string & chan, std::string& var, Json::Value const& js);
  HTTSequence() = default;
  ~HTTSequence();
  ModuleSequence* getSequence(){return &seq;}
  void BuildSequence();
  void BuildETPairs();
  void BuildMTPairs();
  void BuildEMPairs();
  void BuildTTPairs();

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
