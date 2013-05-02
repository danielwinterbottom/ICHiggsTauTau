#ifndef ICHiggsTauTau_Module_HinvPrint_h
#define ICHiggsTauTau_Module_HinvPrint_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "TTree.h"
#include "TFile.h"



#include <string>

namespace ic {

class HinvPrint : public ModuleBase {
 private:
  std::set<unsigned> events_;

 public:
  HinvPrint(std::string const& name);
  virtual ~HinvPrint();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void PrintEvent(unsigned evt);



};

}

#endif
