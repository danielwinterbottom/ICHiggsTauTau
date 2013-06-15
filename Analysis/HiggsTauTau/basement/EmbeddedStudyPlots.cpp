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
	string var, cat, datacard;

	// Program options
	po::options_description config("Configuration");
	po::variables_map vm;
	po::notify(vm);
	config.add_options()    
	  ("var",              		po::value<string>(&var)->required())
	  ("cat",             		po::value<string>(&cat)->default_value(""))
	  ("datacard",            po::value<string>(&datacard)->required())
	  ;
	  po::store(po::command_line_parser(argc, argv).
	            options(config).allow_unregistered().run(), vm);
	  po::notify(vm);


	HTTPlot plot;
	po::store(po::command_line_parser(argc, argv).
	          options(plot.GenerateOptions("")).allow_unregistered().run(), vm);
	po::notify(vm);

	TH1::AddDirectory(false);

	HTTAnalysis ana(channel::mt, "2012", 0);
	ana.AddSMSignalSamples({"125"});
	ana.ReadTrees("output/TauID_2012");
	ana.ParseParamFile("scripts/Paper_params_2012.dat");

	string sel = "os && mt_1<20.";

	HTTAnalysis::HistValueMap hmap;
	ana.FillHistoMap(hmap, 8, var, sel, cat, "wt", "");
	ana.FillSMSignal(hmap, {"125"}, var, sel, cat, "wt", "", "");

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

	plot.GeneratePlot(hmap);
	for (auto iter : hmap) {
		iter.second.first.SetTitle(iter.first.c_str());
		iter.second.first.SetName(iter.first.c_str());
		iter.second.first.Write();
	}
	dc_file.Close();



  return 0;
}

