#include <iostream>
#include <vector>
#include "TH1F.h"
#include "boost/program_options.hpp"
#include "boost/regex.hpp"
#include "Zbb/interface/ZbbAnalysisTools.h"
#include "Zbb/interface/ZbbPlot.h"

using namespace std;
using namespace ic;
namespace po = boost::program_options;

inline std::string param_fmt() {
  return "%-25s %-40s\n";
}

int main(int argc, char* argv[]){

	string cfg;															      // The configuration file
	string paramfile;															// The paramters files									
	string folder;																// Folder containing input files
	string var;																		// Use background methods for chosen category
  string cat;                                   // Use background methods for chosen category
  string sel;                                   // Use background methods for chosen category
  string wt;                                   // Use background methods for chosen category
	unsigned verbosity;														// Verbose output, useful for diagnostic purposes
	string datacard;             									// Channel, e.g. et
	vector<string> set_alias;											// A string like alias1:value1,alias2:value2 etc
	string syst_scale_j;
	string shift_backgrounds;
	bool auto_titles;
  std::string big_label;

	// Program options
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
  	("paramfile",               po::value<string>(&paramfile)->required())
  	("folder",                  po::value<string>(&folder)->required())
	  ("var",              		    po::value<string>(&var)->required())
    ("cat",                     po::value<string>(&cat)->default_value(""))
    ("sel",                     po::value<string>(&sel)->default_value(""))
    ("wt",                      po::value<string>(&wt)->default_value("wt"))
	  ("verbosity",               po::value<unsigned>(&verbosity)->default_value(0))
	  ("datacard",                po::value<string>(&datacard)->default_value(""))
	  ("set_alias",               po::value<vector<string>>(&set_alias)->composing())
	  ("syst_scale_j",            po::value<string>(&syst_scale_j)->default_value(""))
	  ("shift_backgrounds",       po::value<string>(&shift_backgrounds)->default_value(""))
	  ("big_label",               po::value<string>(&big_label)->default_value(""))
	  ("auto_titles",   			    po::value<bool>(&auto_titles)->default_value(true));


	ZbbPlot plot;
	config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
	po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
	po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
	po::notify(vm);

	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << "ZbbPlot" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << boost::format(param_fmt()) % "paramfile" 	% paramfile;
	std::cout << boost::format(param_fmt()) % "cfg" 				% cfg;
	std::cout << boost::format(param_fmt()) % "folder" 			% folder;
	std::cout << boost::format(param_fmt()) % "variable" 		% var;
	std::cout << boost::format(param_fmt()) % "category"    % cat;
	std::cout << boost::format(param_fmt()) % "datacard"    % datacard;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;

	// ************************************************************************
	// Extract variable name and set output filename
	// ************************************************************************
	std::string reduced_var = var;
	std::size_t begin_var = reduced_var.find("[");
	if (begin_var != reduced_var.npos) reduced_var.erase(begin_var, reduced_var.npos);
	begin_var = reduced_var.find("(");
	if (begin_var != reduced_var.npos) reduced_var.erase(begin_var, reduced_var.npos);
	if (plot.plot_name() == "") {
		plot.set_plot_name(reduced_var+"_"+datacard);
	}
	TH1::AddDirectory(false);

	// ************************************************************************
	// Load alias overrides
	// ************************************************************************
	std::vector<std::pair<std::string, std::string>> alias_vec;
	for (unsigned i = 0; i < set_alias.size(); ++i) {
		std::vector<std::string> alias_part;
		boost::split(alias_part, set_alias[i], boost::is_any_of(":"));
		if (alias_part.size() == 2) alias_vec.push_back(std::make_pair(alias_part[0],alias_part[1]));
		std::cout << "[ZbbPlot] Setting alias: " << alias_part[0] << " --> \"" << alias_part[1] << "\"" << std::endl;
	}


	// ************************************************************************
	// Setup ZbbAnalysis 
	// ************************************************************************
	ZbbAnalysis ana(verbosity);
	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	ZbbAnalysis::HistValueMap hmap;
	cat = ana.ResolveAlias(cat);
  sel = ana.ResolveAlias(sel);

	ana.FillHistoMap(hmap, var, sel, cat, wt, "");
	
  vector<pair<string,string>> systematics;

	// ************************************************************************
	// Add tau/electron energy scale systematics
	// ************************************************************************
	std::string wt_b_d = wt;
	std::string wt_b_u = wt;
	boost::replace_all(wt_b_d, "wt_1b_inc", "wt_1b_inc_d");
	boost::replace_all(wt_b_u, "wt_1b_inc", "wt_1b_inc_u");
	boost::replace_all(wt_b_d, "wt_1b_exc", "wt_1b_exc_d");
	boost::replace_all(wt_b_u, "wt_1b_exc", "wt_1b_exc_u");
	boost::replace_all(wt_b_d, "wt_2b_inc", "wt_2b_inc_d");
	boost::replace_all(wt_b_u, "wt_2b_inc", "wt_2b_inc_u");
	ana.FillHistoMap(hmap, var, sel, cat, wt_b_d, "_btagDown");
	ana.FillHistoMap(hmap, var, sel, cat, wt_b_u, "_btagUp");
    
  TH1F *btag_nom_bkg = &(hmap["Bkg"].first);
  TH1F *btag_d_bkg = &(hmap["Bkg_btagDown"].first);
  TH1F *btag_u_bkg = &(hmap["Bkg_btagUp"].first);
  for (unsigned i = 1; i <= btag_nom_bkg->GetNbinsX(); ++i) {
    double old_err = btag_nom_bkg->GetBinError(i);
    double err = std::fabs(btag_u_bkg->GetBinContent(i) - btag_d_bkg->GetBinContent(i)) / 2.0;
    double new_err = std::sqrt(old_err*old_err + err*err);
    btag_nom_bkg->SetBinError(i, new_err);
  }

	if (syst_scale_j != "") {
		systematics.push_back(make_pair("/JES_DOWN", syst_scale_j+"Down"));
		systematics.push_back(make_pair("/JES_UP", syst_scale_j+"Up"));
	}

	for (auto const& syst : systematics) {
		std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		std::cout << "[Zbb] Doing systematic templates for \"" << syst.second << "\"..." << std::endl;
		ZbbAnalysis ana_syst(verbosity);
		for (auto const& a : alias_vec) ana_syst.SetAlias(a.first, a.second);
		ana_syst.ReadTrees(folder+syst.first, folder);
		ana_syst.ParseParamFile(paramfile);
		ana_syst.FillHistoMap(hmap, var, sel, cat, wt, "_"+syst.second);
  }

  for (auto syst : {syst_scale_j}) {
    TH1F *nom_bkg = &(hmap["Bkg"].first);
    TH1F *d_bkg = &(hmap["Bkg_"+syst+"Down"].first);
    TH1F *u_bkg = &(hmap["Bkg_"+syst+"Up"].first);
    for (unsigned i = 1; i <= nom_bkg->GetNbinsX(); ++i) {
      double old_err = nom_bkg->GetBinError(i);
      double err = std::fabs(u_bkg->GetBinContent(i) - d_bkg->GetBinContent(i)) / 2.0;
      std::cout << err << std::endl;
      double new_err = std::sqrt(old_err*old_err + err*err);
      nom_bkg->SetBinError(i, new_err);
    }
  }

	// ************************************************************************
	// Write datacard
	// ************************************************************************
	if (datacard != "") {
		std::string tfile_name = "datacard_"+reduced_var+"_"+datacard+".root";
		TFile dc_file(tfile_name.c_str(),"RECREATE");
		dc_file.cd();
		gDirectory->mkdir(datacard.c_str());
		gDirectory->cd(datacard.c_str());
		for (auto iter : hmap) {
			iter.second.first.SetTitle(iter.first.c_str());
			iter.second.first.SetName(iter.first.c_str());
			iter.second.first.Write();
		}
		std::cout << "[ZbbPlot] Writing datacard input " << tfile_name << std::endl;
		dc_file.Close();
	}


	// ************************************************************************
	// Shift backgrounds
	// ************************************************************************
	vector<string> shift_strs;
	boost::split(shift_strs, shift_backgrounds, boost::is_any_of(","));
	if (shift_strs.size() > 0) {
		std::cout << "[ZbbPlot] Shifting background yields... " << std::endl;
		for (auto shift : shift_strs) {
			std::vector<std::string> shift_part;
			boost::split(shift_part, shift, boost::is_any_of(":"));
			if (shift_part.size() != 2) continue;
			string regex_str = shift_part[0]/*+".*"*/;
			boost::regex rgx = boost::regex(regex_str);
			double shift_val = boost::lexical_cast<double>(shift_part[1]);
			for (auto & entry : hmap) {
				if (boost::regex_match(entry.first, rgx)) {
					std::cout << "Scaling " << entry.first << " by " << shift_val << std::endl;
					entry.second.first.Scale(shift_val);
					entry.second.second.first *= shift_val; // the yield
					entry.second.second.second *= shift_val; // the yield error
				}
			}
		}
	}
	

	// ************************************************************************
	// Deduce titles
	// ************************************************************************
	if (auto_titles) {
		double fb_lumi = ana.GetLumi() / 1000.;
		plot.set_title_left((boost::format("L = %.1f fb^{-1} at 7 TeV") % fb_lumi).str());
    ic::TextElement text(big_label,0.08,0.21,0.82);
    plot.AddTextElement(text);
	}
	plot.GeneratePlot(hmap);

  return 0;
}

