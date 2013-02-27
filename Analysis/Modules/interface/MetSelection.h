#ifndef ICHiggsTauTau_Module_MetSelection_h
#define ICHiggsTauTau_Module_MetSelection_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

  class MetSelection : public ModuleBase {
  private:
    std::string input_name_;
    std::string lepton_name_;
    bool addLeptons_;
    double min_;
    double max_;

    template<class T> double correction(TreeEvent *event){
      double lCor = 0;
      std::vector<T*> lLeps = event->GetPtrVec<T>(lepton_name_);
      for (unsigned iL(0); iL<lLeps.size();++iL){
	lCor += lLeps[iL]->pt();
      }
      return lCor;
    };
    
  public:
    MetSelection(std::string const& name, std::string input_name, double min, double max=14000.);
    MetSelection(std::string const& name, std::string input_name, std::string lepton_name, double min, double max=14000.);
    virtual ~MetSelection();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
