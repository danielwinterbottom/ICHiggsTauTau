#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTMuonEnergyScale.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "boost/format.hpp"

namespace ic {

  HTTMuonEnergyScale::HTTMuonEnergyScale(std::string const& name) : ModuleBase(name), channel_(channel::mt), strategy_(strategy::cpsummer16) {
  }

  HTTMuonEnergyScale::~HTTMuonEnergyScale() {
    ;
  }

  int HTTMuonEnergyScale::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTMuonEnergyScale" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "input_label" % input_label_;
    std::cout << boost::format(param_fmt()) % "barrel"      % barrel_;
    std::cout << boost::format(param_fmt()) % "near_endcap" % near_endcap_;
    std::cout << boost::format(param_fmt()) % "far_endcap"  % far_endcap_;
    return 0;
  }

  int HTTMuonEnergyScale::Execute(TreeEvent *event) {
    if(channel_ != channel::tt && channel_ != channel::zee && channel_ != channel::et){
      std::vector<Muon *> &muons = event->GetPtrVec<Muon>(input_label_);
      for (unsigned i = 0; i < muons.size() ;++i){
        double total_shift = 1.00;
        double m_eta = std::abs(muons[i]->eta());

        if(m_eta < 1.2) total_shift = barrel_;
        else if(m_eta < 2.1) total_shift = near_endcap_;
        else if(m_eta < 2.4) total_shift = far_endcap_;

        muons[i]->set_pt(muons[i]->pt() * total_shift);
        muons[i]->set_energy(muons[i]->energy() * total_shift);
      }
    }
    return 0;
  }



  int HTTMuonEnergyScale::PostAnalysis() {
    return 0;
  }

  void HTTMuonEnergyScale::PrintInfo() {
    ;
  }

}
