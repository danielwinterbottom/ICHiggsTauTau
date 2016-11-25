#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/RunFilter.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"


namespace ic {

  RunFilter::RunFilter(std::string const& name) : ModuleBase(name) {
    ;
  }

  RunFilter::~RunFilter() {
    ;
  }

  int RunFilter::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for Run Filter" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Filtered runs:" << std::endl;
    for (std::set<int>::const_iterator it = runs_to_filter.begin(); it != runs_to_filter.end(); ++it) {
      std::cout << *it << std::endl;
    }
    return 0;
  }

  int RunFilter::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned run = eventInfo->run();
    if (runs_to_filter.count(run) > 0) {
      return 1;
    } else {
      return 0;
    }
  }
  int RunFilter::PostAnalysis() {
    return 0;
  }

  void RunFilter::PrintInfo() {
    ;
  }
}
