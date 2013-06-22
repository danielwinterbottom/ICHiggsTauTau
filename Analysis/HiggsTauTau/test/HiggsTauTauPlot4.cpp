#include <iostream>
#include <vector>
#include "TH1F.h"
#include "boost/program_options.hpp"
#include "boost/regex.hpp"
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
	vector<string> set_alias;											// A string like alias1:value1,alias2:value2 etc
	string sm_masses_str;													
	string mssm_masses_str;												
	string syst_tau_scale;
	string syst_eff_b;
	string syst_fake_b;
	string syst_qcd_shape;
	string syst_fakes_shape;
	string syst_ggh_pt;
	string add_sm_background;
	double sub_ztt_top_frac;
	string fix_empty_bins;
	string fix_negative_bins;
	string fix_empty_hists;
	string add_extra_binning;
	bool auto_titles;
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
	  ("set_alias",           po::value<vector<string>>(&set_alias)->composing())
	  ("sm_masses",           po::value<string>(&sm_masses_str)->default_value(""))
	  ("mssm_masses",         po::value<string>(&mssm_masses_str)->default_value(""))
	  ("syst_tau_scale",      po::value<string>(&syst_tau_scale)->default_value(""))
	  ("syst_eff_b",      		po::value<string>(&syst_eff_b)->default_value(""))
	  ("syst_fake_b",      		po::value<string>(&syst_fake_b)->default_value(""))
	  ("syst_qcd_shape",      po::value<string>(&syst_qcd_shape)->default_value(""))
	  ("syst_fakes_shape",    po::value<string>(&syst_fakes_shape)->default_value(""))
	  ("syst_ggh_pt",    			po::value<string>(&syst_ggh_pt)->default_value(""))
	  ("add_sm_background",   po::value<string>(&add_sm_background)->default_value(""))
	  ("sub_ztt_top_frac",    po::value<double>(&sub_ztt_top_frac)->default_value(-1.0))
	  ("fix_empty_bins",   		po::value<string>(&fix_empty_bins)->default_value(""))
	  ("fix_empty_hists",   	po::value<string>(&fix_empty_hists)->default_value(""))
	  ("fix_negative_bins",  	po::value<string>(&fix_negative_bins)->default_value(""))
	  ("add_extra_binning",   po::value<string>(&add_extra_binning)->default_value(""))
	  ("auto_titles",   			po::value<bool>(&auto_titles)->default_value(true));


	HTTPlot plot;
	config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
	po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
	po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
	po::notify(vm);

	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << "HiggsTauTauPlot4" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << boost::format(param_fmt()) % "paramfile" 	% paramfile;
	std::cout << boost::format(param_fmt()) % "cfg" 				% cfg;
	std::cout << boost::format(param_fmt()) % "folder" 			% folder;
	std::cout << boost::format(param_fmt()) % "channel"     % channel_str;
	std::cout << boost::format(param_fmt()) % "variable" 		% var;
	std::cout << boost::format(param_fmt()) % "method" 		  % method;
	std::cout << boost::format(param_fmt()) % "category"    % cat;
	std::cout << boost::format(param_fmt()) % "datacard"    % datacard;
	std::cout << boost::format(param_fmt()) % "is_2012" 		% is_2012;
	std::cout << boost::format(param_fmt()) % "sm_masses" 	% sm_masses_str;
	std::cout << boost::format(param_fmt()) % "mssm_masses" % mssm_masses_str;
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
		plot.set_plot_name(reduced_var+"_"+datacard+"_"+channel_str+(is_2012 ? "_2012":"_2011"));
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
		std::cout << "[HiggsTauTauPlot4] Setting alias: " << alias_part[0] << " --> \"" << alias_part[1] << "\"" << std::endl;
	}

	// ************************************************************************
	// Extract signal masses to load
	// ************************************************************************
	std::vector<std::string> sm_masses;
	if (sm_masses_str != "") boost::split(sm_masses, sm_masses_str, boost::is_any_of(","));
	std::vector<std::string> mssm_masses;
	if (mssm_masses_str != "") boost::split(mssm_masses, mssm_masses_str, boost::is_any_of(","));

	// ************************************************************************
	// Setup HTTAnalysis 
	// ************************************************************************
	HTTAnalysis ana(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
	for (auto const& a : alias_vec) ana.SetAlias(a.first, a.second);
	ana.AddSMSignalSamples(sm_masses);
	if (add_sm_background != "") ana.AddSMSignalSamples({add_sm_background});
	ana.AddMSSMSignalSamples(mssm_masses);
	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTAnalysis::HistValueMap hmap;

	std::string sel = "os && "+ana.ResolveAlias("sel");
	cat = ana.ResolveAlias(cat);

	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "", 1.0);
	if (add_sm_background != "") ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "", 1.0);

	// ************************************************************************
	// ggH pT Reweighting
	// ************************************************************************
	if (syst_ggh_pt != "") {
		std::cout << "[HiggsTauTauPlot4] Adding ggH pT systematic..." << std::endl;
		for (auto m : sm_masses) {
			hmap["ggH"+m+"_"+syst_ggh_pt+"Up"] = ana.GenerateSignal("GluGluToHToTauTau_M-"+m, var, sel, cat, "wt*wt_ggh_pt_up", 1.0);
			hmap["ggH"+m+"_"+syst_ggh_pt+"Down"] = ana.GenerateSignal("GluGluToHToTauTau_M-"+m, var, sel, cat, "wt*wt_ggh_pt_down", 1.0);
		}
	}

	// ************************************************************************
	// Additional Binning
	// ************************************************************************
	if (add_extra_binning != "") {

		std::vector<std::string> extra_binning;
		boost::split(extra_binning, add_extra_binning, boost::is_any_of(":"));
		if (extra_binning.size() == 2) {
			std::cout << "-----------------------------------------------------------------------------------" << std::endl;
			std::cout << "[HiggsTauTauPlot4] Adding alternative binning \"" << extra_binning[0] 
				<< "\" with postfix \"" << extra_binning[1] << "\"" << std::endl;
			ana.FillHistoMap(hmap, method, reduced_var+extra_binning[0], sel, cat, "wt", extra_binning[1]);
		}
	}


	vector<pair<string,string>> systematics;

	// ************************************************************************
	// Add tau/electron energy scale systematics
	// ************************************************************************
	if (syst_tau_scale != "") {
		systematics.push_back(make_pair("/TSCALE_DOWN", syst_tau_scale+"Down"));
		systematics.push_back(make_pair("/TSCALE_UP", syst_tau_scale+"Up"));
		// std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		// std::cout << "[HiggsTauTauPlot4] Doing energy scale systematics..." << std::endl;
		// HTTAnalysis ana_tscale_down(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
		// for (auto const& a : alias_vec) ana_tscale_down.SetAlias(a.first, a.second);
		// ana_tscale_down.AddSMSignalSamples(sm_masses);
		// if (add_sm_background != "") ana_tscale_down.AddSMSignalSamples({add_sm_background});
		// ana_tscale_down.AddMSSMSignalSamples(mssm_masses);
		// ana_tscale_down.ReadTrees(folder+"/TSCALE_DOWN", folder);
		// ana_tscale_down.ParseParamFile(paramfile);
		// ana_tscale_down.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst_tau_scale+"Down");
		// ana_tscale_down.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Down", 1.0);
		// if (add_sm_background != "") ana_tscale_down.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst_tau_scale+"Down");
		// ana_tscale_down.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Down", 1.0);

		// HTTAnalysis ana_tscale_up(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
		// for (auto const& a : alias_vec) ana_tscale_up.SetAlias(a.first, a.second);
		// ana_tscale_up.AddSMSignalSamples(sm_masses);
		// if (add_sm_background != "") ana_tscale_up.AddSMSignalSamples({add_sm_background});
		// ana_tscale_up.AddMSSMSignalSamples(mssm_masses);
		// ana_tscale_up.ReadTrees(folder+"/TSCALE_UP", folder);
		// ana_tscale_up.ParseParamFile(paramfile);
		// ana_tscale_up.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst_tau_scale+"Up");
		// ana_tscale_up.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Up", 1.0);
		// if (add_sm_background != "") ana_tscale_up.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst_tau_scale+"Up");
		// ana_tscale_up.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Up", 1.0);
		// std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	}

	if (syst_eff_b != "") {
		systematics.push_back(make_pair("/BTAG_DOWN", syst_eff_b+"Down"));
		systematics.push_back(make_pair("/BTAG_UP", syst_eff_b+"Up"));
	}
	if (syst_fake_b != "") {
		systematics.push_back(make_pair("/BFAKE_DOWN", syst_fake_b+"Down"));
		systematics.push_back(make_pair("/BFAKE_UP", syst_fake_b+"Up"));
	}

	for (auto const& syst : systematics) {
		std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		std::cout << "[HiggsTauTauPlot4] Doing systematic templates for \"" << syst.second << "\"..." << std::endl;
		HTTAnalysis ana_syst(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
		for (auto const& a : alias_vec) ana_syst.SetAlias(a.first, a.second);
		ana_syst.AddSMSignalSamples(sm_masses);
		if (add_sm_background != "") ana_syst.AddSMSignalSamples({add_sm_background});
		ana_syst.AddMSSMSignalSamples(mssm_masses);
		ana_syst.ReadTrees(folder+syst.first, folder);
		ana_syst.ParseParamFile(paramfile);
		ana_syst.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst.second);
		ana_syst.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst.second, 1.0);
		if (add_sm_background != "") ana_syst.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst.second);
		ana_syst.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst.second, 1.0);
	}

	// ************************************************************************
	// Reduce top yield to account for contamination in embedded
	// ************************************************************************
	if (sub_ztt_top_frac > 0.) {
		std::string top_label = (channel_str == "em") ? "ttbar" : "TT";
		std::string ztt_label = (channel_str == "em") ? "Ztt" : "ZTT";
		std::cout << "[HiggsTauTauPlot4] Subtracting " << top_label 
			<< " contamination in " << ztt_label << ": " << sub_ztt_top_frac << std::endl;
		HTTAnalysis::Value ztt_rate = hmap[ztt_label].second;
		HTTAnalysis::Value & top_rate = hmap[top_label].second;
		HTTAnalysis::Value contamination = HTTAnalysis::ValueProduct(ztt_rate, std::make_pair(sub_ztt_top_frac,0.));
		HTTAnalysis::PrintValue("Contamination", contamination);
		top_rate = HTTAnalysis::ValueSubtract(top_rate, contamination);
		SetNorm(&(hmap[top_label].first), top_rate.first);
		HTTAnalysis::PrintValue("New "+top_label+ " rate", top_rate);
	}

	// ************************************************************************
	// Generate Fake-rate vs same-sign systematic histograms
	// ************************************************************************
	if (syst_fakes_shape != "") {
		std::cout << "[HiggsTauTauPlot4] Adding fake shape systematic..." << std::endl;
		HTTAnalysis::Value fakes_rate = hmap["Fakes"].second;
		TH1F fr_hist = ana.GetShapeViaFakesMethod(var, sel, cat, "wt");
		SetNorm(&fr_hist, fakes_rate.first);
		TH1F qcd_hist = ana.GetShape(var, "Special_24_Data", "!os && "+ana.ResolveAlias("sel"), cat, "wt");
		SetNorm(&qcd_hist, fakes_rate.first);
		hmap["Fakes_"+syst_fakes_shape+"Up"]  = std::make_pair(qcd_hist, fakes_rate);
		hmap["Fakes_"+syst_fakes_shape+"Down"] = std::make_pair(fr_hist, fakes_rate);
	}

	// ************************************************************************
	// Generate low-mass shifted QCD histograms & systematics
	// ************************************************************************
	if (syst_qcd_shape != "") {
		std::cout << "[HiggsTauTauPlot4] Adding QCD low-mass shape systematic..." << std::endl;
	  TH1F h1 = hmap["QCD"].first;
	  TH1F h2 = hmap["QCD"].first;
	  TH1F h3 = hmap["QCD"].first;
	  float x, y, c, cUp, cDown;
	  for(int i=1;i<h1.GetNbinsX();++i){
	    x = h1.GetXaxis()->GetBinCenter(i);
	    if(x < (is_2012 ? 70 : 50) ){
	      y = h1.GetBinContent(i);
	      c = 1.15; 
	      cUp = 1.3;
	      cDown = 1.0;
	      h1.SetBinContent(i,y*c);
	      h2.SetBinContent(i,y*cUp);
	      h3.SetBinContent(i,y*cDown);
	    }  
	  }
	  SetNorm(&h1, hmap["QCD"].second.first); // this isn't exactly readable code
	  SetNorm(&h2, hmap["QCD"].second.first);
	  SetNorm(&h3, hmap["QCD"].second.first);
	  hmap["QCD"].first = h1;
	  hmap["QCD_"+syst_qcd_shape+"Up"].first = h2;
	  hmap["QCD_"+syst_qcd_shape+"Down"].first = h3;
	}

	// ************************************************************************
	// Fix Empty Bins
	// ************************************************************************
	vector<string> emptybins_regex;
	boost::split(emptybins_regex, fix_empty_bins, boost::is_any_of(","));
	if (emptybins_regex.size() > 0) {
		std::cout << "[HiggsTauTauPlot4] Running FixEmptyBins with patterns: " << fix_empty_bins << std::endl;
		vector<boost::regex> regex_vec;
		for (auto str : emptybins_regex) regex_vec.push_back(boost::regex(str));
		for (auto & entry : hmap) {
			for (auto const& rgx : regex_vec) {
				if (boost::regex_match(entry.first, rgx)) {
					FixEmptyBins(&(entry.second.first), false);
				}
			}
		}
	}

	// ************************************************************************
	// Fix Negative Bins
	// ************************************************************************
	vector<string> negbins_regex;
	boost::split(negbins_regex, fix_negative_bins, boost::is_any_of(","));
	if (negbins_regex.size() > 0) {
		std::cout << "[HiggsTauTauPlot4] Running FixNegativeBins with patterns: " << fix_negative_bins << std::endl;
		vector<boost::regex> regex_vec;
		for (auto str : negbins_regex) regex_vec.push_back(boost::regex(str));
		for (auto & entry : hmap) {
			for (auto const& rgx : regex_vec) {
				if (boost::regex_match(entry.first, rgx)) {
					FixNegativeBins(&(entry.second.first), false);
				}
			}
		}
	}


	// ************************************************************************
	// Fix Empty Histograms
	// ************************************************************************
	vector<string> emptyhists_regex;
	boost::split(emptyhists_regex, fix_empty_hists, boost::is_any_of(","));
	if (emptyhists_regex.size() > 0) {
		std::cout << "[HiggsTauTauPlot4] Running FixEmptyHist with patterns: " << fix_empty_hists << std::endl;
		vector<boost::regex> regex_vec;
		for (auto str : emptyhists_regex) regex_vec.push_back(boost::regex(str));
		for (auto & entry : hmap) {
			for (auto const& rgx : regex_vec) {
				if (boost::regex_match(entry.first, rgx)) {
					FixEmptyHist(&(entry.second.first), false);
				}
			}
		}
	}

	// ************************************************************************
	// Write datacard
	// ************************************************************************
	if (datacard != "") {
		std::string dc_mode_label;
		if (channel_str == "et") 			dc_mode_label = "eleTau";
		if (channel_str == "mt") 			dc_mode_label = "muTau";
		if (channel_str == "mtmet") 	dc_mode_label = "muTauMet";
		if (channel_str == "em") 			dc_mode_label = "emu";
		std::string tfile_name = "datacard_"+reduced_var+"_"+datacard+"_"+channel_str+(is_2012 ? "_2012":"_2011")+".root";
		TFile dc_file(tfile_name.c_str(),"RECREATE");
		dc_file.cd();
		gDirectory->mkdir((dc_mode_label+"_"+datacard).c_str());
		gDirectory->cd((dc_mode_label+"_"+datacard).c_str());

		for (auto iter : hmap) {
			iter.second.first.SetTitle(iter.first.c_str());
			iter.second.first.SetName(iter.first.c_str());
			iter.second.first.Write();
		}
		std::cout << "[HiggsTauTauPlot4] Writing datacard input " << tfile_name << std::endl;
		dc_file.Close();
	}

	// ************************************************************************
	// Replace signal histograms using the correct cross sections, for plotting
	// ************************************************************************
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "");

	// ************************************************************************
	// Deduce titles
	// ************************************************************************
	if (auto_titles) {
		double fb_lumi = ana.GetLumi() / 1000.;
		string com = is_2012 ? "8" : "7";
		plot.set_title_left((boost::format("CMS Preliminary, #sqrt{s} = %s TeV, L = %.1f fb^{-1}") % com % fb_lumi).str());
		if (channel_str == "et") 		plot.set_title_right("e#tau_{h}");
		if (channel_str == "mt") 		plot.set_title_right("#mu#tau_{h}");
		if (channel_str == "mtmet") plot.set_title_right("#mu#tau_{h}+MET");
		if (channel_str == "em") 		plot.set_title_right("e#mu");
	}
	plot.GeneratePlot(hmap);

  return 0;
}

