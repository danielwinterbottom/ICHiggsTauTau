#ifndef HGCAL_HGCALAnalysis_h
#define HGCAL_HGCALAnalysis_h
#include <string>
#include <cstdint>
#include "TGraph2D.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/Candidate.hh"
#include "Objects/interface/GenParticle.hh"
#include "Objects/interface/GenJet.hh"
#include "Objects/interface/CompositeCandidate.hh"
#include "Objects/interface/TriggerObject.hh"
#include "Utilities/interface/FnPairs.h"
#include "HGCAL/interface/HGCALTools.h"
#include "fastjet/ClusterSequence.hh"

namespace ic {

class HGCALObjectBuilder : public ModuleBase {
 private:
 public:
  HGCALObjectBuilder(std::string const &name);
  virtual ~HGCALObjectBuilder();
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
};


struct EvtVars {
  unsigned evt;
  unsigned lumi;
  int pm;

  void AddToTree(TTree *t, TString prefix) {
    t->Branch(prefix+"evt",          &evt);
    t->Branch(prefix+"lumi",         &lumi);
    t->Branch(prefix+"pm",         &pm);
  }
};

struct TauRecoVars {
  float jet_pt;
  float jet_eta;
  float jet_phi;
  float jet_e;
  float nprongs;
  float jet_mass;
  float all_prong_mass;

  void AddToTree(TTree *t, TString prefix) {
    t->Branch(prefix+"jet_pt",          &jet_pt);
    t->Branch(prefix+"jet_eta",         &jet_eta);
    t->Branch(prefix+"jet_phi",         &jet_phi);
    t->Branch(prefix+"jet_e",           &jet_e);
    t->Branch(prefix+"nprongs",         &nprongs);
    t->Branch(prefix+"jet_mass",        &jet_mass);
    t->Branch(prefix+"all_prong_mass",  &all_prong_mass);
  }
};

struct TauGenVars {
  int dm;
  float pt;
  float eta;
  float vis_pt;
  float vis_eta;

  void AddToTree(TTree *t, TString prefix) {
    t->Branch(prefix+"dm",         &dm);
    t->Branch(prefix+"pt",         &pt);
    t->Branch(prefix+"eta",        &eta);
    t->Branch(prefix+"vis_pt",     &vis_pt);
    t->Branch(prefix+"vis_eta",    &vis_eta);
  }
};

struct TauMode1GenVars {
  bool all_reached_ee;
  bool pi_reached_ee;
  bool p1_reached_ee;
  bool p2_reached_ee;
  float pi_e;
  float p1_e;
  float p2_e;
  float p1_p2_dxy;
  bool has_gen_ele_pair;
  float mass;
  bool pi_match_simcluster;
  bool p1_match_simcluster;
  bool p2_match_simcluster;
  float pi_hfrac;
  float p1_hfrac;
  float p2_hfrac;

  void AddToTree(TTree *t, TString prefix) {
    t->Branch(prefix+"all_reached_ee",      &all_reached_ee);
    t->Branch(prefix+"pi_reached_ee",       &pi_reached_ee);
    t->Branch(prefix+"p1_reached_ee",       &p1_reached_ee);
    t->Branch(prefix+"p2_reached_ee",       &p2_reached_ee);
    t->Branch(prefix+"pi_e",                &pi_e);
    t->Branch(prefix+"p1_e",                &p1_e);
    t->Branch(prefix+"p2_e",                &p2_e);
    t->Branch(prefix+"p1_p2_dxy",           &p1_p2_dxy);
    t->Branch(prefix+"has_gen_ele_pair",    &has_gen_ele_pair);
    t->Branch(prefix+"mass",                &mass);
    t->Branch(prefix+"pi_match_simcluster",     &pi_match_simcluster);
    t->Branch(prefix+"p1_match_simcluster",     &p1_match_simcluster);
    t->Branch(prefix+"p2_match_simcluster",     &p2_match_simcluster);
    t->Branch(prefix+"pi_hfrac",                &pi_hfrac);
    t->Branch(prefix+"p1_hfrac",                &p1_hfrac);
    t->Branch(prefix+"p2_hfrac",                &p2_hfrac);
  }

  void Reset() {
    all_reached_ee = false;
    pi_reached_ee = false;
    p1_reached_ee = false;
    p2_reached_ee = false;
    pi_e = 0.;
    p1_e = 0.;
    p2_e = 0.;
    p1_p2_dxy = 0.;
    has_gen_ele_pair = false;
    mass = 0.;
    pi_match_simcluster = false;
    p1_match_simcluster = false;
    p2_match_simcluster = false;
    pi_hfrac = 0.;
    p1_hfrac = 0.;
    p2_hfrac = 0.;
  };
};

struct TauMode1MatchVars {
  bool matched;
  int nmatched_to_pi;
  int nmatched_to_p1;
  int nmatched_to_p2;
  float pi_bestmatch_e;
  float p1_bestmatch_e;
  float p2_bestmatch_e;
  float pi_bestmatch_dr;
  float p1_bestmatch_dr;
  float p2_bestmatch_dr;
  float pi_bestmatch_hfrac;
  float p1_bestmatch_hfrac;
  float p2_bestmatch_hfrac;
  int pi_n_ecal_hits;
  int pi_n_hcal_hits;
  int p1_n_ecal_hits;
  int p1_n_hcal_hits;
  int p2_n_ecal_hits;
  int p2_n_hcal_hits;

