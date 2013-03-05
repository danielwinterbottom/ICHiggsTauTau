#ifndef ICHiggsTauTau_Module_ModifyMet_h
#define ICHiggsTauTau_Module_ModifyMet_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include <string>

namespace ic {
  
  class ModifyMet : public ModuleBase {
  private:
    std::string met_name_;
    std::string lep_name_;
    unsigned lepFlavour_;
    unsigned nLepToAdd_;

    template<class T> void correctMet(TreeEvent *event, Candidate::Vector & aVec){
     
      std::vector<T*> lLeps = event->GetPtrVec<T>(lep_name_);
      std::sort(lLeps.begin(), lLeps.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
 
      unsigned nLep = (nLepToAdd_ > lLeps.size()) ? lLeps.size() : nLepToAdd_;
      for (unsigned iL(0); iL<nLep;++iL){
	aVec += lLeps[iL]->vector();
      }
    };

  public:
    ModifyMet(std::string const& name, std::string met_name, std::string lep_name, 
	      unsigned lepFlavour, unsigned nLepToAdd);
    virtual ~ModifyMet();



    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
