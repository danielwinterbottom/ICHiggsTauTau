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
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

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

void SetStyle(ic::TH1PlotElement & ele, unsigned color) {
  //ele.set_marker_color(color);
  //ele.set_line_color(color);
  ele.set_fill_color(color);
  ele.set_fill_style(1001);
  ele.set_draw_fill(true);
  ele.set_draw_marker(false);
  ele.set_draw_line(false);
  ele.set_line_width(2);
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

  ic::channel channel = channel::zee;
  // ic::channel channel = channel::zmm;

  std::string channel_str = Channel2String(channel);

  double data_lumi = 17000;
  std::string lumi_str = "17 fb^{-1}";
  std::string plotname = "m_z";
  std::string plotfolder = "inclusive";
  std::string x_axis_label = "M_{ll} [GeV]";

  double dy_evt   = 30459503;
  double dy_xs    = 3503.7;
  double tt_evt   = 6923750;
  double tt_xs    = 225.2;

  bool log_y = false;

  TFile f_data(("output/ZJets/Data_Moriond_"+channel_str+"_2012.root").c_str());
  TFile f_dy(("output/ZJets/DYJetsToLL_"+channel_str+"_2012.root").c_str());
  TFile f_tt(("output/ZJets/TTJets_"+channel_str+"_2012.root").c_str());

  ic::TH1PlotElement data("data",&f_data, "/"+plotfolder, plotname, "Data");
  ic::TH1PlotElement dy("dy",&f_dy, "/"+plotfolder, plotname, "Z #rightarrow ll");
  ic::TH1PlotElement tt("tt",&f_tt, "/"+plotfolder, plotname, "t#bar{t}");

  dy.hist_ptr()->Scale( data_lumi * dy_xs / dy_evt );
  tt.hist_ptr()->Scale( data_lumi * tt_xs / tt_evt );

  ic::Plot plot;

  plot.output_filename = "output/ZJets/"+plotfolder+"_"+plotname+"_" +channel_str+"_2012"+".pdf";
  if (log_y) plot.output_filename = "output/ZJets/"+plotfolder+"_"+plotname+"_" +channel_str+"_2012"+".pdf";;
  
  SetStyle(dy, kBlue  - 6);
  SetStyle(tt, kCyan  - 6);
  SetDataStyle(data,1);

  plot.AddTH1PlotElement(tt);
  plot.AddTH1PlotElement(dy);
  plot.AddTH1PlotElement(data);
  // plot.extra_pad = extra_pad;
  // plot.custom_x_axis_range = custom_x_axis_range;
  // if (custom_x_axis_range){
  //   plot.x_axis_min = x_axis_min;
  //   plot.x_axis_max = x_axis_max;
  // }
  // plot.legend_height = 0.045;
  plot.x_axis_title = x_axis_label;
  plot.y_axis_title = "Events";
  plot.title_left = "CMS Preliminary 2012, #sqrt{s} = 8 TeV, "+ lumi_str;
  plot.title_right = channel_str+", "+plotfolder;

  // if (mssm_mode == 1) plot.legend_left = 0.45;

  plot.draw_ratio_hist = true;
  ic::RatioPlotElement ratio("Ratio","data","dy+tt");
  SetStyle(ratio,1);
  ratio.set_multi_mode(true);
  plot.ratio_y_axis_title = "Data/MC";
  plot.AddRatioPlotElement(ratio);
  plot.custom_ratio_y_axis_range = true;
  plot.ratio_y_axis_min = 0.5;
  plot.ratio_y_axis_max = 1.5;
  
  if (log_y) plot.y_axis_log = true;
  // if (y_axis_min > 0) plot.y_axis_min = y_axis_min;
  
  // // plot.y_axis_min = 5.0;
  // if (blind) {
  //   agilbert::TextElement text1("Data in ["+boost::lexical_cast<std::string>(x_blind_min)+","+boost::lexical_cast<std::string>(x_blind_max)+"] blinded",0.03, 0.16,0.89);
  //   plot.AddTextElement(text1);
  // }
  // ic::TextElement ss_text("Same-sign",0.04,0.19,0.89);
  // if (draw_ss) plot.AddTextElement(ss_text);

  plot.GeneratePlot();

  // std::string configfile, paramfile, selection, plot_name;
  // paramfile = "scripts/default_params_2011.dat";
  // unsigned mode = 0;
  // unsigned mssm_mode = 0;
  // unsigned category = 0;
  // bool is_2012 = false;
  // std::string x_axis_label;
  // unsigned rebin;
  // bool custom_x_axis_range = true;
  // double x_axis_min, x_axis_max;
  // double extra_pad;
  // bool blind;
  // double x_blind_min, x_blind_max;
  // bool no_plot;
  // bool make_datacard;
  // bool log_y = false;
  // bool verbose = false;
  // bool non_mass_plot = false;
  // string sel;
  // std::string folder = ".";
  // bool norm_bins = false;
  // bool signal_no_stack = false;
  // bool signal_split_vbf = false;
  // double y_axis_min = -10;
  // unsigned tau_scale_mode = 0;
  // bool draw_ss = false;
  // bool swap_inclusive = false;

  // po::options_description preconfig("Pre-Configuration");
  // preconfig.add_options()
  //     ("cfg", po::value<std::string>(&configfile)->required(), 
  //           "configuration file");

  // po::variables_map vm;
  // po::store(po::command_line_parser(argc, argv).
  //           options(preconfig).allow_unregistered().run(), vm);
  // po::notify(vm);

  // po::options_description config("Configuration");
  // config.add_options()
  //   ("mode",                po::value<unsigned>(&mode)->required(), "0=elec-tau, 1=mu-tau, 2=elec-mu")
  //   ("mssm_mode",           po::value<unsigned>(&mssm_mode)->required(), "0=no, 1=yes")
  //   ("category",            po::value<unsigned>(&category)->required(), "as above")
  //   ("is_2012",             po::value<bool>(&is_2012)->required(), "0=2011, 1=2012")
  //   ("plot_name",           po::value<std::string>(&plot_name)->required(), "plot_name")
  //   ("x_axis_label",        po::value<std::string>(&x_axis_label)->required(), "x_axis_label")
  //   ("rebin",               po::value<unsigned>(&rebin)->default_value(1), "rebin")
  //   ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0), "tau_scale_mode")
  //   ("custom_x_axis_range", po::value<bool>(&custom_x_axis_range)->default_value(false), "custom_x_axis_range")
  //   ("x_axis_min",          po::value<double>(&x_axis_min)->default_value(0), "rebin")
  //   ("x_axis_max",          po::value<double>(&x_axis_max)->default_value(0), "rebin")
  //   ("y_axis_min",          po::value<double>(&y_axis_min)->default_value(-10), "rebin")
  //   ("extra_pad",           po::value<double>(&extra_pad)->default_value(1.0), "extra_pad")
  //   ("blind",               po::value<bool>(&blind)->default_value(false), "blind")
  //   ("x_blind_min",         po::value<double>(&x_blind_min)->default_value(0), "x_blind_min")
  //   ("x_blind_max",         po::value<double>(&x_blind_max)->default_value(0), "x_blind_max")
  //   ("no_plot",             po::value<bool>(&no_plot)->default_value(false), "no_plot")
  //   ("make_datacard",       po::value<bool>(&make_datacard)->default_value(false), "make_datacard")
  //   ("log_y",               po::value<bool>(&log_y)->default_value(false), "log_y")
  //   ("non_mass_plot",       po::value<bool>(&non_mass_plot)->default_value(false), "non_mass_plot")
  //   ("swap_inclusive",      po::value<bool>(&swap_inclusive)->default_value(false), "swap_inclusive")
  //   ("norm_bins",           po::value<bool>(&norm_bins)->default_value(false), "norm_bins")
  //   ("verbose",             po::value<bool>(&verbose)->default_value(false), "verbose")
  //   ("use_ztt_mc",          po::value<bool>(&use_ztt_mc)->default_value(false), "use_ztt_mc")
  //   ("signal_no_stack",     po::value<bool>(&signal_no_stack)->default_value(false), "signal_no_stack")
  //   ("shift_backgrounds",   po::value<bool>(&shift_backgrounds)->default_value(false), "shift_backgrounds")
  //   ("signal_split_vbf",    po::value<bool>(&signal_split_vbf)->default_value(false), "signal_split_vbf")
  //   ("draw_ss",             po::value<bool>(&draw_ss)->default_value(false), "draw_ss")
  //   ("draw_band_on_stack",  po::value<bool>(&draw_band_on_stack)->default_value(false), "draw_band_on_stack")
  //   ("qcd_shift",           po::value<double>(&qcd_shift)->default_value(1.0), "qcd_shift")
  //   ("top_shift",           po::value<double>(&top_shift)->default_value(1.0), "top_shift")
  //   ("ztt_shift",           po::value<double>(&ztt_shift)->default_value(1.0), "ztt_shift")
  //   ("zll_shift",           po::value<double>(&zll_shift)->default_value(1.0), "zll_shift")
  //   ("w_shift",           po::value<double>(&w_shift)->default_value(1.0), "w_shift")
  //   ("vv_shift",           po::value<double>(&vv_shift)->default_value(1.0), "vv_shift")
  //   ("band_size_fractional", po::value<double>(&band_size_fractional)->default_value(0.0), "band_size_fractional")
  //   ("folder",              po::value<std::string>(&folder)->required(), "folder")

  //   ;
  // po::store(po::command_line_parser(argc, argv).
  //     options(config).allow_unregistered().run(), vm);
  // po::store(po::parse_config_file<char>(configfile.c_str(), config), vm);
  // po::notify(vm);

  // if (is_2012) paramfile = "scripts/default_params_2012.dat";

  // // Parse the parameter file
  // SimpleParamParser parser;
  // std::cout << "Parsing parameter file: " << paramfile << std::endl;
  // parser.ParseFile(paramfile);

  // // The signal files that we'll attempt to open, and which will be added to the datacard if produced
  // vector<string> signal_masses, mssm_signal_masses;
  // std::string signal_masses_list = parser.GetParam<string>("SIGNAL_MASSES");
  // std::string mssm_signal_masses_list = parser.GetParam<string>("MSSM_SIGNAL_MASSES");
  // boost::split(signal_masses, signal_masses_list, boost::is_any_of(","));
  // boost::split(mssm_signal_masses, mssm_signal_masses_list, boost::is_any_of(","));
  // bool draw_signal = parser.GetParam<bool>("DRAW_SIGNAL");
  // std::string draw_signal_mass = parser.GetParam<string>("DRAW_SIGNAL_MASS");
  // std::string draw_mssm_signal_mass = parser.GetParam<string>("DRAW_MSSM_SIGNAL_MASS");
  // unsigned draw_signal_factor = parser.GetParam<unsigned>("DRAW_SIGNAL_FACTOR");
  // unsigned draw_mssm_signal_factor = parser.GetParam<unsigned>("DRAW_MSSM_SIGNAL_FACTOR");
  // std::string mode_label;
  // if (mode == 0) mode_label = "ElecTau";
  // if (mode == 1) mode_label = "MuTau";
  // if (mode == 2) mode_label = "ElecMu";

  // string year_label = parser.GetParam<string>("YEAR_LABEL");
  // double data_lumi = parser.GetParam<double>("LUMI_DATA");
  // string lumi_data_label = parser.GetParam<string>("LUMI_DATA_LABEL");


  // vector<string> files;
  // if (mode == 0 || mode == 1) {
  //   files.push_back("Data");
  //   files.push_back("Special_3_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
  //   files.push_back("Special_6_Data"); // QCD shape VBF
  //   files.push_back("Special_7_Data"); // VBF Norm (of W+Jets)
  //   //files.push_back("Special_10_Data"); // VBF Norm (of W+Jets)
  //   //files.push_back("Special_16_Data"); // VBF Norm (of W+Jets)
  //   files.push_back("DYJetsToTauTau"); 
  //   files.push_back("Special_7_DYJetsToTauTau"); 
  //   files.push_back("Embedded");
  //   files.push_back("DYJetsToLL");
  //   files.push_back("DYJetsToLL-L");
  //   files.push_back("DYJetsToLL-J");
  //   files.push_back("Special_7_DYJetsToLL");    // For VBF W sideband
  //   files.push_back("Special_7_DYJetsToLL-L");  // For VBF W sideband
  //   files.push_back("Special_7_DYJetsToLL-J");  // For VBF W sideband
  //   files.push_back("Special_9_DYJetsToLL");    // For b-jet shape
  //   files.push_back("Special_9_DYJetsToLL-L");  // For b-jet shape
  //   files.push_back("Special_18_DYJetsToLL-L");  // For b-jet shape
  //   files.push_back("Special_9_DYJetsToLL-J");  // For b-jet shape
  //   files.push_back("WJetsToLNuSoup");
  //   files.push_back("Special_7_WJetsToLNuSoup"); // For VBF Norm and extrapolation factor
  //   // files.push_back("Special_8_WJetsToLNuSoup"); // For B Jet Norm (just extrapolation factor)
  //   // files.push_back("Special_9_WJetsToLNuSoup"); // For B Jet Shape
  //   files.push_back("WWJetsTo2L2Nu");
  //   files.push_back("WZJetsTo2L2Q");
  //   files.push_back("WZJetsTo3LNu");
  //   files.push_back("ZZJetsTo2L2Nu");
  //   files.push_back("ZZJetsTo2L2Q");
  //   files.push_back("ZZJetsTo4L");
  //   files.push_back("T-tW");
  //   files.push_back("Tbar-tW");
  //   files.push_back("Special_7_WWJetsTo2L2Nu");   // For VV shapes VBF and BJet*
  //   files.push_back("Special_7_WZJetsTo2L2Q");    // For VV shapes VBF and BJet
  //   files.push_back("Special_7_WZJetsTo3LNu");    // For VV shapes VBF and BJet
  //   files.push_back("Special_7_ZZJetsTo2L2Nu");   // For VV shapes VBF and BJet
  //   files.push_back("Special_7_ZZJetsTo2L2Q");    // For VV shapes VBF and BJet
  //   files.push_back("Special_7_ZZJetsTo4L");      // For VV shapes VBF and BJet
  //   files.push_back("Special_7_T-tW");
  //   files.push_back("Special_7_Tbar-tW");
  //   // files.push_back("Special_9_WWJetsTo2L2Nu");   // For VV shapes VBF and BJet*
  //   // files.push_back("Special_9_WZJetsTo2L2Q");    // For VV shapes VBF and BJet
  //   // files.push_back("Special_9_WZJetsTo3LNu");    // For VV shapes VBF and BJet
  //   // files.push_back("Special_9_ZZJetsTo2L2Nu");   // For VV shapes VBF and BJet
  //   // files.push_back("Special_9_ZZJetsTo2L2Q");    // For VV shapes VBF and BJet
  //   // files.push_back("Special_9_ZZJetsTo4L");      // For VV shapes VBF and BJet
  //   // files.push_back("Special_9_T-tW");
  //   // files.push_back("Special_9_Tbar-tW");
  //   files.push_back("TTJets");
  //   files.push_back("Special_7_TTJets");          // For VBF Norm (of W+Jets)
  //   if (is_2012) files.push_back("TT");
  //   if (is_2012) files.push_back("Special_7_TT");

  // } else if (mode == 2) {
  //   files.push_back("Data");
  //   files.push_back("Special_14_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
  //   files.push_back("Special_15_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
  //   files.push_back("Special_17_Data"); // QCD shapes 0/1 Jet, B Jet, QCD Category efficiency VBF, B Jet
  //   files.push_back("DYJetsToTauTau"); 
  //   files.push_back("Embedded");
  //   files.push_back("WWJetsTo2L2Nu");
  //   files.push_back("WZJetsTo2L2Q");
  //   files.push_back("WZJetsTo3LNu");
  //   files.push_back("ZZJetsTo2L2Nu");
  //   files.push_back("ZZJetsTo2L2Q");
  //   files.push_back("ZZJetsTo4L");
  //   files.push_back("T-tW");
  //   files.push_back("Tbar-tW");
  //   files.push_back("TTJets");
  //   if (is_2012) files.push_back("TT");

  // }

  // vector<string> categories;
  // //categories.push_back(""); // Whatever category we are actually plotting for
  // categories.push_back("dilepton");
  // categories.push_back("inclusive");
  // categories.push_back("twojet");
  // categories.push_back("vbfloose");
  // categories.push_back("vbf");
  // categories.push_back("onejet_highpt");
  // categories.push_back("onejet_lowpt");
  // categories.push_back("onebjet_highpt");
  // categories.push_back("onebjet_lowpt");
  // categories.push_back("onebjet_highpt_loose");
  // categories.push_back("onebjet_lowpt_loose");
  // categories.push_back("zerojet_highpt");
  // categories.push_back("zerojet_lowpt");
  // if (mssm_mode > 0) {
  // categories.push_back("btag");
  // categories.push_back("btag_loose");
  // categories.push_back("nobtag");    
  // }

  // if (mssm_mode == 0) {
  //   for (unsigned i = 0; i < signal_masses.size(); ++i) {
  //     files.push_back("GluGluToHToTauTau_M-"+signal_masses[i]);
  //     files.push_back("VBF_HToTauTau_M-"+signal_masses[i]);
  //     files.push_back("WH_ZH_TTH_HToTauTau_M-"+signal_masses[i]);
  //   }
  // } else {
  //   for (unsigned i = 0; i < mssm_signal_masses.size(); ++i) {
  //     files.push_back("SUSYGluGluToHToTauTau_M-"+mssm_signal_masses[i]);
  //     files.push_back("SUSYBBHToTauTau_M-"+mssm_signal_masses[i]);
  //   }
  // }

  // if (tau_scale_mode == 1) folder += "/TSCALE_DOWN";
  // if (tau_scale_mode == 2) folder += "/TSCALE_UP";

  // std::map<std::string, TFile *> tfiles;
  // for (unsigned i = 0; i < files.size(); ++i) {
  //   std::string filename = (files[i]+"_"+mode_label+"_"+year_label+".root");
  //   TFile * tmp = new TFile((folder+"/"+filename).c_str());
  //   if (!tmp) {
  //     //std::cerr << "Error, file " << filename << " could not be opened, quitting." << std::endl;
  //     //exit(1);
  //     std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
  //   } else {
  //     tfiles[files[i]] = tmp;      
  //   }
  // }

  // if (category == 0) sel = "zerojet_lowpt";
  // if (category == 1) sel = "zerojet_highpt";
  // if (category == 2) sel = "onejet_lowpt";
  // if (category == 3) sel = "onejet_highpt";
  // if (category == 4) sel = "vh";
  // if (category == 5) sel = "vbf";
  // if (category == 6) sel = "onebjet_lowpt";
  // if (category == 7) sel = "onebjet_highpt";
  // if (category == 8) sel = "inclusive";
  // if (category == 9) sel = "dilepton";
  // if (category == 10) sel = "twojet";
  // if (category == 11) sel = "nobtag";
  // if (category == 12) sel = "btag";


  // // Get Plots and Scale
  // std::map<std::string, ic::TH1PlotElement> os_sel;
  // std::map<std::string, ic::TH1PlotElement> ss_sel;
  // std::map<std::string, ic::TH1PlotElement> os_con;
  // std::map<std::string, ic::TH1PlotElement> ss_con;
  // for (unsigned i = 0; i < files.size(); ++i) {
  //   for (unsigned j = 0; j < categories.size(); ++j) {
  //     string f = files[i];
  //     string c = categories[j];
  //     std::string nm = files[i]+categories[j];
  //     if (categories[j] == "") {
  //       nm = files[i];
  //       c = sel;
  //     }
  //     if (tfiles[f]->GetDirectory(("/"+c+"_os_sel").c_str())) {
  //       //std::cout << nm << std::endl;
  //       os_sel[nm] = ic::TH1PlotElement(nm+"_os_sel", tfiles[f], "/"+c+"_os_sel", plot_name);
  //       ss_sel[nm] = ic::TH1PlotElement(nm+"_ss_sel", tfiles[f], "/"+c+"_ss_sel", plot_name);
  //       os_con[nm] = ic::TH1PlotElement(nm+"_os_con", tfiles[f], "/"+c+"_os_con", plot_name);
  //       ss_con[nm] = ic::TH1PlotElement(nm+"_ss_con", tfiles[f], "/"+c+"_ss_con", plot_name);
  //       if (os_sel[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
  //       if (ss_sel[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
  //       if (os_con[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
  //       if (ss_con[nm].hist_ptr()->GetSumw2N() == 0) os_sel[nm].hist_ptr()->Sumw2();
  //       string lookup;
  //       if (f.find("Special") != f.npos) {
  //         size_t found = f.find('_');
  //         found = f.find('_',found+1);
  //         lookup = f.substr(found+1);
  //       } else {
  //         lookup = f;
  //       }
  //       double sample_events = parser.GetParam<double>("EVT_"+lookup);
  //       double sample_xs = parser.GetParam<double>("XS_"+lookup);
  //       double sample_lumi = sample_events / sample_xs;
  //       if (sample_xs > 0) {
  //         //std::cout << "Scale: " << nm << " EVT: " << sample_events << " XS: " << sample_xs << std::endl;
  //         os_sel[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
  //         ss_sel[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
  //         os_con[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
  //         ss_con[nm].hist_ptr()->Scale( data_lumi / sample_lumi );
  //       }
  //       os_sel[nm].hist_ptr()->Rebin(rebin);
  //       ss_sel[nm].hist_ptr()->Rebin(rebin);
  //       os_con[nm].hist_ptr()->Rebin(rebin);
  //       ss_con[nm].hist_ptr()->Rebin(rebin); 
  //     }
  //   }
  // }





  return 0;
}

