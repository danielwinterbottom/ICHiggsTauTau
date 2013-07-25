#ifndef ICHiggsTauTau_Module_MCFMStudy_h
#define ICHiggsTauTau_Module_MCFMStudy_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>


namespace ic {

class MCFMStudy : public ModuleBase {
 private:

    unsigned flavour_;

    unsigned countStatus3_;
    double minMll_;
    double maxMll_;
    unsigned nInMassRange_;
    unsigned nZjj_;
    unsigned nZjj_VBF_;
    unsigned nZjj_VBF_MET_;
    unsigned nZjj_VBF_MET_DPhi_;
    unsigned nZmm_;
    unsigned nZjjmm_noDR_;
    unsigned nZjjmm_;
    unsigned nZjjmm_VBF_;
    unsigned nZjjmm_VBF_MET_;
    unsigned nZjjmm_VBF_MET_DPhi_;

 public:
    MCFMStudy(std::string const& name, unsigned flavour,
	       double minMll=75., double maxMll=105.);
    virtual ~MCFMStudy();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
};

}

#endif
