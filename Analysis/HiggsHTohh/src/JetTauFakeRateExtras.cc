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
		wjets_mode_ = false;
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
				loose_taupt_match_ = fs_->make<TH1F>("loose_taupt_match",";p_{T} [GeV]",18,binrange);
				jetpt_ = fs_->make<TH1F>("jetpt",";p_{T} [GeV];",18,binrange);
				histntracks_ = fs_->make<TH1F>("ntracks",";N_{tracks};",50,0,200);

				std::vector<std::string> jettype;
				std::vector<std::string> sumEt;
				std::vector<std::string> sumPt;
				std::vector<std::string> iso;
				std::vector<std::string> nVtx;
				std::vector<std::string> nTracks;
				std::vector<std::string> nJets;

				iso.push_back("dm");
				iso.push_back("loose");
				iso.push_back("medium");
				iso.push_back("tight");
				jettype.push_back("light");
				jettype.push_back("g");
				jettype.push_back("c");
				jettype.push_back("b");
				jettype.push_back("undef");
				sumEt.push_back("20");
				sumEt.push_back("40");
				sumEt.push_back("60");
				sumEt.push_back("80");
				sumEt.push_back("100");
				sumEt.push_back("200");
				sumEt.push_back("high");
				sumPt.push_back("20");
				sumPt.push_back("40");
				sumPt.push_back("60");
				sumPt.push_back("80");
				sumPt.push_back("100");
				sumPt.push_back("150");
				sumPt.push_back("high");
				nVtx.push_back("5");
				nVtx.push_back("10");
				nVtx.push_back("15");
				nVtx.push_back("20");
				nVtx.push_back("high");
				nTracks.push_back("20");
				nTracks.push_back("30");
				nTracks.push_back("40");
				nTracks.push_back("50");
				nTracks.push_back("60");
				nTracks.push_back("80");
				nTracks.push_back("high");
				nJets.push_back("2");
				nJets.push_back("4");
				nJets.push_back("6");
				nJets.push_back("8");
				nJets.push_back("10");
				nJets.push_back("15");
				nJets.push_back("high");

				



				for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
					for(UInt_t etit=0;etit<sumEt.size();etit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str(),";#phi;",100,-3.15,3.15);

					}
					for(UInt_t ptit=0;ptit<sumPt.size();ptit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";#phi;",100,-3.15,3.15);
					}
					for(UInt_t vtxit=0;vtxit<nVtx.size();vtxit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str(),";#phi;",100,-3.15,3.15);
					}
					for(UInt_t ntrackit=0;ntrackit<nTracks.size();ntrackit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str(),";#phi;",100,-3.15,3.15);
					}
					for(UInt_t numjetit=0;numjetit<nJets.size();numjetit++){
						jettype_histos_[("jetpt_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str()]=fs_->make<TH1F>(("jetpt_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str(),";p_{T} [GeV];",18,binrange);
						jettype_histos_[("jeteta_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str()]=fs_->make<TH1F>(("jeteta_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str(),";#eta;",50,-2.5,2.5);
						jettype_histos_[("jetphi_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str()]=fs_->make<TH1F>(("jetphi_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str(),";#phi;",100,-3.15,3.15);
					}


				}
				//	std::map<std::string,TH1F*> tau_jettype_histos;
				for(UInt_t isoit=0;isoit<iso.size();isoit++){
					for(UInt_t jettypeit=0;jettypeit<jettype.size();jettypeit++){
						for(UInt_t etit=0;etit<sumEt.size();etit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_leadtrpt_"+sumEt.at(etit)).c_str(),";#phi;",100,-3.15,3.15);
						}
						for(UInt_t ptit=0;ptit<sumPt.size();ptit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_sumpt_"+sumPt.at(ptit)).c_str(),";#phi;",100,-3.15,3.15);
						}
						for(UInt_t vtxit=0;vtxit<nVtx.size();vtxit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_nvtx_"+nVtx.at(vtxit)).c_str(),";#phi;",100,-3.15,3.15);
						}
						for(UInt_t ntrackit=0;ntrackit<nTracks.size();ntrackit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_ntracks_"+nTracks.at(ntrackit)).c_str(),";#phi;",100,-3.15,3.15);
						}
						for(UInt_t numjetit=0;numjetit<nJets.size();numjetit++){
							tau_jettype_histos_[(iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taupt_match_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str(),";p_{T} [GeV];",18,binrange);
							tau_jettype_histos_[(iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_taueta_match_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str(),";#eta;",50,-2.5,2.5);
							tau_jettype_histos_[(iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str()]=fs_->make<TH1F>((iso.at(isoit)+"_tauphi_match_"+jettype.at(jettypeit)+"_numjets_"+nJets.at(numjetit)).c_str(),";#phi;",100,-3.15,3.15);
						}

					}
				}


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
				if(wjets_mode_){
		genparticles = event->GetPtrVec<GenParticle>("genParticles");
				}
		std::vector<GenJet*> genjets = event->GetPtrVec<GenJet>("genJets");
		std::vector<Track*> tracks = event->GetPtrVec<Track>("tracks");
		std::vector<Vertex*> vertex = event->GetPtrVec<Vertex>("vertices");



		if(write_plots_){
			nvtx_ = eventInfo->good_vertices();
			sumet_=0;
			if(wjets_mode_){
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
			}




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
			sumpt_=0;
			ntracks_=0;
			leadtrpt_=0;
			njets_=jets.size();

				if(vertex.size()!=0){
//			for(UInt_t constrackit=0;constrackit<(jets.at(jetit)->constituent_tracks()).size();constrackit++){
			 for(UInt_t trackit=0;trackit<tracks.size();trackit++){
					//if(tracks.at(trackit)->id()==jets.at(jetit)->constituent_tracks().at(constrackit)){
					if((TMath::Abs(tracks.at(trackit)->vz()-vertex.at(0)->vz())<0.2)&&TMath::Abs(tracks.at(trackit)->eta())<2.3){
					//if(constrackit==0) leadtrpt_ = tracks.at(trackit)->pt();
					  ntracks_+=1;
						sumpt_+=tracks.at(trackit)->pt();
						}
				//}
//				}
				}
			}


			histntracks_->Fill(ntracks_);
			if(ntracks_<20){
			  ntracks_string="20";
			}
			else if(ntracks_<30){
			 ntracks_string="30";
			}
			else if(ntracks_<40){
			 ntracks_string="40";
			}
			else if(ntracks_<50){
			 ntracks_string="50";
			}
			else if(ntracks_<60){
			 ntracks_string="60";
			}
			else if(ntracks_<80){
			 ntracks_string="80";
			}
			else ntracks_string="high";






			if(sumpt_<20){
				sumpt_string="20";
			}
			else if(sumpt_<40){
				sumpt_string="40";
			}
			else if(sumpt_<60){
				sumpt_string="60";
			}
			else if(sumpt_<80){
				sumpt_string="80";
			}
			else if(sumpt_<100){
				sumpt_string="100";
			}
			else if(sumpt_<150){
				sumpt_string="150";
			}
			else sumpt_string="high";

			if(leadtrpt_<20){
				leadtrpt_string="20";
			}
			else if(leadtrpt_<40){
				leadtrpt_string="40";
			}
			else if(leadtrpt_<60){
				leadtrpt_string="60";
			}
			else if(leadtrpt_<80){
				leadtrpt_string="80";
			}
			else if(leadtrpt_<100){
				leadtrpt_string="100";
			}
			else if(leadtrpt_<200){
				leadtrpt_string="200";
			}
			else leadtrpt_string="high";

			if(nvtx_<=5){
				nvtx_string="5";
			}
			else if(nvtx_<=10){
				nvtx_string="10";
			}
			else if(nvtx_<=15){
				nvtx_string="15";
			}
			else if(nvtx_<=20){
				nvtx_string="20";
			}
			else nvtx_string="high";

			if(njets_<=2){;
			njets_string="2";
			}
			else if(njets_<=4){
			njets_string="4";
			}
			else if(njets_<=6){
			njets_string="6";
			}
			else if(njets_<=8){
			njets_string="8";
			}
			else if(njets_<=10){
			njets_string="10";
			}
			else njets_string="high";



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

				if(TMath::Abs(tracks.at(thetrackid)->vz()-vertex.at(0)->vz())<0.2){
			
				

					if(jets.at(jetit)->pt()>20.&&TMath::Abs(jets.at(jetit)->eta())<2.3){
					jetpt_->Fill(jets.at(jetit)->pt());
						jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->pt());
						jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->eta());
						jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->phi());
						jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->pt());
						jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->eta());
						jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->phi());
						jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->pt());
						jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->eta());
						jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->phi());
						jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->pt());
						jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->eta());
						jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->phi());
						jettype_histos_[("jetpt_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->pt());
						jettype_histos_[("jeteta_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->eta());
						jettype_histos_[("jetphi_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->phi());

					}


					theDR=100;
					thetaun=100;
					for(UInt_t tauit=0;tauit<taus.size();tauit++){
						if(ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector())<theDR){
							theDR=ROOT::Math::VectorUtil::DeltaR(taus.at(tauit)->vector(),jets.at(jetit)->vector());
							thetaun=tauit;
						}
					}



//					if(jets.at(jetit)->pt()>20.&&TMath::Abs(jets.at(jetit)->eta())<2.3&&theDR<0.5&&taus.at(thetaun)->GetTauID("decayModeFindingOldDMs")>0.5){
					if(jets.at(jetit)->pt()>20.&&TMath::Abs(jets.at(jetit)->eta())<2.3&&theDR<0.5){

						tau_jettype_histos_[("dm_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->pt());
						tau_jettype_histos_[("dm_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->eta());
						tau_jettype_histos_[("dm_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->phi());
						tau_jettype_histos_[("dm_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->pt());
						tau_jettype_histos_[("dm_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->eta());
						tau_jettype_histos_[("dm_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->phi());
						tau_jettype_histos_[("dm_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->pt());
						tau_jettype_histos_[("dm_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->eta());
						tau_jettype_histos_[("dm_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->phi());
            tau_jettype_histos_[("dm_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->pt());
						tau_jettype_histos_[("dm_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->eta());
						tau_jettype_histos_[("dm_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->phi());
            tau_jettype_histos_[("dm_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->pt());
						tau_jettype_histos_[("dm_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->eta());
						tau_jettype_histos_[("dm_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->phi());




						if(taus.at(thetaun)->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")>0.5){
							loose_taupt_match_->Fill(jets.at(jetit)->pt());
							tau_jettype_histos_[("loose_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->pt());
							tau_jettype_histos_[("loose_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->eta());
							tau_jettype_histos_[("loose_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->phi());
							tau_jettype_histos_[("loose_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->pt());
							tau_jettype_histos_[("loose_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->eta());
							tau_jettype_histos_[("loose_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->phi());
							tau_jettype_histos_[("loose_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->pt());
							tau_jettype_histos_[("loose_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->eta());
							tau_jettype_histos_[("loose_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->phi());
            	tau_jettype_histos_[("loose_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->pt());
							tau_jettype_histos_[("loose_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->eta());
							tau_jettype_histos_[("loose_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->phi());
            	tau_jettype_histos_[("loose_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->pt());
							tau_jettype_histos_[("loose_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->eta());
							tau_jettype_histos_[("loose_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_numjets_"+njets_string).c_str()]->Fill(jets.at(jetit)->phi());


							if(taus.at(thetaun)->GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits")>0.5){
								tau_jettype_histos_[("medium_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("medium_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("medium_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->phi());
								tau_jettype_histos_[("medium_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("medium_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("medium_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->phi());
								tau_jettype_histos_[("medium_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("medium_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("medium_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->phi());
								tau_jettype_histos_[("medium_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->pt());
								tau_jettype_histos_[("medium_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->eta());
								tau_jettype_histos_[("medium_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->phi());

								if(taus.at(thetaun)->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits")>0.5){
									tau_jettype_histos_[("tight_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->pt());
									tau_jettype_histos_[("tight_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->eta());
									tau_jettype_histos_[("tight_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_sumpt_"+sumpt_string).c_str()]->Fill(jets.at(jetit)->phi());
									tau_jettype_histos_[("tight_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->pt());
									tau_jettype_histos_[("tight_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->eta());
									tau_jettype_histos_[("tight_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_leadtrpt_"+leadtrpt_string).c_str()]->Fill(jets.at(jetit)->phi());
									tau_jettype_histos_[("tight_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->pt());
									tau_jettype_histos_[("tight_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->eta());
									tau_jettype_histos_[("tight_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_nvtx_"+nvtx_string).c_str()]->Fill(jets.at(jetit)->phi());
									tau_jettype_histos_[("tight_taupt_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->pt());
									tau_jettype_histos_[("tight_taueta_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->eta());
									tau_jettype_histos_[("tight_tauphi_match_"+jettypelist[jets.at(jetit)->parton_flavour()]+"_ntracks_"+ntracks_string).c_str()]->Fill(jets.at(jetit)->phi());

								}
							}
						}

					}
				} 






			}
		}
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

