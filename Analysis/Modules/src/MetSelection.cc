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
    Met *lpfMet = 0;
    try {
      std::vector<Met*> metvec = event->GetPtrVec<Met>(input_name_);
      lpfMet = metvec[0];
    } catch (...){
      //std::cout << " Met vec not found..." << std::endl;
      lpfMet = event->GetPtr<Met>(input_name_);
      if (!lpfMet) {
	std::cerr << " -- Found no MET " << input_name_ << " in event! Exiting..." << std::endl;
	exit(1);
      }
    }
    double lVal = lpfMet->pt();

    bool passMet = false;
    if (lVal > min_ && lVal < max_) passMet = true;
    
    bool passFilters = true;
    if (doFilters_){
      EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
      passFilters = eventInfo->good_vertices() > 0;
      for (unsigned iF(0); iF<filters_.size(); ++iF){
	//std::cout << "-- Filter " << filters_[iF] << " : " << eventInfo->filter_result(filters_[iF]) << std::endl;
	if (filters_[iF].find("CSCTight")== filters_[iF].npos) {
	  passFilters = passFilters && eventInfo->filter_result(filters_[iF]);
	  if (eventInfo->filter_result(filters_[iF])) counters_[iF]++;
	}
	else {
	  passFilters = passFilters && !eventInfo->filter_result(filters_[iF]);
	  if (!eventInfo->filter_result(filters_[iF])) counters_[iF]++;
	}
      }
    }

    if (passMet && passFilters) return 0;
    else return 1;
  }

  int MetSelection::PostAnalysis(){
    if (doFilters_){
      std::cout << "----------------------------------------" << std::endl
		<< " PostAnalysis Info for MetSelection" << std::endl
		<< "----------------------------------------" << std::endl;
      
      for (unsigned iF(0); iF<filters_.size(); ++iF){
	std::cout << "-- Filter " << filters_[iF] << " : " << counters_[iF] << std::endl;
      }
    }
    return 0;
  }

  void MetSelection::PrintInfo(){
    ;
  }
  



}//namespace


