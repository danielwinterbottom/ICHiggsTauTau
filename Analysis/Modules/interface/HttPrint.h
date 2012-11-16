#ifndef ICHiggsTauTau_Module_HttPrint_h
#define ICHiggsTauTau_Module_HttPrint_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "TTree.h"
#include "TFile.h"



#include <string>

namespace ic {

class HttPrint : public ModuleBase {
 private:
  std::set<unsigned> events_;

 public:
  HttPrint(std::string const& name);
  virtual ~HttPrint();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void PrintEvent(unsigned evt);



};

}

#endif
