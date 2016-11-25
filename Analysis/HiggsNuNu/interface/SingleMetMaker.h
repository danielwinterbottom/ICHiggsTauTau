#ifndef ICHiggsTauTau_Module_SingleMetMaker_h
#define ICHiggsTauTau_Module_SingleMetMaker_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include <string>

namespace ic {
  
  class SingleMetMaker : public ModuleBase {
  private:
    std::string met_name_;

  public:
    SingleMetMaker(std::string const& name, std::string met_name);
    virtual ~SingleMetMaker();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
