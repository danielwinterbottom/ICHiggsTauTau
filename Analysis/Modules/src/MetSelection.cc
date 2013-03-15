#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MetSelection.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"


namespace ic {

  
  MetSelection::MetSelection(std::string const& name, 
			     std::string input_name,
			     bool doFilters,
			     std::vector<std::string> filters,
			     double min, 
			     double max) : ModuleBase(name) {
    input_name_ = input_name;
    doFilters_ = doFilters;
    filters_ = filters;
    min_ = min;
    max_ = max;

    counters_.resize(filters_.size(),0);

  }
 
  MetSelection::~MetSelection(){
    ;
  }
  
  int MetSelection::PreAnalysis(){
    counters_.resize(filters_.size(),0);
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
      for (unsigned iF(0); iF<filters_.size(); ++iF){
	//std::cout << "-- Filter " << filters_[iF] << " : " << eventInfo->filter_result(filters_[iF]) << std::endl;
	passFilters = passFilters && eventInfo->filter_result(filters_[iF]);
	if (eventInfo->filter_result(filters_[iF])) counters_[iF]++;
      }
    }

    if (passMet && passFilters) return 0;
    else return 1;
  }

  int MetSelection::PostAnalysis(){

    std::cout << "----------------------------------------" << std::endl
	      << " PostAnalysis Info for MetSelection" << std::endl
	      << "----------------------------------------" << std::endl;

    for (unsigned iF(0); iF<filters_.size(); ++iF){
      std::cout << "-- Filter " << filters_[iF] << " : " << counters_[iF] << std::endl;
    }
    return 0;
  }

  void MetSelection::PrintInfo(){
    ;
  }
  



}//namespace


