#ifndef ICHiggsTauTau_Module_HttNoBTagCategory_h
#define ICHiggsTauTau_Module_HttNoBTagCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include <string>
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"



namespace ic {

class HttNoBTagCategory : public ModuleBase {
 private:
  double btag_jet_pt_;
  double btag_jet_eta_;

  std::string jets_label_;
  
  CLASS_MEMBER(HttNoBTagCategory, bool, do_met_cut)
  CLASS_MEMBER(HttNoBTagCategory, double, met_cut)
  CLASS_MEMBER(HttNoBTagCategory, fwlite::TFileService*, fs)
  CLASS_MEMBER(HttNoBTagCategory, ic::channel, channel)
  CLASS_MEMBER(HttNoBTagCategory, std::string, met_label)
  CLASS_MEMBER(HttNoBTagCategory, bool, do_btag_weight)

  
  std::vector<HttPlots *> nobtag_plots_;
  std::vector<double> nobtag_yields_;

 public:
  HttNoBTagCategory(std::string const& name, std::string const& jets_label);
  virtual ~HttNoBTagCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}


#endif
