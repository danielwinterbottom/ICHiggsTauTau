#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTL1MetCut.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "boost/format.hpp"

namespace ic {

  HTTL1MetCut::HTTL1MetCut(std::string const& name) : ModuleBase(name) {
    l1_met_label_               = "l1extraMET";
    rand = new TRandom3(0);

  }

  HTTL1MetCut::~HTTL1MetCut() {
    delete rand;
  }

  int HTTL1MetCut::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTL1MetCut" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "l1_met_label"        % l1_met_label_;
    return 0;
  }

  int HTTL1MetCut::Execute(TreeEvent *event) {
    std::vector<Candidate *> & v_l1met = event->GetPtrVec<Candidate>(l1_met_label_);
    Candidate *l1met = v_l1met.at(0);
    double cut = 26;
    if (rand->Uniform()<=(1.0-4.848/7.317)) cut=20;
    if (l1met->pt() > cut) {
      return 0;
    } else {
      return 1;
    }
  }
  
  int HTTL1MetCut::PostAnalysis() {
    return 0;
  }

  void HTTL1MetCut::PrintInfo() {
    ;
  }
}

