#ifndef ICHiggsTauTau_Module_HinvWDecay_h
#define ICHiggsTauTau_Module_HinvWDecay_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>


namespace ic {

class HinvWDecay : public ModuleBase {
 private:

    unsigned flavour_;


 public:
    HinvWDecay(std::string const& name, unsigned flavour);
    virtual ~HinvWDecay();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
};

}

#endif
