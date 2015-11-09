#ifndef HiggsTauTau_HTTMuonEfficiency_h
#define HiggsTauTau_HTTMuonEfficiency_h

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

class HTTMuonEfficiency : public ModuleBase {
 private:
  TFileDirectory * dir_;
  TTree * outtree_;
  double e_mva_id_;
  double eta_;
  double pt_;
  double allcharged03iso_;
  double allcharged04iso_;
  unsigned gen_match_;
  double iso_ea03_;
  double iso_db03_;
  double iso_db03allch_;
  double iso_db04allch_;
  int wt_;
  



  CLASS_MEMBER(HTTMuonEfficiency, fwlite::TFileService*, fs)

 public:
  HTTMuonEfficiency(std::string const& name);
  virtual ~HTTMuonEfficiency();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
