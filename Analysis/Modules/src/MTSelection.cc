#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MTSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {

  
  MTSelection::MTSelection(std::string const& name, 
			   std::string input_name,
			   std::string lepton_name,
			   double min, 
			   double max) : ModuleBase(name) {
    input_name_ = input_name;
    lepton_name_ = lepton_name;
    min_ = min;
    max_ = max;
  }

  MTSelection::~MTSelection(){
    ;
  }
  
  int MTSelection::PreAnalysis(){
    std::cout << " ** PreAnalysis Info for MTSelection **" << std::endl;
    std::cout << "Running MTSelection with lepton collection: " << lepton_name_ << std::endl;
    if (lepton_name_.find("elec") == lepton_name_.npos &&
	lepton_name_.find("muon") == lepton_name_.npos){
      std::cerr << " -- Invalid collection name: should contain \"elec\" or \"muon\" strings. MT cut will not be applied, exiting..." << std::endl;
      throw;
    }
    return 0;
  }


  int MTSelection::Execute(TreeEvent *event){

    Met * lpfMet = event->GetPtr<Met>(input_name_);
    //load the collection under a different name to avoid troubles...

    double lVal = -1;
    if (lepton_name_.find("elec") != lepton_name_.npos){
      std::vector<Electron *> lElecs = event->GetPtrVec<Electron>(lepton_name_);
      if (lElecs.size()==0) return 1;
      lVal = MT(lpfMet,lElecs[0]);
    }
    else if (lepton_name_.find("muon") != lepton_name_.npos){
      std::vector<Muon *> lMuons = event->GetPtrVec<Muon>(lepton_name_);
      if (lMuons.size()==0) return 1;
      lVal = MT(lpfMet,lMuons[0]);
    }
 
    if (lVal > min_ && lVal < max_) return 0;
    else return 1;
    
  }

  int MTSelection::PostAnalysis(){
    return 0;
  }

  void MTSelection::PrintInfo(){
    ;
  }
  



}//namespace


