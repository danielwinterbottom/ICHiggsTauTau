#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MetSelection.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"


namespace ic {

  
  MetSelection::MetSelection(std::string const& name, 
			     std::string input_name, 
			     double min, 
			     double max) : ModuleBase(name) {
    input_name_ = input_name;
    min_ = min;
    max_ = max;
  }

  MetSelection::~MetSelection(){
    ;
  }
  
  int MetSelection::PreAnalysis(){
    return 0;
  }
  int MetSelection::Execute(TreeEvent *event){

    Met * lpfMet = event->GetPtr<Met>(input_name_);
    if (lpfMet->pt() > min_ && lpfMet->pt() < max_) return 0;
    else return 1;

  }

  int MetSelection::PostAnalysis(){
    return 0;
  }

  void MetSelection::PrintInfo(){
    ;
  }
  



}//namespace


