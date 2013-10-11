#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MTSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


#include "TVector3.h"


namespace ic {

  
  MTSelection::MTSelection(std::string const& name, 
			   std::string input_name,
			   std::string lepton_name,
			   unsigned lepton_flavour,
			   double min, 
			   double max,
			   bool doZeppenfeldCut, 
			   std::string jet_name) : ModuleBase(name) {
    input_name_ = input_name;
    lepton_name_ = lepton_name;
    lepton_flavour_ = lepton_flavour;
    min_ = min;
    max_ = max;
    jet_name_ = jet_name;
    doZeppenfeldCut_ = doZeppenfeldCut;
  }

  MTSelection::~MTSelection(){
    ;
  }
  
  int MTSelection::PreAnalysis(){
    std::cout << " ** PreAnalysis Info for MTSelection **" << std::endl;
    std::cout << "Running MTSelection with lepton collection: " << lepton_name_ << std::endl;
    std::cout << "Running MTSelection with lepton flavour: " << lepton_flavour_ << std::endl;
    if (doZeppenfeldCut_) std::cout << " -- Zeppenfeld cut is applied with jet collection: " << jet_name_ << std::endl;

    return 0;
  }


  int MTSelection::Execute(TreeEvent *event){

    Met * lpfMet = event->GetPtr<Met>(input_name_);

    double lVal = -1;
    ROOT::Math::PtEtaPhiEVector lWreco;
    if (lepton_flavour_==1){
      std::vector<Electron *> lElecs = event->GetPtrVec<Electron>(lepton_name_);
      if (lElecs.size()==0) return 1;
      lVal = MT(lpfMet,lElecs[0]);
      lWreco = ic::reconstructWboson(lElecs[0],lpfMet);
    }
    else if (lepton_flavour_==2){
      std::vector<Muon *> lMuons = event->GetPtrVec<Muon>(lepton_name_);
      if (lMuons.size()==0) return 1;
      lVal = MT(lpfMet,lMuons[0]);
      lWreco = ic::reconstructWboson(lMuons[0],lpfMet);
     }
 
    double lZepp = 0;
    if (doZeppenfeldCut_) {
      std::vector<PFJet*> lJets = event->GetPtrVec<PFJet>(jet_name_);
      lZepp = fabs(lWreco.Y() - (lJets[0]->vector().Y()+lJets[1]->vector().Y())/2.);
    }

    if (lVal > min_ && lVal < max_ && lZepp < 1.2) return 0;
    else return 1;
    
  }

  int MTSelection::PostAnalysis(){
    return 0;
  }

  void MTSelection::PrintInfo(){
    ;
  }
  



}//namespace


