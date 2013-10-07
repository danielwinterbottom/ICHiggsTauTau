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
	bool do_ss;                            		    // Tweaking some things for the paper
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
	string syst_tquark;
	string syst_w_fake_rate;
	string syst_eff_t;
	string syst_zl_shift;
  string syst_fakes_os_ss_shape;
	string add_sm_background;
	double sub_ztt_top_frac;
	double shift_tscale;
	string fix_empty_bins;
	string fix_negative_bins;
	string fix_empty_hists;
	string inflate_errors;
	string add_extra_binning;
	string shift_backgrounds;
	bool auto_titles;
	bool check_ztt_top_frac;
	unsigned scan_bins;
  double qcd_os_ss_factor;
  string w_binned;

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
  	("channel",                 po::value<string>(&channel_str)->required())
  	("method",           		    po::value<unsigned>(&method)->required())
	  ("var",              		    po::value<string>(&var)->required())
	  ("cat",             		    po::value<string>(&cat)->default_value(""))
	  ("is_2012",                 po::value<bool>(&is_2012)->required())
	  ("verbosity",               po::value<unsigned>(&verbosity)->default_value(0))
	  ("is_paper",                po::value<bool>(&is_paper)->default_value(false))
	  ("do_ss", 	                po::value<bool>(&do_ss)->default_value(false))
	  ("datacard",                po::value<string>(&datacard)->default_value(""))
	  ("set_alias",               po::value<vector<string>>(&set_alias)->composing())
	  ("sm_masses",               po::value<string>(&sm_masses_str)->default_value(""))
	  ("hww_masses",              po::value<string>(&hww_masses_str)->default_value(""))
	  ("mssm_masses",             po::value<string>(&mssm_masses_str)->default_value(""))
	  ("syst_tau_scale",          po::value<string>(&syst_tau_scale)->default_value(""))
	  ("syst_eff_b",      		    po::value<string>(&syst_eff_b)->default_value(""))
	  ("syst_eff_t",      		    po::value<string>(&syst_eff_t)->default_value(""))
	  ("syst_fake_b",      		    po::value<string>(&syst_fake_b)->default_value(""))
	  ("syst_scale_j",            po::value<string>(&syst_scale_j)->default_value(""))
	  ("syst_l1met",              po::value<string>(&syst_l1met)->default_value(""))
	  ("syst_qcd_shape",          po::value<string>(&syst_qcd_shape)->default_value(""))
	  ("syst_fakes_shape",        po::value<string>(&syst_fakes_shape)->default_value(""))
	  ("syst_fakes_os_ss_shape",  po::value<string>(&syst_fakes_os_ss_shape)->default_value(""))
	  ("syst_ggh_pt",    			    po::value<string>(&syst_ggh_pt)->default_value(""))
	  ("syst_tquark",    			    po::value<string>(&syst_tquark)->default_value(""))
	  ("syst_w_fake_rate",   	    po::value<string>(&syst_w_fake_rate)->default_value(""))
	  ("syst_zl_shift",    		    po::value<string>(&syst_zl_shift)->default_value(""))
	  ("add_sm_background",       po::value<string>(&add_sm_background)->default_value(""))
	  ("sub_ztt_top_frac",        po::value<double>(&sub_ztt_top_frac)->default_value(-1.0))
	  ("shift_tscale",    		    po::value<double>(&shift_tscale)->default_value(0.0))
	  ("fix_empty_bins",   		    po::value<string>(&fix_empty_bins)->default_value(""))
	  ("fix_empty_hists",   	    po::value<string>(&fix_empty_hists)->default_value(""))
	  ("inflate_errors",     	    po::value<string>(&inflate_errors)->default_value(""))
	  ("fix_negative_bins",  	    po::value<string>(&fix_negative_bins)->default_value(""))
	  ("add_extra_binning",       po::value<string>(&add_extra_binning)->default_value(""))
	  ("shift_backgrounds",       po::value<string>(&shift_backgrounds)->default_value(""))
	  ("w_binned",                po::value<string>(&w_binned)->default_value(""))
	  ("auto_titles",   			    po::value<bool>(&auto_titles)->default_value(true))
	  ("check_ztt_top_frac",      po::value<bool>(&check_ztt_top_frac)->default_value(false))
	  ("scan_bins",               po::value<unsigned>(&scan_bins)->default_value(0))
	  ("qcd_os_ss_factor",  	    po::value<double>(&qcd_os_ss_factor)->default_value(1.06));


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
  if (do_ss) ana.SetQCDRatio(1.0);
	for (auto const& a : alias_vec) ana.SetAlias(a.first, a.second);
	ana.AddSMSignalSamples(sm_masses);
	ana.AddHWWSignalSamples(hww_masses);
	if (add_sm_background != "") {
		ana.AddSMSignalSamples({add_sm_background});
		ana.AddHWWSignalSamples({add_sm_background});
	}
	ana.AddMSSMSignalSamples(mssm_masses);
	if (is_2012 && check_ztt_top_frac) ana.AddSample("RecHit-TTJets_FullLeptMGDecays");
	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTAnalysis::HistValueMap hmap;

	std::string sel = "os && "+ana.ResolveAlias("sel");
	if (do_ss) {
		sel = "!os && "+ana.ResolveAlias("sel");
		ana.SetAlias("w_os", "!os");
		ana.SetAlias("w_sdb_os", "!os");
		ana.SetAlias("w_vbf_os", "!os");
		ana.SetAlias("w_shape_os", "!os");
	}
	cat = ana.ResolveAlias(cat);

	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "", 1.0);
	ana.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt", "_hww", "", 1.0);
	if (add_sm_background != "") {
		ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "");
		ana.FillHWWSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_hww_SM", "");
	}
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "", 1.0);


	// ************************************************************************
	// Split ZTT into decay modes
	// ************************************************************************
	// hmap["ZTT-1P0PZ"] = ana.GenerateZTT(method, var, sel, cat+" && tau_decay_mode==0", "wt");
	// hmap["ZTT-1P1PZ"] = ana.GenerateZTT(method, var, sel, cat+" && tau_decay_mode==1", "wt");
	// hmap["ZTT-3P"] = ana.GenerateZTT(method, var, sel, cat+" && tau_decay_mode==10", "wt");


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
  string extra_binning_postfix;
  string extra_binning_range = "";
	if (add_extra_binning != "") {
		std::vector<std::string> extra_binning;
		boost::split(extra_binning, add_extra_binning, boost::is_any_of(":"));
		if (extra_binning.size() == 2) {
			std::cout << "-----------------------------------------------------------------------------------" << std::endl;
			std::cout << "[HiggsTauTauPlot4] Adding alternative binning \"" << extra_binning[0] 
				<< "\" with postfix \"" << extra_binning[1] << "\"" << std::endl;
      extra_binning_range = extra_binning[0];
      extra_binning_postfix = extra_binning[1];
			ana.FillHistoMap(hmap, method, reduced_var+extra_binning[0], sel, cat, "wt", extra_binning[1]);
			ana.FillSMSignal(hmap, sm_masses, reduced_var+extra_binning[0], sel, cat, "wt", "", extra_binning[1], 1.0);
			ana.FillMSSMSignal(hmap, mssm_masses, reduced_var+extra_binning[0], sel, cat, "wt", "", extra_binning[1], 1.0);
		}
	}
  std::vector<std::string> vars = { var };
  std::vector<std::string> vars_postfix = { "" };
  if (extra_binning_range != "") {
    vars.push_back(reduced_var+extra_binning_range);
    vars_postfix.push_back(extra_binning_postfix);
  }
	
  // ************************************************************************
	// Alternative W
	// ************************************************************************
  //ana.SetVerbosity(1);
  if (w_binned != "") {
    std::vector<std::string> w_bins;
    boost::split(w_bins, w_binned, boost::is_any_of(","));
    for (unsigned j = 0; j < vars.size(); ++j) {
      auto new_w = hmap["W"+vars_postfix[j]];
      if (w_bins.size() >= 2) {
        for (unsigned i = 0; i < (w_bins.size()-1); ++i) {
          std::string new_cat = cat + " && "+reduced_var+">"+w_bins[i]+" && "+reduced_var+"<="+w_bins[i+1];
          auto w_range = ana.GenerateW(method, vars[j], sel, new_cat, "wt");
          if (i == 0) {
            new_w = w_range;
          } else {
            new_w.first.Add(&w_range.first);
            new_w.second = HTTAnalysis::ValueAdd(new_w.second, w_range.second);
          }
          HTTAnalysis::PrintValue(w_bins[i]+"-"+w_bins[i+1], w_range.second);
          //ana.SetVerbosity(verbosity);
        }
      }
      hmap["W"+vars_postfix[j]] = new_w;
    }
  }

  // ************************************************************************
	// top-quark pT Reweighting
	// ************************************************************************
	if (syst_tquark != "") {
		std::cout << "[HiggsTauTauPlot4] Adding top-quark weight systematic..." << std::endl;
    for (unsigned j = 0; j < vars.size(); ++j) {
		  hmap["ttbar"+vars_postfix[j]+"_"+syst_tquark+"Up"] = ana.GenerateTOP(method, vars[j], sel, cat, "wt*wt_tquark_up");
		  hmap["ttbar"+vars_postfix[j]+"_"+syst_tquark+"Down"] = ana.GenerateTOP(method, vars[j], sel, cat, "wt*wt_tquark_down");
    }
	}

  // ************************************************************************
	// W+jets fake-rate Reweighting
	// ************************************************************************
	if (syst_w_fake_rate != "") {
		std::cout << "[HiggsTauTauPlot4] Adding W+jets fake-rate systematic..." << std::endl;
    for (unsigned j = 0; j < vars.size(); ++j) {
		  hmap["W_"+syst_w_fake_rate+"Up"+vars_postfix[j]] = ana.GenerateW(method, vars[j], sel, cat, "wt*wt_tau_fake_up");
		  hmap["W_"+syst_w_fake_rate+"Down"+vars_postfix[j]] = ana.GenerateW(method, vars[j], sel, cat, "wt*wt_tau_fake_down");
    }
	}

  // ************************************************************************
	// Tau ID Weights
	// ************************************************************************
	if (syst_eff_t != "") {
		std::cout << "[HiggsTauTauPlot4] Adding high tau pT ID systematic..." << std::endl;
    ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt*wt_tau_id_up", "", "_"+syst_eff_t+"Up", 1.0);
    ana.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt*wt_tau_id_up", "_hww", "_"+syst_eff_t+"Up", 1.0);
    ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt*wt_tau_id_up", "", "_"+syst_eff_t+"Up", 1.0);
    if (add_sm_background != "") {
			ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt*wt_tau_id_up", "_SM", "_"+syst_eff_t+"Up");
			ana.FillHWWSignal(hmap, {add_sm_background}, var, sel, cat, "wt*wt_tau_id_up", "_hww_SM", "_"+syst_eff_t+"Up");
    }
    ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt*wt_tau_id_down", "", "_"+syst_eff_t+"Down", 1.0);
    ana.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt*wt_tau_id_down", "_hww", "_"+syst_eff_t+"Down", 1.0);
    ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt*wt_tau_id_down", "", "_"+syst_eff_t+"Down", 1.0);
    if (add_sm_background != "") {
			ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt*wt_tau_id_down", "_SM", "_"+syst_eff_t+"Down");
			ana.FillHWWSignal(hmap, {add_sm_background}, var, sel, cat, "wt*wt_tau_id_down", "_hww_SM", "_"+syst_eff_t+"Down");
    }
 		hmap["ZTT_"+syst_eff_t+"Up"] = ana.GenerateZTT(method, var, sel, cat, "wt*wt_tau_id_up");
 		hmap["ZTT_"+syst_eff_t+"Down"] = ana.GenerateZTT(method, var, sel, cat, "wt*wt_tau_id_down");
	}

  // ************************************************************************
	// Update ZTT L1MET systematics
	// ************************************************************************
	if (syst_l1met != "") {
    string backup_eff = ana.ResolveAlias("ZTT_Eff_Sample");
    string backup_shape = ana.ResolveAlias("ZTT_Shape_Sample");
    ana.SetAlias("ZTT_Eff_Sample","DYJetsToTauTauSoup");
    ana.SetAlias("ZTT_Shape_Sample","DYJetsToTauTauSoup");
    hmap["ZTTMC"] = ana.GenerateZTT(method, var, sel, cat, "wt");
    ana.SetAlias("ZTT_Eff_Sample", backup_eff);
    ana.SetAlias("ZTT_Shape_Sample", backup_shape);
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
		if (add_sm_background != "") {
			ana_syst.AddSMSignalSamples({add_sm_background});
			ana_syst.AddHWWSignalSamples({add_sm_background});
		}
		ana_syst.AddMSSMSignalSamples(mssm_masses);
		ana_syst.ReadTrees(folder+syst.first, folder);
		ana_syst.ParseParamFile(paramfile);
		ana_syst.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst.second);
		ana_syst.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst.second, 1.0);
		ana_syst.FillHWWSignal(hmap, hww_masses, var, sel, cat, "wt", "_hww", "_"+syst.second, 1.0);
		if (add_sm_background != "") {
			ana_syst.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst.second);
			ana_syst.FillHWWSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_hww_SM", "_"+syst.second);
		}
		ana_syst.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst.second, 1.0);
	  if (syst.second == syst_l1met+"Down" || syst.second == syst_l1met+"Up") {
      string backup_eff = ana_syst.ResolveAlias("ZTT_Eff_Sample");
      string backup_shape = ana_syst.ResolveAlias("ZTT_Shape_Sample");
      ana_syst.SetAlias("ZTT_Eff_Sample","DYJetsToTauTauSoup");
      ana_syst.SetAlias("ZTT_Shape_Sample","DYJetsToTauTauSoup");
      hmap["ZTTMC_"+syst.second] = ana_syst.GenerateZTT(method, var, sel, cat, "wt");
      ana_syst.SetAlias("ZTT_Eff_Sample", backup_eff);
      ana_syst.SetAlias("ZTT_Shape_Sample", backup_shape);
      TH1F const& mc_central = hmap["ZTTMC"].first;
      TH1F const& mc_shift = hmap["ZTTMC_"+syst.second].first;
      TH1F const& em_central = hmap["ZTT"].first;
      TH1F & em_shift = hmap["ZTT_"+syst.second].first;
      for (int i = 1; i <= em_shift.GetNbinsX(); ++i) {
        if (mc_central.GetBinContent(i) > 0.){
          double bin_shift = mc_shift.GetBinContent(i) / mc_central.GetBinContent(i);
          em_shift.SetBinContent(i, em_central.GetBinContent(i) * bin_shift);
          em_shift.SetBinError(i, 0.);
        } else {
          em_shift.SetBinContent(i, em_central.GetBinContent(i));
          em_shift.SetBinError(i, 0.);
        }
      }
	  }
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
	// Specifiy this with parameters:
	// "name:xmax:central:band"
	if (syst_qcd_shape != "") {
		vector<string> sub_strings;
		boost::split(sub_strings, syst_qcd_shape, boost::is_any_of(":"));
		if (sub_strings.size() == 4) {
			string syst_qcd_name 		= sub_strings[0];
			double syst_qcd_xmax 		= boost::lexical_cast<double>(sub_strings[1]);
			double syst_qcd_central = boost::lexical_cast<double>(sub_strings[2]);
			double syst_qcd_band 		= boost::lexical_cast<double>(sub_strings[3]);
			std::cout << "[HiggsTauTauPlot4] Adding QCD low-mass shape systematic..." << std::endl;
			std::cout << boost::format(param_fmt()) % "name" % syst_qcd_name;
			std::cout << boost::format(param_fmt()) % "xmax" % syst_qcd_xmax;
			std::cout << boost::format(param_fmt()) % "central" % syst_qcd_central;
			std::cout << boost::format(param_fmt()) % "uncertainty" % syst_qcd_band;
		  std::string qcd_label = (channel_str == "em") ? "Fakes" : "QCD";
      for (unsigned j = 0; j < vars.size(); ++j) {
        TH1F h1 = hmap[qcd_label+vars_postfix[j]].first;
        TH1F h2 = hmap[qcd_label+vars_postfix[j]].first;
        TH1F h3 = hmap[qcd_label+vars_postfix[j]].first;
        float x, y;
        for(int i=1;i<h1.GetNbinsX();++i){
          x = h1.GetXaxis()->GetBinCenter(i);
          if(x < syst_qcd_xmax){
            y = h1.GetBinContent(i);
            h1.SetBinContent(i,y*syst_qcd_central);
            h2.SetBinContent(i,y*(syst_qcd_central+syst_qcd_band));
            h3.SetBinContent(i,y*(syst_qcd_central-syst_qcd_band));
          }  
        }
        SetNorm(&h1, hmap[qcd_label+vars_postfix[j]].second.first); // this isn't exactly readable code
        SetNorm(&h2, hmap[qcd_label+vars_postfix[j]].second.first);
        SetNorm(&h3, hmap[qcd_label+vars_postfix[j]].second.first);
        hmap[qcd_label+vars_postfix[j]].first = h1;
        hmap[qcd_label+"_"+syst_qcd_name+"Up"+vars_postfix[j]].first = h2;
        hmap[qcd_label+"_"+syst_qcd_name+"Down"+vars_postfix[j]].first = h3;
      }
		}
	}
	// ************************************************************************
	// Apply special e-mu OS/SS fakes correction 
	// ************************************************************************
	if (syst_fakes_os_ss_shape != "") {
		  std::string qcd_label = (channel_str == "em") ? "Fakes" : "QCD";
      for (unsigned j = 0; j < vars.size(); ++j) {
        TH1F weights = ic::GetFromTFile<TH1F>("data/scale_factors/OS_SS_weights.root","/","OSoverSS_msv_nobtag_ratio");
        TH1F h1 = hmap[qcd_label+vars_postfix[j]].first;
        TH1F h2 = hmap[qcd_label+vars_postfix[j]].first;
        TH1F h3 = hmap[qcd_label+vars_postfix[j]].first;
        float x, y;
        for(int i=1;i<h1.GetNbinsX();++i){
          x = h1.GetXaxis()->GetBinCenter(i);
          y = h1.GetBinContent(i);
          float corr = weights.GetBinContent(weights.FindBin(x));
          h1.SetBinContent(i, y*corr);
          h2.SetBinContent(i, y*std::max(2.0*corr - 1.0, 0.0));
        }
        SetNorm(&h1, hmap[qcd_label+vars_postfix[j]].second.first); // this isn't exactly readable code
        SetNorm(&h2, hmap[qcd_label+vars_postfix[j]].second.first);
        SetNorm(&h3, hmap[qcd_label+vars_postfix[j]].second.first);
        hmap[qcd_label+vars_postfix[j]].first = h1;
        hmap[qcd_label+"_"+syst_fakes_os_ss_shape+"Up"+vars_postfix[j]].first = h2;
        hmap[qcd_label+"_"+syst_fakes_os_ss_shape+"Down"+vars_postfix[j]].first = h3;
      }
  }

	// ************************************************************************
	// Generate ZL mass shifted sytematic shapes
	// ************************************************************************
	// Specifiy this with parameters:
	// "name:shift_up:shift_down", e.g. name:1.02:0.98
	if (syst_zl_shift != "") {
		vector<string> sub_strings;
		boost::split(sub_strings, syst_zl_shift, boost::is_any_of(":"));
		if (sub_strings.size() == 3) {
			string syst_zl_name 			= sub_strings[0];
			string syst_zl_shift_up 	= sub_strings[1];
			string syst_zl_shift_down = sub_strings[2];
			std::cout << "[HiggsTauTauPlot4] Adding ZL mass shift shape systematic..." << std::endl;
			std::cout << boost::format(param_fmt()) % "name" % syst_zl_name;
			std::cout << boost::format(param_fmt()) % "shift_up" % syst_zl_shift_up;
			std::cout << boost::format(param_fmt()) % "shift_down" % syst_zl_shift_down;
			hmap["ZL_"+syst_zl_name+"Up"] = ana.GenerateZL(method, syst_zl_shift_up+"*"+var, sel, cat, "wt");
			hmap["ZL_"+syst_zl_name+"Down"] = ana.GenerateZL(method, syst_zl_shift_down+"*"+var, sel, cat, "wt");
		}
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
	// Inflate errors
	// ************************************************************************
	vector<string> inflate_regex;
	boost::split(inflate_regex, inflate_errors, boost::is_any_of(","));
	if (inflate_regex.size() > 0) {
		vector<boost::regex> regex_vec;
		vector<double> err_vec;
		for (auto str : inflate_regex) {
      vector<string> tmp_vec;
	    boost::split(tmp_vec, str, boost::is_any_of(":"));
      if (tmp_vec.size() != 2) continue;
      regex_vec.push_back(boost::regex(tmp_vec[0]));
      err_vec.push_back(boost::lexical_cast<double>(tmp_vec[1]));
    }
		for (auto & entry : hmap) {
			for (unsigned i = 0; i < regex_vec.size(); ++i) {
				if (boost::regex_match(entry.first, regex_vec[i])) {
					InflateErrors(&(entry.second.first), err_vec[i]);
				}
			}
		}
	}

  // ************************************************************************
	// Scan and fix bins that have data, no bkg and some signal
	// ************************************************************************
  if (scan_bins) {
    vector<string> bkgs = {"ZTT","ZL","ZJ","W","QCD","TT","VV"};
    if (channel_str == "em") bkgs = {"Ztt","Fakes","EWK","ttbar"}; 
    vector<string> sm_procs = {"ggH","qqH","VH"};
    vector<string> mssm_procs = {"ggH","bbH"};
    TH1F const& data = hmap["data_obs"].first;
    for (int i = 1; i <= data.GetNbinsX(); ++i) {
      double bkg_tot = 0.;
      double data_bin = data.GetBinContent(i);
      bool has_signal = false;
      for (auto bkg : bkgs) bkg_tot += hmap[bkg].first.GetBinContent(i);
      if (data_bin > 0. && bkg_tot <= 0.) {
        std::cout << "\e[31mWarning: Bin [" << data.GetBinLowEdge(i) << "," << data.GetBinLowEdge(i+1) << "] has data and total background: " <<  bkg_tot << "\e[m" << std::endl;
        for (auto sm_mass : sm_masses) {
          for (auto proc : sm_procs) {
            if (hmap[proc+sm_mass].first.GetBinContent(i) > 0.) {
              std::cout << "\e[31mWarning: Template " << proc+sm_mass << " is populated in this bin\e[m" << std::endl;
              has_signal = true;
            }
          }
        }
        for (auto mssm_mass : mssm_masses) {
          for (auto proc : sm_procs) {
            if (hmap[proc+mssm_mass].first.GetBinContent(i) > 0.) {
              std::cout << "\e[31mWarning: Template " << proc+mssm_mass << " is populated in this bin\e[m" << std::endl;
              has_signal = true;
            }
          }
        }
        if (has_signal && scan_bins>1) {
          std::cout << "\e[32mWarning: This bin will be set to zero in all templates\e[m" << std::endl;
          for (auto & entry : hmap) {
            entry.second.first.SetBinContent(i, 0.);
            entry.second.first.SetBinError(i, 0.);
          }
        }
      }
    }
  }



  if (true) {
    TH1F w = hmap["W"].first;
    w.Add(&(hmap["QCD"].first));
    hmap["WMerged"].first = w;
  	
  	unsigned bbb_added 		= 0;
  	unsigned bbb_removed 	= 0;
  	double add_threshold 			= 0.1;
  	double remove_threshold 	= 0.1;
  	bool expand_after_removal = true;

    vector<string> all_bkgs = {"ZTT","ZL","ZJ","W","QCD","TT","VV"};
    vector<string> bbb_bkgs = {"ZL","ZJ","WMerged"};
    vector<string> sm_procs = {"ggH","qqH","VH"};
    vector<string> mssm_procs = {"ggH","bbH"};
  
    TH1F all_bkg = hmap[all_bkgs[0]].first;
    TH1F bbb_bkg = hmap[bbb_bkgs[0]].first;
    for (unsigned i = 1; i < all_bkgs.size(); ++i) all_bkg.Add(&(hmap[all_bkgs[i]].first));
    for (unsigned i = 1; i < bbb_bkgs.size(); ++i) bbb_bkg.Add(&(hmap[bbb_bkgs[i]].first));
    
    for (int i = 1; i <= all_bkg.GetNbinsX(); ++i) {
      // double all_val = bbb_bkg.GetBinContent(i);
      // double all_err = bbb_bkg.GetBinError(i); 
      
      double tot_bbb_add = 0.;

      for (unsigned j = 0; j < bbb_bkgs.size(); ++j) {
        TH1F const& hist = hmap[bbb_bkgs[j]].first;
        if (hist.GetBinContent(i) == 0. && hist.GetBinError(i) == 0.) continue;
        if (hist.GetBinError(i)/hist.GetBinContent(i) > add_threshold) {
          bbb_added++;
          tot_bbb_add += (hist.GetBinError(i)*hist.GetBinError(i));
        }
      }
      if (tot_bbb_add == 0.) continue;

      std::vector<std::pair<std::string, double>> results;
      for (unsigned j = 0; j < bbb_bkgs.size(); ++j) {
        TH1F const& hist = hmap[bbb_bkgs[j]].first;
        if (hist.GetBinContent(i) == 0. && hist.GetBinError(i) == 0.) continue;
        if (hist.GetBinError(i)/hist.GetBinContent(i) > add_threshold) {
          double frac = hist.GetBinError(i)*hist.GetBinError(i)/tot_bbb_add;
          results.push_back(std::make_pair(bbb_bkgs[j], frac));
        }
      }
      sort(results.begin(), results.end(),
               [](const pair<std::string, double>& lhs, const pair<std::string, double>& rhs) {
                            return lhs.second < rhs.second; } );
      double removed = 0.;
      for (unsigned j = 0; j < results.size(); ++j) {
        bool remove = false;
        if ((removed+results[j].second) < remove_threshold) {
          remove = true;
          bbb_removed++;
          removed += results[j].second;
          hmap[results[j].first].first.SetBinError(i, 0.0);
          if (results[j].first == "WMerged") {
          	hmap["W"].first.SetBinError(i, 0.0);
          	hmap["QCD"].first.SetBinError(i, 0.0);
          }
        }
        std::cout << results[j].first << "\t" << results[j].second << "\t" << remove << std::endl;
      }
      if (expand_after_removal) {
      	for (unsigned j = 0; j < results.size(); ++j) {
      		double expand = 1./(1.-removed);
      		TH1F & hist = hmap[results[j].first].first;
      		std::cout << results[j].first << "\t" << hist.GetBinError(j) << " --> ";
      		hist.SetBinError(i, hist.GetBinError(i) * expand);
      		if (results[j].first == "WMerged") {
      			hmap["W"].first.SetBinError(i, hmap["W"].first.GetBinError(i) * expand);
      			hmap["QCD"].first.SetBinError(i, hmap["QCD"].first.GetBinError(i) * expand);
      		}
		      std::cout << hist.GetBinError(j) << std::endl;
      	}
      }
    }
    std::cout << "TOTAL bbb added:   " << bbb_added << std::endl;
    std::cout << "TOTAL bbb removed: " << bbb_removed << std::endl;
    std::cout << "TOTAL bbb    ====> " << bbb_added-bbb_removed << std::endl;
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
		if (channel_str == "mtmet") 	dc_mode_label = "muTau_soft";
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
	if (hww_masses.size() != 0) {
		//
    //string m = plot.draw_signal_mass();
		for (auto m : sm_masses) {
     //HTTAnalysis::PrintValue("ggH"+m, hmap["ggH"+m].second);
		 //HTTAnalysis::PrintValue("qqH"+m, hmap["qqH"+m].second);
		 //HTTAnalysis::PrintValue("VH"+m, hmap["VH"+m].second);
    }
		// for (auto m : hww_masses) {
    //  HTTAnalysis::PrintValue("ggH(WW/Total)"+m, HTTAnalysis::ValueDivide(hmap["ggH_hww"+m].second,HTTAnalysis::ValueAdd(hmap["ggH"+m].second, hmap["ggH_hww"+m].second)));
    // }
		// for (auto m : hww_masses) {
		//  HTTAnalysis::PrintValue("qqH(WW/Total)"+m, HTTAnalysis::ValueDivide(hmap["qqH_hww"+m].second,HTTAnalysis::ValueAdd(hmap["qqH"+m].second, hmap["qqH_hww"+m].second)));
    // }
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
		std::string ztt_label = (channel_str == "em") ? "Ztt" : "ZTT";
		std::cout << "[HiggsTauTauPlot4] Shifting energy scale by pull: " << shift_tscale << std::endl;
		TH1F ztt_central = hmap[ztt_label].first;
		TH1F ztt_down = hmap[ztt_label+"_"+syst_tau_scale+"Down"].first;
		TH1F ztt_up = hmap[ztt_label+"_"+syst_tau_scale+"Up"].first;
		ztt_down.Scale(Integral(&ztt_central) / Integral(&ztt_down));
		ztt_up.Scale(Integral(&ztt_central) / Integral(&ztt_up));
		ic::VerticalMorph(&ztt_central, &ztt_up, &ztt_down, shift_tscale);
		hmap[ztt_label].first = ztt_central;
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