  void AddToTree(TTree *t, TString prefix) {
    t->Branch(prefix+"matched",              &matched);
    t->Branch(prefix+"nmatched_to_pi",       &nmatched_to_pi);
    t->Branch(prefix+"nmatched_to_p1",       &nmatched_to_p1);
    t->Branch(prefix+"nmatched_to_p2",       &nmatched_to_p2);
    t->Branch(prefix+"pi_bestmatch_e",       &pi_bestmatch_e);
    t->Branch(prefix+"p1_bestmatch_e",       &p1_bestmatch_e);
    t->Branch(prefix+"p2_bestmatch_e",       &p2_bestmatch_e);
    t->Branch(prefix+"pi_bestmatch_dr",       &pi_bestmatch_dr);
    t->Branch(prefix+"p1_bestmatch_dr",       &p1_bestmatch_dr);
    t->Branch(prefix+"p2_bestmatch_dr",       &p2_bestmatch_dr);
    t->Branch(prefix+"pi_bestmatch_hfrac",       &pi_bestmatch_hfrac);
    t->Branch(prefix+"p1_bestmatch_hfrac",       &p1_bestmatch_hfrac);
    t->Branch(prefix+"p2_bestmatch_hfrac",       &p2_bestmatch_hfrac);
    t->Branch(prefix+"pi_n_ecal_hits",       &pi_n_ecal_hits);
    t->Branch(prefix+"pi_n_hcal_hits",       &pi_n_hcal_hits);
    t->Branch(prefix+"p1_n_ecal_hits",       &p1_n_ecal_hits);
    t->Branch(prefix+"p1_n_hcal_hits",       &p1_n_hcal_hits);
    t->Branch(prefix+"p2_n_ecal_hits",       &p2_n_ecal_hits);
    t->Branch(prefix+"p2_n_hcal_hits",       &p2_n_hcal_hits);
  }

  void Reset() {
    matched = false;
    nmatched_to_pi = 0;
    nmatched_to_p1 = 0;
    nmatched_to_p2 = 0;
    pi_bestmatch_e = 0.;
    p1_bestmatch_e = 0.;
    p2_bestmatch_e = 0.;
    pi_bestmatch_dr = 0.;
    p1_bestmatch_dr = 0.;
    p2_bestmatch_dr = 0.;
    pi_bestmatch_hfrac = 0.;
    p1_bestmatch_hfrac = 0.;
    p2_bestmatch_hfrac = 0.;
    pi_n_ecal_hits = 0;
    pi_n_hcal_hits = 0;
    p1_n_ecal_hits = 0;
    p1_n_hcal_hits = 0;
    p2_n_ecal_hits = 0;
    p2_n_hcal_hits = 0;
  }
};

class HGCALTest : public ModuleBase {
 private:
  CLASS_MEMBER(HGCALTest, fwlite::TFileService*, fs)
  CLASS_MEMBER(HGCALTest, bool, do_fakes)
  // TTree * t_jets_;
  // float jet_pt_;
  // float jet_eta_;
  // float jet_phi_;
  // float jet_e_;
  // bool gen_matched_;
  // float genjet_pt_;
  // float genjet_eta_;
  // float genjet_phi_;
  // float genjet_e_;
  // bool gen_nonu_matched_;
  // float genjet_nonu_pt_;
  // float genjet_nonu_eta_;
  // float genjet_nonu_phi_;
  // float genjet_nonu_e_;
  // float efrac_r0p02_;
  // float efrac_r0p04_;
  // float efrac_r0p06_;
  // float efrac_r0p08_;
  // float efrac_r0p10_;
  // bool isDM_r0p02_;
  // bool isDM_r0p04_;
  // bool isDM_r0p06_;
  // bool isDM_r0p08_;
  // bool isDM_r0p10_;
  // bool is_hadtau_;
  // int tau_dm_;

  TTree *t_taus_;
  EvtVars t_taus_evt_;
  TauRecoVars t_taus_rec_;
  TauGenVars t_taus_gen_;
  TauMode1GenVars t_taus_m1_gen_;
  TauMode1MatchVars t_taus_m1_match_;

  // int tt_dm_;
  // unsigned tt_reached_ee_;
  // unsigned tt_pion_reached_ee_;
  // float tt_tau_pt_;
  // float tt_tau_eta_;
  // float tt_vis_tau_pt_;
  // float tt_vis_tau_eta_;
  // float tt_dm1_pho1_e_;
  // float tt_dm1_pho2_e_;
  // float tt_dm1_pion_e_;
  // float tt_pho_dxy_l1_;
  // float tt_prods_dr_;
  // unsigned tt_evt_;
  // int tt_pm_;
  // unsigned tt_rec_prongs_;
  // float tt_jet_mass_;
  // float tt_mass_;

 public:
  HGCALTest(std::string const &name);
  virtual ~HGCALTest();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
};

}
#endif
