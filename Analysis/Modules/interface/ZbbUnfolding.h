#ifndef ICHiggsTauTau_Module_ZbbUnfolding_h
#define ICHiggsTauTau_Module_ZbbUnfolding_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include <string>
#include <map>

namespace ic {

class ZbbUnfolding : public ModuleBase {
 private:
  typedef ROOT::Math::SMatrix<double,3>             SMatrix33;
  typedef ROOT::Math::SMatrix<double,3,2>           SMatrix32;
  typedef ROOT::Math::SMatrix<double,2>             SMatrix22;
  typedef ROOT::Math::SVector<double,2>             SVector2; 

  CLASS_MEMBER(ZbbUnfolding, bool, pu_rw)
  CLASS_MEMBER(ZbbUnfolding, bool, tp_rw)
  CLASS_MEMBER(ZbbUnfolding, bool, btag_rw)
  CLASS_MEMBER(ZbbUnfolding, unsigned, mode)
  CLASS_MEMBER(ZbbUnfolding, double, z_mass_low)
  CLASS_MEMBER(ZbbUnfolding, double, z_mass_high)
  CLASS_MEMBER(ZbbUnfolding, double, gen_jet_pt)
  CLASS_MEMBER(ZbbUnfolding, double, gen_jet_eta)
  CLASS_MEMBER(ZbbUnfolding, double, gen_jet_gen_lepton_dr)
  CLASS_MEMBER(ZbbUnfolding, double, gen_jet_bhadron_dr)
  CLASS_MEMBER(ZbbUnfolding, double, gen_elec_pt)
  CLASS_MEMBER(ZbbUnfolding, double, gen_elec_eta)
  CLASS_MEMBER(ZbbUnfolding, double, gen_muon_pt)
  CLASS_MEMBER(ZbbUnfolding, double, gen_muon_eta)
  CLASS_MEMBER(ZbbUnfolding, double, gen_elec_pt_baseline)
  CLASS_MEMBER(ZbbUnfolding, double, gen_elec_eta_baseline)
  CLASS_MEMBER(ZbbUnfolding, double, gen_muon_pt_baseline)
  CLASS_MEMBER(ZbbUnfolding, double, gen_muon_eta_baseline)
  CLASS_MEMBER(ZbbUnfolding, double, gen_reco_elec_dr)
  CLASS_MEMBER(ZbbUnfolding, double, gen_reco_muon_dr)
  CLASS_MEMBER(ZbbUnfolding, double, reco_elec_pt)
  CLASS_MEMBER(ZbbUnfolding, double, reco_elec_eta)
  CLASS_MEMBER(ZbbUnfolding, double, reco_muon_pt)
  CLASS_MEMBER(ZbbUnfolding, double, reco_muon_eta)
  CLASS_MEMBER(ZbbUnfolding, double, reco_jet_pt)
  CLASS_MEMBER(ZbbUnfolding, double, reco_jet_eta)
  CLASS_MEMBER(ZbbUnfolding, double, reco_jet_lepton_dr)
  CLASS_MEMBER(ZbbUnfolding, double, reco_gen_jet_dr)

  std::map<std::string, double> counters_;
  SMatrix32 gen_mat;
  SMatrix32 gen_kin_mat;
  SMatrix33 e_r_mat;
  SMatrix32 e_b_HE_mat;
  SMatrix32 e_b_HP_mat;

  BTagWeight btag_weight;

 public:
  ZbbUnfolding(std::string const& name);
  virtual ~ZbbUnfolding();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  std::vector<GenParticle *> MakeFinalBHadronsCollection(std::vector<GenParticle *> const& partVec) const;
  void PrintEff(std::string name, double num, double den);
  void PrintEffRatio(std::string name, double num, double den);
  double ElectronIdIsoSF(Electron const* elec) const;
  double MuonIdIsoSF(Muon const* muon) const;
  double ElectronTriggerSF(Electron const* elec1, Electron const* elec2) const;
  double MuonTriggerSF(Muon const* muon1, Muon const* muon2) const;
};

}

#endif
