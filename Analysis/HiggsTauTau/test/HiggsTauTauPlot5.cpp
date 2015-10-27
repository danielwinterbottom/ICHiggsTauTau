#include <iostream>
#include <vector>
#include "TH1F.h"
#include "boost/program_options.hpp"
#include "boost/regex.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTRun2AnalysisTools.h"
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
	unsigned verbosity;														// Verbose output, useful for diagnostic purposes
  unsigned use_status_flags;                    // Use status flag based gen info for sample splitting?
	bool do_ss;                            		    // Tweaking some things for the paper
	string datacard;             									// Channel, e.g. et
	vector<string> set_alias;											// A string like alias1:value1,alias2:value2 etc
	string sm_masses_str;													
	string mssm_masses_str;												
	string syst_tau_scale;
	string syst_met_scale;
	string syst_eff_b;
	string syst_fake_b;
	string syst_scale_j;
	string syst_qcd_shape;
	string syst_ggh_pt;
	string syst_tquark;
	string syst_w_fake_rate;
	string syst_eff_t;
	string syst_zl_shift;
  string syst_fakes_os_ss_shape;
	string add_sm_background;
	double sub_ztt_top_frac;
	bool sub_ztt_top_shape;
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
  bool interpolate;
  string signal_bins;
  bool add_ztt_modes;

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
	  ("verbosity",               po::value<unsigned>(&verbosity)->default_value(0))
    ("use_status_flags",        po::value<unsigned>(&use_status_flags)->default_value(1))
	  ("do_ss", 	                po::value<bool>(&do_ss)->default_value(false))
	  ("interpolate", 	          po::value<bool>(&interpolate)->default_value(false))
	  ("datacard",                po::value<string>(&datacard)->default_value(""))
	  ("set_alias",               po::value<vector<string>>(&set_alias)->composing())
	  ("sm_masses",               po::value<string>(&sm_masses_str)->default_value(""))
	  ("mssm_masses",             po::value<string>(&mssm_masses_str)->default_value(""))
	  ("syst_tau_scale",          po::value<string>(&syst_tau_scale)->default_value(""))
	  ("syst_met_scale",          po::value<string>(&syst_met_scale)->default_value(""))
	  ("syst_eff_b",      		    po::value<string>(&syst_eff_b)->default_value(""))
	  ("syst_eff_t",      		    po::value<string>(&syst_eff_t)->default_value(""))
	  ("syst_fake_b",      		    po::value<string>(&syst_fake_b)->default_value(""))
	  ("syst_scale_j",            po::value<string>(&syst_scale_j)->default_value(""))
	  ("syst_ggh_pt",    			    po::value<string>(&syst_ggh_pt)->default_value(""))
	  ("syst_tquark",    			    po::value<string>(&syst_tquark)->default_value(""))
	  ("syst_w_fake_rate",   	    po::value<string>(&syst_w_fake_rate)->default_value(""))
	  ("syst_zl_shift",    		    po::value<string>(&syst_zl_shift)->default_value(""))
	  ("add_sm_background",       po::value<string>(&add_sm_background)->default_value(""))
	  ("sub_ztt_top_frac",        po::value<double>(&sub_ztt_top_frac)->default_value(-1.0))
	  ("sub_ztt_top_shape",        po::value<bool>(&sub_ztt_top_shape)->default_value(false))
	  ("shift_tscale",    		    po::value<double>(&shift_tscale)->default_value(0.0))
	  ("fix_empty_bins",   		    po::value<string>(&fix_empty_bins)->default_value(""))
	  ("fix_empty_hists",   	    po::value<string>(&fix_empty_hists)->default_value(""))
	  ("inflate_errors",     	    po::value<string>(&inflate_errors)->default_value(""))
	  ("fix_negative_bins",  	    po::value<string>(&fix_negative_bins)->default_value(""))
	  ("add_extra_binning",       po::value<string>(&add_extra_binning)->default_value(""))
	  ("shift_backgrounds",       po::value<string>(&shift_backgrounds)->default_value(""))
	  ("w_binned",                po::value<string>(&w_binned)->default_value(""))
	  ("signal_bins",             po::value<string>(&signal_bins)->default_value(""))
	  ("auto_titles",   			    po::value<bool>(&auto_titles)->default_value(true))
	  ("check_ztt_top_frac",      po::value<bool>(&check_ztt_top_frac)->default_value(false))
	  ("add_ztt_modes",           po::value<bool>(&add_ztt_modes)->default_value(false))
	  ("scan_bins",               po::value<unsigned>(&scan_bins)->default_value(0))
	  ("qcd_os_ss_factor",  	    po::value<double>(&qcd_os_ss_factor)->default_value(1.06));


	HTTPlot plot;
	config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
	po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
	po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
	po::notify(vm);

	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << "HiggsTauTauPlot5" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << boost::format(param_fmt()) % "paramfile" 	% paramfile;
	std::cout << boost::format(param_fmt()) % "cfg" 				% cfg;
	std::cout << boost::format(param_fmt()) % "folder" 			% folder;
	std::cout << boost::format(param_fmt()) % "channel"     % channel_str;
	std::cout << boost::format(param_fmt()) % "variable" 		% var;
	std::cout << boost::format(param_fmt()) % "method" 		  % method;
	std::cout << boost::format(param_fmt()) % "category"    % cat;
	std::cout << boost::format(param_fmt()) % "datacard"    % datacard;
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
		plot.set_plot_name(reduced_var+"_"+datacard+"_"+channel_str+("_2015"));
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
		std::cout << "[HiggsTauTauPlot5] Setting alias: " << alias_part[0] << " --> \"" << alias_part[1] << "\"" << std::endl;
	}

	// ************************************************************************
	// Extract signal masses to load
	// ************************************************************************
	std::vector<std::string> sm_masses;
	if (sm_masses_str != "") boost::split(sm_masses, sm_masses_str, boost::is_any_of(","));
	std::vector<std::string> mssm_masses;
	if (mssm_masses_str != "") boost::split(mssm_masses, mssm_masses_str, boost::is_any_of(","));

	// ************************************************************************
	// Setup HTTRun2Analysis 
	// ************************************************************************
	HTTRun2Analysis ana(String2Channel(channel_str), "2015", use_status_flags, verbosity);
    ana.SetQCDRatio(qcd_os_ss_factor);
    if (do_ss) ana.SetQCDRatio(1.0);
	for (auto const& a : alias_vec) ana.SetAlias(a.first, a.second);
	ana.AddSMSignalSamples(sm_masses);
	if (add_sm_background != "") {
		ana.AddSMSignalSamples({add_sm_background});
	}
	ana.AddMSSMSignalSamples(mssm_masses);
    //if (is_2012 && (check_ztt_top_frac || sub_ztt_top_shape)) ana.AddSample("Embedded-TTJets_FullLeptMGDecays");
	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTRun2Analysis::HistValueMap hmap;

