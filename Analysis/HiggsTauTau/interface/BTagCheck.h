#ifndef ICHiggsTauTau_Module_BTagCheck_h
#define ICHiggsTauTau_Module_BTagCheck_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

#include <string>

namespace ic {

class BTagCheck : public ModuleBase {
 private:
  CLASS_MEMBER(BTagCheck, fwlite::TFileService*, fs)
  CLASS_MEMBER(BTagCheck, std::string, jet_label)
  CLASS_MEMBER(BTagCheck, bool, do_legacy)
  CLASS_MEMBER(BTagCheck,ic::channel, channel)
  CLASS_MEMBER(BTagCheck,std::string, dilepton_label)
  Dynamic2DHistoSet * hists_;
  DynamicHistoSet * hists1d_;
  TTree* outtree_;
  double pt;
  double eta;
  double wt;
  unsigned jet_flavour;
  bool gen_match;
  double csv;
  bool leptonveto;
  double iso_1;
  double iso_2; 
  bool os;
  bool antiele_pass;
  bool antimu_pass;

 public:
  BTagCheck(std::string const& name);
  virtual ~BTagCheck();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
