#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvDataTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"


namespace ic {

  HinvDataTriggerFilter::HinvDataTriggerFilter(std::string const& name) : ModuleBase(name) {
     is_data_ = false;
     trigger_path_="";
     trig_obj_label_="";
     counter1_ = 0;
     counter2_ = 0;
   }

  HinvDataTriggerFilter::~HinvDataTriggerFilter() {
    ;
  }

  int HinvDataTriggerFilter::PreAnalysis() {
    counter1_ = 0;
    counter2_ = 0;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvDataTriggerFilter" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Require match to HLT object: " << is_data_ << std::endl;
    std::cout << "Trigger path : " << trigger_path_ << std::endl;
    std::cout << "Trigger object label : " << trig_obj_label_ << std::endl;
    return 0;
  }

  int HinvDataTriggerFilter::Execute(TreeEvent *event) {

    bool path_found = false;
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label_);
  
    if (is_data_) {

      TriggerPathPtrVec const& triggerPathPtrVec = 
	event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      unsigned run = eventInfo->run();
 
      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
	std::string name = triggerPathPtrVec[i]->name();
	triggerPathPtrVec[i]->prescale();
	if (name.find(trigger_path_) != name.npos) path_found = true; 
      }

      if ( (objs.size()== 0) && path_found ) counter1_++;
      if ( (objs.size() > 0) && !path_found ) counter2_++;

    }
    //for MC
    else {

     if (objs.size() > 0) path_found=true;
      //ic::erase_if(elmus, !boost::bind(IsFilterMatched, _1, objs, elmu_filter, 0.5));
      //ic::erase_if(taus, !boost::bind(IsFilterMatched, _1, objs, tau_filter, 0.5));
    } // do obj match

    if (path_found) {
      return 0;
    } else {
      return 1;
    }
  }

  int HinvDataTriggerFilter::PostAnalysis() {
    std::cout << " -- Number of times path fired but no trigger object: " << counter1_ << std::endl;
    std::cout << " -- Number of times path not fired but trigger objects: " << counter2_ << std::endl;
    return 0;
  }

  void HinvDataTriggerFilter::PrintInfo() {
    ;
  }
}
