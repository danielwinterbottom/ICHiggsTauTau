#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

string Token(string const& file, string const& category, string const& selection) {
	return (file+"_"+category+"_"+selection);
}


double Integral(TH1F const* hist) {
  return hist->Integral(0, hist->GetNbinsX() + 1);
}

double Error(TH1F const* hist) {
  double err = 0.0;
  hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
  return err;
}

void Smooth(TH1F* h) {
  double yield =  h->Integral();
  for(int i = 0; i < h->GetNbinsX()+1; i++) h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
  h->Smooth();
  for(int i = 0; i < h->GetNbinsX()+1; i++) h->SetBinContent(i,h->GetBinContent(i)*h->GetBinWidth(i));
  h->Scale(yield/h->Integral()); 
}

void FixEmptyBins(TH1F *hist, bool is_qcd, bool verbose) {
  if (verbose) cout << "Running FixEmptyBins with QCD Mode: " << is_qcd << endl;
  unsigned bins = hist->GetNbinsX();
  unsigned first_populated = 0;
  unsigned last_populated = 0;
  for (unsigned i = 1; i <= bins; ++i) {
    if (hist->GetBinContent(i) > 0. && first_populated == 0) first_populated = i;
    if (hist->GetBinContent(bins-(i-1)) > 0. && last_populated == 0) last_populated = bins-(i-1);
  }
  if (last_populated <= first_populated) {
    if (verbose) std::cout << "Error: Cannot correct this distribution!" << std::endl;
    return;
  }
  if (verbose) std::cout << "First populated bin: " << first_populated << std::endl;
  if (verbose) std::cout << "Last populated bin: " << last_populated << std::endl;
  double av_weight = ( hist->Integral() / double(hist->GetEntries()));
  if (!is_qcd) {
    if (verbose) std::cout << "Integral: " << hist->Integral() << std::endl;
    if (verbose) std::cout << "Entries: " << hist->GetEntries() << std::endl;
    if (verbose) std::cout << "Av. Weight: " << av_weight << std::endl;
  }
  for (unsigned i = first_populated+1; i < last_populated; ++i) {
    if (hist->GetBinContent(i) == 0.) {
      if (verbose) std::cout << "Bin " << i << " is empty!" << std::endl;
      if (is_qcd) {
        hist->SetBinError(i, 1.0);
      } else {
        if (verbose) std::cout << "Set weight to 1.0 * av_weight = " << (1.0 * av_weight) << std::endl;
        hist->SetBinError(i, av_weight);    
      }
    }
  }
}

double WSideband(map<string, ic::TH1PlotElement> & plots, vector<string> vv_samples, double embed_norm, string const& cat, string const& sel, bool verbose) {
	double yield = Integral(plots[Token("Data",cat,sel)].hist_ptr());
	yield -= Integral(plots[Token("TTJets",cat,sel)].hist_ptr());
	double vv_os_tot = 0.0;
	for (unsigned i = 0; i < vv_samples.size(); ++i) {
	  yield -= Integral(plots[Token(vv_samples[i],cat,sel)].hist_ptr());      
	  vv_os_tot += Integral(plots[Token(vv_samples[i],cat,sel)].hist_ptr());
	}
	yield -= Integral(plots[Token("DYJetsToLL",cat,sel)].hist_ptr());
	yield -= Integral(plots[Token("DYJetsToTauTau",cat,sel)].hist_ptr());
	if (verbose) std::cout << "Control Data: " << 							Integral(plots[Token("Data",cat,sel)].hist_ptr()) << std::endl;
	if (verbose) std::cout << "Control TTBar: " << 							Integral(plots[Token("TTJets",cat,sel)].hist_ptr()) << std::endl;
	if (verbose) std::cout << "Control VV: " << 								vv_os_tot << std::endl;
	if (verbose) std::cout << "Control DYJetsToLL: " << 				Integral(plots[Token("DYJetsToLL",cat,sel)].hist_ptr()) << std::endl;
	if (verbose) std::cout << "Control DYJetsToTauTau: " << 		Integral(plots[Token("DYJetsToTauTau",cat,sel)].hist_ptr()) << std::endl;
	if (verbose) std::cout << "[Control Embedded]: " << 				Integral(plots[Token("Embedded",cat,sel)].hist_ptr())*embed_norm << std::endl;
	if (verbose) std::cout << "=> Control W: " << yield << std::endl;
	return yield;
};

void CleanBinsUpTo(TH1F *hist, double max) {
  unsigned bins = hist->GetNbinsX();
  for (unsigned i = 1; i <= bins; ++i) {
    if (hist->GetBinCenter(i) < max) {
      hist->SetBinContent(i, 0.0);
      hist->SetBinError(i, 0.0);
    }
  }
}

void SetStyle(ic::TH1PlotElement & ele, unsigned color) {
  //ele.set_marker_color(color);
  //ele.set_line_color(color);
  ele.set_fill_color(color);
  ele.set_fill_style(1001);
  ele.set_draw_fill(true);
  ele.set_draw_marker(false);
  ele.set_draw_line(false);
  ele.set_line_width(3);
  ele.set_draw_stat_error_y(false);
  ele.set_in_stack(true);
  return;
}

void SetDataStyle(ic::TH1PlotElement & ele) {
  ele.set_marker_color(1);
  ele.set_line_color(1);
  ele.set_fill_color(1);
  ele.set_fill_style(0);
  ele.set_draw_fill(false);
  ele.set_line_width(3);
  ele.set_draw_marker(true);
  ele.set_draw_line(true);
  ele.set_marker_style(20);
  ele.set_draw_stat_error_y(true);
  ele.set_marker_size(1.3);
  return;
}

void SetStyle(ic::RatioPlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  ele.set_marker_size(1.3);
  ele.set_marker_style(20);
  return;
}


