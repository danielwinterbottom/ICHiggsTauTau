#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MuonIsoFilter.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
namespace ic {

  MuonIsoFilter::MuonIsoFilter(std::string const& name, std::string input_name) : ModuleBase(name) {
    input_name_ = input_name;
  }

  MuonIsoFilter::~MuonIsoFilter() {
    ;
  }

  int MuonIsoFilter::PreAnalysis() {
    return 0;
  }

  int MuonIsoFilter::Execute(TreeEvent *event) {
    std::vector<Muon *> & vec = event->GetPtrVec<Muon>(input_name_);
    ic::erase_if(vec,!boost::bind(MuonIso, _1));
    if (vec.size() >= 2) {
      return 0;
    } else {
      return 1;
    }
  }

  int MuonIsoFilter::PostAnalysis() {
    return 0;
  }

  void MuonIsoFilter::PrintInfo() {
    ;
  }
}
