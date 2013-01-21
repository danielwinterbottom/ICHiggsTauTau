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

void SetStyle(ic::TH1PlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_line(false);
  ele.set_line_width(1);
  ele.set_draw_fill(false);
  ele.set_draw_stat_error_y(true);
  ele.set_marker_style(20);
  ele.set_marker_size(1.2);
  return;
}

void SetStyleBlock(ic::TH1PlotElement & ele) {
  //ele.set_marker_color(color);
  ele.set_line_color(14);
  ele.set_fill_color(18);
  ele.set_fill_style(1001);
  ele.set_draw_fill(true);
  ele.set_draw_marker(false);
  ele.set_draw_line(false);
  ele.set_line_width(3);
  ele.set_draw_stat_error_y(false);
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

  string file1;
  string file2;
  string plot_folder;
  string plot;
  string lumi1;
  string lumi2;
  double lumi1d;
  double lumi2d;
  string label1;
  string label2;
  string x_axis_title;
  bool custom_x_axis_range;                     // Can optionally specify an x-axis range
  double x_axis_min;                            // If custom_x_axis_range is true, use this as min
  double x_axis_max;                            // If custom_x_axis_range is true, use this as max
  int norm_mode = 0; // 0 = no norm, 1 = norm2to1yield, 3=norm2to1lumi, 3=norm both to 1
  unsigned rebin;                               // Rebin factor
  string channel;

  po::options_description config("Configuration");
  po::variables_map vm;
  po::notify(vm);

  config.add_options()    
  ("file1",           po::value<string>(&file1)->required())
    ("file2",           po::value<string>(&file2)->required())
    ("plot_folder",              po::value<string>(&plot_folder)->required())
    ("plot",             po::value<string>(&plot)->required())
    ("lumi1",              po::value<string>(&lumi1)->required())
    ("lumi2",            po::value<string>(&lumi2)->required())
    ("label1",             po::value<string>(&label1)->required())
    ("label2",             po::value<string>(&label2)->required())
    ("channel",             po::value<string>(&channel)->required())
    ("norm_mode",             po::value<int>(&norm_mode)->required())
    ("x_axis_title",             po::value<string>(&x_axis_title)->required())
    ("custom_x_axis_range", po::value<bool>(&custom_x_axis_range)->default_value(false))
    ("x_axis_min",          po::value<double>(&x_axis_min)->default_value(0))
    ("x_axis_max",          po::value<double>(&x_axis_max)->default_value(0))
    ("rebin",               po::value<unsigned>(&rebin)->default_value(1))


    ;
    po::store(po::command_line_parser(argc, argv).
              options(config).allow_unregistered().run(), vm);
    po::notify(vm);

  TFile f1(file1.c_str());
  TFile f2(file2.c_str());



  ic::Plot compare;
  compare.output_filename = label1+"_"+label2+"_"+channel+"_"+plot+".pdf";

  compare.custom_x_axis_range = custom_x_axis_range;
  if (custom_x_axis_range){
    compare.x_axis_min = x_axis_min;
    compare.x_axis_max = x_axis_max;
  }
  unsigned color = 46;
  if (channel == "et") color = 2;
  if (channel == "mt") color = 4;
  if (channel == "em") color = 1;
  if (channel == "et") channel = "#tau_{e}#tau_{h}";
  if (channel == "mt") channel = "#tau_{#mu}#tau_{h}";
  if (channel == "em") channel = "#tau_{e}#tau_{#mu}";

  ic::TH1PlotElement p1(label1, &f1, plot_folder,plot, label1+" ("+lumi1 +" fb^{-1})");
  ic::TH1PlotElement p2(label2, &f2, plot_folder,plot, label2+" ("+lumi2 +" fb^{-1})");

  lumi1d = boost::lexical_cast<double>(lumi1);
  lumi2d = boost::lexical_cast<double>(lumi2);

  p1.hist_ptr()->Rebin(rebin);
  p2.hist_ptr()->Rebin(rebin);

  if (norm_mode == 1) {
    std::cout << "Normalise yield of 2 to 1" << std::endl;
    double yield1 = p1.hist_ptr()->Integral();
    p2.hist_ptr()->Scale(yield1/p2.hist_ptr()->Integral());
  }
  if (norm_mode == 2) {
    std::cout << "Normalise lumi of 2 to 1" << std::endl;
    p2.hist_ptr()->Scale(lumi1d/lumi2d);
    compare.title_right = label2 + " normalised to "+ label1 +" luminosity";
    compare.title_left = "CMS Preliminary 2012";
    // compare.AddTextElement(text);
  }
  if (norm_mode == 3) {
    std::cout << "Normalise both to unity" << std::endl;
    p1.hist_ptr()->Scale(1/p1.hist_ptr()->Integral());
    p2.hist_ptr()->Scale(1/p2.hist_ptr()->Integral());
  }
  ic::RatioPlotElement ratio("Ratio",label2,label1);

  SetStyleBlock(p1);
  SetStyle(p2, color);
  SetStyle(ratio, color);

  compare.AddTH1PlotElement(p1);
  compare.AddTH1PlotElement(p2);

  compare.custom_ratio_y_axis_range = true;
  compare.ratio_y_axis_max = 1.5;
  compare.ratio_y_axis_min = 0.5;
  compare.x_axis_title = x_axis_title;
  compare.y_axis_title = "Events";

  compare.draw_ratio_hist = true;
  compare.AddRatioPlotElement(ratio);
  compare.ratio_y_axis_title = label2+"/"+label1;
  compare.extra_pad = 1.15;


  ic::TextElement text(channel,0.08,0.19,0.89);
  compare.AddTextElement(text);

  compare.GeneratePlot();





  return 0;
}

