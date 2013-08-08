#ifndef ICHiggsTauTau_Module_HTTPrint_h
#define ICHiggsTauTau_Module_HTTPrint_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "TTree.h"
#include "TFile.h"



#include <string>

namespace ic {

class HTTPrint : public ModuleBase {
 private:
  std::set<unsigned> events_;
  CLASS_MEMBER(HTTPrint, bool, skip_events)
 public:
  HTTPrint(std::string const& name);
  virtual ~HTTPrint();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void PrintEvent(unsigned evt);



};

}

#endif
