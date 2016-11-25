#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/SingleMetMaker.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"


namespace ic {

  
  SingleMetMaker::SingleMetMaker(std::string const& name, 
		       std::string met_name
		       ) : ModuleBase(name) {
    met_name_ = met_name;
  }
 
  SingleMetMaker::~SingleMetMaker(){
    ;
  }
  
  int SingleMetMaker::PreAnalysis(){
    std::cout << "----------------------------------------" << std::endl
	      << " PreAnalysis Info for SingleMetMaker " << std::endl
	      << "----------------------------------------" << std::endl
	      << "-- SingleMetMaker is run with parameters:" << std::endl
	      << "---- met name : " << met_name_ << std::endl
	      << "---- outputcol name : " << ModuleName() << std::endl;
   return 0;
  }

  int SingleMetMaker::Execute(TreeEvent *event){


    std::vector<Met *> & lMetVec = event->GetPtrVec<Met>(met_name_);
    Met* lMet=lMetVec[0];
    event->Add(ModuleName()+"Product", *lMet);
    Met *lMetPtr = &(event->Get<Met>(ModuleName()+"Product"));
    event->Add(ModuleName(), lMetPtr);


    return 0;
  }

  int SingleMetMaker::PostAnalysis(){
    return 0;
  }

  void SingleMetMaker::PrintInfo(){
    ;
  }
  



}//namespace


