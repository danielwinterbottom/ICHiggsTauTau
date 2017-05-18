#ifndef ICHiggsTauTau_HiggsTauTau_WMuNJetTauFakes_h
#define ICHiggsTauTau_HiggsTauTau_EMJetTauFakes_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

namespace ic {

class EMJetTauFakes : public ModuleBase {

 private:
  CLASS_MEMBER(EMJetTauFakes, std::string, muon_label)
  CLASS_MEMBER(EMJetTauFakes, std::string, jets_label)
  CLASS_MEMBER(EMJetTauFakes, bool, write_tree)
  CLASS_MEMBER(EMJetTauFakes, bool, is_data)
  CLASS_MEMBER(EMJetTauFakes, fwlite::TFileService*, fs)
 
  TTree *outtree_;
 
  bool tau_iso_vloose_; 
  bool tau_iso_loose_; 
  bool tau_iso_medium_; 
  bool tau_iso_tight_; 
  double tau_pt_;
  double tau_eta_;
  double jet_pt_;
  double jet_eta_;

 public:
  EMJetTauFakes(std::string const& name);
  virtual ~EMJetTauFakes();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();


};

}

#endif
