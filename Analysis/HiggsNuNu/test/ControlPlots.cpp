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
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/Utilities.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

void SumHistograms(std::string lName, ic::TH1PlotElement & lEle,std::string lStr, ic::TH1PlotElement & lHist){
  if (lName.find(lStr) != lName.npos) {
    if (!lHist.hist_ptr()) lHist.SetElement((TH1F*)(lEle.hist_ptr()->Clone()));
    else (lHist.hist_ptr())->Add((TH1F*)(lEle.hist_ptr()->Clone()));
  }
}

string Token(string const& file, string const& selection) {
	return (file+"_"+selection);
}

double Integral(TH1F const* hist) {
  if (hist) {
    return hist->Integral(0, hist->GetNbinsX() + 1);
  }
  else return 0;
}

double Error(TH1F const* hist) {
  double err = 0.0;
  if (hist) {
    hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
  }
  return err;
}



void SetBkgStyle(ic::TH1PlotElement & ele, unsigned color) {
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


void SetSignalStyle(ic::TH1PlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_marker_style(1);
  ele.set_line_color(color);
  ele.set_fill_color(color);
  ele.set_fill_style(0);
  ele.set_draw_fill(false);
  ele.set_draw_marker(false);
  ele.set_draw_line(true);
  ele.set_smooth_curve(true);
  ele.set_line_width(3);
  ele.set_draw_stat_error_y(false);
  ele.set_in_stack(false);
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
  ele.set_marker_style(20);

  return;
}


int main(int argc, char* argv[]){

	// Configurable parameters
  string cfg;			     // The configuration file
  string paramfile;		     // The parameters files									

  string plot_name;		     // Name of the plot to draw
  string folder;		     // Folder containing input files
  bool is_2012;			     // false = 7 TeV, true = 8 TeV
  bool no_plot;			     // Don't actually generate image files if true
  bool verbose;			     // Verbose output, useful for diagnostic purposes

  // Plotting options
  string x_axis_label;		     // Label for the X-axis
  string x_axis_bin_labels;          // of the form "a:b:c"
  unsigned rebin;		     // Rebin factor
  bool custom_x_axis_range;	     // Can optionally specify an x-axis range
  double x_axis_min;		     // If custom_x_axis_range is true, use this as min
  double x_axis_max;		     // If custom_x_axis_range is true, use this as max
  bool custom_y_axis_range;	     // Can optionally specify a y-axis range
  double y_axis_min;		     // If custom_y_axis_range is true, use this as min
  double y_axis_max;		     // If custom_y_axis_range is true, use this as max
  double extra_pad;		     // Expand the y-axis by an additional factor
  bool blind;			     // Blind some region of the data
  double x_blind_min;		     // If bind is true, use this as min x for blinding
  double x_blind_max;		     // If bind is true, use this as min x for blinding
  bool log_y;			     // Draw plot in log scale
  bool norm_bins;		     // Normalise using bin width
  bool signal_no_stack;		     // Don't stack the signal contributions on the backgrounds
  bool draw_ratio;                   // Draw a ratio box

  // Options to manually shift backgrounds and draw uncertainty bands
  bool shift_backgrounds = false;
  double qcd_shift = 1.0;
  double top_shift = 1.0;
  double ztt_shift = 1.0;
  double zll_shift = 1.0;
  double w_shift = 1.0;
  double vv_shift = 1.0;
  bool draw_band_on_stack = false;
  double band_size_fractional = 0.0;

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
    ("is_2012",             po::value<bool>(&is_2012)->required())
    ("draw_ratio",          po::value<bool>(&draw_ratio)->default_value(false))
    ("no_plot",             po::value<bool>(&no_plot)->default_value(false))
    ("verbose",             po::value<bool>(&verbose)->default_value(false))
    ("x_axis_label",        po::value<string>(&x_axis_label)->required())
    ("x_axis_bin_labels",   po::value<string>(&x_axis_bin_labels)->default_value(""))
    ("rebin",               po::value<unsigned>(&rebin)->default_value(1))
    ("custom_x_axis_range", po::value<bool>(&custom_x_axis_range)->default_value(false))
    ("x_axis_min",          po::value<double>(&x_axis_min)->default_value(0))
    ("x_axis_max",          po::value<double>(&x_axis_max)->default_value(0))
    ("custom_y_axis_range", po::value<bool>(&custom_y_axis_range)->default_value(false))
    ("y_axis_min",          po::value<double>(&y_axis_min)->default_value(0))
    ("y_axis_max",          po::value<double>(&y_axis_max)->default_value(0))
    ("extra_pad",           po::value<double>(&extra_pad)->default_value(1.0))
    ("blind",               po::value<bool>(&blind)->default_value(false))
    ("x_blind_min",         po::value<double>(&x_blind_min)->default_value(0))
    ("x_blind_max",         po::value<double>(&x_blind_max)->default_value(0))
    ("log_y",               po::value<bool>(&log_y)->default_value(false))
    ("norm_bins",           po::value<bool>(&norm_bins)->default_value(false))
    ("signal_no_stack",     po::value<bool>(&signal_no_stack)->default_value(false))
    ("shift_backgrounds",   po::value<bool>(&shift_backgrounds)->default_value(false))
    ("draw_band_on_stack",  po::value<bool>(&draw_band_on_stack)->default_value(false))
    ("qcd_shift",           po::value<double>(&qcd_shift)->default_value(1.0))
    ("top_shift",           po::value<double>(&top_shift)->default_value(1.0))
    ("ztt_shift",           po::value<double>(&ztt_shift)->default_value(1.0))
    ("zll_shift",           po::value<double>(&zll_shift)->default_value(1.0))
    ("w_shift",             po::value<double>(&w_shift)->default_value(1.0))
    ("vv_shift",            po::value<double>(&vv_shift)->default_value(1.0))
    ("band_size_fractional", po::value<double>(&band_size_fractional)->default_value(0.0));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  std::cout << "**** ControlPlots *****" << std::endl;

  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "paramfile" 	% paramfile;
  std::cout << boost::format(param_fmt) % "plot_name" 	% plot_name;
  std::cout << boost::format(param_fmt) % "folder" 			% folder;
  std::cout << boost::format(param_fmt) % "is_2012" 		% is_2012;
  std::cout << boost::format(param_fmt) % "no_plot" 		% no_plot;

  // Parse the parameter file
  SimpleParamParser parser;
  std::cout << "** Parsing parameter file... **" << paramfile << std::endl;
  parser.ParseFile(paramfile);


  bool draw_signal = parser.GetParam<bool>("DRAW_SIGNAL");
  unsigned draw_signal_factor = parser.GetParam<unsigned>("DRAW_SIGNAL_FACTOR");
  string year_label = parser.GetParam<string>("YEAR_LABEL");
  double data_lumi = parser.GetParam<double>("LUMI_DATA");
  string lumi_data_label = parser.GetParam<string>("LUMI_DATA_LABEL");


  vector<string> files;
  files.push_back("Data_MET-2012A-06Aug2012-v1");
  files.push_back("Data_MET-2012A-13Jul2012-v1");
  files.push_back("Data_MET-2012B-13Jul2012-v1");
  files.push_back("Data_MET-2012C-24Aug2012-v1");
  files.push_back("Data_MET-2012C-11Dec2012-v1");
  files.push_back("Data_MET-2012C-PromptReco-v2");
  files.push_back("Data_MET-2012D-PromptReco-v1");
  files.push_back("MC_QCD-Pt-30to50-pythia6");
  files.push_back("MC_QCD-Pt-50to80-pythia6");
  files.push_back("MC_QCD-Pt-80to120-pythia6");
  files.push_back("MC_QCD-Pt-120to170-pythia6");
  files.push_back("MC_QCD-Pt-170to300-pythia6");
  files.push_back("MC_QCD-Pt-300to470-pythia6");
  files.push_back("MC_QCD-Pt-470to600-pythia6");
  files.push_back("MC_QCD-Pt-600to800-pythia6");
  files.push_back("MC_QCD-Pt-800to1000-pythia6");
  files.push_back("MC_QCD-Pt-1000to1400-pythia6");
  files.push_back("MC_QCD-Pt-1400to1800-pythia6");
  files.push_back("MC_QCD-Pt-1800-pythia6");
  files.push_back("MC_TTJets");
  files.push_back("MC_T-tW");
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");
  files.push_back("MC_W1JetsToLNu");
  files.push_back("MC_W2JetsToLNu");
  files.push_back("MC_W3JetsToLNu");
  files.push_back("MC_W4JetsToLNu");
  files.push_back("MC_WJetsToLNu-v1");
  files.push_back("MC_WJetsToLNu-v2");
  files.push_back("MC_DYJetsToLL");
  //files.push_back("MC_DY1JetsToLL");
  //files.push_back("MC_DY2JetsToLL");
  //files.push_back("MC_DY3JetsToLL");
  //files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_GJets-HT-200To400-madgraph");
  files.push_back("MC_GJets-HT-400ToInf-madgraph");
  files.push_back("MC_VBF_HToZZTo4Nu_M-120");

  //build a list of selections
  vector<string> selections;
  selections.push_back("JetPair");
  selections.push_back("MET");
  selections.push_back("Mjj");
  selections.push_back("DEta");
  selections.push_back("LeptonVeto");
  selections.push_back("DPhi");

  std::map<std::string, TFile *> tfiles;
  for (unsigned i = 0; i < files.size(); ++i) {
    std::string filename = (files[i]+".root");
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
    for (unsigned k = 0; k < selections.size(); ++k) {

      string f = files[i];
      string s = selections[k];

      std::string nm = Token(f, s);
      if (plots.count(nm)) continue;
      if (verbose) std::cout << "Adding plot: " << nm << std::endl;

      if (tfiles[f]->GetDirectory(("/"+s).c_str())) {
	plots[nm] = ic::TH1PlotElement(nm, tfiles[f], "/"+s, plot_name);
	if (plots[nm].hist_ptr()->GetSumw2N() == 0) {
	  cout << "Warning: Plot " << nm << " does not have a weights structure" << endl;
	  plots[nm].hist_ptr()->Sumw2();
	}
	string lookup = f;
	double sample_events = parser.GetParam<double>("EVT_"+lookup);
	double sample_xs = parser.GetParam<double>("XS_"+lookup);
	double sample_lumi = sample_events / sample_xs;
	double sample_scale = 1;
	if (sample_xs > 0) sample_scale = data_lumi / sample_lumi;
	plots[nm].hist_ptr()->Scale(sample_scale);
	plots[nm].hist_ptr()->Rebin(rebin);
	
	//if (k==0) std::cout << f << " " << sample_scale << std::endl;

      }

    }//loop on selections
  }//loop on files

  
  //output a table with number of events selected
   if (plot_name.find("n_jets") != plot_name.npos) {
      std::cout << "\\begin{table}[h!]" << std::endl
		<< "\\caption{Number of events selected in data and MC." << std::endl
		<< "\\begin{tabular}{|l|c|c|c|c|c||c|c||c|}" << std::endl
		<<"\\hline" << std::endl
		<< "Step & QCD & Top & W+jets & Z+jets & Others & SumMC & Data & Signal 120 \\\\"
		<< std::endl
		<< "\\hline" << std::endl;
   }


  for (unsigned k = 0; k < selections.size(); ++k) {
    ic::Plot plot;
    plot.output_filename = "PLOTS/"+plot_name + "_" + year_label + "_" + selections[k] + ".pdf";
    if (log_y) plot.output_filename = "PLOTS/"+plot_name + "_" + year_label + "_" + selections[k]  + "_log.pdf";
  
    plot.x_bin_labels_ = x_axis_bin_labels;

    ic::TH1PlotElement data_hist = ic::TH1PlotElement("Data");//+selections[k]);
    ic::TH1PlotElement signal_hist = ic::TH1PlotElement("Signal");
    ic::TH1PlotElement qcd_hist = ic::TH1PlotElement("QCD");
    ic::TH1PlotElement top_hist = ic::TH1PlotElement("Top");
    ic::TH1PlotElement WJets_hist = ic::TH1PlotElement("WJets");
    ic::TH1PlotElement ZJetsToLL_hist = ic::TH1PlotElement("ZJetsToLL");
    ic::TH1PlotElement ZJetsToNuNu_hist = ic::TH1PlotElement("ZJetsToNuNu");
    ic::TH1PlotElement VBFZ_hist = ic::TH1PlotElement("VBFZ");
    ic::TH1PlotElement GJets_hist = ic::TH1PlotElement("GJets");
 
    for (unsigned i = 0; i < files.size(); ++i) {
      string f = files[i];
      string s = selections[k];

      std::string nm = Token(f, s);
      SumHistograms(f,plots[nm],"Data",data_hist);
      SumHistograms(f,plots[nm],"VBF_H",signal_hist);
      SumHistograms(f,plots[nm],"MC_QCD",qcd_hist);
      SumHistograms(f,plots[nm],"MC_T",top_hist);
      SumHistograms(f,plots[nm],"MC_W",WJets_hist);
      SumHistograms(f,plots[nm],"JetsToLL",ZJetsToLL_hist);
      SumHistograms(f,plots[nm],"ZJetsToNuNu",ZJetsToNuNu_hist);
      SumHistograms(f,plots[nm],"DYJJ",VBFZ_hist);
      SumHistograms(f,plots[nm],"GJets",GJets_hist);
 
    }//loop on files

    (signal_hist.hist_ptr())->Scale(draw_signal_factor);

    SetSignalStyle(signal_hist,2);
    SetDataStyle(data_hist);
    SetBkgStyle(qcd_hist,7);
    SetBkgStyle(top_hist,5);
    SetBkgStyle(WJets_hist,6);
    SetBkgStyle(ZJetsToLL_hist,3);
    SetBkgStyle(ZJetsToNuNu_hist,3);
    SetBkgStyle(VBFZ_hist,3);
    SetBkgStyle(GJets_hist,7);

    data_hist.set_legend_text("Data");
    signal_hist.set_legend_text("VBF m_{H}=120 GeV #times"+boost::lexical_cast<std::string>(draw_signal_factor));
    qcd_hist.set_legend_text("QCD,#gamma+jets");
    //GJets_hist.set_legend_text("#gamma + jets");
    top_hist.set_legend_text("t#bar{t},tW,#bar{t}W");
    WJets_hist.set_legend_text("W+jets");
    //VBFZ_hist.set_legend_text("VBF Z+2j");
    //ZJetsToLL_hist.set_legend_text("Z#rightarrow ll + jets");
    //ZJetsToNuNu_hist.set_legend_text("Z#rightarrow #nu#nu + jets");
    ZJetsToNuNu_hist.set_legend_text("Z+jets,EWK Z");

    plot.AddTH1PlotElement(ZJetsToNuNu_hist);
    plot.AddTH1PlotElement(ZJetsToLL_hist);
    plot.AddTH1PlotElement(VBFZ_hist);
    plot.AddTH1PlotElement(WJets_hist);
    plot.AddTH1PlotElement(top_hist);
    plot.AddTH1PlotElement(GJets_hist);
    plot.AddTH1PlotElement(qcd_hist);
    
    plot.AddTH1PlotElement(data_hist);

    if (draw_signal) {
      plot.AddTH1PlotElement(signal_hist);
    }

    plot.extra_pad = extra_pad;
    plot.custom_x_axis_range = custom_x_axis_range;
    if (custom_x_axis_range){
      plot.x_axis_min = x_axis_min;
      plot.x_axis_max = x_axis_max;
    }
    if (log_y) plot.y_axis_log = true;
    if (y_axis_min > 0) plot.y_axis_min = y_axis_min;
    if (custom_y_axis_range){
      plot.y_axis_min = y_axis_min;
      plot.y_axis_max = y_axis_max;
    }
    plot.legend_height = 0.045;
    plot.x_axis_title = x_axis_label;
    plot.y_axis_title = "Events";
    plot.title_left = "CMS Preliminary " + year_label +", #sqrt{s} = " + (is_2012 ? "8":"7") +" TeV, "+ lumi_data_label;
    plot.title_right = selections[k];
    

    //ratio plot

    plot.draw_ratio_hist = draw_ratio;
    plot.draw_signif = false;
    
    string background_list = "GJets+QCD+Top+WJets+ZJetsToLL+VBFZ+ZJetsToNuNu";
    ic::RatioPlotElement ratio("DataOverMC","Data",background_list);
    
    plot.band_size_fractional_ = band_size_fractional;
    plot.draw_band_on_stack_ = draw_band_on_stack;
    plot.samples_for_band_ = "GJets+QCD+Top+WJets+ZJetsToLL+VBFZ+ZJetsToNuNu";
    
    SetStyle(ratio,1);
    ratio.set_multi_mode(true);
    //plot.ratio_y_axis_title = "#frac{(N_{obs}-N_{exp})}{#sqrt{N_{obs}}}";
    plot.ratio_y_axis_title = "Data/MC";
    plot.AddRatioPlotElement(ratio);
    plot.custom_ratio_y_axis_range = true;
    plot.ratio_y_axis_min = 0.5;
    plot.ratio_y_axis_max = 1.5;
    
    
    if (!no_plot) plot.GeneratePlot();

    if (plot_name.find("n_jets") != plot_name.npos && plot_name.find("ingap") == plot_name.npos) {

      Utilities n_qcd = Utilities(Integral(qcd_hist.hist_ptr()),Error(qcd_hist.hist_ptr()));
      Utilities n_top = Utilities(Integral(top_hist.hist_ptr()),Error(top_hist.hist_ptr()));
      Utilities n_WJets = Utilities(Integral(WJets_hist.hist_ptr()),Error(WJets_hist.hist_ptr()));
      Utilities n_ZJets = Utilities(Integral(ZJetsToNuNu_hist.hist_ptr())
  				    +Integral(ZJetsToLL_hist.hist_ptr())
  				    +Integral(VBFZ_hist.hist_ptr()),
  				    Error(ZJetsToNuNu_hist.hist_ptr())
  				    +Error(ZJetsToLL_hist.hist_ptr())
  				    +Error(VBFZ_hist.hist_ptr())
  				    );
      Utilities n_others = Utilities(Integral(GJets_hist.hist_ptr()),Error(GJets_hist.hist_ptr()));
      Utilities n_data = Utilities(Integral(data_hist.hist_ptr()),Error(data_hist.hist_ptr()));
      Utilities n_signal = Utilities(Integral(signal_hist.hist_ptr()),Error(signal_hist.hist_ptr()));
      Utilities n_Tot = Utilities(n_qcd.rawNumber()+n_top.rawNumber()+n_WJets.rawNumber()+n_ZJets.rawNumber()+n_others.rawNumber(),
       				  n_qcd.rawError()+n_top.rawError()+n_WJets.rawError()+n_ZJets.rawError()+n_others.rawError());
       
      //std::cout.precision(2);
      //std::cout << std::scientific;

      std::cout << selections[k] << " & " 
		<< n_qcd.roundedResult() << " & " 
		<< n_top.roundedResult() << " & "
		<< n_WJets.roundedResult() << " & "
		<< n_ZJets.roundedResult() << " & "
		<< n_others.roundedResult() << " & "
		<< n_Tot.roundedResult() << " & "
 		<< n_data.roundedNumber() << " & "
 		<< n_signal.roundedResult()
		<< " \\\\ "
		<< std::endl; 
    }



  }//loop on selection

   if (plot_name.find("n_jets") != plot_name.npos) {
     std::cout << "\\hline" << std::endl
	       << "\\end{tabular}" << std::endl
	       << "\\end{table}" << std::endl;
   }

   return 0;
   
}//main
