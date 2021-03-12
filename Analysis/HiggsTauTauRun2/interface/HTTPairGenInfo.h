#ifndef ICHiggsTauTau_Module_HTTPairGenInfo_h
#define ICHiggsTauTau_Module_HTTPairGenInfo_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "TGraph.h"
#include "TF2.h"

#include <string>

namespace ic {
  
class HTTPairGenInfo : public ModuleBase {
 private:
  CLASS_MEMBER(HTTPairGenInfo, std::string, ditau_label)
  CLASS_MEMBER(HTTPairGenInfo, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTPairGenInfo, bool, write_plots)
  CLASS_MEMBER(HTTPairGenInfo, bool, ngenjets)
  CLASS_MEMBER(HTTPairGenInfo, bool, is_embedded)
  CLASS_MEMBER(HTTPairGenInfo, ic::channel, channel)
  CLASS_MEMBER(HTTPairGenInfo, ic::era, era)
  std::vector<Dynamic2DHistoSet *> hists_;

  TFile *embed_corr_;
  TGraph *gr_met_corr_;
  TGraph *gr_met_corr_up_;
  TGraph *gr_met_corr_down_;
  TF2 *func;
  TF2 *func2;

 public:
  HTTPairGenInfo(std::string const& name);
  virtual ~HTTPairGenInfo();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

};

}

#endif
