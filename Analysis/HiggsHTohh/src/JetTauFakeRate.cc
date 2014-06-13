#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/JetTauFakeRate.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"

namespace ic {


	JetTauFakeRate::JetTauFakeRate(std::string const& name) : ModuleBase(name) { 
		//add a few things configurable from the .cpp script
		write_plots_ = true;
		write_tree_ = false;
		fs_ = NULL;
	}


	JetTauFakeRate::~JetTauFakeRate() {
		;
	}

	int JetTauFakeRate::PreAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "JetTauFakeRate" << std::endl;
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
				jetpt_ = fs_->make<TH1F>("jetpt","jetpt",100,0,500);
				jeteta_ = fs_->make<TH1F>("jeteta",";#eta;",100,-2.5,2.5);
				jetphi_ = fs_->make<TH1F>("jetphi","jetphi",100,-3.15,3.15);
				taupt_ = fs_->make<TH1F>("taupt","taupt",100,0,500);
				taueta_ = fs_->make<TH1F>("taueta","taueta",100,-2.5,2.5);
				tauphi_ = fs_->make<TH1F>("tauphi","tauphi",100,-3.15,3.15);
				loose_taupt_ = fs_->make<TH1F>("loose_taupt","loose_taupt",100,0,500);
				loose_taueta_ = fs_->make<TH1F>("loose_taueta","loose_taueta",100,-2.5,2.5);
				loose_tauphi_ = fs_->make<TH1F>("loose_tauphi","loose_tauphi",100,-3.15,3.15);
				medium_taupt_ = fs_->make<TH1F>("medium_taupt","medium_taupt",100,0,500);
				medium_taueta_ = fs_->make<TH1F>("medium_taueta","medium_taueta",100,-2.5,2.5);
				medium_tauphi_ = fs_->make<TH1F>("medium_tauphi","medium_tauphi",100,-3.15,3.15);
				tight_taupt_ = fs_->make<TH1F>("tight_taupt","tight_taupt",100,0,500);
				tight_taueta_ = fs_->make<TH1F>("tight_taueta","tight_taueta",100,-2.5,2.5);
				tight_tauphi_ = fs_->make<TH1F>("tight_tauphi","tight_tauphi",100,-3.15,3.15);
				jetflavour_ = fs_->make<TH1F>("jetflavour","jetflavour",100,-30,30);
				deltar_ = fs_->make<TH1F>("deltaR","deltaR",100,0,0.2);
				loose_deltar_ = fs_->make<TH1F>("loose_deltaR","loose_deltaR",100,0,0.2);
				medium_deltar_ = fs_->make<TH1F>("medium_deltaR","medium_deltaR",100,0,0.2);
				tight_deltar_ = fs_->make<TH1F>("tight_deltaR","tight_deltaR",100,0,0.2);
			}

		}
		return 0;
	}

	int JetTauFakeRate::Execute(TreeEvent *event) {

		// Get the objects we need from the event
		//EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
		std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
		std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");

		if(write_plots_){
			for(UInt_t jetit=0;jetit<jets.size();jetit++){
				jpt_=jets.at(jetit)->pt();
				jeta_=jets.at(jetit)->eta();
				//	 outtree_->Fill();
				jetpt_->Fill(jets.at(jetit)->pt());
				jeteta_->Fill(jets.at(jetit)->eta());
				jetphi_->Fill(jets.at(jetit)->phi());
				jetflavour_->Fill(jets.at(jetit)->parton_flavour());
			}


			for(UInt_t tauit=0;tauit<taus.size();tauit++){
				if(taus.at(tauit)->GetTauID("decayModeFinding")>0.5){
					taupt_->Fill(taus.at(tauit)->pt());
					taueta_->Fill(taus.at(tauit)->eta());
					tauphi_->Fill(taus.at(tauit)->phi());
					deltar_->Fill(GetDeltaR(taus.at(tauit),jets));
					if(taus.at(tauit)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5){
						loose_taupt_->Fill(taus.at(tauit)->pt());
						loose_taueta_->Fill(taus.at(tauit)->eta());
						loose_tauphi_->Fill(taus.at(tauit)->phi());
						loose_deltar_->Fill(GetDeltaR(taus.at(tauit),jets));
					}
					if(taus.at(tauit)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5){
						medium_taupt_->Fill(taus.at(tauit)->pt());
						medium_taueta_->Fill(taus.at(tauit)->eta());
						medium_tauphi_->Fill(taus.at(tauit)->phi());
						medium_deltar_->Fill(GetDeltaR(taus.at(tauit),jets));
					}
					if(taus.at(tauit)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits")>0.5){
						tight_taupt_->Fill(taus.at(tauit)->pt());
						tight_taueta_->Fill(taus.at(tauit)->eta());
						tight_tauphi_->Fill(taus.at(tauit)->phi());
						tight_deltar_->Fill(GetDeltaR(taus.at(tauit),jets));
					}
				}
			}



		}

		//Here goes the actual code to do the study. 

		return 0;
	}
	double JetTauFakeRate::GetDeltaR(Tau* tau_sig,std::vector<PFJet*> jet_vector){
		deltaR_ = 100;
		for(UInt_t ijet=0;ijet<jet_vector.size();ijet++){
			deltaR_calc_ = TMath::Sqrt(TMath::Power(jet_vector.at(ijet)->eta()-tau_sig->eta(),2)+TMath::Power(jet_vector.at(ijet)->phi()-tau_sig->phi(),2));
			deltaR_ = (deltaR_calc_<deltaR_) ? deltaR_calc_ : deltaR_;
		}
		return deltaR_;
	}

	int JetTauFakeRate::PostAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "JetTauFakeRate" << std::endl;
		std::cout << "-------------------------------------" << std::endl;      
		return 0;
	}
	void JetTauFakeRate::PrintInfo() {
		;
	}
}
