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

namespace po = boost::program_options;

using namespace std;
using namespace ic;

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
  unsigned bins = hist->GetNbinsX();
  unsigned first_populated = 0;
  unsigned last_populated = 0;
  for (unsigned i = 1; i <= bins; ++i) {
    if (hist->GetBinContent(i) > 0. && first_populated == 0) first_populated = i;
    if (hist->GetBinContent(bins-(i-1)) > 0. && last_populated == 0) last_populated = bins-(i-1);
  }
  if (last_populated <= first_populated) {
    if (verbose) std::cout << "Cannot correct this distribution!" << std::endl;
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


void SetDataStyle(ic::TH1PlotElement & ele, unsigned color) {
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

  //ele.set_draw_normalised(true);

  //ele.set_in_stack(true);
  return;
}

void SetStyle(ic::RatioPlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  return;
}


int main(int argc, char* argv[]){

  std::string configfile, paramfile, selection, plot_name;
  paramfile = "scripts/default_params_2011.dat";
  unsigned mode = 0;
  unsigned mssm_mode = 0;
  unsigned category = 0;
  bool is_2012 = false;
  std::string x_axis_label;
  unsigned rebin;
  bool custom_x_axis_range = true;
  double x_axis_min, x_axis_max;
  double extra_pad;
  bool blind;
  double x_blind_min, x_blind_max;
  bool no_plot;
  bool make_datacard;
  bool log_y = false;
  bool verbose = false;
  bool non_mass_plot = false;
  string sel;
  std::string folder = ".";
  bool norm_bins = false;
  bool signal_no_stack = false;
  bool signal_split_vbf = false;
  double y_axis_min = -10;
  unsigned tau_scale_mode = 0;
  bool draw_ss = false;
  bool swap_inclusive = false;

  double qcd_shift = 1.0;
  double top_shift = 1.0;
  double ztt_shift = 1.0;
  double zll_shift = 1.0;
  double w_shift = 1.0;
  double vv_shift = 1.0;
  bool shift_backgrounds = false;
  double band_size_fractional = 0.0;
  bool draw_band_on_stack = true;
  bool use_ztt_mc = false;


 // 00      $CHANNEL_incl_low             incl  pt-low
 // 01      $CHANNEL_incl_high            incl  pt-high
 // 02      $CHANNEL_boost_low            boost pt-low
 // 03      $CHANNEL_boost_high           boost pt-high
 // 04      $CHANNEL_2jet                 2jet
 // 05      $CHANNEL_vbf                  vbf
 // 06      $CHANNEL_btag_low             btag  pt-low
 // 07      $CHANNEL_btag_high            btag  pt-high
 // 08 inclusive!!
 // 09 dilepton!!
// 10 twojet!!
 // 11 nobtag
 // 12 btag



  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()
      ("cfg", po::value<std::string>(&configfile)->required(), 
            "configuration file");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);

  po::options_description config("Configuration");
  config.add_options()
    ("mode",                po::value<unsigned>(&mode)->required(), "0=elec-tau, 1=mu-tau, 2=elec-mu")
    ("mssm_mode",           po::value<unsigned>(&mssm_mode)->required(), "0=no, 1=yes")
    ("category",            po::value<unsigned>(&category)->required(), "as above")
    ("is_2012",             po::value<bool>(&is_2012)->required(), "0=2011, 1=2012")
    ("plot_name",           po::value<std::string>(&plot_name)->required(), "plot_name")
    ("x_axis_label",        po::value<std::string>(&x_axis_label)->required(), "x_axis_label")
    ("rebin",               po::value<unsigned>(&rebin)->default_value(1), "rebin")
    ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0), "tau_scale_mode")
    ("custom_x_axis_range", po::value<bool>(&custom_x_axis_range)->default_value(false), "custom_x_axis_range")
    ("x_axis_min",          po::value<double>(&x_axis_min)->default_value(0), "rebin")
    ("x_axis_max",          po::value<double>(&x_axis_max)->default_value(0), "rebin")
    ("y_axis_min",          po::value<double>(&y_axis_min)->default_value(-10), "rebin")
    ("extra_pad",           po::value<double>(&extra_pad)->default_value(1.0), "extra_pad")
    ("blind",               po::value<bool>(&blind)->default_value(false), "blind")
    ("x_blind_min",         po::value<double>(&x_blind_min)->default_value(0), "x_blind_min")
    ("x_blind_max",         po::value<double>(&x_blind_max)->default_value(0), "x_blind_max")
    ("no_plot",             po::value<bool>(&no_plot)->default_value(false), "no_plot")
    ("make_datacard",       po::value<bool>(&make_datacard)->default_value(false), "make_datacard")
    ("log_y",               po::value<bool>(&log_y)->default_value(false), "log_y")
    ("non_mass_plot",       po::value<bool>(&non_mass_plot)->default_value(false), "non_mass_plot")
    ("swap_inclusive",      po::value<bool>(&swap_inclusive)->default_value(false), "swap_inclusive")
    ("norm_bins",           po::value<bool>(&norm_bins)->default_value(false), "norm_bins")
    ("verbose",             po::value<bool>(&verbose)->default_value(false), "verbose")
    ("use_ztt_mc",          po::value<bool>(&use_ztt_mc)->default_value(false), "use_ztt_mc")
    ("signal_no_stack",     po::value<bool>(&signal_no_stack)->default_value(false), "signal_no_stack")
    ("shift_backgrounds",   po::value<bool>(&shift_backgrounds)->default_value(false), "shift_backgrounds")
    ("signal_split_vbf",    po::value<bool>(&signal_split_vbf)->default_value(false), "signal_split_vbf")
    ("draw_ss",             po::value<bool>(&draw_ss)->default_value(false), "draw_ss")
    ("draw_band_on_stack",  po::value<bool>(&draw_band_on_stack)->default_value(false), "draw_band_on_stack")
    ("qcd_shift",           po::value<double>(&qcd_shift)->default_value(1.0), "qcd_shift")
    ("top_shift",           po::value<double>(&top_shift)->default_value(1.0), "top_shift")
    ("ztt_shift",           po::value<double>(&ztt_shift)->default_value(1.0), "ztt_shift")
    ("zll_shift",           po::value<double>(&zll_shift)->default_value(1.0), "zll_shift")
    ("w_shift",           po::value<double>(&w_shift)->default_value(1.0), "w_shift")
    ("vv_shift",           po::value<double>(&vv_shift)->default_value(1.0), "vv_shift")
    ("band_size_fractional", po::value<double>(&band_size_fractional)->default_value(0.0), "band_size_fractional")
    ("folder",              po::value<std::string>(&folder)->required(), "folder")

    ;
  po::store(po::command_line_parser(argc, argv).
      options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(configfile.c_str(), config), vm);
  po::notify(vm);

  if (is_2012) paramfile = "scripts/default_params_2012.dat";

  // Parse the parameter file
  SimpleParamParser parser;
  std::cout << "Parsing parameter file: " << paramfile << std::endl;
  parser.ParseFile(paramfile);

  // The signal files that we'll attempt to open, and which will be added to the datacard if produced
  vector<string> signal_masses, mssm_signal_masses;
  std::string signal_masses_list = parser.GetParam<string>("SIGNAL_MASSES");
  std::string mssm_signal_masses_list = parser.GetParam<string>("MSSM_SIGNAL_MASSES");
  boost::split(signal_masses, signal_masses_list, boost::is_any_of(","));
  boost::split(mssm_signal_masses, mssm_signal_masses_list, boost::is_any_of(","));
  bool draw_signal = parser.GetParam<bool>("DRAW_SIGNAL");
  std::string draw_signal_mass = parser.GetParam<string>("DRAW_SIGNAL_MASS");
  std::string draw_mssm_signal_mass = parser.GetParam<string>("DRAW_MSSM_SIGNAL_MASS");
  unsigned draw_signal_factor = parser.GetParam<unsigned>("DRAW_SIGNAL_FACTOR");
  unsigned draw_mssm_signal_factor = parser.GetParam<unsigned>("DRAW_MSSM_SIGNAL_FACTOR");
  std::string mode_label;
  if (mode == 0) mode_label = "ElecTau";
  if (mode == 1) mode_label = "MuTau";
  if (mode == 2) mode_label = "ElecMu";

  string year_label = parser.GetParam<string>("YEAR_LABEL");
  double data_lumi = parser.GetParam<double>("LUMI_DATA");
  string lumi_data_label = parser.GetParam<string>("LUMI_DATA_LABEL");


  vector<string> files;
  if (mode == 0 || mode == 1) {
    files.push_back("Data");
    files.push_back("Special_3_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
    files.push_back("Special_6_Data"); // QCD shape VBF
    files.push_back("Special_7_Data"); // VBF Norm (of W+Jets)
    //files.push_back("Special_10_Data"); // VBF Norm (of W+Jets)
    //files.push_back("Special_16_Data"); // VBF Norm (of W+Jets)
    files.push_back("DYJetsToTauTau"); 
    files.push_back("Special_7_DYJetsToTauTau"); 
    files.push_back("Embedded");
    files.push_back("DYJetsToLL");
    files.push_back("DYJetsToLL-L");
    files.push_back("DYJetsToLL-J");
    files.push_back("Special_7_DYJetsToLL");    // For VBF W sideband
    files.push_back("Special_7_DYJetsToLL-L");  // For VBF W sideband
    files.push_back("Special_7_DYJetsToLL-J");  // For VBF W sideband
    files.push_back("Special_9_DYJetsToLL");    // For b-jet shape
    files.push_back("Special_9_DYJetsToLL-L");  // For b-jet shape
    files.push_back("Special_18_DYJetsToLL-L");  // For b-jet shape
    files.push_back("Special_9_DYJetsToLL-J");  // For b-jet shape
    files.push_back("WJetsToLNuSoup");
    files.push_back("Special_7_WJetsToLNuSoup"); // For VBF Norm and extrapolation factor
    // files.push_back("Special_8_WJetsToLNuSoup"); // For B Jet Norm (just extrapolation factor)
    // files.push_back("Special_9_WJetsToLNuSoup"); // For B Jet Shape
    files.push_back("WWJetsTo2L2Nu");
    files.push_back("WZJetsTo2L2Q");
    files.push_back("WZJetsTo3LNu");
    files.push_back("ZZJetsTo2L2Nu");
    files.push_back("ZZJetsTo2L2Q");
    files.push_back("ZZJetsTo4L");
    files.push_back("T-tW");
    files.push_back("Tbar-tW");
    files.push_back("Special_7_WWJetsTo2L2Nu");   // For VV shapes VBF and BJet*
    files.push_back("Special_7_WZJetsTo2L2Q");    // For VV shapes VBF and BJet
    files.push_back("Special_7_WZJetsTo3LNu");    // For VV shapes VBF and BJet
    files.push_back("Special_7_ZZJetsTo2L2Nu");   // For VV shapes VBF and BJet
    files.push_back("Special_7_ZZJetsTo2L2Q");    // For VV shapes VBF and BJet
    files.push_back("Special_7_ZZJetsTo4L");      // For VV shapes VBF and BJet
    files.push_back("Special_7_T-tW");
    files.push_back("Special_7_Tbar-tW");
    // files.push_back("Special_9_WWJetsTo2L2Nu");   // For VV shapes VBF and BJet*
    // files.push_back("Special_9_WZJetsTo2L2Q");    // For VV shapes VBF and BJet
    // files.push_back("Special_9_WZJetsTo3LNu");    // For VV shapes VBF and BJet
    // files.push_back("Special_9_ZZJetsTo2L2Nu");   // For VV shapes VBF and BJet
    // files.push_back("Special_9_ZZJetsTo2L2Q");    // For VV shapes VBF and BJet
    // files.push_back("Special_9_ZZJetsTo4L");      // For VV shapes VBF and BJet
    // files.push_back("Special_9_T-tW");
    // files.push_back("Special_9_Tbar-tW");
    files.push_back("TTJets");
    files.push_back("Special_7_TTJets");          // For VBF Norm (of W+Jets)
    if (is_2012) files.push_back("TT");
    if (is_2012) files.push_back("Special_7_TT");

  } else if (mode == 2) {
    files.push_back("Data");
    files.push_back("Special_14_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
    files.push_back("Special_15_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
    files.push_back("Special_17_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
    files.push_back("DYJetsToTauTau"); 
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

  vector<string> categories;
  //categories.push_back(""); // Whatever category we are actually plotting for
  categories.push_back("dilepton");
  categories.push_back("inclusive");
  categories.push_back("twojet");
  categories.push_back("vbfloose");
  categories.push_back("vbf");
  categories.push_back("onejet_highpt");
  categories.push_back("onejet_lowpt");
  categories.push_back("onebjet_highpt");
  categories.push_back("onebjet_lowpt");
  categories.push_back("onebjet_highpt_loose");
  categories.push_back("onebjet_lowpt_loose");
  categories.push_back("zerojet_highpt");
  categories.push_back("zerojet_lowpt");
  if (mssm_mode > 0) {
  categories.push_back("btag");
  categories.push_back("btag_loose");
  categories.push_back("nobtag");    
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

  if (tau_scale_mode == 1) folder += "/TSCALE_DOWN";
  if (tau_scale_mode == 2) folder += "/TSCALE_UP";

  std::map<std::string, TFile *> tfiles;
  for (unsigned i = 0; i < files.size(); ++i) {
    std::string filename = (files[i]+"_"+mode_label+"_"+year_label+".root");
    TFile * tmp = new TFile((folder+"/"+filename).c_str());
    if (!tmp) {
      //std::cerr << "Error, file " << filename << " could not be opened, quitting." << std::endl;
      //exit(1);
      std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
    } else {
      tfiles[files[i]] = tmp;      
    }
  }

  if (category == 0) sel = "zerojet_lowpt";
  if (category == 1) sel = "zerojet_highpt";
  if (category == 2) sel = "onejet_lowpt";
  if (category == 3) sel = "onejet_highpt";
  if (category == 4) sel = "vh";
  if (category == 5) sel = "vbf";
  if (category == 6) sel = "onebjet_lowpt";
  if (category == 7) sel = "onebjet_highpt";
  if (category == 8) sel = "inclusive";
  if (category == 9) sel = "dilepton";
  if (category == 10) sel = "twojet";
  if (category == 11) sel = "nobtag";
  if (category == 12) sel = "btag";


  // Get Plots and Scale
  std::map<std::string, ic::TH1PlotElement> os_sel;
  std::map<std::string, ic::TH1PlotElement> ss_sel;
  std::map<std::string, ic::TH1PlotElement> os_con;
  std::map<std::string, ic::TH1PlotElement> ss_con;
  for (unsigned i = 0; i < files.size(); ++i) {
    for (unsigned j = 0; j < categories.size(); ++j) {
      string f = files[i];
      string c = categories[j];
      std::string nm = files[i]+categories[j];
      if (categories[j] == "") {
        nm = files[i];
        c = sel;
      }
      if (tfiles[f]->GetDirectory(("/"+c+"_os_sel").c_str())) {
        //std::cout << nm << std::endl;
        os_sel[nm] = ic::TH1PlotElement(nm+"_os_sel", tfiles[f], "/"+c+"_os_sel", plot_name);
        ss_sel[nm] = ic::TH1PlotElement(nm+"_ss_sel", tfiles[f], "/"+c+"_ss_sel", plot_name);
        os_con[nm] = ic::TH1PlotElement(nm+"_os_con", tfiles[f], "/"+c+"_os_con", plot_name);
        ss_con[nm] = ic::TH1PlotElement(nm+"_ss_con", tfiles[f], "/"+c+"_ss_con", plot_name);
        if (os_sel[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
        if (ss_sel[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
        if (os_con[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
        if (ss_con[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
        string lookup;
        if (f.find("Special") != f.npos) {
          size_t found = f.find('_');
          found = f.find('_',found+1);
          lookup = f.substr(found+1);
        } else {
          lookup = f;
        }
        double sample_events = parser.GetParam<double>("EVT_"+lookup);
        double sample_xs = parser.GetParam<double>("XS_"+lookup);
        double sample_lumi = sample_events / sample_xs;
        if (sample_xs > 0) {
          //std::cout << "Scale: " << nm << " EVT: " << sample_events << " XS: " << sample_xs << std::endl;
          os_sel[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
          ss_sel[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
          os_con[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
          ss_con[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
        }
        os_sel[nm].hist_ptr()->Rebin(rebin);
        ss_sel[nm].hist_ptr()->Rebin(rebin);
        os_con[nm].hist_ptr()->Rebin(rebin);
        ss_con[nm].hist_ptr()->Rebin(rebin); 
      }
    }
  }

  std::cout << "******** Category: " << category << " (" << sel << ")" << std::endl;
  std::cout << "****** Folder: " << folder << "  Mode: " << mode << "  TSCALE: " << tau_scale_mode << std::endl;
  std::cout << "Non Mass Plot Mode: " << non_mass_plot << std::endl;
  if (swap_inclusive) std::cout << "!!!! Warning: swapping inclusive with 0jet_low !!!!" << std::endl;
  if (use_ztt_mc) std::cout << "!!!! Warning: USING ZTT MC INSTEAD OF EMBEDDED !!!!" << std::endl;
  // ---------------------------------------------------
  // DO DATA
  // ---------------------------------------------------
  TH1F *data_hist_clean = (TH1F*)(os_sel["Data"+sel].hist_ptr()->Clone());
  if (blind) {
    for (int j = 0; j < os_sel["Data"+sel].hist_ptr()->GetNbinsX(); ++j) {
      double low_edge = os_sel["Data"+sel].hist_ptr()->GetBinLowEdge(j+1);
      double high_edge = os_sel["Data"+sel].hist_ptr()->GetBinWidth(j+1)+os_sel["Data"+sel].hist_ptr()->GetBinLowEdge(j+1);
      if ((low_edge > x_blind_min && low_edge < x_blind_max) || (high_edge > x_blind_min && high_edge < x_blind_max)) {
        os_sel["Data"+sel].hist_ptr()->SetBinContent(j+1,0);
        os_sel["Data"+sel].hist_ptr()->SetBinError(j+1,0);
      }
    }
  }
  double data_norm = Integral(data_hist_clean);
  TH1F *data_hist = (TH1F*)(os_sel["Data"+sel].hist_ptr()->Clone());
  TH1F *data_hist_ss = (TH1F*)(ss_sel["Data"+sel].hist_ptr()->Clone());
  ic::TH1PlotElement data_shape("data_shape", data_hist);
  ic::TH1PlotElement data_shape_ss("data_shape_ss", data_hist_ss);

  std::cout << "Dat Norm: " << data_norm << std::endl;


  // ---------------------------------------------------
  // Generate TOP Norm and TOP Shape
  // ---------------------------------------------------
  if (verbose) cout << "**** TOP: " << endl;

  // Norm is the same for all methods
  double top_norm = Integral(os_sel["TTJets"+sel].hist_ptr());
  TH1F *top_hist;

  double top_norm_ss = Integral(ss_sel["TTJets"+sel].hist_ptr());
  TH1F *top_hist_ss = (TH1F*)(ss_sel[(is_2012 ? "TT" : "TTJets")+sel].hist_ptr()->Clone());

  if (mode == 0 || mode == 1) {
    // Take normal selection for 0/1 Jet and dilepton, inclusive and twojet
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 7) top_hist = (TH1F*)(os_sel[(is_2012 ? "TT" : "TTJets")+sel].hist_ptr()->Clone());
    // Take Special Mode 2, relaxed selection for B Jet
    if (category == 6 || category == 7) top_hist = (TH1F*)(os_sel[(is_2012 ? "TT" : "TTJets")+sel].hist_ptr()->Clone());
    //if (category == 6 || category == 7) top_hist = (TH1F*)(os_sel["Special_2_TTJets"+sel].hist_ptr()->Clone());
    // Take twojet shape from Special Mode 5 for VBF
    //if (category == 5) top_hist = (TH1F*)(os_sel["TTJetstwojet"].hist_ptr()->Clone());
    if (category == 5) top_hist = (TH1F*)(os_sel[std::string(is_2012 ? "TT" : "TTJets")+"vbfloose"].hist_ptr()->Clone());
    //if (category == 5) top_hist = (TH1F*)(os_sel["Special_2_TTJetstwojet"].hist_ptr()->Clone());
  } else if (mode == 2) {
    // For e-mu, just take the normal shape (to be replaced with 42X MC shape eventually)
    top_hist = (TH1F*)(os_sel["TT"+sel].hist_ptr());
  }
  top_hist->Scale(top_norm / Integral(top_hist));
  ic::TH1PlotElement top_shape("top_shape", top_hist);
  top_hist_ss->Scale(top_norm_ss / Integral(top_hist_ss));
  ic::TH1PlotElement top_shape_ss("top_shape_ss", top_hist_ss);

  std::cout << "Top Norm: " << top_norm << std::endl;

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
  for (unsigned i = 0; i < vv_samples.size(); ++i) vv_norm += Integral(os_sel[vv_samples[i]+sel].hist_ptr());
  TH1F *vv_hist;

  // !!! Do SS !!!
  double vv_norm_ss = 0.0;
  for (unsigned i = 0; i < vv_samples.size(); ++i) vv_norm_ss += Integral(ss_sel[vv_samples[i]+sel].hist_ptr());
  TH1F *vv_hist_ss =(TH1F*)(ss_sel[vv_samples[0]+sel].hist_ptr()->Clone());
  for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist_ss->Add(ss_sel[vv_samples[i]+sel].hist_ptr());  


  if (mode == 0 || mode == 1) {
    // For 0/1 jet and dilepton, inclusive and twojet take normal shape
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 7) {
      vv_hist = (TH1F*)(os_sel[vv_samples[0]+sel].hist_ptr()->Clone());
      for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel[vv_samples[i]+sel].hist_ptr());  
    }
    // For VBF take Special Mode 2, relaxed isolation
    if (category == 5) {
      vv_hist = (TH1F*)(os_sel[vv_samples[0]+"vbfloose"].hist_ptr()->Clone());
      for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel[vv_samples[i]+"vbfloose"].hist_ptr());
      //vv_hist = (TH1F*)(os_sel[vv_samples[0]+"twojet"].hist_ptr()->Clone());
      //for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel[vv_samples[i]+"twojet"].hist_ptr());
      //vv_hist = (TH1F*)(os_sel["Special_2_"+vv_samples[0]+"twojet"].hist_ptr()->Clone());
      //for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel["Special_2_"+vv_samples[i]+"twojet"].hist_ptr());
    }
    // For B Jet take Special Mode 2, relaxed isolation (option to use 10)
    if (category == 6 || category == 7) {
      vv_hist = (TH1F*)(os_sel[vv_samples[0]+sel].hist_ptr()->Clone());
      for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel[vv_samples[i]+sel].hist_ptr());
      //vv_hist = (TH1F*)(os_sel["Special_2_"+vv_samples[0]+sel].hist_ptr()->Clone());
      //for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel["Special_2_"+vv_samples[i]+sel].hist_ptr());

    }
  } else if (mode == 2) {
    if (category == 0 || category == 1) vv_norm *= 1.23;
    vv_hist = (TH1F*)(os_sel[vv_samples[0]+sel].hist_ptr()->Clone());
    for (unsigned i = 1; i < vv_samples.size(); ++i) vv_hist->Add(os_sel[vv_samples[i]+sel].hist_ptr());
  }


  vv_hist->Scale( vv_norm / Integral(vv_hist) );
  ic::TH1PlotElement vv_shape("vv_shape", vv_hist);
  vv_hist_ss->Scale( vv_norm_ss / Integral(vv_hist_ss) );
  ic::TH1PlotElement vv_shape_ss("vv_shape_ss", vv_hist_ss);

  std::cout << "Diboson Norm: " << vv_norm << std::endl;

  // ---------------------------------------------------
  // Generate ZLL, ZL and ZJ Norm and Shapes
  // ---------------------------------------------------
  if (verbose) cout << "**** ZLL: " << endl;

  double zll_norm, zl_norm, zj_norm;
  TH1F *zll_hist, *zl_hist, *zj_hist;

  TH1F *zll_hist_ss = (TH1F*)(ss_sel["DYJetsToLL"+sel].hist_ptr()->Clone());
  double zll_norm_ss = Integral(ss_sel["DYJetsToLL"+sel].hist_ptr());

  if (mode == 0 || mode == 1) {
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 5) {
      if (verbose) {
        std::cout << "ZLL Inclusive: " << Integral(os_sel["DYJetsToLLinclusive"].hist_ptr()) << std::endl;
        std::cout << "Embedded Inclusive->Category Eff: " << (Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr())) << std::endl;
        std::cout << "ZLL Extrap. Category: " << Integral(os_sel["DYJetsToLLinclusive"].hist_ptr()) * Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr()) << std::endl;
        std::cout << "[MC Norm in Category]: " << Integral(os_sel["DYJetsToLL"+sel].hist_ptr()) << " +/- " << Error(os_sel["DYJetsToLL"+sel].hist_ptr()) << std::endl;
      }
      zll_norm = Integral(os_sel["DYJetsToLL"+sel].hist_ptr());
      zl_norm = Integral(os_sel["DYJetsToLL-L"+sel].hist_ptr());
      zj_norm = Integral(os_sel["DYJetsToLL-J"+sel].hist_ptr());
      if (zll_norm > 0) {
        double zl_norm_new = zll_norm * (zl_norm/(zl_norm+zj_norm));
        double zj_norm_new = zll_norm * (zj_norm/(zl_norm+zj_norm));
        zl_norm = zl_norm_new;
        zj_norm = zj_norm_new;
      }
      zll_hist = (TH1F*)(os_sel["DYJetsToLL"+sel].hist_ptr()->Clone());
      zl_hist = (TH1F*)(os_sel["DYJetsToLL-L"+sel].hist_ptr()->Clone());
      if (category == 3 && !is_2012 && mode == 0) zl_hist = (TH1F*)(os_sel["Special_18_DYJetsToLL-L"+sel].hist_ptr()->Clone());
      zj_hist = (TH1F*)(os_sel["DYJetsToLL-J"+sel].hist_ptr()->Clone());
    }
    if (category == 5 /*|| category == 6 || category == 7*/) {
      //zll_norm = Integral(os_sel["DYJetsToLLinclusive"].hist_ptr()) * Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr());
      std::cout << "ZLL Inclusive: " << Integral(os_sel["DYJetsToLLinclusive"].hist_ptr()) << std::endl;
      std::cout << "Embedded Inclusive->Category Eff: " << (Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr())) << std::endl;
      std::cout << "ZLL Extrap. Category: " << Integral(os_sel["DYJetsToLLinclusive"].hist_ptr()) * Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr()) << std::endl;
      std::cout << "[MC Norm in Category]: " << Integral(os_sel["DYJetsToLL"+sel].hist_ptr()) << " +/- " << Error(os_sel["DYJetsToLL"+sel].hist_ptr()) << std::endl;
      zll_norm = Integral(os_sel["DYJetsToLLinclusive"].hist_ptr()) * Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr()); 
      zl_norm = Integral(os_sel["DYJetsToLL-Linclusive"].hist_ptr()) * Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr()); 
      zj_norm = Integral(os_sel["DYJetsToLL-Jinclusive"].hist_ptr()) * Integral(os_sel["Embedded"+sel].hist_ptr()) / Integral(os_sel["Embeddedinclusive"].hist_ptr()); 
    }
    if (category == 5) {
      // zll_norm = Integral(os_sel["DYJetsToLL"+sel].hist_ptr());
      // zl_norm = Integral(os_sel["DYJetsToLL-L"+sel].hist_ptr());
      // zj_norm = Integral(os_sel["DYJetsToLL-J"+sel].hist_ptr());
      zll_hist = (TH1F*)(os_sel["DYJetsToLLvbfloose"].hist_ptr()->Clone());
      zl_hist = (TH1F*)(os_sel["DYJetsToLL-Lvbfloose"].hist_ptr()->Clone());
      zj_hist = (TH1F*)(os_sel["DYJetsToLL-Jvbfloose"].hist_ptr()->Clone());
    }
    if (category == 6 || category == 7) {
      zll_hist = (TH1F*)(os_sel["Special_9_DYJetsToLL"+sel].hist_ptr()->Clone());
      zl_hist = (TH1F*)(os_sel["Special_9_DYJetsToLL-L"+sel].hist_ptr()->Clone());
      zj_hist = (TH1F*)(os_sel["Special_9_DYJetsToLL-J"+sel].hist_ptr()->Clone());
    }
  } else if (mode == 2) {
    zll_norm = 0.0;
    zl_norm = 0.0;
    zj_norm = 0.0;
    zll_hist = (TH1F*)(os_sel["DYJetsToTauTau"+sel].hist_ptr()->Clone());
    zl_hist = (TH1F*)(os_sel["DYJetsToTauTau"+sel].hist_ptr()->Clone());
    zj_hist = (TH1F*)(os_sel["DYJetsToTauTau"+sel].hist_ptr()->Clone());
  }

  zll_hist->Scale( zll_norm / Integral(zll_hist) );
  zll_hist_ss->Scale( zll_norm_ss / Integral(zll_hist_ss) );
  zl_hist->Scale( zl_norm / Integral(zl_hist) );
  zj_hist->Scale( zj_norm / Integral(zj_hist) );
  if (category == 3 && !is_2012 && mode == 0) {
    std::cout << "Relaxed shape taken for ZL, merging into ZLL!" << std::endl;
    zll_hist = (TH1F*)zl_hist->Clone();
    zll_hist->Add(zj_hist);
  }

  ic::TH1PlotElement zll_shape("zll_shape", zll_hist);
  ic::TH1PlotElement zll_shape_ss("zll_shape_ss", zll_hist_ss);
  ic::TH1PlotElement zl_shape("zl_shape", zl_hist);
  ic::TH1PlotElement zj_shape("zj_shape", zj_hist);

  std::cout << "ZLL Norm: " << zll_norm << std::endl;
  std::cout << "ZL Norm: " << zl_norm << std::endl;
  std::cout << "ZJ Norm: " << zj_norm << std::endl;

  // ---------------------------------------------------
  // Generate ZTT Shape and Norm
  // ---------------------------------------------------
  if (verbose) cout << "**** ZTT: " << endl;

  double ztt_norm;
  TH1F *ztt_hist;
  double ztt_mc_inc_yield = Integral(os_sel["DYJetsToTauTaudilepton"].hist_ptr());
  if (verbose) cout << "- DYJetsToTauTau dilepton OS yield (no mT cut): " << ztt_mc_inc_yield << std::endl;
  double embedded_eff = Integral(os_sel["Embedded"+sel].hist_ptr()) /  Integral(os_sel["Embeddeddilepton"].hist_ptr());
  double embedded_eff_err = (Error(os_sel["Embedded"+sel].hist_ptr())/Integral(os_sel["Embedded"+sel].hist_ptr()))*embedded_eff;
  std::cout << "Eff: " << embedded_eff << std::endl;
  std::cout << "Error: " << embedded_eff_err << std::endl;
  ztt_norm = ztt_mc_inc_yield * embedded_eff;
  //if (use_ztt_mc) ztt_norm = Integral(os_sel["DYJetsToTauTau"+sel].hist_ptr());
  if (verbose) cout << "- [DYJetsToTauTau MC Category Yield]: " << Integral(os_sel["DYJetsToTauTau"+sel].hist_ptr()) << " +/- " << Error(os_sel["DYJetsToTauTau"+sel].hist_ptr()) << endl;
  //e-tau, mu-tau
  if (mode == 0 || mode == 1) {
    //0/1 Jet, Inclusive
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 5) {
      if (use_ztt_mc) {
        ztt_hist = (TH1F*)(os_sel["DYJetsToTauTau"+sel].hist_ptr()->Clone());
      } else {
        ztt_hist = (TH1F*)(os_sel["Embedded"+sel].hist_ptr()->Clone());        
      }
    }
    // VBF
    if (category == 5) ztt_hist = (TH1F*)(os_sel["Embeddedvbfloose"].hist_ptr()->Clone());
    //if (category == 5) ztt_hist = (TH1F*)(os_sel["Embeddedtwojet"].hist_ptr()->Clone());
  //e-mu  
  } else if (mode == 2) {
    ztt_hist = (TH1F*)(os_sel["Embedded"+sel].hist_ptr()->Clone());
    //ztt_hist = (TH1F*)(os_sel["DYJetsToTauTau"+sel].hist_ptr()->Clone());
  }

  double embed_norm = Integral(os_sel["DYJetsToTauTaudilepton"].hist_ptr()) / Integral(os_sel["Embeddeddilepton"].hist_ptr());
  if (verbose) std::cout << "Embedded->Norm Scaling: " << embed_norm << std::endl; 
  ztt_hist->Scale( ztt_norm / Integral(ztt_hist) );
  ic::TH1PlotElement ztt_shape("ztt_shape", ztt_hist);

  std::cout << "ZTT Norm: " << ztt_norm << std::endl;
  // 5.3% (high), 1.5% (low)
  double ztt_norm_ss = Integral(ss_sel["Embedded"+sel].hist_ptr())*embed_norm;
  TH1F *ztt_hist_ss = (TH1F*)(ss_sel["Embedded"+sel].hist_ptr()->Clone());;
  ztt_hist_ss->Scale( ztt_norm_ss / Integral(ztt_hist_ss) );
  ic::TH1PlotElement ztt_shape_ss("ztt_shape_ss", ztt_hist_ss);

  if (category == 6 || category == 7) {
    double top_norm_diff = (category == 7) ? (0.053*ztt_norm) : (0.015*ztt_norm);
    std::cout << "TOP ZTT Subtraction fraction: " << (top_norm_diff/top_norm) << std::endl;
    top_norm = top_norm - top_norm_diff;
    top_shape.hist_ptr()->Scale(top_norm / Integral(top_shape.hist_ptr()));
  }

  if (category == 12) {
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
  double w_os_sel, w_os_con, w_ss_sel, w_ss_con;
  double w_ss_sel_inclusive;
  TH1F *w_hist = NULL;

  if (mode == 0 || mode == 1) {
    // 0/1 Jet, B Jet, dilepton, inclusive, twojet
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 5) {
      w_os_con = Integral(os_con["Data"+sel].hist_ptr());
      w_os_con -= Integral(os_con["TTJets"+sel].hist_ptr());
      double vv_os_tot = 0.0;
      for (unsigned i = 0; i < vv_samples.size(); ++i) {
        w_os_con -= Integral(os_con[vv_samples[i]+sel].hist_ptr());      
        vv_os_tot += Integral(os_con[vv_samples[i]+sel].hist_ptr());
      }
      w_os_con -= Integral(os_con["DYJetsToLL"+sel].hist_ptr());
      w_os_con -= Integral(os_con["DYJetsToTauTau"+sel].hist_ptr());
      if (verbose) std::cout << "OS Control Data: " << Integral(os_con["Data"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control TTBar: " << Integral(os_con["TTJets"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control VV: " << vv_os_tot << std::endl;
      if (verbose) std::cout << "OS Control DYJetsToLL: " << Integral(os_con["DYJetsToLL"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "[OS Control DYJetsToTauTau]: " << Integral(os_con["DYJetsToTauTau"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control Embedded: " << Integral(os_con["Embedded"+sel].hist_ptr())*embed_norm << std::endl;

      if (verbose) std::cout << "=> OS Control W: " << w_os_con << std::endl;
      
      w_ss_con = Integral(ss_con["Data"+sel].hist_ptr());
      w_ss_con -= Integral(ss_con["TTJets"+sel].hist_ptr());
      double vv_ss_tot = 0.0;
      for (unsigned i = 0; i < vv_samples.size(); ++i) {
        w_ss_con -= Integral(ss_con[vv_samples[i]+sel].hist_ptr());    
        vv_ss_tot += Integral(ss_con[vv_samples[i]+sel].hist_ptr());
      }
      w_ss_con -= Integral(ss_con["DYJetsToLL"+sel].hist_ptr());
      w_ss_con -= Integral(ss_con["DYJetsToTauTau"+sel].hist_ptr());
      if (verbose) std::cout << "SS Control Data: " << Integral(ss_con["Data"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "SS Control TTBar: " << Integral(ss_con["TTJets"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "SS Control VV: " << vv_ss_tot << std::endl;
      if (verbose) std::cout << "SS Control DYJetsToLL: " << Integral(ss_con["DYJetsToLL"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "SS Control DYJetsToTauTau: " << Integral(ss_con["DYJetsToTauTau"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "[SS Control Embedded]: " << Integral(ss_con["Embedded"+sel].hist_ptr())*embed_norm << std::endl;
      if (verbose) std::cout << "=> SS Control W: " << w_ss_con << std::endl;
    }
    // Extrapolation for 0/1 Jet, dilepton, inclusive, twojet
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 7) {
      double w_factor_os = Integral(os_sel["WJetsToLNuSoup"+sel].hist_ptr()) / Integral(os_con["WJetsToLNuSoup"+sel].hist_ptr());
      double w_factor_ss = Integral(ss_sel["WJetsToLNuSoup"+sel].hist_ptr()) / Integral(ss_con["WJetsToLNuSoup"+sel].hist_ptr());
      double w_factor_os_num_err = Error(os_sel["WJetsToLNuSoup"+sel].hist_ptr());
      double w_factor_os_den_err = Error(os_con["WJetsToLNuSoup"+sel].hist_ptr());
      double w_factor_ss_num_err = Error(ss_sel["WJetsToLNuSoup"+sel].hist_ptr());
      double w_factor_ss_den_err = Error(ss_con["WJetsToLNuSoup"+sel].hist_ptr());
      if (verbose) std::cout << "W MC OS Signal/Control factor (category): " << Integral(os_sel["WJetsToLNuSoup"+sel].hist_ptr()) 
        << " +/- " << w_factor_os_num_err << " / " << Integral(os_con["WJetsToLNuSoup"+sel].hist_ptr()) 
        << " +/- " << w_factor_os_den_err << " = " << w_factor_os << endl;
      if (verbose) std::cout << "W MC SS Signal/Control factor (category): " << Integral(ss_sel["WJetsToLNuSoup"+sel].hist_ptr()) 
        << " +/- " << w_factor_ss_num_err << " / " << Integral(ss_con["WJetsToLNuSoup"+sel].hist_ptr()) 
        << " +/- " << w_factor_ss_den_err << " = " << w_factor_ss << endl;

      //if (verbose) std::cout << "W MC OS Signal/Control factor (category): " << Integral(os_sel["WJetsToLNuSoup"+sel].hist_ptr()) << " / " << Integral(os_con["WJetsToLNuSoup"+sel].hist_ptr()) << " = " << w_factor_os << endl;
      //if (verbose) std::cout << "W MC SS Signal/Control factor (category): " << Integral(ss_sel["WJetsToLNuSoup"+sel].hist_ptr()) << " / " << Integral(ss_con["WJetsToLNuSoup"+sel].hist_ptr()) << " = "  << w_factor_ss << endl;
      double w_tot_sel = Integral(os_sel["WJetsToLNuSoup"+sel].hist_ptr()) + Integral(ss_sel["WJetsToLNuSoup"+sel].hist_ptr());
      double w_tot_con = Integral(os_con["WJetsToLNuSoup"+sel].hist_ptr()) + Integral(ss_con["WJetsToLNuSoup"+sel].hist_ptr());
      double w_factor = w_tot_sel / w_tot_con;
      if (verbose) std::cout << "W MC TOT Signal/Control factor (category): " << w_tot_sel << "/" << w_tot_con << " = " << w_factor << std::endl;
      w_os_sel = w_os_con * w_factor_os;
      w_ss_sel = w_ss_con * w_factor_ss;
      w_hist = (TH1F*)(os_sel["WJetsToLNuSoup"+sel].hist_ptr()->Clone());
      w_norm = w_os_sel;
    }
    // Extrapolation of B Jet
    if (category == 6 || category == 7 || category == 12) {
      // So we can do this with the nPt20Jets >=1 && nPt30Jets <= 1 selection...
      // double w_factor_os = Integral(os_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) / Integral(os_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr());
      // double w_factor_ss = Integral(ss_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) / Integral(ss_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr());
      // double w_factor_os_num_err = Error(os_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr());
      // double w_factor_os_den_err = Error(os_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr());
      // if (verbose) std::cout << "W MC OS Signal/Control factor (category): " << Integral(os_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) 
      //   << " +/- " << w_factor_os_num_err << " / " << Integral(os_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) 
      //   << " +/- " << w_factor_os_den_err << " = " << w_factor_os << endl;
      // if (verbose) std::cout << "W MC SS Signal/Control factor (category): " << Integral(ss_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) << " / " << Integral(ss_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) << " = "  << w_factor_ss << endl;
      // double w_tot_sel = Integral(os_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) + Integral(ss_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr());
      // double w_tot_con = Integral(os_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr()) + Integral(ss_con["Special_9_WJetsToLNuSoup"+sel].hist_ptr());
      // or with full b-tag selection
      double w_factor_os = Integral(os_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) / Integral(os_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr());
      double w_factor_ss = Integral(ss_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) / Integral(ss_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr());
      double w_factor_os_num_err = Error(os_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr());
      double w_factor_os_den_err = Error(os_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr());
      if (verbose) std::cout << "W MC OS Signal/Control factor (category): " << Integral(os_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) 
        << " +/- " << w_factor_os_num_err << " / " << Integral(os_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) 
        << " +/- " << w_factor_os_den_err << " = " << w_factor_os << endl;
      if (verbose) std::cout << "W MC SS Signal/Control factor (category): " << Integral(ss_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) << " / " << Integral(ss_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) << " = "  << w_factor_ss << endl;
      double w_tot_sel = Integral(os_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) + Integral(ss_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr());
      double w_tot_con = Integral(os_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()) + Integral(ss_con["WJetsToLNuSoup"+sel+"_loose"].hist_ptr());
      double w_factor = w_tot_sel / w_tot_con;
      if (verbose) std::cout << "W MC TOT Signal/Control factor (category): " << w_tot_sel << "/" << w_tot_con << " = " << w_factor << std::endl;
      w_os_sel = w_os_con * w_factor_os;
      w_ss_sel = w_ss_con * w_factor_ss;
      double w_ss_os = Integral(ss_sel["WJetsToLNuSoupinclusive"].hist_ptr()) / Integral(os_sel["WJetsToLNuSoupinclusive"].hist_ptr());
      if (verbose) std::cout << "[W Inclusive SS/OS factor]: " << w_ss_os << std::endl;
      if (verbose) std::cout << "[SS/OS factor * w_os_sel]: " << w_os_sel * w_ss_os << std::endl;
      //w_ss_sel = w_os_sel * w_ss_os;
      std::cout << "W SS Sel: " << w_ss_sel << std::endl;
      w_hist = (TH1F*)(os_sel["WJetsToLNuSoup"+sel+"_loose"].hist_ptr()->Clone());
      //w_hist = (TH1F*)(os_sel["Special_9_WJetsToLNuSoup"+sel].hist_ptr()->Clone());
      w_norm = w_os_sel;
    }
    // Normalisation and extrapolation for VBF
    if (category == 5) {
      w_os_con = Integral(os_con["Special_7_Data"+sel].hist_ptr());
      w_os_con -= Integral(os_con["Special_7_TTJets"+sel].hist_ptr());
      w_os_con -= Integral(os_con["Special_7_DYJetsToTauTau"+sel].hist_ptr());
      w_os_con -= Integral(os_con["Special_7_DYJetsToLL"+sel].hist_ptr());
      double vv_os_tot = 0.0;
      for (unsigned i = 0; i < vv_samples.size(); ++i) {
        vv_os_tot += Integral(os_con["Special_7_"+vv_samples[i]+sel].hist_ptr()); 
        w_os_con -= Integral(os_con["Special_7_"+vv_samples[i]+sel].hist_ptr());      
      }
      if (verbose) std::cout << "OS Control Data (60 < mT < 120): " << Integral(os_con["Special_7_Data"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control TTBar (60 < mT < 120): " << Integral(os_con["Special_7_TTJets"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control ZTT (60 < mT < 120): " << Integral(os_con["Special_7_DYJetsToTauTau"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control ZLL (60 < mT < 120): " << Integral(os_con["Special_7_DYJetsToLL"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "OS Control VV (60 < mT < 120): " << vv_os_tot << std::endl;
      if (verbose) std::cout << "=> OS Control W: " << w_os_con << std::endl;
      double w_factor = Integral(os_sel["Special_7_WJetsToLNuSouptwojet"].hist_ptr()) / Integral(os_con["Special_7_WJetsToLNuSouptwojet"].hist_ptr());
      std::cout << "VBF Loose: " << Integral(os_sel["Special_7_WJetsToLNuSoupvbfloose"].hist_ptr()) / Integral(os_con["Special_7_WJetsToLNuSoupvbfloose"].hist_ptr()) << std::endl;
      w_os_sel = w_os_con * w_factor;
      if (verbose) std::cout << "W MC OS Signal/Control factor (two jets - soup): " << w_factor << endl;
      std::cout << "OS Signal: " << Integral(os_sel["Special_7_WJetsToLNuSouptwojet"].hist_ptr()) << "\t" << Error(os_sel["Special_7_WJetsToLNuSouptwojet"].hist_ptr()) << std::endl;
      double w_ss_os = Integral(ss_sel["WJetsToLNuSoupinclusive"].hist_ptr()) / Integral(os_sel["WJetsToLNuSoupinclusive"].hist_ptr());
      //if (verbose) std::cout << "W MC In SS/OS Factor: " << w_ss_os << endl;
      w_ss_sel = w_os_sel * w_ss_os;// not actually used
      w_hist = (TH1F*)(os_sel["WJetsToLNuSoupvbfloose"].hist_ptr()->Clone());
      //w_hist = (TH1F*)(os_sel["WJetsToLNuSouptwojet"].hist_ptr()->Clone());
      w_norm = w_os_sel;
    }
  } else if (mode == 2) {
    w_norm = 0; 
    w_hist = (TH1F*)(ss_sel["Data"+sel].hist_ptr()->Clone());
  }

  if (mode == 0 || mode == 1) w_hist->Scale( w_norm / Integral(w_hist) );
  if (mode == 2) w_hist->Scale(0);
  ic::TH1PlotElement w_shape("w_shape", w_hist);

  TH1F *w_hist_ss = (TH1F*)(ss_sel["WJetsToLNuSoup"+sel].hist_ptr()->Clone());
  double w_norm_ss = w_ss_sel;
  ic::TH1PlotElement w_shape_ss("w_shape_ss", w_hist_ss);

  if (mode == 0 || mode == 1) w_hist_ss->Scale( w_norm_ss / Integral(w_hist_ss) );


  std::cout << "W Norm: " << w_norm << std::endl;

  // ---------------------------------------------------
  // Generate QCD Shape and Norm
  // ---------------------------------------------------
  if (verbose) cout << "**** QCD: " << endl;


  double qcd_norm = 0.0;
  double qcd_inclusive = 0.0;
  double os_ss_ratio = parser.GetParam<double>("QCD_OS_SS_RATIO");
  TH1F * h1;
  TH1F * h2;
  TH1F * h3;
  TH1F *qcd_hist = NULL;

  if (mode == 0 || mode == 1) {
    //-----------------------
    // 0/1 Jet and dilepton
    //-----------------------
    if (category == 0 || category == 1 || category == 2 || category == 3 || category > 5) {
      double data_ss_sel = Integral(ss_sel["Data"+sel].hist_ptr());
      data_ss_sel -= w_ss_sel;
      data_ss_sel -= Integral(ss_sel["DYJetsToLL"+sel].hist_ptr());
      data_ss_sel -= Integral(ss_sel["TTJets"+sel].hist_ptr());
      //data_ss_sel -= Integral(ss_sel["DYJetsToTauTau"+sel].hist_ptr());
      data_ss_sel -= (Integral(ss_sel["Embedded"+sel].hist_ptr())*embed_norm);

      double vv_ss_total = 0.0;
      for (unsigned i = 0; i < vv_samples.size(); ++i) vv_ss_total += Integral(ss_sel[vv_samples[i]+sel].hist_ptr());
        data_ss_sel -= vv_ss_total;
      if (verbose) std::cout << "SS Data: " << Integral(ss_sel["Data"+sel].hist_ptr()) << endl;
      if (verbose) std::cout << "SS W: " << w_ss_sel << endl;
      if (verbose) std::cout << "SS DYJetsToLL: " << Integral(ss_sel["DYJetsToLL"+sel].hist_ptr()) << endl;
      if (verbose) std::cout << "SS TTJets: " << Integral(ss_sel["TTJets"+sel].hist_ptr()) << endl;
      if (verbose) std::cout << "[SS ZTT]: " << Integral(ss_sel["DYJetsToTauTau"+sel].hist_ptr()) << endl;
      if (verbose) std::cout << "SS Embedded: " << Integral(ss_sel["Embedded"+sel].hist_ptr())*embed_norm << endl;
      if (verbose) std::cout << "SS VV: " << vv_ss_total << std::endl;
      if (verbose) std::cout << "=> SS QCD: " << data_ss_sel << std::endl;
      qcd_norm = os_ss_ratio * data_ss_sel;
      if (qcd_norm < 0.0) qcd_norm = 0.0000001;
      if (non_mass_plot || category == 0 || category == 8 || category == 11) {
        std::cout << "Using Full Isolation for QCD shape!" << std::endl;
        qcd_hist = (TH1F*)(ss_sel["Data"+sel].hist_ptr()->Clone());
        qcd_hist->Add(ss_sel["DYJetsToLL"+sel].hist_ptr(), -1.0);
        qcd_hist->Add(ss_sel["Embedded"+sel].hist_ptr(), -1.0*embed_norm);
        qcd_hist->Add(ss_sel["TTJets"+sel].hist_ptr(), -1.0);
        TH1F* w_cat_ss = (TH1F*)ss_sel["WJetsToLNuSoup"+sel].hist_ptr()->Clone();
        w_cat_ss->Scale(w_ss_sel/ Integral(ss_sel["WJetsToLNuSoup"+sel].hist_ptr()));
        qcd_hist->Add(w_cat_ss, -1.0);
      } else {
        qcd_hist = (TH1F*)(ss_sel["Special_3_Data"+sel].hist_ptr()->Clone());
      }

      for (int j = 0; j < qcd_hist->GetNbinsX(); ++j) {
        if (qcd_hist->GetBinContent(j+1) < 0) {
          qcd_hist->SetBinContent(j+1,0.0000001);
          qcd_hist->SetBinError(j+1, 1.0);
        }
      }
    }


    //-----------------------
    //Inclusive will be needed for QCD - first do inclusive W
    //-----------------------
    if (verbose) cout << "** Determine QCD inclusive: " << endl;
    w_ss_sel_inclusive = Integral(ss_con["Datainclusive"].hist_ptr());
    w_ss_sel_inclusive -= Integral(ss_con["TTJetsinclusive"].hist_ptr());
    w_ss_sel_inclusive -= Integral(ss_con["DYJetsToLLinclusive"].hist_ptr());
    w_ss_sel_inclusive -= Integral(ss_con["DYJetsToTauTauinclusive"].hist_ptr());
    double vv_total = 0.0;
    for (unsigned i = 0; i < vv_samples.size(); ++i) {
      vv_total += Integral(ss_con[vv_samples[i]+"inclusive"].hist_ptr());
      w_ss_sel_inclusive -= Integral(ss_con[vv_samples[i]+"inclusive"].hist_ptr());      
    }
    if (verbose) std::cout << "SS Control Data Inclusive: " << Integral(ss_con["Datainclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS Control TTBar Inclusive: " << Integral(ss_con["TTJetsinclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS Control VV Inclusive: " << vv_total << std::endl;
    if (verbose) std::cout << "SS Control DYJetsToLL Inclusive: " << Integral(ss_con["DYJetsToLLinclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS Control DYJetsToTauTau Inclusive: " << Integral(ss_con["DYJetsToTauTauinclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "[SS Control Embedded Inclusive]: " << Integral(ss_con["Embeddedinclusive"].hist_ptr())*embed_norm << std::endl;
    if (verbose) std::cout << "=> SS Control W Inclusive: " << w_ss_sel_inclusive << std::endl;
    double w_factor_os_inc = Integral(os_sel["WJetsToLNuSoupinclusive"].hist_ptr()) / Integral(os_con["WJetsToLNuSoupinclusive"].hist_ptr());
    double w_factor_ss_inc = Integral(ss_sel["WJetsToLNuSoupinclusive"].hist_ptr()) / Integral(ss_con["WJetsToLNuSoupinclusive"].hist_ptr());
    if (verbose) std::cout << "[W MC OS Signal/Control factor (inclusive)]: " << Integral(os_sel["WJetsToLNuSoupinclusive"].hist_ptr()) << " / " << Integral(os_con["WJetsToLNuSoupinclusive"].hist_ptr()) << " = " << w_factor_os_inc << endl;
    if (verbose) std::cout << "W MC SS Signal/Control factor (inclusive): " << Integral(ss_sel["WJetsToLNuSoupinclusive"].hist_ptr()) << " / " << Integral(ss_con["WJetsToLNuSoupinclusive"].hist_ptr()) << " = "  << w_factor_ss_inc << endl;
    double w_tot_sel_inc = Integral(os_sel["WJetsToLNuSoupinclusive"].hist_ptr()) + Integral(ss_sel["WJetsToLNuSoupinclusive"].hist_ptr());
    double w_tot_con_inc = Integral(os_con["WJetsToLNuSoupinclusive"].hist_ptr()) + Integral(ss_con["WJetsToLNuSoupinclusive"].hist_ptr());
    double w_factor_inc = w_tot_sel_inc / w_tot_con_inc;
    if (verbose) std::cout << "[W MC TOT Signal/Control factor (category)]: " << w_tot_sel_inc << "/" << w_tot_con_inc << " = " << w_factor_inc << std::endl;
    w_ss_sel_inclusive = w_ss_sel_inclusive * w_factor_ss_inc;
    if (verbose) std::cout << "=> SS Signal W Inclusive: " << w_ss_sel_inclusive << std::endl;
    // Now do the QCD
    qcd_inclusive = Integral(ss_sel["Datainclusive"].hist_ptr());
    qcd_inclusive -= w_ss_sel_inclusive;
    qcd_inclusive -= Integral(ss_sel["DYJetsToLLinclusive"].hist_ptr());
    qcd_inclusive -= Integral(ss_sel["TTJetsinclusive"].hist_ptr());
    //qcd_inclusive -= Integral(ss_sel["DYJetsToTauTauinclusive"].hist_ptr());
    qcd_inclusive -= (Integral(ss_sel["Embeddedinclusive"].hist_ptr())*embed_norm);
    double vv_ss_total_inc = 0.0;
    for (unsigned i = 0; i < vv_samples.size(); ++i) vv_ss_total_inc += Integral(ss_sel[vv_samples[i]+"inclusive"].hist_ptr());
    qcd_inclusive -= vv_ss_total_inc;
    if (verbose) std::cout << "SS Data Inclusive: " << Integral(ss_sel["Datainclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS W Inclusive: " << w_ss_sel_inclusive << std::endl;
    if (verbose) std::cout << "SS ZLL Inclusive: " << Integral(ss_sel["DYJetsToLLinclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS TOP Inclusive: " << Integral(ss_sel["TTJetsinclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "SS ZTT Inclusive: " << Integral(ss_sel["DYJetsToTauTauinclusive"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "[SS Embedded Inclusive]: " << Integral(ss_sel["Embeddedinclusive"].hist_ptr())*embed_norm << std::endl;
    if (verbose) std::cout << "SS VV Inclusive: " << vv_ss_total_inc << std::endl;
    qcd_inclusive = os_ss_ratio * qcd_inclusive;
    if (verbose) std::cout << "=> OS QCD Inclusive: " << qcd_inclusive << std::endl;
    if (verbose) std::cout << "QCD Selection Category Efficiency: " << ( Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) / Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) ) << std::endl;

    //-----------------------
    // vbf
    //-----------------------
    if (category == 5) {
      if (verbose) std::cout << "QCD Selection Category Efficiency: " << ( Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) / Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) ) << std::endl;
      qcd_norm = qcd_inclusive * ( Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) / Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) );
      if (verbose) std::cout << "Numberator: " << Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) << std::endl;
      if (verbose) std::cout << "Denominator: " << Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) << std::endl;
      qcd_hist = (TH1F*)(ss_sel["Special_6_Datavbfloose"].hist_ptr()->Clone());
    }
    //-----------------------
    // btag 
    //-----------------------
    if (category == 7) {
      std::cout << "[QCD Selection Category Efficiency]: " << ( Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) / Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) ) << std::endl;
       qcd_norm = qcd_inclusive * ( Integral(ss_sel["Special_3_Data"+sel].hist_ptr()) / Integral(ss_sel["Special_3_Datainclusive"].hist_ptr()) );
      qcd_hist = (TH1F*)(ss_sel["Special_3_Dataonebjet_highpt_loose"].hist_ptr()->Clone());
    }

    if (category == 6) {
       qcd_hist = (TH1F*)(ss_sel["Special_3_Dataonebjet_lowpt_loose"].hist_ptr()->Clone());
    }
    if (category == 12) {
       qcd_hist = (TH1F*)(ss_sel["Special_3_Databtag_loose"].hist_ptr()->Clone());
    }


  } else if (mode == 2) {
    if (verbose) std::cout << "Electron Fakes: " <<  Integral(os_sel["Special_14_Datadilepton"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "Muon Fakes: " <<  Integral(os_sel["Special_15_Datadilepton"].hist_ptr()) << std::endl;
    if (verbose) std::cout << "Double Fakes: " <<  Integral(os_sel["Special_17_Datadilepton"].hist_ptr()) << std::endl;
    double qcd_dilepton = Integral(os_sel["Special_14_Datadilepton"].hist_ptr()) /*+ Integral(os_sel["Special_15_Datadilepton"].hist_ptr()) - Integral(os_sel["Special_17_Datadilepton"].hist_ptr()) */;
    if (category != 5) {
      qcd_norm = qcd_dilepton * ( Integral(ss_sel["Data"+sel].hist_ptr()) / Integral(ss_sel["Datadilepton"].hist_ptr()) );
      qcd_hist = (TH1F*)(ss_sel["Data"+sel].hist_ptr()->Clone());
      // qcd_hist = (TH1F*)(os_sel["Special_14_Data"+sel].hist_ptr()->Clone());
      // qcd_hist->Add(os_sel["Special_15_Data"+sel].hist_ptr());
      // qcd_hist->Add(os_sel["Special_17_Data"+sel].hist_ptr(), -1);

    }
    if (category == 5) {
      qcd_norm = Integral(os_sel["Special_14_Data"+sel].hist_ptr()) /*+ Integral(os_sel["Special_15_Data"+sel].hist_ptr()) - Integral(os_sel["Special_17_Data"+sel].hist_ptr())*/;
      qcd_hist = (TH1F*)(os_sel["Special_14_Data"+sel].hist_ptr()->Clone());
      //1qcd_hist->Add(os_sel["Special_15_Data"+sel].hist_ptr());
    }

    // double ss_data = Integral(ss_sel["Data"+sel].hist_ptr());
    // double ss_top = Integral(ss_sel["TTJets"+sel].hist_ptr());
    // double vv_total = 0.0;
    // for (unsigned i = 0; i < vv_samples.size(); ++i) {
    //   vv_total += Integral(ss_sel[vv_samples[i]+sel].hist_ptr());
    // }
    // double ss_qcd = ss_data - (ss_top + vv_total);
    // if (ss_qcd < 0) ss_qcd = 0.0000001;
    // qcd_norm = 1.06 * ss_qcd;
    // qcd_hist = (TH1F*)(ss_sel["Data"+sel].hist_ptr()->Clone());
  }
  FixEmptyBins(qcd_hist, true, false);
  qcd_hist->Scale( qcd_norm / Integral(qcd_hist) );

  if (mode == 1 && category == 2) {
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
      if(x<50){
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
  std::cout << "QCD Norm: " << qcd_norm << std::endl;

  double qcd_norm_ss = qcd_norm/os_ss_ratio;
  TH1F *qcd_hist_ss = (TH1F*)qcd_hist->Clone();
  qcd_hist_ss->Scale( qcd_norm_ss / Integral(qcd_hist_ss) );
  ic::TH1PlotElement qcd_shape_ss("qcd_shape_ss", qcd_hist_ss);


  std::cout << "Background Norm: " << top_norm + vv_norm + zll_norm + ztt_norm + w_norm + qcd_norm << std::endl;
  std::string dc_mode_label, dc_cat_label, dc_mode_label_alt;
  if (mode == 0) dc_mode_label = "eleTau";
  if (mode == 1) dc_mode_label = "muTau";
  if (mode == 2) dc_mode_label = "emu";
  if (mode == 0) dc_mode_label_alt = "etau";
  if (mode == 1) dc_mode_label_alt = "mutau";
  if (mode == 2) dc_mode_label_alt = "em";
  if (category == 0) dc_cat_label = "0jet_low";
  if (category == 1) dc_cat_label = "0jet_high";
  if (category == 2) dc_cat_label = "boost_low";
  if (category == 3) dc_cat_label = "boost_high";
  if (category == 4) dc_cat_label = "vh";
  if (category == 5) dc_cat_label = "vbf";
  if (category == 6) dc_cat_label = "btag_low";
  if (category == 7) dc_cat_label = "btag_high";
  if (category == 8) dc_cat_label = "inclusive";
  if (category == 9) dc_cat_label = "dilepton";
  if (category == 10) dc_cat_label = "twojet";
  if (category == 11) dc_cat_label = "nobtag";
  if (category == 12) dc_cat_label = "btag";

  if (swap_inclusive) {
    if (category == 8) dc_cat_label = "0jet_low";
    if (category == 0) dc_cat_label = "inclusive";

  }

  if (make_datacard) {

    std::string append = "";
    if (tau_scale_mode == 1) append = "_CMS_scale_t_"+dc_mode_label_alt+(is_2012 ? "_8":"_7")+"TeVDown";
    if (tau_scale_mode == 2) append = "_CMS_scale_t_"+dc_mode_label_alt+(is_2012 ? "_8":"_7")+"TeVUp";

    TFile datacard(("datacard_"+dc_mode_label+"_"+dc_cat_label+"_"+year_label+append+".root").c_str(),"RECREATE");
    datacard.cd();
    gDirectory->mkdir((dc_mode_label+"_"+dc_cat_label).c_str());
    gDirectory->cd((dc_mode_label+"_"+dc_cat_label).c_str());

    double boost_high_clean = 40.0;
    if (plot_name.find("vis_") != plot_name.npos) {
      boost_high_clean = 20.0;
      std::cout << "Vis sample detected: cleaning up to " << boost_high_clean << std::endl;
    }
    if (category == 3 && mode == 0 && !is_2012) {
      std::cout << "Cleaning bins!" << std::endl;
    }
    
    if (mssm_mode == 0) {
      for (unsigned i = 0; i < signal_masses.size(); ++i) {
        TH1F *VH = (TH1F*)(os_sel["WH_ZH_TTH_HToTauTau_M-"+signal_masses[i]+sel].hist_ptr()->Clone());
        VH->Scale(1./ (parser.GetParam<double>("XS_WH_ZH_TTH_HToTauTau_M-"+signal_masses[i])));
        VH->SetName(("VH"+signal_masses[i]+append).c_str());
        VH->SetTitle(("VH"+signal_masses[i]+append).c_str());
        if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(VH, boost_high_clean);
        VH->Write();


        TH1F *ggH = (TH1F*)(os_sel["GluGluToHToTauTau_M-"+signal_masses[i]+sel].hist_ptr()->Clone());
        ggH->Scale(1./ (parser.GetParam<double>("XS_GluGluToHToTauTau_M-"+signal_masses[i])));
        ggH->SetName(("ggH"+signal_masses[i]+append).c_str());
        ggH->SetTitle(("ggH"+signal_masses[i]+append).c_str());
        if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(ggH, boost_high_clean);
        ggH->Write();

        TH1F *qqH = (TH1F*)(os_sel["VBF_HToTauTau_M-"+signal_masses[i]+sel].hist_ptr()->Clone());
        qqH->Scale(1./ (parser.GetParam<double>("XS_VBF_HToTauTau_M-"+signal_masses[i])));
        qqH->SetName(("qqH"+signal_masses[i]+append).c_str());
        qqH->SetTitle(("qqH"+signal_masses[i]+append).c_str());
        if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(qqH, boost_high_clean);
        qqH->Write();
      }
    } else {
      for (unsigned i = 0; i < mssm_signal_masses.size(); ++i) {
        TH1F *ggH = (TH1F*)(os_sel["SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i]+sel].hist_ptr()->Clone());
        ggH->Scale(1./ (parser.GetParam<double>("XS_SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i])));
        if (Integral(ggH) < 0.0000001) {
          std::cout << "Histogram " << "SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i] << ":" << sel << " appears to be empty, adding small value in first bin!" << std::endl;
          ggH->SetBinContent(1, 0.0000001);
        }
        ggH->SetName(("ggH"+mssm_signal_masses[i]+append).c_str());
        ggH->SetTitle(("ggH"+mssm_signal_masses[i]+append).c_str());
        if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(ggH, boost_high_clean);
        ggH->Write();

        TH1F *bbH = (TH1F*)(os_sel["SUSYBBHToTauTau_M-"+mssm_signal_masses[i]+sel].hist_ptr()->Clone());
        bbH->Scale(1./ (parser.GetParam<double>("XS_SUSYBBHToTauTau_M-"+mssm_signal_masses[i])));
        if (Integral(bbH) < 0.0000001) {
          std::cout << "Histogram " << "SUSYBBHToTauTau_M-"+mssm_signal_masses[i]+sel << " appears to be empty, adding small value in first bin!" << std::endl;
          bbH->SetBinContent(1, 0.0000001);
        }
        bbH->SetName(("bbH"+mssm_signal_masses[i]+append).c_str());
        bbH->SetTitle(("bbH"+mssm_signal_masses[i]+append).c_str());
        if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(bbH, boost_high_clean);

        bbH->Write();
      }
    }


    TH1F *dc_ztt = (TH1F*)ztt_hist->Clone();
    dc_ztt->SetName(("ZTT"+append).c_str());
    dc_ztt->SetTitle(("ZTT"+append).c_str());
    if (mode == 2) {
      dc_ztt->SetName("Ztt");
      dc_ztt->SetTitle("Ztt");
    }
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_ztt, boost_high_clean);
    dc_ztt->Write();

    TH1F *dc_zll = (TH1F*)zll_hist->Clone();
    FixEmptyBins(dc_zll, false, false);
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_zll, boost_high_clean);

    dc_zll->SetName(("ZLL"+append).c_str());
    dc_zll->SetTitle(("ZLL"+append).c_str());
    if (mode == 0 || mode == 1) dc_zll->Write();

    TH1F *dc_zl = (TH1F*)zl_hist->Clone();
    FixEmptyBins(dc_zl, false, false);
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_zl, boost_high_clean);

    dc_zl->SetName(("ZL"+append).c_str());
    dc_zl->SetTitle(("ZL"+append).c_str());
    if (mode == 0 || mode == 1) dc_zl->Write();

    TH1F *dc_zj = (TH1F*)zj_hist->Clone();
    FixEmptyBins(dc_zj, false, false);
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_zj, boost_high_clean);

    dc_zj->SetName(("ZJ"+append).c_str());
    dc_zj->SetTitle(("ZJ"+append).c_str());
    if (mode == 0 || mode == 1) dc_zj->Write();

    std::string zee_append = std::string("_CMS_htt_ZEEShape_etau_")+(is_2012 ? "8":"7")+"TeV";
    if (tau_scale_mode == 1) zee_append += "Down";
    if (tau_scale_mode == 2) zee_append += "Up";
    TH1F *dc_zl_zee, *dc_zll_zee;
    if (mode == 0 && tau_scale_mode > 0) {
      dc_zl_zee = (TH1F*)dc_zl->Clone();
      dc_zl_zee->SetName(("ZL"+zee_append).c_str());
      dc_zl_zee->SetTitle(("ZL"+zee_append).c_str());
      dc_zll_zee = (TH1F*)dc_zll->Clone();
      dc_zll_zee->SetName(("ZLL"+zee_append).c_str());
      dc_zll_zee->SetTitle(("ZLL"+zee_append).c_str());
      dc_zl_zee->Write();
      dc_zll_zee->Write();
    }

    TH1F *dc_top = (TH1F*)top_hist->Clone();
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_top, boost_high_clean);
    dc_top->SetName(("TT"+append).c_str());
    dc_top->SetTitle(("TT"+append).c_str());
    if (mode == 2) {
      dc_top->SetName("ttbar");
      dc_top->SetTitle("ttbar");
    }
    dc_top->Write();

    TH1F *dc_vv = (TH1F*)vv_hist->Clone();
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_vv, boost_high_clean);
    dc_vv->SetName(("VV"+append).c_str());
    dc_vv->SetTitle(("VV"+append).c_str());
    if (mode == 2) {
      dc_vv->SetName("EWK");
      dc_vv->SetTitle("EWK");
    }
    dc_vv->Write();

    TH1F *dc_w = (TH1F*)w_hist->Clone();
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_w, boost_high_clean);
    FixEmptyBins(dc_w, false, false);
    dc_w->SetName(("W"+append).c_str());
    dc_w->SetTitle(("W"+append).c_str());
    if (mode == 0 || mode == 1) dc_w->Write();

    TH1F *dc_qcd = (TH1F*)qcd_hist->Clone();
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(dc_qcd, boost_high_clean);
    dc_qcd->SetName(("QCD"+append).c_str());
    dc_qcd->SetTitle(("QCD"+append).c_str());
    if (mode == 2) {
      dc_qcd->SetName("Fakes");
      dc_qcd->SetTitle("Fakes");
    }
   if (mode == 1 && category == 2 && tau_scale_mode == 0) {
        h2->Write();
        h3->Write();
    }
    dc_qcd->Write();


    TH1F *data_obs = (TH1F*)data_hist_clean->Clone();
    if (category == 3 && mode == 0 && !is_2012) CleanBinsUpTo(data_obs, boost_high_clean);
    // if (blind) {
    //   double data_err = 0.0;
    //   double data_tot = data_obs->IntegralAndError(0, data_obs->GetNbinsX() + 1, data_err);
    //   data_obs->Reset();
    //   data_obs->SetBinContent(1, data_tot);
    //   data_obs->SetBinError(1, data_err);
    // }
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
  std::string kind_label = "sm";
  if (mssm_mode > 0) kind_label = "mssm";
  if (mode == 0) mode_label = "et";
  if (mode == 1) mode_label = "mt";
  if (mode == 2) mode_label = "em";
  plot.output_filename = plot_name + "_" + sel + "_" +mode_label + "_" + kind_label + "_" + year_label + (draw_ss ? "_ss" : "") + ".pdf";
  if (log_y) plot.output_filename = plot_name + "_" + sel + "_" +mode_label + "_" + kind_label + "_" + year_label + (draw_ss ? "_ss" : "")  + "_log.pdf";
  


  TH1F* signal_hist;
  if (mssm_mode == 0) {
    signal_hist = (TH1F*)(os_sel["GluGluToHToTauTau_M-"+draw_signal_mass+sel].hist_ptr()->Clone());
    if (!signal_split_vbf) signal_hist->Add(os_sel["VBF_HToTauTau_M-"+draw_signal_mass+sel].hist_ptr());
    signal_hist->Add(os_sel["WH_ZH_TTH_HToTauTau_M-"+draw_signal_mass+sel].hist_ptr());
    signal_hist->Scale(draw_signal_factor);
  } else {
    signal_hist = (TH1F*)(os_sel["SUSYGluGluToHToTauTau_M-"+draw_mssm_signal_mass+sel].hist_ptr()->Clone());
    signal_hist->Add(os_sel["SUSYBBHToTauTau_M-"+draw_mssm_signal_mass+sel].hist_ptr());
    signal_hist->Scale(draw_mssm_signal_factor);
  }
  ic::TH1PlotElement signal_shape("signal_shape", signal_hist);
  ic::TH1PlotElement signalvbf_shape;
  if (mssm_mode == 0) {
    signalvbf_shape =  ic::TH1PlotElement("signalvbf_shape", (TH1F*)os_sel["VBF_HToTauTau_M-"+draw_signal_mass+sel].hist_ptr()->Clone());
    signalvbf_shape.hist_ptr()->Scale(draw_signal_factor);
  } 

  if (shift_backgrounds) {
    std::cout << "Shift ZTT: " << ztt_shift << std::endl;
    std::cout << "Shift QCD: " << qcd_shift << std::endl;
    std::cout << "Shift TOP: " << top_shift << std::endl;
    std::cout << "Shift ZLL: " << zll_shift << std::endl;
    std::cout << "Shift W: " << w_shift << std::endl;
    std::cout << "Shift VV: " << vv_shift << std::endl;
    top_shape.hist_ptr()->Scale(top_shift);
    qcd_shape.hist_ptr()->Scale(qcd_shift);
    w_shape.hist_ptr()  ->Scale(w_shift);
    vv_shape.hist_ptr() ->Scale(vv_shift);
    zll_shape.hist_ptr()->Scale(zll_shift);
    ztt_shape.hist_ptr()->Scale(ztt_shift);
  }


  if (mode == 1) w_shape.hist_ptr()->Add(zll_shape.hist_ptr());
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
  SetDataStyle(data_shape,1);
  if (signal_no_stack) {
    signal_shape.set_in_stack(false);
    signalvbf_shape.set_in_stack(false);
  }

  std::vector<ic::TH1PlotElement *> drawn_ele;
  drawn_ele.push_back(&qcd_shape);
  drawn_ele.push_back(&w_shape);
  drawn_ele.push_back(&ztt_shape);
  drawn_ele.push_back(&top_shape);
  if (mode == 0) drawn_ele.push_back(&zll_shape);
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
  if (mode == 2) qcd_shape.set_legend_text("Fakes");
  w_shape.set_legend_text("Electroweak");
  data_shape.set_legend_text("Observed");
  ztt_shape.set_legend_text("Z#rightarrow#tau#tau");
  zll_shape.set_legend_text("Z#rightarrowee");
  top_shape.set_legend_text("t#bar{t}");
  if (mssm_mode == 0) {
    signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau");
    if (signal_split_vbf) signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau (ggH+VH)");
    if (signal_split_vbf) signalvbf_shape.set_legend_text(boost::lexical_cast<std::string>(draw_signal_factor)+"#times H("+draw_signal_mass+")#rightarrow#tau#tau (qqH)");
  } else {
    signal_shape.set_legend_text(boost::lexical_cast<std::string>(draw_mssm_signal_factor)+"#times #Phi#rightarrow#tau#tau(m_{A}="+draw_mssm_signal_mass+",tan#beta=8)");
  }
  if (mode == 0 || mode == 1) {
    plot.AddTH1PlotElement(top_shape);
    plot.AddTH1PlotElement(qcd_shape);
    plot.AddTH1PlotElement(w_shape);
  } else if (mode == 2) {
    plot.AddTH1PlotElement(qcd_shape);
    plot.AddTH1PlotElement(w_shape);
    plot.AddTH1PlotElement(top_shape);
  }
  if (mode == 0) plot.AddTH1PlotElement(zll_shape);
  plot.AddTH1PlotElement(ztt_shape);


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
  if (mode == 0) plot.title_right = "#tau_{e}#tau_{h}, "+dc_cat_label;
  if (mode == 1) plot.title_right = "#tau_{#mu}#tau_{h}, "+dc_cat_label;
  if (mode == 2) plot.title_right = "#tau_{e}#tau_{#mu}, "+dc_cat_label;
  if (mssm_mode == 1) plot.legend_left = 0.45;

  plot.draw_ratio_hist = false;
  plot.draw_signif = false;
  ic::RatioPlotElement ratio("Mug","data_shape","top_shape+vv_shape+zll_shape+ztt_shape+w_shape+qcd_shape");
  
  plot.band_size_fractional_ = band_size_fractional;
  plot.draw_band_on_stack_ = draw_band_on_stack;
  plot.samples_for_band_ = "top_shape+ztt_shape+w_shape+qcd_shape";
  if (mode == 0) plot.samples_for_band_ += "+zll_shape";

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
  ic::TextElement ss_text("Same-sign",0.04,0.19,0.89);
  if (draw_ss) plot.AddTextElement(ss_text);

  if (!no_plot) plot.GeneratePlot();

  return 0;
}