int main(int argc, char* argv[]){

  

	// Configurable parameters
  string cfg;															      // The configuration file
  string paramfile;															// The paramters files									

  string plot_name;															// Name of the plot to draw
  string folder;																// Folder containing input files
  string channel_str;             							// Channel, e.g. et
  unsigned method;															// Use background methods for chosen category
   // 0 0jet_low  	// 1 0jet_high
   // 2 boost_low 	// 3 boost_high
   // 4 vh 					// 5 vbf
   // 6 btag_low		// 7 btag_high
   // 8 inclusive 	// 9 dilepton
   // 10 twojet
   // 11 nobtag 		// 12 btag
  string category;															// Use background methods for chosen category
  bool is_2012;																	// false = 7 TeV, true = 8 TeV
  unsigned mssm_mode;														// 0 = SM, 1 = MSSM
  bool no_plot;																	// Don't actually generate image files if true
  bool show_category;                           // Print the category name on the plot title
  bool verbose;																	// Verbose output, useful for diagnostic purposes
  bool non_mass_plot;														// In some places use alternative background estimate if true
  bool draw_ss;																	// Draw the same-sign plot instead of opposite-sign
  bool use_ztt_mc;															// Use DYJetsToTauTau for shape instead of Embedded
  bool add_sm_signal_as_bg;										  // Add the 125 GeV SM signal shapes
  bool is_paper;                                // Tweaking some things for the paper

  // Plotting options
  string x_axis_label;													// Label for the X-axis
  string x_axis_bin_labels;                     // of the form "a:b:c"
  unsigned rebin;																// Rebin factor
  bool custom_x_axis_range;											// Can optionally specify an x-axis range
  double x_axis_min;														// If custom_x_axis_range is true, use this as min
  double x_axis_max;														// If custom_x_axis_range is true, use this as max
  double y_axis_min;														// Force a Y-axis min
  double extra_pad;															// Expand the y-axis by an additional factor
  bool blind;																		// Blind some region of the data
  double x_blind_min;														// If bind is true, use this as min x for blinding
  double x_blind_max;														// If bind is true, use this as min x for blinding
  bool log_y;																		// Draw plot in log scale
  bool norm_bins;																// Normalise using bin width
  bool signal_no_stack;													// Don't stack the signal contributions on the backgrounds
  bool signal_split_vbf;												// Split the signal vbf into a separate entry
  bool draw_ratio;                               // Draw a ratio box
 	// Datacard options
  bool make_datacard;														// Generate a datacard from this plot
  bool swap_inclusive;													// For datacard creation: swap labels of inclusive with 0jet_low
  unsigned tau_scale_mode = 0;									// The tau scale mode, used to append labels for plots
  bool official_style;                          // Adopt official plotting style
  bool draw_signal;                             // Draw signal in the plots
  bool ztt_by_decay_mode;                       // For plotting only, split ZTT by decay mode
  string add_text;


  // Options to manually shift backgrounds and draw uncertainty bands
  bool shift_backgrounds = false;
  bool shift_tscale = false;
  double tscale_shift = 0.0;
  double qcd_shift = 1.0;
  double top_shift = 1.0;
  double ztt_shift = 1.0;
  double zl_shift = 1.0;
  double zj_shift = 1.0;
  double w_shift = 1.0;
  double vv_shift = 1.0;
  bool draw_band_on_stack = false;
  double band_size_fractional = 0.0;
  string replace_os_sel;
  string replace_ss_sel;

  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    ("paramfile",           po::value<string>(&paramfile)->required())
    ("plot_name",           po::value<string>(&plot_name)->required())
    ("folder",              po::value<string>(&folder)->required())
    ("channel",             po::value<string>(&channel_str)->required())
    ("replace_os_sel",      po::value<string>(&replace_os_sel)->default_value(""))
    ("replace_ss_sel",      po::value<string>(&replace_ss_sel)->default_value(""))
    ("method",           		po::value<unsigned>(&method)->required())
    ("category",            po::value<string>(&category)->default_value(""))
    ("add_text",            po::value<string>(&add_text)->default_value(""))
    ("is_2012",             po::value<bool>(&is_2012)->required())
    ("draw_ratio",          po::value<bool>(&draw_ratio)->default_value(false))
    ("mssm_mode",           po::value<unsigned>(&mssm_mode)->required())
    ("no_plot",             po::value<bool>(&no_plot)->default_value(false))
    ("show_category",       po::value<bool>(&show_category)->default_value(true))
    ("verbose",             po::value<bool>(&verbose)->default_value(false))
    ("non_mass_plot",       po::value<bool>(&non_mass_plot)->default_value(false))
    ("draw_ss",             po::value<bool>(&draw_ss)->default_value(false))
    ("use_ztt_mc",          po::value<bool>(&use_ztt_mc)->default_value(false))
    ("x_axis_label",        po::value<string>(&x_axis_label)->required())
    ("x_axis_bin_labels",   po::value<string>(&x_axis_bin_labels)->default_value(""))
    ("rebin",               po::value<unsigned>(&rebin)->default_value(1))
    ("custom_x_axis_range", po::value<bool>(&custom_x_axis_range)->default_value(false))
    ("x_axis_min",          po::value<double>(&x_axis_min)->default_value(0))
    ("x_axis_max",          po::value<double>(&x_axis_max)->default_value(0))
    ("y_axis_min",          po::value<double>(&y_axis_min)->default_value(-10))
    ("extra_pad",           po::value<double>(&extra_pad)->default_value(1.0))
    ("blind",               po::value<bool>(&blind)->default_value(false))
    ("x_blind_min",         po::value<double>(&x_blind_min)->default_value(0))
    ("x_blind_max",         po::value<double>(&x_blind_max)->default_value(0))
    ("log_y",               po::value<bool>(&log_y)->default_value(false))
    ("norm_bins",           po::value<bool>(&norm_bins)->default_value(false))
    ("signal_no_stack",     po::value<bool>(&signal_no_stack)->default_value(false))
    ("signal_split_vbf",    po::value<bool>(&signal_split_vbf)->default_value(false))
    ("make_datacard",       po::value<bool>(&make_datacard)->default_value(false))
    ("swap_inclusive",      po::value<bool>(&swap_inclusive)->default_value(false))
    ("ztt_by_decay_mode",   po::value<bool>(&ztt_by_decay_mode)->default_value(false))
    ("add_sm_signal_as_bg", po::value<bool>(&add_sm_signal_as_bg)->default_value(false))
    ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0))
    ("shift_backgrounds",   po::value<bool>(&shift_backgrounds)->default_value(false))
    ("shift_tscale",        po::value<bool>(&shift_tscale)->default_value(false))
    ("official_style",      po::value<bool>(&official_style)->default_value(false))
    ("draw_signal",         po::value<bool>(&draw_signal)->default_value(true))
    ("is_paper",            po::value<bool>(&is_paper)->default_value(false))
    ("draw_band_on_stack",  po::value<bool>(&draw_band_on_stack)->default_value(false))
    ("tscale_shift",        po::value<double>(&tscale_shift)->default_value(0.0))
    ("qcd_shift",           po::value<double>(&qcd_shift)->default_value(1.0))
    ("top_shift",           po::value<double>(&top_shift)->default_value(1.0))
    ("ztt_shift",           po::value<double>(&ztt_shift)->default_value(1.0))
    ("zl_shift",            po::value<double>(&zl_shift)->default_value(1.0))
    ("zj_shift",            po::value<double>(&zj_shift)->default_value(1.0))
    ("w_shift",             po::value<double>(&w_shift)->default_value(1.0))
    ("vv_shift",            po::value<double>(&vv_shift)->default_value(1.0))
    ("band_size_fractional", po::value<double>(&band_size_fractional)->default_value(0.0));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  ic::channel channel   = String2Channel(channel_str);
  if (tau_scale_mode == 1) folder += "/TSCALE_DOWN";
  if (tau_scale_mode == 2) folder += "/TSCALE_UP";
  std::string dc_mode_label, 			dc_cat_label, dc_mode_label_alt;
  if (channel == channel::et) 		dc_mode_label = "eleTau";
  if (channel == channel::mt) 		dc_mode_label = "muTau";
  if (channel == channel::mtmet) 	dc_mode_label = "muTauMet";
  if (channel == channel::em) 		dc_mode_label = "emu";
  if (channel == channel::et) 		dc_mode_label_alt = "etau";
  if (channel == channel::mt) 		dc_mode_label_alt = "mutau";
  if (channel == channel::mtmet) 	dc_mode_label_alt = "mutaumet";
  if (channel == channel::em) 		dc_mode_label_alt = "em";
  if (method == 0) dc_cat_label = "0jet_low";
  if (method == 1) dc_cat_label = "0jet_high";
  if (method == 2) dc_cat_label = "boost_low";
  if (method == 3) dc_cat_label = "boost_high";
  if (method == 4) dc_cat_label = "vh";
  if (method == 5) dc_cat_label = "vbf";
  if (method == 6) dc_cat_label = "btag_low";
  if (method == 7) dc_cat_label = "btag_high";
  if (method == 8) dc_cat_label = "inclusive";
  if (method == 9) dc_cat_label = "dilepton";
  if (method == 10) dc_cat_label = "twojet";
  if (method == 11) dc_cat_label = "nobtag";
  if (method == 12) dc_cat_label = "btag";
  if (category == "") {
    if (method == 0) category = "0jet_low";
    if (method == 1) category = "0jet_high";
    if (method == 2) category = "1jet_low";
    if (method == 3) category = "1jet_high";
    if (method == 4) category = "vh";
    if (method == 5) category = "vbf";
    if (method == 6) category = "btag_low";
    if (method == 7) category = "btag_high";
    if (method == 8) category = "inclusive";
    if (method == 9) category = "dilepton";
    if (method == 10) category = "twojet";
    if (method == 11) category = "nobtag";
    if (method == 12) category = "btag";
  }

  std::cout << "--------------------------------------------------" << std::endl;  
  std::cout << "HiggsTauTauPlot3" << std::endl;
  std::cout << "--------------------------------------------------" << std::endl;  
  if (swap_inclusive) {
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;  
    std::cout << "SWAPPING INCLUSIVE WITH 0jet_low" << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;  
  }
  if (use_ztt_mc) {
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;  
    std::cout << "Using ZTT MC" << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;  
  }

  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "paramfile" 	% paramfile;
  std::cout << boost::format(param_fmt) % "plot_name" 	% plot_name;
  std::cout << boost::format(param_fmt) % "folder" 			% folder;
  std::cout << boost::format(param_fmt) % "channel_str" % channel_str;
  std::cout << boost::format(param_fmt) % "method" 			% method;
  std::cout << boost::format(param_fmt) % "method(str)" % dc_cat_label;
  std::cout << boost::format(param_fmt) % "category" 		% category;
  std::cout << boost::format(param_fmt) % "is_2012" 		% is_2012;
  std::cout << boost::format(param_fmt) % "mssm_mode" 	% mssm_mode;
  std::cout << boost::format(param_fmt) % "no_plot" 		% no_plot;
  std::cout << boost::format(param_fmt) % "non_mass_plot" % non_mass_plot;
  std::cout << boost::format(param_fmt) % "draw_ss" 		% draw_ss;
  std::cout << boost::format(param_fmt) % "use_ztt_mc" 	% use_ztt_mc;

  // Parse the parameter file
  SimpleParamParser parser;
  if (verbose) std::cout << "** Parsing parameter file... **" << paramfile << std::endl;
  parser.ParseFile(paramfile);
  vector<string> signal_masses, mssm_signal_masses;
  std::string signal_masses_list = parser.GetParam<string>("SIGNAL_MASSES");
  std::cout << boost::format(param_fmt) % "sm_signal_masses" 	% signal_masses_list;
  std::string mssm_signal_masses_list = parser.GetParam<string>("MSSM_SIGNAL_MASSES");
  std::cout << boost::format(param_fmt) % "mssm_signal_masses" 	% mssm_signal_masses_list;
  boost::split(signal_masses, signal_masses_list, boost::is_any_of(","));
  boost::split(mssm_signal_masses, mssm_signal_masses_list, boost::is_any_of(","));
  std::string draw_signal_mass = parser.GetParam<string>("DRAW_SIGNAL_MASS");
  std::string draw_mssm_signal_mass = parser.GetParam<string>("DRAW_MSSM_SIGNAL_MASS");
  unsigned draw_signal_factor = parser.GetParam<unsigned>("DRAW_SIGNAL_FACTOR");
  unsigned draw_mssm_signal_factor = parser.GetParam<unsigned>("DRAW_MSSM_SIGNAL_FACTOR");
  string year_label = parser.GetParam<string>("YEAR_LABEL");
  double data_lumi = parser.GetParam<double>("LUMI_DATA");
  string lumi_data_label = parser.GetParam<string>("LUMI_DATA_LABEL");

  std::cout << "-------------------------" << std::endl;  


  // First, use channel to define the files to load
  vector<string> files;
  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    files.push_back("Data");
    files.push_back("Special_3_Data"); 
    files.push_back("DYJetsToTauTau"); 
    files.push_back("Embedded");
    files.push_back("DYJetsToLL");
    files.push_back("DYJetsToLL-L");
    files.push_back("DYJetsToLL-J");
    files.push_back("Special_18_DYJetsToLL-L");
    files.push_back("WJetsToLNuSoup");
    files.push_back("WWJetsTo2L2Nu");
    files.push_back("WZJetsTo2L2Q");
    files.push_back("WZJetsTo3LNu");
    files.push_back("ZZJetsTo2L2Nu");
    files.push_back("ZZJetsTo2L2Q");
    files.push_back("ZZJetsTo4L");
    files.push_back("T-tW");
    files.push_back("Tbar-tW");
    files.push_back("TTJets");
    if (is_2012) {
      files.push_back("TT");
      files.push_back("DYJetsToTauTauSoup"); 
      files.push_back("DYJetsToLLSoup");
      files.push_back("DYJetsToLL-LSoup");
      files.push_back("DYJetsToLL-JSoup");
    }
    if (shift_tscale) {
      files.push_back("TSCALE_UP/Embedded");
      files.push_back("TSCALE_DOWN/Embedded");
    }
  } else if (channel == channel::em) {
    files.push_back("Data");
    files.push_back("Special_20_Data"); 
    files.push_back("Special_21_Data"); 
    files.push_back("Special_22_Data"); 
    files.push_back("Special_23_Data"); 
    files.push_back("Special_24_Data"); 
    files.push_back("DYJetsToTauTau"); 
    files.push_back("DYJetsToTauTauSoup"); 
    files.push_back("Embedded");
    files.push_back("WWJetsTo2L2Nu");
    files.push_back("WZJetsTo2L2Q");
    files.push_back("WZJetsTo3LNu");
    files.push_back("ZZJetsTo2L2Nu");
    files.push_back("ZZJetsTo2L2Q");
    files.push_back("ZZJetsTo4L");
    files.push_back("T-tW");
    files.push_back("Tbar-tW");
    files.push_back("TTJets");
    if (is_2012) files.push_back("TT");
  }
  if (ztt_by_decay_mode) {
    files.push_back("Embedded1P0PZ"); 
    files.push_back("Embedded1P1PZ"); 
    files.push_back("Embedded3P");
  }
  if (mssm_mode == 0) {
    for (unsigned i = 0; i < signal_masses.size(); ++i) {
      files.push_back("GluGluToHToTauTau_M-"+signal_masses[i]);
      files.push_back("VBF_HToTauTau_M-"+signal_masses[i]);
      files.push_back("WH_ZH_TTH_HToTauTau_M-"+signal_masses[i]);
    }
  } else {
    for (unsigned i = 0; i < mssm_signal_masses.size(); ++i) {
      files.push_back("SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i]);
      files.push_back("SUSYBBHToTauTau_M-"+mssm_signal_masses[i]);
    }
  }
  if (add_sm_signal_as_bg) {
    files.push_back("GluGluToHToTauTau_M-125");
    files.push_back("VBF_HToTauTau_M-125");
    files.push_back("WH_ZH_TTH_HToTauTau_M-125");
  }

  // Now build a list of categories to load plots from
  vector<string> categories;
  categories.push_back(category);
  categories.push_back("inclusive");
  if (method == 5) {
    // *** We should be able to override this
  	categories.push_back("vbf_loose");
    categories.push_back("vbf_loose_jets20");
    categories.push_back("twojet");
    if (channel == channel::em) categories.push_back("vbf_no_cjv");
  }
  if (method == 6) categories.push_back("btag_low_loose");
  if (method == 7) categories.push_back("btag_high_loose");
  if (method == 12) categories.push_back("btag_loose");


  // Now build a list of selections
 string os_sel = "os_sel";
 if (replace_os_sel != "") os_sel = replace_os_sel;
 string ss_sel = "ss_sel";
 if (replace_ss_sel != "") ss_sel = replace_ss_sel;
 string os_con = "os_con";
 string ss_con = "ss_con";

  vector<string> selections;
  selections.push_back("os");
  selections.push_back("ss");
  selections.push_back(os_sel);
  selections.push_back(ss_sel);
  if (channel != channel::em) {
  	selections.push_back(os_con);
  	selections.push_back(ss_con);
  }
  if (method == 5) selections.push_back("os_con_mt_60-120");

  std::map<std::string, TFile *> tfiles;
  for (unsigned i = 0; i < files.size(); ++i) {
    std::string filename = (files[i]+"_"+channel_str+"_"+year_label+".root");
    TFile * tmp = new TFile((folder+"/"+filename).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
    } else {
      tfiles[files[i]] = tmp;      
    }
  }

  // Get Plots and Scale
  map<string, ic::TH1PlotElement> plots;
  for (unsigned i = 0; i < files.size(); ++i) {
  	for (unsigned j = 0; j < categories.size(); ++j) {
  		for (unsigned k = 0; k < selections.size(); ++k) {

  			string f = files[i];
  			string c = categories[j];
  			string s = selections[k];

  			std::string nm = Token(f, c, s);
  			if (plots.count(nm)) continue;
  			//if (verbose) std::cout << "Adding plot: " << nm << std::endl;

  			if (tfiles[f]->GetDirectory(("/"+c+"_"+s).c_str())) {
  				plots[nm] = ic::TH1PlotElement(nm, tfiles[f], "/"+c+"_"+s, plot_name);
  				if (plots[nm].hist_ptr()->GetSumw2N() == 0) {
            cout << "Warning: Plot " << nm << " does not have a weights structure" << endl;
            plots[nm].hist_ptr()->Sumw2();
          }
  				string lookup;
  				if (f.find("Special") != f.npos) {
  					size_t found = f.find('_');
  					found = f.find('_',found+1);
  					lookup = f.substr(found+1);
  				} else {
  					lookup = f;
  				}
          if (f.find("TSCALE_UP/") != f.npos || f.find("TSCALE_DOWN/") != f.npos) {
            size_t found = f.find('/');
            lookup = f.substr(found+1);
            std::cout << "Found scaled: " << lookup << "\t" << c << "\t" << s << std::endl;
          }
          if (f.find("Embedded") != f.npos) lookup = "Embedded";
  				double sample_events = parser.GetParam<double>("EVT_"+lookup);
  				double sample_xs = parser.GetParam<double>("XS_"+lookup);
  				double sample_lumi = sample_events / sample_xs;
  				if (sample_xs > 0) plots[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
  				plots[nm].hist_ptr()->Rebin(rebin);
  			}
  		}
  	}
  }
  string cat  	= category;


  string yield_fmt = "%-25s %-20.3f %-20.3f\n";

  // ---------------------------------------------------
  // DATA
  // ---------------------------------------------------
  TH1F *data_hist_clean = (TH1F*)(plots[Token("Data",cat,os_sel)].hist_ptr()->Clone());
  TH1F *data_hist = (TH1F*)(plots[Token("Data",cat,os_sel)].hist_ptr()->Clone());

  if (blind) {
    for (int j = 0; j < data_hist->GetNbinsX(); ++j) {
      double low_edge = data_hist->GetBinLowEdge(j+1);
      double high_edge = data_hist->GetBinWidth(j+1)+data_hist->GetBinLowEdge(j+1);
      if ((low_edge > x_blind_min && low_edge < x_blind_max) || (high_edge > x_blind_min && high_edge < x_blind_max)) {
        data_hist->SetBinContent(j+1,0);
        data_hist->SetBinError(j+1,0);
      }
    }
  }
  double data_norm = Integral(data_hist_clean);
  TH1F *data_hist_ss = (TH1F*)(plots[Token("Data",cat,ss_sel)].hist_ptr()->Clone());
  ic::TH1PlotElement data_shape("data_shape", data_hist);
  ic::TH1PlotElement data_shape_ss("data_shape_ss", data_hist_ss);
  double data_err = Error(data_hist_clean);
  std::cout << boost::format(yield_fmt) % "Data " % data_norm % data_err;

  // ---------------------------------------------------
  // Generate TOP Norm and TOP Shape
  // ---------------------------------------------------
  if (verbose) cout << "**** TOP: " << endl;

  // Norm is the same for all methods
  double top_norm = Integral(plots[Token("TTJets",cat,os_sel)].hist_ptr());
  double top_err = Error(plots[Token("TTJets",cat,os_sel)].hist_ptr());
  TH1F *top_hist = nullptr;
  string top_shape_str = is_2012 ? "TT" : "TTJets";
  if (channel == channel::em) top_shape_str = "TTJets";
  double top_norm_ss = Integral(plots[Token("TTJets",cat,ss_sel)].hist_ptr());
  TH1F *top_hist_ss = (TH1F*)(plots[Token(top_shape_str,cat,ss_sel)].hist_ptr()->Clone());

  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    if (method <= 4 || method >= 6) top_hist 	= (TH1F*)(plots[Token(top_shape_str, cat, os_sel)].hist_ptr()->Clone());
    if (method == 5) top_hist 								= (TH1F*)(plots[Token(top_shape_str, "vbf_loose", os_sel)].hist_ptr()->Clone());
	}
  if (channel == channel::em) {
    if (method <= 4 || method >= 6) top_hist  = (TH1F*)(plots[Token(top_shape_str, cat, os_sel)].hist_ptr()->Clone());
    if (method == 5) top_hist                 = (TH1F*)(plots[Token(top_shape_str, "vbf_no_cjv", os_sel)].hist_ptr()->Clone());

  }
  top_hist->Scale(top_norm / Integral(top_hist));
  ic::TH1PlotElement top_shape("top_shape", top_hist);
  top_hist_ss->Scale(top_norm_ss / Integral(top_hist_ss));
  ic::TH1PlotElement top_shape_ss("top_shape_ss", top_hist_ss);
  std::cout << boost::format(yield_fmt) % "Top" % top_norm % top_err;

  if (verbose) {
    std::cout << "** Top Err: " << Error(plots[Token("TTJets",cat,os_sel)].hist_ptr()) << std::endl;
    std::cout << "** Top Events: " << plots[Token("TTJets",cat,os_sel)].hist_ptr()->GetEntries() << std::endl;
  }

  // ---------------------------------------------------
  // Generate VV Norm and VV Shape
  // ---------------------------------------------------
  if (verbose) cout << "**** VV: " << endl;
  bool use_madgraph = parser.GetParam<bool>("VV_USE_MADGRAPH");
  vector<string> vv_samples;
  if (use_madgraph) {
    vv_samples.push_back("WWJetsTo2L2Nu");
    vv_samples.push_back("WZJetsTo2L2Q");
    vv_samples.push_back("WZJetsTo3LNu");
    vv_samples.push_back("ZZJetsTo2L2Nu");
    vv_samples.push_back("ZZJetsTo2L2Q");
    vv_samples.push_back("ZZJetsTo4L");
    vv_samples.push_back("T-tW");
    vv_samples.push_back("Tbar-tW");
  } else {
    vv_samples.push_back("WW");
    vv_samples.push_back("WZ");
    vv_samples.push_back("ZZ");
    vv_samples.push_back("T-tW");
    vv_samples.push_back("Tbar-tW");
  }


  // Norm is the same for all methods
  double vv_norm = 0.0;
  for (unsigned i = 0; i < vv_samples.size(); ++i) vv_norm += Integral(plots[Token(vv_samples[i],cat,os_sel)].hist_ptr());
  TH1F *vv_hist = nullptr;
  
  // Do SS
  double vv_norm_ss = 0.0;
  for (unsigned i = 0; i < vv_samples.size(); ++i) vv_norm_ss += Integral(plots[Token(vv_samples[i],cat,ss_sel)].hist_ptr());
  TH1F *vv_hist_ss =(TH1F*)(plots[Token(vv_samples[0],cat,ss_sel)].hist_ptr()->Clone());
  for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist_ss->Add(plots[Token(vv_samples[i],cat,ss_sel)].hist_ptr());  

  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    // For 0/1 jet and dilepton, inclusive and twojet take normal shape
    if (method <= 4 || method >= 6) {
      vv_hist = (TH1F*)(plots[Token(vv_samples[0],cat,os_sel)].hist_ptr()->Clone());
      for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(plots[Token(vv_samples[i],cat,os_sel)].hist_ptr());  
    }
    if (method == 5) {
      vv_hist = (TH1F*)(plots[Token(vv_samples[0],"vbf_loose",os_sel)].hist_ptr()->Clone());
      for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(plots[Token(vv_samples[i],"vbf_loose",os_sel)].hist_ptr());
    }
  }
  if (channel == channel::em) {
    if (method == 0 || method == 1) {
      if (verbose) cout << "VV Norm is scaled up by 1.23" << endl;
      vv_norm *= 1.23;
    }
    vv_hist = (TH1F*)(plots[Token(vv_samples[0],cat,os_sel)].hist_ptr()->Clone());
    for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(plots[Token(vv_samples[i],cat,os_sel)].hist_ptr());
  }

  vv_hist->Scale( vv_norm / Integral(vv_hist) );
  double vv_err = Error(vv_hist);
  ic::TH1PlotElement vv_shape("vv_shape", vv_hist);
  vv_hist_ss->Scale( vv_norm_ss / Integral(vv_hist_ss) );
  ic::TH1PlotElement vv_shape_ss("vv_shape_ss", vv_hist_ss);

  std::cout << boost::format(yield_fmt) % "VV" % vv_norm % vv_err;
  if (verbose) std::cout << "**Diboson Err: " << Error(vv_hist) << std::endl;

  // // ---------------------------------------------------
  // // Generate ZLL, ZL and ZJ Norm and Shapes
  // // ---------------------------------------------------
  if (verbose) cout << "**** ZLL: " << endl;

  double zll_norm, zl_norm, zj_norm;
  TH1F *zll_hist = nullptr;
  TH1F *zl_hist = nullptr;
  TH1F *zj_hist = nullptr;

  TH1F *zll_hist_ss = nullptr; 
  double zll_norm_ss = 0;

  std::string soup = is_paper ? "Soup" : "";
  if (!is_2012) soup = "";


  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
		zll_hist_ss = (TH1F*)(plots[Token("DYJetsToLL"+soup,cat,ss_sel)].hist_ptr()->Clone());
		zll_norm_ss = Integral(plots[Token("DYJetsToLL"+soup,cat,ss_sel)].hist_ptr());

    double zll_inclusive = Integral(plots[Token("DYJetsToLL"+soup,"inclusive",os_sel)].hist_ptr());
    double zl_inclusive = Integral(plots[Token("DYJetsToLL-L"+soup,"inclusive",os_sel)].hist_ptr());
    double zj_inclusive = Integral(plots[Token("DYJetsToLL-J"+soup,"inclusive",os_sel)].hist_ptr());

	  double zll_embed_eff = (Integral(plots[Token("Embedded",cat,os_sel)].hist_ptr()) / Integral(plots[Token("Embedded","inclusive",os_sel)].hist_ptr()));
    if (verbose) {
      std::cout << "ZLL Inclusive: " << zll_inclusive << std::endl;
      std::cout << "ZL Inclusive: " << zl_inclusive << std::endl;
      std::cout << "ZJ Inclusive: " << zj_inclusive << std::endl;
      std::cout << "Embedded Inclusive->Category Eff: " << zll_embed_eff << std::endl;
      std::cout << "ZLL Extrap. into Category: " << (zll_inclusive * zll_embed_eff) << std::endl;
      std::cout << "ZL Extrap. into Category: " << (zl_inclusive * zll_embed_eff) << std::endl;
      std::cout << "ZJ Extrap. into Category: " << (zj_inclusive * zll_embed_eff) << std::endl;
      std::cout << "[MC Norm in Category]: " << Integral(plots[Token("DYJetsToLL",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL",cat,os_sel)].hist_ptr()) << std::endl;
      std::cout << "[MC Norm in Category -L]: " << Integral(plots[Token("DYJetsToLL-L",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-L",cat,os_sel)].hist_ptr()) << std::endl;
      std::cout << "[MC Norm in Category -J]: " << Integral(plots[Token("DYJetsToLL-J",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-J",cat,os_sel)].hist_ptr()) << std::endl;
      if (is_2012) {
        std::cout << "[MC Soup Norm in Category]: " << Integral(plots[Token("DYJetsToLLSoup",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLLSoup",cat,os_sel)].hist_ptr()) << std::endl;
        std::cout << "[MC Soup Norm in Category -L]: " << Integral(plots[Token("DYJetsToLL-LSoup",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-LSoup",cat,os_sel)].hist_ptr()) << std::endl;
        std::cout << "[MC Soup Norm in Category -J]: " << Integral(plots[Token("DYJetsToLL-JSoup",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-JSoup",cat,os_sel)].hist_ptr()) << std::endl;
      }
    }
  
    if (method <= 4 || method >= 6) {
      zll_norm = Integral(plots[Token("DYJetsToLL"+soup,cat,os_sel)].hist_ptr());
      zl_norm = Integral(plots[Token("DYJetsToLL-L"+soup,cat,os_sel)].hist_ptr());
      zj_norm = Integral(plots[Token("DYJetsToLL-J"+soup,cat,os_sel)].hist_ptr());
      if (zll_norm > 0 && (zl_norm > 0 || zj_norm > 0)) {
        double zl_norm_new = zll_norm * (zl_norm/(zl_norm+zj_norm));
        double zj_norm_new = zll_norm * (zj_norm/(zl_norm+zj_norm));
        if (verbose) {
          cout << "** Renormalising ZL and ZJ" << endl;
          cout << "ZL old: " << zl_norm << ", ZL new: " << zl_norm_new << endl;
          cout << "ZJ old: " << zj_norm << ", ZJ new: " << zj_norm_new << endl;          
        }
        zl_norm = zl_norm_new;
        zj_norm = zj_norm_new;
      }
      if (shift_backgrounds) {
        zl_norm *= zl_shift;
        zj_norm *= zj_shift;
        zll_norm = zl_norm + zj_norm;
      }
      // zll_hist = (TH1F*)(plots[Token("DYJetsToLL",cat,os_sel)].hist_ptr()->Clone());
      zl_hist = (TH1F*)(plots[Token("DYJetsToLL-L"+soup,cat,os_sel)].hist_ptr()->Clone());
      if (method == 3 && channel == channel::et && !is_paper) zl_hist = (TH1F*)(plots[Token("Special_18_DYJetsToLL-L",cat,os_sel)].hist_ptr()->Clone());
      zj_hist = (TH1F*)(plots[Token("DYJetsToLL-J"+soup,cat,os_sel)].hist_ptr()->Clone());
      if (method == 6 || method == 7 || method == 12) {
        zl_hist = (TH1F*)(plots[Token("DYJetsToLL-L"+soup,cat+"_loose",os_sel)].hist_ptr()->Clone());
        zj_hist = (TH1F*)(plots[Token("DYJetsToLL-J"+soup,cat+"_loose",os_sel)].hist_ptr()->Clone());
      }
      if (Integral(zl_hist) > 0.0) zl_hist->Scale( zl_norm / Integral(zl_hist) );
      if (Integral(zj_hist) > 0.0) zj_hist->Scale( zj_norm / Integral(zj_hist) );
      zll_hist = (TH1F*)zl_hist->Clone();
      zll_hist->Add(zj_hist);
    }
  
    if (method == 5) {
      double zll_twojet_embed_eff = (Integral(plots[Token("Embedded",cat,os_sel)].hist_ptr()) / Integral(plots[Token("Embedded","twojet",os_sel)].hist_ptr()));
      if (is_2012 && verbose) {
        std::cout << "[MC Soup Norm in twojet]: " << Integral(plots[Token("DYJetsToLLSoup","twojet",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLLSoup","twojet",os_sel)].hist_ptr()) << std::endl;
        std::cout << "[MC Soup Norm in twojet -L]: " << Integral(plots[Token("DYJetsToLL-LSoup","twojet",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-LSoup","twojet",os_sel)].hist_ptr()) << std::endl;
        std::cout << "[MC Soup Norm in twojet -J]: " << Integral(plots[Token("DYJetsToLL-JSoup","twojet",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-JSoup","twojet",os_sel)].hist_ptr()) << std::endl;
      }
      if (verbose) {
        std::cout << "[zll_twojet_embed_eff]: " << zll_twojet_embed_eff << std::endl;
        std::cout << "[MC Norm in vbf_loose]: " << Integral(plots[Token("DYJetsToLL","vbf_loose",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL","vbf_loose",os_sel)].hist_ptr()) << std::endl;
        std::cout << "[MC Norm in vbf_loose -L]: " << Integral(plots[Token("DYJetsToLL-L","vbf_loose",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-L","vbf_loose",os_sel)].hist_ptr()) << std::endl;
        std::cout << "[MC Norm in vbf_loose -J]: " << Integral(plots[Token("DYJetsToLL-J","vbf_loose",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-J","vbf_loose",os_sel)].hist_ptr()) << std::endl;
      }
      if (is_2012) {
        if (verbose) {
          std::cout << "[MC Soup Norm in vbf_loose]: " << Integral(plots[Token("DYJetsToLLSoup","vbf_loose",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLLSoup","vbf_loose",os_sel)].hist_ptr()) << std::endl;
          std::cout << "[MC Soup Norm in vbf_loose -L]: " << Integral(plots[Token("DYJetsToLL-LSoup","vbf_loose",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-LSoup","vbf_loose",os_sel)].hist_ptr()) << std::endl;
          std::cout << "[MC Soup Norm in vbf_loose -J]: " << Integral(plots[Token("DYJetsToLL-JSoup","vbf_loose",os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToLL-JSoup","vbf_loose",os_sel)].hist_ptr()) << std::endl;          
        }
        zl_norm = Integral(plots[Token("DYJetsToLL-LSoup","twojet",os_sel)].hist_ptr()) * zll_twojet_embed_eff; 
        zj_norm = Integral(plots[Token("DYJetsToLL-JSoup",cat,os_sel)].hist_ptr()); 
      } else {
        zl_norm = Integral(plots[Token("DYJetsToLL-L","twojet",os_sel)].hist_ptr()) * zll_twojet_embed_eff; 
        zj_norm = Integral(plots[Token("DYJetsToLL-J",cat,os_sel)].hist_ptr());         
      }
      if (shift_backgrounds) {
        zl_norm *= zl_shift;
        zj_norm *= zj_shift;
      }
      zll_norm = zl_norm + zj_norm;
      if (channel == channel::et) {
        if (!is_paper) {
        zl_hist =  (TH1F*)(plots[Token("Special_18_DYJetsToLL-L","vbf_loose_jets20", os_sel)].hist_ptr()->Clone());
        } else {
        zl_hist =  (TH1F*)(plots[Token("DYJetsToLL-LSoup","vbf_loose_jets20", os_sel)].hist_ptr()->Clone());
        }
      }
      if (channel == channel::mt || channel == channel::mtmet) zl_hist =  (TH1F*)(plots[Token("DYJetsToLL-L"+soup,"vbf_loose_jets20", os_sel)].hist_ptr()->Clone());
      zj_hist =  (TH1F*)(plots[Token("DYJetsToLL-J"+soup,"vbf_loose", os_sel)].hist_ptr()->Clone());
      zl_hist->Scale( zl_norm / Integral(zl_hist) );
      zj_hist->Scale( zj_norm / Integral(zj_hist) );
      zll_hist = (TH1F*)zj_hist->Clone();
      zll_hist->Add(zl_hist);
    }

    // if (method == 6 || method == 7) {
    //   zll_hist = (TH1F*)(os_sel["Special_9_DYJetsToLL"+sel].hist_ptr()->Clone());
    //   zl_hist = (TH1F*)(os_sel["Special_9_DYJetsToLL-L"+sel].hist_ptr()->Clone());
    //   zj_hist = (TH1F*)(os_sel["Special_9_DYJetsToLL-J"+sel].hist_ptr()->Clone());
    // }
  } 

  if (channel == channel::em) {
    zll_norm = 0.0;
    zl_norm = 0.0;
    zj_norm = 0.0;
    zll_hist = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());
    zl_hist = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());
    zj_hist = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());
    zll_hist_ss = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());; 
  }


  if (Integral(zll_hist) > 0.0) zll_hist->Scale( zll_norm / Integral(zll_hist) );
  zll_hist_ss->Scale( zll_norm_ss / Integral(zll_hist_ss) );
  if (Integral(zl_hist) > 0.0) zl_hist->Scale( zl_norm / Integral(zl_hist) );
  if (Integral(zj_hist) > 0.0) zj_hist->Scale( zj_norm / Integral(zj_hist) );

  double zll_err = Error(zll_hist);
  double zl_err = Error(zl_hist);
  double zj_err = Error(zj_hist);

  ic::TH1PlotElement zll_shape("zll_shape", zll_hist);
  ic::TH1PlotElement zll_shape_ss("zll_shape_ss", zll_hist_ss);
  ic::TH1PlotElement zl_shape("zl_shape", zl_hist);
  ic::TH1PlotElement zj_shape("zj_shape", zj_hist);

  std::cout << boost::format(yield_fmt) % "ZLL" % zll_norm % zll_err;
  std::cout << boost::format(yield_fmt) % "ZL" % zl_norm % zl_err;
  std::cout << boost::format(yield_fmt) % "ZJ" % zj_norm % zj_err;

  // ---------------------------------------------------
  // Generate ZTT Shape and Norm
  // ---------------------------------------------------
  if (verbose) cout << "**** ZTT: " << endl;

  double ztt_norm = 0;
  TH1F *ztt_hist = nullptr;
  double ztt_mc_inc_yield = Integral(plots[Token("DYJetsToTauTau","inclusive","os")].hist_ptr());
  if (is_paper && is_2012 )ztt_mc_inc_yield = Integral(plots[Token("DYJetsToTauTauSoup","inclusive","os")].hist_ptr());
  if (verbose) cout << "- DYJetsToTauTau dilepton OS yield (no mT/pzeta cut): " << ztt_mc_inc_yield << std::endl;
  double embedded_eff = Integral(plots[Token("Embedded",cat,os_sel)].hist_ptr()) / Integral(plots[Token("Embedded","inclusive","os")].hist_ptr());
  double embedded_eff_err = (Error(plots[Token("Embedded",cat,os_sel)].hist_ptr()) / Integral(plots[Token("Embedded",cat,os_sel)].hist_ptr())) * embedded_eff;
  if (verbose) {
    cout << "Embedded Eff: " << embedded_eff << std::endl;
    cout << "Embedded Error: " << embedded_eff_err << std::endl;
  }
  ztt_norm = ztt_mc_inc_yield * embedded_eff;
  if (use_ztt_mc && is_2012) {
    ztt_norm =  Integral(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr());
    double ztt_mc_eff = Integral(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr()) / Integral(plots[Token("DYJetsToTauTauSoup","inclusive","os")].hist_ptr());
    double ztt_mc_eff_err = (Error(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr()) / Integral(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr())) * ztt_mc_eff;
    if (verbose) {
      cout << "ZTT MC Eff: " << ztt_mc_eff << std::endl;
      cout << "ZTT MC Error: " << ztt_mc_eff_err << std::endl;      
    }
  }
  if (use_ztt_mc && !is_2012) ztt_norm = Integral(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr());
  if (verbose) cout << "- [DYJetsToTauTau MC Category Yield]: " << Integral(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()) << endl;
  if (verbose && is_2012) cout << "- [DYJetsToTauTauSoup MC Category Yield]: " << Integral(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr()) << " +/- " << Error(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr()) << endl;
  
  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    //0/1 Jet, Inclusive
    if (method <= 4 || method >= 6) {
      if (use_ztt_mc) {
        if (is_2012) ztt_hist = (TH1F*)(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr()->Clone());
        if (!is_2012) ztt_hist = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());
      } else {
        ztt_hist = (TH1F*)(plots[Token("Embedded",cat,os_sel)].hist_ptr()->Clone());
      }
    }
    // VBF
    if (method == 5) {
      if (use_ztt_mc) {
        if (is_2012) ztt_hist = (TH1F*)(plots[Token("DYJetsToTauTauSoup",cat,os_sel)].hist_ptr()->Clone());
        if (!is_2012) ztt_hist = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());
      } else {
        if (is_2012) ztt_hist = (TH1F*)(plots[Token("Embedded",cat,os_sel)].hist_ptr()->Clone());        
        if (!is_2012) ztt_hist = (TH1F*)(plots[Token("Embedded","vbf_loose",os_sel)].hist_ptr()->Clone());        
      }
    }
  }

  if (channel == channel::em) {
    if (use_ztt_mc) {
      ztt_hist = (TH1F*)(plots[Token("DYJetsToTauTau",cat,os_sel)].hist_ptr()->Clone());
    } else {
      ztt_hist = (TH1F*)(plots[Token("Embedded",cat,os_sel)].hist_ptr()->Clone());
    }
  }

  double embed_norm = Integral(plots["DYJetsToTauTau_inclusive_os"].hist_ptr()) / Integral(plots["Embedded_inclusive_os"].hist_ptr());
  if (verbose) std::cout << "Embedded->Norm Scaling: " << embed_norm << std::endl; 
  if (verbose) cout << "- [Embedded Category Yield]: " << Integral(plots[Token("Embedded",cat,os_sel)].hist_ptr())*embed_norm << " +/- " << Error(plots[Token("Embedded",cat,os_sel)].hist_ptr())*embed_norm << endl;
  ztt_hist->Scale( ztt_norm / Integral(ztt_hist) );

  TH1F *ztt_hist_up = NULL;
  TH1F *ztt_hist_down = NULL;
  if (shift_tscale) {
    ztt_hist_up   = (TH1F*)(plots[Token("TSCALE_UP/Embedded",cat,os_sel)].hist_ptr()->Clone());
    ztt_hist_down = (TH1F*)(plots[Token("TSCALE_DOWN/Embedded",cat,os_sel)].hist_ptr()->Clone());
    ztt_hist_up->Scale(ztt_norm / Integral(ztt_hist_up));
    ztt_hist_down->Scale(ztt_norm / Integral(ztt_hist_down));
    ic::VerticalMorph(ztt_hist, ztt_hist_up, ztt_hist_down, tscale_shift);
  }

  ic::TH1PlotElement ztt_shape("ztt_shape", ztt_hist);

  double ztt_err = Error(ztt_hist);
  std::cout << boost::format(yield_fmt) % "ZTT" % ztt_norm % ztt_err;

  // 5.3% (high), 1.5% (low)
  double ztt_norm_ss = Integral(plots[Token("Embedded",cat,ss_sel)].hist_ptr()) * embed_norm;
  TH1F *ztt_hist_ss = (TH1F*)(plots[Token("Embedded",cat,ss_sel)].hist_ptr()->Clone());;
  ztt_hist_ss->Scale( ztt_norm_ss / Integral(ztt_hist_ss) );
  ic::TH1PlotElement ztt_shape_ss("ztt_shape_ss", ztt_hist_ss);

  if (method == 6 || method == 7) {
    double top_norm_diff = (method == 7) ? (0.053*ztt_norm) : (0.015*ztt_norm);
    std::cout << "TOP ZTT Subtraction fraction: " << (top_norm_diff/top_norm) << std::endl;
    top_norm = top_norm - top_norm_diff;
    top_shape.hist_ptr()->Scale(top_norm / Integral(top_shape.hist_ptr()));
  }

  if (method == 12) {
    double top_norm_diff = (0.015*ztt_norm);
    std::cout << "TOP ZTT Subtraction fraction: " << (top_norm_diff/top_norm) << std::endl;
    top_norm = top_norm - top_norm_diff;
    top_shape.hist_ptr()->Scale(top_norm / Integral(top_shape.hist_ptr()));
  }

  // ---------------------------------------------------
  // Generate W+Jets Shape and Norm
  // ---------------------------------------------------
  if (verbose) cout << "**** W: " << endl;

  double w_norm = 0.0;
  double w_os_sel = 0.0;
  double w_os_con = 0.0;
  double w_ss_sel = 0.0;
  double w_ss_con = 0.0;
  double w_ss_sel_inclusive = 0.0;
  TH1F *w_hist = NULL;

  double w_err = 0.0;
  double w_err_data_rel = 0.0;
  double w_err_mc_rel = 0.0;

  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    // 0/1 Jet, B Jet, dilepton, inclusive, twojet
    if (method <= 4 || method >= 6) {
    	w_os_con = WSideband(plots, vv_samples, embed_norm, cat, os_con, verbose);
      // Assume the stat error is just data in sideband * stat of W in signal region
      w_err_data_rel = Error(plots[Token("Data",cat,os_con)].hist_ptr()) / Integral(plots[Token("Data",cat,os_con)].hist_ptr());
      if (verbose) std::cout << "=> OS Control W: " << w_os_con << std::endl;
      
      w_ss_con = WSideband(plots, vv_samples, embed_norm, cat, ss_con, verbose);
      if (verbose) std::cout << "=> SS Control W: " << w_ss_con << std::endl;
    }
    // Extrapolation for 0/1 Jet, dilepton, inclusive, twojet
    if ( (method <= 4|| method >= 8) && method != 12 ) {
    	double w_os_sel_r = Integral(plots[Token("WJetsToLNuSoup",cat,os_sel)].hist_ptr());
      w_err_mc_rel = Error(plots[Token("WJetsToLNuSoup",cat,os_sel)].hist_ptr()) / Integral(plots[Token("WJetsToLNuSoup",cat,os_sel)].hist_ptr());
    	double w_os_con_r = Integral(plots[Token("WJetsToLNuSoup",cat,os_con)].hist_ptr());
    	double w_ss_sel_r = Integral(plots[Token("WJetsToLNuSoup",cat,ss_sel)].hist_ptr());
    	double w_ss_con_r = Integral(plots[Token("WJetsToLNuSoup",cat,ss_con)].hist_ptr());
      double w_factor_os = w_os_sel_r / w_os_con_r;
      double w_factor_ss = w_ss_sel_r / w_ss_con_r;
      double w_factor_os_num_err = Error(plots[Token("WJetsToLNuSoup",cat,os_sel)].hist_ptr());
      double w_factor_os_den_err = Error(plots[Token("WJetsToLNuSoup",cat,os_con)].hist_ptr());
      double w_factor_ss_num_err = Error(plots[Token("WJetsToLNuSoup",cat,ss_sel)].hist_ptr());
      double w_factor_ss_den_err = Error(plots[Token("WJetsToLNuSoup",cat,ss_con)].hist_ptr());
      if (verbose) std::cout << "W MC OS Signal/Control factor (category): " << w_os_sel_r 
        << " +/- " << w_factor_os_num_err << " / " << w_os_con_r << " +/- " << w_factor_os_den_err << " = " << w_factor_os << endl;
      if (verbose) std::cout << "W MC SS Signal/Control factor (category): " << w_ss_sel_r 
        << " +/- " << w_factor_ss_num_err << " / " << w_ss_con_r << " +/- " << w_factor_ss_den_err << " = " << w_factor_ss << endl;
      double w_tot_sel = w_os_sel_r + w_ss_sel_r;
      double w_tot_con = w_os_con_r + w_ss_con_r;
      double w_factor = w_tot_sel / w_tot_con;
      if (verbose) std::cout << "W MC TOT Signal/Control factor (category): " << w_tot_sel << "/" << w_tot_con << " = " << w_factor << std::endl;
      w_os_sel = w_os_con * w_factor_os;
      w_ss_sel = w_ss_con * w_factor_ss;
      w_hist = (TH1F*)(plots[Token("WJetsToLNuSoup",cat,os_sel)].hist_ptr()->Clone());
      if (method == 3) w_hist->Add(plots[Token("WJetsToLNuSoup",cat,ss_sel)].hist_ptr());
      w_norm = w_os_sel;
    }
    // Extrapolation of B Jet
    if (method == 6 || method == 7 || method == 12) {
    	double w_os_sel_r = Integral(plots[Token("WJetsToLNuSoup",cat+"_loose",os_sel)].hist_ptr());
    	double w_os_con_r = Integral(plots[Token("WJetsToLNuSoup",cat+"_loose",os_con)].hist_ptr());
    	double w_ss_sel_r = Integral(plots[Token("WJetsToLNuSoup",cat+"_loose",ss_sel)].hist_ptr());
    	double w_ss_con_r = Integral(plots[Token("WJetsToLNuSoup",cat+"_loose",ss_con)].hist_ptr());
    	double w_factor_os = w_os_sel_r / w_os_con_r;
    	double w_factor_ss = w_ss_sel_r / w_ss_con_r;      
    	double w_factor_os_num_err = Error(plots[Token("WJetsToLNuSoup",cat+"_loose",os_sel)].hist_ptr());
    	double w_factor_os_den_err = Error(plots[Token("WJetsToLNuSoup",cat+"_loose",os_con)].hist_ptr());
    	double w_factor_ss_num_err = Error(plots[Token("WJetsToLNuSoup",cat+"_loose",ss_sel)].hist_ptr());
    	double w_factor_ss_den_err = Error(plots[Token("WJetsToLNuSoup",cat+"_loose",ss_con)].hist_ptr());
    	if (verbose) std::cout << "W MC OS Signal/Control factor (category): " << w_os_sel_r 
    	  << " +/- " << w_factor_os_num_err << " / " << w_os_con_r << " +/- " << w_factor_os_den_err << " = " << w_factor_os << endl;
    	if (verbose) std::cout << "W MC SS Signal/Control factor (category): " << w_ss_sel_r 
    	  << " +/- " << w_factor_ss_num_err << " / " << w_ss_con_r << " +/- " << w_factor_ss_den_err << " = " << w_factor_ss << endl;
    	double w_tot_sel = w_os_sel_r + w_ss_sel_r;
    	double w_tot_con = w_os_con_r + w_ss_con_r;
    	double w_factor = w_tot_sel / w_tot_con;
      if (verbose) std::cout << "W MC TOT Signal/Control factor (category): " << w_tot_sel << "/" << w_tot_con << " = " << w_factor << std::endl;
      w_os_sel = w_os_con * w_factor_os;
      w_ss_sel = w_ss_con * w_factor_ss;
      double w_ss_os = Integral(plots[Token("WJetsToLNuSoup","inclusive",ss_sel)].hist_ptr()) / Integral(plots[Token("WJetsToLNuSoup","inclusive",os_sel)].hist_ptr());
      if (verbose) std::cout << "[W Inclusive SS/OS factor]: " << w_ss_os << std::endl;
      if (verbose) std::cout << "[SS/OS factor * w_os_sel]: " << w_os_sel * w_ss_os << std::endl;
      //w_ss_sel = w_os_sel * w_ss_os;
      std::cout << "W SS Sel: " << w_ss_sel << std::endl;
      w_hist = (TH1F*)(plots[Token("WJetsToLNuSoup",cat+"_loose",os_sel)].hist_ptr()->Clone());
      w_norm = w_os_sel;
    }
    // Normalisation and extrapolation for VBF
    if (method == 5) {
    	w_os_con = WSideband(plots, vv_samples, embed_norm, cat, "os_con_mt_60-120", verbose);
      if (verbose) std::cout << "=> OS Control W: " << w_os_con << std::endl;
      // double w_factor = Integral(os_sel["Special_7_WJetsToLNuSouptwojet"].hist_ptr()) / Integral(os_con["Special_7_WJetsToLNuSouptwojet"].hist_ptr());
      double w_factor = Integral(plots[Token("WJetsToLNuSoup","vbf_loose",os_sel)].hist_ptr()) / Integral(plots[Token("WJetsToLNuSoup","vbf_loose","os_con_mt_60-120")].hist_ptr());
      std::cout << "W MC OS Signal/Control factor (vbf_loose): " << w_factor << std::endl;
      w_os_sel = w_os_con * w_factor;
      if (verbose) std::cout << "W MC OS Signal/Control factor (vbf_loose): " << w_factor << endl;
      // std::cout << "OS Signal: " << Integral(os_sel["Special_7_WJetsToLNuSouptwojet"].hist_ptr()) << "\t" << Error(os_sel["Special_7_WJetsToLNuSouptwojet"].hist_ptr()) << std::endl;
      double w_ss_os = Integral(plots[Token("WJetsToLNuSoup","inclusive",ss_sel)].hist_ptr()) / Integral(plots[Token("WJetsToLNuSoup","inclusive",os_sel)].hist_ptr());
      //if (verbose) std::cout << "W MC In SS/OS Factor: " << w_ss_os << endl;
      w_ss_sel = w_os_sel * w_ss_os;// not actually used
      w_hist = (TH1F*)(plots[Token("WJetsToLNuSoup","vbf_loose",os_sel)].hist_ptr()->Clone());
      //w_hist = (TH1F*)(os_sel["WJetsToLNuSouptwojet"].hist_ptr()->Clone());
      w_norm = w_os_sel;
    }
  }

  TH1F *w_hist_ss=0;

  if (channel == channel::em) {
    w_norm = 0; 
    w_hist = (TH1F*)(plots[Token("Data",cat,ss_sel)].hist_ptr()->Clone());
    w_hist->Scale(0);
    w_hist_ss = (TH1F*)(plots[Token("Data",cat,ss_sel)].hist_ptr()->Clone());
    w_hist_ss->Scale(0);
  }

  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
  	w_hist->Scale( w_norm / Integral(w_hist) );
	  w_hist_ss = (TH1F*)(plots[Token("WJetsToLNuSoup",cat,ss_sel)].hist_ptr()->Clone());
	  double w_norm_ss = w_ss_sel;
  	w_hist_ss->Scale( w_norm_ss / Integral(w_hist_ss) );
  }
  
  ic::TH1PlotElement w_shape("w_shape", w_hist);
  // w_err_data_rel = 1. - w_err_data_rel;
  // w_err_mc_rel = 1. - w_err_mc_rel;
  w_err = sqrt(w_err_mc_rel*w_err_mc_rel + w_err_data_rel * w_err_data_rel);
  w_err = w_err * Integral(w_hist);
	ic::TH1PlotElement w_shape_ss("w_shape_ss", w_hist_ss);
  std::cout << boost::format(yield_fmt) % "W" % w_norm % w_err;



  // ---------------------------------------------------
  // Generate QCD Shape and Norm
  // ---------------------------------------------------
  if (verbose) cout << "**** QCD: " << endl;


  double qcd_norm = 0.0;
  double qcd_err = 0.0;
  double qcd_inclusive = 0.0;
  double os_ss_ratio = parser.GetParam<double>("QCD_OS_SS_RATIO");
  TH1F * h1 = nullptr;
  TH1F * h2 = nullptr;
  TH1F * h3 = nullptr;
  TH1F *qcd_hist = nullptr;

  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    //-----------------------
    // 0/1 Jet and dilepton
    //-----------------------
    if (method <= 4 || method >= 6) {
      double data_ss_sel = Integral(plots[Token("Data",cat,ss_sel)].hist_ptr());
      qcd_err += Error(plots[Token("Data",cat,ss_sel)].hist_ptr()) * Error(plots[Token("Data",cat,ss_sel)].hist_ptr());
      data_ss_sel -= w_ss_sel;
      qcd_err += pow(Error(plots[Token("Data",cat,ss_con)].hist_ptr()) / Integral(plots[Token("Data",cat,ss_con)].hist_ptr()) * w_ss_sel, 2.0);
      if (is_2012 && !is_paper) data_ss_sel -= Integral(plots[Token("DYJetsToLL",cat,ss_sel)].hist_ptr());
      if (is_2012 && is_paper) data_ss_sel -= Integral(plots[Token("DYJetsToLLSoup",cat,ss_sel)].hist_ptr());
      data_ss_sel -= Integral(plots[Token("TTJets",cat,ss_sel)].hist_ptr());
      if (is_2012 && !is_paper) data_ss_sel -= Integral(plots[Token("DYJetsToTauTau",cat,ss_sel)].hist_ptr());
      if (is_2012 && is_paper) data_ss_sel -= Integral(plots[Token("DYJetsToTauTauSoup",cat,ss_sel)].hist_ptr());
      qcd_err = sqrt(qcd_err);
      //data_ss_sel -= (Integral(plots[Token("Embedded",cat,ss_sel)].hist_ptr())*embed_norm);
      double vv_ss_total = 0.0;
      for (unsigned i = 0; i < vv_samples.size(); ++i) vv_ss_total += Integral(plots[Token(vv_samples[i],cat,ss_sel)].hist_ptr());
      data_ss_sel -= vv_ss_total;
      if (verbose) std::cout << "SS Data: " << Integral(plots[Token("Data",cat,ss_sel)].hist_ptr()) << endl;
      if (verbose) std::cout << "SS W: " << w_ss_sel << endl;
      if (verbose) std::cout << "SS DYJetsToLL: " << Integral(plots[Token("DYJetsToLL",cat,ss_sel)].hist_ptr()) << endl;
      if (verbose) std::cout << "SS TTJets: " << Integral(plots[Token("TTJets",cat,ss_sel)].hist_ptr()) << endl;
      if (verbose) std::cout << "SS ZTT: " << Integral(plots[Token("DYJetsToTauTau",cat,ss_sel)].hist_ptr()) << endl;
      if (verbose) std::cout << "[SS Embedded]: " << Integral(plots[Token("Embedded",cat,ss_sel)].hist_ptr())*embed_norm << endl;
      if (verbose) std::cout << "SS VV: " << vv_ss_total << std::endl;
      if (verbose) std::cout << "=> SS QCD: " << data_ss_sel << std::endl;
      qcd_norm = os_ss_ratio * data_ss_sel;
      if (qcd_norm <= 0.0) qcd_norm = 0.0000001;
      if (non_mass_plot || method == 0 || method == 8 || method == 11) {
        if (verbose) std::cout << "Using Full Isolation for QCD shape!" << std::endl;
        qcd_hist = (TH1F*)(plots[Token("Data",cat,ss_sel)].hist_ptr()->Clone());
        qcd_hist->Add(plots[Token("DYJetsToLL",cat,ss_sel)].hist_ptr(), -1.0);
        qcd_hist->Add(plots[Token("Embedded",cat,ss_sel)].hist_ptr(), -1.0*embed_norm);
        qcd_hist->Add(plots[Token("TTJets",cat,ss_sel)].hist_ptr(), -1.0);
        TH1F* w_cat_ss = (TH1F*)plots[Token("WJetsToLNuSoup",cat,ss_sel)].hist_ptr()->Clone();
        w_cat_ss->Scale(w_ss_sel/ Integral(plots[Token("WJetsToLNuSoup",cat,ss_sel)].hist_ptr()));
        qcd_hist->Add(w_cat_ss, -1.0);
      } else {
        qcd_hist = (TH1F*)(plots[Token("Special_3_Data",cat,ss_sel)].hist_ptr()->Clone());
      }

      for (int j = 0; j < qcd_hist->GetNbinsX(); ++j) {
        if (qcd_hist->GetBinContent(j+1) < 0) {
          // qcd_hist->SetBinContent(j+1,0.0);
          // qcd_hist->SetBinError(j+1, 0.0);
          qcd_hist->SetBinContent(j+1,0.0000001);
          qcd_hist->SetBinError(j+1, 1.0);
        }
      }
    }

    //-----------------------
    //Inclusive will be needed for QCD - first do inclusive W
    //-----------------------
    if (verbose) cout << "** Determine QCD inclusive: " << endl;
    w_ss_sel_inclusive = WSideband(plots, vv_samples, embed_norm, "inclusive", ss_con, verbose);
    if (verbose) std::cout << "=> SS Control W Inclusive: " << w_ss_sel_inclusive << std::endl;

  	double w_os_sel_inc = Integral(plots[Token("WJetsToLNuSoup","inclusive",os_sel)].hist_ptr());
  	double w_os_con_inc = Integral(plots[Token("WJetsToLNuSoup","inclusive",os_con)].hist_ptr());
  	double w_ss_sel_inc = Integral(plots[Token("WJetsToLNuSoup","inclusive",ss_sel)].hist_ptr());
  	double w_ss_con_inc = Integral(plots[Token("WJetsToLNuSoup","inclusive",ss_con)].hist_ptr());
    double w_factor_os_inc = w_os_sel_inc / w_os_con_inc;
    double w_factor_ss_inc = w_ss_sel_inc / w_ss_con_inc;

    double w_tot_sel_inc = w_os_sel_inc + w_ss_sel_inc;
    double w_tot_con_inc = w_os_con_inc + w_ss_con_inc;
    double w_factor_inc = w_tot_sel_inc / w_tot_con_inc;
    if (verbose) std::cout << "W MC OS Signal/Control factor (inclusive): " << w_os_sel_inc << " / " << w_os_con_inc << " = " << w_factor_os_inc << endl;
    if (verbose) std::cout << "W MC SS Signal/Control factor (inclusive): " << w_ss_sel_inc << " / " << w_ss_con_inc << " = " << w_factor_ss_inc << endl;
    if (verbose) std::cout << "W MC TOT Signal/Control factor (category): " << w_tot_sel_inc << "/" << w_tot_con_inc << " = " << w_factor_inc << std::endl;
    w_ss_sel_inclusive = w_ss_sel_inclusive * w_factor_ss_inc;
    if (verbose) std::cout << "=> SS Signal W Inclusive: " << w_ss_sel_inclusive << std::endl;

    // Now do the QCD
    qcd_inclusive = Integral(plots[Token("Data","inclusive",ss_sel)].hist_ptr());
    qcd_inclusive -= w_ss_sel_inclusive;
    qcd_inclusive -= Integral(plots[Token("DYJetsToLL","inclusive",ss_sel)].hist_ptr());
    qcd_inclusive -= Integral(plots[Token("TTJets","inclusive",ss_sel)].hist_ptr());
    qcd_inclusive -= Integral(plots[Token("DYJetsToTauTau","inclusive",ss_sel)].hist_ptr());
    //qcd_inclusive -= (Integral(plots[Token("Embedded","inclusive",ss_sel)].hist_ptr())*embed_norm);
    double vv_ss_total_inc = 0.0;
    for (unsigned i = 0; i < vv_samples.size(); ++i) vv_ss_total_inc += Integral(plots[Token(vv_samples[i],"inclusive",ss_sel)].hist_ptr());
    qcd_inclusive -= vv_ss_total_inc;
    if (verbose) std::cout << "SS Data Inclusive: " << Integral(plots[Token("Data","inclusive",ss_sel)].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS W Inclusive: " << w_ss_sel_inclusive << std::endl;
    if (verbose) std::cout << "SS ZLL Inclusive: " << Integral(plots[Token("DYJetsToLL","inclusive",ss_sel)].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS TOP Inclusive: " << Integral(plots[Token("TTJets","inclusive",ss_sel)].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS ZTT Inclusive: " << Integral(plots[Token("DYJetsToTauTau","inclusive",ss_sel)].hist_ptr()) << std::endl;
    if (verbose) std::cout << "[SS Embedded Inclusive]: " << Integral(plots[Token("Embedded","inclusive",ss_sel)].hist_ptr())*embed_norm << std::endl;
    if (verbose) std::cout << "SS VV Inclusive: " << vv_ss_total_inc << std::endl;
    qcd_inclusive = os_ss_ratio * qcd_inclusive;
    if (verbose) std::cout << "=> OS QCD Inclusive: " << qcd_inclusive << std::endl;
    if (verbose) std::cout << "QCD Selection Category Efficiency: " << (Integral(plots[Token("Special_3_Data",cat,ss_sel)].hist_ptr()) / Integral(plots[Token("Special_3_Data","inclusive",ss_sel)].hist_ptr()) ) << std::endl;

    //-----------------------
    // vbf
    //-----------------------
    if (method == 5) {
    	double qcd_eff = (Integral(plots[Token("Special_3_Data",cat,ss_sel)].hist_ptr()) / Integral(plots[Token("Special_3_Data","inclusive",ss_sel)].hist_ptr()) );
      if (verbose) std::cout << "QCD Selection Category Efficiency: " << qcd_eff << std::endl;
      qcd_norm = qcd_inclusive * qcd_eff;
      // if (verbose) std::cout << "Numberator: " << Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) << std::endl;
      // if (verbose) std::cout << "Denominator: " << Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) << std::endl;
      if (channel == channel::mtmet) {
        qcd_hist = (TH1F*)(plots[Token("Special_3_Data","vbf",ss_sel)].hist_ptr()->Clone());
      } else {
        qcd_hist = (TH1F*)(plots[Token("Special_3_Data","vbf_loose_jets20",ss_sel)].hist_ptr()->Clone());
      }
    }
    //-----------------------
    // btag 
    //-----------------------
    if (method == 7) {
    	// double qcd_eff = (Integral(plots[Token("Special_3_Data",cat,ss_sel)].hist_ptr()) / Integral(plots[Token("Special_3_Data","inclusive",ss_sel)].hist_ptr()) );
    	// if (verbose) std::cout << "QCD Selection Category Efficiency: " << qcd_eff << std::endl;
    	// qcd_norm = qcd_inclusive * qcd_eff;
      qcd_hist = (TH1F*)(plots[Token("Special_3_Data","btag_high_loose",ss_sel)].hist_ptr()->Clone());
    }
    if (method == 6) {
       qcd_hist = (TH1F*)(plots[Token("Special_3_Data","btag_low_loose", ss_sel)].hist_ptr()->Clone());
    }
    if (method == 12) {
       qcd_hist = (TH1F*)(plots[Token("Special_3_Data","btag_loose", ss_sel)].hist_ptr()->Clone());
    }
  }

  TH1F *em_0jet_high_Up = NULL;
  TH1F *em_0jet_high_Down = NULL;
  if (channel == channel::em) {
    if (verbose) std::cout << "Electron Fakes: " <<  Integral(plots["Special_20_Data_inclusive_os"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "Muon Fakes: " <<  Integral(plots["Special_21_Data_inclusive_os"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "Double Fakes: " <<  Integral(plots["Special_22_Data_inclusive_os"].hist_ptr()) << std::endl;
    double qcd_dilepton = Integral(plots["Special_20_Data_inclusive_os"].hist_ptr()) + Integral(plots["Special_21_Data_inclusive_os"].hist_ptr()) - Integral(plots["Special_22_Data_inclusive_os"].hist_ptr()) ;
    qcd_dilepton *= 0.83;
    if (verbose) std::cout << "Inclusive fakes (no pzeta cut): " << qcd_dilepton << std::endl;
    double qcd_category = qcd_dilepton * ( Integral(plots[Token("Data",cat,ss_sel)].hist_ptr()) / Integral(plots[Token("Data","inclusive","ss")].hist_ptr()) );
    if (verbose) std::cout << "Extrap. into category: " << qcd_category << std::endl;

    if (method == 0 || method == 2) {
      qcd_norm = qcd_category;
      // Fully relaxed electron isolation
      qcd_hist = (TH1F*)(plots[Token("Special_23_Data",cat,ss_sel)].hist_ptr()->Clone());
    } else if (method == 1) {
      qcd_norm = qcd_category;
      // Inverted electron isolation
      qcd_hist = (TH1F*)(plots[Token("Special_24_Data",cat,ss_sel)].hist_ptr()->Clone());
      TH1F *fr_hist = (TH1F*)(plots[Token("Special_20_Data",cat,os_sel)].hist_ptr()->Clone());
      fr_hist->Add((TH1F*)(plots[Token("Special_21_Data",cat,os_sel)].hist_ptr()->Clone()) ,1.0);
      fr_hist->Add((TH1F*)(plots[Token("Special_22_Data",cat,os_sel)].hist_ptr()->Clone()) ,-1.0);
      em_0jet_high_Up = (TH1F*)qcd_hist->Clone(TString("Fakes_CMS_htt_FakeShape_em_0jet_high_")+(is_2012 ? "8":"7")+"TeVUp");
      em_0jet_high_Down = (TH1F*)fr_hist->Clone(TString("Fakes_CMS_htt_FakeShape_em_0jet_high_")+(is_2012 ? "8":"7")+"TeVDown");
      qcd_hist->Scale(0.2 / Integral (qcd_hist)); // 0.2
      fr_hist->Scale(0.8 / Integral (fr_hist)); // 0.8
      qcd_hist->Add(fr_hist);//Fakes_CMS_htt_FakeShape_em_0jet_high_8TeVUp
      qcd_hist->Scale(qcd_norm / Integral (qcd_hist));
      em_0jet_high_Up->Scale(qcd_norm / Integral (em_0jet_high_Up));
      em_0jet_high_Down->Scale(qcd_norm / Integral (em_0jet_high_Down));
      
    } else if (method == 3) {
      qcd_norm = Integral(plots[Token("Special_20_Data",cat,os_sel)].hist_ptr()) + Integral(plots[Token("Special_21_Data",cat,os_sel)].hist_ptr()) - Integral(plots[Token("Special_22_Data",cat,os_sel)].hist_ptr()) ;
      qcd_hist = (TH1F*)(plots[Token("Special_20_Data",cat,os_sel)].hist_ptr()->Clone());
      qcd_hist->Add((TH1F*)(plots[Token("Special_21_Data",cat,os_sel)].hist_ptr()->Clone()) ,1.0);
      qcd_hist->Add((TH1F*)(plots[Token("Special_22_Data",cat,os_sel)].hist_ptr()->Clone()) ,-1.0);
    } else if (method == 5) {
      qcd_norm = Integral(plots[Token("Special_20_Data",cat,os_sel)].hist_ptr()) + Integral(plots[Token("Special_21_Data",cat,os_sel)].hist_ptr()) - Integral(plots[Token("Special_22_Data",cat,os_sel)].hist_ptr()) ;
      qcd_hist = (TH1F*)(plots[Token("Special_20_Data","vbf_no_cjv",os_sel)].hist_ptr()->Clone());
      qcd_hist->Add((TH1F*)(plots[Token("Special_21_Data","vbf_no_cjv",os_sel)].hist_ptr()->Clone()) ,1.0);
      qcd_hist->Add((TH1F*)(plots[Token("Special_22_Data","vbf_no_cjv",os_sel)].hist_ptr()->Clone()) ,-1.0);
    } else {
      qcd_norm = qcd_category;
      qcd_hist = (TH1F*)(plots[Token("Data",cat,ss_sel)].hist_ptr()->Clone());
    }
  }

  FixEmptyBins(qcd_hist, true, false);
  qcd_hist->Scale( qcd_norm / Integral(qcd_hist) );

  if (channel == channel::mt && method == 2) {
    h1 = (TH1F*)(qcd_hist->Clone());
    h2 = (TH1F*)h1->Clone(TString("QCD_CMS_htt_QCDShape_mutau_boost_low_")+(is_2012 ? "8":"7")+"TeVUp");
    h3 = (TH1F*)h1->Clone(TString("QCD_CMS_htt_QCDShape_mutau_boost_low_")+(is_2012 ? "8":"7")+"TeVDown");
    float x;
    float y;
    float c;
    float cUp;
    float cDown;
    for(int i=1;i<h1->GetNbinsX();++i){
      x = h1->GetXaxis()->GetBinCenter(i);
      if(x < (is_2012 ? 70 : 50) ){
        y = h1->GetBinContent(i);
        c = 1.15;  // or whatever other correction we want to apply
        cUp = 1.3;
        cDown = 1.0;
        h1->SetBinContent(i,y*c);
        h2->SetBinContent(i,y*cUp);
        h3->SetBinContent(i,y*cDown);
      }  
    }
    qcd_hist = h1;
    h2->Scale( qcd_norm / Integral(h2) );
    h3->Scale( qcd_norm / Integral(h3) );
  }

  qcd_hist->Scale( qcd_norm / Integral(qcd_hist) );


  ic::TH1PlotElement qcd_shape("qcd_shape", qcd_hist);

  std::cout << boost::format(yield_fmt) % "QCD" % qcd_norm % qcd_err;


  double qcd_norm_ss = qcd_norm/os_ss_ratio;
  TH1F *qcd_hist_ss = (TH1F*)qcd_hist->Clone();
  qcd_hist_ss->Scale( qcd_norm_ss / Integral(qcd_hist_ss) );
  ic::TH1PlotElement qcd_shape_ss("qcd_shape_ss", qcd_hist_ss);


  std::cout << boost::format(param_fmt) % "Background Yield" % (top_norm + vv_norm + zll_norm + ztt_norm + w_norm + qcd_norm);


  if (swap_inclusive) {
    if (method == 8) dc_cat_label = "0jet_low";
    if (method == 0) dc_cat_label = "inclusive";
  }

  if (make_datacard) {

    std::string append = "";
    if (tau_scale_mode == 1 && channel != channel::em) append = "_CMS_scale_t_"+dc_mode_label_alt+(is_2012 ? "_8":"_7")+"TeVDown";
    if (tau_scale_mode == 2 && channel != channel::em) append = "_CMS_scale_t_"+dc_mode_label_alt+(is_2012 ? "_8":"_7")+"TeVUp";
    if (tau_scale_mode == 1 && channel == channel::em) append = string("_CMS_scale_e")+(is_2012 ? "_8":"_7")+"TeVDown";
    if (tau_scale_mode == 2 && channel == channel::em) append = string("_CMS_scale_e")+(is_2012 ? "_8":"_7")+"TeVUp";
    if (tau_scale_mode == 1 && channel == channel::em && (method == 1 || method == 3) && is_2012) append = string("_CMS_scale_e_highpt")+(is_2012 ? "_8":"_7")+"TeVDown";
    if (tau_scale_mode == 2 && channel == channel::em && (method == 1 || method == 3) && is_2012) append = string("_CMS_scale_e_highpt")+(is_2012 ? "_8":"_7")+"TeVUp";
    if (tau_scale_mode == 1 && channel == channel::em && (method == 1 || method == 3) && !is_2012) append = string("_CMS_scale_e")+(is_2012 ? "_8":"_7")+"TeVDown";
    if (tau_scale_mode == 2 && channel == channel::em && (method == 1 || method == 3) && !is_2012) append = string("_CMS_scale_e")+(is_2012 ? "_8":"_7")+"TeVUp";

    TFile datacard(("datacard_"+dc_mode_label+"_"+dc_cat_label+"_"+year_label+append+".root").c_str(),"RECREATE");
    datacard.cd();
    gDirectory->mkdir((dc_mode_label+"_"+dc_cat_label).c_str());
    gDirectory->cd((dc_mode_label+"_"+dc_cat_label).c_str());

    double boost_high_clean = 40.0;
    if (plot_name.find("vis_") != plot_name.npos) {
      boost_high_clean = 20.0;
    }
    if (method == 3 && channel == channel::et && !is_2012) {
      std::cout << "Vis sample detected: cleaning up to " << boost_high_clean << std::endl;
    }
    
    if (mssm_mode == 0) {
      for (unsigned i = 0; i < signal_masses.size(); ++i) {
        TH1F *VH = (TH1F*)(plots[Token("WH_ZH_TTH_HToTauTau_M-"+signal_masses[i],cat,os_sel)].hist_ptr()->Clone());
        VH->Scale(1./ (parser.GetParam<double>("XS_WH_ZH_TTH_HToTauTau_M-"+signal_masses[i])));
       if (Integral(VH) < 0.0000001) {
         std::cout << "Histogram " << "WH_ZH_TTH_HToTauTau_M-"+signal_masses[i] << ":" << os_sel << " appears to be empty, adding small value in central bin!" << std::endl;
         VH->SetBinContent(1, 0.0000001);
       }
        VH->SetName(("VH"+signal_masses[i]+append).c_str());
        VH->SetTitle(("VH"+signal_masses[i]+append).c_str());
        if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(VH, boost_high_clean);
        VH->Write();


        TH1F *ggH = (TH1F*)(plots[Token("GluGluToHToTauTau_M-"+signal_masses[i],cat,os_sel)].hist_ptr()->Clone());
        ggH->Scale(1./ (parser.GetParam<double>("XS_GluGluToHToTauTau_M-"+signal_masses[i])));
        ggH->SetName(("ggH"+signal_masses[i]+append).c_str());
        ggH->SetTitle(("ggH"+signal_masses[i]+append).c_str());
        if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(ggH, boost_high_clean);
        ggH->Write();

        TH1F *qqH = (TH1F*)(plots[Token("VBF_HToTauTau_M-"+signal_masses[i],cat,os_sel)].hist_ptr()->Clone());
        qqH->Scale(1./ (parser.GetParam<double>("XS_VBF_HToTauTau_M-"+signal_masses[i])));
        qqH->SetName(("qqH"+signal_masses[i]+append).c_str());
        qqH->SetTitle(("qqH"+signal_masses[i]+append).c_str());
        if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(qqH, boost_high_clean);
        qqH->Write();
      }
    } else {
      for (unsigned i = 0; i < mssm_signal_masses.size(); ++i) {
        TH1F *ggH = (TH1F*)(plots[Token("SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i],cat,os_sel)].hist_ptr()->Clone());
        ggH->Scale(1./ (parser.GetParam<double>("XS_SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i])));
        if (Integral(ggH) < 0.0000001) {
          std::cout << "Histogram " << "SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i] << ":" << os_sel << " appears to be empty, adding small value in first bin!" << std::endl;
          ggH->SetBinContent(1, 0.0000001);
        }
        ggH->SetName(("ggH"+mssm_signal_masses[i]+append).c_str());
        ggH->SetTitle(("ggH"+mssm_signal_masses[i]+append).c_str());
        if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(ggH, boost_high_clean);
        ggH->Write();

        TH1F *bbH = (TH1F*)(plots[Token("SUSYBBHToTauTau_M-"+mssm_signal_masses[i],cat,os_sel)].hist_ptr()->Clone());
        bbH->Scale(1./ (parser.GetParam<double>("XS_SUSYBBHToTauTau_M-"+mssm_signal_masses[i])));
        if (Integral(bbH) < 0.0000001) {
          std::cout << "Histogram " << "SUSYBBHToTauTau_M-"+mssm_signal_masses[i] << ":" << os_sel << " appears to be empty, adding small value in first bin!" << std::endl;
          bbH->SetBinContent(1, 0.0000001);
        }
        bbH->SetName(("bbH"+mssm_signal_masses[i]+append).c_str());
        bbH->SetTitle(("bbH"+mssm_signal_masses[i]+append).c_str());
        if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(bbH, boost_high_clean);
        bbH->Write();
      }
    }

    if (add_sm_signal_as_bg) {
      TH1F *VH_SM125 = (TH1F*)(plots[Token("WH_ZH_TTH_HToTauTau_M-125",cat,os_sel)].hist_ptr()->Clone());
      VH_SM125->SetName(("VH_SM125"+append).c_str());
      VH_SM125->SetTitle(("VH_SM125"+append).c_str());
      VH_SM125->Write();

      TH1F *ggH_SM125 = (TH1F*)(plots[Token("GluGluToHToTauTau_M-125",cat,os_sel)].hist_ptr()->Clone());
      ggH_SM125->SetName(("ggH_SM125"+append).c_str());
      ggH_SM125->SetTitle(("ggH_SM125"+append).c_str());
      ggH_SM125->Write();

      TH1F *qqH_SM125 = (TH1F*)(plots[Token("VBF_HToTauTau_M-125",cat,os_sel)].hist_ptr()->Clone());
      qqH_SM125->SetName(("qqH_SM125"+append).c_str());
      qqH_SM125->SetTitle(("qqH_SM125"+append).c_str());
      qqH_SM125->Write();
    }

    TH1F *dc_ztt = (TH1F*)ztt_hist->Clone();
    dc_ztt->SetName(("ZTT"+append).c_str());
    dc_ztt->SetTitle(("ZTT"+append).c_str());
    if (channel == channel::em) {
      dc_ztt->SetName(("Ztt"+append).c_str());
      dc_ztt->SetTitle(("Ztt"+append).c_str());
    }
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_ztt, boost_high_clean);
    dc_ztt->Write();

    TH1F *dc_zll = (TH1F*)zll_hist->Clone();
    FixEmptyBins(dc_zll, false, false);
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_zll, boost_high_clean);
    dc_zll->SetName(("ZLL"+append).c_str());
    dc_zll->SetTitle(("ZLL"+append).c_str());
    if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) dc_zll->Write();

    TH1F *dc_zl = (TH1F*)zl_hist->Clone();
    FixEmptyBins(dc_zl, false, false);
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_zl, boost_high_clean);
    dc_zl->SetName(("ZL"+append).c_str());
    dc_zl->SetTitle(("ZL"+append).c_str());
    if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) dc_zl->Write();

    TH1F *dc_zj = (TH1F*)zj_hist->Clone();
    FixEmptyBins(dc_zj, false, false);
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_zj, boost_high_clean);
    dc_zj->SetName(("ZJ"+append).c_str());
    dc_zj->SetTitle(("ZJ"+append).c_str());
    if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) dc_zj->Write();

    // std::string zee_append = std::string("_CMS_htt_ZEEShape_etau_")+(is_2012 ? "8":"7")+"TeV";
    // if (tau_scale_mode == 1) zee_append += "Down";
    // if (tau_scale_mode == 2) zee_append += "Up";
    // TH1F *dc_zl_zee, *dc_zll_zee;
    // if (channel == channel::et && tau_scale_mode > 0) {
    //   dc_zl_zee = (TH1F*)dc_zl->Clone();
    //   dc_zl_zee->SetName(("ZL"+zee_append).c_str());
    //   dc_zl_zee->SetTitle(("ZL"+zee_append).c_str());
    //   dc_zll_zee = (TH1F*)dc_zll->Clone();
    //   dc_zll_zee->SetName(("ZLL"+zee_append).c_str());
    //   dc_zll_zee->SetTitle(("ZLL"+zee_append).c_str());
    //   dc_zl_zee->Write();
    //   dc_zll_zee->Write();
    // }

    TH1F *dc_top = (TH1F*)top_hist->Clone();
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_top, boost_high_clean);
    dc_top->SetName(("TT"+append).c_str());
    dc_top->SetTitle(("TT"+append).c_str());
    if (channel == channel::em) {
      dc_top->SetName(("ttbar"+append).c_str());
      dc_top->SetTitle(("ttbar"+append).c_str());
    }
    dc_top->Write();

    TH1F *dc_vv = (TH1F*)vv_hist->Clone();
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_vv, boost_high_clean);
    dc_vv->SetName(("VV"+append).c_str());
    dc_vv->SetTitle(("VV"+append).c_str());
    if (channel == channel::em) {
      dc_vv->SetName(("EWK"+append).c_str());
      dc_vv->SetTitle(("EWK"+append).c_str());
    }
    dc_vv->Write();

    TH1F *dc_w = (TH1F*)w_hist->Clone();
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_w, boost_high_clean);
    FixEmptyBins(dc_w, false, false);
    dc_w->SetName(("W"+append).c_str());
    dc_w->SetTitle(("W"+append).c_str());
    if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) dc_w->Write();

    TH1F *dc_qcd = (TH1F*)qcd_hist->Clone();
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(dc_qcd, boost_high_clean);
    dc_qcd->SetName(("QCD"+append).c_str());
    dc_qcd->SetTitle(("QCD"+append).c_str());
    if (channel == channel::em) {
      dc_qcd->SetName(("Fakes"+append).c_str());
      dc_qcd->SetTitle(("Fakes"+append).c_str());
      if (method == 1 && tau_scale_mode == 0) {
        em_0jet_high_Up->Write();
        em_0jet_high_Down->Write();
      }
    }
   if (channel == channel::mt && method == 2 && tau_scale_mode == 0) {
        h2->Write();
        h3->Write();
    }

    dc_qcd->Write();


    TH1F *data_obs = (TH1F*)data_hist_clean->Clone();
    if (method == 3 && channel == channel::et && !is_2012) CleanBinsUpTo(data_obs, boost_high_clean);
    /* 
    if (blind) {
       double data_err = 0.0;
       double data_tot = data_obs->IntegralAndError(0, data_obs->GetNbinsX() + 1, data_err);
       data_obs->Reset();
       data_obs->SetBinContent(1, data_tot);
       data_obs->SetBinError(1, data_err);
     }
     */
    data_obs->SetName("data_obs");
    data_obs->SetTitle("data_obs");
    if (tau_scale_mode == 0) data_obs->Write();
    datacard.Close();
  }

  if (draw_ss) {
    data_shape = data_shape_ss;
    qcd_shape = qcd_shape_ss;
    ztt_shape = ztt_shape_ss;
    zll_shape = zll_shape_ss;
    top_shape = top_shape_ss;
    vv_shape = vv_shape_ss;
    w_shape = w_shape_ss;
  }

  ic::Plot plot;
  if (official_style) plot.use_htt_style = true;
  std::string kind_label = "sm";
  if (mssm_mode > 0) kind_label = "mssm";

  plot.output_filename = plot_name + "_" + category + "_" + channel_str  + "_" + kind_label + "_" + year_label + (draw_ss ? "_ss" : "") + ".pdf";
  if (log_y) plot.output_filename = plot_name + "_" + category + "_" +channel_str + "_" + kind_label + "_" + year_label + (draw_ss ? "_ss" : "")  + "_log.pdf";
  
  plot.x_bin_labels_ = x_axis_bin_labels;

  TH1F* signal_hist;
  if (mssm_mode == 0) {
    signal_hist = (TH1F*)(plots[Token("GluGluToHToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr()->Clone());
    if (verbose) {
      std::cout << "ggH Norm: " << Integral(plots[Token("GluGluToHToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr()) << std::endl;
      std::cout << "ggH Err: " << Error(plots[Token("GluGluToHToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr()) << std::endl;      
    }
    if (!signal_split_vbf) signal_hist->Add(plots[Token("VBF_HToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr());
    if (verbose) {
      std::cout << "qqH Norm: " << Integral(plots[Token("VBF_HToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr()) << std::endl;
      std::cout << "qqH Err: " << Error(plots[Token("VBF_HToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr()) << std::endl;      
    }
    signal_hist->Add(plots[Token("WH_ZH_TTH_HToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr());
    std::cout << boost::format(yield_fmt) % "Signal" % Integral(signal_hist) % Error(signal_hist);
    signal_hist->Scale(draw_signal_factor);
  } else {
    signal_hist = (TH1F*)(plots[Token("SUSYGluGluToHToTauTau_M-"+draw_mssm_signal_mass,cat,os_sel)].hist_ptr()->Clone());
    signal_hist->Add(plots[Token("SUSYBBHToTauTau_M-"+draw_mssm_signal_mass,cat,os_sel)].hist_ptr());
    signal_hist->Scale(draw_mssm_signal_factor);
  }
  ic::TH1PlotElement signal_shape("signal_shape", signal_hist);
  ic::TH1PlotElement signalvbf_shape;
  if (mssm_mode == 0) {
    signalvbf_shape =  ic::TH1PlotElement("signalvbf_shape", (TH1F*)plots[Token("VBF_HToTauTau_M-"+draw_signal_mass,cat,os_sel)].hist_ptr()->Clone());
    signalvbf_shape.hist_ptr()->Scale(draw_signal_factor);
  } 

  if (shift_backgrounds) {
    std::cout << "Shift ZTT: " << ztt_shift << std::endl;
    std::cout << "Shift QCD: " << qcd_shift << std::endl;
    std::cout << "Shift TOP: " << top_shift << std::endl;
    std::cout << "Shift ZL: "  << zl_shift << std::endl;
    std::cout << "Shift ZJ: "  << zj_shift << std::endl;
    std::cout << "Shift W: "   << w_shift << std::endl;
    std::cout << "Shift VV: "  << vv_shift << std::endl;
    top_shape.hist_ptr()->Scale(top_shift);
    qcd_shape.hist_ptr()->Scale(qcd_shift);
    w_shape.hist_ptr()  ->Scale(w_shift);
    vv_shape.hist_ptr() ->Scale(vv_shift);
    // zl_shape.hist_ptr()->Scale(zl_shift);
    // zj_shape.hist_ptr()->Scale(zj_shift);
    ztt_shape.hist_ptr()->Scale(ztt_shift);
  }


  if (channel == channel::mt || channel == channel::mtmet) w_shape.hist_ptr()->Add(zll_shape.hist_ptr());
  w_shape.hist_ptr()->Add(vv_shape.hist_ptr());




  SetStyle(signal_shape, kBlue+3);
  signal_shape.set_line_style(11);
  signal_shape.set_fill_color(0);
  signal_shape.set_line_color(kBlue+3);
  signal_shape.set_line_width(3);



  if (mssm_mode == 0) {
    SetStyle(signalvbf_shape, 1);
    signalvbf_shape.set_line_style(2);
    signalvbf_shape.set_fill_color(0);
    signalvbf_shape.set_line_color(1);
  }
  SetStyle(zll_shape, kAzure  + 2);
  SetStyle(qcd_shape, kMagenta-10);
  SetStyle(w_shape, kRed    + 2);
  SetStyle(ztt_shape, kOrange - 4);
  SetStyle(top_shape, kBlue   - 8);
  SetDataStyle(data_shape);
  if (signal_no_stack) {
    signal_shape.set_in_stack(false);
    signalvbf_shape.set_in_stack(false);
  }

  ic::TH1PlotElement ztt_1p0pz_shape("ztt_1p0pz_shape", NULL, "Z#rightarrow#tau#tau (1 Prong 0 #pi^{0})");
  ic::TH1PlotElement ztt_1p1pz_shape("ztt_1p1pz_shape", NULL, "Z#rightarrow#tau#tau (1 Prong 1 #pi^{0})");
  ic::TH1PlotElement ztt_3p_shape("ztt_3p_shape", NULL, "Z#rightarrow#tau#tau (3 Prong)");



  std::vector<ic::TH1PlotElement *> drawn_ele;
  drawn_ele.push_back(&qcd_shape);
  drawn_ele.push_back(&w_shape);
  drawn_ele.push_back(&ztt_shape);
  drawn_ele.push_back(&top_shape);

  if (ztt_by_decay_mode) {
    ztt_1p0pz_shape.set_hist_ptr((TH1F*)(plots[Token("Embedded1P0PZ",cat,os_sel)].hist_ptr()->Clone()));
    ztt_1p1pz_shape.set_hist_ptr((TH1F*)(plots[Token("Embedded1P1PZ",cat,os_sel)].hist_ptr()->Clone()));
    ztt_3p_shape.set_hist_ptr((TH1F*)(plots[Token("Embedded3P",cat,os_sel)].hist_ptr()->Clone()));
    double ztt_split_tot =  Integral(ztt_1p0pz_shape.hist_ptr()) + Integral(ztt_1p1pz_shape.hist_ptr()) + Integral(ztt_3p_shape.hist_ptr());
    double ztt_1p0pz_frac = Integral(ztt_1p0pz_shape.hist_ptr()) / ztt_split_tot;
    double ztt_1p1pz_frac = Integral(ztt_1p1pz_shape.hist_ptr()) / ztt_split_tot;
    double ztt_3p_frac = Integral(ztt_3p_shape.hist_ptr()) / ztt_split_tot;
    ztt_1p0pz_shape.hist_ptr()->Scale( ztt_1p0pz_frac * Integral(ztt_shape.hist_ptr()) /  Integral(ztt_1p0pz_shape.hist_ptr()) );
    ztt_1p1pz_shape.hist_ptr()->Scale( ztt_1p1pz_frac * Integral(ztt_shape.hist_ptr()) /  Integral(ztt_1p1pz_shape.hist_ptr()) );
    ztt_3p_shape.hist_ptr()->Scale( ztt_3p_frac * Integral(ztt_shape.hist_ptr()) /  Integral(ztt_3p_shape.hist_ptr()) );
    SetStyle(ztt_1p0pz_shape, kOrange + 2);
    SetStyle(ztt_1p1pz_shape, kOrange - 0);
    SetStyle(ztt_3p_shape, 17);

    drawn_ele.push_back(&ztt_1p0pz_shape);
    drawn_ele.push_back(&ztt_1p1pz_shape);
    drawn_ele.push_back(&ztt_3p_shape);
    plot.legend_left = 0.54;
  }


  if (channel == channel::et) drawn_ele.push_back(&zll_shape);
  if (draw_signal) drawn_ele.push_back(&signal_shape);
  if (draw_signal && signal_split_vbf) drawn_ele.push_back(&signalvbf_shape);


  drawn_ele.push_back(&data_shape);
  for (unsigned i = 0; i < drawn_ele.size(); ++i) {
  //   //drawn_ele[i]->set_rebin_factor(rebin);
     drawn_ele[i]->set_line_width(2);
     if (norm_bins) drawn_ele[i]->hist_ptr()->Scale(1.0, "width");
  }
  if (signal_no_stack) {
    signalvbf_shape.set_line_width(3);
    signal_shape.set_line_width(3);
  }


  qcd_shape.set_legend_text("QCD");
  if (channel == channel::em) qcd_shape.set_legend_text("fakes");
  w_shape.set_legend_text("electroweak");
  data_shape.set_legend_text("observed");
  ztt_shape.set_legend_text("Z#rightarrow#tau#tau");
  if (use_ztt_mc) ztt_shape.set_legend_text("Z#rightarrow#tau#tau (MC)");
  zll_shape.set_legend_text("Z#rightarrowee");
  top_shape.set_legend_text("t#bar{t}");
  if (mssm_mode == 0) {
    signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau");
    if (signal_split_vbf) signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau (ggH+VH)");
    if (signal_split_vbf) signalvbf_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau (qqH)");
  } else {
    signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_mssm_signal_factor)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_mssm_signal_mass+",tan#beta=8)");
  }
  if (channel == channel::et || channel == channel::mt || channel == channel::mtmet) {
    // top_shape.hist_ptr()->SetTitleSize  (0.055,"Y");
    // top_shape.hist_ptr()->SetTitleOffset(1.600,"Y");
    // top_shape.hist_ptr()->SetLabelOffset(0.014,"Y");
    // top_shape.hist_ptr()->SetLabelSize  (0.040,"Y");
    // top_shape.hist_ptr()->SetLabelFont  (42   ,"Y");
    // top_shape.hist_ptr()->SetTitleSize  (0.055,"X");
    // top_shape.hist_ptr()->SetTitleOffset(1.300,"X");
    // top_shape.hist_ptr()->SetLabelOffset(0.014,"X");
    // top_shape.hist_ptr()->SetLabelSize  (0.040,"X");
    // top_shape.hist_ptr()->SetLabelFont  (42   ,"X");
    // top_shape.hist_ptr()->SetMarkerStyle(20);
    // // top_shape.hist_ptr()->SetMarkerColor(color);
    // top_shape.hist_ptr()->SetMarkerSize (0.6);
    // top_shape.hist_ptr()->GetYaxis()->SetTitleFont(42);
    // top_shape.hist_ptr()->GetXaxis()->SetTitleFont(42);
    plot.AddTH1PlotElement(qcd_shape);
    plot.AddTH1PlotElement(top_shape);
    plot.AddTH1PlotElement(w_shape);
  } else if (channel == channel::em) {
    plot.AddTH1PlotElement(qcd_shape);
    plot.AddTH1PlotElement(w_shape);
    plot.AddTH1PlotElement(top_shape);
  }
  if (channel == channel::et) plot.AddTH1PlotElement(zll_shape);
  if (!ztt_by_decay_mode) {
    plot.AddTH1PlotElement(ztt_shape);
  } else {
    plot.AddTH1PlotElement(ztt_1p0pz_shape);    
    plot.AddTH1PlotElement(ztt_1p1pz_shape);    
    plot.AddTH1PlotElement(ztt_3p_shape);    
  }


  if (!draw_ss) {
    if (draw_signal) plot.AddTH1PlotElement(signal_shape);
    if (draw_signal && signal_split_vbf) plot.AddTH1PlotElement(signalvbf_shape);
  }
  plot.AddTH1PlotElement(data_shape);
  plot.extra_pad = extra_pad;
  plot.custom_x_axis_range = custom_x_axis_range;
  if (custom_x_axis_range){
    plot.x_axis_min = x_axis_min;
    plot.x_axis_max = x_axis_max;
  }
  plot.legend_height = 0.045;
  if (mssm_mode == 1) plot.legend_height = 0.045;
  plot.x_axis_title = x_axis_label;
  plot.y_axis_title = "Events";
  if (norm_bins) plot.y_axis_title = "dN/dm_{#tau#tau} [1/GeV]";
  plot.title_left = "CMS Preliminary " + year_label +", #sqrt{s} = " + (is_2012 ? "8":"7") +" TeV, "+ lumi_data_label;
  if (official_style) plot.title_left = std::string("CMS Preliminary, #sqrt{s} = ") + (is_2012 ? "8":"7") + " TeV, L = "+ lumi_data_label;
  string cat_app = show_category ? (", "+category) : "";
  if (channel == channel::et) plot.title_right = "e#tau_{h}"+cat_app;
  if (channel == channel::mt) plot.title_right = "#mu#tau_{h}"+cat_app;
  if (channel == channel::mtmet) plot.title_right = "#mu#tau_{h}+MET"+cat_app;
  if (channel == channel::em) plot.title_right = "e#mu"+cat_app;
  if (mssm_mode == 1) plot.legend_left = 0.45;
  if (official_style) plot.title_right = "";

  plot.draw_ratio_hist = draw_ratio;
  plot.draw_signif = false;

  string background_list = "top_shape+ztt_shape+w_shape+qcd_shape";
  if (ztt_by_decay_mode) background_list = "top_shape+ztt_3p_shape+ztt_1p1pz_shape+ztt_1p0pz_shape+w_shape+qcd_shape";
  if (channel == channel::et) background_list += "+zll_shape";
  ic::RatioPlotElement ratio("Mug","data_shape",background_list);
  
  plot.band_size_fractional_ = band_size_fractional;
  plot.draw_band_on_stack_ = draw_band_on_stack;
  plot.samples_for_band_ = "top_shape+ztt_shape+w_shape+qcd_shape";
  if (ztt_by_decay_mode) plot.samples_for_band_ = "top_shape+ztt_3p_shape+ztt_1p1pz_shape+ztt_1p0pz_shape+w_shape+qcd_shape";
  if (channel == channel::et) plot.samples_for_band_ += "+zll_shape";

  SetStyle(ratio,1);
  ratio.set_multi_mode(true);
  //plot.ratio_y_axis_title = "#frac{(N_{obs}-N_{exp})}{#sqrt{N_{obs}}}";
  plot.ratio_y_axis_title = "Data/MC";
  plot.AddRatioPlotElement(ratio);
  plot.custom_ratio_y_axis_range = true;
  plot.ratio_y_axis_min = 0.5;
  plot.ratio_y_axis_max = 1.5;
  
  if (log_y) plot.y_axis_log = true;
  if (y_axis_min > 0) plot.y_axis_min = y_axis_min;
  
  // // plot.y_axis_min = 5.0;
  // if (blind) {
  //   agilbert::TextElement text1("Data in ["+boost::lexical_cast<std::string>(x_blind_min)+","+boost::lexical_cast<std::string>(x_blind_max)+"] blinded",0.03, 0.16,0.89);
  //   plot.AddTextElement(text1);
  // }
  // plot.y_axis_min = 5.0;
  if (official_style) {
    string ch_label;
    if (channel == channel::et) ch_label = "e#tau_{h}"+cat_app;
    if (channel == channel::mt) ch_label = "#mu#tau_{h}"+cat_app;
    if (channel == channel::mtmet) ch_label = "#mu#tau_{h}+MET"+cat_app;
    if (channel == channel::em) ch_label = "e#mu"+cat_app;
    ic::TextElement text1(ch_label,0.055, 0.21,0.84);
    plot.AddTextElement(text1);
  }
  ic::TextElement ss_text("Same-sign",0.0,0.19,0.89);
  if (draw_ss) plot.AddTextElement(ss_text);

  if (add_text.size() > 0) {
    std::vector<string> tmp_vec;
    boost::split(tmp_vec, add_text, boost::is_any_of(","));
    if (tmp_vec.size() == 4) {
      ic::TextElement tmp_text(tmp_vec[0], boost::lexical_cast<double>(tmp_vec[1]), boost::lexical_cast<double>(tmp_vec[2]) ,boost::lexical_cast<double>(tmp_vec[3]));
      plot.AddTextElement(tmp_text);
    }
  }


  if (!no_plot) plot.GeneratePlot();

  return 0;
}

