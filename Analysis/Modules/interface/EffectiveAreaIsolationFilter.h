#ifndef ICHiggsTauTau_Module_EffectiveAreaIsolationFilter_h
#define ICHiggsTauTau_Module_EffectiveAreaIsolationFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

  class EffectiveAreaIsolationFilter : public ModuleBase {
  private:
    std::string input_name_;
    double max_;
    
  public:
    EffectiveAreaIsolationFilter(std::string const& name, std::string input_name, double max);
    virtual ~EffectiveAreaIsolationFilter();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();


  };

}//namespace


#endif
