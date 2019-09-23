#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTMuonEnergyScale.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
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
    std::cout << boost::format(param_fmt()) % "input_label"         % input_label_;
    std::cout << boost::format(param_fmt()) % "neg_far_endcap"      % neg_far_endcap_;
    std::cout << boost::format(param_fmt()) % "barrel"              % barrel_;
    std::cout << boost::format(param_fmt()) % "near_endcap"         % near_endcap_;
    std::cout << boost::format(param_fmt()) % "pos_far_endcap"      % pos_far_endcap_;
    return 0;
  }

  int HTTMuonEnergyScale::Execute(TreeEvent *event) {
    if(channel_ != channel::tt && channel_ != channel::zee && channel_ != channel::et){
      std::vector<Muon *> &muons = event->GetPtrVec<Muon>(input_label_);
      std::map<std::size_t, double> muon_scales;
      for (unsigned i = 0; i < muons.size() ;++i){
        double total_shift = 1.00;
        double m_eta = muons[i]->eta();

        if(m_eta > -2.4 && m_eta < -2.1) {
          total_shift = neg_far_endcap_;
        } else if(m_eta > -2.1 && m_eta < -1.2){
          total_shift = near_endcap_;
        } else if(m_eta > -1.2 && m_eta < 1.2){
          total_shift = barrel_;
        } else if(m_eta > 1.2 && m_eta < 2.1){
          total_shift = near_endcap_;
        } else if(m_eta > 2.1 && m_eta < 2.4){
          total_shift = pos_far_endcap_;
        }
        muons[i]->set_pt(muons[i]->pt() * total_shift);
        muons[i]->set_energy(muons[i]->energy() * total_shift);
        muon_scales[muons[i]->id()] = total_shift;
      }
      event->Add("muon_scales", muon_scales);
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
