#include "UserCode/ICHiggsTauTau/Analysis/JetTauFakeRate/interface/ExampleModule.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include<string>

namespace ic {


	ExampleModule::ExampleModule(std::string const& name) : ModuleBase(name) { 
		//add a few things configurable from the .cpp script
		write_plots_ = false;
		write_tree_ = true;
		by_decay_mode_ = false; //make plots for fake rate by decay mode
		by_jet_type_ = false; //make plots for fake rate by jet type
		wjets_mode_ = false;
		fs_ = NULL;
	}


	ExampleModule::~ExampleModule() {
		;
	}

	int ExampleModule::PreAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "ExampleModule" << std::endl;
		std::cout << "-------------------------------------" << std::endl;    
		if (fs_) {
			std::cout << boost::format(param_fmt()) % "write_plots"     % write_plots_;
			std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;


			//Setup a flat tree for the outputs, if this is what you want to use
			if (write_tree_) {
				outtree_ = fs_->make<TTree>("ntuple","ntuple");
				outtree_->Branch("jetpt",             &jetpt_);
				outtree_->Branch("jeteta",            &jeteta_);
				outtree_->Branch("jetphi",						&jetphi_);
				outtree_->Branch("jet_flavour",				&jet_flavour_);
				outtree_->Branch("taupt",							&taupt_);
				outtree_->Branch("taueta",						&taueta_);
				outtree_->Branch("tauphi",						&tauphi_);
				outtree_->Branch("taudmpass",					&taudmpass_);
				outtree_->Branch("taudm",							&taudm_);
				outtree_->Branch("tauloose",					&tauloose_);
				outtree_->Branch("taumedium",					&taumedium_);
				outtree_->Branch("tautight",					&tautight_);
  			outtree_->Branch("nvtx",							&nvtx_);
				//etc etc for whatever is important
			}
			if(write_plots_) {
				//Instead could initiate plots here which can be saved to the same file_
	}

		}

		return 0;
	}

	int ExampleModule::Execute(TreeEvent *event) {

		// Get the objects we need from the event
		EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
		std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
		std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");

		if(write_tree_){
			nvtx_ = eventInfo->good_vertices();

				for(UInt_t jetit=0;jetit<jets.size();jetit++){

					jetpt_=jets.at(jetit)->pt();
					jeteta_=jets.at(jetit)->eta();
					jetphi_=jets.at(jetit)->phi();
					jet_flavour_=jets.at(jetit)->parton_flavour();
				  std::vector<Jet *> jet_to_match;	
          jet_to_match.push_back(jets.at(jetit));
          std::vector<std::pair<Jet*, Tau*> > tau_match = MatchByDR(jet_to_match, taus, 0.2, true, true);
					if(tau_match.size()!=0){
     				taupt_=tau_match.at(0).second->pt();
						taueta_=tau_match.at(0).second->eta();
						tauphi_=tau_match.at(0).second->phi();
						taudmpass_=(double)tau_match.at(0).second->GetTauID("decayModeFindingNewDMs");
						if(taudmpass_>0.5){
						  taudm_=(int)tau_match.at(0).second->decay_mode();
						}
						else taudm_=-999;
						tauloose_=(double)tau_match.at(0).second->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits");
	  				taumedium_=(double)tau_match.at(0).second->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits");					
	  				tautight_=(double)tau_match.at(0).second->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits");					
					}
					else{
				   taupt_=-999;
					 taueta_=-999;
					 tauphi_=-999;
					 taudmpass_=-999;
					 taudm_=-999;
					 tauloose_=-999;
					 taumedium_=-999;
					 tautight_=-999;
					 }




			

		outtree_->Fill();
		}
		}
		return 0;
	}

	int ExampleModule::PostAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "ExampleModule" << std::endl;
		std::cout << "-------------------------------------" << std::endl;      
		return 0;
	}
	void ExampleModule::PrintInfo() {
		;
	}
}

