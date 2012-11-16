#ifndef ICHiggsTauTau_Module_MakeRunStats_h
#define ICHiggsTauTau_Module_MakeRunStats_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {


class MakeRunStats : public ModuleBase {


 private:
  std::map<unsigned, unsigned> yield_map_;
  std::map<unsigned, unsigned> vtx_map_;
  CLASS_MEMBER(MakeRunStats, std::string, output_name)


 public:
  MakeRunStats(std::string const& name);
  virtual ~MakeRunStats();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
