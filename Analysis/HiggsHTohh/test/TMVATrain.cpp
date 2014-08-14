#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "TSystem.h"
#include "TMVA/Factory.h"
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
#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/JetTauFakeRate.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/HhhAnalysisTools.h"

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
	//	std::cout << "[HhhAnalysis::ParseParamFile] Extracting sample info from file " << file << std::endl;
	//	lumi_ = parser.GetParam<double>("LUMI_DATA_"+Channel2String(ch_));
	//	std::cout << "[HhhAnalysis::ParseParamFile] Integrated luminosity set to " << lumi_ << " /pb" << std::endl;
	//	if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % "Sample" % "Events" % "Cross Section" % "Sample Lumi" % "Rel. Lumi";
	//	if (verbosity_ > 1) std::cout << "-----------------------------------------------------------------------------------------\n";
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
			//		if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % sample % unsigned(evt+0.5) % xs % (evt/xs) % (evt/(xs*lumi_));
		}
	}
}

int main(int argc, char* argv[]){

	// Configurable parameters
	// int max_events;                 // Maximum number of events to process
	//string filelist;                // The file containing a list of files to use as input
	//string input_prefix;            // A prefix that will be added to the path of each input file
	string folder;
	string output_name;             // Name of the ouput ROOT File
	string output_folder;           // Folder to write the output in
	string paramfile;
	string paramfile2;
	string classname;

	po::options_description config("Configuration");
	po::variables_map vm;
	po::notify(vm);

	config.add_options()    
		("folder",              po::value<string>(&folder)->default_value("output/EMu/"))
		//   ("input_prefix",        po::value<string>(&input_prefix)->default_value(""))
		("output_name",         po::value<string>(&output_name)->default_value("test_tmva.root"))
		("output_folder",       po::value<string>(&output_folder)->default_value(""))
		("paramfile",						po::value<string>(&paramfile)->default_value("./scripts/Paper_params_2012.dat"))
		("paramfile2", 					po::value<string>(&paramfile2)->default_value("./scripts/TMVAinputs.dat"))
		("classname",						po::value<string>(&classname)->default_value("HhhMVA"))
		;
	po::store(po::command_line_parser(argc, argv).
			options(config).allow_unregistered().run(), vm);
	po::notify(vm);


	std::cout << "-------------------------------------" << std::endl;
	std::cout << "Train MVA" << std::endl;
	std::cout << "-------------------------------------" << std::endl;      string param_fmt = "%-25s %-40s\n";
	std::vector<string> bckglist;
	bckglist.push_back("TTJetsFullLept");
	bckglist.push_back("TTJetsSemiLept");
	bckglist.push_back("TTJetsHadronicExt");
//	bckglist.push_back("WWJetsTo2L2Nu");
//	bckglist.push_back("WZJetsTo2L2Q");
//	bckglist.push_back("WZJetsTo3LNu");
//	bckglist.push_back("ZZJetsTo2L2Nu");
//	bckglist.push_back("ZZJetsTo2L2Q");
//	bckglist.push_back("ZZJetsTo4L");
//	bckglist.push_back("DYJetsToTauTauSoup");
//	bckglist.push_back("DYJetsToLLSoup");
//	bckglist.push_back("DYJetsToTauTau");
//	bckglist.push_back("DYJetsToLL");
//	bckglist.push_back("T-tW");
//	bckglist.push_back("Tbar-tW");

	std::vector<string> signallist;
	signallist.push_back("GluGluToHTohhTo2Tau2B_mH-300");

	sample_names_.reserve(bckglist.size()+signallist.size());
	sample_names_.insert(sample_names_.end(),bckglist.begin(),bckglist.end());
	sample_names_.insert(sample_names_.end(),signallist.begin(),signallist.end());



	std::vector<TFile*> BackgroundSamples;
	for(unsigned int iter=0;iter<bckglist.size();++iter){
		BackgroundSamples.push_back(TFile::Open((folder+bckglist.at(iter)+"_em_2012.root").c_str()));
	}

	std::vector<TFile*> SignalSamples;
	for(unsigned int sigIter=0;sigIter<signallist.size();++sigIter){
		SignalSamples.push_back(TFile::Open((folder+signallist.at(sigIter)+"_em_2012.root").c_str()));
	}

	std::vector<TTree*> backgroundTrees;
	for(unsigned int iter2=0;iter2<BackgroundSamples.size();++iter2){
		backgroundTrees.push_back(dynamic_cast<TTree*>(BackgroundSamples.at(iter2)->Get("ntuple")));
	}

	std::vector<TTree*> signalTrees;
	for(unsigned int sigIter2=0;sigIter2<SignalSamples.size();++sigIter2){
		signalTrees.push_back(dynamic_cast<TTree*>(SignalSamples.at(sigIter2)->Get("ntuple")));
	}

	TFile *outfile = new TFile((output_folder+output_name).c_str(),"RECREATE");

	TMVA::Factory *factory = new TMVA::Factory(classname,outfile,"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");


	std::vector<std::string> vars;
	ifstream parafile(paramfile2.c_str());
	std::cout<<paramfile2.c_str()<<std::endl;
	string line;
	while(getline(parafile,line)){
		vars.push_back(line);
	}
	parafile.close();

	std::cout<<(vars.at(0)).c_str()<<std::endl;

	std::vector<float> var2;
	for(unsigned int variter=0;variter<vars.size();++variter){
		var2.push_back(::atof((vars.at(variter)).c_str()));
	}


	for(unsigned int variter=0;variter<vars.size();++variter){
		factory->AddVariable((vars.at(variter)).c_str(),(vars.at(variter)).c_str(),"",'F');
	}

	factory->AddSpectator("n_prebjets","n_prebjets","",'I');

	double weightval_=0;

 ParseParamFile(paramfile);	

	for(unsigned int bckgit=0;bckgit<backgroundTrees.size();++bckgit){
		auto it = sample_info_.find(bckglist.at(bckgit).c_str());
		if(it!=sample_info_.end()){
			double evt = it->second.first;
			double xs = it->second.second;
			weightval_=(double) xs/evt;
			std::cout<<weightval_<<std::endl;
		}
		factory->AddBackgroundTree(backgroundTrees.at(bckgit),weightval_);
	}
	for(unsigned int sgit=0;sgit<signalTrees.size();++sgit){
		auto it = sample_info_.find(signallist.at(sgit).c_str());
		if(it!=sample_info_.end()){
			double evt = it->second.first;
			double xs=it->second.second;
			weightval_=(Double_t) xs/evt;
		}
		std::cout<<weightval_<<std::endl;
		factory->AddSignalTree(signalTrees.at(sgit),weightval_);
	}
	factory->SetBackgroundWeightExpression("wt");
	factory->SetSignalWeightExpression("wt");
	TCut mycutb="n_prebjets>2";
	TCut mycuts="n_prebjets>2";
//TCut mycutb="";
//TCut mycuts="";
	factory->PrepareTrainingAndTestTree( mycuts, mycutb,"SplitMode=Random:!V");
	factory->BookMethod( TMVA::Types::kBDT, "BDTG","!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5" );

	factory->BookMethod( TMVA::Types::kBDT, "BDT","!H:!V:NTrees=850:nEventsMin=150:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

	factory->TrainAllMethods();
	factory->TestAllMethods();
	factory->EvaluateAllMethods();

	outfile->Close();
	delete factory;

	return 0;
}

