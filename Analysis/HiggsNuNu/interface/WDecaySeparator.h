#ifndef ICHiggsTauTau_Module_WDecaySeparator_h
#define ICHiggsTauTau_Module_WDecaySeparator_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>


namespace ic {

class WDecaySeparator : public ModuleBase {
 private:

    unsigned flavour_;
    unsigned countW_;
    unsigned countPrompt_e_;
    unsigned countPrompt_mu_;
    unsigned countPrompt_tau_;
    unsigned countTau_e_;
    unsigned countTau_mu_;
    unsigned countTau_had_;
    CLASS_MEMBER(WDecaySeparator, bool, do_wgammafilter)

 public:
    WDecaySeparator(std::string const& name, unsigned flavour);
    virtual ~WDecaySeparator();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    bool processParticle(const std::vector<bool> & flags,
			 unsigned & countPrompt,
			 unsigned & countTau,
			 unsigned & countPromptTau);
};

}

#endif
