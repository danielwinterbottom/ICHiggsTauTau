#ifndef ICHiggsTauTau_Module_HttSelection_h
#define ICHiggsTauTau_Module_HttSelection_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>

namespace ic {

class HttSelection : public ModuleBase {
 private:
  std::string jets_label_;
  CLASS_MEMBER(HttSelection, std::string, dilepton_label)
  CLASS_MEMBER(HttSelection, std::string, met_label)
  CLASS_MEMBER(HttSelection, double, mt_max_selection)
  CLASS_MEMBER(HttSelection, double, mt_min_control)
  CLASS_MEMBER(HttSelection, double, mt_max_control)
  CLASS_MEMBER(HttSelection, double, pzeta_min_selection)
  CLASS_MEMBER(HttSelection, double, pzeta_max_control)
  CLASS_MEMBER(HttSelection, double, pzeta_alpha)
  CLASS_MEMBER(HttSelection, ic::channel, channel)
  CLASS_MEMBER(HttSelection, bool, distinguish_os)
  CLASS_MEMBER(HttSelection, fwlite::TFileService*, fs)

  std::vector<HttPlots *> dilepton_plots_;
  std::vector<HttPlots *> inclusive_plots_;

  std::vector<double> dilepton_yields_;
  std::vector<double> inclusive_yields_;


 public:
  HttSelection(std::string const& name);
  virtual ~HttSelection();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
