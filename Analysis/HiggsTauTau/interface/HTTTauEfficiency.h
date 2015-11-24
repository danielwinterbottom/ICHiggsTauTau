#ifndef HiggsTauTau_HTTTauEfficiency_h
#define HiggsTauTau_HTTTauEfficiency_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "TH1F.h"
#include "TH2F.h"

#include <string>

namespace ic {

class HTTTauEfficiency : public ModuleBase {
 private:
  TFileDirectory * dir_;
  TTree * outtree_;
  double iso_mva_newDMwoLTraw_;
  double iso_mva_newDMwLTraw_;
  double iso_mva_oldDMwoLTraw_;
  double iso_mva_oldDMwLTraw_;
  double iso_dbetacorr_;
  double iso_pho_sum_pt_;
  unsigned dm_;
  bool against_mu_tight_; 
  bool against_mu_loose_; 
  bool against_ele_loose_;
  bool against_ele_medium_;
  bool against_ele_tight_;
  bool against_ele_vloose_;
  bool against_ele_vtight_;
  bool dm_finding_old_;
  bool dm_finding_new_;

  double eta_;
  double pt_;
  unsigned gen_match_;
  int wt_;



  CLASS_MEMBER(HTTTauEfficiency, fwlite::TFileService*, fs)

 public:
  HTTTauEfficiency(std::string const& name);
  virtual ~HTTTauEfficiency();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
