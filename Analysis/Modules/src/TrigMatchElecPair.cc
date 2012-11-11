#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/TrigMatchElecPair.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "boost/bind.hpp"

namespace ic {

  TrigMatchElecPair::TrigMatchElecPair(std::string const& name, std::string input_name) : ModuleBase(name) {
    input_name_ = input_name;
  }

  TrigMatchElecPair::~TrigMatchElecPair() {
    ;
  }

  int TrigMatchElecPair::PreAnalysis() {
    return 0;
  }

  int TrigMatchElecPair::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned run = eventInfo->run();
    // TriggerPathPtrVec const& triggerPathPtrVec = 
    //     event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
    // std::string path = "";
    // for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
    //   std::string name = triggerPathPtrVec[i]->name();
    //   if (  run < 170249 && 
    //     name.find("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL") != name.npos) {
    //     path = name;
    //     break;
    //   } else if (run > 170248 && 
    //     name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL") != name.npos) {
    //     path = name;
    //     break;
    //   }
    // }
    // std::size_t hash = CityHash64(path);
      std::string filter;
      std::vector<TriggerObject *> const* objs;
      if (  run < 170249 ) {
        objs = &(event->GetPtrVec<TriggerObject>("triggerObjectsEle17Ele8Early"));
        filter = "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter";
      } else if (run > 170248 ) {
        objs = &(event->GetPtrVec<TriggerObject>("triggerObjectsEle17Ele8Late"));
        filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter";
      }
    std::vector<CompositeCandidate> & vec = 
      event->Get<std::vector<CompositeCandidate> >(input_name_);
    bool ele1matched = IsFilterMatched(vec[0].At(0), *objs, filter, 0.3);
    bool ele2matched = IsFilterMatched(vec[0].At(1), *objs, filter, 0.3);

    if (ele1matched&&ele2matched) {
      return 0;
    } else {
      return 1;
    }

  }

  bool TrigMatchElecPair::IsMatched(CompositeCandidate const& cand, std::size_t const& hash) const {
      bool pass = true;
      Electron const* elec1 = dynamic_cast<Electron const*>(cand.At(0));
      Electron const* elec2 = dynamic_cast<Electron const*>(cand.At(1));
      std::vector<std::size_t> const& elec1filters = elec1->hlt_match_paths();
      std::vector<std::size_t> const& elec2filters = elec2->hlt_match_paths();
      std::vector<std::size_t>::const_iterator it1;
      std::vector<std::size_t>::const_iterator it2;
      it1 = std::find(elec1filters.begin(),elec1filters.end(), hash);
      it2 = std::find(elec2filters.begin(),elec2filters.end(), hash);
      if (it1 == elec1filters.end()) pass = false;
      if (it2 == elec2filters.end()) pass = false;
      if (!pass) {
        std::vector<std::size_t> const& elec1f = elec1->hlt_match_filters();
        std::vector<std::size_t> const& elec2f = elec2->hlt_match_filters();
        std::vector<std::size_t>::const_iterator iter1;
        std::vector<std::size_t>::const_iterator iter2;
        iter1 = std::find(elec1f.begin(),elec1f.end(), CityHash64("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter"));
        iter2 = std::find(elec2f.begin(),elec2f.end(), CityHash64("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter")); 
        if (it1 != elec1filters.end() && it2 != elec2filters.end()) pass = true;
      }
      return pass;
  }


  int TrigMatchElecPair::PostAnalysis() {
    return 0;
  }

  void TrigMatchElecPair::PrintInfo() {
    ;
  }
}
