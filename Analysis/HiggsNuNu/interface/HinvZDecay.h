#ifndef ICHiggsTauTau_Module_HinvZDecay_h
#define ICHiggsTauTau_Module_HinvZDecay_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>


namespace ic {

class HinvZDecay : public ModuleBase {
 public:

    enum cutVar{
      mass=0,
      pt=1,
      ht=2
    };

    HinvZDecay(std::string const& name, unsigned flavour,
 	       double minMll=60., double maxMll=120.);
    HinvZDecay(std::string const& name, unsigned flavour,
	       double min, double max,cutVar variable, bool is2012=true);
    HinvZDecay(std::string const& name, unsigned flavour,
	       double min, double max,bool variable, bool is2012=true);
    virtual ~HinvZDecay();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

 private:

    unsigned flavour_;

    unsigned countStatus3_;
    double min_;
    double max_;
    cutVar variable_;
    bool is2012_;
    unsigned n0pt_;
    
};

}

#endif
