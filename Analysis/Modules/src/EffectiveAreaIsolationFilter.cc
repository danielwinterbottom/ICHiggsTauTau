#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EffectiveAreaIsolationFilter.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/ElectronEffectiveArea.h"

namespace ic {

  
  EffectiveAreaIsolationFilter::EffectiveAreaIsolationFilter(std::string const& name, 
						 std::string input_name, 
						 double max) : ModuleBase(name) {
    input_name_ = input_name;
    max_ = max;
  }
 
  EffectiveAreaIsolationFilter::~EffectiveAreaIsolationFilter(){
    ;
  }
  
  int EffectiveAreaIsolationFilter::PreAnalysis(){
    return 0;
  }

  int EffectiveAreaIsolationFilter::Execute(TreeEvent *event){

    
    std::vector<Electron *> & vec = event->GetPtrVec<Electron>(input_name_);
    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");

    double lRho = eventInfo->lepton_rho();
 
    //if (vec.size()>1) std::cout << "* Electrons size: " << vec.size() << std::endl;

    std::vector<Electron *>::iterator iter = vec.begin();
    for (unsigned iEle(0); iEle<vec.size(); ++iEle,++iter){
      Electron * lElec = vec[iEle];

      float lEffArea = ElectronEffectiveArea::GetElectronEffectiveArea( ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03 , lElec->sc_eta() , ElectronEffectiveArea::kEleEAData2012);

      double lIso = lElec->dr03_pfiso_charged()
	+ std::max(lElec->dr03_pfiso_gamma() + lElec->dr03_pfiso_neutral() - lRho * lEffArea, 0.);
      lIso = lIso / lElec->pt();

      //if (vec.size()>1) std::cout << " -- Element " << iEle << "/" << vec.size() << " iso " << lIso ;
      
      if (lIso >= max_) {
	vec.erase(iter);
	//if (vec.size()>0) std::cout << " --- failed " << std::endl;
	--iter;
	--iEle;
      }
      //else if (vec.size()>1) std::cout << " --- passed " << std::endl;

    }//loop on electrons


    
    return 0;
  
  }

  int EffectiveAreaIsolationFilter::PostAnalysis(){
    return 0;
  }

  void EffectiveAreaIsolationFilter::PrintInfo(){
    ;
  }
  



}//namespace


