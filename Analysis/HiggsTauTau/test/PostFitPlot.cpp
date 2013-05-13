#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"

#include "TLatex.h"
#include "TH2F.h"
#include "TStyle.h"


#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/mssm_xs_tools.h"


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
  ele.set_marker_size(1.0);
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


  string channel        = "";
  string selection      = "";
  string eras           = "";
  string pulls_file     = "";
  string datacard_path  = "";
  string root_file_path = "";
  string signal_mass    = "";
  string title_left     = "";
  string tanb           = "";
  bool postfit          = true;
  bool mssm             = false;
  bool log_y            = false;
  string default_title  = "CMS Preliminary, #sqrt{s} = 7-8 TeV, L = 24.3 fb^{-1}";
  int signal_factor     = 1;
  bool blind;                                   // Blind some region of the data
  double x_blind_min;                           // If bind is true, use this as min x for blinding
  double x_blind_max;                           // If bind is true, use this as min x for blinding

  po::options_description config("Configuration");
  config.add_options()
    ("help,h",  "print the help message")
    ("channel",              po::value<string>(&channel)->required(),         "[REQUIRED] channel, choose one of [et,mt,em,tt]")
    ("selection",            po::value<string>(&selection)->required(),       "[REQUIRED] categories to combine with label, e.g. \"0-jet:0+1\" combines the 0-jet low and high categories")
    ("eras",                 po::value<string>(&eras)->required(),            "[REQUIRED] data-taking periods to combine, e.g. \"7TeV,8TeV\"")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),   "[REQUIRED] path to the folder containing datacard *.txt files")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),  "[REQUIRED] path to the folder containing datacard *.root files")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),      "[REQUIRED] path to the file containing the pulls from a maximum-likelihood fit")
    ("signal_mass",          po::value<string>(&signal_mass)->required(),     "[REQUIRED] signal mass point to plot, though note this will not be drawn if the total signal rate is zero")
    ("postfit",              po::value<bool>(&postfit)->required(),           "[REQUIRED] use the pulls file to make a post-fit plot")
    ("tanb",                 po::value<string>(&tanb)->default_value(""),     "the value of tan(beta) for the signal, will be used to calculate cross sections. Required if making an MSSM plot")
    ("title_left",           po::value<string>(&title_left)->default_value(default_title),   "the plot title")
    ("mssm",                 po::value<bool>(&mssm)->default_value(false),                   "input is an MSSM datacard")
    ("log_y",                po::value<bool>(&log_y)->default_value(false),                  "y-axis in log scale")
    ("signal_factor",        po::value<int>(&signal_factor)->default_value(1),               "scale the signal by an integer factor")
    ("blind",                po::value<bool>(&blind)->default_value(false),  "bind the data distribution")
    ("x_blind_min",          po::value<double>(&x_blind_min)->default_value(0), "set the min x-axis value for blinding")
    ("x_blind_max",          po::value<double>(&x_blind_max)->default_value(0), "set the max x-axis value for blinding");
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  if (vm.count("help")) {
      cout << config << "\n";
      return 1;
  }
  po::notify(vm);


  vector<string> signal_procs = {"ggH", "qqH", "VH"};
  if (mssm) signal_procs = {"ggH","bbH"};


  vector<string> v_eras;
  boost::split(v_eras, eras, boost::is_any_of(","));

  pair<string,vector<string>> v_columns;
  vector<string> tmp_split;
  boost::split(tmp_split, selection, boost::is_any_of(":"));
  if (tmp_split.size() == 2) {
    vector<string> tmp_cats;
    boost::split(tmp_cats, tmp_split[1], boost::is_any_of("+"));
    v_columns = make_pair(tmp_split[0],tmp_cats);
  }

  HTTSetup setup;
  for (unsigned j = 0; j < v_eras.size(); ++j) {
    for (unsigned k = 0; k < v_columns.second.size(); ++k) {
      string cat = v_columns.second[k];
      setup.ParseDatacard(datacard_path+"/"+"htt_"+channel+"_"+cat+"_"+v_eras[j]+".txt", channel, boost::lexical_cast<int>(cat), v_eras[j], signal_mass);        
    }
  }
  for (unsigned i = 0; i < v_eras.size(); ++i) {
    if (!mssm) {
      setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".input_"+v_eras[i]+".root", channel, v_eras[i]);
    } else {
      setup.ParseROOTFile(root_file_path+"/"+"htt_"+channel+".inputs-mssm-"+v_eras[i]+"-0.root", channel, v_eras[i]);
    }
  }
  setup.ParsePulls(pulls_file);
  if (postfit) setup.ApplyPulls();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "Z->TauTau" 
    % setup.process({"ZTT","Ztt"}).GetRate() 
    % setup.process({"ZTT","Ztt"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "QCD" 
    % setup.process({"QCD","Fakes"}).GetRate() 
    % setup.process({"QCD","Fakes"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "EWK" 
    % setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetRate() 
    % setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "ttbar" 
    % setup.process({"TT","ttbar"}).GetRate() 
    % setup.process({"TT","ttbar"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "Total" 
    % setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetRate() 
    % setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "H->TauTau" 
    % setup.process(signal_procs).GetRate() 
    % setup.process(signal_procs).GetUncertainty();

    std::cout << boost::format("%-20s %-10.1f \n\n") % "Observed" 
      % setup.GetObservedRate();


  ic::Plot plot;
  plot.use_htt_style = true;
  string catstring = "";
  for (unsigned i = 0; i < v_columns.second.size(); ++i) catstring += v_columns.second.at(i);
  plot.output_filename = (postfit ? "postfit_":"prefit_") + channel + "_" + catstring + ".pdf";



  TH1F signal_hist = setup.process(signal_procs).GetShape();
  // std::cout << "Pre-scaled Signal: " << Integral(&signal_hist) << std::endl;
  if (mssm) {
    signal_hist.Reset("ICE");
    double d_mass = boost::lexical_cast<double>(signal_mass);
    double d_tanb = boost::lexical_cast<double>(tanb);
    std::cout << "*** Finding MSSM cross sections for mA = " << d_mass << " tan(beta) = " << d_tanb << std::endl;
    std::cout << "*****************************************************************************" << std::endl;

    for (unsigned i = 0; i < v_eras.size(); ++i) {
      mssm_xs_tools xs_tool;
      string file;
      if (v_eras[i] == "7TeV") {
        file = "data/scale_factors/out.mhmax_mu200_7_nnlo.tanBeta_gte1.root";
      } else if (v_eras[i] == "8TeV") {
        file = "data/scale_factors/out.mhmax_mu200_8_nnlo.tanBeta_gte1_FHv274.root";
      } else {
        continue;
      }
      xs_tool.SetInput(file.c_str());
      std::cout << "*****************************************************************************" << std::endl;
      double br =  xs_tool.Give_BR_A_tautau(d_mass, d_tanb);
      double xs_ggh = xs_tool.Give_Xsec_ggFA(d_mass, d_tanb) / 1000.;
      double xs_bbh = xs_tool.Give_Xsec_bbA5f(d_mass, d_tanb) / 1000.;
      std::cout << "Era: " << v_eras[i] << " BR: " << br << " XS(ggH): " << xs_ggh << " XS(bbH): " << xs_bbh << std::endl; 
      TH1F ggh_hist = setup.era({v_eras[i]}).process({"ggH"}).GetShape();
      TH1F bbh_hist = setup.era({v_eras[i]}).process({"bbH"}).GetShape();
      std::cout << "Scaling ggH: " << Integral(&ggh_hist) << " ---> " << Integral(&ggh_hist)*br*xs_ggh << endl;
      std::cout << "Scaling bbH: " << Integral(&bbh_hist) << " ---> " << Integral(&bbh_hist)*br*xs_bbh << endl;
      ggh_hist.Scale(br * xs_ggh);
      bbh_hist.Scale(br * xs_bbh);
      signal_hist.Add(&ggh_hist);
      signal_hist.Add(&bbh_hist);
    }
  }

  signal_hist.Scale(signal_factor);

  string signal_str = boost::lexical_cast<string>(signal_factor)+"#times H("+signal_mass+")#rightarrow#tau#tau";
  if (mssm) signal_str = boost::lexical_cast<string>(signal_factor)+"#times #Phi#rightarrow#tau#tau(m_{A}="+signal_mass+",tan#beta="+tanb+")";
  ic::TH1PlotElement signal_shape("signal_shape", &signal_hist, signal_str);

  SetStyle(signal_shape, kBlue+3);
  signal_shape.set_line_style(11);
  signal_shape.set_fill_color(0);
  signal_shape.set_line_color(kBlue+3);
  signal_shape.set_line_width(3);
  if (log_y) signal_shape.set_in_stack(false);


  TH1F ztt_hist = setup.process({"ZTT","Ztt"}).GetShape();
  ic::TH1PlotElement ztt_shape("ztt_shape", &ztt_hist, "Z#rightarrow#tau#tau");
  SetStyle(ztt_shape, kOrange - 4);

  TH1F qcd_hist = setup.process({"QCD","Fakes"}).GetShape();
  ic::TH1PlotElement qcd_shape("qcd_shape", &qcd_hist, "QCD");
  SetStyle(qcd_shape, kMagenta-10);

  vector<string> ewk_str = {"W","VV","EWK"};
  if (channel != "et") {
    ewk_str.push_back("ZLL");
    ewk_str.push_back("ZL");
    ewk_str.push_back("ZJ");
  }

  TH1F ewk_hist = setup.process(ewk_str).GetShape();
  ic::TH1PlotElement ewk_shape("ewk_shape", &ewk_hist, "electroweak");
  SetStyle(ewk_shape, kRed    + 2);

  TH1F zee_hist = setup.process({"ZLL","ZL","ZJ"}).GetShape();
  ic::TH1PlotElement zee_shape("zee_shape", &zee_hist, "Z#rightarrowee");
  SetStyle(zee_shape, kAzure  + 2);


  TH1F top_hist = setup.process({"TT","ttbar"}).GetShape();
  ic::TH1PlotElement top_shape("top_shape", &top_hist, "t#bar{t}");
  SetStyle(top_shape, kBlue   - 8);

  TH1F total_hist = setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetShape();
  ic::TH1PlotElement total_shape("total_shape", &total_hist,"bkg. uncertainty");

  TH1F data_hist = setup.GetObservedShape();

  if (blind) {
    for (int j = 0; j < data_hist.GetNbinsX(); ++j) {
      double low_edge = data_hist.GetBinLowEdge(j+1);
      double high_edge = data_hist.GetBinWidth(j+1)+data_hist.GetBinLowEdge(j+1);
      if ((low_edge > x_blind_min && low_edge < x_blind_max) || (high_edge > x_blind_min && high_edge < x_blind_max)) {
        data_hist.SetBinContent(j+1,0);
        data_hist.SetBinError(j+1,0);
      }
    }
  }

  ic::TH1PlotElement data_shape("data_shape", &data_hist, "observed");
  SetDataStyle(data_shape);

  std::vector<ic::TH1PlotElement *> drawn_ele;
  drawn_ele.push_back(&qcd_shape);
  drawn_ele.push_back(&ewk_shape);
  drawn_ele.push_back(&zee_shape);
  drawn_ele.push_back(&ztt_shape);
  drawn_ele.push_back(&top_shape);
  drawn_ele.push_back(&signal_shape);
  drawn_ele.push_back(&data_shape);
  drawn_ele.push_back(&total_shape);

  for (unsigned i = 0; i < drawn_ele.size(); ++i) {
     drawn_ele[i]->set_line_width(2);
     drawn_ele[i]->hist_ptr()->Scale(1.0, "width");
  }
  total_shape.set_marker_size(0);
  total_shape.set_fill_color(1);
  total_shape.set_fill_style(3013);
  total_shape.set_line_width(1);
  total_shape.set_draw_stat_error_y(true);
  total_shape.set_draw_fill(true);
  total_shape.set_draw_line(false);
  total_shape.set_draw_marker(false);
  total_shape.set_draw_options("e2");


  plot.AddTH1PlotElement(qcd_shape);
  plot.AddTH1PlotElement(top_shape);
  plot.AddTH1PlotElement(ewk_shape);
  if (channel == "et") plot.AddTH1PlotElement(zee_shape);
  plot.AddTH1PlotElement(ztt_shape);
  if (setup.process(signal_procs).GetRate() != 0.0) plot.AddTH1PlotElement(signal_shape);
  plot.AddTH1PlotElement(total_shape);
  plot.AddTH1PlotElement(data_shape);
  plot.legend_height = 0.045;
  plot.x_axis_title = "M_{#tau#tau} [GeV]";
  plot.y_axis_title = "dN/dm_{#tau#tau} [1/GeV]";
  plot.title_left = title_left;
  string channel_str = "";
  if (channel == "et") channel_str = "e#tau_{h}";
  if (channel == "mt") channel_str = "#mu#tau_{h}";
  if (channel == "em") channel_str = "e#mu";
  if (channel == "tt") channel_str = "#tau_{h}#tau_{h}";
  ic::TextElement text(channel_str,0.05,0.22,0.87);
  ic::TextElement text2(v_columns.first,0.05,0.22,0.80);
  plot.AddTextElement(text);
  plot.AddTextElement(text2);
  plot.extra_pad = 1.1;
  if (log_y) plot.y_axis_log = true;
  if (log_y) plot.extra_pad = 5.;

  plot.GeneratePlot();




  return 0;
}

