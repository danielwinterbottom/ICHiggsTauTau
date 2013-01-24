#ifndef ICHiggsTauTau_Module_MetSelection_h
#define ICHiggsTauTau_Module_MetSelection_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

  class MetSelection : public ModuleBase {
  private:
    std::string input_name_;
    double min_;
    double max_;


  public:
    MetSelection(std::string const& name, std::string input_name, double min, double max=14000.);
    virtual ~MetSelection();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
