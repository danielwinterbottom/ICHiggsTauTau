#ifndef ICHiggsTauTau_Module_MakeRunStats_h
#define ICHiggsTauTau_Module_MakeRunStats_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

  struct RunStats{
    unsigned nEvents;
    unsigned nVertices;
    RunStats(){
      nEvents = 0;
      nVertices = 0;
    }
    void IncEvents(){
      ++nEvents;
    }
    void IncVertices(unsigned v){
      nVertices += v;
    }
  };

class MakeRunStats : public ModuleBase {

 private:
  std::map<unsigned,RunStats> run_yield_map_;
  std::string output_name_;


 public:
  MakeRunStats(std::string const& name, std::string const& output_name);
  virtual ~MakeRunStats();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
