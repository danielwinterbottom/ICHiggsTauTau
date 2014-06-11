#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/JetTauFakeRateExample.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"

namespace ic {


  JetTauFakeRateExample::JetTauFakeRateExample(std::string const& name) : ModuleBase(name) { 
      //add a few things configurable from the .cpp script
      write_plots_ = false;
      write_tree_ = true;
      fs_ = NULL;
  }
  
  
  JetTauFakeRateExample::~JetTauFakeRateExample() {
    ;
  }
  
  int JetTauFakeRateExample::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "JetTauFakeRateExample" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    if (fs_) {
      std::cout << boost::format(param_fmt()) % "write_plots"     % write_plots_;
      std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;
     
     //Setup a flat tree for the outputs, if this is what you want to use
      if (write_tree_) {
        outtree_ = fs_->make<TTree>("ntuple","ntuple");
        outtree_->Branch("jpt",             &jpt_);
        outtree_->Branch("jeta",            &jeta_);
        //etc etc for whatever is important
      }
      if(write_plots_) {
          //Instead could initiate plots here which can be saved to the same file_
      }

    }
  }

  int JetTauFakeRateExample::Execute(TreeEvent *event) {
    
    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");

    //Here goes the actual code to do the study. 
  
    return 0;
  }
  
  int JetTauFakeRateExample::PostAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "JetTauFakeRateExample" << std::endl;
    std::cout << "-------------------------------------" << std::endl;      
    return 0;
  }
  void JetTauFakeRateExample::PrintInfo() {
    ;
  }
}
