#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MuonTightFilter.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
namespace ic {

  MuonTightFilter::MuonTightFilter(std::string const& name, std::string input_name) : ModuleBase(name) {
    input_name_ = input_name;
  }

  MuonTightFilter::~MuonTightFilter() {
    ;
  }

  int MuonTightFilter::PreAnalysis() {
    return 0;
  }

  int MuonTightFilter::Execute(TreeEvent *event) {
    std::vector<Muon *> & vec = event->GetPtrVec<Muon>(input_name_);
    ic::erase_if(vec,!boost::bind(MuonTight, _1));
    if (vec.size() >= 2) {
      return 0;
    } else {
      return 1;
    }
  }

  int MuonTightFilter::PostAnalysis() {
    return 0;
  }

  void MuonTightFilter::PrintInfo() {
    ;
  }
}
