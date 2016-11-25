#include "UserCode/ICHiggsTauTau/Analysis/JetTauFakeRate/interface/JetTauFakeRate.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
//#include "DataFormats/TauReco/interface/PFTauFwd.h"
//#include "DataFormats/TauReco/interface/PFTau.h"
//#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include<string>

namespace ic {


	JetTauFakeRate::JetTauFakeRate(std::string const& name) : ModuleBase(name) { 
		//add a few things configurable from the .cpp script
		write_plots_ = false;
		write_tree_ = true;
		by_decay_mode_ = false; //make plots for fake rate by decay mode
		by_jet_type_ = false; //make plots for fake rate by jet type
		wjets_mode_ = false;
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
				outtree_->Branch("jetpt",             &jetpt_);
				outtree_->Branch("jeteta",            &jeteta_);
				outtree_->Branch("jetphi",						&jetphi_);
				outtree_->Branch("jettype",						&jettype_);
				outtree_->Branch("jetlinradmom",			&jetlinradmom_);
				outtree_->Branch("taupt",							&taupt_);
				outtree_->Branch("taueta",						&taueta_);
				outtree_->Branch("tauphi",						&tauphi_);
				outtree_->Branch("taudmpass",					&taudmpass_);
				outtree_->Branch("taudm",							&taudm_);
				outtree_->Branch("tauloose",					&tauloose_);
				outtree_->Branch("taumedium",					&taumedium_);
				outtree_->Branch("tautight",					&tautight_);
			  //outtree_->Branch("taumva",						&taumva_);
				outtree_->Branch("deltaR",						&theDR);
				outtree_->Branch("nvtx",							&nvtx_);
				outtree_->Branch("jetdz",							&jetdz_);
				outtree_->Branch("genjetpt",					&genjetpt_);
				outtree_->Branch("genjeteta",					&genjeteta_);
				outtree_->Branch("genjetphi",					&genjetphi_);
				outtree_->Branch("genjetdR",					&theDRgj);
				outtree_->Branch("ntracks",						&ntracks_);
				outtree_->Branch("sumpt",							&sumpt_);
				outtree_->Branch("sumet",							&sumet_);
				//etc etc for whatever is important
			}
			if(write_plots_) {
				//Instead could initiate plots here which can be saved to the same file_
	}

		}

		return 0;
	}

	int JetTauFakeRate::Execute(TreeEvent *event) {

		// Get the objects we need from the event
		EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
		std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
		std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
		Met const*  met = event->GetPtr<Met>("pfMet");
		std::vector<GenParticle*> genparticles;
		if(wjets_mode_){
		 genparticles = event->GetPtrVec<GenParticle>("genParticles");
		}
		std::vector<GenJet*> genjets = event->GetPtrVec<GenJet>("genJets");
		std::vector<Track*> tracks = event->GetPtrVec<Track>("tracks");
		std::vector<Vertex*> vertex = event->GetPtrVec<Vertex>("vertices");



		if(write_tree_){
			nvtx_ = eventInfo->good_vertices();
			ntaus_dm_ = 0;
			ntaus_loose_=0;
			ntaus_medium_=0;
			ntaus_tight_=0;



			ntracks_=0;
				sumpt_=0;
				if(vertex.size()!=0){
			 for(UInt_t trackit=0;trackit<tracks.size();trackit++){
					if((TMath::Abs(tracks.at(trackit)->vz()-vertex.at(0)->vz())<0.2)&&TMath::Abs(tracks.at(trackit)->eta())<2.3){
					  ntracks_+=1;
						sumpt_+=tracks.at(trackit)->pt();
						}
				}
			}

			sumet_ = met->sum_et();


				for(UInt_t jetit=0;jetit<jets.size();jetit++){


					thetrackid=-1;
					for(UInt_t trackit=0;trackit<tracks.size();trackit++){
						if(tracks.at(trackit)->id()==jets.at(jetit)->constituent_tracks().at(0)){
							thetrackid=trackit;
						}
					}
					if(thetrackid==-1){
						std::cout<<"ERROR!"<<std::endl;
						continue;
					}


					if(vertex.size()==0){
						std::cout<<"Vertex error!"<<std::endl;
						continue;
					}
					jetdz_=TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz());

					theDRgj=100;
					thegenjetn=100;
					for(UInt_t genjetit=0;genjetit<genjets.size();genjetit++){
						if(ROOT::Math::VectorUtil::DeltaR(genjets.at(genjetit)->vector(),jets.at(jetit)->vector())<theDR){
							theDRgj=ROOT::Math::VectorUtil::DeltaR(genjets.at(genjetit)->vector(),jets.at(jetit)->vector());
							thegenjetn=genjetit;
						}
					}
					if(thegenjetn!=100){
					genjetpt_ = genjets.at(thegenjetn)->pt();
					genjeteta_ = genjets.at(thegenjetn)->eta();
					genjetphi_ = genjets.at(thegenjetn)->phi();
					}
					else{
					genjetpt_ =-999;
					genjeteta_=-999;
					genjetphi_=-999;
					}
					jetpt_=jets.at(jetit)->pt();
					jeteta_=jets.at(jetit)->eta();
					jetphi_=jets.at(jetit)->phi();
					jettype_=jets.at(jetit)->parton_flavour();
					jetlinradmom_=jets.at(jetit)->linear_radial_moment();
					
					theDR=100;
					thetaun=100;
					for(UInt_t tauit=0;tauit<taus.size();tauit++){
						if(ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector())<theDR){
							theDR=ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector());
							thetaun=tauit;
						}
					}

					if(thetaun!=100){
     				taupt_=taus.at(thetaun)->pt();
						taueta_=taus.at(thetaun)->eta();
						tauphi_=taus.at(thetaun)->phi();
						taudmpass_=(double)taus.at(thetaun)->GetTauID("decayModeFindingOldDMs");
						if(taudmpass_>0.5){
						  taudm_=(int)taus.at(thetaun)->decay_mode();
						}
						else taudm_=-999;
						tauloose_=(double)taus.at(thetaun)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits");
	  				taumedium_=(double)taus.at(thetaun)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits");					
	  				tautight_=(double)taus.at(thetaun)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits");					
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

