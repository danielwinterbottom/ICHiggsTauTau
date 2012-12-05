#ifndef ICHiggsTauTau_Module_HttZeroJetCategory_h
#define ICHiggsTauTau_Module_HttZeroJetCategory_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {

class HttZeroJetCategory : public ModuleBase {
 private:
  CLASS_MEMBER(HttZeroJetCategory, bool, do_met_cut)
  CLASS_MEMBER(HttZeroJetCategory, double, met_cut)
  CLASS_MEMBER(HttZeroJetCategory, bool, high_pt)
  CLASS_MEMBER(HttZeroJetCategory, bool, no_pt_split)
  CLASS_MEMBER(HttZeroJetCategory, fwlite::TFileService*, fs)
  CLASS_MEMBER(HttZeroJetCategory, ic::channel, channel)
  CLASS_MEMBER(HttZeroJetCategory, std::string, met_label)
  CLASS_MEMBER(HttZeroJetCategory, std::string, jets_label)
  CLASS_MEMBER(HttZeroJetCategory, double, veto_jet_pt)
  CLASS_MEMBER(HttZeroJetCategory, double, veto_jet_eta)
  CLASS_MEMBER(HttZeroJetCategory, bool, do_btag_weight)


  
  std::vector<HttPlots *> zerojet_plots_;
  std::vector<double> zerojet_yields_;
  std::vector<double> mass_yields_;


 public:
  HttZeroJetCategory(std::string const& name);
  virtual ~HttZeroJetCategory();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}



#endif
