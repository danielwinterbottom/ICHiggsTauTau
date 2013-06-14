#include <iostream>
#include <vector>
#include "TH1F.h"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPlotTools.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

using namespace std;
using namespace ic;
namespace po = boost::program_options;

int main(int argc, char* argv[]){

	string cfg;															      // The configuration file
	string paramfile;															// The paramters files									
	string folder;																// Folder containing input files
	string channel_str;             							// Channel, e.g. et
	unsigned method;															// Use background methods for chosen category
	string var;																		// Use background methods for chosen category
	string cat;																		// Use background methods for chosen category
	bool is_2012;																	// false = 7 TeV, true = 8 TeV
	unsigned verbosity;														// Verbose output, useful for diagnostic purposes
	bool is_paper;                                // Tweaking some things for the paper
	string datacard;             									// Channel, e.g. et
	string set_alias;															// A string like alias1:value1,alias2:value2 etc
	string sm_masses_str;															// A string like alias1:value1,alias2:value2 etc
	string mssm_masses_str;														// A string like alias1:value1,alias2:value2 etc

	// Program options
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
  	("paramfile",           po::value<string>(&paramfile)->required())
  	("folder",              po::value<string>(&folder)->required())
  	("channel",             po::value<string>(&channel_str)->required())
  	("method",           		po::value<unsigned>(&method)->required())
	  ("var",              		po::value<string>(&var)->required())
	  ("cat",             		po::value<string>(&cat)->default_value(""))
	  ("is_2012",             po::value<bool>(&is_2012)->required())
	  ("verbosity",           po::value<unsigned>(&verbosity)->default_value(0))
	  ("is_paper",            po::value<bool>(&is_paper)->default_value(false))
	  ("datacard",            po::value<string>(&datacard)->default_value(""))
	  ("set_alias",           po::value<string>(&set_alias)->default_value(""))
	  ("sm_masses",           po::value<string>(&sm_masses_str)->default_value(""))
	  ("mssm_masses",         po::value<string>(&mssm_masses_str)->default_value(""));
	HTTPlot plot;
	config.add(plot.GenerateOptions(""));
	po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
	po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
	po::notify(vm);


	TH1::AddDirectory(false);

	HTTAnalysis ana(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
	
	// Check if any aliases need to be re-defined
	if (set_alias != "") {
		std::vector<std::string> alias_list;
		boost::split(alias_list, set_alias, boost::is_any_of(","));
		for (unsigned i = 0; i < alias_list.size(); ++i) {
			std::vector<std::string> alias_part;
			boost::split(alias_part, alias_list[i], boost::is_any_of(":"));
			if (alias_part.size() == 2) {
				ana.SetAlias(alias_part[0], alias_part[1]);
			}
		}
	}
	std::vector<std::string> sm_masses;
	if (sm_masses_str != "") {
		boost::split(sm_masses, sm_masses_str, boost::is_any_of(","));
		ana.AddSMSignalSamples(sm_masses);
	}
	std::vector<std::string> mssm_masses;
	if (mssm_masses_str != "") {
		boost::split(mssm_masses, mssm_masses_str, boost::is_any_of(","));
		ana.AddMSSMSignalSamples(mssm_masses);
	}

	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTAnalysis::HistValueMap hmap;

	std::string sel = ana.ResolveAlias("os_sel");
	cat = ana.ResolveAlias(cat);

	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "", 1.0);
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "", 1.0);

	// HTTAnalysis ana_tscale_down(channel::mt, "2012", 0);
	// ana_tscale_down.ReadTrees("output/TauID_2012/TSCALE_DOWN", "output/TauID_2012");
	// ana_tscale_down.ParseParamFile("scripts/Paper_params_2012.dat");
	// ana_tscale_down.FillHistoMap(hmap, 8, var, sel, cat, "wt", "_CMS_scale_t_mutau_8TeVDown");

	// HTTAnalysis ana_tscale_up(channel::mt, "2012", 0);
	// ana_tscale_up.ReadTrees("output/TauID_2012/TSCALE_UP", "output/TauID_2012");
	// ana_tscale_up.ParseParamFile("scripts/Paper_params_2012.dat");
	// ana_tscale_up.FillHistoMap(hmap, 8, var, sel, cat, "wt", "_CMS_scale_t_mutau_8TeVUp");

	TFile dc_file(("datacard_"+datacard+".root").c_str(),"RECREATE");
	dc_file.cd();
	gDirectory->mkdir(datacard.c_str());
	gDirectory->cd(datacard.c_str());

	for (auto iter : hmap) {
		iter.second.first.SetTitle(iter.first.c_str());
		iter.second.first.SetName(iter.first.c_str());
		iter.second.first.Write();
	}
	dc_file.Close();
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "");
	plot.GeneratePlot(hmap);



  return 0;
}

