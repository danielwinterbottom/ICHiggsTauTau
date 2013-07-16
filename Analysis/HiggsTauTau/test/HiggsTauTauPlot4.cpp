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
	string hww_masses_str;
	string mssm_masses_str;												
	string syst_tau_scale;
	string syst_eff_b;
	string syst_fake_b;
	string syst_scale_j;
	string syst_l1met;
	string syst_qcd_shape;
	string syst_fakes_shape;
	string syst_ggh_pt;
	string add_sm_background;
	double sub_ztt_top_frac;
	double shift_tscale;
	string fix_empty_bins;
	string fix_negative_bins;
	string fix_empty_hists;
	string add_extra_binning;
	string shift_backgrounds;
	bool auto_titles;
	bool check_ztt_top_frac;
  double qcd_os_ss_factor;
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
	  ("hww_masses",           po::value<string>(&hww_masses_str)->default_value(""))
	  ("mssm_masses",         po::value<string>(&mssm_masses_str)->default_value(""))
	  ("syst_tau_scale",      po::value<string>(&syst_tau_scale)->default_value(""))
	  ("syst_eff_b",      		po::value<string>(&syst_eff_b)->default_value(""))
	  ("syst_fake_b",      		po::value<string>(&syst_fake_b)->default_value(""))
	  ("syst_scale_j",        po::value<string>(&syst_scale_j)->default_value(""))
	  ("syst_l1met",          po::value<string>(&syst_l1met)->default_value(""))
	  ("syst_qcd_shape",      po::value<string>(&syst_qcd_shape)->default_value(""))
	  ("syst_fakes_shape",    po::value<string>(&syst_fakes_shape)->default_value(""))
	  ("syst_ggh_pt",    			po::value<string>(&syst_ggh_pt)->default_value(""))
	  ("add_sm_background",   po::value<string>(&add_sm_background)->default_value(""))
	  ("sub_ztt_top_frac",    po::value<double>(&sub_ztt_top_frac)->default_value(-1.0))
	  ("shift_tscale",    		po::value<double>(&shift_tscale)->default_value(0.0))
	  ("fix_empty_bins",   		po::value<string>(&fix_empty_bins)->default_value(""))
	  ("fix_empty_hists",   	po::value<string>(&fix_empty_hists)->default_value(""))
	  ("fix_negative_bins",  	po::value<string>(&fix_negative_bins)->default_value(""))
	  ("add_extra_binning",   po::value<string>(&add_extra_binning)->default_value(""))
	  ("shift_backgrounds",   po::value<string>(&shift_backgrounds)->default_value(""))
	  ("auto_titles",   			po::value<bool>(&auto_titles)->default_value(true))
	  ("check_ztt_top_frac",  po::value<bool>(&check_ztt_top_frac)->default_value(false))
	  ("qcd_os_ss_factor",  	po::value<double>(&qcd_os_ss_factor)->default_value(1.06));


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
	std::cout << boost::format(param_fmt()) % "hww_masses" 	% hww_masses_str;
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
	std::vector<std::string> hww_masses;
	if (hww_masses_str != "") boost::split(hww_masses, hww_masses_str, boost::is_any_of(","));
	std::vector<std::string> mssm_masses;
	if (mssm_masses_str != "") boost::split(mssm_masses, mssm_masses_str, boost::is_any_of(","));

	// ************************************************************************
	// Setup HTTAnalysis 
	// ************************************************************************
	HTTAnalysis ana(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
  ana.SetQCDRatio(qcd_os_ss_factor);
	for (auto const& a : alias_vec) ana.SetAlias(a.first, a.second);
	ana.AddSMSignalSamples(sm_masses);
	ana.AddHWWSignalSamples(hww_masses);
	if (add_sm_background != "") ana.AddSMSignalSamples({add_sm_background});
	ana.AddMSSMSignalSamples(mssm_masses);
	if (is_2012 && check_ztt_top_frac) ana.AddSample("RecHit-TTJets_FullLeptMGDecays");
	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTAnalysis::HistValueMap hmap;

	std::string sel = "os && "+ana.ResolveAlias("sel");
	cat = ana.ResolveAlias(cat);

	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "", 1.0);
	ana.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt", "_hww", "", 1.0);
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
			ana.FillSMSignal(hmap, sm_masses, reduced_var+extra_binning[0], sel, cat, "wt", "", extra_binning[1], 1.0);
			ana.FillMSSMSignal(hmap, mssm_masses, reduced_var+extra_binning[0], sel, cat, "wt", "", extra_binning[1], 1.0);
		}
	}

	vector<pair<string,string>> systematics;

	// ************************************************************************
	// Add tau/electron energy scale systematics
	// ************************************************************************
	if (syst_tau_scale != "") {
		systematics.push_back(make_pair("/TSCALE_DOWN", syst_tau_scale+"Down"));
		systematics.push_back(make_pair("/TSCALE_UP", syst_tau_scale+"Up"));
	}
	if (syst_eff_b != "") {
		systematics.push_back(make_pair("/BTAG_DOWN", syst_eff_b+"Down"));
		systematics.push_back(make_pair("/BTAG_UP", syst_eff_b+"Up"));
	}
	if (syst_fake_b != "") {
		systematics.push_back(make_pair("/BFAKE_DOWN", syst_fake_b+"Down"));
		systematics.push_back(make_pair("/BFAKE_UP", syst_fake_b+"Up"));
	}
	if (syst_scale_j != "") {
		systematics.push_back(make_pair("/JES_DOWN", syst_scale_j+"Down"));
		systematics.push_back(make_pair("/JES_UP", syst_scale_j+"Up"));
	}
	if (syst_l1met != "") {
		systematics.push_back(make_pair("/L1MET_DOWN", syst_l1met+"Down"));
		systematics.push_back(make_pair("/L1MET_UP", syst_l1met+"Up"));
	}

	for (auto const& syst : systematics) {
		std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		std::cout << "[HiggsTauTauPlot4] Doing systematic templates for \"" << syst.second << "\"..." << std::endl;
		HTTAnalysis ana_syst(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
    ana_syst.SetQCDRatio(qcd_os_ss_factor);
		for (auto const& a : alias_vec) ana_syst.SetAlias(a.first, a.second);
		ana_syst.AddSMSignalSamples(sm_masses);
		ana_syst.AddHWWSignalSamples(hww_masses);
		if (add_sm_background != "") ana_syst.AddSMSignalSamples({add_sm_background});
		ana_syst.AddMSSMSignalSamples(mssm_masses);
		ana_syst.ReadTrees(folder+syst.first, folder);
		ana_syst.ParseParamFile(paramfile);
		ana_syst.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst.second);
		ana_syst.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst.second, 1.0);
		ana_syst.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt", "_hww", "_"+syst.second, 1.0);
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
		HTTAnalysis::PrintValue("New "+top_label+ " rate", top_rate);
		boost::regex top_regex(top_label+".*");
		for (auto & entry : hmap) {
			if (boost::regex_match(entry.first, top_regex)) {
				std::cout << "Correcting rate in " << entry.first << std::endl;
				entry.second.second = HTTAnalysis::ValueSubtract(entry.second.second, contamination);
				SetNorm(&(entry.second.first), entry.second.second.first);
			}
		}
	}

  // ************************************************************************
	// Check ttbar MC embedded
	// ************************************************************************
  if (is_2012 && check_ztt_top_frac) {
		std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		std::cout << "[HiggsTauTauPlot4] Checking TOP contamination in ZTT embedded..." << std::endl;
    HTTAnalysis::Value embedded_ttbar = ana.GetLumiScaledRate("RecHit-TTJets_FullLeptMGDecays", sel, cat, "wt");
    HTTAnalysis::Value embedded_ttbar_inc = ana.GetLumiScaledRate("RecHit-TTJets_FullLeptMGDecays", "os", "", "wt");
    HTTAnalysis::Value embedded_data = ana.GetRate("Embedded", sel, cat, "wt");
    HTTAnalysis::Value embedded_data_inc = ana.GetRate("Embedded", "os", "", "wt");
    HTTAnalysis::PrintValue("EmbeddedTop", embedded_ttbar);
    HTTAnalysis::PrintValue("EmbeddedData", embedded_data);
    double uncorr_eff = embedded_data.first / embedded_data_inc.first;
    double corr_eff = (embedded_data.first - embedded_ttbar.first) / (embedded_data_inc.first - embedded_ttbar_inc.first);
    std::cout << "Uncorrected Eff:  " << uncorr_eff << std::endl;
    std::cout << "Corrected Eff:    " << corr_eff << std::endl;
    std::cout << "Ratio:            " << corr_eff/uncorr_eff << std::endl;
		std::string top_label = (channel_str == "em") ? "ttbar" : "TT";
		std::string ztt_label = (channel_str == "em") ? "Ztt" : "ZTT";
    HTTAnalysis::Value scaled_embedded = hmap[ztt_label].second;
    double norm_sf = scaled_embedded.first / embedded_data.first;
    double embedded_ttbar_norm = embedded_ttbar.first * norm_sf;
    TH1F embedded_ttbar_shape = ana.GetLumiScaledShape(var,"RecHit-TTJets_FullLeptMGDecays", sel, cat, "wt");
    SetNorm(&embedded_ttbar_shape, embedded_ttbar_norm);
    hmap[top_label+"Embedded"] = make_pair(embedded_ttbar_shape, make_pair(embedded_ttbar_norm, 0.));
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
	// Print H->WW contribution for e-mu
	// ************************************************************************
	/*
	if (channel_str == "em" && is_2012) {
		hmap["ggHWW125"] = std::make_pair(
      ana.GetLumiScaledShape(var, "GluGluToHToWWTo2LAndTau2Nu_M-125", sel, cat, "wt"),
      ana.GetLumiScaledRate("GluGluToHToWWTo2LAndTau2Nu_M-125", sel, cat, "wt"));
		hmap["qqHWW125"] = std::make_pair(
      ana.GetLumiScaledShape(var, "VBF_HToWWTo2LAndTau2Nu_M-125", sel, cat, "wt"),
      ana.GetLumiScaledRate("VBF_HToWWTo2LAndTau2Nu_M-125", sel, cat, "wt"));
		HTTAnalysis::PrintValue("ggHWW125", hmap["ggHWW125"].second);
		HTTAnalysis::PrintValue("qqHWW125", hmap["qqHWW125"].second);
	}
	*/

	// ************************************************************************
	// Write datacard
	// ************************************************************************
	if (datacard != "") {
		std::string dc_mode_label;
		if (channel_str == "et") 			dc_mode_label = "eleTau";
		if (channel_str == "mt") 			dc_mode_label = "muTau";
		if (channel_str == "mtmet") 	dc_mode_label = "muTauSoft";
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
	ana.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt", "_hww", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "");
	if (sm_masses.size() != 0) {
		string m = plot.draw_signal_mass();
		HTTAnalysis::PrintValue("ggH"+m, hmap["ggH"+m].second);
		HTTAnalysis::PrintValue("qqH"+m, hmap["qqH"+m].second);
		HTTAnalysis::PrintValue("VH"+m, hmap["VH"+m].second);
	}



	// ************************************************************************
	// Shift backgrounds
	// ************************************************************************
	vector<string> shift_strs;
	boost::split(shift_strs, shift_backgrounds, boost::is_any_of(","));
	if (shift_strs.size() > 0) {
		std::cout << "[HiggsTauTauPlot4] Shifting background yields... " << std::endl;
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
	// Shift tau energy scale
	// ************************************************************************
	if (shift_tscale != 0.0) {
		std::cout << "[HiggsTauTauPlot4] Shifting tau energy scale by pull: " << shift_tscale << std::endl;
		TH1F ztt_central = hmap["ZTT"].first;
		TH1F ztt_down = hmap["ZTT_"+syst_tau_scale+"Down"].first;
		TH1F ztt_up = hmap["ZTT_"+syst_tau_scale+"Up"].first;
		ztt_down.Scale(Integral(&ztt_central) / Integral(&ztt_down));
		ztt_up.Scale(Integral(&ztt_central) / Integral(&ztt_up));
		ic::VerticalMorph(&ztt_central, &ztt_up, &ztt_down, shift_tscale);
		hmap["ZTT"].first = ztt_central;
	}

	// ************************************************************************
	// Deduce titles
	// ************************************************************************
	if (auto_titles) {
		double fb_lumi = ana.GetLumi() / 1000.;
		string com = is_2012 ? "8" : "7";
		plot.set_title_left((boost::format("CMS Preliminary, #sqrt{s} = %s TeV, L = %.1f fb^{-1}") % com % fb_lumi).str());
		if (channel_str == "et") 		plot.set_title_right("e#tau_{h}");
		if (channel_str == "mt") 		plot.set_title_right("#mu#tau_{h}");
		if (channel_str == "mtmet") plot.set_title_right("#mu_{soft}#tau_{h}");
		if (channel_str == "em") 		plot.set_title_right("e#mu");
	}
	plot.GeneratePlot(hmap);

  return 0;
}

