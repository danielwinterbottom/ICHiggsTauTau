#ifndef ICHiggsTauTau_Module_HinvWDecay_h
#define ICHiggsTauTau_Module_HinvWDecay_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>


namespace ic {

class HinvWDecay : public ModuleBase {
 private:

    unsigned flavour_;
    bool isEmbedded_;
    unsigned countStatus3_;
    unsigned countDecay_e_;
    unsigned countDecay_mu_;
    unsigned countRest_;
    CLASS_MEMBER(HinvWDecay, bool, do_wgammafilter)
    CLASS_MEMBER(HinvWDecay, bool, do_newstatuscodes)
    CLASS_MEMBER(HinvWDecay, bool, do_statusflags)

 public:
    HinvWDecay(std::string const& name, unsigned flavour, bool isEmbedded=false);
    virtual ~HinvWDecay();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
};

}

#endif
