#ifndef ICHiggsTauTau_Module_MetSelection_h
#define ICHiggsTauTau_Module_MetSelection_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include <string>

namespace ic {

  class MetSelection : public ModuleBase {
  private:
    std::string input_name_;
    std::string lepton_name_;
    bool addLeptons_;
    unsigned nLepToAdd_;
    double min_;
    double max_;

    template<class T> double correction(TreeEvent *event){
      double lCor = 0;
      std::vector<T*> lLeps = event->GetPtrVec<T>(lepton_name_);
      std::sort(lLeps.begin(), lLeps.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
 
      if (nLepToAdd_ > lLeps.size()) nLepToAdd_ = lLeps.size();
      for (unsigned iL(0); iL<nLepToAdd_;++iL){
	lCor += lLeps[iL]->pt();
      }
      return lCor;
    };
    
  public:
    MetSelection(std::string const& name, std::string input_name, double min, double max=14000.);
    MetSelection(std::string const& name, std::string input_name, std::string lepton_name, unsigned nLepToAdd, double min, double max=14000.);
    virtual ~MetSelection();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
