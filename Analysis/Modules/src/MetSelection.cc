#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MetSelection.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"


namespace ic {

  
  MetSelection::MetSelection(std::string const& name, 
			     std::string input_name,
			     bool doFilters,
			     double min, 
			     double max) : ModuleBase(name) {
    input_name_ = input_name;
    doFilters_ = doFilters;
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
    double lVal = lpfMet->pt();

    bool passMet = false;
    if (lVal > min_ && lVal < max_) passMet = true;
    
    bool passFilters = true;
    if (doFilters_){
      EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
      //std::map<std::size_t, float>::const_iterator it;
      //for (it = eventInfo->filters().begin(); it != eventInfo->filters().end(); ++it) {
      //std::cout << it->first << " " << it->second << std::endl;
      //}
      passFilters = eventInfo->total_filter_result();
    }

    if (passMet && passFilters) return 0;
    else return 1;
  }

  int MetSelection::PostAnalysis(){
    return 0;
  }

  void MetSelection::PrintInfo(){
    ;
  }
  



}//namespace


