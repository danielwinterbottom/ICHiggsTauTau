#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HhhMetScale.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "boost/format.hpp"

namespace ic {

  HhhMetScale::HhhMetScale(std::string const& name) : ModuleBase(name) {
    met_label_="PFMet";
    scale_shift_=0.04;
    met_scale_mode_=0;
  }

  HhhMetScale::~HhhMetScale() {
    ;
  }

  int HhhMetScale::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HhhMetScale" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "met_label" % met_label_;
    std::cout << boost::format(param_fmt()) % "scale_shift" % scale_shift_;
    std::cout << boost::format(param_fmt()) % "met_scale_mode" % met_scale_mode_;
    return 0;
  }

  int HhhMetScale::Execute(TreeEvent *event) {
    Met * pfMet = event->GetPtr<Met>(met_label_);
    
    if(met_scale_mode_==0) return 0;
    
    double pfmet = pfMet->pt();
    if(met_scale_mode_==2){
      pfmet = pfmet*(1+scale_shift_);
    }
    else if(met_scale_mode_==1){
      pfmet = pfmet*(1-scale_shift_);
    }

    pfMet->set_pt(pfmet);
    pfMet->set_energy(pfmet);
    return 0;



  }



  int HhhMetScale::PostAnalysis() {
    return 0;
  }

  void HhhMetScale::PrintInfo() {
    ;
  }

}
