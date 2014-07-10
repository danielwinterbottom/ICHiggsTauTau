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
				Float_t binrange[19]={0,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300,400,500};
				jetpt_ = fs_->make<TH1F>("jetpt",";p_{T};",18,binrange);
				jeteta_ = fs_->make<TH1F>("jeteta",";#eta;",50,-2.5,2.5);
				jetphi_ = fs_->make<TH1F>("jetphi",";#phi;",100,-3.15,3.15);
				jetpt_b_ = fs_->make<TH1F>("jetpt_b",";p_{T};",18,binrange);
				jeteta_b_ = fs_->make<TH1F>("jeteta_b",";#eta;",50,-2.5,2.5);
				jetphi_b_ = fs_->make<TH1F>("jetphi_b",";#phi;",100,-3.15,3.15);
				jetpt_c_ = fs_->make<TH1F>("jetpt_c",";p_{T};",18,binrange);
				jeteta_c_ = fs_->make<TH1F>("jeteta_c",";#eta;",50,-2.5,2.5);
				jetphi_c_ = fs_->make<TH1F>("jetphi_c",";#phi;",100,-3.15,3.15);
				jetpt_g_ = fs_->make<TH1F>("jetpt_g",";p_{T};",18,binrange);
				jeteta_g_ = fs_->make<TH1F>("jeteta_g",";#eta;",50,-2.5,2.5);
				jetphi_g_ = fs_->make<TH1F>("jetphi_g",";#phi;",100,-3.15,3.15);
				jetpt_light_ = fs_->make<TH1F>("jetpt_light",";p_{T};",18,binrange);
				jeteta_light_ = fs_->make<TH1F>("jeteta_light",";#eta;",50,-2.5,2.5);
				jetphi_light_ = fs_->make<TH1F>("jetphi_light",";#phi;",100,-3.15,3.15);
				jetpt_undef_ = fs_->make<TH1F>("jetpt_undef",";p_{T};",18,binrange);
				jeteta_undef_ = fs_->make<TH1F>("jeteta_undef",";#eta;",50,-2.5,2.5);
				jetphi_undef_ = fs_->make<TH1F>("jetphi_undef",";#phi;",100,-3.15,3.15);


				taupt_ = fs_->make<TH1F>("taupt",";p_{T};",18,binrange);
				taueta_ = fs_->make<TH1F>("taueta",";#eta;",50,-2.5,2.5);
				tauphi_ = fs_->make<TH1F>("tauphi",";#phi;",100,-3.15,3.15);
				taupt_match_ = fs_->make<TH1F>("taupt_match",";p_{T};",18,binrange);
				taueta_match_ = fs_->make<TH1F>("taueta_match",";#eta;",50,-2.5,2.5);
				tauphi_match_ = fs_->make<TH1F>("tauphi_match",";#phi;",100,-3.15,3.15);
				dm_taupt_ = fs_->make<TH1F>("dm_taupt",";p_{T};",18,binrange);
				dm_taueta_ = fs_->make<TH1F>("dm_taueta",";#eta;",50,-2.5,2.5);
				dm_tauphi_ = fs_->make<TH1F>("dm_tauphi",";#phi;",100,-3.15,3.15);
				dm_taupt_match_ = fs_->make<TH1F>("dm_taupt_match",";p_{T};",18,binrange);
				dm_taueta_match_ = fs_->make<TH1F>("dm_taueta_match",";#eta;",50,-2.5,2.5);
				dm_tauphi_match_ = fs_->make<TH1F>("dm_tauphi_match",";#phi;",100,-3.15,3.15);
				loose_taupt_ = fs_->make<TH1F>("loose_taupt",";p_{T};",18,binrange);
				loose_taueta_ = fs_->make<TH1F>("loose_taueta",";#eta;",50,-2.5,2.5);
				loose_tauphi_ = fs_->make<TH1F>("loose_tauphi",";#phi;",100,-3.15,3.15);
				loose_taupt_match_ = fs_->make<TH1F>("loose_taupt_match",";p_{T};",18,binrange);
				loose_taueta_match_ = fs_->make<TH1F>("loose_taueta_match",";#eta;",50,-2.5,2.5);
				loose_tauphi_match_ = fs_->make<TH1F>("loose_tauphi_match",";#phi;",100,-3.15,3.15);
				loose_taupt_match_b_ = fs_->make<TH1F>("loose_taupt_match_b",";p_{T};",18,binrange);
				loose_taueta_match_b_ = fs_->make<TH1F>("loose_taueta_match_b",";#eta;",50,-2.5,2.5);
				loose_tauphi_match_b_ = fs_->make<TH1F>("loose_tauphi_match_b",";#phi;",100,-3.15,3.15);
				loose_taupt_match_c_ = fs_->make<TH1F>("loose_taupt_match_c",";p_{T};",18,binrange);
				loose_taueta_match_c_ = fs_->make<TH1F>("loose_taueta_match_c",";#eta;",50,-2.5,2.5);
				loose_tauphi_match_c_ = fs_->make<TH1F>("loose_tauphi_match_c",";#phi;",100,-3.15,3.15);
				loose_taupt_match_g_ = fs_->make<TH1F>("loose_taupt_match_g",";p_{T};",18,binrange);
				loose_taueta_match_g_ = fs_->make<TH1F>("loose_taueta_match_g",";#eta;",50,-2.5,2.5);
				loose_tauphi_match_g_ = fs_->make<TH1F>("loose_tauphi_match_g",";#phi;",100,-3.15,3.15);
				loose_taupt_match_light_ = fs_->make<TH1F>("loose_taupt_match_light",";p_{T};",18,binrange);
				loose_taueta_match_light_ = fs_->make<TH1F>("loose_taueta_match_light",";#eta;",50,-2.5,2.5);
				loose_tauphi_match_light_ = fs_->make<TH1F>("loose_tauphi_match_light",";#phi;",100,-3.15,3.15);
				loose_taupt_match_undef_ = fs_->make<TH1F>("loose_taupt_match_undef",";p_{T};",18,binrange);
				loose_taueta_match_undef_ = fs_->make<TH1F>("loose_taueta_match_undef",";#eta;",50,-2.5,2.5);
				loose_tauphi_match_undef_ = fs_->make<TH1F>("loose_tauphi_match_undef",";#phi;",100,-3.15,3.15);

				medium_taupt_ = fs_->make<TH1F>("medium_taupt",";p_{t};",18,binrange);
				medium_taueta_ = fs_->make<TH1F>("medium_taueta",";#eta;",50,-2.5,2.5);
				medium_tauphi_ = fs_->make<TH1F>("medium_tauphi",";#phi;",100,-3.15,3.15);
				medium_taupt_match_ = fs_->make<TH1F>("medium_taupt_match",";p_{t};",18,binrange);
				medium_taueta_match_ = fs_->make<TH1F>("medium_taueta_match",";#eta;",50,-2.5,2.5);
				medium_tauphi_match_ = fs_->make<TH1F>("medium_tauphi_match",";#phi;",100,-3.15,3.15);
				tight_taupt_ = fs_->make<TH1F>("tight_taupt",";p_{T};",18,binrange);
				tight_taueta_ = fs_->make<TH1F>("tight_taueta",";#eta;",50,-2.5,2.5);
				tight_tauphi_ = fs_->make<TH1F>("tight_tauphi",";#phi;",100,-3.15,3.15);
				tight_taupt_match_ = fs_->make<TH1F>("tight_taupt_match",";p_{T};",18,binrange);
				tight_taueta_match_ = fs_->make<TH1F>("tight_taueta_match",";#eta;",50,-2.5,2.5);
				tight_tauphi_match_ = fs_->make<TH1F>("tight_tauphi_match",";#phi;",100,-3.15,3.15);
				jetflavour_ = fs_->make<TH1F>("jetflavour",";flavour;",100,-30,30);
				deltar_ = fs_->make<TH1F>("deltaR",";#Delta R;",100,0,0.2);
				dm_deltar_ = fs_->make<TH1F>("dm_deltaR",";#Delta R;",100,0,0.2);
				loose_deltar_ = fs_->make<TH1F>("loose_deltaR",";Delta R;",100,0,0.2);
				medium_deltar_ = fs_->make<TH1F>("medium_deltaR",";Delta R;",100,0,0.2);
				tight_deltar_ = fs_->make<TH1F>("tight_deltaR",";#Delta R;",100,0,0.2);
				jetnvtx_ = fs_->make<TH1F>("jetnvtx_",";N_{vtx};",100,0,40);
				taunvtx_ = fs_->make<TH1F>("taunvtx_",";N_{vtx};",100,0,40);
				dm_taunvtx_ = fs_->make<TH1F>("dm_taunvtx_",";N_{vtx};",100,0,40);
				loose_taunvtx_ = fs_->make<TH1F>("loose_taunvtx_",";N_{vtx};",100,0,40);
				medium_taunvtx_ = fs_->make<TH1F>("medium_taunvtx_",";N_{vtx};",100,0,40);
				tight_taunvtx_ = fs_->make<TH1F>("tight_taunvtx_",";N_{vtx};",100,0,40);
				genjetpt_ = fs_->make<TH1F>("genjetpt","",18,binrange);

			}

		}
		return 0;
	}

	int JetTauFakeRate::Execute(TreeEvent *event) {

		// Get the objects we need from the event
		EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
		std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
		std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
		std::vector<GenJet*> genjets = event->GetPtrVec<GenJet>("genJets");

		if(write_plots_){
			nvtx_ = eventInfo->good_vertices();
			njets_ = jets.size();
			ntaus_ = taus.size();
			ntaus_dm_ = 0;
			ntaus_loose_=0;
			ntaus_medium_=0;
			ntaus_tight_=0;
			for(UInt_t tauit=0;tauit<taus.size();tauit++){
				if(taus.at(tauit)->GetTauID("decayModeFinding")>0.5){
					ntaus_dm_+=1;
					if(taus.at(tauit)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5) ntaus_loose_+=1;
					if(taus.at(tauit)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5) ntaus_medium_+=1;
					if(taus.at(tauit)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits")>0.5) ntaus_tight_+=1;
				}
			}
			if(njets_>0){
				jetnvtx_->Fill(nvtx_,njets_);
				taunvtx_->Fill(nvtx_,ntaus_);
				dm_taunvtx_->Fill(nvtx_,ntaus_dm_);
				loose_taunvtx_->Fill(nvtx_,ntaus_loose_);
				medium_taunvtx_->Fill(nvtx_,ntaus_medium_);
				tight_taunvtx_->Fill(nvtx_,ntaus_tight_);
				for(UInt_t genjetit=0;genjetit<genjets.size();genjetit++){
					genjetpt_->Fill(genjets.at(genjetit)->pt());
				}
			}

			for(UInt_t jetit=0;jetit<jets.size();jetit++){
				//jpt_=jets.at(jetit)->pt();
				//jeta_=jets.at(jetit)->eta();
				//	 outtree_->Fill();
				jetpt_->Fill(jets.at(jetit)->pt());
				jeteta_->Fill(jets.at(jetit)->eta());
				jetphi_->Fill(jets.at(jetit)->phi());
				jetflavour_->Fill(jets.at(jetit)->parton_flavour());
				if(jets.at(jetit)->parton_flavour()==5||jets.at(jetit)->parton_flavour()==-5){
					jetpt_b_->Fill(jets.at(jetit)->pt());
					jeteta_b_->Fill(jets.at(jetit)->eta());
					jetphi_b_->Fill(jets.at(jetit)->phi());
				}
				else if(jets.at(jetit)->parton_flavour()==4||jets.at(jetit)->parton_flavour()==-4){
					jetpt_c_->Fill(jets.at(jetit)->pt());
					jeteta_c_->Fill(jets.at(jetit)->eta());
					jetphi_c_->Fill(jets.at(jetit)->phi());
				}
				else if(jets.at(jetit)->parton_flavour()==1||jets.at(jetit)->parton_flavour()==-1||jets.at(jetit)->parton_flavour()==2||jets.at(jetit)->parton_flavour()==-2||
						jets.at(jetit)->parton_flavour()==3||jets.at(jetit)->parton_flavour()==-3){
					jetpt_light_->Fill(jets.at(jetit)->pt());
					jeteta_light_->Fill(jets.at(jetit)->eta());
					jetphi_light_->Fill(jets.at(jetit)->phi());
				}
				else if(jets.at(jetit)->parton_flavour()==21){
					jetpt_g_->Fill(jets.at(jetit)->pt());
					jeteta_g_->Fill(jets.at(jetit)->eta());
					jetphi_g_->Fill(jets.at(jetit)->phi());
				}

				else if(jets.at(jetit)->parton_flavour()==0){
					jetpt_undef_->Fill(jets.at(jetit)->pt());
					jeteta_undef_->Fill(jets.at(jetit)->eta());
					jetphi_undef_->Fill(jets.at(jetit)->phi());
				}


				theDR=100;
				thetaun=100;
				for(UInt_t tauit=0;tauit<taus.size();tauit++){
					if(ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector())<theDR){
						theDR=ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector());
						thetaun=tauit;
					}
				}

				if(jets.at(jetit)->pt()>20.&&TMath::Abs(jets.at(jetit)->eta())<2.3&&theDR<0.5){
					taupt_match_->Fill(jets.at(jetit)->pt());
					taueta_match_->Fill(jets.at(jetit)->eta());
					tauphi_match_->Fill(jets.at(jetit)->phi());

					if(taus.at(thetaun)->GetTauID("decayModeFinding")>0.5){
						dm_taupt_match_->Fill(jets.at(jetit)->pt());
						dm_taueta_match_->Fill(jets.at(jetit)->eta());
						dm_tauphi_match_->Fill(jets.at(jetit)->phi());

						if(taus.at(thetaun)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5){
							loose_taupt_match_->Fill(jets.at(jetit)->pt());
							loose_taueta_match_->Fill(jets.at(jetit)->eta());
							loose_tauphi_match_->Fill(jets.at(jetit)->phi());

							if(jets.at(jetit)->parton_flavour()==-5||jets.at(jetit)->parton_flavour()==5){
								loose_taupt_match_b_->Fill(jets.at(jetit)->pt());
								loose_taueta_match_b_->Fill(jets.at(jetit)->eta());
								loose_tauphi_match_b_->Fill(jets.at(jetit)->phi());
							}
							else if(jets.at(jetit)->parton_flavour()==1||jets.at(jetit)->parton_flavour()==-1||jets.at(jetit)->parton_flavour()==2||
									jets.at(jetit)->parton_flavour()==-2||jets.at(jetit)->parton_flavour()==3||jets.at(jetit)->parton_flavour()==-3){
								loose_taupt_match_light_->Fill(jets.at(jetit)->pt());
								loose_taueta_match_light_->Fill(jets.at(jetit)->eta());
								loose_tauphi_match_light_->Fill(jets.at(jetit)->phi());
							}
							else if(jets.at(jetit)->parton_flavour()==4||jets.at(jetit)->parton_flavour()==-4){
								loose_taupt_match_c_->Fill(jets.at(jetit)->pt());
								loose_taueta_match_c_->Fill(jets.at(jetit)->eta());
								loose_tauphi_match_c_->Fill(jets.at(jetit)->phi());
							}
							else if(jets.at(jetit)->parton_flavour()==21){
								loose_taupt_match_g_->Fill(jets.at(jetit)->pt());
								loose_taueta_match_g_->Fill(jets.at(jetit)->eta());
								loose_tauphi_match_g_->Fill(jets.at(jetit)->phi());
							}
							else if(jets.at(jetit)->parton_flavour()==0){
								loose_taupt_match_undef_->Fill(jets.at(jetit)->pt());
								loose_taueta_match_undef_->Fill(jets.at(jetit)->eta());
								loose_tauphi_match_undef_->Fill(jets.at(jetit)->phi());
							}
						}
					}

				}
			}


			/*	if(jets.size()!=0){
					for(UInt_t tauit=0;tauit<taus.size();tauit++){
					taupt_->Fill(taus.at(tauit)->pt());
					taueta_->Fill(taus.at(tauit)->eta());
					tauphi_->Fill(taus.at(tauit)->phi());
					if(GetDeltaR(taus.at(tauit),jets).first>-1){
					deltar_->Fill(GetDeltaR(taus.at(tauit),jets).first);
					if(jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt()>20.&&TMath::Abs(jets.at(GetDeltaR(taus.at(tauit),jets).second)->eta())<2.3){
//std::cout<<jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt()<<std::endl;
taupt_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt());
taueta_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->eta());
tauphi_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->phi());
}
}
if(taus.at(tauit)->GetTauID("decayModeFinding")>0.5){
dm_taupt_->Fill(taus.at(tauit)->pt());
dm_taueta_->Fill(taus.at(tauit)->eta());
dm_tauphi_->Fill(taus.at(tauit)->phi());
if(GetDeltaR(taus.at(tauit),jets).first>-1){
dm_deltar_->Fill(GetDeltaR(taus.at(tauit),jets).first);
dm_taupt_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt());
dm_taueta_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->eta());
dm_tauphi_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->phi());
}

if(taus.at(tauit)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5){
loose_taupt_->Fill(taus.at(tauit)->pt());
loose_taueta_->Fill(taus.at(tauit)->eta());
loose_tauphi_->Fill(taus.at(tauit)->phi());
if(GetDeltaR(taus.at(tauit),jets).first>-1){

loose_deltar_->Fill(GetDeltaR(taus.at(tauit),jets).first);


loose_taupt_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt());
loose_taueta_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->eta());
loose_tauphi_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->phi());
}
}
if(taus.at(tauit)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5){
medium_taupt_->Fill(taus.at(tauit)->pt());
medium_taueta_->Fill(taus.at(tauit)->eta());
medium_tauphi_->Fill(taus.at(tauit)->phi());
if(GetDeltaR(taus.at(tauit),jets).first>-1){

medium_deltar_->Fill(GetDeltaR(taus.at(tauit),jets).first);
medium_taupt_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt());
medium_taueta_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->eta());
medium_tauphi_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->phi());
}
}
if(taus.at(tauit)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits")>0.5){
tight_taupt_->Fill(taus.at(tauit)->pt());
tight_taueta_->Fill(taus.at(tauit)->eta());
tight_tauphi_->Fill(taus.at(tauit)->phi());
if(GetDeltaR(taus.at(tauit),jets).first>-1){

tight_deltar_->Fill(GetDeltaR(taus.at(tauit),jets).first);
tight_taupt_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->pt());
tight_taueta_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->eta());
tight_tauphi_match_->Fill(jets.at(GetDeltaR(taus.at(tauit),jets).second)->phi());
}
}
}
}
}
			 */


}

//Here goes the actual code to do the study. 

return 0;
}
//std::pair<double,unsigned int> JetTauFakeRate::GetDeltaR(Tau* tau_sig,std::vector<PFJet*> jet_vector){
	//deltaR_ = 100;
	//		std::pair<double,int> deltaR_pair_;
//	deltaR_pair_ = std::make_pair(100,0);

//	for(UInt_t ijet=0;ijet<jet_vector.size();ijet++){
//		deltaR_calc_ = TMath::Sqrt(TMath::Power(jet_vector.at(ijet)->eta()-tau_sig->eta(),2)+TMath::Power(jet_vector.at(ijet)->phi()-tau_sig->phi(),2));
//		deltaR_pair_ = (deltaR_calc_<deltaR_pair_.first) ? std::make_pair(deltaR_calc_,ijet) : deltaR_pair_;
//	}
//	deltaR_pair_ = deltaR_pair_.first>0.5 ? std::make_pair(-999.,(UInt_t) 999) : deltaR_pair_;
//	return deltaR_pair_;
//}

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
