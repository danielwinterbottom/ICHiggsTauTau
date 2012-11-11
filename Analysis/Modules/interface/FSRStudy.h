#ifndef ICHiggsTauTau_Module_FSRStudy_h
#define ICHiggsTauTau_Module_FSRStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"


#include <string>
#include <map>

namespace ic {

class FSRStudy : public ModuleBase {
 private:

  CLASS_MEMBER(FSRStudy, double, st1_pt1)
  CLASS_MEMBER(FSRStudy, double, st1_pt2)
  CLASS_MEMBER(FSRStudy, double, st1_eta)
  CLASS_MEMBER(FSRStudy, fwlite::TFileService*, fs)
  DynamicHistoSet* hists_;

 public:
  FSRStudy(std::string const& name);
  virtual ~FSRStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void PrintEff(std::string name, double num, double den);
  void PrintEffRatio(std::string name, double num, double den);

};

}

#endif
