 #include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HinvDataTriggerFilter.h"
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
     do_obj_match_ = false;
   }

  HinvDataTriggerFilter::~HinvDataTriggerFilter() {
    ;
  }

  int HinvDataTriggerFilter::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvDataTriggerFilter" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Require match to HLT object: " << do_obj_match_ << std::endl;
    return 0;
  }

  int HinvDataTriggerFilter::Execute(TreeEvent *event) {

    bool path_found = false;

    //dirty fix for data
    if (!do_obj_match_) {

      TriggerPathPtrVec const& triggerPathPtrVec = 
	event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      unsigned run = eventInfo->run();
 
      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
	std::string name = triggerPathPtrVec[i]->name();
	triggerPathPtrVec[i]->prescale();
	if (name.find("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v") != name.npos) path_found = true; 
	/*
      In preparation for Z->ee, Z->mm:
      if (channel == EE) {
        //2011 Triggers
        if (run >= 160404 && run <= 167913 && name.find("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v") != name.npos) path_found = true;
        if (run >= 170249 && run <= 180252 && name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true;
        //2012 Triggers
        if (run >= 190456 && run <= ??? && name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true;
        
      }
      if (channel == MM) {
        //2011 Triggers
        if (run >= 160404 && run <= 163869 && name.find("HLT_DoubleMu7_v") != name.npos) path_found = true;
        if (run >= 165088 && run <= 178380 && name.find("HLT_Mu13_Mu8_v") != name.npos) path_found = true;
        if (run >= 178420 && run <= 180252 && name.find("HLT_Mu17_Mu8_v") != name.npos) path_found = true;
        //2012 Triggers
        if (run >= 190456 && run <= ??? && name.find("HLT_Mu17_Mu8_v") != name.npos) path_found = true;
      }
      */
      }
    }
    //for MC
    else {

      std::string trig_obj_label;

      trig_obj_label = "triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets";
      //met_filter = "";
      //jet_filter = "";  
      //std::vector<Electron *> & elmus = event->GetPtrVec<Electron>(lep1label_);
      //std::vector<Tau *> & taus = event->GetPtrVec<Tau>(lep2label_);
      std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);
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
    return 0;
  }

  void HinvDataTriggerFilter::PrintInfo() {
    ;
  }
}
