#ifndef HiggsTauTau_HTT2016Studies_h
#define HiggsTauTau_HTT2016Studies_h
#include <string>
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"

namespace ic {

class ZmmTreeProducer : public ModuleBase {
 private:
  CLASS_MEMBER(ZmmTreeProducer, fwlite::TFileService*, fs)
  CLASS_MEMBER(ZmmTreeProducer, std::string, sf_workspace)
  TTree *outtree_;
  std::shared_ptr<RooWorkspace> ws_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;

  float wt;
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

  bool trg_IsoMu22;

 public:
  ZmmTreeProducer(std::string const& name);
  virtual ~ZmmTreeProducer();

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

  float pt_t;
  float eta_t;
  float phi_t;
  bool id_t;
  float iso_t;

  float pt_p;
  float eta_p;
  float phi_p;
  bool id_p;
  float iso_p;

  float m_ll;

  bool trg_t_IsoMu22;
  bool trg_p_IsoMu22;

 public:
  ZmmTPTreeProducer(std::string const& name);
  virtual ~ZmmTPTreeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};
}


#endif
