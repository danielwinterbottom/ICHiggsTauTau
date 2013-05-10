#include <fstream>
#include <sstream>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/MetLaserFilters.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

namespace ic {

  
  MetLaserFilters::MetLaserFilters(std::string const& name, 
				   std::string hcal_input,
				   std::string ecal_input,
				   bool doFilters) : ModuleBase(name) {
    hcal_input_name_ = hcal_input;
    ecal_input_name_ = ecal_input;
    doFilters_=doFilters;
  }
 
  MetLaserFilters::~MetLaserFilters(){
    ;
  }
  
  int MetLaserFilters::PreAnalysis(){

    std::cout << "----------------------------------------" << std::endl
	      << " PreAnalysis Info for MetLaserFilters " << std::endl
	      << "----------------------------------------" << std::endl
	      << "-- MetLaserFilters is run with parameters:" << std::endl
	      << "---- hcal input name : " << hcal_input_name_ << std::endl
	      << "---- ecal input name : " << ecal_input_name_ << std::endl
	      << "---- doFilters : " << doFilters_ << std::endl;

    if (doFilters_){
      badEvts_.clear();
      //badRuns_.clear();
      //badEvts_.reserve(515891);
      if (!extractEvents(hcal_input_name_)) return 1;
      if (!extractEvents(ecal_input_name_)) return 1;
    }


    std::cout << "-------- Loaded " << badEvts_.size() << " bad events." << std::endl;

    return 0;
  }

  int MetLaserFilters::Execute(TreeEvent *event){

     EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

     RunLumiEvent lEvt;
     lEvt.evt = eventInfo->event();
     lEvt.run = eventInfo->run();
     lEvt.lumi = eventInfo->lumi_block();

     //bool print = (lEvt.run == 204554) || (lEvt.run == 205311);
     //if (print) std::cout << "--MetLaserFilters** Processing run " << lEvt.run << ":" << lEvt.lumi << ":" << lEvt.evt << std::endl;
     std::set<RunLumiEvent>::iterator lIter = badEvts_.find(lEvt);
     if (lIter != badEvts_.end()) {
       badEvts_.erase(lIter);
       //if (print) std::cout << " ----MetLaserFilters** Deleted event " << lEvt.run << ", size of badEvents = " << badEvts_.size() << std::endl;
       return 1;
     }
     
     return 0;
  }

  bool MetLaserFilters::extractEvents(std::string inputfile){

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

  int MetLaserFilters::PostAnalysis(){
   return 0;
  }

  void MetLaserFilters::PrintInfo(){
    ;
  }
  



}//namespace


