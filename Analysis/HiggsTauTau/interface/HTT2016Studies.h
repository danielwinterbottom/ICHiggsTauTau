#ifndef HiggsTauTau_HTT2016Studies_h
#define HiggsTauTau_HTT2016Studies_h
#include <string>
#include <cstdint>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/Candidate.hh"
#include "Objects/interface/CompositeCandidate.hh"
#include "Objects/interface/TriggerObject.hh"
#include "HiggsTauTau/interface/HTTPairGenInfo.h"

namespace ic {

bool HLTPathCheck(ic::TreeEvent* event, std::string const& label,
                  std::string const& path);

//union ui64 {
//   uint64_t one;
//   int16_t four[4];
// };

std::set<int16_t> GetTriggerTypes(ic::TriggerObject* obj);

bool SortByIsoMT(CompositeCandidate const* c1, CompositeCandidate const* c2);

void CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift);

class ZmmTreeProducer : public ModuleBase {
 private:
  CLASS_MEMBER(ZmmTreeProducer, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZmmTreeProducer, std::string, sf_workspace)
  TTree *outtree_;
  std::shared_ptr<RooWorkspace> ws_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;

  float wt;
  float wt_pu;
  float wt_trk;
  float wt_id;
  float wt_iso;
  float wt_trg;

  int n_vtx;

  bool os;

  float pt_1;
  float eta_1;
  float phi_1;
  float iso_1;

  float pt_2;
  float eta_2;
  float phi_2;
  float iso_2;

  float m_ll;
  float pt_ll;
  float dr_ll;

  bool trg_IsoMu22;
  bool trg_IsoTkMu22;

 public:
  ZmmTreeProducer(std::string const& name);
  virtual ~ZmmTreeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

class ZeeTreeProducer : public ModuleBase {
 private:
  CLASS_MEMBER(ZeeTreeProducer, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZeeTreeProducer, std::string, sf_workspace)
  TTree *outtree_;
  std::shared_ptr<RooWorkspace> ws_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;

  float wt;
  float wt_trk;
  float wt_pu;
  float wt_id;
  float wt_iso;
  float wt_trg;

  int n_vtx;

  bool os;

  float pt_1;
  float eta_1;
  float phi_1;
  float iso_1;

  float pt_2;
  float eta_2;
  float phi_2;
  float iso_2;

  float m_ll;
  float pt_ll;
  float dr_ll;

  bool trg_Ele25eta2p1WPTight;

 public:
  ZeeTreeProducer(std::string const& name);
  virtual ~ZeeTreeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

class ZmmTPTreeProducer : public ModuleBase {
 private:
  CLASS_MEMBER(ZmmTPTreeProducer, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZmmTPTreeProducer, std::string, sf_workspace)
  TTree *outtree_;
  std::shared_ptr<RooWorkspace> ws_;

  float wt;

  int n_vtx;
  int run;

  float pt_t;
  float eta_t;
  float phi_t;
  bool id_t;
  float iso_t;

  bool muon_p;
  float pt_p;
  float eta_p;
  float phi_p;
  bool id_p;
  float iso_p;

  float m_ll;

  bool trg_t_IsoMu22;
  bool trg_t_IsoMu19Tau; // Did something fire the L1 tau?
  bool trg_p_IsoMu22;
  bool trg_p_IsoTkMu22;
  bool trg_p_PFTau120;
  bool trg_p_IsoMu19TauL1;
  bool trg_p_IsoMu19Tau;

 public:
  ZmmTPTreeProducer(std::string const& name);
  virtual ~ZmmTPTreeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

class ZeeTPTreeProducer : public ModuleBase {
 private:
  CLASS_MEMBER(ZeeTPTreeProducer, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZeeTPTreeProducer, std::string, sf_workspace)
  TTree *outtree_;
  std::shared_ptr<RooWorkspace> ws_;

  float wt;

  int n_vtx;
  int run;

  float pt_t;
  float eta_t;
  float phi_t;
  bool id_t;
  float iso_t;

  float pt_p;
  float eta_p;
  float sc_eta_p;
  float phi_p;
  bool id_p;
  float iso_p;

  float m_ll;

  bool trg_t_Ele25eta2p1WPTight;
  bool trg_t_Ele24Tau; // Did something fire the L1 tau?
  bool trg_p_Ele25eta2p1WPTight;
  bool trg_p_PFTau120;
  bool trg_p_Ele24TauL1;
  bool trg_p_Ele24Tau;

 public:
  ZeeTPTreeProducer(std::string const& name);
  virtual ~ZeeTPTreeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

class ZmtTPTreeProducer : public ModuleBase {
 private:
  CLASS_MEMBER(ZmtTPTreeProducer, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZmtTPTreeProducer, std::string, sf_workspace)
  TTree *outtree_;
  std::shared_ptr<RooWorkspace> ws_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;

  HTTPairGenInfo geninfo_module_;

  float wt;
  float wt_pu_hi;

  int n_vtx;

  float pt_m;
  float eta_m;
  float mt_m;
  float pzeta;

  float pt_t;
  float eta_t;
  int dm_t;
  bool anti_e_t;
  bool anti_m_t;
  bool mva_vl_t;
  bool mva_l_t;
  bool mva_m_t;
  bool mva_t_t;
  bool mva_vt_t;
  bool mva_vvt_t;
  bool cmb_l_t;
  bool cmb_m_t;
  bool cmb_t_t;
  float cbiso_t;
  float cbiso_0p5_t;
  float cbiso_1p0_t;
  float cbiso_1p5_t;
  float cbiso_2p0_t;
  float chiso_t;
  float ntiso_t;
  float ntiso_0p5_t;
  float ntiso_1p0_t;
  float ntiso_1p5_t;
  float ntiso_2p0_t;
  float puiso_t;
  float pho_out_t;
  float pho_out_0p0_t;
  float pho_out_0p5_t;
  float pho_out_1p0_t;
  float pho_out_1p5_t;
  float pho_out_2p0_t;

  float nt_density_0p0_0p1;
  float nt_density_0p1_0p2;
  float nt_density_0p2_0p3;
  float nt_density_0p3_0p4;
  float nt_density_0p4_0p5;

  unsigned n_iso_ph_0p5;
  unsigned n_sig_ph_0p5;
  unsigned n_iso_ph_1p0;
  unsigned n_sig_ph_1p0;
  unsigned n_iso_ph_1p5;
  unsigned n_sig_ph_1p5;
  unsigned n_iso_ph_2p0;
  unsigned n_sig_ph_2p0;

  bool trg_m_IsoMu19TauL1;
  bool trg_t_IsoMu19TauL1;
  bool trg_m_IsoMu19Tau;
  bool trg_t_IsoMu19Tau;

  unsigned gen_1;
  unsigned gen_2;

  int n_bjets;

  bool os;
  float m_ll;

 public:
  ZmtTPTreeProducer(std::string const& name);
  virtual ~ZmtTPTreeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
};

}


#endif
