#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/ModifyMet.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"


namespace ic {

  
  ModifyMet::ModifyMet(std::string const& name, 
		       std::string met_name,
		       std::string lep_name,
		       unsigned lepFlavour,
		       unsigned nLepToAdd) : ModuleBase(name) {
    met_name_ = met_name;
    lep_name_ = lep_name;
    lepFlavour_ = lepFlavour;
    nLepToAdd_ = nLepToAdd;
  }
 
  ModifyMet::~ModifyMet(){
    ;
  }
  
  int ModifyMet::PreAnalysis(){
    std::cout << "----------------------------------------" << std::endl
	      << " PreAnalysis Info for ModifyMet " << std::endl
	      << "----------------------------------------" << std::endl
	      << "-- ModifyMet is run with parameters:" << std::endl
	      << "---- met name : " << met_name_ << std::endl
	      << "---- lepton name : " << lep_name_ << std::endl
	      << "---- lepton flavour : " << lepFlavour_ << std::endl
	      << "---- Number of leptons to add: " << nLepToAdd_ << std::endl
	      << "---- outputcol name : " << ModuleName() << std::endl;
   return 0;
  }

  int ModifyMet::Execute(TreeEvent *event){

    Met *lMet = 0;
    try {
      std::vector<Met*> metvec = event->GetPtrVec<Met>(met_name_);
      lMet = metvec[0];
    }
    catch (...){
      //std::cout << " Met vec not found..." << std::endl;
      lMet = event->GetPtr<Met>(met_name_);
      if (!lMet) {
	std::cerr << " -- Found no MET " << met_name_ << " in event! Exiting..." << std::endl;
	exit(1);
      }
    }

    event->Add(ModuleName()+"Product", *lMet);
    Met *lMetPtr = &(event->Get<Met>(ModuleName()+"Product"));
    event->Add(ModuleName(), lMetPtr);


    ROOT::Math::PtEtaPhiEVector lVec = lMetPtr->vector();

    if (lepFlavour_==2)
      correctMet<Muon>(event,lVec);
    else if (lepFlavour_==1)
      correctMet<Electron>(event,lVec);

    lMetPtr->set_vector(lVec);
 

    return 0;
  }

  int ModifyMet::PostAnalysis(){
    return 0;
  }

  void ModifyMet::PrintInfo(){
    ;
  }
  



}//namespace


