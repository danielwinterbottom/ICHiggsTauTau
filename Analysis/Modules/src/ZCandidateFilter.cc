#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ZCandidateFilter.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
namespace ic {

  ZCandidateFilter::ZCandidateFilter(std::string const& name, std::string input_name, 
                                     double mass_low, double mass_high, 
                                     bool do_charge) : ModuleBase(name) {
    input_name_ = input_name;
    mass_low_ = mass_low;
    mass_high_ = mass_high;
    do_charge_ = do_charge;
  }

  ZCandidateFilter::~ZCandidateFilter() {
    ;
  }

  int ZCandidateFilter::PreAnalysis() {
    return 0;
  }

  int ZCandidateFilter::Execute(TreeEvent *event) {
    std::vector<CompositeCandidate> & vec = 
      event->Get<std::vector<CompositeCandidate> >(input_name_);
    //ic::erase_if(vec,!boost::bind(PairMassInRange, _1, mass_low_, mass_high_));
    if (do_charge_) {
      ic::erase_if(vec,!boost::bind(PairOppCharge, _1));
    }
    if (vec.size() >= 1) {
      return 0;
    } else {
      return 1;
    }

  }

  int ZCandidateFilter::PostAnalysis() {
    return 0;
  }

  void ZCandidateFilter::PrintInfo() {
    ;
  }
}
