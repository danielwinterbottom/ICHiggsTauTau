#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MTSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {

  
  MTSelection::MTSelection(std::string const& name, 
			   std::string input_name,
			   std::string lepton_name,
			   double min, 
			   double max) : ModuleBase(name) {
    input_name_ = input_name;
    lepton_name_ = lepton_name;
    min_ = min;
    max_ = max;
  }

  MTSelection::~MTSelection(){
    ;
  }
  
  int MTSelection::PreAnalysis(){
    return 0;
  }


  int MTSelection::Execute(TreeEvent *event){

    Met * lpfMet = event->GetPtr<Met>(input_name_);
    std::vector<Candidate *> lLeptons = event->GetPtrVec<Candidate>(lepton_name_);

    if (lLeptons.size()==0) return 1;
    double lVal = MT(lpfMet,lLeptons[0]);

    if (lVal > min_ && lVal < max_) return 0;
    else return 1;

  }

  int MTSelection::PostAnalysis(){
    return 0;
  }

  void MTSelection::PrintInfo(){
    ;
  }
  



}//namespace


