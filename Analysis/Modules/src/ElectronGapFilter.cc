#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ElectronGapFilter.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
namespace ic {

  ElectronGapFilter::ElectronGapFilter(std::string const& name, std::string input_name) : ModuleBase(name) {
    input_name_ = input_name;
  }

  ElectronGapFilter::~ElectronGapFilter() {
    ;
  }

  int ElectronGapFilter::PreAnalysis() {
    return 0;
  }

  int ElectronGapFilter::Execute(TreeEvent *event) {
    std::vector<Electron *> & vec = event->GetPtrVec<Electron>(input_name_);
    ic::erase_if(vec,boost::bind(InEcalGap, _1));
    if (vec.size() >= 2) {
      return 0;
    } else {
      return 1;
    }
  }

  int ElectronGapFilter::PostAnalysis() {
    return 0;
  }

  void ElectronGapFilter::PrintInfo() {
    ;
  }
}
