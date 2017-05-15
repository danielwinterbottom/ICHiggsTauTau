#ifndef ICHiggsTauTau_HiggsTauTau_WMuNJetTauFakes_h
#define ICHiggsTauTau_HiggsTauTau_WMuNuJetTauFakes_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

namespace ic {

class WMuNuJetTauFakes : public ModuleBase {

 private:
  CLASS_MEMBER(WMuNuJetTauFakes, std::string, muon_label)
  CLASS_MEMBER(WMuNuJetTauFakes, std::string, jets_label)
  CLASS_MEMBER(WMuNuJetTauFakes, bool, write_tree)
  CLASS_MEMBER(WMuNuJetTauFakes, bool, is_data)
  CLASS_MEMBER(WMuNuJetTauFakes, fwlite::TFileService*, fs)
 
  TTree *outtree_;
 
  bool tau_iso_medium_; 
  bool tau_iso_tight_; 
  double tau_pt_;
  double tau_eta_;
  double jet_pt_;
  double jet_eta_;

 public:
  WMuNuJetTauFakes(std::string const& name);
  virtual ~WMuNuJetTauFakes();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();


};

}

#endif
