#ifndef HiggsTauTau_HTTElectronEfficiency_h
#define HiggsTauTau_HTTElectronEfficiency_h

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

class HTTElectronEfficiency : public ModuleBase {
 private:
  TFileDirectory * dir_;
  TTree * outtree_;
  double e_mva_id_;
  double eta_;
  double sc_eta;
  double pt_;
  double allcharged03iso_;
  double allcharged04iso_;
  double electronmvaID_;
  double electronTrigmvaID_;
  unsigned gen_match_;
  double iso_ea03_;
  double iso_db03_;
  double iso_db03allch_;
  double iso_db04allch_;
  bool pass_preselection;
  bool pass_cut_preselection;
  int wt_;
  



  CLASS_MEMBER(HTTElectronEfficiency, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTElectronEfficiency, std::string, dirname)

 public:
  HTTElectronEfficiency(std::string const& name);
  virtual ~HTTElectronEfficiency();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
