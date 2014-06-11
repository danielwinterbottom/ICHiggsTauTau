#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "TSystem.h"
#include "TMath.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CopyCollection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CompositeProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OneCollCompositeProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/PileupWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPairSelector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/QuarkGluonDiscriminatorStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTRecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTSync.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPrint.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EnergyShifter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SVFit.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/SVFitTest.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/JetEnergyCorrections.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/JetEnergyUncertainty.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEnergyScale.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauDzFixer.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEMuExtras.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEMuMVA.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTL1MetCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTL1MetCut.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauEfficiency.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/EmbeddingKineReweightProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/BTagCheck.h"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"

using namespace ic;

int main(int argc, char * argv[]){
	gSystem->Load("libFWCoreFWLite.dylib");
	gSystem->Load("libUserCodeICHiggsTauTau.dylib");
	AutoLibraryLoader::enable();

	if(argc<2){
		std::cout<<"NEED TO PROVIDE OUTPUT FILENAME"<<std::endl;
		std::exit(1);
	}

	std::string inputfile;
	inputfile="../HiggsTauTau/";
	inputfile+=argv[1];
	std::string outputfile;
	outputfile = "SELECTION_Out_";
	outputfile += argv[1];



	TFile *file1 = TFile::Open(inputfile.c_str());
	std::cout<<"File opened"<<std::endl;
	TTree *tree = dynamic_cast<TTree*>(file1->Get("EventTree"));
	std::vector<ic::Tau> *tau_vector = new std::vector<ic::Tau>();
	std::vector<ic::PFJet> * jet_vector = new std::vector<ic::PFJet>();
	std::vector<ic::Tau> *tau_vector2 = new std::vector<ic::Tau>();
	std::vector<ic::PFJet> *jet_vector2 = new std::vector<ic::PFJet>();
	ic::EventInfo * evt = new ic::EventInfo();
	std::vector<Double_t> *jet_pt_vec = new std::vector<Double_t>();
	std::vector<Double_t> *tau_pt_vec = new std::vector<Double_t>();
	std::vector<Double_t> *jet_eta_vec = new std::vector<Double_t>();
	std::vector<Double_t> *tau_eta_vec = new std::vector<Double_t>();
	std::vector<Double_t> *jet_phi_vec = new std::vector<Double_t>();
	std::vector<Double_t> *tau_phi_vec = new std::vector<Double_t>();
	std::vector<Double_t> *tau_id_loose = new std::vector<Double_t>();
	std::vector<Double_t> *tau_id_med = new std::vector<Double_t>();
	std::vector<Double_t> *tau_id_tight = new std::vector<Double_t>();
	std::vector<Double_t> *tau_dm_find = new std::vector<Double_t>();
	std::vector<Int_t> *jet_flavour_vec = new std::vector<Int_t>();
	Int_t njets=0;
	Int_t ntaus=0;
	Int_t jvecsize=0;
	Int_t tvecsize=0;
	Int_t nvtx=0;

	TBranch *tau_branch=tree->GetBranch("taus");
	TBranch *jet_branch=tree->GetBranch("pfJetsPFlow");
	tau_branch->SetAddress(&tau_vector);
	jet_branch->SetAddress(&jet_vector);
	TBranch* nvtx_branch=tree->GetBranch("eventInfo");
	nvtx_branch->SetAddress(&evt);
	Int_t nEntries = tree->GetEntries();

	TFile *foutput = new TFile(outputfile.c_str(),"RECREATE");
	TTree *outtree = new TTree("EventTree2","EventTree2");
	outtree->Branch("njets",&njets,"njets/I");
	outtree->Branch("ntaus",&ntaus,"ntaus/I");
	outtree->Branch("nvtx",&nvtx,"nvtx/I");
	outtree->Branch("tau_sel_pt",&tau_pt_vec);
	outtree->Branch("tau_sel_eta",&tau_eta_vec);
	outtree->Branch("tau_sel_phi",&tau_phi_vec);
	outtree->Branch("jet_sel_pt",&jet_pt_vec);
	outtree->Branch("jet_sel_eta",&jet_eta_vec);
	outtree->Branch("jet_sel_phi",&jet_phi_vec);
	outtree->Branch("jet_sel_flavour",&jet_flavour_vec);
	outtree->Branch("tau_id_loose",&tau_id_loose);
	outtree->Branch("tau_id_med",&tau_id_med);
	outtree->Branch("tau_id_tight",&tau_id_tight);
	outtree->Branch("tau_dm_find",&tau_dm_find);




	for(Int_t i=0;i<nEntries;i++){
		if(i%10000==0) std::cout<<"PROCESSING EVENT "<<i<<std::endl;
		tree->GetEntry(i);
		jvecsize=jet_vector->size();
		nvtx=evt->good_vertices();
		if(jvecsize!=0){
			for(Int_t jetit=0;jetit<jvecsize;jetit++){
				if(jet_vector->at(jetit).pt()>20&&TMath::Abs(jet_vector->at(jetit).eta())<2.3){
					jet_pt_vec->push_back(jet_vector->at(jetit).pt());
					jet_eta_vec->push_back(jet_vector->at(jetit).eta());
					jet_phi_vec->push_back(jet_vector->at(jetit).phi());
					jet_flavour_vec->push_back(jet_vector->at(jetit).parton_flavour());
				}
			}
		}

		if(jvecsize==0) njets=0;
		else njets = jet_pt_vec->size();

		tvecsize = tau_vector->size();
		if(tvecsize!=0){
			for(Int_t tauit=0;tauit<tvecsize;tauit++){
				if(tau_vector->at(tauit).pt()>20&&TMath::Abs(tau_vector->at(tauit).eta())<2.3){
					tau_pt_vec->push_back(tau_vector->at(tauit).pt());
					tau_eta_vec->push_back(tau_vector->at(tauit).eta());
					tau_phi_vec->push_back(tau_vector->at(tauit).phi());
					tau_id_loose->push_back(tau_vector->at(tauit).GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits"));
					tau_id_med->push_back(tau_vector->at(tauit).GetTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"));
					tau_id_tight->push_back(tau_vector->at(tauit).GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits"));
					tau_dm_find->push_back(tau_vector->at(tauit).GetTauID("decayModeFinding"));
				}
			}
		}
		if(tvecsize==0) ntaus=0;
		else ntaus = tau_pt_vec->size();

		outtree->Fill();
		jet_vector2->clear();
		tau_vector2->clear();
		jet_pt_vec->clear();
		jet_eta_vec->clear();
		jet_phi_vec->clear();
		jet_flavour_vec->clear();
		tau_pt_vec->clear();
		tau_eta_vec->clear();
		tau_phi_vec->clear();
		tau_id_loose->clear();
		tau_id_med->clear();
		tau_id_tight->clear();
		tau_dm_find->clear();


	}
	foutput->Close();
	delete tau_vector;
	delete jet_vector;
	delete tau_vector2;
	delete jet_vector2;
	delete jet_pt_vec;
	delete jet_eta_vec;
	delete jet_phi_vec;
	delete jet_flavour_vec;
	delete tau_pt_vec;
	delete tau_eta_vec;
	delete tau_phi_vec;
	delete tau_id_loose;
	delete tau_id_med;
	delete tau_id_tight;
	delete tau_dm_find;



}
