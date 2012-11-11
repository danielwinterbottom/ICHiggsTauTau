#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ElectronEtaSCMax.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
namespace ic {

  ElectronEtaSCMax::ElectronEtaSCMax(std::string const& name, std::string input_name, double const& max) : ModuleBase(name) {
    input_name_ = input_name;
    max_ = max;
  }

  ElectronEtaSCMax::~ElectronEtaSCMax() {
    ;
  }

  int ElectronEtaSCMax::PreAnalysis() {
    return 0;
  }

  int ElectronEtaSCMax::Execute(TreeEvent *event) {
    std::vector<Electron *> & vec = event->GetPtrVec<Electron>(input_name_);
    ic::erase_if(vec,!boost::bind(MaxEtaSC, _1, max_));
    if (vec.size() >= 2) {
      return 0;
    } else {
      return 1;
    }
  }

  int ElectronEtaSCMax::PostAnalysis() {
    return 0;
  }

  void ElectronEtaSCMax::PrintInfo() {
    ;
  }
}
