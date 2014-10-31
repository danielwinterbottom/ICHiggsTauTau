#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/JetTauFakeRateExtras.h"
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


	JetTauFakeRateExtras::JetTauFakeRateExtras(std::string const& name) : ModuleBase(name) { 
		//add a few things configurable from the .cpp script
		write_plots_ = true;
		write_tree_ = false;
		by_decay_mode_ = false; //make plots for fake rate by decay mode
		by_jet_type_ = false; //make plots for fake rate by jet type
		wjets_mode_ = true;
		fs_ = NULL;
	}


	JetTauFakeRateExtras::~JetTauFakeRateExtras() {
		;
	}

	int JetTauFakeRateExtras::PreAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "JetTauFakeRateExtras" << std::endl;
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
				sum_et_ = fs_->make<TH1F>("sum_et",";#Sigma E_{T};",100,0,500);
				sum_pt_ = fs_->make<TH1F>("sum_pt",";#Sigma p_{T};",100,0,500);

   std::vector<std::string> jettype;
	 std::vector<std::string> sumEt;
	 std::vector<std::string> sumPt;
	 std::vector<std::string> iso;

				iso.push_back("dm");
				iso.push_back("loose");
				iso.push_back("medium");
				iso.push_back("tight");
				jettype.push_back("light");
				jettype.push_back("g");
				jettype.push_back("c");
				jettype.push_back("b");
				jettype.push_back("undef");
				sumEt.push_back("50");
				sumEt.push_back("100");
				sumEt.push_back("150");
				sumEt.push_back("200");
				sumEt.push_back("300");
				sumEt.push_back("400");
				sumEt.push_back("high");
				sumPt.push_back("50");
				sumPt.push_back("100");
				sumPt.push_back("150");
				sumPt.push_back("200");
				sumPt.push_back("300");
				sumPt.push_back("400");
				sumPt.push_back("high");
				


      for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
			for(UInt_t etit=0;etit<sumEt.size();etit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_sumet_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_sumet_"+sumEt.at(etit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_sumet_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_sumet_"+sumEt.at(etit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_sumet_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_sumet_"+sumEt.at(etit)).c_str(),";#phi;",100,-3.15,3.15);
						}
			for(UInt_t ptit=0;ptit<sumPt.size();ptit++){
			jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";#phi;",100,-3.15,3.15);
						}

					}
					//	std::map<std::string,TH1F*> tau_jettype_histos;
					for(UInt_t isoit=0;isoit<iso.size();isoit++){
						for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)).c_str(),";#phi;",100,-3.15,3.15);
							tau_jettype_histos_[(iso.at(isoit)+"_taunvtx_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taunvtx_match_"+jettype.at(jettypeit)).c_str(),";N_{vtx};",50,-0.5,49.5);
						}
					}


				/*jets_dz_ = fs_->make<TH1F>("jets_dz",";#Delta Z;",100,0,1);
				jets_pu_ = fs_->make<TH1F>("jets_pu",";Pileup jet ID;",100,-1,2);
				jets_overlap_=fs_->make<TH1F>("jets_overlap","",1,0,2);
				jetpt_dz_rej_ = fs_->make<TH1F>("jetpt_dz_rej_","",18,binrange);
				jetpt_puid_rej_ = fs_->make<TH1F>("jetpt_puid_rej_","",18,binrange);
				jetpt_dz_and_puid_rej_=fs_->make<TH1F>("jetpt_dz_and_puid_rej_","",18,binrange);
				genjetpt_taupt_200_300_ = fs_->make<TH1F>("genjetpt_taupt_200_300_","",18,binrange);
				genjetpt_taupt_300_400_ = fs_->make<TH1F>("genjetpt_taupt_300_400_","",18,binrange);
				genjetpt_taupt_400_ = fs_->make<TH1F>("genjetpt_taupt_400_","",18,binrange);
				genjetpt_jetpt_200_300_ = fs_->make<TH1F>("genjetpt_jetpt_200_300_","",18,binrange);
				genjetpt_jetpt_300_400_ = fs_->make<TH1F>("genjetpt_jetpt_300_400_","",18,binrange);
				genjetpt_jetpt_400_ = fs_->make<TH1F>("genjetpt_jetpt_400_","",18,binrange);
				taupt_test_hist_ = fs_->make<TH1F>("taupt_test_hist","",50,0,100);


				taupt_dm_iso_nomatch_ = fs_->make<TH1F>("taupt_dm_iso_nomatch",";p_{T};",18,binrange);
				taueta_dm_iso_nomatch_ = fs_->make<TH1F>("taueta_dm_iso_nomatch",";#eta;",50,-2.5,2.5);
				tauphi_dm_iso_nomatch_ = fs_->make<TH1F>("tauphi_dm_iso_nomatch",";#phi;",100,-3.15,3.15);





				//				jet_type_["jetpt"]=fs_->make<TH1F>("jetpt",";p_{T};",18,binrange);
				std::vector<std::string> iso;
				std::vector<std::string> dm;
				std::vector<std::string> jettype;
				std::cout<<"type vectors created"<<std::endl;

				iso.push_back("dm");
				iso.push_back("loose");
				iso.push_back("medium");
				iso.push_back("tight");
				dm.push_back("OneProng0PiZero");
				dm.push_back("OneProng1PiZero");
				dm.push_back("OneProng2PiZero");
				dm.push_back("OneProng3PiZero");
				dm.push_back("OneProngNPiZero");
				dm.push_back("TwoProng0PiZero");
				dm.push_back("TwoProng1PiZero");
				dm.push_back("TwoProng2PiZero");
				dm.push_back("TwoProng3PiZero");
				dm.push_back("TwoProngNPiZero");
				dm.push_back("ThreeProng0PiZero");
				dm.push_back("ThreeProng1PiZero");
				dm.push_back("ThreeProng2PiZero");
				dm.push_back("ThreeProng3PiZero");
				dm.push_back("ThreeProngNPiZero");
				jettype.push_back("light");
				jettype.push_back("g");
				jettype.push_back("c");
				jettype.push_back("b");
				jettype.push_back("undef");

				std::cout<<"type vectors filled"<<std::endl;

				jetpt_ = fs_->make<TH1F>("jetpt",";p_{T};",18,binrange);
				jeteta_ = fs_->make<TH1F>("jeteta",";#eta;",50,-2.5,2.5);
				jetphi_ = fs_->make<TH1F>("jetphi",";#phi;",100,-3.15,3.15);
				jetnvtx_ = fs_->make<TH1F>("jetnvtx",";N_{vtx};",50,-0.5,49.5);
				std::cout<<"created stuff"<<std::endl;
				//std::map<std::string,TH1F*> standard_tau_histos_;
				for(UInt_t isoit=0;isoit<iso.size();isoit++){
					standard_tau_histos_[(iso.at(isoit)+"_taupt_match").c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match").c_str(),";p_{T} [GeV];",18,binrange);
					standard_tau_histos_[(iso.at(isoit)+"_taueta_match").c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match").c_str(),";#eta;",50,-2.5,2.5);
					standard_tau_histos_[(iso.at(isoit)+"_tauphi_match").c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match").c_str(),";#phi;",100,-3.15,3.15);
					standard_tau_histos_[(iso.at(isoit)+"_taunvtx_match").c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taunvtx_match").c_str(),";N_{vtx};",50,-0.5,49.5);
				}
				std::cout<<"standard_tau_histos_ initialised"<<std::endl;



				if(by_jet_type_){
					std::map<std::string,TH1F*> jettype_histos;
					for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)).c_str(),";#phi;",100,-3.15,3.15);
						jettype_histos_[("jetnvtx_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>(("jetnvtx_"+jettype.at(jettypeit)).c_str(),";N_{vtx};",50,-0.5,49.5);
					}
					//	std::map<std::string,TH1F*> tau_jettype_histos;
					for(UInt_t isoit=0;isoit<iso.size();isoit++){
						for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)).c_str(),";#phi;",100,-3.15,3.15);
							tau_jettype_histos_[(iso.at(isoit)+"_taunvtx_match_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taunvtx_match_"+jettype.at(jettypeit)).c_str(),";N_{vtx};",50,-0.5,49.5);
						}
					}


				}




				jetflavour_ = fs_->make<TH1F>("jetflavour",";flavour;",100,-30,30);
				genjetpt_ = fs_->make<TH1F>("genjetpt","",18,binrange);
				genjeteta_ = fs_->make<TH1F>("genjeteta","",18,binrange);

				if(by_decay_mode_){
					//			std::map<std::string,TH1F*> dm_tau_histos;
					for(UInt_t isoit=0;isoit<iso.size();isoit++){
						for(UInt_t dmit=0;dmit<dm.size();dmit++){
							dm_tau_histos_[(iso.at(isoit)+"_taupt_match_"+dm.at(dmit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+dm.at(dmit)).c_str(),";p_{T} [GeV];",18,binrange);
							dm_tau_histos_[(iso.at(isoit)+"_taueta_match_"+dm.at(dmit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+dm.at(dmit)).c_str(),";#eta;",50,-2.5,2.5);
							dm_tau_histos_[(iso.at(isoit)+"_tauphi_match_"+dm.at(dmit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+dm.at(dmit)).c_str(),";#phi;",100,-3.15,3.15);
							dm_tau_histos_[(iso.at(isoit)+"_taunvtx_match_"+dm.at(dmit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taunvtx_match_"+dm.at(dmit)).c_str(),";N_{vtx};",50,-0.5,49.5);
							if(by_jet_type_){
								for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
									dm_tau_histos_[(iso.at(isoit)+"_taupt_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str(),";p_{T} [GeV];",18,binrange);
									dm_tau_histos_[(iso.at(isoit)+"_taueta_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str(),";#eta;",50,-2.5,2.5);
									dm_tau_histos_[(iso.at(isoit)+"_tauphi_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str(),";#phi;",100,-3.15,3.15);
									dm_tau_histos_[(iso.at(isoit)+"_taunvtx_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taunvtx_match_"+dm.at(dmit)+"_"+jettype.at(jettypeit)).c_str(),";N_{vtx};",50,-0.5,49.5);
								}
							}
						}
					}
				}



			*/
			}

		}

		return 0;
	}

	int JetTauFakeRateExtras::Execute(TreeEvent *event) {

		// Get the objects we need from the event
		EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
		std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
		std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
		std::vector<GenParticle*> genparticles;
		//if(wjets_mode_){
		 genparticles = event->GetPtrVec<GenParticle>("genParticles");
		//}
		std::vector<GenJet*> genjets = event->GetPtrVec<GenJet>("genJets");
		std::vector<Track*> tracks = event->GetPtrVec<Track>("tracks");
		std::vector<Vertex*> vertex = event->GetPtrVec<Vertex>("vertices");



		if(write_plots_){
			nvtx_ = eventInfo->good_vertices();
			sumet_=0;
			for(UInt_t genparticleit=0;genparticleit<genparticles.size();genparticleit++){
				//std::cout<<genparticles.at(genparticleit)->eta()<<std::endl;
				//std::cout<<"PDG ID: "<<genparticles.at(genparticleit)->pdgid()<<std::endl;
				//std::cout<<genparticles.at(genparticleit)->status()<<std::endl;

			  if(genparticles.at(genparticleit)->status()!=3){
				  if(genparticles.at(genparticleit)->pdgid()!=12&&genparticles.at(genparticleit)->pdgid()!=14&&genparticles.at(genparticleit)->pdgid()!=16&&genparticles.at(genparticleit)->pdgid()!=-12&&genparticles.at(genparticleit)->pdgid()!=-14&&genparticles.at(genparticleit)->pdgid()!=-16){
					sumet_+=genparticles.at(genparticleit)->energy();
					}
			}
			}
			
			sum_et_->Fill(sumet_);

					sumpt_=0;
					for(UInt_t trackit=0;trackit<tracks.size();trackit++){
					if(TMath::Abs(tracks.at(trackit)->vz()-vertex.at(0)->vz())<0.2){
					sumpt_+=tracks.at(trackit)->pt();
					}
					}
					sum_pt_->Fill(sumpt_);

					

					

         if(sumpt_<50){
				   sumpt_string="50";
					 }
				 else if(sumpt_<100){
				   sumpt_string="100";
					 }
				 else if(sumpt_<150){
				   sumpt_string="150";
					 }
				 else if(sumpt_<200){
				   sumpt_string="200";
					 }
				 else if(sumpt_<300){
				   sumpt_string="300";
					 }
				 else if(sumpt_<400){
				   sumpt_string="400";
					 }
				else sumpt_string="high";

         if(sumet_<50){
				   sumet_string="50";
					 }
				 else if(sumet_<100){
				   sumet_string="100";
					 }
				 else if(sumet_<150){
				   sumet_string="150";
					 }
				 else if(sumet_<200){
				   sumet_string="200";
					 }
				 else if(sumet_<300){
				   sumet_string="300";
					 }
				 else if(sumet_<400){
				   sumet_string="400";
					 }
				else sumet_string="high";

					

         	std::map<int,std::string> jettypelist;
					jettypelist[-5]="b";
					jettypelist[5]="b";
					jettypelist[-4]="c";
					jettypelist[4]="c";
					jettypelist[-3]="light";
					jettypelist[-2]="light";
					jettypelist[-1]="light";
					jettypelist[1]="light";
					jettypelist[2]="light";
					jettypelist[3]="light";
					jettypelist[21]="g";
					jettypelist[0]="undef";


			
			 for(UInt_t jetit=0;jetit<jets.size();jetit++){
			  jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumet_"+sumet_string).c_str()]->Fill(jets.at(jetit)->pt());
				jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumet_"+sumet_string).c_str()]->Fill(jets.at(jetit)->eta());
				jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumet_"+sumet_string).c_str()]->Fill(jets.at(jetit)->phi());
			  jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->pt());
				jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->eta());
				jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->phi());
				}










					/*for(UInt_t tauit=0;tauit<taus.size();tauit++){
					if(taus.at(tauit)->GetTauID("decayModeFindingOldDMs")>0.5){
						ntaus_dm_+=1;
						if(taus.at(tauit)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5){
							ntaus_loose_+=1;
							taupt_dm_iso_nomatch_->Fill(taus.at(tauit)->pt());
							taueta_dm_iso_nomatch_->Fill(taus.at(tauit)->eta());
							tauphi_dm_iso_nomatch_->Fill(taus.at(tauit)->phi());
						}
						if(taus.at(tauit)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5) ntaus_medium_+=1;
						if(taus.at(tauit)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits")>0.5) ntaus_tight_+=1;



					}
				}

				std::cout<<"EVENT NUMBER "<<eventInfo->event()<<std::endl;
				for(UInt_t jetit=0;jetit<jets.size();jetit++){
				std::cout<<"jetPT "<<jets.at(jetit)->pt()<<" jet eta "<<jets.at(jetit)->eta()<<" jet phi "<<jets.at(jetit)->phi()<<std::endl;

					//std::cout<<jets.at(jetit)->pu_id_mva_loose()<<std::endl;

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
					jets_dz_->Fill(TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz()));
					if(TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz())>0.2){
						jetpt_dz_rej_->Fill(jets.at(jetit)->pt());
					}
					if(!PileupJetID(jets.at(jetit),2)){
						jetpt_puid_rej_->Fill(jets.at(jetit)->pt());
					}

					jets_pu_->Fill(PileupJetID(jets.at(jetit),2));

					if(TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz())>0.2&&!PileupJetID(jets.at(jetit),2)){
						jets_overlap_->Fill(1);
						jetpt_dz_and_puid_rej_->Fill(jets.at(jetit)->pt());
					}
					std::cout<<"Delta Z "<<TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz())<<std::endl;
					std::cout<<"AAA"<<std::endl;

					if(TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz())<0.2){

					theDRgj=100;
					thegenjetn=100;
					for(UInt_t genjetit=0;genjetit<genjets.size();genjetit++){
						if(ROOT::Math::VectorUtil::DeltaR(genjets.at(genjetit)->vector(),jets.at(jetit)->vector())<theDR){
							theDRgj=ROOT::Math::VectorUtil::DeltaR(genjets.at(genjetit)->vector(),jets.at(jetit)->vector());
							thegenjetn=genjetit;
						}
					}
					if(theDRgj<0.5){
						if(jets.at(jetit)->pt()>400){
							genjetpt_jetpt_400_->Fill(genjets.at(thegenjetn)->pt()); 
						}
						else if(jets.at(jetit)->pt()>300){
							genjetpt_jetpt_300_400_->Fill(genjets.at(thegenjetn)->pt());
						}
						else if(jets.at(jetit)->pt()>200){
							genjetpt_jetpt_200_300_->Fill(genjets.at(thegenjetn)->pt());
						}
					} 

					//jpt_=jets.at(jetit)->pt();
					//jeta_=jets.at(jetit)->eta();
					//	 outtree_->Fill();
					jetpt_->Fill(jets.at(jetit)->pt());
					jeteta_->Fill(jets.at(jetit)->eta());
					jetphi_->Fill(jets.at(jetit)->phi());
					jetflavour_->Fill(jets.at(jetit)->parton_flavour());

					std::map<int,std::string> jettypelist;
					jettypelist[-5]="b";
					jettypelist[5]="b";
					jettypelist[-4]="c";
					jettypelist[4]="c";
					jettypelist[-3]="light";
					jettypelist[-2]="light";
					jettypelist[-1]="light";
					jettypelist[1]="light";
					jettypelist[2]="light";
					jettypelist[3]="light";
					jettypelist[21]="g";
					jettypelist[0]="undef";
					//				std::cout<<"jettypelist created"<<std::endl;


					//				std::cout<<"starting to fill jettype histos"<<std::endl;
					jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
					jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
					jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
					//				std::cout<<"finished filling jettype_histos"<<std::endl;




					theDR=100;
					thetaun=100;
					std::cout<<"asjd"<<std::endl;
					for(UInt_t tauit=0;tauit<taus.size();tauit++){
						if(ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector())<theDR){
							theDR=ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector());
							thetaun=tauit;
						}
					}
					
				if(theDR<0.5){
				std::cout<<"theDR "<<theDR<<" tau pT "<<taus.at(thetaun)->pt()<<" eta "<<taus.at(thetaun)->eta()<<std::endl;
					std::cout<<"dm finding "<<taus.at(thetaun)->GetTauID("decayModeFinding")<<" loose isolation "<<taus.at(thetaun)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")<<std::endl;
					}
					




					if(jets.at(jetit)->pt()>20.&&TMath::Abs(jets.at(jetit)->eta())<2.3&&theDR<0.5&&taus.at(thetaun)->GetTauID("decayModeFindingOldDMs")>0.5){
						standard_tau_histos_["dm_taupt_match"]->Fill(jets.at(jetit)->pt());
						standard_tau_histos_["dm_taueta_match"]->Fill(jets.at(jetit)->eta());
						standard_tau_histos_["dm_tauphi_match"]->Fill(jets.at(jetit)->phi());

						if(taus.at(thetaun)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5){
							standard_tau_histos_["loose_taupt_match"]->Fill(jets.at(jetit)->pt());
							standard_tau_histos_["loose_taueta_match"]->Fill(jets.at(jetit)->eta());
							standard_tau_histos_["loose_tauphi_match"]->Fill(jets.at(jetit)->phi());
							if(jets.at(jetit)->pt()<50.){
								taupt_test_hist_->Fill(taus.at(thetaun)->pt());
							}

							if(theDRgj<0.5){
								if(jets.at(jetit)->pt()>400){
									genjetpt_taupt_400_->Fill(genjets.at(thegenjetn)->pt());
								}
								else if(jets.at(jetit)->pt()>300){
									genjetpt_taupt_300_400_->Fill(genjets.at(thegenjetn)->pt());
								}
								else if(jets.at(jetit)->pt()>200){
									genjetpt_taupt_200_300_->Fill(genjets.at(thegenjetn)->pt());
								}
							}
							if(by_jet_type_){
								tau_jettype_histos_[("loose_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("loose_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("loose_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
							}

							if(by_decay_mode_){
								dm_tau_histos_[("loose_taupt_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->pt());
								dm_tau_histos_[("loose_taueta_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->eta());
								dm_tau_histos_[("loose_tauphi_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->phi());
								if(by_jet_type_){
									dm_tau_histos_[("loose_taupt_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
									dm_tau_histos_[("loose_taueta_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
									dm_tau_histos_[("loose_tauphi_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
								}
							}
						}

						if(taus.at(thetaun)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5){
							standard_tau_histos_["medium_taupt_match"]->Fill(jets.at(jetit)->pt());
							standard_tau_histos_["medium_taueta_match"]->Fill(jets.at(jetit)->eta());
							standard_tau_histos_["medium_tauphi_match"]->Fill(jets.at(jetit)->phi());

							if(by_jet_type_){
								tau_jettype_histos_[("medium_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("medium_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("medium_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
							}

							if(by_decay_mode_){
								dm_tau_histos_[("medium_taupt_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->pt());
								dm_tau_histos_[("medium_taueta_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->eta());
								dm_tau_histos_[("medium_tauphi_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->phi());
								if(by_jet_type_){
									dm_tau_histos_[("medium_taupt_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
									dm_tau_histos_[("medium_taueta_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
									dm_tau_histos_[("medium_tauphi_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
								}
							}
						}


						if(taus.at(thetaun)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits")>0.5){
							standard_tau_histos_["tight_taupt_match"]->Fill(jets.at(jetit)->pt());
							standard_tau_histos_["tight_taueta_match"]->Fill(jets.at(jetit)->eta());
							standard_tau_histos_["tight_tauphi_match"]->Fill(jets.at(jetit)->phi());

							if(by_jet_type_){
								tau_jettype_histos_[("tight_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("tight_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("tight_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
							}

							if(by_decay_mode_){
								dm_tau_histos_[("tight_taupt_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->pt());
								dm_tau_histos_[("tight_taueta_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->eta());
								dm_tau_histos_[("tight_tauphi_match_"+dmlist[taus.at(thetaun)->decay_mode()]).c_str()]->Fill(jets.at(jetit)->phi());
								if(by_jet_type_){
									dm_tau_histos_[("tight_taupt_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->pt());
									dm_tau_histos_[("tight_taueta_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->eta());
									dm_tau_histos_[("tight_tauphi_match_"+dmlist[taus.at(thetaun)->decay_mode()]+"_"+jettypelist[jets.at(jetit)->parton_flavour()]).c_str()]->Fill(jets.at(jetit)->phi());
								}
							}
						}



					}
					}
				}

				//Here goes the actual code to do the study. 
			
		*/}
		return 0;
	}

	int JetTauFakeRateExtras::PostAnalysis() {
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "JetTauFakeRateExtras" << std::endl;
		std::cout << "-------------------------------------" << std::endl;      
		return 0;
	}
	void JetTauFakeRateExtras::PrintInfo() {
		;
	}
}

