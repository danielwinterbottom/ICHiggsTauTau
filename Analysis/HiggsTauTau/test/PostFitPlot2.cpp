#include <iostream>
#include <vector>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPlotTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/mssm_xs_tools.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

HTTAnalysis::HistValuePair FillHistValuePair(HTTSetup setup) {
  return std::make_pair(
      setup.GetShape(), 
      std::make_pair(setup.GetRate(), setup.GetUncertainty()));
}


int main(int argc, char* argv[]){
  string cfg;                                   // The configuration file
  string channel        = "";
  string selection      = "";
  string eras           = "";
  string pulls_file     = "";
  string datacard_path  = "";
  string root_file_path = "";
  bool postfit          = true;
  bool mssm             = false;
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    ("help,h",  "print the help message")
    ("channel",              po::value<string>(&channel)->required(),         "[REQUIRED] channel, choose one of [et,mt,em,tt]")
    ("selection",            po::value<string>(&selection)->required(),       "[REQUIRED] categories to combine with label, e.g. \"0-jet:0+1\" combines the 0-jet low and high categories")
    ("eras",                 po::value<string>(&eras)->required(),            "[REQUIRED] data-taking periods to combine, e.g. \"7TeV,8TeV\"")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),   "[REQUIRED] path to the folder containing datacard *.txt files")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),  "[REQUIRED] path to the folder containing datacard *.root files")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),      "[REQUIRED] path to the file containing the pulls from a maximum-likelihood fit")
    ("postfit",              po::value<bool>(&postfit)->required(),           "[REQUIRED] use the pulls file to make a post-fit plot")
    ("mssm",                 po::value<bool>(&mssm)->default_value(false),                   "input is an MSSM datacard");
  HTTPlot plot;
  config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config, true), vm);
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

  string signal_mass = plot.draw_signal_mass();
  string tanb = plot.draw_signal_tanb();
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

  HTTAnalysis::HistValueMap hmap;

  hmap["data_obs"] = make_pair(setup.GetObservedShape(), 
    make_pair(setup.GetObservedRate(), sqrt(setup.GetObservedRate())));
  HTTAnalysis::PrintValue("data_obs", hmap["data_obs"].second);

  vector<string> samples;
  if (channel != "em") {
    samples = {"ZTT","QCD","W","ZL","ZJ","ZLL","VV","TT"};
  } else {
    samples = {"Ztt","Fakes","EWK","ttbar"};
  }
  for (auto const& s : samples) {
    hmap[s] = FillHistValuePair(setup.process({s}));
    HTTAnalysis::PrintValue(s, hmap[s].second);
  }
  for (auto const& s : signal_procs) {
    hmap[s+signal_mass] = FillHistValuePair(setup.process({s}));
    HTTAnalysis::PrintValue(s+signal_mass, hmap[s+signal_mass].second);
  }

  string catstring = "";
  for (unsigned i = 0; i < v_columns.second.size(); ++i) catstring += v_columns.second.at(i);
  plot.set_plot_name((postfit ? "postfit_":"prefit_") + channel + "_" + catstring);

  if (mssm) {
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
      TH1F & ggh_hist = hmap["ggH"+signal_mass].first;
      TH1F & bbh_hist = hmap["bbH"+signal_mass].first;
      std::cout << "Scaling ggH: " << Integral(&ggh_hist) << " ---> " << Integral(&ggh_hist)*br*xs_ggh << endl;
      std::cout << "Scaling bbH: " << Integral(&bbh_hist) << " ---> " << Integral(&bbh_hist)*br*xs_bbh << endl;
      ggh_hist.Scale(br * xs_ggh);
      bbh_hist.Scale(br * xs_bbh);
    }
  }
  TH1F total_hist = setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetShape();
  hmap["Bkg"] = make_pair(total_hist, make_pair(0.,0.));
  plot.GeneratePlot(hmap);
  // ic::TH1PlotElement total_shape("total_shape", &total_hist,"bkg. uncertainty");

  // TH1F data_hist = setup.GetObservedShape();

  // if (blind) {
  //   for (int j = 0; j < data_hist.GetNbinsX(); ++j) {
  //     double low_edge = data_hist.GetBinLowEdge(j+1);
  //     double high_edge = data_hist.GetBinWidth(j+1)+data_hist.GetBinLowEdge(j+1);
  //     if ((low_edge > x_blind_min && low_edge < x_blind_max) || (high_edge > x_blind_min && high_edge < x_blind_max)) {
  //       data_hist.SetBinContent(j+1,0);
  //       data_hist.SetBinError(j+1,0);
  //     }
  //   }
  // }

  // ic::TH1PlotElement data_shape("data_shape", &data_hist, "observed");
  // SetDataStyle(data_shape);

  // std::vector<ic::TH1PlotElement *> drawn_ele;
  // drawn_ele.push_back(&qcd_shape);
  // drawn_ele.push_back(&ewk_shape);
  // drawn_ele.push_back(&zee_shape);
  // drawn_ele.push_back(&ztt_shape);
  // drawn_ele.push_back(&top_shape);
  // drawn_ele.push_back(&signal_shape);
  // drawn_ele.push_back(&data_shape);
  // drawn_ele.push_back(&total_shape);

  // for (unsigned i = 0; i < drawn_ele.size(); ++i) {
  //    drawn_ele[i]->set_line_width(2);
  //    if (norm_y_axis) drawn_ele[i]->hist_ptr()->Scale(1.0, "width");
  // }
  // total_shape.set_marker_size(0);
  // total_shape.set_fill_color(1);
  // total_shape.set_fill_style(3013);
  // total_shape.set_line_width(1);
  // total_shape.set_draw_stat_error_y(true);
  // total_shape.set_draw_fill(true);
  // total_shape.set_draw_line(false);
  // total_shape.set_draw_marker(false);
  // total_shape.set_draw_options("e2");


  // plot.AddTH1PlotElement(qcd_shape);
  // plot.AddTH1PlotElement(top_shape);
  // plot.AddTH1PlotElement(ewk_shape);
  // if (channel == "et") plot.AddTH1PlotElement(zee_shape);
  // if (channel == "mt") plot.AddTH1PlotElement(zee_shape);
  // plot.AddTH1PlotElement(ztt_shape);
  // // if (setup.process(signal_procs).GetRate() != 0.0) plot.AddTH1PlotElement(signal_shape);
  // plot.AddTH1PlotElement(total_shape);
  // plot.AddTH1PlotElement(data_shape);
  // plot.custom_x_axis_range = custom_x_axis_range;
  // if (custom_x_axis_range){
  //   plot.x_axis_min = x_axis_min;
  //   plot.x_axis_max = x_axis_max;
  // }
  // plot.custom_y_axis_range = custom_y_axis_range;
  // if (custom_y_axis_range){
  //   plot.y_axis_min = y_axis_min;
  //   plot.y_axis_max = y_axis_max;
  // }
  // plot.legend_height = 0.045;
  // plot.x_axis_title = x_axis_label;
  // plot.y_axis_title = "dN/dm_{#tau#tau} [1/GeV]";
  // plot.title_left = title_left;
  // string channel_str = "";
  // if (channel == "et") channel_str = "e#tau_{h}";
  // if (channel == "mt") channel_str = "#mu#tau_{h}";
  // if (channel == "em") channel_str = "e#mu";
  // if (channel == "tt") channel_str = "#tau_{h}#tau_{h}";
  // ic::TextElement text(channel_str,0.05,0.22,0.87);
  // ic::TextElement text2(v_columns.first,0.05,0.22,0.80);
  // plot.AddTextElement(text);
  // plot.AddTextElement(text2);
  // plot.extra_pad = 1.1;
  // if (log_y) plot.y_axis_log = true;
  // if (log_y) plot.extra_pad = 5.;

  // plot.GeneratePlot();




  return 0;
}

