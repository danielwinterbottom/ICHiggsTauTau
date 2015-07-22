#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "TSystem.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CopyCollection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/JetTauFakeRate/interface/JetTauFakeRate.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"

using boost::lexical_cast;
using boost::bind;
namespace po = boost::program_options;
using std::string;
using std::vector;
using namespace ic;
std::vector<string> sample_names_;

std::map<std::string, std::pair<double, double>> sample_info_;
void ParseParamFile(std::string const& file) {
	SimpleParamParser parser;
	parser.ParseFile(file);
	for (auto sample : sample_names_) {
		std::string lookup = sample;
		if (sample.find("Special") != sample.npos) {
			size_t found = sample.find('_');
			found = sample.find('_',found+1);
			lookup = sample.substr(found+1);
		}
		if (parser.ParamExists("XS_"+sample) && parser.ParamExists("EVT_"+sample)) {
			double evt = parser.GetParam<double>("EVT_"+sample);
			double xs = parser.GetParam<double>("XS_"+sample);
			if (xs <= 0) continue;
			sample_info_[sample] = std::make_pair(evt, xs);
		}
	}
}

int main(int argc, char* argv[]){

	// Configurable parameters
	string folder;
	string output_name;             // Name of the ouput ROOT File
	string output_folder;           // Folder to write the output in
	string paramfile;
	string input_name;
	string paramfile2;

	po::options_description config("Configuration");
	po::variables_map vm;
	po::notify(vm);

	config.add_options()    
		("folder",              po::value<string>(&folder)->default_value("output/EMu/"))
		("input_name", po::value<string>(&input_name)->default_value("TTJetsFullLept_em_2012.root"))
		("output_name",         po::value<string>(&output_name)->default_value("run_tmva.root"))
		("output_folder",       po::value<string>(&output_folder)->default_value(""))
		("paramfile",						po::value<string>(&paramfile)->default_value("./scripts/Paper_params_2012.dat"))
		("paramfile2",						po::value<string>(&paramfile2)->default_value("./scripts/TMVAinputs.dat"))
		;
	po::store(po::command_line_parser(argc, argv).
			options(config).allow_unregistered().run(), vm);
	po::notify(vm);


	std::cout << "-------------------------------------" << std::endl;
	std::cout << "TMVARun" << std::endl;
	std::cout << "-------------------------------------" << std::endl;      string param_fmt = "%-25s %-40s\n";

	TMVA::Reader *reader = new TMVA::Reader("!Color:!Silent");
	std::vector<std::string> vars;
	std::ifstream parafile(paramfile2.c_str());
	std::cout<<paramfile2.c_str()<<std::endl;
	string line;
	while(getline(parafile,line)){
		vars.push_back(line);
	}
	parafile.close();


	std::vector<float> floatvars;
	for(unsigned int varit=0;varit<vars.size();++varit){
		floatvars.push_back(::atof((vars.at(varit)).c_str()));
	}

	for(unsigned int varit=0;varit<floatvars.size();++varit){
		reader->AddVariable((vars.at(varit)).c_str(),&(floatvars.at(varit)));
	}

	Int_t n_prebjets;
	reader->AddSpectator("n_prebjets",&n_prebjets);
	TFile *target = new TFile(output_name.c_str(),"RECREATE");


	TString dir    = "weights/";
	TString prefix = "HTohhMVA";

	// Book method(s)
	reader->BookMVA("BDT method",dir+prefix+TString("_")+TString("BDT.weights.xml"));
	reader->BookMVA("BDTG method",dir+prefix+TString("_")+TString("BDTG.weights.xml"));

	TH1F *histBDT(0), *histBDTG(0);
	histBDT = new TH1F("MVA_BDT","MVA_BDT",100,-1,1);
	histBDTG = new TH1F("MVA_BDTG","MVA_BDTG",100,-1,1);

	Float_t bdt=0;
	Float_t bdtg=0;
	TTree *writetree = new TTree("results","results");
	writetree->Branch("bdt",&bdt,64000,0);
	writetree->Branch("bdtg",&bdtg,64000,0);


	TFile *input = TFile::Open((folder+input_name).c_str());
	TTree *theTree = (TTree*)input->Get("ntuple");
	for(unsigned int varit=0;varit<floatvars.size();++varit){
		theTree->SetBranchAddress((vars.at(varit)).c_str(),&(floatvars.at(varit)));
	}


	std::vector<float> vecVar(floatvars.size()); // vector for EvaluateMVA tests

	std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
	Int_t maxEvt = theTree->GetEntries();
	for (Long64_t ievt=0; ievt<maxEvt;ievt++) {

		if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

		theTree->GetEntry(ievt);

		bdt = reader->EvaluateMVA("BDT method");
		bdtg = reader->EvaluateMVA("BDTG method");
		histBDT->Fill(bdt);
		histBDTG->Fill(bdtg);
		writetree->Fill();
	} 
	target->cd();
	writetree->Write();
	histBDT->Write();
	histBDTG->Write();
	target->Close();
	delete reader;

	return 0;
}

