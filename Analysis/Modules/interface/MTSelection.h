#ifndef ICHiggsTauTau_Module_MTSelection_h
#define ICHiggsTauTau_Module_MTSelection_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

  class MTSelection : public ModuleBase {
  private:
    std::string input_name_;
    std::string lepton_name_;
    double min_;
    double max_;

  public:
    MTSelection(std::string const& name, std::string input_name, std::string lepton_name_, double min, double max=14000.);
    virtual ~MTSelection();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
