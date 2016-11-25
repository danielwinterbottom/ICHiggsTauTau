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
    std::string jet_name_;
    unsigned lepton_flavour_;
    double min_;
    double max_;
    bool doZeppenfeldCut_;

  public:
    MTSelection(std::string const& name, std::string input_name, std::string lepton_name, unsigned lepton_flavour, double min, double max=14000., bool doZeppenfeldCut=false, std::string jet_name="");
    virtual ~MTSelection();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
