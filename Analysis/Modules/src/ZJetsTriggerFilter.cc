 #include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ZJetsTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"


namespace ic {

  ZJetsTriggerFilter::ZJetsTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::zee) {
  }

  ZJetsTriggerFilter::~ZJetsTriggerFilter() {
    ;
  }

  int ZJetsTriggerFilter::PreAnalysis() {
    std::cout << "** PreAnalysis Info for ZJetsTriggerFilter **" << std::endl;
    std::cout << "Channel: " << Channel2String(channel_) << std::endl;
    std::cout << "Pair Collection: " << pair_label_ << std::endl;
    std::cout << "Is Data?: " << is_data_ << std::endl;
    return 0;
  }

  int ZJetsTriggerFilter::Execute(TreeEvent *event) {

    std::string trig_obj_label;
    std::string low_leg_filter;
    std::string high_leg_filter;

    if (is_data_) {
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      unsigned run = eventInfo->run();
      bool path_found = false;
      TriggerPathPtrVec const& triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPaths");

      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
        std::string name = triggerPathPtrVec[i]->name();

        if (channel_ == channel::zee) {
        //2011 Triggers
          if (run >= 160404 && run <= 167913 && name.find("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v") != name.npos) path_found = true;
          if (run >= 170249 && run <= 180252 && name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true;
        //2012 Triggers
        if (run >= 190456 /*&& run <= ???*/ && name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true;
        }
        if (channel_ == channel::zmm) {
        //2011 Triggers
          if (run >= 160404 && run <= 163869 && name.find("HLT_DoubleMu7_v") != name.npos) path_found = true;
          if (run >= 165088 && run <= 178380 && name.find("HLT_Mu13_Mu8_v") != name.npos) path_found = true;
          if (run >= 178420 && run <= 180252 && name.find("HLT_Mu17_Mu8_v") != name.npos) path_found = true;
        //2012 Triggers
        if (run >= 190456 /*&& run <= ???*/ && name.find("HLT_Mu17_Mu8_v") != name.npos) path_found = true;
        }
      }
      if (!path_found) return 1;

      if (channel_ == channel::zee) {
        // 2012 Triggers
        if (run >= 190456 /*&& run <= ?????*/) {
          trig_obj_label = "triggerObjectsEle17Ele8";
          low_leg_filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter";
          high_leg_filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter";
        }
      }
      if (channel_ == channel::zmm) {
        // 2012 Triggers
        if (run >= 190456 && run <= 196045) {
          trig_obj_label = "triggerObjectsMu17Mu8";
          low_leg_filter = "hltL3pfL1DoubleMu10MuOpenL1f0L2pf0L3PreFiltered8";
          high_leg_filter = "hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17";
        }
        if (run >= 196046/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsMu17Mu8";
          low_leg_filter = "hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8";
          high_leg_filter = "hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17";
        }
      }
    } else {

      // Define MC triggers here
    }


    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>("dileptons");
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

    std::vector<CompositeCandidate *> dileptons_pass;

    for (unsigned i = 0; i < dileptons.size(); ++i) {
      unsigned npass_low = 0;
      unsigned npass_high = 0;
      if (IsFilterMatched(dileptons[i]->At(0), objs, low_leg_filter, 0.5)) ++npass_low;
      if (IsFilterMatched(dileptons[i]->At(1), objs, low_leg_filter, 0.5)) ++npass_low;
      if (IsFilterMatched(dileptons[i]->At(0), objs, high_leg_filter, 0.5)) ++npass_high;
      if (IsFilterMatched(dileptons[i]->At(1), objs, high_leg_filter, 0.5)) ++npass_high;
      if (npass_low >= 2 && npass_high >= 1) dileptons_pass.push_back(dileptons[i]);
    }

    dileptons = dileptons_pass;
    if (dileptons.size() >= 1) {
      return 0;
    } else {
      return 1;
    }
}

  int ZJetsTriggerFilter::PostAnalysis() {
    return 0;
  }

  void ZJetsTriggerFilter::PrintInfo() {
    ;
  }
}
