#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MetSelection.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"


namespace ic {

  
  MetSelection::MetSelection(std::string const& name, 
			     std::string input_name, 
			     double min, 
			     double max) : ModuleBase(name) {
    input_name_ = input_name;
    addLeptons_ = false;
    min_ = min;
    max_ = max;
  }
 
  MetSelection::MetSelection(std::string const& name, 
			     std::string input_name,
			     std::string lepton_name,
			     double min, 
			     double max) : ModuleBase(name) {
    input_name_ = input_name;
    addLeptons_ = true;
    lepton_name_ = lepton_name;
    min_ = min;
    max_ = max;
  }

  MetSelection::~MetSelection(){
    ;
  }
  
  int MetSelection::PreAnalysis(){
    return 0;
  }
  int MetSelection::Execute(TreeEvent *event){

    Met * lpfMet = event->GetPtr<Met>(input_name_);

    double lVal = lpfMet->pt();

    if (addLeptons_) {
      if (lepton_name_.find("muon") != lepton_name_.npos)
	lVal+=correction<Muon>(event);
      if (lepton_name_.find("electron") != lepton_name_.npos)
	lVal+=correction<Electron>(event);
    }
    if (lVal > min_ && lVal < max_) return 0;
    else return 1;

  }

  int MetSelection::PostAnalysis(){
    return 0;
  }

  void MetSelection::PrintInfo(){
    ;
  }
  



}//namespace


