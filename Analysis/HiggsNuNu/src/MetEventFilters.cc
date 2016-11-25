#include <fstream>
#include <sstream>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/MetEventFilters.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

namespace ic {

  
  MetEventFilters::MetEventFilters(std::string const& name, 
				   std::vector<std::string> input_vec,
				   bool doFilters) : ModuleBase(name) {
    input_vec_ = input_vec;
    doFilters_=doFilters;
    countRejected_ = 0;
  }
 
  MetEventFilters::~MetEventFilters(){
    ;
  }
  
  int MetEventFilters::PreAnalysis(){

    countRejected_ = 0;
    std::cout << "----------------------------------------" << std::endl
	      << " PreAnalysis Info for MetEventFilters " << std::endl
	      << "----------------------------------------" << std::endl
	      << "-- MetEventFilters is run with parameters:" << std::endl
	      << "---- number of input files : " << input_vec_.size() << std::endl;
    for (unsigned i(0);i<input_vec_.size();++i){
      std::cout << "---- " <<  input_vec_[i] << std::endl;
    }
    
    std::cout << "---- doFilters : " << doFilters_ << std::endl;
    
    if (doFilters_){
      badEvts_.clear();
      //badRuns_.clear();
      //badEvts_.reserve(515891);
      for (unsigned i(0);i<input_vec_.size();++i){
	if (!extractEvents(input_vec_[i])) {
	  std::cout << " Warning! Could not extract events from file: " << input_vec_[i] << ", file is ignored." << std::endl;
	  return 1;
	}
      }
    }

    std::cout << "-------- Loaded " << badEvts_.size() << " bad events." << std::endl;

    return 0;
  }

  int MetEventFilters::Execute(TreeEvent *event){

     EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

     RunLumiEvent lEvt;
     lEvt.evt = eventInfo->event();
     lEvt.run = eventInfo->run();
     lEvt.lumi = eventInfo->lumi_block();

     //bool print = (lEvt.run == 204554) || (lEvt.run == 205311);
     //if (print) std::cout << "--MetEventFilters** Processing run " << lEvt.run << ":" << lEvt.lumi << ":" << lEvt.evt << std::endl;
     std::set<RunLumiEvent>::iterator lIter = badEvts_.find(lEvt);
     if (lIter != badEvts_.end()) {
       badEvts_.erase(lIter);
       countRejected_++;
       //if (print) std::cout << " ----MetEventFilters** Deleted event " << lEvt.run << ", size of badEvents = " << badEvts_.size() << std::endl;
       return 1;
     }
     
     return 0;
  }

  bool MetEventFilters::extractEvents(std::string inputfile){

    std::cout << " -- Reading file: " << inputfile << std::endl
	      << " ---- Size of badEvents before = " << badEvts_.size() << std::endl;

    std::ifstream lInput;
    lInput.open(inputfile);
    if(!lInput.is_open()){
      std::cerr << "Unable to open file: " << inputfile << std::endl;
      return false; 
    }
    lInput>>badEvts_;

    std::cout << " ---- Size of badEvents after = " << badEvts_.size() << std::endl;

    lInput.close();

    return true;
  }

  int MetEventFilters::PostAnalysis(){
    std::cout << "----------------------------------------" << std::endl
	      << " PostAnalysis Info for MetEventFilters " << std::endl
	      << "----------------------------------------" << std::endl
	      << "-- Rejected " << countRejected_ << " events." << std::endl;
   return 0;
  }

  void MetEventFilters::PrintInfo(){
    ;
  }
  



}//namespace


