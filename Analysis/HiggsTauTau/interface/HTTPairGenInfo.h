#ifndef ICHiggsTauTau_Module_HTTPairGenInfo_h
#define ICHiggsTauTau_Module_HTTPairGenInfo_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


#include <string>

namespace ic {
  
class HTTPairGenInfo : public ModuleBase {
 private:
  CLASS_MEMBER(HTTPairGenInfo, std::string, ditau_label)
  CLASS_MEMBER(HTTPairGenInfo, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTPairGenInfo, bool, write_plots)
  std::vector<Dynamic2DHistoSet *> hists_;

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
