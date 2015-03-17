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
#include "TH1.h"
#include "TFile.h"

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

void SetStyle(ic::TH1PlotElement & ele, unsigned color, unsigned marker) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_line(false);
  ele.set_line_width(2);
  ele.set_draw_fill(false);
  ele.set_draw_stat_error_y(true);
  ele.set_marker_style(marker);
  ele.set_marker_size(1.0);
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

void SetStyle(ic::RatioPlotElement & ele, unsigned color, unsigned marker) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  ele.set_marker_style(marker);
  return;
}


int main(int argc, char* argv[]){


  vector<string> plots;

  string outname;
  string title;
  string x_axis_title;
  bool custom_x_axis_range;                     // Can optionally specify an x-axis range
  double x_axis_min;                            // If custom_x_axis_range is true, use this as min
  double x_axis_max;                            // If custom_x_axis_range is true, use this as max
  int norm_mode = 0; // 0 = no norm, 1 = norm2to1yield, 3=norm2to1lumi, 3=norm all to 1
  unsigned rebin;                               // Rebin factor
  string channel;
  string ratios;
  double ratio_y_min;
  double ratio_y_max;
  string ratio_axis_label;
  bool log_y;
  bool norm_bins;
  string title_left;
  string title_right;
  bool custom_y_axis_min;
  double y_axis_min;
  double extra_pad;

  po::options_description config("Configuration");
  po::variables_map vm;
  po::notify(vm);

  config.add_options()
    ("plot,p",              po::value<vector<string>>(&plots)->required())
    ("outname",             po::value<string>(&outname)->required())
    ("ratios",              po::value<string>(&ratios)->default_value(""))
    ("big_label",           po::value<string>(&channel)->required())
    ("norm_mode",           po::value<int>(&norm_mode)->required())
    ("x_axis_title",        po::value<string>(&x_axis_title)->default_value(""))
    ("custom_x_axis_range", po::value<bool>(&custom_x_axis_range)->default_value(false))
    ("log_y",               po::value<bool>(&log_y)->default_value(false))
    ("x_axis_min",          po::value<double>(&x_axis_min)->default_value(0))
    ("x_axis_max",          po::value<double>(&x_axis_max)->default_value(0))
    ("rebin",               po::value<unsigned>(&rebin)->default_value(1))
    ("ratio_axis_label",    po::value<string>(&ratio_axis_label)->default_value("Ratio"))
    ("title_left",          po::value<string>(&title_left)->default_value(""))
    ("title_right",         po::value<string>(&title_right)->default_value(""))
    ("ratio_y_min",         po::value<double>(&ratio_y_min)->default_value(0.8))
    ("ratio_y_max",         po::value<double>(&ratio_y_max)->default_value(1.2))
    ("norm_bins",           po::value<bool>(&norm_bins)->default_value(false))
    ("custom_y_axis_min",   po::value<bool>(&custom_y_axis_min)->default_value(false))
    ("y_axis_min",          po::value<double>(&y_axis_min)->default_value(0.0))
    ("extra_pad",           po::value<double>(&extra_pad)->default_value(1.15))
    ;
    po::store(po::command_line_parser(argc, argv).
              options(config).allow_unregistered().run(), vm);
    po::notify(vm);


  vector<TH1PlotElement> elements;
  vector<double> lumi;
  vector<double> rate;
  vector<TFile *> files;

    string param_fmt = "%-25s %-40s\n";

  std::vector<unsigned> markers = {20, 21, 22, 23, 24, 25, 26};
  auto marker_it = markers.begin();
  for (auto p : plots) {
    if (marker_it == markers.end()) marker_it = markers.begin();
    // title(0):label(1):file(2):folder(3):plot(4):lumi(5):style(6):colour(7)
    // e.g. embedded:PF Embedded:Embedded_mt_2012.root:inclusive_os_sel:-1.0:0:4
    vector<string> split;
    boost::split(split, p, boost::is_any_of(":"));
    if (split.size() != 8) {
      cout << "Plot descriptor << " << p << " not recognised..." << endl;
    }
    std::cout << "----PLOT----" << std::endl;
    std::cout << boost::format(param_fmt) % "title" % split[0];
    std::cout << boost::format(param_fmt) % "legend" % split[1];
    std::cout << boost::format(param_fmt) % "file" % split[2];
    std::cout << boost::format(param_fmt) % "folder" % split[3];
    std::cout << boost::format(param_fmt) % "plot" % split[4];
    std::cout << boost::format(param_fmt) % "lumi" % split[5];
    std::cout << boost::format(param_fmt) % "style" % split[6];
    std::cout << boost::format(param_fmt) % "color" % split[7];

    files.push_back(new TFile(split[2].c_str()));
    elements.emplace_back(split[0], files.back(), split[3], split[4], split[1]);
    elements.back().hist_ptr()->Rebin(rebin);
    lumi.emplace_back(boost::lexical_cast<double>(split[5]));
    std::cout << boost::format(param_fmt) % "lumi" % lumi.back();
    rate.emplace_back(Integral(elements.back().hist_ptr()));
    std::cout << boost::format(param_fmt) % "rate" % rate.back();

    if (norm_mode == 3) {
      elements.back().hist_ptr()->Scale(1. / rate.back());
    }
    if (norm_bins) elements.back().hist_ptr()->Scale(1.0, "width");

    if (elements.size() > 1) {
      if (norm_mode == 1) {
        elements.back().hist_ptr()->Scale(rate[0] / rate.back());
      }
      if (norm_mode == 2) {
        elements.back().hist_ptr()->Scale(lumi[0] / lumi.back());
      }
    }
    int col = boost::lexical_cast<int>(split[7]);
    if (split[6] == "0") {
      SetStyle(elements.back(), col, *marker_it);
      ++marker_it;
    } else if (split[6] == "1") {
      SetStyleBlock(elements.back());
    } else if (split[6] == "2") {
      SetStyle(elements.back(), col, *marker_it);
      elements.back().set_draw_line(true);
      elements.back().set_line_color(col);
      elements.back().set_draw_fill(true);

    } else if (split[6] == "3") {
      SetStyle(elements.back(), col, 0);
      elements.back().set_draw_line(true);
      elements.back().set_line_color(col);
      elements.back().set_draw_fill(true);
    }
  
  }
  
  marker_it = markers.begin();
  vector<string> rsplit;
  boost::split(rsplit, ratios, boost::is_any_of(":"));
  vector<RatioPlotElement> relements;
  for (auto r : rsplit) {
    if (marker_it == markers.end()) marker_it = markers.begin();
    vector<string> tmp;
    boost::split(tmp, r, boost::is_any_of("/"));
    if (tmp.size() != 3) continue;
    relements.emplace_back(r, tmp[0], tmp[1]);
    SetStyle(relements.back(), boost::lexical_cast<double>(tmp[2]), 20);
    ++marker_it;
  }

  ic::Plot compare;
  compare.title_left = title_left;
  compare.title_right = title_right;
  compare.output_filename = outname;
  compare.custom_x_axis_range = custom_x_axis_range;
  if (custom_x_axis_range){
    compare.x_axis_min = x_axis_min;
    compare.x_axis_max = x_axis_max;
  }
  compare.extra_pad = extra_pad;

  std::for_each(elements.begin(),elements.end(), [&] (TH1PlotElement & x) {compare.AddTH1PlotElement(x);});
  std::for_each(relements.begin(),relements.end(), [&] (RatioPlotElement & x) {compare.AddRatioPlotElement(x);});


  // if (channel == "et") channel = "#tau_{e}#tau_{h}";
  // if (channel == "mt") channel = "#mu#tau_{h}";
  // if (channel == "em") channel = "#tau_{e}#tau_{#mu}";

  compare.custom_ratio_y_axis_range = true;
  compare.ratio_y_axis_max = 1.2;
  compare.ratio_y_axis_min = 0.8;
  compare.x_axis_title = x_axis_title;
  compare.y_axis_title = "Events";

  compare.draw_ratio_hist = relements.size() > 0;
  compare.ratio_y_axis_title = ratio_axis_label;
  compare.ratio_y_axis_min = ratio_y_min;
  compare.ratio_y_axis_max = ratio_y_max;

  compare.y_axis_log = log_y;

  if (custom_y_axis_min) {
    compare.y_axis_min = y_axis_min;
  }


  ic::TextElement text(channel,0.07,0.19,0.89);
  compare.AddTextElement(text);


  compare.GeneratePlot();


  return 0;
}