//	std::string sel = " "+ana.ResolveAlias("sel");
	std::string sel = "os && "+ana.ResolveAlias("sel");
	if (do_ss) {
		sel = "!os && "+ana.ResolveAlias("sel");
		ana.SetAlias("w_os", "!os");
		ana.SetAlias("w_sdb_os", "!os");
		ana.SetAlias("w_vbf_os", "!os");
		ana.SetAlias("w_shape_os", "!os");
	}
	cat = ana.ResolveAlias(cat);

  double signal_xs = interpolate ? -1.0 : 1.0;
  std::string morph_var = "m_sv(70,0,350)";
  std::string sig_var = var;
  if (signal_bins != "") sig_var = reduced_var+signal_bins;

	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");
  

   
 ana.FillSMSignal(hmap, sm_masses, sig_var, sel, cat, "wt", "", "", signal_xs);
	if (add_sm_background != "") {
		ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "");
	}
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "", 1.0);


	// ************************************************************************
	// Split ZTT into decay modes
	// ************************************************************************
  if (add_ztt_modes) {
	  hmap["ZTT-1P0PZ"] = ana.GenerateZTT(method, var, sel, cat+" && tau_decay_mode==0", "wt");
	  hmap["ZTT-1P1PZ"] = ana.GenerateZTT(method, var, sel, cat+" && tau_decay_mode==1", "wt");
	  hmap["ZTT-3P"] = ana.GenerateZTT(method, var, sel, cat+" && tau_decay_mode==10", "wt");
  }


	// ************************************************************************
	// ggH pT Reweighting
	// ************************************************************************
	if (syst_ggh_pt != "") {
		std::cout << "[HiggsTauTauPlot5] Adding ggH pT systematic..." << std::endl;
		for (auto m : sm_masses) {
			hmap["ggH"+m+"_"+syst_ggh_pt+"Up"] = ana.GenerateSignal("GluGluToHToTauTau_M-"+m, sig_var, sel, cat, "wt*wt_ggh_pt_up", 1.0);
			hmap["ggH"+m+"_"+syst_ggh_pt+"Down"] = ana.GenerateSignal("GluGluToHToTauTau_M-"+m, sig_var, sel, cat, "wt*wt_ggh_pt_down", 1.0);
		}
	}
	
	// ************************************************************************
	// Additional Binning
	// ************************************************************************
  string extra_binning_postfix;
  string extra_binning_range = "";
  std::vector<std::string> extra_binning;
	if (add_extra_binning != "") {
		boost::split(extra_binning, add_extra_binning, boost::is_any_of(":"));
		if (extra_binning.size() == 2) {
			std::cout << "-----------------------------------------------------------------------------------" << std::endl;
			std::cout << "[HiggsTauTauPlot5] Adding alternative binning \"" << extra_binning[0] 
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
	// top-quark pT Reweighting
	// ************************************************************************
	if (syst_tquark != "") {
		std::cout << "[HiggsTauTauPlot5] Adding top-quark weight systematic..." << std::endl;
    for (unsigned j = 0; j < vars.size(); ++j) {
		  hmap["ttbar"+vars_postfix[j]+"_"+syst_tquark+"Up"] = ana.GenerateTOP(method, vars[j], sel, cat, "wt*wt_tquark_up");
		  hmap["ttbar"+vars_postfix[j]+"_"+syst_tquark+"Down"] = ana.GenerateTOP(method, vars[j], sel, cat, "wt*wt_tquark_down");
    }
	}

  // ************************************************************************
	// W+jets fake-rate Reweighting
	// ************************************************************************
	if (syst_w_fake_rate != "") {
		std::cout << "[HiggsTauTauPlot5] Adding W+jets fake-rate systematic..." << std::endl;
    for (unsigned j = 0; j < vars.size(); ++j) {
		  hmap["W_"+syst_w_fake_rate+"Up"+vars_postfix[j]] = ana.GenerateW(method, vars[j], sel, cat, "wt*wt_tau_fake_up");
		  hmap["W_"+syst_w_fake_rate+"Down"+vars_postfix[j]] = ana.GenerateW(method, vars[j], sel, cat, "wt*wt_tau_fake_down");
    }
	}

  // ************************************************************************
	// Tau ID Weights
	// ************************************************************************
/*	if (syst_eff_t != "") {
		std::cout << "[HiggsTauTauPlot5] Adding high tau pT ID systematic..." << std::endl;
    ana.FillSMSignal(hmap, sm_masses, sig_var, sel, cat, "wt*wt_tau_id_up", "", "_"+syst_eff_t+"Up", 1.0);
    ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt*wt_tau_id_up", "", "_"+syst_eff_t+"Up", 1.0);
    if (add_sm_background != "") {
			ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt*wt_tau_id_up", "_SM", "_"+syst_eff_t+"Up");
    }
    ana.FillSMSignal(hmap, sm_masses, sig_var, sel, cat, "wt*wt_tau_id_down", "", "_"+syst_eff_t+"Down", 1.0);
    ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt*wt_tau_id_down", "", "_"+syst_eff_t+"Down", 1.0);
    if (add_sm_background != "") {
			ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt*wt_tau_id_down", "_SM", "_"+syst_eff_t+"Down");
    }
 		hmap["ZTT_"+syst_eff_t+"Up"] = ana.GenerateZTT(method, var, sel, cat, "wt*wt_tau_id_up");
 		hmap["ZTT_"+syst_eff_t+"Down"] = ana.GenerateZTT(method, var, sel, cat, "wt*wt_tau_id_down");
	}*/
	
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
	if (syst_met_scale != "") {
		systematics.push_back(make_pair("/MET_DOWN", syst_met_scale+"Down"));
		systematics.push_back(make_pair("/MET_UP", syst_met_scale+"Up"));
	}

	for (auto const& syst : systematics) {
		std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		std::cout << "[HiggsTauTauPlot5] Doing systematic templates for \"" << syst.second << "\"..." << std::endl;
		HTTRun2Analysis ana_syst(String2Channel(channel_str), "2015", use_status_flags, verbosity);
        ana_syst.SetQCDRatio(qcd_os_ss_factor);
		for (auto const& a : alias_vec) ana_syst.SetAlias(a.first, a.second);
		ana_syst.AddSMSignalSamples(sm_masses);
		if (add_sm_background != "") {
			ana_syst.AddSMSignalSamples({add_sm_background});
		}
		ana_syst.AddMSSMSignalSamples(mssm_masses);
		ana_syst.ReadTrees(folder+syst.first, folder);
		ana_syst.ParseParamFile(paramfile);
		ana_syst.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst.second);
		ana_syst.FillSMSignal(hmap, sm_masses, sig_var, sel, cat, "wt", "", "_"+syst.second, signal_xs);
		if (add_sm_background != "") {
			ana_syst.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst.second);
		}
		ana_syst.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst.second, 1.0);
		if (extra_binning.size() == 2) {
			ana_syst.FillHistoMap(hmap, method, reduced_var+extra_binning[0], sel, cat, "wt", "_"+syst.second+extra_binning[1]);
			ana_syst.FillSMSignal(hmap, sm_masses, reduced_var+extra_binning[0], sel, cat, "wt", "", "_"+syst.second+extra_binning[1], 1.0);
			ana_syst.FillMSSMSignal(hmap, mssm_masses, reduced_var+extra_binning[0], sel, cat, "wt", "", "_"+syst.second+extra_binning[1], 1.0);
		}
  }

	// ************************************************************************
	// Reduce top yield to account for contamination in embedded
	// ************************************************************************
	if (sub_ztt_top_frac > 0.) {
		std::string top_label =  "TT";
		std::string ztt_label =  "ZTT";
		std::cout << "[HiggsTauTauPlot5] Subtracting " << top_label 
			<< " contamination in " << ztt_label << ": " << sub_ztt_top_frac << std::endl;
		HTTRun2Analysis::Value ztt_rate = hmap[ztt_label].second;
		HTTRun2Analysis::Value & top_rate = hmap[top_label].second;
		HTTRun2Analysis::Value contamination = HTTRun2Analysis::ValueProduct(ztt_rate, std::make_pair(sub_ztt_top_frac,0.));
		HTTRun2Analysis::PrintValue("Contamination", contamination);
		HTTRun2Analysis::PrintValue("New "+top_label+ " rate", top_rate);
		boost::regex top_regex(top_label+".*");
		for (auto & entry : hmap) {
			if (boost::regex_match(entry.first, top_regex)) {
				std::cout << "Correcting rate in " << entry.first << std::endl;
				entry.second.second = HTTRun2Analysis::ValueSubtract(entry.second.second, contamination);
				SetNorm(&(entry.second.first), entry.second.second.first);
			}
		}
	}

  // ************************************************************************
	// Check ttbar MC embedded
	// ************************************************************************
  if (check_ztt_top_frac) {
		std::cout << "-----------------------------------------------------------------------------------" << std::endl;
		std::cout << "[HiggsTauTauPlot5] Checking TOP contamination in ZTT embedded..." << std::endl;
    //HTTRun2Analysis::Value embedded_ttbar = ana.GetLumiScaledRate("RecHit-TTJets_FullLeptMGDecays", sel, cat, "wt");
    //HTTRun2Analysis::Value embedded_ttbar_inc = ana.GetLumiScaledRate("RecHit-TTJets_FullLeptMGDecays", "os", "", "wt");
    HTTRun2Analysis::Value embedded_ttbar = ana.GetLumiScaledRate("Embedded-TTJets_FullLeptMGDecays", sel, cat, "wt");
    HTTRun2Analysis::Value embedded_ttbar_inc = ana.GetLumiScaledRate("Embedded-TTJets_FullLeptMGDecays", "os", "", "wt");
    HTTRun2Analysis::Value embedded_data = ana.GetRate("Embedded", sel, cat, "wt");
    HTTRun2Analysis::Value embedded_data_inc = ana.GetRate("Embedded", "os", "", "wt");
    HTTRun2Analysis::PrintValue("EmbeddedTop", embedded_ttbar);
    HTTRun2Analysis::PrintValue("EmbeddedData", embedded_data);
    double uncorr_eff = embedded_data.first / embedded_data_inc.first;
    double corr_eff = (embedded_data.first - embedded_ttbar.first) / (embedded_data_inc.first - embedded_ttbar_inc.first);
    std::cout << "Uncorrected Eff:  " << uncorr_eff << std::endl;
    std::cout << "Corrected Eff:    " << corr_eff << std::endl;
    std::cout << "Ratio:            " << corr_eff/uncorr_eff << std::endl;
		std::string top_label = "TT";
		std::string ztt_label = "ZTT";
    HTTRun2Analysis::Value scaled_embedded = hmap[ztt_label].second;
    double norm_sf = scaled_embedded.first / embedded_data.first;
    double embedded_ttbar_norm = embedded_ttbar.first * norm_sf;
    //TH1F embedded_ttbar_shape = ana.GetLumiScaledShape(var,"RecHit-TTJets_FullLeptMGDecays", sel, cat, "wt");
    TH1F embedded_ttbar_shape = ana.GetLumiScaledShape(var,"Embedded-TTJets_FullLeptMGDecays", sel, cat, "wt");
    SetNorm(&embedded_ttbar_shape, embedded_ttbar_norm);
    hmap[top_label+"Embedded"] = make_pair(embedded_ttbar_shape, make_pair(embedded_ttbar_norm, 0.));
  }
  
  // ************************************************************************
	// Subtract ttbar MC embedded from ZTT
	// ************************************************************************
  if (sub_ztt_top_shape) {
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << "[HiggsHTohhPlot] Subtracting TOP contamination from ZTT embedded using TT embedded shape..." << std::endl;	
    std::string top_label = "TT";
	std::string ztt_label = "ZTT";
    
    HTTRun2Analysis::Value embedded_ttbar = ana.GetLumiScaledRate("Embedded-TTJets_FullLeptMGDecays", sel, cat, "wt");
    TH1F embedded_ttbar_shape = ana.GetLumiScaledShape(var,"Embedded-TTJets_FullLeptMGDecays", sel, cat, "wt");
    TH1F ztt_hist = hmap[ztt_label].first;
    HTTRun2Analysis::PrintValue("ztt_label.second", hmap[ztt_label].second);
    HTTRun2Analysis::Value embedded_data = ana.GetRate("Embedded", sel, cat, "wt");
    HTTRun2Analysis::Value scaled_embedded = hmap[ztt_label].second;
    double norm_sf = scaled_embedded.first / embedded_data.first;
    double embedded_ttbar_norm = embedded_ttbar.first * norm_sf;
    double embedded_ttbar_uncert = embedded_ttbar.second * norm_sf;
    SetNorm(&embedded_ttbar_shape, embedded_ttbar_norm);
    hmap[top_label+"_emb"] = make_pair(embedded_ttbar_shape, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
    ztt_hist.Add(&hmap[top_label+"_emb"].first, -1);
    HTTRun2Analysis::Value ztt_norm = HTTRun2Analysis::ValueSubtract(hmap[ztt_label].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
    hmap[ztt_label].first = ztt_hist;
    hmap[ztt_label] = make_pair(ztt_hist, ztt_norm);
    HTTRun2Analysis::PrintValue("ztt_label.second", hmap[ztt_label].second);
    
    //If systematics are included, make the same subtraction from the ZTT ones
    if(syst_tau_scale!="") {
        TH1F ztt_hist_Up = hmap[ztt_label+"_"+syst_tau_scale+"Up"].first;
        ztt_hist_Up.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Up = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_tau_scale+"Up"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_tau_scale+"Up"].first = ztt_hist_Up;
        hmap[ztt_label+"_"+syst_tau_scale+"Up"] = make_pair(ztt_hist_Up, ztt_norm_Up);
        TH1F ztt_hist_Down = hmap[ztt_label+"_"+syst_tau_scale+"Down"].first;
        ztt_hist_Down.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Down = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_tau_scale+"Down"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_tau_scale+"Down"].first = ztt_hist_Down;
        hmap[ztt_label+"_"+syst_tau_scale+"Down"] = make_pair(ztt_hist_Down, ztt_norm_Down);
    }
    if(syst_scale_j!="") {
        TH1F ztt_hist_Up = hmap[ztt_label+"_"+syst_scale_j+"Up"].first;
        ztt_hist_Up.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Up = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_scale_j+"Up"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_scale_j+"Up"].first = ztt_hist_Up;
        hmap[ztt_label+"_"+syst_scale_j+"Up"] = make_pair(ztt_hist_Up, ztt_norm_Up);
        TH1F ztt_hist_Down = hmap[ztt_label+"_"+syst_scale_j+"Down"].first;
        ztt_hist_Down.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Down = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_scale_j+"Down"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_scale_j+"Down"].first = ztt_hist_Down;
        hmap[ztt_label+"_"+syst_scale_j+"Down"] = make_pair(ztt_hist_Down, ztt_norm_Down);
    }
    if(syst_met_scale!="") {
        TH1F ztt_hist_Up = hmap[ztt_label+"_"+syst_met_scale+"Up"].first;
        ztt_hist_Up.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Up = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_met_scale+"Up"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_met_scale+"Up"].first = ztt_hist_Up;
        hmap[ztt_label+"_"+syst_met_scale+"Up"] = make_pair(ztt_hist_Up, ztt_norm_Up);
        TH1F ztt_hist_Down = hmap[ztt_label+"_"+syst_met_scale+"Down"].first;
        ztt_hist_Down.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Down = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_met_scale+"Down"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_met_scale+"Down"].first = ztt_hist_Down;
        hmap[ztt_label+"_"+syst_met_scale+"Down"] = make_pair(ztt_hist_Down, ztt_norm_Down);
    }
    if(syst_eff_b!="") {
        TH1F ztt_hist_Up = hmap[ztt_label+"_"+syst_eff_b+"Up"].first;
        ztt_hist_Up.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Up = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_eff_b+"Up"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_eff_b+"Up"].first = ztt_hist_Up;
        hmap[ztt_label+"_"+syst_eff_b+"Up"] = make_pair(ztt_hist_Up, ztt_norm_Up);
        TH1F ztt_hist_Down = hmap[ztt_label+"_"+syst_eff_b+"Down"].first;
        ztt_hist_Down.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Down = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_eff_b+"Down"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_eff_b+"Down"].first = ztt_hist_Down;
        hmap[ztt_label+"_"+syst_eff_b+"Down"] = make_pair(ztt_hist_Down, ztt_norm_Down);
    }
    if(syst_fake_b!="") {
        TH1F ztt_hist_Up = hmap[ztt_label+"_"+syst_fake_b+"Up"].first;
        ztt_hist_Up.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Up = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_fake_b+"Up"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_fake_b+"Up"].first = ztt_hist_Up;
        hmap[ztt_label+"_"+syst_fake_b+"Up"] = make_pair(ztt_hist_Up, ztt_norm_Up);
        TH1F ztt_hist_Down = hmap[ztt_label+"_"+syst_fake_b+"Down"].first;
        ztt_hist_Down.Add(&hmap[top_label+"_emb"].first, -1);
        HTTRun2Analysis::Value ztt_norm_Down = HTTRun2Analysis::ValueSubtract(hmap[ztt_label+"_"+syst_fake_b+"Down"].second, make_pair(embedded_ttbar_norm, embedded_ttbar_uncert));
        hmap[ztt_label+"_"+syst_fake_b+"Down"].first = ztt_hist_Down;
        hmap[ztt_label+"_"+syst_fake_b+"Down"] = make_pair(ztt_hist_Down, ztt_norm_Down);
    }   
  }


	// ************************************************************************
	// Apply special e-mu OS/SS fakes correction 
	// ************************************************************************
	if (syst_fakes_os_ss_shape != "") {
		  std::string qcd_label =  "QCD";
      for (unsigned j = 0; j < vars.size(); ++j) {
        TH1F weights = ic::GetFromTFile<TH1F>("input/scale_factors/OS_SS_weights.root","/","OSoverSS_msv_nobtag_ratio");
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
/*	if (syst_zl_shift != "") {
		vector<string> sub_strings;
		boost::split(sub_strings, syst_zl_shift, boost::is_any_of(":"));
		if (sub_strings.size() == 3) {
			string syst_zl_name 			= sub_strings[0];
			string syst_zl_shift_up 	= sub_strings[1];
			string syst_zl_shift_down = sub_strings[2];
			std::cout << "[HiggsTauTauPlot5] Adding ZL mass shift shape systematic..." << std::endl;
			std::cout << boost::format(param_fmt()) % "name" % syst_zl_name;
			std::cout << boost::format(param_fmt()) % "shift_up" % syst_zl_shift_up;
			std::cout << boost::format(param_fmt()) % "shift_down" % syst_zl_shift_down;
			hmap["ZL_"+syst_zl_name+"Up"] = ana.GenerateZL(method, syst_zl_shift_up+"*"+var, sel, cat, "wt");
			hmap["ZL_"+syst_zl_name+"Down"] = ana.GenerateZL(method, syst_zl_shift_down+"*"+var, sel, cat, "wt");
		}
	}*/

	// ************************************************************************
	// Fix Empty Bins
	// ************************************************************************
	vector<string> emptybins_regex;
	boost::split(emptybins_regex, fix_empty_bins, boost::is_any_of(","));
	if (emptybins_regex.size() > 0) {
		std::cout << "[HiggsTauTauPlot5] Running FixEmptyBins with patterns: " << fix_empty_bins << std::endl;
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
		std::cout << "[HiggsTauTauPlot5] Running FixEmptyHist with patterns: " << fix_empty_hists << std::endl;
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
    if (channel_str == "em") bkgs = {"ZTT","QCD","VV","TT"}; 
    if (channel_str == "tt") bkgs = {"QCD","W","VV","TT"};
    vector<string> sm_procs = {"ggH","qqH","VH"};
    vector<string> mssm_procs = {"ggH","bbH"};
    TH1F const& data = hmap["data_obs"].first;
    for (int i = 1; i <= data.GetNbinsX(); ++i) {
      double bkg_tot = 0.;
      double data_bin = data.GetBinContent(i);
      bool has_signal = false;
      for (auto bkg : bkgs) bkg_tot += hmap[bkg].first.GetBinContent(i);
      bool has_data_zero_bkg = false;
      if (data_bin > 0. && bkg_tot <= 0.) {
        if (bkg_tot == 0.) has_data_zero_bkg = true;
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
        // Two conditions to zero out a bin:
        //  1) Bkg content is -ve or zero and there is signal and data
        //  2) Bkg content is zero and there is data
        if ((has_signal || has_data_zero_bkg) && scan_bins>1) {
          std::cout << "\e[32mWarning: This bin will be set to zero in all templates\e[m" << std::endl;
          for (auto & entry : hmap) {
            entry.second.first.SetBinContent(i, 0.);
            entry.second.first.SetBinError(i, 0.);
          }
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
		std::cout << "[HiggsTauTauPlot5] Running FixNegativeBins with patterns: " << fix_negative_bins << std::endl;
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
		HTTRun2Analysis::PrintValue("ggHWW125", hmap["ggHWW125"].second);
		HTTRun2Analysis::PrintValue("qqHWW125", hmap["qqHWW125"].second);
	}
	*/

	// ************************************************************************
	// Write datacard
	// ************************************************************************
	if (datacard != "") {
		std::string dc_mode_label;
    if (channel_str == "et") 			dc_mode_label = "eleTau";
    if (channel_str == "mt") 			dc_mode_label = "muTau";
    if (channel_str == "tt")      dc_mode_label = "tauTau";
    if (channel_str == "em") 			dc_mode_label = "emu";
		std::string tfile_name = "datacard_"+reduced_var+"_"+datacard+"_"+channel_str+("_2015")+".root";
		TFile dc_file(tfile_name.c_str(),"RECREATE");
		dc_file.cd();
		gDirectory->mkdir((dc_mode_label+"_"+datacard).c_str());
		gDirectory->cd((dc_mode_label+"_"+datacard).c_str());

		for (auto iter : hmap) {
			iter.second.first.SetTitle(iter.first.c_str());
			iter.second.first.SetName(iter.first.c_str());
			iter.second.first.Write();
		}
		std::cout << "[HiggsTauTauPlot5] Writing datacard input " << tfile_name << std::endl;
		dc_file.Close();
	}

	// ************************************************************************
	// Replace signal histograms using the correct cross sections, for plotting
	// ************************************************************************
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "");
		for (auto m : sm_masses) {
     HTTAnalysis::PrintValue("ggH"+m, hmap["ggH"+m].second);
		 //HTTAnalysis::PrintValue("qqH"+m, hmap["qqH"+m].second);
		 //HTTAnalysis::PrintValue("VH"+m, hmap["VH"+m].second);
    }

	
    // ************************************************************************
	// Shift backgrounds
	// ************************************************************************
	vector<string> shift_strs;
	boost::split(shift_strs, shift_backgrounds, boost::is_any_of(","));
	if (shift_strs.size() > 0) {
		std::cout << "[HiggsTauTauPlot5] Shifting background yields... " << std::endl;
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
		std::string ztt_label = "ZTT";
		std::cout << "[HiggsTauTauPlot5] Shifting energy scale by pull: " << shift_tscale << std::endl;
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
		double pb_lumi = ana.GetLumi();
		string com = "13";
        plot.set_lumi_label((boost::format("%.1f pb^{-1} at %s TeV") % pb_lumi % com).str());
        plot.set_cms_label("CMS");
        plot.set_cms_extra("Preliminary");
        std::string channel_fmt = ""; 
		//if (channel_str == "et") 		plot.set_title_right("e#tau_{h}");
		//if (channel_str == "mt") 		plot.set_title_right("#mu#tau_{h}");
		//if (channel_str == "mtmet") plot.set_title_right("#mu_{soft}#tau_{h}");
		//if (channel_str == "em") 		plot.set_title_right("e#mu");
		if (channel_str == "et") 		channel_fmt = "e#tau_{h}";
		if (channel_str == "mt") 		channel_fmt = "#mu#tau_{h}";
		if (channel_str == "mtmet") channel_fmt = "#mu_{soft}#tau_{h}";
		if (channel_str == "em") 		channel_fmt = "e#mu";
    ic::TextElement text(channel_fmt,0.05,0.16,0.96);

    //ic::TextElement text2("#splitline{Same-sign}{region}",0.05,0.65,0.5);
    plot.AddTextElement(text);
    //plot.AddTextElement(text2);
	}

   
   /*if(n_vtx){
    TH1F * data = &hmap["data_obs"].first;
    TH1F *data_test = (TH1F*)data->Clone("data_test");
    TH1F * nvtx_weights = new TH1F("nvtx_weights","nvtx_weights",101,-0.5,100.5);
    data_test->Scale(1./data_test->Integral(1,data_test->GetNbinsX()));
    std::cout<<data_test->Integral()<<std::endl;
    vector<string>bkgs = {"ZTT","ZL","ZJ","W","TT","VV"};
    if (channel_str == "em") bkgs = {"ZTT","VV","TT"}; 
    if (channel_str == "tt") bkgs = {"W","VV","TT"};

    double bckg_yield=0;
    for (int i = 1; i<= data_test->GetNbinsX(); ++i){
      for (auto bkg : bkgs) bckg_yield += hmap[bkg].first.GetBinContent(i);
    }
    std::cout <<bckg_yield<<std::endl;
    for (int i = 1; i <= data_test->GetNbinsX(); ++i){
      double bkg_tot = 0;
      for (auto bkg : bkgs) bkg_tot += hmap[bkg].first.GetBinContent(i);
      bkg_tot = (double)bkg_tot/bckg_yield;
    if(bkg_tot !=0){
      nvtx_weights->SetBinContent(nvtx_weights->FindBin(data_test->GetBinCenter(i)),(data_test->GetBinContent(i)/bkg_tot)); 
   }

   }
   std::string weightfilename = "VertexWeightDistribution_"+channel_str+".root";
   TFile *fileout = new TFile(weightfilename.c_str(),"RECREATE");
     nvtx_weights->Write();
   fileout->Close();
  }*/

   //data->Divide(bkg_hist);

	plot.GeneratePlot(hmap);

  return 0;
}

