#ifndef ICHiggsTauTau_Module_GoodVertexFilter_h
#define ICHiggsTauTau_Module_GoodVertexFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class GoodVertexFilter : public ModuleBase {
 private:

 public:
  GoodVertexFilter(std::string const& name);
  virtual ~GoodVertexFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
